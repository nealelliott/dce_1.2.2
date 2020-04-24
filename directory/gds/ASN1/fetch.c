/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fetch.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:27  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:37  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:42  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:34  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:23:22  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:52:41  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:28:52  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:59:00  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:34:45  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:54  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:16:52  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:18:11  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: fetch.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:37 $";
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
* asn1_fetch:								   *
* 									   *
* fetch an ASN1 object from the ``input'' file. This is a recursive	   *
* function. It returns -1 in case of errors, the length of the data	   *
* otherwise. The pointer "b" points to the beginning of a reallocatable	   *
* buffer; the integer "l" contains the length of that buffer; "b" and "l"  *
* may be reallocated by the routine. The data are stored at location "b +  *
* d", the integer variable "d" being lower than "l".			   *
* 									   *
\**************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1_fetch(input,b,l,d)
FILE * input;
asn1* b;
int *l, d;
{
	int ltot, c, ll, lll;
	asn1 z, z0;

	/* if not enough space, reallocate */
	if ((*l - d) < 128)
		*b = (asn1) realloc((char *)*b,(unsigned) (*l += 128));
	/* get the type */
	z0 = z = *b + d;
	if ((c = getc(input)) == EOF) return(-1);
	*z++ = c;
	if ((c & 31) == 31){
		do{
			if ((c = getc(input)) == EOF) return(-1);
			*z++ = c;
		} while (c & 128);
	}
	/* get the length */
	if ((ll = getc(input)) == EOF) return(-1);
	*z++ = ll;
	if (ll == 0x80){
		ltot = d + (z - z0);
		for (;;){
			if ((c = asn1_fetch(input,b,l,ltot)) < 0)
				return(c);
			else if ((*b)[ltot] == 0)
				return(c);
			else	ltot = c;
		}
	}
	if (ll & 0x80){
		lll = ll&0x7F;
		ll = 0;
		while (lll--){
			if ((c = getc(input)) == EOF) return(-1);
			*z++ = c;
			ll = (ll << 8) + c;
		}
	}
	ltot = d + ll + (z - z0);
	/* if not enough space, reallocate */
	if (ltot > *l)
		*b = (asn1 ) realloc((char*)*b, (unsigned)(*l = ltot));
	/* read the data */
	return((fread((char*)(*b + d + (z-z0)), 1, ll, input)<(size_t)ll)?-1:ltot);
}
