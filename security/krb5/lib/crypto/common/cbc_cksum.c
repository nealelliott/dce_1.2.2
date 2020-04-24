/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cbc_cksum.c,v $
 * Revision 1.1.6.2  1996/02/18  00:09:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:33  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:48:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:09  root]
 * 
 * Revision 1.1.4.2  1992/12/29  14:21:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:03:59  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  20:56:33  pato
 * 	Initial revision
 * 	[1992/05/14  15:21:57  pato]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/crypto/common/cbc_cksum.c,v $
 * $Author: marty $
 *
 * Derived from:
 * Source: /afs/athena.mit.edu/astaff/project/krb5/src/lib/des/RCS/cbc_cksum.c,v
 * Author: jtkohl
 *
 * Copyright 1985, 1986, 1987, 1988, 1990 by the Massachusetts Institute
 * of Technology.
 * All Rights Reserved.
 *
 * Under U.S. law, this software may not be exported outside the US
 * without license from the U.S. Commerce department.
 *
 * These routines form the library interface to the DES facilities.
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
 */

#ifdef MODULE_VERSION_ID
static char rcsid_cbc_cksum_c[] =
"$Id: cbc_cksum.c,v 1.1.6.2 1996/02/18 00:09:55 marty Exp $";
#endif

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

#include <krb5/des_int.h>

/*
	produces cbc cheksum of sequence "in" of the length "in_length" 
	with the help of key "key" of size "key_size" (which should be 8);
	fills out krb5_checksum structure.

	caller is responsible for freeing "contents" element in 
	krb5_checksum structure.

	returns: errors
*/
krb5_error_code mit_des_cbc_checksum(DECLARG(krb5_pointer, in),
				     DECLARG(size_t, in_length),
				     DECLARG(krb5_pointer, key),
				     DECLARG(size_t, key_size),
				     DECLARG(krb5_checksum *, cksum))
OLDDECLARG(krb5_pointer, in)
OLDDECLARG(size_t, in_length)
OLDDECLARG(krb5_pointer, key)
OLDDECLARG(size_t, key_size)
OLDDECLARG(krb5_checksum *, cksum)
{
    struct mit_des_ks_struct       *schedule;      /* pointer to key schedules */
    krb5_octet 	*contents;

    if (key_size != sizeof(mit_des_cblock))
	return KRB5_BAD_KEYSIZE;

    if (!(schedule = (struct mit_des_ks_struct *) malloc(sizeof(mit_des_key_schedule))))
        return ENOMEM;

#define cleanup() { memset((char *)schedule, 0, sizeof(mit_des_key_schedule));\
		    free( (char *) schedule); }

    switch (mit_des_key_sched ((krb5_octet *)key, schedule)) {
    case -1:
        cleanup();
        return KRB5DES_BAD_KEYPAR;

    case -2:
        cleanup();
        return KRB5DES_WEAK_KEY;

    default:
        ;
    }

    if (!(contents = (krb5_octet *) malloc(sizeof(mit_des_cblock)))) {
	cleanup();
        return ENOMEM;
    }

    mit_des_cbc_cksum((krb5_octet *)in, contents, in_length,
		  schedule, (krb5_octet *)key);

    cksum->checksum_type = CKSUMTYPE_DESCBC;
    cksum->length = sizeof(mit_des_cblock);
    cksum->contents = contents;
    cleanup();

    return 0;
}
    
