/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: nsattrid.h,v $
 * Revision 1.1.4.2  1996/02/18  22:57:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:17  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:23:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:01:12  root]
 * 
 * Revision 1.1.2.1  1994/07/28  18:30:31  mori_m
 * 	CR 11294  Added well-known uuid for codesets attribute
 * 	[1994/07/28  18:19:11  mori_m]
 * 
 * $EndLog$
 */
/*
**
**  NAME:
**
**      nsattrid.h
**
**	This file is used to reference well know UUIDs for RPC
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  This file defines references to well-known UUIDs for RPC.
**  For DCE 1.1 release, it only defines a reference to a code sets attributes.
**  
**
*/

#ifndef _NSATTRID_H
#define _NSATTRID_H

/* 
 * Name service attribute value for lookup code sets attribute 
 */
extern uuid_t *rpc_c_attr_codesets;

#endif
