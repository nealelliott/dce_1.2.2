/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dumpDB.c,v $
 * Revision 1.1.6.2  1996/03/09  20:51:29  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:16  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:43:27  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/01  13:59 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  18:07  giza
 * 	merge kk & hpdce01
 * 	[1995/12/11  22:34:53  root]
 * 
 * Revision 1.1.2.2  1993/08/24  18:54:38  giza
 * 	Initial version.
 * 	[1993/08/24  18:54:13  giza]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <dce/cma.h>
#include <dce/rpcexc.h>
#include <dce/cmalib_crtlx.h>       /* C RTL wrapper routines (thread-safe) */
#include <dce/pthread_exc.h>
#include "pipetest.h"
#include "tet_code.h"


extern char *error_text();
extern void print_exception();

FILE *fp;
int Pass;

main(ac, av)
int ac;
char *av[];
{
    error_status_t st;
    rpc_binding_handle_t h = NULL;
    ndr_char *string_binding;
    static ndr_char nil_string[] = "";
    unsigned32 s_only = 0;
    

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
        fprintf(stderr, "Can't register interface - %s\n", error_text(st));
        fprintf(stderr, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }

    /* Set handle to something plausible so rpc_binding_dup succeeds. */
    h = (rpc_binding_handle_t)12345678;
#else

  /* If no parameters are specified, assume auto_handle for now */
  if (ac != 2) {
    if (ac < 4 ) {
        fprintf(stderr, "usage: %s protseq hostid endpoint\n", av[0]);
        fprintf(stderr, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }

    if ( strcmp(av[1],"-r") == 0 ) s_only = 1;

    rpc_string_binding_compose(NULL,        /* uuid */
                               (ndr_char *)av[1+s_only], /* protocol sequence */
                               (ndr_char *)av[2+s_only], /* netaddr */
                               (ndr_char *)av[3+s_only], /* endpoint */
                               NULL,        /* options */
                               &string_binding,
                               &st);
    if (st != error_status_ok) 
    {
        fprintf(stderr, "Can't compose string binding %s\n", 
                error_text(st));
        fprintf(stderr, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }

    rpc_binding_from_string_binding(string_binding, &h, &st);
    if (st != error_status_ok) 
    {
        fprintf(stderr, "Can't get binding from string binding %s - %s\n", 
                string_binding, error_text(st));
        fprintf(stderr, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }
  }

# if !defined(PRT) && defined(RT_DEBUG)
    rpc__dbg_set_switches("0-4.3", &st);
# endif
#endif

    dumpDB(h, s_only, &st);
    fprintf(stdout,"%d\n", st);
    
    CATCH_ALL
        print_exception(THIS_CATCH);
    ENDTRY
}


uuid_t Uuid; 
   handle_t s_h;

handle_t my_handle_bind(my_handle m)
{
    /* printf("Binding my_handle\n"); */
    return( s_h );
}

void my_handle_unbind(my_handle m, handle_t h)
{
	;
    /* printf("Unbinding my_handle\n"); */
}
