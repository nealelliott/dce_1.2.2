/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ns_binding_export.c,v $
 * Revision 1.1.6.2  1996/02/17  23:21:07  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:09  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:06:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:49  root]
 * 
 * Revision 1.1.4.5  1993/04/08  19:24:29  weisman
 * 	Remove all udp and accept protseq from command-line.
 * 	[1993/04/08  19:13:31  weisman]
 * 
 * Revision 1.1.4.4  1993/01/11  23:36:37  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:08:52  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/07  13:20:14  zeliff
 * 	Removed duplicate OSF copyright markers
 * 	[1992/12/06  00:44:15  zeliff]
 * 
 * Revision 1.1.4.2  1992/10/12  16:54:07  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  20:18:56  cuti]
 * 
 * Revision 1.1.2.2  1992/07/17  20:11:02  dceguest
 * 	made changes to STATUS MSG.
 * 	[1992/07/17  20:10:19  dceguest]
 * 
 * Revision 1.1  1992/01/19  03:35:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
# Initial revision
#
*/

/* Include files */

#include <tsh_common.h>
#include <tsh_ns.h>
#include <tsh_call.h>
#include <mgmt_copy.h>
#include <mgmts.h>
#include <dce/uuid.h>
#include <tsh_ns_utility.h>


/*  Args required by the test program.  */

#define ARGS_REQUIRED	8

ndr_long_int TestFunction (ctx, argc, argv)

/* Formal parameters */

   CtxBlock          *ctx;
   ndr_long_int      argc;
   unsigned_char_p_t argv[];
{

/* Local variables */

   int               testflag=0;	/* normal, or special test */
   ndr_long_int      subcase;		/* argv[0], subcase to run */
   error_status_t    estatus;		/* argv[1], expected return from test */
   unsigned32        tstatus;		/* status value from rpc tests... */
   error_status_t    cleanup_status;	/* status value from rpc tests... */
   error_status_t    all_cleanup_status;/* status value from rpc tests... */
   unsigned32	     entry_name_syntax; /* argv[2] */
   unsigned_char_t   entry_name[DNS_FNAME_MAX];        /* from argv[3] */
   unsigned32	     if_spec_flag;	/* argv[4] */
   unsigned32	     uuid_vector_flag;	/* argv[5] */
   unsigned32	     binding_vector_flag; /* argv[6] */
   unsigned_char_p_t  rpc_protseq ;     /* argv[7] */

   rpc_binding_vector_p_t binding_vector_p;/* required by rpc_ns_binding_{,un}export */
   uuid_vector_p_t   uuid_vector_p;	 /* required by rpc_ns_binding_{,un}export */
   artificial_vector art_uuid_vector;	/* required by rpc_ns_binding_{,un}export  */
   rpc_endpoint_vector_elt_t   endpoints;

/*
 * Kludge defs
 */

char xname[72];
struct if_reg {
    rpc_if_handle_t *s_ifspec;
    char *name;
} If[] =  {
        { &mgmtcopy_v1_0_s_ifspec,      "Mgmt_Copy"     },
        { &mgmts_v1_0_s_ifspec, "Mgmt_Tests"    },
        {0,0}
        };

struct if_reg *	if_reg;

/* Check the number of arguments passed in.  They should be exactly the   
   required number when you come into this routine. */

   if (argc < ARGS_REQUIRED)
     {
       TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
					ARGS_REQUIRED, argc);
       return(TSH_BAD_ARG_COUNT);
     }

/* Obtain the input arguments from the .tsh command line. */

   tstatus = 0;
   subcase = atol(argv[0]);
   estatus = atol(argv[1]);
   entry_name_syntax = atol(argv[2]);
   strcpy(entry_name, "/.:/");  /* local cell root */
   strcat(entry_name, argv[3]);
   if_spec_flag = atol (argv[4]);
   uuid_vector_flag = atol (argv[5]);
   binding_vector_flag = atol (argv[6]);
   rpc_protseq = (unsigned_char_p_t)argv[7];
   endpoints.rpc_protseq = rpc_protseq;

   endpoints.endpoint = (unsigned_char_p_t)"7000";
   NIDL_ifspec.endpoint_vector.count = 1;
   NIDL_ifspec.endpoint_vector.endpoint_vector_elt = &endpoints;

