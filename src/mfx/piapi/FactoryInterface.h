/*****************************************************************************

        FactoryInterface.h
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_piapi_FactoryInterface_HEADER_INCLUDED)
#define mfx_piapi_FactoryInterface_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/piapi/PluginInterface.h"
#include "fstb/def.h"

#include <memory>
#include <vector>



namespace mfx
{
namespace piapi
{



class HostInterface;
class PluginDescInterface;
class PluginInterface;

class FactoryInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	// Use fstb_EXPORT() to export the function
	typedef int (fstb_CDECL * EnumFactoriesPtr) (std::vector <std::unique_ptr <FactoryInterface> > &fact_list);

	               FactoryInterface ()                              = default;
	               FactoryInterface (const FactoryInterface &other) = default;
	               FactoryInterface (FactoryInterface &&other)      = default;

	virtual        ~FactoryInterface ()                             = default;

	virtual FactoryInterface &
	               operator = (const FactoryInterface &other)       = default;
	virtual FactoryInterface &
	               operator = (FactoryInterface &&other)            = default;

	const PluginDescInterface &
	               describe ();
	std::unique_ptr <PluginInterface>
	               create (HostInterface &host);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

	virtual const PluginDescInterface &
	               do_describe () = 0;
	virtual std::unique_ptr <PluginInterface>
	               do_create (HostInterface &host) = 0;



}; // class FactoryInterface



}  // namespace piapi
}  // namespace mfx



//#include "mfx/piapi/FactoryInterface.hpp"



#endif   // mfx_piapi_FactoryInterface_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
