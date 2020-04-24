/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ns_binding_select.c,v $
 * Revision 1.1.4.2  1996/02/17  23:21:12  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:17  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:06:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:53  root]
 * 
 * Revision 1.1.2.4  1993/01/25  18:05:55  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:38:23  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  23:36:45  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:09:13  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  17:00:11  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  20:20:14  cuti]
 * 
 * Revision 1.1  1992/01/19  03:35:51  devrcs
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



#include <tsh_common.h>
#include <tsh_ns.h>
#include <tsh_call.h>
#include <tsh_ns_utility.h>


/*  Args required by the test program.  */

#define ARGS_REQUIRED	2

ndr_long_int TestFunction (ctx, argc, argv)

/* Formal parameters */

   CtxBlock          *ctx;
   ndr_long_int      argc;
   unsigned_char_p_t argv[];
{

/* Local variables */

   ndr_long_int      subcase;		/* argv[0], subcase to run */
   error_status_t    estatus;		/* argv[1], expected return from test */
   error_status_t    tstatus;		/* status value from rpc tests... */
   error_status_t    cleanup_status;	/* status value from rpc tests... */
   error_status_t    all_clean_status;	/* status value from rpc tests... */
   rpc_binding_vector_p_t binding_vector_p;/* required by rpc_ns_binding_select */
   rpc_binding_handle_t binding_handle; /* required by rpc_ns_binding_select */
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
   endpoints.rpc_protseq = (unsigned_char_p_t)"ncadg_ip_udp";
   endpoints.endpoint = (unsigned_char_p_t)"7002";
   NIDL_ifspec.endpoint_vector.count = 1;
   NIDL_ifspec.endpoint_vector.endpoint_vector_elt = &endpoints;

/* Setup for rpc_ns_binding_select test. */ 

   rpc_server_use_protseq_if((unsigned char *)"ncadg_ip_udp",
			     1, (rpc_if_handle_t)&NIDL_ifspec, &tstatus);

#ifdef NOTDEF
    rpc_server_use_all_protseqs_if (1, (rpc_if_handle_t)&NIDL_ifspec, &tstatus);
#endif
 
   if (TSHStatus (ctx, SETUP, "rpc_server_use_protseq_if failed", tstatus, rpc_s_ok))
     return (tstatus);

       rpc_server_inq_bindings (&binding_vector_p, &tstatus);

       if (TSHStatus (ctx,SETUP,"rpc_server_inq_bindings failed",tstatus,rpc_s_ok))
         return (tstatus);
      
/* Now invoke the rpc to be tested. */ 
   cleanup_status = rpc_s_ok;

   do
     {
       rpc_ns_binding_select (binding_vector_p, &binding_handle, &tstatus);

       if (tstatus != rpc_s_ok)
          break;

       rpc_binding_free (&binding_handle, &tstatus);

       if (TSHStatus (ctx, CLNUP, "rpc_binding_free() failed",
		      cleanup_status, rpc_s_ok))
           break;

     } while (subcase == NS_BINDING_SELECT_ALL);

       TSHStatus (ctx, MOFAILURE, "rpc_ns_binding_select() failed",
		      tstatus, estatus);

/* Cleanup from the rpc test. */

    all_clean_status = cleanup_status;
    rpc_binding_vector_free (&binding_vector_p, &cleanup_status);

    TSHStatus (ctx, CLNUP, "rpc_binding_vector_free() failed",
		      cleanup_status, rpc_s_ok);
    if (all_clean_status == rpc_s_ok)
       all_clean_status = cleanup_status;

    if (tstatus != estatus)
       return(FAILURE);
    if (all_clean_status != rpc_s_ok)
       return(all_clean_status);

/* Exit this function. */

   return (rpc_s_ok);

}  /* End ns_binding_select */

