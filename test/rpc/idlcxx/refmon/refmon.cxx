// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: refmon.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:49:42  marty
// 	Add OSF copyright
// 	[1996/03/09  20:28:02  marty]
//
// Revision 1.1.2.1  1995/12/11  19:35:32  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:40  root]
// 
// $EndLog$
#include "test_com.h"
#include <iostream.h>
#include <dce/rpc.h>
#include <dce/id_base.h>
#include <dce/dce_error.h>
#include <dce/exc_handling.h>
#include "refmon.h"

int reference_monitor(
	rpc_binding_handle_t handle,
	long                 *status
) {

	rpc_authz_handle_t privileges;      // client's name
	unsigned32 protection_level;        // client's desired protection level
	unsigned32 authentication_service;  // client's authentication policy
	unsigned32 authorization_service;   // client's authorization policy
	unsigned32 st;                      // generic status code
	unsigned_char_t *name;
	
	dce_error_string_t     dce_error_string;  // generic error string
	error_status_t         _ignore;           // error status to ignore


	rpc_binding_inq_auth_client(
		handle,                  // binding handle
		&privileges,             // info from the authorization service 
		&name,                   // server principal name used by client
		&protection_level,       // client's protection level
		&authentication_service, // client's authentication protocol
		&authorization_service,  // client's authorization protocol
		&st                      // error status
	);

	if (st != rpc_s_ok) {
		if (st == rpc_s_binding_has_no_auth) {
			cout << "Unauthenticated access denied\n";
			*status = NO_AUTH;
			return(DENY_ACCESS);
		} else {
			exc_raise_status(st);
		}
	}

   	/* test authentication_service level */
	if (authentication_service != rpc_c_authn_dce_secret) {
		cout << "Unauthenticated access denied\n";
		*status = NO_AUTH;
		return(DENY_ACCESS);
	} 

	/* test protection_level */
	if (protection_level < rpc_c_protect_level_pkt_integ){
		cout << "refmon server requires application data privacy:";
		cout << "access denied\n";
		*status = LOW_INTEGRITY;
		return(DENY_ACCESS);
	}
    
	*status = AUTH_OK;
	return(GRANT_ACCESS);
}
