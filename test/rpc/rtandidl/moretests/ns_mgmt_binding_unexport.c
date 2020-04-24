/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ns_mgmt_binding_unexport.c,v $
 * Revision 1.1.8.2  1996/02/17  23:21:18  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:26  marty]
 *
 * Revision 1.1.8.1  1995/12/11  20:07:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:57  root]
 * 
 * Revision 1.1.4.7  1993/04/08  19:24:39  weisman
 * 	Remove all udp and accept protseq from command-line.
 * 	[1993/04/08  19:13:51  weisman]
 * 
 * Revision 1.1.4.6  1993/01/27  17:29:49  bbelch
 * 	Restore truncated files
 * 	[1993/01/27  17:27:45  bbelch]
 * 
 * Revision 1.1.4.5  1993/01/25  18:05:59  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:38:33  bbelch]
 * 
 * Revision 1.1.4.4  1993/01/11  23:36:58  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:09:37  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/09  22:08:23  cuti
 * 	Fix on NO_INTERFACE_EXPORTED subcase to export no inerface and no binding
 * 	[1992/12/09  18:19:40  cuti]
 * 
 * Revision 1.1.4.2  1992/10/12  17:04:16  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  20:21:09  cuti]
 * 
 * Revision 1.1.2.2  1992/07/08  20:18:27  smith
 * 	Modified TSHMessage to include estatus, add TSHMessage for if_id and obj_flag,
 * 	changed TSHMessage MOTRACE to MOINFO, and add return status of
 * 	rpc_s_ok if tstatus == estatus.
 * 	[1992/07/08  20:17:23  smith]
 * 
 * Revision 1.1  1992/01/19  03:35:57  devrcs
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
#include <tsh_call.h>
#include <dce/uuid.h>
#include <tsh_ns_utility.h>


/*  Args required by the test program.  */

#define ARGS_REQUIRED	8

/* This routine is called to see if any bindings are in the namespace. */
static error_status_t check_bindings(ctx, entry_name_syntax, entry_name, 
                                     a_handle, no_expected_bindings)
CtxBlock          *ctx;
unsigned32	  entry_name_syntax;
unsigned_char_p_t entry_name;
rpc_if_handle_t   a_handle;
ndr_short_int     no_expected_bindings;
{              
  unsigned32             tstatus;
  unsigned32             cstatus;
  rpc_ns_lookup_handle_t lookup_context;
  rpc_binding_vector_t   *binding_vec;
  ndr_long_int           i;

  rpc_ns_binding_lookup_begin(entry_name_syntax, entry_name, a_handle, 
		              NULL, 4, &lookup_context, &tstatus);
	TSHMessage(ctx, MOTRACE, "rpc_ns_binding_lookup_begin status: %d\n", tstatus);

  if (TSHStatus (ctx, TEST, "rpc_ns_binding_lookup_begin() failed", 
                              tstatus, rpc_s_ok))
     return(FAILURE);
  i = 0;  /* must count the number of bindings still here */
  while (true)
  {
     rpc_ns_binding_lookup_next(lookup_context, &binding_vec, &tstatus);
     TSHMessage(ctx, MOTRACE, "rpc_ns_binding_lookup_next status: %d\n", tstatus);
     if (tstatus != rpc_s_ok)
	break;

     ++i;
  } 

  rpc_ns_binding_lookup_done(&lookup_context, &cstatus);
  TSHMessage(ctx, MOTRACE, "rpc_ns_binding_lookup_done status: %d\n", cstatus);


  if (TSHStatus(ctx,TEST,"loop should have terminated due to no more bindings",
		 tstatus, rpc_s_no_more_bindings))
     return(FAILURE);
  if (TSHStatus (ctx, TEST, "wrong number of bindings found", 
                                                i, no_expected_bindings))
     return(FAILURE);
  return(rpc_s_ok);
}              

/* This routine is called to see if any objects are in the namespace. */
static error_status_t check_objects(ctx, entry_name_syntax, entry_name,
                                  no_expected_objs)
