/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: auth_policy.c,v $
 * Revision 1.1.13.3  1996/02/17  23:25:15  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:25  marty]
 *
 * Revision 1.1.13.2  1995/12/11  21:48:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:08  root]
 * 
 * Revision 1.1.9.1  1994/10/25  17:18:00  sekhar
 * 	[ OT 12398 ]  Checking in Sean Mullan's changes for testing
 * 		      security replication.
 * 	[1994/10/24  17:00:40  sekhar]
 * 
 * Revision 1.1.6.1  1993/10/07  18:58:59  mccann
 * 	CR9057 remove unnecessary verison requirement
 * 	[1993/10/07  18:57:22  mccann]
 * 
 * Revision 1.1.4.3  1993/02/25  22:05:51  cuti
 * 	Change sec_rgy_site_open() to sec_rgy_site_open_update() for using site update operation.
 * 	[1993/02/25  18:59:02  cuti]
 * 
 * Revision 1.1.4.2  1992/12/31  17:28:30  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:53:57  htf]
 * 
 * Revision 1.1.2.2  1992/05/12  15:52:29  ahop
 * 	 CR 3104: reset RGY auth_policy to original values at end of test
 * 	[1992/05/12  02:37:19  ahop]
 * 
 * Revision 1.1  1992/01/19  04:08:27  devrcs
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
 *	Extracted	1/4/92 17:06:02
 *	from delta on	12/4/91 15:25:39
 *
 *  $LOG$
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/auth_policy.c, , dce_osf, dce.75d";
#endif

/* Include files */

#include <tsh_auth_pol.h>
#include <sec_rgy_bind.h>

#define min(a,b) ( (a > b) ? b:a )

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
   sec_rgy_handle_t	write_context;	/* sec_rgy_site_open_update */
   sec_rgy_handle_t	read_context;	/* sec_rgy_site_open_query */
   sec_rgy_pgo_item_t	pgo_item;	/* sec_rgy_pgo_add */
   sec_rgy_acct_key_t	key_parts;	/* sec_rgy_acct_add */
   sec_rgy_acct_user_t  user_part;	/* sec_rgy_acct_add */
   sec_rgy_acct_admin_t admin_part;	/* sec_rgy_acct_add */
   sec_passwd_version_t	passwd_ver_no;	/* sec_rgy_acct_add */
   sec_passwd_rec_t	caller_key;	/* sec_rgy_acct_add */
   sec_passwd_rec_t	new_key;	/* sec_rgy_acct_add */
   sec_passwd_type_t	new_keytype;	/* sec_rgy_acct_add */
   sec_rgy_login_name_t test_login_name;/* sec_rgy_acct_add */
   sec_rgy_plcy_auth_t	*set_auth_policy_p;/* sec_rgy_auth_plcy_set_info */
   sec_rgy_plcy_auth_t	get_auth_policy;/* sec_rgy_auth_plcy_get_info */
   sec_rgy_plcy_auth_t	get_eff_policy;/* sec_rgy_auth_plcy_get_effective */
   sec_rgy_plcy_auth_t	save_rgy_policy;


/* Check the number of arguments passed in.  They should be exactly the
   required number when you come into this routine. */

   if (argc != ARGS_REQUIRED)
     {
       TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n",
                  ARGS_REQUIRED, argc);
       return (TSH_BAD_ARG_COUNT);
     }

/* Obtain the input arguments from the .tsh command line. */

   subcase = atol(argv[0]);
   estatus = atol(argv[1]);

/* Bind to a registry server. */

   SEC_RGY_SITE_OPEN_WRITE(&write_context);
   SEC_RGY_SITE_OPEN_READ(&read_context);

   /* save the original RGY auth policy to restore later */
   sec_rgy_auth_plcy_get_info (write_context,
                               &AUTH_POL_LOGIN_RGY,
                               &save_rgy_policy,
                               &tstatus);
   TSHMessage (ctx, MOTRACE,
                   "Saved original rgy auth_policy status: %d\n", tstatus);



