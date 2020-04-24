/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: ns_binding_lookup.c,v $
 * Revision 1.1.6.2  1996/02/17  23:21:10  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:14  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:06:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:51  root]
 * 
 * Revision 1.1.4.6  1993/02/24  21:55:40  weisman
 * 	Error in ns_binding_lookup.c: Code was erroneously
 * 	passing a non-uuid into rpc_ns_binding_lookup_begin.
 * 	[1993/02/24  21:55:25  weisman]
 * 
 * Revision 1.1.4.5  1993/01/11  23:36:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:09:08  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/09  22:08:10  cuti
 * 	Change the lookup_next algorithm and remove &NIDL_ifspec, use a real one.
 * 	[1992/12/09  18:19:20  cuti]
 * 
 * Revision 1.1.4.3  1992/12/07  13:20:19  zeliff
 * 	Removed duplicate OSF copyright markers
 * 	[1992/12/06  00:44:23  zeliff]
 * 
 * Revision 1.1.4.2  1992/10/12  16:58:20  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  20:19:48  cuti]
 * 
 * Revision 1.1.2.2  1992/07/17  20:20:21  dceguest
 * 	Removed dead commented out code.  And modified TSMmessages.
 * 	[1992/07/17  20:19:35  dceguest]
 * 
 * Revision 1.1  1992/01/19  03:35:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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



#include <math.h>
#include <tsh_common.h>
#include <tsh_ns.h>
#include <tsh_call.h>
#include <mgmt_copy.h>
#include <mgmts.h>
#include <tsh_ns_utility.h>


/*  Args required by the test program...
 */
#define ARGS_REQUIRED 8


ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
ndr_long_int      subcase;		/* argv[0], subcase to run */
error_status_t    all_cleanup_status,	/* status value from lookup calls... */
                  expected_status,	/* argv[1], expected return from test */
                  cleanup_status,	/* status value from cleanup */
                  expected_status_begin, 
                  expected_status_next, 
                  expected_status_done,
                  test_status = rpc_s_ok;	/* status value from rpc calls... */
unsigned32        i,use_object_uuid,
		  iter, 
                  if_spec_flag,	/* argv[4] */
                  test_entry_name_syntax,
                  use_entry_name_syntax,
                  test_binding_max_count,
                  test_type_begin, 
                  test_type_next, 
                  test_type_done;
unsigned_char_t   test_entry_name[DNS_FNAME_MAX];        /* from argv[3] */
unsigned_char_p_t test_annotation,
                  test_export_name,
                  string_binding,
                  elt_member;
uuid_t            *test_object_uuid, obj_uuid;
uuid_vector_p_t   uuid_vector_p;	 /* required by rpc_ns_binding_{,un}export */
artificial_vector art_uuid_vector;	/* required by rpc_ns_binding_{,un}export  */
uuid_vector_t     obj_vector;
rpc_binding_vector_p_t binding_vector_p;
rpc_if_rep_t      test_if_spec;
rpc_ns_lookup_handle_t lookup_context;
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

struct if_reg * if_reg;


/*  check the number of arguments passed in.  They should be exactly the   
 *  required number when you come into this routine. Looking for a subcase,
 *  a bogus expected_status, and a hostname.
 */
if(argc < ARGS_REQUIRED)
    {
    TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
        ARGS_REQUIRED, argc);
    return(TSH_BAD_ARG_COUNT);
    }


/*  atol() the input args...
 */
   subcase                  = atol(argv[0]);
   expected_status          = atol(argv[1]);
   test_entry_name_syntax   = atol(argv[2]);
   strcpy(test_entry_name, "/.:/");
   strcat(test_entry_name, argv[3]);
   if_spec_flag             =atol(argv[4]);
   use_object_uuid          = atol(argv[5]);
   test_binding_max_count   = atol(argv[6]); 
/*
   endpoints.rpc_protseq = (unsigned_char_p_t)argv[7];
   endpoints.endpoint = (unsigned_char_p_t)"7003";
   NIDL_ifspec.endpoint_vector.count = 1;
   NIDL_ifspec.endpoint_vector.endpoint_vector_elt = &endpoints;  
*/
   cleanup_status = rpc_s_ok;



/* 
 * Each of the BEGIN, NEXT, DONE trio has its own subcase
 */
