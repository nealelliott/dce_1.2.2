/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_acl.c,v $
 * Revision 1.1.10.2  1996/02/17  23:25:46  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:51  marty]
 *
 * Revision 1.1.10.1  1995/12/11  21:50:09  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/16  18:35 UTC  aha
 * 	sec_acl test change from master to node_name
 * 
 * 	HP revision /main/aha_mothra3/1  1995/03/16  17:59 UTC  aha
 * 	SERVER_NAME is no longer always <master>
 * 	[1995/12/11  20:52:59  root]
 * 
 * Revision 1.1.8.1  1994/06/02  21:14:43  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:21:42  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.6.1  1993/10/07  14:13:57  mccann
 * 	CR9056 pass address of acl_handle
 * 	[1993/10/07  14:12:15  mccann]
 * 
 * Revision 1.1.4.3  1993/01/29  21:12:33  cuti
 * 	Add sec_acl_bind(), sec_acl_get_error_info() and sec_acl_release_handle() test
 * 	[1993/01/28  16:06:06  cuti]
 * 
 * Revision 1.1.4.2  1992/12/31  17:32:59  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:10  htf]
 * 
 * Revision 1.1.2.4  1992/06/24  19:27:30  cuti
 * 	OT4337: Modified mask recalculation to exclude user_obj, other_obj, and unauthen.
 * 	[1992/06/24  19:18:47  cuti]
 * 
 * Revision 1.1.2.3  1992/05/20  22:06:33  sekhar
 * 	Removed do_site_open() since it is already defined
 * 	in pgo_util.c which is linked in when build sec_acl
 * 	[1992/05/20  22:05:19  sekhar]
 * 
 * Revision 1.1.2.2  1992/05/15  18:23:14  cuti
 * 	 Add more return data checking.
 * 	[1992/05/15  15:45:52  cuti]
 * 
 * 	 Add posix acl mask testing.
 * 	[1992/05/14  20:32:35  cuti]
 * 
 * Revision 1.1  1992/01/19  04:08:09  devrcs
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
 *  sec_acl.c
 */

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/sec_acl.c, , dce_osf, dce.75d";
#endif

/* Include files */
#define main_code    

#include <stdio.h> 
#include <sec_rgy.h>
#include <tsh_acl.h> 
#include <tsh_common.h> 
#include <dce/dce_error.h>

/* Args required by the test program. */

#define ARGS_REQUIRED 5 
dce_error_string_t	error_buf;
int status;
         
void construct_pac
#ifndef __STDC__                          
    (ctx, pac, st ) 
    CtxBlock                *ctx;
    sec_id_pac_t            **pac;              /* [out] */
    error_status_t          *st;
#else
    ( 
	CtxBlock                *ctx,
    sec_id_pac_t            **pac,
    error_status_t          *st
    )
