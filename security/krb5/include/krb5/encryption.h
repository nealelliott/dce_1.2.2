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
 * Revision 1.1.7.4  1996/11/13  18:04:00  arvind
 * 	Segregate DES_MD5 keys from DES keys.
 * 	[1996/11/05  02:08 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_4/1]
 *
 * 	CHFts20148: make MD5 work.
 * 	[1996/10/16  23:21 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_3/1]
 *
 * 	Bring into agreement with latest pre-draft of pk-init.
 * 	[1996/07/18  17:33 UTC  aha  /main/DCE_1.2.2/2]
 *
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/15  19:43 UTC  aha  /main/DCE122_PK/aha_pk6/3]
 *
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:50 UTC  psn  /main/DCE122_PK/1]
 *
 * 	Merge ASN.1 code and encoding functions for public key login
 * 	[1996/05/07  20:12 UTC  aha  /main/aha_pk2/1]
 *
 * 	Additions for public key login
 * 	[1992/12/29  13:58:37  zeliff  1.1.3.3]
 *
 * Revision 1.1.7.3  1996/10/03  14:56:09  arvind
 * 	Bring into agreement with latest pre-draft of pk-init.
 * 	[1996/07/18  17:33 UTC  aha  /main/DCE_1.2.2/2]
 * 
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/15  19:43 UTC  aha  /main/DCE122_PK/aha_pk6/3]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:50 UTC  psn  /main/DCE122_PK/1]
 * 
 * 	Merge ASN.1 code and encoding functions for public key login
 * 	[1996/05/07  20:12 UTC  aha  /main/aha_pk2/1]
 * 
 * 	Additions for public key login
 * 	[1992/12/29  13:58:37  zeliff  1.1.3.3]
 * 
 * Revision 1.1.7.2  1996/08/09  12:07:27  arvind
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/15  19:43 UTC  aha  /main/DCE122_PK/aha_pk6/3]
 * 
 * 	Changes for Public Key Login; support private key storage for KDC in Registry
 * 	[1996/06/05  16:14 UTC  aha  /main/DCE12_PK/aha_pk5/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  17:56 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:50 UTC  psn  /main/DCE122_PK/1]
 * 
 * 	Merge ASN.1 code and encoding functions for public key login
 * 	[1996/05/07  20:12 UTC  aha  /main/aha_pk2/1]
 * 
 * 	Additions for public key login
 * 	[1992/12/29  13:58:37  zeliff  1.1.3.3]
 * 
 * Revision 1.1.7.1  1996/07/08  18:34:38  arvind
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:50 UTC  psn  /main/DCE122_PK/1]
 * 
 * 	Merge ASN.1 code and encoding functions for public key login
 * 	[1996/05/07  20:12 UTC  aha  /main/aha_pk2/1]
 * 
 * 	Additions for public key login
 * 	[1992/12/29  13:58:37  zeliff  1.1.3.3]
 * 
 * Revision 1.1.5.2  1996/02/18  23:02:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:45  marty]
 * 
 * Revision 1.1.5.1  1995/12/08  17:41:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:58  root]
 * 
 * Revision 1.1.3.3  1992/12/29  13:58:37  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:52:11  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:18:58  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:33:10  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:48:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  encryption.h V=5 10/23/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/encryption.h,v $
 * $Author: arvind $
 * $Id: encryption.h,v 1.1.7.4 1996/11/13 18:04:00 arvind Exp $
 *
 * Copyright 1989,1990,1991 by the Massachusetts Institute of Technology.
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
 * Encryption interface-related declarations
 */


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

/*
 * The following defines are in a state of transition.
 *
 * Kerberos phased out distinct keytypes and etypes in favor of a combined "enctype"
 * which means both.
 *
 * The keytype, etype, and enctype spaces must be kept in synch with
 * each other; the enctype space is the master; the etype/keytype
 * spaces should be defined in terms of the etype space or better yet,
 * phased out altogether.
 *
 * All enctypes and cksumtypes defined here are not necessarily implemented (yet).
 */

#define	ENCTYPE_NULL		0x0000
#define	ENCTYPE_DES_CBC_CRC	0x0001	/* DES cbc mode with CRC-32 */
#define ENCTYPE_DES_CBC_MD4       0x0002
#define ENCTYPE_DES_CBC_MD5       0x0003
#define	ENCTYPE_DES_CBC_RAW     0x0004  /* DES cbc mode raw */
#define	ENCTYPE_DES3_CBC_SHA	0x0005	/* DES-3 cbc mode with NIST-SHA */
#define	ENCTYPE_DES3_CBC_RAW	0x0006	/* DES-3 cbc mode raw */

#define ENCTYPE_SIGN_MD5RSA       0x0011  /* AI_MD5WithRSAEncryption - used in pk-init */
#define ENCTYPE_ENCRYPT_RSA_PRIV  0x0021  /* AI_PKCS_RSAPrivate - used in pk-init */
#define ENCTYPE_ENCRYPT_RSA_PUB   0x0022  /* ??? from pk-init draft */

/* per Kerberos v5 protocol spec */
#define	KEYTYPE_NULL		ENCTYPE_NULL
#define KEYTYPE_DES		ENCTYPE_DES_CBC_CRC	/* Data Encryption Standard, FIPS 46,81 */
#define KEYTYPE_DES_MD5		ENCTYPE_DES_CBC_MD5

