/*****************************************************************************

        WorldAudio.h
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_WorldAudio_HEADER_INCLUDED)
#define mfx_WorldAudio_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "conc/CellPool.h"
#include "fstb/AllocAlign.h"
#include "mfx/ui/UserInputInterface.h"
#include "mfx/Cst.h"
#include "mfx/MsgQueue.h"
#include "piapi/EventTs.h"

#include <vector>



namespace mfx
{


class PluginPool;
class ProcessingContext;

class WorldAudio
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	explicit       WorldAudio (PluginPool &plugin_pool, MsgQueue &queue_from_cmd, MsgQueue &queue_to_cmd, ui::UserInputInterface::MsgQueue &queue_from_input, ui::UserInputInterface &input_device, conc::CellPool <Msg> &msg_pool_cmd);
	virtual        ~WorldAudio ();

	void           set_process_info (double sample_freq, int max_block_size);

	void           set_context (const ProcessingContext &ctx);
	void           process_block (float * const * dst_arr, const float * const * src_arr, int nbr_spl);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	static const int  _max_nbr_evt = 1024; // Per plug-ins. This is a soft limit.

	typedef std::vector <float, fstb::AllocAlign <float, 64> > AlignedZone;
	typedef std::array <float *, Cst::_max_nbr_buf> BufferArray;

	typedef std::array <
		const float *,
		Cst::_max_nbr_input                          * piapi::PluginInterface::_max_nbr_chn
	> BufSrcArr;
	typedef std::array <
		float *,
		(Cst::_max_nbr_input + Cst::_max_nbr_output) * piapi::PluginInterface::_max_nbr_chn
	> BufDstArr;

	typedef std::vector <piapi::EventTs> EventArray;

	typedef std::vector <const piapi::EventTs *> EventPtrList;
	typedef std::array <EventPtrList, Cst::_max_nbr_plugins> EventPtrListArray;

	void           collect_msg_cmd ();
	void           collect_msg_ui ();
	void           handle_controller (const ControlSource &controller, float val_raw);
	void           copy_input (const float * const * src_arr, int nbr_spl);
	void           copy_output (float * const * dst_arr, int nbr_spl);
	void           process_plugin_bundle (const ProcessingContext::PluginContext &pi_ctx, int nbr_spl);
	void           process_single_plugin (int plugin_id, piapi::PluginInterface::ProcInfo &proc_info);
	void           prepare_buffers (piapi::PluginInterface::ProcInfo &proc_info, const ProcessingContextNode &node, bool use_byp_as_src_flag);

	void           handle_msg_ctx (Msg::Ctx &msg);
	void           handle_msg_param (Msg::Param &msg);

	PluginPool &   _pi_pool;
	MsgQueue &     _queue_from_cmd;
	MsgQueue &     _queue_to_cmd;
	ui::UserInputInterface::MsgQueue &
	               _queue_from_input;
	ui::UserInputInterface &
	               _input_device;
	conc::CellPool <Msg> &
	               _msg_pool_cmd;

	int            _max_block_size;
	float          _sample_freq;

	AlignedZone    _buf_zone;
	BufferArray    _buf_arr;

	const ProcessingContext *
	               _ctx_ptr;

	EventArray     _evt_arr;            // Preallocated
	EventPtrList   _evt_ptr_arr;        // Preallocated



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               WorldAudio ()                               = delete;
	               WorldAudio (const WorldAudio &other)        = delete;
	WorldAudio &   operator = (const WorldAudio &other)        = delete;
	bool           operator == (const WorldAudio &other) const = delete;
	bool           operator != (const WorldAudio &other) const = delete;

}; // class WorldAudio



}  // namespace mfx



//#include "mfx/WorldAudio.hpp"



#endif   // mfx_WorldAudio_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
