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
 * $Log: rsa-md4.h,v $
 * Revision 1.1.2.1  1996/06/05  20:30:32  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:29:36  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/crypto/md4/rsa-md4.h
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 * RSA MD4 header file, with Kerberos/STDC additions.
 */

#ifndef __KRB5_RSA_MD4_H__
#define __KRB5_RSA_MD4_H__

#ifdef unicos61
#include <sys/types.h>
#endif /* unicos61 */

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

#include <k5-config.h>

/* Data structure for MD4 (Message Digest) computation */
typedef struct {
  krb5_ui_4 i[2];			/* number of _bits_ handled mod 2^64 */
  krb5_ui_4 buf[4];			/* scratch buffer */
  unsigned char in[64];			/* input buffer */
  unsigned char digest[16];		/* actual digest after MD4Final call */
} MD4_CTX;

#if defined(__STDC__) || defined(_WINDOWS)
extern void MD4Init(MD4_CTX FAR *);
extern void MD4Update(MD4_CTX FAR *, unsigned char FAR *, unsigned int);
extern void MD4Final(MD4_CTX FAR *);
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
