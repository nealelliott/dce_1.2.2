/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: unix_getgrnam.c,v $
 * Revision 1.1.7.2  1996/02/17  23:25:55  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:57  marty]
 *
 * Revision 1.1.7.1  1995/12/11  21:50:32  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  22:07 UTC  mullan_s
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/31  18:09  mdf
 * 	merged forward
 * 	[1995/12/11  20:53:13  root]
 * 
 * Revision 1.1.5.2  1993/07/09  10:45:47  root
 * 	Initial King Kong branch
 * 	[1993/07/09  10:45:08  root]
 * 
 * Revision 1.1.3.2  1993/05/21  16:13:16  rps
 * 	Initialize group_cmp.members for comparison
 * 	[1993/05/20  20:44:05  rps]
 * 
 * Revision 1.1.1.8  1993/05/04  22:17:12  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.5.3  1993/02/25  22:09:03  cuti
 * 	Change sec_rgy_site_open() to sec_rgy_site_open_update() for using site update operation.
 * 	[1993/02/25  16:34:34  cuti]
 * 
 * Revision 1.1.5.2  1992/12/31  17:34:28  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:59:07  htf]
 * 
 * Revision 1.1.3.2  1992/06/02  21:24:07  sekhar
 * 	Drop Unix interface changes made by frisco
 * 	[1992/06/02  20:15:28  sekhar]
 * 
 * Revision 1.1.1.2  1992/05/22  20:11:06  frisco
 * 	Initial revision - getgrnam test
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
static char sccs_id[] = "src/test/security/api/moretests/unix_getgrnam.c";
#endif

/* Include files */

#include "tsh_acct.h"

/* Defined's for unix call under test */

#define MAX_NUM_MEMBERS 20
#define NUM_PERSONS 10

ndr_long_int TestFunction (ctx, argc, argv)

/* Formal parameters */

   CtxBlock          *ctx;
   ndr_long_int      argc;
   unsigned_char_p_t argv[];

{

/* Local variables */

   int                     i;
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
   sec_rgy_name_t          group_name;
   signed32                name_len;
   signed32                num_members;      /* sec_rgy_unix_getgrgid */
   sec_rgy_member_t        members[MAX_NUM_MEMBERS];
   sec_rgy_unix_group_t    group_cmp;        /* sec_rgy_unix_getgrgid */
   sec_rgy_unix_group_t    group_entry;      /* sec_rgy_unix_getgrgid */
   sec_rgy_name_t          person_name;      /* sec_rgy_pgo_add */
   sec_rgy_acct_key_t	lkp_key_parts;	/* sec_rgy_acct_lookup */
   sec_rgy_acct_user_t	lkp_user_part;	/* sec_rgy_acct_lookup */
   sec_rgy_acct_admin_t	lkp_admin_part;	/* sec_rgy_acct_lookup */
   error_status_t	tstatus;	/* test status */
   error_status_t	cstatus;	/* cleanup status */

   account_cursor.valid = false;

   sprintf ((char *)site_name, "");

   sec_rgy_site_open_update (site_name,
                      &context,
                      &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_site_open_update status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
                  "sec_rgy_site_open_update failed\n", rpc_s_ok, tstatus))
     return (tstatus);

