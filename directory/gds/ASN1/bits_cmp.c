/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: bits_cmp.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:15  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:05:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:15  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:34  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:15  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:21:20  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:51:17  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:20:10  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:57:37  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:33:59  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:20  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:15:26  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:17:27  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: bits_cmp.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:27 $";
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

/************************************************************\
* 							     *
* asn1_bits_cmp						     *
* 							     *
* Compares two `L,V' fields, where L is the number of BITS.  *
* Returns 0 if the two fields are identical, -1 otherwise.   *
* 							     *
\************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

int asn1_bits_cmp (v1,l1,v2,l2)
asn1 v1, v2;
int l1, l2;
{	register m,r,l;

	if (l1 != l2)
		return (-1);
	l = (l1>>3);
	if (l && ASN1_CMP((char *)v1,(char *)v2,l))
		return(-1);
	if (r=(l1 - (l<<3))){
		r = 8-r; m=0;
		while (r--) {m = m<<1; m |= 1;}
		return(((v1[l]|m) != (v2[l]|m))?-1:0);
	}else	return(0);
}

int asn1_flags_cmp (v1,l1,v2,l2)
int *v1, l1, *v2, l2;
{
	return(asn1_bits_cmp ((asn1)v1,l1,(asn1)v2,l2));
}
