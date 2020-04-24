/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: aryc.c,v $
 * Revision 1.1.6.2  1996/02/17  23:19:57  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:38:32  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:02:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:42:42  root]
 * 
 * Revision 1.1.4.3  1993/01/11  22:10:52  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:03:24  bbelch]
 * 
 * Revision 1.1.4.2  1992/10/12  16:20:42  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/12  14:38:47  cuti]
 * 
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  17:56:44  cuti]
 * 
 * Revision 1.1.2.2  1992/07/15  17:03:53  cuti
 * 	Changed to not report debugging info regularly.
 * 	[1992/07/15  17:00:42  cuti]
 * 
 * Revision 1.1  1992/01/19  03:34:40  devrcs
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



#include <ary.h>
#include <aryc.h>
#include <scal.h>
#include <nidltest.h>
#include <tsh_common.h>
#include <stdio.h>
#include <math.h>
#include <dce/rpc.h> 
#include <pthread.h>
#include <dce/rpcexc.h> 

#ifdef OSF
  extern void *malloc();
#endif

#define ARGS_REQUIRED 4

static ndr_long_int square_l(i)
ndr_long_int i;
{
    return (i*i);
}

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock *ctx;
ndr_long_int argc;
unsigned_char_p_t argv[];
{
    ndr_long_int subcase;
	short		dbg=0;
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
        TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", ARGS_REQUIRED, argc);
        return(TSH_BAD_ARG_COUNT);
    }

    subcase = atol(argv[0]);         /* turn subcase string into an int.      */
                                     /* ignore argv[2] expected status value, */
                                     /* each test does its own error checking.*/

	if (dbg)
rpc__dbg_set_switches("0-4.10", &status);

    /**************************************************************************/
    /*  Do the compose of the string, followed by the binding from string.    */
    /**************************************************************************/
    rpc_string_binding_compose(
	    NULL, (unsigned_char_p_t)argv[3], 
		argv[2], NULL, NULL, &bound_string, &status);

    if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", status, rpc_s_ok))
       return(status);
    rpc_binding_from_string_binding(bound_string, &h, &status);
    if (TSHStatus(ctx, SETUP, "rpc_binding_from_string_binding", status, rpc_s_ok))
       return(status);
    return(arrays_test(h, ctx, subcase));
}

