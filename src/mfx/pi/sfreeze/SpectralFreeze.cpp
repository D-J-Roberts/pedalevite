/*****************************************************************************

        SpectralFreeze.cpp
        Author: Laurent de Soras, 2020

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://www.wtfpl.net/ for more details.

*Tab=3***********************************************************************/



#if defined (_MSC_VER)
	#pragma warning (1 : 4130 4223 4705 4706)
	#pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/Approx.h"
#include "fstb/def.h"
#include "fstb/fnc.h"
#include "mfx/dsp/mix/Align.h"
#include "mfx/dsp/mix/Generic.h"
#include "mfx/pi/sfreeze/Param.h"
#include "mfx/pi/sfreeze/SpectralFreeze.h"
#include "mfx/piapi/Dir.h"
#include "mfx/piapi/Err.h"
#include "mfx/piapi/EventParam.h"
#include "mfx/piapi/EventTs.h"
#include "mfx/piapi/EventType.h"
#include "mfx/piapi/ProcInfo.h"

#include <algorithm>
#include <complex>

#include <cassert>
#include <cmath>



namespace mfx
{
namespace pi
{
namespace sfreeze
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



SpectralFreeze::SpectralFreeze ()
{
	dsp::mix::Align::setup ();
	dsp::mix::Generic::setup ();

	const ParamDescSet & desc_set = _desc.use_desc_set ();
	_state_set.init (piapi::ParamCateg_GLOBAL, desc_set);

	_state_set.set_val_nat (desc_set, Param_FREEZE, 0);

	_state_set.add_observer (Param_FREEZE, _param_change_flag_misc);

	_param_change_flag_misc.add_observer (_param_change_flag);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



piapi::PluginInterface::State	SpectralFreeze::do_get_state () const
{
	return _state;
}



double	SpectralFreeze::do_get_param_val (piapi::ParamCateg categ, int index, int note_id) const
{
	fstb::unused (categ, note_id);
	assert (categ == piapi::ParamCateg_GLOBAL);

	return _state_set.use_state (index).get_val_tgt ();
}



int	SpectralFreeze::do_reset (double sample_freq, int max_buf_len, int &latency)
{
	/*** To do:
		Resampling to 44.1 or 48 kHz when the sampling rate is higher, so the
		sound remains more or less the same whatever the sampling rate.
	***/

	latency = 0;

	_sample_freq = float (    sample_freq);
	_inv_fs      = float (1 / sample_freq);

	_state_set.set_sample_freq (sample_freq);
	_state_set.clear_buffers ();

	for (int chn_idx = 0; chn_idx < _max_nbr_chn; ++chn_idx)
	{
		auto &         chn = _chn_arr [chn_idx];

		int            ofs_com = 0;
		int            ofs_syn = 0;

		// Because we use small processing block size, the calculation load
		// is unevenly spread across the blocks. With multiple channels, we
		// can make the load more homogenous by interleaving the FFT frames
		// of each channel.
		// We have no real latency issue for a freeze, so we can interleave
		// analysis and synthesis too.
		// Ideally we should use bit reversal if _max_nbr_chn is high.
		if (max_buf_len < _hop_size)
		{
			ofs_com = _hop_size * chn_idx / _max_nbr_chn;
			ofs_syn = max_buf_len;
		}

		chn._fo_ana.setup (_fft_len, _hop_size, ofs_com);
		chn._fo_syn.setup (_fft_len, _hop_size, ofs_com + ofs_syn);
	}

	_frame_win.setup (_fft_len);

	_param_change_flag_misc.set ();

	update_param (true);
	_param_proc.req_steady ();

	clear_buffers ();

	_state = State_ACTIVE;

	return piapi::Err_OK;
}



void	SpectralFreeze::do_process_block (piapi::ProcInfo &proc)
{
	const int      nbr_chn_src = proc._dir_arr [piapi::Dir_IN ]._nbr_chn;
	const int      nbr_chn_dst = proc._dir_arr [piapi::Dir_OUT]._nbr_chn;
	assert (nbr_chn_src <= nbr_chn_dst);
	const int      nbr_chn_proc = std::min (nbr_chn_src, nbr_chn_dst);

	// Events
	_param_proc.handle_msg (proc);

	// Parameters
	_state_set.process_block (proc._nbr_spl);
	update_param ();
	if (_param_proc.is_full_reset ())
	{
		clear_buffers ();
	}

	// Signal processing
	const int      nbr_spl = proc._nbr_spl;
	for (int chn_index = 0; chn_index < nbr_chn_proc; ++chn_index)
	{
		Channel &      chn = _chn_arr [chn_index];

		int            pos = 0;
		do
		{
			const int      rem_len_a = chn._fo_ana.get_len_before_next_frame ();
			const int      rem_len_s = chn._fo_syn.get_len_before_next_frame ();
			const int      rem_len   = std::min (rem_len_a, rem_len_s);
			const int      work_len  = std::min (rem_len, nbr_spl - pos);

			chn._fo_ana.process_block (proc._src_arr [chn_index] + pos, work_len);
			chn._fo_syn.process_block (proc._dst_arr [chn_index] + pos, work_len);
			dsp::mix::Generic::mix_1_1 (
				proc._dst_arr [chn_index] + pos,
				proc._src_arr [chn_index] + pos,
				work_len
			);

			const bool     req_ana_flag = chn._fo_ana.is_frame_proc_required ();
			const bool     req_syn_flag = chn._fo_syn.is_frame_proc_required ();

			if (req_ana_flag)
			{
				// Input frame extraction and windowing
				chn._fo_ana.get_frame (_buf_pcm.data ());
				_frame_win.process_frame_mul (_buf_pcm.data ());

				// PCM -> frequency bins
				_fft.do_fft (_buf_bins.data (), _buf_pcm.data ());

				// Analysis
				analyse_bins (chn);
			}

			if (req_syn_flag)
			{
				// Synthesis
				sythesise_bins (chn);

				// Frequency bins -> PCM
				_fft.do_ifft (_buf_bins.data (), _buf_pcm.data ());

				// Output frame windowing and overlap
				_frame_win.process_frame_mul (_buf_pcm.data ());
				chn._fo_syn.set_frame (_buf_pcm.data ());
			}

			pos += work_len;
		}
		while (pos < nbr_spl);
	}

	// Duplicates the remaining output channels
	for (int chn_index = nbr_chn_proc; chn_index < nbr_chn_dst; ++chn_index)
	{
		dsp::mix::Align::copy_1_1 (
			proc._dst_arr [chn_index],
			proc._dst_arr [0],
			proc._nbr_spl
		);
	}
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void	SpectralFreeze::clear_buffers ()
{
	for (auto &chn : _chn_arr)
	{
		chn._fo_ana.clear_buffers ();
		chn._fo_syn.clear_buffers ();
	}
}



void	SpectralFreeze::update_param (bool force_flag)
{
	if (_param_change_flag (true) || force_flag)
	{
		// Misc param
		if (_param_change_flag_misc (true) || force_flag)
		{
			const bool     frz_flag = _state_set.get_val_bool (Param_FREEZE);
			for (auto &chn : _chn_arr)
			{
				if (! frz_flag)
				{
					chn._frz_state = FreezeState::NONE;
				}
				else if (chn._frz_state == FreezeState::NONE)
				{
					chn._frz_state = FreezeState::CAPTURE1;
				}
			}
		}
	}
}



int	SpectralFreeze::conv_freq_to_bin (float f) const noexcept
{
	assert (f >= 0);
	assert (f < _sample_freq * 0.5f);

	return fstb::round_int (f * _inv_fs * _fft_len);
}



void	SpectralFreeze::analyse_bins (Channel &chn) noexcept
{
	if (chn._frz_state == FreezeState::CAPTURE1)
	{
		// Normalizes each bin, so we keep only its phase information
		for (int bin_idx = _bin_beg; bin_idx < _nbr_bins; ++bin_idx)
		{
			std::complex <float> bin {
				_buf_bins [bin_idx            ],
				_buf_bins [bin_idx + _nbr_bins]
			};

			/*** To do: optimise this with a fast inverse square root ***/
			const float    mag = std::abs (bin);
			if (fstb::is_null (mag))
			{
				bin = std::complex <float> { 1, 0 };
			}
			else
			{
				const float    inv_mag = (mag > 0) ? 1.f / mag : 0;
				bin *= inv_mag;
			}

			chn._buf_freeze [bin_idx            ] = bin.real ();
			chn._buf_freeze [bin_idx + _nbr_bins] = bin.imag ();
		}

		chn._frz_state = FreezeState::CAPTURE2;
	}

	else if (chn._frz_state == FreezeState::CAPTURE2)
	{
		// For each bin, evaluates the phase difference between this frame and
		// the previous one.
		for (int bin_idx = _bin_beg; bin_idx < _nbr_bins; ++bin_idx)
		{
			std::complex <float> bin {
				_buf_bins [bin_idx            ],
				_buf_bins [bin_idx + _nbr_bins]
			};
			std::complex <float> bin_old {
				chn._buf_freeze [bin_idx            ],
				chn._buf_freeze [bin_idx + _nbr_bins]
			};

			// Phase difference
			float          arg_n = 0;
			const float    b0r   = bin_old.real ();
			const float    b0i   = bin_old.imag ();
			if (! fstb::is_null (fabsf (b0r) + fabsf (b0i)))
			{
				// Division formula without scaling: bin / bin_old = b1 / b0
				const float    b1r   = bin.real ();
				const float    b1i   = bin.imag ();
				const float    dr    = b1r * b0r + b1i * b0i;
				const float    di    = b1i * b0r - b1r * b0i;

				// Extracts the angle
				/*** To do: use a faster approximation ***/
				const float    angle = atan2f (di, dr);

				// Scales the resulting phase from [-pi ; +pi] to [-1/2; +1/2]
				arg_n = angle * float (0.5 / fstb::PI);
			}

			chn._buf_freeze [bin_idx            ] = std::abs (bin);
			chn._buf_freeze [bin_idx + _nbr_bins] = arg_n;
		}

		chn._frz_state = FreezeState::REPLAY;
		chn._nbr_hops  = 1;
	}
}



void	SpectralFreeze::sythesise_bins (Channel &chn) noexcept
{
	clear_bins ();

	if (chn._frz_state == FreezeState::REPLAY)
	{

		++ chn._nbr_hops;

		// FFT normalisation factor combined with window scaling
		constexpr int     hop_ratio = 1 << (_fft_len_l2 - _hop_size_l2);
		constexpr float   scale_win = (hop_ratio <= 2) ? 1 : 8.f / (3 * hop_ratio);
		constexpr float   scale_fft = 1.f / _fft_len;
		constexpr float   scale     = scale_fft * scale_win;

		for (int bin_idx = _bin_beg; bin_idx < _nbr_bins; ++bin_idx)
		{
			const float    mag   = chn._buf_freeze [bin_idx            ];
			float          arg_n = chn._buf_freeze [bin_idx + _nbr_bins];

			// Reports the phase difference between the two initial frames
			arg_n *= chn._nbr_hops;

			// Why 0.5? Got this by trial & error. Maths behind this are clear
			// like mud
			const float    omega = 0.5f * bin_idx;
			arg_n += omega;

			// Keeps [-1/2; +1/2] range
			arg_n -= float (fstb::round_int (arg_n));

			const float    mag_scale = mag * scale;
			const auto     cs = fstb::Approx::cos_sin_nick_2pi (arg_n);
			const float    br = cs [0] * mag_scale;
			const float    bi = cs [1] * mag_scale;

			_buf_bins [bin_idx            ] = br;
			_buf_bins [bin_idx + _nbr_bins] = bi;
		}
	}
}



void	SpectralFreeze::clear_bins () noexcept
{
	_buf_bins.fill (0.f);
}



}  // namespace sfreeze
}  // namespace pi
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/