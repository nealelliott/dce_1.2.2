/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: if_reg_auth_info.c,v $
 * Revision 1.1.7.2  1996/02/17  23:20:36  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:21  marty]
 *
 * Revision 1.1.7.1  1995/12/11  20:05:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:19  root]
 * 
 * Revision 1.1.5.3  1993/01/11  22:14:20  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:06:25  bbelch]
 * 
 * Revision 1.1.5.2  1992/10/12  16:37:34  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/12  14:41:36  cuti]
 * 
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  17:58:50  cuti]
 * 
 * Revision 1.1.3.3  1992/07/21  20:34:45  dceguest
 * 	Added a site_open before log_out.  This
 * 	prevented testcase cleanup failures.
 * 	[1992/07/21  15:21:55  dceguest]
 * 
 * Revision 1.1.3.2  1992/07/15  01:33:57  smith
 * 	*** empty log message ***
 * 
 * Revision 1.1.2.2  1992/07/15  01:32:50  smith
 * 	removed declarations for sec_login_setup_identity.
 * 
 * 	declared a local error (cstatus) value for errors not
 * 	necessarily related to the completion of the test.
 * 
 * 	took out dead commented out code.
 * 
 * 	changed the do_add_pgo...  to log_in(), add_account(), log_out()
 * 		sequence.
 * 
 * 	added subroutine clean_up to remove the accounts added
 * 		when a failure occurs, and at the end of the test.
 * 
 * 	changed method of returning failures to indicate more clearly
 * 		which function is failing in the test and for what reason.
 * 
 * Revision 1.1  1992/01/19  03:35:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * Copyright 1989 by Hewlett-Packard company 1991
 * Unpublished work.  All rights reserved.
 * 
 *   
 *	Extracted	1/4/92 16:53:28
 *	from delta on	11/8/91 16:47:09
 *
 *  $Log
 *
 *
*/


#ifndef LINT
static char sccs_id[] = "src/test/rpc/rtandidl/moretests/if_reg_auth_info.c, , dce_osf, dce.75d";
#endif

/* Include files */
    
#define main_code
#include <sec_rgy.h>
#include <tsh_auth.h>
#include <tsh_mgmt.h>
#include <dce/sec_login.h>

/*  Args required by the test program.  */

#define ARGS_REQUIRED	11
static sec_rgy_handle_t        context;
static void	clean_up ();

static sec_login_handle_t       auth_id;

