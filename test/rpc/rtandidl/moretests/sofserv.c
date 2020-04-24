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
 * $Log: sofserv.c,v $
 * Revision 1.1.7.2  1996/02/17  23:22:11  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:09  marty]
 *
 * Revision 1.1.7.1  1995/12/11  20:10:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:03  root]
 * 
 * Revision 1.1.5.7  1993/03/19  19:50:49  weisman
 * 	Fix for OT 7500
 * 	[1993/03/19  19:50:05  weisman]
 * 
 * Revision 1.1.5.6  1993/02/03  18:22:07  cuti
 * 	Add initialization to some local data, otherwise it willhave core dump in HP-UX
 * 	[1993/02/01  22:40:00  cuti]
 * 
 * Revision 1.1.5.5  1993/01/29  19:57:03  hsiao
 * 	Add "if(server_princ_name_p != NULL)" checking in test_inq_auth_client
 * 	[1993/01/29  19:38:10  hsiao]
 * 
 * Revision 1.1.5.4  1993/01/11  23:40:16  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:13:52  bbelch]
 * 
 * Revision 1.1.5.3  1992/10/14  13:26:39  cuti
 * 	ot5157: Remove rpc_stop_server_listening and change key's structure to be a plain type in set_key call, use all protocols..
 * 	[1992/10/13  21:10:03  cuti]
 * 
 * Revision 1.1.5.2  1992/09/29  21:23:12  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:48:13  weisman]
 * 
 * Revision 1.1.2.3  1992/07/15  14:54:13  smith
 * 	changed the do_add_pgo...  to log_in(), add_account(), log_out()
 * 		sequence.
 * 
 * 	changed getopt to work.
 * 
 * 	checking return codes for open, write, close sequence of
 * 		the binding file.
 * 	[1992/07/15  14:28:32  smith]
 * 
 * Revision 1.1.2.2  1992/06/30  19:19:45  cuti
 * 	     Add server right to account add.
 * 	     [1992/06/30  14:45:29  cuti]
 * 
 * Revision 1.1  1992/01/19  03:36:59  devrcs
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
 *      Extracted       1/4/92 16:53:55
 *      from delta on   11/8/91 16:53:49
 *
 *  $Log
 *
 *
*/


#ifndef LINT
static char sccs_id[] = "src/test/rpc/rtandidl/moretests/sofserv.c, , dce_osf, dce.75d";
#endif

/* Include files */
  
#define main_code

#include <tsh_main.h>
#include <tsh_auth.h> 
#include <test_rpc_auth.h>
#include <dce/uuid.h>
#include <dce/rpc.h>
#include <dce/id_base.h>
#ifdef SNI_SVR4
#include <fcntl.h>
#else
#include <sys/file.h>
#endif
#include <sec_rgy.h> 

#define KEY_TAB "/krb5/v5srvtab"

/* 
 * Porting note: There is currently a string.idl in the rtandidl 
 * suite.  This idl file is compiled into string.h.  
 * Unfortunately this name clashes with the real Posix string.h.  
 * 
 * For the moment, we just declare the function we need locally.
 * See OT 7549 for more.
 */

extern char *strstr ();

static sec_rgy_handle_t        context;
CtxBlock	lctx, *ctx = &lctx;
static  sec_login_handle_t      auth_id;

char *prot_level_name[] = {  
 	"rpc_c_protect_level_default",
 	"rpc_c_protect_level_none",
	"rpc_c_protect_level_connect",
 	"rpc_c_protect_level_call",
 	"rpc_c_protect_level_pkt", 
 	"rpc_c_protect_level_pkt_integ",
 	"rpc_c_protect_level_pkt_privacy"
	} ;

char *authn_name[] = { 
 	"rpc_c_authn_none ",
 	"rpc_c_authn_dce_secret",
 	"rpc_c_authn_dce_public",
 	"rpc_c_authn_dce_dummy",
 	"rpc_c_authn_dssa_public"
	} ;

char *authz_name[] = {
 	"rpc_c_authz_none",
 	"rpc_c_authz_name",
 	"rpc_c_authz_dce"
	} ;


/*
 * These are stubs so we can
 * use the pgo_util file.
 */
void TSHMessage();

void
TSHMessage(ctx, flag, list) 
CtxBlock *ctx;
int flag;
char **list;
{ return; }

