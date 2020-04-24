/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: type_cod.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:01  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:08:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:36  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:49:05  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:34  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:29:16  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:56:33  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:52:53  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:03:09  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:10:38  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:34  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:20:21  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:21:36  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: type_cod.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:07 $";
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

/**********************************************************\
* 							   *
* asn1_type_cod(z,m,n,s)				   *
* 							   *
* Coding of the ASN-1 ``type'' field.			   *
* 	z= an area where to encode the field.		   *
* 	m= the ``mode'' or ``class'' of the identifier.	   *
* 	n= the type number in the class.		   *
* 	s= the ``structure'' bit.			   *
* Returns a pointer to the next byte.			   *
* 							   *
\**********************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_type_cod(z,m,n,s)
register asn1 z;
int m,n,s;
{	unsigned k,l=0;
	if (((unsigned)n) < 31){
		*z++ = (m<<6)|(s<<5)|n;
	}else{
		*z++ = (m<<6)|(s<<5)|31;
		k = (unsigned)n; 
		while(k){ 
			l++; k = k>>7;}
		while (l--){
			if (l)
				*z++ = 128 | (((unsigned)n>>(7*l))&127);
			else	*z++ = n&127;
		}
	}
	return(z);
}
