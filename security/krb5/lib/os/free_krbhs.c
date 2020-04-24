/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: free_krbhs.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:37  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:03:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:31  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:01:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:17:28  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  free_krbhs.c V=3 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/free_krbhs.c,v $
 * $Author: marty $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * krb5_free_krbhst() function
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_free_krbhs_c[] =
"$Id: free_krbhs.c,v 1.1.4.2 1996/02/18 00:15:14 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

#include <krb5/ext-proto.h>

/*
 Frees the storage taken by a host list returned by krb5_get_krbhst.
 */

krb5_error_code
krb5_free_krbhst(hostlist)
char * const *hostlist;
{
    register char * const *cp;

    for (cp = hostlist; *cp; cp++)
	free(*cp);
    xfree(hostlist);
    return 0;
}
