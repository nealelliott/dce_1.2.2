/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: binmov.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:13  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:05:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:09  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:33  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:12  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:20:55  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:51:01  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:18:38  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:57:21  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:33:50  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:14  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:15:13  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:12:57  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: binmov.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:25 $";
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

/******************************************************************\
* 								   *
* asn1_binmov(z,s,m,l)						   *
* 								   *
* Move an asn1 encoded string into a character string.		   *
* 	z= points to the TYPE field of an ASN-1 octet string.	   *
* 	s= points to the target character string.		   *
* 	l= will contain the number of bits in the string ``s''.	   *
* Returns a pointer to the next byte following the value field .   *
* Note that the string ``s'' is NOT null terminated.		   *
* 								   *
\******************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_binmov(z,zm,s,l)
asn1 z, zm;
unsigned char * s;
int *l;
{	asn1 z0, z2, z3;
	unsigned char last_byte;
	int lz;

	*l = 0;
	if ((z=asn1_skiptype(z0=z,zm))==0) return(0);
	if ((z=asn1_length(z,zm,&z2))==0) return(0);
	if (((*z0)&32) == 0){
		if (z2 == 0 || z2 > zm) return(0);
		last_byte = *z++;
		ASN1_MOVE(z,s,*l = (z2 - z));
		z = z2;
	}else{
		*l =0;
		if (z2){
			if(z2 > zm) return(0);
			zm = z2;
		}
		while (z < zm && *z){
			if (*z != 3) return(0);
			if ((z = asn1_length(z+1,zm,&z3))==0 || z3 == 0)
				return(0);
			if (z3 == z2 || *z3 == 0) last_byte = *z++;
			ASN1_MOVE(z,s + *l,lz = (z3 - z));
			*l += lz; z = z3;
		}
		if (z2 && z != z2) return(0);
		if (z2 == 0){
			if (z+1 < zm && z[0] == 0 && z[1] == 0)
				z += 2;
			else	return(0);
		}
	}
	if (last_byte > 8) return(0);
	*l = (*l)*8 - last_byte;
	return(z);
}

