/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: streal.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:05  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:18  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:14  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:47:56  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:12  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:47:13  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:27  keutel
 * 	creation
 * 	[1994/03/21  13:16:41  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: streal.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:17 $" } ;
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
static char *sccsid = " @(#) streal.c 1.1 93/11/29  ST_MAVROS";
/****************************************************************************\
* 									     *
* Encoding and decoding of ``REAL'' values.				     *
* 									     *
* The X.209 standard defines a very peculiar ``flexible'' encoding for 	     *
* real values. Null values have a length of zero; otherwise, the first 	     *
* byte contains:							     *
* 									     *
* 	bit 8: 1 => binary;						     *
* 	bit 8 = 0 and 7 = 0 => decimal value;				     *
* 	bit 8 = 0 and 7 = 1 => Special Real value.			     *
* If binary encoding is used, then the mantissa is encoded as:		     *
* 	M = S.N.2**F	0 <= F < 4					     *
* The bit 7 of the first byte encodes the SIGN ``S''.			     *
* The bits 6 and 5 encode the the value of the base ``B'': 0 => 2, 1=>8;     *
* 2=>16.								     *
* The bits 4 and 3 define the scaling factor ``F''.			     *
* The bits 2 to 1 define the length of the exponent:			     *
* 	00:	single byte integer.					     *
* 	01:	two bytes integer.					     *
* 	10:	three bytes integer.					     *
* 	11:	next byte is length of exponent -- in bytes.		     *
* The remaining bytes encode N as an unsigned binary number.		     *
* The special values include ``+ infinity'' and ``- infinity'': 0x40, 0x41.  *
* 									     *
* There is a possibility of decimal encoding, i.e. a prefix:		     *
* 	``000000??''							     *
* Where the last two bits specify the form of encoding. We use this 	     *
* encoding when we don't have a precise idea of the actual value of the	     *
* field; this should be precised somehow in the future, as we may well not   *
* be following completely the standard...				     *
* 									     *
* If the ``IEEE REAL'' bit is set, then we assume that the ``double''	     *
* elements are encoded according to the ANSI-IEEE Std 754-1985:		     *
* 	IEEE Standard for						     *
* 	Binary Floating Point Arithmetic.				     *
* The standard defines the double as being made of:			     *
* 	. 1 bit sign ``s''.						     *
* 	.11 bits exponent ``e''.					     *
* 	.52 bits float value ``f''.					     *
* The value would be computed as:					     *
* 	if e = 2047 and f != 0 then V:= NaN.				     *
* 	if e = 2047 and f = 0 then V:= (-1)**s . Infinite		     *
* 	if 0 < e < 2047 then V := (-1)**s. 2**(e-1023).(1.f)		     *
* 	if e = 0 and f != 0, then V:= (-1)**s.2**-1022.(0.f)		     *
* 	if e = 0 and f = 0, then V:= (-1)**s.0	(zero)			     *
* This enables some optimisations...					     *
* 									     *
\****************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>
#include <math.h>
#ifndef HUGE_VAL
#define HUGE_VAL HUGE
#endif

#ifndef IEEE_REAL
int PASCAL asn1st_realcod(
STOREHANDLE hdl,
double v)
{
	register s,t;
	int e;
	double f;
	unsigned char tt[8];
	char dbst[16];
	char *z = dbst;
	int sterr;

	sterr = ro0063_sterror(hdl);
	if (sterr) return sterr;
	if (v == 0)
		*z++ = 0; 
	else {
	    if (v < 0){
		    s = 1; v = -v;
	    }else   s = 0;
	
	v = frexp(v, &e);
	e -= 53;
	if (e < 128 && e >= -128){
		*z++ = 9;
		*z++ = 128 | (s<<6);
		*z++ = e;
	}else{
		*z++ = 10;
		*z++ = 129 | (s<<6);
		*z++ = (e>>8);
		*z++ = e;
	}
	for (s=0, t=5; s<7 && v != 0; t=8, s++){
		v = ldexp(v,t);
		f = floor(v);
		v -= f;
		*z++ = (int)f;
	}
	for (;s<7;s++){
		*z++ = 0;
	}
     }
     sterr = ro0068_stputm(hdl,dbst,(long)(z - dbst));
     return (sterr);
}
#else
int PASCAL asn1st_realcod(
STOREHANDLE hdl,
double v)
{
	asn1 t;
	register s,e,f;

	char dbst[16];
	char *z = dbst;
	int sterr;

	sterr = ro0063_sterror(hdl);
	if (sterr) return sterr;
	if (v == 0){
		*z++ = 0; }
	else {
	t = (asn1) &v;
	s = (*t >> 7);
	e = (*t++ & 127);
	e = (e << 4) | ((*t >>4)&15);
	if (e == 2047){	/* We ignore NaNs !!!! */
		*z++ = 1; *z++ = (0x40)|s; return(z);
	}
	if (e == 0){
		f = 0; e = -1075;}
	else{	f = 1; e -= 1075;}
	if (e < 128 && e >= -128){
		*z++ = 9;
		*z++ = 128 | (s<<6);
		*z++ = e;
	}else{
		*z++ = 10;
		*z++ = 129 | (s<<6);
		*z++ = (e>>8);
		*z++ = e;
	}
	*z++ = (f)? (*t&15)|16 : (*t&15); t++;
