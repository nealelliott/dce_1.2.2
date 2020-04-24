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
 * $Log: dce_attr_bind.c,v $
 * Revision 1.1.4.2  1996/02/18  19:20:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:40  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:35:54  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  20:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/23  00:12 UTC  tatsu_s
 * 	Added dce_attr_sch_bind_local().
 * 	[1995/12/08  18:07:43  root]
 * 
 * Revision 1.1.2.5  1994/09/27  17:15:00  pwang
 * 	Allow the "xattrschem" be part of entryname.
 * 	[1994/09/27  17:13:02  pwang]
 * 
 * Revision 1.1.2.4  1994/09/21  21:49:37  pwang
 * 	Made sure attr_schema_ob name started with 'xattrschema'
 * 	[OT#12146]
 * 
 * Revision 1.1.2.3  1994/07/27  20:59:18  pwang
 * 	Returned the status of rpc_binding_set_auth_info if it fails.
 * 	[1994/07/27  20:58:07  pwang]
 * 
 * Revision 1.1.2.2  1994/07/23  17:34:04  pwang
 * 	Fixed a couple of warnings
 * 	[1994/07/23  17:33:08  pwang]
 * 
 * Revision 1.1.2.1  1994/07/22  21:39:43  pwang
 * 	Initial implementation
 * 	[1994/07/22  20:34:46  pwang]
 * 
 * $EndLog$
 */

/*
** Module: dce_attr_bind.c
**
** FACILITY: DCE generic attribute interfaces (Client) 
**
** ABSTRACT: This module implements the dce attr binding functions
**
** Date:
*/

#include <attrimpl.h>

/*
**++
** dce_attr__ckeck_auth: validate the input auth info
**--
*/
PRIVATE
void dce_attr__ckeck_auth (
    /* [in] */   dce_attr_bind_auth_info_t       *auth_info,
    /* [out]*/   sec_login_handle_t              *login_context,
    /* [out]*/   error_status_t                  *st
)
{
        CLEAR_STATUS(st);

	*login_context = NULL;

        switch (auth_info->info_type) {
        case sec_attr_bind_auth_none:
                break;
        case sec_attr_bind_auth_dce:
                sec_login_get_current_context(login_context, st);
                break;
        default:
                SET_STATUS(st, dce_attr_s_unknown_auth_info_type);
        }

        return;
}

/*
**++
** dce_attr__setup_auth: setup req auth info on the binding.
**--
*/
PRIVATE
void dce_attr__setup_auth (
    /* [in] */	 rpc_binding_handle_t		 rpc_binding,
    /* [in] */   dce_attr_bind_auth_info_t       *auth_info,
    /* [in] */   rpc_auth_identity_handle_t      identity,
    /* [out]*/   error_status_t                  *st
)
{
	CLEAR_STATUS(st);
	
	/* 
	** Set up the binding handle auth info 
	*/
	switch (auth_info->info_type) {
	case sec_attr_bind_auth_none:
		break;
	case sec_attr_bind_auth_dce:
		rpc_binding_set_auth_info(
				rpc_binding,
				auth_info->tagged_union.dce_info.svr_princ_name,
				auth_info->tagged_union.dce_info.protect_level,
				auth_info->tagged_union.dce_info.authn_svc,
				(rpc_auth_identity_handle_t) identity, 
				auth_info->tagged_union.dce_info.authz_svc,
				st);
		break;
	default:
		SET_STATUS(st, dce_attr_s_unknown_auth_info_type);
	}
	
	return;
}

/*
**++
** dce_attr__bind: a real routine to inq a binding for the
**		   targeted nsi entry and set req auth info 
**		   on the binding.
**--
*/
PRIVATE
void dce_attr__bind (
    /* [in] */   dce_attr_component_name_t       entity_name,
    /* [in] */   dce_attr_bind_auth_info_t       *auth_info,
    /* [in] */   rpc_auth_identity_handle_t      identity,
    /* [out]*/   dce_attr_binding_handle_rep_p_t *binding_handle,
    /* [out]*/   error_status_t                  *st
)
{
	dce_attr_binding_handle_rep_t	*binding_rep_p;
	uuid_t				obj_uuid;
 	rpc_ns_handle_t			import_context;
	rpc_binding_handle_t		rpc_binding;
	unsigned_char_t			*entryname, *objectname;
	char				*ptr, *ptr1;
	error_status_t			status, auth_status;


	CLEAR_STATUS(st);

	/*
	** Initialize the return, in case we're failed in the middle
	*/
	*binding_handle = NULL;

	/*
	** Resolve the entity name into a nsi entry name and a dce_attr
	** component/object name
	*/
	entryname = objectname = NULL;
	rpc_ns_entry_inq_resolution(rpc_c_ns_syntax_dce, entity_name,
				    &entryname, &objectname, st);
	if (!GOOD_STATUS(st) && !STATUS_EQUAL(st, rpc_s_partial_results)) {
		return;
        }
	
	/*
	** Validate the given scheme name that either entryname contains or
        ** objectname starts with dce_attr_c_schema_name.
        */
        ptr = (char *) objectname;
	if (ptr[0] == '/') {
		ptr++;
        }
	if ((((ptr1 = strstr((char *) entryname, dce_attr_c_schema_name)) &&
	      ((*(ptr1-1) != '/') ||  
               ((ptr1[dce_attr_c_schema_name_len] != '\0') && 
                (ptr1[dce_attr_c_schema_name_len] != '/')))) ||
	     (!ptr1)) &&

            (NULL_STRING(ptr) || 
             (strncmp(ptr, dce_attr_c_schema_name, 
	              dce_attr_c_schema_name_len) || 
              ((ptr[dce_attr_c_schema_name_len] != '\0') && 
               (ptr[dce_attr_c_schema_name_len] != '/'))))) {

		SET_STATUS(st, dce_attr_s_bad_schema_name);
		return;
	}

	/* 
	** Get a binding from the entry
	*/
	uuid_create_nil(&obj_uuid, &status);
	rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce, entryname,
				    (rpc_if_handle_t) NULL, &obj_uuid,
                                    &import_context, st);
	CHECK_STATUS_OUT(st);
    
	CLEAR_STATUS(&auth_status);
	for ( ; ; ) {

		rpc_ns_binding_import_next(import_context, &rpc_binding, st);
		CHECK_STATUS_BRK(st);

		dce_attr__setup_auth(rpc_binding, auth_info, identity, st);
		if (!GOOD_STATUS(st)) {
			rpc_binding_free(&rpc_binding, &status);
			COPY_STATUS(st, &auth_status);
		}

		if (GOOD_STATUS(st) || 
		    STATUS_EQUAL(st, rpc_s_auth_tkt_expired) ||
		    STATUS_EQUAL(st, dce_attr_s_unknown_auth_info_type)) {
			break;
		}
	}

	rpc_ns_binding_import_done(&import_context, &status);

	/*
	** If auth_status is not clean, the failure is due to
	** setup req. auth info on the binding. 
	*/  
	if (!GOOD_STATUS(st) && !GOOD_STATUS(&auth_status)) {
		SET_STATUS(st, auth_status);
	}
	CHECK_STATUS_OUT(st);

	/*
	** If we're success up to now, allocate and fill out 
	** the binding rep, otherwise cleanup  
	*/
	MALLOC_AND_RESET(binding_rep_p, dce_attr_binding_handle_rep_t, st);
	if (GOOD_STATUS(st)) {
		RPC_BINDING(binding_rep_p) = rpc_binding;
		if (objectname) {
			COMPONENT_NAME(binding_rep_p) = 
			  	(dce_attr_component_name_t)dce_strdup(ptr);
		} 
		*binding_handle = binding_rep_p;
	}
	else {	
		rpc_binding_free(&rpc_binding, &status);
	}
out:
	rpc_string_free(&entryname, &status);
	if (objectname) {
		rpc_string_free(&objectname, &status);
	}

	return;
}

