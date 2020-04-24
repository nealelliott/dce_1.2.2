//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_main.cxx,v $
// Revision 1.1.2.1  1996/10/03  20:31:34  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/03  19:30:42  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:30 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:54 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// Module:  sec_pkss_main.cxx
//
// Purpose:  This module contains the main calling function for the
//           PKSS server.
//

#define PKSSD_MAIN


extern "C" {
#include <sys/types.h>
#include <signal.h>
#include <limits.h>
extern mode_t umask (mode_t);
extern int chdir (const char *);
extern pid_t setsid (void);
extern pid_t fork (void);
#include <sys/stat.h>
#include <locale.h>
#include <pthread.h>
#include <dce/rpc.h>
#include <dce/sec_login.h>
#include <dce/dce_msg.h>
}
#include "sec_pkss_clt_if.h"
#include "sec_pkss_mgmt_if.h"
#include "sec_pkss_db.h"
#include "sec_pkss_util.h"
#include "pkss_acl_mgr.h"
extern "C" {
#include <dce/dcesvcmsg.h>
#include <dce/dcepksmsg.h>
#include <dce/dcepkssvc.h>
#include <dce/dcepksmac.h>
}
#include "pkss_config_common.h"
#include "pkss_config_server.h"

//
// This function obtains pertinent PKSS server configuration values.
//

error_status_t sec_pkss_get_svr_configs
(
  char **pkss_svc_grp_name_p,
  char **pkss_svc_pfl_name_p,
  char **pkss_svr_bindings_name_p,
  char **pkss_princ_name_p,
  char **pkss_db_name_p
)

{
  error_status_t estatus = 0;
  
  //
  // Initialize PKSS configuration facility.
  //

  estatus = pkss_config__server_init (PKSS_SERVER_NAME);

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "PKSS server config init",
		   (long) estatus);

    return (estatus);
  }

  //
  // Obtain PKSS service group name - used by PKSS namespace setup.
  //

  *pkss_svc_grp_name_p = (char *)pkss_config__cds__service_group_name();

  if (*pkss_svc_grp_name_p == 0)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "pkss_svr_grp_name_p == 0",
		   (long) estatus);

    return (estatus);
  }

  //
  // Obtain PKSS service profile name - used by PKSS namespace setup.
  //

  *pkss_svc_pfl_name_p = (char *)pkss_config__cds__service_profile_name();

  if (*pkss_svc_pfl_name_p == 0)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "pkss_svr_pfl_name_p == 0",
		   (long) estatus);

    return (estatus);
  }

  //
  // Obtain entry name to which binding handles will be exported - used
  // by PKSS namespace setup and bindings export.
  //

  *pkss_svr_bindings_name_p = (char *)pkss_config__cds__server_bindings_name();

  if (*pkss_svr_bindings_name_p == 0)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "pkss_svr_bindings_name_p == 0",
		   (long) estatus);

    return (estatus);
  }

  //
  // Obtain PKSS principal name - used by login.
  //

  *pkss_princ_name_p = (char *)pkss_config__security__server_princ_name();

  if (*pkss_princ_name_p == 0)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "pkss_princ_name_p == 0",
		   (long) estatus);

    return (estatus);
  }

  //
  // Obtain pathname to PKSS database files.
  //

  *pkss_db_name_p = (char *)pkss_config__local__database_name();

  if (*pkss_db_name_p == 0)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "pkss_db_name_p == 0",
		   (long) estatus);

    return (estatus);
  }

  //
  // Exit this function
  //

  return (estatus);

} // End sec_pkss_get_svr_configs


//
// This function performs basic namespace setup for the PKSS.
//

error_status_t sec_pkss_ns_setup
(
  const char *pkss_svc_grp_name_p,
  const char *pkss_svr_bindings_name_p,
  const char *pkss_svc_pfl_name_p
)

