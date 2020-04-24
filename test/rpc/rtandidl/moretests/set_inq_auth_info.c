/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: set_inq_auth_info.c,v $
 * Revision 1.1.13.2  1996/02/17  23:22:09  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:08  marty]
 *
 * Revision 1.1.13.1  1995/12/11  20:10:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:01  root]
 * 
 * Revision 1.1.11.1  1994/09/13  16:16:29  tom
 * 	Bug 8541 - Using TSHMessage instead of TSHStatus to report failure.
 * 	Actually use null server name flag if desired.
 * 	[1994/09/13  16:13:33  tom]
 * 
 * Revision 1.1.9.1  1993/10/14  21:59:50  tom
 * 	Bug 8300 - Include stdlib.h.
 * 	[1993/10/14  21:57:52  tom]
 * 
 * Revision 1.1.7.4  1993/01/11  23:40:13  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:13:47  bbelch]
 * 
 * Revision 1.1.7.3  1992/10/12  19:01:09  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/12  18:45:44  cuti]
 * 
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  21:22:55  cuti]
 * 
 * Revision 1.1.7.2  1992/09/29  21:23:03  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:48:08  weisman]
 * 
 * Revision 1.1.3.3  1992/07/21  20:39:33  dceguest
 * 	Added do_site_open after login routine.
 * 	[1992/07/21  15:54:00  dceguest]
 * 
 * Revision 1.1.3.2  1992/07/15  02:40:50  smith
 * 	removed declarations for sec_login_setup_identity.
 * 
 * 	declared a local error (cstatus) value for errors not necessarily
 * 	       related to the completion of the test.
 * 
 * 	took out dead commented out code.
 * 
 * 	changed the do_add_pgo...  to log_in(), add_account(), log_out()
 * 	       sequence.
 * 
 * 	added logic to add the server account as well as the client account.
 * 
 * 	added subroutine clean_up to remove the accounts added
 * 	       when a failure occurs, and at the end of the test.
 * 
 * 	changed: bzero ((unsigned_char_t) binding, sizeof (binding));
 * 	       to:      binding = (rpc_binding_handle_t)NULL;
 * 
 * 	       This fixed the a core dump.
 * 
 * 	changed method of returning failures to indicate more clearly
 * 	       which function is failing in the test and for what reason.
 * 	[1992/07/15  02:40:04  smith]
 * 
 * Revision 1.1  1992/01/19  03:36:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* 
 * Copyright 1991 by Apollo Computer, Inc.
 * 
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of Apollo Computer, Inc not
 * be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * Apollo Computer, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 * 
 *
 *   
 *	Extracted	1/14/92 11:32:41
 *	from delta on	11/8/91 16:53:45
 *
 *  $LOG$
 *
 *
*/

#define main_code
#define INQ_ARG 8
#define SET_ARG 11 
                 
/* Include files */ 
#include <sec_rgy.h>
#include <tsh_auth.h>
#include <dce/sec_login.h>
#include <sys/file.h>
#include <errno.h>
#include <stdlib.h>

static	void clean_up ();
static	sec_rgy_handle_t	context;
static  sec_login_handle_t      auth_id;

