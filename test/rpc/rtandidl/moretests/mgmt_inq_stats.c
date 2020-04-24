/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mgmt_inq_stats.c,v $
 * Revision 1.1.4.2  1996/02/17  23:20:50  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:46  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:06:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:36  root]
 * 
 * Revision 1.1.2.4  1993/01/25  18:05:19  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:37:24  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  23:35:26  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:07:44  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  16:46:23  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/12  14:43:19  cuti]
 * 
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  18:00:21  cuti]
 * 
 * Revision 1.1  1992/01/19  03:35:29  devrcs
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
#include <dce/stubbase.h>
#include <tsh_mgmt.h>
#include <my_mgmt.h>

#define ARGS_REQUIRED 4


ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock            *ctx;
ndr_long_int        argc;
unsigned_char_p_t   argv[];
{
  ndr_long_int           SubCase;		/* argv[0], subcase to run		*/
  error_status_t         EStatus;		/* argv[1], expected return from test	*/
  rpc_stats_vector_t     *statistics;		/* argv[2], option for call_handle	*/
  unsigned_char_p_t      bound_string;		/* bound handle				*/
  unsigned_char_p_t      endpoint;		/* the endpoint for a given protseq	*/
  error_status_t         status;		/* generic status variable		*/
  handle_t               binding;		/* ectra binding handle pointer		*/
  rpc_call_handle_t      call_h = NULL;		/* call_handle				*/
  rpc_transfer_syntax_t  transfer_syntax;	/* transfer_syntax for call_handle	*/
  rpc_if_handle_t        *if_spec;		/* if_spec for call_handle		*/
  int                    index;			/* generic index value			*/


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


  SubCase = atol(argv[0]);
  EStatus = atol(argv[1]);


  if (SubCase != NULL_BINDING) {
    /**************************************************************************/
    /*  Do the compose of the string, followed by the binding from string.    */
    /**************************************************************************/
    switch (SubCase) {
      case INQ_STATS:
        rpc_string_binding_compose(
            NULL, (unsigned_char_p_t)argv[3], argv[2], 
            NULL, NULL, &bound_string, &status);
        break;
/*
      case BINDING_MISSING_ENDPOINT:
        rpc_string_binding_compose(
            NULL, (unsigned_char_p_t)argv[3], argv[2], 
            NULL, NULL, &bound_string, &status);
        break;
*/
      case BINDING_MISSING_UUID:
        rpc_string_binding_compose(
            NULL,
            (unsigned_char_p_t)argv[3], argv[2], 
            NULL, NULL, &bound_string, &status);
        break;
      case BINDING_INCOMPLETE:
        rpc_string_binding_compose(
            NULL, 
            (unsigned_char_p_t)argv[3], argv[2], 
            NULL, NULL, &bound_string, &status);
        break;
      default:
        TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", SubCase);
        return(TSH_UNKNOWN_SUBCASE);
    }
    if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose() error", status, rpc_s_ok))
      return(status);
    rpc_binding_from_string_binding(bound_string, &binding, &status);
    if (TSHStatus(ctx, SETUP, "rpc_binding_from_string_binding() error", status, rpc_s_ok))
      return(status);
  }

  /* run the requested SubCase */

  switch(SubCase) {
    case INQ_STATS:
    case BINDING_MISSING_UUID:
      TSHMessage(ctx, MOINFO, "Running subcase INQ_STATS\n");
      /* rpc inquires fully_bound handle to check server if_ids */ 
     rpc_ep_resolve_binding(binding, mgmttest_v1_0_c_ifspec, &status);
     if (TSHStatus(ctx, SETUP, "rpc_ep_resolve_binding", status, rpc_s_ok))
         return(status); 

      rpc_mgmt_inq_stats(binding, &statistics, &status); 
      TSHMessage(ctx, MOTRACE, "rpc_mgmt_inq_stats status: %d\n", status);
      if (TSHStatus(ctx, TEST, "rpc_mgmt_inq_stats() failed", status, EStatus)) {
        return(status);
      }

      rpc_binding_free(&binding, &status);
      if (TSHStatus(ctx, CLNUP, "rpc_binding_free() error", status, rpc_s_ok)) {
        return(status);
      }

      rpc_mgmt_stats_vector_free(&statistics, &status);
      if (TSHStatus(ctx, CLNUP, "rpc_mgmt_stats_vector_free() error", status, rpc_s_ok)) {
        return(status);
      }

      break;

/*
    case BINDING_MISSING_ENDPOINT:

 * since the mgmt_server does not register in the endpoint map 
 * we cannot leave the enpoint open and expect the call to 
 * succeed. However, if the enpoint was registered the call 
 * would succeed and return rpc_s_ok as a status.
 * 
 */

    case NULL_BINDING:
      TSHMessage(ctx, MOINFO, "Running subcase NULL_BINDING_HANDLE\n");

      rpc_mgmt_inq_stats(NULL, &statistics, &status);
      TSHMessage(ctx, MOTRACE, "rpc_mgmt_inq_stats status: %d\n", status);
      if (TSHStatus(ctx, TEST, "rpc_mgmt_inq_stats() error", status, EStatus)) {
        return(status);
      }

      rpc_mgmt_stats_vector_free(&statistics, &status);
      if (TSHStatus(ctx, CLNUP, "rpc_mgmt_stats_vector_free() error", status, rpc_s_ok)) {
        return(status);
      }

      break;

    case BINDING_INCOMPLETE:
      TSHMessage(ctx, MOINFO, "Running subcase BINDING_INCOMPLETE\n");

      rpc_mgmt_inq_stats(binding, &statistics, &status);
      TSHMessage(ctx, MOTRACE, "rpc_mgmt_inq_stats status: %d\n", status);
      if (TSHStatus(ctx, TEST, "rpc_mgmt_inq_stats() error", status, EStatus)) {
        return(status);
      }

      rpc_binding_free(&binding, &status);
      if (TSHStatus(ctx, CLNUP, "rpc_binding_free() error", status, rpc_s_ok)) {
        return(status);
      }

      break;
/*
    case INVALID_BINDING:
 *
 * There is no easy way to create an invalid binding intentionally. 
 * One way would be to gain access to the internal data structures
 * and do some changes to a valid binding handle. Since we did not
 * want to do this we cannot test this case.
 *
 */

    default:
      TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", SubCase);
      return(TSH_UNKNOWN_SUBCASE);
  }

  return(TSH_SUCCESS);
}
