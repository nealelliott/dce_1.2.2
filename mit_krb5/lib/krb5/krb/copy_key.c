/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: copy_key.c,v $
 * Revision 1.1.2.1  1996/06/05  20:59:16  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:56:47  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/krb/copy_key.c
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
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

#include "k5-int.h"

/*
 * Copy a keyblock, including alloc'ed storage.
 */
krb5_error_code
krb5_copy_keyblock(context, from, to)
    krb5_context context;
    const krb5_keyblock *from;
    krb5_keyblock **to;
{
	krb5_keyblock	*new_key;

	if (!(new_key = (krb5_keyblock *) malloc(sizeof(krb5_keyblock))))
		return ENOMEM;
#ifdef HAVE_C_STRUCTURE_ASSIGNMENT
	*new_key = *from;
#else
	memcpy(new_key, from, sizeof(krb5_keyblock));
#endif
	if (!(new_key->contents = (krb5_octet *)malloc(new_key->length))) {
		krb5_xfree(new_key);
		return(ENOMEM);
	}
	memcpy((char *)new_key->contents, (char *)from->contents,
	       new_key->length);
	*to = new_key;
	return 0;
}
