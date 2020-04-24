/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ihc.c,v $
 * Revision 1.1.4.2  1996/02/17  23:20:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:23  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:05:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:22  root]
 * 
 * Revision 1.1.2.4  1993/01/25  18:04:45  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:36:41  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  22:14:30  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:06:37  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  16:40:59  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/12  14:42:14  cuti]
 * 
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  17:59:23  cuti]
 * 
 * Revision 1.1  1992/01/19  03:35:15  devrcs
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



#include <ih.h>
#include <ihc.h>
#include <nidltest.h>
#include <tsh_common.h>
#include <stdio.h>
#include <math.h>
#include <dce/rpc.h> 

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
    return(implicit_handle_test(h, ctx, subcase));
}

int implicit_handle_test (h, ctx, subcase)
handle_t h;
CtxBlock *ctx;
ndr_long_int subcase;
{
    ndr_long_int o;

    implicit_handle_var = h;

    switch(subcase)
    {

        case IMPLICIT_HANDLE_USE:
        /**********************************************************************/
        /* test implicit_handle_op1: pass in a long int, add 1 to it and pass */
        /* it back in a separate slot.  Actually the data is irrelevent since */
        /* marshalling isn't being tested here.  Only the fact that the call  */
        /* succeeds without supplying a handle is important.                  */
        /**********************************************************************/
        implicit_test_op1 (10L, &o);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        TSHMessage (ctx, MOTRACE, "o (%ld) ? 11\n", o);
        if (o != 11) 
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage (ctx, MOWARNING, "o (%ld) != 11\n", o);
            return(TSH_INCORRECT_DATA);
        }
        break;

        case IMPLICIT_HANDLE_OVERRIDE:
        /**********************************************************************/
        /* test implicit_handle_op1: pass in a long int, add 1 to it and pass */
        /* it back in a separate slot.  Actually the data is irrelevent since */
        /* marshalling isn't being tested here.  Only the fact that the call  */
        /* succeeds by ignoring a fake handle_t arg in first position.        */
        /**********************************************************************/
        implicit_test_op2 (h, 100L, &o);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        TSHMessage (ctx, MOTRACE, "o (%ld) ? 101\n", o);
        if (o != 101) 
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage (ctx, MOWARNING, "o (%ld) != 101\n", o);
            return(TSH_INCORRECT_DATA);
        }
        break;
    }
    return(SUCCESS);
}
