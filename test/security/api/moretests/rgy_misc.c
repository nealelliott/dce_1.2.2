/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rgy_misc.c,v $
 * Revision 1.1.6.2  1996/02/17  23:25:43  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:49  marty]
 *
 * Revision 1.1.6.1  1995/12/11  21:49:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:50  root]
 * 
 * Revision 1.1.4.3  1993/01/29  21:12:18  cuti
 * 	Add login_get_effective()
 * 	[1993/01/28  16:05:29  cuti]
 * 
 * Revision 1.1.4.2  1992/12/31  17:32:20  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:57:37  htf]
 * 
 * Revision 1.1.2.2  1992/05/07  13:27:51  cuti
 * 	Changed to do components' compare instead of bcmp the whole sec_rgy_pgo_item_t structure.
 * 	[1992/05/06  20:22:59  cuti]
 * 
 * Revision 1.1  1992/01/19  04:08:01  devrcs
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
 *	Extracted	1/4/92 16:57:57
 *	from delta on	11/19/91 19:13:03
 *
 *  $Log: rgy_misc.c,v $
 * Revision 1.1.6.2  1996/02/17  23:25:43  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:49  marty]
 *
 * Revision 1.1.7.2  1996/02/17  22:43:49  marty
 * 	Update OSF copyright year
 *
 * Revision 1.1.6.1  1995/12/11  21:49:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:50  root]
 *
 * Revision 1.1.5.2  1995/12/11  20:52:50  root
 * 	Submit OSF/DCE 1.2.1
 *
 * Revision 1.1.4.3  1993/01/29  21:12:18  cuti
 * 	Add login_get_effective()
 * 	[1993/01/28  16:05:29  cuti]
 *
 * Revision 1.1.5.2  1993/01/28  16:05:29  cuti
 * 	Add login_get_effective()
 *
 * Revision 1.1.4.2  1992/12/31  17:32:20  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:57:37  htf]
 *
 * Revision 1.1.3.2  1992/12/31  16:57:37  htf
 * 	Embedding copyright notice
 *
 * Revision 1.1.2.2  1992/05/07  13:27:51  cuti
 * 	Changed to do components' compare instead of bcmp the whole sec_rgy_pgo_item_t structure.
 * 	[1992/05/06  20:22:59  cuti]
 *
 * Revision 1.1.1.2  1992/05/06  20:22:59  cuti
 * 	Changed to do components' compare instead of bcmp the whole sec_rgy_pgo_item_t structure.
 *
 * Revision 1.1  1992/01/19  04:08:01  devrcs
 * Initial revision
 *
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/rgy_misc.c, , dce_osf, dce.75d";
#endif

/* Include files */

#include <tsh_misc.h>
#include <dce/misc.h>
#include <dce/pgo.h>

unsigned cache_refresh_count=0;

void cache_refresh_handler()
    {
    cache_refresh_count++;
    }



