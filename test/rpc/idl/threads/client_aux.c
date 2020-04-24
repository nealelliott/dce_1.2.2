/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.4.2  1996/02/17  23:17:57  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:59  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:31:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:19  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:39:53  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:46:09  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:28:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <math.h>
#include <threads.h>
#include <dce/cma_stdio.h>
#include <dce/rpcexc.h>
#include <dce/cmalib_crtlx.h>       /* C RTL wrapper routines (thread-safe) */
#include <test_common.h>

#define NUM_THREADS             10
#define MAX_CALLS_PER_THREAD    50

/*
 * Table of thread handles.
 */
static cma_t_thread th_handle[NUM_THREADS];
/*
 * Table of context handles, one per thread.
 */
static ch_t th_ctx[NUM_THREADS];

static handle_t H;


long thread_main
#ifdef NIDL_PROTOTYPES
(
    long    tid     /* Thread ID */
)
#else
(tid)
    long    tid;    /* Thread ID */
#endif

{
    long i, r, s, failures = 0;

TRY
    /*
     * Main routine for each thread of execution.
     */
#ifdef DEBUG
    fprintf(stderr, "Thread %d executing...\n", tid);
#endif

    /*
     * Call manager routine to establish a context on the server.
     */
#ifdef DEBUG
    fprintf(stderr, "open_counter tid %d\n", tid);
#endif
    open_counter(H, tid, &th_ctx[tid]);

    /*
     * Call manager routine repeatedly to modify the context (bump counter).
     */
    r = rand() % MAX_CALLS_PER_THREAD;

    for (i = 0 ; i < r ; i++)
    {
#ifdef DEBUG
        fprintf(stderr, "incr_counter tid %d\n", tid);
#endif
        incr_counter(th_ctx[tid]);
    }

    /*
     * Call manager routine to close the context and return counter value.
     */
#ifdef DEBUG
    fprintf(stderr, "close_counter tid %d\n", tid);
#endif
    s = close_counter(&th_ctx[tid]);

    if (s != r)
    {
        fprintf(stderr, "Thread %d error: r = %d, s = %d\n", tid, r, s);
        failures++;
    }
CATCH_ALL
    fprintf(stderr, "Error in thread_main, thread #%d\n", tid);
    print_exception(THIS_CATCH);
    failures++;
ENDTRY

    return failures;
}


int client_aux
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    int passno
)
#else
(h, passno)
    handle_t h;
    int passno;
#endif

{
    long i, failures = 0;
    cma_t_exit_status   th_status;
    long                th_errors;

    /*
     * Main thread of execution.
     */
    H = h;
    th_init(h);

    /*
     * Initialize all the context handles to junk.
     */
    for (i = 0 ; i < NUM_THREADS ; i++)
        th_ctx[i] = (ch_t)0xAAAAAAAA;

    /*
     * Create a bunch of threads and start them executing.
     */
    for (i = 0 ; i < NUM_THREADS ; i++)
    {
        TRY
#ifdef DEBUG
            fprintf(stderr, "Creating thread %d\n", i);
#endif
            cma_thread_create(
                &th_handle[i],
                &cma_c_null,
                (cma_t_start_routine)thread_main,
                (cma_t_address)i);
        CATCH_ALL
            fprintf(stderr, "Error in cma_thread_create, thread #%d\n", i);
            print_exception(THIS_CATCH);
            failures++;
        ENDTRY
    }

    /*
     * Wait for all threads to complete execution.
     */
    for (i = 0 ; i < NUM_THREADS ; i++)
    {
        TRY
#ifdef DEBUG
            fprintf(stderr, "Joining thread %d\n", i);
#endif
            cma_thread_join(
                &th_handle[i],
                &th_status,
                (cma_t_address *)&th_errors);
        CATCH(cma_e_existence)
#ifdef DEBUG
            fprintf(stderr, "Info: Thread %d had already exited\n");
#endif
        CATCH_ALL
            fprintf(stderr, "Error in cma_thread_join, thread #%d\n", i);
            print_exception(THIS_CATCH);
            failures++;
        FINALLY
            failures += th_errors;
        ENDTRY

        /*
         * Make sure thread has cleaned up its context handle.
         */
        if (th_ctx[i] != NULL)
        {
            fprintf(stderr, "Thread %d exited with non-NULL context!\n", i);
            failures++;
        }
    }

    failures += th_cleanup(h);
    return failures;
}
