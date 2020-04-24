/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stintdec.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:48  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:04  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:15:29  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:03:28  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:01  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:40  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:11  keutel
 * 	creation
 * 	[1994/03/21  13:16:10  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stintdec.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:59 $" } ;
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
static char *sccsid = " @(#) stintdec.c 1.1 93/11/29  ST_MAVROS";
/*****************************************************************\
*                                                                 *
* asn1st_intdec(hdl,endpos,r):                                    *
*                                                                 *
* Transform an ASN-1 encoded value into a C 32 bits integer       *
*      hdl = Handle to the Store (r/w-pos at the ASN-1 L-field).  *
*   endpos = endposition of the buffer.                           *
*       r = a pointer to the integer variable where the result    *
*       will be stored.                                           *
*                                                                 *
* Returns the current error status.                               *
*                                                                 *
\*****************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int PASCAL asn1st_intdec(
STOREHANDLE hdl,
long endpos,
long FAR * r)
{
			     /* for there are only a few bytes needed */
				    /* the decoding is done in memory */
		      /* like MAVROS did before. So only the pointers */
			/* and absolute positions have to be changed. */


   unsigned char v[8];
   unsigned char FAR *z;
   long l;
   long n;
   int sterr;

   if ((sterr = asn1st_lendec(hdl,endpos,(long FAR *)&l)) != OK)     /* get the length */
	return(sterr);
   if ( l > 4 || l < 1 ) {
			/* only a 4-byte integer or less will be supported */
       return(asn1st_errdecset(hdl,ASN1_ERR_INTLEN));
   }

   ro0071_stgetm(hdl,(char FAR *) v+1,l);  /* copy the V-field to a buffer */

   z= ((unsigned char FAR *)v)+1;                       /* work in memory like Std-Mavros */
	switch (l){

#ifndef ASN1_CHAR_IS_UNSIGNED
	case 1:
		*r = *((char FAR *)z);
		return(OK);
#ifdef ANYBOUNDARY
	case 2:

#ifdef BIGENDIAN
		*r = *((short *)z);
#else
		*r = ASN1_SWAP_SHORT(*((short *)z));
#endif
		return(OK);
      case 3:

		n = *((long *)(z-1));
		if (*z&128)
			n |= 0xff000000;
		else
			n &= 0x00ffffff;
#ifdef BIGENDIAN
		*r = n;
#else
		*r = ASN1_SWAP_LONG(n);
#endif
		return(OK);
	case 4:

#ifdef BIGENDIAN
		*r = *((long *)z);
#else
		*r = ASN1_SWAP_LONG(*((long *)z));
#endif
		return(OK);
#endif
#endif /* !ASN1_CHAR_IS_UNSIGNED */

	default:

#ifdef ASN1_CHAR_IS_UNSIGNED
		n = *z++;
		if (n >= 128)
			n = -(256-n);

#else
		n = * (char FAR *) z++;                 /* read first byte */
#endif
		while (--l){                              /* read the rest */
		  n = (n<<8) + *z++;                /* build up an integer */
		 }
	      *r = n;
	      return (OK);
     }/*switch*/
}
