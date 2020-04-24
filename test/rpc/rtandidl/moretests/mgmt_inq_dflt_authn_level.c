/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mgmt_inq_dflt_authn_level.c,v $
 * Revision 1.1.6.2  1996/02/17  23:20:47  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:41  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:05:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:34  root]
 * 
 * Revision 1.1.4.2  1993/01/11  23:35:21  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:07:34  bbelch]
 * 
 * Revision 1.1.2.2  1992/07/15  02:20:19  smith
 * 	took out dead commented out code.
 * 
 * 	changed TSHMessage() to TSHStatus().
 * 	[1992/07/15  02:19:33  smith]
 * 
 * Revision 1.1  1992/01/19  03:35:27  devrcs
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
 *	Extracted	1/4/92 16:53:32
 *	from delta on	11/8/91 16:48:18
 *
 *  $LOG$
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/rpc/rtandidl/moretests/mgmt_inq_dflt_authn_level.c, , dce_osf, dce.75d";
#endif

/* Include files */

#include <tsh_auth.h>


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
   unsigned32	     authn_level;	/* rpc_mgmt_inq_dflt_authn_level */
   unsigned32	     authn_svc;		/* rpc_mgmt_inq_dflt_authn_level */


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
   authn_svc = atol(argv[2]);

/* Adjust input values as necessary. */

   switch (authn_svc)
     {
       case AUTHN_SVC_NONE:
	 authn_svc = rpc_c_authn_none;
	 break;

       case AUTHN_SVC_DCE_PRIVATE:
	 authn_svc = rpc_c_authn_dce_private;
	 break;

       case AUTHN_SVC_DCE_PUBLIC:
	 authn_svc = rpc_c_authn_dce_public;
	 break;

       case AUTHN_SVC_DEFAULT:
	 authn_svc = rpc_c_authn_default;
	 break;

     }

/* Call the rpc under test. */

   rpc_mgmt_inq_dflt_authn_level (authn_svc, &authn_level, &tstatus);

   TSHMessage (ctx, MOTRACE,
	       "rpc_mgmt_inq_dflt_authn_level status: %d\n", tstatus);

   if (tstatus == estatus)
     tstatus = rpc_s_ok;
   else {
     TSHStatus (ctx, TEST, "rpc_mgmt_inq_dflt_authn_level failed\n",
		estatus, tstatus);
   }

/* Exit this function. */

   return (tstatus);

}  /* End mgmt_inq_dflt_authn_level */

