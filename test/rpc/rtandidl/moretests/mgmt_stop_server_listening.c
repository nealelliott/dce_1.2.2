/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: mgmt_stop_server_listening.c,v $
 * Revision 1.1.6.2  1996/02/17  23:20:57  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:58  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:06:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:41  root]
 * 
 * Revision 1.1.4.1  1994/01/14  16:13:55  tom
 * 	Bug 9741 - Don't check second rpc_mgmt_is_server_listening status code.
 * 	[1994/01/14  15:41:01  tom]
 * 
 * Revision 1.1.2.5  1993/01/25  18:05:29  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:37:44  bbelch]
 * 
 * Revision 1.1.2.4  1993/01/11  23:35:47  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:08:05  bbelch]
 * 
 * Revision 1.1.2.3  1992/12/04  20:50:51  cuti
 * 	Change the expected return status of rpc_mgmt_is_server_listening to reflect rpc code change.
 * 	[1992/12/04  16:13:19  cuti]
 * 
 * Revision 1.1.2.2  1992/10/12  16:50:29  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/12  14:44:01  cuti]
 * 
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  18:00:57  cuti]
 * 
 * Revision 1.1  1992/01/19  03:35:34  devrcs
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
# Revision 1.1  91/09/11  16:11:02  marco
# Initial revision
# 
*/



#include <math.h>
#include <tsh_common.h>
#include <tsh_mgmt.h>
#include <tsh_util.h>
#include <dce/rpc.h> 
#include <my_mgmt.h>

#define ARGS_REQUIRED 4

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock *ctx;
ndr_long_int argc;
unsigned_char_p_t argv[];
{
    unsigned_char_p_t bound_string;
    error_status_t status;
    handle_t h;
    ndr_long_int subcase;
    error_status_t estatus;



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
    if (TSHStatus(ctx, SETUP, "rpc_binding_from_string_binding", status, rpc_s_ok))
       return(status);
    /* rpc enquires fully_bound handle to check server */ 
    rpc_ep_resolve_binding(h, mgmttest_v1_0_c_ifspec, &status);
    if (TSHStatus(ctx, SETUP, "rpc_ep_resolve_binding", status, rpc_s_ok))
      return(status);
    subcase = atoi(argv[0]);
    estatus = atoi(argv[1]);
    return(my_test(h, subcase, estatus, ctx));
}

int my_test (h, subcase, estatus, ctx)
handle_t h;
ndr_long_int subcase;
error_status_t estatus;
CtxBlock *ctx;
{
    error_status_t tstatus;
    boolean32 listening;

    listening = rpc_mgmt_is_server_listening(h, &tstatus);
	TSHMessage(ctx, MOINFO, "rpc_mgmt_is_server_listening status: %d\n", tstatus); 
    if (TSHStatus(ctx, SETUP, "rpc_mgmt_is_server_listening failed", tstatus, rpc_s_ok))
       return(tstatus);
    if (TSHStatus(ctx, SETUP, "server has not been started", listening, true))
       return(FAILURE);
    hoohah(h, &tstatus);     
    TSHMessage(ctx, MOINFO, "rpc_mgmt_stop_server_listening status: %d\n", tstatus); 
	if (TSHStatus(ctx, TEST, "call to stop server listening failed", tstatus, estatus))
       return(tstatus);
    listening = rpc_mgmt_is_server_listening(h, &tstatus);

    /*
     * The CN RPC code will return rpc_s_connection_closed  generally
     * so comparing this against rpc_s_comm_failure isn't correct.
     * if (TSHStatus(ctx, TEST, "rpc_mgmt_is_server_listening failed", listening, rpc_s_comm_failure))
     *  return(tstatus);
     */

    if (TSHStatus(ctx, TEST, "server has not stopped listening", listening, false))
       return(FAILURE);
    return(rpc_s_ok);   
} /* End mgmt_stop_server_listening.c */