switch(subcase)
    {
    case BINDING_LOOKUP_BEGIN:
        TSHMessage(ctx, MOINFO, "running subcase BINDING_LOOKUP_BEGIN\n");
        test_type_begin = TEST; 
        expected_status_begin = expected_status;

        test_type_next  = CLNUP;
        expected_status_next  = rpc_s_ok;

        test_type_done  = CLNUP;
        expected_status_done  = rpc_s_ok;
        break;

    case BINDING_LOOKUP_NEXT:
        TSHMessage(ctx, MOINFO, "running subcase BINDING_LOOKUP_NEXT\n");
        test_type_begin = SETUP;
        expected_status_begin = rpc_s_ok;

        test_type_next  = TEST;
        expected_status_next  = expected_status;

        test_type_done  = CLNUP;
        expected_status_done  = rpc_s_ok;
        break;

    case BINDING_LOOKUP_DONE:
        TSHMessage(ctx, MOINFO, "running subcase BINDING_LOOKUP_DONE\n");
        test_type_begin = SETUP;
        expected_status_begin = rpc_s_ok;

        test_type_next  = SETUP;
        expected_status_next  = rpc_s_ok;

        test_type_done  = TEST;
        expected_status_done  = expected_status;
        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", subcase);
        return(TSH_UNKNOWN_SUBCASE);
    }


/*
 *  Adjust the values passed in as necessary
 */
if(!strcmp((char *)test_entry_name, (char *)"<NULL>"))
    strcpy(test_entry_name, NULL);  

tsh_util_uuid_vec_create (ctx, &art_uuid_vector);
uuid_vector_p = (uuid_vector_p_t) &art_uuid_vector;  /* create object uuid vector for export
                                                  and later on to use in lookup begin */


if(use_object_uuid != 0)
     test_object_uuid = uuid_vector_p->uuid[0];
else
    test_object_uuid = (uuid_t *)NULL;


/* Setup for rpc_ns_binding_{,un}export.  These rpc's require considerable setup
   as follows.  For further information on individual rpc's and their setup and 
   cleanup requirements see "Functional Specification for OSF DCE RPC". */

   if_reg = If;
         
   if ((test_status = ns_register(ctx, *if_reg->s_ifspec, &binding_vector_p, if_reg->name))
		!= error_status_ok)
	return (test_status);   

     
/* Now invoke the rpc's to be tested.  rpc_ns_binding_export requires
   rpc_ns_binding_unexport for cleanup which requires rpc_ns_binding_export for
   setup.  Therefore, both are called unconditionally.  The test cases are
   distinguished in ns_binding_{un,}export.tsh. */

   rpc_ns_binding_export (test_entry_name_syntax, test_entry_name, 
     if_spec_flag ? (rpc_if_handle_t)*if_reg->s_ifspec : NULL,
			  binding_vector_p, uuid_vector_p, &test_status);

TSHMessage(ctx,MOINFO,"rpc_ns_binding_export:tstatus=%d estatus=%d\n",
           test_status,expected_status);

   rpc_binding_vector_free(&binding_vector_p, &cleanup_status);

   if (!(TSHStatus (ctx, CLNUP, "rpc_binding_vector_free() failed",
		  cleanup_status, rpc_s_ok))) 
	 all_cleanup_status = cleanup_status;

    /*
     * Now do a lookup for all interfaces compatible with the entry_name
     */
    switch(expected_status_begin)
    {
        case rpc_s_invalid_name_syntax:
             use_entry_name_syntax = 3000;
             break;

        case rpc_s_unsupported_name_syntax:
             use_entry_name_syntax = rpc_c_ns_syntax_unknown;
             break;

        default:
             use_entry_name_syntax = test_entry_name_syntax;
    }

    rpc_ns_binding_lookup_begin (
        use_entry_name_syntax, 
        test_entry_name, 
        if_spec_flag ? (rpc_if_handle_t)*if_reg->s_ifspec : NULL,
        test_object_uuid,
        test_binding_max_count,
        &lookup_context, 
        &test_status);

