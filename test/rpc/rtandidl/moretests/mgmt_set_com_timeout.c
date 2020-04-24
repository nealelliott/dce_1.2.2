/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mgmt_set_com_timeout.c,v $
 * Revision 1.1.7.2  1996/02/17  23:20:54  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:53  marty]
 *
 * Revision 1.1.7.1  1995/12/11  20:06:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:39  root]
 * 
 * Revision 1.1.5.3  1993/01/25  18:05:25  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:37:34  bbelch]
 * 
 * Revision 1.1.5.2  1993/01/11  23:35:40  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:07:57  bbelch]
 * 
 * Revision 1.1.2.2  1992/08/13  19:35:48  root
 * 	Initialized TestTimeout to proper invalid value in INVALID timeout test
 * 	[1992/08/13  19:33:31  root]
 * 
 * Revision 1.1  1992/01/19  03:35:32  devrcs
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



#include <math.h>
#include <dce/rpc.h>
#include <dce/stubbase.h>
#include <tsh_mgmt.h>


/*  Args required by the test program...
 */
#define ARGS_REQUIRED 5


/*  Generic test progrm entry point
 */
ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
ndr_long_int      SubCase;		/* argv[0], subcase to run */
error_status_t    EStatus;		/* argv[1], expected return from test */
unsigned32        timeout;		/* argv[2], option for call_handle */
unsigned_char_p_t bound_string;		/* bound handle */
unsigned_char_p_t endpoint;     	/* the endpoint for a given protseq */
error_status_t    status;		/* generic status variable */
handle_t          binding;		/* ectra binding handle pointer */
rpc_call_handle_t call_h = NULL;	/* call_handle */
rpc_transfer_syntax_t transfer_syntax;	/* transfer_syntax for call_handle */
rpc_if_handle_t   *if_spec; 		/* if_spec for call_handle */
int               index;		/* generic index value */
unsigned32          TestTimeout;		/* timeout value received from call */



/*  check the number of arguments passed in.  They should be exactly the   
 *  required number when you come into this routine. Looking for a subcase,
 *  a bogus expected_status, and a hostname.
 */
if(argc != ARGS_REQUIRED)
    {
    TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
        ARGS_REQUIRED, argc);
    return(TSH_BAD_ARG_COUNT);
    }


/*  atol() the input args...
 */
SubCase = atol(argv[0]);
EStatus = atol(argv[1]);
timeout = atol(argv[2]);


if (SubCase != INVALID_BINDING)
{
    /*  Assign the correct endpoint to the right protocol sequence
     */
    for (index = 0, endpoint = NULL; index < tsh_c_number_of_protseqs; index++)
    {
        if (!strcmp((char *)argv[3], (char *)NIDL_endpoints[index].rpc_protseq))
        endpoint = NIDL_endpoints[index].endpoint;
    }


    /*  Do the compose of the string, followed by the binding from string.
     */
    rpc_string_binding_compose( TSH_CALL_UUID, argv[3], argv[4], endpoint, NULL, 
        &bound_string, &status);
    if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose() error", status, rpc_s_ok))
        return(status);
    rpc_binding_from_string_binding(bound_string, &binding, &status);
    if (TSHStatus(ctx, SETUP, "rpc_binding_from_string() error", status, rpc_s_ok))
        return(status);
}

/*  run the requested SubCase
 */
switch(SubCase)
    {
    case SET_COM_TIMEOUT:
        TSHMessage(ctx, MOINFO, "Running subcase SET_COM_TIMEOUT\n");

        rpc_mgmt_set_com_timeout(binding, timeout, &status);
        if (TSHStatus(ctx, TEST, "rpc_mgmt_set_com_timeout() error", 
           status, rpc_s_ok))
           {
           return(status);
           }

        rpc_mgmt_inq_com_timeout(binding, &TestTimeout, &status);
        if (TSHStatus(ctx, TEST, "rpc_mgmt_inq_com_timeout() error", 
           status, rpc_s_ok))
           {
           return(status);
           }

        if((status == rpc_s_ok) && (TSHStatus(ctx, TEST, 
            "rpc_mgmt_inq_com_timeout() returned incorrect data", 
            timeout, TestTimeout)))
            {
            return(TSH_DATA_MISSMATCH);
            }
        
        break;

    case INVALID_BINDING:
        TSHMessage(ctx, MOINFO, "Running subcase INVALID_BINDING_HANDLE\n");

        rpc_mgmt_set_com_timeout(tsh_c_invalid_handle, TestTimeout, &status);
        if (TSHStatus(ctx, TEST, "rpc_mgmt_set_com_timeout() error", 
           status, EStatus))
           {
           return(status);
           }

        break;
 
    case INVALID_TIMEOUT:
        TSHMessage(ctx, MOINFO, "Running subcase INVALID_TIMEOUT_VALUE\n");

	TestTimeout=timeout;
        rpc_mgmt_set_com_timeout(binding, TestTimeout, &status);
        if (TSHStatus(ctx, TEST, "rpc_mgmt_set_com_timeout() error", 
           status, EStatus))
           {
           return(status);
           }

        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", SubCase);
        return(TSH_UNKNOWN_SUBCASE);
    }


return(TSH_SUCCESS);
}
