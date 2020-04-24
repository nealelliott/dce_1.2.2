/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util_binding.h,v $
 * Revision 1.1.4.2  1996/02/18  23:31:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:24:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:58:12  root]
 * 
 * Revision 1.1.2.2  1994/08/24  19:20:39  pwang
 * 	Added DCP_BINDING_MAX_BYTES_PER_LINE [OT#11575]
 * 	[1994/08/24  19:19:29  pwang]
 * 
 * Revision 1.1.2.1  1994/03/28  23:00:29  melman
 * 	Added rpcentry show and binding utilities
 * 	[1994/03/28  22:54:59  melman]
 * 
 * $EndLog$
 */
/*
 * MODULE: util_binding.h
 *
 * DESCRIPTION:
 *   This module contains prototypes for routines that manipulate 
 *   string bindings and bindings in tcl syntax.
 */

/* INCLUDE FILES  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dce/rpc.h>
#include <dce/dce.h>
#include <dce/dcedcpmsg.h>

/* DECLARATIONS */
#define DCP_BINDING_MAX_BYTES_PER_LINE 20

/* PROTOTYPES */
extern char *dcp_binding_string_to_tcl(char *, unsigned32 *);
extern char *dcp_binding_tcl_to_string(int, char **, unsigned32 *);
extern char *dcp_binding_to_bytes(twr_t*, char*, error_status_t*);
