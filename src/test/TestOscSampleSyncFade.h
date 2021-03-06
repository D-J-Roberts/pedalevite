/*****************************************************************************

        TestOscSampleSyncFade.h
        Author: Laurent de Soras, 2019

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (TestOscSampleSyncFade_HEADER_INCLUDED)
#define TestOscSampleSyncFade_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/dsp/rspl/InterpFtor.h"
#include "mfx/dsp/osc/OscSample.h"
#include "mfx/dsp/osc/OscSampleSyncFade.h"
#include "test/TestSampleMipMapper.h"

#include <vector>

#include <cstdint>



class TestOscSampleSyncFade
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	static int     perform_test ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	typedef TestSampleMipMapper MipMapper;

	typedef mfx::dsp::osc::OscSample <
		MipMapper::SampleDataType,
		mfx::dsp::rspl::InterpFtor::CubicHermite,
		int32_t
	> InternalOscType;

	typedef mfx::dsp::osc::OscSampleSyncFade <InternalOscType> OscType;

	static void    perform_test_internal (OscType &osc, std::vector <float> &result_m, int dest_len, int block_len, int sync_pos_start, float sync_pos_speed, int master_pitch_start, int master_pitch_end, int slave_pitch_start, int slave_pitch_end);
	static void    perform_test_internal_2 (OscType &osc, std::vector <OscType::CalcDataType> &dest, int dest_len, int block_len, int sync_pos_start, float sync_pos_speed, int master_pitch_start, int master_pitch_end, int slave_pitch_start, int slave_pitch_end);
	static void    add_to_dest (std::vector <float> &result_m, const std::vector <OscType::CalcDataType> &dest_int);



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               TestOscSampleSyncFade ()                               = delete;
	               TestOscSampleSyncFade (const TestOscSampleSyncFade &other) = delete;
	virtual        ~TestOscSampleSyncFade ()                              = delete;
	TestOscSampleSyncFade &
	               operator = (const TestOscSampleSyncFade &other)        = delete;
	bool           operator == (const TestOscSampleSyncFade &other) const = delete;
	bool           operator != (const TestOscSampleSyncFade &other) const = delete;

}; // class TestOscSampleSyncFade



//#include "test/TestOscSampleSyncFade.hpp"



#endif   // TestOscSampleSyncFade_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
