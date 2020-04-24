/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: utctime.c,v $
 * Revision 1.1.11.2  1996/02/18  19:38:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:03  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:08:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:37  root]
 * 
 * Revision 1.1.9.2  1994/05/10  15:49:06  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:37  marrek]
 * 
 * Revision 1.1.9.1  1994/02/22  15:29:28  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:56:42  marrek]
 * 
 * Revision 1.1.7.2  1993/08/11  11:53:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:03:18  marrek]
 * 
 * Revision 1.1.5.4  1992/12/31  17:10:40  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:37  bbelch]
 * 
 * Revision 1.1.5.3  1992/11/27  16:56:24  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  14:44:11  marrek]
 * 
 * Revision 1.1.5.2  1992/09/29  21:09:35  devsrc
 * 	[hinman@sni] - Check in merges from SNI
 * 	[1992/09/14  20:22:11  treff]
 * 
 * Revision 1.1.2.2  1992/06/01  20:20:26  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:21:40  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: utctime.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 19:38:08 $";
#endif

 
/* ****************************************************************** */
/*                                                                    */
/*   COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONSSYSTEME AG   */
/*                           AND INRIA                                */
/*   All rights reserved                                              */
/*                                                                    */
/*   This software is furnished under licence and may be used only    */
/*   in accordance with the terms of that licence and with the        */
/*   inclusion of the above copyright notice.                         */
/*   This software may not be provided or otherwise made available    */
/*   to, or used by, any other person. No title to or ownership of    */
/*   the software is hereby  transferred.                             */
/*                                                                    */
/* ****************************************************************** */

/****************************************************************************\
* 									     *
* Time conversion routines.						     *
* 									     *
* Conversion from the local time format to the ``UTC-TIME'' or		     *
* ``Generalized time'' formats, and back.				     *
* 									     *
* The UTCTime format is a character string:				     *
* 									     *
*      yymmddhhmnss+dhdm    -- positive delta				     *
*      yymmddhhmnss-dhdm    -- negative delta				     *
*      yymmddhhmnssZ        -- Zoulou format				     *
* 									     *
* The ``second'' component can optionally be omitted. The GeneralizedTime    *
* has the same format, except that the ``seconds'' can be completed by an    *
* arbitrary fraction, e.g. ``ss.xxx'' instead of ``ss'';another              *
* difference is that years will be noted over 4 digits, not two.             *
* 									     *
* The local time format is as described in the ``ftime'' system call:	     *
* 									     *
*      struct timeb							     *
*      {								     *
*           time_t   time;						     *
*           unsigned short millitm;					     *
*           short    timezone;						     *
*           short    dstflag;						     *
*      };								     *
* 									     *
* Where ``time'' is the system time since the beginning of the ``epoch''.    *
* The milliseconds are only taken when converting to ``generalized time''.   *
* 									     *
* Note that there is a slight difference between UNIX and ASN-1 	     *
* conventions; the UNIX timezone is in minutes westward of Greenwich, 	     *
* giving positive values in America, whilst the ASN-1 is the arithmetic	     *
* difference between the local time and the Greenwhich time, i.e. the exact  *
* opposite..								     *
* 									     *
* The computation underneath use an EPOCH constant, defined as the 	     *
* number of days between the 1 january 1901 and the start of the ``EPOCH''   *
* referred to by the ``ftime'' manual pages. The default value, 25202, is    *
* computed assuming that the actual epoch started the 1 january 1970.	     *
* 									     *
\****************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <ctype.h>
#include <dce/asn1.h>

static short qm[] = 
{31,  59,  90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
#ifndef EPOCH
#define EPOCH 25202
#endif

int asn1_text_time (char *, struct timeb *, int);
int asn1_text2time (char *, int, struct timeb *, int);

asn1_utc_time(z,x)
char * z;
struct timeb *x;
{	struct timeb y;

	y = *x;
	y.millitm = 0;
	return(asn1_text_time(z,x,0));
}

asn1_gen_time(z,x)
char * z;
struct timeb *x;
{
	return(asn1_text_time(z,x,1));
}

asn1_text_time(z,x,long_year)
char * z;
struct timeb *x;
int long_year;
{	int yy, mm, dd, hh, mn, ss, dh, dm, quadrat;
	char * z0 = z;

	ss = x->time - 60*x->timezone;
	dd = ss/(60*60*24);
	ss -= dd * (60*60*24);
	if (ss < 0){
		dd -= 1;
		ss += 60*60*24;
	}
	mn = ss / 60;
	ss -= 60*mn;
	hh = mn / 60;
	mn -= 60*hh;

	dd += EPOCH; /* Start 1 jan 1901, 0 h 00 */
	quadrat = dd / 1461;
	dd -= 1461*quadrat;
	if (dd == 1154){
		yy = 3; mm = 1; dd = 28;}
	else{
		if (dd > 1154) dd--;
		yy = dd/365;
		dd -= yy*365;
		for (mm=0;mm<11 && dd >= qm[mm]; mm++);
		if (mm)	dd -= qm[mm-1];
	}
	yy += 1 + 4*quadrat;
	if (long_year){
		yy += 1900;
		sprintf(z,"%.2d",yy/100);
		z += 2;
	}
	yy %= 100;

	sprintf(z,"%.2d%.2d%.2d%.2d%.2d%.2d",yy,mm+1,dd+1,hh,mn,ss);
	z += 12;
	if (x->millitm && long_year){
		sprintf(z,".%.3d",x->millitm);
		z += 4;
	}
	if (x->timezone == 0){
		*z++ = 'Z';
		*z = 0;
	}else{
		if ((dm = x->timezone) > 0)
			*z = '-';
		else{   *z = '+'; dm = -dm;}
		dh = dm/60;
		dm -= 60*dh;
		sprintf(z+1,"%.2d%.2d",dh,dm);
		z += 5;
	}
	return(z-z0);
}

