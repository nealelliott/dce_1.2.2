/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsa-md4.h,v $
 * Revision 1.1.8.1  1996/11/13  18:04:57  arvind
 * 	CHFts20148: add MD5
 * 	[1996/10/16  23:19 UTC  sommerfeld  /main/sommerfeld_pk_kdc_3/1]
 *
 * Revision 1.1.6.2  1996/02/18  23:03:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:36  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:44:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:22  root]
 * 
 * Revision 1.1.4.1  1993/10/14  17:45:17  mccann
 * 	CR8651 64bit port
 * 	[1993/10/14  17:42:38  mccann]
 * 
 * Revision 1.1.2.2  1992/12/29  14:01:49  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:53:54  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rsa-md4.h V=2 10/23/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/rsa-md4.h,v $
 * $Author: arvind $
 * $Id: rsa-md4.h,v 1.1.8.1 1996/11/13 18:04:57 arvind Exp $
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 * RSA MD4 header file, with Kerberos/STDC additions.
 */

#ifndef __KRB5_RSA_MD4_H__
#define __KRB5_RSA_MD4_H__


/* 16 u_char's in the digest */
#define RSA_MD4_CKSUM_LENGTH	16
/* des blocksize is 8, so this works nicely... */
#define RSA_MD4_DES_CKSUM_LENGTH	16

extern krb5_checksum_entry
    rsa_md4_cksumtable_entry,
    rsa_md4_des_cksumtable_entry;

/*
 **********************************************************************
 ** md4.h -- Header file for implementation of MD4                   **
 ** RSA Data Security, Inc. MD4 Message Digest Algorithm             **
 ** Created: 2/17/90 RLR                                             **
 ** Revised: 12/27/90 SRD,AJ,BSK,JT Reference C version              **
 **********************************************************************
 */

/*
 **********************************************************************
 ** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved. **
 **                                                                  **
 ** License to copy and use this software is granted provided that   **
 ** it is identified as the "RSA Data Security, Inc. MD4 Message     **
 ** Digest Algorithm" in all material mentioning or referencing this **
 ** software or this function.                                       **
 **                                                                  **
 ** License is also granted to make and use derivative works         **
 ** provided that such works are identified as "derived from the RSA **
 ** Data Security, Inc. MD4 Message Digest Algorithm" in all         **
 ** material mentioning or referencing the derived work.             **
 **                                                                  **
 ** RSA Data Security, Inc. makes no representations concerning      **
 ** either the merchantability of this software or the suitability   **
 ** of this software for any particular purpose.  It is provided "as **
 ** is" without express or implied warranty of any kind.             **
 **                                                                  **
 ** These notices must be retained in any copies of any part of this **
 ** documentation and/or software.                                   **
 **********************************************************************
 */

#ifndef __KRB5_RSA_MD5_H__
#ifdef BITS32
/* typedef a 32 bit type */
typedef unsigned long int UINT4;
#else
#ifdef  BITS64
/* still using 32 bits */
typedef unsigned int UINT4;
#else
 error: you gotta fix this implementation to deal with non-32 bit words;
#endif
#endif
#endif

/* Data structure for MD4 (Message Digest) computation */
typedef struct {
  UINT4 i[2];                   /* number of _bits_ handled mod 2^64 */
  UINT4 buf[4];                                    /* scratch buffer */
  unsigned char in[64];                              /* input buffer */
  unsigned char digest[16];     /* actual digest after MD4Final call */
} MD4_CTX;

#ifdef __STDC__
extern void MD4Init(MD4_CTX *);
extern void MD4Update(MD4_CTX *, unsigned char *, unsigned int);
extern void MD4Final(MD4_CTX *);
#else
void MD4Init ();
void MD4Update ();
void MD4Final ();
#endif

/*
 **********************************************************************
 ** End of md4.h                                                     **
 ******************************* (cut) ********************************
 */
#endif /* __KRB5_RSA_MD4_H__ */
