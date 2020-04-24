/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stlendec.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:52  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:07  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:06  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:47:46  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:03  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:44  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:16  keutel
 * 	creation
 * 	[1994/03/21  13:16:16  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stlendec.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:03 $" } ;
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
static char *sccsid = " @(#) stlendec.c 1.1 93/11/29  ST_MAVROS";
/***********************************************************\
* 							    *
* Decodes the length field if it's encoded definite.        *
* Indefinite length-encoding is reportet as l=-1            *
* asn1st_lendec(hdl,endpos,l)                               *
*       hdl= the STORE to decode,                           *
*       endpos = the limit of the buffer,                   *
* 	l= will contain the decoded value.		    *
* Returns the error staus.                                  *
* 							    *
\***********************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int  PASCAL asn1st_lendec(
STOREHANDLE hdl,
long endpos,
long FAR * l)
{
  int i,k;
  long wh;

  if ( (wh = ro0076_stwhere(hdl)) >= endpos )
     {
      return(asn1st_errdecset(hdl,ASN1_ERR_LEN));
     }
  i = ro0070_stgetc(hdl);
  if (i == 128)          /* this function decodes only definite length ! */
     {
      *l = -1;
      return(asn1st_errdecset(hdl,ASN1_ERR_LDEF));
     }
  if (i < 128)                                        /* 'normal' length */
      *l = (long)i;
  else{
       for ( i &= 0x7f, k=0; i; i--){
	   if (ro0076_stwhere(hdl) >= endpos)
	       return(asn1st_errdecset(hdl,ASN1_ERR_LLEN));
	   k = (k<<8) + ro0070_stgetc(hdl);
       }
	*l = (long)k;
  }
  if (*l + ro0076_stwhere(hdl) > endpos)
	return(asn1st_errdecset(hdl,ASN1_ERR_LEN));
  else
	return(ro0063_sterror(hdl));
}
