/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: group_mbr_inquiry.c,v $
 * Revision 1.1.534.2  1996/02/17  23:20:33  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:17  marty]
 *
 * Revision 1.1.534.1  1995/12/11  20:04:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:14  root]
 * 
 * Revision 1.1.532.1  1994/01/21  22:40:17  cbrooks
 * 	RPC Code Cleanup-Initial Submission
 * 	[1994/01/21  20:16:05  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/25  18:04:23  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:35:58  bbelch]
 * 
 * Revision 1.1.4.2  1993/01/11  22:13:56  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:06:01  bbelch]
 * 
 * Revision 1.1.2.2  1992/07/08  19:35:08  smith
 * 	Modified arg list check, TSHMessage changed from MOTRACE to MOINFO,
 * 	and removed EStatus.
 * 	[1992/07/08  19:33:34  smith]
 * 
 * Revision 1.1  1992/01/19  03:35:04  devrcs
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


#include <grouptest.h>
#include <tsh_common.h>
#include <tsh_group.h>
#include <tsh_ns.h>

#define ARGS_REQUIRED 7

/*  Generic entry point for function under test  */

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
	ndr_long_int		SubCase;
	error_status_t		EStatus;
	ndr_long_int		SetupAction;
	unsigned32		test_group_name_syntax;
	unsigned_char_t	test_group_name[DNS_FNAME_MAX];
	unsigned32		test_member_name_syntax;

	/*
	 *  NB setup_member_name is passed as an argument; it is the
	 *  name of a group member to add to the database prior to testing. 
	 *  In contrast, test_member_name is used for actually doing the
	 *  inquiries.  We must keep track of both names so we can clean up
	 *  properly.
	 */
	unsigned_char_t	setup_member_name[DNS_FNAME_MAX];
	unsigned_char_p_t	test_member_name = NULL;

	rpc_ns_inq_handle_t	test_inquiry_context;
	error_status_t		test_status;
	int			i, passes, phase;
	error_status_t		expected_status;
	rpc_ns_inq_rep_t	*test_inq;

#ifdef DEBUG
printf("-------------------------------\nTestFunction:\nInput args are:\n");
printf("    Program Name is %s\n", ctx->ProgramName);
PRINT_ARGS(argc, argv);
#endif

	/*
	 *  Retrieve arguments.
	 */
	if ( argc < ARGS_REQUIRED ) {
		TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n",
			   ARGS_REQUIRED, argc);
		return TSH_BAD_ARG_COUNT;
	}
	SubCase                 = atol((char *)argv[0]);
	EStatus                 = atol((char *)argv[1]);
	SetupAction             = atol((char *)argv[2]);
	test_group_name_syntax  = atol((char *)argv[3]); 
        if (EStatus == rpc_s_incomplete_name)
		strcpy(test_group_name, argv[4]);
	else {
    	strcpy(test_group_name, "/.:/");  /* local cell root */
    	strcat(test_group_name, argv[4]);
	}
	test_member_name_syntax = atol((char *)argv[5]);
	if (EStatus == rpc_s_incomplete_name)
		strcpy(setup_member_name, argv[6]);
	else {
    	strcpy(setup_member_name, "/.:/");  /* local cell root */
    	strcat(setup_member_name, argv[6]);
   	}
	/*
	 *  NS DATABASE SETUP
	 *	Here we add entries to the name service database to
	 *	put it in the expected state prior to running the test.
	 */
	switch ( SetupAction ) {

	case GRP_SETUP_GROUP:
		/*
		 *  The specified test_group_name should be valid for
		 *  this test, so we must create it.
		 */
		rpc_ns_group_mbr_add (test_group_name_syntax,
				      test_group_name,
				      test_member_name_syntax,
				      setup_member_name,
				      &test_status); 
TSHMessage(ctx,MOINFO,"rpc_ns_group_mbr_add status: %d EStatus %d\n", 
           test_status,EStatus);
 		if ( TSHStatus( ctx, SETUP, "rpc_ns_group_mbr_add() error",
			       test_status, rpc_s_ok ) ) {
			return test_status;
		}
		break;

	case GRP_SETUP_NONGROUP:
		/*
		 *  The specified test_group_name should appear in the
		 *  name service database, but *not* with the group
		 *  attribute.  To create such an entry without using
		 *  the private routine rpc__nsentry_create(), we
		 *  create a normal group entry and then delete its
		 *  group attribute using rpc_ns_group_delete().
		 */
		rpc_ns_group_mbr_add (test_group_name_syntax,
				      test_group_name,
				      test_member_name_syntax,
				      setup_member_name,
				      &test_status);
TSHMessage(ctx,MOINFO, "rpc_ns_group_mbr_add status: %d EStatus %d\n", 	
	   test_status,EStatus);
		if ( TSHStatus( ctx, SETUP, "rpc_ns_group_mbr_add() error",
			       test_status, rpc_s_ok ) ) {
			return test_status;
		}
		rpc_ns_group_delete(test_group_name_syntax,
				    test_group_name,
				    &test_status);
TSHMessage(ctx,MOINFO,"rpc_ns_group_delete status: %d EStatus %d\n", 
           test_status,EStatus);
		if ( TSHStatus( ctx, SETUP, "rpc_ns_group_delete() error",
			       test_status, rpc_s_ok ) ) {
			return test_status;
		}
		break;
		/* ??? Hey, how do I clean that up?
		   With rpc_ns_mgmt_entry_delete()? */

	case GRP_SETUP_NOP:		/* No-op, no special setup needed */
	case GRP_SETUP_NULL_CONTEXT:	/* NULL_CONTEXT done after ...begin()*/
		break;

	default:
		TSHMessage(ctx, MOWARNING, "Unrecognized setup action: %d\n",
			   SetupAction);
		return TSH_UNKNOWN_SUBCASE;
	}


	/*
	 *  Here we prepare to call rpc_ns_group_inq_begin().
	 *  We are either testing ...begin() itself or setting up.
	 */
	switch ( SubCase ) {

	case GROUP_T_BEGIN:
	case GROUP_T_BEGIN_DONE:
		phase = TEST;
		expected_status = EStatus;
		break;

	case GROUP_BEGIN_T_NEXT:
	case GROUP_BEGIN_T_NEXT_DONE:
	case GROUP_BEGIN_T_NEXT_null_DONE:
	case GROUP_BEGIN_NEXT_T_NEXT_DONE:
	case GROUP_BEGIN_INVALIDATE_T_NEXT:
	case GROUP_BEGIN_T_DONE:
	case GROUP_BEGIN_INVALIDATE_T_DONE:
		phase = SETUP;
		/* Expect bad status if null context is needed later. */
		/* expected_status = (SetupAction == GRP_SETUP_NULL_CONTEXT)
				? rpc_s_entry_not_found
				: rpc_s_ok;  */
        expected_status = rpc_s_ok;
		break;

	default:
		TSHMessage(ctx, MOWARNING, "Unrecognized subcase: %d\n",
			   SubCase);
		return TSH_UNKNOWN_SUBCASE;	
	}

	rpc_ns_group_mbr_inq_begin(test_group_name_syntax,
				   test_group_name,
				   test_member_name_syntax,
				   &test_inquiry_context,
				   &test_status);
