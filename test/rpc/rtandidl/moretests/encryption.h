/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: encryption.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:25  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:04:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:06  root]
 * 
 * Revision 1.1.2.2  1993/01/11  22:13:20  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:05:14  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:34:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  encryption.h V=4 12/15/90 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/test/rpc/rtandidl/moretests/encryption.h,v $
 * $Author: marty $
 * $Id: encryption.h,v 1.1.4.2 1996/02/18 23:06:17 marty Exp $
 *
 * Copyright 1989,1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * Encryption interface-related declarations
 */

#include <krb5/copyright.h>

#ifndef KRB5_ENCRYPTION__
#define KRB5_ENCRYPTION__

typedef struct _krb5_keyblock {
    krb5_keytype keytype;
    int length;
    krb5_octet *contents;
} krb5_keyblock;

typedef struct _krb5_checksum {
    krb5_cksumtype checksum_type;	/* checksum type */
    int length;
    krb5_octet *contents;
} krb5_checksum;

typedef struct _krb5_encrypt_block {
    struct _krb5_cryptosystem_entry *crypto_entry;
    krb5_keyblock *key;
    krb5_pointer priv;			/* for private use, e.g. DES
					   key schedules */
} krb5_encrypt_block;

typedef struct _krb5_enc_data {
    krb5_enctype etype;
    krb5_kvno kvno;
    krb5_data ciphertext;
} krb5_enc_data;

#include <krb5/widen.h>

/* could be used in a table to find an etype and initialize a block */
typedef struct _krb5_cryptosystem_entry {
    krb5_error_code (*encrypt_func) PROTOTYPE((krb5_const_pointer /* in */,
					       krb5_pointer /* out */,
					       const size_t,
					       krb5_encrypt_block *,
					       krb5_pointer));
    krb5_error_code (*decrypt_func) PROTOTYPE((krb5_const_pointer /* in */,
					       krb5_pointer /* out */,
					       const size_t,
					       krb5_encrypt_block *,
					       krb5_pointer));
    krb5_error_code (*process_key) PROTOTYPE((krb5_encrypt_block *,
					      const krb5_keyblock *));
    krb5_error_code (*finish_key) PROTOTYPE((krb5_encrypt_block *));
    krb5_error_code (*string_to_key) PROTOTYPE((const krb5_keytype,
						krb5_keyblock *,
						const krb5_data *,
 	                                        const krb5_data *));
    krb5_error_code  (*init_random_key) PROTOTYPE((const krb5_keyblock *,
						   krb5_pointer *));
    krb5_error_code  (*finish_random_key) PROTOTYPE((krb5_pointer *));
    krb5_error_code (*random_key) PROTOTYPE((krb5_pointer,
					     krb5_keyblock **));
    int block_length;
    int pad_minimum;			/* needed for cksum size computation */
    int keysize;
    krb5_enctype proto_enctype;		/* encryption type,
					   (assigned protocol number AND
					    table index) */
    krb5_keytype proto_keytype;		/* key type,
					   (assigned protocol number AND
					    table index) */
} krb5_cryptosystem_entry;

#include <krb5/narrow.h>

typedef struct _krb5_cs_table_entry {
    krb5_cryptosystem_entry *system;
    krb5_pointer random_sequence;	/* from init_random_key() */
} krb5_cs_table_entry;

/* could be used in a table to find a sumtype */
typedef struct _krb5_checksum_entry {
    krb5_error_code  (*sum_func) PROTOTYPE ((krb5_pointer /* in */,
					     size_t /* in_length */,
					     krb5_pointer /* key/seed */,
					     size_t /* key/seed size */,
					     krb5_checksum * /* out_cksum */));
    int checksum_length;		/* length of stuff returned by
					   sum_func */
    unsigned int is_collision_proof:1;
    unsigned int uses_key:1;
} krb5_checksum_entry;

/* per Kerberos v5 protocol spec */
#define	KEYTYPE_NULL		0x0000
#define KEYTYPE_DES		0x0001	/* Data Encryption Standard,
					   FIPS 46,81 */

#define	ETYPE_NULL		0x0000
#define	ETYPE_DES_CBC_CRC	0x0001	/* DES cbc mode with CRC-32 */

#define	CKSUMTYPE_CRC32		0x0001
#define	CKSUMTYPE_RSA_MD4	0x0002
#define	CKSUMTYPE_XEROX		0x0003	/* SNEFRU */
#define	CKSUMTYPE_DESCBC	0x0004

/* macros to determine if a type is a local type */
#define KEYTYPE_IS_LOCAL(keytype) (keytype & 0x8000)
#define ETYPE_IS_LOCAL(etype) (etype & 0x8000)
#define CKSUMTYPE_IS_LOCAL(cksumtype) (cksumtype & 0x8000)

#ifndef krb5_roundup
/* round x up to nearest multiple of y */
#define krb5_roundup(x, y) ((((x) + (y) - 1)/(y))*(y))
#endif /* roundup */

/* macro function definitions to help clean up code */
#define	krb5_encrypt_size(length, crypto) \
     krb5_roundup((length)+(crypto)->pad_minimum, (crypto)->block_length)

/* This array is indexed by encryption type */
extern krb5_cs_table_entry *krb5_csarray[];
extern int krb5_max_cryptosystem;		/* max entry in array */

/* This array is indexed by key type, and has (should have) pointers to
   the same entries as krb5_csarray */
/* XXX what if a given keytype works for several etypes? */
extern krb5_cs_table_entry *krb5_keytype_array[];
extern int krb5_max_keytype;		/* max entry in array */

/* This array is indexed by checksum type */
extern krb5_checksum_entry *krb5_cksumarray[];
extern int krb5_max_cksum;		/* max entry in array */

#define valid_etype(etype)     ((etype <= krb5_max_cryptosystem) && (etype > 0) && krb5_csarray[etype])

#define valid_keytype(ktype)     ((ktype <= krb5_max_keytype) && (ktype > 0) && krb5_keytype_array[ktype])

#define valid_cksumtype(cktype)     ((cktype <= krb5_max_cksum) && (cktype > 0) && krb5_cksumarray[cktype])

#define is_coll_proof_cksum(cktype) (krb5_cksumarray[cktype]->is_collision_proof)
#define is_keyed_cksum(cktype) (krb5_cksumarray[cktype]->uses_key)

/* set up *eblockp to use etype */
#define krb5_use_cstype(eblockp, etype) (eblockp)->crypto_entry = krb5_csarray[(etype)]->system

#define krb5_encrypt(inptr, outptr, size, eblock, ivec) (*(eblock)->crypto_entry->encrypt_func)(inptr, outptr, size, eblock, ivec)
#define krb5_decrypt(inptr, outptr, size, eblock, ivec) (*(eblock)->crypto_entry->decrypt_func)(inptr, outptr, size, eblock, ivec)
#define krb5_process_key(eblock, key) (*(eblock)->crypto_entry->process_key)(eblock, key)
#define krb5_finish_key(eblock) (*(eblock)->crypto_entry->finish_key)(eblock)
#define krb5_string_to_key(eblock, keytype, keyblock, data, princ) (*(eblock)->crypto_entry->string_to_key)(keytype, keyblock, data, princ)
#define krb5_init_random_key(eblock, keyblock, ptr) (*(eblock)->crypto_entry->init_random_key)(keyblock, ptr)
#define krb5_finish_random_key(eblock, ptr) (*(eblock)->crypto_entry->finish_random_key)(ptr)
#define krb5_random_key(eblock, ptr, keyblock) (*(eblock)->crypto_entry->random_key)(ptr, keyblock)

#endif /* KRB5_ENCRYPTION__ */