TSHMessage(ctx,MOINFO,"rpc_ns_binding_lookup_begin:tstatus=%d estatus=%d\n",
           test_status,expected_status);
    if(subcase == BINDING_LOOKUP_BEGIN)
      TSHStatus(ctx, test_type_begin, "rpc_ns_binding_lookup_begin() failed", 
                test_status, expected_status_begin);

    if(subcase == BINDING_LOOKUP_BEGIN && test_status == expected_status)
      {
        TSHMessage(ctx,MOINFO,"SUCCESS: tstatus=%d estatus=%d\n",
                   test_status, expected_status);
        return(rpc_s_ok); 
      }

     i = iter = 0;
    if(test_status == rpc_s_ok && subcase == BINDING_LOOKUP_NEXT)
    {
        TSHMessage(ctx, MOINFO, "performed a successful ns lookup\n");

    /*
     * Perform the lookup based on the specified lookup requirements.
     */   

	do
        {
            if(argc > ARGS_REQUIRED) /* Invalid Lookup Context Case */
              rpc_ns_binding_lookup_next 
                  (NULL, &binding_vector_p, &test_status);
            else
              rpc_ns_binding_lookup_next 
                  (lookup_context, &binding_vector_p, &test_status);

             TSHMessage(ctx, MOTRACE, "rpc_ns_binding_lookup_next status: %d\n", test_status);
                    
            if(test_status == expected_status)
              {
                if(binding_vector_p == NULL)
                  TSHMessage(ctx,MOINFO,"Binding Vector NULL\n");
                TSHMessage(ctx,MOINFO,"SUCCESS: tstatus=%d estatus=%d\n",
                           test_status,expected_status);  
              }

                     ++i;       
           if (test_status != rpc_s_ok)
               break;  
           rpc_binding_vector_free(&binding_vector_p, &cleanup_status);
			TSHMessage(ctx, MOTRACE, "rpc_ns_binding_vector_free status: %d\n", test_status);
           TSHStatus (ctx, CLNUP, "rpc_binding_vector_free() failed",
		          cleanup_status, rpc_s_ok); 
			if (all_cleanup_status == rpc_s_ok)
               all_cleanup_status = cleanup_status;
        }while (expected_status_next != rpc_s_ok && i < 5); 
        TSHStatus(ctx, test_type_next, "rpc_ns_binding_lookup_next() failed", 
            test_status, expected_status_next);
    }

    if(test_status == rpc_s_ok ) /*|| iter == 1) */
    {
        if (subcase == BINDING_LOOKUP_DONE)
        {
           /* This is to simulate an invalid inquiry context */
           if(argc > ARGS_REQUIRED)
             lookup_context=NULL;
           rpc_ns_binding_lookup_done (&lookup_context, &test_status);
           TSHMessage(ctx, MOTRACE, "rpc_ns_binding_lookup_done status: %d\n", test_status);
/* Print out the current Stats */
TSHMessage(ctx,MOINFO,"rpc_ns_binding_lookup_done:tstatus=%d estatus=%d\n",
           test_status,expected_status);
          TSHStatus(ctx, test_type_done, "rpc_ns_binding_lookup_done() failed", 
                    test_status, expected_status_done);
        }
        else
        {
           /* This is to simulate an invalid inquiry context */
           if(argc > ARGS_REQUIRED)
             lookup_context=NULL;
           rpc_ns_binding_lookup_done (&lookup_context, &cleanup_status);

			TSHMessage(ctx,MOINFO,"rpc_ns_binding_lookup_done:cleanstatus=%d estatus=%d\n",
           cleanup_status,expected_status);

           TSHMessage(ctx, MOTRACE, "rpc_ns_binding_lookup_done status: %d\n", cleanup_status);
           TSHStatus(ctx, test_type_done, "rpc_ns_binding_lookup_done() failed", 
            cleanup_status, rpc_s_ok);
        }
    }

   /* In case I have already had an error, I don't want to turn it into an ok later */
   if(!(TSHStatus (ctx, CLNUP, "rpc_binding_vector_free() failed",
		  all_cleanup_status, rpc_s_ok)))
      all_cleanup_status = cleanup_status;

   rpc_ns_binding_unexport (test_entry_name_syntax, test_entry_name,
     if_spec_flag ? (rpc_if_handle_t)*if_reg->s_ifspec : NULL,
			    uuid_vector_p, &cleanup_status);
	TSHMessage(ctx,MOINFO,"rpc_ns_binding_unexport:cleanstatus=%d estatus=%d\n",
           cleanup_status,error_status_ok);

   TSHStatus (ctx, CLNUP,
		  "rpc_ns_binding_unexport() failed",
		  cleanup_status, rpc_s_ok);
   if (all_cleanup_status == rpc_s_ok)
       all_cleanup_status = cleanup_status;


   rpc_server_unregister_if (*if_reg->s_ifspec, (uuid_p_t)0, &cleanup_status);
   TSHStatus (ctx, CLNUP,
		  "rpc_server_unregister_if() failed",
		  cleanup_status, rpc_s_ok);
   if (all_cleanup_status == rpc_s_ok)
       all_cleanup_status = cleanup_status;

   /* Now figure out what to return */
   if (test_status != expected_status)
       return(FAILURE);
   if (all_cleanup_status != rpc_s_ok)
       return(all_cleanup_status);
return(rpc_s_ok);
}

void fake(handle)
handle_t handle;
{
}

void fakes(handle)
handle_t handle;
{
}

