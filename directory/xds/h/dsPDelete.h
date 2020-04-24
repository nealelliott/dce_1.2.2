/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPDelete.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:12  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:34:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:56  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:09:15  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:25:21  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:19:15  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:37  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:23:02  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:09:46  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:37:32  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:55:44  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSPDELETE_H
#define _DSPDELETE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dspdelete_rcsid[] = "@(#)$RCSfile: dsPDelete.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:24 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
dsP_delete_specific_value(
	OMX_workspace	workspace,
	OM_descriptor	*desc_ptr);

OM_return_code
dsP_delete_switch(
	OM_object	subject);

OM_return_code
dsP_del_session(
	OMX_workspace workspace,
	xds_session *xobject);

void
dsP_delete_str(
	OMX_workspace workspace,
	D2_str *xobject);

OM_return_code
dsP_del_psap(
	OMX_workspace workspace,
	xds_psap xobject);

OM_return_code
dsP_del_dist_name(
	OMX_workspace	workspace,
	xds_dist_name xobject);

void
dsP_delete_values(
	OMX_workspace	workspace,
	D2_a_value *val_array,
	OM_sint	val_nb);

void
dsP_delete_type(
	OMX_workspace workspace,
	D2_a_type *type);

void
dsP_delete_criteria(
	OMX_workspace workspace,
	D2_criteria *xobject);

void
dsP_delete_a_value(
	OMX_workspace workspace,
	D2_a_value *value_ptr);

OM_return_code
dsP_del_attribute(
	OMX_workspace	workspace,
	xds_attribute *xobject);

OM_return_code
dsP_del_extension(
	OMX_workspace	workspace,
	xds_extension *xobject);

OM_return_code
dsP_del_entry_mod(
	OMX_workspace	workspace,
	xds_entry_mod *xobject);

OM_return_code
dsP_del_a_filter(
	OMX_workspace workspace,
	xds_filter *xobject);

OM_return_code
dsP_del_fi_item(
	OMX_workspace workspace,
	xds_fi_item *xobject);

OM_return_code
dsP_del_certificate(
	OMX_workspace workspace,
	Certificate *xobject);

#endif /* ifndef _DSPDELETE_H */
