/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stoctscd.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:59  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:14  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:11  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:47:53  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:07  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:58  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:22  keutel
 * 	creation
 * 	[1994/03/21  13:16:31  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stoctscd.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:12 $" } ;
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
static char *sccsid = " @(#) stoctscd.c 1.1 93/11/29  ST_MAVROS";

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

int PASCAL asn1st_octets_cod(      /* encodes to Store from memory */
STOREHANDLE hdl,
char FAR * x,
long l)
{
  asn1st_lencod(hdl,l);                               /* encode the length */
  ro0068_stputm(hdl,x,l);                  /* copy the octets to the store */
  return (ro0063_sterror(hdl));                              /* that's all */
}


int PASCAL asn1st_octstcod(
STOREHANDLE hdl1,                        /* encode to a Store from a Store */
STOREHANDLE hdl2,
long offset,
long length)
{
  asn1st_lencod(hdl1,length);                         /* encode the length */
  ro0079_stcat(hdl1,hdl2,offset,length);             /* concate the stores */
  return (ro0063_sterror(hdl1));                             /* that's all */
}

