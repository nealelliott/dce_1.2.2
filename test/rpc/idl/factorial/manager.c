/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.4.2  1996/02/17  23:14:09  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:41  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:05:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:31:19  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:56:18  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:33:24  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:32:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <factorial.h>
#include <test_common.h>

globalref handle_t server_binding;

static idl_long_int factorial(h, npp)
    handle_t h;
    idl_long_int **npp;
{
    idl_long_int n = **npp;
    idl_long_int *numberp;
    idl_long_int *in_numberp;
    idl_long_int result;
    handle_t local_binding; 
    error_status_t status;

    if (n == 0) return 1;
    /*
     * Allocate a pointer to (n-1) integer for recursive call.
     */
    numberp = in_numberp = (idl_long_int *)rpc_ss_allocate(sizeof(idl_long_int));
    *numberp = n - 1;

    /* Can only use a binding once, so make a copy of it */
    rpc_binding_copy(server_binding,&local_binding,&status);
    if (status != error_status_ok) 
        printf("***FAILURE***: Bad status (%s) from rpc_binding_copy\n",error_text(status));

    result = n * net_factorial(local_binding, &numberp);

    /* Free our local copy of the binding */
    rpc_binding_free(&local_binding,&status);
    if (status != error_status_ok) 
        printf("***FAILURE***: Bad status (%s) from rpc_binding_free\n",error_text(status));

    /*
     * Make sure the full ptr passed in didn't get munged during the recursion.
     */
    if (numberp != in_numberp)
        fprintf(stderr, "Full pointer allocated in factorial() got munged!\n");
    return result;
}

globaldef factorial_v0_0_epv_t factorial_v0_0_m_epv = {factorial};