#endif
{
    sec_rgy_name_t              cell_namep, princ_namep;
    uuid_t                      cell_uuid, princ_uuid;
    sec_rgy_login_name_t        login_name, ret_login_name;
    sec_rgy_cursor_t            cursor;
    sec_rgy_sid_t               sid;
    sec_rgy_unix_sid_t          unix_sid;
    sec_rgy_acct_key_t          key_parts; 
    sec_rgy_acct_user_t         user_part; 
    sec_rgy_acct_admin_t        admin_part;
    signed32                    num_returned, num_projects; 
    uuid_t                      *projlist;
    signed32                    *unix_projlist;
    signed32                    i;
    sec_rgy_handle_t  			rgy_context;
    static signed32             max_num = 10;
       
                        
	*st = rpc_s_ok;
    *pac = (sec_id_pac_t *)NULL;

    cell_namep[0] = '\0';
    princ_namep[0] = '\0';
                   
    do_site_open(ctx, sec_rgy_status_ok, &rgy_context, st); 
  
	sec_id_parse_name(rgy_context, "bin", cell_namep, &cell_uuid, princ_namep, &princ_uuid, st); 
    dce_error_inq_text(*st, error_buf, &status);
    TSHMessage (ctx, MOTRACE,
               "sec_id_parse_name status: %s\n", error_buf);
    if (TSHStatus (ctx, SETUP, "sec_id_parse_name failed", *st, rpc_s_ok))
		return;
  
    strcpy(login_name.pname, "bin");
    strcpy(login_name.gname, "bin");
    strcpy(login_name.oname, "none"); 
    sec_rgy_cursor_reset(&cursor); 

    sec_rgy_acct_lookup(rgy_context, &login_name, &cursor, &ret_login_name, 
                         &sid, &unix_sid, &key_parts,
                         &user_part, &admin_part, st);
    dce_error_inq_text(*st, error_buf, &status);
    TSHMessage (ctx, MOTRACE,
               "sec_rgy_acct_lookup status: %s\n", error_buf);
    if (TSHStatus (ctx, SETUP, "sec_rgy_acct_lookup failed", *st, rpc_s_ok))
		return;


    sec_rgy_cursor_reset(&cursor);
    projlist = (uuid_t *)malloc(max_num * sizeof(*projlist));
    if(projlist == NULL) {
	    TSHMessage (ctx, MOTRACE,
               "Cant allocate memory\n");
        *st = TSH_CANT_ALLOCATE_MEMORY;
        return;
    }
    unix_projlist = (signed32 *)malloc(max_num * sizeof(*unix_projlist));
    if(unix_projlist == NULL) {
	    TSHMessage (ctx, MOTRACE,
               "Cant allocate memory\n");
        *st = TSH_CANT_ALLOCATE_MEMORY;
        return;
    }
    sec_rgy_acct_get_projlist(rgy_context, &login_name, &cursor, max_num,
                           &num_returned, projlist, unix_projlist,
                           &num_projects, st);
    dce_error_inq_text(*st, error_buf, &status);
    TSHMessage (ctx, MOTRACE,
               "sec_rgy_acct_get_projlist status: %s\n", error_buf);
    while( *st == rpc_s_ok && (num_projects > max_num) ) {
        free(projlist);
        free(unix_projlist);
        max_num *= 2;
        sec_rgy_cursor_reset(&cursor);
        projlist = (uuid_t *)malloc(max_num * sizeof(*projlist));
        if(projlist == NULL) {
	    	TSHMessage (ctx, MOTRACE,
               "Cant allocate memory\n");
        	*st = TSH_CANT_ALLOCATE_MEMORY;
            return;
        }
        unix_projlist = (signed32 *)malloc(max_num * sizeof(*unix_projlist));
        if(unix_projlist == NULL) {
 	    	TSHMessage (ctx, MOTRACE,
               "Cant allocate memory\n");
        	*st = TSH_CANT_ALLOCATE_MEMORY;
            return;
        }
        sec_rgy_acct_get_projlist(rgy_context, &login_name, &cursor, max_num,
                               &num_returned, projlist, unix_projlist,
                               &num_projects, st);
    	dce_error_inq_text(*st, error_buf, &status);
	    TSHMessage (ctx, MOTRACE,
               "sec_rgy_acct_get_projlist status: %s\n", error_buf);

    }
    if(*st != rpc_s_ok) {
        free(projlist);
        free(unix_projlist);
        return;
    }
 
    /* Construct a PAC */
    (*pac) = (sec_id_pac_t *)malloc(sizeof(**pac));
    (*pac)->pac_type           = sec_id_pac_format_v1;
    (*pac)->authenticated      = true;
    (*pac)->realm.uuid         = cell_uuid;
    (*pac)->realm.name         = (unsigned_char_p_t)NULL;
    (*pac)->principal.uuid     = princ_uuid;
    (*pac)->principal.name     = (unsigned_char_p_t)NULL;
    (*pac)->group.uuid         = sid.group;
    (*pac)->group.name         = (unsigned_char_p_t)NULL;
    (*pac)->num_groups         = num_returned;
    (*pac)->num_foreign_groups = 0;
    if(num_returned > 0) {
        (*pac)->groups = 
            (sec_id_t *)malloc(num_returned * sizeof(*((*pac)->groups)));
        if((*pac)->groups == NULL) {
	    	TSHMessage (ctx, MOTRACE,
               "Cant allocate memory\n");
        	*st = TSH_CANT_ALLOCATE_MEMORY;
            return;
        }
        for( i = 0; i < num_returned; i++ ) {
            (*pac)->groups[i].uuid = projlist[i];
            (*pac)->groups[i].name = (unsigned_char_p_t)NULL;
        }
    }
    free(projlist);
    free(unix_projlist);
}







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
   error_status_t	lstatus;	/* local expected status */
   sec_login_handle_t	auth_identity;	/* sec_login_setup_identity */
   boolean32	        reset_passwd;	/* sec_login_validate_identity */
   sec_login_auth_src_t auth_src;	/* sec_login_validate_identity */
   unsigned_char_p_t	entry_name;	/* sec_acl_bind */
   boolean32		bind_to_entry;	/* sec_acl_bind */
   sec_acl_handle_t	acl_handle = NULL; /* sec_acl_bind */
   unsigned32		size_used;	/* sec_acl_get_manager_types */
   unsigned32		num_types;	/* sec_acl_get_manager_types */
   uuid_t		manager_types[SEC_ACL_MGR_TYPES_SIZE_AVAIL];
					/* sec_acl_get_manager_types */
   sec_acl_posix_semantics_t	sec_acl_posix_sems[SEC_ACL_MGR_TYPES_SIZE_AVAIL]; 
					/* sec_acl_get_manager_types_semantics */
   unsigned32		test_val;	/* sec_acl_get_manager_types */
   sec_acl_permset_t	net_rights;	/* sec_acl_get_access */
   uuid_t		manager_type;	/* sec_acl_get_printstring */
   uuid_t		mgr_type_chain;	/* sec_acl_get_printstring */
   sec_acl_printstring_t mgr_info;	/* sec_acl_get_printstring */
   boolean32		tokenize;	/* sec_acl_get_printstring */
   unsigned32		total_num_pstrs;/* sec_acl_get_printstring */
   sec_acl_printstring_t printstrings[SEC_ACL_PRINT_STR_SIZE_AVAIL];
					/* sec_acl_get_printstring */
   unsigned16		i, j;		/* sec_acl_get_printstring */
   unsigned16		num_entries; /* sec_acl_calc_mask */
   sec_acl_list_t	acl_list;	/* sec_acl_lookup */
   sec_acl_t		acl;		/* sec_acl_release */
   sec_acl_list_t	repl_acl;	/* sec_acl_replace */
   sec_acl_list_t	sav_acl;    /* to restore acl */
   sec_acl_permset_t	desired_permset;/* sec_acl_test_access */
   boolean32		access_result;	/* sec_acl_test_access */
   sec_id_pac_t		*pac;		/* sec_acl_test_access_on_behalf */
   rpc_authz_handle_t	privs;		/* rpc_binding_inq_auth_client */
   unsigned32		msg_type;	/* TSHStatus, TSHMessage */
   sec_rgy_pname_t	node_name;	/* gethostname */
   sec_rgy_pname_t	site_name;	/* sec_acl_bind */
   sec_rgy_pname_t	full_user_name;	/* sec_acl_bind */
   sec_passwd_rec_t      prec; 
   sec_rgy_bind_auth_info_t auth_info;  /* sec_rgy_site_bind_update */
   unsigned_char_t      *site;          /* sec_rgy_site_bind_update */
   sec_rgy_handle_t     new_context;    /* sec_rgy_site_bind_update */

	sec_acl_permset_t		old_perms, perms; 
	unsigned 		check;  



