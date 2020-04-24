/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: real.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:55  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:28  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:49:01  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:25  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:28:15  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:55:47  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:48:18  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:02:24  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:36:59  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:17  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:19:54  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:19:56  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: real.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:01 $";
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
* 	``000000XX''							     *
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

#include <dce/asn1.h>
#include <math.h>
#ifndef HUGE_VAL
#define HUGE_VAL HUGE
#endif

#ifndef IEEE_REAL
asn1 asn1_realcod(z,v)
asn1 z;
double v;
{
	register s,t;
	int e;
	double f;

	if (v == 0){
		*z++ = 0; return(z);}
	else if (v < 0){
		s = 1; v = -v;
	}else	s = 0;
	
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
	return(z);
}
#else
asn1 asn1_realcod(z,v)
asn1 z;
double v;
{
	asn1 t;
	register s, e, f;


	if (v == 0){
		*z++ = 0; return(z);}
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
	return(z+6);
#else
	*z++ = *t++;
	*z++ = *t++;
	*z++ = *t++;
	*z++ = *t++;
	*z++ = *t++;
	*z++ = *t;
	return(z);
#endif
}
#endif /* !IEEE_REAL */

double asn1_realval(z,zm)
asn1 z, zm;
{	double v=0;
	int s, e, f, l, b, first;
	char buf[64];
	asn1 zmm;

	if ((l = *z++) == 0
	||  (zmm = z+l) > zm){
		return(0);
	}
	if ((*z&128) == 0){
		/* decimal chars coding. */
		if ((*z&64) == 0){
			if (l > 63) l = 63;
			(void) strncpy(buf,(char *)z+1,l-1);
			buf[l-1] = 0;
			sscanf(buf,"%lg",&v);
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
	if (e = e*b +f) v = ldexp(v,e);
	if (s) v = -v;
	return(v);
}

/*****************************************************************\
* 								  *
* Formal coding functions, when a macro cannot be easily used...  *
* 								  *
\*****************************************************************/

asn1 asn1_real_cod(z,m,n,x)
asn1 z;
int m,n;
double x;
{
	if (m == -1)
		*z++ = 9;
	else	z = asn1_type_cod(z,m,n&~ASN1_X509,0);
	return(asn1_realcod(z,x));
}

/***************************************************************\
* 							        *
* A preview on version 2 functions,			        *
* which have the immense advantage of being thourougly tested.  *
* (well, much more tested than the previous ones).	        *
* 							        *
\***************************************************************/

#ifdef IEEE_REAL
/*ARGSUSED*/
int * asn1_real_ftcd(z,zm,v)
int *z, **zm;
double v;
{
	*z++ = *(int *)&v;
	*z++ = *(((int *)&v) + 1);
	return(z);
}

double asn1_real_ftdec(z)
int *z;
{	double v;

	*(unsigned long *)&v = *(unsigned int *)z;
	*(((unsigned long *)&v) + 1) = *(unsigned int *)(z+1);
	return(v);
}
/*ARGSUSED*/
int * asn1_real_ftdc(z,zm,v)
int *z, **zm;
double *v;
{
	*(unsigned long *)v = *(unsigned long *)z;
	*(((unsigned long *)v) + 1) = 	*(unsigned int *)(z+1);
	return(z+2);
}
#else
/*ARGSUSED*/
int * asn1_real_ftcd(z,zm,v)
int *z, **zm;
double v;
{	int s, e;
	double f;

	if (s = ((v= frexp(v,&e)) < 0)) v = -v;

	if (e > 1023){
		*z++ = (s)? 0xFFF00000: 0x7FF00000;
		*z++ = 0;
	}else{
		if (e < -1023){
			v = ldexp(v,e+1023);
			e = -1023;
		}
		v = ldexp(v,20);
		f = floor(v);
		v -= f;
		e += 1023;
		if (s) e |= (1<<11);
		*z++ = (e<<20)|(int)f;
		v = ldexp(v,32);
		*z++ = floor(v);
	}
	return(z);
}

double asn1_real_ftdec(z)
int *z;
{	register unsigned int x = *((unsigned *)z++);
	double f;
	int s, e;

	if (s = (x & 0x80000000)) x -= s;
	e = x>>20;
	if (e == 2047)
		return((s)?-HUGE_VAL:HUGE_VAL);
	if (e)
		f = ldexp(0x100000 + x &0x000FFFFF,32);
	else if (f = x &0x000FFFFF)
		f = ldexp(f,32);
	f += *((unsigned *)z++);
	if (f)
		return((s)?-ldexp(f, e - 1075):ldexp(f, e - 1075));
	else	return(0);
}

int * asn1_real_ftdc(z,zm,v)
int *z, **zm;
double *v;
{
	*v = asn1_real_ftdec(z);
	return(z+2);
}
#endif

/*********************************************\
* 					      *
* Support for the LWTS syntax, now obsolete.  *
* 					      *
\*********************************************/

/*ARGSUSED*/
#ifdef IEEE_REAL
#ifndef  REAL_ALIGNMENT
void    asn1_reallw (z,d)
asn1 z;
double d;
{	asn1 x;
	int i;

	x = (asn1)&d;
	for (i=0; i<8; i++){
		*z++ = *x++;
	}
}

double  asn1_lwreal(z)
asn1 z;
{	asn1 x;
	double d;
	int i;

	x = (asn1)&d;
	for (i=0; i<8; i++){
		*x++ = *z++;
	}
	return(d);
}
#endif /* !REAL_ALIGNMENT */
#else 
void    asn1_reallw (z,d)
asn1 z;
double d;
{	asn1 x;
	int i, t[2],*tt;

	tt=t+2;
	(void)asn1_real_ftcd(t,&tt,d);
	x = (asn1)t;
	for (i=0; i<8; i++){
		*z++ = *x++;
	}
}
double  asn1_lwreal(z)
asn1 z;
{	asn1 x;
	int i, t[2];
	double d;

	x = (asn1)t;
	for (i=0; i<8; i++){
		*x++ = *z++;
	}
	d = asn1_real_ftdec(t);
	return(d);
}
#endif /* ! IEEE_REAL */
/*ARGSUSED*/
asn1 asn1_real_lwd(z,zm,x)
asn1 z,zm;
double *x;
{
	*x = ASN1_LWREAL(z);
	return(z+8);
}


asn1 asn1_real_lwc (z,x)
asn1 z;
double x;
{
	ASN1_REALLW(z,x);
	return(z+8);
}
