/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: lencod.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:36  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:50  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:48  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:49  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:24:32  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:53:35  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:33:54  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:00:01  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:35:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:15  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:17:45  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:18:39  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: lencod.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:44 $";
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

/***************************************\
* 				        *
* asn1_lencod(z,l)		        *
* 				        *
* Encodes the ``length'' field.	        *
* 	z= the area where to encode.    *
* 	l= the length to encode.        *
* Returns a pointer to the next byte.   *
* 				        *
\***************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_lencod(z,l)
register asn1 z;
register l;
{
	if (l < 128)
		*z++ = l;
	else if (l < 256){
		*z++ = 128 + 1; *z++ = l;}
	else if (l < 0x010000){
		*z++ = 128 + 2; *z++ = (l>>8)&0xFF; *z++ = l&0xFF;}
	else{	*z++ = 128 + 3; *z++ = (l>>16)&0xFF;
		*z++ = (l>>8)&0xFF; *z++ = l&0xFF;}
	return(z);
}