int arrays_test (h, ctx, subcase)
handle_t h;
CtxBlock *ctx;
ndr_long_int subcase;
{
    arraytest_t1 fa1, fa2;
    ndr_long_int *oa1, *oa2;
    arraytest_t3 fa_2d_1, fa_2d_2;
    ndr_long_int (*oa_2d_1)[arraytest_c3], (*oa_2d_2)[arraytest_c3];

    ndr_long_int i, j, k, l;
    ndr_long_int fault_test_num;
    ndr_long_int flag;

    ndr_small_int   expected_sm,   sm1[arraytest_c1],   sm2[arraytest_c1];
    ndr_usmall_int  expected_usm,  usm1[arraytest_c1],  usm2[arraytest_c1];
    ndr_short_int   expected_sh,   sh1[arraytest_c1],   sh2[arraytest_c1];
    ndr_ushort_int  expected_ush,  ush1[arraytest_c1],  ush2[arraytest_c1];
    ndr_long_int    expected_l,    l1[arraytest_c1],    l2[arraytest_c1];
    ndr_ulong_int   expected_ul,   ul1[arraytest_c1],   ul2[arraytest_c1];
    ndr_hyper_int   expected_h,    h1[arraytest_c1],    h2[arraytest_c1];
    ndr_uhyper_int  expected_uh,   uh1[arraytest_c1],   uh2[arraytest_c1];
    ndr_boolean     expected_bool, bool1[arraytest_c1], bool2[arraytest_c1];
    ndr_byte        expected_byte, byte1[arraytest_c1], byte2[arraytest_c1];
    ndr_short_float expected_sf,   sf1[arraytest_c1],   sf2[arraytest_c1];
    ndr_long_float  expected_lf,   lf1[arraytest_c1],   lf2[arraytest_c1];
    ndr_char        expected_char, char1[arraytest_c1], char2[arraytest_c1];
    arraytest_t8     expected_t8,   t8_1[arraytest_c1],  t8_2[arraytest_c1];
    arraytest_t9     expected_t9,   t9_1[arraytest_c1],  t9_2[arraytest_c1];
    arraytest_t10                   t10_1[arraytest_c1], t10_2[arraytest_c1];
    
    /**********************************************************************/
    /* allocate a couple of open arrays of longs                          */
    /**********************************************************************/
    oa1 = (ndr_long_int *) malloc (arraytest_c1 * sizeof(ndr_long_int));
    oa2 = (ndr_long_int *) malloc (arraytest_c1 * sizeof(ndr_long_int));
    /**********************************************************************/
    /* allocate a couple of 2d arrays of longs                            */
    /**********************************************************************/
    oa_2d_1 = (ndr_long_int (*)[arraytest_c3]) 
                malloc (arraytest_c2 * arraytest_c3 * sizeof(ndr_long_int));
    oa_2d_2 = (ndr_long_int (*)[arraytest_c3]) 
                malloc (arraytest_c2 * arraytest_c3 * sizeof(ndr_long_int));

    flag = SUCCESS;
    switch(subcase)
    {
        case ARRAY_FIXED_LONGS:
        /**********************************************************************/
        /* test arraytest_op1: send a fixed array of longs in fa1 and check   */
        /* that their squares come back in fa2.                               */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            fa1[i] = i*3+10;
        arraytest_op1 (h, fa1, fa2);
        for (i=0; i<arraytest_c1; i++)
        {
            expected_l = square_l((ndr_long_int) i*3+10);
            TSHMessage(ctx,MOTRACE,"fa2[%ld] (%ld) ? expected_l (%ld)\n",
                                 i, fa2[i], expected_l);
            if (fa2[i] != expected_l) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage(ctx,MOWARNING,"fa2[%ld] (%ld) != expected_l (%ld)\n",
                                 i, fa2[i], expected_l);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;

        case ARRAY_LAST_LONGS: 
        /**********************************************************************/
        /* test arraytest_op2: send in part of an array of longs using last_is*/
        /* attribute, in fa1, and check that their squares come back in fa2.  */
        /**********************************************************************/
        for (j = -1; j<arraytest_c1; j++) 
        {
            for (i=0; i<arraytest_c1; i++)
                fa1[i] = fa2[i] = i*7+10;
            k = 0;
            arraytest_op2 (h, fa1, fa2, j, &k);
            /******************************************************************/
            /* test to see if the correct number of elements was processed.   */
            /******************************************************************/
            TSHMessage(ctx,MOTRACE,"k (%ld) ? j (%ld) \n",k,j);
            if (k != j) 
            {
                TSHStatus(ctx, TEST,"last_is is not returned correctly",
                                                               FAILURE,SUCCESS);
                TSHMessage(ctx,MOWARNING,"k (%ld) != j (%ld) \n",k,j);
                flag = TSH_INCORRECT_DATA;
                break;
            }
            /******************************************************************/
            /* test to see if the values are correct.                         */
            /******************************************************************/
            for (i=0; i<=j; i++)
            {
                expected_l = square_l((ndr_long_int) i*7+10);
                TSHMessage(ctx,MOTRACE, 
                 "fa2[%ld] (%ld) ? expected_l (%ld) \n", i, fa2[i], expected_l);
                if (fa2[i] != expected_l) 
                {
                    TSHStatus(ctx, TEST,"incorrect data returned",
                                                               FAILURE,SUCCESS);
                    TSHMessage(ctx,MOWARNING, 
                        "fa2[%ld] (%ld) != expected_l (%ld) \n",
                                                        i, fa2[i], expected_l);
                    flag = TSH_INCORRECT_DATA;
                    break;
                }
            }
            if (flag)
                break;
            /******************************************************************/
            /* test to see that the rest of the array has not been clobbered. */
            /******************************************************************/
            for (; i<arraytest_c1; i++)
            {
                expected_l = i*7+10;
                TSHMessage(ctx,MOTRACE, 
                    "fa2[%ld] (%ld) ? expected_l (%ld) \n",
                                               i, fa2[i], expected_l);
                if (fa2[i] != expected_l) 
                {
                    TSHStatus(ctx, TEST, "value past last_is overwritten",
                                                              FAILURE, SUCCESS);
                    TSHMessage(ctx,MOWARNING, 
                        "fa2[%ld] (%ld) != expected_l (%ld) \n",
                                                         i, fa2[i], expected_l);
                    flag = TSH_INCORRECT_DATA;
                    break;
                }
            }
        }
        break;

        case ARRAY_OPEN_LONGS: 
        /**********************************************************************/
        /* test arraytest_op3: send open arrays of longs from 0 to            */
        /* arraytest_c1 elements in length get back an array containing       */
        /* the squares of the inputs.                                         */
        /**********************************************************************/

        for (j = -1; j<arraytest_c1; j++) {
            for (i=0; i<10; i++)
                oa1[i] = oa2[i] = i*7+10;
            k = j;
            arraytest_op3 (h, oa1, oa2, j, j, &k, j);
            /******************************************************************/
            /* test to see if the correct number of elements was processed.   */
            /******************************************************************/
            TSHMessage(ctx,MOTRACE,"k (%ld) ? j (%ld) \n",k,j);
            if (k != j) 
            {
                TSHStatus(ctx, TEST,"last_is is not returned correctly",
                                                              FAILURE, SUCCESS);
                TSHMessage(ctx,MOWARNING,"k (%ld) != j (%ld) \n",k,j);
                flag = TSH_INCORRECT_DATA;
                break;
            }
            /******************************************************************/
            /* test to see if the values are correct.                         */
            /******************************************************************/
            for (i=0; i<=j; i++)
            {
                expected_l = square_l((ndr_long_int) i*7+10);
                TSHMessage(ctx,MOTRACE,
                    "oa2[%ld] (%ld) ? expected_l (%ld) \n",
                             i, oa2[i], expected_l);
                if (oa2[i] != expected_l)
                {
                    TSHStatus(ctx, TEST,"incorrect data returned",
                                                               FAILURE,SUCCESS);
                    TSHMessage(ctx,MOWARNING,
                        "oa2[%ld] (%ld) != expected_l (%ld) \n",
                                 i, oa2[i], expected_l);
                    flag = TSH_INCORRECT_DATA;
                    break;
                }
            }
            if (flag)
                break;
            /******************************************************************/
            /* test to see that the rest of the array has not been clobbered. */
            /******************************************************************/
            for (; i<arraytest_c1; i++)
            {
                expected_l = oa1[i];
                TSHMessage(ctx,MOTRACE,
                    "oa2[%ld] (%ld) ? expected_l (%ld) \n",
                             i, oa2[i], expected_l);
                if (oa2[i] != expected_l) 
                {
                    TSHStatus(ctx, TEST, "value past last_is overwritten",
                                                              FAILURE, SUCCESS);
                    TSHMessage(ctx,MOWARNING,
                        "oa2[%ld] (%ld) != expected_l (%ld) \n",
                                 i, oa2[i], expected_l);
                    flag = TSH_INCORRECT_DATA;
                    break;
                }
            }
        }
        break;

        case ARRAY_SAME_LONGS: 
        /**********************************************************************/
        /* test arraytest_op4: send an array of longs in fa1 and check that   */
        /* their squares come back in fa1. Testing [in, out] same array.      */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            fa1[i] = i*3+10;
        arraytest_op4 (h, fa1);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++)
        {
            expected_l = (9 * i + 60) * i + 100;
            TSHMessage(ctx,MOTRACE,
                "fa1[%ld] (%ld) ? expected_l (%ld) \n", i, fa1[i], expected_l);
            if (fa1[i] != expected_l) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage(ctx,MOWARNING,
                    "fa1[%ld] (%ld) != expected_l (%ld) \n",
                         i, fa1[i], expected_l);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_SAME_LAST_LONGS: 
        /**********************************************************************/
        /* test arraytest_op5: send initial segments of an array of longs in  */
        /* fa1 and check that their squares and only their squares come back  */
        /* in fa1.                                                            */
        /**********************************************************************/
        for (j = -1; j<arraytest_c1; j++) 
        {
            for (i=0; i<arraytest_c1; i++)
                fa1[i] = i*5+10;
            k = j;
            arraytest_op5 (h, fa1, &k);
            /******************************************************************/
            /* test to see if the correct number of elements was processed.   */
            /******************************************************************/
            TSHMessage(ctx,MOTRACE,"k (%ld) ? j (%ld) \n",k,j);
            if (k != j) 
            {
                TSHStatus(ctx, TEST,"last_is is not returned correctly",
                                                              FAILURE, SUCCESS);
                TSHMessage(ctx,MOWARNING,"k (%ld) != j (%ld) \n", k,j);
                flag = TSH_INCORRECT_DATA;
                break;
            }
            /******************************************************************/
            /* test to see if the values are correct.                         */
            /******************************************************************/
            for (i=0; i<=k; i++)
            {
                expected_l = (25*i+100)*i + 100;
                TSHMessage(ctx,MOTRACE,
                    "fa1[%ld] (%ld) ? expected_l (%ld) \n",
                         i, fa1[i], expected_l);
                if (fa1[i] != expected_l) 
                {
                    TSHStatus(ctx, TEST,"incorrect data returned",
                                                               FAILURE,SUCCESS);
                    TSHMessage(ctx,MOWARNING,
                        "fa1[%ld] (%ld) != expected_l (%ld) \n",
                             i, fa1[i], expected_l);
                    flag = TSH_INCORRECT_DATA;
                    break;
                }
            }
            if (flag)
                break;
            /******************************************************************/
            /* test to see that the rest of the array has not been clobbered. */
            /******************************************************************/
            for (; i<arraytest_c1; i++)
            {
                expected_l = i*5+10;
                TSHMessage(ctx,MOTRACE,
                    "fa1[%ld] (%ld) ? expected_l (%ld) \n",
                         i, fa1[i], expected_l);
                if (fa1[i] != expected_l) 
                {
                    TSHStatus(ctx, TEST, "value past last_is overwritten",
                                                              FAILURE, SUCCESS);
                    TSHMessage(ctx,MOWARNING,
                        "fa1[%ld] (%ld) != expected_l (%ld) \n",
                             i, fa1[i], expected_l);
                    flag = TSH_INCORRECT_DATA;
                    break;
                }
            }
        }
        break;
    
    
        case ARRAY_SAME_OPEN_LONGS:
        /**********************************************************************/
        /* test arraytest_op6: send an open array of longs from 0 to          */
        /* arraytest_c1 elements in length get back same array containing the */
        /* squares of the inputs.                                             */
        /**********************************************************************/
        for (j = -1; j<arraytest_c1; j++) 
        {
            for (i=0; i<10; i++)
                oa1[i] = oa2[i] = i*7+10;
            k = j;
            arraytest_op6 (h, oa1, &k, j);
            /******************************************************************/
            /* test to see if the correct number of elements was processed.   */
            /******************************************************************/
            TSHMessage(ctx,MOTRACE,"k (%ld) ? j (%ld) \n",k,j);
            if (k != j) 
            {
                TSHStatus(ctx, TEST,"last_is is not returned correctly",
                                                              FAILURE, SUCCESS);
                TSHMessage (ctx,MOWARNING,"k (%ld) != j (%ld) \n",k,j);
                flag = TSH_INCORRECT_DATA;
                break;
            }
            /******************************************************************/
            /* test to see if the values are correct.                         */
            /******************************************************************/
            for (i=0; i<=j; i++)
            {
                expected_l = (49*i + 140)*i +100;
                TSHMessage (ctx, MOTRACE, 
                    "oa1[%ld] (%ld) ? expected_l (%ld) \n",
                         i, oa1[i], expected_l);
                if (oa1[i] != expected_l) 
                {
                    TSHStatus(ctx, TEST,"incorrect data returned",
                                                               FAILURE,SUCCESS);
                    TSHMessage (ctx, MOWARNING, 
                          "oa1[%ld] (%ld) != expected_l (%ld) \n",
                                                         i, oa1[i], expected_l);
                    flag = TSH_INCORRECT_DATA;
                    break;
                }
            }
            if (flag)
                break;
            /******************************************************************/
            /* test to see that the rest of the array has not been clobbered. */
            /******************************************************************/
            for (; i<arraytest_c1; i++)
            {
                expected_l = i*7+10; 
                TSHMessage (ctx, MOTRACE,
                      "oa1[%ld] (%ld) ? expected_l (%ld) \n",
                                                     i, oa1[i], expected_l);
                if (oa1[i] != expected_l) 
                {
                    TSHStatus(ctx, TEST, "value past last_is overwritten",
                                                              FAILURE, SUCCESS);
                    TSHMessage (ctx, MOWARNING,
                          "oa1[%ld] (%ld) != expected_l (%ld) \n",
                                                         i, oa1[i], expected_l);
                    flag = TSH_INCORRECT_DATA;
                    break;
                }
            }
        }
        break;
    
        case ARRAY_STATIC_LONGS:
        /**********************************************************************/
        /* test arraytest_op7 and arraytest_op8: send an array of longs with  */
        /* op7 and check that their squares are returned by op8               */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            fa1[i] = fa2[i] = i*3+10;
        arraytest_op7 (h, fa1);
        arraytest_op8 (h, fa2);
        for (i=0; i<arraytest_c1; i++) 
        {
            /******************************************************************/
            /* test to see if the values are correct.  fa1 should be unchanged*/
            /******************************************************************/
            expected_l = (3*i + 10);
            TSHMessage (ctx, MOTRACE,
                "fa1[%ld] (%ld) ? expected_l (%ld) \n", i, fa1[i], expected_l);
            if (fa1[i] != expected_l) 
            {
                TSHStatus(ctx, TEST,"input data corrupted",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
                    "fa1[%ld] (%ld) != expected_l (%ld) \n",
                         i, fa1[i], expected_l);
                flag = TSH_INCORRECT_DATA;
                break;
            }
            /******************************************************************/
            /* test to see if the values are correct.  fa2 should be squared  */
            /******************************************************************/
            expected_l = (9*i + 60)*i + 100;
            TSHMessage (ctx, MOTRACE,
                "fa2[%ld] (%ld) ? expected_l (%ld) \n",
                     i, fa2[i], expected_l);
            if (fa2[i] != expected_l) 
            {
                TSHStatus(ctx, TEST,"output data incorrect",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
                    "fa2[%ld] (%ld) != expected_l (%ld) \n",
                         i, fa2[i], expected_l);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_STATIC_LAST_LONGS:
        /**********************************************************************/
        /* test arraytest_op9 and arraytest_op10: send initial segments of an */
        /* array of longs with op9 and check that their squares and only their*/
        /* squares come back from op10.                                       */
        /**********************************************************************/
        for (j = -1; j<arraytest_c1; j++) 
        {
            for (i=0; i<arraytest_c1; i++)
                fa1[i] = fa2[i] = i*7+10;
            k = 0;
            arraytest_op9 (h, fa1, j);
            arraytest_op10 (h, fa2, &k);
            /******************************************************************/
            /* test to see if the correct number of elements was processed.   */
            /******************************************************************/
            TSHMessage(ctx,MOTRACE,"k (%ld) ? j (%ld) \n",k,j);
            if (k != j) 
            {
                TSHStatus(ctx, TEST,"last_is is not returned correctly",
                                                              FAILURE, SUCCESS);
                TSHMessage (ctx, MOWARNING, "k (%ld) != j (%ld) \n", k, j);
                flag = TSH_INCORRECT_DATA;
                break;
            }
            /******************************************************************/
            /* test to see if the values are correct.                         */
            /******************************************************************/
            for (i=0; i<=j; i++)
            {
                expected_l = (49*i + 140)*i + 100;
                TSHMessage (ctx, MOTRACE,
                    "fa2[%ld] (%ld) ? expected_l (%ld) \n",
                         i, fa2[i], expected_l);
                if (fa2[i] != expected_l)
                {
                    TSHStatus(ctx, TEST,"incorrect data returned",
                                                               FAILURE,SUCCESS);
                    TSHMessage (ctx, MOWARNING,
                        "fa2[%ld] (%ld) != expected_l (%ld) \n",
                             i, fa2[i], expected_l);
                    flag = TSH_INCORRECT_DATA;
                    break;
                }
            }
            if (flag)
                break;
            /******************************************************************/
            /* test to see that the rest of the array has not been clobbered. */
            /******************************************************************/
            for (; i<arraytest_c1; i++)
            {
                expected_l = fa1[i];
                TSHMessage (ctx, MOTRACE,
                    "fa2[%ld] (%ld) ? expected_l (%ld) \n",
                         i, fa2[i], expected_l);
                if (fa2[i] != expected_l) 
                {
                    TSHStatus(ctx, TEST, "value past last_is overwritten",
                                                              FAILURE, SUCCESS);
                    TSHMessage (ctx, MOWARNING,
                        "fa2[%ld] (%ld) != expected_l (%ld) \n",
                             i, fa2[i], expected_l);
                    flag = TSH_INCORRECT_DATA;
                    break;
                }
            }
        }
        break;
    
        case ARRAY_STATIC_OPEN_LONGS:
        /**********************************************************************/
        /* test arraytest_op11 and arraytest_op12: send open arrays of longs  */
        /* with op11 and get back an array of their squares with op12.        */
        /**********************************************************************/
        for (j = -1; j<arraytest_c1; j++) 
        {
            for (i=0; i<10; i++)
                oa1[i] = oa2[i] = i*7+10;
            k = j;
            arraytest_op11(h, j, j, oa1);
            arraytest_op12(h, &k, j, oa2);
            /******************************************************************/
            /* test to see if the correct number of elements was processed.   */
            /******************************************************************/
            TSHMessage(ctx,MOTRACE,"k (%ld) ? j (%ld) \n",k,j);
            if (k != j) 
            {
                TSHStatus(ctx, TEST,"last_is is not returned correctly",
                                                              FAILURE, SUCCESS);
                TSHMessage (ctx, MOWARNING,
                                  "k (%ld) != j (%ld) \n", k, j);
                flag = TSH_INCORRECT_DATA;
                break;
            }
            /******************************************************************/
            /* test to see if the values are correct.                         */
            /******************************************************************/
            for (i=0; i<=j; i++)
            {
                expected_l = (49*i + 140)*i +100;
                TSHMessage (ctx, MOTRACE,
                   "oa2[%ld] (%ld) ? expected_l (%ld) \n",
                         i, oa2[i], expected_l);
                if (oa2[i] != expected_l) 
                {
                    TSHStatus(ctx, TEST,"incorrect data returned",
                                                               FAILURE,SUCCESS);
                    TSHMessage (ctx, MOWARNING,
                       "oa2[%ld] (%ld) != expected_l (%ld) \n",
                             i, oa2[i], expected_l);
                    flag = TSH_INCORRECT_DATA;
                    break;
                }
            }
            if (flag)
                break;
            /******************************************************************/
            /* test to see that the rest of the array has not been clobbered. */
            /******************************************************************/
            for (; i<arraytest_c1; i++)
            {
                expected_l = oa1[i];
                TSHMessage (ctx, MOTRACE,
                   "oa2[%ld] (%ld) ? expected_l (%ld) \n",
                         i, oa2[i], expected_l);
                if (oa2[i] != expected_l) 
                {
                    TSHStatus(ctx, TEST, "value past last_is overwritten",
                                                              FAILURE, SUCCESS);
                    TSHMessage (ctx, MOWARNING,
                       "oa2[%ld] (%ld) != expected_l (%ld) \n",
                             i, oa2[i], expected_l);
                    flag = TSH_INCORRECT_DATA;
                    break;
                }
            }
        }
        break;
    
    
        case ARRAY_2D_FIXED_LONGS:
        /**********************************************************************/
        /* test arraytest_op13: send an array of longs in fa_2d_1 and check   */
        /* that their squares come back in fa_2d_22.                          */
        /**********************************************************************/
        for (i=0; i<arraytest_c2; i++)
            for (j=0; j<arraytest_c3; j++)
                fa_2d_1[i][j] = (i+j)*3+10;
        arraytest_op13 (h, fa_2d_1, fa_2d_2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c2; i++)
        {
            for (j=0; j<arraytest_c3; j++)
            {
                expected_l = square_l((ndr_long_int) (i+j)*3+10);
                TSHMessage (ctx, MOTRACE,
                    "fa_2d_2[%ld][%ld] (%ld) ? expected_l (%ld) \n",
                         i, j, fa_2d_2[i][j], expected_l);
                if (fa_2d_2[i][j] != expected_l) 
                {
                    TSHStatus(ctx, TEST,"incorrect data returned",
                                                               FAILURE,SUCCESS);
                    TSHMessage (ctx, MOWARNING,
                     "fa_2d_2[%ld][%ld] (%ld) != expected_l (%ld) \n",
                             i, j, fa_2d_2[i][j], expected_l);
                    flag = TSH_INCORRECT_DATA;
                    break;
                }
            }
            if (flag)
                break;
        }
        break; 

#ifdef BROKEN_CODE

        case ARRAY_2D_LAST_LONGS:
        /**********************************************************************/
        /* test arraytest_op14: send initial segments of a 2d array of longs  */
        /* in fa_2d_1 and check that their squares and only their squares come*/
        /* back in fa_2d_2.                                                   */
        /**********************************************************************/
        for (l = -1; l<arraytest_c2; l++) 
        {
            for (i=0; i<arraytest_c2; i++)
                for (j=0; j<arraytest_c3; j++)
                    fa_2d_1[i][j] = fa_2d_2[i][j] = (i+j)*7+10;
            k = 0;                    
            arraytest_op14 (h, fa_2d_1, fa_2d_2, l, &k);
            /******************************************************************/
            /* test to see if the correct number of elements was processed.   */
            /******************************************************************/
            TSHMessage(ctx,MOTRACE,"k (%ld) ? l (%ld) \n",k,l);
            if (k != l) 
            {
                TSHStatus(ctx, TEST,"last_is is not returned correctly",
                                                              FAILURE, SUCCESS);
                TSHMessage (ctx, MOWARNING, "k (%ld) != l (%ld) \n", k, l);
                flag = TSH_INCORRECT_DATA;
                break;
            }
            /******************************************************************/
            /* test to see if the values are correct.                         */
            /******************************************************************/
            for (i=0; i<=k; i++)
            {
                for (j=0; j<arraytest_c3; j++)
                {
                    expected_l = square_l((ndr_long_int) (i+j)*7+10);
                    TSHMessage (ctx, MOTRACE,
                     "fa_2d_2[%ld][%ld] (%ld) ? expected_l (%ld) \n",
                                 i, j,  fa_2d_2[i][j], expected_l);
                    if (fa_2d_2[i][j] != expected_l)
                    {
                        TSHStatus(ctx, TEST,
                                    "incorrect data returned",FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                     "fa_2d_2[%ld][%ld] (%ld) != expected_l (%ld) \n",
                                 i, j,  fa_2d_2[i][j], expected_l);
                        flag = TSH_INCORRECT_DATA;
                        break;

                    }
                }
                if (flag)
                break;
            }
            for (; i<arraytest_c2; i++)
            {
                for (j=0; j<arraytest_c3; j++)
                {
                    expected_l = (i+j)*7+10;
                    TSHMessage (ctx, MOTRACE,
                     "fa_2d_2[%ld][%ld] (%ld) ? expected_l (%ld) \n",
                                 i, j, fa_2d_2[i][j], expected_l);
                    if (fa_2d_2[i][j] != expected_l) 
                    {
                        TSHStatus(ctx, TEST, "value past last_is overwritten",
                                                              FAILURE, SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                     "fa_2d_2[%ld][%ld] (%ld) != expected_l (%ld) \n",
                                 i, j, fa_2d_2[i][j], expected_l);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                }
                if (flag)
                    break;
            }
            if (flag)
                break;
        }
        break;
    
    
        case ARRAY_2D_OPEN_LONGS:
        /**********************************************************************/
        /* test arraytest_op15: send open 2d arrays of longs from 0 to        */
        /* arraytest_c1 slices in length; get back an array containing the    */
        /* squares of the inputs.                                             */
        /**********************************************************************/
        for (l = -1; l<arraytest_c2; l++) 
        {
            for (i=0; i<arraytest_c2; i++)
                for (j=0; j<arraytest_c3; j++)
                    oa_2d_1[i][j] = oa_2d_2[i][j] = (i+j)*7+10;
            k = 92;
            arraytest_op15 (h, oa_2d_1, oa_2d_2, l, l, &k, l);
            /******************************************************************/
            /* test to see if the correct number of elements was processed.   */
            /******************************************************************/
            TSHMessage(ctx,MOTRACE,"k (%ld) ? l (%ld) \n",k,l);
            if (k != l) 
            {
                TSHStatus(ctx, TEST,"last_is is not returned correctly",
                                                              FAILURE, SUCCESS);
                TSHMessage (ctx, MOWARNING, "k (%ld) != l (%ld) \n", k, l);
                flag = TSH_INCORRECT_DATA;
                break;
            }
            /******************************************************************/
            /* test to see if the values are correct.                         */
            /******************************************************************/
            for (i=0; i<=l; i++)
            {
                for (j=0; j<arraytest_c3; j++)
                {
                    expected_l = square_l((ndr_long_int) (i+j)*7+10);
                    TSHMessage (ctx, MOTRACE,
                     "oa_2d_2[%ld][%ld] (%ld) ? expected_l (%ld) \n",
                                 i, j, oa_2d_1[i][j], expected_l);
                    if (oa_2d_2[i][j] != expected_l)
                    {
                        TSHStatus(ctx, TEST,"incorrect data returned",
                                                               FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                     "oa_2d_2[%ld][%ld] (%ld) != expected_l (%ld) \n",
                                 i, j, oa_2d_1[i][j], expected_l);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                }
                if (flag)
                    break;
            }
            if (flag)
                break;
           
            /******************************************************************/
            /* test to see that the rest of the array has not been clobbered. */
            /******************************************************************/
            for (; i<arraytest_c2; i++)
            {
                for (j=0; j<arraytest_c3; j++)
                {
                    expected_l = (i+j)*7+10;
                    TSHMessage (ctx, MOTRACE,
                     "oa_2d_2[%ld][%ld] (%ld) ? expected_l (%ld) \n",
                                 i, j, oa_2d_1[i][j], expected_l);
                    if (oa_2d_2[i][j] != expected_l) 
                    {
                        TSHStatus(ctx, TEST, "value past last_is overwritten",
                                                              FAILURE, SUCCESS);
                        TSHMessage (ctx, MOWARNING,
                     "oa_2d_2[%ld][%ld] (%ld) != expected_l (%ld) \n",
                                 i, j, oa_2d_1[i][j], expected_l);
                        flag = TSH_INCORRECT_DATA;
                        break;
                    }
                }
                if (flag)
                    break;
            }
            if (flag)
                break;
        }
        break;
    
#endif

        case ARRAY_FIXED_SMALLS:
        /**********************************************************************/
        /* test arraytest_op20: send a fixed array of smalls in sm1 and check */
        /* that their values + 1 come back in sm2.                            */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            sm1[i] = i;
        arraytest_op20 (h, sm1, sm2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++)
        {
            TSHMessage (ctx, MOTRACE, "sm1 (%d) ? sm2 (%ld) - 1\n", sm1, sm2);
            if (sm2[i] != (expected_sm = sm1[i] + 1)) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING, 
                  "sm2[%ld] (%ld) != expected_sm (%ld) \n",
                        i, sm2[i], expected_sm);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_FIXED_USMALLS:
        /**********************************************************************/
        /* test arraytest_op21: send a fixed array of unsigned smalls in usm1 */
        /* and check that their values + 1 come back in usm2.                 */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            usm1[i] = i*6;
        arraytest_op21 (h, usm1, usm2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++)
        {
            TSHMessage (ctx, MOTRACE, "usm1 (%d) ? usm2 (%ld) - 1\n", usm1,usm2);
            if (usm2[i] != (expected_usm = usm1[i] + 1)) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
                "usm2[%ld] (%ld) != expected_usm (%ld) \n",
                        i, usm2[i], expected_usm);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_FIXED_SHORTS:
        /**********************************************************************/
        /* test arraytest_op22: send a fixed array of shorts in sh1 and check */
        /* that their values + 1 come back in sh2.                            */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            sh1[i] = i*18;
        arraytest_op22 (h, sh1, sh2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++)
        {
            TSHMessage (ctx, MOTRACE, "sh1 (%d) ? sh2 (%ld) - 1\n", sh1, sh2);
            if (sh2[i] != (expected_sh = sh1[i] + 1)) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING, 
                  "sh2[%ld] (%ld) != expected_sh (%ld) \n",
                        i, sh2[i], expected_sh);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_FIXED_USHORTS:
        /**********************************************************************/
        /* test arraytest_op21: send a fixed array of unsigned shorts in ush1 */
        /* and check that their values + 1 come back in ush2.                 */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            ush1[i] = i*25;
        arraytest_op23 (h, ush1, ush2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++)
        {
            TSHMessage (ctx, MOTRACE,"ush1 (%d) ? ush2 (%ld) - 1\n", ush1, ush2);
            if (ush2[i] != (expected_ush = ush1[i] + 1)) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING, 
                "ush2[%ld] (%ld) != expected_ush (%ld) \n",
                        i, ush2[i], expected_ush);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_FIXED_LONGS_AGAIN:
        /**********************************************************************/
        /* test arraytest_op24: send a fixed array of longs in l1 and check   */
        /* that their values + 1 come back in l2.                             */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            l1[i] = i*101;
        arraytest_op24 (h, l1, l2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++)
        {
            TSHMessage (ctx, MOTRACE, 
                "l2[%ld] (%ld) ? l1[%ld] (%ld) + 1 \n", i, l2[i], i, l1[i]);
            if (l2[i] != (expected_l = l1[i] + 1)) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
                    "l2[%ld] (%ld) != expected_l (%ld) \n",
                        i, l2[i], expected_l);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_FIXED_ULONGS:
        /**********************************************************************/
        /* test arraytest_op25: send a fixed array of unsigned longs in ul1   */
        /* and check that their values + 1 come back in ul2.                  */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            ul1[i] = 201*i;
        arraytest_op25 (h, ul1, ul2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++)
        {
            TSHMessage (ctx, MOTRACE,
                "ul2[%ld] (%ld) ? ul1[%ld] (%ld) + 1 \n", i, ul2[i], i, ul1[i]);
            if (ul2[i] != (expected_ul = ul1[i] + 1)) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
                  "ul2[%ld] (%ld) != expected_ul (%ld) \n",
                        i, ul2[i], expected_ul);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
#ifndef cray
        case ARRAY_FIXED_HYPERS:
        /**********************************************************************/
        /* test arraytest_op26: send a fixed array of hypers in h1 and check  */
        /* that their values + 1 come back in h2.                             */
        /**********************************************************************/
        for (i = 0; i < arraytest_c1; i++) 
        {
            h1[i].low = i * 2345;
            h1[i].high = i * 5678;
            h2[i].low = 0;
            h2[i].high = 0;
        }
        arraytest_op26 (h, h1, h2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i = 0; i < arraytest_c1; i++) 
        {
            if (h1[i].low == 0xFFFFFFFF) 
            {
                expected_h.low = 0;
                expected_h.high = h1[i].high + 1;
            } 
            else 
            {
                expected_h.low = h1[i].low + 1;
                expected_h.high = h1[i].high;
            }
            TSHMessage (ctx, MOTRACE,
            "h2[%ld] (%ld,%ld) ? expected_h (%ld,%ld) \n",
                     i, h2[i].high, h2[i].low, expected_h.high, expected_h.low);
            if (h2[i].low != expected_h.low || h2[i].high != expected_h.high) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
            "h2[%ld] (%ld,%ld) != expected_h (%ld,%ld) \n",
                     i, h2[i].high, h2[i].low, expected_h.high, expected_h.low);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_FIXED_UHYPERS:
        /**********************************************************************/
        /* test arraytest_op27: send a fixed array of unsigned hypers in uh1  */
        /* and check that their values + 1 come back in uh2.                  */
        /**********************************************************************/
        for (i = 0; i < arraytest_c1; i++) 
        {
            uh1[i].low = i * 2345;
            uh1[i].high = i * 5678;
            uh2[i].low = 0;
            uh2[i].high = 0;
        }
        arraytest_op27 (h, uh1, uh2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i = 0; i < arraytest_c1; i++) 
        {
            if (uh1[i].low == 0xFFFFFFFF) 
            {
                expected_uh.low = 0;
                expected_uh.high = uh1[i].high + 1;
            }
            else 
            {
                expected_uh.low = uh1[i].low + 1;
                expected_uh.high = uh1[i].high;
            }
            TSHMessage (ctx, MOTRACE,
                "uh2[%ld] (%ld,%ld) ? expected_uh (%ld,%ld) \n",
                 i, uh2[i].high, uh2[i].low, expected_uh.high, expected_uh.low);
            if(uh2[i].low != expected_uh.low || uh2[i].high != expected_uh.high)
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
                    "uh2[%ld] (%ld,%ld) != expected_uh (%ld,%ld) \n",
                 i, uh2[i].high, uh2[i].low, expected_uh.high, expected_uh.low);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;

#else
        case ARRAY_FIXED_HYPERS:
        /**********************************************************************/
        /* test arraytest_op26: send a fixed array of hypers in h1 and check  */
        /* that their values + 1 come back in h2.                             */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++) 
        {
            h2[i] = 0;
            h1[i] = i * 0x71234568;
        }
        arraytest_op26 (h, h1, h2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++)
        {
            TSHMessage (ctx, MOTRACE,
                ("h2[%ld] (%ld) ? h1[%ld] (%ld) + 1 \n", i, h2[i], i, h1[i]);
            if (h2[i] != (expected_h = h1[i] + 1)) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING, 
                   ("h2[%ld] (%ld) != expected_h (%ld) \n",
                        i, h2[i], expected_h);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_FIXED_UHYPERS:
        /**********************************************************************/
        /* test arraytest_op27: send a fixed array of unsigned hypers in uh1  */
        /* and check that their values + 1 come back in uh2.                  */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            uh1[i] = 0x76543218;
        arraytest_op27 (h, uh1, uh2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++)
        {
            TSHMessage (ctx, MOTRACE,
                "uh2[%ld] (%ld) ? uh1[%ld] (%ld) + 1 \n", i, uh2[i], i, uh1[i]);
            if (uh2[i] != (expected_uh = uh1[i] +1)) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
                 ("uh2[%ld] (%ld) != expected_uh (%ld) \n",
                        i, uh2[i], expected_uh);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
#endif
    
        case ARRAY_FIXED_BOOLS:
        /**********************************************************************/
        /* test arraytest_op28: send a fixed array of bools in bool1 and check*/
        /* that their values come back inverted in bool2.                     */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            bool1[i] = (i%2) ? true : false;
        arraytest_op28 (h, bool1, bool2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++)
        {
            expected_bool = i%2;
            TSHMessage (ctx, MOTRACE, "bool2[%ld] (%x) ? expected_bool (%x) \n",
                        i, bool2[i], expected_bool);
            if (bool2[i] != (expected_bool ? false : true))
            {
                printf ("bool2[%ld] (%x) != expected_bool (%x) \n",
                        i, bool2[i], expected_bool);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_FIXED_BYTES:
        /**********************************************************************/
        /* test arraytest_op29: send a fixed array of bytes in byte1 and check*/
        /* that their values come back complemented in byte2.                 */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            byte1[i] = (ndr_byte) i*21;
        arraytest_op29 (h, byte1, byte2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++) 
        {
            expected_byte = ~ byte1[i];
            TSHMessage (ctx, MOTRACE, "byte2[%ld] (%x) ? expected_byte (%x) \n",
                        i, byte2[i], expected_byte);
            if (byte2[i] != expected_byte) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
                    "byte2[%ld] (%x) != expected_byte (%x) \n",
                        i, byte2[i], expected_byte);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_SHORT_FLOATS:
        /**********************************************************************/
        /* test arraytest_op30: send a fixed array of short floats in sf1 and */
        /* check that their squares come back in sf2.                         */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            sf1[i] = 0.8 * i;
        arraytest_op30 (h, sf1, sf2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++)
        {
            expected_sf = sf1[i] * sf1[i];
            TSHMessage (ctx, MOTRACE, "sf2[%ld] (%f) ? expected_sf (%f) \n",
                        i, sf2[i], expected_sf);
            if (!close_sf(sf2[i], expected_sf)) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING, 
                    "sf2[%ld] (%f) != expected_sf (%f) \n",
                        i, sf2[i], expected_sf);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_LONG_FLOATS:
        /**********************************************************************/
        /* test arraytest_op31: send a fixed array of long floats in lf1 and  */
        /* check that their squares come back in lf2.                         */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            lf1[i] = 0.8 * i;
        arraytest_op31 (h, lf1, lf2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++)
        {
            expected_lf = lf1[i] * lf1[i];
            TSHMessage (ctx, MOTRACE, "lf2[%ld] (%f) ? expected_lf (%f) \n",
                        i, lf2[i], expected_lf);
            if (!close_lf(lf2[i], expected_lf)) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING, 
                    "lf2[%ld] (%f) != expected_lf (%f) \n",
                        i, lf2[i], expected_lf);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_CHARS:
        /**********************************************************************/
        /* test arraytest_op32: send a fixed array of chars in char1 and check*/
        /* that their case is inverted in char2.                              */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++)
            char1[i] = (i%2) ? 'G' : 'f';
        arraytest_op32 (h, char1, char2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++)
        {
            expected_char = (i%2) ? 'g' : 'F';
            TSHMessage (ctx, MOTRACE, "char2[%ld] (%c) ? expected_char (%c) \n",
                        i, char2[i], expected_char);
            if (char2[i] != expected_char) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING, 
                    "char2[%ld] (%c) != expected_char (%c) \n",
                        i, char2[i], expected_char);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_STRUCTS:
        /**********************************************************************/
        /* test arraytest_op40: send a fixed array of structs in t8_1 and     */
        /* check that each member returns its value + 1 in t8_2.              */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++) 
        {
            t8_1[i].a = i*39;
            t8_1[i].b = i*41;
        }
        arraytest_op40 (h, t8_1, t8_2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++) 
        {
            expected_t8.a = t8_1[i].a + 1;
            TSHMessage (ctx, MOTRACE,
                "t8_2[%ld].a (%ld) ? expected_t8.a (%ld) \n",
                        i, t8_2[i].a, expected_t8.a);
            if (t8_2[i].a != expected_t8.a) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
                    "t8_2[%ld].a (%ld) != expected_t8.a (%ld) \n",
                        i, t8_2[i].a, expected_t8.a);
                flag = TSH_INCORRECT_DATA;
                break;
            }
            expected_t8.b = t8_1[i].b + 1;
            TSHMessage (ctx, MOTRACE,
                "t8_2[%ld].b (%ld) ? expected_t8.b (%ld) \n",
                        i, t8_2[i].b, expected_t8.b);
            if (t8_2[i].b != expected_t8.b) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
                    "t8_2[%ld].b (%ld) != expected_t8.b (%ld) \n",
                        i, t8_2[i].b, expected_t8.b);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;
    
        case ARRAY_UNIONS:
        /**********************************************************************/
        /* test arraytest_op41: send a fixed array of unions in t9_1 and check*/
        /* that each switch member returns its value + 1 in t9_2.             */
        /**********************************************************************/
        for (i=0; i<arraytest_c1; i++) 
        {
            t9_1[i].flag = ((i % 2) == 0) ? ndr_true : ndr_false;
            switch (t9_1[i].flag) 
            {
                case false:
                    t9_1[i].tagged_union.a = i*39;
                    break;
                case true:
                    t9_1[i].tagged_union.b = i*41;
            }
        }
        arraytest_op41 (h, t9_1, t9_2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++) 
        {
            TSHMessage (ctx, MOTRACE,"t9_2[%ld].flag (%ld) ? t9_2.flag (%ld)\n",
                        i, t9_2[i].flag, t9_2[i].flag);
            if (t9_2[i].flag != t9_1[i].flag)
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
                    "t9_2[%ld].flag (%ld) != t9_2.flag (%ld) \n",
                        i, t9_2[i].flag, t9_2[i].flag);
                flag = TSH_INCORRECT_DATA;
                break;
            }
            switch (t9_1[i].flag) 
            {
                case false:
                    TSHMessage (ctx, MOTRACE,
          "t9_2[%ld].tagged_union.a (%c) ? expected_t9.tagged_union.a (%c) \n",
                         i, t9_2[i].tagged_union.a, expected_t9.tagged_union.a);
                    if (t9_2[i].tagged_union.a != 
                      (expected_t9.tagged_union.a = t9_1[i].tagged_union.a + 1))
                    {
                        TSHStatus(ctx, TEST,
                                     "incorrect data returned",FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING, 
          "t9_2[%ld].tagged_union.a (%c) != expected_t9.tagged_union.a (%c) \n",
                         i, t9_2[i].tagged_union.a, expected_t9.tagged_union.a);
                        flag = TSH_INCORRECT_DATA;
                    }
                    break;
                case true:
                    TSHMessage (ctx, MOTRACE,
          "t9_2[%ld].tagged_union.b (%c) ? expected_t9.tagged_union.b (%c) \n",
                         i, t9_2[i].tagged_union.b, expected_t9.tagged_union.b);
                    if (t9_2[i].tagged_union.b !=
                      (expected_t9.tagged_union.b = t9_1[i].tagged_union.b + 1))
                    {
                        TSHStatus(ctx, TEST,
                                     "incorrect data returned",FAILURE,SUCCESS);
                        TSHMessage (ctx, MOWARNING, 
          "t9_2[%ld].tagged_union.b (%c) != expected_t9.tagged_union.b (%c) \n",
                         i, t9_2[i].tagged_union.b, expected_t9.tagged_union.b);
                        flag = TSH_INCORRECT_DATA;
                    }
                    break;
            }
            if (flag)
                break;
        }
        break;
    
        case ARRAY_FIXED_STRINGS:
        /**********************************************************************/
        /* test arraytest_op42: send a fixed array of strings in t10_1 and    */
        /* check that each element returns its value case inverted.           */
        /**********************************************************************/
        for (i = 0; i < arraytest_c1; i++)
            strcpy (t10_1[i], "AbCdEfGhIjKlMn");
        arraytest_op42 (h, t10_1, t10_2);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (i=0; i<arraytest_c1; i++)
        {
            TSHMessage (ctx, MOTRACE, 
                "strcmp (t10_1[%ld] (%s), \"aBcDeFgHiJkLmN\") ? 0 \n",
                        i, t10_2[i]);
            if (strcmp (t10_2[i], "aBcDeFgHiJkLmN") != 0) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
                    "strcmp (t10_1[%ld] (%s), \"aBcDeFgHiJkLmN\") != 0 \n",
                        i, t10_2[i]);
                flag = TSH_INCORRECT_DATA;
                break;
            }
        }
        break;

        case ARRAY_FAULTS:
        /**********************************************************************/
        /* test arraytest operations with calls that should raise an exception*/
        /**********************************************************************/
        /**********************************************************************/
        /* Add this line back in when all tests for 2D arrays work. For now   */
        /* stop at test number 6.                                             */
        /**********************************************************************/
#ifdef BROKEN_CODE
        for ( fault_test_num = 0; fault_test_num < 10; ++fault_test_num)
#else
	for ( fault_test_num = 0; fault_test_num < 7; ++fault_test_num)
#endif
        {
            TRY
            if (flag)
                break;
            switch (fault_test_num) 
            {
                case 0:
                arraytest_op2 (h, fa1, fa2, (ndr_long_int) arraytest_c1, &k);
                TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
                flag = TSH_EXCEPTION_NOT_RAISED;
                break;

                case 1:
                arraytest_op3 (h, oa1, oa2, 5L, 4L, &k, 5L);
                TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
                flag = TSH_EXCEPTION_NOT_RAISED;
                break;

                case 2:
                arraytest_op3 (h, oa1, oa2, 5L, 5L, &k, 4L);
                TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
                flag = TSH_EXCEPTION_NOT_RAISED;
                break;

                case 3:
                k = arraytest_c1;
                arraytest_op5 (h, fa1, &k);
                TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
                flag = TSH_EXCEPTION_NOT_RAISED;
                break;

                case 4:
                k = 6;
                arraytest_op6 (h, oa1, &k, 5L);
                TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
                flag = TSH_EXCEPTION_NOT_RAISED;
                break;

                case 5:
                arraytest_op9 (h, fa1, (ndr_long_int) arraytest_c1);
                TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
                flag = TSH_EXCEPTION_NOT_RAISED;
                break;

                case 6:
                arraytest_op11 (h, 7L, 10L, oa1);
                arraytest_op12 (h, &k, 6L, oa2);
                TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
                flag = TSH_EXCEPTION_NOT_RAISED;
                break;

                case 7:
#ifdef BROKEN_CODE
                arraytest_op14 (h, fa_2d_1, fa_2d_2, 
                                  (ndr_long_int) arraytest_c2, &k);
#endif
                TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
                flag = TSH_EXCEPTION_NOT_RAISED;
                break;

                case 8:
#ifdef BROKEN_CODE
                arraytest_op15 (h, oa_2d_1, oa_2d_2, 5L, 4L, &k, 5L);
#endif
                TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
                flag = TSH_EXCEPTION_NOT_RAISED;
                break;

                case 9:
#ifdef BROKEN_CODE
                arraytest_op15 (h, oa_2d_1, oa_2d_2, 5L, 5L, &k, 4L);
#endif
                TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
                flag = TSH_EXCEPTION_NOT_RAISED;
                break;
            }
            /* Catch only out of bounds here, let others filter up */
            CATCH(rpc_x_invalid_bound)
            ++fault_test_num;
            ENDTRY
        }
        break;

    }
    return(flag);
}
