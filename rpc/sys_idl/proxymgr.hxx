// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
//
//
// HISTORY
// $Log: proxymgr.hxx,v $
// Revision 1.1.3.2  1996/03/09  20:45:31  marty
// 	Add OSF copyright
// 	[1996/03/09  20:24:59  marty]
//
// Revision 1.1.3.1  1995/12/08  00:23:32  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/08  00:07:17  root]
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

#ifndef _PROXY_TABLE
#define _PROXY_TABLE

// This file defines a Proxy Manager class which allows a proxy creator 
// functions to be retrieved given an inteface id.

const int DEFAULT_PROXYMGR_TABLE_SIZE = 10;

typedef void *(IDL_ENTRY* ProxyCreator)(ObjectRef &);

/////////////////////////// Proxy Manager class ////////////////////////////////

class ProxyMgr {

public:
	ProxyMgr::ProxyMgr(const uuid_t &, ProxyCreator);
	static ProxyCreator getProxyCreator(const uuid_t & iid);
};

#endif

