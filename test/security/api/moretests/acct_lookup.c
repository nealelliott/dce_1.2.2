/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acct_lookup.c,v $
 * Revision 1.1.6.2  1996/02/17  23:25:02  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:14  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:20:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:51:51  root]
 * 
 * Revision 1.1.4.1  1993/10/07  18:59:02  mccann
 * 	CR9057 remove unnecessary verison requirement
 * 	[1993/10/07  18:57:35  mccann]
 * 
 * Revision 1.1.2.3  1993/02/25  22:03:51  cuti
 * 	Change sec_rgy_site_open() to sec_rgy_site_open_update() for using site update operation.
 * 	[1993/02/25  16:28:07  cuti]
 * 
 * Revision 1.1.2.2  1992/12/31  17:27:29  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:52:56  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:29  devrcs
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
 *	Extracted	1/4/92 16:56:15
 *	from delta on	11/13/91 14:17:19
 *
 *  $LOG$
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/acct_lookup.c, , dce_osf, dce.75d";
#endif

/* Include files */

#include "tsh_acct.h"


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
   sec_rgy_pname_t	site_name;	/* sec_rgy_site_open_update */
   sec_rgy_handle_t	context;	/* sec_rgy_site_open_update */
   sec_rgy_pgo_item_t	pgo_item;	/* sec_rgy_pgo_add */
   sec_rgy_acct_key_t	key_parts;	/* sec_rgy_acct_add */
   sec_rgy_acct_user_t  user_part;	/* sec_rgy_acct_add */
   sec_rgy_acct_admin_t admin_part;	/* sec_rgy_acct_add */
   sec_passwd_version_t	passwd_ver_no;	/* sec_rgy_acct_add */
   sec_passwd_rec_t	caller_key;	/* sec_rgy_acct_add */
   sec_passwd_rec_t	new_key;	/* sec_rgy_acct_add */
   sec_passwd_type_t	new_keytype;	/* sec_rgy_acct_add */
   sec_rgy_cursor_t	account_cursor;	/* sec_rgy_acct_lookup */
   sec_rgy_login_name_t	name_result;	/* sec_rgy_acct_lookup */
   sec_rgy_sid_t	id_sid;		/* sec_rgy_acct_lookup */
   sec_rgy_unix_sid_t	unix_sid;	/* sec_rgy_acct_lookup */
   sec_rgy_acct_key_t	lkp_key_parts;	/* sec_rgy_acct_lookup */
   sec_rgy_acct_user_t	lkp_user_part;	/* sec_rgy_acct_lookup */
   sec_rgy_acct_admin_t	lkp_admin_part;	/* sec_rgy_acct_lookup */
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

/*
   gethostname (node_name, sec_rgy_pname_t_size);
   sprintf ((char *)site_name, "ip:%s", node_name);
*/
   sprintf ((char *)site_name, "");


   sec_rgy_site_open_update (site_name,
                      &context,
                      &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_site_open_update status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
                  "sec_rgy_site_open_update failed\n", rpc_s_ok, tstatus))
     return (tstatus);

/* Add a test person item to the registry. */

   bzero ((char *)&pgo_item, sizeof (pgo_item));

   uuid_create_nil (&pgo_item.id, &tstatus);

   pgo_item.unix_num = 5002; 
   pgo_item.quota = 1;
   pgo_item.flags = sec_rgy_pgo_flags_none;

   memcpy ((char *)pgo_item.fullname,
	   (char *)"marco's test person item\0",
	   sizeof (pgo_item.fullname));

   sec_rgy_pgo_add (context,
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

   sec_rgy_pgo_add (context,
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

   sec_rgy_pgo_add (context,
                    (sec_rgy_domain_t)sec_rgy_domain_org,
		    ACCT_ORG_NAME,
		    &pgo_item,
                    &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_add (org) status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
                  "sec_rgy_pgo_add (org) failed\n", rpc_s_ok, tstatus))
     goto EXIT_DEL_GROUP_ITEM;

/* Add the test person item to the test group. */

   sec_rgy_pgo_add_member (context,
                           (sec_rgy_domain_t)sec_rgy_domain_group,
                           ACCT_GROUP_NAME,
                           ACCT_PERSON_NAME,
                           &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_add_member (group) status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
              "sec_rgy_pgo_add_member (group) failed\n", rpc_s_ok, tstatus))
     goto EXIT_DEL_ORG_ITEM;

