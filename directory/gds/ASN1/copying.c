/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: copying.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:19  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:22  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:36  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:19  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:21:55  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:51:42  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:22:46  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:58:01  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:34:14  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:30  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:15:45  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:17:40  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: copying.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:30 $";
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

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

void asn1_octets_cpy(out_v,out_l,in_v,in_l)
asn1 * out_v, in_v;
int * out_l, in_l;
{
	if (*out_v = (asn1) asn1_malloc((unsigned)(*out_l = in_l)))
		ASN1_MOVE(in_v, *out_v, in_l);
}

void asn1_bits_cpy(out_v,out_l,in_v,in_l)
asn1 * out_v, in_v;
int * out_l, in_l;
{	register l;

	*out_l = in_l;
	l = (in_l + 7) >> 3;
	if (*out_v = (asn1) asn1_malloc((unsigned)(l)))
		ASN1_MOVE(in_v, *out_v, l);
}

void asn1_chars_cpy(out_v,in_v)
char ** out_v, * in_v;
{	register l;

	l = strlen((char *)in_v) + 1;
	if (*out_v = (char *) asn1_malloc((unsigned)(l)))
		ASN1_MOVE(in_v, *out_v, l);
}

