/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mgmt_set_authz_fn.c,v $
 * Revision 1.1.6.2  1996/02/17  23:20:53  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:50  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:06:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:38  root]
 * 
 * Revision 1.1.4.2  1993/06/24  20:43:17  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:41:51  hinman]
 * 
 * Revision 1.1.2.2  1993/01/11  23:35:35  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:07:53  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:35:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* 
*/ 
/* 
 * Copyright 1991 by Apollo Computer, Inc.
 * 
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of Apollo Computer, Inc not
 * be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * Apollo Computer, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 *
 *   
 *	Extracted	1/4/92 16:53:33
 *	from delta on	11/8/91 16:48:36
 *
 *  $LOG$
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/rpc/rtandidl/moretests/mgmt_set_authz_fn.c, , dce_osf, dce.75d";
#endif

/* Include files */

#include <tsh_auth.h>
#include <dce/rpc.h>


/*  Args required by the test program.  */

#define ARGS_REQUIRED	3

ndr_long_int TestFunction (ctx, argc, argv)

/* Formal parameters */

   CtxBlock          *ctx;
   ndr_long_int      argc;
   unsigned_char_p_t argv[];
{

/* Local variables */

   ndr_long_int      subcase;		/* argv[0], subcase to run */
   error_status_t    estatus;		/* argv[1], expected return from test */
   error_status_t    tstatus;		/* actual value returned from rpc */
   unsigned32	     authz_func_flag;	/* argv[2] */

#ifndef SNI_SVR4
   typedef boolean32 (*rpc_mgmt_authorization_fn_t)
	(
	  rpc_binding_handle_t	client_binding,
	  unsigned32		requested_mgmt_operation,
	  unsigned32		*status
	);
#endif

   rpc_mgmt_authorization_fn_t authz_fn_p;
   boolean32 authz_func();

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
   authz_func_flag = atol(argv[2]);

/* Adjust command line arguments as necessary. */

   if (authz_func_flag == USE_AUTHZ_FUNC)
     authz_fn_p = &authz_func;
   else
     authz_fn_p = NULL;

/* Call the rpc under test. */

   rpc_mgmt_set_authorization_fn (authz_fn_p, &tstatus);

   TSHMessage (ctx, MOTRACE,
	       "rpc_mgmt_set_authorization_fn status: %d\n", tstatus);

   if ((TSHStatus (ctx, MOFAILURE, "rpc_mgmt_set_authorization_fn failed\n",
	    	   estatus, tstatus)))
     return (tstatus);

/* Exit this function. */

   return (rpc_s_ok);

}  /* End mgmt_set_authz_func */

boolean32 authz_func (client_binding, requested_mgmt_operation, status)

   rpc_binding_handle_t		client_binding;
   unsigned32			requested_mgmt_operation;
   unsigned32			*status;
{

/* Return the authorization status */

   switch (requested_mgmt_operation)
     {

       case rpc_c_mgmt_inq_if_ids:
         return ((boolean32)0);     

       case rpc_c_mgmt_inq_stats:
         return ((boolean32)0);     
  
       case rpc_c_mgmt_is_server_listen:
         return ((boolean32)0);     
  
       case rpc_c_mgmt_stop_server_listen:
         return ((boolean32)1);     

       default:
         return ((boolean32)0);
     }

} /* End authz_func */

