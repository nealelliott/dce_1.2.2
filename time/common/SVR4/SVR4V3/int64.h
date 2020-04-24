/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @SNI_COPYRIGHT@
 */
/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: int64.h,v $
 * Revision 1.1.6.2  1996/02/18  23:11:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:00  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:05:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:35  root]
 * 
 * Revision 1.1.4.1  1994/06/10  20:51:48  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:40  devsrc]
 * 
 * Revision 1.1.2.2  1993/02/01  20:18:11  hinman
 * 	[hinman] - Check in SNI revision 1.1.2.2
 * 	[1993/01/08  14:09:53  hinman]
 * 
 * Revision 1.1.2.2  92/11/02  17:57:47  steveg
 * 	New file to support SVR4V3 TARGET_OS_PLATFORM
 * 	[92/11/02  16:37:48  steveg]
 * 
 * $EndLog$
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

