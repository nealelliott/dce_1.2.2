/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pattrc.c,v $
 * Revision 1.1.8.2  1996/02/17  23:21:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:49  marty]
 *
 * Revision 1.1.8.1  1995/12/11  20:08:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:20  root]
 * 
 * Revision 1.1.5.1  1994/04/07  22:56:32  tom
 * 	Bug 9967 - Fix up race in CONTEXT_HANDLE test,
 * 	just check the return value for true/false.
 * 	[1994/04/07  22:55:57  tom]
 * 
 * Revision 1.1.2.7  1993/04/08  20:11:41  cuti
 * 	Add CN's return code (rpc_s_connection_closed) to check
 * 	for rpc_is_server_listening.
 * 	[1993/04/08  20:10:25  cuti]
 * 
 * Revision 1.1.2.6  1993/03/02  14:55:07  cuti
 * 	Remove rpc_ss_free() 'cause stub has freed memory.
 * 	[1993/03/01  22:01:47  cuti]
 * 
 * Revision 1.1.2.5  1993/02/03  18:21:35  cuti
 * 	Add rpc_ss_free(), rpc_ss_client_free() and rpc_ss_destroy_client_context()
 * 	[1993/02/02  22:38:22  cuti]
 * 
 * Revision 1.1.2.4  1993/01/25  18:06:39  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:42:19  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  23:38:04  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:11:05  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  17:11:48  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  20:22:57  cuti]
 * 
 * Revision 1.1  1992/01/19  03:36:17  devrcs
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
# Revision 1.1  91/09/11  16:12:40  marco
# Initial revision
# 
*/

#include <pattr.h>
#include <pattrc.h>
#include <dce/rpc.h>
#include <dce/idlbase.h>
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
    return(parameter_attributes_test(h, ctx, subcase));
}

