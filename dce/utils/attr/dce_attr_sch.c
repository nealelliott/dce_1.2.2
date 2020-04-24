/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dce_attr_sch.c,v $
 * Revision 1.1.4.2  1996/02/18  19:20:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:42  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:35:56  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  20:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/23  00:12 UTC  tatsu_s
 * 	Added dce_attr_sch_bind_local().
 * 	[1995/12/08  18:07:45  root]
 * 
 * Revision 1.1.2.1  1994/07/22  21:39:44  pwang
 * 	Initial implementation
 * 	[1994/07/22  20:35:37  pwang]
 * 
 * $EndLog$
 */


/*
** Module: dce_attr_sch.c
**
** FACILITY: DCE generic attribute interfaces (Client) 
**
** ABSTRACT: This module implements the dce attr schema operations
**
** Date:
*/

#include <attrimpl.h>

    
/* 
**++
** d c e _ a t t r _ s c h _ c r e a t e _ e n t r y
**
** Create a new schema entry.  Presumably you must be 
** authorized to add entries to the schema - 
** that is handled by the targetserver.
**	In Parameters:
**		h - opaque handle to the schema on which this
**			operation is being performed
**		schema_entry - a schema entry with a value in every
**			field except perhaps the optional fields:
**			trig_types, trig_bind, and comment.
**
** Errors:
**  dce_attr_s_bad_binding
**  ...
**--
*/
PUBLIC 
void dce_attr_sch_create_entry (
	/* [in]	*/ dce_attr_sch_handle_t	h,
	/* [in]	*/ dce_attr_schema_entry_t	*schema_entry,
	/* [out]*/ error_status_t		*st
)
{ 
	dce_attr_binding_handle_rep_t		*binding_handle;
	rs_cache_data_t				cache_info;

	CLEAR_STATUS(st);

	binding_handle = (dce_attr_binding_handle_rep_p_t) h;

	DCE_ATTR_BIND_VALIDATE_SCH(binding_handle, st);

	/*
	** Call the corresponding wire protocol and ignore the cache_info 
	** because there is no rs cache to update in this case
	*/
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	if (IS_LOCAL_HANDLE(binding_handle)) {
		(*(V0_0_EPV(binding_handle).rs_attr_schema_create_entry))
			(RPC_BINDING(binding_handle),
			 COMPONENT_NAME(binding_handle),
			 schema_entry,
			 &cache_info,
			 st);
	}
	else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
	(*rs_attr_schema_v0_0_c_epv.rs_attr_schema_create_entry)(
						RPC_BINDING(binding_handle),
						COMPONENT_NAME(binding_handle),
						schema_entry,
						&cache_info,
						st);

	return;	
}

/* 
**++
** d c e _ a t t r _ s c h _ d e l e t e _ e n t r y
**
** Delete a schema entry.  This is a radical operation
** that will delete or invalidate any existing 
** attributes of this type on nodes dominated by  
** the schema.  Access to this operation should be
** severely limited.
**	In Parameters:
**		h - opaque handle to the schema on which this
**			operation is being performed
**		attr_id - attribute type uuid of entry to be deleted.
**
** Errors:
**  dce_attr_s_bad_binding
**  ...
**--
*/
PUBLIC 
void dce_attr_sch_delete_entry (
	/* [in] */ dce_attr_sch_handle_t	h,
	/* [in]	*/ uuid_t			*attr_id,
	/* [out]*/ error_status_t		*st
)
{
	dce_attr_binding_handle_rep_t		*binding_handle;
	rs_cache_data_t				cache_info;

	CLEAR_STATUS(st);

	binding_handle = (dce_attr_binding_handle_rep_p_t) h;

	DCE_ATTR_BIND_VALIDATE_SCH(binding_handle, st);

	/*
	** Call the corresponding wire protocol and ignore the cache_info 
	** because there is no rs cache to update in this case
	*/
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	if (IS_LOCAL_HANDLE(binding_handle)) {
		(*(V0_0_EPV(binding_handle).rs_attr_schema_delete_entry))
			(RPC_BINDING(binding_handle),
			 COMPONENT_NAME(binding_handle),
			 attr_id,
			 &cache_info,
			 st);
	}
	else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
	(*rs_attr_schema_v0_0_c_epv.rs_attr_schema_delete_entry)(
						RPC_BINDING(binding_handle),
						COMPONENT_NAME(binding_handle),
						attr_id,
						&cache_info,
						st);

	return;	
}

