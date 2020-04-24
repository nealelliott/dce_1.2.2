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
 * $Log: md4glue.c,v $
 * Revision 1.1.2.1  1996/06/05  20:30:22  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:29:29  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/crypto/md4/md4glue.c
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
 * Kerberos glue for MD4 sample implementation.
 */

#include "k5-int.h"
#include "rsa-md4.h"

krb5_error_code
md4_sum_func NPROTOTYPE((krb5_pointer in, size_t in_length,
    krb5_pointer seed, size_t seed_length, krb5_checksum *outcksum));

krb5_error_code
md4_sum_func(in, in_length, seed, seed_length, outcksum)
krb5_pointer in;
size_t in_length;
krb5_pointer seed;
size_t seed_length;
krb5_checksum FAR *outcksum;
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
    0,
    md4_sum_func,
    RSA_MD4_CKSUM_LENGTH,
    1,					/* is collision proof */
    0,					/* doesn't use key */
};