{
  error_status_t estatus = 0;
  rpc_if_id_t clt_if_id;
  rpc_if_id_t mgmt_if_id;

  //
  // Create a group containing pointers to all pkssd server bindings
  // in the cell.
  //

  rpc_ns_group_mbr_add (rpc_c_ns_syntax_default,
		        (unsigned char *)pkss_svc_grp_name_p,
		        rpc_c_ns_syntax_default,
		        (unsigned char *)pkss_svr_bindings_name_p,
		        &estatus);

  if (estatus != rpc_s_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                   "add PKSS server group",
                   (long) estatus);

    return (estatus);
  }

  //
  // Create a profile containing a pointer to the group specified in
  // *.cds.service_profile_name.
  //

  rpc_if_inq_id (sec_pkss_clt_if_v1_0_s_ifspec, &clt_if_id, &estatus);

  if (estatus != rpc_s_ok)
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                   "obtain PKSS server profile mgmt_if_id",
                   (long) estatus);

  rpc_ns_profile_elt_add (rpc_c_ns_syntax_default,
			  (unsigned char *)pkss_svc_pfl_name_p,
			  &clt_if_id,
			  rpc_c_ns_syntax_default,
			  (unsigned char *)pkss_svc_grp_name_p,
			  0, // default profile member priority
			  (unsigned char *)pkss_svc_pfl_name_p,
			  &estatus);

  if (estatus != rpc_s_ok)
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                   "add PKSS server profile sec_pkss_clt_if_v1_0_s_if_id",
                   (long) estatus);
  else
  {
    rpc_if_inq_id (sec_pkss_mgmt_if_v1_0_s_ifspec, &mgmt_if_id, &estatus);

    if (estatus != rpc_s_ok)
      dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                     "obtain PKSS server profile mgmt_if_id",
                     (long) estatus);

    rpc_ns_profile_elt_add (rpc_c_ns_syntax_default,
			    (unsigned char *)pkss_svc_pfl_name_p,
			    &mgmt_if_id,
			    rpc_c_ns_syntax_default,
			    (unsigned char *)pkss_svc_grp_name_p,
			    0, // default profile member priority
			    (unsigned char *)pkss_svc_pfl_name_p,
			    &estatus);
  }

  if (estatus != rpc_s_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                   "add PKSS server profile sec_pkss_mgmt_if_v1_0_s_if_id",
                   (long) estatus);

    error_status_t tstatus = 0;
    rpc_ns_group_delete (rpc_c_ns_syntax_default,
			 (unsigned char *)pkss_svc_grp_name_p,
			 &tstatus);

    if (tstatus != rpc_s_ok)
    {
      dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                     "remove PKSS server group",
                     (long) estatus);
    }

    return (estatus);

  } // End if (estatus != rpc_s_ok)

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_ns_setup


//
// This function logs a PKSS server into DCE.
//

error_status_t sec_pkss_svr_login
(
  char *pkss_name_p,
  sec_login_handle_t *login_context_p
)

{
  error_status_t estatus = 0;

  //
  // Setup PKSS network login identity.
  //

  sec_login_setup_identity ((unsigned char *)pkss_name_p,
			    sec_login_no_flags,
			    login_context_p,
			    &estatus);
  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "login setup identity",
		   (long) estatus);
    return (estatus);
  }

  //
  // Login using password from specified keytab.
  //

  boolean32 used_key_vno = 0;
  boolean32 reset_pwd = 0;
  sec_login_auth_src_t auth_src;
  sec_login_valid_from_keytable (*login_context_p,
				 rpc_c_authn_dce_secret,
				 0, // use default keytab
				 0, // obtain most recent key
				 &used_key_vno,
				 &reset_pwd,
				 &auth_src,
//				 sec_login_auth_src_network,
				 &estatus);
  if (estatus != error_status_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "login valid from keytable",
		   (long) estatus);
    return (estatus);
  }

  //
  // Set network credetials to those specified by the login context.
  //

  sec_login_set_context (*login_context_p, &estatus);

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "login set context",
		   (long) estatus);
  }

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_svr_login


//
// This function implements the PKSS main calling procedure.
//

