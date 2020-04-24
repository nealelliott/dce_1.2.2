/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: free_hstrl.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:03:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:30  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:01:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:17:24  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  free_hstrl.c V=2 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/free_hstrl.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * krb5_free_host_realm()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_free_hstrl_c[] =
"$Id: free_hstrl.c,v 1.1.4.2 1996/02/18 00:15:13 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

#include <krb5/ext-proto.h>
#include <stdio.h>
#include <krb5/los-proto.h>

/*
 Frees the storage taken by a realm list returned by krb5_get_local_realm.
 */

krb5_error_code
krb5_free_host_realm(realmlist)
char * const *realmlist;
{
    /* same format, so why duplicate code? */
    return krb5_free_krbhst(realmlist);
}
