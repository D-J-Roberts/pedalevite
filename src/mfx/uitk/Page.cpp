/*****************************************************************************

        Page.cpp
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

#include "fstb/fnc.h"
#include "mfx/ui/DisplayInterface.h"
#include "mfx/uitk/NodeEvt.h"
#include "mfx/uitk/Page.h"
#include "mfx/uitk/PageInterface.h"
#include "mfx/uitk/PageMgrInterface.h"
#include "mfx/Cst.h"
#include "mfx/Model.h"
#include "mfx/View.h"

#include <cassert>
#include <cstring>



namespace mfx
{
namespace uitk
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



Page::Page (Model &model, View &view, ui::DisplayInterface &display, ui::UserInputInterface::MsgQueue &queue_input_to_gui, ui::UserInputInterface &input_device, const ui::Font &fnt_t, const ui::Font &fnt_s, const ui::Font &fnt_m, const ui::Font &fnt_l)
:	_model (model)
,	_view (view)
,	_display (display)
,	_queue_input_to_gui (queue_input_to_gui)
,	_input_device (input_device)
,	_fnt_set (fnt_t, fnt_s, fnt_m, fnt_l)
,	_disp_size (display.get_width (), display.get_height ())
,	_screen (_disp_node_id)
,	_zone_inval (Vec2d (), _disp_size)
,	_content_ptr (nullptr)
,	_nav_list ()
,	_curs_pos (-1)
,	_curs_id (-1)
,	_timer_set ()
,	_timer_mod_flag (false)
,	_but_hold (Button_INVALID)
,	_but_hold_date (INT64_MIN)
,	_but_hold_count (0)
,	_rec_spc ()
,	_recursive_flag (false)
,	_first_refresh_date (INT64_MIN)
{
	_screen.set_coord (Vec2d ());
	_screen.set_size (_disp_size, _disp_size);
	_screen.notify_attachment (this);
}



Page::~Page ()
{
	clear (false);
}



void	Page::set_page_content (PageInterface &content, void *usr_ptr)
{
	_rec_spc.push_back ({ &content, usr_ptr });

	if (! _recursive_flag)
	{
		_recursive_flag = true;

		while (! _rec_spc.empty ())
		{
			const SetPageContent &	spc = _rec_spc.front ();

			clear ();

			_content_ptr = spc._content_ptr;
			_content_ptr->connect (
				_model, _view, *this, _disp_size, spc._usr_ptr, _fnt_set
			);
			_view.add_observer (*_content_ptr);
			_screen.invalidate_all ();

			check_curs (true);

			_rec_spc.pop_front ();

			// Force several refreshing at the beginning
			if (_first_refresh_date.count () == INT64_MIN)
			{
				_first_refresh_date = _model.get_cur_date ();
			}
		}

		_recursive_flag = false;
	}
}



const PageInterface *	Page::get_page_content () const
{
	return _content_ptr;
}



// -1: no cursor or position not known
int	Page::get_cursor_node () const
{
	return (_curs_id);
}



void	Page::clear (bool evt_flag)
{
	_nav_list.clear ();
	_timer_set.clear ();

	if (_content_ptr != nullptr)
	{
		if (_curs_id >= 0 && evt_flag)
		{
			NodeEvt        evt (NodeEvt::create_cursor (_curs_id, NodeEvt::Curs_LEAVE));
			send_event (evt);
		}
		_curs_pos = -1;
		_curs_id  = -1;

		_view.remove_observer (*_content_ptr);
		_content_ptr->disconnect ();
		_content_ptr = nullptr;
	}

	const int      nbr_nodes = _screen.get_nbr_nodes ();
	for (int pos = nbr_nodes - 1; pos >= 0; --pos)
	{
		_screen.erase (pos);
	}
}



bool	Page::process_messages ()
{
	// Messages from the user input
	bool           proc_flag = process_input ();

	// Redraw
	handle_redraw ();

	_timer_mod_flag = false;
	auto           it_nid = _timer_set.begin ();
	while (! _timer_mod_flag && it_nid != _timer_set.end ())
	{
		const int      node_id = *it_nid;
		++ it_nid;
		NodeEvt        evt (NodeEvt::create_timer (node_id));
		send_event (evt);
	}

	return proc_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void	Page::do_set_nav_layout (const NavLocList &nav_list)
{
	_nav_list = nav_list;
	check_curs (true);
}



void	Page::do_jump_to (int node_id)
{
	if (_curs_id >= 0)
	{
		NodeEvt        evt (NodeEvt::create_cursor (_curs_id, NodeEvt::Curs_LEAVE));
		send_event (evt);
	}

	_curs_pos = find_nav_node (node_id);
	if (_curs_pos < 0)
	{
		_curs_id = -1;
	}
	else
	{
		_curs_id = node_id;
		NodeEvt        evt (NodeEvt::create_cursor (_curs_id, NodeEvt::Curs_ENTER));
		send_event (evt);
	}
}



void	Page::do_set_timer (int node_id, bool enable_flag)
{
	if (enable_flag)
	{
		const bool     ins_flag = _timer_set.insert (node_id).second;
		_timer_mod_flag |= ins_flag;
	}
	else
	{
		auto        it = _timer_set.find (node_id);
		if (it != _timer_set.end ())
		{
			_timer_set.erase (it);
			_timer_mod_flag = true;
		}
	}
}



void	Page::do_reset_display ()
{
	_display.force_reset ();
}



Vec2d	Page::do_get_coord_abs () const
{
	return Vec2d ();
}



void	Page::do_invalidate (const Rect &zone)
{
	// The simplest way to do this
	_zone_inval.merge (zone);
}



void	Page::do_push_back (NodeSPtr node_sptr)
{
	_screen.push_back (node_sptr);
}



void	Page::do_set_node (int pos, NodeSPtr node_sptr)
{
	_screen.set_node (pos, node_sptr);
}



void	Page::do_insert (int pos, NodeSPtr node_sptr)
{
	_screen.insert (pos, node_sptr);
}



void	Page::do_erase (int pos)
{
	_screen.erase (pos);
}



int	Page::do_get_nbr_nodes () const
{
	return _screen.get_nbr_nodes ();
}



ContainerInterface::NodeSPtr	Page::do_use_node (int pos)
{
	return _screen.use_node (pos);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



bool	Page::process_input ()
{
	bool           proc_flag       = false;
	bool           check_hold_flag = true;

	ui::UserInputInterface::MsgCell * cell_ptr = nullptr;
	do
	{
		cell_ptr = _queue_input_to_gui.dequeue ();
		if (cell_ptr != nullptr)
		{
			proc_flag = true;

			const ui::UserInputType type  = cell_ptr->_val.get_type ();
			const int               index = cell_ptr->_val.get_index ();
			const float             val   = cell_ptr->_val.get_val ();
//			const int64_t           date  = cell_ptr->_val.get_date ();

			int            node_id = _screen.get_id ();
			if (_curs_id >= 0)
			{
				node_id = _curs_id;
			}

			switch (type)
			{
			case ui::UserInputType_SW:
				{
					Button         but = Button_INVALID;
					switch (index)
					{
					/*** To do: a better map ***/
					case  0: but = Button_S; break;
					case  1: but = Button_E; break;
					case 10: but = Button_U; break;
					case 11: but = Button_D; break;
					case 12: but = Button_L; break;
					case 13: but = Button_R; break;
					case 18: but = Button_S; break;
					case 19: but = Button_S; break;
					}
					if (but != Button_INVALID)
					{
						if (val < 0.5f)
						{
							_but_hold = Button_INVALID;
						}
						else
						{
							_but_hold       = but;
							_but_hold_date  = _model.get_cur_date ();
							_but_hold_count = 0;
							send_button (node_id, but);
						}
						check_hold_flag = false;
					}
				}
				break;
			case ui::UserInputType_ROTENC:
				{
					Button         but     = Button_INVALID;
					const int      val_lim =
						fstb::limit (fstb::round_int (val), -1, 1);
					switch (index * val_lim)
					{
					case -Cst::RotEnc_NAV_VER: but = Button_U; break;
					case  Cst::RotEnc_NAV_VER: but = Button_D; break;
					case -Cst::RotEnc_NAV_HOR: but = Button_L; break;
					case  Cst::RotEnc_NAV_HOR: but = Button_R; break;
					}
					if (but != Button_INVALID)
					{
						send_button (node_id, but);
					}
				}
				break;
			default:
				// Nothing
				break;
			}

			_input_device.return_cell (*cell_ptr);
		}
	}
	while (cell_ptr != nullptr);

	// Key repeat
	if (check_hold_flag && _but_hold != Button_INVALID)
	{
		const std::chrono::microseconds  cur_date (_model.get_cur_date ());
		const std::chrono::microseconds  dist (cur_date - _but_hold_date);
		const int      count    = int (
			  (dist - Cst::_key_time_hold + Cst::_key_time_repeat).count ()
			/ Cst::_key_time_repeat.count ()
		);
		if (count > _but_hold_count)
		{
			const bool     pass_flag = process_nav (_but_hold);
			if (pass_flag)
			{
				int            node_id = _screen.get_id ();
				if (_curs_id >= 0)
				{
					node_id = _curs_id;
				}
				NodeEvt        evt (NodeEvt::create_button (node_id, _but_hold));
				send_event (evt);
			}

			if (_but_hold_count + 2 >= count)
			{
				++ _but_hold_count;
			}
			else
			{
				_but_hold_count = count;
			}
		}
	}

	return proc_flag;
}



