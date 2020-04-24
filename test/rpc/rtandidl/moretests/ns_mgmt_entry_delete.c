/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ns_mgmt_entry_delete.c,v $
 * Revision 1.1.6.2  1996/02/17  23:21:21  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:30  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:07:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:59  root]
 * 
 * Revision 1.1.4.3  1993/01/25  18:06:04  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:40:33  bbelch]
 * 
 * Revision 1.1.4.2  1993/01/11  23:37:08  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:09:47  bbelch]
 * 
 * Revision 1.1.2.2  1992/07/09  15:25:35  smith
 * 	Changes TSHStatus to TSHMessage and returned
 * 	additional information.
 * 	[1992/07/09  01:31:31  smith]
 * 
 * Revision 1.1  1992/01/19  03:36:00  devrcs
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

/* Include files */

#include <tsh_common.h>
#include <tsh_ns.h>


/*  Args required by the test program.  */

#define ARGS_REQUIRED 4

ndr_long_int TestFunction (ctx, argc, argv)

/* Formal parameters */

   CtxBlock          *ctx;
   ndr_long_int      argc;
   unsigned_char_p_t argv[];
{

/* Local variables */

   ndr_long_int      subcase;		/* argv[0], subcase to run */
   error_status_t    estatus;		/* argv[1], expected return from test */
   error_status_t    tstatus;		/* status value from rpc calls... */
   unsigned32	     entry_name_syntax; /* argv[2] */
   unsigned_char_t   entry_name[DNS_FNAME_MAX];        /* from argv[3] */


/* Check the number of arguments passed in.  They should be exactly the   
   required number when you come into this routine. Looking for a subcase,
   a bogus estatus, and a hostname. */

   if (argc != ARGS_REQUIRED)
     {
       TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
					ARGS_REQUIRED, argc);
       return(TSH_BAD_ARG_COUNT);
     }

/* Obtain the input arguments from the .tsh command line. */

   subcase = atol(argv[0]);
   estatus = atol(argv[1]);
   entry_name_syntax = atol(argv[2]);
   strcpy(entry_name, "/.:/");
   strcat(entry_name, argv[3]);

/* Adjust the values passed in as necessary. */

   if (!strcmp((char *)entry_name, (char *)"<NULL>"))
     strcpy(entry_name, NULL);

/*** IMPORTANT:  test for rpc_s_no_ns_privilege is not yet implemented. ***/

/*** IMPORTANT:  The documentation for rpc_ns_mgmt_entry_delete does not specify an
		 error return for an attempt to delete a database entry that does
		 not exist.  This case should be tested, nevertheless. ***/

/* Setup the database for delete testing. */

   if (subcase != ENTRY_DOESNT_EXIST_DELETE)
     {
        rpc_ns_mgmt_entry_create (entry_name_syntax, entry_name, &tstatus);
   TSHMessage (ctx,MOINFO,"rpc_ns_mgmt_entry_create status: %d estatus %d\n", 
               tstatus, estatus);
      }

/* Perform the delete test. */

   rpc_ns_mgmt_entry_delete (entry_name_syntax, entry_name, &tstatus);
   TSHMessage (ctx,MOINFO,"rpc_ns_mgmt_entry_delete status: %d estatus %d\n", 
               tstatus, estatus);

/* Exit this function. */
   if(tstatus == estatus)
     return (TSH_SUCCESS);
   else
     return(TSH_TEST_FAILED);

}  /* End ns_mgmt_entry_delete */

