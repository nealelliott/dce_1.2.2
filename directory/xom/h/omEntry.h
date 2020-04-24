/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omEntry.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:41  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:38:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:46  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:21:27  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:15  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:22  marrek
 * 	Beautification.
 * 	[1994/02/09  15:40:50  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  11:00:24  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMENTRY_H
#define _OMENTRY_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char omentry_rcsid[] = "@(#)$RCSfile: omEntry.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:19 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code
omP_encode (
	OM_private_object	original,
	OM_object_identifier	rules,
	OM_private_object		*encoding
);
OM_return_code
omP_decode (
	OM_private_object		encoding,
	OM_private_object		*original
);
OM_return_code
omP_copy(
	OM_private_object			original,
	OM_workspace				workspace,
	OM_private_object			*copy
);
OM_return_code
omP_copy_value(
	OM_private_object			source,
	OM_type						source_type,
	OM_value_position			source_value_position,
	OM_private_object			destination,
	OM_type						destination_type,
	OM_value_position			destination_value_position
);
OM_return_code
omP_create(
	OM_object_identifier	class,
	OM_boolean				initialise,
	OM_workspace			workspace,
	OM_private_object		*object
);

OM_return_code
omP_create_handle(
	OM_object_identifier	class,
	OM_workspace			workspace,
	void *priv_object,
	OM_private_object		*object
);
OM_return_code
omP_delete(
	OM_object  subject
);
OM_return_code
omP_get(
	OM_private_object		original,
	OM_exclusions			exclusions,
	OM_type_list			included_types,
	OM_boolean				local_strings,
	OM_value_position		initial_value,
	OM_value_position		limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number
);
OM_return_code
omP_instance(
	OM_object					subject,
	OM_object_identifier		class,
	OM_boolean					*instance
);
OM_return_code
omP_put(
	OM_private_object		destination,
	OM_modification			modification,
	OM_object				source,
	OM_type_list			included_types,
	OM_value_position		initial_value,
	OM_value_position		limiting_value
);

OM_return_code
omP_read(
	OM_private_object			subject,
	OM_type						type,
	OM_value_position			value_position,
	OM_boolean					local_string,
	OM_string_length			*element_position,
	OM_string					*elements
);
OM_return_code
omP_remove(
	OM_private_object			subject,
	OM_type						type,
	OM_value_position			initial_value,
	OM_value_position			limiting_value
);
OM_return_code
omP_write(
	OM_private_object			subject,
	OM_type						type,
	OM_value_position			value_position,
	OM_syntax					syntax,
	OM_string_length			*element_position,
	OM_string					elements
);
OM_return_code
omP_version(
	OM_workspace	workspace,
	OMP_feature	feature_list[]
);
void
omP_shutdown(
	OM_workspace	workspace);
OM_return_code
omP_package_present	(
	OM_workspace			workspace ,
	OM_object_identifier	package_oid	,
	OM_boolean				*package_present
);
OM_return_code
omP_add_package	(
	OM_workspace			workspace ,
	OMP_package				package_feature
);
OM_return_code
omP_delete_package (
	OM_workspace			workspace ,
	OM_object_identifier	package_oid
);
OM_return_code
omP_set_generic_workspace (
	OM_workspace	workspace ,
	void *generic_workspace
);
OM_return_code
omP_get_generic_workspace (
	OM_workspace	workspace ,
	void **generic_workspace
);
void
omP_complete_shutdown(
	OM_workspace	workspace);
OM_return_code
omP_negotiate (
	OM_workspace			workspace ,
	OMP_negotiate_feature	feature_list[]
);
OM_return_code
omP_initialize(
	struct OMP_switch_functions_body *sw_functions,
	OMP_package		*packages_table,
	OMP_package		*closure_packages_table,
	void *generic_workspace	,
	struct alloc_table *alloc_table,
	void *default_session,
	OM_workspace	*workspace_return
);
char	*
mmgt_calloc(
	char *file,
	OM_sint	line,
	OM_sint	n,
	OM_sint	size
);
void
mmgt_free (
char *file,
OM_sint	line,
char *buffer
);
char	*
mmgt_malloc(
	char *file,
	OM_sint	line,
	OM_sint	size
);
char	*
mmgt_realloc (
	char *file,
	OM_sint	line,
	char *old,
	OM_sint	size
);
OM_return_code
omP_iw_encode (
	OM_private_object	original,
	OM_object_identifier	rules,
	OM_private_object		*encoding
);
OM_return_code
omP_iw_decode (
	OM_private_object		encoding,
	OM_private_object		*original
);
OM_return_code
omP_iw_copy(
	OM_private_object			original,
	OM_workspace				workspace,
	OM_private_object			*copy
);
OM_return_code
omP_iw_copy_value(
	OM_private_object			source,
	OM_type						source_type,
	OM_value_position			source_value_position,
	OM_private_object			destination,
	OM_type						destination_type,
	OM_value_position			destination_value_position
);
OM_return_code
omP_iw_create(
	OM_object_identifier	class,
	OM_boolean				initialise,
	OM_workspace			workspace,
	OM_private_object		*object
);

OM_return_code
omP_iw_delete(
	OM_object  subject
);
OM_return_code
omP_iw_get(
	OM_private_object		original,
	OM_exclusions			exclusions,
	OM_type_list			included_types,
	OM_boolean				local_strings,
	OM_value_position		initial_value,
	OM_value_position		limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number
);
OM_return_code
omP_iw_instance(
	OM_object					subject,
	OM_object_identifier		class,
	OM_boolean					*instance
);
OM_return_code
omP_iw_put(
	OM_private_object		destination,
	OM_modification			modification,
	OM_object				source,
	OM_type_list			included_types,
	OM_value_position		initial_value,
	OM_value_position		limiting_value
);

OM_return_code
omP_iw_read(
	OM_private_object			subject,
	OM_type						type,
	OM_value_position			value_position,
	OM_boolean					local_string,
	OM_string_length			*element_position,
	OM_string					*elements
);
OM_return_code
omP_iw_remove(
	OM_private_object			subject,
	OM_type						type,
	OM_value_position			initial_value,
	OM_value_position			limiting_value
);
OM_return_code
omP_iw_write(
	OM_private_object			subject,
	OM_type						type,
	OM_value_position			value_position,
	OM_syntax					syntax,
	OM_string_length			*element_position,
	OM_string					elements
);
#endif /* ifndef _OMENTRY_H */
