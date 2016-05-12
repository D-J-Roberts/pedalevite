/*****************************************************************************

        FreqAnalyser.cpp
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

#include "mfx/pi/FreqAnalyser.h"
#if defined (mfx_pi_USE_SIMD)
#include "fstb/ToolsSimd.h"
#endif

#include <algorithm>

#include <cassert>
#include <cmath>



namespace mfx
{
namespace pi
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void	FreqAnalyser::set_sample_freq (double sample_freq)
{
	assert (sample_freq > 0);

	_sample_freq           = float (sample_freq);
	_min_delta             = int (sample_freq / _max_freq);
	_win_len               = int (sample_freq / _min_freq) + 1;
#if defined (mfx_pi_USE_SIMD)
	_win_len = (_win_len + 3) & -4;
#endif
	size_t         buf_len = 1 << int (ceil (log2 (_win_len * 3)));
#if defined (mfx_pi_USE_SIMD)
	assert ((buf_len & 3) == 0);
	for (BufAlign &buf : _buf_arr)
	{
		buf.resize (buf_len, 0);
	}
#else
	_buffer.resize (buf_len, 0);
#endif
	_buf_mask              = int (buf_len) - 1;
	_cmndf_arr.resize (_win_len, 0);
	_step_size             = _win_len;
	_delta                 = 1;
	_freq                  = 0;
	_freq_prev             = 0;
	_dif_sum               = 0;
}



float	FreqAnalyser::process_block (const float spl_ptr [], int nbr_spl)
{
	assert (_sample_freq > 0);
	assert (spl_ptr != 0);
	assert (nbr_spl >= 0);

	for (int pos = 0; pos < nbr_spl; ++pos)
	{
		process_sample (spl_ptr [pos]);
	}

	return _freq_prev;
}



float	FreqAnalyser::process_sample (float x)
{
	int            write_pos = _buf_pos + _win_len + _delta - 1;
#if defined (mfx_pi_USE_SIMD)
	for (int buf_index = 0; buf_index < 4; ++buf_index)
	{
		_buf_arr [buf_index] [(write_pos - buf_index) & _buf_mask] = x;
	}
#else
	_buffer [write_pos & _buf_mask] = x;
#endif

	analyse_sample ();

	++ _delta;
	if (_delta > _win_len)
	{
		_delta   = 1;
		_buf_pos = (_buf_pos + _step_size) & _buf_mask;
	}

	return _freq_prev;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// Alain de Chevigne, Hideki Kawahara
// YIN, a fundamental frequency estimator for speech and music,
// Acoustical Society of America, 2002
// Implemented up to step 5.
void	FreqAnalyser::analyse_sample ()
{
	if (_delta <= 1)
	{
		_freq          = 0;
		_dif_sum       = 0;
		_cmndf_arr [0] = 1;
	}

	else if (_freq <= 0)
	{
		float          sum = 0;

#if defined (mfx_pi_USE_SIMD)

		auto           sum_v     = fstb::ToolsSimd::set_f32_zero ();
		const int      p1_beg    = _buf_pos;
		const int      p2_beg    = _buf_pos + _delta;
		const int      p1_buf    = p1_beg & 3;
		const int      p2_buf    = p2_beg & 3;
		const int      p1_beg_v  = p1_beg & -4;
		const int      p2_beg_v  = p2_beg & -4;
		const float *  buf_1_ptr = &_buf_arr [p1_buf] [0];
		const float *  buf_2_ptr = &_buf_arr [p2_buf] [0];
		for (int i = 0; i < _win_len; i += 4)
		{
			const auto    v1 = fstb::ToolsSimd::load_f32 (
				&buf_1_ptr [(p1_beg_v + i) & _buf_mask]);
			const auto    v2 = fstb::ToolsSimd::load_f32 (
				&buf_2_ptr [(p2_beg_v + i) & _buf_mask]);
			const auto    dif1 = v1 - v2;
			fstb::ToolsSimd::mac (sum_v, dif1, dif1);
		}

		sum = fstb::ToolsSimd::sum_h_flt (sum_v);

#else

		for (int pos = 0; pos < _win_len; ++pos)
		{
			const int      p1   = _buf_pos + pos;
			const int      p2   = p1 + _delta;
			const float    dif1 =
				_buffer [p1 & _buf_mask] - _buffer [p2 & _buf_mask];
			sum += dif1 * dif1;
		}

#endif

		_dif_sum += sum;

		_cmndf_arr [_delta] = float (sum * _delta / _dif_sum);

		if (_delta >= _min_delta)
		{
			const float    r1 = _cmndf_arr [_delta - 2];
			const float    r2 = _cmndf_arr [_delta - 1];
			const float    r3 = _cmndf_arr [_delta    ];

			if (r2 < _threshold && r3 > r2 && r1 >= r2)
			{
				// The peak is located between r1 and r3
				// Parabolic interpolation
				const float    frac = (r1 - r3) * 0.5f / (r1 + r3 - 2 * r2);
				assert (frac >= -1);
				assert (frac <= 1);
				_freq = _sample_freq / (_delta - 1 + frac);

				// Smoothing
				if (_freq_prev > 0)
				{
					const float    dif = _freq - _freq_prev;
					if (fabs (dif) < _smooth_thr * _freq)
					{
						_freq = _freq_prev + dif * _smoothing;
					}
				}
			}
		}
	}

	if (_delta == _win_len)
	{
		_freq_prev = _freq;
	}
}



/*

f (x) = a * x^2 + b * x + c

f (-1) = r1
f ( 0) = r2
f ( 1) = r3

a - b + c = r1
        c = r2
a + b + c = r3

a - b = r1 - r2
a + b = r3 - r2
c = r2

2 * a = r1 + r3 - 2 * r2
2 * b = r3 - r1
c = r2

a = r2 + (r3 + r1) * 0.5
b =      (r3 - r1) * 0.5
c = r2

f'(x) = 2 * a * x + b
f'(0) = 0
2 * a * x + b = 0
(r1 + r3 - 2 * r2) * x + (r3 - r1) * 0.5 = 0
x = (r1 - r3) * 0.5 / (r1 + r3 - 2 * r2)

*/



}  // namespace pi
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
