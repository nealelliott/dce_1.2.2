/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: bitcod.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:14  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:05:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:12  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:34  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:13  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:21:08  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:51:10  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:19:22  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:57:29  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:33:54  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:17  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:15:20  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:13:02  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: bitcod.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:26 $";
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
* asn1_bitcod(z,s,l)							    *
* 									    *
* Encode into z the length and value field of a ``bit string'' of	    *
* length ``l'' stored at the address ``s''.				    *
* 									    *
\***************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_bitcod(z,s,l)
asn1 z, s;
int l;
{
	int ll,remainder;

	if ((remainder = (l&0x07)) == 0)
		ll = l >> 3;
	else{	ll = (l>>3)+1;
		remainder = 8 - remainder;
	}
	z = asn1_lencod(z,ll+1);
	*z++ = remainder;
	ASN1_MOVE(s,z,ll);
	return(z+ll);
}

asn1 asn1_flagcod(z,x,l)
asn1 z;
int x, l;
{
	return(asn1_bitcod(z,(asn1)(&x),l));
}