#define TWODIGITS(z)  ((*(z) - '0')*10 + *(z+1) - '0')

asn1_time(z,l,x)
char * z;
int l;
struct timeb *x;
{
	return(asn1_text2time(z,l,x,0));
}

asn1_time_gen(z,l,x)
char * z;
int l;
struct timeb *x;
{
	return(asn1_text2time(z,l,x,1));
}

asn1_text2time(z,l,x,long_year)
char * z;
int l;
struct timeb *x;
int long_year;
{	int yy, mm, dd, hh, mn, ss, dh, dm, 
	quadrat, sdelta, pre, milli, div;


	x->time = x->millitm = x->timezone = x->dstflag = 0;
	if (long_year){
		if (l < 4) return (-1);
		yy = (TWODIGITS(z))*100 + TWODIGITS(z+2) - 1900;
		l -= 2;
		z += 2;
	}else{  if (l < 2) return (-1);
		yy = TWODIGITS(z);
	}
	yy--;
	if ( l < 10
	||  (yy < 1 || (long_year == 0 && yy > 99))
	||  (mm = TWODIGITS(z+2)) < 1 || mm-- > 12
	||  (dd = TWODIGITS(z+4)) < 1 || dd-- > 31
	||  (hh = TWODIGITS(z+6)) < 0 || hh > 24
	||  (mn = TWODIGITS(z+8)) < 0 || mn > 60) return(-1);
	if (l >= 12 && isdigit(z[10])){
		if ((ss = TWODIGITS(z+10)) < 0 || ss > 60) return(-1);
		z += 12; l -= 12;
	}else{	ss = 0; l -= 10; z+= 10;}

	quadrat = yy/4;
	yy -= 4*quadrat;
	
	if (mm){
		dd += qm[mm-1];
		if (yy == 3 && mm > 1) dd++;
	}

	dd += 365*yy + 1461*quadrat - EPOCH;
	x->time = ((dd * 24 + hh)*60 + mn)*60 + ss;

	if (l && *z == '.'){
		z++;
		for (pre=0, milli = 0, div = 1; 
		(pre < l) && isdigit(z[pre]); pre++){
			milli = milli*10 + z[pre] - '0';
			div = div * 10;
		}
		if (div < 1000)
			x->millitm = milli *(1000/div);
		else{	div /= 1000;
			x->millitm = milli /div;
		}
		z+= pre;
		l -= pre+1;
	}else	x->millitm = 0;

	sdelta = 1; dh = 0; dm = 0;
	if (l) switch (*z){
	case '+':
		sdelta = -1;
	case '-':
		z++; l--;
		if (l >= 2){
			dh = TWODIGITS(z);
			if (l >= 4)
				dm = TWODIGITS(z+2);
		}
	default: break;
	}
	x->time += 60*(x->timezone = sdelta * (60*dh + dm));
	x->dstflag = 0;
	return(0);
}