ndr_long_int TestFunction (ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
   ndr_long_int		subcase;	    /* argv[0], subcase to run */
   error_status_t	estatus;	    /* argv[1], expected return from test */
   error_status_t	tstatus;	    /* actual value returned from rpc */
   error_status_t	cstatus;	    /* status of cleanup operation */
   sec_rgy_name_t	client_name;        /* name fo client to add to rgy */
   sec_passwd_str_t	passwd;             /* passwrod for name account */
   sec_passwd_str_t	upasswd;             /* passwrod for name account */
   sec_rgy_name_t	server_name;        /* name fo client to add to rgy */
   unsigned_char_t	str_binding_buf[128];    /* rpc_binding_from_string_binding */
   rpc_binding_handle_t	binding;	    /* used by rpc_binding_*_auth_info */
   unsigned_char_p_t	string_binding;
   unsigned_char_t	*server_princ_name; /* used by rpc_binding_*_auth_info */
   unsigned32		authn_level;	    /* used by rpc_binding_*_auth_info */
   unsigned32		authn_svc;	    /* used by rpc_binding_*_auth_info */
   unsigned32		authz_svc;	    /* used by rpc_binding_*_auth_info */
   unsigned32		args_required;      /* depends on who called this function */
   unsigned32		servname_flag;	    /* argv [2] */
   ndr_long_int		fd;		    /* open, read, write system calls */

   /*
    * Obtain the minimum input arguments
    * from the .tsh command line.
    */

   subcase = atol(argv[0]);
   estatus = atol(argv[1]);
   servname_flag = atol(argv[2]);
   strcpy(client_name, argv[3]);
   strcpy(passwd, argv[4]);
   strcpy(upasswd, argv[5]);
   strcpy(server_name, argv[6]);

   TSHMessage(ctx, MOINFO, "0=%d 1=%d 2=%d 3=%s 4=%s 5=%s 6=%s\n",
      subcase, estatus, servname_flag, client_name,
      passwd, upasswd, server_name);

   /*
    * The number of arguments required
    * will vary with the test being conducted.
    */

   if (subcase == INQ_AUTH_INFO_WO_SETUP)
      args_required = INQ_ARG;
   else
      args_required = SET_ARG;

   /*
    * Check the number of arguments passed in.
    * They should be exactly the required number
    * when you come into this routine.
    */

   if (argc != args_required) {
      TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
   	args_required, argc);
      return(TSH_BAD_ARG_COUNT);
   }

   /*
    * Obtain the rest of the command
    * line args, if required.
    */

   if (args_required == SET_ARG) {

      authn_level = atol(argv[8]);
      authn_svc = atol(argv[9]);
      authz_svc = atol(argv[10]);

      TSHMessage(ctx, MOINFO, "7=%d 8=%d 9=%d 10=%d\n",
              atol(argv[7]), atol(argv[8]), atol(argv[9]), atol(argv[10]));

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

   	case AUTHN_SVC_UNKNOWN:
   	   authn_svc = AUTHN_SVC_UNKNOWN;
   	   break;

   	case AUTHN_SVC_DEFAULT:
   	   authn_svc = rpc_c_authn_default;
   	   break;
      }

      switch (authz_svc) {

   	case AUTHZ_NONE:
   	   authz_svc = rpc_c_authz_none;
   	   break;

   	case AUTHZ_NAME:
   	   authz_svc = rpc_c_authz_name;
   	   break;

   	case AUTHZ_DCE:
   	   authz_svc = rpc_c_authz_dce;
   	   break;
      }

   }

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

   /*
    * Add the server to the rgy data base.
    */
   if (add_account (ctx, &context, server_name, SERVER_UNIX_NUM, passwd,
   	upasswd, &tstatus)) {
      remove_acct (ctx, &context, client_name, &cstatus);
      log_out (ctx, &auth_id, &cstatus);
      return tstatus;
   }

   log_out (ctx, &auth_id, &cstatus);

   /*
    * Log in as the client.
    */
   if (log_in (ctx, &auth_id, client_name, passwd, &tstatus)) {
      clean_up (ctx, client_name, server_name);
      return tstatus;
   }

   /*
    * Obtain a server binding handle
    * with which to test the rpc's.
    */

   /**************************************************************************/
   /*  Do the compose of the string, followed by the binding from string.    */
   /**************************************************************************/

   rpc_string_binding_compose(
      (unsigned_char_p_t) "41c460ba-a000-0000-0d00-00c366000000",
      (unsigned_char_p_t)argv[7], getenv("SERVERHOST"),
      (unsigned_char_p_t)"5005", NULL, &string_binding, &tstatus);

   TSHMessage(ctx,MOINFO,
      "rpc_string_binding_compose:%d\n",tstatus);

   if (TSHMessage (ctx, MOTRACE, "rpc_binding_compose status: %d\n", tstatus)) {
      clean_up (ctx, client_name, server_name);
      return tstatus;
   }

   rpc_binding_from_string_binding(string_binding, &binding, &tstatus);

   TSHMessage (ctx, MOTRACE,
   	"rpc_binding_from_string_binding status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
             "rpc_binding_from_string_binding failed", tstatus, rpc_s_ok)) {
      clean_up (ctx, client_name, server_name);
      return tstatus;
   }

   /*
    * The rpc_binding_set_auth_info rpc is called either to test it or to setup for
    * the rpc_binding_inq_auth_info tests.  Note that to force the rpc_s_no_auth_data
    * pathological cases for the inquire tests, no setup is required, and 
    * rpc_binding_set_auth_info is skipped.
    */

   TSHMessage(ctx,MOINFO,"+=+=+=+=+=+ Test Function Begins +=+=+=+=+=+\n");

   if (subcase != INQ_AUTH_INFO_WO_SETUP) {

      /*
       * If this is the rpc_binding_set_auth_info test and the
       * rpc_s_invalid_binding pathological case, make an invalid handle.  Note
       * that to force the rpc_s_invalid_binding error for the inquire tests, a
       * good handle is required here lest the test fail on setup.
       */

      if ((subcase == SET_AUTH_INFO) && (estatus == rpc_s_invalid_binding))
   	binding = (rpc_binding_handle_t)NULL;

      /*
       * Note we use the servname_flag correctly, but the NULL
       * server name argument to rpc_binding_set_auth_info isn't
       * supported.
       */
      rpc_binding_set_auth_info (binding, (servname_flag ? server_name : NULL), 
        authn_level, authn_svc, auth_id, authz_svc, &tstatus);

      TSHMessage(ctx,MOTRACE,"rpc_binding_set_auth_info status: %d\n",
      	tstatus);

      /*
       * If this is the set_auth_info case and expected status == test status then
       * the test succeeded; otherwise, if this is an inquire test, then we are
       * only doing setup and test status must be rpc_s_ok for the test to proceed.
       */

      if (subcase == SET_AUTH_INFO) {
      	if (estatus == tstatus)
      		tstatus = rpc_s_ok;
      	else
      		TSHStatus (ctx, MOFAILURE, "rpc_binding_set_auth_info failed\n", estatus, tstatus);
      }
      else {
      	if (tstatus != rpc_s_ok)
      		TSHStatus (ctx, SETUP, "rpc_binding_set_auth_info failed\n", estatus, tstatus);
      }

   } /* End if (subcase != INQ_AUTH_INFO_WO_SETUP) */

   /*
    * The rpc_binding_inq_auth_info rpc is called only
    * when it is being tested and setup succeeded.
    */

   if (((subcase == INQ_AUTH_INFO) ||
   	(subcase == INQ_AUTH_INFO_WO_SETUP)) &&
   	(tstatus == rpc_s_ok)) {

      /*
       * If this is the rpc_s_invalid_binding pathological test case,
       * make an invalid binding handle.  Note that a good handle
       * was required at setup time so that the error could
       * be forced here for the inquire tests.
       */

      if (estatus == rpc_s_invalid_binding)
   	binding = (rpc_binding_handle_t)NULL;

      rpc_binding_inq_auth_info (binding, &server_princ_name,
   	&authn_level, &authn_svc, auth_id, 
   	&authz_svc, &tstatus);

      TSHMessage (ctx, MOTRACE,
   	 "rpc_binding_inq_auth_info status: %d\n", tstatus);

      if (!TSHStatus (ctx, TEST, "rpc_binding_inq_auth_info failed\n",
   	estatus, tstatus))
	tstatus = rpc_s_ok;
   }

   /* Cleanup from the rpc's under test. */

   if (estatus != rpc_s_invalid_binding) {

      rpc_binding_free (&binding, &cstatus);

      TSHMessage (ctx, MOTRACE, "rpc_binding_free status: %d\n", cstatus);
   }

   clean_up (ctx, client_name, server_name);

   /* Exit this function. */

   return (tstatus);
}

static void
clean_up (ctx, client_name, server_name)
CtxBlock	*ctx;
sec_rgy_name_t	client_name; 
sec_rgy_name_t	server_name; 
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

   /* remove the client and server accounts */
   remove_acct (ctx, &context, client_name, &cstatus);
   remove_acct (ctx, &context, server_name, &cstatus);

   /* log out as the cell administrator */
   log_out (ctx, &auth_id, &cstatus);

   return;
}
