/*****************************************************************************

        DistRandWalk.cpp
        Author: Laurent de Soras, 2018

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

#include "mfx/dsp/shape/DistRandWalk.h"

#include <cassert>



namespace mfx
{
namespace dsp
{
namespace shape
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void	DistRandWalk::set_sample_freq (double sample_freq) noexcept
{
	assert (sample_freq > 0);

	_sample_freq = float (sample_freq);
	_avg_per     = _sample_freq / float (_freq);
}



void	DistRandWalk::clear_buffers () noexcept
{
	_sum     = 0;
	_val     = 0;
	_inc     = 0;
	_pos     = 0;
	_nbr_spl = 1;
}



void	DistRandWalk::process_block (float dst_ptr [], const float src_ptr [], int nbr_spl) noexcept
{
	assert (dst_ptr != nullptr);
	assert (src_ptr != nullptr);
	assert (nbr_spl != 0);

	for (int pos = 0; pos < nbr_spl; ++pos)
	{
		dst_ptr [pos] = process_sample (src_ptr [pos]);
	}
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace shape
}  // namespace dsp
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
