/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ns_entry_object_inq.c,v $
 * Revision 1.1.8.2  1996/02/17  23:21:16  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:24  marty]
 *
 * Revision 1.1.8.1  1995/12/11  20:07:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:56  root]
 * 
 * Revision 1.1.4.4  1993/01/27  17:29:41  bbelch
 * 	Restore truncated files
 * 	[1993/01/27  17:27:33  bbelch]
 * 
 * Revision 1.1.4.3  1993/01/25  18:05:57  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:38:28  bbelch]
 * 
 * Revision 1.1.4.2  1993/01/11  23:36:55  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:09:32  bbelch]
 * 
 * Revision 1.1.2.2  1992/07/08  19:53:30  smith
 * 	Added "break" if (test_status == expected_status) while
 * 	expected_status != rpc_s_ok.
 * 	[1992/07/08  19:52:21  smith]
 * 
 * Revision 1.1  1992/01/19  03:35:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
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
#include <tsh_ns_utility.h>


/*  Args required by the test program...
 */
#define ARGS_REQUIRED 4

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
ndr_long_int      subcase;		/* argv[0], subcase to run */
error_status_t    expected_status;	/* argv[1], expected return from test */
error_status_t    cleanup_status;	/* status value from rpc calls... */
error_status_t    all_clean_status;	/* status value from rpc calls... */
error_status_t    test_status;		/* status value from rpc calls... */
unsigned32        test_entry_name_syntax;
unsigned_char_t   test_entry_name[DNS_FNAME_MAX];        /* from argv[3] */
rpc_if_id_t       test_if_id;
rpc_ns_inq_handle_t inquiry_context;

artificial_vector some_obj_vector;
uuid_t            a_uuid;
rpc_if_rep_t      test_if_spec;


/*  check the number of arguments passed in.  They should be exactly the   
 *  required number when you come into this routine. Looking for a subcase
 *  and a bogus expected_status.
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
test_entry_name_syntax = atol(argv[2]);
strcpy(test_entry_name, "/.:/");
strcat(test_entry_name, argv[3]);

cleanup_status = rpc_s_ok;

/*
 *  Set up the name server database with some entries to test against.
 */

	test_status = tsh_util_uuid_vec_create(ctx, &some_obj_vector);
	if (TSHStatus(ctx, SETUP, "object vector setup failed, aborting test", 
                    test_status, rpc_s_ok))
   		 return(test_status);
           
	if (expected_status != rpc_s_obj_uuid_not_found) 
      test_status = tsh_util_ns_export(ctx,&test_if_spec,&some_obj_vector);
	/* no export will create  rpc_s_obj_uuid_not_found status */
	
	
	if (TSHStatus(ctx, SETUP, "name space setup failed, aborting test", 
                    test_status, rpc_s_ok))
    	return(test_status);

    /* 
     *   run the requested subcase
     */
    switch(subcase)
    {
        case NS_ENTRY_OBJECT_INQ_BEGIN:
            /* do the begin test here */
            rpc_ns_entry_object_inq_begin(
            (expected_status == rpc_s_unsupported_name_syntax) ?
              rpc_c_ns_syntax_unknown : test_entry_name_syntax, 
              test_entry_name, &inquiry_context, &test_status);
     	    TSHMessage(ctx,MOTRACE,"rpc_ns_entry_object_inq_begin status: %d\n",
                                                            test_status);
            TSHStatus(ctx, TEST,"rpc_ns_entry_object_inq_begin() failed",
                                             test_status, expected_status);
            if (test_status == rpc_s_ok)
            {
                rpc_ns_entry_object_inq_done(&inquiry_context,&cleanup_status);
                TSHStatus(ctx, TEST,"rpc_ns_entry_object_inq_begin() failed",
                                             cleanup_status, rpc_s_ok);
            }
            break;

        case NS_ENTRY_OBJECT_INQ_DONE:
            /* do the done test here */
            rpc_ns_entry_object_inq_begin(test_entry_name_syntax, 
              test_entry_name, &inquiry_context, &test_status);
     	    TSHMessage(ctx,MOTRACE,"rpc_ns_entry_object_inq_begin status: %d\n",
                                                                 test_status);
            if (TSHStatus(ctx, TEST,"rpc_ns_entry_object_inq_begin() failed",
                                             test_status, rpc_s_ok))
                break;
            rpc_ns_entry_object_inq_done(&inquiry_context,&test_status);
     	    TSHMessage(ctx,MOTRACE, "rpc_ns_entry_object_inq_done status: %d\n",
                                                                 test_status);
            TSHStatus(ctx, TEST, "rpc_ns_entry_object_inq_next() failed", 
                test_status, expected_status);
            break;
    
        case NS_ENTRY_OBJECT_INQ_NEXT:
            /* do the next test here */
            rpc_ns_entry_object_inq_begin(test_entry_name_syntax, 
              test_entry_name, &inquiry_context, &test_status);
     	    TSHMessage(ctx,MOTRACE,"rpc_ns_entry_object_inq_begin status: %d\n", test_status);
             do
             {
                 rpc_ns_entry_object_inq_next(inquiry_context,&a_uuid,
                                                               &test_status);
     	         TSHMessage(ctx, MOTRACE,
                    "rpc_ns_entry_object_inq_next status: %d\n", test_status);
                 if (test_status != rpc_s_ok)
                    break;
             }while (expected_status != rpc_s_ok);
             TSHStatus(ctx, TEST, "rpc_ns_entry_object_inq_next() failed",
                test_status, expected_status);

             if(test_status == expected_status)
               break;

             rpc_ns_entry_object_inq_done(&inquiry_context,&cleanup_status);
     	    TSHMessage(ctx, MOTRACE, 
               "rpc_ns_entry_object_inq_done status: %d\n", cleanup_status);
            TSHStatus(ctx, CLNUP, 
               "rpc_ns_entry_object_inq_next() failed", 
                                                  cleanup_status, rpc_s_ok);
             break;
    
        default:
            TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", subcase);
            test_status = TSH_UNKNOWN_SUBCASE;
    }

all_clean_status = cleanup_status;
if (expected_status != rpc_s_entry_not_found) 
{
   cleanup_status = tsh_util_ns_unexport(ctx);
   if (TSHStatus(ctx, SETUP, "name space cleanup failed", 
                                         cleanup_status, rpc_s_ok))
       TSHMessage(ctx, MOWARNING, 
   "this test may not have removed all of the name space entries it created\n");
} 
if (all_clean_status == rpc_s_ok)
   all_clean_status = cleanup_status;
if (test_status == expected_status)
   test_status = rpc_s_ok;
if (test_status == rpc_s_ok)
   test_status = all_clean_status;
return(test_status);
}