/* Check the number of arguments passed in.  They should be exactly the required
   number when you come into this routine. */
   
   if (argc != ARGS_REQUIRED )
     {
       TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n",
                  ARGS_REQUIRED, argc);
       return (TSH_BAD_ARG_COUNT);
     }

/* Obtain the input arguments from the .tsh command line. */

   subcase = atol((char *)argv[0]);
   estatus = atol((char *)argv[1]);

/* Log this client in. */

   sec_login_setup_identity ( (unsigned_char_p_t) argv[2],
/*************************** SEC_ACL_BIND_REALM, ********************/
                             (sec_login_flags_t)0,
                             &auth_identity,
                             &tstatus);
   dce_error_inq_text(tstatus, error_buf, &status);
   TSHMessage (ctx, MOTRACE,
               "sec_login_setup_identity status: %s\n",error_buf);

   if (TSHStatus (ctx, SETUP,
	  "sec_login_setup_identity failed\n", tstatus, rpc_s_ok))
     return (tstatus);
         
    prec.key.key_type = sec_passwd_plain;  
    prec.key.tagged_union.plain = (unsigned_char_p_t) argv[3];
    prec.pepper = NULL;
    prec.version_number = sec_passwd_c_version_none;

   (void) sec_login_validate_identity (auth_identity,
                                       &prec,
                                       &reset_passwd,
                                       &auth_src,
                                       &tstatus);
   dce_error_inq_text(tstatus, error_buf, &status);
   TSHMessage (ctx, MOTRACE,
               "sec_login_validate_identity status: %s\n", error_buf);

   if (TSHStatus (ctx, SETUP,
		  "sec_login_validate_identity failed\n", tstatus, rpc_s_ok))
     return (tstatus);

   sec_login_set_context (auth_identity, &tstatus);
   dce_error_inq_text(tstatus, error_buf, &status);
   TSHMessage (ctx, MOTRACE,
               "sec_login_set_context status: %s\n", error_buf);

   if (TSHStatus (ctx, SETUP,
		  "sec_login_set_context failed\n", tstatus, rpc_s_ok))
     {
       sec_login_purge_context (&auth_identity, &cstatus);
    	dce_error_inq_text(cstatus, error_buf, &status);
   		TSHMessage (ctx, MOTRACE,
               "sec_login_purge_context status: %s\n", error_buf);
      if (TSHStatus (ctx, CLNUP,
		      "sec_login_purge_context failed\n", cstatus, rpc_s_ok))
         return (cstatus);
       else
         return (tstatus);
     }


