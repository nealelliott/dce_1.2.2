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
 * $Log: dced_rkeytab.c,v $
 * Revision 1.1.4.2  1996/02/18  19:15:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:43  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:26:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:58:58  root]
 * 
 * Revision 1.1.2.9  1994/08/24  19:26:51  bowe
 * 	Expunge key encryption and transmit_rep code. [CR 11773]
 * 
 * 	Expunge key encryption and transmit_rep code. [CR 11773]
 * 	Revision 1.1.2.8  1994/08/03  22:06:29  pwang
 * 	Added local keytab support [OT#11280]
 * 	[1994/08/03  21:59:05  pwang]
 * 
 * Revision 1.1.2.7  1994/06/09  16:02:50  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:26  devsrc]
 * 
 * Revision 1.1.2.6  1994/05/31  21:08:46  pwang
 * 	Set cursor to NULL in *_release_cursor().
 * 	[1994/05/31  20:52:32  pwang]
 * 
 * Revision 1.1.2.5  1994/05/05  18:06:12  bowe
 * 	New error codes: dced_s_no_more_entriesi [10524]
 * 	[1994/05/05  17:58:31  bowe]
 * 
 * Revision 1.1.2.4  1994/05/05  14:41:13  pwang
 * 	Rename _rkeytab_ to _keytab_.
 * 	[1994/05/05  14:34:05  pwang]
 * 
 * Revision 1.1.2.3  1994/04/15  22:29:02  pwang
 * 	General cleanup and fixed a number of problems.
 * 	[1994/04/15  21:31:08  pwang]
 * 
 * Revision 1.1.2.2  1994/03/09  22:05:19  pwang
 * 	Fixed a overlook in dced_keytab_release_cursor().
 * 	[1994/03/09  22:04:48  pwang]
 * 
 * Revision 1.1.2.1  1994/02/28  16:27:02  pwang
 * 	Initial Implementation.
 * 	[1994/02/28  16:08:55  pwang]
 * 
 * $EndLog$
 */

/*
** Module: dced_rkeytab.c
**
** FACILITY: DCE Host Daemon API
**
** ABSTRACT: This module implements the dced rkeytab functions
**
** SCOPE: Public 
**
** Date:
*/


#include <dcedimpl.h>

/*
**++
**
** dced_keytab_create creates a keytab entry and stores a list
** of keys to the keytab maintained by dced on a specified host.
** The difference between the this api and dced_entry_add
** is that the former creates a entry and keys but the latter 
** only adds a entry.
**
**--
*/
void 
dced_keytab_create(
	/* [in]      */	dced_binding_handle_t	h,
	/* [in, out] */	dced_entry_t		*entry,
	/* [in, out] */	dced_key_list_t		*keys,
	/* [out]     */	error_status_t		*st
)
{
	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for rkeytab,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_keytab, st);


	/*
	** If the caller doesn't supply a uuid for the
	** new entry, call uuid_create to create one.
	*/
	if (uuid_is_nil(&entry->id, st)) {
		uuid_create(&entry->id, st);
		CHECK_STATUS_RET(st);
	}

	Rkeytab_create(h, entry, FALSE, keys, st);
	CHECK_STATUS_RET(st);

	/*
	** Invalidate the local cache
	*/      
	SET_INVALID_CACHE(h);

	return;
} 
	
/*
**++
**
** dced_keytab_delete deletes a keytab entry and 
** its keys maintained by dced on a specified host.
** The difference between the this api and dced_entry_remove
** is that the former deletes a entry and its keytab but 
** the latter only removes a entry.
**
**--
*/
void 
dced_keytab_delete(
	/* [in]  */	dced_binding_handle_t	h,
	/* [in]  */	uuid_t			*id,
	/* [out] */	error_status_t		*st
)
{

	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for rkeytab,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_keytab, st);

	Rkeytab_delete(h, id, TRUE, st);
	CHECK_STATUS_RET(st);

	/*
	** delete this keytab entry from local cache
	*/
	dced__cache_delete(h, id, st);
	if (!GOOD_STATUS(st)) {
		SET_INVALID_CACHE(h);
		RESET_STATUS(st);
	}

        return;
}
	
