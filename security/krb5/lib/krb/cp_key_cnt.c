/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cp_key_cnt.c,v $
 * Revision 1.1.6.2  1996/02/18  00:13:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:23  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:58:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:42  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:54:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:40  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  cp_key_cnt.c V=1 06/06/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/cp_key_cnt.c,v $
 * $Author: marty $
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 * krb5_copy_keyblock()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_copy_key_cnt_c[] =
"$Id: cp_key_cnt.c,v 1.1.6.2 1996/02/18 00:13:40 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

/*
 * Copy a keyblock, including alloc'ed storage.
 */
krb5_error_code
krb5_copy_keyblock_contents(from, to)
const krb5_keyblock *from;
krb5_keyblock *to;
{
    *to = *from;
    to->contents = (krb5_octet *)malloc(to->length);
    if (!to->contents)
	return ENOMEM;
    memcpy((char *)to->contents, (char *)from->contents, to->length);
    return 0;
}
