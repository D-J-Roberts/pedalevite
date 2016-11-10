/*****************************************************************************

        DataAlign.h
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (fstb_DataAlign_HEADER_INCLUDED)
#define fstb_DataAlign_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/ToolsSimd.h"

#include <type_traits>



namespace fstb
{



template <bool A>
class DataAlign
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	typedef std::integral_constant <bool, A> Alignment;

	static inline bool
		            check_ptr (const void *ptr);
	static inline ToolsSimd::VectF32
		            load_f32 (const void *ptr);
	static inline void
		            store_f32 (void *ptr, ToolsSimd::VectF32 v);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               ~DataAlign ()                              = delete;
	               DataAlign ()                               = delete;
	               DataAlign (const DataAlign &other)         = delete;
	DataAlign &    operator = (const DataAlign &other)        = delete;
	bool           operator == (const DataAlign &other) const = delete;
	bool           operator != (const DataAlign &other) const = delete;

}; // class DataAlign



}  // namespace fstb



#include "fstb/DataAlign.hpp"



#endif   // fstb_DataAlign_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
