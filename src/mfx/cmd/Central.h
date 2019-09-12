/*****************************************************************************

        Central.h
        Author: Laurent de Soras, 2016

All function calls must be synchronised by the caller.

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_cmd_Central_HEADER_INCLUDED)
#define mfx_cmd_Central_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/cmd/Plugin.h"
#include "mfx/cmd/Slot.h"
#include "mfx/piapi/PluginState.h"
#include "mfx/ui/UserInputInterface.h"
#include "mfx/ChnMode.h"
#include "mfx/PluginPool.h"
#include "mfx/ProcessingContext.h"
#include "mfx/WaMsgQueue.h"
#include "mfx/WorldAudio.h"

#include <array>
#include <chrono>
#include <map>
#include <memory>
#include <set>
#include <vector>



namespace mfx
{

namespace doc
{
	class CtrlLinkSet;
	class Preset;
}

class MeterResultSet;

namespace cmd
{



class CentralCbInterface;

class Central
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	explicit       Central (ui::UserInputInterface::MsgQueue &queue_input_to_audio, ui::UserInputInterface &input_device);
	virtual        ~Central ();

	void           set_callback (CentralCbInterface *cb_ptr);

	// Audio
	void           set_process_info (double sample_freq, int max_block_size);
	void           process_block (float * const * dst_arr, const float * const * src_arr, int nbr_spl);
	MeterResultSet &
	               use_meters ();
	float          get_audio_period_ratio () const;

	// Transactionnal operations
	void           commit ();
	void           rollback ();

	void           clear ();
	void           insert_slot (int pos);
	void           delete_slot (int pos);
	void           clear_slot (int pos);
	int            set_plugin (int pos, std::string model, bool force_reset_flag, bool gen_audio_flag);
	void           preinstantiate_plugins (std::string model, int count, const piapi::PluginState *state_ptr);
	void           remove_plugin (int pos);
	int            set_mixer (int pos);
	void           remove_mixer (int pos);
	void           force_mono (int pos, bool flag);
	int            find_pi (int pi_id);
	void           set_pi_state (int pi_id, const std::vector <float> &param_list);
	void           clear_mod (int pi_id);
	void           set_mod (int pi_id, int index, const doc::CtrlLinkSet &cls);
	void           set_sig_source (int pi_id, int sig_pin, int port_id);
	void           set_chn_mode (ChnMode mode);
	void           set_master_vol (double vol);
	void           set_transition (bool smooth_flag);

	// Immediate operations
	void           set_param (int pi_id, int index, float val);
	void           set_tempo (float bpm);

	// Call this regularly
	void           process_queue_audio_to_cmd ();

	// Misc
	PluginPool &   use_pi_pool ();
	const PluginPool &
	               use_pi_pool () const;
	std::chrono::microseconds
	               get_cur_date () const;
	int            get_dummy_mix_id () const;
	const piapi::PluginState &
	               use_default_settings (std::string model) const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	typedef std::shared_ptr <ProcessingContext> ContextSPtr;

	class Document
	{
	public:
		typedef std::map <int, bool> InstanceMap; // [Id] = use

		class PluginLoc
		{
		public:
			int            _slot_pos;
			PiType         _type;
		};

		std::vector <Slot>
		               _slot_list;
		ContextSPtr    _ctx_sptr;
		std::map <std::string, InstanceMap>       // All the existing plug-ins, ordered by model.
		               _map_model_id;
		std::map <int, PluginLoc>
		               _map_id_loc;
		ChnMode        _chn_mode   = ChnMode_1M_1M;
		float          _master_vol = 1;
		bool           _smooth_transition_flag = false;
	};

	typedef std::shared_ptr <Document> DocumentSPtr;

	typedef std::set <ContextSPtr> ContextSet;

	typedef std::map <std::string, piapi::PluginState> DefaultMap;

	Document &     modify ();
	Plugin &       find_plugin (Document &doc, int pi_id);
	bool           is_plugin_used_in_audio_thread (int pi_id) const;
	int            set_plugin (int pos, std::string model, PiType type, bool force_reset_flag, bool gen_audio_flag);
	void           remove_plugin (int pos, PiType type);
	void           create_routing ();
	void           create_mod_maps ();
	void           add_controller (ProcessingContext &ctx, const doc::CtrlLink &link, std::shared_ptr <ControlledParam> &ctrl_param_sptr, bool abs_flag);
	void           create_param_msg (std::vector <conc::LockFreeCell <WaMsg> *> &msg_list);
	conc::LockFreeCell <WaMsg> *
	               make_param_msg (int pi_id, int index, float val);
	void           check_and_get_default_settings (piapi::PluginInterface &plug, const piapi::PluginDescInterface &desc, std::string model);

	conc::CellPool <WaMsg>
						_msg_pool;

	ui::UserInputInterface &
	               _input_device;
	ui::UserInputInterface::MsgQueue &
	               _queue_input_to_audio;
	WaMsgQueue     _queue_cmd_to_audio;
	WaMsgQueue     _queue_audio_to_cmd;
	PluginPool     _plugin_pool;
	WorldAudio     _audio;
	DefaultMap     _default_map;
	double         _sample_freq;  // 0 = not set
	int            _max_block_size;  // 0 = not set
	CentralCbInterface *
	               _cb_ptr;       // 0 = not set

	DocumentSPtr   _cur_sptr;     // Current state
	DocumentSPtr   _new_sptr;     // Context being created or modified
	ContextSet     _ctx_trash;    // Contexts waiting to be finally destroyed.

	int            _dummy_mix_id;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               Central ()                               = delete;
	               Central (const Central &other)           = delete;
	Central &      operator = (const Central &other)        = delete;
	bool           operator == (const Central &other) const = delete;
	bool           operator != (const Central &other) const = delete;

}; // class Central



}  // namespace cmd
}  // namespace mfx



//#include "mfx/cmd/Central.hpp"



#endif   // mfx_cmd_Central_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
