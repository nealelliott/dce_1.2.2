/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_acl_stubs.c,v $
 * Revision 1.1.4.2  1996/02/17  23:25:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:52  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:50:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:01  root]
 * 
 * Revision 1.1.2.2  1992/12/31  17:33:09  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:19  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:22  devrcs
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
 *	Extracted	10/14/91 15:55:55
 *	from delta on	8/5/91 14:45:13
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/sec_acl_stubs.c, security.test, dce_osf";
#endif

#include <tsh_common.h>
#include <dce/aclbase.h>
#include <dce/daclif.h>

/* This file contains stubs for routines not implemented yet. */

void sec_acl_get_access (acl_handle, manager_type, user_obj, group_obj,
			 net_rights, status)

  sec_acl_handle_t acl_handle;
  uuid_t *manager_type;
  sec_id_t *user_obj;
  sec_id_t *group_obj;
  sec_acl_permset_t *net_rights;
  unsigned32 *status;
{
  *status = error_status_ok;
}

/* End sec_acl_stubs.c */

