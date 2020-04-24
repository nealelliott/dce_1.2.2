/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client.c,v $
 * Revision 1.1.4.2  1996/02/17  23:12:47  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:50  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:57:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:46  root]
 * 
 * Revision 1.1.2.1  1994/05/02  22:43:45  tom
 * 	Initial checkin.
 * 	[1994/05/02  21:06:39  tom]
 * 
 * $EndLog$
 */

#include <test_common.h>
#include "bindcall.h"

extern bindcall_v0_0_epv_t bindcall_v0_0_manager_epv;

void my_bh_callout
(
  rpc_binding_handle_t *p_binding,
  rpc_if_handle_t interface_handle,
  error_status_t *p_st
)
{
    printf("in [binding_callout] routine my_bh_callout()\n");
#ifdef PRT
    /* Force auto-binding failure for a PRT test */
    *p_st = rpc_s_no_more_bindings;
#else
    rpc_ss_bind_authn_client(p_binding, interface_handle, p_st);
#endif
}

main(int argc, char *argv[])
{
    int pass, passes=10, failures=0;
    error_status_t st;
    idl_long_int a=7, b=4, c;

    if (argc >= 2) passes = atoi(argv[1]);

    for (pass = 0; pass < passes; pass++)
    {
        printf("PASS %d\n", pass);
#ifdef __osf__
        fflush(stdout);
#endif
#ifdef PRT
        /* For the pseudo-runtime, we expect a "no more bindings" exception */
        TRY
            bc_add(a, b, &c);
            failures++;
        CATCH(rpc_x_no_more_bindings)
            /* Expected case */
        CATCH_ALL
            failures++;
        ENDTRY
#else
        bc_add(a, b, &c);
        if (c != (a+b)) failures++;
#endif
    }

    printf("Summary: %d passes, %d failures\n", passes, failures);
}