/* Add the test person item to the test org item. */

   sec_rgy_pgo_add_member (context,
                           (sec_rgy_domain_t)sec_rgy_domain_org,
                           ACCT_ORG_NAME,
                           ACCT_PERSON_NAME,
                           &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_add_member (org) status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
              "sec_rgy_pgo_add_member (org) failed\n", rpc_s_ok, tstatus))
     goto EXIT_DEL_GROUP_MEMBER;

/* Setup the parameters to the sec_rgy_acct_add function. */

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
   
   key_parts = (sec_rgy_acct_key_t)sec_rgy_acct_key_person;

/* Add the test account. */

   sec_rgy_acct_add (context,
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

   if (TSHStatus (ctx, SETUP,
                  "sec_rgy_acct_add failed\n", rpc_s_ok, tstatus))
     goto EXIT_DEL_ORG_MEMBER;

/* Call the rpc under test. */
   
   sec_rgy_acct_lookup (context,
                        &ACCT_LOGIN_NAME,
                        &account_cursor,
                        &name_result,
                        &id_sid,
                        &unix_sid,
                        &lkp_key_parts,
                        &lkp_user_part,
                        &lkp_admin_part,
                        &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_acct_lookup (test) status: %d\n", tstatus);

/* If the lookup fails or if the data supplied as input to acct_add does
   not match the data returned as output from the acct_lookup, then treat
   the acct_lookup as if it had failed. */

   if ((tstatus != rpc_s_ok)
       || (key_parts != lkp_key_parts)
       || (memcmp ((char *)&admin_part,
                   (char *)&lkp_admin_part,
                   sizeof (sec_rgy_acct_admin_t)))
       || (memcmp ((char *)&user_part,
                   (char *)&lkp_user_part,
                   sizeof (sec_rgy_acct_user_t))))
     TSHMessage (ctx, SETUP, "sec_rgy_acct_lookup failed\n");

/* Cleanup from the rpc under test. */

   sec_rgy_acct_delete (context,
                        &ACCT_LOGIN_NAME,
                        &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_acct_delete (cleanup) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_acct_delete failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

   else
     {
    /* Even if tstatus came back with the normal return on the acct_delete
       above, check to see if the account was really deleted by attempting
       to look it up in the registry. */

       sec_rgy_acct_lookup (context,
                            &ACCT_LOGIN_NAME,
			    &account_cursor,
			    &name_result,
                            &id_sid,
			    &unix_sid,
			    &lkp_key_parts,
                            &lkp_user_part,
			    &lkp_admin_part,
			    &tstatus);

    /* On the acct_lookup after the acct_delete, we expect that the lookup
       would fail with object_not_found.  If it returns anything else, 
       then signal a cleanup error. */

       if (tstatus == sec_rgy_object_not_found)
	 tstatus = rpc_s_ok;
       else
         TSHMessage (ctx, MOFAILURE, "sec_rgy_acct_lookup failed\n");

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_acct_lookup (after delete) status: %d\n", tstatus);
     }

EXIT_DEL_ORG_MEMBER:

   sec_rgy_pgo_delete_member (context, (sec_rgy_domain_t)sec_rgy_domain_org,
		              ACCT_ORG_NAME, ACCT_PERSON_NAME, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete_member (org) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete_member (org) failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

EXIT_DEL_GROUP_MEMBER:

   sec_rgy_pgo_delete_member (context, (sec_rgy_domain_t)sec_rgy_domain_group,
		              ACCT_GROUP_NAME, ACCT_PERSON_NAME, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete_member (group) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete_member (group) failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

EXIT_DEL_ORG_ITEM:

   sec_rgy_pgo_delete (context, (sec_rgy_domain_t)sec_rgy_domain_org,
                       ACCT_ORG_NAME, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete (org) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete (org) failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

EXIT_DEL_GROUP_ITEM:

   sec_rgy_pgo_delete (context, (sec_rgy_domain_t)sec_rgy_domain_group,
                       ACCT_GROUP_NAME, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete (group) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete (group) failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

EXIT_DEL_PERSON_ITEM:

   sec_rgy_pgo_delete (context, (sec_rgy_domain_t)sec_rgy_domain_person,
                       ACCT_PERSON_NAME, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete (person) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete (person) failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

EXIT_SITE_CLOSE:

   sec_rgy_site_close (context, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_site_close status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

/* Exit this routine. */

   return (tstatus);

} /* End acct_lookup */

