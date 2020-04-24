/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: repasc.c,v $
 * Revision 1.1.4.2  1996/02/17  23:21:52  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:58  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:09:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:42  root]
 * 
 * Revision 1.1.2.4  1993/01/25  19:27:28  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:43:39  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  23:39:21  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:12:37  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  17:25:04  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  20:26:40  cuti]
 * 
 * Revision 1.1  1992/01/19  03:36:44  devrcs
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



#include <repas.h>
#include <tsh_common.h>
#include <repasx.c>



#define	ARGS_REQUIRED	4

ndr_long_int 
TestFunction(ctx, argc, argv)
CtxBlock *ctx;
ndr_long_int argc;
unsigned_char_p_t argv[];
{
	ndr_long_int subcase;
	unsigned_char_p_t bound_string;
	error_status_t status;
	ndr_long_int success = 0;
	ndr_long_int fail = 1;
	handle_t h;


	/**************************************************************************/
	/* Check the number of arguments passed in.  They should be exactly the   */
	/* required number when you come into this routine. Looking for a subcase,*/
	/* a bogus expected_status, and a hostname.                               */
	/**************************************************************************/
	if (argc !=  ARGS_REQUIRED) {
		TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
                                                               ARGS_REQUIRED, argc);
		return(TSH_BAD_ARG_COUNT);
	}

	subcase = atol(argv[0]);		 /* turn subcase string into an int.	  */
	/* ignore argv[1] expected status value, */
	/* each test does its own error checking.*/

	/**************************************************************************/
	/*  Do the compose of the string, followed by the binding from string.	*/
	/**************************************************************************/
	rpc_string_binding_compose(
	    NULL, (unsigned_char_p_t)argv[3], 
		argv[2], NULL, NULL, &bound_string, &status);

	if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", status, rpc_s_ok))
		return(status);
	rpc_binding_from_string_binding(bound_string, &h, &status);
	if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", status, rpc_s_ok))
		return(status);
	return(represent_as_test(h, ctx, subcase));
}

int represent_as_test (h, ctx)
handle_t h;
CtxBlock *	ctx;
{
    repastest_t1 t1_i, t1_o;

    int indx;

    /******************************************************************/
    /* test repastest_op1: fill a single member struct with values    */
    /* from 2 to 1000 in increments of 2.  Before passing it over the */
    /* network convert it to a two member struct by dividing the value*/
    /* of the single member struct in half, placing each half in the  */
    /* two member struct.  At the server side, reconvert the two parts*/
    /* to a single number, and double the resultant.  Then convert it */
    /* back to a two member struct for retransmittal.  Once the data  */
    /* is back here, convert it again and see if the result is double */
    /* what the original value was.                                   */
    /******************************************************************/
    for (indx=2; indx<=1000; indx+=2) 
    {
	t1_i.f1 = indx;
	repastest_op1 (h, t1_i, &t1_o);
        if (!(from_local_rep && to_local_rep 
                 && free_net_rep)) 
        {
            TSHStatus(ctx, TEST,"incorrect data returned", FAILURE,SUCCESS);
            TSHMessage (ctx, MOWARNING, 
              "(!(from_local_rep (%c) && to_local_rep (%c) && free_net_rep (%c)))\n",
              from_local_rep?'T':'F', to_local_rep?'T':'F', 
                                  free_net_rep?'T':'F' );
            return(TSH_INCORRECT_DATA);
        }
	TSHMessage (ctx,MOTRACE,"t1_o.f1 (%d) ? indx (%ld) * 2\n",t1_o.f1,indx);
	if (t1_o.f1 != indx * 2) 
        {
	    TSHStatus(ctx, TEST, "incorrect data returned",FAILURE,SUCCESS);
	    TSHMessage(ctx,MOWARNING,
                    "t1_o.f1 (%d) != indx (%ld) * 2\n", t1_o.f1,indx);
	    return(TSH_INCORRECT_DATA);
	}
        from_local_rep = FALSE;
        to_local_rep = FALSE;
        free_net_rep = FALSE;
        free_local_rep = FALSE;
    }
    return(SUCCESS);
}