/* 
**++
** d c e _ a t t r _ s c h _ b i n d
**
** Return an opaque dce_attr_sch_handle_t to the schema 
** object named 'schema_name.'
**
**  In Parameters:
**          schema_name - global name identifying the schema 
**                  object on which subsequent dce_attr_sch
**                  operations (using the returned 
**                  dce_attr_sch_handle_t) will be performed.
**          auth_info - authentication and authorization info
**                  to use with binding handle.  If input pointer
**                  is NULL, use default auth_info settings.
**  Out Parameters:
**          h - an opaque handle to the named schema object
**                  suitable for use with operations in the
**                  dce_attr_sch interface.
**
** Errors:
**  dce_attr_s_bad_name
**  sec_login_s_no_current_context
**  rpc_s_entry_not_found
**  rpc_s_no_more_bindings
**  dce_attr_s_unknown_auth_info_type
**  dce_attr_s_no_memory
**  ...
**--
*/
PUBLIC
void dce_attr_sch_bind (
    /* [in] */   dce_attr_component_name_t       schema_name,
    /* [in] */   dce_attr_bind_auth_info_t       *auth_info,
    /* [out]*/   dce_attr_sch_handle_t           *h,
    /* [out]*/   error_status_t                  *st
)
{

	dce_attr_binding_handle_rep_t	*new_binding_handle;
	sec_login_handle_t		login_context;

	CLEAR_STATUS(st);

	/*
	** Inititalize the return, in case we're failed in the middle
	*/
	*h = NULL;

	if (NULL_STRING(schema_name)) {
		SET_STATUS(st, dce_attr_s_bad_name);
		return;
	}

        /*
        ** Validate the input auth info
        */
        dce_attr__ckeck_auth(auth_info, &login_context, st);
        CHECK_STATUS_RET(st);

	/*
	** Inq the binding and setup the req auth info 
	*/
	dce_attr__bind(schema_name, auth_info, 
		       (rpc_auth_identity_handle_t) login_context, 
		       &new_binding_handle, st);
	CHECK_STATUS_RET(st);

	/*
	** It's for dce_attr_schema interface
	*/ 
	IF_TYPE(new_binding_handle) = dce_attr_e_if_attr_sch;
	HANDLE_VALID(new_binding_handle) = TRUE;
	*h = (dce_attr_sch_handle_t) new_binding_handle;
	
	return;
}

