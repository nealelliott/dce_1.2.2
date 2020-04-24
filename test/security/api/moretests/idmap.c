/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: idmap.c,v $
 * Revision 1.1.4.2  1996/02/17  23:25:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:34  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:48:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:16  root]
 * 
 * Revision 1.1.2.3  1993/01/29  21:11:06  cuti
 * 	Add id_group test
 * 	[1993/01/28  16:02:28  cuti]
 * 
 * Revision 1.1.2.2  1992/12/31  17:28:46  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:54:18  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:55  devrcs
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
 *	Extracted	1/4/92 16:56:22
 *	from delta on	11/13/91 14:20:22
 *
 *  $Log: idmap.c,v $
 * Revision 1.1.4.2  1996/02/17  23:25:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:34  marty]
 *
 * Revision 1.1.5.2  1996/02/17  22:43:34  marty
 * 	Update OSF copyright year
 *
 * Revision 1.1.4.1  1995/12/11  21:48:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:16  root]
 *
 * Revision 1.1.3.2  1995/12/11  20:52:16  root
 * 	Submit OSF/DCE 1.2.1
 *
 * Revision 1.1.2.3  1993/01/29  21:11:06  cuti
 * 	Add id_group test
 * 	[1993/01/28  16:02:28  cuti]
 *
 * Revision 1.1.3.2  1993/01/28  16:02:28  cuti
 * 	Add id_group test
 *
 * Revision 1.1.2.2  1992/12/31  17:28:46  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:54:18  htf]
 *
 * Revision 1.1.1.2  1992/12/31  16:54:18  htf
 * 	Embedding copyright notice
 *
 * Revision 1.1  1992/01/19  04:07:55  devrcs
 * Initial revision
 *
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/idmap.c, , dce_osf, dce.75d";
#endif

/* Include files */

#include <idmap.h>
#include <dce/secidmap.h>
#include <dce/pgo.h>


#define PRINCIPLE_NAME "user"



CtxBlock          *lCtx;

/* Args required by the test program. */

#define ARGS_REQUIRED 3

ndr_long_int TestFunction (ctx, argc, argv)

/* Formal parameters */

   CtxBlock          *ctx;
   ndr_long_int      argc;
   unsigned_char_p_t argv[];

