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
 * Revision 1.1.6.2  1996/02/18  23:10:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:35:36  marty]
 *
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/23  15:21:51  pare]
 *
 * Revision 1.1.6.1  1995/12/08  18:04:18  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  18:58 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1995/12/08  17:23:23  root]
 * 
 * 	Recreate GAMERA branch.
 * 	[1993/04/05  17:18:27  truitt]
 * 
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/19  13:43:40  truitt]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/30  19:05:32  htf]
 * 
 * Revision 1.1.8.3  1993/09/15  17:45:23  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.7.2  1993/07/08  19:32:31  root
 * 	Initial King Kong branch
 * 	[1993/07/08  19:32:01  root]
 * 
 * Revision 1.1.3.5  1993/03/11  22:30:03  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.2.3  1992/12/21  23:20:04  smythe
 * 	reversed long order for int64 struct to mirror
 * 	big endian RIOS rather than little endian MIPS
 * 	since HPUX is big endian
 * 	[1992/12/19  16:47:43  smythe]
 * 
 * Revision 1.1.2.2  1992/06/24  16:59:52  grober
 * 	Initial HPUX revision
 * 	[1992/06/24  16:59:30  grober]
 * 
 * Revision 1.1.2.2  1992/06/05  18:51:09  hopkins
 * 	New file for HP800 port
 * 	(taken from MIPS directory)
 * 	[1992/06/05  15:49:50  hopkins]
 * 
 * Revision 1.1.2.2  1992/05/06  21:52:33  comuzzi
 * 	Fix defects 2295 and 3143. Copy from file time/common/int64.h.
 * 	[1992/05/06  21:20:49  comuzzi]
 * 
 * Revision 1.1  1992/01/19  15:31:51  devrcs
 * 	Initial revision
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
	unsigned long hi;
	unsigned long lo;
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

