/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pipec.c,v $
 * Revision 1.1.4.2  1996/02/17  23:21:43  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:52  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:08:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:27  root]
 * 
 * Revision 1.1.2.4  1993/01/25  18:06:48  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:42:36  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  23:38:29  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:11:36  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  14:26:43  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	Move anarray[4090] to be a global data for HP/PA port.
 * 	[1992/10/09  20:23:51  cuti]
 * 
 * Revision 1.1  1992/01/19  03:35:23  devrcs
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



#include <pipe.h>
#include <pipec.h>
#include <nidltest.h>
#include <dce/stubbase.h>
#include <tsh_common.h>
#include <math.h>
/* #include <pthread.h> */
#include <dce/rpc.h>



#define ARGS_REQUIRED 4
ndr_long_int *myarray;
ndr_long_int mycount;
ndr_long_int anarray[4090]; /* This has to be a global data.  It was defined
                               in main, it failed in HPPA 2/26/92 [cuti] */


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
    return(pipes_test(h, ctx, subcase));
}

static void myalloc(state, bsize, buf, bcount)
void *state;
ndr_ulong_int bsize;
ndr_long_int **buf;
ndr_ulong_int *bcount;
{
    /* Fake the whole thing by pointing directly to the real array */
    *buf = myarray;   /* point to the correct position in the array */
    *bcount = bsize;  /* give them back the size they requested */
}

static void mypull(state, buf, bsize, bcount)
void *state;
ndr_long_int *buf;
ndr_ulong_int bsize;
ndr_ulong_int *bcount;
{
    /* don't really have to copy anything cause it's there already */
    if (mycount > bsize)
    {
        *bcount = bsize;
        mycount -= bsize;
    }
    else
    {
        *bcount = mycount;
        mycount = 0;
    }
	if(*bcount == 0)
		myarray = state;
	else 
    	myarray += *bcount;
}

static void mypush(state, buf, count)
void *state;
ndr_long_int *buf;
ndr_ulong_int count;
{
    ndr_long_int i;

    for (i = 0; i < count; ++i)
        *myarray++ = *buf++;
}

