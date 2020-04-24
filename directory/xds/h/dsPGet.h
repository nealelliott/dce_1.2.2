/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPGet.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:16  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:34:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:57  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:09:16  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:25:23  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:19:17  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:39  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:23:05  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:09:49  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:37:41  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:55:53  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSPGET_H
#define _DSPGET_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dspget_rcsid[] = "@(#)$RCSfile: dsPGet.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:25 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
dsP_get_switch(
	const OM_private_object	original,
	const OM_exclusions		exclusions,
	const OM_type_list		included_types,
	const OM_boolean		local_strings,
	const OM_value_position	initial_value,
	const OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number);

OM_return_code
dsP_check_specific_syntax(
	OM_syntax syntax);

OM_return_code
dsP_validate_type_list(
	OMP_type_list	*legal_types,
	OM_type_list	included_types);

OM_return_code
dsP_free_resources(
	OMX_workspace workspace,
	OM_descriptor *desc0, OM_descriptor	*cur_desc,
	OM_sint	nb_desc);

OM_return_code
dsP_get_dn(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_dist_name		xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number);

OM_return_code
dsP_get_psap(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_psap		xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number);

OM_return_code
dsP_alloc_descriptor(
	OMX_workspace workspace,
	OM_sint	nb_desc,
	OM_descriptor **desc);
#endif /* ifndef _DSPGET_H */
