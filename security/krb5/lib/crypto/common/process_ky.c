/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: process_ky.c,v $
 * Revision 1.1.6.2  1996/02/18  00:10:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:39  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:48:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:16  root]
 * 
 * Revision 1.1.4.2  1992/12/29  14:22:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:04:36  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  21:00:21  pato
 * 	Initial revision
 * 	[1992/05/14  15:26:14  pato]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
*/

/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/crypto/common/process_ky.c,v $
 * $Author: marty $
 *
 * Derived from:
 * Source: /afs/athena.mit.edu/astaff/project/krb5/src/lib/des/RCS/process_ky.c,v
 * Author: jtkohl
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
 */

#ifdef MODULE_VERSION_ID
static char rcsid_process_ky_c[] =
"$Id: process_ky.c,v 1.1.6.2 1996/02/18 00:10:15 marty Exp $";
#endif

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

#include <krb5/des_int.h>

/*
        does any necessary key preprocessing (such as computing key
                schedules for DES).
        eblock->crypto_entry must be set by the caller; the other elements
        of eblock are to be assigned by this function.
        [in particular, eblock->key must be set by this function if the key
        is needed in raw form by the encryption routine]

        The caller may not move or reallocate "keyblock" before calling
        finish_key on "eblock"

        returns: errors
 */

krb5_error_code mit_des_process_key (DECLARG(krb5_encrypt_block *, eblock),
				     DECLARG(const krb5_keyblock *,keyblock))
OLDDECLARG(krb5_encrypt_block *, eblock)
OLDDECLARG(const krb5_keyblock *,keyblock)
{
    struct mit_des_ks_struct       *schedule;      /* pointer to key schedules */
    
    if (keyblock->length != sizeof (mit_des_cblock))
	return KRB5_BAD_KEYSIZE;

    if ( !(schedule = (struct mit_des_ks_struct *) malloc(sizeof(mit_des_key_schedule))) )
        return ENOMEM;
#define cleanup() { free( (char *) schedule); }

    switch (mit_des_key_sched (keyblock->contents, schedule)) {
    case -1:
	cleanup();
	return KRB5DES_BAD_KEYPAR;

    case -2:
	cleanup();
	return KRB5DES_WEAK_KEY;

    default:
	eblock->key = (krb5_keyblock *) keyblock;
	eblock->priv = (krb5_pointer) schedule;
	return 0;
    }
}
