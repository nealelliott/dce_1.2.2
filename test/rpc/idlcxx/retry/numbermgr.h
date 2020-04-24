/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: numbermgr.h,v $
 * Revision 1.1.2.2  1996/03/09  20:49:48  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:07  marty]
 *
 * Revision 1.1.2.1  1995/12/11  19:35:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:18:44  root]
 * 
 * $EndLog$
 */
#ifndef number_i_h
#define number_i_h
#include <iostream.h>
#include <dce/trace.hxx>
#include "number.h"

static doerror = 0;
extern void exit_server();

// Local Implementation of the Number class

class numberMgr : public Number {
    public: 

	numberMgr(idl_long_float x) : n(x) {};

	idl_long_float IDL_STD_STDCALL get(
	) { 
	        Trace("numberMgr get");
		return n;
	}

	void IDL_STD_STDCALL die();
      
    private:
	idl_long_float n;
};

#endif
