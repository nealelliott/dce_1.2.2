/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omRemove.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:52  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:39:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:50  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:21:33  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:20  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:28  marrek
 * 	Beautification.
 * 	[1994/02/09  15:41:29  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  11:01:50  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMREMOVE_H
#define _OMREMOVE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char omremove_rcsid[] = "@(#)$RCSfile: omRemove.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:25 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
omP_xremove(
	OM_private_object		subject,
	OM_type			type,
	OM_value_position		initial_value,
	OM_value_position		limiting_value
);
#endif /* ifndef _OMREMOVE_H */
