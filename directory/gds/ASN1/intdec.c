/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: intdec.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:34  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:48  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:46  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:47  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:24:21  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:53:27  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:33:08  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:59:52  marrek]
 * 
 * Revision 1.1.4.3  1993/02/02  10:07:47  marrek
 * 	fix for OT 6572
 * 	[1993/02/01  17:39:18  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:35:14  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:12  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:17:36  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:18:34  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: intdec.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:43 $";
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

/*****************************************************************\
* 								  *
* asn1_intdec(z,zm,r):						  *
* 								  *
* Transform an ASN-1 encoded value into a C 32 bits integer	  *
* 	z = a pointer to the ASN-1 LENGTH field.		  *
* 	zm= a pointer to the end of the buffer.			  *
* 	r = a pointer to the integer variable where the result	  *
* 	will be stored.						  *
* 								  *
* Returns a pointer to the following byte, 0 in case of error.	  *
* 								  *
\*****************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/asn1.h>

asn1 asn1_intdec(z,zm,r)
register asn1 z,zm;
int * r;
{
	int l;
	register long n;

	switch (l=*z++){
	case 0: *r = 0; return(z);
#ifndef ASN1_CHAR_IS_UNSIGNED
	case 1: if (z >= zm) return(0);
		*r = *((char *)z);
		return(z+1);
#ifdef ANYBOUNDARY
	case 2:
		if (z+2 > zm) return(0);
#ifdef BIGENDIAN
		*r = *((short *)z);
#else
		*r = ASN1_SWAP_SHORT(*((short *)z));
#endif
		return(z+2);
	case 3:
		if (z+3 > zm) return(0);
		n = *((long *)(z-1));
		if (*z&128)
			n |= 0xff000000;
		else	n &= 0x00ffffff;
#ifdef BIGENDIAN
		*r = n;
#else
		*r = ASN1_SWAP_LONG(n);
#endif
		return(z+3);
	case 4:
		if (z+4 > zm) return(0);
#ifdef BIGENDIAN
		*r = *((long *)z);
#else
		*r = ASN1_SWAP_LONG(*((long *)z));
#endif
		return(z+4);
#endif
#endif /* !ASN1_CHAR_IS_UNSIGNED */
	default:
		if (l&128){
			n = l&0x7f;
			if (n == 0 || z+n > zm) return(0);
			for (l=0;n > 0; n--)
				l = (l<<8) + *z++;
			if (l == 0){ *r = 0; return(z);}
		}
		if (z+l > zm || l <= 0) {
			ASN1_CKERR(ASN1_ERR_LEN,z);
			return(0);
		}
#ifdef ASN1_CHAR_IS_UNSIGNED
		n = *z++;
		if (n >= 128){
			n = -(256 - n);
		}
#else
		n = *((char *) z++);
#endif
		while (--l){
			n = (n<<8) + *z++;
		}
		*r = n;
		return(z);
	}
}
