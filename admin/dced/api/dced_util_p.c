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
 * $Log: dced_util_p.c,v $
 * Revision 1.1.4.2  1996/02/18  19:15:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:50  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:26:42  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/23  00:22 UTC  tatsu_s
 * 	Added dced_c_binding_syntax_local.
 * 	[1995/12/08  17:59:08  root]
 * 
 * Revision 1.1.2.13  1994/08/24  19:26:55  bowe
 * 	Expunge key encryption and transmit_rep code. [CR 11773]
 * 	[1994/08/24  19:10:25  bowe]
 * 
 * Revision 1.1.2.12  1994/08/03  22:06:31  pwang
 * 	Added local keytab support [OT 11280]
 * 	[1994/08/03  22:02:37  pwang]
 * 
 * Revision 1.1.2.11  1994/07/06  20:17:05  pwang
 * 	Reorg dced free routines [OT#11003]
 * 	[1994/07/06  20:06:23  pwang]
 * 
 * Revision 1.1.2.10  1994/06/09  16:02:55  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:31  devsrc]
 * 
 * Revision 1.1.2.9  1994/05/18  20:21:14  pwang
 * 	Used macro DCED_GEN_RANDOM_KEY to generate a random key.
 * 	[1994/05/18  20:20:32  pwang]
 * 
 * Revision 1.1.2.8  1994/05/05  18:06:13  bowe
 * 	New error codes: dced_s_not_found [10524]
 * 	[1994/05/05  17:58:49  bowe]
 * 
 * Revision 1.1.2.7  1994/05/05  14:41:11  pwang
 * 	Rename _rkeytab_ to _keytab_.
 * 	[1994/05/05  14:35:21  pwang]
 * 
 * Revision 1.1.2.6  1994/04/15  22:29:06  pwang
 * 	General cleanup and fixed a number of problems.
 * 	[1994/04/15  21:38:03  pwang]
 * 
 * Revision 1.1.2.5  1994/04/14  20:08:27  rsalz
 * 	Make server and API all compile.
 * 	[1994/04/14  19:45:05  rsalz]
 * 
 * Revision 1.1.2.4  1994/04/11  18:45:48  rsalz
 * 	Update to final RFC revision.
 * 	[1994/04/11  18:26:17  rsalz]
 * 
 * Revision 1.1.2.3  1994/03/11  00:07:26  pwang
 * 	Replaced the sec_attr_t cleanup routine with
 * 	sec_attr_util_inst_free_ptrs()
 * 	[1994/03/10  23:57:57  pwang]
 * 
 * Revision 1.1.2.2  1994/03/09  20:57:10  pwang
 * 	Modified to match with attribute changes
 * 	[1994/03/09  20:55:50  pwang]
 * 
 * Revision 1.1.2.1  1994/02/28  16:27:05  pwang
 * 	Initial Implementation.
 * 	[1994/02/28  16:11:02  pwang]
 * 
 * $EndLog$
 */

/*
** Module: dced_util_p.c
**
** FACILITY: DCE Host Daemon API
**
** ABSTRACT: This module implements the dced api internal functions
**
** SCOPE: private 
**
** Date:
*/

#include <dcedimpl.h>
#include <stdarg.h>

/*
**++
**
** dced__service_from_name converts a service name strings to
** a dced service type.
**
**-- 
*/
void
dced__service_from_name(
	/* [in]  */	dced_string_t 		name,
	/* [out] */	dced_service_type_t	*service,
	/* [out] */	error_status_t		*st
)
{
	
	CLEAR_STATUS(st);

        if (!strncasecmp(dced_c_service_srvrconf, (char *)name,
			 dced_c_service_srvrconf_len)) {
                *service = dced_e_service_type_srvrconf;
        } else if (!strncasecmp(dced_c_service_srvrexec, (char *)name,
                                dced_c_service_srvrexec_len)) {
                *service = dced_e_service_type_srvrexec;
        } else if (!strncasecmp(dced_c_service_hostdata, (char *)name,
                                dced_c_service_hostdata_len)) {
                *service = dced_e_service_type_hostdata;
        } else if (!strncasecmp(dced_c_service_keytab, (char *)name,
                                dced_c_service_keytab_len)) {
                *service = dced_e_service_type_keytab;
        } else if (!strncasecmp(dced_c_service_secval, (char *)name,
                                dced_c_service_secval_len)) {
                *service = dced_e_service_type_secval;
        } else {
		SET_STATUS(st, dced_s_unknown_service);
        }

        return;
}