TSHMessage(ctx,MOINFO,"Expected Return Status: %d\n",estatus);

/* This is for the case where you don't setup any protseqs, or do any 
   initialization prior to the call.                                   */
   if(argc>ARGS_REQUIRED)
     {
     rpc_ns_binding_export (entry_name_syntax, entry_name, 
    (rpc_if_handle_t)if_spec_flag, binding_vector_p, uuid_vector_p, &tstatus); 
     TSHMessage(ctx,MOINFO,"rpc_ns_binding_export: tstatus=%d estatus=%d\n",
                tstatus,estatus);
     if(tstatus==estatus)
       return(TSH_SUCCESS);
     else
       return(TSH_TEST_FAILED);
     }


/* Adjust the values passed in as necessary. */

   if (!strcmp((char *)entry_name, (char *)"<NULL>"))
     strcpy(entry_name, NULL);


/* Setup for rpc_ns_binding_{,un}export.  These rpc's require considerable setup
   as follows.  For further information on individual rpc's and their setup and 
   cleanup requirements see "Functional Specification for OSF DCE RPC". */

   /*If[0].name = &xname;*/
   if_reg = If;

   rpc_server_register_if (*if_reg->s_ifspec, 
                           (uuid_p_t) 0,
			   (rpc_mgr_epv_t) 0,
                            &tstatus);


   if (TSHStatus (ctx, SETUP, "rpc_server_register_if failed", tstatus, rpc_s_ok))
     return (tstatus);

   rpc_server_use_protseq_if(rpc_protseq, 1, (rpc_if_handle_t) &NIDL_ifspec,
 			     &tstatus);


#ifdef NOTDEF
    rpc_server_use_all_protseqs_if (1, (rpc_if_handle_t)&NIDL_ifspec, &tstatus);
#endif
 
   if (TSHStatus (ctx, SETUP, "rpc_server_use_protseq_if failed", tstatus, rpc_s_ok))
   {
      rpc_server_unregister_if (*if_reg->s_ifspec, (uuid_p_t)0, 
           &cleanup_status);
      return (tstatus);
   }


   if (binding_vector_flag == MAKE_GOOD_BINDING_VECTOR)
   {
       rpc_server_inq_bindings (&binding_vector_p, &tstatus);

       if (TSHStatus (ctx,SETUP,"rpc_server_inq_bindings failed",
                                                    tstatus,rpc_s_ok))
       {
          rpc_server_unregister_if (*if_reg->s_ifspec, (uuid_p_t)0, 
                    &cleanup_status);
          return (tstatus);
       }
   }
   else
       binding_vector_p = (rpc_binding_vector_p_t) 0;
      
   if (uuid_vector_flag == MAKE_GOOD_UUID_VECTOR)
   {
       tsh_util_uuid_vec_create (ctx, &art_uuid_vector);
       uuid_vector_p = (uuid_vector_p_t) &art_uuid_vector;
   }
   else
       uuid_vector_p = (uuid_vector_p_t) 0;

   all_cleanup_status = rpc_s_ok;
/*****************************************************************************
   Now invoke the rpc's to be tested.  rpc_ns_binding_export requires          
   rpc_ns_binding_unexport for cleanup which requires rpc_ns_binding_export for
   setup.  Therefore, both are called unconditionally.  The test cases are
   distinguished in ns_binding_{un,}export.tsh. 
*****************************************************************************/