/* If this is the test of binding to an object in the global namespace,
   then set the bind_to_entry flag accordingly; otherwise, set the flag
   so that the bind will operate on the named object directly. */



/*      gethostname (node_name, sec_rgy_pname_t_size);
      sprintf ((char *)site_name, "ip:%s", node_name);
*/

      strcpy((char *) site_name, "/.:");

/*
*/
      entry_name=argv[4]; 
   {
          FILE  *foo;
          char  tmpstr[BUFSIZ];
          foo = fopen ("/opt/dcelocal/etc/security/pe_site","r");
          if (foo == NULL) {
             fprintf(stderr, "ERROR: Cannot open /opt/dcelocal/etc/security/pe_site\n");
             exit(1);
          }
          fscanf(foo, "%s %s\n", tmpstr, (char *)site_name);

          /* Get name of security master: /.../<cell>/subsys/dce/sec/<master_node> */
          auth_info.info_type = sec_rgy_bind_auth_none;
          auth_info.tagged_union.dce_info.authn_level= rpc_c_protect_level_pkt_integ;
          auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
          auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
          auth_info.tagged_union.dce_info.identity  = NULL;
          sec_rgy_site_bind_update((unsigned_char_t *)site, &auth_info, &new_context, &tstatus);
          if (tstatus != error_status_ok) {
             fprintf(stderr, "ERROR: Error from sec_rgy_site_bind_update\n");
             exit(1);
          }
          sec_rgy_site_get(new_context, &site, &tstatus);
          if (tstatus != error_status_ok) {
             fprintf(stderr, "ERROR: Error from sec_rgy_site_get\n");
             exit(1);
          }

          /* construct full user name from master registry and entry */
          sprintf((char *)full_user_name, "%s/%s", (char *)site, entry_name); 
          free(site);
          fclose(foo);
     }
    
    
    /* sec_acl_bind is called either for its own test, or as setup to the
       functions which require an acl_handle input argument. */
    
    /* Obtain the site name for the bind. */
    
    
       TSHMessage (ctx, MOTRACE, "site name: %s\n", site_name);
    
            
	if (subcase == SEC_ACL_BIND || subcase == SEC_ACL_BIND_GLOBAL_NAME_SP) {
 	   if (subcase == SEC_ACL_BIND_GLOBAL_NAME_SP)
     	 {
      	bind_to_entry = 1;
      	}
   		else
      	{
      	bind_to_entry = 0;
      	}
         
		if (estatus == sec_acl_object_not_found) {
			sec_acl_bind(entry_name, bind_to_entry, &acl_handle, &tstatus);
   			TSHMessage (ctx, MOTRACE, "entry name: %s\n", entry_name); 
		} 
		else {
    	/* Bind to the acl object without having to use the CDS. */
			sec_acl_bind(full_user_name, bind_to_entry, &acl_handle, &tstatus);
   			TSHMessage (ctx, MOTRACE, "entry name: %s\n", full_user_name);
        }
		
   		dce_error_inq_text(tstatus, error_buf, &status);
       	TSHMessage (ctx, MOTRACE, "sec_acl_bind  (bind_to_entry: %d) status: %d, %s\n", bind_to_entry, tstatus, error_buf);
	}
	else { /* do site_bind */ 
  		TSHMessage (ctx, MOTRACE, "entry name: %s\n", entry_name);
       sec_acl_bind_to_addr (site_name,
                           entry_name,
                           &acl_handle,
                           &tstatus);
    
   		dce_error_inq_text(tstatus, error_buf, &status);
       TSHMessage (ctx, MOTRACE, "sec_acl_bind_to_address status: %s\n", error_buf);
	} 
printf(" sec_acl_get_error_info: %u\n",  sec_acl_get_error_info(acl_handle));
   	dce_error_inq_text(sec_acl_get_error_info(acl_handle), error_buf, &status);
   	TSHMessage (ctx, MOTRACE, "Get error message from sec_acl_get_error_info of acl_handle: %s\n",  error_buf);
 


	
/* If this is a bind test case, set the test reporting variables thus;
   otherwise, set the test variables to report setup status. */

   if (subcase <= SEC_ACL_BIND_GLOBAL_NAME_SP)
     {
       lstatus = estatus;
       msg_type = MOFAILURE;
     }
   else
     {
       lstatus = error_status_ok;
       msg_type = SETUP;
     }