/* Add NUM_PERSONS test person items to the registry. */

   strcpy(person_name, ACCT_PERSON_NAME);

   for (i = 0; i < NUM_PERSONS; i++) {
       bzero ((char *)&pgo_item, sizeof (pgo_item));

       uuid_create_nil (&pgo_item.id, &tstatus);

       pgo_item.unix_num = 5002 + i; 
       pgo_item.quota = 1;
       pgo_item.flags = sec_rgy_pgo_flags_none;

       memcpy ((char *)pgo_item.fullname,
    	   (char *)"marco's test person item\0",
    	   sizeof (pgo_item.fullname));

       strcat(person_name, "I");

       sec_rgy_pgo_add (context,
                    (sec_rgy_domain_t)sec_rgy_domain_person,
		    person_name,
		    &pgo_item,
                    &tstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_add (person) status: %d\n", tstatus);

       if (TSHStatus (ctx, SETUP,
                      "sec_rgy_pgo_add (person) failed\n", rpc_s_ok, tstatus))
       goto EXIT_SITE_CLOSE;
   }

/* Add a test group item to the registry. */

   bzero ((char *)&pgo_item, sizeof (pgo_item));

   uuid_create_nil (&pgo_item.id, &tstatus);

   pgo_item.unix_num = 5003;
   pgo_item.quota = 1;
   pgo_item.flags = sec_rgy_pgo_flags_none;
   
   strcpy(group_name, ACCT_GROUP_NAME);
   name_len = strlen(group_name);

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

/* fill group entry for comparison */

   strcpy(group_cmp.name, ACCT_GROUP_NAME);
   group_cmp.gid = pgo_item.unix_num;
   strcpy(person_name, ACCT_PERSON_NAME);
   strcpy(group_cmp.members, "");
   for (i = 0; i < NUM_PERSONS; i++) {
       strcat(person_name, "I");
       if (i > 0)
           strcat(group_cmp.members, ",");
       strcat(group_cmp.members, person_name);
   }

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

/* Add the test person items to the test group. */

   strcpy(person_name, ACCT_PERSON_NAME);

   for (i = 0; i < NUM_PERSONS; i++) {
       strcat(person_name, "I");
       sec_rgy_pgo_add_member (context,
                           (sec_rgy_domain_t)sec_rgy_domain_group,
                           ACCT_GROUP_NAME,
                           person_name,
                           &tstatus);

       TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_add_member (group) status: %d\n", tstatus);

       if (TSHStatus (ctx, SETUP,
              "sec_rgy_pgo_add_member (group) failed\n", rpc_s_ok, tstatus))
         goto EXIT_DEL_ORG_ITEM;
   }

/* Add the test person items to the test org item. */

   strcpy(person_name, ACCT_PERSON_NAME);

   for (i = 0; i < NUM_PERSONS; i++) {
       strcat(person_name, "I");
       sec_rgy_pgo_add_member (context,
                           (sec_rgy_domain_t)sec_rgy_domain_org,
                           ACCT_ORG_NAME,
                           person_name,
                           &tstatus);

       TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_add_member (org) status: %d\n", tstatus);

       if (TSHStatus (ctx, SETUP,
              "sec_rgy_pgo_add_member (org) failed\n", rpc_s_ok, tstatus))
         goto EXIT_DEL_GROUP_MEMBER;
   }

/* Call the unix interface under test. */
   
   sec_rgy_unix_getgrnam (context,
                        group_name,
                        name_len,
                        MAX_NUM_MEMBERS,
                        &account_cursor,
                        &group_entry,
                        &num_members,
                        members,
                        &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_unix_getgrid (test) status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
                  "sec_rgy_unix_getgrnam failed\n", rpc_s_ok, tstatus))
     goto EXIT_DEL_ORG_MEMBER;

/* If the lookup fails or if the data supplied as input to pgo_add does
   not match the data returned as output from the getgrnam, then treat
   the getgrnam as if it had failed. */

     if (strcmp(group_cmp.name, 
                  group_entry.name)
       || strcmp(group_cmp.members, 
                  group_entry.members)
       || (group_cmp.gid != group_entry.gid)) {
         tstatus = TSH_TEST_FAILED; /* failure status */
         TSHStatus (ctx, TEST, "sec_rgy_unix_getgrnam failed - entry mismatch\n",
                   tstatus, rpc_s_ok);
     }

/* check membership list */
   strcpy(person_name, ACCT_PERSON_NAME);
   for (i = 0; i < NUM_PERSONS; i++) {
       strcat(person_name, "I");
       if (strcmp(members[i], person_name)) {
           tstatus = TSH_TEST_FAILED;
           TSHStatus (ctx, TEST, 
               "sec_rgy_unix_getgrnam failed - member list mismatch\n",
               tstatus, rpc_s_ok);
           break;
       }
   }

/* Cleanup from the rpc under test. */

EXIT_DEL_ORG_MEMBER:

   strcpy(person_name, ACCT_PERSON_NAME);

   for (i = 0; i < NUM_PERSONS; i++) {
       strcat(person_name, "I");
       sec_rgy_pgo_delete_member (context, (sec_rgy_domain_t)sec_rgy_domain_org,
		              ACCT_ORG_NAME, person_name, &cstatus);

       TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete_member (org) status: %d\n", cstatus);

       if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete_member (org) failed\n", rpc_s_ok, cstatus))
         tstatus = cstatus;
   }

EXIT_DEL_GROUP_MEMBER:

   strcpy(person_name, ACCT_PERSON_NAME);

   for (i = 0; i < NUM_PERSONS; i++) {
       strcat(person_name, "I");
       sec_rgy_pgo_delete_member (context, (sec_rgy_domain_t)sec_rgy_domain_group,
		              ACCT_GROUP_NAME, person_name, &cstatus);

       TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete_member (group) status: %d\n", cstatus);

       if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete_member (group) failed\n", rpc_s_ok, cstatus))
         tstatus = cstatus;
   }

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

   strcpy(person_name, ACCT_PERSON_NAME);

   for (i = 0; i < NUM_PERSONS; i++) {
       strcat(person_name, "I");
       sec_rgy_pgo_delete (context, (sec_rgy_domain_t)sec_rgy_domain_person,
                       person_name, &cstatus);

       TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete (person) status: %d\n", cstatus);

       if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete (person) failed\n", rpc_s_ok, cstatus))
         tstatus = cstatus;
   }

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

