/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPGetBdcp.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:18  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:34:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:58  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:19:50  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:39  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:23:07  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:09:51  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:37:49  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:56:01  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSPGETBDCP_H
#define _DSPGETBDCP_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dspgetbdcp_rcsid[] = "@(#)$RCSfile: dsPGetBdcp.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:26 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

OM_return_code
dsP_get_search_guide(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_search_guide	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number);

OM_return_code
dsP_get_postal_address(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_postal_address	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number);

OM_return_code
dsP_get_telex_number(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_telex_number	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number);

OM_return_code
dsP_get_ttxid(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_ttxid		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number);

OM_return_code
dsP_get_faxnumber(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_faxnumber		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number);

OM_return_code
dsP_get_search_criterion(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_search_criterion	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number);

OM_return_code
dsP_get_G3_Fax_NBP(
	OMX_workspace		workspace,
	OM_private_object	original,
	OM_sint32			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number);

OM_return_code
dsP_get_Teletex_NBP(
	OMX_workspace		workspace,
	OM_private_object	original,
	D2_ttxid		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number);


#endif /* ifndef _DSPGETBDCP_H */
