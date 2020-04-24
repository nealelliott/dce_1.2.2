/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isutil1.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:28  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:30:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:04  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:30  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:04:12  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:14:17  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:07:42  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:57:16  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:16:26  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:38:38  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:41:46  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: isutil1.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:25 $";
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
 *  Title:	isutil1.c
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

#ifndef NOFLOAT
static char fltnull[sizeof(double)] = {-1,-1,-1,-1,-1,-1,-1,-1};
#endif /* NOFLOAT */


#ifndef	NOFLOAT
/*
 * load float number - check for null first
 */

double ldfltnull(cptr, nullflag)
char * cptr;
short * nullflag;    /*  set to true if cptr represents null float */
{
    /*
     *  if all the characters = -1 then it's the null value
     */

    if (bycmpr(cptr, fltnull, sizeof(float)) == 0)
	{
	*nullflag = TRUE;
	return( (double) 0.0);
	}
    else
	{
	*nullflag = FALSE;
	return( ldfloat(cptr));
	}

}

/*
 * store float number -- if null store all -1's
 */

stfltnull( flt, ptr, nullflag )
double flt;
register char * ptr;
short  nullflag;
{
    if (nullflag)
	byfill(ptr, sizeof(float), -1);
    else
	stfloat( flt, ptr );
}

/*
 * load double number - check for null first
 */

double lddblnull(cptr, nullflag)
char * cptr;
short * nullflag;    /*  set to true if cptr represents null double */
{
    /*
     *  if all the characters = -1 then it's the null value
     */

    if (bycmpr(cptr, fltnull, sizeof(double)) == 0)
	{
	*nullflag = TRUE;
	return( (double) 0);
	}
    else
	{
	*nullflag = FALSE;
	return( lddbl(cptr));
	}

}

/*
 * store double number -- if null store all -1's
 */

stdblnull(dbl, ptr, nullflag)
double dbl;
register char * ptr;
short  nullflag;
{
/*  register int cc;	unused. TP */

    if (nullflag)
	byfill(ptr, sizeof(double), -1);
    else
	stdbl(dbl, ptr);
}
#endif	/* NOFLOAT */

#ifdef WX200
#undef SINIX
/* Sorry, NO assembler routines in WX200 for stchar ldchar byleng */
#endif /* WX200 */

#ifndef RIOS
#undef SINIX
/* Sorry, NO assembler routines in RIOS for stchar ldchar byleng */
#endif /* RIOS */

#ifndef SINIX	/* assembler routines. Schafer 15.09.88 */
/*
 * ldchar -- copy a fixed length blank padded field
 *	     to a null terminated string without trailing blanks
 */

ldchar(from, count, to)
    register char *from, *to;
    register int count;
{
    count = byleng(from, count);
    bycopy(from, to, count);
    to[count] = 0;
}

/*
 * stchar -- store character strings into fixed length field
 *	     padding with trailing blanks if needed
 */

stchar(from, to, count)
    register char *from, *to;
    register int count;
{
    if (from)
	while (--count >= 0)
	    *to++ = (*from ? *from++ : ' ');
    else
	byfill(to, count, ' ');
}

/*
 * byleng -- get length of string which is blank padded
 *	     i.e. strip trailing blanks
 */

byleng(beg, cnt)
    register char *beg;
    int cnt;
{
    register char *end = beg + cnt;

    while (end > beg && end[-1] == ' ')
	--end;
    return (end - beg);
}
#endif /* SINIX */
