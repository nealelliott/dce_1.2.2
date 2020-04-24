/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpc_auth_stubs.c,v $
 * Revision 1.1.4.2  1996/02/17  23:21:57  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:01  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:09:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:46  root]
 * 
 * Revision 1.1.2.2  1993/01/11  23:39:35  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:12:51  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:36:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/* 
*/ 
/* 
 * Copyright 1991 by Apollo Computer, Inc.
 * 
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of Apollo Computer, Inc not
 * be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * Apollo Computer, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 * 
 *
 *
 *   
 *	Extracted	1/4/92 16:53:52
 *	from delta on	11/8/91 16:52:47
 *
 *  $LOG$
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/rpc/rtandidl/moretests/rpc_auth_stubs.c, , dce_osf, dce.75d";
#endif

#include <tsh_common.h>

/* This file contains stubs for routines not implemented yet. */

/*****************************************************************************

typedef boolean32 (*rpc_mgmt_authorization_fn_t)
	(
	  rpc_binding_handle_t	client_binding,
	  unsigned32		requested_mgmt_operation,
	  unsigned32		*status
	);

*******************************************************************************/

/*****************************************************************************

void rpc_mgmt_set_authorization_fn (authz_fn, status)
  rpc_mgmt_authorization_fn_t authz_fn;
  unsigned32 *status;
{
  *status = rpc_s_ok;
}

*******************************************************************************/

void rpc_if_register_auth_info (if_spec, server_princ_name, authn_level,
				authn_svc, auth_identity, authz_svc, status)
  rpc_if_handle_t if_spec;
  unsigned_char_p_t server_princ_name;
  unsigned32 authn_level;
  unsigned32 authn_svc;
  rpc_auth_identity_handle_t auth_identity;
  unsigned32 authz_svc;
  unsigned32 *status;
{
  *status = rpc_s_ok;
}