/* If the function returned something other than the expected status,
   post a failure message. */
   
   if (TSHStatus (ctx, msg_type,
		  "sec_acl_bind failed\n", lstatus, tstatus))
     if (tstatus == error_status_ok)
       tstatus = TSH_TEST_FAILED;
                 
   if (subcase == SEC_ACL_GET_MGR_TYPES_SEMANTICS ) {
      sec_acl_get_mgr_types_semantics (acl_handle,
                              SEC_ACL_TYPE,
                              SEC_ACL_MGR_TYPES_SIZE_AVAIL,
                              &size_used,
                              &num_types,
                              manager_types,
							  sec_acl_posix_sems,
			      			  &tstatus);
      
   		dce_error_inq_text(tstatus, error_buf, &status);
       TSHMessage (ctx, MOTRACE,
                   "sec_acl_get_mgr_types_semantics status: %s\n", error_buf); 
   		dce_error_inq_text(sec_acl_get_error_info(acl_handle), error_buf, &status);
   		TSHMessage (ctx, MOTRACE, "Get error message from sec_acl_get_error_info of acl_handle: %s\n",  error_buf);
                     
		for (i=0; i<size_used; i++)  {
         TSHMessage (ctx, MOTRACE,
         "sec_acl_get_mgr_types_semantics return posix acl: %d\n", sec_acl_posix_sems[i]); 
		}    

	   return (tstatus);

	}

/* sec_acl_get_manager_types is called for its own test or as setup for
   those functions which take a manager_type input argument.  Note that
   for the sec_acl_unknown_manager_type tests, this function is skipped. */

   if ((estatus != sec_acl_unknown_manager_type) &&
       (tstatus == error_status_ok))
     {
       sec_acl_get_manager_types (acl_handle,
                              SEC_ACL_TYPE,
                              SEC_ACL_MGR_TYPES_SIZE_AVAIL,
                              &size_used,
                              &num_types,
                              manager_types,
			      &tstatus);
      
   		dce_error_inq_text(tstatus, error_buf, &status);
       TSHMessage (ctx, MOTRACE,
                   "sec_acl_get_manager_types status: %s\n", error_buf); 
	       
    /* If the test succeeded, then the size_used argument must be the minimum 
       of num_types and size_avail.  If it is not, signal a test failure even
       if the function reported successful completion. */

       if (tstatus == error_status_ok)
         {
           test_val = 
            (num_types < SEC_ACL_MGR_TYPES_SIZE_AVAIL) ?
                                   num_types : SEC_ACL_MGR_TYPES_SIZE_AVAIL;

           if (size_used != test_val)
             tstatus = TSH_TEST_FAILED;
         }

    /* If this is a get_manager_types test case, set the test reporting
       variables thus; otherwise, set the test variables to report setup 
       status. */

       if (subcase == SEC_ACL_GET_MGR_TYPES)
         {
           lstatus = estatus;
           msg_type = MOFAILURE;
         }

       else
         {
           lstatus = error_status_ok;
           msg_type = SETUP;
         }

    /* If the the function returned something other than the expected status,
       post a failure message. */
    
       if (TSHStatus (ctx, msg_type,
		      "sec_acl_get_manager_types failed\n", lstatus, tstatus))
         if (tstatus == error_status_ok)
           tstatus = TSH_TEST_FAILED;

    /* Obtain a copy of one of the defined manager types for use in
       functions which require a manager_type input argument. */

       if (tstatus == error_status_ok)
         memcpy ((char *)&manager_type,
	         (char *)&manager_types[0],
	         sizeof (uuid_t));

     } /* End if (estatus != sec_acl_unknown_manager_type) */

   else
     uuid_create (&manager_type, &lstatus);

/* sec_acl_get_access is only called for its own test. */

   if ((subcase == SEC_ACL_GET_ACCESS) &&
       (tstatus == error_status_ok))
     {
       sec_acl_get_access (acl_handle,
                           &manager_type,
                           &net_rights,
                           &tstatus);

   		dce_error_inq_text(tstatus, error_buf, &status);
       TSHMessage (ctx, MOTRACE,
                   "sec_acl_get_access status: %s\n", error_buf);

    /* If the the function returned something other than the expected status,
       post a failure message. */

       if (TSHStatus (ctx, MOFAILURE,
		      "sec_acl_get_access failed\n", tstatus, estatus))
         if (tstatus == error_status_ok)
           tstatus = TSH_TEST_FAILED;

     } /* End if (subcase == SEC_ACL_GET_ACCESS) */

