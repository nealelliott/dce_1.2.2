/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: ns_utility.c,v $
 * Revision 1.1.6.2  1996/02/17  23:21:29  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:44  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:07:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:06  root]
 * 
 * Revision 1.1.3.6  1993/01/25  18:06:08  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:40:43  bbelch]
 * 
 * Revision 1.1.3.5  1993/01/11  23:37:22  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:10:06  bbelch]
 * 
 * Revision 1.1.3.4  1992/12/11  15:19:10  cuti
 * 	Add return status for successful return.
 * 	[1992/12/11  15:17:46  cuti]
 * 
 * Revision 1.1.3.3  1992/12/09  22:08:38  cuti
 * 	Add ns_register() to register some interface.
 * 	[1992/12/09  18:20:02  cuti]
 * 
 * Revision 1.1.3.2  1992/09/29  21:22:41  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:47:49  weisman]
 * 
 * Revision 1.1  1992/01/19  03:39:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
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



#include <math.h>
#include <tsh_common.h> 
#include <tsh_ns.h>
#include <tsh_ns_utility.h>
#ifdef SNI_SVR4
#include <fcntl.h>
#else
#include <sys/file.h>
#endif    
static rpc_syntax_id_t NIDL_ndr_id = {
{0x8a885d04, 0x1ceb, 0x11c9, 0x9f, 0xe8, {0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}},
2};

extern unsigned32 tsh_util_ns_unexport();
           
error_status_t ns_register (ctx, ifspec, binding_vector_p, name)
CtxBlock          *ctx;
rpc_if_handle_t ifspec;
rpc_binding_vector_p_t *binding_vector_p; 
char	*name;
{
	error_status_t	estatus;
    error_status_t	test_status;

   rpc_server_register_if (ifspec, (uuid_p_t)0,
                           (rpc_mgr_epv_t) 0,  /* use default, in ifspec */
                           &test_status);

	TSHMessage(ctx,MOINFO,"rpc_eserver_register_if:tstatus=%d estatus=%d\n",
           test_status,error_status_ok);
   if (TSHStatus (ctx, SETUP, "rpc_server_register_if failed", test_status, rpc_s_ok))
     return (test_status);

/*
   rpc_server_use_protseq_if((unsigned char *)argv[7],
			     1, (rpc_if_handle_t)&NIDL_ifspec, &test_status);
   TSHMessage(ctx,MOINFO,"rpc_server_use_protseqs_if:tstatus=%d estatus=%d\n",
           test_status,error_status_ok);  
   if (TSHStatus (ctx, SETUP, "rpc_server_use_protseq_if failed", test_status, rpc_s_ok))
     return (test_status);   
*/
   rpc_server_use_all_protseqs(1,  &test_status);
 
	TSHMessage(ctx,MOINFO,"rpc_server_use_all_protseqs:tstatus=%d estatus=%d\n",
           test_status,error_status_ok);  

   if (TSHStatus (ctx, SETUP, "rpc_server_use_all_protseqs failed", test_status, rpc_s_ok))
     return (test_status);

    rpc_server_inq_bindings (binding_vector_p, &test_status);

	TSHMessage(ctx,MOINFO,"rpc_server_inq_bindings:tstatus=%d estatus=%d\n",
           test_status,error_status_ok);
    if (TSHStatus (ctx,SETUP,"rpc_server_inq_bindings failed",test_status,rpc_s_ok))
         return (test_status);
  
	rpc_ep_register(ifspec, *binding_vector_p, (uuid_vector_t *) NULL, 
                          (unsigned_char_p_t) name, &test_status);

	TSHMessage(ctx,MOINFO,"rpc_ep_register:tstatus=%d estatus=%d\n",
           test_status,error_status_ok);
    TSHStatus (ctx,SETUP,"rpc_ep_register failed",test_status,rpc_s_ok);

     return (test_status);
            

}

