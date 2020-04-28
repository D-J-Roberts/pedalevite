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

#include "mfx/dsp/va/mna/PartInterface.h"
#include "mfx/dsp/va/mna/SimulInterface.h"
#include "Eigen/Dense"

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

	static const int     _max_it      = 100;

#if defined (mfx_dsp_va_mna_Simulator_STATS)
	typedef std::array <int, _max_it      +   1> HistIt;
	typedef std::array <int, _max_it * 16 + 100> HistEval;
	class Stats
	{
	public:
		HistIt         _hist_it = {{ 0 }}; // Histogram for the number of NR iterations
		HistEval       _hist_f0 = {{ 0 }}; // Histogram for the number of f evaluations
		HistEval       _hist_f1 = {{ 0 }}; // Histogram for the number of f' evaluations
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

	void           process_sample ();
	float          get_node_voltage (IdNode nid) const;

	void           clear_buffers ();

#if defined (mfx_dsp_va_mna_Simulator_STATS)
	void           reset_stats ();
	void           get_stats (Stats &stats) const;
#endif // mfx_dsp_va_mna_Simulator_STATS



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

	// SimulInterface
	virtual bool   do_is_node_gnd (int node_idx) const final;
	virtual float  do_get_voltage (int node_idx) const final;
	virtual float  do_get_voltage (int n1_idx, int n2_idx) const final;
	virtual void   do_add_coef_mat (int row, int col, float val) final;
	virtual void   do_add_coef_vec (int row, float val) final;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	void           build_matrix ();

	std::vector <PartSPtr>
	               _part_arr;
	std::map <IdNode, int>
	               _nid_to_idx_map;

	int            _nbr_nodes = 0;
	int            _nbr_src_v = 0;
	int            _idx_gnd   = 0;
	int            _msize     = 0; // _nbr_nodes + _nbr_src_v
	bool           _nl_flag   = false;
	float          _sample_freq = 0; // Hz, > 0. 0 = not set

	float          _max_dif   = 0.1f;

	Eigen::MatrixXf
	               _mat_a;
	Eigen::VectorXf
	               _vec_z;
	Eigen::VectorXf
	               _vec_x;
	Eigen::VectorXf
	               _vec_x_old;
	Eigen::ColPivHouseholderQR <Eigen::MatrixXf>
	               _qr;

#if defined (mfx_dsp_va_mna_Simulator_STATS)
	Stats          _st;
	int            _st_nbr_it = 0;
	int            _st_nbr_f0 = 0;
	int            _st_nbr_f1 = 0;
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
