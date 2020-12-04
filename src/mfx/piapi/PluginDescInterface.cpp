/*****************************************************************************

        PluginDescInterface.cpp
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

#include "mfx/piapi/PluginDescInterface.h"
#include "mfx/piapi/PluginInterface.h"

#include <cassert>



namespace mfx
{
namespace piapi
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



PluginInfo	PluginDescInterface::get_info () const
{
	const PluginInfo  info { do_get_info () };

	assert (! info._unique_id.empty ());
	assert (! info._name.empty ());

	return info;
}



void	PluginDescInterface::get_nbr_io (int &nbr_i, int &nbr_o, int &nbr_s) const
{
	assert (nbr_i == 1);
	assert (nbr_o == 1);
	assert (nbr_s == 0);

	do_get_nbr_io (nbr_i, nbr_o, nbr_s);
	assert (nbr_i >= 0);
	assert (nbr_i <= PluginInterface::_max_nbr_pins);
	assert (nbr_o >= 0);
	assert (nbr_o <= PluginInterface::_max_nbr_pins);
	assert (nbr_s >= 0);
	assert (nbr_s <= PluginInterface::_max_nbr_sig);
}



int	PluginDescInterface::get_nbr_param (ParamCateg categ) const
{
	assert (categ >= 0);
	assert (categ < ParamCateg_NBR_ELT);

	const int      nbr_param = do_get_nbr_param (categ);
	assert (nbr_param >= 0);

	return (nbr_param);
}




const ParamDescInterface &	PluginDescInterface::get_param_info (ParamCateg categ, int index) const
{
	assert (categ >= 0);
	assert (categ < ParamCateg_NBR_ELT);
	assert (index >= 0);
	assert (index < get_nbr_param (categ));

	const auto &   desc = do_get_param_info (categ, index);

	return (desc);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace piapi
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
