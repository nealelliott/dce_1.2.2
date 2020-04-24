/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: props.c,v $
 * Revision 1.1.9.3  1996/02/17  23:25:41  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:48  marty]
 *
 * Revision 1.1.9.2  1995/12/11  21:49:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:42  root]
 * 
 * Revision 1.1.5.1  1994/10/25  17:18:04  sekhar
 * 	[ OT 12398 ]  Checking in Sean Mullan's changes for testing
 * 		      security replication.
 * 	[1994/10/24  17:01:37  sekhar]
 * 
 * Revision 1.1.2.3  1993/02/25  22:07:50  cuti
 * 	Change sec_rgy_site_open() to sec_rgy_site_open_update() for using site update operation.
 * 	[1993/02/25  16:32:49  cuti]
 * 
 * Revision 1.1.2.2  1992/12/31  17:31:31  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:56:51  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:41  devrcs
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
 *	Extracted	1/4/92 16:56:19
 *	from delta on	11/13/91 14:18:45
 *
 *  $LOG$
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/props.c, , dce_osf, dce.75d";
#endif

/* Include files */

#include "tsh_props.h"
#include <sec_rgy_bind.h>


/* Args required by the test program. */

#define ARGS_REQUIRED 2

ndr_long_int TestFunction (ctx, argc, argv)

/* Formal parameters */

   CtxBlock          *ctx;
   ndr_long_int      argc;
   unsigned_char_p_t argv[];

{

/* Local variables */

   ndr_long_int		subcase;	/* argv[0], subcase to run */
   error_status_t	estatus;	/* argv[1], expected return from test */
   error_status_t	tstatus;	/* test status */
   error_status_t	cstatus;	/* cleanup status */
   sec_rgy_pname_t	node_name;	/* gethostname */
   sec_rgy_handle_t	write_context;	/* sec_rgy_site_open_update */
   sec_rgy_handle_t	read_context;	/* sec_rgy_site_open_query */
   sec_rgy_properties_t	set_props;      /* sec_rgy_properties_set_info */
   sec_rgy_properties_t	get_props;      /* sec_rgy_properties_get_info */
   sec_rgy_properties_t	props_saved;      /* sec_rgy_properties_get_info */
   
   
/* Check the number of arguments passed in.  They should be exactly the required
   number when you come into this routine. */
   
   if (argc != ARGS_REQUIRED)
     {
       TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n",
                  ARGS_REQUIRED, argc);
       return (TSH_BAD_ARG_COUNT);
     }

/* Obtain the input arguments from the .tsh command line. */

   subcase = atol(argv[0]);
   estatus = atol(argv[1]);

/* Bind to a registry server. */

   SEC_RGY_SITE_OPEN_WRITE(&write_context);
   SEC_RGY_SITE_OPEN_READ(&read_context);
                
/* get the properties to save for retoration */

   sec_rgy_properties_get_info (write_context,
                                &props_saved,
                                &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_properties_get_info status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
                  "sec_rgy_properties_get_info failed\n", rpc_s_ok, tstatus))
     return (tstatus);  

/* sec_rgy_properties_set_info is called either for its own test or as
   setup for sec_rgy_properties_get_info. */
  
	set_props.read_version = props_saved.read_version;
    set_props.write_version = props_saved.write_version;
    set_props.minimum_ticket_lifetime = props_saved.minimum_ticket_lifetime;
    set_props.default_certificate_lifetime = props_saved.default_certificate_lifetime;
    set_props.low_unix_id_person = 0;
    set_props.low_unix_id_group = 0;
    set_props.low_unix_id_org = 0;
    set_props.max_unix_id = 1000;
    set_props.flags = props_saved.flags;
    strcpy(set_props.realm, props_saved.realm);
    set_props.realm_uuid = props_saved.realm_uuid;
    set_props.unauthenticated_quota = props_saved.unauthenticated_quota;
             

   sec_rgy_properties_set_info (write_context,
                                &set_props,
                                &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_properties_set_info status: %d\n", tstatus);

   if (estatus == tstatus)
     tstatus = sec_rgy_status_ok;
   else
     TSHMessage (ctx, MOFAILURE,
                 "sec_rgy_properties_set_info failed\n");

   /* wait for propagation to any replicas */
   SEC_RGY_SITE_SYNCH(write_context);

/* sec_rgy_properties_get_info is called either for its own test or as
   verification for the sec_rgy_properties_set_info test. */

   sec_rgy_properties_get_info (read_context,
                                &get_props,
                                &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_properties_get_info status: %d\n", tstatus);

   if (estatus == tstatus)
     {

    /* Verify that the input to set_info came back as output from 
       get_info. */

       if (tstatus == sec_rgy_status_ok)
         if (memcmp ((char *)&get_props,
                     (char *)&set_props,
                     16))
           {
             TSHMessage (ctx, MOFAILURE,
                        "Output from get_info did not match input to set_info\n");
             tstatus = TSH_TEST_FAILED;
           }
         else
           TSHMessage (ctx, MOTRACE,
                       "Output from get_info matched input to set_info\n");
       else
         tstatus = sec_rgy_status_ok;

      } /* End if (estatus == tstatus) */

   else
     TSHMessage (ctx, MOFAILURE,
                 "sec_rgy_properties_get_info failed\n");
                             
/* restore the properties */


   sec_rgy_properties_set_info (write_context,
                                &props_saved,
                                &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_properties_set_info status: %d\n", tstatus);

   if (TSHStatus (ctx, SETUP,
                  "sec_rgy_properties_set_info failed\n", rpc_s_ok, tstatus))
     return (tstatus);  

/* Close the registry site. */

   sec_rgy_site_close (write_context, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_site_close status: %d\n", cstatus);
    
   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

   sec_rgy_site_close (read_context, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_site_close status: %d\n", cstatus);
   
   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

/* Exit this routine. */

   return (tstatus);

} /* End props */

