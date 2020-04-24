/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: field_cmp.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:29  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:40  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:43  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:36  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:23:34  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:52:49  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:29:37  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:59:08  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:34:52  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:57  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:16:59  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:18:15  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: field_cmp.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:38 $";
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
* asn1_field_cmp							    *
* 									    *
* Compares two `L,V' fields. Returns 0 if the two fields are identical, -1  *
* otherwise.								    *
* 									    *
\***************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

int asn1_field_cmp (id1, id2)
asn1_field *id1, *id2;
{		
	if (id1->l != id2->l)
		return (-1);
	if (id1->l)
		return(ASN1_CMP((char *)(id1->v),(char *)(id2->v),id1->l));
	else	return(0);
}