/* KEYTYPE_ENCRYPT_RSA_PRIV is not per Kerberos v5 protocol spec */
#define KEYTYPE_ENCRYPT_RSA_PRIV ENCTYPE_ENCRYPT_RSA_PRIV /* pk-init ENCTYPE_ENCRYPT_RSA_PRIV */

#define	ETYPE_NULL		ENCTYPE_NULL
#define	ETYPE_DES_CBC_CRC	ENCTYPE_DES_CBC_CRC	/* DES cbc mode with CRC-32 */
#define ETYPE_DES_CBC_MD4       ENCTYPE_DES_CBC_MD4
#define ETYPE_DES_CBC_MD5       ENCTYPE_DES_CBC_MD5

#define ETYPE_SIGN_MD5RSA       ENCTYPE_SIGN_MD5RSA  /* AI_MD5WithRSAEncryption - used in pk-init */
#define ETYPE_ENCRYPT_RSA_PRIV  ENCTYPE_ENCRYPT_RSA_PRIV  /* AI_PKCS_RSAPrivate - used in pk-init */
#define ETYPE_ENCRYPT_RSA_PUB   ENCTYPE_ENCRYPT_RSA_PUB   /* ??? from pk-init draft */

#define	CKSUMTYPE_CRC32		0x0001
#define	CKSUMTYPE_RSA_MD4	0x0002
#define	CKSUMTYPE_RSA_MD4_DES	0x0003
#define	CKSUMTYPE_DESCBC	0x0004
#define CKSUMTYPE_DES_MAC_K     0x0005
#define CKSUMTYPE_RSA_MD4_DES_K 0x0006
#define	CKSUMTYPE_RSA_MD5	0x0007
#define	CKSUMTYPE_RSA_MD5_DES	0x0008
#define CKSUMTYPE_NIST_SHA	0x0009
#define CKSUMTYPE_HMAC_SHA	0x000a

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

/* Use following when a static size calculation is possible and required */
#define krb5_encrypt_size_static(length,pad_minimum,block_length) \
     krb5_roundup((length)+pad_minimum, block_length)

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

#define valid_etype(etype)     (((int) etype <= krb5_max_cryptosystem) && (etype > 0) && krb5_csarray[etype])

#define valid_keytype(ktype)     (((int) ktype <= krb5_max_keytype) && (ktype > 0) && krb5_keytype_array[ktype])

#define valid_enctype(ktype)     (((int) ktype <= krb5_max_keytype) && (ktype > 0) && krb5_keytype_array[ktype])

#define valid_cksumtype(cktype)     (((int) cktype <= krb5_max_cksum) && (cktype > 0) && krb5_cksumarray[cktype])

#define is_coll_proof_cksum(cktype) (krb5_cksumarray[cktype]->is_collision_proof)
#define is_keyed_cksum(cktype) (krb5_cksumarray[cktype]->uses_key)

/* set up *eblockp to use etype */
#define krb5_use_cstype(eblockp, etype) (eblockp)->crypto_entry = krb5_csarray[(etype)]->system
/* ...or keytype */
#define krb5_use_enctype(eblockp, keytype) (eblockp)->crypto_entry = krb5_keytype_array[(keytype)]->system

#define krb5_use_keytype(eblockp, keytype) (eblockp)->crypto_entry = krb5_keytype_array[(keytype)]->system

#define krb5_encrypt(inptr, outptr, size, eblock, ivec) (*(eblock)->crypto_entry->encrypt_func)(inptr, outptr, size, eblock, ivec)
#define krb5_decrypt(inptr, outptr, size, eblock, ivec) (*(eblock)->crypto_entry->decrypt_func)(inptr, outptr, size, eblock, ivec)
#define krb5_process_key(eblock, key) (*(eblock)->crypto_entry->process_key)(eblock, key)
#define krb5_finish_key(eblock) (*(eblock)->crypto_entry->finish_key)(eblock)
#define krb5_string_to_key(eblock, keytype, keyblock, data, princ) (*(eblock)->crypto_entry->string_to_key)(keytype, keyblock, data, princ)
#define krb5_init_random_key(eblock, keyblock, ptr) (*(eblock)->crypto_entry->init_random_key)(keyblock, ptr)
#define krb5_finish_random_key(eblock, ptr) (*(eblock)->crypto_entry->finish_random_key)(ptr)
#define krb5_random_key(eblock, ptr, keyblock) (*(eblock)->crypto_entry->random_key)(ptr, keyblock)

/*
 * Here's the stuff for the checksum switch:
 */
#define krb5_checksum_size(ctype)  (krb5_cksumarray[ctype]->checksum_length)
#define krb5_calculate_checksum(ctype, in, in_length, seed, seed_length, outcksum) ((*krb5_cksumarray[ctype]->sum_func)(in, in_length, seed, seed_length, outcksum))

/*
 * encryption routine prototypes
 */
krb5_error_code krb5_encrypt_data
	PROTOTYPE((krb5_context context, krb5_keyblock *key, 
		   krb5_pointer ivec, krb5_data *data, 
		   krb5_enc_data *enc_data));

krb5_error_code krb5_decrypt_data
	PROTOTYPE((krb5_context context, krb5_keyblock *key, 
		   krb5_pointer ivec, krb5_enc_data *data, 
		   krb5_data *enc_data));


#endif /* KRB5_ENCRYPTION__ */