get_cell_name(cell_name)
char *cell_name; 
{  
   ndr_long_int		fd;		/* file description to obtain cell name */
   char             tmp[DNS_FNAME_MAX]; 
	char            *tmp_ptr;
	

   fd = open (CONFIG_FILE, O_RDONLY);

   read (fd, tmp, DNS_FNAME_MAX); 
	tmp_ptr = tmp;
/*	tmp_ptr = tmp_ptr + 10;  */
	while (*tmp_ptr != ' ')
		tmp_ptr++;
	tmp_ptr++;  /* skip the blank between cellname title and cell name which we want */
    while (*tmp_ptr != '\n' && *tmp_ptr != ' ' && *tmp_ptr != EOF) {
  	 	*cell_name = *tmp_ptr;
        cell_name++;
		tmp_ptr++;
	} 
	*cell_name = '\0';
	
   close (fd);

}

error_status_t tsh_util_uuid_vec_create(ctx, vector)
CtxBlock               *ctx;
artificial_vector      *vector;
{
    ndr_long_int i;
    error_status_t         test_status;	/* status value from rpc calls... */

    vector->count = 3;
    for (i = 0; i < 3; ++i)
    {
        vector->a[i] = &vector->b[i];
        uuid_create (vector->a[i], &test_status);
        if(TSHStatus(ctx, SETUP, "uuid_create() failed", 
            test_status, rpc_s_ok))
            break;
    }
    return(test_status);
}


unsigned32 tsh_util_ns_export(ctx, entry_if_spec, object_vector)
CtxBlock               *ctx;
rpc_if_rep_t           *entry_if_spec;
artificial_vector      *object_vector;
{
error_status_t         test_status;		/* status value from rpc calls... */
unsigned_char_p_t      entry_export_name;
rpc_binding_vector_p_t binding_vector;
unsigned32             loop_index,
                       continue_loop;

/*
 * Cleanup the name space, just in case.
 */
test_status = tsh_util_ns_unexport(ctx);
if ( (test_status != rpc_s_ok) && 
     (test_status != rpc_s_not_found) && 
     (test_status != rpc_s_entry_not_found))
    {
    TSHMessage(ctx, MOFAILURE, "failures during name space cleanup\n");
    return(test_status); 
    }


TSHMessage(ctx, MOINFO, "setting up the name space\n");

uuid_create (&(entry_if_spec)->id, &test_status);
if(TSHStatus(ctx, SETUP, "uuid_create() failed", 
    test_status, rpc_s_ok))
    return(test_status);

entry_if_spec->ifspec_vers = 0;
entry_if_spec->opcnt = 1;
entry_if_spec->stub_rtl_if_vers = 2;
entry_if_spec->endpoint_vector.count = 0;
entry_if_spec->endpoint_vector.endpoint_vector_elt = 
                    (rpc_endpoint_vector_elt_t *) NULL;
entry_if_spec->syntax_vector.count = 1;
entry_if_spec->syntax_vector.syntax_id = 
                    (rpc_syntax_id_t *)&NIDL_ndr_id;
entry_if_spec->server_epv = NULL;
entry_if_spec->manager_epv = (rpc_mgr_epv_t) NULL;


/*
 * Register this protocol sequence for this interface in order 
 * to inquire on the bindings needed for the exports.
 */
rpc_server_use_protseq_ep(
    tsh_c_dummy_protseq, 
    tsh_c_max_calls,
    tsh_c_dummy_endpoint, 
    &test_status);
if(TSHStatus(ctx, SETUP, "rpc_server_use_protseq_ep() failed", 
    test_status, rpc_s_ok))
    return(test_status);


/*
 * Inquire to get the bindings supported.
 */
rpc_server_inq_bindings(&binding_vector, &test_status);
if(TSHStatus(ctx, SETUP, "rpc_server_inq_bindings() failed", 
    test_status, rpc_s_ok))
    return(test_status);


for(loop_index = 1, continue_loop = TRUE;  continue_loop; loop_index++)
    {
    switch(loop_index)
        {
        case 1:
            entry_export_name = (unsigned_char_t *) "/.:/NSTEST_DIR/profile_test_export1_1";
            entry_if_spec->vers = 0x00010001;
            break;

        case 2:
            entry_export_name = (unsigned_char_t *) "/.:/NSTEST_DIR/profile_test_export1_3";
            entry_if_spec->vers = 0x00030001;
            break;

        case 3:
            entry_export_name = (unsigned_char_t *) "/.:/NSTEST_DIR/profile_test_export1_7";
            entry_if_spec->vers = 0x00070001;
            break;

        case 4:
            entry_export_name = (unsigned_char_t *) "/.:/NSTEST_DIR/profile_test_export1_8";
            entry_if_spec->vers = 0x00080001;
            break;

        case 5:
            /*
             * Set up another entry with the identical interface.
             */
            entry_export_name = (unsigned_char_t *) "/.:/NSTEST_DIR/profile_test_export1_8_dup";

            continue_loop = FALSE;
            break;
        }

TSHMessage(ctx, MOINFO, "rpc_ns_binding_export entry name: %s\n",entry_export_name);
    rpc_ns_binding_export (
        rpc_c_ns_syntax_default, 
        entry_export_name,
        (rpc_if_handle_t) entry_if_spec, 
        binding_vector, 
        (uuid_vector_t *)object_vector, 
        &test_status);
    if(TSHStatus(ctx, SETUP, "rpc_ns_binding_export() failed", 
        test_status, rpc_s_ok))
        return(test_status);
    }

rpc_binding_vector_free (&binding_vector, &test_status);
TSHMessage(ctx, MOTRACE, "free binding vector: %d\n", test_status);
TSHStatus(ctx, SETUP, "rpc_binnding_vector_free() failed", 
    test_status, rpc_s_ok);


return(test_status);
}