/* 
**++
** d c e _ a t t r _ s c h _ u p d a t e _ e n t r y
**
** Update the modifiable fields of a schema entry
** identified by the attr_id in the input schema_entry.
** See dce_attr_base.idl for the modifiable fields.
**
** If a field is indicated by its flag in modify_parts, that
** field from the input schema entry will completely replace
** the current field of the existing schema entry.  All other
** fields will remain untouched.  Note that fields which are
** arrays of structures (like acl_mgr_set and trig_binding)
** will be *completely* replaced by the new input array.
** This operation will not simply add one more element to
** the existing array.
**
**	In Parameters:
**		h - opaque handle to the schema on which this
**			operation is being performed
**		modify_parts - this flag set identifies the fields
**			in the input schema_entry that should be updated.
**			Fields not indicated by modify_parts will retain
**			their current values.
**		schema_entry - a schema entry whow fields are null
**			except for the fields indicated by modify_parts.
**
** Errors:
**  dce_attr_s_bad_binding
**  ...
**--
*/
PUBLIC
void dce_attr_sch_update_entry (
	/* [in]	*/ dce_attr_sch_handle_t		h,
	/* [in]	*/ dce_attr_schema_entry_parts_t	modify_parts,
	/* [in]	*/ dce_attr_schema_entry_t		*schema_entry,
	/* [out]*/ error_status_t			*st
)
{
	dce_attr_binding_handle_rep_t		*binding_handle;
	rs_cache_data_t				cache_info;

	CLEAR_STATUS(st);

	binding_handle = (dce_attr_binding_handle_rep_p_t) h;

	DCE_ATTR_BIND_VALIDATE_SCH(binding_handle, st);

	/*
	** Call the corresponding wire protocol and ignore the cache_info 
	** because there is no rs cache to update in this case
	*/
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	if (IS_LOCAL_HANDLE(binding_handle)) {
		(*(V0_0_EPV(binding_handle).rs_attr_schema_update_entry))
			(RPC_BINDING(binding_handle),
			 COMPONENT_NAME(binding_handle),
			 modify_parts,
			 schema_entry,
			 &cache_info,
			 st);
	}
	else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
	(*rs_attr_schema_v0_0_c_epv.rs_attr_schema_update_entry)(
						RPC_BINDING(binding_handle),
						COMPONENT_NAME(binding_handle),
						modify_parts,
						schema_entry,
						&cache_info,
						st);

	return;	
}