main (int argc, char **argv)
{
  error_status_t estatus = 0;

  rpc_if_handle_t rdacl_ifspec;
  uuid_t object_uuid;
  uuid_vector_t obj_uuids;
  short debug_flag = 0;

  //
  // Initialize servicability and messaging.
  // 

  setlocale(LC_ALL, "");

  dce_svc_set_progname("pks", &estatus);

  pks_svc_handle = dce_svc_register (pks_svc_table,
				     (idl_char*)"pks",
				     &estatus);  

  if (estatus != svc_s_ok) {
    fprintf(stderr, "unable to register serviceability message table\n");
  }

  pks_svc_handle_is_valid = true;

  dce_msg_define_msg_table (pks__table,
			    sizeof (pks__table) / sizeof (pks__table[0]),
			    &estatus);
  
  // 
  // Setup optional routing location.
  // 

  for (int i=1; i < argc; ++i) 
  {
    if (strcmp(argv[i], "-w") == 0) 
    {
      dce_svc_routing((unsigned_char_t *)argv[++i], &estatus);
      if (estatus != 0) 
      {
	fprintf(stderr, "bad routespec\n");
	exit(1);
      }
    }
    break;
  }
 
  // 
  // Run in background if not debugging
  // 

  for (i= 1; i < argc; ++i) 
  {
    if (strcmp(argv[i], "-d") == 0) 
    {
      debug_flag = 1;
    }
    break;
  }

  if (!debug_flag)
  {
    if (fork())
      exit(0);
  
    (void) setsid ();
    (void) chdir ("/");
    (void) umask (177);

    for (i = 0;i<_POSIX_OPEN_MAX;i++)
      close(i);

#ifdef SIGTTOU
    (void) signal (SIGTTOU, SIG_IGN);
#endif
#ifdef SIGTTIN
    (void) signal (SIGTTIN, SIG_IGN);
#endif
#ifdef SIGTSTP
    (void) signal (SIGTSTP, SIG_IGN);
#endif
  }
 
  dce_svc_printf(PKS_S_START_MSG);

  char *pkss_svc_grp_name_p = 0;
  char *pkss_svc_pfl_name_p = 0;
  char *pkss_svr_bindings_name_p = 0;
  char *pkss_princ_name_p = 0;
  char *pkss_db_name_p = 0;

  //
  // Initialize PKSS configuration facility, and obtain required PKSS
  // configuration values.
  //


  estatus = sec_pkss_get_svr_configs (&pkss_svc_grp_name_p,
				      &pkss_svc_pfl_name_p,
				      &pkss_svr_bindings_name_p,
				      &pkss_princ_name_p,
				      &pkss_db_name_p);

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "obtain PKSS server configs",
		   (long) estatus);
    exit (1);
  }

  //
  // Log the PKSS server into DCE.
  //

  sec_login_handle_t login_context;
  estatus = sec_pkss_svr_login (pkss_princ_name_p, &login_context);

  if (estatus != error_status_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "PKSS server login",
		   (long) estatus);
    exit(1);
  }

  rpc_server_register_auth_info ((unsigned char *)pkss_princ_name_p,
				 rpc_c_authn_default,
				 0,
				 0,
				 &estatus);

  if (estatus != error_status_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "rpc_server_register_auth_info",
		   (long) estatus);
    exit(1);
  }

  //
  // Setup PKSS namespace.
  //

  estatus = sec_pkss_ns_setup (pkss_svc_grp_name_p,
			       pkss_svr_bindings_name_p,
			       pkss_svc_pfl_name_p);

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "setup PKSS namespace entries",
		   (long) estatus);
    exit (1);
  }


  //
  // Instantiate the PKSS database class, that is, open PKSS database and
  // obtain a private PKSS database handle to be used by PKSS DB (gdb44)
  // methods.
  //

  pkss_db_p = new Gdb44C ((char *)pkss_db_name_p);

  if (pkss_db_p == 0)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "instantiate PKSS DB handle",
		   (long) estatus);

    goto PURGE_LOGIN_CONTEXT;
  }

  //
  // ACL manager initialization goes here.
  //
  estatus = pkss_initialize_acl_manager(pkss_db_p,
                                        &object_uuid,
					&rdacl_ifspec);

  if (estatus != rpc_s_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "initialize ACL manager",
		   (long) estatus);

    goto CLOSE_DB;
  }


  //
  // Register PKSS user login interface.
  //

  rpc_server_register_if (sec_pkss_clt_if_v1_0_s_ifspec,
			  (uuid_t *)0,
			  (rpc_mgr_epv_t )0,
			  &estatus);

  if (estatus != rpc_s_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "register login if sec_pkss_clt_if_v1_0",
		   (long) estatus);

    goto UNINIT_ACL_MANAGER;
  }

  //
  // Register PKSS management interface.
  //

  rpc_server_register_if (sec_pkss_mgmt_if_v1_0_s_ifspec,
			  (uuid_t *)0,
			  (rpc_mgr_epv_t )0,
			  &estatus);


  if (estatus != rpc_s_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "register management if sec_pkss_mgmt_if_v1_0",
		   (long) estatus);

    goto UNREGISTER_CLT_IF;
  }


  rpc_server_register_if (rdacl_ifspec,
			  (uuid_t *)0,
			  (rpc_mgr_epv_t )0,
			  &estatus);

  if (estatus != rpc_s_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "register rdacl interface",
		   (long) estatus);

    goto UNREGISTER_MGMT_IF;
  }


  //
  // The PKSS shall use all available protocol sequences.
  //

  rpc_server_use_all_protseqs (rpc_c_protseq_max_reqs_default, &estatus);

  if (estatus != rpc_s_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "use all protseqs",
		   (long) estatus);

    goto UNREGISTER_RDACL_IF;
  }

  //
  // Obtain PKSS binding handles.
  //

  rpc_binding_vector_t *binding_vector_p;
  rpc_server_inq_bindings (&binding_vector_p, &estatus);

  if (estatus != rpc_s_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "inquire bindings",
		   (long) estatus);

    goto UNSET_PROTSEQS;
  }

  //
  // Register binding handles over the PKSS user login client inteface
  // with the endpoint mapper.
  //

  rpc_ep_register (sec_pkss_clt_if_v1_0_s_ifspec,
		   binding_vector_p,
		   (uuid_vector_t *)0,
		   (unsigned char *)"sec_pkss_clt_if_v1_0",
		   &estatus);

  if (estatus != rpc_s_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "ep register binding sec_pkss_clt_if_v1_0",
		   (long) estatus);

    goto FREE_BINDING_VECTOR;
  }



  //
  // Register binding handles over the PKSS management inteface with the
  // endpoint mapper.
  //

  rpc_ep_register (sec_pkss_mgmt_if_v1_0_s_ifspec,
		   binding_vector_p,
		   (uuid_vector_t *)0,
		   (unsigned char *)"sec_pkss_mgmt_if_v1_0",
		   &estatus);

  if (estatus != rpc_s_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "ep register binding sec_pkss_mgmt_if_v1_0",
		   (long) estatus);

    goto UNREGISTER_CLT_EPV;
  }



  //
  // Register binding handles for the ACL manager interface with the 
  // endpoint mapper.
  //
  obj_uuids.count=1;
  obj_uuids.uuid[0] = &object_uuid;

  rpc_ep_register (rdacl_ifspec,
		   binding_vector_p,
		   &obj_uuids,
		   (unsigned char *)"PKSS ACL manager, V1.0",
		   &estatus);

  if (estatus != rpc_s_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "ep register binding rdacf_if",
		   (long) estatus);

    goto UNREGISTER_MGMT_EPV;
  }


  //
  // Export binding handles for the PKSS user login client interface.
  //

  rpc_ns_binding_export (rpc_c_ns_syntax_dce,
                         (unsigned char *)pkss_svr_bindings_name_p,
			 sec_pkss_clt_if_v1_0_s_ifspec,
			 binding_vector_p,
			 (uuid_vector_t *)0,
			 &estatus);

  if (estatus != rpc_s_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "export binding vector sec_pkss_clt_if_v1_0",
		   (long) estatus);

    goto UNREGISTER_RDACL_EPV;
  }

  //
  // Export binding handles for the PKSS management interface.
  //

  rpc_ns_binding_export (rpc_c_ns_syntax_dce,
                         (unsigned char *)pkss_svr_bindings_name_p,
			 sec_pkss_mgmt_if_v1_0_s_ifspec,
			 binding_vector_p,
			 (uuid_vector_t *)0,
			 &estatus);

  if (estatus != rpc_s_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "export binding vector sec_pkss_mgmt_if_v1_0",
		   (long) estatus);

    goto UNEXPORT_CLT_IF;
  }

  //
  // Export binding handles for the ACL management interface
  //

  rpc_ns_binding_export (rpc_c_ns_syntax_dce,
                         (unsigned char *)pkss_svr_bindings_name_p,
			 rdacl_ifspec,
			 binding_vector_p,
			 &obj_uuids,
			 &estatus);

  if (estatus != rpc_s_ok)
  {
    dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		   "export binding vector for rdacl interface",
		   (long) estatus);

    goto UNEXPORT_MGMT_IF;
  }


  //
  // Begin listening for client requests.
  //

  TRY
  {
    dce_svc_printf(PKS_S_READY_TO_LISTEN_MSG);

    rpc_server_listen (rpc_c_listen_max_calls_default, &estatus);

    if (estatus != rpc_s_ok)
    {
      dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
		     "server listen",
		     (long) estatus);
    }
  }

  CATCH_ALL
  {
    if (pkss_db_p)
    {
      delete pkss_db_p;
      pkss_db_p = 0;
    }

    //
    // Drop through to the rundown sequence.
    //
  }

  FINALLY
  {
    if (pkss_db_p)
    {
      delete pkss_db_p;
      pkss_db_p = 0;
    }

    //
    // Drop through to the rundown sequence.
    //

  } ENDTRY;

