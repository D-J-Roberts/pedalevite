/*****************************************************************************

        PartDiode.cpp
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

#include "fstb/def.h"
#include "fstb/Approx.h"
#include "mfx/dsp/va/mna/PartDiode.h"

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



PartDiode::PartDiode (IdNode nid_1, IdNode nid_2, float is, float n)
:	_nid_arr {{ nid_1, nid_2 }}
,	_is (is)
,	_n (n)
,	_mul_e (compute_mul_e ())
,	_vcrit (compute_vcrit ())
{
	assert (nid_1 >= 0);
	assert (nid_2 >= 0);
	assert (nid_2 != nid_1);
	assert (is >= 1e-20f);
	assert (is <= 1e-1f);
	assert (n > 0);
}



void	PartDiode::set_is (float is)
{
	assert (is >= 1e-20f);
	assert (is <= 1e-1f);

	_is = is;
	compute_vcrit ();
}



void	PartDiode::set_n (float n)
{
	assert (n > 0);

	_n = n;
	compute_mul_e ();
	compute_vcrit ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void	PartDiode::do_get_info (PartInfo &info) const
{
	info._nid_arr.clear ();
	for (auto &nid : _nid_arr)
	{
		info._nid_arr.push_back (nid);
	}
	info._nbr_src_v       = 0;
	info._non_linear_flag = true;
}



void	PartDiode::do_prepare (SimulInterface &sim, const SimInfo &info)
{
	_sim_ptr = &sim;

	assert (info._node_idx_arr.size () == _node_arr.size ());
	for (int pos = 0; pos < int (_node_arr.size ()); ++pos)
	{
		_node_arr [pos] = info._node_idx_arr [pos];
	}
}



void	PartDiode::do_add_to_matrix ()
{
	const float    v   = _sim_ptr->get_voltage (_node_arr [0], _node_arr [1]);
	const float    e   = _is * fstb::Approx::exp2 (v * _mul_e);
	const float    s   = e - _is;
	const float    sd  = _mul_e * e;

	const float    geq = sd;
	const float    ieq = s - sd * v;

	_sim_ptr->add_norton (_node_arr [0], _node_arr [1], geq, ieq);
}



void	PartDiode::do_step ()
{
	// Nothing
}



void	PartDiode::do_clear_buffers ()
{
	// Nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



float	PartDiode::compute_mul_e () const
{
	return float (1.0 / (_n * _vt * fstb::LN2));
}



float	PartDiode::compute_vcrit () const
{
	const float    nvt = _n * _vt;
	const float    l2i = nvt / (_is * float (fstb::SQRT2));
	const float    l2o = nvt * float (fstb::LN2);

	return l2o * fstb::Approx::log2 (l2i);
}



}  // namespace mna
}  // namespace va
}  // namespace dsp
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
