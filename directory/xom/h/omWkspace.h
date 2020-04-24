/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omWkspace.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:39:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:52  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:21:37  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:22  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:30  marrek
 * 	Beautification.
 * 	[1994/02/09  15:41:44  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  11:02:21  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMWKSPACE_H
#define _OMWKSPACE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char omwkspace_rcsid[] = "@(#)$RCSfile: omWkspace.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:27 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
omP_xpackage_present (
	OM_workspace		workspace ,
	OM_object_identifier	package_oid	,
	OM_boolean		*package_present
);
OM_return_code
omP_free_package_table (
	OM_workspace	workspace
);
OM_return_code
omP_xadd_package (
	OM_workspace		workspace ,
	OMP_package		package_feature
);
OM_return_code
omP_xdelete_package	(
	OM_workspace			workspace ,
	OM_object_identifier	package_oid
);
OM_return_code
omP_xset_generic_workspace (
	OM_workspace	workspace ,
	void *generic_workspace
);
OM_return_code
omP_xget_generic_workspace (
	OM_workspace	workspace ,
	void **generic_workspace
);
OM_return_code
omP_xinitialize	(
	struct OMP_switch_functions_body *sw_functions,
	OMP_package	*packages_table,
	OMP_package	*closure_packages_table,
	void *generic_workspace	,
	struct alloc_table *alloc_table,
	void *default_session,
	OM_workspace	*workspace_return
);
void
omP_free_workspace(
	OM_workspace	workspace
);
void
omP_xshutdown(
	OM_workspace	workspace);
void
omP_xcomplete_shutdown(
	OM_workspace	workspace
);
OM_return_code
omP_xnegotiate (
	OM_workspace		workspace ,
	OMP_negotiate_feature	feature_list[]
);
OM_return_code
omP_xversion(
	OM_workspace	workspace,
	OMP_feature	feature_list[]
);
#endif /* ifndef _OMWKSPACE_H */
