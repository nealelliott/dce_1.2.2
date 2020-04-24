/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: md4glue.c,v $
 * Revision 1.1.4.2  1996/02/18  00:16:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:22  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:57  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:32:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:20:29  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:53:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  md4glue.c V=2 10/24/91 //littl/prgy/krb5/lib/rsa-md4
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/rsa-md4/md4glue.c,v $
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
 * Kerberos glue for MD4 sample implementation.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_md4glue_c[] =
"$Id: md4glue.c,v 1.1.4.2 1996/02/18 00:16:12 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/rsa-md4.h>

static krb5_error_code
md4_sum_func(in, in_length, seed, seed_length, outcksum)
krb5_pointer in;
size_t in_length;
krb5_pointer seed;
size_t seed_length;
krb5_checksum *outcksum;
{
    krb5_octet *input = (krb5_octet *)in;
    MD4_CTX working;

    MD4Init(&working);
    MD4Update(&working, input, in_length);
    MD4Final(&working);

    outcksum->checksum_type = CKSUMTYPE_RSA_MD4;
    outcksum->length = RSA_MD4_CKSUM_LENGTH;

    memcpy((char *)outcksum->contents, (char *)&working.digest[0], 16);

    memset((char *)&working, 0, sizeof(working));
    return 0;
}

krb5_checksum_entry rsa_md4_cksumtable_entry = {
    md4_sum_func,
    RSA_MD4_CKSUM_LENGTH,
    1,					/* is collision proof */
    0,					/* doesn't use key */
};
