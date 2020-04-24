/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: specc.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:15  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:11  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:10:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:08  root]
 * 
 * Revision 1.1.2.4  1993/01/25  19:28:08  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:44:27  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  23:40:24  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:14:05  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  17:37:24  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  21:24:08  cuti]
 * 
 * Revision 1.1  1992/01/19  03:37:05  devrcs
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



#include <spec.h>
#include <specc.h>
#include <nidltest.h>
#include <tsh_common.h>
#include <stdio.h>
#include <math.h>
#include <dce/rpc.h> 
#include <dce/rpcexc.h>
#include <dce/pthread_exc.h>
#include <specexc.h>

#ifdef OSF
  extern void *malloc();
#else
/*
  extern char *malloc ();
*/
#endif

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
    return(specials_test(h, ctx, subcase));
}

int specials_test (h, ctx, subcase)
handle_t h;
CtxBlock *ctx;
ndr_long_int subcase;
{
    int i, failures = 0;
    specialtest_t1 *big_array = NULL;
    ndr_long_int last;
    ndr_long_int flag = SUCCESS;
    ndr_boolean value;
    EXCEPTION *valid = NULL;

    TRY
    switch(subcase)
    {

        case SPEC_SEQUENCE:
        /**********************************************************************/
        /* test specialtest_op1 op_2 op_3: All three are needed to see if data*/
        /* is being sent, stored, and retrieved properly.                     */
        /**********************************************************************/
        for (i = 0; i < 10; i++) 
        {
            value = specialtest_op3(h);
            /******************************************************************/
            /* test to see if the value is correct.                           */
            /******************************************************************/
            TSHMessage (ctx, MOTRACE, "specialtest_op3() %s ? false.\n",
                value ? "true" : "false");
            if (value != false) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOFAILURE, "specialtest_op3() %s != false.\n",
                                                     value ? "true" : "false");
                flag = TSH_INCORRECT_DATA;
                break;
            }
            specialtest_op1(h);
            value = specialtest_op3(h);
            /******************************************************************/
            /* test to see if the value is correct.                           */
            /******************************************************************/
            TSHMessage (ctx, MOTRACE, "specialtest_op3() %s ? true.\n",
                value ? "true" : "false");
            if (value != true) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOFAILURE, "specialtest_op3() %s != true.\n",
                                                     value ? "true" : "false");
                flag = TSH_INCORRECT_DATA;
                break;
            }
            specialtest_op2(h);
        }
        break;

        case SPEC_SERVER_FREE_STORAGE_EXCEPTION:
        /**********************************************************************/
        /* specialtest_op4: tests whether or not an application specific      */
        /* exception is mapped onto rpc_x_unknown_remote_fault. There is no   */
        /* support for application specific faults. The intention of this test*/
        /* was also to test the memory allocation in the server. If an        */
        /* exception is raised in the server you want to make sure that all   */
        /* the memory that has been allocated in the server stub so far is    */
        /* going to be freed correctly. However, this is very hard to test    */
        /* from within the application itself.                                */
        /**********************************************************************/
        big_array = (specialtest_t1 *) malloc (sizeof(specialtest_t1) * 20001);
        valid = &rpc_x_unknown_remote_fault;
        for (i = 0; i < 10; ++i ) 
        {
            specialtest_op4 (h, big_array, big_array, big_array, &last, 20000L);
            TSHStatus(ctx, TEST, "unexpected normal return \n",
                                                              FAILURE, SUCCESS);
            flag = TSH_EXCEPTION_NOT_RAISED;
            break;
        }
        free (big_array);
        break;
    }

    CATCH_ALL

        if (big_array != NULL)
            free (big_array);

        if (valid && (exc_matches(THIS_CATCH, valid)))
            flag = SUCCESS;
        else
        {
            TSHStatus(ctx, TEST,"wrong exception raised", FAILURE,SUCCESS);
            RERAISE;
            flag = TSH_EXCEPTION_CAUGHT;
        }
    ENDTRY
    return(flag);
}
