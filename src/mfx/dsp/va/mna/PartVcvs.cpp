/*****************************************************************************

        PartVcvs.cpp
        Author: Laurent de Soras, 2020

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://www.wtfpl.net/ for more details.

*Tab=3***********************************************************************/



#if defined (_MSC_VER)
	#pragma warning (1 : 4130 4223 4705 4706)
	#pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/dsp/va/mna/PartVcvs.h"

#include <cassert>



namespace mfx
{
namespace dsp
{
namespace va
{
namespace mna
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



PartVcvs::PartVcvs (IdNode nid_i_1, IdNode nid_i_2, IdNode nid_o_1, IdNode nid_o_2, float g)
:	_nid_i_arr {{ nid_i_1, nid_i_2 }}
,	_nid_o_arr {{ nid_o_1, nid_o_2 }}
,	_g (g)
{
	assert (nid_i_1 >= 0);
	assert (nid_i_2 >= 0);
	assert (nid_i_2 != nid_i_1);
	assert (nid_o_1 >= 0);
	assert (nid_o_2 >= 0);
	assert (nid_o_2 != nid_o_1);
	assert (   nid_o_1 != nid_i_1 || nid_o_1 != nid_i_2
	        || nid_o_2 != nid_i_1 || nid_o_2 != nid_i_2);
}



void	PartVcvs::set_gain (float g)
{
	_g = g;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void	PartVcvs::do_get_info (SimulInterface &sim, PartInfo &info)
{
	_sim_ptr = &sim;

	info._nid_arr.clear ();
	for (auto &nid : _nid_i_arr)
	{
		info._nid_arr.push_back (nid);
	}
	for (auto &nid : _nid_o_arr)
	{
		info._nid_arr.push_back (nid);
	}
	info._nbr_src_v       = 1;
	info._non_linear_flag = false;
}



void	PartVcvs::do_prepare (const SimInfo &info)
{
	assert (info._node_idx_arr.size () == _node_i_arr.size () + _node_o_arr.size ());
	for (int pos = 0; pos < int (_node_i_arr.size ()); ++pos)
	{
		_node_i_arr [pos] = info._node_idx_arr [pos];
		_node_o_arr [pos] = info._node_idx_arr [pos + int (_node_i_arr.size ())];
	}
	_src_v_pos = info._src_v_idx_arr [0];
}



void	PartVcvs::do_add_to_matrix ()
{
	_sim_ptr->add_coef_mat (_node_o_arr [0], _src_v_pos, -1);
	_sim_ptr->add_coef_mat (_node_o_arr [1], _src_v_pos, +1);
	_sim_ptr->add_coef_mat (_src_v_pos, _node_i_arr [0], +_g);
	_sim_ptr->add_coef_mat (_src_v_pos, _node_i_arr [1], -_g);
	_sim_ptr->add_coef_mat (_src_v_pos, _node_o_arr [0], -1);
	_sim_ptr->add_coef_mat (_src_v_pos, _node_o_arr [1], +1);
}



void	PartVcvs::do_step ()
{
	// Nothing
}



void	PartVcvs::do_clear_buffers ()
{
	// Nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace mna
}  // namespace va
}  // namespace dsp
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
