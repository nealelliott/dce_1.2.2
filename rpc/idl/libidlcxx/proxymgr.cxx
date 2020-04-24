// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: proxymgr.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:45:17  marty
// 	Add OSF copyright
// 	[1996/03/09  20:24:50  marty]
//
// Revision 1.1.2.1  1995/12/08  00:13:20  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/07  23:55:15  root]
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


#include <pthread.h>
/*suppress pthread_exc, as pthread will be used*/
#define PTHREAD_EXC

#include <dce/rpcexc.h>
#include <dce/objref.hxx>
#include <dce/proxymgr.hxx>
#include <dce/trace.hxx>
#include <dce/hashtab.hxx>

// The proxy creator table
RpcHashTable    *proxyTable = 0;

////////////////////////////////////////////////////////////////////////////////
//
// This is the constructor, which is used by client stubs
// to enter a {interface id, proxy creator function} into the client
// proxy creator table.
//
// The table is created if necessary.
//
// Input:
//	iid 	- interface id
//	creatorFunction	- the proxy creator function
//
// Output:
//	none 
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

ProxyMgr::ProxyMgr(
	const uuid_t & iid,
	ProxyCreator creatorFunction
) {
	Trace("ProxyMgr::ProxyMgr()");

	if (proxyTable == 0)
		proxyTable = new RpcHashTable(DEFAULT_PROXYMGR_TABLE_SIZE);

	(*proxyTable)[iid] = (void *) creatorFunction;

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function retrieves a proxy creator function 
// given an interface id.
//
// Input:
//	iid 	- interface id
//
// Output:
//	ProxyCreator - the proxy creator function
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

ProxyCreator
ProxyMgr::getProxyCreator(
	const uuid_t & iid
) {
	ProxyCreator proxyCreator;

	// make sure there is a proxy table
	if (proxyTable == 0) RAISE(rpc_x_no_client_stub);

	// find the proxy creator for the inteface
	proxyCreator = (ProxyCreator) (void *) (*proxyTable)[iid];
	if (proxyCreator == 0) RAISE(rpc_x_no_client_stub);

	return proxyCreator;
}