/*
**++
**  d c e _ a t t r _ s c h _ c u r s o r _ i n i t
**
** Initialize a scan cursor. The cursor must be released
** with dce_attr_sch_cursor_release.
**	In Parameters:
**		h - opaque handle to the schema on which this
**			operation is being performed
**		
**	Out Parameters:
**		cur_num_entries - the current total number of entries in 
**			the schema at the time of this call.
**		cursor - cursor initialized to the first in the
**			list of entries in the named schema.
**
** Errors:
**  dce_attr_s_no_memory
**  dce_attr_bad_binding
**  ...
**--
*/
PUBLIC 
void dce_attr_sch_cursor_init (
	/* [in]	*/ dce_attr_sch_handle_t	h,
	/* [out]*/ unsigned32			*cur_num_entries,
	/* [out]*/ dce_attr_cursor_t		*cursor,
	/* [out]*/ error_status_t		*st
)
{
	dce_attr_binding_handle_rep_t		*binding_handle;
	rs_attr_cursor_t			*cursor_rep_p;
	rs_cache_data_t				cache_info;

	CLEAR_STATUS(st);

	binding_handle = (dce_attr_binding_handle_rep_p_t) h;

	DCE_ATTR_BIND_VALIDATE_SCH(binding_handle, st);

	*cursor = NULL;

	/*
	** Allocate memory for the cursor and initialize the cursor
	*/
	MALLOC_AND_RESET(cursor_rep_p, rs_attr_cursor_t, st);
	CHECK_STATUS_RET(st);

	/*
	** Call the corresponding wire protocol and ignore the cache_info 
	** because there is no rs cache to update in this case
	*/
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	if (IS_LOCAL_HANDLE(binding_handle)) {
		(*(V0_0_EPV(binding_handle).rs_attr_schema_cursor_init))
			(RPC_BINDING(binding_handle),
			 COMPONENT_NAME(binding_handle),
			 cur_num_entries,
			 cursor_rep_p,
			 &cache_info,
			 st);
	}
	else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
	(*rs_attr_schema_v0_0_c_epv.rs_attr_schema_cursor_init)(
						RPC_BINDING(binding_handle),
						COMPONENT_NAME(binding_handle),
						cur_num_entries,
						cursor_rep_p,
						&cache_info,
						st);

	if (!GOOD_STATUS(st)) {
		free(cursor_rep_p);
		return;
	}
	*cursor = (dce_attr_cursor_t) cursor_rep_p;

	return;	
}


/*
**++
** d c e _ a t t r _ s c h _ c u r s o r _ a l l o c
**
** Allocate a scan cursor which must be released
** with dce_attr_sch_cursor_release.
** This is a local operation that can be used *instead of*
** the remote dce_attr_sch_cursor_init.
** Use dce_attr_sch_cursor_alloc when performance
** is more important than learning the current number of 
** entries, which gets returned by dce_attr_sch_cursor_init.
**
** Either dce_attr_sch_cursor_init or dce_attr_sch_cursor_alloc
** must be used to allocate a cursor for use in subsequent
** lookup operations.
**
**	Out Parameters:
**		cursor - cursor initialized to the first in the
**			list of attributes on this object.
**
** Errors:
**	dce_attr_s_no_memory
**--
*/
PUBLIC
void dce_attr_sch_cursor_alloc (
	/* [out] */	dce_attr_cursor_t	*cursor,
	/* [out] */	error_status_t		*st
)
{
	rs_attr_cursor_t	*cursor_rep_p;

	CLEAR_STATUS(st);

	*cursor = NULL;
	
	/*
	** Allocate memory for the cursor and initialize the cursor
	*/
	MALLOC_AND_RESET(cursor_rep_p, rs_attr_cursor_t, st);
	CHECK_STATUS_RET(st);

	*cursor = (dce_attr_cursor_t) cursor_rep_p;

	return;	
}


/*
**++
** d c e _ a t t r _ s c h _ c u r s o r _ r e s e t
**
** Reset a cursor that has been allocated with 
** either dce_attr_sch_cursor_init or dce_attr_sch_cursor_alloc
** so that it may be used to process a new query.
** This allows the caller to reuse a curser instead of
** releasing and re-allocating. 
** 
** This is a local operation.
**
**	Out Parameters:
**		cursor - cursor initialized to the first in the
**			list of attributes on this object.
**
** Errors:
**	dce_attr_s_bad_cursor
**
**--
*/
PUBLIC
void dce_attr_sch_cursor_reset (
	/* [in, out] */	dce_attr_cursor_t	*cursor,
	/* [out]     */	error_status_t		*st
)
{
	rs_attr_cursor_t	*cursor_rep_p;

	CLEAR_STATUS(st);

	cursor_rep_p = (rs_attr_cursor_t *) *cursor;
	if (!cursor_rep_p) {
		SET_STATUS(st, dce_attr_s_bad_cursor);
		return;
	}
	
	/*
	** Reset the cursor
	*/
	memset(cursor_rep_p, 0, sizeof(rs_attr_cursor_t));

	return;	
}


