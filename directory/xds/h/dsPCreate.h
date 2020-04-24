/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPCreate.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:10  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:34:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:55  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:19:14  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:36  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:23:01  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:09:43  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:37:24  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:55:36  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSPCREATE_H
#define _DSPCREATE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dspcreate_rcsid[] = "@(#)$RCSfile: dsPCreate.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:23 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
dsP_create_switch (
	const OMP_object_type	class_sub_id,
	OM_private_object		destination,
	const OM_boolean		initialise);

OM_return_code
dsP_cr_handle_switch (
	const OMP_object_type	class_sub_id);

#endif /* ifndef _DSPCREATE_H */
