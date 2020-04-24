/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: strzc.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:30  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:19  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:11:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:29  root]
 * 
 * Revision 1.1.2.4  1993/01/25  19:28:47  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:45:17  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  23:41:31  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:14:53  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  17:45:07  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  21:26:06  cuti]
 * 
 * Revision 1.1  1992/01/19  03:37:24  devrcs
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



#include <strz.h>
#include <tsh_common.h>
#include <stdio.h>
#include <math.h>
#include <dce/rpc.h> 


static char ctab_in[] = {'N', 'i', 'd', 'l', 't', 'e', 's', 't', '1', '2', '3'};
static char ctab_out[] = {'n', 'I', 'D', 'L', 'T', 'E', 'S', 'T', '1', '2', '3'};

#define ARGS_REQUIRED 4

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock *ctx;
ndr_long_int argc;
unsigned_char_p_t argv[];
{
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
    return(string0s_test(h, ctx));
}

int string0s_test (h, ctx)
handle_t h;
CtxBlock *ctx;
{
    int failures = 0;
    int i, j;
    ndr_char expected_ch;

    string0test_t1 str0_in, str0_out;

    /**********************************************************************/
    /* pass in a NULL terminated string str0_in, and see that the case    */
    /* becomes inverted for alphas and is untouched for non-alphas in     */
    /* str0_out.
    /**********************************************************************/
    for (i = 0; i < 100; i += 3) 
    {
        for (j = 0; j < i; j++) 
        {
            str0_in[j] = ctab_in[(i + j) % 11];
            str0_out[j] = 'A';
        }
        str0_in[j] = '\000';
        string0test_op1 (h, str0_in, str0_out);
        /******************************************************************/
        /* test to see if the values are correct.                         */
        /******************************************************************/
        for (j = 0; str0_out[j]; j++) 
        {
            expected_ch = ctab_out[(i + j) % 11];
            TSHMessage (ctx, MOTRACE,"str0_out[%d] %c ? %c\n", j, str0_out[j],
                                                                  expected_ch);
            if (str0_out[j] != expected_ch) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx,MOWARNING,"str0_out[%d] %c != %c\n", j,
                                                     str0_out[j], expected_ch);
                return(TSH_INCORRECT_DATA);
            }
        }
    }

    return(SUCCESS);
}
