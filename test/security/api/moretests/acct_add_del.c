/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acct_add_del.c,v $
 * Revision 1.1.11.3  1996/02/17  23:24:58  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:11  marty]
 *
 * Revision 1.1.11.2  1995/12/11  20:20:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:51:49  root]
 * 
 * Revision 1.1.7.1  1994/10/25  17:17:51  sekhar
 * 	[ OT 12398 ]  Checking in Sean Mullan's changes for testing
 * 		      security replication.
 * 	[1994/10/24  16:58:34  sekhar]
 * 
 * Revision 1.1.4.1  1993/10/07  18:58:51  mccann
 * 	CR9057 remove unnecessary verison requirement
 * 	[1993/10/07  18:56:24  mccann]
 * 
 * Revision 1.1.2.3  1993/02/25  22:03:20  cuti
 * 	Change sec_rgy_site_open() to sec_rgy_site_open_update() for using site update operation.
 * 	[1993/02/25  16:27:19  cuti]
 * 
 * Revision 1.1.2.2  1992/12/31  17:27:13  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:52:45  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:26  devrcs
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
 *	Extracted	1/4/92 16:56:14
 *	from delta on	11/13/91 14:17:09
 *
 *  $LOG$
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/acct_add_del.c, , dce_osf, dce.75d";
#endif

/* Include files */

#include "tsh_acct.h"
#include <sec_rgy_bind.h>


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
   sec_rgy_pname_t	node_name;	/* gethostname */
   sec_rgy_handle_t     write_context;  /* sec_rgy_site_open_update */
   sec_rgy_handle_t     read_context;   /* sec_rgy_site_open_query */
   sec_rgy_pgo_item_t	pgo_item;	/* sec_rgy_pgo_add */
   sec_rgy_acct_key_t	key_parts;	/* sec_rgy_acct_add */
   sec_rgy_acct_user_t  user_part;	/* sec_rgy_acct_add */
   sec_rgy_acct_admin_t admin_part;	/* sec_rgy_acct_add */
   sec_passwd_rec_t	caller_key;	/* sec_rgy_acct_add */
   sec_passwd_rec_t	new_key;	/* sec_rgy_acct_add */
   sec_passwd_type_t	new_keytype;	/* sec_rgy_acct_add */
   sec_passwd_version_t	passwd_ver_no;	/* sec_rgy_acct_add */
   sec_rgy_cursor_t	account_cursor;	/* sec_rgy_acct_lookup */
   sec_rgy_login_name_t	name_result;	/* sec_rgy_acct_lookup */
   sec_rgy_sid_t	id_sid;		/* sec_rgy_acct_lookup */
   sec_rgy_unix_sid_t	unix_sid;	/* sec_rgy_acct_lookup */
   sec_rgy_acct_key_t	new_key_parts;	/* sec_rgy_acct_lookup */
   sec_rgy_acct_user_t	new_user_part;	/* sec_rgy_acct_lookup */
   sec_rgy_acct_admin_t	new_admin_part;	/* sec_rgy_acct_lookup */
   error_status_t	tstatus;	/* test status */
   error_status_t	cstatus;	/* cleanup status */


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

/* Add a test person item to the registry. */

   bzero ((char *)&pgo_item, sizeof (pgo_item));

   uuid_create_nil (&pgo_item.id, &tstatus);

   pgo_item.unix_num = 5002; 
   pgo_item.quota = 1;
   pgo_item.flags = sec_rgy_pgo_flags_none;

   memcpy ((char *)pgo_item.fullname,
	   (char *)"marco's test person item\0",
	   sizeof (pgo_item.fullname));

   sec_rgy_pgo_add (write_context,
                    (sec_rgy_domain_t)sec_rgy_domain_person,
		    ACCT_PERSON_NAME,
		    &pgo_item,
                    &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_add (person) status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
                  "sec_rgy_pgo_add (person) failed\n", rpc_s_ok, tstatus))
     goto EXIT_SITE_CLOSE;

