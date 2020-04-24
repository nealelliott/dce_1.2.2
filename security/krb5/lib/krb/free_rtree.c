/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: free_rtree.c,v $
 * Revision 1.1.8.2  1996/02/18  00:13:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:31  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:58:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:46  root]
 * 
 * Revision 1.1.4.1  1994/05/11  19:21:03  ahop
 * 	remove const from free_realm_tree declaration
 * 	[1994/05/03  15:56:43  ahop]
 * 
 * Revision 1.1.2.2  1992/12/29  15:55:32  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:14:00  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  free_rtree.c V=2 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/free_rtree.c,v $
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
 * krb5_free_realm_tree()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_free_rtree_c[] =
"$Id: free_rtree.c,v 1.1.8.2 1996/02/18 00:13:49 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

void
krb5_free_realm_tree(realms)
krb5_principal *realms;
{
    register krb5_principal *nrealms = realms;
    while (*nrealms) {
	krb5_free_principal(*nrealms);
	nrealms++;
    }
    xfree(realms);
}
