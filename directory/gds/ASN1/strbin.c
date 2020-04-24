/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: strbin.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:58  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:08:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:32  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:49:03  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:31  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:28:51  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:56:17  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:51:01  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:02:51  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:10:29  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:27  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:20:11  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:20:11  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: strbin.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:05 $";
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

/**********************************************************************\
* 								       *
* asn1_bits_dec(z,zm,x,l)					       *
* 								       *
* Pack a structured asn1 bit string.				       *
* 	z= pointer to the ASN1 encoded bit string		       *
* 	zm= pointer to the maximum allowed position after z	       *
* 	x= will contain a pointer to the packed string.		       *
* 	l= will contain the length (in bits) of the packed string.     *
* Returns a pointer to the next byte after the ASN-1 string.	       *
* 								       *
* In ASN-1, a bit string can be encoded following either the	       *
* ``primitive'' format, i.e. as a string of bytes, or following the    *
* ``structured'' format, i.e. as a sequence of strings. The latter     *
* case is hard to use for ``C'' programs. Hence, we will write some    *
* procedures to ``pack'' all the substring in a single one. We don't   *
* want to perform memory allocation, thus we must reuse the existing   *
* fields. For that, we will start by the last substring, and we will   *
* concatenate in front of it the preceeding substrings, resulting      *
* in a single string plus some unused bytes. The packing of bit	       *
* strings is very similar to that of octets strings, except that the   *
* first byte of the last component indicates ``the number of unused    *
* bits in the last byte''. Thus, we must remeber this byte, and	       *
* replace it in front of the packed bit string. Then, we will try to   *
* restore some ASN-1 signification to the preceeding bytes; they will  *
* be filled with the new length field, and possibly with some	       *
* null substrings.						       *
* 								       *
\**********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_bits_dec(z, zm, x,l)
asn1 z, zm;
unsigned char **x;
int * l;
{	asn1 z1, z2, z3, target, final;
	unsigned char first_byte;
	int i,j,k,l1,l2,ll;

	if ((z = asn1_skiptype(z1 = z,zm)) == 0) return(0);
	if ((z = asn1_length(z3=z,zm,&final)) == 0) return(0);

	if ((*z1&32) == 0){
		if (final == 0)
			ASN1_CKERR(ASN1_ERR_LDEF,z);
		*l = (((final -z) -1)<<3);
		*l -= (*z++&7);
		*x = z;
		return(final);
	}

	if ((target = final) == 0){
		target = z;
		while (target + 1 < zm && *target) 
			if ((target = asn1_skip(target,zm))==0)
				return(0);
		if (target + 1 < zm && target[0] == 0 && target[1] == 0)
			target += 2;
		else	ASN1_CKERR(ASN1_ERR_BOUND,target);
		final = target;
	}

	k = asn1_number(z,zm);
	for (j=0; j<(k-1); j++)
		if ((z = asn1_skip(z,final))==0) return(0);
	if (z >= zm || (*z)&32) return(0);
	if ((z = asn1_skiptype(z,final))==0) return(0);
	if ((z = asn1_length(z,final,&z2)) == 0) return(0);
	first_byte = *z++;
	if (z2 == target)	target = z;
	else while (z < z2)	*(--target) = *(--z2);
	for (i=0; i<(k-1); i++){
		z = z3;
		for (j=0; j<(k-1); j++){
			if ((*z)&32) return(0);
			z = asn1_skip(z,zm);
		}
		z = asn1_length(z+1,zm,&z2);
		if (z2 == 0) return(0);
		while (z < z2)	*(--target) = *(--z2);
	}
	*x = target;
	*l = (target - final)*8 - first_byte;
	*(--target) = first_byte;
	ll = target - final;

	if (ll < 128)
		l1 = 1;
	else	for(i= ll,l1=2;i>256;i=i>>8) l1++;

	if ((i = target - z3) < 128)
		l2 = 1;
	else	for(l2=2;i>128;i=i>>8) l2++;
	z = *x;
	if ((z-z3) > (l1 + l2)){
		if (ll < 128)
			*(--z) = ll;
		else{	for(i= ll;i<128;i=i>>8) *(--z) = i;
			*(--z) = l1 + 128;
		}
		*(--z) = 3;
		while ((z - z3) > (2 + l2)){
			*(--z) = 0; *(--z) = 3;}
	}else	*z1 &= ~32;

	if ((z - z3) == 1)
		*z3 = (final - z);
	else{
		*z3 = l2 = (z - z3 -1);
		*z3 |= 0x80;
		for(i=final-z; l2; i=i>>8, l2--) *(--z) = i;
	}
	return(final);
}

/*
Flags decoding
*/
/*ARGSUSED*/
asn1 asn1_flags_dec(z,zm,f,lf)
asn1 z, zm;
int *f, lf;
{	unsigned char * x;
	int lx;

	if (z = asn1_bits_dec(z,zm,&x,&lx)){
		if (lx == 0) *f = 0;
		else {
			lx = (lx+7)>>3;
			if (lx > sizeof(int))
			  {
			  ASN1_CKERR(ASN1_ERR_FLAGS,z);
			  return (0);
			  }
			ASN1_MOVE(x,f,lx);
		}
	}
	return(z);
}
