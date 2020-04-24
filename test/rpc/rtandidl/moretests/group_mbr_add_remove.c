/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: group_mbr_add_remove.c,v $
 * Revision 1.1.6.2  1996/02/17  23:20:32  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:15  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:04:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:12  root]
 * 
 * Revision 1.1.4.4  1993/02/03  18:20:09  cuti
 * 	Add ns_group_delete() to remove group entry
 * 	[1993/02/02  22:38:58  cuti]
 * 
 * Revision 1.1.4.3  1993/01/25  18:04:20  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:35:52  bbelch]
 * 
 * Revision 1.1.4.2  1993/01/11  22:13:49  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:05:56  bbelch]
 * 
 * Revision 1.1.2.2  1992/07/08  19:07:59  smith
 * 	Modified TSHMessage from "group_mbr_remove"
 * 	to "group_mbr_add".
 * 	[1992/07/08  19:06:33  smith]
 * 
 * Revision 1.1  1992/01/19  03:35:03  devrcs
 * 	     Initial revision
 * 
 * $EndLog$
 */
/*
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
   

#include <dce/rpc.h>
#include <tsh_common.h>
#include <tsh_group.h>
#include <tsh_ns.h>

#define ARGS_REQUIRED 6

/*  Generic entry point for function under test  */

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
	ndr_long_int		SubCase;
	error_status_t		EStatus;
	unsigned32			test_group_name_syntax;
	unsigned_char_t		test_group_name[DNS_FNAME_MAX];
	unsigned32			test_member_name_syntax;
	unsigned_char_t		test_member_name[DNS_FNAME_MAX];

	error_status_t		test_status;
	int			i, passes, phase;
	error_status_t		expected_status;

#ifdef DEBUG
printf("-------------------------------\nTestFunction:\nInput args are:\n");
printf("    Program Name is %s\n", ctx->ProgramName);
PRINT_ARGS(argc, argv);
#endif

	/*
	 *  Retrieve arguments.
	 */
	if ( argc != ARGS_REQUIRED ) {
		TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n",
			   ARGS_REQUIRED, argc);
		return TSH_BAD_ARG_COUNT;
	}
	SubCase                 = atol(argv[0]);
	EStatus                 = atol(argv[1]);
	test_group_name_syntax  = atol(argv[2]);
    strcpy(test_group_name, "/.:/");
    strcat(test_group_name, argv[3]);
	test_member_name_syntax = atol(argv[4]);
    strcpy(test_member_name, "/.:/");
    strcat(test_member_name, argv[5]);

	/*
	 *  Prepare for ADD/SETUP phase.
	 */
	switch ( SubCase ) {
	case GROUP_ADD:
	case GROUP_ADD_WITH_CLEANUP:
		phase = TEST;		/* We're testing ADD */
		passes = 1;		/* We'll do it once */
		break;
	case GROUP_REPEAT_ADD:
		phase = TEST;		/* We're testing ADD */
		passes = 2;		/* We'll do an add twice! */
		break;
	case GROUP_DEL_WITH_SETUP:
		phase = SETUP;		/* Just setting up for REMOVE */
		passes = 1;		/* We'll do a preliminary add. */
		break;
	case GROUP_DEL:
		phase = TEST;	/* We're testing REMOVE */
		passes = 0;	/* Don't do initial "add"; later we'll */
		break;		/* remove a nonexistent entry. */
	default:
		TSHMessage(ctx, MOWARNING, "Unrecognized subcase: %d\n",
			   SubCase);
		return TSH_UNKNOWN_SUBCASE;
	}

	/* If we're just setting up, we expect things to go OK. */
	expected_status = (phase == SETUP) ? rpc_s_ok : EStatus;

	/*
	 *  ADD/SETUP PHASE
	 */
	for ( i = 0; i < passes; i++ ) {
		rpc_ns_group_mbr_add (test_group_name_syntax,
				      test_group_name,
				      test_member_name_syntax,
				      test_member_name,
				      &test_status);
TSHMessage(ctx,MOINFO,"group_mbr_add:test_status %d expected_status %d\n",
           test_status,expected_status);
		if ( TSHStatus( ctx, phase, "rpc_ns_group_mbr_add() error",
			       test_status, expected_status ) ) 
			return test_status;
	}

	/*
	 *  Whew!  Now it's time to prepare for the REMOVE/CLEANUP
	 *  phase.
	 */

	/* Doing ADD but no cleanup (i.e. ADD failed)?  Then we're done! */
	if ( SubCase == GROUP_ADD )
		return TSH_SUCCESS;

	/*
	 *  If we were just setting up before, this is the Real Thing
	 *  now!  But if we were testing ADD, then this is cleanup and
	 *  things should work OK.
	 */
	if ( phase != CLNUP ){
		phase = TEST;
		expected_status = EStatus;
	} else {
		phase = CLNUP;
		expected_status = rpc_s_ok;
	}

	/*
		*  REMOVE/CLEANUP PHASE
		*/

	/*	"Just do it," as they say in the TV commercials. */
	rpc_ns_group_mbr_remove	(test_group_name_syntax,
					test_group_name,
					test_member_name_syntax,
					test_member_name,
					&test_status);
TSHMessage(ctx,MOINFO,"group_mbr_remove:test_status	%d expected_status %d\n",
	          test_status,expected_status);
	if	( TSHStatus( ctx, phase, "rpc_ns_group_mbr_remove() error",
			      test_status, expected_status ) ) {
		return	test_status;
	}	   
	    

	rpc_ns_group_delete(test_group_name_syntax,
					test_group_name,
	                &test_status);  
	                          
	/* modify the expected status for group_delete)
 	if (expected_status == rpc_s_group_member_not_found || 
		test_member_name_syntax != rpc_c_ns_syntax_dce )
	
		expected_status	= rpc_s_ok; 
		
TSHMessage(ctx,MOINFO,"group_delete:test_status %d expected_status %d\n",
           test_status,expected_status);
	if ( TSHStatus( ctx, CLNUP, "rpc_ns_group_delete() error",
		       test_status, expected_status ) ) {
		return test_status;
	}   

	/* So much for all that.... */
	return TSH_SUCCESS;
}

