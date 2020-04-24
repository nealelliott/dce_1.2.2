/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omCreate.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:38:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:44  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:21:24  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:13  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  15:49:18  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:33  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:20  marrek
 * 	Beautification.
 * 	[1994/02/09  15:40:30  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:59:44  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMCREATE_H
#define _OMCREATE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char omcreate_rcsid[] = "@(#)$RCSfile: omCreate.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:17 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
omP_xcreate(
	OM_object_identifier	class,
	OM_boolean		initialise,
	OM_workspace		workspace,
	OM_private_object	*object);

OM_return_code
omP_xcreate_handle(
	OM_object_identifier	class,
	OM_workspace		workspace,
	void *priv_object,
	OM_private_object	*object);

OM_return_code
omP_get_object_type	(
	OMX_workspace			workspace,
	OM_object_identifier	class,
	OMP_object_type	*class_id);

OM_boolean
omP_check_negotiated_package(
	OM_workspace workspace,
	OM_sint	package_no);

OM_return_code
omP_get_class_sub_id (
	OMX_workspace	workspace,
	OM_object_identifier	class,
	OMP_object_type	*class_sub_id);

OMP_private_object *
omP_get_object_handle (
	OMX_workspace	workspace,
	OM_object_identifier	class,
	OMP_object_type		class_sub_id,
	void *priv_object);

#endif /* ifndef _OMCREATE_H */
