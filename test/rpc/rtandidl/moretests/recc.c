/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: recc.c,v $
 * Revision 1.1.6.1  1996/06/04  22:11:52  arvind
 * 	Remove local malloc prototype
 * 	[1996/01/24  17:41 UTC  dat  /main/DCE_1.2/1]
 *
 * Revision 1.1.4.2  1996/02/17  23:21:49  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:56  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  20:09:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:37  root]
 * 
 * Revision 1.1.2.4  1993/01/25  19:27:13  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:43:17  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  23:39:02  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:12:13  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  17:21:37  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  20:25:40  cuti]
 * 
 * Revision 1.1  1992/01/19  03:36:36  devrcs
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



#include <rec.h>
#include <recc.h>
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
	return(records_test(h, ctx, subcase));
}

int records_test (h, ctx, subcase)
handle_t h;
CtxBlock *	ctx;
int		subcase;
{
    recordtest_t1 t1_i, t1_o;
    recordtest_t2 t2_io;
    recordtest_t3 t3_i, t3_o;
    recordtest_t4 *t4_i, *t4_o; /* open record type */
    recordtest_t5 t5_i, t5_o;

    int jndx, indx;
    int failures = 0;

	switch(subcase) {
	case REC_LONG:
    	/* test recordtest_op1 */
    	for (indx=0; indx<1000; indx+=97) {
        	t1_i.f1 = indx;
        	recordtest_op1 (h, t1_i, &t1_o);
		TSHMessage (ctx, MOTRACE, "t1_o.f1 (%d) ? indx (%ld) + 1\n",
					t1_o.f1,indx);
        	if (t1_o.f1 != indx + 1) {
				TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
				TSHMessage(ctx,MOWARNING,"t1_o.f1 (%d) != indx (%ld) + 1\n",
					t1_o.f1,indx);
				return(TSH_INCORRECT_DATA);
        	}
    	}
		break;

	case REC_MULTI_MEMBER:
    	/* test recordtest_op2 */
    	for (indx=0; indx<1000; indx+=97) {
        	t2_io.f1 = indx;
        	t2_io.f2 = 0;
        	recordtest_op2 (h, &t2_io);
		TSHMessage (ctx, MOTRACE, "t2_o.f2 (%d) ? indx (%ld) + 1\n",
					t2_io.f2,indx);
        	if (t2_io.f2 != indx + 1) {
				TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
				TSHMessage(ctx,MOWARNING,"t2_o.f2 (%d) != indx (%ld) + 1\n",
					t2_io.f2,indx);
				return(TSH_INCORRECT_DATA);
        	}
        	if (t2_io.f1 != indx) {
				TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
				TSHMessage(ctx,MOWARNING,"t2_o.f1 (%d) != indx (%ld)\n",
					t2_io.f1,indx);
				return(TSH_INCORRECT_DATA);
        	}
    	}

	case REC_ARRAYS:
	    /* test recordtest_op3 */
	    for (indx=0; indx<1000; indx+=97) {
	        for (jndx = 0; jndx < recordtest_c1; jndx++) {
	            t3_i.f1[jndx] = indx;
	            t3_o.f1[jndx] = 0;
	        }
	        recordtest_op3 (h, t3_i, &t3_o);
	        for (jndx = 0; jndx < recordtest_c1; jndx++) {
		    TSHMessage (ctx, MOTRACE, "t3_o.f1[%d] (%d) ? indx (%d) + 1\n",
						jndx,t3_o.f1[jndx],indx);
	            if (t3_o.f1[jndx] != indx + 1) {
					TSHStatus(ctx, TEST,
						"incorrect data returned",FAILURE,SUCCESS);
					TSHMessage(ctx,MOWARNING,
						"t3_o.f1[%d] (%d) != indx (%d) + 1\n",
						jndx,t3_o.f1[jndx],indx);
					return(TSH_INCORRECT_DATA);
	            }
		    TSHMessage (ctx, MOTRACE, "t3_i.f1[%d] (%d) ? indx (%d)\n",
						jndx,t3_i.f1[jndx],indx);
	            if (t3_i.f1[jndx] != indx) {
					TSHStatus(ctx, TEST,
						"incorrect data returned",FAILURE,SUCCESS);
					TSHMessage(ctx,MOWARNING,
						"t3_i.f1[%d] (%d) != indx (%d)\n",
						jndx,t3_i.f1[jndx],indx);
					return(TSH_INCORRECT_DATA);
	            }
	        }
	    }
		break;
	
	case REC_MULTI_ARRAY:
    	/* test recordtest_op4 */
    	t4_i = (recordtest_t4 *) malloc( sizeof(recordtest_t4) +
            ((recordtest_c2) * sizeof(ndr_long_int)));

    	t4_o = (recordtest_t4 *) malloc( sizeof(recordtest_t4) +
            ((recordtest_c2) * sizeof(ndr_long_int)));

    	for (indx=0; indx<1000; indx+=97) {
        	for (jndx = 0; jndx <= recordtest_c2; jndx++) {
            	t4_i->f3[jndx] = indx;
            	t4_o->f3[jndx] = 0;
        	}
        	t4_i->f1 = t4_i->f2 = t4_o->f2 = recordtest_c2;
        	t4_o->f1 = 0;
        	recordtest_op4 (h, t4_i, t4_o);
        	for (jndx = 0; jndx <= recordtest_c2; jndx++) {
		TSHMessage (ctx, MOTRACE, "t4_o.f3[%d] (%d) ? indx (%d) + 1\n",
						jndx,t4_o->f3[jndx],indx);
            	if (t4_o->f3[jndx] != indx + 1) {
					TSHStatus(ctx, TEST,
						"incorrect data returned",FAILURE,SUCCESS);
					TSHMessage(ctx,MOWARNING,
						"t4_o.f3[%d] (%d) != indx (%d) + 1\n",
						jndx,t4_o->f3[jndx],indx);
					return(TSH_INCORRECT_DATA);
            	}
		TSHMessage(ctx,MOTRACE, "t4_i.f3[%d] (%d) ? indx (%d)\n",
					jndx,t4_i->f3[jndx],indx);
            	if (t4_i->f3[jndx] != indx) {
					TSHStatus(ctx, TEST,
						"incorrect data returned",FAILURE,SUCCESS);
					TSHMessage(ctx,MOWARNING,
						"t4_i.f3[%d] (%d) != indx (%d)\n",
						jndx,t4_i->f3[jndx],indx);
					return(TSH_INCORRECT_DATA);
            	}
        	}
    	}
		break;

	case REC_MULTI_IO:
    	/* test recordtest_op5 */
    	for (indx=0; indx<1000; indx+=97) {

        	t5_i.f1.f1 = indx;

        	for (jndx = 0; jndx < recordtest_c1; jndx++) {
            	t5_i.f2.f1[jndx] = indx;
            	t5_o.f2.f1[jndx] = 0;
        	}

        	recordtest_op5 (h, t5_i, &t5_o);
		TSHMessage (ctx, MOTRACE, "t5_o.f1.f1 (%d) ? indx (%d) + 1\n",
						t5_o.f1.f1,indx);
        	if (t5_o.f1.f1 != indx + 1) {
					TSHStatus(ctx, TEST,
						"incorrect data returned",FAILURE,SUCCESS);
					TSHMessage(ctx,MOWARNING,
						"t5_o.f1.f1 (%d) != indx (%d) + 1\n",
						t5_o.f1.f1,indx);
					return(TSH_INCORRECT_DATA);
        	}

        	for (jndx = 0; jndx < recordtest_c1; jndx++) {
		TSHMessage(ctx,MOTRACE, "t5_o.f2.f1[%d] (%d) ? indx (%d) + 1\n",
                        jndx, t5_o.f2.f1[jndx], indx);
            	if (t5_o.f2.f1[jndx] != indx + 1) {
					TSHStatus(ctx, TEST,
						"incorrect data returned",FAILURE,SUCCESS);
					TSHMessage(ctx,MOWARNING,
						"t5_o.f2.f1[%d] (%d) != indx (%d) + 1\n",
                        jndx, t5_o.f2.f1[jndx], indx);
					return(TSH_INCORRECT_DATA);
            	}
		TSHMessage(ctx,MOTRACE, "t5_i.f2.f1[%d] (%d) ? indx (%d)\n",
                        jndx, t5_i.f2.f1[jndx], indx);
            	if (t5_i.f2.f1[jndx] != indx) {
					TSHStatus(ctx, TEST,
						"incorrect data returned",FAILURE,SUCCESS);
					TSHMessage(ctx,MOWARNING,
						"t5_i.f2.f1[%d] (%d) != indx (%d)\n",
                        jndx, t5_i.f2.f1[jndx], indx);
					return(TSH_INCORRECT_DATA);
            	}
        	}
    	}
	}
    return(SUCCESS);
}
