/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ns_entry_inq_if_ids.c,v $
 * Revision 1.1.6.2  1996/02/17  23:21:15  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:22  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:07:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:55  root]
 * 
 * Revision 1.1.4.3  1993/01/11  23:36:49  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:09:25  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/07  13:20:28  zeliff
 * 	Removed duplicate OSF copyright markers
 * 	[1992/12/06  00:44:32  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/21  20:55:08  dceguest
 * 	Added code to prevent core dump.
 * 	[1992/07/21  19:23:58  dceguest]
 * 
 * Revision 1.1  1992/01/19  03:35:54  devrcs
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
unsigned32		  i, j;
boolean32         match;
unsigned_char_p_t if_uuid_string;
rpc_if_id_vector_p_t if_id_vec_p;        
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

cleanup_status = atol(argv[3]);
if(cleanup_status == 1000) strcpy(test_entry_name,NULL);

/*
 *  Set up the name server database with some entries to test against.
 */
      
 
test_status = rpc_s_ok; /* initialize */

TSHMessage(ctx,MOINFO,"Expected Return Status : %d\n",expected_status); 


if (expected_status == rpc_s_ok) 
  {
    test_status = tsh_util_uuid_vec_create(ctx, &some_obj_vector);
    TSHMessage(ctx,MOINFO,"util_vec_create status: %d\n",test_status); 
    test_status = tsh_util_ns_export(ctx, &test_if_spec, &some_obj_vector);
    TSHMessage(ctx,MOINFO,"util_ns_export status: %d\n",test_status); 
  }
else
  {
  rpc_ns_mgmt_entry_create(test_entry_name_syntax,test_entry_name,&test_status);
  TSHMessage(ctx,MOINFO,"namespace setup status: %d\n",test_status); 
  }

  rpc_ns_mgmt_entry_inq_if_ids(
    (expected_status == rpc_s_unsupported_name_syntax) ?
    rpc_c_ns_syntax_unknown : test_entry_name_syntax,
    test_entry_name,  &if_id_vec_p, &test_status);
  TSHMessage(ctx,MOINFO,"rpc_ns_mgmt_entry_if_ids status: %d estatus: %d\n",
             test_status,expected_status);

    if(test_status == expected_status)
      {
       tsh_util_ns_unexport(ctx);
       return(rpc_s_ok);
      }
    else
       return(test_status);

    if (expected_status != rpc_s_ok) 
    {
        rpc_ns_mgmt_entry_delete(test_entry_name_syntax,
                                          test_entry_name, &cleanup_status);
        TSHMessage(ctx,MOINFO,"rpc_ns_mgmt_entry_delete status : %d\n", 
                    cleanup_status);
        if (expected_status == test_status)
            test_status = rpc_s_ok;
        else
            test_status = FAILURE;
        if (test_status == rpc_s_ok)
            test_status = cleanup_status;
        return(test_status);
    }
    rpc_if_id_vector_free(&if_id_vec_p, &cleanup_status);
    TSHMessage(ctx, MOTRACE, 
             "rpc_if_id_vector_free test status: %d\n", cleanup_status);
    TSHStatus(ctx,  CLNUP, 
               "rpc_if_id_vector_free() failed", 
                cleanup_status,  rpc_s_ok);

all_clean_status = cleanup_status;
cleanup_status = tsh_util_ns_unexport(ctx);
TSHStatus(ctx, CLNUP, "name space cleanup failed", cleanup_status, rpc_s_ok);
if (all_clean_status == rpc_s_ok)
    all_clean_status = cleanup_status;
if (test_status == rpc_s_ok)
    return(all_clean_status);
return(test_status);
}