CtxBlock          *lCtx;

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
   sec_rgy_pname_t	site_name;	/* sec_rgy_site_open */
   sec_rgy_handle_t	context;	/* sec_rgy_site_open */ 
   sec_rgy_login_name_t	*login_name;

   sec_rgy_acct_key_t key_parts;/* sec_login_get_info */
   sec_rgy_sid_t sid;           /* sec_login_get_info */
   sec_rgy_unix_sid_t unix_sid; /* sec_login_get_info */
   sec_rgy_acct_user_t user_part;/* sec_login_get_info */
   sec_rgy_acct_admin_t admin_part;/* sec_login_get_info */
   sec_rgy_plcy_t policy_data;     /* sec_login_get_info */
   signed32 max_number=10;
   signed32 supplied_number;
   uuid_t id_projlist[10];
   signed32 unix_projlist[10];
   signed32 num_projects;
   sec_rgy_name_t cell_name;
   uuid_t cell_uuid;
   sec_override_fields_t	overriden;


   sec_rgy_cursor_t cursor;
   sec_rgy_pgo_item_t itemInfo1,itemInfo2;
   sec_passwd_str_t	plain_passwd;	/* sec_rgy_acct_add */


   sec_rgy_name_t      scope,           /* sec_rgy_pgo_get_next */
                       name;



   error_status_t	tstatus;	/* test status */
   error_status_t	cstatus;	/* cleanup status */


   lCtx=ctx;

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

    strcpy((char *) site_name,"");

    if ( (tstatus= RegistryOpen(&context)) != sec_rgy_status_ok )
         return tstatus;

   memcpy ((char *)plain_passwd,
           ACCT_PERSON_PASSWD,
           sizeof (sec_passwd_str_t));

    if ( (tstatus=AddPerson(context,  (sec_rgy_name_t *) ACCT_PERSON_NAME,  0,  1001,  1,  sec_rgy_pgo_flags_none,  (sec_rgy_pname_t *)ACCT_PERSON_FULLNAME ) ) )
        goto RGY_CLOSE_EXIT;
    if ( (tstatus=AddGroup(context,  (sec_rgy_name_t *)ACCT_GROUP_NAME,  0,  1001,  1,  sec_rgy_pgo_flags_none,  (sec_rgy_pname_t *)ACCT_GROUP_FULLNAME ) ) )
        goto DELETE_PERSON_EXIT;
    if ( (tstatus=AddOrg(context,  (sec_rgy_name_t *)ACCT_ORG_NAME,  0,  1001,  1,  sec_rgy_pgo_flags_none,  (sec_rgy_pname_t *)ACCT_ORG_FULLNAME )  ) )
        goto DELETE_GROUP_EXIT;

    if ( (tstatus=AddGroupMember(context,  (sec_rgy_name_t *) ACCT_GROUP_NAME,  (sec_rgy_name_t *) ACCT_PERSON_NAME ) ) )
        goto DELETE_GROUP_MEMBER_EXIT;

    if ( (tstatus=AddOrgMember(context,    (sec_rgy_name_t *) ACCT_ORG_NAME,  (sec_rgy_name_t *) ACCT_PERSON_NAME ) ) )
        goto DELETE_ORG_MEMBER_EXIT;

    if ( (tstatus=CreateAccount(context, &ACCT_LOGIN_NAME, (sec_rgy_pname_t *)ACCT_GECOS, (sec_rgy_pname_t *)ACCT_HOMEDIR, (sec_rgy_pname_t *)ACCT_SHELL, plain_passwd )) )
        goto DELETE_ORG_EXIT;

    switch (subcase)
        {
        case LOGIN_GET_INFO_OK:
        case LOGIN_GET_INFO_BAD_ACCT:
           {  
			if (subcase == LOGIN_GET_INFO_OK)  
				login_name = &ACCT_LOGIN_NAME; 
			else  
				login_name = &ACCT_ALT_LOGIN_NAME;

            sec_rgy_login_get_info(context, 
                                   login_name, 
                                   &key_parts, 
                                   &sid, 
                                   &unix_sid, 
                                   &user_part, 
                                   &admin_part,
                                   &policy_data,
                                   max_number,
                                   &supplied_number,
                                   id_projlist,
                                   unix_projlist,
                                   &num_projects,
                                   cell_name,
                                   &cell_uuid,
                                   &tstatus );

            TSHMessage (ctx, MOTRACE,
               "sec_rgy_login_get_info status: %d\n", tstatus);

            if (!TSHStatus (ctx, SETUP,
                  "sec_rgy_login_get_info failed\n", estatus, tstatus)) 
				tstatus = error_status_ok;

            break;
            }
                           

        case LOGIN_GET_EFFECTIVE_OK:
        case LOGIN_GET_EFFECTIVE_BAD_ACCT:
           {  
			if (subcase == LOGIN_GET_EFFECTIVE_OK)  
				login_name = &ACCT_LOGIN_NAME; 
			else  
				login_name = &ACCT_ALT_LOGIN_NAME;

            sec_rgy_login_get_effective(context, 
                                   login_name, 
                                   &key_parts, 
                                   &sid, 
                                   &unix_sid, 
                                   &user_part, 
                                   &admin_part,
                                   &policy_data,
                                   max_number,
                                   &supplied_number,
                                   id_projlist,
                                   unix_projlist,
                                   &num_projects,
                                   cell_name,
                                   &cell_uuid,
								   &overriden,
                                   &tstatus );

            TSHMessage (ctx, MOTRACE,
               "sec_rgy_login_get_effective status: %d\n", tstatus);

            if (!TSHStatus (ctx, SETUP,
                  "sec_rgy_login_get_effective failed\n", estatus, tstatus)) 
				tstatus = error_status_ok;

            break;
            }

#ifdef FOO
        case RGY_SET_RESOLVE_MODE_ON:
            {
            sec_rgy_set_resolve_mode(context, sec_rgy_resolve_pname );

            TSHMessage (ctx, MOTRACE,
               "sec_rgy_set_resolve_mode \n");

            tstatus=sec_rgy_status_ok;
            break;
            }

        case RGY_SET_RESOLVE_MODE_OFF:
            {
            sec_rgy_set_resolve_mode(context, sec_rgy_no_resolve_pname );

            TSHMessage (ctx, MOTRACE,
               "sec_rgy_set_resolve_mode\n");

            tstatus=sec_rgy_status_ok;
            break;
            }
#endif
         case RGY_WAIT_UNTIL_CONSISTENT:
            {
            sec_rgy_wait_until_consistent(context, &tstatus);

            TSHMessage (ctx, MOTRACE,
               "sec_rgy_wait_until_consistent status: %d\n", tstatus);

            TSHStatus (ctx, SETUP,
                  "sec_rgy_wait_until_consistent failed\n", rpc_s_ok, tstatus);

            break;
            }

#ifdef FOO
         case RGY_SET_CACHE_HANDLER_ON:
            {
            sec_rgy_cache_set_refresh_handler(context, cache_refresh_handler, 0,0,0 );

            TSHMessage (ctx, MOTRACE,
               "sec_rgy_cache_set_refresh_handler\n" );


            tstatus=sec_rgy_status_ok;
            break;
            }



         case RGY_SET_CACHE_HANDLER_OFF:
            {
            sec_rgy_cache_set_refresh_handler(context, NULL, 0,0,0 );

            TSHMessage (ctx, MOTRACE,
               "sec_rgy_cache_set_refresh_handler\n" );

            tstatus=sec_rgy_status_ok;
            break;
            }
#endif

        case RGY_CURSOR_RESET:
            {
            sec_rgy_cursor_reset( &cursor );
            TSHMessage (ctx, MOTRACE, "sec_rgy_cursor_reset\n" );

            strcpy(scope,"");
            
            sec_rgy_pgo_get_next ( context,
                                  (sec_rgy_domain_t) sec_rgy_domain_person,
                                   scope,
                                  &cursor,
                                  &itemInfo1,
                                   name,
                                  &tstatus );

            TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_get_next status: %d\n", tstatus);

            if ( TSHStatus (ctx, TEST, "sec_rgy_get_next failed\n", rpc_s_ok, tstatus) )
                break;

            TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_get_next name: %s\n", name);

            sec_rgy_cursor_reset( &cursor );
            TSHMessage (ctx, MOTRACE, "sec_rgy_cursor_reset\n" );

            sec_rgy_pgo_get_next ( context,
                                  (sec_rgy_domain_t) sec_rgy_domain_person,
                                   scope,
                                  &cursor,
                                  &itemInfo2,
                                   name,
                                  &tstatus );


            TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_get_next status: %d\n", tstatus);

            if ( TSHStatus (ctx, TEST, "sec_rgy_get_next failed\n", rpc_s_ok, tstatus) )
                break;

            TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_get_next name: %s\n", name);

         if (!uuid_equal (&itemInfo1.id, &itemInfo2.id, &tstatus) ||
             itemInfo1.unix_num != itemInfo2.unix_num ||
             itemInfo1.quota != itemInfo2.quota ||
             itemInfo1.flags != itemInfo2.flags ||
             strcmp (itemInfo1.fullname, itemInfo2.fullname))
                {
                tstatus=TSH_TEST_FAILED;
                TSHStatus (ctx, TEST, "sec_rgy_cursor_reset failed: cursor did not reset\n", rpc_s_ok,tstatus ) ;
                }

             break;
            }
        default:
	break;
        }
    
   sec_rgy_acct_delete (context, &ACCT_LOGIN_NAME, &cstatus);

   TSHMessage (ctx, MOTRACE, "sec_rgy_acct_delete status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_acct_delete failed\n", rpc_s_ok, cstatus))

