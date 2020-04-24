/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: vrecc.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:59  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:39  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:13:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:21  root]
 * 
 * Revision 1.1.2.4  1993/01/25  19:30:35  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:47:52  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/13  19:50:59  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:18:04  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  17:55:47  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  21:28:26  cuti]
 * 
 * Revision 1.1  1992/01/19  03:38:12  devrcs
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



#include <vrec.h>
#include <vrecc.h>
#include <tsh_common.h>
#include <stdio.h>
#include <math.h>
#include <dce/rpc.h> 
#include <pthread.h>
#include <dce/rpcexc.h>


#define ARGS_REQUIRED 4

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock *ctx;
ndr_long_int argc;
unsigned_char_p_t argv[];
{
    ndr_long_int subcase;
    unsigned_char_p_t bound_string;
    error_status_t status;
    handle_t h;



    /**************************************************************************/
    /* Check the number of arguments passed in.  They should be exactly the   */
    /* required number when you come into this routine. Looking for a subcase,*/
    /* a bogus expected_status, and a hostname.                               */
    /**************************************************************************/
    if(argc != ARGS_REQUIRED)
    {
	TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
                                                               ARGS_REQUIRED, argc);
        return(TSH_BAD_ARG_COUNT);
    }

    subcase = atol(argv[0]);         /* turn subcase string into an int.      */
                                     /* ignore argv[2] expected status value, */
                                     /* each test does its own error checking.*/

    /**************************************************************************/
    /*  Do the compose of the string, followed by the binding from string.    */
    /**************************************************************************/
    rpc_string_binding_compose(
       NULL, (unsigned_char_p_t)argv[3], 
       argv[2], NULL, NULL, &bound_string, &status);

    if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", status, rpc_s_ok))
       return(status);
    rpc_binding_from_string_binding(bound_string, &h, &status);
    if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", status, rpc_s_ok))
       return(status);
    return(variant_records_test(h, ctx, subcase));
}

