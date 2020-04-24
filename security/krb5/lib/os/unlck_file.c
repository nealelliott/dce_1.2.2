/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: unlck_file.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:43  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:31:32  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:19:03  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  unlck_file.c V=2 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/unlck_file.c,v $
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
 * libos: krb5_lock_file routine
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_unlock_file_c [] =
"$Id: unlck_file.c,v 1.1.4.2 1996/02/18 00:15:52 marty Exp $";
#endif	/* !lint & !SABER */

#include <stdio.h>

#include <krb5/krb5.h>
#include <krb5/libos.h>
#include <krb5/los-proto.h>

krb5_error_code
krb5_unlock_file(filep, pathname)
FILE *filep;
char *pathname;
{
    return krb5_lock_file(filep, pathname, KRB5_LOCKMODE_UNLOCK);
}
