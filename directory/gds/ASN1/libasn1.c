/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: libasn1.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:39  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:01  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:50  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:55  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:25:50  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:53:59  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:38:28  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:00:26  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:35:53  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:36  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:18:32  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:19:02  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: libasn1.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:48 $";
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

asn1 asn1_boolean_cod(z,m,n,x)
asn1 z;
int m,n,x;
{
	if (m == -1)
		*z++ = 1;
	else	z = asn1_type_cod(z,m,n&~ASN1_X509,0);
	*z++ = 1;
	*z++ = (x)?0xff:0;
	return(z);
}

asn1 asn1_boolean_dec(z,zm,x)
asn1 z, zm;
int *x;
{
	if (z = asn1_skiptype(z,zm)){
		if (*z != 1 || (z+2) > zm)
			ASN1_CKERR(ASN1_ERR_BOOL,z);
		*x = (*(z+1) != 0);
		return(z+2);
	}else	return((asn1)0);
}

asn1 asn1_int_cod(z,m,n,x)
asn1 z;
int m,n,x;
{
	if (m == -1)
		*z++ = 2;
	else	z = asn1_type_cod(z,m,n&~ASN1_X509,0);
	return(asn1_intcod(z,x));
}


asn1 asn1_bits_cod(z,m,n,x,l)
asn1 z, x;
int m,n,l;
{
	if (m == -1)
		*z++ = 3;
	else	z = asn1_type_cod(z,m,n&~ASN1_X509,0);
	return(asn1_bitcod(z,x,l));
}


asn1 asn1_flags_cod(z,m,n,x,l)
asn1 z;
int m,n,x,l;
{
	if (m == -1)
		*z++ = 3;
	else	z = asn1_type_cod(z,m,n&~ASN1_X509,0);
	return(asn1_bitcod(z,(asn1)&x,l));
}

asn1 asn1_octets_cod(z,m,n,x,l)
asn1 z, x;
int m,n,l;
{
	if (m == -1)
		*z++ = 4;
	else	z = asn1_type_cod(z,m,n&~ASN1_X509,0);
	z = asn1_lencod(z,l);
	ASN1_MOVE(x,z,l);
	return(z + l);
}

asn1 asn1_null_cod(z,m,n)
asn1 z;
int m,n;
{
	if (m == -1)
		*z++ = 5;
	else	z = asn1_type_cod(z,m,n&~ASN1_X509,0);
	*z++ = 0;
	return(z);
}


asn1 asn1_null_dec(z,zm)
asn1 z, zm;
{
	if (z = asn1_skiptype(z,zm)){
		if (*z != 0 || (z+1) > zm)
			ASN1_CKERR(ASN1_ERR_NULL,z);
		return(z+1);
	}else	return((asn1)0);
}
