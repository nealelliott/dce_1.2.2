// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: refcnt.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:45:18  marty
// 	Add OSF copyright
// 	[1996/03/09  20:24:51  marty]
//
// Revision 1.1.2.1  1995/12/08  00:13:21  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/07  23:55:16  root]
// 
// $EndLog$

#include <dce/rpc.h>
#include <dce/stubbase.h>
#include <dce/rpcexc.h>
#include <dce/objref.hxx>
#include <dce/trace.hxx>

/*
**
**  Copyright (c) 1994 by
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**  NAME:
**
**      refcnt.cxx
**
**  FACILITY:
**
**      IDL Stub Runtime Support for Reference Counting
**
**  ABSTRACT:
**
**      Provides support routines for the RPC Reference Counting interface 
**      (refcnt.idl) used with distributed objects to manage memory.
**
**  DESIGN:
**
**	This interface is called by the base class operations 
**	incRef() and decRef() from the base class that all
**	distributed objects are derived from.
*/

extern "C" {

/*
** It may be the case that an incRef() is attempted for an object which
** does not support the interface. This case arises when a client binds
** an existing object to a new interface. The object exists but does not
** support the new interface because the manager object is not derived
** from it in an inheritance situation. The client side of the bind()
** operation will invoke an incRef() on the new interface. If the object
** does not support this interface, then a zero is returned so that the
** client stub can inform the user the the interface is unsupported by the
** object.
*/

idl_ulong_int
incRef( handle_t bh, uuid_t oid, uuid_t iid)
{
	Trace("incRef");

	rpc_object_reference *obj;

	// if the object is found and supports the interface
	// then bump the reference count
	obj = (basecast *) rpc_object_reference::_getThis(bh, iid);
	if (obj)
		return obj->incRef();

	// if the object is found but does not support the interface
	// then return 0 to indicate interface is not supported
	obj = (rpc_object_reference *) rpc_object_reference::get_object(oid);
	if (obj)
		return 0;

	// if the object is not found
	// then raise the exception which will be sent to the client
	RAISE(rpc_x_object_not_found);

        return 0;
}


/*
** This function just decrements the reference count. The decRef() base
** class operation will handle the case when the count reaches zero.
*/

void
decRef( handle_t bh, uuid_t u)
{
	Trace("decRef");

	rpc_object_reference *obj;

	obj = (rpc_object_reference *) rpc_object_reference::get_object(u);
	if (obj)
		obj->decRef();
	return;
}

}
