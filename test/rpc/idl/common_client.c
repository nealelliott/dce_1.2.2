/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: common_client.c,v $
 * Revision 1.1.4.2  1996/02/17  23:13:03  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:04  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:59:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:09  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:10:35  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:29:04  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:34:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <dce/cma.h>
#include <dce/rpcexc.h>
#include <dce/cmalib_crtlx.h>       /* C RTL wrapper routines (thread-safe) */

#define BINDING_LEN 256
extern char *error_text();
extern void print_exception();

int Pass;
extern int client_aux();

main(ac, av)
int ac;
char *av[];
{
    error_status_t st;
    rpc_binding_handle_t h = NULL;
    ndr_char *string_binding;
    static ndr_char nil_string[] = "";
    int pass, passes=10, failures = 0;


    TRY
#ifdef PRT
    {
    globalref make_test_id(_v0_0_epv_t);
#ifndef NO_M_EPV
    globalref make_test_id(_v0_0_m_epv);
#endif
    cma_init();
    rpc_server_register_if(
                    make_test_id(_v0_0_s_ifspec), 
                    NULL,
#ifdef NO_M_EPV
		    NULL,
#else
                    (rpc_mgr_epv_t)&make_test_id(_v0_0_m_epv), 
#endif
                    &st);
    }

    if (st != error_status_ok) 
    {
        printf("Can't register interface - %s\n", error_text(st));
        exit(2);
    }
    if (ac >= 2) passes = atoi(av[1]);
    if ((passes < 1) || (passes > 100)) passes = 10;

    /* Set handle to something plausible so rpc_binding_dup succeeds. */
    h = (rpc_binding_handle_t)12345678;
#else

  /* If no parameters are specified, assume auto_handle for now */
  if (ac != 1)
  {

    if (ac != 5) 
    {
        fprintf(stderr, "usage: %s protseq hostid endpoint passes\n", av[0]);
        exit(2);
    }

    passes = atoi(av[4]);

    rpc_string_binding_compose(NULL,        /* uuid */
                               (ndr_char *)av[1], /* protocol sequence */
                               (ndr_char *)av[2], /* netaddr */
                               (ndr_char *)av[3],
                               NULL,        /* options */
                               &string_binding,
                               &st);
    if (st != error_status_ok) 
    {
        fprintf(stderr, "Can't compose string binding %s\n", 
                error_text(st));
        exit(2);
    }

    rpc_binding_from_string_binding(string_binding, &h, &st);
    if (st != error_status_ok) 
    {
        fprintf(stderr, "Can't get binding from string binding %s - %s\n", 
                string_binding, error_text(st));
        exit(2);
    }
  }

# if !defined(PRT) && defined(RT_DEBUG)
    rpc__dbg_set_switches("0-4.3", &st);
# endif
#endif

    for (pass = 0; pass < passes; pass++)
    {
        Pass = pass;
        printf("PASS %d\n", pass);
        failures += client_aux(h, passes);
    }

    printf("Summary: %d passes, %d failures\n", passes, failures);

    CATCH_ALL
      print_exception(THIS_CATCH);
    ENDTRY

    exit ( failures ? 1 : 0 );
}

