/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: site_bind.c,v $
 * Revision 1.1.6.2  1996/02/17  23:25:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:54  marty]
 *
 * Revision 1.1.6.1  1995/12/11  21:50:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:04  root]
 * 
 * Revision 1.1.4.3  1993/02/17  21:04:46  cuti
 * 	Initialize auth_info.info_type before calling sec_rgy_site_bind.
 * 	[1993/02/12  20:23:05  cuti]
 * 
 * Revision 1.1.4.2  1992/12/31  17:33:18  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:26  htf]
 * 
 * Revision 1.1.2.2  1992/05/12  20:35:55  cuti
 * 	CR2889: Changed constant password to a variable, so sec lib can write to it.
 * 	[1992/05/12  13:37:31  cuti]
 * 
 * Revision 1.1  1992/01/19  04:08:34  devrcs
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
 *	Extracted	1/4/92 17:06:04
 *	from delta on	12/4/91 15:26:39
 *
 *  $LOG$
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/site_bind.c, , dce_osf, dce.75d";
#endif

/* Include files */

#include "tsh_site.h"


/* Args required by the test program. */

#define ARGS_REQUIRED 2

ndr_long_int TestFunction (ctx, argc, argv)

/* Formal parameters */

   CtxBlock          *ctx;
   ndr_long_int      argc;
   unsigned_char_p_t argv[];

{

/* Local variables */

   ndr_long_int		subcase;	/* argv[0], subcase to run */
   error_status_t	estatus;	/* argv[1], expected return from test */
   error_status_t	tstatus;	/* test status */
   error_status_t	cstatus;	/* cleanup status */
   sec_rgy_pname_t	node_name;	/* gethostname */
   sec_login_handle_t	auth_identity;	/* sec_login_setup_identity */
   boolean32	        reset_passwd;	/* sec_login_validate_identity */
   sec_login_auth_src_t auth_src;	/* sec_login_validate_identity */
   sec_rgy_bind_auth_info_t auth_info;	/* sec_rgy_site_bind */
   sec_rgy_pname_t	site_name;	/* sec_rgy_site_bind */
   sec_passwd_str_t passwd;
   sec_rgy_handle_t	context;	/* sec_rgy_site_bind */
   unsigned_char_t	*get_cell_name;/* sec_rgy_site_binding_get_info */
   unsigned_char_t	*get_serv_name;/* sec_rgy_site_binding_get_info */
   unsigned_char_t	*get_strg_bind;/* sec_rgy_site_binding_get_info */
   sec_rgy_bind_auth_info_t get_auth_info;/* sec_rgy_site_binding_get_info */
   unsigned_char_t	**get_cell_name_p=&get_cell_name;/* sec_rgy_site_binding_get_info pointer */
   unsigned_char_t	**get_serv_name_p=&get_serv_name;/* sec_rgy_site_binding_get_info poiner*/
   unsigned_char_t	**get_strg_bind_p=&get_strg_bind;/* sec_rgy_site_binding_get_info pointer*/
   sec_rgy_bind_auth_info_t *get_auth_info_p=&get_auth_info;/* sec_rgy_site_binding_get_info pointer*/
   unsigned32		msg_type;	/* TSHStatus, TSHMessage */
   boolean32	        is_readonly;	/* sec_rgy_site_is_readonly */
   sec_passwd_rec_t      prec;

/* Check the number of arguments passed in.  They should be exactly the required
   number when you come into this routine. */
   
   if (argc != ARGS_REQUIRED)
     {
       TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n",
                  ARGS_REQUIRED, argc);
       return (TSH_BAD_ARG_COUNT);
     }

/* Obtain the input arguments from the .tsh command line. */

   subcase = atol(argv[0]);
   estatus = atol(argv[1]);

/* Log this client in. */

   sec_login_setup_identity (SITE_BIND_USERNAME,
/*************************** SITE_BIND_REALM, ********************/
                             (sec_login_flags_t)0,
                             &auth_identity,
                             &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_login_setup_identity status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
		  "sec_login_setup_identity failed", tstatus, rpc_s_ok))
     return (tstatus);
               
    prec.key.key_type = sec_passwd_plain;  
	strcpy(passwd, SITE_BIND_PASSWD);
    prec.key.tagged_union.plain = &passwd;
    prec.pepper = NULL;
    prec.version_number = sec_passwd_c_version_none;

   (void) sec_login_validate_identity (auth_identity,
                                       &prec,
                                       &reset_passwd,
                                       &auth_src,
                                       &tstatus);
   TSHMessage (ctx, MOTRACE,
               "sec_login_validate_identity status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
		  "sec_login_validate_identity failed", tstatus, rpc_s_ok))
     return (tstatus);

   sec_login_set_context (auth_identity, &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_login_set_context status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
		  "sec_login_set_context failed", tstatus, rpc_s_ok))
     {
       sec_login_purge_context (&auth_identity, &cstatus);
       if (TSHStatus (ctx, CLNUP,
		      "sec_login_purge_context failed", cstatus, rpc_s_ok))
         return (cstatus);
       else
         return (tstatus);
     }

