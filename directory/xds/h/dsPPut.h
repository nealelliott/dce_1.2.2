/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPPut.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:34:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:02  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:19:58  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:44  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:23:14  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:10:01  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:38:19  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:56:32  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSPPUT_H
#define _DSPPUT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dspput_rcsid[] = "@(#)$RCSfile: dsPPut.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:30 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

OM_return_code
dsP_put_switch (
	const OMP_object_type	object_type,
	OM_private_object		destination,
	const OM_modification	modification,
	const OM_object			source,
	const OM_type_list		included_types,
	const OM_value_position	initial_value,
	const OM_value_position	limiting_value
);

OM_return_code
dsP_update_string(
	OM_sint	*len,
	void **str,
	OM_uint32	new_len,
	void *new_str,
	OM_modification	modification,
	OMX_workspace	workspace
);

OM_return_code
dsP_validate_type_and_mod(
	OMP_type_list	*t_l,
	OM_type_list	list,
	OM_value_position	initial_value,
	OM_value_position	limiting_value
);

OM_return_code
dsP_pub_type_in_list(
	OMP_type_list	*t_l,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_descriptor	*desc_ptr
);

OM_return_code
dsP_test_source_priv(
	OM_object		source,
	OM_type_list	included_types,
	OM_public_object	*src_pub,
	OM_value_position	*desc_nb,
	OM_boolean		*flag,
	OM_boolean		all
);

OM_return_code
dsP_put_psap (
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
dsP_put_dist_name (
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
dsP_get_nb_rdn_ava (
	char *dn_ptr,
	OM_string	*desc_dn,
	OM_sint	*rdn_nb,
	OM_sint	sep
);

#endif /* ifndef _DSPPUT_H */
