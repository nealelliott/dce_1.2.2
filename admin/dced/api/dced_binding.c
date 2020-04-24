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
 * $Log: dced_binding.c,v $
 * Revision 1.1.4.2  1996/02/18  19:15:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:26:00  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/29  22:36 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/23  00:21 UTC  tatsu_s
 * 	Added dced_register_local_mgrs() and dced_c_binding_syntax_local.
 * 
 * 	HP revision /main/HPDCE02/1  1995/01/27  00:08 UTC  mk
 * 	Merge in fix from mk_mothra_dced/1.
 * 
 * 	HP revision /main/mk_mothra_dced/1  1995/01/27  00:04 UTC  mk
 * 	In dced_binding_from_rpc_binding(), clear object uuid in rpc binding before
 * 	calling rpc_mgmt_ep_elt_inq_begin().
 * 	[1995/12/08  17:58:45  root]
 * 
 * Revision 1.1.2.19  1994/09/21  21:53:19  pwang
 * 	Accepted full qualified hostname [OT#12188]
 * 	[1994/09/21  21:51:55  pwang]
 * 
 * Revision 1.1.2.18  1994/09/12  15:59:12  baum
 * 	Fixed binding lookup to handle obj UUIDS
 * 	[1994/09/12  15:43:51  baum]
 * 
 * Revision 1.1.2.17  1994/09/07  21:44:17  baum
 * 	Temporary fix for OT 12071, NULL dced handle obj UUID
 * 	[1994/09/07  21:43:50  baum]
 * 
 * Revision 1.1.2.16  1994/08/03  22:06:27  pwang
 * 	Added local keytab support [OT#11280]
 * 	[1994/08/03  21:58:20  pwang]
 * 
 * Revision 1.1.2.15  1994/07/28  14:03:39  rsalz
 * 	Use dce_strdup (OT CR 11343).
 * 	[1994/07/28  13:41:30  rsalz]
 * 
 * Revision 1.1.2.14  1994/07/26  22:07:09  pwang
 * 	Don't reset the binding while creating a dced binding [OT#11211]
 * 	[1994/07/26  22:05:39  pwang]
 * 
 * Revision 1.1.2.13  1994/07/08  21:23:21  pwang
 * 	Checked servicename returned from rpc_ns_entry_inq_resolution,
 * 	it could be NULL.
 * 	[1994/07/08  21:22:48  pwang]
 * 
 * Revision 1.1.2.12  1994/06/30  19:25:05  pwang
 * 	Used NULL rpc_if_handle when inq rpc binding handle from NS
 * 	[OT#11027]
 * 
 * Revision 1.1.2.11  1994/06/23  15:39:44  pwang
 * 	Removed the status check of rpc_binding_import_done().
 * 	[1994/06/23  15:39:14  pwang]
 * 
 * Revision 1.1.2.10  1994/06/22  20:13:35  pwang
 * 	Removed the loop on binding_import_next [OT#10948].
 * 	[1994/06/22  20:11:04  pwang]
 * 
 * Revision 1.1.2.9  1994/06/09  16:02:47  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:23  devsrc]
 * 
 * Revision 1.1.2.8  1994/05/31  21:08:38  pwang
 * 	Checked rpc_s_entry_not_found in binding import loop.
 * 	[1994/05/31  20:48:01  pwang]
 * 
 * Revision 1.1.2.7  1994/05/05  14:41:12  pwang
 * 	Changed dced_binding_from_rpc_binding() to use dced_service_type_t.
 * 	Renmae _rkeytab_ variable name to _keytab_.
 * 	[1994/05/05  14:09:49  pwang]
 * 
 * Revision 1.1.2.6  1994/04/26  20:18:31  pwang
 * 	dced_s_unknown_service in dced_binding_set_auth_info()
 * 	should have been dced_s_bad_binding.
 * 	[1994/04/26  20:17:53  pwang]
 * 
 * Revision 1.1.2.5  1994/04/22  15:53:36  pwang
 * 	Call rpc_binding_reset() in dced_binding_from_rpc_binding().
 * 	[1994/04/22  15:53:13  pwang]
 * 
 * Revision 1.1.2.4  1994/04/19  17:49:11  pwang
 * 	rpc_s_no_more_entries should have been rpc_s_no_more_bindings
 * 	[1994/04/19  17:48:08  pwang]
 * 
 * Revision 1.1.2.3  1994/04/15  22:28:59  pwang
 * 	General cleanup and fixed a number of problems.
 * 	[1994/04/15  21:29:47  pwang]
 * 
 * Revision 1.1.2.2  1994/03/09  20:50:58  pwang
 * 	Implemented dced_binding_set_auth_info().
 * 	[1994/03/09  20:50:33  pwang]
 * 
 * Revision 1.1.2.1  1994/02/28  16:26:59  pwang
 * 	Initial Implementation.
 * 	[1994/02/28  16:06:58  pwang]
 * 
 * $EndLog$
 */

/*
** Module: dced_binding.c
**
** FACILITY: DCE Host Daemon API
**
** ABSTRACT: This module implements the dced api binding functions
**
** Date:
*/

#include <dcedimpl.h>
#include <dce/dce_cf_const.h>

#define	prefix_host	"hosts/"
#define	prefix_host_len	6

/*
**++
**
**  dced_binding_create is responsible for importing a dced host binding 
**  for a dced service on a specified host. The resultant binding
**  will be used by the caller to contact the requested dced service. 
**
**--
*/
void
dced_binding_create(
	/* [in]  */     dced_string_t           name,
	/* [in]  */     unsigned32		binding_syntax,
	/* [out] */     dced_binding_handle_t   *h,
	/* [out] */     error_status_t          *st
        )
{

	struct dced_binding_handle_s_t	*ptr;
	dced_service_type_t		service;
	rpc_ns_handle_t			import_context;
	rpc_binding_handle_t		rpc_binding;
	char				*name_ptr, *hostname, 
					*servicename, *entryname;
	int				servicename_len = 1,
					entryname_len = 1;
	boolean				rpc_free_names = FALSE;
	error_status_t			status;

	CLEAR_STATUS_2(st, &status);

	/*
	** There could be the following formats of input parameter 
	** hostname,
	** 	- /.../cellname/host/config/service 
	**	- /.:/host/config/service
	**	- service@host
	**	- service 
	**
	** The host should have the format as "hosts/name",
	** where the name is the return of unix command "hostname",
	** for example foster.  In the last case the local host 
	** and local cell will be assumed. 
	*/
	if ((name_ptr = strchr((char *) name, '@')) || 
	    (!(name_ptr = strrchr((char *) name, '/')))) {
		/*
		** service@host or service
		*/
		if (name_ptr) {
			hostname = name_ptr+1;
                	if ((strncmp(hostname, CELL_DIR_ROOT, 
				     CELL_DIR_ROOT_LEN)) &&
                    	    (strncmp(hostname, GLOBAL_DIR_ROOT, 
				     GLOBAL_DIR_ROOT_LEN)) &&
                            (strncmp(prefix_host, hostname, 
                                     prefix_host_len))) {
                                SET_STATUS_RET(st, dced_s_invalid_arg);
                        }
			if (!strstr(hostname, prefix_host)) {
				SET_STATUS_RET(st, dced_s_invalid_arg);
			}
			servicename_len += (name_ptr - (char *) name);
			MALLOC_RESET_RET(servicename, char, servicename_len,st);
			strncpy(servicename, (char *) name, servicename_len-1);
			servicename[servicename_len-1] = '\0';
		} else {
			hostname = NULL;
			servicename = dce_strdup((char *) name);
		}
		dce_cf_dced_entry_from_host(hostname, &name_ptr, st);	
		CHECK_STATUS_RET(st);
                if ((strncmp(name_ptr, CELL_DIR_ROOT, CELL_DIR_ROOT_LEN)) &&
                    (strncmp(name_ptr, GLOBAL_DIR_ROOT, GLOBAL_DIR_ROOT_LEN))) {
			/*
			** Assume local cell being requested
			*/
			entryname_len += CELL_DIR_ROOT_LEN+strlen(name_ptr);
			MALLOC_RESET(entryname, char, entryname_len, st);
			if (!GOOD_STATUS(st)) {
				free(servicename);
				free(name_ptr);
				return;
			}
			strcpy(entryname, CELL_DIR_ROOT);
			strcpy(entryname+CELL_DIR_ROOT_LEN, name_ptr);
                }
		else {
			entryname_len += strlen(name_ptr);
			MALLOC_RESET(entryname, char, entryname_len, st);
			if (!GOOD_STATUS(st)) {
				free(servicename);
				free(name_ptr);
				return;
			}
			strcpy(entryname, name_ptr);
		}
		free(name_ptr);
	}
	else {
		/*
		** /.../cellname/host/config/service or 
		** /.:/host/config/service
		*/
		entryname = servicename = NULL;	
		rpc_ns_entry_inq_resolution(dced_c_entry_syntax, 
					    (unsigned char *)name,
					    (unsigned char **)&entryname, 
					    (unsigned char **)&servicename, 
					    st);
		if (!GOOD_STATUS(st) && 
		    !STATUS_EQUAL(st, rpc_s_partial_results)) {
			return;
		}
		if (!servicename) {
			rpc_string_free((unsigned char **)&entryname, &status);
			SET_STATUS_RET(st, dced_s_invalid_arg);
		}
		rpc_free_names = TRUE;
	}
	
	/*
	** Determine the requested dced service
	*/
	dced__service_from_name((unsigned char *)servicename, 
				&service, 
				st);
	CHECK_STATUS_OUT(st);

	rpc_binding = NULL;
	switch(binding_syntax) {
	case dced_c_binding_syntax_default:
	default:
		/*
		** Import the binding from the entryname
		*/
		rpc_ns_binding_import_begin(dced_c_entry_syntax, 
					    (unsigned_char_t *) entryname,
					    (rpc_if_handle_t) NULL, 
					    (uuid_t *) 0, 
					    &import_context, 
					    st);
		CHECK_STATUS_OUT(st);
	
		/*
		** Get a valid rpc binding
		*/
		rpc_ns_binding_import_next(import_context, &rpc_binding, st);
		
		rpc_ns_binding_import_done(&import_context, &status);
		CHECK_STATUS_OUT(st);
		break;
	case dced_c_binding_syntax_bypass:
		break;
	case dced_c_binding_syntax_local:
		if (hostdata_v1_0_r_epv == NULL
		    || rkeytab_v1_0_r_epv == NULL
		    || srvrconf_v1_0_r_epv == NULL
		    || srvrexec_v1_0_r_epv == NULL)
		    SET_STATUS(st, dced_s_no_support);
		CHECK_STATUS_OUT(st);
		break;
	}
	/*
	** Allocate the dced binding structure
	*/
	MALLOC_RESET_RET(ptr, struct dced_binding_handle_s_t, 1, st);
		
	/*
	** Initialize the dced binding structure
	*/
	DCED_SERVICE(ptr) = service;
	RPC_BINDING(ptr) = rpc_binding;
	DCED_SYNTAX(ptr) = binding_syntax;
	CACHE_STATE(ptr) = dced_e_cache_state_null;
	CACHE_COUNT(ptr) = 0;
	CACHE_HEAD(ptr) = NULL;
	*h = ptr;

out:
	/*
	** Free memory and then return
	*/
	if (rpc_free_names) {
		/*
		** rpc_string_free returns only rpc_s_ok
		*/  
		rpc_string_free((unsigned char **)&entryname, &status);
		rpc_string_free((unsigned char **)&servicename, &status);
	} else {
		free(entryname);
		free(servicename);
	}

	return;
}

/*
**++
**
**  dced_binding_from_rpc_binding converts an rpc binding 
**  to a dced host binding handle. 
**
**--
*/
void
dced_binding_from_rpc_binding (
	/* [in]  */     dced_service_type_t     service,
	/* [in]  */     rpc_binding_handle_t	rpc_handle,
	/* [out] */     dced_binding_handle_t   *h,
	/* [out] */     error_status_t          *st
)
{
	struct dced_binding_handle_s_t	*ptr;
	error_status_t			status;
	rpc_ep_inq_handle_t		inq_context;
	rpc_if_handle_t			*p_ifspec;
	rpc_if_id_t			ifid, ifid_out;
	uuid_t				dced_obj_uuid;

	CLEAR_STATUS(st);

	/*
	 * Get the obj UUID for the DCED service we want from the
	 * host specified in the supplied binding.   Go to the target
	 * host EP map to get the OBJ UUID of the given dced service (by
	 * if_id).  The first EP entry will contain the UUID we need.
	 */
	switch ( service )
	{
	case dced_e_service_type_srvrconf:
	  p_ifspec = &srvrconf_v1_0_c_ifspec;
	  break;
	case dced_e_service_type_srvrexec:
	  p_ifspec = &srvrexec_v1_0_c_ifspec;
	  break;
	case dced_e_service_type_hostdata:
	  p_ifspec = &hostdata_v1_0_c_ifspec;
	  break;
	case dced_e_service_type_keytab:
	  p_ifspec = &rkeytab_v1_0_c_ifspec;
	  break;
	case dced_e_service_type_secval:
	  p_ifspec = &secval_v1_0_c_ifspec;
	  break;
	default:
	  SET_STATUS(st, dced_s_unknown_service);
	  return;
	}

	rpc_if_inq_id ( *p_ifspec, &ifid, st );
	CHECK_STATUS_RET(st);

#ifndef HPDCE_DCED_BINDING_FIXES
	rpc_binding_set_object ( rpc_handle, NULL, st );
	CHECK_STATUS_RET(st);
#endif /* HPDCE_DCED_BINDING_FIXES */

	rpc_mgmt_ep_elt_inq_begin ( rpc_handle, rpc_c_ep_match_by_if,
				    &ifid, rpc_c_vers_compatible, NULL,
				    &inq_context, st );
	CHECK_STATUS_RET(st);

	rpc_mgmt_ep_elt_inq_next ( inq_context, &ifid_out, NULL, 
				   &dced_obj_uuid, NULL, st );
	
	rpc_mgmt_ep_elt_inq_done ( &inq_context, &status );

	CHECK_STATUS_RET(st);

	
	/*
	** Before using the supplied rpc binding, reset it into
	** partial binding so that it can certainly be used to
	** contact dced on the indicated host. Also, replace any
	** obj uuid in the binding with that of the dced.
	*/
	rpc_binding_reset(rpc_handle, st);
	CHECK_STATUS_RET(st);
	  
	rpc_binding_set_object ( rpc_handle, &dced_obj_uuid, st );
	CHECK_STATUS_RET(st);

	/*
	** Allocate the dced binding structure
	*/
	MALLOC_RESET_RET(ptr, struct dced_binding_handle_s_t, 1, st);
		
	/*
	** Initialize the dced binding structure
	*/
	DCED_SERVICE(ptr) = service;
	RPC_BINDING(ptr) = rpc_handle;
	DCED_SYNTAX(ptr) = dced_c_binding_syntax_default;
	CACHE_STATE(ptr) = dced_e_cache_state_null;
	CACHE_COUNT(ptr) = 0;
	CACHE_HEAD(ptr) = NULL;
	*h = ptr;
	return;
}

/*
**++
**
** dced_binding_set_auth_info sets authentication, authorization, and
** protection level information for the dced server binding handle 
**
**--
*/
void
dced_binding_set_auth_info(
	/* [in]  */     dced_binding_handle_t   	h,
	/* [in]  */     unsigned32           		protect_level,
	/* [in]  */     unsigned32           		authn_svc,
	/* [in]  */     rpc_auth_identity_handle_t	authn_identity,
	/* [in]  */     unsigned32			authz_svc,
	/* [out] */     error_status_t          	*st
)
{

	rpc_if_handle_t		dced_if_handle = NULL;
	unsigned char		*server_princ;
	error_status_t		status;

	CLEAR_STATUS_2(st, &status);

	/*
	** If this is a bypass operation, it's unauthenticated
	*/
	if (SYNTAX_EQUAL(h, dced_c_binding_syntax_bypass)
	    || SYNTAX_EQUAL(h, dced_c_binding_syntax_local)) {
		return;
	}

	switch (DCED_SERVICE(h)) {

	case dced_e_service_type_null:
	case dced_e_service_type_ep:
		SET_STATUS(st, dced_s_no_support);
		break;
	case dced_e_service_type_srvrconf:
		dced_if_handle = srvrconf_v1_0_c_ifspec;
		break;
	case dced_e_service_type_srvrexec:
		dced_if_handle = srvrexec_v1_0_c_ifspec;
		break;
	case dced_e_service_type_hostdata:
		dced_if_handle = hostdata_v1_0_c_ifspec;
		break;
	case dced_e_service_type_keytab:
		dced_if_handle = rkeytab_v1_0_c_ifspec;
		break;
	case dced_e_service_type_secval:
		dced_if_handle = secval_v1_0_c_ifspec;
		break;
	default:
		SET_STATUS(st, dced_s_bad_binding);
		break;
	}

	/*
	** Out of here if the service is invalid
	*/
	CHECK_STATUS_RET(st);

	/*
	** We need full rpc binding handle to make rpc mgmt call.
	** But if the binding is already a full binding the
	** rpc_ep_resolve_binding will not contact with rpcd/dced.
	*/
	rpc_ep_resolve_binding(RPC_BINDING(h), dced_if_handle, st);
	CHECK_STATUS_RET(st);

	/*
	** Get the host server principal name which is needed
	** to have authenticated call to dced secval.
	*/
	rpc_mgmt_inq_server_princ_name(RPC_BINDING(h),
				       authn_svc,
				       &server_princ,
				       st);
	CHECK_STATUS_RET(st);

	/*
	** Setup the authentication information on the
	** binding handle.
	*/
	rpc_binding_set_auth_info(RPC_BINDING(h), 
				  server_princ,
				  protect_level,
				  authn_svc,
				  authn_identity, 
				  authz_svc,
				  st);
	/*
	** Free the server_princ_name
	*/
	rpc_string_free(&server_princ, &status);
	if (GOOD_STATUS(st) && !GOOD_STATUS(&status)) {
		SET_STATUS(st, status);
	}
	
	return;
}


/*
**++
**
**  dced_binding_free releases a dced host binding 
**
**--
*/
void
dced_binding_free(
                /* [in]  */     dced_binding_handle_t   h,
                /* [out] */     error_status_t          *st
)
{

	CLEAR_STATUS(st);

	/*
	** Free the rpc_binding_handle.
	*/
	if ((RPC_BINDING(h)) && 
	    !(SYNTAX_EQUAL(h, dced_c_binding_syntax_bypass)
	      || SYNTAX_EQUAL(h, dced_c_binding_syntax_local))) {
		rpc_binding_free(&RPC_BINDING(h), st);
		CHECK_STATUS_RET(st);
	}

	/*
	** Unload caches
	*/
	dced__unload_cache(h, st);
	CHECK_STATUS_RET(st);

	/*
	** Zero out the service and syntax
	*/
	DCED_SERVICE(h) = dced_e_service_type_null;
	DCED_SYNTAX(h) = dced_c_binding_syntax_default;
	free(h);

	return;
}

hostdata_v1_0_epv_t		*hostdata_v1_0_r_epv = NULL;
rkeytab_v1_0_epv_t		*rkeytab_v1_0_r_epv = NULL;
srvrconf_v1_0_epv_t		*srvrconf_v1_0_r_epv = NULL;
srvrexec_v1_0_epv_t		*srvrexec_v1_0_r_epv = NULL;

/*
**++
**
**  dced_register_local_mgrs registers local, i.e., within the same
**  address space, manager epvs for dced_c_binding_syntax_local
**
**--
*/
void
dced_register_local_mgrs(
	/* [in]  */	rpc_mgr_epv_t	hdepv,
	/* [in]  */	rpc_mgr_epv_t	kmepv,
	/* [in]  */	rpc_mgr_epv_t	scepv,
	/* [in]  */	rpc_mgr_epv_t	sxepv,
	/* [out] */	error_status_t*	st
)
{

	CLEAR_STATUS(st);

	hostdata_v1_0_r_epv = (hostdata_v1_0_epv_t *)hdepv;
	rkeytab_v1_0_r_epv = (rkeytab_v1_0_epv_t *)kmepv;
	srvrconf_v1_0_r_epv = (srvrconf_v1_0_epv_t *)scepv;
	srvrexec_v1_0_r_epv = (srvrexec_v1_0_epv_t *)sxepv;

	return;
}