/* Add a test person item and an account to the registry if (1) this is NOT a 
   "not found" test case; or, (2) this is NOT a test of setting or getting
   the auth policy for the entire registry. */

   if (((subcase % 100) != NOT_FOUND_CASES) &&
       ((subcase % 100) != RGY_CASES))
     {

    /* Setup the parameters to add a person item. */

       bzero ((char *)&pgo_item, sizeof (pgo_item));

       uuid_create_nil (&pgo_item.id, &tstatus);

       pgo_item.unix_num = 5002; 
       pgo_item.quota = 1;
       pgo_item.flags = sec_rgy_pgo_flags_none;

       strcpy ((char *)pgo_item.fullname,
	       (char *)"marco's test person item\0");

    /* Add a person item. */

       sec_rgy_pgo_add (write_context,
                        (sec_rgy_domain_t)sec_rgy_domain_person,
		        AUTH_POL_PERSON_NAME,
		        &pgo_item,
                        &tstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_add (person) status: %d\n", tstatus);

    /* Cleanup and exit if adding the pgo item failed. */

       if (TSHStatus (ctx, SETUP,
                      "sec_rgy_pgo_add (person) failed\n", rpc_s_ok, tstatus))
         {
	   sec_rgy_site_close (write_context, &cstatus);

           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_close status: %d\n", cstatus);
    
           if (TSHStatus (ctx, CLNUP,
                          "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
             tstatus = cstatus;

           sec_rgy_site_close (read_context, &cstatus);

           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_close status: %d\n", cstatus);

           if (TSHStatus (ctx, CLNUP,
                          "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
             tstatus = cstatus;

           return (tstatus);
         }

       sec_rgy_pgo_add_member (write_context,
                               (sec_rgy_domain_t)sec_rgy_domain_group,
                               (unsigned_char_p_t)"none\0",
                               AUTH_POL_PERSON_NAME,
                               &tstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_add_member (group) status: %d\n", tstatus);

       if (TSHStatus (ctx, SETUP,
                  "sec_rgy_pgo_add_member (group) failed\n", rpc_s_ok, tstatus))
         {
           sec_rgy_pgo_delete (write_context, (sec_rgy_domain_t)sec_rgy_domain_person,
                               AUTH_POL_PERSON_NAME, &cstatus);
    
           TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_delete (cleanup person) status: %d\n", cstatus);

           if (TSHStatus (ctx, CLNUP,
                      "sec_rgy_pgo_delete (person) failed\n", rpc_s_ok, cstatus))
             tstatus = cstatus;

	   sec_rgy_site_close (write_context, &cstatus);

           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_close status: %d\n", cstatus);
    
           if (TSHStatus (ctx, CLNUP,
                          "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
             tstatus = cstatus;

           sec_rgy_site_close (read_context, &cstatus);

           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_close status: %d\n", cstatus);

           if (TSHStatus (ctx, CLNUP,
                          "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
             tstatus = cstatus;

           return (tstatus);
         }

       sec_rgy_pgo_add_member (write_context,
                               (sec_rgy_domain_t)sec_rgy_domain_org,
                               (unsigned_char_p_t)"none\0",
                               AUTH_POL_PERSON_NAME,
                               &tstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_add_member (org) status: %d\n", tstatus);

       if (TSHStatus (ctx, SETUP,
                  "sec_rgy_pgo_add_member (org) failed\n", rpc_s_ok, tstatus))
         {
           sec_rgy_pgo_delete_member (write_context,
				      (sec_rgy_domain_t)sec_rgy_domain_group,
                                      (unsigned_char_p_t)"none\0",
				      AUTH_POL_PERSON_NAME, &cstatus);
           TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete_member (cleanup group) status: %d\n", cstatus);

           if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete_member (group) failed\n", rpc_s_ok, cstatus))
             tstatus = cstatus;

           sec_rgy_pgo_delete (write_context, (sec_rgy_domain_t)sec_rgy_domain_person,
                               AUTH_POL_PERSON_NAME, &cstatus);
    
           TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_delete (cleanup person) status: %d\n", cstatus);

           if (TSHStatus (ctx, CLNUP,
                      "sec_rgy_pgo_delete (person) failed\n", rpc_s_ok, cstatus))
             tstatus = cstatus;

	   sec_rgy_site_close (write_context, &cstatus);

           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_close status: %d\n", cstatus);
    
           if (TSHStatus (ctx, CLNUP,
                          "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
             tstatus = cstatus;

           sec_rgy_site_close (read_context, &cstatus);

           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_close status: %d\n", cstatus);

           if (TSHStatus (ctx, CLNUP,
                          "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
             tstatus = cstatus;

           return (tstatus);
         }

    /* Setup the parameters to add an account. */

       bzero ((char *)&user_part, sizeof (user_part));
    
       user_part.passwd_version_number = 1;
       user_part.passwd_dtm = time(NULL);
       user_part.flags = 0;

       strcpy ((char *)user_part.gecos,
	       (char *)"marco's test user account\0");

       strcpy ((char *)user_part.homedir,
	       (char *)"//node_feb5/marco\0");

       strcpy ((char *)user_part.shell,
	       (char *)"/bin/seashell\0");

       bzero ((char *)&admin_part, sizeof (admin_part));

       admin_part.creation_date = time(NULL);
       admin_part.change_date = time(NULL);
       admin_part.expiration_date = time(NULL) + (60*10);
       admin_part.good_since_date = time(NULL);
       admin_part.flags = sec_rgy_acct_admin_valid;
       admin_part.authentication_flags = sec_rgy_acct_auth_tgt;

       bzero ((char *)&caller_key, sizeof (sec_passwd_rec_t));

       caller_key.version_number = 0;
       caller_key.key.key_type = sec_passwd_plain;
       caller_key.key.tagged_union.plain = (idl_char *)"-dce-";
   
       bzero ((char *)&new_key, sizeof (sec_passwd_rec_t));

       new_key.key.key_type = sec_passwd_plain;
       new_key.key.tagged_union.plain = AUTH_POL_PERSON_PASSWD;
   
       key_parts = (sec_rgy_acct_key_t)sec_rgy_acct_key_person;

    /* Add an account. */

       sec_rgy_acct_add (write_context,
                         &AUTH_POL_LOGIN_NAME,
		         &key_parts,
		         &user_part,
		         &admin_part,
		         &caller_key,
		         &new_key,
                         sec_passwd_des,
		         &passwd_ver_no,
		         &tstatus);

       TSHMessage (ctx, MOTRACE, "sec_rgy_acct_add status: %d\n", tstatus);

    /* Cleanup and exit if adding the account failed. */

       if (TSHStatus (ctx, SETUP,
                      "sec_rgy_acct_add failed\n", rpc_s_ok, tstatus))
         {
           sec_rgy_pgo_delete_member (write_context,
				      (sec_rgy_domain_t)sec_rgy_domain_org,
                                      (unsigned_char_p_t)"none\0",
				      AUTH_POL_PERSON_NAME, &cstatus);
           TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete_member (cleanup org) status: %d\n", cstatus);

           if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete_member (org) failed\n", rpc_s_ok, cstatus))
             tstatus = cstatus;

           sec_rgy_pgo_delete_member (write_context,
				      (sec_rgy_domain_t)sec_rgy_domain_group,
                                      (unsigned_char_p_t)"none\0",
				      AUTH_POL_PERSON_NAME, &cstatus);
           TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete_member (cleanup group) status: %d\n", cstatus);

           if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete_member (group) failed\n", rpc_s_ok, cstatus))
             tstatus = cstatus;

           sec_rgy_pgo_delete (write_context, (sec_rgy_domain_t)sec_rgy_domain_person,
                               AUTH_POL_PERSON_NAME, &cstatus);
    
           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_pgo_delete (cleanup person) status: %d\n", cstatus);

           if (TSHStatus (ctx, CLNUP,
                         "sec_rgy_pgo_delete (person) failed\n", rpc_s_ok, cstatus))
             tstatus = cstatus;

	   sec_rgy_site_close (write_context, &cstatus);

           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_close status: %d\n", cstatus);
    
           if (TSHStatus (ctx, CLNUP,
                          "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
             tstatus = cstatus;

           sec_rgy_site_close (read_context, &cstatus);

           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_close status: %d\n", cstatus);

           if (TSHStatus (ctx, CLNUP,
                          "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
             tstatus = cstatus;

           return (tstatus);
         }

     } /* End test of whether or not to setup an account */

/* Setup the login name for set_info.  Note that if login_name is not set,
   the policy for the registry is set.  Note also that if this is a 
   "not found" case, no account was added during setup. */ 

   if ((subcase % 100) == RGY_CASES)
     {
       bzero ((char *)&test_login_name, sizeof (sec_rgy_login_name_t));
       set_auth_policy_p = &AUTH_POL_RGY; 
     }

   else
     {
       memcpy ((char *)&test_login_name,
	       (char *)&AUTH_POL_LOGIN_NAME,
	       sizeof (sec_rgy_login_name_t));
       set_auth_policy_p = &AUTH_POL_USER; 
     }

/* sec_rgy_auth_policy_set_info is called either for its own test or as
   setup for sec_rgy_auth_plcy_get_{effective,info}.  Note that for the
   "not found" cases of get_{info,effective}, it is not necessary to set
   info. */

   if ((subcase != GET_AUTH_POL_NOT_FOUND) &&
       (subcase != GET_EFF_AUTH_POL_NOT_FOUND))
     {
       sec_rgy_auth_plcy_set_info (write_context,
                                   &test_login_name,
                                   set_auth_policy_p,
                                   &tstatus);
       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_auth_plcy_set_info status: %d\n", tstatus);

       if (estatus == tstatus)
         tstatus = sec_rgy_status_ok;
       else
         TSHMessage (ctx, MOFAILURE,
                     "sec_rgy_auth_plcy_set_info failed\n");
     }

/* sec_rgy_auth_plcy_get_info is called either for its own test or as
   verification for the sec_rgy_auth_plcy_set_info test of all valid data or
   as setup for sec_rgy_auth_plcy_get_effective test. */

   if (((subcase % 100) == RGY_CASES) ||
       ((subcase % 100) == NORMAL_CASES) ||
       (subcase == GET_AUTH_POL_NOT_FOUND) ||
       (subcase == GET_AUTH_POL_WILDCARD))
     {
       if (subcase == GET_AUTH_POL_WILDCARD)
         memcpy ((char *)&test_login_name,
	         (char *)&AUTH_POL_LOGIN_WILDCARD,
	         sizeof (sec_rgy_login_name_t));

       /* wait for propagation to any replicas */
       SEC_RGY_SITE_SYNCH(write_context);

       sec_rgy_auth_plcy_get_info (read_context,
                                   &test_login_name,
                                   &get_auth_policy,
                                   &tstatus);
       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_auth_plcy_get_info status: %d\n", tstatus);

       if (estatus == tstatus)
	 {

        /* Verify that the input to set_info came back as output from 
	   get_info. */

           if (tstatus == sec_rgy_status_ok)
	     if (memcmp ((char *)&get_auth_policy,
	                 (char *)set_auth_policy_p,
                         sizeof (sec_rgy_plcy_auth_t)))
	       {
	         TSHMessage (ctx, MOFAILURE,
                        "Output from get_info did not match input to set_info\n");
                 tstatus = TSH_TEST_FAILED;
               }
	     else
	       TSHMessage (ctx, MOTRACE,
                        "Output from get_info matched input to set_info\n");
           else
             tstatus = sec_rgy_status_ok;

          } /* End if (estatus == tstatus) */

       else
         TSHMessage (ctx, MOFAILURE,
                     "sec_rgy_auth_plcy_get_info failed\n");

     } /* End sec_rgy_auth_plcy_get_info path */

/* sec_rgy_auth_plcy_get_effective is only called for its own test. */

   if (subcase >= GET_EFF_AUTH_POL)
     {
       if (subcase == GET_EFF_AUTH_POL_WILDCARD)
         memcpy ((char *)&test_login_name,
	         (char *)&AUTH_POL_LOGIN_WILDCARD,
	         sizeof (sec_rgy_login_name_t));

    /* The effective policy requires both a registry and account auth policy.
       The account policy was added as part of NORMAL_CASES.  An auth policy
       now has to be added for the registry.  Because the auth policy for the
       account is the more restrictive of the account or the registry, we
       anticipate that the effective policy will be the account policy. 
       It has been revealed, however, that for rev 1.0 the effective auth
       policy will unconditionally be the rgy policy. */

       if (subcase == GET_EFF_AUTH_POL)
	 {

           sec_rgy_auth_plcy_set_info (write_context,
                                       &AUTH_POL_LOGIN_RGY,
                                       &AUTH_POL_RGY,
                                       &tstatus);
           TSHMessage (ctx, MOTRACE,
                   "sec_rgy_auth_plcy_set_info (rgy) status: %d\n", tstatus);

           if (TSHStatus (ctx, SETUP,
                       "sec_rgy_auth_plcy_set_info failed\n", estatus, tstatus));

         } /* End if (subcase == GET_EFF_AUTH_POL) */

       /* wait for propagation to any replicas */
       SEC_RGY_SITE_SYNCH(write_context);

       sec_rgy_auth_plcy_get_effective (read_context,
                                        &test_login_name,
                                        &get_eff_policy,
                                        &tstatus);
       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_auth_plcy_get_effective status: %d\n", tstatus);

       if (estatus == tstatus)
	 {
           if (tstatus == sec_rgy_status_ok)
             {
               if (subcase == GET_EFF_AUTH_POL)
	         {
                 if ( get_eff_policy.max_ticket_lifetime != 
                      min(AUTH_POL_RGY.max_ticket_lifetime,get_auth_policy.max_ticket_lifetime )
                      ||
                      get_eff_policy.max_renewable_lifetime != 
                      min(AUTH_POL_RGY.max_renewable_lifetime,get_auth_policy.max_renewable_lifetime )
                    )
/*	           if (memcmp ((char *)&get_eff_policy,
	                       (char *)&AUTH_POL_RGY,
                               sizeof (sec_rgy_plcy_auth_t))) */
	             {
	               TSHMessage (ctx, MOFAILURE,
                                  "Effective auth policy not returned\n");
                       tstatus = TSH_TEST_FAILED;
	             }
                   else
	             TSHMessage (ctx, MOTRACE,
                                "Effective auth policy returned\n");
                                                       
                 } /* End if (subcase == GET_EFF_AUTH_POL) */

               if (subcase == GET_EFF_AUTH_POL_RGY)
	         {
	           if (memcmp ((char *)&get_eff_policy,
	                       (char *)set_auth_policy_p,
                               sizeof (sec_rgy_plcy_auth_t)))
	             {
	               TSHMessage (ctx, MOFAILURE,
                               "Effective auth policy for rgy not returned\n");
                       tstatus = TSH_TEST_FAILED;
                     }
                   else
	             TSHMessage (ctx, MOTRACE,
                                 "Effective auth policy for rgy returned\n");

                 } /* End if (subcase == GET_EFF_AUTH_POL_RGY) */

             } /* End if (tstatus == sec_rgy_status_ok) */

	   else
	     tstatus = sec_rgy_status_ok;

         } /* End if (estatus == tstatus) */

       else
         TSHMessage (ctx, MOFAILURE,
                     "sec_rgy_auth_plcy_get_effective failed\n");

     } /* End if (subcase >= GET_EFF_AUTH_POL) */

/* Delete the account and pgo item, if necessary. */

   /* restore saved RGY auth policy */
   sec_rgy_auth_plcy_set_info (write_context,
                               &AUTH_POL_LOGIN_RGY,
                               &save_rgy_policy,
                               &tstatus);
   TSHMessage (ctx, MOTRACE,
                   "Restored original rgy auth_policy status: %d\n", tstatus);
		   


   if (((subcase % 100) != NOT_FOUND_CASES) &&
       ((subcase % 100) != RGY_CASES))
     {
       sec_rgy_acct_delete (write_context,
                            &AUTH_POL_LOGIN_NAME,
                            &cstatus);
    
       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_acct_delete (cleanup) status: %d\n", cstatus);

       if (TSHStatus (ctx, CLNUP,
                      "sec_rgy_acct_delete failed\n", rpc_s_ok, cstatus))
         tstatus = cstatus;

       sec_rgy_pgo_delete_member (write_context,
                                  (sec_rgy_domain_t)sec_rgy_domain_org,
                                  (unsigned_char_p_t)"none\0",
                                  AUTH_POL_PERSON_NAME, &cstatus);
       TSHMessage (ctx, MOTRACE,
           "sec_rgy_pgo_delete_member (cleanup org) status: %d\n", cstatus);

       if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete_member (org) failed\n", rpc_s_ok, cstatus))
         tstatus = cstatus;

       sec_rgy_pgo_delete_member (write_context,
                                  (sec_rgy_domain_t)sec_rgy_domain_group,
                                  (unsigned_char_p_t)"none\0",
                                  AUTH_POL_PERSON_NAME, &cstatus);
       TSHMessage (ctx, MOTRACE,
           "sec_rgy_pgo_delete_member (cleanup group) status: %d\n", cstatus);

       if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete_member (group) failed\n", rpc_s_ok, cstatus))
         tstatus = cstatus;

       sec_rgy_pgo_delete (write_context, (sec_rgy_domain_t)sec_rgy_domain_person,
                           AUTH_POL_PERSON_NAME, &cstatus);
    
       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_delete (cleanup person) status: %d\n", cstatus);

       if (TSHStatus (ctx, CLNUP,
                      "sec_rgy_pgo_delete (person) failed\n", rpc_s_ok, cstatus))
         tstatus = cstatus;
     }

/* Close the registry site. */

   sec_rgy_site_close (write_context, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_site_close status: %d\n", cstatus);
    
   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

   sec_rgy_site_close (read_context, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_site_close status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

/* Exit this routine. */

   return (tstatus);

} /* End auth_policy */