/*
**++
**
** dced_keytab_initialize_cursor generates a keytable cursor to
** traverse the requested keytable for particular principal's key(s)
**
**--
*/
void
dced_keytab_initialize_cursor(
	/* [in]  */	dced_binding_handle_t	h,
	/* [in]  */	uuid_t			*id,
	/* [out] */	dced_keytab_cursor_t	*cursor,
	/* [out] */	error_status_t		*st
)
{
	struct dced_keytab_cursor_s_t	*ptr;
	dced_key_list_t			*key_table;
	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for rkeytab,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_keytab, st);

	/*
	** Allocate the dced keytable cursor
	*/
	MALLOC_RESET_RET(ptr, struct dced_keytab_cursor_s_t, 1, st);
	
	/*
	** Fetch the keytab object
	*/
	dced_object_read(h, id, (void **) &key_table, st); 
	if (!GOOD_STATUS(st)) {
		free(ptr);
		return;
	}
	
	/*
	** Initialize the cursor
	*/
	SET_KEYTAB_CURSOR(ptr, h, 0, key_table);
	*cursor = ptr;

	return;
}

/*
**++
**
** dced_keytab_get_next_key returns the key under the cursor
**
**--
*/
void
dced_keytab_get_next_key(
	/* [in,out] */	dced_keytab_cursor_t	cursor,
	/* [out]    */	dced_key_t		**key,
	/* [out]    */	error_status_t		*st
)
{
	dced_key_list_t	*keytab;
	int		pos;

	CLEAR_STATUS(st);

	keytab = CURSOR_KEYS(cursor);
	pos = CURSOR_PTR(cursor);

	if (pos < keytab->count) {
		*key = &keytab->list[pos];
		CURSOR_PTR(cursor)++;
	} else {        
		SET_STATUS(st, dced_s_no_more_entries);
	}

	return;
}

/*
**++
**
** dced_keytab_release_cursor relases a keytable cursor
**
**--
*/
void
dced_keytab_release_cursor(
	/* [in,out] */	dced_keytab_cursor_t	*cursor,
	/* [out]    */	error_status_t		*st
)
{

	CLEAR_STATUS(st);

	if (!cursor || !(*cursor)) {
		return;
	}

	/*
	** Release the keytab object
	*/
	if (CURSOR_KEYS(*cursor)) {
		dced_objects_release(CURSOR_BINDING(*cursor), 
				     1, 
				     (void *) CURSOR_KEYS(*cursor), 
				     st);
		CHECK_STATUS_RET(st);
	}

	/*
	** Free the cursor
	*/
	SET_KEYTAB_CURSOR(*cursor, NULL, 0, NULL);
	free(*cursor);
	*cursor = NULL;

	return;
}

/*
**++
**
** dced_keytab_add_key adds a key to the keytab 
** on a specified host. 
**
**--
*/
void 
dced_keytab_add_key(
	/* [in]      */	dced_binding_handle_t	h,
	/* [in]      */	uuid_t			*id,
	/* [in,out]  */	dced_key_t		*key,
	/* [out]     */	error_status_t		*st
)
{
	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for rkeytab,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_keytab, st);

	Rkeytab_add_key(h, id, key, st);

	return;
}

/*
**++
**
** dced_keytab_remove_key removes a key from the keytab 
** on a specified host.
**
**--
*/
void 
dced_keytab_remove_key(
	/* [in]      */	dced_binding_handle_t	h,
	/* [in]      */	uuid_t			*id,
	/* [in,out]  */	dced_key_t		*key,
	/* [out]     */	error_status_t		*st
)
{

	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for rkeytab,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_keytab, st);

	Rkeytab_delete_key(h, id, key, st);

	return;
}

/*
**++
**
** dced_keytab_change_key updates a key in both registry and
** keytab on a specified host.
**
**--
*/
void 
dced_keytab_change_key(
	/* [in]       */	dced_binding_handle_t	h,
	/* [in]       */	uuid_t			*id,
	/* [in, out]  */	dced_key_t		*key,
	/* [out]      */	error_status_t		*st
)
{
	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for rkeytab,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_keytab, st);
	
	Rkeytab_change_key(h, id, key, st);

	return;
}

