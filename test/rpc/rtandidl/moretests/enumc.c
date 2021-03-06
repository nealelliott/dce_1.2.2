/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: enumc.c,v $
 * Revision 1.1.4.2  1996/02/17  23:20:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:09  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:04:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:08  root]
 * 
 * Revision 1.1.2.4  1993/01/25  18:04:01  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:35:28  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  22:13:24  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:05:30  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  16:34:19  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/12  14:40:58  cuti]
 * 
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  17:58:16  cuti]
 * 
 * Revision 1.1  1992/01/19  03:34:56  devrcs
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



#include <enum.h>
#include <enumc.h>
#include <tsh_common.h>

#define	ARGS_REQUIRED	4

ndr_long_int 
TestFunction(ctx, argc, argv)
CtxBlock *ctx;
ndr_long_int argc;
unsigned_char_p_t argv[];
{
	ndr_long_int subcase;
	unsigned_char_p_t bound_string;
	error_status_t status;
	ndr_long_int success = 0;
	ndr_long_int fail = 1;
	handle_t h;


	/**************************************************************************/
	/* Check the number of arguments passed in.  They should be exactly the   */
	/* required number when you come into this routine. Looking for a subcase,*/
	/* a bogus expected_status, and a hostname.                               */
	/**************************************************************************/
	if (argc !=  ARGS_REQUIRED) {
		TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
                                                               ARGS_REQUIRED, argc);
		return(TSH_BAD_ARG_COUNT);
	}

	subcase = atol(argv[0]);		 /* turn subcase string into an int.	  */
	/* ignore argv[1] expected status value, */
	/* each test does its own error checking.*/

	/**************************************************************************/
	/*  Do the compose of the string, followed by the binding from string.	*/
	/**************************************************************************/
	rpc_string_binding_compose(
	    NULL, (unsigned_char_p_t)argv[3], 
		argv[2], NULL, NULL, &bound_string, &status);

	if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", status, rpc_s_ok))
		return(status);
	rpc_binding_from_string_binding(bound_string, &h, &status);
	if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", status, rpc_s_ok))
		return(status);
	return(enums_test(h, ctx, subcase));
}


int	enums_test (h, ctx, subcase)
handle_t	h;
CtxBlock *	ctx;
int		subcase;
{
	int	failures = 0;

	enumtest_t1 et1_out;
	enumtest_t2 et2_out;

	switch (subcase) {
	case ENUM_TEST_LONG:
		TSHMessage (ctx, MOTRACE, "testing for long enum: value %d\n",
			enumtest_t1_id0);
		enumtest_op1 (h, enumtest_t1_id0, &et1_out);
		if (et1_out != enumtest_t1_id1) {
			TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
			TSHMessage(ctx,MOWARNING,"et1_out (%d) != enumtest_t1_id (%ld)\n",
				et1_out,enumtest_t1_id1);
				return(TSH_INCORRECT_DATA);
		}

		TSHMessage (ctx, MOTRACE, "testing for long enum: value %d\n",
			enumtest_t1_id1);
		enumtest_op1 (h, enumtest_t1_id1, &et1_out);
		if (et1_out != enumtest_t1_id2) {
			TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
			TSHMessage(ctx,MOWARNING,"et1_out (%d) != enumtest_t1_id (%ld)\n",
				et1_out,enumtest_t1_id2);
				return(TSH_INCORRECT_DATA);
		}

		TSHMessage (ctx, MOTRACE, "testing for long enum: value %d\n",
			enumtest_t1_id2);
		enumtest_op1 (h, enumtest_t1_id2, &et1_out);
		if (et1_out != enumtest_t1_id3) {
			TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
			TSHMessage(ctx,MOWARNING,"et1_out (%d) != enumtest_t1_id (%ld)\n",
				et1_out,enumtest_t1_id3);
				return(TSH_INCORRECT_DATA);
		}

		TSHMessage (ctx, MOTRACE, "testing for long enum: value %d\n",
			enumtest_t1_id3);
		enumtest_op1 (h, enumtest_t1_id3, &et1_out);
		if (et1_out != enumtest_t1_id0) {
			TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
			TSHMessage(ctx,MOWARNING,"et1_out (%d) != enumtest_t1_id (%ld)\n",
				et1_out,enumtest_t1_id0);
				return(TSH_INCORRECT_DATA);
		}
		break;

	case ENUM_TEST_SHORT:
		TSHMessage (ctx, MOTRACE, "testing for short enum: value %d\n",
			enumtest_t2_id0);
		enumtest_op2 (h, enumtest_t2_id0, &et2_out);
		if (et2_out != enumtest_t2_id1) {
			TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
			TSHMessage(ctx,MOWARNING,"et2_out (%d) != enumtest_t2_id (%d)\n",
				et2_out,enumtest_t2_id1);
				return(TSH_INCORRECT_DATA);
		}

		TSHMessage (ctx, MOTRACE, "testing for short enum: value %d\n",
			enumtest_t2_id1);
		enumtest_op2 (h, enumtest_t2_id1, &et2_out);
		if (et2_out != enumtest_t2_id2) {
			TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
			TSHMessage(ctx,MOWARNING,"et2_out (%d) != enumtest_t2_id (%d)\n",
				et2_out,enumtest_t2_id2);
				return(TSH_INCORRECT_DATA);
		}

		TSHMessage (ctx, MOTRACE, "testing for short enum: value %d\n",
			enumtest_t2_id2);
		enumtest_op2 (h, enumtest_t2_id2, &et2_out);
		if (et2_out != enumtest_t2_id3) {
			TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
			TSHMessage(ctx,MOWARNING,"et2_out (%d) != enumtest_t2_id (%d)\n",
				et2_out,enumtest_t2_id3);
				return(TSH_INCORRECT_DATA);
		}

		TSHMessage (ctx, MOTRACE, "testing for short enum: value %d\n",
			enumtest_t2_id3);
		enumtest_op2 (h, enumtest_t2_id3, &et2_out);
		if (et2_out != enumtest_t2_id0) {
			TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
			TSHMessage(ctx,MOWARNING,"et2_out (%d) != enumtest_t2_id (%d)\n",
				et2_out,enumtest_t2_id0);
				return(TSH_INCORRECT_DATA);
		}
		break;
	}
	return(SUCCESS);
}


