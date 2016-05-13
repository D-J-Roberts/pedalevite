/*****************************************************************************

        Slot.h
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_doc_Slot_HEADER_INCLUDED)
#define mfx_doc_Slot_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/doc/PluginSettings.h"
#include "mfx/pi/PluginModel.h"

#include <map>
#include <string>



namespace mfx
{
namespace doc
{



class Slot
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	typedef std::shared_ptr <PluginSettings> SettingsSPtr;
	typedef std::map <pi::PluginModel, SettingsSPtr> SettingHistory;

	               Slot ()                        = default;
	               Slot (const Slot &other)       = default;
	virtual        ~Slot ()                       = default;
	Slot &         operator = (const Slot &other) = default;

	pi::PluginModel
	               _pi_model = pi::PluginModel_INVALID;
	SettingHistory _settings_all;
	PluginSettings _settings_mixer;
	std::string    _name;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool           operator == (const Slot &other) const = delete;
	bool           operator != (const Slot &other) const = delete;

}; // class Slot



}  // namespace doc
}  // namespace mfx



//#include "mfx/doc/Slot.hpp"



#endif   // mfx_doc_Slot_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
