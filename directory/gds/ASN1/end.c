/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: end.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:22  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:27  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:38  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:23  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:22:19  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:52:01  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:24:49  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:58:17  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  16:34:22  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:38  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  16:55:45  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  14:42:41  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:16:02  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:17:48  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: end.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:32 $";
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
* MAVROS encode ASN-1 components by reserving a single byte for the length  *
* field, and then coding the value. The actual length of the component is   *
* only known at the end of the coding; at that stage, MAVROS generate a     *
* call to ``asn1_regular_end()'' if the unique encoding (see X.509) is not  *
* requested.When the unique encoding is requested, MAVROS generate a call   *
* to either ``asn1_defined_end'' or ``asn1_end_set'' if the component was   *
* of type ``SET'' or ``SET OF''.					    *
* 									    *
* The global variables ``asn1_end'' and ``asn1_end_set'' are not used any   *
* more. They remain defined for compatibility sake.			    *
* 									    *
\***************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_regular_end(z1,z0)
register asn1 z1, z0;
{	register l;

	if ((l = (z1 - z0) -1) < 128)
		*(z0) = l;
	else{	*(z0) = 128;
		*z1++ = 0; *z1++ = 0;
	}
	return(z1);
}

asn1 (*asn1_end) (asn1, asn1) = asn1_regular_end;
asn1 (*asn1_end_set) (asn1, asn1) = asn1_regular_end;

void asn1_std_mode_on (void)
{
	asn1_end = asn1_regular_end;
	asn1_end_set = asn1_regular_end;
}
