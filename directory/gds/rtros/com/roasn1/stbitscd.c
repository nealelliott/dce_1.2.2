/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stbitscd.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:31  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:51  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:15:17  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:03:20  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:53  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:08  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:54  keutel
 * 	creation
 * 	[1994/03/21  13:15:40  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stbitscd.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:43 $" } ;
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
/*   the software is hereby transferred.                              */
/*                                                                    */
/* ****************************************************************** */
static char *sccsid = " @(#) stbitscd.c 1.1 93/11/29  ST_MAVROS";
/***************************************************************************\
* 									    *
* asn1st_bitstcod(hdl1,hdl2,offset,length)                                  *
* 									    *
* Encode into a Store <hdl1> the length and value field of a BIT-STRING of  *
* bit-length <length> stored in the store <hdl2> at the position <offset>.  *
* 									    *
\***************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int PASCAL asn1st_bitstcod ( 
STOREHANDLE hdl1,
STOREHANDLE hdl2,
long offset,
long length)
{
  int rest,freebits=0;
  int sterr;

  rest = (int)(length & 0x07);     /* number of used bits in the last byte */
  if (rest == 0)                      /* if length is a multiple of 8 bits */
      length =length >> 3;    /* number of bytes needed to encode the bits */
  else
     {                             /* need one byte more for the rest-bits */
      length = (length>>3)+1;
      freebits = 8 - rest;  /* the number of unused bits has to be encoded */
     }

				 /* the freebits-byte increases the length */
  if ((sterr = asn1st_lencod(hdl1,length+1)) != 0 )
	return(sterr);
  ro0067_stputc(hdl1, (unsigned char) freebits); /* encode the freebits-byte */
  sterr = ro0079_stcat(hdl1,hdl2,offset,length);
					     /* move the bits to the Store */
  return (sterr);

}