/* Add a test group item to the registry. */

   bzero ((char *)&pgo_item, sizeof (pgo_item));

   uuid_create_nil (&pgo_item.id, &tstatus);

   pgo_item.unix_num = 5003;
   pgo_item.quota = 1;
   pgo_item.flags = sec_rgy_pgo_flags_none;

   memcpy ((char *)pgo_item.fullname,
	   (char *)"locus:  marco's test group item\0",
	   sizeof (pgo_item.fullname));

   sec_rgy_pgo_add (write_context,
                    (sec_rgy_domain_t)sec_rgy_domain_group,
		    ACCT_GROUP_NAME,
		    &pgo_item,
                    &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_add (group) status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
                  "sec_rgy_pgo_add (group) failed\n", rpc_s_ok, tstatus))
     goto EXIT_DEL_PERSON_ITEM;

/* Add a test org item to the registry. */

   bzero ((char *)&pgo_item, sizeof (pgo_item));


   uuid_create_nil (&pgo_item.id, &tstatus);

   pgo_item.unix_num = 5004;
   pgo_item.quota = 1;
   pgo_item.flags = sec_rgy_pgo_flags_none;

   memcpy ((char *)pgo_item.fullname,
	   (char *)"quality:  marco's test org item\0",
	   sizeof (pgo_item.fullname));

   sec_rgy_pgo_add (write_context,
                    (sec_rgy_domain_t)sec_rgy_domain_org,
		    ACCT_ORG_NAME,
		    &pgo_item,
                    &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_add (org) status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
                  "sec_rgy_pgo_add (org) failed\n", rpc_s_ok, tstatus))
     goto EXIT_DEL_GROUP_ITEM;

