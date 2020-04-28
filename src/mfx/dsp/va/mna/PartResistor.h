/*****************************************************************************

        PartResistor.h
        Author: Laurent de Soras, 2020

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://www.wtfpl.net/ for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_dsp_va_mna_PartResistor_HEADER_INCLUDED)
#define mfx_dsp_va_mna_PartResistor_HEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/dsp/va/mna/PartInterface.h"
#include "mfx/dsp/va/mna/SimulInterface.h"

#include <array>



namespace mfx
{
namespace dsp
{
namespace va
{
namespace mna
{



class PartResistor
:	public PartInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	explicit       PartResistor (IdNode nid_1, IdNode nid_2, float r);
	virtual        ~PartResistor () = default;

	void           set_resistance (float r);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

	// PartInterface
	void           do_get_info (PartInfo &info) const final;
	void           do_prepare (SimulInterface &sim, const SimInfo &info) final;
	void           do_add_to_matrix () final;
	void           do_step () final;
	void           do_clear_buffers () final;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	std::array <IdNode, 2>
	               _nid_arr  = {{ _nid_invalid, _nid_invalid }};
	std::array <int, 2>
	               _node_arr = {{
	               	SimulInterface::_node_invalid,
	               	SimulInterface::_node_invalid
	               }};
	SimulInterface *
	               _sim_ptr = nullptr;
	float          _r       = 1000;     // Resistance, ohm. > 0
	float          _g       = 1.f / _r; // Conductance, siemens. > 0



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               PartResistor ()                               = delete;
	               PartResistor (const PartResistor &other)      = delete;
	               PartResistor (PartResistor &&other)           = delete;
	PartResistor & operator = (const PartResistor &other)        = delete;
	PartResistor & operator = (PartResistor &&other)             = delete;
	bool           operator == (const PartResistor &other) const = delete;
	bool           operator != (const PartResistor &other) const = delete;

}; // class PartResistor



}  // namespace mna
}  // namespace va
}  // namespace dsp
}  // namespace mfx



//#include "mfx/dsp/va/mna/PartResistor.hpp"



#endif   // mfx_dsp_va_mna_PartResistor_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
