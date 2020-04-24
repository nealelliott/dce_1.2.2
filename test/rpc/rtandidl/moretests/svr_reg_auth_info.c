/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: svr_reg_auth_info.c,v $
 * Revision 1.1.6.2  1996/02/17  23:22:33  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:21  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:11:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:32  root]
 * 
 * Revision 1.1.4.3  1993/01/11  23:41:38  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:15:01  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/04  20:51:34  cuti
 * 	Change the key_tyep to sec_passwd_plain to avoid DEC_parity error.
 * 	[1992/12/04  16:13:40  cuti]
 * 
 * Revision 1.1.2.4  1992/07/15  15:22:54  smith
 * 	removed dead code.
 * 
 * 	moved declaration of get_key_func to top of file to separate
 * 		declarations and code for easier reading.
 * 
 * 	declared a local error (cstatus) value for errors not necesarily
 * 		related to the completion of the test.
 * 
 * 	removed declaration of get_key_arg_ptr, it was incorrect.
 * 
 * 	added cleanup flag indicating that the security data
 * 		base should be returned to its original state.
 * 
 * 	added key declaration for the sec_key_mgmt_set_key() call.
 * 
 * 	changed get_key_arg to:
 * 
 * 		get_key_arg = (void *)"/krb5/v5srvtab";
 * 
 * 		when the test calls for it to be non-NULL.
 * 
 * 	moved the setting of get_key_arg_ptr and get_key_fn_p close
 * 		together because logically they are doing similar
 * 		operations and it makes the code easier to read.
 * 
 * 	removed the comments from the commented out case:
 * 		case AUTHN_SVC_DEFAULT:
 * 
 * 		No reason to be commented, if it is not called it
 * 		should be removed.
 * 
 * 	added logic to add the server to the security data base and
 * 		set a key for him in the v5srvtab file.  This is
 * 		necessary with certain combinations of input arguments.
 * 
 * 	changed the do_add_...  to log_in(), add_account(), log_out()
 * 		sequence.
 * 
 * 	changed the input argument get_key_arg_ptr to
 * 		rpc_server_register_auth_info to get_key_arg, it was wrong.
 * 
 * 	modified get_key_func to return error values on fails of mallocs.
 * 	[1992/07/15  15:17:19  smith]
 * 
 * Revision 1.1  1992/01/19  03:37:27  devrcs
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
 *   
 *	Extracted	1/4/92 16:54:00
 *	from delta on	11/8/91 16:55:05
 *
 *  $LOG$
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/rpc/rtandidl/moretests/svr_reg_auth_info.c, , dce_osf, dce.75d";
#endif

/* Include files */

#include <dce/rpcbase.h>
#include <tsh_auth.h> 
#include <base-defs.h>
#include <encryption.h>
#define main_code
#include <sec_rgy.h>
#include <dce/passwd.h>

#ifdef __STDC__
	void get_key_func (void *arg, unsigned_char_t *server_princ_name,
			   unsigned32 key_type, unsigned32 key_ver,
			   void **key, unsigned32 *status );
#else
	void get_key_func ();
#endif

/*  Args required by the test program.  */

#define ARGS_REQUIRED	6
static	sec_rgy_handle_t	context;
static  sec_login_handle_t      auth_id;

ndr_long_int TestFunction (ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
   ndr_long_int		subcase;	   /* argv[0], subcase to run */
   error_status_t	estatus;	   /* argv[1], expected return from test*/
   error_status_t	tstatus;	   /* actual value returned from rpc */
   error_status_t	cstatus;	   /* local error */
   sec_rgy_name_t	server_name;       /* name fo client to add to rgy */
   unsigned_char_t	*server_princ_name;/* rpc_server_register_auth_info */
   unsigned32		authn_svc;	   /* rpc_server_register_auth_info */
   char			*get_key_arg;	   /* rpc_server_register_auth_info */
   unsigned32		get_key_func_flag; /* argv[3] */
   unsigned32		get_key_arg_flag;  /* argv[4] */
   unsigned32		cleanup = 0;
   rpc_auth_key_retrieval_fn_t get_key_fn_p; /* rpc_server_register_auth_info */
   sec_passwd_rec_t   key;

   /*
    * Check the number of arguments passed in.
    * They should be exactly the   
    * required number when you come into this routine.
    */

   if (argc != ARGS_REQUIRED) {
      TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
			ARGS_REQUIRED, argc);
      return(TSH_BAD_ARG_COUNT);
   }

/* Obtain the input arguments from the .tsh command line. */

   subcase = atol(argv[0]);
   estatus = atol(argv[1]);
   authn_svc = atol(argv[2]);
   get_key_func_flag = atol(argv[3]);
   get_key_arg_flag = atol(argv[4]);
   strcpy(server_name, argv[5]);

