/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ns_profile_elt_add.c,v $
 * Revision 1.1.6.2  1996/02/17  23:21:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:39  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:07:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:03  root]
 * 
 * Revision 1.1.4.4  1993/03/21  20:54:49  weisman
 * 	Fix for OT 7555, initialize test_if_id.  Also cleaned up several
 * 	gcc warnings.
 * 	[1993/03/21  20:49:21  weisman]
 * 
 * Revision 1.1.4.3  1993/01/11  23:37:17  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:09:56  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/07  13:20:35  zeliff
 * 	Removed duplicate OSF copyright markers
 * 	[1992/12/06  00:44:38  zeliff]
 * 
 * Revision 1.1.2.4  1992/07/21  20:56:49  dceguest
 * 	Changed another "foo" to "/.:/foo" since a fully qualified name is
 * 	required.
 * 	[1992/07/21  15:26:00  dceguest]
 * 
 * Revision 1.1.2.3  1992/07/17  21:30:59  dceguest
 * 	Changed foo /.:/foo
 * 	[1992/07/17  21:30:20  dceguest]
 * 
 * Revision 1.1.2.2  1992/07/09  17:20:12  smith
 * 	     Add a define for  PROFILE_INVALID_IFID, add status
 * 	     check for tsh_util_ns_export, included TSHMessage
 * 	     for each case, added case to check for an invalid
 * 	     profile if_id, and removed TSHStatus for
 * 	     space cleanup failure.
 * 	     [1992/07/09  17:19:31  smith]
 * 
 * Revision 1.1  1992/01/19  03:39:08  devrcs
 * 	     Initial revision
 * 
 * $EndLog$
 */
/*
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/


#include <math.h>
#include <tsh_common.h>
#include <tsh_ns.h>


/*  Args required by the test program...
 */
