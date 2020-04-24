/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: skip.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:56  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:30  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:49:02  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:27  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:28:26  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:55:56  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:49:12  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:02:33  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:10:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:21  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:20:00  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:20:01  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: skip.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:03 $";
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

/*************************************************************************\
* 									  *
* asn1_skip(z,zm)							  *
* 									  *
* Skip over an ASN-1 ``T-L-V'' field.					  *
* 	z = a pointer to the field to be skipped.			  *
* 	zm= the limit of the current buffer.				  *
* Returns a pointer to the first byte following the field, or zero if an  *
* error was detected.							  *
* 									  *
\*************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_skip(z,zm)
register asn1 z, zm;
{	asn1 z2;
	int s = (z && z < zm)?(*z)&32:0;

	if ((z = asn1_skiptype(z,zm)) == 0)
		return(0);
	if ((z = asn1_length(z,zm,&z2)) == 0)
		return(0);
	if (z2){
		return(z2);
	}else if (s){
		while (z < zm && *z){
			if ((z = asn1_skip(z,zm))==0)
				return(0);
		}
		return(asn1_close(z,zm,z2));
	}else{
		ASN1_CKERR(ASN1_ERR_LDEF,z);
		return(0);
	}
}

/***************************************************************************\
* 									    *
*  asn1_check(z1,z2)							    *
*  Checks whether the area enclosed in [z1,z2[ is encoded according to	    *
*  ASN-1. It will return:						    *
* 	``z2'' if everything is correct.				    *
* 	``0'' in case of error.						    *
* 	In some cases, a value lower than z2 if the area pointed by z1	    *
* 	is encoded in ASN-1, but is shorted that z2.			    *
* 									    *
\***************************************************************************/

asn1 asn1_check(z,zm)
register asn1 z, zm;
{	asn1 z2;
	int s = (z && z < zm)?(*z)&32:0;

	if ((z = asn1_skiptype(z,zm)) == 0)
		return((asn1)0);
	if ((z = asn1_length(z,zm,&z2)) == 0)
		return((asn1)0);
	if (z2)	zm = z2;
	if (s){
		while (z < zm && *z){
			if ((z = asn1_check(z,zm))==0)
				return(0);
		}
		return(asn1_close(z,zm,z2));
	}else{
		if (z2 == 0)
			ASN1_CKERR(ASN1_ERR_LDEF,z);
		return(z2);
	}
}

/*********************************************************\
* 							  *
* asn1_size(z,zm)					  *
* 							  *
* Returns an upper bound of the length, in bytes, of the  *
* element pointed to by ``z''.				  *
* 							  *
\*********************************************************/
int asn1_size(z,zm)
register asn1 z, zm;
{	register asn1 z0;
	if (z && z < zm){
		if (z0 = asn1_skip(z,zm))
			return(z0-z);
		else	return(zm-z);
	}else	return(2);
}

/***********************************************************************\
* 								        *
* asn1_any_dec:							        *
* 								        *
* Is merely a redefinition of ``asn1_skip'', but we cannot do it by a   *
* simple macro, as we need an extra register.			        *
* 								        *
\***********************************************************************/
asn1 asn1_any_dec(z,zm,x,l)
asn1 z, zm, *x;
int *l;
{
	*l = (z = asn1_skip(*x = z,zm))?(z - *x):0;
	return(z);
}
