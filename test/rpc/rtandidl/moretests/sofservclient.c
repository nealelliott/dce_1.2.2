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
 * $Log: sofservclient.c,v $
 * Revision 1.1.7.2  1996/02/17  23:22:14  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:10  marty]
 *
 * Revision 1.1.7.1  1995/12/11  20:10:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:05  root]
 * 
 * Revision 1.1.5.4  1993/01/11  23:40:19  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:13:57  bbelch]
 * 
 * Revision 1.1.5.3  1992/10/14  13:26:54  cuti
 * 	Removed status checking for stop_server_listening.  User has to do it manually. & Use all protocols.
 * 	[1992/10/13  21:10:25  cuti]
 * 
 * Revision 1.1.5.2  1992/09/29  21:23:19  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:48:19  weisman]
 * 
 * Revision 1.1.2.3  1992/07/21  20:41:33  dceguest
 * 	Added the do_site_open() routine after log_in().
 * 	[1992/07/21  15:59:05  dceguest]
 * 
 * Revision 1.1.2.2  1992/07/15  15:03:54  smith
 * 	changed the do_add_pgo...  to log_in(), add_account(), log_out()
 * 		sequence.
 * 
 * 	checking return codes for open, read, close sequence of
 * 		the binding file.
 * 
 * 	added logic for tests that were in .tsh file but not in the code.
 * 
 * 	added subroutine clean_up to remove the accounts added
 * 		when a failure occurs, and at the end of the test.
 * 
 * 	changed method of returning failures to indicate more clearly
 * 		which function is failing in the test and for what reason.
 * 	[1992/07/15  15:01:36  smith]
 * 
 * Revision 1.1  1992/01/19  03:37:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
 *	Extracted	1/4/92 16:53:55
 *	from delta on	11/8/91 16:53:53
 *
 *  $LOG$
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/rpc/rtandidl/moretests/sofservclient.c, , dce_osf, dce.75d";
#endif

/* Include files */
     
#define main_code
#include <tsh_auth.h>
#include <sec_rgy.h>
#include <dce/sec_login.h>
#include <test_rpc_auth.h>
#include <dce/uuid.h>
#include <dce/rpc.h>
#ifdef SNI_SVR4
#include <fcntl.h>
#else
#include <sys/file.h>
#endif
#include <sys/errno.h>

/* Args required by the test program. */

#define ARGS_REQUIRED 8 

static void clean_up ();
static sec_rgy_handle_t        context;
static sec_login_handle_t     auth_id;