/* sec_acl_get_printstring is only called for its own test. */

   if ((subcase == SEC_ACL_GET_PRINTSTRING) &&
       (tstatus == error_status_ok))
     {
    /* Initialize the printstrings structure for the test. */

       bzero ((char *)printstrings, sizeof (printstrings)); 

       sec_acl_get_printstring (acl_handle,
				&manager_type,
				SEC_ACL_PRINT_STR_SIZE_AVAIL,
				&mgr_type_chain,
				&mgr_info,
				&tokenize,
				&total_num_pstrs,
				&size_used,
				printstrings,
				&tstatus);

   		dce_error_inq_text(tstatus, error_buf, &status);
       TSHMessage (ctx, MOTRACE,
                   "sec_acl_get_printstring status: %s\n", error_buf);

    /* If the test status did not match the expected status, 
        signal a test failure; otherwise,
       display the printstring output as a trace. */

       if (estatus != tstatus) 
	 {      
           TSHMessage (ctx, MOFAILURE, "sec_acl_get_printstring failed\n");
           if (tstatus == error_status_ok)
             tstatus = TSH_TEST_FAILED;
         }

       else
         {    
			if (tstatus  == error_status_ok) {
           TSHMessage (ctx, MOTRACE,
                       "total num printstrings: %d\n", total_num_pstrs);
           for (i=0; i<total_num_pstrs; i++)
              {
                TSHMessage (ctx, MOTRACE,
                   "printstring [%d]: %s\n", i, printstrings[i].printstring);
                TSHMessage (ctx, MOTRACE,
                   "helpstring [%d]: %s\n", i, printstrings[i].helpstring);
              } 
			}
         }
                
     } /* End if (subcase == SEC_ACL_GET_PRINTSTRING) */
 
/* sec_acl_lookup is called for its own test, as verification for the
   sec_acl_replace test, or as setup for the sec_acl_release test. */

   if (((subcase == SEC_ACL_LOOKUP)  ||
		(subcase == SEC_ACL_CALC_MASK) ||
        (subcase == SEC_ACL_RELEASE) ||
        (subcase == SEC_ACL_RELEASE) ||
        (subcase == SEC_ACL_RELEASE_NULL) ||
	(subcase == SEC_ACL_REPLACE)) &&
       (tstatus == error_status_ok))
     {
       sec_acl_lookup (acl_handle,
                       &manager_type,
                       SEC_ACL_TYPE,
                       &acl_list,
                       &tstatus);

  		 dce_error_inq_text(tstatus, error_buf, &status);
       TSHMessage (ctx, MOTRACE,
                   "sec_acl_lookup status: %s\n", error_buf);

    /* If this is a lookup test case, set the test reporting variables
       thus; otherwise, set the test variables to report setup status. */

       if (subcase == SEC_ACL_LOOKUP || estatus == sec_acl_unknown_manager_type)
         {
           lstatus = estatus;
           msg_type = MOFAILURE;
         }

       else
         {
           lstatus = error_status_ok;
           msg_type = SETUP;
         }

    /* If the the function returned something other than the expected status,
       post a failure message. */

       if (TSHStatus (ctx, msg_type,
		      "sec_acl_lookup failed\n", tstatus, lstatus))
         if (tstatus == error_status_ok)
           tstatus = TSH_TEST_FAILED;
                                      

 /*      if ((subcase == SEC_ACL_RELEASE) &&
	   (tstatus == error_status_ok))
	 memcpy ((char *)&acl,
		 (char *)&acl_list.sec_acls[0], 
		 sizeof (sec_acl_t));      */


    /* Obtain the acl to do the replacement acl for the
       sec_acl_replace test. */
                                    
       if (subcase == SEC_ACL_CALC_MASK) { 
			sec_acl_calc_mask(&acl_list, &tstatus); 
   			dce_error_inq_text(tstatus, error_buf, &status);
       		TSHMessage (ctx, MOTRACE,
                   "sec_acl_calc_mask status: %s\n", error_buf);  
 

		for (j=0; j<acl_list.num_acls; j++)  {
			check = -1; 
			perms = 0; 
			for (i=0; i<acl_list.sec_acls[j]->num_entries; i++) { 
    		    TSHMessage (ctx, MOINFO,
                       "sec_acl_calc_mask retunred perms[%d]: %x\n", i, acl_list.sec_acls[j]->sec_acl_entries[i].perms);  
    			switch (acl_list.sec_acls[j]->sec_acl_entries[i].entry_info.entry_type) {
                	case sec_acl_e_type_mask_obj:
               			check = i;
    			    	TSHMessage (ctx, MOINFO,
                       		"sec_acl_calc_mask retunred new entry type: %d for entry (%d)\n", 
    						acl_list.sec_acls[j]->sec_acl_entries[i].entry_info.entry_type, i); 
                		break;
    				case sec_acl_e_type_group_obj:
                    case sec_acl_e_type_any_other:
                    case sec_acl_e_type_user:
                    case sec_acl_e_type_group:
                    case sec_acl_e_type_foreign_other:
                    case sec_acl_e_type_foreign_user:
                    case sec_acl_e_type_foreign_group:
                    case sec_acl_e_type_extended:
    			    	perms = perms | acl_list.sec_acls[0]->sec_acl_entries[i].perms;
      					break;
    				default:
    					break;        
				}  
			}
           	if (check == 0)  {
    			tstatus = TSH_DATA_MISSMATCH;
         		TSHStatus (ctx, MOFAILURE,
    		      "sec_acl_calc_mask failed, no one returned entry type is e_type_mask. \n", 
    				1, 0);   
    		return;
    		} 
       		else {
    		    TSHMessage (ctx, MOINFO,
                       "sec_acl_calc_mask expected new perms: %x\n", perms);  
    			if (TSHStatus (ctx, MOFAILURE,  "sec_acl_calc_mask failed, returned permset is wrong. \n", perms, acl_list.sec_acls[j]->sec_acl_entries[check].perms))
    			tstatus = TSH_DATA_MISSMATCH;
            } 
		}

	 }/* end of calc_mask */
		
       if ((subcase == SEC_ACL_REPLACE) &&
	   (tstatus == error_status_ok))
         {  /* replace other_obj's acl from r right to rc(read and control) right */
		old_perms = acl_list.sec_acls[0]->sec_acl_entries[1].perms;
		acl_list.sec_acls[0]->sec_acl_entries[1].perms = sec_acl_perm_read |
							sec_acl_perm_owner; 

	 	}

     } /* End if (subcase == SEC_ACL_LOOKUP) */