int pipes_test (h, ctx, subcase)
handle_t h;
CtxBlock *ctx;
ndr_long_int subcase;
{
    ndr_long_int i, total, value = 0;
    ndr_long_int flag = SUCCESS;
    pipe_t my_pipe;
    pipe_t *my_pipe_ptr;
    ndr_long_int result = 0;



    for (i = 0; i < 4090; ++i)
        result += i;
    myarray = anarray;
    mycount = 4090;
    my_pipe.pull = mypull;
    my_pipe.alloc = myalloc;
    my_pipe.push = mypush;
	my_pipe.state = (void *)anarray;
    my_pipe_ptr = &my_pipe;
    switch(subcase)
    {
        case PIPE_IN:
        /**********************************************************************/
        /* test pipetest_op1: Use a pipe as an in only.  See if the data all  */
        /* makes it to the server by checking the return value.               */
        /**********************************************************************/
        for (i = 0; i < 4090; ++i)
            anarray[i] = i;
        pipetest_op1 (h, my_pipe, &value);
        if (value != result)
        {
            TSHStatus(ctx, TEST, "incorrect data returned", value, result);
            TSHMessage(ctx, MOWARNING, "expected (%ld) != value (%ld)\n", 
                         result, value);
            flag = TSH_INCORRECT_DATA;
        }
        break;

        case PIPE_PTR_IN:
        /**********************************************************************/
        /* test pipetest_op2: Use a pointer to a pipe as an in only.  See if  */
        /* the data all makes it to the server by checking the return value.  */
        /**********************************************************************/
        for (i = 0; i < 4090; ++i)
            anarray[i] = i;
        pipetest_op2 (h, my_pipe_ptr, &value);
        if (value != result)
        {
            TSHStatus(ctx, TEST, "incorrect data returned", value, result);
            TSHMessage(ctx, MOWARNING, "expected (%ld) != value (%ld)\n", 
                         result, value);
            flag = TSH_INCORRECT_DATA;
        }
        break;

        case PIPE_OUT:
        /**********************************************************************/
        /* test pipetest_op3: Use a pipe as an out only.  See if the data all */
        /* makes it to the client by checking the return value.               */
        /**********************************************************************/
        for (i = 0; i < 4090; ++i)
            anarray[i] = 0;
        pipetest_op3 (h, my_pipe);
        for (i = 0; i < 4090; ++i)
            value += anarray[i];
        if (value != result)
        {
            TSHStatus(ctx, TEST, "incorrect data returned", value, result);
            TSHMessage(ctx, MOWARNING, "expected (%ld) != value (%ld)\n", 
                         result, value);
            flag = TSH_INCORRECT_DATA;
        }
        break;

        case PIPE_PTR_OUT:
        /**********************************************************************/
        /* test pipetest_op4: Use a pointer to a pipe as an out only.  See if */
        /* the data all makes it to the client by checking the return value.  */
        /**********************************************************************/
        for (i = 0; i < 4090; ++i)
            anarray[i] = 0;
        pipetest_op4 (h, my_pipe_ptr);
        for (i = 0; i < 4090; ++i)
            value += anarray[i];
        if (value != result)
        {
            TSHStatus(ctx, TEST, "incorrect data returned", value, result);
            TSHMessage(ctx, MOWARNING, "expected (%ld) != value (%ld)\n", 
                         result, value);
            flag = TSH_INCORRECT_DATA;
        }
        break;

        case PIPE_IN_OUT:
        /**********************************************************************/
        /* test pipetest_op5: Use a pipe as an in and out. See if the data    */
        /* gets in and out properly.                                          */
        /**********************************************************************/
        total = 0;
        for (i = 0; i < 4090; ++i)
            anarray[i] = i;
        pipetest_op5 (h, my_pipe, &value);
        if (value != result)
        {
            TSHStatus(ctx, TEST, "incorrect data returned", value, result);
            TSHMessage(ctx, MOWARNING, "expected (%ld) != value (%ld)\n", 
                         result, value);
            flag = TSH_INCORRECT_DATA;
            break;
        }
        if (anarray[0] == 4089)
        {
            for (i = 0; i < 4090; ++i)
                total += anarray[i];
        }
        else
        {
            TSHStatus(ctx, TEST, "incorrect data returned", 0, 1);
            TSHMessage(ctx, MOWARNING, "expected (4089) != value (%ld)\n", 
                         anarray[0]);
            flag = TSH_INCORRECT_DATA;
            break;
        }
        if (total != result)
        {
            TSHStatus(ctx, TEST, "incorrect data returned", total, result);
            TSHMessage(ctx, MOWARNING, "expected (%ld) != value (%ld)\n", 
                         result, total);
            flag = TSH_INCORRECT_DATA;
        }
        break;

        case PIPE_ORDER_ERROR:
        /**********************************************************************/
        /* test pipetest_op6: raise a pipe order exception by pushing data    */
        /* into the pipe before pullind data out of it.                       */
        /**********************************************************************/
        TRY
        total = 0;
        for (i = 0; i < 4090; ++i)
            anarray[i] = i;
        pipetest_op6 (h, my_pipe, &value);
        TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
        flag = TSH_EXCEPTION_NOT_RAISED;
        CATCH(rpc_x_ss_pipe_order)
        ENDTRY
        break;

        case PIPE_CLOSED_ERROR:
        /**********************************************************************/
        /* test pipetest_op7: raise a pipe closed error by sending more data  */
        /* after sending a zero length datum to close the pipe.               */
        /**********************************************************************/
        TRY
        for (i = 0; i < 4090; ++i)
            anarray[i] = 0;
        pipetest_op7 (h, my_pipe);
        TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
        flag = TSH_EXCEPTION_NOT_RAISED;
        CATCH(rpc_x_ss_pipe_closed)
        ENDTRY
        break;

        case PIPE_EMPTY_ERROR:
        /**********************************************************************/
        /* test pipetest_op8: raise a pipe empty error by attempting to read  */
        /* from a pipe that already returned a zero length chunk.             */
        /**********************************************************************/
        TRY
        for (i = 0; i < 4090; ++i)
            anarray[i] = i;
        pipetest_op8 (h, my_pipe, &value);
        TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
        flag = TSH_EXCEPTION_NOT_RAISED;
        CATCH(rpc_x_ss_pipe_empty)
        ENDTRY
        break;

        case PIPE_MEMORY_ERROR:
        /**********************************************************************/
        /* test pipetest_op9: raise a pipe memory error by attempting a pull  */
        /* and supplying a zero amount to the pull routine.                   */
        /**********************************************************************/
        TRY
        for (i = 0; i < 4090; ++i)
            anarray[i] = i;
        pipetest_op9 (h, my_pipe, &value);
        TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
        flag = TSH_EXCEPTION_NOT_RAISED;
        CATCH(rpc_x_ss_pipe_memory)
        ENDTRY
        break;


/*      case PIPE_DISCIPLINE_ERROR:     */
        /**********************************************************************/
        /* test pipetest_op8: raise a pipe discipline error by returning to   */
        /* the caller before filling the pipe properly (sending the zero).    */
        /**********************************************************************/
/*      TRY
        for (i = 0; i < 4090; ++i)
            anarray[i] = 0;
        pipetest_op8 (h, my_pipe);
        TSHStatus(ctx,TEST,"unexpected normal return",FAILURE,SUCCESS);
        flag = TSH_EXCEPTION_NOT_RAISED;
        CATCH(rpc_x_ss_pipe_closed)
        ENDTRY
        break;
*/
    }
    return(flag);
}
