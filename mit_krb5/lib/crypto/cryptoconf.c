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
 * $Log: cryptoconf.c,v $
 * Revision 1.1.2.2  1996/11/14  19:42:07  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/14  19:40:39  arvind]
 *
 * Revision 1.1.2.1  1996/06/05  20:29:48  arvind
 * 	     DCE1.2.2 Drop 2 to OSF
 * 	     [1996/06/05  19:28:57  arvind]
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/crypto/cryptoconf.c
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
 * Cryptosystem configurations
 */

#include "k5-int.h"

#if defined(PROVIDE_DES_CBC_CRC) || defined(PROVIDE_CRC32)
#include "crc-32.h"
#define CRC32_CKENTRY &crc32_cksumtable_entry
#else
#define CRC32_CKENTRY 0
#endif

#ifdef PROVIDE_RSA_MD4
#include "rsa-md4.h"
#define MD4_CKENTRY &rsa_md4_cksumtable_entry
#define MD4_DES_CKENTRY &rsa_md4_des_cksumtable_entry
#else
#define MD4_CKENTRY 0
#define MD4_DES_CKENTRY 0
#endif

#ifdef PROVIDE_RSA_MD5
#include "rsa-md5.h"
#define MD5_CKENTRY &rsa_md5_cksumtable_entry
#define MD5_DES_CKENTRY &rsa_md5_des_cksumtable_entry
#else
#define MD5_CKENTRY 0
#define MD5_DES_CKENTRY 0
#endif

#ifdef PROVIDE_SNEFRU
#define XEROX_CKENTRY &snefru_cksumtable_entry
#else
#define XEROX_CKENTRY 0
#endif

#ifdef PROVIDE_DES_CBC_CKSUM
#include "des_int.h"
#define _DES_DONE__
#ifdef OSF_DCE
#define DES_CBC_CKENTRY &mit_des_cbc_cksumtable_entry
#else
#define DES_CBC_CKENTRY &krb5_des_cbc_cksumtable_entry
#endif
#else
#define DES_CBC_CKENTRY 0
#endif

#ifdef PROVIDE_DES_CBC_CRC
#ifndef _DES_DONE__
#include "des_int.h"
#define _DES_DONE__
#endif
#ifdef OSF_DCE
static krb5_cs_table_entry mit_des_cbc_crc_csentry = {
    0, &mit_des_cryptosystem_entry, 0 };
#define DES_CBC_CRC_CSENTRY &mit_des_cbc_crc_csentry
#else
#define DES_CBC_CRC_CSENTRY &krb5_des_crc_cst_entry
#endif
#else
#define DES_CBC_CRC_CSENTRY 0
#endif

#ifdef PROVIDE_DES_CBC_MD5
#ifndef _DES_DONE__
#include "des_int.h"
#define _DES_DONE__
#endif
#ifdef OSF_DCE
extern krb5_cs_table_entry krb5_des_md5_cst_entry;
#endif
#define DES_CBC_MD5_CSENTRY &krb5_des_md5_cst_entry
#else
#define DES_CBC_MD5_CSENTRY 0
#endif
    
#ifdef PROVIDE_RAW_DES_CBC
#ifndef _DES_DONE__
#include "des_int.h"
#define _DES_DONE__
#endif
#define RAW_DES_CBC_CSENTRY &krb5_raw_des_cst_entry
#else
#define RAW_DES_CBC_CSENTRY 0
#endif


/* WARNING:
   make sure the order of entries in these tables matches the #defines in
   "krb5/encryption.h"
 */

krb5_cs_table_entry * NEAR krb5_csarray[] = {
    0,				/* ETYPE_NULL */
    DES_CBC_CRC_CSENTRY,	/* ETYPE_DES_CBC_CRC */
    0,				/* ETYPE_DES_CBC_MD4 */
    DES_CBC_MD5_CSENTRY,	/* ETYPE_DES_CBC_MD5 */
    RAW_DES_CBC_CSENTRY,	/* ETYPE_RAW_DES_CBC */
};

int krb5_max_cryptosystem = sizeof(krb5_csarray)/sizeof(krb5_csarray[0]) - 1;

krb5_cs_table_entry * NEAR krb5_keytype_array[] = {
    0,				/* KEYTYPE_NULL */
    DES_CBC_CRC_CSENTRY,	/* KEYTYPE_DES */
    0,
    DES_CBC_MD5_CSENTRY		/* KEYTYPE_DES_MD5 */
};

int krb5_max_keytype = sizeof(krb5_keytype_array)/sizeof(krb5_keytype_array[0]) - 1;

krb5_checksum_entry * NEAR krb5_cksumarray[] = {
    0,
    CRC32_CKENTRY,		/* CKSUMTYPE_CRC32 */
    MD4_CKENTRY,		/* CKSUMTYPE_RSA_MD4 */
    MD4_DES_CKENTRY,		/* CKSUMTYPE_RSA_MD4_DES */
    DES_CBC_CKENTRY,		/* CKSUMTYPE_DESCBC */
    0,				/* des-mac */
    0,				/* des-mac-k */
    MD5_CKENTRY,		/* CKSUMTYPE_RSA_MD5 */
    MD5_DES_CKENTRY		/* CKSUMTYPE_RSA_MD5_DES */
};

int krb5_max_cksum = sizeof(krb5_cksumarray)/sizeof(krb5_cksumarray[0]);

#undef _DES_DONE__
