/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPWkspace.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:32  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:34:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:02  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:19:59  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:45  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:23:16  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:10:04  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:38:27  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:56:39  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSPWKSPACE_H
#define _DSPWKSPACE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dspwkspace_rcsid[] = "@(#)$RCSfile: dsPWkspace.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:31 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


#ifdef THREADSAFE
static void
xds_init_routine(void);
#endif /* THREADSAFE */

OM_return_code
dsP_init_switch(
	const OMX_workspace workspace,
	xds_session *default_session);

#endif /* ifndef _DSPWKSPACE_H */
