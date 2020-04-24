/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isutil2.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:30  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:30:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:05  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:31  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:04:20  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:15:07  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:07:51  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:57:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:16:30  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:38:50  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:41:50  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: isutil2.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:26 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/***************************************************************************
 *
 *			   INFORMIX SOFTWARE, INC.
 *
 *			      PROPRIETARY DATA
 *
 *	THIS DOCUMENT CONTAINS TRADE SECRET DATA WHICH IS THE PROPERTY OF 
 *	INFORMIX SOFTWARE, INC.  THIS DOCUMENT IS SUBMITTED TO RECIPIENT IN
 *	CONFIDENCE.  INFORMATION CONTAINED HEREIN MAY NOT BE USED, COPIED OR 
 *	DISCLOSED IN WHOLE OR IN PART EXCEPT AS PERMITTED BY WRITTEN AGREEMENT 
 *	SIGNED BY AN OFFICER OF INFORMIX SOFTWARE, INC.
 *
 *	THIS MATERIAL IS ALSO COPYRIGHTED AS AN UNPUBLISHED WORK UNDER
 *	SECTIONS 104 AND 408 OF TITLE 17 OF THE UNITED STATES CODE. 
 *	UNAUTHORIZED USE, COPYING OR OTHER REPRODUCTION IS PROHIBITED BY LAW.
 *
 *
 *  Title:	isutil2.c
 *  Description:
 *		Low level utility routines for C-ISAM routines.
 *		(module 1 of 2 - gotten by splitting isutil.c)
 *
 ***************************************************************************
 */


/*
 * utility routines
 *
 * Relational Database Systems, Inc.
 * Roy Harrington	November 4, 1981
 *
 *  06-07-83  RVH	V104 -- ifdefs for routines not used in kernel
 *  05-11-84		moved isam-specific functions to issupp.c
 *  02-13-85		include just machine.h; change CASTCP, FLOATSIZE,
 *			etc, to vanilla C
 *  08-01-86		split isutil.c into isutil1.c and isutil2.c, for COBOL
 */


#include <machine.h>
#include <isam.h>

#define TRUE	(1)
#define FALSE	(0)

#ifndef ISKERNEL

#ifdef SINIX	/* macros in machine.h must not be expanded. TP */
#undef stleng
#undef stcopy
#undef stcat
#undef stcmpr
#endif /* SINIX */
/*
 * stcat -- concatenate first null terminated string onto second
 */

stcat(src, dst)
    register char *src, *dst;
{
    while (*dst) dst++;
    while (*dst++ = *src++);
}

/*
 * stcmpr -- compare two null terminated strings
 *	     return -1, 0, 1  for  <, ==, > respectively
 */

stcmpr(s1, s2)
    register char *s1, *s2;
{
    for ( ; *s1 == *s2; s1++, s2++)
	if (*s1 == 0) return(0);

    if ((*s1 & BYTEMASK) < (*s2 & BYTEMASK))
	return(-1);
    else return(1);
}

#endif /* ISKERNEL */


/*
 * stcopy -- copy null terminated string
 */

stcopy(src, dst)
    register char *src, *dst;
{
    while (*dst++ = *src++);
}

/*
 * stleng -- calculate null terminated string length
 */

stleng(src)
    register char *src;
{
    register char *dst;

    dst = src;
    while (*dst) dst++;
    return(dst-src);
}

#ifndef NOFLOAT

/*
 * load float number
 */
 
#ifdef ldfloat
#undef ldfloat
#endif /* ldfloat */

double ldfloat(p)
register char *p;
{
	float fval;
	register char *f;
	register int c;

	f = (char *) &fval;
	c = sizeof(float);
	do
	    *f++ = *p++;
	while (--c);
	return(fval);
}

/*
 * store float number
 */

#ifdef stfloat
#undef stfloat
#endif /* stfloat */

stfloat(f,p)
double f;
register char *p;
{
	register char *s;
	register int c;
	float flo = f;

	s = (char *) &flo;
	c = sizeof(float);
	do
	    *p++ = *s++;
	while (--c);
}

/*
 * load double number
 */
 
#ifdef lddbl
#undef lddbl
#endif /* lddbl */

double lddbl(p)
register char *p;
{
	double fval;
	register char *f;
	register int c;

	f = (char *) &fval;
	c = sizeof(double);
	do
	    *f++ = *p++;
	while (--c);
	return(fval);
}

/*
 * store double number
 */

#ifdef stdbl
#undef stdbl
#endif /* stdbl */

stdbl(f,p)
double f;
register char *p;
{
	register char *s;
	register int c;

	s = (char *) &f;
	c = sizeof(double);
	do
	    *p++ = *s++;
	while (--c);
}
#endif /* NOFLOAT */

#ifdef NOLONGMULT
/*
 * long multiply
 */

long longmult(x,y)
long x,y;
{
	long z;

	for (z=0; y!=0 && x!=0; x<<=1, y>>=1)
	    if (y&1) z += x;

	return(z);
}
#endif /* NOLONGMULT */
