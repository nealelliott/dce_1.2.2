/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stlencod.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:49  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:05  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:05  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:47:45  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:02  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:42  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:14  keutel
 * 	creation
 * 	[1994/03/21  13:16:14  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stlencod.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:01 $" } ;
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
static char *sccsid = " @(#) stlencod.c 1.1 93/11/29  ST_MAVROS";
/***************************************\
*                                       *
* asn1st_lencod(hdl,l)                  *
*                                       *
* Encodes the ``length'' field.         *
*     hdl= STORE where to encode.       *
*       l= the length to encode.        *
* Returns   error status                *
*                                       *
\***************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

#define BL 5
/* length of intermediat buffer */

int PASCAL asn1st_lencod(
STOREHANDLE hdl,
long  l)
{
	int n;
	char interbuf[BL];
	if (l < 128) 
		ro0067_stputc(hdl, (unsigned char) l);
	else {
		n = BL - 1;
		while (l) {
			interbuf[n--] = (char) l;
			l = l >> 8;
		}        
		interbuf[n] = 128 +BL -1  -n;
		ro0068_stputm(hdl,(char FAR *)&interbuf[n],BL-n);
	}
	return(ro0063_sterror(hdl));
}
