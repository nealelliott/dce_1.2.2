/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rattrc.c,v $
 * Revision 1.1.5.2  1996/02/17  23:21:46  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:54  marty]
 *
 * Revision 1.1.5.1  1995/12/11  20:08:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:32  root]
 * 
 * Revision 1.1.2.4  1993/01/25  19:27:01  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:43:00  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  23:38:48  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:11:56  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  17:17:28  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  20:24:44  cuti]
 * 
 * Revision 1.1  1992/01/19  03:36:30  devrcs
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



#include <rattr.h>
#include <rattrc.h>
#include <nidltest.h>
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
	short		dbg;
    unsigned_char_p_t bound_string;
    ndr_char *addr_string;
    ndr_char *address;
    error_status_t status;
    handle_t h;


    address = addr_string = (ndr_char *)"253.1.1.1";

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
    if (subcase == RATTR_MAYBE || subcase == RATTR_IDEMPOTENT)
        address = argv[2]; 
    dbg = atoi(argv[1]);
/*	if (!dbg)
rpc__dbg_set_switches("0-4.7", &status); */

	
    /**************************************************************************/
    /*  Do the compose of the string, followed by the binding from string.    */
    /**************************************************************************/
    rpc_string_binding_compose(
       NULL, (unsigned_char_p_t)argv[3], 
       address, NULL, NULL, &bound_string, &status);

    if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", status, rpc_s_ok))
       return(status);
    rpc_binding_from_string_binding(bound_string, &h, &status);
    if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", status, rpc_s_ok))
       return(status);
    return(routine_attributes_test(h, ctx, subcase));
}

int routine_attributes_test (h, ctx, subcase)
handle_t h;
CtxBlock *ctx;
ndr_long_int subcase;
{
    ndr_boolean call_rejected;
    ndr_long_int flag, test;

    ndr_long_int i, j;

    switch(subcase)
    {

        case RATTR_MAYBE:
        /**********************************************************************/
        /* test [maybe] operations.  Make a call with the maybe attribute set */
        /* which assigns a value to a static param in the server. Attempt to  */
        /* read it back with a subsequent call.  If it ever gets there, the   */
        /* test succeeds.  How many times the test succeeds is irrelevent.    */
        /**********************************************************************/
        flag = TSH_TEST_FAILED;
        for (i = 0; i < 10; i++) 
        {
            rattrtest_op1 (h, 22);
            j = rattrtest_op2(h);
            TSHMessage (ctx, MOTRACE, "value read (%ld) ?  value sent 22\n", j);
            if (j == 22)  /* a single maybe which gets there is a success */
            {
                flag = SUCCESS;
                break;
            }
            if (j != 0)  /* any value which is not 22 or 0 indicates an error*/
                break;
        }
        if (flag)
        {
            if (j != 0)
            {
                TSHStatus(ctx, TEST,"[maybe] data was corrupt",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
                   "value read (%ld) ?  value sent 22\n", j);
            }
            else
            {
                TSHStatus(ctx, TEST,"[maybe] call was lost",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,
                   "value read (%ld) ?  value sent 22\n", j);
            }
        }
        break;

        case RATTR_IDEMPOTENT:
        /**********************************************************************/
        /* test [idempotent] operations.  Make a call with the idempotent     */
        /* attribute set which assigns a value to a static param in the server*/
        /* Attempt to read it back with a subsequent call.  If it there, one  */
        /* or more times it's successful, else it fails.                      */
        /**********************************************************************/
        flag = SUCCESS;
        for (i = 1; i < 10; i++) 
        {
            rattrtest_op4(h); /* clear the value in the server */
            j = rattrtest_op3(h); /* bump the count */
            TSHMessage(ctx,MOTRACE,"value read (%ld) ?  value expected > 0\n",j);
            switch (j) 
            {
                case 0:
                    TSHStatus(ctx, TEST,"no data returned",FAILURE,SUCCESS);
                    flag = TSH_INCORRECT_DATA;
                    break;
                case 1:
                    break;
                default:
                    TSHMessage(ctx,MOTRACE,
                        "call was executed %ld times with one call.\n", j);
            }
            if (flag)
                break;
        }
        break;


        case RATTR_NON_BROADCAST:
        /**********************************************************************/
        /* verify that a handle with a bogus address will generate a fault, if*/
        /* the cstub is not set up to do a broadcast.  (If it were set up to  */
        /* broadcast, the address should be ignored).                         */
        /**********************************************************************/
        TRY
            flag = SUCCESS;
            call_rejected = false;
            rattrtest_op2(h);
        CATCH_ALL
            /* looking for rpc_x_comm_failure */
            call_rejected = true;
/*#ifdef OSF */
            if (!exc_matches(THIS_CATCH, &rpc_x_comm_failure))
/*#else
            if (!cma_exc_matches(THIS_CATCH, &rpc_x_comm_failure))
#endif     */
            {
                flag = TSH_EXCEPTION_CAUGHT;
                TSHStatus(ctx, TEST,
                 "Calling with a bogus address caused wrong exception.\n",
                                                            FAILURE,SUCCESS);
                RERAISE;
            }
        ENDTRY
        if (!call_rejected) 
        {
            TSHStatus(ctx, TEST,
             "Calling with a bogus address did not cause an exception.\n",
                                                            FAILURE,SUCCESS);
            flag = TSH_EXCEPTION_NOT_RAISED;
        }
        break;

        case RATTR_BROADCAST:
        /**********************************************************************/
        /* verify that a handle with a bogus address will not generate a fault*/
        /* if the cstub is set up to do a broadcast.  (The address should be  */
        /* ignored).                                                          */
        /**********************************************************************/
        TRY
        flag = SUCCESS;
        test = 1;
        rattrtest_op5(h, 1059L);
        test = 2;
/*        if (rattrtest_op6(h)) 
            flag = TSH_EXCEPTION_NOT_RAISED;  */
        CATCH_ALL
        if (test == 1)  /* I shouldn't be here. It means that a fault was */
        {               /* raised on broadcast. The address was used.     */
            flag = TSH_EXCEPTION_CAUGHT;
            RERAISE;
        }
        else
        {   
             flag = TSH_EXCEPTION_CAUGHT;
             RERAISE;
            /* op6 test faulted, which it should'nt be. The handle is  */
            /* filled in by the op5 call.                              */
        }
        ENDTRY
        break;
    }

    return(flag);
}
