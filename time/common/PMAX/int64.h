/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: int64.h,v $
 * Revision 1.1.6.2  1996/02/18  23:10:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:35:45  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:04:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:29  root]
 * 
 * Revision 1.1.4.2  1992/12/30  20:37:38  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:05:58  htf]
 * 
 * Revision 1.1.2.2  1992/05/06  21:52:45  comuzzi
 * 	Fix defect 2295 and 3143. Copy from time/common/int64.h
 * 	[1992/05/06  21:22:42  comuzzi]
 * 
 * Revision 1.1  1992/01/19  15:31:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*	@(#)int64.h	5.1	(ULTRIX)	4/23/90	*/

#ifndef _INT64_H
#define _INT64_H	"@(#)int64.h	5.1	(ULTRIX)	4/23/90"

/************************************************************************
 *									*
 *			Copyright (c) 1990 by				*
 *		Digital Equipment Corporation, Maynard, MA		*
 *			All rights reserved.				*
 *									*
 *   This software is furnished under a license and may be used and	*
 *   copied  only  in accordance with the terms of such license and	*
 *   with the  inclusion  of  the  above  copyright  notice.   This	*
 *   software  or  any  other copies thereof may not be provided or	*
 *   otherwise made available to any other person.  No title to and	*
 *   ownership of the software is hereby transferred.			*
 *									*
 *   The information in this software is subject to change  without	*
 *   notice  and should not be construed as a commitment by Digital	*
 *   Equipment Corporation.						*
 *									*
 *   Digital assumes no responsibility for the use  or  reliability	*
 *   of its software on equipment which is not supplied by Digital.	*
 *									*
 ************************************************************************/

/*
 * This file is a collection of 64 bit integer stuff used by
 * kern_utctime.c. It has been isolated in an include file to assist
 * in migration to 64 bit platforms. Obvisously for 64 bit machines, these
 * macros could be greatly simplified.
 */

/*
 * 64 Bit integer type
 */

struct int64 {
	unsigned long lo;
	unsigned long hi;
};

/*
 * UTC_ADD32P - macro to add a positive 32-bit integer to a 64-bit integer
 */
#define UTC_ADD32P(I,extint)						\
{									\
    int _ADD32P_intermediate = ((extint)->lo&0x80000000);		\
									\
    (extint)->lo += (I) ;						\
    if (_ADD32P_intermediate)						\
    {									\
	if ((long int) (extint)->lo >= 0) 				\
	    (extint)->hi++ ;						\
    }									\
}
/* #define UTC_ADD32P(I,extint)	(extint) += (I) */

/*
 * UTC_MUL32P - macro to multiply two positive 32-bit numbers and return
 *	    a 64-bit result.
 */
#define UTC_MUL32P(I, J, extint)						\
{									\
    unsigned long int _MUL32P_u1 = (I) >> 16;				\
    unsigned long int _MUL32P_u2 = (I) & 0xffff;			\
    unsigned long int _MUL32P_v1 = (J) >> 16;				\
    unsigned long int _MUL32P_v2 = (J) & 0xffff;			\
    register unsigned long int _MUL32P_temp;				\
									\
    _MUL32P_temp = _MUL32P_u2 * _MUL32P_v2;				\
    (extint)->lo = _MUL32P_temp & 0xffff;				\
    _MUL32P_temp = _MUL32P_u1 * _MUL32P_v2 + (_MUL32P_temp >> 16);	\
    (extint)->hi = _MUL32P_temp >> 16;					\
    _MUL32P_temp = _MUL32P_u2 * _MUL32P_v1 + (_MUL32P_temp & 0xffff);	\
    (extint)->lo += (_MUL32P_temp & 0xffff) << 16;			\
    (extint)->hi += _MUL32P_u1 * _MUL32P_v1 + (_MUL32P_temp >> 16);	\
}

/* #define UTC_MUL32P(I,extint)	(extint) = (I) * (J) */


/* 
 * UTC_ADD64 - macro to add two 64-bit integers.
 */

#define UTC_ADD64(ext1,ext2)						\
    (ext2)->hi += (ext1)->hi;						\
    if (!(((ext1)->lo&0x80000000) ^ ((ext2)->lo&0x80000000)))		\
    {									\
	(ext2)->lo += (ext1)->lo; 					\
	if ((long int) (ext1)->lo < 0)					\
	    (ext2)->hi++; 						\
    }									\
    else								\
    {									\
	(ext2)->lo += (ext1)->lo;					\
	if ((long int) (ext2)->lo >= 0) 				\
	    (ext2)->hi++ ;						\
    }
/* #define UTC_ADD64(ext1,ext2)	(ext2) += (ext1) */

/* 
 * UTC_SUB64 - macro to subtract two 64-bit integers.
 */

#define UTC_SUB64(ext1,ext2)						\
    (ext2)->hi += ~((ext1)->hi);					\
    if (!((~((ext1)->lo)&0x80000000) ^ ((ext2)->lo&0x80000000)))	\
    {									\
	(ext2)->lo += ~((ext1)->lo); 					\
	if ((long int) (ext1)->lo >= 0)					\
	    (ext2)->hi++; 						\
    }									\
    else								\
    {									\
	(ext2)->lo += ~((ext1)->lo);					\
	if ((long int) (ext2)->lo >= 0) 				\
	    (ext2)->hi++ ;						\
    }									\
    if (0 == ++((ext2)->lo))						\
	(ext2)->hi++;
/* #define UTC_SUB64(ext1,ext2)	(ext2) -= (ext1) */

#endif /* _INT64_H */

