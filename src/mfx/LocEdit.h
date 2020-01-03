/*****************************************************************************

        LocEdit.h
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_LocEdit_HEADER_INCLUDED)
#define mfx_LocEdit_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/PiType.h"



namespace mfx
{



class View;

class LocEdit
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	               LocEdit ()  = default;
	virtual        ~LocEdit () = default;

	void           fix_chain_flag (const View &view);

	int            _slot_id       = -1;          // -1 if none
	PiType         _pi_type       = PiType_MIX;  // Should always be valid
	int            _param_index   = -1;          // -1 if none
	int            _ctrl_index    = -1;          // -1 if none or new. Always 0 for existing direct links
	bool           _ctrl_abs_flag = true;        // Link or simple modulation
	bool           _chain_flag    = true;        // When _slot_id is negative, indicates if we should focus on the main processing chain or elsewhere.



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               LocEdit (const LocEdit &other)           = delete;
	LocEdit &      operator = (const LocEdit &other)        = delete;
	bool           operator == (const LocEdit &other) const = delete;
	bool           operator != (const LocEdit &other) const = delete;

}; // class LocEdit



}  // namespace mfx



//#include "mfx/LocEdit.hpp"



#endif   // mfx_LocEdit_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