ndr_long_int
TSHStatus(ctx, level, ErrorString, AStatus, EStatus)
CtxBlock *ctx;
int level;
char *ErrorString;
unsigned long AStatus;
unsigned long EStatus;
{ return 0; }

    
int
do_account_prep (pgo_name, passwd, tstatus)
sec_rgy_name_t     pgo_name;
sec_passwd_str_t   passwd;
error_status_t     *tstatus;
{
   sec_passwd_rec_t   key;
   error_status_t	cstatus;

   if (log_in (ctx, &auth_id, CELL_ADMIN, ADMIN_PASSWD, tstatus)) {
      printf("\nsofserv: Failed to login: %d\n", *tstatus);
      return *tstatus;
   }
   printf("\nsofserv: Logged in as: %s\n", CELL_ADMIN);

   if (do_site_open (ctx, error_status_ok, &context, tstatus)) {
      printf("\nsofserv: Failed to open site: %d\n", *tstatus);
      log_out (ctx, &auth_id, &cstatus);
      return *tstatus;
   }

   /*
    * Add the client to the security data base.
    */
   if (add_account (ctx, &context, pgo_name, SERVER_UNIX_NUM, passwd,
   	ADMIN_PASSWD, tstatus)) {
      printf("\nsofserv: Failed to add account: %d\n", *tstatus);
      log_out (ctx, &auth_id, &cstatus);
      return *tstatus;
   }

   printf("\nsofserv: Added account: %s\n", pgo_name);

   /*  add key to key table */ 
   key.pepper = NULL;
   key.version_number = sec_passwd_c_version_none;
   key.key.key_type = sec_passwd_plain;
   key.key.tagged_union.plain = passwd;

   sec_key_mgmt_set_key(rpc_c_authn_dce_private, (void *)KEY_TAB,
   	pgo_name, 1, &key, tstatus);

   printf ("\nsofserv: Added key to keytab file: %d\n", *tstatus);

   log_out (ctx, &auth_id, &cstatus);

   return 0;
}

int
do_acct_cleanup (pgo_name, tstatus)
sec_rgy_name_t	 pgo_name;
error_status_t   *tstatus;
{  
   error_status_t	cstatus;

   /* remove the binding file , shouldn't be removed `Cause other client still need it*/
/*   unlink ((char *)BINDING_DATA_FNAME);    */

   /* log out as the server name first.  */

   log_out (ctx, &auth_id, &cstatus);

   if (log_in (ctx, &auth_id, CELL_ADMIN, ADMIN_PASSWD, tstatus)) {
      printf("\nsofserv: Failed to login: %d\n", *tstatus);
      return *tstatus;
   }

   if (do_site_open (ctx, error_status_ok, &context, tstatus)) {
      printf("\nsofserv: Failed to open site: %d.\n", *tstatus);
      log_out (ctx, &auth_id, &cstatus);
      return *tstatus;
   }

   /* remove server account */
   remove_acct (ctx, &context, pgo_name, tstatus);

   printf ("\nsofserv: Removed account: %s.\n", pgo_name);

   /* delete key from keytab file */
   sec_key_mgmt_delete_key (rpc_c_authn_dce_private, (void *)KEY_TAB,
   	pgo_name, 1, tstatus);

   printf ("\nsofserv: Removed key from keytab file\n");

   log_out (ctx, &auth_id, &cstatus);

   return 0;
}         

