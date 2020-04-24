/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: length.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:38  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:59  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:49  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:53  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:24:56  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:53:51  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:35:28  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:00:17  marrek]
 * 
 * Revision 1.1.4.3  1993/02/02  10:07:56  marrek
 * 	fix for OT 6572
 * 	[1993/02/01  17:40:33  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:35:32  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:21  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:18:05  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:18:47  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: length.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:47 $";
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

/*********************************************************************\
* 								      *
* asn1_length(z,zm,z2)						      *
* 								      *
* Decode the asn1 ``length'' field.				      *
* 	z= a pointer to the length field.			      *
* 	zm= a pointer to the last byte in the buffer		      *
* 	z2= a return argument. Either a pointer to the first byte     *
* 		following the ``value'' field, or zero if the	      *
* 		length is undefined.				      *
* Returns a pointer to the first byte of the value field, or a null   *
* pointer in case of error. The following errors are detected:	      *
* 								      *
* . ASN1_ERR_TAG	The tag length exceeds the buffer limit.      *
* . ASN1_ERR_LLEN	The length's length exceeds the buffer limit. *
* . ASN1_ERR_LEN	The component size exceeds the buffer limit.  *
* 								      *
\*********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_length(z,zm,z2)
register asn1 z, zm, * z2;
{	register k,l;

	if (z == 0)
		return(0);
 	if (z >= zm){
		ASN1_CKERR(ASN1_ERR_TAG,z);
		return(0);
	}
	if ((l = *z++) & 0x80){
		if (k = (l&0x7F)){
			l = 0;
			if (z+k > zm){
				ASN1_CKERR(ASN1_ERR_LLEN,z);
				return(0);
			}
			while (k--) l = (l<<8)+ *z++;
		}else{	*z2 = 0; return(z);}
	}
	*z2 = z + l;
	if (*z2 > zm || *z2 < z ){
		ASN1_CKERR(ASN1_ERR_LEN,z);
		return(0);
	}
	return(z);
}
/**************************************************************\
* 							       *
* asn1_close(z1,z2,limit)				       *
* 							       *
* Tests that the component is correctly terminated.	       *
* Detect the following errors:				       *
* 							       *
* . ASN1_ERR_SHORT	The value is smaller than expected.    *
* . ASN1_ERR_BOUND	The EOC is not within the limits       *
* . ASN1_ERR_EOC		The length of EOC is not 0     *
* 							       *
\**************************************************************/
asn1 asn1_close(z1,z2,limit)
register asn1 z1, z2, limit;
{
	if (z1 == 0) return(z1);
	if (limit){
		if (z1 != z2){
			ASN1_CKERR(ASN1_ERR_SHORT,z1);
			return(0);
		}
		return(z1);
	}else{
		if ((z2 - z1) < 2){
			ASN1_CKERR(ASN1_ERR_BOUND,z1);
			return(0);
		}
		if (z1[0] || z1[1]){
			ASN1_CKERR(ASN1_ERR_EOC,z1);
			return(0);
		}
		return(z1+2);
	}
}

/*************************************************************************\
* 									  *
* asn1_unstack(st, stn)							  *
* 	st  = the stack of end of components,				  *
* 	stn = the number of components in the stack.			  *
* Returns the uppermost non null limit, which can be used as ``end of	  *
* buffer'' after closing the component.					  *
* 									  *
\*************************************************************************/
asn1 asn1_unstack(st,stn)
register asn1 * st;
register int stn;
{	register asn1 z;

	do if(z = st[stn]) return(z);
	while (stn--);
	return((asn1)0);
}

#ifndef ASN1_SWAP_LONG
long asn1_swap_long(l)
long l;
{	register asn1 x = (asn1)&l;
	register y;

	y = (((((x[3]<<8)|x[2])<<8)|x[1])<<8)|x[0];
	return(y);
}
#endif

#ifndef ASN1_SWAP_SHORT
short asn1_swap_short(s)
short s;
{	register asn1 x = (asn1)&s;
	register y;

	y = (x[1]<<8)|x[0];
	return(y);
}
#endif
