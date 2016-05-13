/*****************************************************************************

        PluginPool.h
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_PluginPool_HEADER_INCLUDED)
#define mfx_PluginPool_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/BitFieldSparse.h"
#include "mfx/piapi/PluginInterface.h"
#include "mfx/Cst.h"
#include "mfx/SharedRscState.h"

#include <array>
#include <atomic>
#include <memory>
#include <vector>



namespace mfx
{



class PluginPool
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	typedef std::unique_ptr <piapi::PluginInterface> PluginUPtr;

	class PluginDetails
	{
	public:
		PluginUPtr     _pi_uptr;
		std::vector <float>              // Only for ParamCateg_GLOBAL. Read-only for non-audio threads.
		               _param_arr;
		fstb::BitFieldSparse             // Same remark. Managed by the audio thread only.
		               _param_update;
		std::vector <bool>               // Indicates that the latest change comes from the audio thread. Same remark. Audio thread only
		               _param_update_from_audio;
	};

	               PluginPool ()  = default;
	virtual        ~PluginPool () = default;

	// Command thread
	int            get_room () const;
	int            add (PluginUPtr &pi_uptr);
	void           schedule_for_release (int index);
	void           release (int index);
	std::vector <int>
	               list_plugins (SharedRscState state = SharedRscState_INUSE) const;

	// Any thread
	PluginDetails& use_plugin (int index);
	SharedRscState get_state (int index);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	class PluginSlot
	{
	public:
		               ~PluginSlot ();
		PluginDetails  _details;
		SharedRscState _state = SharedRscState_FREE;
	};

	typedef std::array <PluginSlot, Cst::_max_nbr_plugins> PluginArray;

	PluginArray    _pi_arr;
	int            _nbr_plugins = 0;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               PluginPool (const PluginPool &other)        = delete;
	PluginPool &   operator = (const PluginPool &other)        = delete;
	bool           operator == (const PluginPool &other) const = delete;
	bool           operator != (const PluginPool &other) const = delete;

}; // class PluginPool



}  // namespace mfx



//#include "mfx/PluginPool.hpp"



#endif   // mfx_PluginPool_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