/*
**++
** d c e _ a t t r _ s c h _ c u r s o r _ r e l e a s e
**
** Release any state associated with a scan cursor allocated
** with either dce_attr_sch_cursor_init or 
** dce_attr_sch_cursor_alloc.
** This is a local-only operation.
**	In/Out Parameters:
**		cursor - in: initialized cursor;
**			 out: initialized cursor with resources released
** Errors:
**--
*/
PUBLIC
void dce_attr_sch_cursor_release (
	/* [in, out] */  dce_attr_cursor_t	*cursor,
	/* [out]     */  error_status_t		*st
)
{
	rs_attr_cursor_t	*cursor_rep_p;

	CLEAR_STATUS(st);

	cursor_rep_p = (rs_attr_cursor_t *) *cursor;
	if (cursor_rep_p) {
		free(cursor_rep_p);
	}
	*cursor = NULL;

	return;	
}

 
/*
**++
** d c e _ a t t r _ s c h _ s c a n
**
** Read num_to_read entries from the named schema - 
** useful for a browser.
**	In Parameters:
**		h - opaque handle to the schema on which this
**			operation is being performed
**		num_to_read - specifies the size of the 
**			schema_entries array and the max number of
**			entries to be returned in this call.
**
**	In/Out Parameters:
**		cursor - in: initialized cursor; 
**			out: cursor advanced past entries returned
**			in this call.
**
**	Out Parameters:
**		num_read - specifies the number of entries 
**			returned in schema_entries.
**		schema_entries - array containing num_read entries.
**
** Errors:
**  dce_attr_s_bad_binding 
**  dce_attr_s_bad_cursor
**  ...
**--
*/
PUBLIC 
void dce_attr_sch_scan (
	/* [in]     */ dce_attr_sch_handle_t	h,
	/* [in, out]*/ dce_attr_cursor_t	*cursor,
	/* [in]     */ unsigned32		num_to_read,
	/* [out]    */ unsigned32		*num_read,
	/* [out]    */ dce_attr_schema_entry_t	schema_entries[],
	/* [out]    */ error_status_t		*st
)
{
	dce_attr_binding_handle_rep_t		*binding_handle;
	rs_attr_cursor_t			*cursor_rep_p;
	rs_cache_data_t				cache_info;

	CLEAR_STATUS(st);

	binding_handle = (dce_attr_binding_handle_rep_p_t) h;

	DCE_ATTR_BIND_VALIDATE_SCH(binding_handle, st);

	cursor_rep_p = (rs_attr_cursor_t *) *cursor;

	/*
	** Call the corresponding wire protocol and ignore the cache_info 
	** because there is no rs cache to update in this case
	*/
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	if (IS_LOCAL_HANDLE(binding_handle)) {
		(*(V0_0_EPV(binding_handle).rs_attr_schema_scan))
			(RPC_BINDING(binding_handle),
			 COMPONENT_NAME(binding_handle),
			 cursor_rep_p,
			 num_to_read,
			 num_read,
			 schema_entries,
			 &cache_info,
			 st);
	}
	else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
	(*rs_attr_schema_v0_0_c_epv.rs_attr_schema_scan)(
						RPC_BINDING(binding_handle),
						COMPONENT_NAME(binding_handle),
						cursor_rep_p,
						num_to_read,
						num_read,
						schema_entries,
						&cache_info,
						st);


	return;	
}
 

