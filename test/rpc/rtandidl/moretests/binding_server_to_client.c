/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: binding_server_to_client.c,v $
 * Revision 1.1.5.2  1996/02/17  23:20:13  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:38:53  marty]
 *
 * Revision 1.1.5.1  1995/12/11  20:03:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:42:53  root]
 * 
 * Revision 1.1.2.6  1993/04/08  20:11:32  cuti
 * 	Add CN's return code (rpc_s_connection_closed) for rpc_is_server_listening status checking.
 * 	[1993/04/08  20:07:17  cuti]
 * 
 * Revision 1.1.2.5  1993/01/25  18:03:48  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:35:01  bbelch]
 * 
 * Revision 1.1.2.4  1993/01/11  22:12:01  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:04:09  bbelch]
 * 
 * Revision 1.1.2.3  1992/12/04  20:50:37  cuti
 * 	Change the expected return status of rpc_mgmt_is_server_listening to reflect rpc code change.
 * 	[1992/12/04  16:13:01  cuti]
 * 
 * Revision 1.1.2.2  1992/10/12  16:30:06  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/12  14:40:00  cuti]
 * 
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  17:57:28  cuti]
 * 
 * Revision 1.1  1992/01/19  03:38:17  devrcs
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



#include <math.h>
#include <dce/rpc.h> 
#include <tsh_common.h>
#include <tsh_binding.h>
#include <binding_server.h>

#define ARGS_REQUIRED 4

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock            *ctx;
ndr_long_int        argc;
unsigned_char_p_t   argv[];
{
  unsigned_char_p_t   bound_string;
  error_status_t      status, tstatus, estatus;
  handle_t            handle, test_binding;
  boolean32           listening;
  ndr_long_int        subcase;


  /**************************************************************************/
  /* Check the number of arguments passed in.  They should be exactly the   */
  /* required number when you come into this routine. Looking for a subcase,*/
  /* a bogus expected_status, and a hostname.                               */
  /**************************************************************************/

  if (argc != ARGS_REQUIRED) {
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
  rpc_binding_from_string_binding(bound_string, &handle, &status);
  if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", status, rpc_s_ok))
    return(status);
  /* rpc enquires fully_bound handle to check server listening */ 
  rpc_ep_resolve_binding(handle, binding_server_v1_0_c_ifspec, &status);
  TSHMessage(ctx, MOINFO, "rpc_ep_resolve_binding status: %d\n", status); 
  if (TSHStatus(ctx, SETUP, "rpc_ep_resolve_binding", status, rpc_s_ok))
    return(status);              



  subcase = atoi(argv[0]);
  estatus = atoi(argv[1]);

  listening = rpc_mgmt_is_server_listening(handle, &tstatus);
  TSHMessage(ctx, MOINFO, "rpc_mgmt_is_server_listening status: %d\n", tstatus); 
  if (TSHStatus(ctx, SETUP, "rpc_mgmt_is_server_listening failed", tstatus, rpc_s_ok))
    return(tstatus);
  if (TSHStatus(ctx, SETUP, "server has not been started", listening, true))
    return(FAILURE);

  switch (subcase) {
    case VALID_HANDLE:
      TSHMessage(ctx, MOINFO, "Running subcase VALID_HANDLE\n");
      binding_server_op1(handle, subcase, &status);
      break;

    case INVALID_HANDLE:
      TSHMessage(ctx, MOINFO, "Running subcase INVALID_HANDLE\n");
      binding_server_op1(handle, subcase, &status);
      break;

    case WRONG_KIND:
      TSHMessage(ctx, MOINFO, "Running subcase WRONG_KIND\n");
      rpc_binding_server_to_client(handle, &test_binding, &status);
      break;

    case TERMINATE:
      TSHMessage(ctx, MOINFO, "Running subcase TERMINATE\n");
      binding_server_op1(handle, subcase, &status);
      TSHMessage(ctx, MOTRACE, "rpc_stop_server_listening status: %d\n", status);
      listening = rpc_mgmt_is_server_listening(handle, &tstatus);
      TSHMessage(ctx, MOTRACE, "rpc_mgmt_is_server_listening status: %d\n", tstatus);
      if (tstatus != rpc_s_comm_failure && tstatus != rpc_s_connection_closed) {
	  TSHStatus(ctx, TEST, "rpc_mgmt_is_server_listening failed", 1, 0);
	  return(tstatus);
      }
      if (TSHStatus(ctx, TEST, "server has not stopped listening", listening, false))
        return(FAILURE);
      break;

    default:
      TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", subcase);
      return(TSH_UNKNOWN_SUBCASE);
  }

  TSHMessage(ctx, MOTRACE, "rpc_binding_server_to_client status: %d\n", status);
  if (TSHStatus(ctx, TEST, "rpc_binding_server_to_client() failed", status, estatus)) {
    return(status);
  }

  return(TSH_SUCCESS);   
} 
