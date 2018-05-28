/*****************************************************************************

        ModelObserverDefault.h
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_ModelObserverDefault_HEADER_INCLUDED)
#define mfx_ModelObserverDefault_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/ModelObserverInterface.h"



namespace mfx
{



class ModelObserverDefault
:	public ModelObserverInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	virtual        ~ModelObserverDefault () = default;

	virtual void   do_set_tempo (double bpm) {}
	virtual void   do_set_edit_mode (bool edit_flag) {}
	virtual void   do_set_save_mode (doc::Setup::SaveMode mode) {}
	virtual void   do_set_setup_name (std::string name) {}
	virtual void   do_set_pedalboard_layout (const doc::PedalboardLayout &layout) {}
	virtual void   do_set_pedal (const PedalLoc &loc, const doc::PedalActionGroup &content) {}
	virtual void   do_set_bank (int index, const doc::Bank &bank) {}
	virtual void   do_select_bank (int index) {}
	virtual void   do_set_bank_name (std::string name) {}
	virtual void   do_set_preset_name (std::string name) {}
	virtual void   do_set_preset (int bank_index, int preset_index, const doc::Preset &preset) {}
	virtual void   do_activate_preset (int index) {}
	virtual void   do_store_preset (int preset_index, int bank_index) {}
	virtual void   do_set_chn_mode (ChnMode mode) {}
	virtual void   do_set_master_vol (float vol) {}
	virtual void   do_set_tuner (bool active_flag) {}
	virtual void   do_set_tuner_freq (float freq) {}
	virtual void   do_set_slot_info_for_current_preset (const SlotInfoMap &info_map) {}
	virtual void   do_set_param (int slot_id, int index, float val, PiType type) {}
	virtual void   do_set_param_beats (int slot_id, int index, float beats) {}
	virtual void   do_add_slot (int slot_id) {}
	virtual void   do_remove_slot (int slot_id) {}
	virtual void   do_insert_slot_in_chain (int index, int slot_id) {}
	virtual void   do_erase_slot_from_chain (int index) {}
	virtual void   do_set_slot_label (int slot_id, std::string name) {}
	virtual void   do_set_plugin (int slot_id, const PluginInitData &pi_data) {}
	virtual void   do_remove_plugin (int slot_id) {}
	virtual void   do_set_plugin_mono (int slot_id, bool mono_flag) {}
	virtual void   do_set_plugin_reset (int slot_id, bool reset_flag) {}
	virtual void   do_set_param_pres (int slot_id, PiType type, int index, const doc::ParamPresentation *pres_ptr) {}
	virtual void   do_set_param_ctrl (int slot_id, PiType type, int index, const doc::CtrlLinkSet &cls) {}
	virtual void   do_override_param_ctrl (int slot_id, PiType type, int index, int rotenc_index) {}
	virtual void   do_set_signal_port (int port_id, const doc::SignalPort &port) {}
	virtual void   do_clear_signal_port (int port_id) {}
	virtual void   do_add_settings (std::string model, int index, std::string name, const doc::PluginSettings &s_main, const doc::PluginSettings &s_mix) {}
	virtual void   do_remove_settings (std::string model, int index) {}
	virtual void   do_clear_all_settings () {}
	virtual void   do_set_click (bool click_flag) {}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



}; // class ModelObserverDefault



}  // namespace mfx



//#include "mfx/ModelObserverDefault.hpp"



#endif   // mfx_ModelObserverDefault_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
