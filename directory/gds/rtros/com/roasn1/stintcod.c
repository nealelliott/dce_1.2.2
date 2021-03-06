/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stintcod.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:46  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:03  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:15:28  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:03:27  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:00  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:36  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:10  keutel
 * 	creation
 * 	[1994/03/21  13:16:08  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stintcod.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:57 $" } ;
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
static char *sccsid = " @(#) stintcod.c 1.1 93/11/29  ST_MAVROS";
/****************************************************************\
*                                                                *
* asn1st_intcod(hdl,r)                                           *
*                                                                *
* Encoding the length and value components of an ASN-1 integer.  *
*       hdl= Handle to the Store.                                *
*       r= the integer to encode.                                *
* Returns the error status.                                      *
*                                                                *
\****************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int PASCAL asn1st_intcod(
STOREHANDLE hdl,
long r)
{
  char v[5];                                        /* intermediate buffer */
  char FAR *z;
  int sterr;

  z = (char FAR *)v;

	if (r < 128){                                    /* left unchanged */
		if (r >= -128){ *z++ =1; *z++ = (char)r;}
		else if (r >= -0x8000L) {
			*z++ = 2; 
			*z++ = (char)(r>>8); 
			*z++= (char)r;
			}
		else if (r >= -0x800000L) {
			*z++ = 3; 
			*z++ = (char)(r>>16);
			*z++ = (char)((r>>8)&0xFF); 
			*z++ = (char)(r&0xFF);
			}
		else{   *z++ = 4; 
			*z++ = (char)(r>>24); 
			*z++ = (char)((r>>16)&0xFF);
			*z++ = (char)((r>>8)&0xFF); 
			*z++ = (char)(r&0xFF);
		     }
	}else if (r < 0x8000L) {
			*z++ = 2; 
			*z++ = (char)(r>>8); 
			*z++ = (char)(r&0xFF);
		     }
	else  if (r < 0x800000L) {
		*z++ = 3; *z++ = (char)(r>>16);
		*z++ = (char)((r>>8)&0xFF); 
		*z++ = (char)(r&0xFF);
		}
	else{   *z++ = 4; *z++ = (char)(r>>24); 
		*z++ = (char)((r>>16)&0xFF);
		*z++ = (char)((r>>8)&0xFF); 
		*z++ = (char)(r&0xFF);
		}

	sterr = ro0068_stputm (hdl,v,v[0]+1);
	return(sterr);
}