CtxBlock          *ctx;
unsigned32	  entry_name_syntax;
unsigned_char_p_t entry_name;
ndr_short_int     no_expected_objs;
{              
  rpc_if_id_vector_t  a_if_id_vec;
  error_status_t      tstatus;
  rpc_ns_inq_handle_t inquiry_context;
  uuid_t              some_obj;
  ndr_long_int        i;

  /* This requires a set of calls to rpc_ns_entry_object_inq() */
  rpc_ns_entry_object_inq_begin(entry_name_syntax, entry_name,
       &inquiry_context, &tstatus);
  TSHMessage(ctx, MOTRACE, "rpc_ns_entry_object_inq_begin status: %d\n", tstatus);

  i = 0;
  while (true)
  {
     rpc_ns_entry_object_inq_next(inquiry_context, &some_obj, &tstatus);
	 TSHMessage(ctx, MOTRACE, "rpc_ns_entry_object_inq_next status: %d\n", tstatus);

     if (tstatus != rpc_s_ok)
	break;
     ++i;
  } 
  if (TSHStatus(ctx,TEST,"loop should have terminated due to no more members",
		 tstatus, rpc_s_no_more_members))
     return(FAILURE);
  rpc_ns_entry_object_inq_done(&inquiry_context, &tstatus);
  TSHMessage(ctx, MOTRACE, "rpc_ns_entry_object_inq_done status: %d\n", tstatus);

  if (TSHStatus(ctx,TEST,"rpc_ns_entry_object_inq_done() failed",
		 tstatus, rpc_s_ok))
     return(FAILURE);
  if (TSHStatus (ctx, TEST, "wrong number of objects found", 
                                                i, no_expected_objs))
     return(FAILURE);
  return(rpc_s_ok);
}

ndr_long_int TestFunction (ctx, argc, argv)