/*
**++
** d c e _ a t t r _ s c h _ b i n d _ f r e e
**
** Release an opaque dce_attr_sch_handle_t to the schema
** object named 'schema_name.'
**
**  In Parameters:
**          h - an opaque handle to the named schema object
**                  suitable for use with operations in the
**                  dce_attr_sch interface.
**  Out Parameters:
**
** Errors:
**--
*/
PUBLIC
void dce_attr_sch_bind_free (
       /* [in] */   dce_attr_sch_handle_t           *h,
       /* [out]*/   error_status_t                  *st
)
{
	dce_attr_binding_handle_rep_t	*binding_handle;

	CLEAR_STATUS(st);

	binding_handle = (dce_attr_binding_handle_rep_p_t) *h;

	/*
	** Turn off the binding first
	*/
	HANDLE_VALID(binding_handle) = FALSE;

	if (COMPONENT_NAME(binding_handle)) {
		free(COMPONENT_NAME(binding_handle));
	}
	IF_TYPE(binding_handle) = dce_attr_e_if_null;

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	if (RPC_BINDING(binding_handle) != NULL) {
	    rpc_binding_free(&RPC_BINDING(binding_handle), st);
	    CHECK_STATUS_RET(st);
	}
#else
	rpc_binding_free(&RPC_BINDING(binding_handle), st);
	CHECK_STATUS_RET(st);
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */

	free(binding_handle);
	*h = NULL;

	return;
}

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
/* 
**++
** d c e _ a t t r _ s c h _ b i n d _ l o c a l
**
**  Debug interface bind, which returns a handle to a schema object
**  within the same address space whose entry pointer vector is lepv.
**
**	In Parameters:
**		if_version - Interface version.
**		lepv - entry pointer vector (really rpc_mgr_epv_t).
**		schema_name - local name identifying the schema 
**			object on which subsequent dce_attr_sch
**			operations (using the returned 
**			dce_attr_sch_handle_t) will be performed.
**	Out Parameters:
**		h - an opaque handle to the named schema object
**			suitable for use with operations in the
**			dce_attr_sch interface.
**
** Errors:
**	dce_attr_s_bad_name
**	dce_attr_s_no_memory
**  ...
**--
*/
PUBLIC
void dce_attr_sch_bind_local(
    /* [in] */	unsigned32			if_version,
    /* [in] */	idl_void_p_t			lepv,
    /* [in] */	dce_attr_component_name_t	schema_name,
    /* [out] */	dce_attr_sch_handle_t		*h,
    /* [out] */	error_status_t			*st
)
{
	dce_attr_binding_handle_rep_t	*binding_rep_p;

	CLEAR_STATUS(st);

	/*
	** Inititalize the return, in case we're failed in the middle
	*/
	*h = NULL;

	if (NULL_STRING(schema_name)) {
		SET_STATUS(st, dce_attr_s_bad_name);
		return;
	}

	if (lepv == NULL || if_version != 0) {
		SET_STATUS(st, dce_attr_s_bad_name);/* Need a better status! */
		return;
	}

	if ((strncmp((char *)schema_name, dce_attr_c_schema_name,
		     dce_attr_c_schema_name_len) != 0) ||
	    ((((char *)schema_name)[dce_attr_c_schema_name_len] != '\0') && 
	     (((char *)schema_name)[dce_attr_c_schema_name_len] != '/'))) {
		SET_STATUS(st, dce_attr_s_bad_schema_name);
		return;
	}

	/*
	** If we're success up to now, allocate and fill out 
	** the binding rep, otherwise cleanup  
	*/
	MALLOC_AND_RESET(binding_rep_p, dce_attr_binding_handle_rep_t, st);
	if (GOOD_STATUS(st)) {
		IF_VERSION(binding_rep_p) = if_version;
		binding_rep_p->lepv.epv = lepv;
		COMPONENT_NAME(binding_rep_p) =
			(dce_attr_component_name_t)dce_strdup((char *)schema_name);
		/*
		** It's for dce_attr_schema interface
		*/ 
		IF_TYPE(binding_rep_p) = dce_attr_e_if_attr_sch;
		HANDLE_VALID(binding_rep_p) = TRUE;
		*h = binding_rep_p;
	}
	return;
}
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
