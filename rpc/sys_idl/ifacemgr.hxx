// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
//
//
// HISTORY
// $Log: ifacemgr.hxx,v $
// Revision 1.1.3.2  1996/03/09  20:45:26  marty
// 	Add OSF copyright
// 	[1996/03/09  20:24:56  marty]
//
// Revision 1.1.3.1  1995/12/08  00:23:16  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/08  00:07:14  root]
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

#ifndef _IFACE_TABLE
#define _IFACE_TABLE

// This file defines an Interface Manager class which allows a server or
// client interface specification to be retrieved given an inteface id.
//
// There is a separate interface table for clients and servers since the
// interface specifications are not quite identical and it is possible to
// link a client and server stub together which necessitates a unique 
// name for the tables. Both tables are indexed by the same interface
// id however.

const int DEFAULT_IFACEMGR_TABLE_SIZE = 10;

////////////////////// server Interface Manager class //////////////////////////

class IfaceMgr {

private:
	rpc_if_handle_t	if_spec;		// interface spec

public:
	// constructor that causes an interface to be stored in the table
	IfaceMgr(const uuid_t &, const rpc_if_handle_t);

	// constructor that causes an interface to be stored and registered
	IfaceMgr(const uuid_t &, const rpc_if_handle_t, const int);

	// destructor that causes an interface to be unregistered
	~IfaceMgr();

	// returns an interface spec given an interface id
	static rpc_if_handle_t IDL_ENTRY getIFSpec(const uuid_t & iid);
};

////////////////////// client Interface Manager class //////////////////////////

class CIfaceMgr {

public:
	// constructor that causes an interface to be stored in the table
	CIfaceMgr(const uuid_t &, const rpc_if_handle_t);

	// returns an interface spec given an interface id
	static rpc_if_handle_t IDL_ENTRY getCIFSpec(const uuid_t & iid);
};

#endif
