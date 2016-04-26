/*****************************************************************************

        UserInputMsg.cpp
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

#include "mfx/ui/UserInputMsg.h"

#include <cassert>



namespace mfx
{
namespace ui
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void	UserInputMsg::set (UserInputType type, int index, float val)
{
	assert (type >= 0);
	assert (type < UserInputType_NBR_ELT);
	assert (index >= 0);
	assert (val >= 0);
	assert (val <= 1);

	_type  = type;
	_index = index;
	_val   = val;
}



UserInputType	UserInputMsg::get_type () const
{
	assert (is_valid ());

	return _type;
}



int	UserInputMsg::get_index () const
{
	assert (is_valid ());

	return _index;
}



float	UserInputMsg::get_val () const
{
	assert (is_valid ());

	return _val;
}



bool	UserInputMsg::is_valid () const
{
	return (_type >= 0);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace ui
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
