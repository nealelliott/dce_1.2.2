/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sterror.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:42  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:59  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:15:25  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:03:25  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:58  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:27  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:06  keutel
 * 	creation
 * 	[1994/03/21  13:15:58  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: sterror.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:53 $" } ;
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

static char *sccsid = " @(#) sterror.c 1.1 93/11/29  ST_MAVROS";
/*********************************************************************\
* 								      *
* Here we find all the error signalling routine:		      *
* 								      *
* asn1st_errdec(pos,number):     for BER                              *
* 								      *
* 								      *
* The routine declares that an error of type "number" was detected    *
* when parsing the location "pos".                                    *
* 								      *
\*********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int     asn1st_diagnostic;
long    asn1st_wrongbyte;

int PASCAL asn1st_errdec(
long pos,
int n)
{
   asn1st_wrongbyte = pos;
   asn1st_diagnostic = n;
   return(0);                                           /* always ok */
}

int PASCAL asn1st_errdecset(
STOREHANDLE hdl,
int error)
{
  int sterr;
  if ((sterr = ro0063_sterror(hdl)) != 0)  /* if there is already an error */
      return(sterr);                                        /* just return */
  else
     {
      sterr = error;
      if (error > 0 && error < ASN1_ERR_LOCSTART){
	sterr = RCro6l_UnrecognizedPpduParam;
					/* ASN1 decoding or encoding error */
      } else if (error > ASN1_ERR_LOCSTART && error < ASN1_ERR_LOCEND) {
	sterr = RCro6n_InvalidPpduParam;
				  /* ASN1 local decoding or encoding error */
				  /* Restrictions in local syntax */
      }
      ro0080_seterror(hdl,(signed short int)sterr); /* report error to Store */
      ASN1_CKERR(ro0076_stwhere(hdl),error);  /* set where and which error */
      return(sterr);                          /* as a sign for 'good' work */
     }
}
