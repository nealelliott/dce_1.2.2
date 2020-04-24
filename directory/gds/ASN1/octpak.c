/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: octpak.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:50  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:23  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:58  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:14  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:27:39  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:55:15  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:45:17  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:01:47  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:36:44  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:06  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:19:34  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:19:41  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: octpak.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:58 $";
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

/***************************************************************************\
* 									    *
* In ASN-1, an octet string can be encoded following either the		    *
* ``primitive'' format, i.e. as a string of bytes, or following the	    *
* ``structured'' format, i.e. as a sequence of strings. The latter 	    *
* case is hard to use for ``C'' programs. Hence, we will write some	    *
* procedures to ``pack'' all the substring in a single one. We don't 	    *
* want to perform memory allocation, thus we must reuse the existing	    *
* fields. For that, we will start by the last substring, and we will	    *
* concatenate in front of it the preceeding substrings, resulting	    *
* in a single string plus some unused bytes. Then, we will try to	    *
* restore some ASN-1 signification to the preceeding bytes; they will	    *
* be filled with the new length field, and possibly with some		    *
* null substrings.							    *
* 									    *
\***************************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

/**************************************************************************\
* 									   *
* asn1_octpak(z,zm,target)						   *
* Moves the content of the ASN-1 string whose LENGTH is pointed		   *
* by ``z'' into a continuous field terminated by ``target''.		   *
* 									   *
* Note that the buffer limit ``zm'' is never tested. It is supposed that   *
* the procedure ``stroct'' provided thorough checking...		   *
* 									   *
\**************************************************************************/
asn1 asn1_octpak(z,zm,target)
register asn1 z,target;
asn1 zm;
{	register j;
	asn1 z0, z2;
	register asn1 z3;
	int i,k = asn1_number(z,zm),s;

	z0 = asn1_length(z,zm,&z2);
	for (i=0; i<k; i++){
		z = z0;
		for (j=0; j<i; j++)
			z = asn1_skip(z,zm);
		s =(*z&32);
		z = asn1_skiptype(z,zm);
		if (s){
			target = asn1_octpak(z,zm,target);
		}else{	z = asn1_length(z,zm,&z2);
			if ((z3=z2) == target)	target = z;
			else	while (z < z3)	*(--target) = *(--z3);
		}
	}
	return (target);
}
/***************************************************************************\
* 									    *
* asn1_stroct(z,x,l,m)							    *
* Pack a structured asn1 string.					    *
* 	z= pointer to the LENGTH component of the ASN1 string		    *
* 	x= will contain a pointer to the packed string.			    *
* 	l= will contain the length of the packed string.		    *
* 	m= will indicate whether the result is structured or not.	    *
* Returns a pointer to the next byte after the ASN-1 string.		    *
* 									    *
\***************************************************************************/
asn1 asn1_stroct(z,zm,x,l,m)
asn1 z, zm, *x;
int * l, * m;
{	asn1 z2, z3, target;
	int i,l1,l2;

	if ((z = asn1_length(z3=z,zm,&z2))==0) return(0);

	if ((target = z2)==0){
		if ((target = z) > zm - 1) return(0);
		while (*target){
			if ((target = asn1_skip(target,zm))==0
			|| target + 2 > zm)
				return(0);
		}
		target += 2;
		z2 = target;
	}

	*x = asn1_octpak(z,zm,target);
	*l = target - *x;
	if (*l < 128)
		l1 = 1;
	else	for(i= *l,l1=2;i>256;i=i>>8) l1++;
	if ((i = z2 - z3) < 128)
		l2 = 1;
	else	for(l2=2;i>128;i=i>>8) l2++;
	z = *x;
	if ((z-z3) > (l1 + l2)){
		*m = 1;
		if (*l < 128)
			*(--z) = *l;
		else{	for(i= *l;i<128;i=i>>8) *(--z) = i;
			*(--z) = l1 + 128;
		}
		*(--z) = 4;
		while ((z - z3) > (2 + l2)){
			*(--z) = 0; *(--z) = 4;}
	}else	*m = 0;
	while ((z - z3) > 128){
		*z-- = 0; *z-- = 4;}
	if ((z - z3) == 1)
		*z3 = (z2 - z);
	else{
		*z3 = l2 = (z - z3 -1);
		for(i=z2-z; l2; i=i>>8, l2--) *(--z) = i;
	}
	return(z2);
}

asn1 asn1_octets_dec(z,zm,x,l)
asn1 z, zm, *x;
int *l;
{	int s;
	asn1 z0;

	if (z = asn1_skiptype(z0=z,zm)){
		if (*z0 & 32){
			z = asn1_stroct(z,zm,x,l,&s);
			if (s == 0)	*z0 &= ~32;
			return(z);
		}else if (z = asn1_length(z,zm,&z0)){
			*x = z;
			if (z0 && z0 <= zm){
				*l = z0 - z;
				return(z0);
			}
		}
	}
	return((asn1)0);
}
