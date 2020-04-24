/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: binding_vector_free.c,v $
 * Revision 1.1.4.2  1996/02/17  23:20:17  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:38:57  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:03:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:42:55  root]
 * 
 * Revision 1.1.2.4  1993/01/25  18:03:54  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:35:11  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  22:12:09  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:04:17  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  16:31:48  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/12  14:40:19  cuti]
 * 
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  17:57:44  cuti]
 * 
 * Revision 1.1  1992/01/19  03:34:51  devrcs
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



#include <tsh_ns.h>
#include <fake.h>
#include <tsh_ns_utility.h>

/*  Args required by the test program.  */

#define ARGS_REQUIRED	2    
    
/*
void op
#ifdef __STDC__ 
(
	handle_t h
) 
#else
(h)
handle_t h;
#endif
{

} 
*/

ndr_long_int TestFunction (ctx, argc, argv)

/* Formal parameters */

   CtxBlock          *ctx;
   ndr_long_int      argc;
   unsigned_char_p_t argv[];
{

/* Local variables */

   ndr_long_int      subcase;		/* argv[0], subcase to run                */
   error_status_t    estatus;		/* argv[1], expected return from test     */
   error_status_t    tstatus;		/* status value from rpc tests...         */
   rpc_binding_vector_p_t binding_vector_p;/* required by rpc_binding_vector_free */
   rpc_endpoint_vector_elt_t   endpoints;

/* Check the number of arguments passed in.  They should be exactly the   
   required number when you come into this routine. */

   if (argc != ARGS_REQUIRED)
     {
       TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
					ARGS_REQUIRED, argc);
       return(TSH_BAD_ARG_COUNT);
     }

/* Obtain the input arguments from the .tsh command line. */

   subcase = atol(argv[0]);
   estatus = atol(argv[1]);

   rpc_server_register_if ((rpc_if_handle_t)fake_v1_0_s_ifspec, (uuid_p_t)0,
                           (rpc_mgr_epv_t) 0,  /* use default, in ifspec */
                           &tstatus);
   if (TSHStatus (ctx, SETUP, "rpc_server_register_if failed", tstatus, rpc_s_ok))
     return (tstatus);

   rpc_server_use_all_protseqs(1, &tstatus);
   if (TSHStatus (ctx, SETUP, "rpc_server_use_all_protseqs failed", tstatus, rpc_s_ok))
     return (tstatus);


   rpc_server_inq_bindings (&binding_vector_p, &tstatus);

   if (TSHStatus (ctx,SETUP,"rpc_server_inq_bindings failed",tstatus,rpc_s_ok))
     return (tstatus);
      
   rpc_server_unregister_if ((rpc_if_handle_t)fake_v1_0_s_ifspec, (uuid_p_t)0, &tstatus);

   if (TSHStatus (ctx, SETUP, "rpc_server_unregister_if() failed", tstatus, rpc_s_ok))
     return (tstatus);

/* Invoke the rpc to be tested. */

   rpc_binding_vector_free (&binding_vector_p, &tstatus);

   if (TSHStatus(ctx, MOFAILURE, "rpc_binding_vector_free() failed", tstatus, estatus))
     return (tstatus);

/* Exit this function. */

   return (rpc_s_ok);

}  /* End ns_binding_vector_free */