/* Setup test parameters from command line input. */

   switch (authn_svc)
     {
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

   if (get_key_arg_flag == NULL_ARG_VALUE)
     get_key_arg = (void *)NULL;
   else
     get_key_arg = (void *)"/krb5/v5srvtab";

   if (get_key_func_flag == USE_GET_KEY_FUNC)
     get_key_fn_p = (rpc_auth_key_retrieval_fn_t) get_key_func;
   else
     get_key_fn_p = (rpc_auth_key_retrieval_fn_t) NULL;

   if ((get_key_func_flag != USE_GET_KEY_FUNC) &&
         /*
          * Only one supported now.
          * This should be changed when
          * more services are added.
          */
	 ((authn_svc == rpc_c_authn_dce_private) ||
          /*
           * If the authn_svc is rpc_c_authn_default and the
           * get_key_func_flag indicates a NULL value for
           * the authentication function, the "arg" parameter
           * is passed to the default authentication function
           * which expects a name of a file.  If this
           * file does not exist the test will fail.
           */
          ((authn_svc == rpc_c_authn_default) &&
           (get_key_arg_flag != NULL_ARG_VALUE)))) {

     /*
      * Need to add server to security data base
      * and need to add key to keytab file.
      * This is done because the get key function
      * becomes the default get key function
      * which requires a principal to be present
      * and a key to be present in the keytab file
      * for this principal.
      */

      if (log_in (ctx, &auth_id, CELL_ADMIN, ADMIN_PASSWD, &tstatus))
         return tstatus;

      if (do_site_open (ctx, error_status_ok, &context, &tstatus)) {
            log_out (ctx, &auth_id, &cstatus);
         return tstatus;
      }

      /*
       * Add the server to the rgy data base.
       */
      if (add_account (ctx, &context, server_name, SERVER_UNIX_NUM, ADMIN_PASSWD,
	   ADMIN_PASSWD, &tstatus)) {
         log_out (ctx, &auth_id, &cstatus);
         return tstatus;
      }

      key.key.key_type = sec_passwd_plain;
      key.key.tagged_union.plain = ADMIN_PASSWD;

      sec_key_mgmt_set_key(rpc_c_authn_dce_private, (void *)NULL, server_name,
           1, &key, &tstatus);

      /* If key exists then ignore error. */
      if (tstatus == sec_key_mgmt_e_key_version_ex)
         tstatus = error_status_ok;

      TSHMessage (ctx, MOTRACE,
           "sec_key_mgmt_set_key status: %d\n", tstatus);

      if (TSHStatus (ctx, MOTRACE,"sec_key_mgmt_set_key failed\n",
	   error_status_ok, tstatus)) {
	 remove_acct (ctx, &context, server_name, &cstatus);
         log_out (ctx, &auth_id, &cstatus);
         return tstatus;
      }

      log_out (ctx, &auth_id, &cstatus);

      cleanup = 1;
   }

/* Call the rpc under test. */

   server_princ_name = (unsigned_char_t *) &server_name;

   rpc_server_register_auth_info (server_princ_name, authn_svc, get_key_fn_p,
				  (ndr_void_p_t) get_key_arg, &tstatus);
   TSHMessage (ctx, MOTRACE,
	       "rpc_server_register_auth_info status: %d\n", tstatus);

   if (!TSHStatus (ctx, TEST, "rpc_server_register_auth_info failed\n",
	    	   estatus, tstatus))
	tstatus = error_status_ok;

   /*
    * Delete the server and remove his
    * key from the keytab file.
    */
   if (cleanup) {
      if (log_in (ctx, &auth_id, CELL_ADMIN, ADMIN_PASSWD, &cstatus))
         return tstatus;
      sec_key_mgmt_delete_key (rpc_c_authn_dce_private, (void *)NULL,
            server_name, 0, &key, &cstatus);
      TSHMessage (ctx, MOTRACE,
           "sec_key_mgmt_delete_key status: %d\n", cstatus);
      remove_acct (ctx, &context, server_name, &cstatus);
      log_out (ctx, &auth_id, &cstatus);
   }

   return (tstatus);
}
                       

void
get_key_func (arg, server_princ_name, key_type, key_ver, key, status)
void		*arg;
unsigned_char_t	*server_princ_name;
unsigned32      key_type;
unsigned32	key_ver;
void		**key;
unsigned32	*status;
{

   *status = rpc_s_ok;
              
   /*
    * In current implementation,
    * key has to be key data structure
    */ 
   *key = (void *) malloc (sizeof(krb5_keyblock));
   if (!*key) {
	*status = rpc_s_cant_nmalloc;
	return;
   }

   ((krb5_keyblock*)(*key))->contents = (char *)malloc(strlen(server_princ_name) + 1);
   if (!((krb5_keyblock*)(*key))->contents) {
	*status = rpc_s_cant_nmalloc;
	return;
   }

   ((krb5_keyblock*)(*key))->length = strlen (server_princ_name);

   strcpy(((krb5_keyblock*)(*key))->contents, server_princ_name);

   return;
}