/* Formal parameters */

   CtxBlock          *ctx;
   ndr_long_int      argc;
   unsigned_char_p_t argv[];
{

/* Local variables */

   ndr_long_int                subcase;         /* argv[0], subcase to run   */
   error_status_t              estatus;		/* argv[1], expected return  */
   error_status_t              tstatus;		/* status value from rpc     */
   error_status_t              dummy_status;	
   unsigned32                  entry_name_syntax; /* argv[2] */
   unsigned_char_t             entry_name[DNS_FNAME_MAX];        /* from argv[3] */
   unsigned32                  if_id_flag;        /* argv[4] */
   unsigned32                  obj_flag;          /* argv[5] */
   unsigned32                  vers_value;        /* argv[6] */
   unsigned_char_p_t           rpc_protseq;       /* argv[7] */

   artificial_vector           some_obj_vector;
   artificial_vector           diff_obj_vector;
   rpc_endpoint_vector_elt_t   endpoints;
   rpc_if_id_vector_t          a_if_id_vec;
   rpc_ns_inq_handle_t         inquiry_context;
   uuid_vector_t               *ptr_to_obj_vector;
   rpc_if_id_t                 my_if_id;
   ndr_long_int                j;
   unsigned32                  original_version;
   rpc_binding_vector_p_t binding_vector_p;/* required by rpc_ns_binding_{,un}export */

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
   entry_name_syntax = atol(argv[2]);
   strcpy(entry_name, "/.:/");
   strcat(entry_name, argv[3]);
   if_id_flag =atol(argv[4]);
   obj_flag =atol(argv[5]);
   vers_value =atol(argv[6]);
   rpc_protseq =  (unsigned_char_p_t)argv[7];
   endpoints.rpc_protseq = rpc_protseq;

   endpoints.endpoint = (unsigned_char_p_t)"7004";
   NIDL_ifspec.endpoint_vector.count = 1;
   NIDL_ifspec.endpoint_vector.endpoint_vector_elt = &endpoints;
   if (subcase == NOTHING_TO_UNEXPORT)
   {
      rpc_ns_mgmt_binding_unexport(entry_name_syntax, entry_name,
           NULL, vers_value, NULL, &tstatus);
TSHMessage(ctx,MOINFO,"rpc_ns_mgmt_binding_unexport status: %d estatus %d\n", 
           tstatus,estatus);
            
      if (TSHStatus (ctx, TEST, 
         "rpc_ns_mgmt_binding_unexport() failed, namespace probably corrupt",
	                 tstatus, estatus))
              return (tstatus);
      return(rpc_s_ok);
   }

   tstatus = tsh_util_uuid_vec_create(ctx, &some_obj_vector);
   if (TSHStatus(ctx, SETUP, "object vector setup failed, aborting test", 
                       tstatus, rpc_s_ok))
       return(tstatus);

   rpc_server_use_protseq_if(rpc_protseq, 1, (rpc_if_handle_t)&NIDL_ifspec,
			     &tstatus);
 
   if (TSHStatus (ctx, SETUP, "rpc_server_use_protseq_if failed", tstatus, rpc_s_ok))
     return (tstatus);

       rpc_server_inq_bindings (&binding_vector_p, &tstatus);

       if (TSHStatus (ctx,SETUP,"rpc_server_inq_bindings failed",tstatus,rpc_s_ok))
         return (tstatus);
      

   /* need to get the interface UUID */
   rpc_if_inq_id((rpc_if_handle_t)&NIDL_ifspec,(rpc_if_id_t *)&my_if_id,&tstatus);
   original_version = NIDL_ifspec.vers;
   for (j=0; j <= 2 ;++j)
   {
      rpc_ns_binding_export (entry_name_syntax, entry_name, 
			  (subcase == NO_INTERFACES_EXPORTED) ? NULL :
              (rpc_if_handle_t)&NIDL_ifspec, 
              (subcase == NO_INTERFACES_EXPORTED) ?
              NULL : binding_vector_p,
             (vers_value == rpc_c_vers_all) ?
                (uuid_vector_t *)&some_obj_vector : NULL, &tstatus);
      TSHMessage(ctx,MOINFO,"rpc_ns_binding_export status: %d estatus %d\n", 
                 tstatus,error_status_ok);

      if (vers_value == rpc_c_vers_all) 
         break;
      /* export again with a minor number greater than before */
      /* and also with a major number less than before        */
      if (j == 0)
          NIDL_ifspec.vers += 0x00010000; /* bump the minor */
      else if (j == 1)
          NIDL_ifspec.vers -= 0x00000001; /* decrement the major */
   }
   /* remember to do a rpc_if_id_vector_free() */
   rpc_binding_vector_free (&binding_vector_p, &dummy_status);


   switch(vers_value)
   {
      case rpc_c_vers_all:
          my_if_id.vers_major = 0;
      case rpc_c_vers_major_only:
          my_if_id.vers_minor = 0;
          break;
   }


   /* create different uuids than the three normally used */
   /* then figure out how to get rid of them later */
   if (subcase == NOT_ALL_OBJECTS_UNEXPORTED)
   {
      tstatus = tsh_util_uuid_vec_create(ctx, &diff_obj_vector);
      if (TSHStatus(ctx, SETUP, "object vector setup failed, aborting test", 
                       tstatus, rpc_s_ok))
         return(tstatus);
      ptr_to_obj_vector = (uuid_vector_t *)&diff_obj_vector;
   }
   else
      ptr_to_obj_vector = (uuid_vector_t *)&some_obj_vector;

   TSHMessage(ctx,MOINFO,"if_id = %d\n",if_id_flag);
   if(obj_flag==99999999)
   {
   TSHMessage(ctx,MOINFO,"obj_flag = %d\n",obj_flag);
   rpc_ns_mgmt_binding_unexport(entry_name_syntax, entry_name,
                                if_id_flag ? 
                                (rpc_if_id_t *)&my_if_id : 
                                (rpc_if_id_p_t) NULL,
                                vers_value,(uuid_vector_p_t)-1,&tstatus);
 TSHMessage(ctx,MOINFO,"rpc_ns_mgmt_binding_unexport status: %d estatus: %d\n", 
            tstatus,estatus);
   }
   else
   {
   rpc_ns_mgmt_binding_unexport(entry_name_syntax, entry_name,
       if_id_flag ? (rpc_if_id_t *)&my_if_id : NULL, vers_value,
       obj_flag ? ptr_to_obj_vector : NULL, &tstatus);
 TSHMessage(ctx,MOINFO,"rpc_ns_mgmt_binding_unexport status: %d estatus: %d\n", 
            tstatus,estatus);
   }
   if(tstatus == estatus)
     return(rpc_s_ok);
            
   if (TSHStatus (ctx, TEST, 
      "rpc_ns_mgmt_binding_unexport() failed, namespace probably corrupt",
	                 tstatus, estatus))
      return(tstatus);

   /* Now we must see what remains in the data base to verify that only those */
   /* entries requested to be deleted, were in fact deleted. */
   /* This section must also handle specific cleanup as needed. */
   switch(subcase)
   {
      case REMOVE_ALL:
          if ((tstatus = check_bindings(ctx, entry_name_syntax, entry_name, 
             (rpc_if_handle_t)&NIDL_ifspec, 0)) != rpc_s_ok)
               break;
          tstatus = check_objects (ctx, entry_name_syntax, entry_name, 0);
          break;
          
      case REMOVE_BINDINGS:
          if ((tstatus = check_bindings(ctx, entry_name_syntax, entry_name, 
             (rpc_if_handle_t)&NIDL_ifspec, 0)) != rpc_s_ok)
                break;
          tstatus = check_objects (ctx, entry_name_syntax, entry_name, 3);
          break;

      case REMOVE_OBJECTS:
          if ((tstatus = check_bindings(ctx, entry_name_syntax, entry_name, 
             (rpc_if_handle_t)&NIDL_ifspec, 1)) != rpc_s_ok)
                break;
          tstatus = check_objects (ctx, entry_name_syntax, entry_name, 0);
          break;
          
      case REMOVE_COMPAT:
      case REMOVE_MAJOR_ONLY:
      case REMOVE_EXACT:
          NIDL_ifspec.vers = original_version;
          if ((tstatus = check_bindings(ctx, entry_name_syntax, entry_name, 
             (rpc_if_handle_t)&NIDL_ifspec, 0)) != rpc_s_ok)
                break;
          NIDL_ifspec.vers += 0x00010000; /* bump the minor */
          if ((tstatus = check_bindings(ctx, entry_name_syntax, entry_name, 
             (rpc_if_handle_t)&NIDL_ifspec, 
             (subcase == REMOVE_MAJOR_ONLY) ? 0 : 1)) != rpc_s_ok)
                break;
          NIDL_ifspec.vers -= 0x00000001; /* decrement the major */
          if ((tstatus = check_bindings(ctx, entry_name_syntax, entry_name, 
             (rpc_if_handle_t)&NIDL_ifspec, 
             (subcase == REMOVE_UPTO) ? 0 : 1)) != rpc_s_ok)
                break;

      case NOT_ALL_OBJECTS_UNEXPORTED:
      case NO_INTERFACES_EXPORTED:
          tstatus = rpc_s_ok;

   }
   /* This should cleanup everything that may have been left in the namespace.*/
   my_if_id.vers_major = 0;
   my_if_id.vers_minor = 0;
   rpc_ns_mgmt_binding_unexport(entry_name_syntax, entry_name,
       (subcase == NO_INTERFACES_EXPORTED || subcase == REMOVE_BINDINGS) ? NULL : (rpc_if_id_t *)&my_if_id, 
       rpc_c_vers_all, (uuid_vector_t *)&some_obj_vector, &dummy_status);
	TSHMessage(ctx, MOTRACE, "rpc_ns_mgmt_binding_unexport status: %d\n", dummy_status);

   /* It doesn't matter what returns here, it's only cleanup. In some cases   */
   /* bindings won't be there, in others objects won't be there.  Anything is */
   /* possible based on the test being done.                                  */

/* Exit this function. */
   return(tstatus);
}

