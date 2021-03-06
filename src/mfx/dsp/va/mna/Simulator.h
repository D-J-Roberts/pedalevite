/*****************************************************************************

        Simulator.h
        Author: Laurent de Soras, 2020

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://www.wtfpl.net/ for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_dsp_va_mna_Simulator_HEADER_INCLUDED)
#define mfx_dsp_va_mna_Simulator_HEADER_INCLUDED



#define mfx_dsp_va_mna_Simulator_STATS



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "lal/Mat.h"
#include "mfx/dsp/va/mna/PartInterface.h"
#include "mfx/dsp/va/mna/SimulInterface.h"

#include <array>
#include <map>
#include <memory>
#include <vector>



namespace mfx
{
namespace dsp
{
namespace va
{
namespace mna
{



class Simulator
:	public SimulInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	typedef PartInterface::IdNode IdNode;
	typedef std::shared_ptr <PartInterface> PartSPtr;

	static const IdNode  _nid_invalid = PartInterface::_nid_invalid;
	static const IdNode  _nid_gnd     = PartInterface::_nid_gnd;
	static const IdNode  _nid_internal = 1 << 24;

	static const int     _max_it      = 500;

#if defined (mfx_dsp_va_mna_Simulator_STATS)
	typedef std::array <int, _max_it      +   1> HistIt;
	typedef std::array <int, _max_it * 16 + 100> HistEval;
	class Stats
	{
	public:
		HistIt         _hist_it = {{ 0 }}; // Histogram for the number of NR iterations
		int            _nbr_spl_proc = 0;  // Number of processed samples since the statistics start
	};
#endif // mfx_dsp_va_mna_Simulator_STATS

	               Simulator ()                        = default;
	               Simulator (const Simulator &other)  = default;
	               Simulator (Simulator &&other)       = default;

	virtual        ~Simulator ()                       = default;

	Simulator &    operator = (const Simulator &other) = default;
	Simulator &    operator = (Simulator &&other)      = default;

	void           add_part (PartSPtr part_sptr);

	void           prepare (double sample_freq);

	void           process_sample () noexcept;
	Flt            get_node_voltage (IdNode nid) const noexcept;

	void           clear_buffers () noexcept;

	// Information
	int            get_matrix_size () const noexcept;
	int            get_nbr_nodes () const noexcept;
	int            get_nbr_src_v () const noexcept;
	std::vector <Flt>
	               get_matrix () const;
	std::vector <Flt>
	               get_vector () const;

#if defined (mfx_dsp_va_mna_Simulator_STATS)
	void           reset_stats ();
	void           get_stats (Stats &stats) const;
#endif // mfx_dsp_va_mna_Simulator_STATS



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

	// SimulInterface
	PartInterface::IdNode
	               do_allocate_node () final;
	bool           do_is_node_gnd (int node_idx) const noexcept final;
	Flt            do_get_voltage (int node_idx) const noexcept final;
	Flt            do_get_voltage (int n1_idx, int n2_idx) const noexcept final;
	void           do_add_coef_mat (int row, int col, Flt val) noexcept final;
	void           do_add_coef_vec (int row, Flt val) noexcept final;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	typedef lal::Mat <Flt> TypeMatrix;
	typedef std::vector <Flt> TypeVector;

	void           build_matrix (int it_cnt) noexcept;

	std::vector <PartSPtr>
	               _part_arr;
	std::map <IdNode, int>
	               _nid_to_idx_map;

	IdNode         _nid_int_cur = _nid_internal;
	int            _nbr_nodes = 0;
	int            _nbr_src_v = 0;
	int            _idx_gnd   = 0;
	int            _msize     = 0; // _nbr_nodes + _nbr_src_v
	bool           _nl_flag   = false;
	bool           _matrix_ok_flag = false; // Indicates the matrix content is valid (and not an LU decomposition)
	Flt            _sample_freq = 0; // Hz, > 0. 0 = not set

	Flt            _max_dif   = Flt (0.15);

	std::vector <bool>             // Indicates which voltage are fixed (independant voltage sources referenced to GND)
	               _known_voltage_arr;
	TypeMatrix     _mat_a;
	TypeVector     _vec_z;
	TypeVector     _vec_x;
	TypeVector     _vec_x_old;

	TypeVector     _tmp_y;
	std::vector <int>
	               _reorder_r_arr;
	std::vector <int>
	               _reorder_c_arr;

#if defined (mfx_dsp_va_mna_Simulator_STATS)
	Stats          _st;
	int            _st_nbr_it = 0;
#endif // mfx_dsp_va_mna_Simulator_STATS



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool           operator == (const Simulator &other) const = delete;
	bool           operator != (const Simulator &other) const = delete;

}; // class Simulator



}  // namespace mna
}  // namespace va
}  // namespace dsp
}  // namespace mfx



//#include "mfx/dsp/va/mna/Simulator.hpp"



#endif   // mfx_dsp_va_mna_Simulator_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