int parameter_attributes_test(h, ctx, subcase)
handle_t h;
CtxBlock *ctx;
ndr_long_int subcase;
{
    ndr_long_int i, o;
    ndr_long_int  no_of_exceptions = 7;
    ndr_long_int my_exception;
    ndr_long_int flag = SUCCESS; 
	EXCEPTION	st_in;
    error_status_t st_out;
    realstruct a_struct;
    ptrtest a_ptrtest_ptr;
    ptrtest *someptr;

    switch(subcase)
    {

        case PATTR_AUTO_COMM_STAT:
        /**********************************************************************/
        /* test pattrtest_op1: test comm_status parameter attribute as an out.*/
        /* See if rpc_x_call_failed, rpc_x_comm_failure,  rpc_x_invalid_tag,  */
        /* rpc_x_unknown_error, rpc_x_no_memory are the exceptions returned   */
        /* in status.                                                         */
        /**********************************************************************/
        for (my_exception = 0; my_exception < no_of_exceptions; ++my_exception) 
        {
	    TRY
            /* must check all possible exceptions	*/
			st_out = my_exception;     
			pattrtest_op1(h, (error_status_t *) &st_out);

            switch (my_exception) {
		case 0 : if (st_out != rpc_s_fault_unspec)
				flag = TSH_INCORRECT_DATA;
			break;
                case 1 : if (st_out != rpc_s_fault_unspec)
                             flag = TSH_INCORRECT_DATA;
                         break;
                case 2 : if (st_out != rpc_s_fault_invalid_tag)
                             flag = TSH_INCORRECT_DATA;
                         break;
                case 3 : if (st_out != rpc_s_fault_invalid_bound)
                             flag = TSH_INCORRECT_DATA;
                         break;
                case 4 : if (st_out != rpc_s_fault_unspec)
                             flag = TSH_INCORRECT_DATA;
                         break;
                case 5 : if (st_out != rpc_s_fault_unspec)
                             flag = TSH_INCORRECT_DATA;
                         break;
                case 6 : if (st_out != rpc_s_fault_remote_no_memory)
                             flag = TSH_INCORRECT_DATA;
                         break;
                default: flag = TSH_INCORRECT_DATA;
                         break;
            } 
           

            if (flag == TSH_INCORRECT_DATA)
                TSHStatus(ctx, TEST, "incorrect status returned", FAILURE, SUCCESS);

            CATCH_ALL
            if (my_exception == 1)
            { /* If an exception raised for a comm failure */
                flag = TSH_EXCEPTION_CAUGHT;  /* its an error. */
                RERAISE;
            }
            ENDTRY
	    if(flag)
	        break;
        }
        break;

        case PATTR_COMM_STAT:
        /**********************************************************************/
        /* test pattrtest_op2: test comm_status parameter attribute as an out.*/
        /* See if rpc_x_comm_failure is the only exception returned in status.*/
        /**********************************************************************/
        for (my_exception = 0; my_exception < no_of_exceptions; ++my_exception) 
        {
	    TRY
            /* must check all possible exceptions to see that only a comm     */
            /* failure gets reported back in status.                          */
 
            pattrtest_op2(h, my_exception, &st_out);

            /* If it gets here, it must be returned as an out not an exception*/
            /* if the out is not a comm failure, error */ 

            switch (my_exception) {
		case 0 : if (st_out != rpc_s_fault_unspec)
				flag = TSH_INCORRECT_DATA;
			break;
                case 1 : if (st_out != rpc_s_fault_unspec)
                             flag = TSH_INCORRECT_DATA;
                         break;
                case 2 : if (st_out != rpc_s_fault_invalid_tag)
                             flag = TSH_INCORRECT_DATA;
                         break;
                case 3 : if (st_out != rpc_s_fault_invalid_bound)
                             flag = TSH_INCORRECT_DATA;
                         break;
                case 4 : if (st_out != rpc_s_fault_unspec)
                             flag = TSH_INCORRECT_DATA;
                         break;
                case 5 : if (st_out != rpc_s_fault_unspec)
                             flag = TSH_INCORRECT_DATA;
                         break;
                case 6 : if (st_out != rpc_s_fault_remote_no_memory)
                             flag = TSH_INCORRECT_DATA;
                         break;
                default: flag = TSH_INCORRECT_DATA;
                         break;
            } 
/*

            if ((my_exception != 1) || 
                (my_exception == 1 && st_out != rpc_s_comm_failure))
            {
                TSHStatus(ctx, TEST, "incorrect status returned", FAILURE, SUCCESS);
                flag = TSH_INCORRECT_DATA;
            } 
*/

            CATCH_ALL
            if (my_exception == 1)/* If an exception raised for a comm failure*/
                flag = TSH_EXCEPTION_CAUGHT;    /* its an error. */ 
            ENDTRY
	    if(flag)
	        break;
        }
        break;

        case PATTR_INT_REF_IN:
        /**********************************************************************/
        /* test pattrtest_op3: pass in an int by reference, add 1 to it and   */
        /* pass it back in a separate slot.                                   */
        /**********************************************************************/

        i = 34;
        o = 98;
        pattrtest_op3 (h, &i, &o);
        TSHMessage (ctx, MOTRACE, "o (%ld) ? i + 1 (%ld).\n", o, i + 1);
        if (o != i + 1) 
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage (ctx, MOWARNING, "o (%ld) != i + 1 (%ld).\n", o, i + 1);
            flag = TSH_INCORRECT_DATA;
        }
        break;

        case PATTR_PTR_DATA:
        /**********************************************************************/
        /* test pattrtest_op4: pass in a pointer to a pointer to a struct and */
        /* receive back a filled in struct.                                   */
        /**********************************************************************/
        a_ptrtest_ptr = &a_struct; 
        pattrtest_op4 (h, &a_ptrtest_ptr, &o);
        if (a_ptrtest_ptr == NULL)
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage (ctx, MOWARNING, "pointer is still null\n");
            flag = TSH_INCORRECT_DATA;
            break;
        }
        TSHMessage (ctx, MOTRACE,
    "a_ptrtest_ptr->a = %ld, a_ptrtest_ptr->b = %ld, a_ptrtest_ptr->c = %ld\n", 
            a_ptrtest_ptr->a, a_ptrtest_ptr->b, a_ptrtest_ptr->c);
        if (a_ptrtest_ptr->a != 1 || a_ptrtest_ptr->b != 2 || 
                         a_ptrtest_ptr->c != 3)
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage (ctx, MOWARNING,
             "expected 1,2,3 a = %ld, b = %ld, c = %ld\n",
            a_ptrtest_ptr->a, a_ptrtest_ptr->b, a_ptrtest_ptr->c);
            flag = TSH_INCORRECT_DATA;
        }  
		rpc_ss_client_free(a_ptrtest_ptr);
		TSHMessage(ctx, MOTRACE, "rpc_ss_client_free called \n"); 
        break;

        case PATTR_PTR_NON_NULL_TO_NON_NULL:
        /**********************************************************************/
        /* test pattrtest_op5: pass in a pointer to a pointer to a struct and */
        /* receive back a filled in pointer to a different struct.            */
        /**********************************************************************/