DELETE_ORG_MEMBER_EXIT:
    DeleteOrgMember(context,  (sec_rgy_name_t *) ACCT_ORG_NAME,  (sec_rgy_name_t *) ACCT_PERSON_NAME );

DELETE_GROUP_MEMBER_EXIT:
    DeleteGroupMember(context,  (sec_rgy_name_t *) ACCT_GROUP_NAME,  (sec_rgy_name_t *) ACCT_PERSON_NAME );

DELETE_ORG_EXIT:

   sec_rgy_pgo_delete (context, (sec_rgy_domain_t)sec_rgy_domain_org,
                       (sec_rgy_name_t *)ACCT_ORG_NAME, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete (cleanup org) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete (org) failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;


DELETE_GROUP_EXIT:

   sec_rgy_pgo_delete (context, (sec_rgy_domain_t)sec_rgy_domain_group,
                       (sec_rgy_name_t *)ACCT_GROUP_NAME, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete (cleanup group) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete (group) failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;


DELETE_PERSON_EXIT:

   sec_rgy_pgo_delete (context, (sec_rgy_domain_t)sec_rgy_domain_person,
                       (sec_rgy_name_t *)ACCT_PERSON_NAME, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_pgo_delete (cleanup person) status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_pgo_delete (person) failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

RGY_CLOSE_EXIT:

   sec_rgy_site_close (context, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_site_close status: %d\n", cstatus);

   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;


   return (tstatus);
   }