#define ARGS_REQUIRED 9
#define PROFILE_INVALID_IFID 10


ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
char *            argv[];
{
    ndr_long_int      subcase;		/* argv[0], subcase to run */
    error_status_t    expected_status;	/* argv[1], expected return from test */
    error_status_t    test_status,      /* status value from rpc calls... */
    cleanup_status;                     /* status value from cleanup */
    error_status_t    inquire_status;	/* status value from inquire calls... */
    unsigned32        i, 
    test_profile_name_syntax,
    test_member_name_syntax,
    test_priority,
    test_interface_id,
    test_state = rpc_s_ok;
    unsigned_char_t	  test_member_name[DNS_FNAME_MAX],
    test_profile_name[DNS_FNAME_MAX];        
    unsigned_char_p_t test_annotation,
    test_export_name,
    string_uuid,
    string_binding,
    elt_member,
    elt_annotation;
    rpc_if_id_t       test_if_id;
    rpc_ns_inq_handle_t inquiry_context;
    rpc_ns_lookup_handle_t lookup_context;
    rpc_binding_vector_p_t binding_vector;

    rpc_if_rep_t      test_if_spec;


    /*  check the number of arguments passed in.  They should be exactly the   
     *  required number when you come into this routine. Looking for a subcase,
     *  a bogus expected_status, and a hostname.
     */
    if(argc != ARGS_REQUIRED)
    {
        TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
                   ARGS_REQUIRED, argc);
        return(TSH_BAD_ARG_COUNT);
    }


    /*  atol() the input args...
     */
    subcase                  = atol(argv[0]);
    expected_status          = atol(argv[1]);
    test_profile_name_syntax = atol(argv[2]);
    strcpy(test_profile_name, "/.:/");
    strcat(test_profile_name, argv[3]);
    test_interface_id        = atol(argv[4]);
    test_member_name_syntax  = atol(argv[5]);
    strcpy(test_member_name, "/.:/");
    strcat(test_member_name, argv[6]);
    test_priority            = atol(argv[7]);
    test_annotation          = (unsigned_char_p_t) argv[8];


    /*
     *  Adjust the values passed in as necessary
     */
    if(!strcmp((char *)test_profile_name, (char *)"<NULL>"))
        strcpy(test_profile_name, NULL);
    if(!strcmp((char *)test_member_name,  (char *)"<NULL>"))
        strcpy(test_member_name, NULL);
    if(!strcmp((char *)test_annotation,   (char *)"<NULL>"))
        test_annotation = NULL;
    if(test_interface_id != NULL_INTERFACE)
    {
        uuid_create (&(test_if_spec.id), &test_status);
        if(TSHStatus(ctx, SETUP, "uuid_create() failed", 
                     test_status, rpc_s_ok))
            return(test_status);

        test_if_id.uuid = test_if_spec.id;
        test_if_id.vers_major = 1;
        test_if_id.vers_minor = test_interface_id;
    }


    /*
     *  Set up the name server database with some entries to test against.
     */
    test_status = tsh_util_ns_export(ctx, &test_if_spec, NULL);
    if(test_status != rpc_s_ok)
    {
        printf("status=%d\n",test_status);
        return(test_status);
    }
    if(!(TSHStatus(ctx, SETUP, "name space setup failed, aborting test", 
                   test_status, rpc_s_ok)))
    {

        /* 
         *  if the setup was successful,  run the requested subcase
         */
        switch(subcase)
        {
        case REPEAT_PROFILE_ELT_ADD:
            TSHMessage(ctx, MOINFO, "running subcase REPEAT_PROFILE_ELT_ADD\n");
    
            rpc_ns_profile_elt_add (
                                    test_profile_name_syntax, 
                                    test_profile_name, 
                                    &test_if_id, 
                                    test_member_name_syntax, 
                                    test_member_name, 
                                    test_priority, 
                                    test_annotation, 
                                    &test_status);
            TSHMessage(ctx, MOINFO, "REPEAT ADD test_status=%d estatus=%d\n",
                       test_status,expected_status);
            if(TSHStatus(ctx, SETUP, "rpc_ns_profile_elt_add() failed", 
                         test_status, rpc_s_ok))
                break;

            /*  Fall through to the normal case */

        case PROFILE_ELT_ADD:
        case PROFILE_ELT_REMOVE:
            if (subcase == PROFILE_ELT_ADD)
                TSHMessage(ctx, MOINFO, "running subcase PROFILE_ELT_ADD\n");
            else if (subcase == PROFILE_ELT_REMOVE)
                TSHMessage(ctx, MOINFO, "running subcase PROFILE_ELT_REMOVE\n");
    
            rpc_ns_profile_elt_add (
                                    test_profile_name_syntax, 
                                    test_profile_name, 
                                    &test_if_id, 
                                    test_member_name_syntax, 
                                    test_member_name, 
                                    test_priority, 
                                    test_annotation, 
                                    &test_status);
            TSHMessage(ctx, MOINFO, "ADD test_status=%d estatus=%d\n",
                       test_status,expected_status);
            if (subcase == PROFILE_ELT_REMOVE)
            {
                if(TSHStatus(ctx, SETUP, "rpc_ns_profile_elt_add() failed", 
                             test_status, rpc_s_ok))
                    break;
            }
            else
            {
                TSHStatus(ctx, TEST, "rpc_ns_profile_elt_add() failed", 
                          test_status, expected_status);
                if (test_status == expected_status)
                    test_status = rpc_s_ok;
                else
                    if (test_status == rpc_s_ok)
                        test_status = TSH_TEST_FAILED;
                break;
            }

        case PROFILE_REMOVE_ERRORS:
            if (subcase != PROFILE_ELT_REMOVE)
                TSHMessage(ctx, MOINFO, "running subcase PROFILE_REMOVE\n");
            if (expected_status == rpc_s_profile_element_not_found)
                strcpy(test_member_name , "/.:/foo");
    
            rpc_ns_profile_elt_remove (
                                       test_profile_name_syntax, 
                                       test_profile_name, 
                                       &test_if_id, 
                                       test_member_name_syntax, 
                                       test_member_name, 
                                       &test_status);
            TSHMessage(ctx, MOINFO,"REMOVE test_status=%d expected status=%d\n",
                       test_status,expected_status);
            TSHStatus(ctx, TEST, "rpc_ns_profile_elt_remove() failed", 
                      test_status, expected_status);
            if (test_status == expected_status)
                test_status = rpc_s_ok;
            else
                if (test_status == rpc_s_ok)
                    test_status = TSH_TEST_FAILED;

            break;

        case ORDERED_PROFILE_ELT_ADD:
            TSHMessage(ctx, MOINFO, "running subcase ORDERED_PROFILE_ELT_ADD\n");

            /*
             * Now, create a profile of members for the 
             * above exported interfaces.
             */
            test_profile_name_syntax = rpc_c_ns_syntax_dce;
            test_member_name_syntax = rpc_c_ns_syntax_dce;
            test_if_id.uuid = test_if_spec.id;

            test_export_name = (unsigned_char_p_t) "/.:/NSTEST_DIR/profile_test_export1_7";
            test_if_id.vers_major = 1;
            test_if_id.vers_minor = 7;
            rpc_ns_profile_elt_add (
                                    test_profile_name_syntax, 
                                    test_profile_name, 
                                    &test_if_id, 
                                    test_member_name_syntax, 
                                    test_export_name, 
                                    2, 
                                    (unsigned_char_p_t) "interface_p7", 
                                    &test_status);
            TSHMessage(ctx, MOINFO, "test_status=%d estatus=%d\n",
                       test_status,expected_status);
            if(TSHStatus(ctx, TEST, "rpc_ns_profile_elt_add() failed", 
                         test_status, expected_status))
                break;



            test_export_name = (unsigned_char_p_t) "/.:/NSTEST_DIR/profile_test_export1_3";
            test_if_id.vers_major = 1;
            test_if_id.vers_minor = 3;
            rpc_ns_profile_elt_add (
                                    test_profile_name_syntax, 
                                    test_profile_name, 
                                    &test_if_id, 
                                    test_member_name_syntax, 
                                    test_export_name, 
                                    3, 
                                    (unsigned_char_p_t) "interface_p3", 
                                    &test_status);
            TSHMessage(ctx, MOINFO, "test_status=%d estatus=%d\n",
                       test_status,expected_status);
            if(TSHStatus(ctx, TEST, "rpc_ns_profile_elt_add() failed", 
                         test_status, expected_status))
                break;
    
    
            /*
             * The highest priority (0) being the current 
             * version (designated by 1.8)
             */
            test_export_name = (unsigned_char_p_t) "/.:/NSTEST_DIR/profile_test_export1_8";
            test_if_id.vers_major = 1;
            test_if_id.vers_minor = 8;
            rpc_ns_profile_elt_add (
                                    test_profile_name_syntax, 
                                    test_profile_name, 
                                    &test_if_id, 
                                    test_member_name_syntax, 
                                    test_export_name, 
                                    0, 
                                    (unsigned_char_p_t) "interface_p0", 
                                    &test_status);
            TSHMessage(ctx, MOINFO, "test_status=%d estatus=%d\n",
                       test_status,expected_status);
            if(TSHStatus(ctx, TEST, "rpc_ns_profile_elt_add() failed", 
                         test_status, expected_status))
                break;
    
        
            /*
             * Set up a member name with an identical interface.
             */
            test_export_name = (unsigned_char_p_t) 
                "/.:/NSTEST_DIR/profile_test_export1_8_dup";
            rpc_ns_profile_elt_add (
                                    test_profile_name_syntax, 
                                    test_profile_name, 
                                    &test_if_id, 
                                    test_member_name_syntax, 
                                    test_export_name, 
                                    0, 
                                    (unsigned_char_p_t) "interface_p0_dup", 
                                    &test_status);
            TSHMessage(ctx, MOINFO, "test_status=%d estatus=%d\n",
                       test_status,expected_status);
            if(TSHStatus(ctx, TEST, "rpc_ns_profile_elt_add() failed", 
                         test_status, expected_status))
                break;
    
    
    
            test_export_name = (unsigned_char_p_t) "/.:/NSTEST_DIR/profile_test_export1_1";
            test_if_id.vers_major = 1;
            test_if_id.vers_minor = 1;
            rpc_ns_profile_elt_add (
                                    test_profile_name_syntax, 
                                    test_profile_name, 
                                    &test_if_id, 
                                    test_member_name_syntax, 
                                    test_export_name, 
                                    4, 
                                    (unsigned_char_p_t) "interface_p1", 
                                    &test_status);
            TSHMessage(ctx, MOINFO, "test_status=%d estatus=%d\n",
                       test_status,expected_status);
            if(TSHStatus(ctx, TEST, "rpc_ns_profile_elt_add() failed", 
                         test_status, expected_status))
                break;
    
    
            /*
             * Now do a lookup for all interfaces compatible with test_if_id
             * V1.8 and check the priorities in which they are returned.
             */
            test_if_spec.vers = 0x00080001;
            rpc_ns_binding_lookup_begin (
                                         test_profile_name_syntax, 
                                         test_profile_name, 
                                         (rpc_if_handle_t)&test_if_spec, 
                                         NULL, 
                                         0, 
                                         &lookup_context, 
                                         &test_status);
            TSHMessage(ctx, MOINFO, "test_status=%d estatus=%d\n",
                       test_status,expected_status);
            if(TSHStatus(ctx, TEST, "rpc_ns_binding_lookup_begin() failed", 
                         test_status, rpc_s_ok))
                break;
    
    
            /*
             * Do a check until there are no more entries.
             */
            do
            {
                rpc_ns_binding_lookup_next 
                    (lookup_context, &binding_vector, &inquire_status);
    
                if ((inquire_status != rpc_s_ok) &&
                    (inquire_status != rpc_s_no_more_bindings))
                {
                    TSHMessage(ctx, MOFAILURE, "rpc_ns_binding_lookup_next() failed\n");
                    test_state = inquire_status;
                }
                else
                {
                    TSHMessage(ctx, MOINFO, "performed a successful ns lookup\n");
    
                    if (inquire_status == rpc_s_ok)
                    {
                        for (i = 0; i < binding_vector->count; i++)
                        {
                            rpc_ns_binding_inq_entry_name (
                                                           binding_vector->binding_h[i],
                                                           rpc_c_ns_syntax_default, 
                                                           &elt_member, 
                                                           &test_status);
                            if(TSHStatus(ctx, TEST, "rpc_ns_binding_inq_entry_name() failed", 
                                         test_status, rpc_s_ok))
                                break;


                            TSHMessage (ctx, MOINFO, "binding from entry: %s \n", elt_member);

                            rpc_binding_to_string_binding (
                                                           binding_vector->binding_h[i],
                                                           &string_binding,
                                                           &test_status);
                            if(TSHStatus(ctx, TEST, "rpc_ns_binding_to_string_binding() failed", 
                                         test_status, rpc_s_ok))
                                break;

                    
                
                            rpc_string_free (&elt_member, &test_status);
                            rpc_string_free (&string_binding, &test_status);
                        }
                    }
                }
            }
            while (inquire_status != rpc_s_no_more_bindings);

            rpc_ns_binding_lookup_done (&lookup_context, &test_status);
            TSHStatus(ctx, TEST, "rpc_ns_binding_lookup_done() failed", 
                      test_status, rpc_s_ok);

            break;

        case PROFILE_INVALID_IFID:
            if (subcase != PROFILE_ELT_REMOVE)
                TSHMessage(ctx, MOINFO, "running subcase INVALID_IF_ID\n");
            if (expected_status == rpc_s_profile_element_not_found)
                strcpy(test_member_name , "/.:/foo");

            test_if_id.vers_major=(unsigned16)NULL;
            test_if_id.vers_minor=(unsigned16)NULL;
            rpc_ns_profile_elt_remove (
                                       test_profile_name_syntax,
                                       test_profile_name,
                                       &test_if_id,
                                       test_member_name_syntax,
                                       test_member_name,
                                       &test_status);
            TSHMessage(ctx, MOINFO,"REMOVE test_status=%d expected status=%d\n",
                       test_status,expected_status);
            TSHStatus(ctx, TEST, "rpc_ns_profile_elt_remove() failed",
                      test_status, expected_status);
            if (test_status == expected_status)
                test_status = rpc_s_ok;
            else
                if (test_status == rpc_s_ok)
                    test_status = TSH_TEST_FAILED;

            break;


        default:
            TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", subcase);
            test_status = TSH_UNKNOWN_SUBCASE;
        }
    }

    /*
      cleanup_status = tsh_util_ns_unexport(ctx);
      
      if(TSHStatus(ctx, SETUP, "name space cleanup failed", cleanup_status, rpc_s_ok))
      {
      TSHMessage(ctx, MOWARNING, 
      "this test may not have removed all of the name space entries it created\n");
      if(test_status == rpc_s_ok)
      test_status = cleanup_status;
      }
      
      */

    if(test_status == expected_status)
        return(rpc_s_ok);
    else
        return(test_status);
}

