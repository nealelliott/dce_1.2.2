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
 * $Log: dced_secval.c,v $
 * Revision 1.1.4.2  1996/02/18  19:15:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:45  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:26:26  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/04  17:25 UTC  mdf
 * 	Merge for CHFts13120
 * 	[1995/12/08  17:59:01  root]
 * 
 * Revision 1.1.2.6  1994/08/24  16:32:01  baum
 * 	Added secval_status API
 * 	[1994/08/24  16:31:38  baum]
 * 
 * Revision 1.1.2.5  1994/07/26  22:07:12  pwang
 * 	Some adjustment.
 * 	[1994/07/26  22:06:17  pwang]
 * 
 * Revision 1.1.2.4  1994/06/09  16:02:51  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:27  devsrc]
 * 
 * Revision 1.1.2.3  1994/04/15  22:29:03  pwang
 * 	General cleanup and fixed a number of problems.
 * 	[1994/04/15  21:31:38  pwang]
 * 
 * Revision 1.1.2.2  1994/03/09  20:57:07  pwang
 * 	Modified to match with attribute changes
 * 	[1994/03/09  20:54:03  pwang]
 * 
 * Revision 1.1.2.1  1994/02/28  16:27:03  pwang
 * 	Inital Implementation.
 * 	[1994/02/28  16:09:40  pwang]
 * 
 * $EndLog$
 */

/*
** Module: dced_secval.c
**
** FACILITY: DCE Host Daemon API
**
** ABSTRACT: This module implements the dced secval functions
**
** SCOPE: Public 
**
** Date:
*/

#include <dcedimpl.h>

/*
**++
**
** dced_secval_start starts secval service on a
** specified host.
**
**--
*/
void
dced_secval_start(
        /* [in]  */     dced_binding_handle_t	h,
        /* [out] */     error_status_t		*st
)
{

	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for secval,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_secval, st);

	(*secval_v1_0_c_epv.secval_enable)(RPC_BINDING(h), st);
	CHECK_STATUS_RET(st);

	return;
}


/*
**++
**
** dced_secval_stop stops secval service in a
** specified host.
**
**--
*/
void
dced_secval_stop(
        /* [in]  */     dced_binding_handle_t	h,
        /* [out] */     error_status_t		*st
)
{

	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for secval,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_secval, st);

	(*secval_v1_0_c_epv.secval_disable)(RPC_BINDING(h), st);	
	CHECK_STATUS_RET(st);

	return;
}

/*
**++
**
** dced_secval_status returns whether dced-secval has been activated.
** Does not require authentication.
**
**--
*/
void
dced_secval_status(
        /* [in]  */     dced_binding_handle_t	h,
        /* [out] */	boolean32		*secval_is_active,
        /* [out] */     error_status_t		*st
)
{

	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for secval,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_secval, st);

	(*secval_v1_0_c_epv.secval_status)(RPC_BINDING(h), secval_is_active,
					   st);
	CHECK_STATUS_RET(st);

	return;
}

/*
**++
**
** dced_secval_update_pesite update's the pe_site file.
**
**--
*/
void
dced_secval_update_pesite(
        /* [in]  */     dced_binding_handle_t	h,
	/* [in]  */	signed32		delay,
        /* [out] */     error_status_t		*st
)
{

	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for secval,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_secval, st);

	(*secval_v1_0_c_epv.secval_update_pesite)(RPC_BINDING(h), delay, st);
	CHECK_STATUS_RET(st);
	return;
}

/*
**++
**
** dced_secval_validate validates the principal's credential
** via a DCE security server at the specified host
**
**--
*/
void 
dced_secval_validate(
        /* [in]  */     dced_binding_handle_t	h,
        /* [out] */     error_status_t		*st
)
{
	sec_login_handle_t	caller_context;
	unsigned char		*server_princ_name;
	rpc_binding_handle_t	work_handle;
	error_status_t		status;
	
	CLEAR_STATUS_2(st, &status);

	/*
	** If the dced binding handle is not for secval,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_secval, st);

	/*
	** Get caller's network identity, if it's not setup yet
	** quit now with not identity status.
	*/
	sec_login_get_current_context(&caller_context, st);
	CHECK_STATUS_RET(st);

	/*
	** We need full rpc binding handle to make rpc mgmt call.
	** But if the binding is already a full binding the
	** rpc_ep_resolve_binding will not contact with rpcd/dced.
	*/
	rpc_ep_resolve_binding(RPC_BINDING(h), secval_v1_0_c_ifspec, st);
	CHECK_STATUS_RET(st);

	/*
	** Get the host server principal name which is needed
	** to have authenticated call to dced secval.
	*/
	rpc_mgmt_inq_server_princ_name(RPC_BINDING(h),
				       rpc_c_authn_dce_private,
				       &server_princ_name,
				       st);
	CHECK_STATUS_RET(st);

	/*
	** Get a copy of the binding and we don't want to interfere
	** the user's original binding since the user may have called
	** dced_binding_set_auth_info to set his/her authn/authz
	** information on the original binding.
	*/
	rpc_binding_copy(RPC_BINDING(h), &work_handle, st);
	if (!GOOD_STATUS(st)) {
		rpc_string_free(&server_princ_name, &status);
		return;
	}

	/*
	** Setup the authentication information on the
	** binding handle.
	*/
	rpc_binding_set_auth_info(work_handle, 
				  server_princ_name,
				  rpc_c_authn_level_pkt_integrity,
				  rpc_c_authn_dce_private,
				  (rpc_auth_identity_handle_t) caller_context, 
				  rpc_c_authz_dce,
				  st);
	CHECK_STATUS_OUT(st);

	/*
	** Call dced secval to validate the ceritification authority
	*/	
	(*secval_v1_0_c_epv.secval_validate_cert_auth)(work_handle, st);
out:
	/*
	** Free the server_princ_name and work_handle.
	*/ 
	rpc_string_free(&server_princ_name, &status);
	if (GOOD_STATUS(st) && !GOOD_STATUS(&status)) {
		SET_STATUS(st, status);
	}
	rpc_binding_free(&work_handle, &status);
	if (GOOD_STATUS(st) && !GOOD_STATUS(&status)) {
		SET_STATUS(st, status);
	}

	return;
}
