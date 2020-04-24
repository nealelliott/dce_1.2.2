/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ns_profile_elt_inq.c,v $
 * Revision 1.1.536.2  1996/02/17  23:21:28  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:42  marty]
 *
 * Revision 1.1.536.1  1995/12/11  20:07:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:05  root]
 * 
 * Revision 1.1.534.1  1994/01/21  22:40:23  cbrooks
 * 	RPC Code Cleanup-Initial Submission
 * 	[1994/01/21  20:16:28  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/11  23:37:20  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:10:01  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/07  13:20:41  zeliff
 * 	Removed duplicate OSF copyright markers
 * 	[1992/12/06  00:44:42  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/09  16:20:39  smith
 * 	Added inqctx_flag flag, changed arg  check to argc < ARGS_REQUIRED,
 * 	and changed case for rpc_s_profile_not_found to set the test_profile_name
 * 	from "foo" to "..."t
 * 
 * 	Changed test_if_spec.vers from 0x00080001 to test_if_spec.vers = 0x00000000.
 * 	Added TSHStatus for rpc_ns_profile_elt_inq_begin.
 * 	Added TSHMessage invalid inquiry context.
 * 	[1992/07/09  16:19:44  smith]
 * 
 * Revision 1.1  1992/01/19  03:39:10  devrcs
 * 	Initial revision
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


ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
ndr_long_int      subcase;		/* argv[0], subcase to run */
error_status_t    expected_status,	/* argv[1], expected return from test */
                  test_status = rpc_s_ok,/* status value from rpc calls... */
                  cleanup_status = rpc_s_ok,	/* status value from cleanup */
                  all_cleanup_status,	/* status value from cleanup */
                  expected_status_begin, 
                  expected_status_next, 
                  expected_status_done;
unsigned32        i, inqctx_flag=0,
                  elt_priority,
                  test_profile_name_syntax,
                  test_member_name_syntax,
                  test_vers_option,
                  test_inquiry_type,
                  test_interface_id,
                  test_type_begin, 
                  test_type_next, 
                  test_type_done;
unsigned_char_t	  test_member_name[DNS_FNAME_MAX],
                  test_profile_name[DNS_FNAME_MAX];        
unsigned_char_p_t test_annotation,
                  test_export_name,
                  elt_member,
                  elt_annotation;
rpc_if_id_t       test_if_id;
rpc_ns_inq_handle_t inquiry_context;
rpc_binding_vector_p_t binding_vector;
rpc_if_rep_t      test_if_spec;


/*  check the number of arguments passed in.  They should be exactly the   
 *  required number when you come into this routine. Looking for a subcase,
 *  a bogus expected_status, and a hostname.
 */
if(argc < ARGS_REQUIRED)
    {
    TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
        ARGS_REQUIRED, argc);
    return(TSH_BAD_ARG_COUNT);
    }


/*  atol() the input args...
 */
subcase                  = atol((char *)argv[0]);
expected_status          = atol((char *)argv[1]);
test_profile_name_syntax = atol((char *)argv[2]);
strcpy(test_profile_name, "/.:/"); 
strcat(test_profile_name, argv[3]);
test_inquiry_type        = atol((char *)argv[4]);
test_interface_id        = atol((char *)argv[5]);
test_vers_option         = atol((char *)argv[6]);
test_member_name_syntax  = atol((char *)argv[7]);
strcpy(test_member_name, "/.:/");
strcat(test_member_name, argv[8]);


/* 
 * Each of the BEGIN, NEXT, DONE trio has its own subcase
 */
switch(subcase)
    {
    case PROFILE_ELT_INQ_BEGIN:
        TSHMessage(ctx, MOINFO, "running subcase PROFILE_ELT_INQ_BEGIN\n");
        test_type_begin = TEST; 
        expected_status_begin = expected_status;

        test_type_next  = CLNUP;
        expected_status_next  = rpc_s_ok;

        test_type_done  = CLNUP;
        expected_status_done  = rpc_s_ok;
        break;

    case PROFILE_ELT_INQ_NEXT:
        TSHMessage(ctx, MOINFO, "running subcase PROFILE_ELT_INQ_NEXT\n");
        test_type_begin = SETUP;
        expected_status_begin = rpc_s_ok;

        test_type_next  = TEST;
        expected_status_next  = expected_status;

        test_type_done  = CLNUP;
        expected_status_done  = rpc_s_ok;
        break;

    case PROFILE_ELT_INQ_DONE:
        TSHMessage(ctx, MOINFO, "running subcase PROFILE_ELT_INQ_DONE\n");
        test_type_begin = SETUP;
        expected_status_begin = rpc_s_ok;

        test_type_next  = SETUP;
        expected_status_next  = rpc_s_ok;

        test_type_done  = TEST;
        expected_status_done  = expected_status;
        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", subcase);
        return(TSH_UNKNOWN_SUBCASE);
    }


/*
 *  Adjust the values passed in as necessary
 */
if(!strcmp((char *)test_profile_name, (char *)"<NULL>"))
    strcpy(test_profile_name, NULL);
if(!strcmp((char *)test_member_name,  (char *)"<NULL>"))
    strcpy(test_member_name, NULL);
if(test_interface_id != NULL_INTERFACE)
    {
    uuid_create (&(test_if_spec.id), &test_status);
    if(TSHStatus(ctx, SETUP, "uuid_create() failed", 
        test_status, rpc_s_ok))
    return(test_status);

    test_if_id.vers_major = 1;
    test_if_id.vers_minor = test_interface_id;
    }


if(test_status == rpc_s_ok)
    {
    /*
     *  Set up the name server database with some entries to test against.
     */
    test_status = tsh_util_ns_export(ctx, &test_if_spec, NULL);
    TSHStatus(ctx, SETUP, "name space setup failed", test_status, rpc_s_ok);
    }


/* 
 *  if the setup was successful,  run the requested subcase
 */
if(test_status == rpc_s_ok && expected_status_begin != rpc_s_profile_not_found)
    {
    /*
     * Now, create a profile of members for the above exported interfaces.
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
        "interface_p7", 
        &test_status);
    TSHStatus(ctx, SETUP, "rpc_ns_profile_elt_add() failed", 
        test_status, rpc_s_ok);
    }


if(test_status == rpc_s_ok && expected_status_begin != rpc_s_profile_not_found)
    {
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
        "interface_p3", 
        &test_status);
    TSHStatus(ctx, SETUP, "rpc_ns_profile_elt_add() failed", 
        test_status, rpc_s_ok);
    }
    
    
if(test_status == rpc_s_ok && expected_status_begin != rpc_s_profile_not_found)
    {
    /*
     * The highest priority (0) being the current version (designated by 1.8)
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
        "interface_p0", 
        &test_status);
    TSHStatus(ctx, SETUP, "rpc_ns_profile_elt_add() failed", 
        test_status, rpc_s_ok);
    }

        
if(test_status == rpc_s_ok && expected_status_begin != rpc_s_profile_not_found)
    {
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
        "interface_p0_dup", 
        &test_status);
    TSHStatus(ctx, SETUP, "rpc_ns_profile_elt_add() failed", 
        test_status, rpc_s_ok);
    }



if(test_status == rpc_s_ok && expected_status_begin != rpc_s_profile_not_found)
    {
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
        "interface_p1", 
        &test_status);
    TSHStatus(ctx, SETUP, "rpc_ns_profile_elt_add() failed", 
        test_status, rpc_s_ok);
    }
test_profile_name_syntax = atol((char *)argv[2]);
test_member_name_syntax  = atol((char *)argv[7]);

if(test_status == rpc_s_ok)
    {
    /*
     * Now do a lookup for all interfaces compatible with test_if_id
     * V1.8 and check the priorities in which they are returned.
     */
    switch(expected_status_begin)
    {
       case rpc_s_profile_not_found:
            strcpy(test_profile_name, "..." );
            break;

       case rpc_s_invalid_vers_option:
            test_vers_option = 32;
            break;

       case rpc_s_unsupported_name_syntax:
            test_profile_name_syntax = rpc_c_ns_syntax_unknown;
            break;
    }

    test_if_spec.vers = 0x00000000;
    rpc_ns_profile_elt_inq_begin (
        test_profile_name_syntax, 
        test_profile_name, 
        test_inquiry_type,
        &test_if_id, 
        test_vers_option, 
        test_member_name_syntax,
        test_member_name, 
        &inquiry_context, 
        &test_status);
TSHMessage(ctx,MOINFO,"rpc_ns_profile_elt_inq_begin"
  "\ntest_profile_name=%s test_member_name=%s test_status=%d estatus=%d\n",
   test_profile_name,test_member_name,test_status,expected_status_begin);
    TSHStatus(ctx, test_type_begin, "rpc_ns_profile_elt_inq() failed", 
        test_status, expected_status_begin);
    }
    if((test_status == expected_status_begin) && 
       (subcase == PROFILE_ELT_INQ_BEGIN))
       goto FINISHED;


i = 0;
if(test_status == rpc_s_ok && subcase == PROFILE_ELT_INQ_NEXT)
  {
    TSHMessage(ctx, MOINFO, "performed a successful ns lookup\n");

    /*
     * Perform the lookup based on the specified lookup requirements.
     */
    do
    {
        if(argc > ARGS_REQUIRED)
          {
            inquiry_context=NULL;
            TSHMessage(ctx,MOINFO,"invalid inquiry context test\n");
          }
        rpc_ns_profile_elt_inq_next (
            inquiry_context, 
            &test_if_id, 
            &elt_member,
            &elt_priority,
            &elt_annotation,
            &test_status);

    if(test_status == expected_status)
      {
        i=50;
        inqctx_flag=1;
      }
    TSHMessage(ctx, MOINFO, "rpc_ns_profile_elt_inq_next test_status=%d\n",
               test_status);
            ++i;

            if (test_status == rpc_s_ok)
            {
                rpc_string_free (&elt_member,     &test_status);
                rpc_string_free (&elt_annotation, &test_status);
            }
            else
                break;
        } while (expected_status_next != rpc_s_ok && i < 10);
  }
  if((test_status == expected_status_next) && 
     (subcase == PROFILE_ELT_INQ_NEXT))
     goto FINISHED;

    if((test_status == rpc_s_ok || i > 0) && (inqctx_flag == 0))
    {
       if (subcase == PROFILE_ELT_INQ_DONE)
       {
          if(argc > ARGS_REQUIRED)
            {
              inquiry_context=NULL;
              TSHMessage(ctx,MOINFO,"invalid inquiry context test\n");
            }
          rpc_ns_profile_elt_inq_done (&inquiry_context, &test_status);
          TSHMessage(ctx,MOINFO,"rpc_ns_profile_elt_inq_done test_status=%d\n",
                     test_status);
          if((test_status == expected_status) && 
             (subcase == PROFILE_ELT_INQ_DONE))
             goto FINISHED;
          TSHStatus(ctx, test_type_done, "rpc_ns_profile_elt_inq_done() failed",
           test_status, expected_status_done);
       }
       else
       {
          if(argc > ARGS_REQUIRED)
            {
              inquiry_context=NULL;
              TSHMessage(ctx,MOINFO,"invalid inquiry context test\n");
            }
          rpc_ns_profile_elt_inq_done (&inquiry_context, &cleanup_status);
        TSHMessage(ctx,MOINFO,"rpc_ns_profile_elt_inq_done cleanup_status=%d\n",
                   cleanup_status);
          if((test_status == expected_status) && 
             (subcase == PROFILE_ELT_INQ_DONE))
             goto FINISHED;
          TSHStatus(ctx, test_type_done, "rpc_ns_profile_elt_inq_done() failed",
           cleanup_status, rpc_s_ok);
       }
    }

all_cleanup_status = cleanup_status;
test_profile_name_syntax = rpc_c_ns_syntax_dce;
if(expected_status_begin != rpc_s_profile_not_found)
{
    rpc_ns_profile_delete (test_profile_name_syntax, test_profile_name, 
                                                   &cleanup_status);
    TSHStatus(ctx, CLNUP, "rpc_ns_profile_delete() failed", 
				     cleanup_status, rpc_s_ok);
}
if (all_cleanup_status == rpc_s_ok)
    all_cleanup_status = cleanup_status;
cleanup_status = tsh_util_ns_unexport(ctx);
if(TSHStatus(ctx, SETUP, "name space cleanup failed", cleanup_status, rpc_s_ok))
    TSHMessage(ctx, MOWARNING, 
   "this test may not have removed all of the name space entries it created\n");
if (all_cleanup_status == rpc_s_ok)
    all_cleanup_status = cleanup_status;
if(test_status != expected_status)
   return(FAILURE);
if (all_cleanup_status != rpc_s_ok)
   return(all_cleanup_status);
FINISHED:
return(rpc_s_ok);
}
