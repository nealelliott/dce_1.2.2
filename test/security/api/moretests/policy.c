/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: policy.c,v $
 * Revision 1.1.11.3  1996/02/17  23:25:40  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:47  marty]
 *
 * Revision 1.1.11.2  1995/12/11  21:49:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:41  root]
 * 
 * Revision 1.1.7.1  1994/10/25  17:18:02  sekhar
 * 	[ OT 12398 ]  Checking in Sean Mullan's changes for testing
 * 		      security replication.
 * 	[1994/10/24  17:01:24  sekhar]
 * 
 * Revision 1.1.4.3  1993/02/25  22:07:34  cuti
 * 	Change sec_rgy_site_open() to sec_rgy_site_open_update() for using site update operation.
 * 	[1993/02/25  16:32:26  cuti]
 * 
 * Revision 1.1.4.2  1992/12/31  17:31:26  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:56:46  htf]
 * 
 * Revision 1.1.2.2  1992/05/12  15:53:10  ahop
 * 	 CR 3104: reset RGY policy to original values at end of test
 * 	[1992/05/12  02:38:50  ahop]
 * 
 * Revision 1.1  1992/01/19  04:07:40  devrcs
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
 *	Extracted	1/4/92 16:56:18
 *	from delta on	11/13/91 14:18:40
 *
 *  $LOG$
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/policy.c, , dce_osf, dce.75d";
#endif

/* Include files */

#include "tsh_policy.h"
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
   sec_rgy_pgo_item_t	pgo_item;	/* sec_rgy_pgo_add */
   sec_rgy_name_t	test_org_name; /* sec_rgy_pgo_add */
   sec_rgy_plcy_t	*set_policy_p;  /* sec_rgy_plcy_set_info */
   sec_rgy_plcy_t	get_policy;     /* sec_rgy_plcy_get_info */
   sec_rgy_plcy_t	get_eff_policy; /* sec_rgy_plcy_get_effect */
   sec_rgy_plcy_t	save_rgy_policy; 


/* Check the number of arguments passed in.  They should be exactly the
   required number when you come into this routine. */

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

   /* save the original RGY policy to restore later */
   sec_rgy_plcy_get_info (write_context,
                          POLICY_RGY_NAME,
                          &save_rgy_policy,
                          &tstatus);
   TSHMessage (ctx, MOTRACE,
                   "Saved original rgy policy status: %d\n", tstatus);




