/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ns_profile_delete.c,v $
 * Revision 1.1.6.2  1996/02/17  23:21:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:37  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:07:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:02  root]
 * 
 * Revision 1.1.4.3  1993/01/25  18:06:06  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:40:38  bbelch]
 * 
 * Revision 1.1.4.2  1993/01/11  23:37:14  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:09:51  bbelch]
 * 
 * Revision 1.1.2.2  1992/07/09  15:48:11  smith
 * 	Added TSHMessage to check for rpc_ns_profile_delete()
 * 	return status.
 * 	[1992/07/09  15:47:32  smith]
 * 
 * Revision 1.1  1992/01/19  03:36:02  devrcs
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


/*  Args required by the test program...
 */
#define ARGS_REQUIRED 9


ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
ndr_long_int      subcase;		/* argv[0], subcase to run */
error_status_t    expected_status;	/* argv[1], expected return from test */
error_status_t    test_status,		/* status value from rpc calls... */
                  cleanup_status;	/* status value from cleanup */
error_status_t    inquire_status;	/* status value from inquire calls... */
unsigned32        i, 
                  test_profile_name_syntax,
                  test_member_name_syntax,
                  test_priority,
                  test_interface_id,
                  test_state = rpc_s_ok;
unsigned_char_p_t test_annotation,
                  test_export_name,
                  string_uuid,
                  string_binding,
                  elt_member,
                  elt_annotation; 
unsigned_char_t	  test_member_name[DNS_FNAME_MAX],
                  test_profile_name[DNS_FNAME_MAX];        
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
test_annotation          = argv[8];

/*
 *  Set up the name server database with some entries to test against for the
 *  normal case test.  The pathological case does not require setup/cleanup.
 */

if (expected_status == rpc_s_ok)
  {
    test_status = tsh_util_ns_export(ctx, &test_if_spec, NULL);

    if (!(TSHStatus(ctx, SETUP, "name space setup failed, aborting test", 
         test_status, rpc_s_ok)))
      return (test_status); 
 
    rpc_ns_profile_elt_add (
        test_profile_name_syntax, 
        test_profile_name, 
        &test_if_id, 
        test_member_name_syntax, 
        test_member_name, 
        test_priority, 
        test_annotation, 
        &test_status);

    if (TSHStatus(ctx, SETUP, "rpc_ns_profile_elt_add() failed", 
        test_status, rpc_s_ok))
      goto CLEANUP_EXIT;
  }

TSHMessage(ctx, MOINFO, "running subcase PROFILE_DELETE\n");

/* Call the rpc under test. */

rpc_ns_profile_delete (
    test_profile_name_syntax,
    test_profile_name,
    &test_status);

TSHMessage(ctx,MOTRACE,"rpc_ns_profile_delete status: %d\n",test_status);
    
if (test_status == expected_status)
  test_status = rpc_s_ok;
else
  TSHMessage (ctx, MOFAILURE, "rpc_ns_profile_delete() failed", 
              test_status, expected_status);

/* Cleanup after rpc test on the normal case.  The pathological case does not
   require setup/cleanup. */

CLEANUP_EXIT:

if (expected_status == rpc_s_ok)
  {
    cleanup_status = tsh_util_ns_unexport(ctx);
    if(TSHStatus(ctx, CLNUP, "name space cleanup failed", cleanup_status, rpc_s_ok))
      {
        TSHMessage(ctx, MOWARNING, 
         "this test may not have removed all of the name space entries it created\n");
        if (test_status == rpc_s_ok)
          test_status = cleanup_status;
      }
  }

return(test_status);
}

