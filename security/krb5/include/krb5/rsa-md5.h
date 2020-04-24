/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsa-md5.h,v $
 * Revision 1.1.8.1  1996/11/13  18:05:10  arvind
 * 	CHFts20148: add MD5
 * 	[1996/10/16  23:20 UTC  sommerfeld  /main/sommerfeld_pk_kdc_3/1]
 *
 * Revision 1.1.6.2  1996/02/18  23:03:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:38  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:44:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:23  root]
 * 
 * Revision 1.1.4.1  1993/10/14  17:45:18  mccann
 * 	CR8651 64bit port
 * 	[1993/10/14  17:42:43  mccann]
 * 
 * Revision 1.1.2.2  1992/12/29  14:01:55  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:53:59  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rsa-md5.h V=1 09/10/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/rsa-md5.h,v $
 * $Author: arvind $
 * $Id: rsa-md5.h,v 1.1.8.1 1996/11/13 18:05:10 arvind Exp $
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
 * RSA MD5 header file, with Kerberos/STDC additions.
 */

#ifndef __KRB5_RSA_MD5_H__
#define __KRB5_RSA_MD5_H__


/* 16 u_char's in the digest */
#define RSA_MD5_CKSUM_LENGTH    16
/* des blocksize is 8, so this works nicely... */
#define RSA_MD5_DES_CKSUM_LENGTH        16

extern krb5_checksum_entry
    rsa_md5_cksumtable_entry,
    rsa_md5_des_cksumtable_entry;


/*
 ***********************************************************************
 ** md5.h -- header file for implementation of MD5                    **
 ** RSA Data Security, Inc. MD5 Message-Digest Algorithm              **
 ** Created: 2/17/90 RLR                                              **
 ** Revised: 12/27/90 SRD,AJ,BSK,JT Reference C version               **
 ** Revised (for MD5): RLR 4/27/91                                    **
 **   -- G modified to have y&~z instead of y&z                       **
 **   -- FF, GG, HH modified to add in last register done             **
 **   -- Access pattern: round 2 works mod 5, round 3 works mod 3     **
 **   -- distinct additive constant for each step                     **
 **   -- round 4 added, working mod 7                                 **
 ***********************************************************************
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

#ifndef __KRB5_RSA_MD4_H__
#ifdef BITS32
/* typedef a 32 bit type */
typedef unsigned long int UINT4;
#else
#ifdef  BITS64
/* still using 32bits */
typedef unsigned int UINT4;
#else
 error: you gotta fix this implementation to deal with non-32 bit words;
#endif
#endif
#endif

/* Data structure for MD5 (Message Digest) computation */
typedef struct {
  UINT4 i[2];                   /* number of _bits_ handled mod 2^64 */
  UINT4 buf[4];                                    /* scratch buffer */
  unsigned char in[64];                              /* input buffer */
  unsigned char digest[16];     /* actual digest after MD5Final call */
} MD5_CTX;

#ifdef __STDC__
extern void MD5Init(MD5_CTX *);
extern void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
extern void MD5Final(MD5_CTX *);
#else
void MD5Init ();
void MD5Update ();
void MD5Final ();
#endif

/*
 **********************************************************************
 ** End of md4.h                                                     **
 ******************************* (cut) ********************************
 */
#endif /* __KRB5_RSA_MD5_H__ */
