/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stbitcod.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:30  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:50  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:15:15  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:03:19  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:53  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:05  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:52  keutel
 * 	creation
 * 	[1994/03/21  13:15:38  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stbitcod.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:42 $" } ;
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
static char *sccsid = " @(#) stbitcod.c 1.1 93/11/29  ST_MAVROS";
/***************************************************************************\
*                                                                           *
* asn1st_bitcod(hdl,s,l)                                                    *
*                                                                           *
* Encode into the Store <hdl> the length and value field of a BIT-STRING of *
* length <l> stored at the address <s>.                                     *
*                                                                           *
\***************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int PASCAL asn1st_bitcod(
STOREHANDLE hdl,
char FAR * s,
long l)
{
	int ll,remainder;
	int sterr;

	if ((remainder = (int)(l&0x07)) == 0)
		ll = (int)(l >> 3);
	else{   ll = (int)((l>>3)+1);
		remainder = 8 - remainder;
	}
	asn1st_lencod(hdl,ll+1);
	sterr = ro0067_stputc(hdl,(unsigned char) remainder);
	if (ll > 0)
	    sterr = ro0068_stputm(hdl,s,ll);
	return(sterr);
}

int PASCAL asn1st_flagcod(
STOREHANDLE hdl,
unsigned long int flag,
long int l)
{
	asn1st_swapint( (unsigned long int FAR *)&flag);
	return(asn1st_bitcod(hdl,(char FAR *)&flag,l));
}
