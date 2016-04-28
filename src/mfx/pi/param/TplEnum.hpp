/*****************************************************************************

        TplEnum.hpp
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if ! defined (mfx_pi_param_TplEnum_CODEHEADER_INCLUDED)
#define mfx_pi_param_TplEnum_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/pi/param/Tools.h"
#include	"fstb/txt/utf8/ConvNum.h"
#include	"fstb/txt/utf8/fnc.h"
#include "fstb/fnc.h"
#include	"fstb/Err.h"

#include	<cassert>
#include	<cstdio>
#include	<cstring>



namespace mfx
{
namespace pi
{
namespace param
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// name_0 = printf format string, with group_index as optional argument (can
// be used up to 8 times)
TplEnum::TplEnum (const char *val_list_0, const char *name_0, const char *unit_0, int group_index, const char *format_0)
:	_group_index (group_index)
,	_name_list ()
,	_name ()
,	_unit (unit_0)
,	_print_format (format_0)
{
	assert (val_list_0 != 0);
	assert (name_0);
	assert (unit_0);
	assert (format_0 != 0);

	const char *	eol_0 = val_list_0;
	do
	{
		const char *	beg_0 = eol_0;
		eol_0 = strchr (beg_0, '\n');
		size_t			len;
		if (eol_0 == 0)
		{
			len = strlen (beg_0);
		}
		else
		{
			len = eol_0 - beg_0;
			++ eol_0;
		}

		const std::string	val (beg_0, len);
		_name_list.push_back (val);
	}
	while (eol_0 != 0);

	char           txt_0 [1023+1];
	fstb::snprintf4all (txt_0, sizeof (txt_0), name_0,
		_group_index, _group_index, _group_index, _group_index,
		_group_index, _group_index, _group_index, _group_index
	);
	_name = txt_0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



std::string	TplEnum::do_get_name (int len) const
{
	return (Tools::print_name_bestfit (len, _name.c_str ()));
}



std::string	TplEnum::do_get_unit (int len) const
{
	return (Tools::print_name_bestfit (len, _unit.c_str ()));
}



piapi::ParamDescInterface::Range	TplEnum::do_get_range () const
{
	return (piapi::ParamDescInterface::Range_DISCRETE);
}



piapi::ParamDescInterface::Categ	TplEnum::do_get_categ () const
{
	return (piapi::ParamDescInterface::Categ_UNDEFINED);
}



double	TplEnum::do_get_nat_min () const
{
	return (0);
}



double	TplEnum::do_get_nat_max () const
{
	return (double (_name_list.size () - 1));
}



std::string	TplEnum::do_conv_nat_to_str (double nat, int len) const
{
	const int      index = fstb::round_int (nat);
	char           txt_0 [1023+1];

	fstb::snprintf4all (
		txt_0,
		sizeof (txt_0),
		_print_format.c_str (),
		_name_list [index].c_str ()
	);

	return (txt_0);
}



bool	TplEnum::do_conv_str_to_nat (double &nat, const std::string &txt) const
{
	bool           ok_flag = true;

	const long     nbr_str    = long (_name_list.size ());
	bool           found_flag = false;
	for (long index = 0
	;	index < nbr_str && ! found_flag && ok_flag
	;	++index)
	{
		const std::string &  str = _name_list [index];
		long           pos;
		int            loc_ret_val =
			fstb::txt::utf8::strstr_ci (txt.c_str (), str.c_str (), pos);
		ok_flag = (loc_ret_val == fstb::Err_OK);
		if (ok_flag && pos >= 0)
		{
			nat = index;
			assert (nat >= do_get_nat_min ());
			assert (nat <= do_get_nat_max ());
			found_flag = true;
		}
	}

	if (ok_flag && ! found_flag)
	{
		int64_t        index;
		if (fstb::txt::utf8::ConvNum::conv_str_to_int64 (index, txt.c_str (), 10, 0))
		{
			nat = double (index);
			if (   nat >= do_get_nat_min ()
			    && nat <= do_get_nat_max ())
			{
				found_flag = true;
			}
		}
	}

	if (ok_flag && ! found_flag)
	{
		ok_flag = false;
	}

	return (ok_flag);
}



double	TplEnum::do_conv_nrm_to_nat (double nrm) const
{
	const double   w = double (_name_list.size () - 1);

	return (fstb::round (nrm * w));
}



double	TplEnum::do_conv_nat_to_nrm (double nat) const
{
	const double   w = double (_name_list.size () - 1);

	return (fstb::round (nat) / w);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace param
}  // namespace pi
}  // namespace mfx



#endif   // mfx_pi_param_TplEnum_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