unsigned32 tsh_util_ns_unexport(ctx)
CtxBlock               *ctx;
{
error_status_t         test_status;		/* status value from rpc calls... */
unsigned32             loop_index, 
                       continue_loop,
                       failure = rpc_s_ok;
unsigned_char_t        *test_profile_name,
                       *test_export_name;


/*
 * first of all, delete the profile attribute from these tests.
 */
test_profile_name = tsh_c_test_profile;
rpc_ns_profile_delete(rpc_c_ns_syntax_dce, 
    test_profile_name, 
    &test_status);
if ( (test_status != rpc_s_ok) && 
     (test_status != rpc_s_not_found) && 
     (test_status != rpc_s_entry_not_found))
    {
    TSHMessage(ctx, MOFAILURE, "rpc_ns_profile_delete() failed: %d\n", test_status);
    failure = test_status;
    }


/*
 * Remove all elements 
 */
for (loop_index = 1, continue_loop = TRUE; continue_loop; loop_index++)
    {
    switch (loop_index)
        {
        case 1:
            test_export_name = (unsigned_char_t *) "/.:/NSTEST_DIR/profile_test_export1_1";
            break;

        case 2:
            test_export_name = (unsigned_char_t *) "/.:/NSTEST_DIR/profile_test_export1_3";
            break;

        case 3:
            test_export_name = (unsigned_char_t *) "/.:/NSTEST_DIR/profile_test_export1_7";
            break;

        case 4:
            test_export_name = (unsigned_char_t *) "/.:/NSTEST_DIR/profile_test_export1_8";

            continue_loop = FALSE;
            break;
        }


    rpc_ns_mgmt_entry_delete (rpc_c_ns_syntax_dce, test_export_name,
        &test_status);
    if ( (test_status != rpc_s_ok) && 
         (test_status != rpc_s_not_found) && 
         (test_status != rpc_s_entry_not_found))
        {
        TSHMessage(ctx, MOFAILURE, "rpc_ns_mgmt_entry_delete() failed: %d\n", test_status);
        failure = test_status;
        }
    }
    
return failure;
}
