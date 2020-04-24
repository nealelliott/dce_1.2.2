/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: liblw.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:40  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:04  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:51  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:56  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:26:02  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:54:07  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:39:14  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:00:35  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:35:58  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:39  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:18:38  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:19:06  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: liblw.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:49 $";
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

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_boolean_lwc(z,x)
asn1 z;
int x;
{
	*z++ = (x != 0);
	return(z+1);
}

/* ARGSUSED */
asn1 asn1_boolean_lwd(z,zm,x)
asn1 z, zm;
int * x;
{
	*x = *z++;
	return (z);
}

asn1 asn1_int_lwc (z,x)
asn1 z;
int x;
{
	ASN1_INTLW(z,x);
	return(z+4);
}

/*ARGSUSED*/
asn1 asn1_int_lwd(z,zm,x)
asn1 z,zm;
int *x;
{
	*x = ASN1_LWINT(z);
	return(z+4);
}

asn1 asn1_bits_lwc(z,x,l)
asn1 z,x;
int l;
{	ASN1_INTLW(z,l);
	l = (l+7)>>3;
	z += 4; 
	ASN1_MOVE(x,z,l);
	return(z+l);
}
/*ARGSUSED*/
asn1 asn1_bits_lwd(z,zm,x,l)
asn1 z,zm, *x;
int *l;
{	*l = ASN1_LWINT(z);
	z += 4;
	*x = z;
	return (z + ((*l +7)>>3));
}

asn1 asn1_flags_lwc(z,x,l)
asn1 z,x;
int l;
{	ASN1_INTLW(z,l);
	l = (l+7)>>3;
	z += 4; 
	ASN1_MOVE(&x,z,l);
	return(z+l);
}

asn1 asn1_flags_lwd(z,zm,x,l)
asn1 z, zm;
int *x, l;
{	int ll;
	asn1 xx;
	z = asn1_bits_lwd(z,zm,&xx,&ll);
	if (ll > l) ll = l;
	if (ll)
		ASN1_MOVE(xx,x,(ll +7)>>3);
	else	*x = 0;
	return(z);
}

asn1 asn1_octets_lwc(z,x,l)
asn1 z, x;
int l;
{	ASN1_INTLW(z,l); z+= 4;
	ASN1_MOVE(x,z,l);
	return(z+l);
}

/*ARGSUSED*/
asn1 asn1_octets_lwd(z,zm,x,l)
asn1 z, zm, *x;
int *l;
{
	*l = ASN1_LWINT(z); z+= 4;
	*x = z;
	return(z+ (*l));
}

asn1 asn1_chars_lwd(z,zm,s)
asn1 z, zm;
char *s;
{	int ll;
	asn1 x;

	z = asn1_octets_lwd(z,zm,&x,&ll);
	if (ll)
		ASN1_MOVE(x,s,ll);
	s[ll]=0;
	return(z);
}



	
