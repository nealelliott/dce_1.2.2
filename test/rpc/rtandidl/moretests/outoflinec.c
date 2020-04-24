/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: outoflinec.c,v $
 * Revision 1.1.4.2  1996/02/17  23:21:34  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:48  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:08:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:12  root]
 * 
 * Revision 1.1.2.4  1993/01/25  18:06:22  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:41:58  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  23:37:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:10:38  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  17:08:05  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  20:22:02  cuti]
 * 
 * Revision 1.1  1992/01/19  03:36:09  devrcs
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



#include <outofline.h>
#include <outoflinec.h>
#include <nidltest.h>
#include <tsh_common.h>
#include <stdio.h>
#include <math.h>
#include <dce/rpc.h> 
#include <pthread.h>
#include <dce/rpcexc.h> 

/* extern char * malloc(); */
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
    return(outofline_test(h, ctx, subcase));
}

int outofline_test (h, ctx, subcase)
handle_t h;
CtxBlock *ctx;
ndr_long_int subcase;
{
    outofline_t1 fa1, fa2;
    ndr_long_int i, expected_l;
    ndr_long_int flag;
    
    flag = SUCCESS;
    switch(subcase)
    {
        case OUTOFLINE_FIXED_LONGS:
        /**********************************************************************/
        /* test outofline_op1: send a fixed array of longs in fa1 and check   */
        /* that their squares come back in fa2.                               */
        /**********************************************************************/
        for (i=0; i<outofline_c1; i++)
            fa1[i] = i*3+10;
        outofline_op1 (h, fa1, fa2);
        for (i=0; i<outofline_c1; i++)
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

    }
    return(flag);
}