/* Add a test org item the registry if (1) this is NOT a "not found" test
   case; or, (2) this is NOT a test of setting or getting the policy for
   the entire registry. */

   if (((subcase % 100) != NOT_FOUND_CASES) &&
       ((subcase % 100) != RGY_CASES))
     {

    /* Setup the parameters to add an org item. */

       bzero ((char *)&pgo_item, sizeof (pgo_item));

       uuid_create_nil (&pgo_item.id, &tstatus);

       pgo_item.unix_num = 5000; 
       pgo_item.quota = 1;
       pgo_item.flags = sec_rgy_pgo_flags_none;

       strcpy ((char *)pgo_item.fullname,
	       (char *)"marco's test org item\0");

    /* Add an org item. */

       sec_rgy_pgo_add (write_context,
                        (sec_rgy_domain_t)sec_rgy_domain_org,
		        POLICY_ORG_NAME,
		        &pgo_item,
                        &tstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_add (org) status: %d\n", tstatus);

    /* Cleanup and exit if adding the pgo item failed. */

       if (TSHStatus (ctx, SETUP,
                      "sec_rgy_pgo_add (person) failed\n", rpc_s_ok, tstatus))
         {
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

           return (tstatus);
         }

     } /* End test of whether or not to setup an org item. */

/* Setup the org name for set_info.  Note that if the org name is empty,
   the policy for the registry is set.  Note also that if this is a 
   "not found" case, no org was added during setup. */ 

   if ((subcase % 100) == RGY_CASES)
     {
       bzero ((char *) test_org_name, sizeof (sec_rgy_name_t));
       set_policy_p = &POLICY_RGY; 
     }

   else
     {
       strcpy (test_org_name, POLICY_ORG_NAME);
       set_policy_p = &POLICY_ORG; 
     }

/* sec_rgy_plcy_set_info is called either for its own test or as
   setup for sec_rgy_plcy_get_{effective,info}.  Note that for the
   "not found" cases of get_{info,effective}, it is not necessary to
   set info. */

   if ((subcase != GET_POLICY_NOT_FOUND) &&
       (subcase != GET_EFF_POLICY_NOT_FOUND))
     {
       sec_rgy_plcy_set_info (write_context,
                              test_org_name,
                              set_policy_p,
                              &tstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_plcy_set_info status: %d\n", tstatus);

       if (estatus == tstatus)
         tstatus = sec_rgy_status_ok;
       else
         {
           if (tstatus == sec_rgy_status_ok)
             tstatus = TSH_TEST_FAILED;
           TSHMessage (ctx, MOFAILURE,
                       "sec_rgy_plcy_set_info failed\n");
         }
     }

/* sec_rgy_plcy_get_info is called either for its own test or as
   verification for the sec_rgy_plcy_set_info test of all valid data or
   as setup for sec_rgy_plcy_get_effective test. */

   if (((subcase % 100) == RGY_CASES) ||
       ((subcase % 100) == NORMAL_CASES) ||
       (subcase == GET_POLICY_NOT_FOUND))
     {

       /* wait for propagation to any replicas */
       SEC_RGY_SITE_SYNCH(write_context);

       sec_rgy_plcy_get_info (read_context,
                              test_org_name,
                              &get_policy,
                              &tstatus);

       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_plcy_get_info status: %d\n", tstatus);

       if (estatus == tstatus)
	 {

        /* Verify that the input to set_info came back as output from 
	   get_info. */

           if (tstatus == sec_rgy_status_ok)
	     if (memcmp ((char *)&get_policy,
	                 (char *)set_policy_p,
                         sizeof (sec_rgy_plcy_t)))
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
         {
           if (tstatus == sec_rgy_status_ok)
             tstatus = TSH_TEST_FAILED;
           TSHMessage (ctx, MOFAILURE,
                       "sec_rgy_plcy_get_info failed\n");
         }

     } /* End sec_rgy_plcy_get_info path */

/* sec_rgy_plcy_get_effective is only called for its own test. */

   if (subcase >= GET_EFF_POLICY)
     {
    /* The effective policy requires both a registry and org policy.
       The org policy was added as part of NORMAL_CASES.  A policy
       now has to be added for the registry.  Because the policy for the
       account is the more restrictive of the account or the registry, we
       anticipate that the effective policy will be the account policy. */

       if (subcase == GET_EFF_POLICY)
	 {
           sec_rgy_plcy_set_info (write_context,
                                  POLICY_RGY_NAME,
                                  &POLICY_RGY,
                                  &tstatus);

           TSHMessage (ctx, MOTRACE,
                   "sec_rgy_plcy_set_info (rgy) status: %d\n", tstatus);

           if (TSHStatus (ctx, SETUP,
                       "sec_rgy_plcy_set_info failed\n", estatus, tstatus));

         } /* End if (subcase == GET_EFF_POLICY) */

       /* wait for propagation to any replicas */
       SEC_RGY_SITE_SYNCH(write_context);

       sec_rgy_plcy_get_effective (read_context,
                                   test_org_name,
                                   &get_eff_policy,
                                   &tstatus);
       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_plcy_get_effective status: %d\n", tstatus);

       if (estatus == tstatus)
	 {
           if (tstatus == sec_rgy_status_ok)
             {
               if (subcase == GET_EFF_POLICY)
	         {
	           if (memcmp ((char *)&get_eff_policy,
	                       (char *)&get_policy,
                               sizeof (sec_rgy_plcy_t)))
	             {
	               TSHMessage (ctx, MOFAILURE,
                                  "Effective policy not returned\n");
                       tstatus = TSH_TEST_FAILED;
	             }
                   else
	             TSHMessage (ctx, MOTRACE,
                                "Effective policy returned\n");

                 } /* End if (subcase == GET_EFF_POLICY) */

               if (subcase == GET_EFF_POLICY_RGY)
	         {
	           if (memcmp ((char *)&get_eff_policy,
	                       (char *)&POLICY_RGY,
                               sizeof (sec_rgy_plcy_t)))
	             {
	               TSHMessage (ctx, MOFAILURE,
                               "Effective policy for rgy not returned\n");
                       tstatus = TSH_TEST_FAILED;
                     }
                   else
	             TSHMessage (ctx, MOTRACE,
                                 "Effective policy for rgy returned\n");

                 } /* End if (subcase == GET_EFF_POLICY_RGY) */

             } /* End if (tstatus == sec_rgy_status_ok) */

	   else
	     tstatus = sec_rgy_status_ok;

         } /* End if (estatus == tstatus) */

       else
         {
           if (tstatus == sec_rgy_status_ok)
             tstatus = TSH_TEST_FAILED;
           TSHMessage (ctx, MOFAILURE,
                       "sec_rgy_plcy_get_effect failed\n");
         }

     } /* End if (subcase >= GET_EFF_POLICY) */

/* Delete the org item, if necessary. */

   /* restore saved RGY policy */
   sec_rgy_plcy_set_info (write_context,
                          POLICY_RGY_NAME,
                          &save_rgy_policy,
                          &tstatus);
   TSHMessage (ctx, MOTRACE,
                   "Restored original rgy policy status: %d\n", tstatus);
		   
   if (((subcase % 100) != NOT_FOUND_CASES) &&
       ((subcase % 100) != RGY_CASES))
     {
       sec_rgy_pgo_delete (write_context, (sec_rgy_domain_t)sec_rgy_domain_org,
                           POLICY_ORG_NAME, &cstatus);
    
       TSHMessage (ctx, MOTRACE,
                   "sec_rgy_pgo_delete (cleanup org) status: %d\n", cstatus);

       if (TSHStatus (ctx, CLNUP,
                      "sec_rgy_pgo_delete (org) failed\n", rpc_s_ok, cstatus))
         tstatus = cstatus;
     }

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

} /* End policy */