/* Add an additional test org item to the registry for the test of
   key_parts uniqueness.  Also, add the test person item to the
   additional test org item. */

   if (subcase == ACCT_ADD_UNIQUE_KEY_PARTS)
     {
       bzero ((char *)&pgo_item, sizeof (pgo_item));

       uuid_create_nil (&pgo_item.id, &tstatus);

       pgo_item.unix_num = 5005;
       pgo_item.quota = 1;
       pgo_item.flags = sec_rgy_pgo_flags_none;

       memcpy ((char *)pgo_item.fullname,
	       (char *)"unique:  marco's alt test org item\0",
	       sizeof (pgo_item.fullname));

       sec_rgy_pgo_add (write_context,
                        (sec_rgy_domain_t)sec_rgy_domain_org,
                        ACCT_ALT_ORG_NAME,
		        &pgo_item,
                        &tstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_add (alt org) status: %d\n", tstatus);

       if (TSHStatus (ctx, SETUP,
              "sec_rgy_pgo_add (alt org) failed\n", rpc_s_ok, tstatus))
         goto EXIT_DEL_ORG_ITEM;

       sec_rgy_pgo_add_member (write_context,
                               (sec_rgy_domain_t)sec_rgy_domain_org,
                               ACCT_ALT_ORG_NAME,
                               ACCT_PERSON_NAME,
                               &tstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_add_member (alt org) status: %d\n", tstatus);

       if (TSHStatus (ctx, SETUP,
             "sec_rgy_pgo_add_member (alt org) failed\n", rpc_s_ok, tstatus))
         goto EXIT_DEL_ALT_ORG_ITEM;
     }

/* Add the test person item to the test group item unless the test
   demands that it not be added. */

   if ((subcase != ACCT_ADD_NOT_MBR_GROUP) &&
       (subcase != ACCT_ADD_NOT_MBR_BOTH))
     {
       sec_rgy_pgo_add_member (write_context,
                               (sec_rgy_domain_t)sec_rgy_domain_group,
                               ACCT_GROUP_NAME,
                               ACCT_PERSON_NAME,
                               &tstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_add_member (group) status: %d\n", tstatus);

       if (TSHStatus (ctx, SETUP,
                  "sec_rgy_pgo_add_member (group) failed\n", rpc_s_ok, tstatus))
         goto EXIT_DEL_ALT_ORG_MEMBER;
     }

/* Add the test person item to the test org item unless the test
   demands that it not be added. */

   if ((subcase != ACCT_ADD_NOT_MBR_ORG) &&
       (subcase != ACCT_ADD_NOT_MBR_BOTH))
     {
       sec_rgy_pgo_add_member (write_context,
                               (sec_rgy_domain_t)sec_rgy_domain_org,
                               ACCT_ORG_NAME,
                               ACCT_PERSON_NAME,
                               &tstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_add_member (org) status: %d\n", tstatus);

       if (TSHStatus (ctx, SETUP,
                  "sec_rgy_pgo_add_member (org) failed\n", rpc_s_ok, tstatus))
         goto EXIT_DEL_GROUP_MEMBER;
     }

/* Setup the parameters to the sec_rgy_acct_add function. */

   if (subcase == ACCT_ADD_UNIQUE_KEY_PARTS)
     key_parts = (sec_rgy_acct_key_t)sec_rgy_acct_key_group;
   else
     key_parts = (sec_rgy_acct_key_t)sec_rgy_acct_key_person;

   bzero ((char *)&user_part, sizeof (user_part));

   user_part.passwd_version_number = 1;
   user_part.passwd_dtm = time(NULL);
   user_part.flags = 0;

   memcpy ((char *)user_part.gecos,
	   (char *)"marco's test user account\0",
	   sizeof (user_part.gecos));

   memcpy ((char *)user_part.homedir,
	   (char *)"//node_feb5/marco\0",
	   sizeof (user_part.homedir));

   memcpy ((char *)user_part.shell,
	   (char *)"/bin/seashell\0",
	   sizeof (user_part.shell));

   bzero ((char *)&admin_part, sizeof (admin_part));

   admin_part.creation_date = time(NULL);
   admin_part.change_date = time(NULL);
   admin_part.expiration_date = time(NULL) + (60*60*24);
   admin_part.good_since_date = time(NULL);
   admin_part.flags = sec_rgy_acct_admin_valid;
   admin_part.authentication_flags = sec_rgy_acct_auth_tgt;

   bzero ((char *)&caller_key, sizeof (sec_passwd_rec_t));

   caller_key.version_number = 0;
   caller_key.key.key_type = sec_passwd_plain;
   caller_key.key.tagged_union.plain = (idl_char *)"-dce-";
   
   bzero ((char *)&new_key, sizeof (sec_passwd_rec_t));

   new_key.key.key_type = sec_passwd_plain;
   new_key.key.tagged_union.plain = ACCT_PERSON_PASSWD;
   
/* sec_rgy_acct_add is called unconditionally either for its test
   or as setup for sec_rgy_acct_delete. */

   sec_rgy_acct_add (write_context,
                     &ACCT_LOGIN_NAME,
		     &key_parts,
		     &user_part,
		     &admin_part,
		     &caller_key,
		     &new_key,
                     sec_passwd_des,
		     &passwd_ver_no,
		     &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_acct_add status: %d\n", tstatus);

/* If this is neither a delete test case nor the add twice
   test case, treat the previous acct_add as the function
   under test; otherwise, treat it as setup for a test case. */

   if ((subcase < ACCT_DEL) &&
       (subcase != ACCT_ADD_2X))
     {
       if (estatus == tstatus)
	 tstatus = rpc_s_ok;
       else
         {
           TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_add failed\n");
           goto EXIT_DEL_ORG_MEMBER;
         }
      }

   else
     if (tstatus != rpc_s_ok)
       {
         TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_add failed\n");
	 goto EXIT_DEL_ORG_MEMBER;
       }

/* Even if tstatus came back with the normal return on the acct_add
   above, check to see if the account really exists by attempting to
   look it up in the registry.  If this is one of the "not member"
   tests, do not attempt the lookup, because the account was not
   added; acct_delete, therefore, will be skipped. */

   if ((subcase < ACCT_ADD_NOT_MBR_GROUP) &&
       (subcase > ACCT_ADD_NOT_MBR_BOTH))
     {

       /* wait for propagation to any replicas */
       SEC_RGY_SITE_SYNCH(write_context);

       sec_rgy_acct_lookup (read_context,
                            &ACCT_LOGIN_NAME,
			    &account_cursor,
			    &name_result,
                            &id_sid,
			    &unix_sid,
			    &new_key_parts,
                            &new_user_part,
			    &new_admin_part,
			    &tstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_acct_lookup status: %d\n", tstatus);

    /* If the lookup failed, then if this was the normal case acct_add test,
       then signal a test failure; otherwise, signal a setup failure. */

       if ((tstatus != rpc_s_ok)
	   || (key_parts != new_key_parts)
           || (admin_part.expiration_date != new_admin_part.expiration_date)
           || (admin_part.good_since_date != new_admin_part.good_since_date)
           || (admin_part.flags != new_admin_part.flags)
           || (admin_part.authentication_flags != new_admin_part.authentication_flags)
           || (strcmp(user_part.gecos, new_user_part.gecos))
           || (strcmp(user_part.homedir, new_user_part.homedir))
           || (strcmp(user_part.shell, new_user_part.shell)))
         {
           if (subcase == ACCT_ADD)
             TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_lookup failed\n");
           else	
             TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_lookup failed\n");

	   if (tstatus == rpc_s_ok)
	     tstatus = TSH_TEST_FAILED;
         }
     }

   else
     goto EXIT_DEL_ORG_MEMBER;

/* sec_rgy_acct_add is only called twice to test the sec_rgy_object_exists
   case.  If the previous acct_add or acct_lookup failed, it will not be
   called. */

   if ((subcase == ACCT_ADD_2X) &&
       (tstatus == rpc_s_ok))
     {
       sec_rgy_acct_add (write_context,
                         &ACCT_LOGIN_NAME,
	                 &key_parts,
		         &user_part,
		         &admin_part,
		         &caller_key,
		         &new_key,
                         sec_passwd_des,
		         &passwd_ver_no,
		         &tstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_acct_add (2x) status: %d\n", tstatus);

       if (estatus == tstatus)
         tstatus = rpc_s_ok;
       else
         TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_add (add 2x) failed\n");
     }

/* Attempt to add another account with the same key_parts abbreviation.  The
   registry should return the next shortest key_parts abbreviation. */

   if ((subcase == ACCT_ADD_UNIQUE_KEY_PARTS) &&
       (tstatus == rpc_s_ok))
     {
       key_parts = (sec_rgy_acct_key_t)sec_rgy_acct_key_group;

       sec_rgy_acct_add (write_context,
                         &ACCT_ALT_LOGIN_NAME,
	                 &key_parts,
		         &user_part,
		         &admin_part,
		         &caller_key,
		         &new_key,
                         sec_passwd_des,
		         &passwd_ver_no,
		         &tstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_acct_add (key parts) status: %d\n", tstatus);

    /* If the add failed or if the key_parts were not bumped up to the
       next most unique abbreviation, then signal a test failure.  The
       alt account only needs to be deleted if the add succeeded. */

       if (tstatus == estatus)
	 {
	   if (key_parts != sec_rgy_acct_key_org)
	     {
              TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_add (key parts) failed\n");
	      tstatus = TSH_TEST_FAILED;
	     }

	   sec_rgy_acct_delete (write_context,
                                &ACCT_ALT_LOGIN_NAME, 
                                &cstatus);

           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_acct_delete (key parts) status: %d\n", tstatus);

           if (cstatus != rpc_s_ok)
	     {
               TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_delete failed\n");
	       tstatus = cstatus;
             }
         }

       else
	 TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_add (key parts) failed\n");
     }

/* sec_rgy_acct_delete is called unconditionally for all of the acct_delete
   test cases and as cleanup for the ACCT_ADD, ACCT_ADD_2X, and
   ACCT_ADD_UNIQUE_KEY_PARTS test cases. */

   sec_rgy_acct_delete (write_context,
                        &ACCT_LOGIN_NAME,
                        &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_acct_delete status: %d\n", tstatus);

/* If this was the test case for an expected normal return account
   deletion, then check for test case errors; otherwise, if it was
   cleanup after an account addition test case, check for cleanup
   errors; otherwise, if this was the setup for the account delete
   sec_rgy_object_not_found test, check for setup errors. */

   if (subcase == ACCT_DEL)
     {
       if (estatus == tstatus)
	 tstatus = rpc_s_ok;
       else
	 {
	   TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_delete failed\n");
	   goto EXIT_DEL_ORG_MEMBER;
	 }
     }

   else
     if (tstatus != rpc_s_ok)
       {
	 if (subcase < ACCT_DEL)
	   TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_delete failed\n");
	 else
	   TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_delete failed\n");
	 goto EXIT_DEL_ORG_MEMBER;
       }
	
/* Even if tstatus came back with the normal return on the acct_delete
   above, check to see if the account was really deleted by attempting
   to look it up in the registry. */

   /* wait for propagation to any replicas */
   SEC_RGY_SITE_SYNCH(write_context);

   sec_rgy_acct_lookup (read_context,
                        &ACCT_LOGIN_NAME,
			&account_cursor,
			&name_result,
                        &id_sid,
			&unix_sid,
			&new_key_parts,
                        &new_user_part,
			&new_admin_part,
			&tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_acct_lookup (verify delete) status: %d\n", tstatus);

/* On the acct_lookup after the acct_delete, we expect that the lookup
   would fail with object_not_found.  If it returns anything else, 
   then if this is the normal case acct_delete, signal a test failure;
   otherwise, if this is the delete_2x test case, signal a setup
   failure; otherwise, if this acct_delete is a cleanup operation
   after an acct_add, signal a setup failure. */

   if (tstatus != sec_rgy_object_not_found)
     switch (subcase)
       {
	 case ACCT_DEL:
           TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_lookup failed\n");
	   break;

         case ACCT_DEL_2X:
           TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_lookup failed\n");
	   break;

         default:
           TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_lookup failed\n");
           break;

       } /* End switch (subcase) */

   else /* sec_rgy_acct_delete is called twice to test the 
           sec_rgy_object_not_found case. */
     {
       if (subcase == ACCT_DEL_2X)
         {
           sec_rgy_acct_delete (write_context,
                                &ACCT_LOGIN_NAME,
                                &tstatus);
  
           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_acct_lookup (delete 2x) status: %d\n", tstatus);

           if (estatus == tstatus)
             tstatus = rpc_s_ok;
           else
             TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_delete (del 2x) failed\n");
         }
     }

/* Cleanup from the rpc under test. */

EXIT_DEL_ORG_MEMBER:

   sec_rgy_pgo_delete_member (write_context, (sec_rgy_domain_t)sec_rgy_domain_org,
		              ACCT_ORG_NAME, ACCT_PERSON_NAME, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete_member (cleanup org) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete_member (org) failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

EXIT_DEL_GROUP_MEMBER:

   sec_rgy_pgo_delete_member (write_context, (sec_rgy_domain_t)sec_rgy_domain_group,
		              ACCT_GROUP_NAME, ACCT_PERSON_NAME, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete_member (cleanup group) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete_member (group) failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

EXIT_DEL_ALT_ORG_MEMBER:

   if (subcase == ACCT_ADD_UNIQUE_KEY_PARTS)
     {
       sec_rgy_pgo_delete_member (write_context, (sec_rgy_domain_t)sec_rgy_domain_org,
                                  ACCT_ALT_ORG_NAME, ACCT_PERSON_NAME, &cstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_delete_member (cleanup alt org) status: %d\n", cstatus);

       if (TSHStatus (ctx, CLNUP,
             "sec_rgy_pgo_delete_member (alt org) failed\n", rpc_s_ok, cstatus))
         tstatus = cstatus;
     }

EXIT_DEL_ALT_ORG_ITEM:

   if (subcase == ACCT_ADD_UNIQUE_KEY_PARTS)
     {
       sec_rgy_pgo_delete (write_context, (sec_rgy_domain_t)sec_rgy_domain_org,
                           ACCT_ALT_ORG_NAME, &cstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_delete (cleanup alt org) status: %d\n", cstatus);

       if (TSHStatus (ctx, CLNUP,
             "sec_rgy_pgo_delete (alt org) failed\n", rpc_s_ok, cstatus))
         tstatus = cstatus;
     }
       
EXIT_DEL_ORG_ITEM:

   sec_rgy_pgo_delete (write_context, (sec_rgy_domain_t)sec_rgy_domain_org,
                       ACCT_ORG_NAME, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete (cleanup org) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete (org) failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

EXIT_DEL_GROUP_ITEM:

   sec_rgy_pgo_delete (write_context, (sec_rgy_domain_t)sec_rgy_domain_group,
                       ACCT_GROUP_NAME, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete (cleanup group) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete (group) failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

EXIT_DEL_PERSON_ITEM:

   sec_rgy_pgo_delete (write_context, (sec_rgy_domain_t)sec_rgy_domain_person,
                       ACCT_PERSON_NAME, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete (cleanup person) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete (person) failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

EXIT_SITE_CLOSE:

   sec_rgy_site_close (read_context, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_site_close (cleanup) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

   sec_rgy_site_close (write_context, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_site_close (cleanup) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

/* Exit this routine. */

   return (tstatus);

} /* End acct_add_del */

