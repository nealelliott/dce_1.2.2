/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sttypcod.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:10  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:22  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:16  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:47:59  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:14  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:47:21  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:32  keutel
 * 	creation
 * 	[1994/03/21  13:16:50  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: sttypcod.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:21 $" } ;
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
static char *sccsid = " @(#) sttypcod.c 1.1 93/11/29  ST_MAVROS";
/**********************************************************\
*                                                          *
* asn1st_type_cod(hdl,m,n,s)                               *
*                                                          *
* Coding of the ASN-1 ``type'' field.                      *
*    hdl = Handle to STORE where to encode the field.      *
*       m= the ``mode'' or ``class'' of the identifier.    *
*       n= the type number in the class.                   *
*       s= the ``structure'' bit.                          *
* Returns errorcode    0 no error ; else error             *
*                                                          *
\**********************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

#define BL 6
/* Length of the Buffer for intermediat store of the encoded type */ 

long PASCAL asn1st_type_cod(
STOREHANDLE hdl,
int    m,
long n,
int s)
{
	int sterr;
	unsigned l= BL - 2;
	char interbuf[BL];
	if (((unsigned)n) < 31){
		ro0067_stputc(hdl,( (unsigned char) ((m<<6)|(s<<5)|n) ) );
	}else{
		interbuf[BL-1]=(char)(n&127);
		n = n >> 7;
		while (n) {
			interbuf[l] = (char)(n | 128);
			n = n >> 7;
			l--;
		}
		interbuf[l] = (char) ((m<<6) | (s<<5) |31);
		sterr = ro0068_stputm(hdl, &interbuf[l] , BL-l);
	}
	return(sterr);
}