/* 
**++
** d c e _ a t t r _ s c h _ l o o k u p _ b y _ n a m e 
**
** Read a schema entry by name - useful for an interactive
** editor.
**	In Parameters:
**		h - opaque handle to the schema on which this
**			operation is being performed
**		attr_name - attribute name identifying the entry
**
**	Out Parameters:
**		schema_entry - entry identified by attr_name. 
**
** Errors:
**  dce_attr_s_bad_binding 
**  ...
*/
PUBLIC 
void dce_attr_sch_lookup_by_name (
	/* [in] */ dce_attr_sch_handle_t	h,
	/* [in]	*/ idl_char			*attr_name,
	/* [out]*/ dce_attr_schema_entry_t	*schema_entry,
	/* [out]*/ error_status_t		*st
)
{
	dce_attr_binding_handle_rep_t		*binding_handle;
	rs_cache_data_t				cache_info;

	CLEAR_STATUS(st);

	binding_handle = (dce_attr_binding_handle_rep_p_t) h;

	DCE_ATTR_BIND_VALIDATE_SCH(binding_handle, st);

	/*
	** Call the corresponding wire protocol and ignore the cache_info 
	** because there is no rs cache to update in this case
	*/
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	if (IS_LOCAL_HANDLE(binding_handle)) {
		(*(V0_0_EPV(binding_handle).rs_attr_schema_lookup_by_name))
			(RPC_BINDING(binding_handle),
			 COMPONENT_NAME(binding_handle),
			 attr_name,
			 schema_entry,
			 &cache_info,
			 st);
	}
	else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
	(*rs_attr_schema_v0_0_c_epv.rs_attr_schema_lookup_by_name)(
						RPC_BINDING(binding_handle),
						COMPONENT_NAME(binding_handle),
						attr_name,
						schema_entry,
						&cache_info,
						st);


	return;	
}
 

/* 
**++
** d c e _ a t t r _ s c h _ l o o k u p _ b y _ i d
**
** Read a schema entry by ID.  Useful for 
** programmatic access.
**	In Parameters:
**		h - opaque handle to the schema on which this
**			operation is being performed
**		attr_id - attribute type uuid identifying the entry.
**
**	Out Parameters:
**		schema_entry - entry identified by attr_id. 
**
** Errors:
**  dce_attr_s_bad_binding 
**  ...
**--
*/
PUBLIC 
void dce_attr_sch_lookup_by_id (
	/* [in]	*/ dce_attr_sch_handle_t	h,
	/* [in]	*/ uuid_t			*attr_id,
	/* [out]*/ dce_attr_schema_entry_t	*schema_entry,
	/* [out]*/ error_status_t		*st
)
{
	dce_attr_binding_handle_rep_t		*binding_handle;
	rs_cache_data_t				cache_info;

	CLEAR_STATUS(st);

	binding_handle = (dce_attr_binding_handle_rep_p_t) h;

	DCE_ATTR_BIND_VALIDATE_SCH(binding_handle, st);

	/*
	** Call the corresponding wire protocol and ignore the cache_info 
	** because there is no rs cache to update in this case
	*/
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	if (IS_LOCAL_HANDLE(binding_handle)) {
		(*(V0_0_EPV(binding_handle).rs_attr_schema_lookup_by_id))
			(RPC_BINDING(binding_handle),
			 COMPONENT_NAME(binding_handle),
			 attr_id,
			 schema_entry,
			 &cache_info,
			 st);
	}
	else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
	(*rs_attr_schema_v0_0_c_epv.rs_attr_schema_lookup_by_id)(
						RPC_BINDING(binding_handle),
						COMPONENT_NAME(binding_handle),
						attr_id,
						schema_entry,
						&cache_info,
						st);


	return;	
}

/* 
**++
** d c e _ a t t r _ s c h _ g e t _ a c l _ m g r s 
**
** Retrieve the acl_mgr_types that protect the objects
** dominated by the named schema.  The returned acl_mgr_types
** are valid for use in the acl_mgr_set field of a schema_entry
** for the named schema.
**
** If num_acl_mgr_types is greater than size_used, this operation
** should be called again with a bigger acl_mgr_types buffer.
**
**	In Parameters:
**		h - opaque handle to the schema on which this
**			operation is being performed
**		size_avail - size of the acl_mgr_types array
**
**	Out Parameters:
**		size_used - number of acl_mgr_types returned
**		num_acl_mgr_types - total number of acl_mgr_types
**			supported for this schema.  
**		acl_mgr_types - array containing 'size_used' 
**			acl_mgr_type UUIDs.
**
** Errors:
**  dce_attr_s_not_implemented 
**--
*/
PUBLIC 
void dce_attr_sch_get_acl_mgrs (
	/* [in]	*/ dce_attr_sch_handle_t	h,
	/* [in]	*/ unsigned32			size_avail,
	/* [out]*/ unsigned32			*size_used,
	/* [out]*/ unsigned32			*num_acl_mgr_types,
	/* [out]*/ uuid_t			acl_mgr_types[],
	/* [out]*/ error_status_t		*st
)
{
	dce_attr_binding_handle_rep_t		*binding_handle;
	/*
	rs_cache_data_t				cache_info;
	*/

	CLEAR_STATUS(st);

	binding_handle = (dce_attr_binding_handle_rep_p_t) h;

	DCE_ATTR_BIND_VALIDATE_SCH(binding_handle, st);

	/*
	** Call the corresponding wire protocol and ignore the cache_info 
	** because there is no rs cache to update in this case
	*/
	SET_STATUS(st, dce_attr_s_not_implemented);

	return;	
}


