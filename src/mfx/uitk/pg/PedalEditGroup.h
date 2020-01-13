/*****************************************************************************

        PedalEditGroup.h
        Author: Laurent de Soras, 2017

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_uitk_pg_PedalEditGroup_HEADER_INCLUDED)
#define mfx_uitk_pg_PedalEditGroup_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/uitk/pg/PedalEditContext.h"
#include "mfx/uitk/NText.h"
#include "mfx/uitk/PageInterface.h"
#include "mfx/uitk/PageMgrInterface.h"

#include <array>
#include <memory>



namespace mfx
{
namespace uitk
{

class PageSwitcher;

namespace pg
{



class PedalEditGroup final
:	public PageInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	               PedalEditGroup (PageSwitcher &page_switcher, PedalEditContext &ctx);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

	// mfx::uitk::PageInterface
	void           do_connect (Model &model, const View &view, PageMgrInterface &page, Vec2d page_size, void *usr_ptr, const FontSet &fnt) final;
	void           do_disconnect () final;

	// mfx::uitk::MsgHandlerInterface via mfx::uitk::PageInterface
	EvtProp        do_handle_evt (const NodeEvt &evt) final;

	// mfx::ModelObserverInterface via mfx::uitk::PageInterface
	void           do_set_pedalboard_layout (const doc::PedalboardLayout &layout) final;
	void           do_set_pedal (const PedalLoc &loc, const doc::PedalActionGroup &content) final;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	enum Entry
	{
		Entry_TITLE = 1000,

		Entry_TRIG_BEG,
		Entry_PRESS = Entry_TRIG_BEG,
		Entry_HOLD,
		Entry_RELEASE,

		Entry_NBR_ELT
	};

	typedef std::shared_ptr <NText> TxtSPtr;
	typedef std::array <TxtSPtr, doc::ActionTrigger_NBR_ELT> TrigArray;

	const doc::PedalboardLayout &
	               use_layout () const;
	void           update_display ();

	PageSwitcher & _page_switcher;
	PedalEditContext &
	               _ctx;
	Model *        _model_ptr;    // 0 = not connected
	const View *   _view_ptr;     // 0 = not connected
	PageMgrInterface *            // 0 = not connected
	               _page_ptr;
	Vec2d          _page_size;
	const ui::Font *              // 0 = not connected
	               _fnt_ptr;

	TxtSPtr        _title_sptr;
	TrigArray      _trig_sptr_arr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               PedalEditGroup ()                               = delete;
	               PedalEditGroup (const PedalEditGroup &other)    = delete;
	               PedalEditGroup (PedalEditGroup &&other)         = delete;
	PedalEditGroup &
	               operator = (const PedalEditGroup &other)        = delete;
	PedalEditGroup &
	               operator = (PedalEditGroup &&other)             = delete;
	bool           operator == (const PedalEditGroup &other) const = delete;
	bool           operator != (const PedalEditGroup &other) const = delete;

}; // class PedalEditGroup



}  // namespace pg
}  // namespace uitk
}  // namespace mfx



//#include "mfx/uitk/pg/PedalEditGroup.hpp"



#endif   // mfx_uitk_pg_PedalEditGroup_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
