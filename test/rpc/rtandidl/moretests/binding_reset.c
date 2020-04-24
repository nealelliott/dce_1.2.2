/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: binding_reset.c,v $
 * Revision 1.1.4.2  1996/02/17  23:20:11  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:38:51  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:03:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:42:51  root]
 * 
 * Revision 1.1.2.3  1993/01/25  18:03:44  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:34:51  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  22:11:56  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:04:02  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:34:48  devrcs
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



#include    <tsh_string.h>
#include    <bindtest.h>

/* Define number of command line arguments required by this function. */

#define ARGS_REQUIRED        7

/* Function to pick string arguments off the command line. */

static unsigned_char_t	nil_string = '\0';

unsigned_char_p_t get_args(args)
unsigned_char_p_t 	args;
{
  if (!strcmp (NULL_TOKEN, (char *)args))
    return ((unsigned_char_p_t)0);
  else
    if (!strcmp(NIL_TOKEN, (char *)args))
      return(&nil_string);
    else
      return(args);
}

ndr_long_int TestFunction (ctx, argc, argv)

/* Formal parameters */

   CtxBlock		*ctx;
   ndr_long_int		argc;
   unsigned_char_p_t	argv[];

{

/* Local variables */

   ndr_long_int    subcase;		 /* agrv[0] */ 
   error_status_t  estatus;		 /* argv[1] */
   unsigned_char_p_t str_uuid_p;	 /* argv[2] */
   unsigned_char_p_t protseq_p;		 /* argv[3] */
   unsigned_char_p_t netaddr_p;		 /* argv[4] */
   unsigned_char_p_t endpoint_p;	 /* argv[5] */
   unsigned_char_p_t net_opts_p;	 /* argv[6] */
   unsigned_char_p_t str_binding_p;	 /* Output from rpc_string_binding_compose */
   unsigned_char_p_t res_str_uuid_p;	 /* Output from rpc_string_binding_parse   */
   unsigned_char_p_t res_protseq_p;	 /* Output from rpc_string_binding_parse   */
   unsigned_char_p_t res_netaddr_p;	 /* Output from rpc_string_binding_parse   */
   unsigned_char_p_t res_endpoint_p;	 /* Output from rpc_string_binding_parse   */
   unsigned_char_p_t res_net_opts_p;	 /* Output from rpc_string_binding_parse   */
   unsigned_char_p_t res_str_binding_p;	 /* Output from rpc_binding_to_string_binding*/
   rpc_binding_handle_t binding_h;	 /* Opaque handle used by app programmers  */
   rpc_binding_rep_p_t	binding_h_p;	 /* Ptr to real handle used by conoscenti  */
   error_status_t  tstatus;		 /* Status code from all rpc's		   */ 
   error_status_t  cstatus;		 /* Status code from cleanup		   */ 
   

/* Exit on wrong number of command line arguments. */

   if (argc != ARGS_REQUIRED)
     {
       TSHMessage (ctx, MOFAILURE, "Expecting %d arguments, got %d.\n", 
		   ARGS_REQUIRED, argc);
       return(FAILURE);
     }

/* Obtain command line arguments. */

   subcase = atol(argv[0]);
   estatus = atol(argv[1]);
   str_uuid_p = get_args(argv[2]);
   protseq_p = get_args(argv[3]);
   netaddr_p = get_args(argv[4]);
   endpoint_p = get_args(argv[5]);
   net_opts_p = get_args(argv[6]);

   res_str_uuid_p = NULL;
   res_protseq_p = NULL;
   res_netaddr_p = NULL;
   res_endpoint_p = NULL;
   res_net_opts_p = NULL;

/* Initialize rpc test status. */

   tstatus = rpc_s_ok;

/* Write informational messages about command line arguments. */

   TSHMessage (ctx, MOINFO, "Object UUID %s, Protocol Sequence = %s\n",
		str_uuid_p, protseq_p);

   TSHMessage (ctx, MOINFO, "Network Address %s, Endpoint %s, Options %s\n",
		netaddr_p, endpoint_p, net_opts_p);

/* Setup for the binding_reset. */

   rpc_string_binding_compose (str_uuid_p, protseq_p, netaddr_p, endpoint_p,
			       net_opts_p, &str_binding_p, &tstatus);
   if (tstatus != rpc_s_ok) 
     {
       TSHMessage (ctx, SETUP, "Failure on rpc_string_binding_compose\n");
       return (tstatus);
     }

   rpc_binding_from_string_binding (str_binding_p, &binding_h, &tstatus);

   if (tstatus != rpc_s_ok) 
     {
       TSHMessage (ctx, SETUP, "Failure on rpc_binding_from_string_binding\n");
       return (tstatus);
     }

/* Obtain a pointer to the real data.  This pointer is used to setup the
   pathological test cases. */

   binding_h_p = (rpc_binding_rep_p_t) binding_h;

/* Modify the data in the binding handle for test purposes. */

   switch (subcase)
     {
       case SRC_HANDLE_ZERO:
         binding_h = (rpc_binding_handle_t) 0;
         break;

       case SRC_HANDLE_MINUS_ONE:
         binding_h = (rpc_binding_handle_t) -1;
         break;

       case DATA_OFFSET_ERR:
/*         binding_h_p->data_offset = (unsigned16) -1; */
         binding_h_p->link.next = (pointer_t) -1;
          break;

       case PROTOCOL_ID_ERR:
         binding_h_p->protocol_id = (rpc_protocol_id_t) -1;
         break;

       case REFCNT_ERR:
         binding_h_p->refcnt = (signed8) -1;
         break;

       case OBJ_ERR:
         bzero ((char *)&(binding_h_p->obj), sizeof(uuid_t));
         break;

       case RPC_ADDR_ERR:
         binding_h_p->rpc_addr = (rpc_addr_p_t) 0;
         break;

       case RPC_ADDR_RPC_PROTSEQ_ID_ERR:
         binding_h_p->rpc_addr->rpc_protseq_id = (rpc_protseq_id_t) 0;
         break;

       case RPC_ADDR_LEN_ERR:
         binding_h_p->rpc_addr->len = (unsigned32) 0;
         break;

       case RPC_ADDR_SA_FAMILY_ERR:
         binding_h_p->rpc_addr->sa.family = (unsigned16) 0;
         break;

       case RPC_ADDR_SA_DATA_ERR:
         bzero ((char *)(binding_h_p->rpc_addr->sa.data), sizeof(unsigned8));
         break;

       case TIMEOUT_ERR:
         binding_h_p->timeout = (signed32) -1;
         break;

       case CALLS_IN_PROGRESS_ERR:
         binding_h_p->calls_in_progress = (signed8) -1;
         break;

       case NS_SPECIFIC_ERR:
         binding_h_p->ns_specific = (pointer_t) 0;
         break;

     } /* End switch (subcase) */

/* Call the rpc under test. */

   rpc_binding_reset (binding_h, &tstatus);

/* If rpc_binding_reset returned no errors, continue normal case test processing;
   otherwise, report the failure, cleanup, and exit. */

   if (tstatus == rpc_s_ok) 
     {

    /* Obtain a string representation of the binding after the reset to compare
       with the original string binding.  If the rpc fails, discontinue test
       processing, cleanup, and exit. */

       rpc_binding_to_string_binding (binding_h, &res_str_binding_p, &tstatus);

       if (tstatus == rpc_s_ok) 
	 {

	/* If the binding did not change after the reset, return an error; otherwise,
	   output an informational message (if required) showing the change. */

	   if (strcmp ((char *)str_binding_p, (char *)res_str_binding_p) != 0)
	     {
	       TSHMessage (ctx, MOINFO, "original binding was %s\n", str_binding_p);
	       TSHMessage (ctx, MOINFO, "   reset binding was %s\n",res_str_binding_p);
	     }

	   else
  	     {
	       TSHMessage (ctx, MOFAILURE, "Binding unchanged after reset attempt\n");
	       tstatus = FAILURE;
	       goto CLEANUP_EXIT;
	     }

	 } /* End if (tstatus == rpc_s_ok) after rpc_binding_to_string_binding. */

       else
	 {
           TSHMessage (ctx, SETUP, "Failure on rpc_binding_to_string_binding\n");
	   goto CLEANUP_EXIT;
	 }

    /* Obtain string representations of individual binding handle values after the
       reset to compare with original values.  If the rpc fails, discontinue test
       processing, cleanup, and exit. */

       rpc_string_binding_parse (res_str_binding_p, &res_str_uuid_p, &res_protseq_p,
				 &res_netaddr_p, &res_endpoint_p, &res_net_opts_p,
				 &tstatus);
       if (tstatus == rpc_s_ok) 
	 {
	   if (strcmp ("", (char *)res_endpoint_p) != 0)
	     {
	       TSHMessage (ctx,MOFAILURE,"endpoint after reset = %s\n",res_endpoint_p);
	       tstatus = FAILURE;
	     }
	   
	   if (strcmp ("", (char *)res_net_opts_p) != 0)
	     {
	       TSHMessage (ctx,MOFAILURE,"net opts after reset = %s\n",res_net_opts_p);
	       tstatus = FAILURE;
	     }

	 } /* End if (tstatus == rpc_s_ok) after rpc_string_binding_parse. */

       else
	 TSHMessage (ctx, SETUP, "Failure on rpc_string_binding_parse\n");

     } /* End if (tstatus == rpc_s_ok) after rpc_binding_reset */

   else
     TSHMessage (ctx, MOFAILURE, "Failure on rpc_binding_reset\n");

/* Cleanup after rpc_binding_reset test. */

CLEANUP_EXIT:

   rpc_binding_free (&binding_h, &cstatus);

   if (cstatus != rpc_s_ok)
     TSHMessage (ctx, CLNUP, "Failure on rpc_binding_free\n");

/* Exit this function. */

   return (tstatus);

} /* End binding_reset */

