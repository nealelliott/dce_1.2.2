/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPMdup.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:23  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:34:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:00  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:19:54  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:42  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:23:11  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:09:56  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:38:03  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:56:17  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSPMDUP_H
#define _DSPMDUP_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dspmdup_rcsid[] = "@(#)$RCSfile: dsPMdup.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:28 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


#include <xom.h>
#include <d2dir.h>
#include <xdsiapl.h>
#include <xds.h>
#include <xomip.h>
#include <alloc.h>
#include <xdsgds.h>
#include <xdsbdcp.h>
#include <osiaddr.h>
#include <cmx_addr.h>
#include <xdsdef.h>
#include <xomdef.h>
#include <xmhdef.h>

OM_return_code
dsP_del_str_list(
	OMX_workspace workspace,
	xds_str_list	*xobject);

OM_return_code
dsP_del_OR_address(
	OMX_workspace workspace,
	xds_OR_address *xobject);

OM_return_code
dsP_del_OR_name(
	OMX_workspace workspace,
	xds_OR_name *xobject);

OM_return_code
dsP_del_dlsp(
	OMX_workspace workspace,
	xds_dl_submit_perm *xobject);

OM_return_code
dsP_get_dlsp(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_dl_submit_perm	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number);

OM_return_code
dsP_get_OR_name(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_OR_name		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number);

OM_return_code
dsP_get_OR_address(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_OR_address		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number);

OM_return_code
dsP_put_dlsp(
	OMX_workspace	workspace,
	void **xds_object,
	OM_object	source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value
);

OM_return_code
dsP_put_OR_name(
	OMX_workspace	workspace,
	void **xds_object,
	OM_object	source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value
);

OM_return_code
dsP_put_OR_address(
	OMX_workspace	workspace,
	void **xds_object,
	OM_object	source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value
);

OM_return_code
dsP_del_a_str(
	OMX_workspace	workspace,
	xds_str_list *xobject);

OM_return_code
dsP_put_str_list(
	OMX_workspace	workspace,
	void **xds_object,
	OM_modification	modification,
	OMP_type_list	*pt_l,
	void **pt_obj
);

#endif /* ifndef _DSPMDUP_H */