/***********************************************************/
/* Test for "invalid" if_spec, binding_vector, uuid_vector */
/***********************************************************/
   if(if_spec_flag==500)
     {
     TSHMessage(ctx,MOINFO,"Test with invalid if_spec\n");
     rpc_ns_binding_export(entry_name_syntax,entry_name,(rpc_if_handle_t)NULL, 
    			    binding_vector_p, uuid_vector_p, &tstatus); 
     TSHMessage(ctx,MOINFO,"rpc_ns_binding_export status: %d estatus: %d\n", 
                tstatus,estatus);
     goto EXIT;
     }
   if(uuid_vector_flag==500)
     {
     TSHMessage(ctx,MOINFO,"Test with invalid uuid_vector\n");
     rpc_ns_binding_export (entry_name_syntax, entry_name, 
                           (rpc_if_handle_t)&NIDL_ifspec,
    			   binding_vector_p,(uuid_vector_p_t)NULL, &tstatus); 
     TSHMessage(ctx,MOINFO,"rpc_ns_binding_export status: %d estatus: %d\n", 
                tstatus,estatus);
     goto EXIT;
     }
   if(binding_vector_flag==500)
     {
     TSHMessage(ctx,MOINFO,"Test with invalid binding_vector\n");
     rpc_ns_binding_export (entry_name_syntax, entry_name, 
                           (rpc_if_handle_t)&NIDL_ifspec,
    			 (rpc_binding_vector_p_t)NULL,uuid_vector_p,&tstatus);
     TSHMessage(ctx,MOINFO,"rpc_ns_binding_export status: %d estatus: %d\n", 
                tstatus,estatus);
     goto EXIT;
     }
   if(estatus == rpc_s_unsupported_name_syntax)
      entry_name_syntax = rpc_c_ns_syntax_unknown;
/***********************************/
/* Now test all the "normal" cases */
/***********************************/

   if((!testflag) && (binding_vector_flag == 0))
     {
             rpc_ns_binding_export (entry_name_syntax, entry_name, 
                  if_spec_flag ? (rpc_if_handle_t)&NIDL_ifspec : NULL,
                  (rpc_binding_vector_p_t)NULL, uuid_vector_p, &tstatus); 

      TSHMessage(ctx,MOINFO,"rpc_ns_binding_export status: %d estatus: %d\n", 
                 tstatus,estatus);
     goto EXIT;
     }

   if(!testflag)
     {
             rpc_ns_binding_export (entry_name_syntax, entry_name, 
                  if_spec_flag ? (rpc_if_handle_t)&NIDL_ifspec : NULL,
                  binding_vector_p, uuid_vector_p, &tstatus); 
      TSHMessage(ctx,MOINFO,"rpc_ns_binding_export status: %d estatus: %d\n", 
                 tstatus,estatus);
     goto EXIT;
     }

     if (tstatus == rpc_s_ok)
     {
        rpc_ns_binding_unexport (entry_name_syntax, entry_name,
          (if_spec_flag && binding_vector_flag) ? (rpc_if_handle_t)&NIDL_ifspec : NULL,
			    uuid_vector_p, &all_cleanup_status);
        TSHMessage(ctx, MOTRACE, "rpc_ns_binding_unexport status: %d\n", 
                                                        all_cleanup_status);
     }


/* Cleanup from these rpc tests. */

   rpc_server_unregister_if (*if_reg->s_ifspec, (uuid_p_t)0, 
           &cleanup_status);

   TSHStatus (ctx, CLNUP,
		  "rpc_server_unregister_if() failed",
		  cleanup_status, rpc_s_ok);
   if (all_cleanup_status == rpc_s_ok)
       all_cleanup_status = cleanup_status;

   if (binding_vector_flag == MAKE_GOOD_BINDING_VECTOR)
   {
       rpc_binding_vector_free (&binding_vector_p, &cleanup_status);

       TSHStatus (ctx, CLNUP,
		      "rpc_binding_vector_free() failed",
		      cleanup_status, rpc_s_ok);
       if (all_cleanup_status == rpc_s_ok)
           all_cleanup_status = cleanup_status;
       
   }

/************************************************************/
/* Print out the status flags to see what is being returned */
/************************************************************/

TSHMessage(ctx,MOINFO,"tstatus = %d estatus = %d all_cleanup_status= %d\n",
        tstatus,estatus,all_cleanup_status);
EXIT:

/* Exit this function. */
   if (tstatus != estatus)
      return (FAILURE);
   else
      return (rpc_s_ok);
   if (all_cleanup_status != rpc_s_ok)
      return(all_cleanup_status);
   return (rpc_s_ok);

}  /* End ns_binding_export */

void fake(handle)
handle_t handle;
{
}

void fakes(handle)
handle_t handle;
{
}
