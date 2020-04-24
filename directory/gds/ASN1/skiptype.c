/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: skiptype.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:57  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:31  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:49:02  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:29  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:28:38  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:56:09  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:50:00  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:02:42  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:10:25  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:24  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:20:06  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:20:06  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: skiptype.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:04 $";
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

/***********************************************************************\
* 								        *
* asn1_skiptype(z,zm)						        *
* 								        *
* Skip the type field: z= points to the type field to be skipped.  zm=  *
* points to the last byte in the buffer.  Returns a pointer to the      *
* following byte, or a null pointer in case of error. The following     *
* errors are detected:						        *
* 								        *
* . ASN1_ERR_TAG	The tag length exceeds the buffer limit.        *
* 								        *
\***********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_skiptype(z,zm)
register asn1 z, zm;
{
	if (z == 0) return((asn1)0);
	if (((*z++)&0x1F) == 31)
		while ((z < zm) && ((*z++)&0x80));
	if (z < zm)
		return(z);
	else{	ASN1_CKERR(ASN1_ERR_TAG,z);
		return(0);
	}
}