/* sec_acl_replace is only called for its own test. */

   if ((subcase == SEC_ACL_REPLACE) &&
       (tstatus == error_status_ok))
     {
    /* Setup the replacement acl. */

       sec_acl_replace (acl_handle,
                        &manager_type,
                        SEC_ACL_TYPE,
                        &acl_list,
                        &tstatus);

    	dce_error_inq_text(tstatus, error_buf, &status);
      TSHMessage (ctx, MOTRACE,
                   "sec_acl_replace status: %s\n", error_buf);
	                      
       if (TSHStatus (ctx, MOFAILURE,
		      "sec_acl_replace failed\n", tstatus, estatus))
		return;	

    /* Verify that the acl was replaced.  If it was not, signal a test
       failure even if sec_acl_replace reported successful completion. */
                                  
		if (tstatus == rpc_s_ok) {
       		sec_acl_lookup (acl_handle,
                       &manager_type,
                       SEC_ACL_TYPE,
                       &repl_acl,
                       &tstatus);

   			dce_error_inq_text(tstatus, error_buf, &status);
       		TSHMessage (ctx, MOTRACE,
                   "sec_acl_lookup status: %s\n", error_buf);
  
			if (acl_list.sec_acls[0]->sec_acl_entries[1].perms !=
		    	repl_acl.sec_acls[0]->sec_acl_entries[1].perms) {
				TSHMessage(ctx, MOTRACE, "Returnd data is different from what was set.\n");
	        	tstatus = TSH_TEST_FAILED;
	   		}
         
	   /* restore the original acl */   
    	   acl_list.sec_acls[0]->sec_acl_entries[1].perms = old_perms;
    
           sec_acl_replace (acl_handle,
                            &manager_type,
                            SEC_ACL_TYPE,
                            &acl_list,
                            &tstatus);
    		dce_error_inq_text(tstatus, error_buf, &status);
          TSHMessage (ctx, MOTRACE,
                       "sec_acl_replace status: %s\n", error_buf);
    	                      
           if (TSHStatus (ctx, MOFAILURE,
    		      "sec_acl_replace failed\n", tstatus, rpc_s_ok))
    		return;	
		} /* if replace is ok */
    
		if (tstatus != rpc_s_ok)
			return;

     } /* End if (subcase == SEC_ACL_REPLACE) */

/* sec_acl_release is only called for its own test. */

   if ((subcase == SEC_ACL_RELEASE) &&
       (tstatus == error_status_ok))
     {        
	for (i = 0; i < acl_list.num_acls; i++)  {
       sec_acl_release (acl_handle,
                        acl_list.sec_acls[i],
                        &tstatus);

   		dce_error_inq_text(tstatus, error_buf, &status);
       TSHMessage (ctx, MOTRACE,
                   "sec_acl_release status: %s\n", error_buf);  
	}
                
    if ((subcase == SEC_ACL_RELEASE_NULL) &&
	   (tstatus == error_status_ok)) {
	 memcpy ((char *)&acl,
		 (char *)&acl_list.sec_acls[0], 
		 sizeof (sec_acl_t));      

       sec_acl_release (acl_handle,
                        &acl,
                        &tstatus); 
	   dce_error_inq_text(tstatus, error_buf, &status);
       TSHMessage (ctx, MOTRACE,
                   "sec_acl_release status: %s\n", error_buf);  

	}


    /* Verify that the acl was released.  If it was not, signal a test
       failure even if sec_acl_release reported successful completion. */

       if ((acl.num_entries != 0) &&
           (tstatus == error_status_ok))
	 tstatus == TSH_TEST_FAILED;

    /* If the the function returned something other than the expected status,
       post a failure message. */

       if (TSHStatus (ctx, MOFAILURE,
		      "sec_acl_release failed\n", tstatus, estatus))
         if (tstatus == error_status_ok)
           tstatus = TSH_TEST_FAILED;

     } /* End if (subcase == SEC_ACL_RELEASE) */