/*      rpc_ss_client_allocate_enable();  */
        a_ptrtest_ptr = &a_struct;
        pattrtest_op5 (h, &a_ptrtest_ptr, &o);
        if (a_ptrtest_ptr == NULL)
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage (ctx, MOWARNING, "pointer is still null\n");
            flag = TSH_INCORRECT_DATA;
            break;
        }
        TSHMessage (ctx, MOTRACE,
    "a_ptrtest_ptr->a = %ld, a_ptrtest_ptr->b = %ld, a_ptrtest_ptr->c = %ld\n", 
            a_ptrtest_ptr->a, a_ptrtest_ptr->b, a_ptrtest_ptr->c);
        if (a_ptrtest_ptr->a != 1 || a_ptrtest_ptr->b != 2 || 
                         a_ptrtest_ptr->c != 3)
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage (ctx, MOWARNING,
             "expected 1,2,3 a = %ld, b = %ld, c = %ld\n",
            a_ptrtest_ptr->a, a_ptrtest_ptr->b, a_ptrtest_ptr->c);
            flag = TSH_INCORRECT_DATA;
        }    
        break;

        case PATTR_PTR_NULL_TO_NON_NULL:
        /**********************************************************************/
        /* test pattrtest_op5: pass in a pointer to a pointer to a null and   */
        /* receive back a filled in pointer to data.                          */
        /**********************************************************************/

/*      rpc_ss_client_allocate_enable();  */
        a_ptrtest_ptr = NULL;
        someptr = &a_ptrtest_ptr;
        pattrtest_op5 (h, someptr, &o);
        if (a_ptrtest_ptr == NULL)
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage (ctx, MOWARNING, "pointer is still null\n");
            flag = TSH_INCORRECT_DATA;
            break;
        }
        TSHMessage (ctx, MOTRACE,
    "a_ptrtest_ptr->a = %ld, a_ptrtest_ptr->b = %ld, a_ptrtest_ptr->c = %ld\n", 
            a_ptrtest_ptr->a, a_ptrtest_ptr->b, a_ptrtest_ptr->c);
        if (a_ptrtest_ptr->a != 1 || a_ptrtest_ptr->b != 2 || 
                         a_ptrtest_ptr->c != 3)
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage (ctx, MOWARNING,
             "expected 1,2,3 a = %ld, b = %ld, c = %ld\n",
            a_ptrtest_ptr->a, a_ptrtest_ptr->b, a_ptrtest_ptr->c);
            flag = TSH_INCORRECT_DATA;
        }
        break;

        case PATTR_PTR_DATA_TO_NULL:
        /**********************************************************************/
        /* test pattrtest_op4: pass in a pointer to a pointer to a struct and */
        /* receive back a null pointer.                                       */
        /**********************************************************************/

        a_ptrtest_ptr = &a_struct;
        someptr = &a_ptrtest_ptr;
        pattrtest_op6 (h, someptr, &o);
        if (a_ptrtest_ptr != NULL)
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage (ctx, MOWARNING, "pointer is still valid\n");
            flag = TSH_INCORRECT_DATA;
            break;
        }
        break;  
	
	case CONTEXT_HANDLE: 
        {
    	    my_context this_object=NULL;  
	    int	listening;
    
    	    ch_op1(h, &this_object);
    	    ch_op2(h, this_object, &st_out);
            TSHMessage (ctx, MOTRACE, "ch_op2: %d\n", st_out);
            listening = rpc_mgmt_is_server_listening(h, &st_out);
            TSHMessage (ctx, MOTRACE, "rpc_mgmt_is_server_listening: %d\n", st_out);
            while (listening) {
                listening = rpc_mgmt_is_server_listening(h, &st_out);
                TSHMessage (ctx, MOTRACE, "rpc_mgmt_is_server_listening: %d\n", st_out);
    	    }

	    rpc_ss_destroy_client_context(&this_object);
	    TSHMessage (ctx, MOTRACE, "rpc_ss_destroy_client_context, context: %d\n", this_object);
	    if (this_object == NULL)
                TSHMessage (ctx, MOTRACE, "rpc_ss_destroy_client_context passed\n");
	    else
        	TSHMessage (ctx, MOTRACE, "rpc_ss_destroy_client_context failed\n");
	    break;
   	}
    }
    return(flag);
}

