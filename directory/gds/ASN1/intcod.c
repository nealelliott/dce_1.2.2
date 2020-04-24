/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: intcod.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:33  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:46  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:45  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:45  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:24:09  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:53:19  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:32:23  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:59:44  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:35:09  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:08  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:17:27  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:18:29  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: intcod.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:42 $";
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

/****************************************************************\
* 								 *
* asn1_intcod(z,r)						 *
* 								 *
* Encoding the length and value components of an ASN-1 integer.	 *
* 	z= the field where to encode.				 *
* 	r= the integer to encode.				 *
* Returns a pointer to the following byte.			 *
* 								 *
\****************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_intcod(z,r)
register asn1 z;
register int r;
{
	if (r < 128){
		if (r >= -128){	*z++ =1; *z++ = r;}
		else if (r >= -0x8000) {
			*z++ = 2; *z++ = (r>>8); *z++= r;}
		else if (r >= -0x800000) {
			*z++ = 3; *z++ = r>>16;
			*z++ = (r>>8)&0xFF; *z++ = r&0xFF;}
		else{	*z++ = 4; *z++ = r>>24; *z++ = (r>>16)&0xFF;
			*z++ = (r>>8)&0xFF; *z++ = r&0xFF;}
	}else if (r < 0x8000) {
		*z++ = 2; *z++ = r>>8; *z++ = r&0xFF;}
	else  if (r < 0x800000) {
		*z++ = 3; *z++ = r>>16;
		*z++ = (r>>8)&0xFF; *z++ = r&0xFF;}
	else{	*z++ = 4; *z++ = r>>24; *z++ = (r>>16)&0xFF;
		*z++ = (r>>8)&0xFF; *z++ = r&0xFF;}
	return(z);
}

