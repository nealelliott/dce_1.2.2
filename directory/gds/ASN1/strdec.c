/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: strdec.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:00  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:08:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:34  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:49:04  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:33  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:29:02  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:56:25  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:51:51  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:03:00  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:10:33  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:31  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:20:16  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:21:31  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: strdec.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:06 $";
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

/*******************************************************************\
* 								    *
* asn1_strdec(z,s,m)						    *
* Move an ASN-1 encoded string into a null terminated string.	    *
* 	z= pointer to the LENGTH field of the ASN-1 string.	    *
* 	s= pointer to the C string.				    *
* 	m= format of ``z'': primitive(0) or structured		    *
* Returns a pointer to the next byte in ``z''.			    *
* 								    *
\*******************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_strdec(z,zm,s,m)
asn1 z,zm;
register char * s;
register int *m;
{	int l;

	z = asn1_octmov(z,zm,(asn1)s,&l);
	s[l] = 0;
	return(z);
}
