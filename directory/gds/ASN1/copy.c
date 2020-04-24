/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: copy.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:18  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:20  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:35  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:17  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:21:43  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:51:35  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:21:49  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:57:53  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:34:09  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:27  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:15:39  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:17:35  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: copy.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:29 $";
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
* asn1_copy(z,m,n,zz)							    *
* 									    *
* Copy an ASN1 encoded area pointed by ``zz'' into the area ``z''; the tag  *
* is changed to ``mode=m, number=n''. Returns a pointer to the following    *
* byte.									    *
* 									    *
\***************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_copy(z,m,n,zz,l)
register asn1 z, zz;
register int m, n, l;
{
	if (m != -1){
		asn1  zz0 = zz;

		z = asn1_type_cod(z,m,n&~ASN1_X509,(int)(*zz&0x20));
		if ((zz = asn1_skiptype(zz, zz+l)) == 0){
			*z++ = 0;
			return(z);
		}
		l -= (zz - zz0);
	}
	ASN1_MOVE(zz,z,l);
	return(z + l);
}