/*
**++
**
** dced__get_entry_list reads the entry list of a dced service from a
** specified host.
** If the list is NULL, the entry list will be loaded into the local
** cache.
**--
*/
void
dced__get_entry_list(
	/* [in]  */	dced_binding_handle_t	h,
	/* [out] */	dced_entry_list_t	*list,
	/* [out] */	error_status_t          *st
)
{
	dced_entry_list_t	entry_list;

	CLEAR_STATUS(st);

	entry_list.count = 0;
	entry_list.list = NULL;

	switch(DCED_SERVICE(h)) {

	case dced_e_service_type_ep:
	case dced_e_service_type_secval:
		/*
		** There is not entry being readable by user
		** in ep and secval services of dced
		*/
		SET_STATUS(st, dced_s_no_support);
		break;
	case dced_e_service_type_srvrconf:
		/*
		** Get the srvrconf entry list. 
		*/
		Rsrvrconf_get_list(h, &entry_list, st);
		break;
	case dced_e_service_type_srvrexec:
		/*
		** Get the srvrexec entry list. 
		*/
		Rsrvrexec_get_list(h, &entry_list, st); 
		break;
	case dced_e_service_type_hostdata:
		/*
		** Get the hostdata entry list. 
		*/
		Rhostdata_get_list(h, &entry_list, st); 
		break;
	case dced_e_service_type_keytab:	
		/*
		** Get the keytab entry list. 
		*/
		Rkeytab_get_list(h, &entry_list, st); 
		break;
	default:
		SET_STATUS(st, dced_s_bad_binding);
		break;
	}

	CHECK_STATUS_RET(st);

	/*
	** If the list == NULL, the entry list will be loaded into local
	** cache
	*/
	if (list) {
		list->count = entry_list.count;
		list->list = entry_list.list;
	} else {
		CACHE_COUNT(h) = entry_list.count;
		CACHE_HEAD(h) = entry_list.list;
	}
	return;	
}

/*
**++
**
** dced__cache_delete erases/zeroes a entry in the cache.
**
**--
*/
void
dced__cache_delete(
	/* [in]  */	dced_binding_handle_t	h,
	/* [in]  */	uuid_t			*id,
	/* [out] */	error_status_t          *st
)
{
	dced_entry_t	*entry;
	int		i, count;
	boolean32	found = FALSE;

	CLEAR_STATUS(st);

	if (!VALID_CACHE(h)) {
		return;
	}
	/*
	** Walk thr the cache to find the entry which is
	** identified by id
	*/ 
	count = CACHE_COUNT(h);
	entry = CACHE_HEAD(h);
	for (i=0; i<count; i++) {
		if (found = uuid_equal(&entry[i].id, id, st)) {
			entry_free_ptrs(&entry[i], dced_sm_client_free);
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
** dced__load_cache loads local cache
**
**--
*/
void 
dced__load_cache(
	/* [in]  */	dced_binding_handle_t	h,
	/* [out  */	error_status_t		*st
)
{

	CLEAR_STATUS(st);

	if (VALID_CACHE(h)) {
		return;
	}

	/*
	** Unload the invalid cache
	*/ 
	dced__unload_cache(h, st);
	CHECK_STATUS_RET(st);

	/*
	** Load the cache
	*/
	dced__get_entry_list(h, NULL, st);
	CHECK_STATUS_RET(st);
	SET_VALID_CACHE(h);

	return;
}


/*
**++
**
** dced__unload_cache unloads the local cache
**
**--
*/
void 
dced__unload_cache(
	/* [in]  */	dced_binding_handle_t	h,
	/* [out] */	error_status_t		*st
)
{
	
	CLEAR_STATUS(st);

	if (EMPTY_CACHE(h)) {
		return;
	}

	Dced_entry_list_free_ptrs(h, &CACHE_DATA(h));

	CACHE_STATE(h) = dced_e_cache_state_null;
	CACHE_COUNT(h) = 0;
	CACHE_HEAD(h) = NULL;
		
	return;
}

