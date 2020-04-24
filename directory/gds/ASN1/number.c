/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: number.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:47  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:16  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:55  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:08  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:27:02  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:54:49  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:43:03  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:01:22  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:36:29  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:55  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:19:13  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:19:28  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: number.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:55 $";
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

/************************************************************************\
* 									 *
* asn1_number(z,zm)							 *
* 									 *
* Returns the number of components in the ASN-1 field ``z'', within the	 *
* buffer limit ``zm''.							 *
* 									 *
\************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1_number(z,zm)
register asn1 z, zm;
{	asn1 z2;
	register i=0;

	if (z == 0 || z+2 > zm) return(0);
	if ((z = asn1_length(z,zm,&z2))==0)
		return(0);
	if (z2){
		if (z2 > zm) 
			return(0);
		else	zm = z2;
	}
	while (z && z < zm && *z){
		z = asn1_skip(z,zm);
		i++;
	}
	return(i);
}
