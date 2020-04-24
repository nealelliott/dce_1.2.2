/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mit-des.h,v $
 * Revision 1.1.4.2  1996/02/18  23:02:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:15  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:13  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:00:29  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:53:10  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  mit-des.h V=2 10/23/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/mit-des.h,v $
 * $Author: marty $
 * $Id: mit-des.h,v 1.1.4.2 1996/02/18 23:02:45 marty Exp $
 *
 * Copyright 1989,1990 by the Massachusetts Institute of Technology.
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
 * MIT Data Encryption Standard software implementation declarations.
 */


#ifndef KRB5_MIT_DES__
#define KRB5_MIT_DES__

typedef krb5_octet mit_des_cblock[8];	/* crypto-block size */

/* Key schedule--used internally by DES routines to gain some speed */
typedef struct mit_des_ks_struct {
    mit_des_cblock _;
} mit_des_key_schedule[16];

#define MIT_DES_ENCRYPT	1
#define MIT_DES_DECRYPT	0

typedef struct mit_des_ran_key_seed {
    krb5_octet sequence_number[8];
    mit_des_key_schedule random_sequence_key;
} mit_des_random_key_seed;

/* the first byte of the key is already in the keyblock */

#define MIT_DES_BLOCK_LENGTH 		(8*sizeof(krb5_octet))
#define	MIT_DES_CBC_CRC_PAD_MINIMUM	CRC32_CKSUM_LENGTH
#define MIT_DES_KEYSIZE		 	(8*sizeof(krb5_octet))

#define MIT_DES_CBC_CKSUM_LENGTH	(4*sizeof(krb5_octet))

/* cryptosystem entry descriptor for MIT's DES encryption library */
extern krb5_cryptosystem_entry  mit_des_cryptosystem_entry;
extern krb5_checksum_entry	mit_des_cbc_cksumtable_entry;

#endif /* KRB5_MIT_DES__ */