//
// This section implements server cleanup/shutdown.  Observe that rundown
// occurs in exactly reverse order from initialization.
//

RUNDOWN:

    ;
    

UNEXPORT_RDACL_IF:

    rpc_ns_binding_unexport (rpc_c_ns_syntax_dce,
                             (unsigned char *)pkss_svr_bindings_name_p,
                             rdacl_ifspec,
                             &obj_uuids,
                             &estatus);

    if (estatus != rpc_s_ok)
    {
      dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                     "binding unexport rdacl interface",
                     (long) estatus);
    }


UNEXPORT_MGMT_IF:

    rpc_ns_binding_unexport (rpc_c_ns_syntax_dce,
                             (unsigned char *)pkss_svr_bindings_name_p,
                             sec_pkss_mgmt_if_v1_0_s_ifspec,
                             (uuid_vector_t *)0,
                             &estatus);

    if (estatus != rpc_s_ok)
    {
      dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                     "binding unexport sec_pkss_mgmt_if_v1_0",
                     (long) estatus);
    }


UNEXPORT_CLT_IF:

    rpc_ns_binding_unexport (rpc_c_ns_syntax_dce,
                             (unsigned char *)pkss_svr_bindings_name_p,
                             sec_pkss_clt_if_v1_0_s_ifspec,
                             (uuid_vector_t *)0,
                             &estatus);

    if (estatus != rpc_s_ok)
    {
      dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                     "binding unexport sec_pkss_clt_if_v1_0",
                     (long) estatus);
    }