main (argc, argv)
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
   error_status_t    tstatus;		/* actual value returned from rpc */
   rpc_binding_vector_t *bindings;		/* rpc_server_inq_bindings */
   unsigned_char_p_t str_binding_p;	/* rpc_binding_to_string_binding */
   ndr_long_int fd;			/* open, write, close system calls */
   sec_rgy_name_t   	server_name; 
   sec_passwd_str_t	passwd; 
   sec_passwd_str_t	upasswd; 
   int			c;
   extern char 		*optarg;
   extern int 		optind; 
   short			argcount=0; 
   int 			i;

   /* Setup some pthread stuff. */

   	pthread_setasynccancel (CANCEL_OFF); 
  
   while((c = (int) getopt(argc, (char **)argv, "n:p:d")) != EOF) {
   	switch (c) {
   	   case 'd' : /* debug */
   		rpc__dbg_set_switches("0-4.10", &tstatus);
   		break;
   	   case 'n' : /* server name */
   		strcpy(server_name, optarg);
   		argcount += 2;
   		break;
   	   case 'p':  /* server password */
   		strcpy(passwd, optarg); 
   		argcount += 2;
   		break;  
   	}  
   }  
   if (argcount == 0) {
   	printf("Usage: sofserv -n server_name -p server_password [-d]\n"); 
   	exit(1);
   }

   /*
    * Add server to security data base.
    */
   do_account_prep(server_name, passwd, &tstatus); 

   if (log_in (ctx, &auth_id, server_name, passwd, &tstatus)) {
       printf("\nsofserv: Failed to login as %s: %d\n", server_name, tstatus);
       do_acct_cleanup(server_name, &tstatus);
       exit (tstatus);
   }

   printf ("\n\nsofserv: Logged in as %s.\n", server_name);

   printf ("\n\nsofserv: Calling rpc_server_use_all_protseqs\n");

   /*
    * Register all supported protocol
    * sequences with the RPC runtime.
    */
     
   rpc_server_use_all_protseqs (1, &tstatus);
   if (tstatus != rpc_s_ok) {
   	printf ("\nsofserv: rpc_server_use_all_protseqs  failed with error %d\n", tstatus);
   	do_acct_cleanup(server_name, &tstatus);
   	exit (tstatus);
   }

   /*
    * Register an interface with the rpc runtime.
    * The if_spec was obtained from test_rpc_auth.h
    * which was generated from test_rpc_auth.idl.
    */

   printf ("\nsofserv: Calling rpc_server_register_if\n");

   rpc_server_register_if (test_rpc_auth_v1_0_s_ifspec, (uuid_p_t *) NULL,
                           (rpc_mgr_epv_t) NULL, &tstatus);

   if (tstatus != rpc_s_ok) {
   	printf ("\nsofserv: rpc_server_register_if failed with error %d\n", tstatus);
   	rpc_binding_vector_free (&bindings, &tstatus); 
   	do_acct_cleanup(server_name, &tstatus);
   	exit (tstatus);
   }

   /*
    * Obtain this server's vector of binding handles.
    * and write them out to a file that is readable
    * by the client.
    */

   printf ("\nsofserv: Calling rpc_server_inq_bindings\n");

   rpc_server_inq_bindings (&bindings, &tstatus);

   if (tstatus != rpc_s_ok) {
   	printf ("\nsofserv: rpc_server_inq_bindings failed with error %d\n", tstatus);
   	do_acct_cleanup(server_name, &tstatus);
   	exit (tstatus);
   }
 



   /* Register endpoints specified by rpc_server_use_all_protseqs. */
   
   printf ("\nsofserv: Calling rpc_server_ep_register\n");

   rpc_ep_register (test_rpc_auth_v1_0_s_ifspec, bindings, (uuid_vector_t *) NULL,
   	(unsigned_char_p_t) "Test Server", &tstatus);

   if (tstatus != rpc_s_ok) {
   	printf ("\nsofserv: rpc_ep_register failed with error %d\n", tstatus);
   	rpc_binding_vector_free (&bindings, &tstatus);
   	do_acct_cleanup(server_name, &tstatus);
   	exit (tstatus);
   }

   /* Register for private key authentication. */

   printf ("\nsofserv: Calling rpc_server_register_auth_info\n");

   rpc_server_register_auth_info (server_name,
                                  rpc_c_authn_dce_private,
                                  (rpc_auth_key_retrieval_fn_t) NULL,
                                  (void *) NULL,
                                  &tstatus);
   if (tstatus != rpc_s_ok)
     {
       printf ("\nsofserv: rpc_server_register_auth_info failed with error %d\n", tstatus);
       do_acct_cleanup(server_name, &tstatus);
       rpc_ep_unregister (test_rpc_auth_v1_0_s_ifspec,
                          bindings,
                          (uuid_vector_t *) NULL,
                          &tstatus);
   	if (tstatus != rpc_s_ok) {
   		printf ("\nsofserv: rpc_ep_unregister failed with error %d\n", tstatus);
   		exit (tstatus);
   	}

   	rpc_binding_vector_free (&bindings, &tstatus);

   	if (tstatus != rpc_s_ok) {
   		printf ("\nsofserv: rpc_binding_vector_free failed with error %d\n",
   			tstatus);
   		exit (tstatus);
   	}
   	exit (tstatus);
   }

   /*
    * Start listening for client requests.
    * The only request this server will
    * catch is defined below.
    */

   printf ("\nsofserv: Calling rpc_server_listen\n");
         
   TRY
      rpc_server_listen (1, &tstatus);
                              
      CATCH_ALL
      do_acct_cleanup(server_name, &tstatus); 
      exit(1);                       

   ENDTRY

   /* Unregister endpoints before returning. */

   printf ("\nsofserv: Calling rpc_ep_unregister\n");

   rpc_ep_unregister (test_rpc_auth_v1_0_s_ifspec, bindings,
   	(uuid_vector_t *) NULL, &tstatus);

   /* Free the binding vector allocated by rpc_server_inq_bindings. */

   printf ("\nsofserv: Calling rpc_binding_vector_free\n");

   rpc_binding_vector_free (&bindings, &tstatus); 

   /* remove the account */
   do_acct_cleanup(server_name, &tstatus);                        

   printf ("\nsofserv: Exiting with status %d\n", tstatus);

   exit (tstatus);
}