void	Page::handle_redraw ()
{
	// Forced refresh
	if (_first_refresh_date.count () != INT64_MIN)
	{
		const std::chrono::microseconds  cur_time (_model.get_cur_date ());
		const std::chrono::microseconds  dist (cur_time - _first_refresh_date);
		if (dist < std::chrono::seconds (1))
		{
			_zone_inval = Rect (Vec2d (), _disp_size);
		}
	}

	// Clips the zone to the physical boundaries of the screen
	_zone_inval.intersect (Rect (Vec2d (), _disp_size));

	if (! _zone_inval.is_empty ())
	{
		// Clears the background
		const int      stride  = _display.get_stride ();
		uint8_t *      buf_ptr = _display.use_screen_buf ();
		const Vec2d    z_size  = _zone_inval.get_size ();
		buf_ptr += _zone_inval [0] [1] * stride + _zone_inval [0] [0];
		for (int y = 0; y < z_size [1]; ++y)
		{
			memset (buf_ptr, 0, z_size [0]);
			buf_ptr += stride;
		}

		// Actual redraw
		_screen.redraw (_display, _zone_inval, Vec2d ());

		// To the display
		_display.refresh (
			_zone_inval [0] [0], _zone_inval [0] [1],
			z_size [0], z_size [1]
		);
	}

	_zone_inval = Rect ();
}