ndr_long_int TestFunction (ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
   ndr_long_int		subcase;	/* argv[0], subcase to run */
   error_status_t	estatus;	/* argv[1], expected return from test */
   sec_rgy_name_t	client_name; 
   sec_passwd_str_t	passwd;
   sec_passwd_str_t	upasswd;
   sec_rgy_name_t	server_name; 
   unsigned32		authn_svc = rpc_c_authn_dce_private;
   sec_login_auth_src_t	auth_src;	/* sec_login_validate_identity */
   rpc_binding_handle_t	binding;	/* rpc_binding_from_string_binding */
   error_status_t	tstatus;	/* test status */
   error_status_t	cstatus;	/* cleanup status */
   ndr_long_int		fd;		/* file description to obtain binding */
   ndr_long_int		bytes_read;	/* number of bytes read on read */
   unsigned_char_p_t	str_binding_buf;/* rpc_binding_from_string_binding */

   /*
    * Check the number of arguments passed in.
    * They should be exactly the required number
    * when you come into this routine.
    */

   if (argc != ARGS_REQUIRED) {
   	TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n",
   		ARGS_REQUIRED, argc);
   	return (TSH_BAD_ARG_COUNT);
   }

   /*
    * Obtain the input arguments from
    * the .tsh command line.
    */

   subcase = atol(argv[0]);
   estatus = atol(argv[1]);
   strcpy(client_name, argv[2]);
   strcpy(passwd, argv[3]);
   strcpy(upasswd, argv[4]);
   strcpy(server_name, argv[5]);

   /* Initialize some other things. */

   pthread_setasynccancel (CANCEL_ON);
   
   /*
    * Authenticate as the cell administrator
    * and add the necessary entries to the
    * security data base.
    */
                
   if (log_in (ctx, &auth_id, CELL_ADMIN, ADMIN_PASSWD, &tstatus))
      return tstatus;

   if (do_site_open (ctx, error_status_ok, &context, &tstatus)) {
      log_out (ctx, &auth_id, &cstatus);
      return tstatus;
   }

   /*
    * Add the client to the security data base.
    */
   if (add_account (ctx, &context, client_name, CLIENT_UNIX_NUM, passwd,
    	ADMIN_PASSWD, &tstatus)) {
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

   /*
    * Obtain a binding handle for this client
    * from opening and reading a file left from the server.
    */
             
    rpc_string_binding_compose(
       NULL, (unsigned_char_p_t)argv[7], 
       argv[6], NULL, NULL, &str_binding_buf, &tstatus);

    if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", tstatus, rpc_s_ok))
       return(tstatus);
    rpc_binding_from_string_binding (str_binding_buf, &binding, &tstatus);
    TSHMessage (ctx, MOTRACE, "rpc_binding_from_string_binding status: %d\n",
   		tstatus);

   if (TSHStatus (ctx, SETUP, "rpc_binding_from_string_binding failed\n",
   	rpc_s_ok, tstatus)) { 
   	clean_up (ctx, client_name);
   	return (tstatus);
   }
   else {
   	TSHMessage (ctx, MOTRACE, "binding handle is %s\n",
   		str_binding_buf);
   }

   if (subcase == AUTHN_SVC_NONE)
	authn_svc = rpc_c_authn_none;

   if (subcase != INQ_AUTH_CLIENT_WO_SETUP) {

       /*
        * Insert authentication information
        * into the binding handle.
        */

   	rpc_binding_set_auth_info (binding,
                              server_name,
                              rpc_c_authn_level_default,
                              authn_svc,
                              auth_id,
                              rpc_c_authz_dce,
                              &tstatus);

   	TSHMessage (ctx, MOTRACE,
   	   "rpc_binding_set_auth_info status: %d\n", tstatus);

   	if (TSHStatus (ctx, SETUP, "rpc_binding_set_auth_info failed\n",
   	   rpc_s_ok, tstatus)) {
   	   rpc_binding_free (&binding, &cstatus);
   	   TSHMessage (ctx, MOTRACE,
   		"rpc_binding_free status: %d\n", cstatus);
   	   clean_up (ctx, client_name);
   	   return (tstatus);
      }
   }

   /*
    * Poke the server with a request to rpc_binding_inq_auth_client
    * to test it.  To call a client epv stub in this manner requires
    * that the -cepv switch was used on the nidl compiler command line.
    */

/*
   rpc__dbg_set_switches("0-4.10", &tstatus);

   (*test_rpc_auth_v1_0_c_epv.test_inq_auth_client)
   	(binding, &tstatus, &cstatus);
*/

   tstatus = estatus; 
   test_inq_auth_client(binding, subcase, server_name, rpc_c_authn_level_default, authn_svc, rpc_c_authz_dce, &tstatus, &cstatus);

   TSHMessage (ctx, MOTRACE, "rpc_binding_inq_auth_client status: %d\n",
   	tstatus);

   if (estatus == tstatus)
   	tstatus = rpc_s_ok;
   else {
	if (tstatus == TSH_DATA_MISSMATCH)  
		TSHMessage(ctx, MOFAILURE, 
			"rpc_binding_inq_auth_client: expected NULL server name, got non-NULL\n");
	TSHMessage(ctx,MOFAILURE,"rpc_binding_inq_auth_client failed\n"); 
	}

   /* Cleanup from the rpc under test. */

   rpc_binding_free (&binding, &cstatus);

   TSHMessage (ctx, MOTRACE, "rpc_binding_free status: %d\n", cstatus);

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