TSHMessage(ctx,MOINFO,"rpc_ns_group_mbr_inq_begin status: %d EStatus %d\n", 
           test_status,EStatus);

	/*
	 *  If a null context was desired, make sure it's really null.
	 */
	if ( SetupAction == GRP_SETUP_NULL_CONTEXT
	    && test_status == rpc_s_entry_not_found
	    && test_inquiry_context != NULL ) {
		TSHMessage( ctx, MOFAILURE,
"rpc_group_mbr_inq_begin() error: expected NULL context, got 0x%x",
			   test_inquiry_context );
		return test_status;
	}

	if ( TSHStatus( ctx, phase, "rpc_group_mbr_inq_begin() error",
		       test_status, expected_status ) ) {
		return test_status;
	}

	/*
	 *  Here we prepare to call rpc_ns_group_inq_next().
	 *  We may call it 0, 1, or 2 times depending on the subcase.
	 *  Some test subcases want an invalid inquiry context, so
	 *  we do that here since we've just gotten a valid one back
	 *  from ...begin().
	 */
	switch ( SubCase ) {

	case GROUP_BEGIN_INVALIDATE_T_NEXT:
		/*
		 *  We want an invalid context for ...next() testing.
		 */
		test_inq = (rpc_ns_inq_rep_t *)test_inquiry_context;
		test_inq->usage = rpc_e_profile_element;
		/* FALLTHRU */

	case GROUP_BEGIN_T_NEXT:		/* We're testing the */
	case GROUP_BEGIN_T_NEXT_DONE:		/* ..._inq_next() routine, */
	case GROUP_BEGIN_T_NEXT_null_DONE:	/* only calling it once. */
		passes = 1;
		phase = TEST;
		break;

	case GROUP_BEGIN_NEXT_T_NEXT_DONE:	/* Calling it twice! */
		passes = 2;
		phase = TEST;
		break;

	case GROUP_T_BEGIN:
		/*
		 *  We were testing begin and we don't expect to have
		 *  to do any further cleanup, so....
		 */
		return TSH_SUCCESS;

	case GROUP_BEGIN_INVALIDATE_T_DONE:
		/*
		 *  We want an invalid context for ...done() testing.
		 */
		test_inq = (rpc_ns_inq_rep_t *)test_inquiry_context;
		test_inq->usage = rpc_e_profile_element;
		/*FALLTHRU*/

	case GROUP_T_BEGIN_DONE:	/* These subcases don't call the */
	case GROUP_BEGIN_T_DONE:	/* ..._next() routine at all. */
		passes = 0;
		break;
	}
      
	if (SetupAction == GRP_SETUP_NULL_CONTEXT)
		test_inquiry_context = NULL;
	/*
	 *  Here we call the ...next() routine.  If we're calling it
	 *  more than once, we expect the first call to succeed, i.e.
	 *  we assume exactly one member in the group.
	 *
	 *  One subcase requires that we pass a NULL argument....
	 */
        if(argc >ARGS_REQUIRED)
          test_member_name=NULL;
        if(test_member_name == NULL)
