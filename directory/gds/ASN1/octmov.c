/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: octmov.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:49  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:20  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:57  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:12  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:27:26  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:55:05  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:44:31  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:01:38  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:36:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:02  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:19:28  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:19:36  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: octmov.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:57 $";
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
* asn1_octmov(z,s,m,l)							    *
* 									    *
* Move an asn1 encoded string into a character string.			    *
* 	z  = points to the TYPE field of an ASN-1 octet string.		    *
* 	zm = points to the end of the buffer.				    *
* 	s  = points to the target character string.			    *
* 	l  = will contain the number of bytes in the string ``s''.	    *
* Returns a pointer to the next byte in ``z'', or a null pointer in case    *
* of error.								    *
* 									    *
* Note that the string ``s'' is NOT null terminated.			    *
* 									    *
\***************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_octmov(z,zm,s,l)
asn1 z, zm;
asn1 s;
int *l;
{	asn1 z2;
	int lz = 0, m;

	*l = 0;
	m = (z < zm)?(*z&32):0;
	if ((z = asn1_skiptype(z,zm)) == 0) 
		return(0);
	if ((z = asn1_length(z,zm,&z2))==0) 
		return(0);
	if (m){
		if (z2)	zm = z2;
		while (z < zm && *z){
			if ((z = asn1_octmov(z,zm,s + *l,&lz)) == 0)
				return(0);
			*l += lz;
		}
		return(asn1_close(z,zm,z2));
	}else{
		if (z2 == 0)
			ASN1_CKERR(ASN1_ERR_LDEF,z);
		ASN1_MOVE(z,s,*l = z2-z);
		return(z2);
	}
}

asn1 asn1_strmov(z,zm,s)
asn1 z, zm;
char * s;
{
	int l;

	if (z = asn1_octmov(z,zm,(asn1)s,&l))
		s[l]=0;
	return(z);
}


asn1 asn1_chars_dec(z,zm,s)
asn1 z, zm;
char * s;
{	int l;

	z = asn1_octmov(z,zm,(asn1)s,&l);
	s[l] = 0;
	return(z);
}