void	Page::send_button (int node_id, Button but)
{
	const bool     pass_flag = process_nav (but);
	if (pass_flag)
	{
		NodeEvt        evt (NodeEvt::create_button (node_id, but));
		send_event (evt);
	}
}



void	Page::send_event (const NodeEvt &evt)
{
	MsgHandlerInterface::EvtProp  propag = _content_ptr->handle_evt (evt);
	if (propag == MsgHandlerInterface::EvtProp_PASS)
	{
		propag = _screen.handle_evt (evt);
	}
}



// -1 if not found
int	Page::find_nav_node (int node_id) const
{
	int            found_pos = -1;
	const int      nbr_nodes = int (_nav_list.size ());
	for (int pos = 0; pos < nbr_nodes && found_pos < 0; ++pos)
	{
		const NavLoc & loc = _nav_list [pos];
		if (loc._node_id == node_id)
		{
			found_pos = pos;
		}
	}

	return found_pos;
}



bool	Page::check_curs (bool send_msg_flag)
{
	bool           move_flag = false;

	if (_curs_id >= 0)
	{
		_curs_pos = find_nav_node (_curs_id);
		if (_curs_pos < 0)
		{
			_curs_id = -1;
		}
	}
	if (_curs_id < 0 && ! _nav_list.empty ())
	{
		_curs_pos = 0;
		const NavLoc & loc = _nav_list [_curs_pos];
		_curs_id  = loc._node_id;
		move_flag     = true;
		send_msg_flag = true;
	}

	if (send_msg_flag && _curs_id >= 0)
	{
		NodeEvt        evt (NodeEvt::create_cursor (_curs_id, NodeEvt::Curs_ENTER));
		send_event (evt);
	}

	return move_flag;
}



bool	Page::process_nav (Button but)
{
	bool           pass_flag = false;

	if (check_curs (false) || _nav_list.empty ())
	{
		pass_flag = true;
	}
	else
	{
		assert (_curs_pos >= 0);

		const NavLoc & loc = _nav_list [_curs_pos];
		int         jump_id   = loc._action_arr [but];
		bool        jump_flag = true;
		int         jump_pos  = -1;
		if (jump_id >= 0)
		{
			jump_pos = find_nav_node (jump_id);
			assert (jump_pos >= 0);
		}
		else if (jump_id == NavLoc::OvrAction_STOP)
		{
			jump_flag = false;
		}
		else if (jump_id == NavLoc::OvrAction_DEFAULT)
		{
			const int      nbr_nav = int (_nav_list.size ());
			if (but == Button_U)
			{
				jump_pos = (_curs_pos + nbr_nav - 1) % nbr_nav;
				jump_id  = _nav_list [jump_pos]._node_id;
			}
			else if (but == Button_D)
			{
				jump_pos = (_curs_pos           + 1) % nbr_nav;
				jump_id  = _nav_list [jump_pos]._node_id;
			}
			else
			{
				jump_id = NavLoc::OvrAction_PASS;
			}
		}

		if (jump_id == NavLoc::OvrAction_PASS)
		{
			jump_flag = false;
			pass_flag = true;
		}

		if (jump_flag)
		{
			assert (jump_id  >= 0);
			assert (jump_pos >= 0);

			NodeEvt        evt (NodeEvt::create_cursor (_curs_id, NodeEvt::Curs_LEAVE));
			send_event (evt);

			_curs_id  = jump_id;
			_curs_pos = jump_pos;
			evt = NodeEvt::create_cursor (_curs_id, NodeEvt::Curs_ENTER);
			send_event (evt);
		}
	}

	return pass_flag;
}



}  // namespace uitk
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
