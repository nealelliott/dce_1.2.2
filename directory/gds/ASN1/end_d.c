/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: end_d.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:23  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:29  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:39  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:26  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:22:30  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:52:09  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:25:46  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:58:26  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:34:27  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:41  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:16:12  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:17:53  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: end_d.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:33 $";
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
* defined end:								    *
* 									    *
* This procedure can be substituted to ``regular_end'' as value of	    *
* ``asn1_end'' or ``asn1_set_end'' if defined encoding of length fields is  *
* necessary.								    *
* 									    *
\***************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>
#include <stdio.h>

asn1 asn1_defined_end(z1, z0)
register asn1 	z1, 	/* after the V field	*/
	   	z0;	/* start of the L field	*/
{	register l, d, r;


	if ((l = (z1 - z0) -1) < 128){
		*(z0) = l;
		return(z1);
	}
#ifdef DEBUG_END_DEF
	printf("Before asn1_defined:\n");
	*z0 = 0x80;
	z1[0] = 0;
	z1[1] = 0;
	asn1_dump(stdout,z0-1,z1+2);
#endif
	if (l < 256)
		d = 1;
	else if (l < 65536)
		d = 2;
	else if (l < 16777216)
		d = 3;
	else{	*(z0-1) = 128;
		*z1++ = 0; *z1++ = 0;
	}

	*z0++ = d|128;
	r = l;
	while(r--)
		z0[d+r] = z0[r];
	z1 += d;
	while (d--){
		z0[d] = l;
		l = l>>8;
	}
#ifdef DEBUG_END_DEF
	printf("After asn1_defined:\n");
	asn1_dump(stdout,z0-2,z1);
#endif
	return(z1);
}
