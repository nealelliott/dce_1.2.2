/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: copy_key.c,v $
 * Revision 1.1.6.2  1996/02/18  00:13:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:18  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:57:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:39  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:54:29  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:30  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  copy_key.c V=3 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/copy_key.c,v $
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
 * krb5_copy_keyblock()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_copy_key_c[] =
"$Id: copy_key.c,v 1.1.6.2 1996/02/18 00:13:35 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

/*
 * Copy a keyblock, including alloc'ed storage.
 */
krb5_error_code
krb5_copy_keyblock(from, to)
const krb5_keyblock *from;
krb5_keyblock **to;
{
	krb5_keyblock	*new_key;

	if (!(new_key = (krb5_keyblock *) malloc(sizeof(krb5_keyblock))))
		return ENOMEM;
	*new_key = *from;
	if (!(new_key->contents = (krb5_octet *)malloc(new_key->length))) {
		xfree(new_key);
		return(ENOMEM);
	}
	memcpy((char *)new_key->contents, (char *)from->contents,
	       new_key->length);
	*to = new_key;
	return 0;
}
