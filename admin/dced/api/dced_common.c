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
 * $Log: dced_common.c,v $
 * Revision 1.1.4.2  1996/02/18  19:15:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:26:04  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/23  00:22 UTC  tatsu_s
 * 	Added dced_c_binding_syntax_local.
 * 	[1995/12/08  17:58:48  root]
 * 
 * Revision 1.1.2.11  1994/08/24  19:26:50  bowe
 * 	Expunge key encryption and transmit_rep code. [CR 11773]
 * 	[1994/08/24  19:09:17  bowe]
 * 
 * Revision 1.1.2.10  1994/08/03  22:06:28  pwang
 * 	Added local keytab support [OT#11280]
 * 	[1994/08/03  21:58:41  pwang]
 * 
 * Revision 1.1.2.9  1994/07/28  14:03:40  rsalz
 * 	Use dce_strdup (OT CR 11343).
 * 	[1994/07/28  13:41:40  rsalz]
 * 
 * Revision 1.1.2.8  1994/07/06  20:17:01  pwang
 * 	Reorg dced free routines [OT#11003]
 * 	[1994/07/06  20:04:32  pwang]
 * 
 * Revision 1.1.2.7  1994/06/09  16:02:48  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:24  devsrc]
 * 
 * Revision 1.1.2.6  1994/05/31  21:08:39  pwang
 * 	Set the cursor to NULL in *_release_cursor().
 * 	[1994/05/31  20:49:37  pwang]
 * 
 * Revision 1.1.2.5  1994/05/05  18:06:10  bowe
 * 	New error codes: dced_s_not_found,dced_s_no_more_entries
 * 	[1994/05/05  17:58:02  bowe]
 * 
 * Revision 1.1.2.4  1994/05/05  14:41:06  pwang
 * 	Rename _cursor_ routines and _rkeytab_ variables.
 * 	[1994/05/05  14:12:00  pwang]
 * 
 * Revision 1.1.2.3  1994/04/15  22:29:00  pwang
 * 	General cleanup and fixed a number of problems.
 * 	[1994/04/15  21:30:12  pwang]
 * 
 * Revision 1.1.2.2  1994/03/09  20:52:36  pwang
 * 	Modified to match with attribute changes
 * 	[1994/03/09  20:52:17  pwang]
 * 
 * Revision 1.1.2.1  1994/02/28  16:26:57  pwang
 * 	Initial Implementation.
 * 	[1994/02/28  16:07:38  pwang]
 * 
 * $EndLog$
 */

/*
** Module: dced_common.c
**
** FACILITY: DCE Host Daemon API
**
** ABSTRACT: This module implements the dced api common functions
**
** SCOPE: Public 
**
** Date:
*/

#include <dcedimpl.h>

static	sec_attr_t	s_null_sec_attr;
static	dced_key_list_t	s_null_key_list;
static	uuid_t		s_null_uuid;

/*
**++
**
** dced_inq_id inquires one's uuid by its name from a specified host.
**
**--
*/
void
dced_inq_id(
	/* [in]  */	dced_binding_handle_t   h,
	/* [in]  */	dced_string_t           name,
	/* [out] */	uuid_t                  *id,
	/* [out] */	error_status_t          *st
)
{
	int		i;
	boolean32	found = FALSE;
	
	CLEAR_STATUS(st);

	/*
	** Null the id in case of error before finishing
	*/
	*id = s_null_uuid;

	/*
	** Load the local cache if it's not there or is invalid 
	*/
	if (!VALID_CACHE(h)) {
		dced__load_cache(h, st);
		CHECK_STATUS_RET(st);
	}	

	/*
	** Walk thr the entry list and search for the name
	*/	
	for (i=0; i<CACHE_COUNT(h); i++) {
		if (!(strcmp((char *)CACHE_ENTRY(h, i).name, (char *)name))) {
			memcpy(id, &CACHE_ENTRY(h, i).id, sizeof(uuid_t));
			found = TRUE;
			break;
		}
	}
	
	if (!found) {
		SET_STATUS(st, dced_s_not_found);
	}
	return;
}

/*
**++
**
** dced_inq_name inquires one's name by its uuid from a specified host.
**
**--
*/
void
dced_inq_name(
	/* [in]  */	dced_binding_handle_t   h,
	/* [in]  */	uuid_t                  *id,
	/* [out] */	dced_string_t           *name,
	/* [out] */	error_status_t          *st
)
{
	int		i;
	boolean32	found = FALSE;
	
	CLEAR_STATUS(st);

	/*
	** Null the name in case of error before finishing
	*/
	*name = NULL;

	/*
	** Load the local cache if it's not there or is invalid 
	*/
	if (!VALID_CACHE(h)) {
		dced__load_cache(h, st);
		CHECK_STATUS_RET(st);
	}	

	/*
	** Walk thr the entry list and search for the uuid 
	*/      
	for (i=0; i<CACHE_COUNT(h); i++) {
		if (uuid_equal(&CACHE_ENTRY(h, i).id, id, st)) { 
			*name = (unsigned char *) 
				dce_strdup((char *) CACHE_ENTRY(h, i).name);	
			found = TRUE;
			break;
		}
		CHECK_STATUS_RET(st);
	}

	if (!found) {
		SET_STATUS(st, dced_s_not_found);
	}
	return;
}


/*
**++
**
** dced_list_get retruns a list of entries maintained by 
** dced on a specified host.
**
**--
*/
void
dced_list_get(
	/* [in]  */	dced_binding_handle_t   h,
	/* [out] */	dced_entry_list_t	*list,
	/* [out] */	error_status_t          *st
)
{

	CLEAR_STATUS(st);

	/*
	** get entry list from dced server
	*/
	dced__get_entry_list(h, list, st);

	return;	
}

/*
**++
**
** dced_list_release release the list return by dced_list_get
**
**--
*/
void
dced_list_release(
	/* [in]      */	dced_binding_handle_t   h,
	/* [in,out]  */	dced_entry_list_t	*list,
	/* [out]     */	error_status_t          *st
)
{
	
	CLEAR_STATUS(st);

	/*
	** Free  entry list
	*/
	Dced_entry_list_free_ptrs(h, list);

	return;	
}

/*
**++
**
** dced_initialize_cursor sets the cursor to the starting position 
** of the cached list. This cursor will be used to traverse this
** list.
**
**--
*/
void
dced_initialize_cursor(
	/* [in]  */	dced_binding_handle_t   h,
	/* [out] */	dced_cursor_t		*cursor,
	/* [out] */	error_status_t          *st
)
{
	struct dced_cursor_s_t  *ptr;

	CLEAR_STATUS(st);

	/*
	** Allocate the dced cursor structure
	*/
	MALLOC_RESET_RET(ptr, struct dced_cursor_s_t, 1, st);

	if (!VALID_CACHE(h)) {
		dced__load_cache(h, st);
		if (!GOOD_STATUS(st)) {
			free(ptr);
			return;
		}
	}
	SET_CURSOR(ptr, h, 0);

	*cursor = ptr;
	return;
}

/*
**++
**
** dced_entry_get_next returns the entry pointed by cursor and advances
** cursor. 
**
**--
*/
void
dced_entry_get_next(
	/* [in,out] */	dced_cursor_t		cursor,
	/* [out]    */	dced_entry_t		**entry,
	/* [out]    */	error_status_t          *st
)
{
	dced_binding_handle_t	h;
	int			pos;

	CLEAR_STATUS(st);

	h = CURSOR_BINDING(cursor);
	pos = CURSOR_PTR(cursor);

	if (pos < CACHE_COUNT(h)) {
		*entry = &CACHE_ENTRY(h, pos);
		CURSOR_PTR(cursor)++;
	} else {	
		SET_STATUS(st, dced_s_no_more_entries);
	}

	return;
}

/*
**++
**
** dced_release_cursor releases the cursor.
**
**--
*/
void
dced_release_cursor(
	/* [in,out] */	dced_cursor_t		*cursor,
	/* [out]    */	error_status_t          *st
)
{

	CLEAR_STATUS(st);

	if (!cursor || !(*cursor)) {
		return;
	}

	SET_CURSOR(*cursor, NULL, 0);
	free(*cursor);
	*cursor = NULL;

	return;
}

/*
**++
**
** dced_entry_add adds a entry to dced on a specified host.
**
**--
*/
void
dced_entry_add(
	/* [in]     */	dced_binding_handle_t   h,
	/* [in,out] */	dced_entry_t		*entry,
	/* [out]    */	error_status_t          *st
)
{
	sec_attr_t	dced_attr = s_null_sec_attr;
	dced_key_list_t	dced_key_list = s_null_key_list;

	CLEAR_STATUS(st);

	/*
	** If the caller doesn't supply a uuid for the
	** new entry, call uuid_create to create one.
	*/
	if (uuid_is_nil(&entry->id, st)) {
		uuid_create(&entry->id, st);
		CHECK_STATUS_RET(st);
	}

	switch (DCED_SERVICE(h)) {

	case dced_e_service_type_ep:
	case dced_e_service_type_srvrconf:
	case dced_e_service_type_srvrexec:
	case dced_e_service_type_secval:
		/*
		** entry creation operation by user is not directly 
		** supported in ep, srvrconf, srvrexec, secval service.
		** They are internally created by dced.
		*/
		SET_STATUS(st, dced_s_no_support);
		break;
	case dced_e_service_type_hostdata:
		dced_attr.attr_value.attr_encoding = sec_attr_enc_void;
		Rhostdata_create(h, entry, TRUE, &dced_attr, st);
		break;
	case dced_e_service_type_keytab:
		Rkeytab_create(h, entry, TRUE, &dced_key_list, st);
		break;
	default:
		SET_STATUS(st, dced_s_bad_binding);
		break;
	}

	/*
	** Invalidate the caches
	*/
	if (GOOD_STATUS(st) && VALID_CACHE(h)) {
		SET_INVALID_CACHE(h);
	}

	return;
}

/*
**++
**
**  dced_entry_remove removes a entry from dced on a 
**  specified host.
**
**--
*/
void
dced_entry_remove(
	/* [in]  */	dced_binding_handle_t   h,
	/* [in]  */	uuid_t			*id,
	/* [out] */	error_status_t          *st
)
{
	
	CLEAR_STATUS(st);

	switch (DCED_SERVICE(h)) {

	case dced_e_service_type_ep:
	case dced_e_service_type_srvrconf:
	case dced_e_service_type_srvrexec:
	case dced_e_service_type_secval:
		/*
		** entry deletion operation by user is not directly 
		** supported in ep, srvrconf, srvrexec, secval service.
		** They are internally controlled by dced.
		*/
		SET_STATUS(st, dced_s_no_support);
		break;
	case dced_e_service_type_hostdata:
		Rhostdata_delete(h, id, FALSE, st);
		break;
	case dced_e_service_type_keytab:
		Rkeytab_delete(h, id, FALSE, st);
		break;
	default:
		SET_STATUS(st, dced_s_bad_binding);
		break;
	}

	/*
	** Delete this entry from local cache
	*/
	if (GOOD_STATUS(st)) {
		dced__cache_delete(h, id, st);
		if (!GOOD_STATUS(st)) {
			SET_INVALID_CACHE(h);
			RESET_STATUS(st);
		}
	}

	return;
}

/*
**++
**
** dced_object_read_all reads all object data of a dced service from a 
** specified host.
**
**--
*/
void
dced_object_read_all(
	/* [in]  */	dced_binding_handle_t   h,
	/* [out] */	unsigned32		*count,
	/* [out] */	void			**list, 
	/* [out] */	error_status_t          *st
)
{
	int		i, j, ent_cnt;
	dced_entry_t	*entries;
	server_t	*sc_ptr;
	sec_attr_t	*hd_ptr;
	dced_key_list_t	*rk_ptr;
	error_status_t	status;

	CLEAR_STATUS_2(st, &status);

	/*
	**  If the cache is not valid, load cache
	*/
	if (!VALID_CACHE(h)) {
		dced__load_cache(h, st);
		CHECK_STATUS_RET(st);
	}
	/*
	** Initialize the count and list in case it is failed
	** in the middle.
	*/
	*count = 0;
	*list = NULL;

	ent_cnt = CACHE_COUNT(h);
	entries = CACHE_HEAD(h);
	/*
	** For each entry of entry_list, get its object and
	** cache the object in the h->object_list.
	*/
	switch (DCED_SERVICE(h)) {
	case dced_e_service_type_ep:
	case dced_e_service_type_secval:
		/*
		** There is not object being readable by user
		** in ep and secval services of dced
		*/
		SET_STATUS(st, dced_s_no_support);
		break;
	case dced_e_service_type_srvrconf:
		/*
		** srvrconf service uses the same object structure
		** as srvrexec, the different is the object doesn't 
		** contain the srvrexec_data_t here.
		*/
		MALLOC_RESET_RET(sc_ptr, server_t, ent_cnt, st);

		/*
		** Fetch the srvrconf objects
		*/
		for (i=0; i<ent_cnt; i++) {
			Rsrvrconf_get(h, &entries[i].id, &sc_ptr[i], st);
			if (!GOOD_STATUS(st)) {
				for (j=0; j<i; j++) {
					server_free_ptrs(&sc_ptr[j],
							 dced_sm_client_free);
				}
				free(sc_ptr);
				return;
			}
		}
		*count = ent_cnt;
		*list = sc_ptr;
		break;
	case dced_e_service_type_srvrexec:
		/*
		** srvrexec service uses the same object structure
		** as srvrconf, the different is the object contains 
		** the srvrexec_data_t here.
		*/
		MALLOC_RESET_RET(sc_ptr, server_t, ent_cnt, st);

		/*
		** Fetch the srvrexec objects
		*/
		for (i=0; i<ent_cnt; i++) {
			Rsrvrexec_get(h, &entries[i].id, &sc_ptr[i], st);
			if (!GOOD_STATUS(st)) {
				for (j=0; j<i; j++) {
					server_free_ptrs(&sc_ptr[j],
							 dced_sm_client_free);
				}
				free(sc_ptr);
				return;
			}
		}
		*count = ent_cnt;
		*list = sc_ptr;
		break;
	case dced_e_service_type_hostdata:
		/*
		** Allocate the hostdata object list
		*/
		MALLOC_RESET_RET(hd_ptr, sec_attr_t, ent_cnt, st);

		/*
		** Fetch the hostdata data -): reading file contents
		** attributes only
		*/
		for (i=0; i<ent_cnt; i++) {
			Rhostdata_get(h, &entries[i].id, 
					&dced_g_uuid_fileattr, 
					&hd_ptr[i], st);
			if (!GOOD_STATUS(st)) {
				for (j=0; j<i; j++) {
					sec_attr_util_free(dced_sm_client_free,
							   &hd_ptr[j]);
				}
				free(hd_ptr);
				return;
			}
		}
		*count = ent_cnt;
		*list = hd_ptr;
		break;
	case dced_e_service_type_keytab:	
		/*
		** Allocate the rkeytab object list
		*/
		MALLOC_RESET_RET(rk_ptr, dced_key_list_t, ent_cnt, st); 

		/*
		** Fetch the key tables.
		*/
		for (i=0; i<ent_cnt; i++) {
			Rkeytab_get_keys(h, &entries[i].id, &rk_ptr[i], st);
			if (!GOOD_STATUS(st)) {
				for (j=0; j<i; j++) {
					Dced_key_list_free_ptrs(h, &rk_ptr[j]);
				}
				free(rk_ptr);
				return;
			} 
		}
		*count = ent_cnt;
		*list = rk_ptr;
		break;
	default:
		SET_STATUS(st, dced_s_bad_binding);
		break;
	}

	return;	
}

/*
**++
**
** dced_object_read reads object data of dced from a 
** specified host.
**
**--
*/
void
dced_object_read(
	/* [in]  */	dced_binding_handle_t   h,
	/* [in]  */	uuid_t			*id,
	/* [out] */	void			**data, 
	/* [out] */	error_status_t          *st
)
{
	server_t	*sc_ptr;
	sec_attr_t	*hd_ptr;
	dced_key_list_t	*rk_ptr;

	CLEAR_STATUS(st);

	/*
	** Initialize the return data in case it's failed in the
	** middle
	*/
	*data = NULL;

	switch (DCED_SERVICE(h)) {

	case dced_e_service_type_ep:
	case dced_e_service_type_secval:
		/*
		** There is not object being read by user
		** in ep and secval services of dced
		*/
		SET_STATUS(st, dced_s_no_support);
		break;
	case dced_e_service_type_srvrconf:
		/*
		** srvrconf service uses the same object structure
		** as srvrexec, the different is the object doesn't 
		** contain the srvrexec_data_t here.
		*/
		MALLOC_RESET_RET(sc_ptr, server_t, 1, st);
		/*
		** Read the data from dced server
		*/
		Rsrvrconf_get(h, id, sc_ptr, st);
		if (!GOOD_STATUS(st)) {
			free(sc_ptr);
			break;
		}
		*data = sc_ptr;
		break;
	case dced_e_service_type_srvrexec:
		/*
		** srvrexec service uses the same object structure
		** as srvrconf, the different is the object contains 
		** the srvrexec_data_t here.
		*/
		MALLOC_RESET_RET(sc_ptr, server_t, 1, st);
		/*
		** Read the data from dced server
		*/
		Rsrvrexec_get(h, id, sc_ptr, st);
		if (!GOOD_STATUS(st)) {
			free(sc_ptr);
			break;
		}
		*data = sc_ptr;
		break;
	case dced_e_service_type_hostdata:
		/*
		** Allocate the hostdata object list
		*/
		MALLOC_RESET_RET(hd_ptr, sec_attr_t, 1, st);

		/*
		** Read the data from dced server -): read the
		** file contents attributes only
		*/
		Rhostdata_get(h, id, &dced_g_uuid_fileattr, hd_ptr, st);
		if (!GOOD_STATUS(st)) {
			free(hd_ptr);
			break;
		}
		*data = hd_ptr;
		break;
	case dced_e_service_type_keytab:
		/*
		** Allocate the rkeytab object list
		*/
		MALLOC_RESET_RET(rk_ptr, dced_key_list_t, 1, st);

		/*
		** Read the data from dced server
		*/
		Rkeytab_get_keys(h, id, rk_ptr, st);
		if (!GOOD_STATUS(st)) {
			free(rk_ptr);
			break;
		}
		*data = rk_ptr;
		break;
	default:
		SET_STATUS(st, dced_s_bad_binding);
		break;
	}

	return;
}

/*
**++
**
** dced_objects_release releases object data of dced
**
**--
*/
void
dced_objects_release(
	/* [in]     */	dced_binding_handle_t   h,
	/* [in]     */	unsigned32		count,
	/* [in,out] */	void			*objects, 
	/* [out]    */	error_status_t          *st
)
{
	int		i;
	server_t	*sc_ptr;
	sec_attr_t	*hd_ptr;
	dced_key_list_t	*rk_ptr;

	CLEAR_STATUS(st);

	if ((count == 0) || (objects == NULL)) {
		return;
	}
	/* 
	** free each object in the list
	*/
	switch (DCED_SERVICE(h)) {

	case dced_e_service_type_ep:
	case dced_e_service_type_secval:
		/*
		** There is not object being handled by user
		** in ep and secval services of dced
		*/
		SET_STATUS(st, dced_s_no_support);
		break;
	case dced_e_service_type_srvrconf:
	case dced_e_service_type_srvrexec: 
		sc_ptr = (server_t *) objects;
		/*
		** Free the memory allocated by stub.
		*/
		for (i=0; i<count; i++) {
			server_free_ptrs(&sc_ptr[i], dced_sm_client_free);
		}
		break;
	case dced_e_service_type_hostdata:
		hd_ptr = (sec_attr_t *) objects;
		/*
		** Free the memory allocated by stub.
		*/
		for (i=0; i<count; i++) {
			sec_attr_util_free(dced_sm_client_free, &hd_ptr[i]);
		}
		break;
	case dced_e_service_type_keytab:
		rk_ptr = (dced_key_list_t *) objects;
		/*
		** Free the memory allocated by stub.
		*/
		for (i=0; i<count; i++) {
			Dced_key_list_free_ptrs(h, &rk_ptr[i]);
		}
		break;
	default:
		SET_STATUS(st, dced_s_bad_binding);
		break;
	}

	/*
	** Release the objects themselves
	*/
	if (GOOD_STATUS(st)) {
		free(objects);
	}
	return;	
}


