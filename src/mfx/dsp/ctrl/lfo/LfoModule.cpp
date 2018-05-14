/*****************************************************************************

        LfoModule.cpp
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if defined (_MSC_VER)
	#pragma warning (1 : 4130 4223 4705 4706)
	#pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/Approx.h"
#include "fstb/fnc.h"
#include "mfx/dsp/ctrl/lfo/LfoModule.h"
#include "mfx/dsp/ctrl/lfo/OscNoiseFlt.h"
#include "mfx/dsp/ctrl/lfo/OscNPhase.h"
#include "mfx/dsp/ctrl/lfo/OscParabola.h"
#include "mfx/dsp/ctrl/lfo/OscSaw.h"
#include "mfx/dsp/ctrl/lfo/OscSine.h"
#include "mfx/dsp/ctrl/lfo/OscSquare.h"
#include "mfx/dsp/ctrl/lfo/OscStepSeq.h"
#include "mfx/dsp/ctrl/lfo/OscTri.h"
#include "mfx/dsp/ctrl/lfo/OscVariSlope.h"

#include <new>

#include <cassert>
#include <cmath>
#include <cstdint>



namespace mfx
{
namespace dsp
{
namespace ctrl
{
namespace lfo
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



LfoModule::LfoModule ()
:	_osc ()
,	_sample_freq (-1)
,	_period (1)
,	_phase_chaos (0)
,	_phase_dist (0.5)
,	_phase_dist_ofs (0)
,	_inv_flag (false)
,	_unipolar_flag (false)
,	_variation_arr ()
,	_snh_ratio (0)
,	_smooth (0)
,	_type (Type_INVALID)
,	_step_seq_uptr ()
,	_osc_ptr (get_aligned_osc ())
,	_snh_flag (false)
,	_smooth_flag (false)
,	_snh_pos (0)
,	_snh_step (0)
,	_snh_state (0)
,	_smooth_state (0)
,	_smooth_spl_coef (0)
{
	for (auto & var : _variation_arr)
	{
		var = 0;
	}

	::new (get_aligned_osc ()) OscSine;
	_type = Type_SINE;

	set_sample_freq (44100);

	apply_osc_settings ();
	set_snh (_snh_ratio);
	set_smooth (_smooth);
	set_phase (0);
}



LfoModule::~LfoModule ()
{
	if (_type >= 0)
	{
		use_osc ().~OscInterface ();
	}
}



void	LfoModule::set_sample_freq (double sample_freq)
{
	assert (sample_freq > 0);

	_sample_freq = sample_freq;

	use_osc ().set_sample_freq (sample_freq);
	update_snh ();
	update_smooth ();
}



void	LfoModule::set_period (double per)
{
	assert (per > 0);

	_period = per;
	use_osc ().set_period (per);
	update_smooth ();
	update_snh ();
}



void	LfoModule::set_phase (double phase)
{
	assert (phase >= 0);
	assert (phase < 1);

	use_osc ().set_phase (phase);
	_snh_pos = phase;
	_snh_state = use_osc ().get_val ();
}



void	LfoModule::set_chaos (double chaos)
{
	assert (chaos >= 0);
	assert (chaos <= 1);

	use_osc ().set_chaos (chaos);
	_phase_chaos = chaos;
}



void	LfoModule::set_phase_dist (double dist)
{
	assert (dist >= 0);
	assert (dist <= 1);

	use_osc ().set_phase_dist (dist);
	_phase_dist = dist;
}



void	LfoModule::set_phase_dist_offset (double ofs)
{
	assert (ofs >= 0);
	assert (ofs <= 1);

	use_osc ().set_phase_dist_offset (ofs);
	_phase_dist_ofs = ofs;
}



void	LfoModule::set_sign (bool inv_flag)
{
	use_osc ().set_sign (inv_flag);
	_inv_flag = inv_flag;
}



void	LfoModule::set_polarity (bool unipolar_flag)
{
	use_osc ().set_polarity (unipolar_flag);
	_unipolar_flag = unipolar_flag;
}



void	LfoModule::set_variation (int param, double val)
{
	assert (param >= 0);
	assert (param < OscInterface::Variation_NBR_ELT);
	assert (val >= 0);
	assert (val <= 1);

	_variation_arr [param] = val;

	use_osc ().set_variation (param, val);
}



bool	LfoModule::is_using_variation (int param) const
{
	assert (param >= 0);
	assert (param < OscInterface::Variation_NBR_ELT);

	return use_osc ().is_using_variation (param);
}



void	LfoModule::set_type (Type type)
{
	assert (type >= 0);
	assert (type < Type_NBR_ELT);

	if (type != _type)
	{
		// Remove previous instance
		const double   phase = get_phase ();
		if (_type != Type_STEP_SEQ)
		{
			use_osc ().~OscInterface ();
		}
		_type = Type_INVALID;

#define mfx_dsp_ctrl_lfo_LfoModule_BUILD(c, T, a) \
		case	c: \
			static_assert (sizeof (T) <= sizeof (_osc) - _storage_alig + 1, "Storage size"); \
			::new (get_aligned_osc ()) T a; \
			break;

		// Build new oscillator
		switch (type)
		{
		mfx_dsp_ctrl_lfo_LfoModule_BUILD (Type_SINE      , OscSine        ,       )
		mfx_dsp_ctrl_lfo_LfoModule_BUILD (Type_TRIANGLE  , OscTri         ,       )
		mfx_dsp_ctrl_lfo_LfoModule_BUILD (Type_SQUARE    , OscSquare      ,       )
		mfx_dsp_ctrl_lfo_LfoModule_BUILD (Type_SAW       , OscSaw         ,       )
		mfx_dsp_ctrl_lfo_LfoModule_BUILD (Type_PARABOLA  , OscParabola    ,       )
		mfx_dsp_ctrl_lfo_LfoModule_BUILD (Type_BIPHASE   , OscNPhase      , (true))
		mfx_dsp_ctrl_lfo_LfoModule_BUILD (Type_N_PHASE   , OscNPhase      ,       )
		mfx_dsp_ctrl_lfo_LfoModule_BUILD (Type_VARISLOPE , OscVariSlope   ,       )
		mfx_dsp_ctrl_lfo_LfoModule_BUILD (Type_NOISE_FLT1, OscNoiseFlt <1>,       )
		mfx_dsp_ctrl_lfo_LfoModule_BUILD (Type_NOISE_FLT2, OscNoiseFlt <2>,       )
		case Type_STEP_SEQ:
			assert (has_step_seq ());
			break;
		default:
			assert (false);
			break;
		}
#undef mfx_dsp_ctrl_lfo_LfoModule_BUILD

		// Construction successful
		_type = type;
		if (type == Type_STEP_SEQ)
		{
			_osc_ptr = _step_seq_uptr.get ();
		}
		else
		{
			_osc_ptr = get_aligned_osc ();
		}
		use_osc ().set_sample_freq (_sample_freq);
		apply_osc_settings ();
		set_phase (phase);
	}
}



void	LfoModule::set_snh (double ratio)
{
	assert (ratio >= 0);

	_snh_ratio = ratio;
	update_snh ();
}



void	LfoModule::set_smooth (double ratio)
{
	assert (ratio >= 0);

	_smooth = ratio;
	update_smooth ();
}



void	LfoModule::set_step_seq (bool flag)
{
	if (! flag)
	{
		_step_seq_uptr.reset ();
	}
	else if (_step_seq_uptr.get () == 0)
	{
		_step_seq_uptr = StepSeqUPtr (new OscStepSeq);
	}
}



bool	LfoModule::has_step_seq () const
{
	return (_step_seq_uptr.get () != 0);
}



OscStepSeq &	LfoModule::use_step_seq ()
{
	assert (has_step_seq ());

	return *_step_seq_uptr;
}



void	LfoModule::tick (int nbr_spl)
{
	int            work_len = nbr_spl;
	if (_snh_flag)
	{
		// Compute the S&H position at the end of this block
		const double   end_snh_pos = _snh_pos + _snh_step * nbr_spl;
		const int      end_snh_pos_int = fstb::floor_int (end_snh_pos);

		// If it is triggered at least once during the block
		if (end_snh_pos_int > 0)
		{
			const int      spl_before_last_snh =
				fstb::ceil_int ((end_snh_pos_int - _snh_pos) / _snh_step);
			work_len = spl_before_last_snh;
		}
	}
	tick_sub (work_len);

	const int      rem_len = nbr_spl - work_len;
	if (rem_len > 0)
	{
		tick_sub (rem_len);
	}
}



double	LfoModule::get_val () const
{
	// Source: smooth, S&H or direct
	double         val;
	if (_smooth_flag)
	{
		val = _smooth_state;
	}
	else if (_snh_flag)
	{
		val = _snh_state;
	}
	else
	{
		val = use_osc ().get_val ();
	}

	return val;
}



double	LfoModule::get_phase () const
{
	return use_osc ().get_phase ();
}



void	LfoModule::clear_buffers ()
{
	use_osc ().clear_buffers ();
	const double   raw_val = use_osc ().get_val ();
	_snh_state    = raw_val;
	_smooth_state = raw_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



OscInterface *	LfoModule::get_aligned_osc () const
{
	const intptr_t   base = reinterpret_cast <intptr_t> (&_osc [0]);
	const intptr_t   alig = (base + _storage_alig - 1) & ~(_storage_alig - 1);

	return reinterpret_cast <OscInterface *> (alig);
}



const OscInterface &	LfoModule::use_osc () const
{
	assert (_type >= 0);

	return *_osc_ptr;
}



OscInterface &	LfoModule::use_osc ()
{
	assert (_type >= 0);

	return *_osc_ptr;
}



void	LfoModule::apply_osc_settings ()
{
	set_period (_period);
	set_chaos (_phase_chaos);
	set_phase_dist (_phase_dist);
	set_phase_dist_offset (_phase_dist_ofs);
	set_sign (_inv_flag);
	set_polarity (_unipolar_flag);
	for (int var = 0; var < OscInterface::Variation_NBR_ELT; ++var)
	{
		set_variation (var, _variation_arr [var]);
	}
}



void	LfoModule::update_snh ()
{
	if (_snh_ratio <= 1e-6)
	{
		_snh_step = 0;
		_snh_flag = false;
	}

	else
	{
		// Turn it on : initialize the state
		if (! _snh_flag)
		{
			_snh_state = use_osc ().get_val ();
		}

		_snh_step = 1.0 / (_period * _snh_ratio * _sample_freq);
		_snh_flag = true;
	}
}



void	LfoModule::update_smooth ()
{
	if (_smooth <= 0)
	{
		_smooth_spl_coef = 0;
		_smooth_flag     = false;
	}

	else
	{
		// Turn it on : initialize the state
		if (! _smooth_flag)
		{
			_smooth_state = _snh_flag ? _snh_state : use_osc ().get_val ();
		}

		// exp (-4) ~= 0.018, this means we reach 98 % of the final amplitude
		// after t_spl.
		const float    t_spl = float (_period * _smooth * _sample_freq);
//		_smooth_spl_coef = exp (-4.0f / t_spl);
      // +0.1f to avoid too low numbers at the input of fast_exp2
		_smooth_spl_coef = fstb::Approx::exp2 (-5.75f / (t_spl + 0.1f));
		_smooth_flag     = true;
	}
}



void	LfoModule::tick_sub (long nbr_spl)
{
	assert (nbr_spl > 0);

	use_osc ().tick (nbr_spl);

	// We process smoothing before S&H because any S&H event is supposed
	// to occur at the very end of the block. Therefore we operate with
	// the previous block value. For non-S&H source timing may be erroneous
	// but we can't do much better without sample per sample processing.
	if (_smooth_flag)
	{
		const double   src = _snh_flag ? _snh_state : use_osc ().get_val ();
		const double   k   = fstb::ipowp (_smooth_spl_coef, nbr_spl);
		const double   a   = 1 - k;
		_smooth_state += a * (src - _smooth_state);
	}

	if (_snh_flag)
	{
		_snh_pos += _snh_step * nbr_spl;
		if (_snh_pos >= 1)
		{
			_snh_pos   -= fstb::floor_int (_snh_pos);
			_snh_state  = use_osc ().get_val ();
		}
	}
}



}  // namespace lfo
}  // namespace ctrl
}  // namespace dsp
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
