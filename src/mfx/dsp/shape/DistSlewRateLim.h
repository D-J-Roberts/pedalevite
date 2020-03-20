/*****************************************************************************

        DistSlewRateLim.h
        Author: Laurent de Soras, 2018

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_dsp_shape_DistSlewRateLim_HEADER_INCLUDED)
#define mfx_dsp_shape_DistSlewRateLim_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/def.h"



namespace mfx
{
namespace dsp
{
namespace shape
{



class DistSlewRateLim
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	void           set_sample_freq (double sample_freq);
	void           clear_buffers ();
	inline float   process_sample (float x);
	void           process_block (float dst_ptr [], const float src_ptr [], int nbr_spl);

	void           set_rate_limit (float rate_max_s);
	void           set_rate_limit_pos (float rate_max_s);
	void           set_rate_limit_neg (float rate_max_s);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	void           update_rate_p ();
	void           update_rate_n ();

	float          _sample_freq   = 44100;
	float          _inv_fs        = 1.f / _sample_freq;
	float          _rate_max_p_s  = 1000;                    // Pos, units/s
	float          _rate_max_n_s  = 1000;                    // Neg, units/s
	float          _rate_max_p    = _rate_max_p_s * _inv_fs; // Pos, units/spl
	float          _rate_max_n    = _rate_max_n_s * _inv_fs; // Neg, units/spl

	float          _state         = 0;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool           operator == (const DistSlewRateLim &other) const = delete;
	bool           operator != (const DistSlewRateLim &other) const = delete;

}; // class DistSlewRateLim



}  // namespace shape
}  // namespace dsp
}  // namespace mfx



#include "mfx/dsp/shape/DistSlewRateLim.hpp"



#endif   // mfx_dsp_shape_DistSlewRateLim_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
