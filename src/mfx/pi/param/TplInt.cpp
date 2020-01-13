/*****************************************************************************

        TplInt.cpp
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
#include "mfx/pi/param/HelperDispNum.h"
#include "mfx/pi/param/Tools.h"
#include "mfx/pi/param/TplInt.h"

#include <algorithm>

#include <cassert>



namespace mfx
{
namespace pi
{
namespace param
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// name_0 = printf format string, with group_index as optional argument (can
// be used up to 8 times)
TplInt::TplInt (int val_min, int val_max, const char *name_0, const char *unit_0, int group_index, const char *format_0)
:	_phdn ()
,	_group_index (group_index)
,	_name ()
,	_unit (unit_0)
,	_flags (0)
{
	assert (val_min < val_max);
	assert (name_0);
	assert (unit_0);
	assert (format_0 != nullptr);

	_phdn.set_range (double (val_min), double (val_max));
	_phdn.set_print_format (format_0);
	_phdn.set_preset (HelperDispNum::Preset_INT_BASE_0);

	char           txt_0 [1023+1];
	fstb::snprintf4all (txt_0, sizeof (txt_0), name_0,
		_group_index, _group_index, _group_index, _group_index,
		_group_index, _group_index, _group_index, _group_index,
		_group_index, _group_index, _group_index, _group_index
	);
	_name = txt_0;
}



HelperDispNum &	TplInt::use_disp_num ()
{
	return (_phdn);
}



const HelperDispNum &	TplInt::use_disp_num () const
{
	return (_phdn);
}



void	TplInt::set_flags (int32_t flags)
{
	_flags = flags;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



std::string	TplInt::do_get_name (int len) const
{
	if (len == 0)
	{
		return _name;
	}

	return (Tools::print_name_bestfit (len, _name.c_str ()));
}



std::string	TplInt::do_get_unit (int len) const
{
	if (len == 0)
	{
		return _unit;
	}

	return (Tools::print_name_bestfit (len, _unit.c_str ()));
}



piapi::ParamDescInterface::Range	TplInt::do_get_range () const
{
	return (piapi::ParamDescInterface::Range_DISCRETE);
}



piapi::ParamDescInterface::Categ	TplInt::do_get_categ () const
{
	return (piapi::ParamDescInterface::Categ_UNDEFINED);
}



int32_t	TplInt::do_get_flags () const
{
	return _flags;
}



double	TplInt::do_get_nat_min () const
{
	return (_phdn.get_val_min ());
}



double	TplInt::do_get_nat_max () const
{
	return (_phdn.get_val_max ());
}



std::string	TplInt::do_conv_nat_to_str (double nat, int len) const
{
	const int      max_len = 1024;
	char           txt_0 [max_len+1];
	nat = fstb::round (nat);
	len = (len > 0) ? std::min (len, max_len) : max_len;
	_phdn.conv_to_str (nat, txt_0, len);

	return (txt_0);
}



bool	TplInt::do_conv_str_to_nat (double &nat, const std::string &txt) const
{
	const int      ret_val = _phdn.conv_from_str (txt.c_str (), nat);
	const bool     ok_flag = (ret_val == HelperDispNum::Err_OK);
	if (ok_flag)
	{
		nat = fstb::round (nat);
	}
	else
	{
		nat = _phdn.get_val_min ();
	}

	return (ok_flag);
}



double	TplInt::do_conv_nrm_to_nat (double nrm) const
{
	const double   val_min = _phdn.get_val_min ();
	const double   val_max = _phdn.get_val_max ();
	const double	w       = val_max - val_min;

	return (val_min + fstb::round (nrm * w));
}



double	TplInt::do_conv_nat_to_nrm (double nat) const
{
	const double   val_min = _phdn.get_val_min ();
	const double   val_max = _phdn.get_val_max ();
	const double	w       = val_max - val_min;

	return (fstb::round (nat - val_min) / w);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace param
}  // namespace pi
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
