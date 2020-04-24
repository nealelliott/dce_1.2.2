/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cryptoconf.c,v $
 * Revision 1.1.6.1  1996/11/13  18:06:51  arvind
 * 	Finish adding MD5 support
 * 	[1996/11/05  02:14 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_4/1]
 *
 * 	CHFts20148: add MD5.
 * 	[1996/10/16  22:51 UTC  sommerfeld  /main/sommerfeld_pk_kdc_3/1]
 *
 * Revision 1.1.4.2  1996/02/18  00:11:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:59  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:50:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:39  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:25:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:07:06  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  cryptoconf.c V=3 10/24/91 //littl/prgy/krb5/lib
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/cryptoconf.c,v $
 * $Author: arvind $
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
 * Cryptosystem configurations
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_cryptoconf_c[] =
"$Id: cryptoconf.c,v 1.1.6.1 1996/11/13 18:06:51 arvind Exp $";
#endif	/* !lint & !SABER */

#include <krb5/config.h>
#include <krb5/osconf.h>
#include <krb5/krb5.h>

#if defined(PROVIDE_DES_CBC_CRC) || defined(PROVIDE_LUCIFER_CRC) || defined(PROVIDE_CRC32)
#include <krb5/crc-32.h>
#define CRC32_CKENTRY &crc32_cksumtable_entry
#else
#define CRC32_CKENTRY 0
#endif

#ifdef PROVIDE_RSA_MD4
#include <krb5/rsa-md4.h>
#define MD4_CKENTRY &rsa_md4_cksumtable_entry
#define MD4_DES_CKENTRY &rsa_md4_des_cksumtable_entry
#else
#define MD4_CKENTRY 0
#define MD4_DES_CKENTRY 0
#endif

#ifdef PROVIDE_RSA_MD5
#include <krb5/rsa-md5.h>
#define MD5_CKENTRY &rsa_md5_cksumtable_entry
#define MD5_DES_CKENTRY &rsa_md5_des_cksumtable_entry
extern krb5_cs_table_entry krb5_des_md5_cst_entry;
#define DES_CBC_MD5_CSENTRY &krb5_des_md5_cst_entry
#else
#define MD5_CKENTRY 0
#define MD5_DES_CKENTRY 0
#define DES_CBC_MD5_CSENTRY 0
#endif

#ifdef PROVIDE_SNEFRU
#define XEROX_CKENTRY &snefru_cksumtable_entry
#else
#define XEROX_CKENTRY 0
#endif

#ifdef PROVIDE_DES_CBC_CKSUM
#include <krb5/mit-des.h>
#define _DES_DONE__
#define DES_CBC_CKENTRY &mit_des_cbc_cksumtable_entry
#else
#define DES_CBC_CKENTRY 0
#endif

#ifdef PROVIDE_DES_CBC_CRC
#ifndef _DES_DONE__
#include <krb5/mit-des.h>
#define _DES_DONE__
#endif
static krb5_cs_table_entry mit_des_cbc_crc_csentry = {
    &mit_des_cryptosystem_entry, 0 };
#define DES_CBC_CRC_CSENTRY &mit_des_cbc_crc_csentry
#else
#define DES_CBC_CRC_CSENTRY 0
#endif

#ifdef PROVIDE_LUCIFER_CRC
static krb5_cs_table_entry lucifer_crc_csentry = {
    &lucifer_cryptosystem_entry, 0 };
#define LUCIFER_CRC_CSENTRY &lucifer_crc_csentry
#else
#define LUCIFER_CRC_CSENTRY 0
#endif

/* for the future ... */
#define SHA_CKENTRY 0
#define HMAC_SHA_CKENTRY 0
#define RAW_DES_CBC_CSENTRY 0

/* WARNING:
   make sure the order of entries in these tables matches the #defines in
   <krb5/encryption.h>
 */

krb5_cs_table_entry *krb5_csarray[] = {
    0,
    DES_CBC_CRC_CSENTRY,
    0,				/* ETYPE_DES_CBC_MD4 */
    DES_CBC_MD5_CSENTRY,	/* ETYPE_DES_CBC_MD5 */
    RAW_DES_CBC_CSENTRY,	/* ETYPE_RAW_DES_CBC */
};

int krb5_max_cryptosystem = sizeof(krb5_csarray)/sizeof(krb5_csarray[0]) - 1;

krb5_cs_table_entry *krb5_keytype_array[] = {
    0,
    DES_CBC_CRC_CSENTRY,
    LUCIFER_CRC_CSENTRY,
    DES_CBC_MD5_CSENTRY,
};

int krb5_max_keytype = sizeof(krb5_keytype_array)/sizeof(krb5_keytype_array[0]) - 1;

krb5_checksum_entry *krb5_cksumarray[] = {
    0,
    CRC32_CKENTRY,
    MD4_CKENTRY,
    MD4_DES_CKENTRY,
    DES_CBC_CKENTRY,
    0,				/* 5 - des-mac-k */
    0,				/* 6 - rsa-md4-des-k */
    MD5_CKENTRY,		/* 7 - CKSUMTYPE_RSA_MD5 */
    MD5_DES_CKENTRY,		/* 8 - CKSUMTYPE_RSA_MD5_DES */
    SHA_CKENTRY,		/* 9 - CKSUMTYPE_NIST_SHA */
    HMAC_SHA_CKENTRY		/* 10 - CKSUMTYPE_NIST_SHA_DES3 */
};

int krb5_max_cksum = sizeof(krb5_cksumarray)/sizeof(krb5_cksumarray[0]);

#undef _DES_DONE__
