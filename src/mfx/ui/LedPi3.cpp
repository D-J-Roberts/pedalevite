/*****************************************************************************

        LedPi3.cpp
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

#include "fstb/fnc.h"
#include "mfx/ui/LedPi3.h"

#include <wiringPi.h>

#include <stdexcept>

#include <cassert>



namespace mfx
{
namespace ui
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



const int	LedPi3::_gpio_pin_arr [_nbr_led] = { 11, 13, 15 };



// Before calling:
// ::wiringPiSetupPhys ()
LedPi3::LedPi3 ()
:	_gpio_pwm (_pwm_resol)
,	_state_arr ()
,	_quit_flag (false)
,	_refresher (&LedPi3::refresh_loop, this)
{
	const int      ret_val = _gpio_pwm.init_chn (_pwm_chn, _pwm_cycle);
	if (ret_val != 0)
	{
		throw std::runtime_error ("Cannot initialize DMA channel");
	}

	for (int i = 0; i < _nbr_led; ++i)
	{
		::pinMode  (_gpio_pin_arr [i], OUTPUT);
		::digitalWrite (_gpio_pin_arr [i], LOW);
	}
}



LedPi3::~LedPi3 ()
{
	if (_refresher.joinable ())
	{
		_quit_flag = true;
		_refresher.join ();
	}
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



int	LedPi3::do_get_nbr_led () const
{
	return _nbr_led;
}



void	LedPi3::do_set_led (int index, float val)
{
	_state_arr [index]._val_cur = val;
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void	LedPi3::refresh_loop ()
{
	while (! _quit_flag)
	{
		for (int index = 0; index < _nbr_led && ! _quit_flag; ++index)
		{
			LedState &     state = _state_arr [index];
			if (state._val_cur != state._val_prev)
			{
				state._val_prev = state._val_cur;

				const int      pw = fstb::round_int (state._val_cur * _pwm_cycle);
				_gpio_pwm.set_pulse (_pwm_chn, _gpio_pin_arr [index], 0, pw);
			}
		}

		::delay (20);
	}

	_quit_flag = false;
}



}  // namespace ui
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
