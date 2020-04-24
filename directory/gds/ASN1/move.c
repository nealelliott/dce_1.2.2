/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: move.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:45  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:13  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:54  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:06  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:26:50  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:54:41  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:42:20  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:01:13  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:36:24  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:52  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:19:04  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:19:23  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: move.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:54 $";
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

/**************************************************************************\
* 									   *
* asn1_move(zz,zm,z)							   *
* 									   *
* Copy an ASN1 encoded area, pointed to by ``zz'', and included in the	   *
* buffer limited by ``zm'', into an area pointed to by ``z''. Returns a	   *
* pointer to the byte following the area ``zz'', or a null value in case   *
* of error.								   *
* 									   *
\**************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_move(zz,zm,z)
register asn1 zz,zm,z;
{
	if (zm = asn1_skip(zz,zm))
		ASN1_MOVE(zz,z,zm-zz);
	return(zm);
}
