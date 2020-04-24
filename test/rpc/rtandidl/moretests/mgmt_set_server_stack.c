/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mgmt_set_server_stack.c,v $
 * Revision 1.1.4.2  1996/02/17  23:20:55  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:56  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:06:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:40  root]
 * 
 * Revision 1.1.2.3  1993/01/25  18:05:27  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:37:39  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:35:45  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:08:01  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:19  devrcs
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



#include <tsh_mgmt.h>

#define ARGS_REQUIRED 3


ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock            *ctx;
ndr_long_int        argc;
unsigned_char_p_t   argv[];
{
  ndr_long_int           SubCase;		/* argv[0], subcase to run		*/
  error_status_t         EStatus;		/* argv[1], expected return from test	*/
  unsigned32             stack_size;            /* argv[2], stack size to set           */
  error_status_t         status;		/* generic status variable		*/
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
  stack_size = atol(argv[2]);


  switch(SubCase) {
    case SET_STACK_SIZE:
      TSHMessage(ctx, MOINFO, "Running subcase SET_STACK_SIZE\n");

      /**************************************************************************/
      /* the specific value of the stack size is dependent on the underlaying	*/
      /* thread package that is used. "reasonable" values could be 1-10k	*/
      /**************************************************************************/

      rpc_mgmt_set_server_stack_size(stack_size, &status); 
      TSHMessage(ctx, MOTRACE, "rpc_mgmt_set_server_stack_size status: %d\n", status);
      if (TSHStatus(ctx, TEST, "rpc_mgmt_set_server_stack_size() failed", status, EStatus)) {
        return(status);
      }

      break;

    default:
      TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", SubCase);
      return(TSH_UNKNOWN_SUBCASE);
  }

  return(TSH_SUCCESS);
}