{

/* Local variables */

   ndr_long_int		subcase;	/* argv[0], subcase to run */
   error_status_t	estatus;	/* argv[1], expected return from test */
   sec_rgy_name_t	node_name;	/* gethostname */
   sec_rgy_pname_t	site_name;	/* sec_rgy_site_open */
   sec_rgy_handle_t	context;	/* sec_rgy_site_open */


   sec_rgy_name_t   global_cell_name;
   sec_rgy_name_t   cell_name;
   uuid_t           cell_uuid;
   sec_rgy_name_t   princ_name;
   uuid_t           princ_uuid;
   sec_rgy_name_t   group_name;
   uuid_t           group_uuid;


   sec_rgy_name_t   global_name_ret;
   sec_rgy_name_t   cell_name_ret;
   sec_rgy_name_t   princ_name_ret;
   sec_rgy_name_t   group_name_ret;


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

    if ( (tstatus= RegistryOpen(&context)) != sec_rgy_status_ok )
         return tstatus;
#ifdef FOO
   memcpy ((char *)plain_passwd,
           ACCT_PERSON_PASSWD,
           sizeof (sec_passwd_str_t));

    if ( (tstatus=AddPerson(context,  (sec_rgy_name_t *) ACCT_PERSON_NAME,  0,  1001,  1,  sec_rgy_pgo_flags_none,  (sec_rgy_pname_t *)ACCT_PERSON_FULLNAME ) ) )
        goto RGY_CLOSE_EXIT;
    if ( (tstatu\s=AddGroup(context,  (sec_rgy_name_t *)ACCT_GROUP_NAME,  0,  1001,  1,  sec_rgy_pgo_flags_none,  (sec_rgy_pname_t *)ACCT_GROUP_FULLNAME ) ) )
        goto DELETE_PERSON_EXIT;
    if ( (tstatus=AddOrg(context,  (sec_rgy_name_t *)ACCT_ORG_NAME,  0,  1001,  1,  sec_rgy_pgo_flags_none,  (sec_rgy_pname_t *)ACCT_ORG_FULLNAME )  ) )
        goto DELETE_GROUP_EXIT;

    if ( (tstatus=AddGroupMember(context,  (sec_rgy_name_t *) ACCT_GROUP_NAME,  (sec_rgy_name_t *) ACCT_PERSON_NAME ) ) )
        goto DELETE_GROUP_MEMBER_EXIT;

    if ( (tstatus=AddOrgMember(context,    (sec_rgy_name_t *) ACCT_ORG_NAME,  (sec_rgy_name_t *) ACCT_PERSON_NAME ) ) )
        goto DELETE_ORG_MEMBER_EXIT;

    if ( (tstatus=CreateAccount(context, &ACCT_LOGIN_NAME, (sec_rgy_pname_t *)ACCT_GECOS, (sec_rgy_pname_t *)ACCT_HOMEDIR, (sec_rgy_pname_t *)ACCT_SHELL, plain_passwd )) )
        goto DELETE_ORG_EXIT;
#endif

	 strcpy(global_cell_name, argv[2]);

    switch (subcase)
        {
        case ID_GEN_NAME_OK:
        case ID_PARSE_NAME_OK:

             sec_id_parse_name(context, global_cell_name,
                              cell_name, &cell_uuid,
                              princ_name, &princ_uuid,
                              &tstatus );


            TSHMessage (ctx, MOTRACE,
               "sec_id_parse_name cell name: %s, princ. name: %s, status: %d\n", 
                                                                cell_name,
                                                                princ_name,
                                                                tstatus);

            if (TSHStatus (ctx, TEST, "sec_id_parse_name failed\n", estatus, tstatus) )
                {
                TSHMessage (ctx, MOTRACE, "expected status %d, returned status %d\n",
                                         estatus, tstatus );
                break;
                }


            if ( subcase == ID_GEN_NAME_OK)
                {
                sec_id_gen_name(context, &cell_uuid, &princ_uuid, global_name_ret,
                                cell_name_ret, princ_name_ret, &tstatus);

                TSHMessage (ctx, MOTRACE,
                   "sec_id_gen_name global_name: %s, cell name: %s, princ. name: %s, status: %d\n", 
                                                                    global_name_ret,
                                                                    cell_name_ret,
                                                                    princ_name_ret,
                                                                    tstatus);

                if (TSHStatus (ctx, TEST, "sec_id_gen_name failed\n", estatus, tstatus) )
                    {
                    TSHMessage (ctx, MOTRACE, "expected status %d, returned status %d\n",
                                             estatus, tstatus );
                    break;
                    }

/*                if ( strcmp(global_cell_name, global_name_ret) )
                    {
                    TSHStatus (ctx, TEST, "sec_id_gen_name failed. global cell name mismatch\n", TSH_TEST_FAILED, 0);
                    tstatus=TSH_TEST_FAILED;
                    }
*/
                if ( strcmp(cell_name, cell_name_ret) )
                    {
                    TSHStatus (ctx, TEST, "sec_id_gen_name failed. cell name mismatch\n", TSH_TEST_FAILED, 0);
                    tstatus=TSH_TEST_FAILED;
                    }
          
                if ( strcmp(princ_name, princ_name_ret) )
                    {
                    TSHStatus (ctx, TEST, "sec_id_gen_name failed. principle name mismatch\n", TSH_TEST_FAILED, 0);
                    tstatus=TSH_TEST_FAILED;
                    } 
			}
         
			break;
  
      case ID_GEN_GROUP_OK:
        case ID_PARSE_GROUP_OK:  
  
           sec_id_parse_group(context, global_cell_name,
                              cell_name, &cell_uuid,
                              group_name, &group_uuid,
                              &tstatus );


            TSHMessage (ctx, MOTRACE,
               "sec_id_parse_group cell name: %s, group. name: %s, status: %d\n", 
                                                                cell_name,
                                                                group_name,
                                                                tstatus);

            if (TSHStatus (ctx, TEST, "sec_id_parse_group failed\n", estatus, tstatus) )
                {
                TSHMessage (ctx, MOTRACE, "expected status %d, returned status %d\n",
                                         estatus, tstatus );
                break;
                }


            if ( subcase == ID_GEN_GROUP_OK)
                {
                sec_id_gen_group(context, &cell_uuid, &group_uuid, global_name_ret,
                                cell_name_ret, group_name_ret, &tstatus);

                TSHMessage (ctx, MOTRACE,
                   "sec_id_gen_group global_name: %s, cell name: %s, princ. name: %s, status: %d\n", 
                                                                    global_name_ret,
                                                                    cell_name_ret,
                                                                    group_name_ret,
                                                                    tstatus);

                if (TSHStatus (ctx, TEST, "sec_id_gen_group failed\n", estatus, tstatus) )
                    {
                    TSHMessage (ctx, MOTRACE, "expected status %d, returned status %d\n",
                                             estatus, tstatus );
                    break;
                    }

/*                if ( strcmp(global_cell_name, global_name_ret) )
                    {
                    TSHStatus (ctx, TEST, "sec_id_gen_group failed. global cell name mismatch\n", TSH_TEST_FAILED, 0);
                    tstatus=TSH_TEST_FAILED;
                    }
*/
                if ( strcmp(cell_name, cell_name_ret) )
                    {
                    TSHStatus (ctx, TEST, "sec_id_gen_group failed. cell name mismatch\n", TSH_TEST_FAILED, 0);
                    tstatus=TSH_TEST_FAILED;
                    }
          
                if ( strcmp(princ_name, princ_name_ret) )
                    {
                    TSHStatus (ctx, TEST, "sec_id_gen_group failed. principle name mismatch\n", TSH_TEST_FAILED, 0);
                    tstatus=TSH_TEST_FAILED;
                    }
             }
 	    break;

        default:
  		break;
        }    

#ifdef FOO
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

#endif
   return (tstatus == estatus) ? TSH_SUCCESS:TSH_TEST_FAILED;
   }

