/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: osidat.c,v $
 * Revision 1.1.10.2  1996/02/18  18:21:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:53  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:07:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:17  root]
 * 
 * Revision 1.1.8.2  1994/09/06  12:26:22  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:24:06  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:11:18  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:02:26  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:47:09  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:31:52  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  21:30:10  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:34:36  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/09  16:18:19  marrek
 * 	November 1992 code drop
 * 	remove convtable
 * 	[1992/12/09  14:35:17  marrek]
 * 
 * Revision 1.1.4.2  1992/12/04  18:11:39  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	[1992/12/04  14:04:17  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:01:18  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:53:03  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: osidat.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:21:03 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

#include <gds.h>
#include <dce/cmx_addr.h>
#include <dce/osiaddr.h>


	/* Selector description table */
Sel_description sel_info[] = {
	{ T_PSAPINFO, T_PSELVERS, T_PSELSIZE },
	{ T_SSAPINFO, T_SSELVERS, T_SSELSIZE },
	{ T_TSAPINFO, T_TSELVERS, T_TSELSIZE }
} ;
