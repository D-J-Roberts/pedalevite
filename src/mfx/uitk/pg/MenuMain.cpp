/*****************************************************************************

        MenuMain.cpp
        Author: Laurent de Soras, 2016

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

#include "fstb/def.h"
#include "mfx/uitk/pg/EndMsg.h"
#include "mfx/uitk/pg/MenuMain.h"
#include "mfx/uitk/NodeEvt.h"
#include "mfx/uitk/PageMgrInterface.h"
#include "mfx/uitk/PageSwitcher.h"
#include "mfx/ui/Font.h"
#include "mfx/Model.h"
#include "mfx/View.h"

#include <cassert>
#include <cstdlib>



namespace mfx
{
namespace uitk
{
namespace pg
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



MenuMain::MenuMain (PageSwitcher &page_switcher, PedalEditContext &pedal_ctx)
:	_page_switcher (page_switcher)
,	_pedal_ctx (pedal_ctx)
,	_model_ptr (nullptr)
,	_view_ptr (nullptr)
,	_page_ptr (nullptr)
,	_page_size ()
,	_state (State_NORMAL)
,	_edit_prog_sptr (  std::make_shared <NText> (Entry_PROG  ))
,	_edit_bank_sptr (  std::make_shared <NText> (Entry_BANKS ))
,	_edit_layout_sptr (std::make_shared <NText> (Entry_LAYOUT))
,	_edit_levels_sptr (std::make_shared <NText> (Entry_LEVELS))
,	_settings_sptr    (std::make_shared <NText> (Entry_SETTINGS))
,	_tuner_sptr (      std::make_shared <NText> (Entry_TUNER ))
,	_reboot_sptr (     std::make_shared <NText> (Entry_REBOOT))
,	_reboot_arg ()
{
	_edit_prog_sptr  ->set_justification (0.5f, 0, false);
	_edit_bank_sptr  ->set_justification (0.5f, 0, false);
	_edit_layout_sptr->set_justification (0.5f, 0, false);
	_edit_levels_sptr->set_justification (0.5f, 0, false);
	_settings_sptr   ->set_justification (0.5f, 0, false);
	_tuner_sptr      ->set_justification (0.5f, 0, false);
	_reboot_sptr     ->set_justification (0.5f, 0, false);

	_edit_prog_sptr  ->set_text ("Edit program");
	_edit_bank_sptr  ->set_text ("Banks");
	_edit_layout_sptr->set_text ("Pedal layout");
	_edit_levels_sptr->set_text ("Volume & levels");
	_settings_sptr   ->set_text ("Other settings");
	_tuner_sptr      ->set_text ("Tuner");
	_reboot_sptr     ->set_text ("Restart");
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void	MenuMain::do_connect (Model &model, const View &view, PageMgrInterface &page, Vec2d page_size, void *usr_ptr, const FontSet &fnt)
{
	fstb::unused (usr_ptr);

	_model_ptr = &model;
	_view_ptr  = &view;
	_page_ptr  = &page;
	_page_size = page_size;

	if (_state == State_REBOOT)
	{
		if (_reboot_arg._ok_flag)
		{
#if fstb_IS (SYS, LINUX)
			switch (_reboot_arg._selection)
			{
			case RestartMenu_RESTART:
				_page_switcher.switch_to (
					PageType_END_MSG,
					reinterpret_cast <void *> (EndMsg::EndType_RESTART)
				);
				break;
			case RestartMenu_REBOOT:
				_page_switcher.switch_to (
					PageType_END_MSG,
					reinterpret_cast <void *> (EndMsg::EndType_REBOOT)
				);
				break;
			case RestartMenu_SHUTDOWN:
				_page_switcher.switch_to (
					PageType_END_MSG,
					reinterpret_cast <void *> (EndMsg::EndType_SHUTDOWN)
				);
				break;
			default:
				// Nothing
				break;
			}
#else
			_page_switcher.call_page (PageType_NOT_YET, nullptr, -1);
#endif
		}
	}
	_state = State_NORMAL;

	if (_view_ptr->use_setup ()._save_mode != doc::Setup::SaveMode_AUTO)
	{
		model.set_edit_mode (false);
	}

	_edit_prog_sptr  ->set_font (fnt._m);
	_edit_bank_sptr  ->set_font (fnt._m);
	_edit_layout_sptr->set_font (fnt._m);
	_edit_levels_sptr->set_font (fnt._m);
	_settings_sptr   ->set_font (fnt._m);
	_tuner_sptr      ->set_font (fnt._m);
	_reboot_sptr     ->set_font (fnt._m);

	const int      x_mid =  _page_size [0]      >> 1;
	const int      w_sel = (_page_size [0] * 3) >> 2;
	const int      h_m   = fnt._m.get_char_h ();

	_edit_prog_sptr  ->set_frame (Vec2d (w_sel, 0), Vec2d ());
	_edit_bank_sptr  ->set_frame (Vec2d (w_sel, 0), Vec2d ());
	_edit_layout_sptr->set_frame (Vec2d (w_sel, 0), Vec2d ());
	_edit_levels_sptr->set_frame (Vec2d (w_sel, 0), Vec2d ());
	_settings_sptr   ->set_frame (Vec2d (w_sel, 0), Vec2d ());
	_tuner_sptr      ->set_frame (Vec2d (w_sel, 0), Vec2d ());
	_reboot_sptr     ->set_frame (Vec2d (w_sel, 0), Vec2d ());

	_edit_prog_sptr  ->set_coord (Vec2d (x_mid, h_m * 0));
	_edit_bank_sptr  ->set_coord (Vec2d (x_mid, h_m * 1));
	_edit_layout_sptr->set_coord (Vec2d (x_mid, h_m * 2));
	_edit_levels_sptr->set_coord (Vec2d (x_mid, h_m * 3));
	_settings_sptr   ->set_coord (Vec2d (x_mid, h_m * 4));
	_tuner_sptr      ->set_coord (Vec2d (x_mid, h_m * 5));
	_reboot_sptr     ->set_coord (Vec2d (x_mid, h_m * 6));

	_page_ptr->push_back (_edit_prog_sptr  );
	_page_ptr->push_back (_edit_bank_sptr  );
	_page_ptr->push_back (_edit_layout_sptr);
	_page_ptr->push_back (_edit_levels_sptr);
	_page_ptr->push_back (_settings_sptr   );
	_page_ptr->push_back (_tuner_sptr      );
	_page_ptr->push_back (_reboot_sptr     );

	PageMgrInterface::NavLocList  nav_list (Entry_NBR_ELT);
	for (int pos = 0; pos < int (nav_list.size ()); ++pos)
	{
		NavLoc &       loc = nav_list [pos];
		loc._node_id = pos;
	}
	page.set_nav_layout (nav_list);
	page.jump_to (nav_list [0]._node_id);
}



void	MenuMain::do_disconnect ()
{
	// Nothing
}



MsgHandlerInterface::EvtProp	MenuMain::do_handle_evt (const NodeEvt &evt)
{
	EvtProp        ret_val = EvtProp_PASS;

	if (evt.is_button_ex ())
	{
		const Button   but = evt.get_button_ex ();
		switch (but)
		{
		case Button_S:
			ret_val = EvtProp_CATCH;
			switch (evt.get_target ())
			{
			case Entry_PROG:
				_page_switcher.switch_to (PageType_PROG_EDIT, nullptr);
				break;
			case Entry_BANKS:
				_page_switcher.switch_to (pg::PageType_BANK_MENU, nullptr);
				break;
			case Entry_LAYOUT:
				_pedal_ctx._type     = PedalEditContext::Type_GLOBAL;
				_pedal_ctx._ret_page = PageType_PROG_CUR;
				_page_switcher.switch_to (pg::PageType_PEDALBOARD_CONFIG, nullptr);
				break;
			case Entry_LEVELS:
				_page_switcher.switch_to (PageType_LEVELS, nullptr);
				break;
			case Entry_SETTINGS:
				_page_switcher.switch_to (PageType_SETTINGS_OTHER, nullptr);
				break;
			case Entry_REBOOT:
				_reboot_arg._title      = "RESTART";
				_reboot_arg._ok_flag    = false;
				_reboot_arg._selection  = 0;
				_reboot_arg._choice_arr = { "Cancel", "Restart", "Reboot", "Shutdown" };
				_page_switcher.call_page (PageType_QUESTION, &_reboot_arg);
				_state = State_REBOOT;
				break;
			case Entry_TUNER:
				_model_ptr->set_tuner (true);
				break;
			default:
				ret_val = EvtProp_PASS;
				break;
			}
			break;
		case Button_E:
			_page_switcher.switch_to (pg::PageType_PROG_CUR, nullptr);
			ret_val = EvtProp_CATCH;
			break;
		default:
			// Nothing
			break;
		}
	}

	return ret_val;
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace pg
}  // namespace uitk
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