/* 
**++
** d c e _ a t t r _ s c h _ a c l m g r _ s t r i n g s
**
** Retrieve printable representations for each permission bit that the 
** input acl_mgr_type will support.  There may be aliases for common
** permission combinations - by convention simple entries should
** appear at the beginning of the array, and combinations should at the
** appear at the end.  When false the tokenize flag indicates that
** permission printstrings are unambiguous and therefore printstrings
** for various permissions can be concatenated.  When true, however,
** this property does not hold and the strings should be tokenized
** before input or output. 
**
** The acl_mgr_info string provides a name and
** help info for the manager type as well as the complete set of
** supported permission bits. total_num_printstrings is the total
** number of acl printstrings supported by this acl manager type - if
** it is greater than the size avail, this function should be invoked
** again with a buffer of the appropriate size.
**
** If acl_mgr_type supports more than 32 permission
** bits, multiple manager types can be used - one for each 32 bit wide
** slice of permissions.  When this is the case the acl_mgr_type_chain
** parameter is set to the uuid of the next manager type in the set.  The
** final result for the chain returns uuid_nil in the manager_type_chain
** parameter.
**
**	In Parameters:
**		h - opaque handle to the schema on which this
**			operation is being performed
**		acl_mgr_type - the UUID of the acl_mgr_type for which
**			the printstrings are to be returned.
**		size_avail - size of the printstrings array
**
**	Out Parameters:
**		acl_mgr_type_chain - if not uuid_nil, identifies the
**			next acl_mgr_type UUID in a chain supporting
**			acl managers with more than 32 permission bits.
**		acl_mgr_info - printstrings containing the name, help info,
**			and complete set of supported permission bits
**			for this acl_mgr.
**		tokenize - if true, permission bit strings should be
**			tokenized.
**		total_num_permstrings -  the total number of permission 
**			printstrings supported by this acl_mgr_type.
**		size_used - number of perm printstrings returned
**		permstrings - array containing the printstrings for
**			each permission supported by this acl_mgr_type.
**
** Errors:
**  dce_attr_s_not_implemented
**--
*/
PUBLIC
void dce_attr_sch_aclmgr_strings (
	/* [in] */ dce_attr_sch_handle_t	h,
	/* [in]	*/ uuid_t			*acl_mgr_type,   
	/* [in] */ unsigned32			size_avail,
	/* [out]*/ uuid_t			*acl_mgr_type_chain,
	/* [out]*/ sec_acl_printstring_t	*acl_mgr_info,
	/* [out]*/ boolean32			*tokenize,
	/* [out]*/ unsigned32			*total_num_printstrings,
	/* [out]*/ unsigned32			*size_used,
	/* [out]*/ sec_acl_printstring_t	permstrings[],
	/* [out]*/ error_status_t		*st
)
{
	dce_attr_binding_handle_rep_t		*binding_handle;
	/*
	rs_cache_data_t				cache_info;
	*/

	CLEAR_STATUS(st);

	binding_handle = (dce_attr_binding_handle_rep_p_t) h;

	DCE_ATTR_BIND_VALIDATE_SCH(binding_handle, st);

	/*
	** Call the corresponding wire protocol and ignore the cache_info 
	** because there is no rs cache to update in this case
	*/
	SET_STATUS(st, dce_attr_s_not_implemented);

	return;	
}

