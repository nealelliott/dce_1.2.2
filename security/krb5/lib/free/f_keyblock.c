/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: f_keyblock.c,v $
 * Revision 1.1.4.2  1996/02/18  00:11:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:15  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:52:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:56  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:27:48  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:08:48  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  f_keyblock.c V=3 10/24/91 //littl/prgy/krb5/lib/free
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/free/f_keyblock.c,v $
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
 * krb5_free_keyblock()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_f_keyblock_c [] =
"$Id: f_keyblock.c,v 1.1.4.2 1996/02/18 00:11:54 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

void
krb5_free_keyblock(val)
register krb5_keyblock *val;
{
    if (val->contents) {
	memset((char *)val->contents, 0, val->length);
	xfree(val->contents);
    }
    xfree(val);
    return;
}
