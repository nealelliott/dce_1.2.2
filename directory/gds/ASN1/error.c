/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: error.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:26  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:35  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:41  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:32  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:23:10  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:52:33  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:28:07  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:58:52  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:34:40  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:50  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:16:41  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:18:06  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: error.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:36 $";
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

/*********************************************************************\
* 								      *
* Here we find all the error signalling routine:		      *
* 								      *
* asn1_errdec(z,n):	for BER,				      *
* 								      *
* which also appears under the names:				      *
* 								      *
* asn1_fltws_err(z,n):	for FTLWS (in fact, a macro),		      *
* asn1_input_err(z,n):	for TEXT (also a macro)..		      *
* 								      *
* All these routines declares that an error of type "n" was detected  *
* when parsing the location "z". The pointer "z" can be of type	      *
* "asn1" (BER), "int *" (FTLWS), or "char *" (TEXT); the macros for   *
* FTLWS and TEXT include the necessary casting.			      *
* 								      *
\*********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

int	asn1_diagnostic;
asn1	asn1_wrongbyte;

asn1 asn1_errdec(z,t)
asn1 z;
int t;
{
	if (z){
		asn1_wrongbyte = z;
		asn1_diagnostic = t;
	}
	return((asn1)0);
}