int variant_records_test (h, ctx, subcase)
handle_t h;
CtxBlock *ctx;
ndr_long_int subcase;
{
    int failures = 0;
    varrectest_t1 op1_a, op1_b;
    varrectest_t2 op2_a, op2_b;
    int i;
    char tmpstr[varrectest_c1];
    error_status_t fst, rst;
    int flag = SUCCESS;
    EXCEPTION *valid = NULL;

    TRY
    /* test varrectest_op1 */
    switch(subcase)
    {

        case VREC_UNNAMED_UNION:
        /**********************************************************************/
        /* test varrectest_op1: using a pair of unnamed unions, which become  */
        /* tagged unions by default, see if you can pass data in as one member*/
        /* and receive it back as another.  Essentially do float to int etc.  */
        /**********************************************************************/
        for (i = 0; i < 23; i ++) 
        {
            if (flag)
                break;
            op1_a.tag = i % 4;
            switch (op1_a.tag) 
            {
                case 0:
                case 2:
                    op1_a.tagged_union.case_0.a = i;
                    op1_a.tagged_union.case_0.b = i * 45;
                    break;
                case 1:
                    op1_a.tagged_union.case_1.x = 1.3 * i;
                    op1_a.tagged_union.case_1.y = 2.3 * i;
                    break;
                case 3:
                    sprintf (op1_a.tagged_union.s, "%f %f", i * 3.1416, 10.0/i);
            }
            varrectest_op1 (h, op1_a, &op1_b);
            /******************************************************************/
            /* test to see if the values are correct.                         */
            /******************************************************************/
            switch (op1_a.tag) 
            {
    
                case 0:
                case 2:
                    TSHMessage (ctx, MOTRACE,"op1_b.tag (%d) ? 1\n", op1_b.tag);
                    if (op1_b.tag != 1) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                                           "op1_b.tag (%d) != 1\n", op1_b.tag);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                    TSHMessage (ctx, MOTRACE,
"op1_a.tagged_union.case_0.a (%f) (close?) op1_b.tagged_union.case_1.x (%f)\n",
                                (float) op1_a.tagged_union.case_0.a,
                                                  op1_b.tagged_union.case_1.x);
                    if (!close_sf((float) op1_a.tagged_union.case_0.a, 
                                                  op1_b.tagged_union.case_1.x)) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
"op1_a.tagged_union.case_0.a (%f) (close?) op1_b.tagged_union.case_1.x (%f)\n",
                                (float) op1_a.tagged_union.case_0.a,
                                                  op1_b.tagged_union.case_1.x);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                    TSHMessage (ctx, MOTRACE,
"op1_a.tagged_union.case_0.b (%f) (close?) op1_b.tagged_union.case_1.y (%f)\n",
                                (ndr_short_float) op1_a.tagged_union.case_0.b,
                                                  op1_b.tagged_union.case_1.y);
                    if (!close_sf((ndr_short_float) op1_a.tagged_union.case_0.b,
                                                  op1_b.tagged_union.case_1.y)) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
"op1_a.tagged_union.case_0.b (%f) (close?) op1_b.tagged_union.case_1.y (%f)\n",
                                (ndr_short_float) op1_a.tagged_union.case_0.b,
                                                  op1_b.tagged_union.case_1.y);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                    break;
    
                case 1:
                    TSHMessage (ctx, MOTRACE,"op1_b.tag (%d) ? 3\n", op1_b.tag);
                    if (op1_b.tag != 3) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                                            "op1_b.tag (%d) != 3\n", op1_b.tag);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                    sprintf (tmpstr, "%f %f",
                             op1_a.tagged_union.case_1.x,
                             op1_a.tagged_union.case_1.y);
                    TSHMessage (ctx, MOTRACE,
                        "tmpstr (%s) ? op1_b.tagged_union.s (%s)\n",
                                                  tmpstr, op1_b.tagged_union.s);
                    if (strncmp (tmpstr, op1_b.tagged_union.s, 
                                                           varrectest_c1) != 0) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                            "tmpstr (%s) ? op1_b.tagged_union.s (%s)\n",
                                                  tmpstr, op1_b.tagged_union.s);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                    break;
    
                case 3:
                    TSHMessage (ctx, MOTRACE,"op1_b.tag (%d) ? 0\n", op1_b.tag);
                    if (op1_b.tag != 0) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                                            "op1_b.tag (%d) != 0\n", op1_b.tag);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                    TSHMessage (ctx, MOTRACE,
     "op1_b.tagged_union.case_0.a (%d) ? (ndr_long_int) ((i * 3.1416) (%f))\n",
                                       op1_b.tagged_union.case_0.a, i * 3.1416);
                    if (op1_b.tagged_union.case_0.a != 
                                                   (ndr_long_int) (i * 3.1416)) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
     "op1_b.tagged_union.case_0.a (%d) != (ndr_long_int) ((i * 3.1416) (%f))\n",
                                       op1_b.tagged_union.case_0.a, i * 3.1416);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                    TSHMessage (ctx, MOTRACE,
       "op1_b.tagged_union.case_0.b (%d) ? (ndr_long_int) ((10.0 / i) (%f))\n",
                                         op1_b.tagged_union.case_0.b, 10.0 / i);
                    if (op1_b.tagged_union.case_0.b != 
                                                     (ndr_long_int) (10.0 / i)) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
       "op1_b.tagged_union.case_0.b (%d) != (ndr_long_int) ((10.0 / i) (%f))\n",
                                         op1_b.tagged_union.case_0.b, 10.0 / i);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
            }
        }
        break;
    
        case VREC_NAMED_UNION:
        /**********************************************************************/
        /* test varrectest_op2: using a pair of named unions, see if you can  */
        /* pass data in as one member (in op2_a), and receive it back as      */
        /* another member (in op2_b).  Essentially do float to int etc.       */
        /**********************************************************************/
        for (i = 0; i < 23; i ++) 
        {
            if (flag)
                break;
            op2_a.tag = i % 4;
            switch (op2_a.tag) 
            {
                case 0:
                case 2:
                    op2_a.tu.c0.a = i;
                    op2_a.tu.c0.b = i * 45;
                    break;
                case 1:
                    op2_a.tu.c1.x = 1.3 * i;
                    op2_a.tu.c1.y = 2.3 * i;
                    break;
                case 3:
                    sprintf (op2_a.tu.s, "%f %f", i * 3.1416, 10.0/i);
            }
            varrectest_op2 (h, op2_a, &op2_b);
            /******************************************************************/
            /* test to see if the values are correct.                         */
            /******************************************************************/
            switch (op2_a.tag) 
            {
    
                case 0:
                case 2:
                    TSHMessage (ctx, MOTRACE,"op2_b.tag (%d) ? 1\n", op2_b.tag);
                    if (op2_b.tag != 1) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                                           "op2_b.tag (%d) != 1\n", op2_b.tag);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                    TSHMessage (ctx, MOTRACE,
                        "op2_a.tu.c0.a (%f) (close?) op2_b.tu.c1.x (%f)\n",
                                (float) op2_a.tu.c0.a, op2_b.tu.c1.x);
                    if (!close_sf((float) op2_a.tu.c0.a, op2_b.tu.c1.x)) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                        "op2_a.tu.c0.a (%f) (close?) op2_b.tu.c1.x (%f)\n",
                                (float) op2_a.tu.c0.a, op2_b.tu.c1.x);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                    TSHMessage (ctx, MOTRACE,
                        "op2_a.tu.c0.b (%f) (close?) op2_b.tu.c1.y (%f)\n",
                                (ndr_short_float) op2_a.tu.c0.b, op2_b.tu.c1.y);
                    if (!close_sf((ndr_short_float)op2_a.tu.c0.b,op2_b.tu.c1.y))
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                        "op2_a.tu.c0.b (%f) (close?) op2_b.tu.c1.y (%f)\n",
                                (ndr_short_float) op2_a.tu.c0.b, op2_b.tu.c1.y);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                    break;
    
                case 1:
                    TSHMessage (ctx, MOTRACE,"op2_b.tag (%d) ? 3\n", op2_b.tag);
                    if (op2_b.tag != 3) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                            "op2_b.tag (%d) != 3\n", op2_b.tag);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                    sprintf (tmpstr, "%f %f",
                             op2_a.tu.c1.x,
                             op2_a.tu.c1.y);
                    TSHMessage (ctx, MOTRACE, "tmpstr (%s) ? op2_b.tu.s (%s)\n",
                                                            tmpstr, op2_b.tu.s);
                    if (strncmp (tmpstr, op2_b.tu.s, varrectest_c1) != 0) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                            "tmpstr (%s) ? op2_b.tu.s (%s)\n",
                                                            tmpstr, op2_b.tu.s);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                    break;
    
                case 3:
                    TSHMessage (ctx, MOTRACE,"op2_b.tag (%d) ? 0\n", op2_b.tag);
                    if (op2_b.tag != 0) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                                            "op2_b.tag (%d) != 0\n", op2_b.tag);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                    TSHMessage (ctx, MOTRACE,
                   "op2_b.tu.c0.a (%d) ? (ndr_long_int) ((i * 3.1416) (%f))\n",
                                                     op2_b.tu.c0.a, i * 3.1416);
                    if (op2_b.tu.c0.a != (ndr_long_int) (i * 3.1416)) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                   "op2_b.tu.c0.a (%d) != (ndr_long_int) ((i * 3.1416) (%f))\n",
                                                     op2_b.tu.c0.a, i * 3.1416);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                    TSHMessage (ctx, MOTRACE,
                     "op2_b.tu.c0.b (%d) ? (ndr_long_int) ((10.0 / i) (%f))\n",
                                                       op2_b.tu.c0.b, 10.0 / i);
                    if (op2_b.tu.c0.b != (ndr_long_int) (10.0 / i)) 
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                              FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                     "op2_b.tu.c0.b (%d) != (ndr_long_int) ((10.0 / i) (%f))\n",
                                                       op2_b.tu.c0.b, 10.0 / i);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
            }
        }
        break;

        case VREC_INVALID_TAG_IN:
        /**********************************************************************/
        /* test varrectest_op3: test that undeclared tag values as [in]s cause*/
        /* the rpc_x_invalid_tag exception to be raised.                      */
        /**********************************************************************/
        op1_a.tag = 5;
        valid = &rpc_x_invalid_tag;
        varrectest_op3 (h, op1_a);
        TSHStatus(ctx, TEST, "unexpected normal return\n", FAILURE,SUCCESS);
	flag = TSH_EXCEPTION_NOT_RAISED;
        break;

        case VREC_INVALID_TAG_OUT:
        /**********************************************************************/
        /* test varrectest_op4: test that undeclared tag values as [out]s     */
        /* cause the rpc_x_invalid_tag exception to be raised.                */
        /**********************************************************************/
        valid = &rpc_x_invalid_tag;
        varrectest_op4 (h, 5, &op1_b);
        TSHStatus(ctx, TEST, "unexpected normal return\n", FAILURE,SUCCESS);
	flag = TSH_EXCEPTION_NOT_RAISED;
        break;
    }
    CATCH_ALL
/*#ifdef OSF */
        if (valid && (exc_matches(THIS_CATCH, valid)))
/*#else
        if (valid && (cma_exc_matches(THIS_CATCH, valid)))
#endif */
            flag = SUCCESS;
        else
        {
            flag = TSH_EXCEPTION_CAUGHT;
            RERAISE;
        }
    ENDTRY
    return(flag);
}