UNREGISTER_RDACL_EPV:

    rpc_ep_unregister (rdacl_ifspec,
                       binding_vector_p,
                       &obj_uuids,
                       &estatus);

    if (estatus != rpc_s_ok)
    {
      dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                     "server unregister ep rdacl interface",
                     (long) estatus);
    }


UNREGISTER_MGMT_EPV:

    rpc_ep_unregister (sec_pkss_mgmt_if_v1_0_s_ifspec,
                       binding_vector_p,
                       (uuid_vector_t *)0,
                       &estatus);

    if (estatus != rpc_s_ok)
    {
      dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                     "server unregister ep sec_pkss_mgmt_if_v1_0",
                     (long) estatus);
    }


UNREGISTER_CLT_EPV:

    rpc_ep_unregister (sec_pkss_clt_if_v1_0_s_ifspec,
                       binding_vector_p,
                       (uuid_vector_t *)0,
                       &estatus);

    if (estatus != rpc_s_ok)
    {
      dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                     "server unregister ep sec_pkss_clt_if_v1_0",
                     (long) estatus);
    }


FREE_BINDING_VECTOR:

    rpc_binding_vector_free (&binding_vector_p, &estatus);

    if (estatus != rpc_s_ok)
    {
        dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                       "free binding vector",
                       (long) estatus);
    }


UNSET_PROTSEQS:

    ;

  
UNREGISTER_RDACL_IF:

    rpc_server_unregister_if (rdacl_ifspec,
                              (uuid_t *)0,
                              &estatus);

    if (estatus != rpc_s_ok)
    {
        dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                       "server unregister if sec_pkss_mgmt_if_v1_0",
                       (long) estatus);
    }


UNREGISTER_MGMT_IF:

    rpc_server_unregister_if (sec_pkss_mgmt_if_v1_0_s_ifspec,
                              (uuid_t *)0,
                              &estatus);

    if (estatus != rpc_s_ok)
    {
       dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                      "server unregister if sec_pkss_mgmt_if_v1_0",
                      (long) estatus);
    }


UNREGISTER_CLT_IF:

    rpc_server_unregister_if (sec_pkss_clt_if_v1_0_s_ifspec,
                              (uuid_t *)0,
                              &estatus);

    if (estatus != rpc_s_ok)
    {
       dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                      "server unregister if sec_pkss_clt_if_v1_0",
                      (long) estatus);
    }


UNINIT_ACL_MANAGER:

    ;

  
CLOSE_DB:

    ;

  
PURGE_LOGIN_CONTEXT:

    sec_login_purge_context (&login_context, &estatus);

    if (estatus != error_status_ok)
    {
       dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                      "PKSS server login context",
                      (long) estatus);
    }


DELETE_NS_PROFILE:

    rpc_ns_profile_delete (rpc_c_ns_syntax_default,
			   (unsigned char *)pkss_svc_pfl_name_p,
			   &estatus);

    if (estatus != rpc_s_ok)
    {
      dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                     "remove PKSS ns profile entry",
                     (long) estatus);
    }


DELETE_NS_GROUP:

    rpc_ns_group_delete (rpc_c_ns_syntax_default,
                         (unsigned char *)pkss_svc_grp_name_p,
                         &estatus);

    if (estatus != rpc_s_ok)
    {
      dce_svc_printf(PKS_S_CANT_INIT_SOMETHING_MSG,
                     "remove PKSS server group",
                     (long) estatus);
    }

  //
  // Exit this program
  //

    return (estatus);

} // End main for the PKSS server

