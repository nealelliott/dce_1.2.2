/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tattrc.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:34  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:22  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:11:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:35  root]
 * 
 * Revision 1.1.2.4  1993/01/25  19:28:57  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:45:32  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  23:41:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:15:09  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  17:49:14  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  21:27:16  cuti]
 * 
 * Revision 1.1  1992/01/19  03:37:30  devrcs
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



#include <tattr.h>
#include <tsh_common.h>
#include <stdio.h>
#include <math.h>
#include <dce/rpc.h> 

#include <tattrx.c>
#include <tattrh.c>

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
    return(type_attributes_test(h, ctx, subcase));
}

int type_attributes_test (h, ctx)
handle_t h;
CtxBlock *ctx;
{
    astring msg;
    tattrtest_t1 o;
    tattrtest_t2 t2a, t2b;
    ndr_boolean temp;

    
    /**********************************************************************/
    /* Put a fully bound handle (h) into the global (handle_to_use), for  */
    /* tattrtest_t3_bind. Normally this routine would perform a bind, but */
    /* the test isn't to see if we know how to write a binding routine. It*/
    /* is to see if the call is automatically made to the binding routine.*/
    /* The routine simply passes back the fully bound handle.             */
    /**********************************************************************/

    handle_to_use = h;

    /**********************************************************************/
    /* set up client side globals that should get modified during call on */
    /* tattrtest_op1 by user binding and user marshalling routines ...    */
    /**********************************************************************/
    o =
    h_into_bind =
    uh_into_unbind = 45;
    h_into_unbind = (handle_t)&h_into_unbind;

    value_into_to_xr = 46;
    xmit_value_from_to_xr = &t2a;
    xmit_value_into_free_xr = &t2b;

    xmit_value_into_from_xr = 46;
    value_into_free_xr = 46;
    from_xr_called = false;
    to_xr_called = false;
    free_called = false;
    free_xr_called = false;
   
    
    /******************************************************************/
    /* reset server side globals also ....                            */
    /******************************************************************/
    tattrtest_op2 (h); 
    /******************************************************************/
    /* make call using user binding and user marshalling              */
    /******************************************************************/
    tattrtest_op1 (1L, 3L, &o);
    /******************************************************************/
    /* test to see if the values are correct.                         */
    /******************************************************************/

    TSHMessage (ctx, MOTRACE, "o (%d) ? 4\n", o);
    if (o != 3 + 1) 
    {
        TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
        TSHMessage (ctx, MOWARNING, "o (%d) != 3\n", o);
        return(TSH_INCORRECT_DATA);
    }

    /******************************************************************/
    /* check that the auto bind and unbind routines got the right args*/
    /******************************************************************/
    TSHMessage (ctx, MOTRACE, " h_into_bind (%d) ? 1\n", h_into_bind);
    if (h_into_bind != 1) 
    {
        TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
        TSHMessage (ctx, MOWARNING, " h_into_bind (%d) != 1\n", h_into_bind);
        return(TSH_INCORRECT_DATA);
    }
    TSHMessage (ctx, MOTRACE," uh_into_unbind (%d) ? 1\n", uh_into_unbind);
    if (uh_into_unbind != 1) 
    {
        TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
        TSHMessage(ctx,MOWARNING," uh_into_unbind (%d) != 1\n", uh_into_unbind);
        return(TSH_INCORRECT_DATA);
    }
    TSHMessage (ctx, MOTRACE, " h_into_unbind (%8lx) ? h (%8lx)\n",
                                                  h_into_unbind, h);
    if (h_into_unbind != h) 
    {
        TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
        TSHMessage (ctx, MOWARNING, " h_into_unbind (%8lx) != h (%8lx)\n",
                                                  h_into_unbind, handle_to_use);
        return(TSH_INCORRECT_DATA);
    }

    /*******************************************************************/
    /* check that the user marshalling routines got the right args ... */
    /*******************************************************************/

    TSHMessage (ctx, MOTRACE, " value_into_to_xr (%d) ? 3\n", value_into_to_xr);
    if (value_into_to_xr != 3) 
    {
        TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
        TSHMessage (ctx, MOWARNING, " value_into_to_xr (%d) != 3\n",
                                                              value_into_to_xr);
        return(TSH_INCORRECT_DATA);
    }

    TSHMessage (ctx, MOTRACE, 
        "xmit_value_from_to_xr (%8lx) ? xmit_value_into_free_xr (%8lx)\n",
                                xmit_value_from_to_xr, xmit_value_into_free_xr);
    if (xmit_value_from_to_xr != xmit_value_into_free_xr) 
    {
        TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
        TSHMessage (ctx, MOWARNING, 
            "xmit_value_from_to_xr (%8lx) != xmit_value_into_free_xr (%8lx)\n",
                                xmit_value_from_to_xr, xmit_value_into_free_xr);
        return(TSH_INCORRECT_DATA);
    }

    TSHMessage (ctx, MOTRACE, 
        "value_into_to_xr (%8lx) ? value_into_free_xr (%8lx)\n",
                                          value_into_to_xr, value_into_free_xr);
    if (value_into_to_xr != value_into_free_xr) 
    {
        TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
        TSHMessage (ctx, MOWARNING,
            "value_into_to_xr (%8lx) != value_into_free_xr (%8lx)\n",
                                          value_into_to_xr, value_into_free_xr);
        return(TSH_INCORRECT_DATA);
    }

    TSHMessage (ctx, MOTRACE, "xmit_value_into_from_xr (%d) ? 4\n",
                                                       xmit_value_into_from_xr);
    if (xmit_value_into_from_xr != 3 + 1) 
    {
        TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
        TSHMessage (ctx, MOWARNING,"xmit_value_into_from_xr (%d) != 4\n",
                                                       xmit_value_into_from_xr);
        return(TSH_INCORRECT_DATA);
    }

    TSHMessage (ctx, MOTRACE,
        "(!(from_xr_called (%c) && to_xr_called (%c) && free_xr_called (%c))) || free_called (%c)\n",
          from_xr_called?'T':'F', to_xr_called?'T':'F', free_xr_called?'T':'F', free_called?'T':'F' );
    if ((!(from_xr_called && to_xr_called && free_xr_called)) || free_called) 
    {
        TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
        TSHMessage (ctx, MOWARNING, 
        "(!(from_xr_called (%c) && to_xr_called (%c) && free_xr_called (%c))) || free_called (%c)\n",
          from_xr_called?'T':'F', to_xr_called?'T':'F', free_xr_called?'T':'F', free_called?'T':'F' );
        return(TSH_INCORRECT_DATA);
    }

    /*******************************************************************/
    /* check server side globals                                       */
    /*******************************************************************/
    temp = tattrtest_op3(h, (astring *)msg); 
    TSHMessage (ctx, MOTRACE, "tattrtest (from server): %s\n", msg);
    if (temp)
    {
        TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
        TSHMessage (ctx, MOWARNING, "tattrtest (from server): %s\n", msg);
        return(TSH_INCORRECT_DATA);
    }

    return (SUCCESS);
}
