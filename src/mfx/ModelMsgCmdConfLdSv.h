/*****************************************************************************

        ModelMsgCmdConfLdSv.h
        Author: Laurent de Soras, 2019

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_ModelMsgCmdConfLdSv_HEADER_INCLUDED)
#define mfx_ModelMsgCmdConfLdSv_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/ModelMsgCmdInterface.h"

#include <string>


namespace mfx
{



class ModelMsgCmdCbInterface;

class ModelMsgCmdConfLdSv final
:	public ModelMsgCmdInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	enum Type
	{
		Type_LOAD = 0,
		Type_SAVE,

		Type_NBR_ELT
	};

	explicit       ModelMsgCmdConfLdSv (Type type, std::string pathname, ModelMsgCmdCbInterface *cb_ptr);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

	// ModelMsgCmdInterface
	void           do_process (Model &model) final;
	void           do_clear () final;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	Type           _type;
	std::string    _pathname;
	ModelMsgCmdCbInterface *   // Can be 0.
	               _cb_ptr;
	int            _ret_val;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               ModelMsgCmdConfLdSv ()                               = delete;
	               ModelMsgCmdConfLdSv (const ModelMsgCmdConfLdSv &other) = delete;
	               ModelMsgCmdConfLdSv (ModelMsgCmdConfLdSv &&other)    = delete;
	ModelMsgCmdConfLdSv &
	               operator = (const ModelMsgCmdConfLdSv &other)        = delete;
	ModelMsgCmdConfLdSv &
	               operator = (ModelMsgCmdConfLdSv &&other)             = delete;
	bool           operator == (const ModelMsgCmdConfLdSv &other) const = delete;
	bool           operator != (const ModelMsgCmdConfLdSv &other) const = delete;

}; // class ModelMsgCmdConfLdSv



}  // namespace mfx



//#include "mfx/ModelMsgCmdConfLdSv.hpp"



#endif   // mfx_ModelMsgCmdConfLdSv_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
