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
 * $Log: dced_hostdata.c,v $
 * Revision 1.1.4.2  1996/02/18  19:15:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:42  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:26:20  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/23  00:22 UTC  tatsu_s
 * 	Added dced_c_binding_syntax_local.
 * 	[1995/12/08  17:58:56  root]
 * 
 * Revision 1.1.2.5  1994/06/09  16:02:49  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:25  devsrc]
 * 
 * Revision 1.1.2.4  1994/04/18  19:45:45  pwang
 * 	Added attr_id to dced_hostdata_read() (RFC47.3)
 * 	[1994/04/18  19:31:19  pwang]
 * 
 * Revision 1.1.2.3  1994/04/15  22:29:01  pwang
 * 	General cleanup and fixed a number of problems.
 * 	[1994/04/15  21:30:43  pwang]
 * 
 * Revision 1.1.2.2  1994/03/09  20:57:05  pwang
 * 	Modified to match with attribute changes
 * 	[1994/03/09  20:53:37  pwang]
 * 
 * Revision 1.1.2.1  1994/02/28  16:27:01  pwang
 * 	Initial Implementation.
 * 	[1994/02/28  16:08:13  pwang]
 * 
 * $EndLog$
 */

/*
** Module: dced_hostdata.c
**
** FACILITY: DCE Host Daemon API
**
** ABSTRACT: This module implements the dced hostdata functions
**
** SCOPE: Public 
**
** Date:
*/

#include <dcedimpl.h>

/*
**++
**
** dced_hostdata_create creates a configuration entry and data 
** maintained by dced on a specified host.
** The difference between the this api and dced_entry_add
** is that the former creates a entry and its data but the latter 
** only adds a entry.
**
**--
*/
void 
dced_hostdata_create(
	/* [in]     */	dced_binding_handle_t	h,
	/* [in,out] */	dced_entry_t		*entry,
	/* [in]     */	dced_attr_list_t	*data,
	/* [out]    */	error_status_t		*st
)
{

	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for hostdata,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_hostdata, st);

	/*
	** If the caller doesn't supply a uuid for the
	** new entry, call uuid_create to create one.
	*/
	if (uuid_is_nil(&entry->id, st)) {
		uuid_create(&entry->id, st);
		CHECK_STATUS_RET(st);
	}

	/*
	** At present, hostdata only support one datum per
	** entry. So if there are more than one datum
	** supplied, the first will be taken and rest of them
	** will be ignored
	*/
	Rhostdata_create(h, entry, FALSE, (sec_attr_t *)data->list, st);  

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
** dced_hostdata_delete deletes the configuration 
** entry and data maintained by dced on a specified host.
** The difference between the this api and dced_entry_remove
** is that the former deletes a entry and its hostdata but 
** the latter only removes a entry.
**
**--
*/
void 
dced_hostdata_delete(
	/* [in]  */	dced_binding_handle_t	h,
	/* [in]  */	uuid_t			*id,
	/* [out] */	error_status_t		*st
)
{

	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for hostdata,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_hostdata, st);

	Rhostdata_delete(h, id, TRUE, st);
	CHECK_STATUS_RET(st);

	/*
	** delete this entry from cache too
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
** dced_hostdata_read reads the data associated with
** id on a specified host. 
**
**--
*/
void 
dced_hostdata_read(
	/* [in]  */	dced_binding_handle_t	h,
	/* [in]  */	uuid_t			*id,
	/* [in]  */	uuid_t			*attr_id,
	/* [out] */	sec_attr_t		**data,
	/* [out] */	error_status_t		*st
)
{
	sec_attr_t	*hd_ptr;

	CLEAR_STATUS(st);

	/*
	** Initialize the return data in case it's failed in the
	** middle
	*/
	*data = NULL;

	/*
	** If the dced binding handle is not for hostdata,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_hostdata, st);

	/*
	** Allocate the hostdata object list
	*/
	MALLOC_RESET_RET(hd_ptr, sec_attr_t, 1, st);

	/*
	** Read the data from dced server
	*/
	Rhostdata_get(h, id, attr_id, hd_ptr, st);
	if (!GOOD_STATUS(st)) {
		free(hd_ptr);
		return;
	}

	*data = hd_ptr;
	return;	
}

/*
**++
**
** dced_hostdata_write replaces any existing data associated 
** with id on a specified host. 
**
**--
*/
void 
dced_hostdata_write(
	/* [in]  */	dced_binding_handle_t	h,
	/* [in]  */	uuid_t			*id,
	/* [in]  */	dced_attr_list_t	*data,
	/* [out] */	error_status_t		*st
)
{

	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for hostdata,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_hostdata, st);

	/*
	** At present, hostdata only support one datum per
	** entry. So if there are more than one datum
	** supplied, the first will be taken and rest of them
	** will be ignored
	*/
	Rhostdata_set(h, id, (sec_attr_t *)data->list, st);

	return;
}
