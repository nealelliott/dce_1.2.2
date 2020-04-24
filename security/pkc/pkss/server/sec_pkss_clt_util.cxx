//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_clt_util.cxx,v $
// Revision 1.1.2.1  1996/10/03  20:30:57  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/03  19:29:05  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:29 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:52 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// Module:  sec_pkss_clt_util.cxx
//
// Purpose:  This module contains utility functions of particular interest
//	     to PKSS login and management clients.
//


#include <dce/rpc.h>
#include <dce/pgo.h>
#include <dce/binding.h>
#include <dce/sec_login.h>
#include <dce/sec_pk_base.h>
#include "sec_pkss_clt_if.h"
#include "sec_pkss_lits.h"
#include "pkss_config_common.h"
#include <dce/dce_msg.h>
#include <dce/dcepksmsg.h>
#include <dce/dcepkssvc.h>  
#include <dce/dcepksmac.h> 


//
// Bind login client to PKSS server over the specified interface using the
// first handle that works, and return that binding handle.
//

error_status_t sec_pkss_bind
(
  const char *pkss_svc_profile_name_p,
  rpc_if_handle_t ifspec,
  rpc_binding_handle_t *handle_p
)

{

  //
  // Initialize binding import for the interface given by ifspec.
  //

  error_status_t estatus = 0;
  rpc_ns_handle_t import_context;
  rpc_ns_binding_import_begin (rpc_c_ns_syntax_default,
			       (unsigned char *)pkss_svc_profile_name_p,
			       ifspec,
			       NULL,
			       &import_context,
			       &estatus);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from rpc_ns_binding_import_begin : 0x%x", estatus));

  if (estatus != rpc_s_ok)
  {
    // error
    return (estatus);
  }

  //
  // Find and return the first handle that gets to the PKSS over the interface
  // given by ifspec (above).
  //

  while (1)
  {
    //
    // Obtain next binding handle.  This loop will terminate as a result of
    // a successful "ping" or on the break executed as a result of any
    // rpc_ns_binding_import_next failure.  In the latter case, we would
    // normally expect that the failure will be rpc_s_no_more_bindings.
    //

    rpc_ns_binding_import_next (import_context, handle_p, &estatus);

      DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from rpc_ns_binding_import_next : 0x%x", estatus));

    if (estatus != rpc_s_ok)
    {
      // error
      break;
    }

    //
    // Ping the binding handle returned above.  If there's a server up and
    // running that responds, return the handle.
    //

    if (rpc_mgmt_is_server_listening (*handle_p, &estatus))
      break;

    else
    {
      //
      // If the "ping" failed because the binding was incomplete, obtain a
      // full binding from the partial binding and attempt to ping again.
      //

      if (estatus == rpc_s_binding_incomplete)
      {
	rpc_ep_resolve_binding (*handle_p, ifspec, &estatus);

          DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from rpc_ep_resolve_binding : 0x%x", estatus));

	if (estatus != rpc_s_ok)
	{
          // error
	}

	else
	{
          if (rpc_mgmt_is_server_listening (*handle_p, &estatus))
	    break;

	  else
          {
	    // error

          } // End else of inner if (rpc_mgmt_is_server_listening (...) == 0)

          DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from rpc_mgmt_is_server_listening : 0x%x", estatus));

	} // End else of if (estatus != rpc_s_ok) after rpc_ep_resolve_binding

      } // End if (estatus == rpc_s_binding_incomplete)

    } // End else path of outer if (rpc_mgmt_is_server_listening (...))

    //
    // Free the binding returned from the current call to
    // rpc_ns_binding_import_next if it was not possible to ping a PKSS server
    // over it.
    //

    rpc_binding_free (handle_p, &estatus);

    if (estatus != rpc_s_ok)
    {
      // error
      break;
    }

  } // End while (1)

  //
  // End binding import loop.
  //

  rpc_ns_binding_import_done (&import_context, &estatus);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from rpc_ns_binding_import_done : 0x%x", estatus));

  if (estatus != rpc_s_ok)
  {
    // error
  }

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_bind


//
// Count the number of usage flags bits set, and return an error
// if that number is greater than one.
//

error_status_t sec_pkss_count_usages
(
  sec_pk_usage_flags_t usages
)

{
  error_status_t estatus = 0;

  if (usages == 0)
    return (1);

  while (usages & 1)
  {
    usages >>= 1;
  }

  return (usages & ~1);

} // End sec_pkss_count_usages


//
// Set security information into a binding handle.  Used only for
// PKSS management API operations.
//

error_status_t sec_pkss_secure_clt_handle
(
  rpc_binding_handle_t binding_handle
)

{
  //
  // Obtain handle to the registry - required by sec_rgy_site_bind
  //

  error_status_t estatus = 0;
  error_status_t rstatus = 0;
  sec_rgy_bind_auth_info_t  binding_auth_info;
  binding_auth_info.info_type = sec_rgy_bind_auth_dce;
  binding_auth_info.tagged_union.dce_info.authn_level
            = rpc_c_authn_level_pkt_integrity;
  binding_auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_private;
  binding_auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
  binding_auth_info.tagged_union.dce_info.identity = (void *)NULL;
  sec_rgy_handle_t rgy_handle;

  sec_rgy_site_bind ((unsigned char *)"/.:",
		     &binding_auth_info,
		     &rgy_handle,
		     &estatus);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_rgy_site_bind : 0x%x", estatus));

  if (estatus != 0)
  {
    return (estatus);
  }

  //
  // Obtain server's group name - required by sec_rgy_site_bind
  //
  
  char *server_group_name_p = 0;
  server_group_name_p = pkss_config__security__server_group_name();

  if (server_group_name_p == 0)
  {
    estatus = pks_s_no_group_name;
    return (estatus);
  }

  // 
  // Obtain server's principal name - required by sec_rgy_site_bind
  //

  unsigned char *server_princ_name_p = 0;
  rpc_mgmt_inq_server_princ_name (binding_handle,
				  rpc_c_authn_default,
				  &server_princ_name_p,
				  &estatus);
  if (estatus != 0)
  {
    return (estatus);
  }

  //
  // Verify the why's and wherefore's of the server in question, and
  // set authentication information into the binding handle if successful.
  //

  boolean32 svr_is_member = 0;
  svr_is_member = sec_rgy_pgo_is_member (rgy_handle,
					 sec_rgy_domain_group,
					 (unsigned char *)server_group_name_p,
				 	 server_princ_name_p,
					 &estatus); 

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_rgy_pgo_is_member : 0x%x", estatus));

  sec_rgy_site_close (rgy_handle, &rstatus);

  if (!svr_is_member)
  {
    rpc_string_free (&server_princ_name_p, &rstatus);
    return (estatus);
  }

  else
  {
    rpc_binding_set_auth_info (binding_handle,
			       server_princ_name_p,
			       rpc_c_protect_level_pkt_integ,
			       rpc_c_authn_default,
			       NULL,
			       rpc_c_authz_dce,
			       &estatus);

      DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from rpc_binding_set_auth_info : 0x%x", estatus));

    if (estatus != 0)
    {
      rpc_string_free (&server_princ_name_p, &rstatus);
      return (estatus);
    }

  } // End else path of if (!svr_is_member)

  //
  // Free memory allocated in this function.
  //

  rpc_string_free (&server_princ_name_p, &rstatus);

  if (rstatus != 0)
  {
    // error
  }

  //
  // Exit this function
  //

  return (estatus);


} // End sec_pkss_secure_clt_handle

