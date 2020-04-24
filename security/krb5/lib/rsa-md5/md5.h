/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: md5.h,v $
 * Revision 1.1.6.2  1996/02/18  23:03:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:10  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:45:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:59  root]
 * 
 * Revision 1.1.4.1  1993/10/14  17:45:42  mccann
 * 	CR8651 64bit port
 * 	[1993/10/14  17:44:38  mccann]
 * 
 * Revision 1.1.2.2  1992/12/29  16:32:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:20:45  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  md5.h V=1 07/08/91 //littl/prgy/krb5/lib/rsa-md5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
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
 ***********************************************************************
 ** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved.  **
 **                                                                   **
 ** License to copy and use this software is granted provided that    **
 ** it is identified as the "RSA Data Security, Inc. MD5 Message-     **
 ** Digest Algorithm" in all material mentioning or referencing this  **
 ** software or this function.                                        **
 **                                                                   **
 ** License is also granted to make and use derivative works          **
 ** provided that such works are identified as "derived from the RSA  **
 ** Data Security, Inc. MD5 Message-Digest Algorithm" in all          **
 ** material mentioning or referencing the derived work.              **
 **                                                                   **
 ** RSA Data Security, Inc. makes no representations concerning       **
 ** either the merchantability of this software or the suitability    **
 ** of this software for any particular purpose.  It is provided "as  **
 ** is" without express or implied warranty of any kind.              **
 **                                                                   **
 ** These notices must be retained in any copies of any part of this  **
 ** documentation and/or software.                                    **
 ***********************************************************************
 */

/* typedef a 32-bit type */
#ifdef BITS64
typedef unsigned int UINT4;
#else
typedef unsigned long int UINT4;
#endif

/* Data structure for MD5 (Message-Digest) computation */
typedef struct {
  UINT4 i[2];                   /* number of _bits_ handled mod 2^64 */
  UINT4 buf[4];                                    /* scratch buffer */
  unsigned char in[64];                              /* input buffer */
  unsigned char digest[16];     /* actual digest after MD5Final call */
} MD5_CTX;

void MD5Init ();
void MD5Update ();
void MD5Final ();

/*
 ***********************************************************************
 ** End of md5.h                                                      **
 ******************************** (cut) ********************************
 */