TSHMessage(ctx,MOINFO,"before: test_member_name: NULL\n");
	for ( i = 0; i < passes; i++ ) {
		rpc_ns_group_mbr_inq_next(test_inquiry_context,
				  ( (SubCase == GROUP_BEGIN_T_NEXT_null_DONE)
				  ? NULL
				  : &test_member_name ),
				  &test_status);
		expected_status = ((passes == 2) && (i == 0))
				? rpc_s_ok
				: EStatus;
TSHMessage(ctx,MOINFO,"rpc_ns_group_mbr_inq_next status: %d EStatus %d\n", 
           test_status,EStatus);
        if(test_member_name == NULL)
TSHMessage(ctx,MOINFO,"after: test_member_name: NULL\n");
		if ( TSHStatus( ctx, phase, "rpc_group_mbr_inq_next() error",
			       test_status, expected_status ) ) {
			return test_status;
		}
	}

	/*
	 *  Here we prepare to call rpc_ns_group_inq_done().
	 *  Either we're testing ...done() or else this is cleanup.
	 *  Those subcases not expecting to call ...done() because of
	 *  intentionally induced errors can return here.
	 */
	switch ( SubCase ) {

	case GROUP_BEGIN_T_NEXT:
	case GROUP_BEGIN_INVALIDATE_T_NEXT:
		return TSH_SUCCESS;

	case GROUP_T_BEGIN_DONE:
	case GROUP_BEGIN_T_NEXT_DONE:
	case GROUP_BEGIN_T_NEXT_null_DONE:
	case GROUP_BEGIN_NEXT_T_NEXT_DONE:
	 	if (SetupAction == 3)
		  return (rpc_s_ok);
		phase = CLNUP;
		expected_status = rpc_s_ok;
		break;

	case GROUP_BEGIN_T_DONE:
	case GROUP_BEGIN_INVALIDATE_T_DONE:
		phase = TEST;
		expected_status = EStatus;
		break;
	}


	rpc_ns_group_mbr_inq_done(&test_inquiry_context, &test_status);
TSHMessage(ctx,MOINFO,"rpc_ns_group_mbr_inq_done status: %d EStatus %d\n", 
           test_status,EStatus);
        if(test_inquiry_context == NULL)
TSHMessage(ctx,MOINFO,"rpc_ns_group_mbr_inq_done inquiry_context: NULL\n");
	if ( TSHStatus( ctx, phase, "rpc_group_mbr_inq_done() error",
		       test_status, expected_status ) ) {
		return test_status;
	}

	/*
	 *  Free any member name strings we might have found.
	 */
	if ( test_member_name != NULL )
		rpc_string_free(&test_member_name, &test_status);

	/*
	 *  Final cleanup.  Here we remove any name service entries
	 *  added during our initial setup, by way of
	 *  "switch (SetupAction) { ... }".
	 */
	switch ( SetupAction ) {

	case GRP_SETUP_GROUP:
		/*
		 *  ??? This assumes ...remove() will delete the group
		 *  upon deleting the last group member; is that the case?
		 */
		rpc_ns_group_mbr_remove (test_group_name_syntax,
					 test_group_name,
					 test_member_name_syntax,
					 setup_member_name,
					 &test_status);
TSHMessage(ctx,MOINFO,"rpc_ns_group_mbr_remove status: %d EStatus %d\n", 
           test_status,EStatus);
		if ( TSHStatus( ctx, CLNUP, "rpc_ns_group_mbr_remove() error",
			       test_status, rpc_s_ok ) ) {
			return test_status;
		}
		break;

	case GRP_SETUP_NONGROUP:
		/*
		 *  ??? I hope this is the proper way to delete the
		 *  "non-group" constructed during setup, but I'm
		 *  not sure and the OSF DCE RPC functional spec
		 *  (V3.0, 25-Sep-90) isn't clear on it.
		 */
		rpc_ns_mgmt_entry_delete (test_group_name_syntax,
					  test_group_name,
					  &test_status); 
TSHMessage(ctx,MOINFO,"rpc_ns_mgmt_entry_delete status: %d EStatus %d\n", 
           test_status,EStatus);

		if ( TSHStatus( ctx, CLNUP, "rpc_ns_mgmt_entry_delete() error",
			       test_status, rpc_s_ok ) ) {
			return test_status;
		}
	}

	return TSH_SUCCESS;
}