ndr_long_int
TestFunction (ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
   ndr_long_int		subcase;	/* argv[0], subcase to run */
   error_status_t	estatus;	/* argv[1], expected return from test */
   error_status_t	tstatus;	/* actual value returned from rpc */
   error_status_t	cstatus;	/* status of cleanup operation */
   sec_rgy_name_t	client_name; 
   sec_rgy_name_t	server_name; 
   sec_passwd_str_t	passwd;
   sec_passwd_str_t	upasswd;
   unsigned32		authn_level;	/* used by rpc_if_register_auth_info */
   unsigned32		authn_svc;	/* used by rpc_if_register_auth_info */
   unsigned32		authz_svc;	/* used by rpc_if_register_auth_info */
   unsigned_char_p_t	server_princ_name; /* used by rpc_if_register_auth_info */
   rpc_endpoint_vector_elt_t endpoints;	/* required by NIDL_ifspec */
   unsigned32		servname_flag;	/* argv[5] */

   /*
    * Check the number of arguments passed in.
    * They should be exactly the required number
    * when you come into this routine.
    */

   if (argc != ARGS_REQUIRED) {
   	TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
   			ARGS_REQUIRED, argc);
   	return(TSH_BAD_ARG_COUNT);
   }

   /* Obtain the input arguments from the .tsh command line. */

   subcase = atol(argv[0]);
   estatus = atol(argv[1]);
   authn_level = atol(argv[2]);
   authn_svc = atol(argv[3]);
   authz_svc = atol(argv[4]);
   servname_flag = atol(argv[5]);
   strcpy(client_name, argv[6]);
   strcpy(passwd, argv[7]);
   strcpy(upasswd, argv[8]);
   strcpy(server_name, argv[9]);

   /* Adjust input values as necessary. */

   switch (authn_level) {

      case AUTHN_LEVEL_DEFAULT:
   	authn_level = rpc_c_authn_level_default;
   	break;

      case AUTHN_LEVEL_NONE:
      	authn_level = rpc_c_authn_level_none;
      	break;

      case AUTHN_LEVEL_CONNECT:
   	authn_level = rpc_c_authn_level_connect;
   	break;

      case AUTHN_LEVEL_CALL:
   	authn_level = rpc_c_authn_level_call;
   	break;

      case AUTHN_LEVEL_PKT:
   	authn_level = rpc_c_authn_level_pkt;
   	break;

      case AUTHN_LEVEL_PKT_INTEGRITY:
   	authn_level = rpc_c_authn_level_pkt_integrity;
   	break;

      case AUTHN_LEVEL_PKT_PRIVACY:
   	authn_level = rpc_c_authn_level_pkt_privacy;
   	break;
   }

   switch (authn_svc) {

      case AUTHN_SVC_NONE:
   	authn_svc = rpc_c_authn_none;
   	break;

      case AUTHN_SVC_DCE_PRIVATE:
   	authn_svc = rpc_c_authn_dce_private;
   	break;

      case AUTHN_SVC_DCE_PUBLIC:
   	authn_svc = rpc_c_authn_dce_public;
   	break;

      case AUTHN_SVC_DEFAULT:
   	authn_svc = rpc_c_authn_default;
   	break;
   }

   switch (authz_svc) {

      case AUTHZ_NONE:
   	authn_svc = rpc_c_authz_none;
   	break;

      case AUTHZ_NAME:
   	authn_svc = rpc_c_authz_name;
   	break;

      case AUTHZ_DCE:
   	authn_svc = rpc_c_authz_dce;
   	break;
   }

   if (servname_flag == USE_SERVNAME)
      server_princ_name = server_name;
   else
      server_princ_name = NULL;
       
   if (log_in (ctx, &auth_id, CELL_ADMIN, ADMIN_PASSWD, &tstatus))
      return tstatus;

   if (do_site_open (ctx, error_status_ok, &context, &tstatus)) {
   	   log_out (ctx, &auth_id, &cstatus);
      return tstatus;
   }

   /*
    * Add the client to the rgy data base.
    */
   if (add_account (ctx, &context, client_name, CLIENT_UNIX_NUM, passwd,
   	upasswd, &tstatus)) {
   	   log_out (ctx, &auth_id, &cstatus);
      return tstatus;
   }

   log_out (ctx, &auth_id, &cstatus);

   /*
    * Log in as the client.
    */
   if (log_in (ctx, &auth_id, client_name, passwd, &tstatus)) {
      clean_up (ctx, client_name);
      return tstatus;
   }

   /* Setup the interface spec for the register. */

   endpoints.rpc_protseq = (unsigned_char_p_t)argv[10];
   endpoints.endpoint = (unsigned_char_p_t)"5003";
   NIDL_ifspec.endpoint_vector.count = 1;
   NIDL_ifspec.endpoint_vector.endpoint_vector_elt = &endpoints;

   /* Call the rpc under test. */

   rpc_if_register_auth_info ((rpc_if_handle_t)&NIDL_ifspec, server_princ_name,
   	authn_level, authn_svc, auth_id, authz_svc, &tstatus);

   TSHMessage (ctx, MOTRACE,
   	"rpc_if_register_auth_info status: %d\n", tstatus);

   if ((TSHStatus (ctx, MOFAILURE, "rpc_if_register_auth_info failed\n",
   	estatus, tstatus)));

   /* Cleanup from the rpc under test. */

   clean_up (ctx, client_name);

   return (tstatus);
}


static void
clean_up (ctx, client_name)
CtxBlock	*ctx;
sec_rgy_name_t	client_name; 
{
   error_status_t	cstatus;

   /* log out as the client.  */
   log_out (ctx, &auth_id, &cstatus);

   /*
    * log in as the cell administrator
    * if we cannot, not much we can do
    */
   if (log_in (ctx, &auth_id, CELL_ADMIN, ADMIN_PASSWD, &cstatus))
   	return;

   if (do_site_open (ctx, error_status_ok, &context, &cstatus)) {
      log_out (ctx, &auth_id, &cstatus);
      return;
   }

   /* remove the client account */
   remove_acct (ctx, &context, client_name, &cstatus);

   /* log out as the cell administrator */
   log_out (ctx, &auth_id, &cstatus);

   return;
}