/* Setup the auth_info argument. */
   auth_info.info_type = sec_rgy_bind_auth_dce;
   auth_info.tagged_union.dce_info.authn_level = rpc_c_authn_level_call;
   auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_private;
   auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
   auth_info.tagged_union.dce_info.identity = auth_identity;

/* Setup the site_name argument. */

/*   if (subcase == SITE_BIND_DEFAULT || subcase == SITE_BIND_UPDATE_DEFAULT )
     bzero ((char *)site_name, sizeof (sec_rgy_pname_t));

   else
     {
       gethostname (node_name, sec_rgy_pname_t_size);
       sprintf ((char *)site_name, "ip:%s", node_name);
     }
*/
     strcpy( (char *) site_name, "/.:");


/* Bind to a registry server. */

   switch (subcase)
     {
/*       case SITE_BIND_GET_INFO:
       case SITE_BIND:
       case SITE_BIND_DEFAULT: */
	default:
	   sec_rgy_site_bind (site_name,
			      &auth_info,
                              &context,
                              &tstatus);

           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_bind status: %d\n", tstatus);

           if (subcase = SITE_BIND_GET_INFO)
             msg_type = MOTRACE;
           else
             msg_type = MOFAILURE;
	     
           if (TSHStatus (ctx, msg_type,
                  "sec_rgy_site_bind failed\n", rpc_s_ok, tstatus))
             return (tstatus);

           break;

       case SITE_BIND_QUERY:
	   sec_rgy_site_bind_query (site_name,
			            &auth_info,
                                    &context,
                                    &tstatus);

           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_bind_query status: %d\n", tstatus);

           if (TSHStatus (ctx, MOFAILURE,
                  "sec_rgy_site_bind_query failed\n", rpc_s_ok, tstatus))
             return (tstatus);

           break;

       case SITE_BIND_UPDATE:
       case SITE_BIND_UPDATE_DEFAULT:
	   sec_rgy_site_bind_update (site_name,
			             &auth_info,
                                     &context,
                                     &tstatus);
           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_bind_update status: %d\n", tstatus);

           if (TSHStatus (ctx, MOFAILURE,
                  "sec_rgy_site_bind_update failed\n", rpc_s_ok, tstatus))
             return (tstatus);

           break;

     } /* End switch (subcase) */


/* sec_rgy_site_binding_get_info is called for its own test or
   to verify the bind. */

   switch (subcase)
     {
       case SITE_BIND_GET_INFO_NULL_CELL_NAME:
         get_cell_name_p = NULL;
         break;

       case SITE_BIND_GET_INFO_NULL_SERV_NAME:
         get_serv_name_p = NULL;
         break;

       case SITE_BIND_GET_INFO_NULL_STRG_BIND:
         get_strg_bind_p = NULL;
         break;

       case SITE_BIND_GET_INFO_NULL_AUTH_INFO:
         get_auth_info_p = NULL;
         break;
     }

   sec_rgy_site_binding_get_info (context,
                                  get_cell_name_p,
                                  get_serv_name_p,
                                  get_strg_bind_p,
                                  get_auth_info_p,
                                  &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_site_binding_get_info status: %d\n", tstatus);

   if (estatus == tstatus)
     {
       if (tstatus == sec_rgy_status_ok)
	 {
	   if (subcase != SITE_BIND_GET_INFO_NULL_CELL_NAME)
	     free (get_cell_name);
	   if (subcase != SITE_BIND_GET_INFO_NULL_SERV_NAME)
	     free (get_serv_name);
	   if (subcase != SITE_BIND_GET_INFO_NULL_STRG_BIND)
	     free (get_strg_bind);
          }
        else
          tstatus = sec_rgy_status_ok;

     } /* End if (estatus == tstatus) */

   else
     {
       if ((tstatus == sec_rgy_not_implemented) &&
           (subcase != SITE_BIND_GET_INFO))
         tstatus = sec_rgy_status_ok;
       else
         TSHMessage (ctx, MOFAILURE,
                     "sec_rgy_site_binding_get_info failed\n");
     }

/* Close the registry site. */

   sec_rgy_site_close (context, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_site_close status: %d\n", cstatus);
    
   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

/* Purge the login context. */

   sec_login_purge_context (&auth_identity, &cstatus);

   if (TSHStatus(ctx, CLNUP, "sec_login_purge_context failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

/* Exit this routine. */

   return (tstatus);

} /* End site_bind */

