/*****************************************************************************

        OscSquare.cpp
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

#include "mfx/dsp/ctrl/lfo/OscSquare.h"

#include <utility>

#include <cassert>



namespace mfx
{
namespace dsp
{
namespace ctrl
{
namespace lfo
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void	OscSquare::do_set_sample_freq (double sample_freq)
{
	_phase_gen.set_sample_freq (sample_freq);
}



void	OscSquare::do_set_period (double per)
{
	_phase_gen.set_period (per);
}



void	OscSquare::do_set_phase (double phase)
{
	_phase_gen.set_phase (phase);
}



void	OscSquare::do_set_chaos (double chaos)
{
	_phase_gen.set_chaos (chaos);
}



void	OscSquare::do_set_phase_dist (double dist)
{
	_phase_dist.set_phase_dist (dist);
}



void	OscSquare::do_set_phase_dist_offset (double ofs)
{
	_phase_dist.set_phase_dist_offset (ofs);
}



void	OscSquare::do_set_sign (bool inv_flag)
{
	_inv_flag = inv_flag;
	update_time_values ();
}



void	OscSquare::do_set_polarity (bool unipolar_flag)
{
	_unipolar_flag = unipolar_flag;
	update_time_values ();
}



void	OscSquare::do_set_variation (int /*param*/, double /*val*/)
{
	// Nothing
}



bool	OscSquare::do_is_using_variation (int /*param*/) const
{
	return false;
}



void	OscSquare::do_tick (int nbr_spl)
{
	_phase_gen.tick (nbr_spl);
}



double	OscSquare::do_get_val () const
{
	const double   phase   = _phase_dist.process_phase (_phase_gen.get_phase ());
	const bool     up_flag = (phase >= 0.5f);

	return (up_flag) ? _val_1 : _val_0;
}



double	OscSquare::do_get_phase () const
{
	return _phase_gen.get_phase ();
}



void	OscSquare::do_clear_buffers ()
{
	_phase_gen.clear_buffers ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void	OscSquare::update_time_values ()
{
	_val_0 = (_unipolar_flag) ? 0.f : -1.f;
	_val_1 = 1.f;
	if (_inv_flag)
	{
		std::swap (_val_0, _val_1);
	}
}

}  // namespace lfo
}  // namespace ctrl
}  // namespace dsp
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
