/*****************************************************************************

        ControlSource.h
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_ControlSource_HEADER_INCLUDED)
#define mfx_ControlSource_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/ControllerType.h"



namespace mfx
{

namespace doc
{
	class SerRInterface;
	class SerWInterface;
}



class ControlSource
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	inline         ControlSource (ControllerType type, int index);
	               ControlSource ()                           = default;
	               ~ControlSource ()                          = default;
	               ControlSource (const ControlSource &other) = default;
	ControlSource &
	               operator = (const ControlSource &other)    = default;

	ControllerType _type  = ControllerType_INVALID;
	int            _index = -1;

	inline bool    is_valid () const;
	inline bool    is_relative () const;
	inline bool    is_bipolar () const;
	inline bool    is_physical () const;

	inline bool    operator == (const ControlSource &other) const;
	inline bool    operator != (const ControlSource &other) const;

	void           ser_write (doc::SerWInterface &ser) const;
	void           ser_read (doc::SerRInterface &ser);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:


}; // class ControlSource



inline bool	operator < (const ControlSource &lhs, const ControlSource &rhs);



}  // namespace mfx



#include "mfx/ControlSource.hpp"



#endif   // mfx_ControlSource_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
