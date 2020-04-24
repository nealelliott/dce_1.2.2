/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: lendec.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:37  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:53  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:49  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:50  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:24:44  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:53:43  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:34:39  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:00:09  marrek]
 * 
 * Revision 1.1.4.3  1993/02/02  10:07:51  marrek
 * 	fix for OT 6572
 * 	[1993/02/01  17:39:52  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:35:26  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:18  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:17:57  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:18:43  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: lendec.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:45 $";
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

/***********************************************************\
* 							    *
* asn1_lendec(z,zm,l)					    *
* Decodes the length field.				    *
* 	z= the field to decode,				    *
* 	zm= the limit of the buffer,			    *
* 	l= will contain the decoded value.		    *
* Returns a pointer to the next byte.			    *
* 							    *
\***********************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_lendec(z,zm,l)
register asn1 z, zm;
int * l;
{	register i,k;

	if (z == 0 || z >= zm) return(0);

	if ((i = *z++) < 128)
		*l = i;
	else{
		for ( i &= 0x7f, k=0; i; i--){
			if (z >= zm) return(0);
			k = (k<<8) + *z++;
		}
		if (( z + k > zm ) || k < 0 ) {
			ASN1_CKERR(ASN1_ERR_LEN,z);
			return(0);
		}
		*l = k;
	}
	return(z);
}
