/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stskptyp.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:21  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:15  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:47:58  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:13  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:47:18  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:30  keutel
 * 	creation
 * 	[1994/03/21  13:16:48  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stskptyp.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:20 $" } ;
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
static char *sccsid = " @(#) stskptyp.c 1.1 93/11/29  ST_MAVROS";
/***********************************************************************\
* 								        *
* asn1st_skiptype(hs, endpos)                                           *
* 								        *
* Skip the type field:                                                  *
* The r/w pointer of the STORE is at the position of the type field     *
* to be skipped.                                                        *
* <endpos> represents the last byte in the ASN1 String.                 *
* Returns Errorcode != 0  in case of error. The following               * 
* errors are detected:						        *
* 								        *
* . ASN1_ERR_TAG	The tag length exceeds the buffer limit.        *
* . Errors of the storefunctions                                        *
* 								        *
\***********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int PASCAL asn1st_skiptype(
STOREHANDLE hdl,
long  endpos)
{
    int sterr;
    long maxstlength;


    maxstlength = ro0062_storelength(hdl);
    if (maxstlength < endpos)
	asn1st_errdecset(hdl,ASN1_ERR_LEN); /* otherwise endless loop in */
					    /* while loop below possible */
    sterr = ro0063_sterror(hdl);
    if(sterr) return(sterr);

    if ((ro0070_stgetc(hdl) & 0x1F) == 31)
	while ( ( (ro0076_stwhere(hdl)) < endpos ) &&
		(ro0070_stgetc(hdl) & 0x80 )); /* skip tag byte for byte */

    if (ro0076_stwhere(hdl) < endpos)
	return(ro0063_sterror(hdl));
    else{
	return( asn1st_errdecset(hdl,ASN1_ERR_TAG));
    }
}