/* sec_acl_test_access is only called for its own test. */

   if (((subcase == SEC_ACL_TEST_ACCESS_TRUE) ||
        (subcase == SEC_ACL_TEST_ACCESS_FALSE)) &&
       (tstatus == error_status_ok))
     {
       desired_permset = sec_acl_perm_read;

       if (subcase == SEC_ACL_TEST_ACCESS_FALSE)
	 desired_permset |= sec_acl_perm_write |
	                    sec_acl_perm_execute |
	                    sec_acl_perm_owner |
	                    sec_acl_perm_insert |
	                    sec_acl_perm_delete;

       access_result = sec_acl_test_access (acl_handle,
                                            &manager_type,
                                            desired_permset,
                                            &tstatus);
	   dce_error_inq_text(tstatus, error_buf, &status);
       TSHMessage (ctx, MOTRACE,
                   "sec_acl_test_access status: %s\n", error_buf);

    /* Verify that the correct value was returned.  If it was not, signal a test
       failure even if sec_acl_test_access reported successful completion. */

       if (tstatus == error_status_ok)
	 {
           switch (access_result)
	     {
	       case 0:
                 if (subcase == SEC_ACL_TEST_ACCESS_TRUE)
                   tstatus = TSH_TEST_FAILED;
                 break;

	       case 1:
                 if (subcase == SEC_ACL_TEST_ACCESS_FALSE)
                   tstatus = TSH_TEST_FAILED;
                 break;
              
	       default:
                 tstatus = TSH_TEST_FAILED;
                 break;

	     } /* End switch (access_result) */

	 } /* End if (tstatus == error_status_ok) */

      /* If the the function returned something other than the expected status,
         post a failure message. */

	 if (TSHStatus (ctx, MOFAILURE,
			"sec_acl_test_access failed\n", estatus, tstatus))
           if (tstatus == error_status_ok)
             tstatus = TSH_TEST_FAILED;

     } /* End if (subcase == SEC_ACL_TEST_ACCESS) */


/* sec_acl_test_access_on_behalf is only called for its own test. */

   if ((subcase == SEC_ACL_TEST_ACCESS_BEHALF_FALSE) &&
       (tstatus == error_status_ok)) {            

        construct_pac(ctx, &pac, &tstatus);
        desired_permset = sec_acl_perm_read;

        if (subcase == SEC_ACL_TEST_ACCESS_BEHALF_FALSE)
             desired_permset |= sec_acl_perm_write ||
	                        sec_acl_perm_execute ||
	                        sec_acl_perm_owner ||
	                        sec_acl_perm_insert ||
	                        sec_acl_perm_delete;

        access_result = sec_acl_test_access_on_behalf (acl_handle,
            &manager_type, pac, desired_permset, &tstatus);
        dce_error_inq_text(tstatus, error_buf, &status);
        TSHMessage (ctx, MOTRACE,
            "sec_acl_test_access_on_behalf status: %s\n", error_buf);

        /* If the the function returned something other than the expected
         * status, post a failure message.
         */

        if (TSHStatus (ctx, MOFAILURE,
            "sec_acl_test_access_on_behalf failed\n", tstatus, estatus))
                if (tstatus == error_status_ok)
                    tstatus = TSH_TEST_FAILED;

     } /* End if (subcase == SEC_ACL_TEST_ACCESS_BEHALF_FALSE) */

/* If the expected return from the test matched the actual return, then
   signal test success. */

   if (estatus == tstatus)
     tstatus = error_status_ok;

/* Purge the login context. */

   sec_login_purge_context (&auth_identity, &cstatus);
   dce_error_inq_text(cstatus, error_buf, &status);
   TSHMessage (ctx, MOTRACE,
                       "sec_login_purge_context status: %s\n", error_buf);

   if (TSHStatus (ctx, CLNUP,
		  "sec_login_purge_context failed\n", error_status_ok, cstatus))
     tstatus = cstatus;
    
/* release acl handle */ 
	if (estatus != sec_acl_object_not_found) {          
    	sec_acl_release_handle(&acl_handle, &cstatus);
       	dce_error_inq_text(cstatus, error_buf, &status);
       	TSHMessage (ctx, MOTRACE,
                           "sec_acl_release_handle status: %s\n", error_buf);
    
       if (TSHStatus (ctx, CLNUP,
    		  "sec_acl_release_handle failed\n", error_status_ok, cstatus))
         tstatus = cstatus;  
	}

/* Exit this routine. */

   return (tstatus);

} /* End sec_acl */