/******************************************************************/
/*                                                                */
/* test_inq_auth_client:  This function is called from a client's */
/* entry point vector to test rpc_binding_inq_auth_client.        */
/* is tested.                                                     */
/*                                                                */
/******************************************************************/

void test_inq_auth_client 
(client_handle, subcase, server_name, i_authn_level, i_authn_svc, i_authz_svc, tstatus, cstatus)

/* Formal parameters */

   	handle_t		client_handle;
	ndr_long_int		subcase; 
	unsigned_char_p_t		server_name;
   	unsigned32		i_authn_level;
   	unsigned32		i_authn_svc;
   	unsigned32		i_authz_svc;
   	error_status_t	*tstatus;
   	error_status_t	*cstatus;
{

    /* Local variables */

    rpc_authz_handle_t	privs;
    unsigned_char_p_t	server_princ_name;
    unsigned32		authn_level=0;
    unsigned32		authn_svc=0;
    unsigned32		authz_svc=0;   

    error_status_t 	estatus;

    /* Call the rpc under test. */

    estatus = *tstatus; 

    if (estatus == rpc_s_invalid_binding)
        client_handle = NULL;   

    rpc_binding_inq_auth_client (client_handle,
                                 &privs,
                                 &server_princ_name,
                                 &authn_level,
                                 &authn_svc,
                                 &authz_svc,
                                 tstatus);

    printf ("\nsofserv: Calling rpc_binding_inq_auth_client: %d\n", *tstatus); 

    /* Verify the data returning from rpc_binding_inq_auth_client is correct. */
                                                      
    if (*tstatus == rpc_s_ok) { 
        if(server_princ_name != NULL){
            printf("sofserv: server name: %s\n", server_princ_name);  

            if (strstr(server_princ_name, server_name) != NULL) {
                printf("sofserv: principal name not match, (in): %s\n", server_name);
                *tstatus = TSH_DATA_MISSMATCH;
            }		
        }
    	printf("sofserv: protocol level: %d, %s\n", authn_level, prot_level_name[authn_level]); 
        if (i_authn_level == rpc_c_authn_level_default)
    	{
            rpc_mgmt_inq_dflt_authn_level (i_authn_svc, &i_authn_level, tstatus);
            if (*tstatus != rpc_s_ok)
            	return;
    	}
        if (i_authn_level != authn_level) {
            printf("sofserv: authentication protocol level not match, in: %d\n", i_authn_level);
            *tstatus = TSH_DATA_MISSMATCH;	
        }	
        printf("sofserv: authn service: %d, %s\n", authn_svc, authn_name[authn_svc]);
        if (i_authn_svc == rpc_c_authn_default) 
            i_authn_svc = rpc_c_authn_dce_private;
        if (i_authn_svc != authn_svc) {
            printf("sofserv: authentication service not match: %d\n", i_authn_svc);
            *tstatus = TSH_DATA_MISSMATCH;		
        }
    	printf("sofserv: authz service: %d, %s\n", authz_svc, authz_name[authz_svc]);
        if (i_authz_svc != authz_svc) {
            printf("sofserv: authorization not match: %d\n", i_authz_svc);
            *tstatus = TSH_DATA_MISSMATCH;		
        }  

        if (server_princ_name != NULL) {
            rpc_string_free (&server_princ_name, cstatus);		
            printf ("\nsofserv: rpc_string_free: %d\n", *cstatus); 
    	}

    } 
            
    if (subcase == VALID_DATA_NULL_SERVNAME && server_princ_name != NULL) { 
        printf ("ERROR: suppled NULL server name should return NULL server name as well\n");
        *tstatus = TSH_DATA_MISSMATCH;
    }



}                                       /* End test_inq_auth_client */

