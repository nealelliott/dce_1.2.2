// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: ifacemgr.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:45:06  marty
// 	Add OSF copyright
// 	[1996/03/09  20:24:44  marty]
//
// Revision 1.1.2.1  1995/12/08  00:13:12  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/07  23:55:12  root]
// 
// $EndLog$
//
//  Copyright (c) Digital Equipment Corporation, 1995
//  All Rights Reserved.  Unpublished rights reserved
//  under the copyright laws of the United States.
//  
//  The software contained on this media is proprietary
//  to and embodies the confidential technology of 
//  Digital Equipment Corporation.  Possession, use,
//  duplication or dissemination of the software and
//  media is authorized only pursuant to a valid written
//  license from Digital Equipment Corporation.
//
//  RESTRICTED RIGHTS LEGEND   Use, duplication, or 
//  disclosure by the U.S. Government is subject to
//  restrictions as set forth in Subparagraph (c)(1)(ii)
//  of DFARS 252.227-7013, or in FAR 52.227-19, as
//  applicable.
//

#include <dce/rpc.h>
#include <dce/ifacemgr.hxx>
#include <dce/trace.hxx>
#include <pthread.h>
#include <dce/hashtab.hxx>

// The interface tables
RpcHashTable    *ifaceTable  = 0;		// server interface table
RpcHashTable    *CifaceTable = 0;		// client interface table

////////////////////////////////////////////////////////////////////////////////
//
// This is the constructor, which is used by server stubs
// to enter a {interface id, interface spec} into the server
// interface table.
//
// The table is created if necessary.
//
// Input:
//	iid 	- interface id
//	ifspec	- server interface specification
//
// Output:
//	none 
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

IfaceMgr::IfaceMgr(
	const uuid_t & iid,
	const rpc_if_handle_t ifspec
) {
	Trace("IfaceMgr::IfaceMgr()");

	if (ifaceTable == 0)
		ifaceTable = new RpcHashTable(DEFAULT_IFACEMGR_TABLE_SIZE);

	(*ifaceTable)[iid] = (void *) ifspec;

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This is another constructor, which is used by server stubs
// to enter a {interface id, interface spec} into the server
// interface table and also register the server interface.
//
// The table is created if necessary.
//
// Input:
//	iid 	- interface id
//	ifspec	- server interface specification
//
// Output:
//	none 
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

IfaceMgr::IfaceMgr(
	const uuid_t & iid,
	const rpc_if_handle_t ifspec,
	const int dummy
) {
	Trace("IfaceMgr::IfaceMgr()");

	error_status_t	st;

	if (ifaceTable == 0)
		ifaceTable = new RpcHashTable(DEFAULT_IFACEMGR_TABLE_SIZE);

	(*ifaceTable)[iid] = (void *) ifspec;

	// register the interface
	rpc_server_register_if(
		ifspec,
		NULL,
		NULL,
		&st
	);
	if (st != rpc_s_ok && st != rpc_s_type_already_registered)
		exc_raise_status(st);

	if_spec = ifspec;

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This is the destructor, which unregisters an interface.
//
// Input:
//	none
//
// Output:
//	none 
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

IfaceMgr::~IfaceMgr(
) {
	Trace("IfaceMgr::~IfaceMgr()");

	error_status_t	st;

	// unregister the interface
	rpc_server_unregister_if(
		if_spec,
		NULL,
		&st
	);
	if (st != rpc_s_ok) exc_raise_status(st);

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function retrieves a server interface specification 
// given an interface id.
//
// Input:
//	iid 	- interface id
//
// Output:
//	if_handle_t - interface specification
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

rpc_if_handle_t
IfaceMgr::getIFSpec(
	const uuid_t & iid
) {
	// make sure there is an interface table
	if (ifaceTable == 0) return 0;

	// find the ifspec for the inteface
	return (rpc_if_handle_t) (void *) (*ifaceTable)[iid];
}

////////////////////////////////////////////////////////////////////////////////
//
// This is the constructor, which is used by client stubs
// to enter a {interface id, interface spec} into the client
// interface table.
//
// The table is created if necessary.
//
// Input:
//	u 	- interface id
//	ifspec	- server interface specification
//
// Output:
//	none 
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

CIfaceMgr::CIfaceMgr(
	const uuid_t & iid,
	const rpc_if_handle_t ifspec
) {
	Trace("CIfaceMgr::CIfaceMgr()");

	if (CifaceTable == 0)
		CifaceTable = new RpcHashTable(DEFAULT_IFACEMGR_TABLE_SIZE);

	(*CifaceTable)[iid] = (void *) ifspec;

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function retrieves a client interface specification 
// given an interface id.
//
// Input:
//	iid 	- interface id
//
// Output:
//	if_handle_t - interface specification
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

rpc_if_handle_t
CIfaceMgr::getCIFSpec(
	const uuid_t & iid
) {
	// make sure there is an interface table
	if (CifaceTable == 0) return 0;

	// find the ifspec for the inteface
	return (rpc_if_handle_t) (void *) (*CifaceTable)[iid];
}
