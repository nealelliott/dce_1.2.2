/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omGet.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:43  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:38:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:47  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:21:28  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:16  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  15:49:19  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:34  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:23  marrek
 * 	Beautification.
 * 	[1994/02/09  15:40:57  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  11:00:41  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMGET_H
#define _OMGET_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char omget_rcsid[] = "@(#)$RCSfile: omGet.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:21 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
omP_xget(
	OM_private_object	original,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number
);

void
omP_update_descriptor(
	OM_private_object	original,
	OM_descriptor	*desc0);

OM_return_code
omP_alloc_sub_handle(
	OM_private_object original,
	OM_object_identifier class,
	void *pri_obj,
	OM_private_object *handle);
#endif /* ifndef _OMGET_H */
