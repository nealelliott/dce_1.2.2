/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isvalid.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:31  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:30:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:06  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:33  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:04:29  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:35:02  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:08:00  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:57:25  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:16:33  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:38:59  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:41:54  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: isvalid.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:27 $";
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
 *  Title:	isvalid1.c
 *  Description:
 *		Serial number validation routines
 *		(module 1 of 2 - gotten by splitting isvalidate.c)
 *
 ************************************************************************
 */



/*
 * This file contains serial number validation routine 
 */


#include <machine.h>
#include <sn.h>

#ifndef SINIX
static char snsave[] = INFBLANK;
char snnumber[80] = "Software Serial Number RDS#R000000";
short demoversion = 0; 
#else /* SINIX */
char siemens[] = "SIEMENS-";		/* TP 28.10.87 */
char snnumber[] = "@(#)!0#0@0%0&0@(#)";
#endif /* SINIX */
#ifdef PCDOS
char netname[17];
#endif

#ifndef NOVALIDATE

/*
 * validate -	checks to see if internally stored serial number is valid, 
 *    and if it is valid, whether it is a demo serial number or not.
 */

/* THESE NUMBERS ARE FOR INFORMIX ONLY */

#define MODULUS 81114323L	/* 37*61*433*83 */
#define INCREMENT 181L		/* relatively prime to MODULUS */

validate()

{
long isatol();
register int ii;
register long t, n;
char check[4+1];	/* computed check digits */
char string[20+1];	/* to hold serial number */
char digits[6+1];	/* to temporarily hold serial number digits */
int ret;

	stcopy(&snsave[SIZEIDENT], string);
	decode(string);

	/* get six digits */
	bycopy(&string[INFDIGITSTART], digits, 6);
	digits[6] = '\0';
	n = isatol(digits);

	t = (n + (long)string[0] 
		+ (long)string[INFDEMOPART]
		+ ((long)string[1] << 7) 
		+ ((long)string[2] << 14)) << 5;
	t = ( (((MODULUS+1L)*t) + INCREMENT ) & 0x7FFFFFFF) % MODULUS;

	t %= (26L * 26L * 26L * 26L);
	if (t<0) t = -t;

	for (ii=3; ii >= 0; ii--)
	    {
	    check[ii] = t % 26L;
	    t /= 26L;

	    check[ii] += 'A';			/* make alpha */
					    /* are check sums correct */
	    if (check[ii] != string[ii+INFSTRSIZE])
		return(WRONG);
	    }

	/* copy snsave to snnumber */	

	string[INFSTRSIZE] = 0;
	stcopy("Software Serial Number ", snnumber);
	stcat(string, snnumber);

	ret = (string[INFDEMOPART] == 'D') ? DEMO : REAL;

#ifdef PCDOS
	stcopy(&snsave[NETNAMEPOS], netname);
	decode(netname);
#endif

	return(ret);
}


/*
 * decode -	this function decodes the serial number into
 *		the correct form.
 */

static decode(string)

char *string;
	
{
	/* complement */
	rencode(string);
}

int (*funcptr)();		/* pointer to indirect function */

/*
 * snverify-	this function takes as arguments three pointers to functions.
 *     the first is a pointer to a function that should be executed for a
 *     real copy of the product, the second is a function that should be
 *     executed for a demo copy. The final function is one that should
 *     be executed if the serial number is not a valid one.  This function
 *     should do all necessary clean up and exit.
 *     The function validate() needs to be defined for each product.
 */

snverify(real, demo, wrong, msgroutine)

int (*real)();
int (*demo)();
int (*wrong)();
int (*msgroutine)();

{
	switch(validate())
		{
		case REAL:
			funcptr = real;	
			break;
		case DEMO:	
			funcptr = demo;
			break;
		case WRONG:
			(*msgroutine)();
			funcptr = wrong;
			break;
		}
}



/*
 * snchk -	this function pretends to be a serial number checksum
 *       computation.  It should be passed at least two integer arguments, 
 *	 which will be used in the giant scam.  Somewhere in all of this mess
 *       a call will be made to (*funcptr)() which will execute one of the
 *	 functions selected by snverify.
 */

snchk(one, two)

int one, two;

#define	AMODULUS	911*251*149*19
#define	AINCREMENT	241

	{
	/* lots of garbage */
	long t,n;
	char r,s,q;

	r = (char) one >> 13;
	s = (char) two;
	q = (char) two >> 3;
	n = one + (one << 8) - (two >> 3);
	t = (n + (long)r + ((long)s << 7) + ((long)q << 14)) << 5;
	t = ( (((AMODULUS+1L)*t) + AINCREMENT ) & 0x7FFFFFFF) % AMODULUS;

	/* this is it */
	(*funcptr)();

	/* more garbage */
	s = one >> 16;
	r = (char) two;
	r = (char) two >> 3;
	n = one + (one << 8) - (two >> 3);
	t = (n + (long)r + ((long)s << 4) + ((long)q << 14)) << 5;
	t = ( (((AMODULUS+1L)*t) + AINCREMENT ) & 0x7FFFFFFF) % AMODULUS;
	}

/*
 * encode even further 
 */

static int rencode(s)

register char *s;
{
	while (*s) *s++ ^= 0xDB;
}

/*
 * serial number routines
 */

real()
{
}

demo()
{
	demoversion = 1;
}

char *windz()
{
	return(&snsave[SIZEIDENT]);
}

wrong()
{
	exit(1);
}

static char badmsg[] =
	"\r\n\nThis C-ISAM has a bad serial number.\r\n\n";

snbadmsg()
{
	write(2, badmsg, stleng(badmsg));
}


long isatol(s)

register char *s;
{
	register long lval;

	lval = 0;				/* initial value	*/
	while (*s >= '0' && *s <= '9')
	    {					
	    lval = 10 * lval + (*s - '0');
	    s++;
	    }

	return(lval);
}

#endif /* NOVALIDATE */