#if defined(ANYBOUNDARY) && defined(BIGENDIAN)
	*(short *)(z) = *(short *)(t);
	*(long *)(z+2) = *(long *)(t+2);
	z += 6;
#else
	*z++ = *t++;
	*z++ = *t++;
	*z++ = *t++;
	*z++ = *t++;
	*z++ = *t++;
	*z++ = *t;
#endif
	}
	sterr = ro0068_stputm(hdl,dbst,(long)(z-dbst));
	return (sterr);
}
#endif /* !IEEE_REAL */

double PASCAL asn1st_realval(
STOREHANDLE hdl,
long  endpos)
{	double v=0;
	int s, e, f, b, first;
	char buf[128];
	char * z = buf, *zmm;
	long  l,inpos;
	int stop;

	if (ro0063_sterror(hdl)) return(0);
	inpos = ro0076_stwhere(hdl);
	l = (long) ro0070_stgetc(hdl);
	if ((l == 0) || (inpos + l) >= endpos){
		return(0);
	}
	zmm = buf + l;

	if (l > 127) { l = 127;
		       stop = 1; }
	else stop = 0;

	/* Read encoded real into buffer */
	ro0071_stgetm(hdl, buf, l);
	buf[l] = 0;
	if ((*z&128) == 0){
		/* decimal chars coding. */
		if ((*z&64) == 0){
			sscanf((char *)(buf + 1),"%lg",&v);
			return(v);
		}else	return((*z&1)?-HUGE_VAL:HUGE_VAL);
	}

	first = *z & 0xFF; z++;
	s = first&64;
	switch((first >> 4)&3){
	case 0: b = 1; break;
	case 1: b = 3; break;
	default: b = 4; break;
	}
	f = (first >> 2)&3;
#ifdef ASN1_CHAR_IS_UNSIGNED
	{
	register l=first&3;


	if (l == 3){
		if ((l = (*z++)) > 1)
			l -= 2;
		else	return(0);
	}
	if ((e = *z++) > 128)
		e = -(256 - e);
	while (l--) e = (e<<8) | *z++;
	}
#else
	switch (first & 3){
	case 0:	e = *((char *)z++); break;
	case 1: e = *((char *)z++); e = (e<<8)|*z++; break;
	case 2: e = *((char *)z++); e = (e<<8)|*z++; e = (e<<8)|*z++; break;
	case 3: l = (*z++) -1; e = *((char *) z);
		while (l--) e = (e<<8) | *z++;
	}
#endif
	v = 0;
	while (z < zmm){
		v = ldexp(v,8) + (double)(*z++);
	}
	if ((e = e*b+f)!=0)
	    v = ldexp(v,e); /* no equality in if statement */
	if (s) v = -v;
	return(v);
}


/*********************************\
* 				  *
* Hash and comparaison routines.  *
* 				  *
\*********************************/

/* not used jet */
/*
int PASCAL asn1_real_cmp (
double a,
double b)
{
	return((a == b)?0:((a<b)?-1:1));
}

int PASCAL asn1_real_hash (
register h,
double a)
{	int exposant;

	h += (int) floor( ldexp( frexp(a,&exposant), 32));
	h += exposant;
	return(h);
}
	
*/
