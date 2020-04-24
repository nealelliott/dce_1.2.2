/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stend.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:40  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:58  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:15:23  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:03:24  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:58  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:24  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:04  keutel
 * 	creation
 * 	[1994/03/21  13:15:56  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stend.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:52 $" } ;
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
static char *sccsid = " @(#) stend.c 1.1 93/11/29  ST_MAVROS";
/***************************************************************************\
* 									    *
* MAVROS encode ASN-1 components by reserving a single byte for the length  *
* field, and then coding the value. The actual length of the component is   *
* only known at the end of the coding; at that stage, MAVROS generate a     *
* call to ``asn1_regular_end()'' if the unique encoding (see X.509) is not  *
* requested.When the unique encoding is requested, MAVROS generate a call   *
* to either ``asn1_defined_end'' or ``asn1_end_set'' if the component was   *
* of type ``SET'' or ``SET OF''.					    *
* 									    *
* The global variables ``asn1_end'' and ``asn1_end_set'' are not used any   *
* more. They remain defined for compatibility sake.			    *
* 									    *
\***************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int  PASCAL asn1st_regular_end(
STOREHANDLE hdl,
long  posz0)
{       
  long now,dist;
  int sterr;

  sterr = ro0063_sterror(hdl);
  if (sterr)                      /* no execution in case of error */
      return(sterr);

  now = ro0076_stwhere(hdl);                 /* get absolute storeposition */
  dist = now - posz0 - 1;                              /* calculate length */
  if (dist > 127)                    /* length > 127 is encoded indefinite */
     {
      ro0067_stputc(hdl,0);                     /* just add two zero-bytes */
      sterr = ro0067_stputc(hdl,0);
     }
  else
     {
      ro0066_storepos(hdl,posz0);                   /* move to length-byte */
      ro0067_stputc(hdl, (unsigned char) dist);     /* write definite length */
      sterr = ro0066_storepos(hdl,now);       /* move back to old position */
     }
  return(sterr);
}
