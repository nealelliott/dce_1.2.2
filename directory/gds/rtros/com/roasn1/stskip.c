/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stskip.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:06  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:19  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:14  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:47:57  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:12  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:47:16  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:28  keutel
 * 	creation
 * 	[1994/03/21  13:16:43  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stskip.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:18 $" } ;
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
static char *sccsid = " @(#) stskip.c 1.1 93/11/29  ST_MAVROS";
/*************************************************************************\
* 									  *
* asn1st_skip(hdl,endpos)                                                 *
* 									  *
* Skip over an ASN-1 ``T-L-V'' field.					  *
*       hdl    = Store at Position of the T-field.                        *
*       endpos = the limit of the current buffer.                         *
* Returns an Errorvalue != 0  in case of error.                           *
* 									  *
\*************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int PASCAL asn1st_skip(
STOREHANDLE hdl,
long  endpos)
{
	long posz2,wh;
	int s;
	int sterr;

	wh = ro0076_stwhere(hdl);
	sterr = ro0063_sterror(hdl);

	if (wh < endpos)
	    s = ro0077_stgetcp(hdl) & 32;
	else {
	    return (asn1st_errdecset(hdl, ASN1_ERR_TAG));
	}

	if ( (sterr = asn1st_skiptype(hdl,endpos)) != 0 )
		return(sterr);
	if ( (sterr = asn1st_length(hdl,endpos,&posz2)) != 0 )
		return(sterr);
	if (posz2 != 0)                    /* definite length encoding used */
	    return (ro0066_storepos(hdl,posz2));     /* do the skip + leave */
	if (s){
	   while ((ro0076_stwhere(hdl) < endpos) && ro0077_stgetcp(hdl)){
		  if ( (sterr = asn1st_skip(hdl,endpos)) != 0 )
		      return(sterr);
	   }
	   return(asn1st_close(hdl,endpos,posz2));
	}else{
	      return(asn1st_errdecset(hdl,ASN1_ERR_LDEF));
	     }
}


/* *****************************************
 New Function : to handle extensibility 
* ***************************************** */

int PASCAL asn1st_skipclose (
STOREHANDLE hdl,
long  poszm,
long  posz2)
{
	int sterr;

	sterr = ro0063_sterror(hdl);

	while ( (sterr == 0) && (ro0076_stwhere(hdl) < poszm)
			     && ro0077_stgetcp(hdl)){
	       sterr = asn1st_skip(hdl,poszm);
	}

	if (sterr) return(sterr);
	return(asn1st_close(hdl,poszm,posz2));
}


/*********************************************************\
* 							  *
* asn1_size(hdl,endpos)                                   *
* 							  *
* Returns an upper boundary of the length, in bytes, of   *
* the current element. The Store-position is at the       *
* T-Field.                                                *
* 							  *
* on error 0 is returned !!!                              *
* 							  *
\*********************************************************/

/* not used jet , not tested */
/*
long PASCAL asn1st_size(
STOREHANDLE hdl,
long  endpos)
{
	long posz0, wh;
	if (!ro0063_sterror(hdl) && (posz0 = ro0076_stwhere(hdl)) < endpos ){
		if ( asn1st_skip(hdl,endpos) ) {
		     ro0066_storepos(hdl,posz0);
		     return(0);
		}
		else {
		     wh = ro0076_stwhere(hdl) ;
		     ro0066_storepos(hdl,posz0);
		     return (wh - posz0);
		}
	}
	else   return(0);
}

*/
