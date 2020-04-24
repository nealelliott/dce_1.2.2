/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: site_mgmt.c,v $
 * Revision 1.1.4.2  1996/02/17  23:25:52  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:55  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:50:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:05  root]
 * 
 * Revision 1.1.2.3  1993/03/18  16:44:54  cuti
 * 	Remove checking "site_is_readonly" after site_open_query() since it open any site.
 * 	[1993/03/18  16:42:51  cuti]
 * 
 * Revision 1.1.2.2  1992/12/31  17:33:24  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:30  htf]
 * 
 * Revision 1.1  1992/01/19  04:08:35  devrcs
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
 *	Extracted	1/4/92 17:06:04
 *	from delta on	12/4/91 15:26:47
 *
 *  $LOG$
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/site_mgmt.c, , dce_osf, dce.75d";
#endif

/* Include files */

#include "tsh_site.h"


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
   sec_rgy_pname_t	site_name;	/* sec_rgy_site_open */
   sec_rgy_pname_t	*get_site_name;	/* sec_rgy_site_open */
   sec_rgy_handle_t	context;	/* sec_rgy_site_open */
   boolean32		is_readonly;	/* sec_rgy_site_is_readonly */   
   unsigned32		msg_type;	/* TSHStatus, TSHMessage */

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

/*   if (subcase == SITE_OPEN_DEFAULT || subcase == SITE_OPEN_UPDATE_DEFAULT)
*/
     bzero ((char *)site_name, sizeof (sec_rgy_pname_t));
/*
   else
     {
       gethostname (node_name, sec_rgy_pname_t_size);
       sprintf ((char *)site_name, "ip:%s", node_name);
     }
*/

   switch (subcase)
     {
       case SITE_CLOSE:
       case SITE_GET:
       case SITE_OPEN:
       case SITE_OPEN_DEFAULT:
	   sec_rgy_site_open (site_name,
                              &context,
                              &tstatus);

           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_open status: %d\n", tstatus);

        /* If this is the "server unavailable" case for the site_open or
	   site_open w/default site_name test, check for the test result
	   and exit on success. */

	   if ((tstatus == sec_rgy_server_unavailable) &&
	       (estatus == sec_rgy_server_unavailable) &&
	       ((subcase == SITE_OPEN) ||
		(subcase == SITE_OPEN_DEFAULT)))
             {
	       tstatus = sec_rgy_status_ok;
	       return (tstatus);
             }
	     
	   switch (subcase)
	     {
	       case SITE_CLOSE:
	       case SITE_GET:
	       case SITE_IS_READONLY:
		 msg_type = MOTRACE;
		 break;

	       case SITE_OPEN:
	       case SITE_OPEN_DEFAULT:
		 msg_type = MOFAILURE;
		 break;
	     } 
	     
           if (TSHStatus (ctx, msg_type,
                  "sec_rgy_site_open failed\n", rpc_s_ok, tstatus))
             return (tstatus);

           break;

       case SITE_OPEN_QUERY:
       case SITE_IS_READONLY:
	   sec_rgy_site_open_query (site_name,
                                    &context,
                                    &tstatus);

           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_open_query status: %d\n", tstatus);

           if (TSHStatus (ctx, MOFAILURE,
                  "sec_rgy_site_open_query failed\n", rpc_s_ok, tstatus))
             return (tstatus);

           break;

       case SITE_OPEN_UPDATE:
       case SITE_OPEN_UPDATE_DEFAULT:
	   sec_rgy_site_open_update (site_name,
                                     &context,
                                     &tstatus);
           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_open_update status: %d\n", tstatus);

           if (TSHStatus (ctx, MOFAILURE,
                  "sec_rgy_site_open_update failed\n", rpc_s_ok, tstatus))
             return (tstatus);

           break;

     } /* End switch (subcase) */


/* sec_rgy_site_get is called for its own test or to verify the open. */

   sec_rgy_site_get (context,
                     &get_site_name,
                     &tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_site_get (site_name: %s) status: %d\n", get_site_name, tstatus);

   if (estatus == tstatus)
     {
       if (tstatus == sec_rgy_status_ok)
	 {
           if ((subcase == SITE_OPEN) ||
               (subcase == SITE_GET))
             {
	       if (strlen(get_site_name) == 0)
	         {
                   TSHMessage (ctx, MOFAILURE, "site_name not returned\n");
                   tstatus = TSH_TEST_FAILED;
                 }
                else
                   TSHMessage (ctx, MOTRACE,"site_name is: %s\n",get_site_name);
             }
          }
        else
          tstatus = sec_rgy_status_ok;

     } /* End if (estatus == tstatus) */

   else
     {
       if ((tstatus == sec_rgy_not_implemented) &&
           (subcase != SITE_GET))
         tstatus = sec_rgy_status_ok;
       else
         TSHMessage (ctx, MOFAILURE,
                     "sec_rgy_site_get failed\n");
     }

/* sec_rgy_site_is_readonly is called for its test or as verification
   for sec_rgy_open_update, site_open-query will open any secd (master or slave)
   so no need to check is_readonly on site_open query(readonly) */

      if ((subcase == SITE_OPEN_UPDATE)||
       (subcase == SITE_OPEN_UPDATE_DEFAULT) )
     {
       is_readonly = sec_rgy_site_is_readonly (context);
             if (is_readonly != 0)
               tstatus = TSH_TEST_FAILED;

       if (tstatus == TSH_TEST_FAILED)
	 {
           TSHMessage (ctx, MOTRACE,
                       "sec_rgy_site_is_readonly: wrong value returned\n");

         }
       else
         TSHMessage (ctx, MOTRACE,
                     "sec_rgy_site_is_readonly: correct value returned\n");

     } /* End SITE_IS_READONLY path */

/* Close the registry site. */

   sec_rgy_site_close (context, &cstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_site_close status: %d\n", cstatus);
    
   if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_site_close failed\n", rpc_s_ok, cstatus))
     tstatus = cstatus;

/* Exit this routine. */

   return (tstatus);

} /* End site_mgmt */

