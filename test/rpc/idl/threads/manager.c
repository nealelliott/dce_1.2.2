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
 * Revision 1.1.4.2  1996/02/17  23:17:58  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:00  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:31:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:20  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:40:00  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:46:13  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:28:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <threads.h>
#include <dce/cma_stdio.h>
#include <dce/cma.h>
#include <dce/cmalib_crtlx.h>       /* C RTL wrapper routines (thread-safe) */
#include <test_common.h>

#define SPIN_CYCLE 1000000      /* #times thru spin loop to get concurrency */

/* Assume only one client at a time. */
static int concurrency_count = 0;
static int concurrency_max   = 0;
static cma_t_mutex  mutex;

typedef struct {
    long    tid;        /* Thread ID */
    long    counter;    /* Counter */
} counter_ctx_t;

void _th_init
#ifdef NIDL_PROTOTYPES
(
    handle_t        h           /* [in] Binding handle */
)
#else
(h)
    handle_t        h;          /* [in] Binding handle */
#endif

{
    TRY
        cma_mutex_create(&mutex, &cma_c_null);
    CATCH_ALL
        fprintf(stderr, "Error in _init() during cma_mutex_create\n");
        print_exception(THIS_CATCH);
    ENDTRY
}


ndr_long_int _th_cleanup
#ifdef NIDL_PROTOTYPES
(
    handle_t        h           /* [in] Binding handle */
)
#else
(h)
    handle_t        h;          /* [in] Binding handle */
#endif

{
    long failures = 0;

    TRY
        cma_mutex_delete(&mutex);
    CATCH_ALL
        fprintf(stderr, "Error in _cleanup() during cma_mutex_delete\n");
        print_exception(THIS_CATCH);
    ENDTRY

    printf("Max concurrency count: %d\n", concurrency_max);
    if (concurrency_max < 1)
    {
        fprintf(stderr, "Error: max concurrency less than one!\n");
        failures++;
    }
    if (concurrency_count != 0)
    {
        fprintf(stderr, "Error: concurrency indicator finished nonzero!\n");
        failures++;
    }

    return failures;
}


void _open_counter
#ifdef NIDL_PROTOTYPES
(
    handle_t        h,          /* [in] Binding handle */
    ndr_long_int    tid,        /* [in] Thread ID */
    ch_t            *th_ctx     /*[out] Thread context handle on counter data */
)
#else
(h, tid, th_ctx)
    handle_t        h;          /* [in] Binding handle */
    ndr_long_int    tid;        /* [in] Thread ID */
    ch_t            *th_ctx;    /*[out] Thread context handle on counter data */
#endif

{
    counter_ctx_t *ct_ctx;

#ifdef DEBUG
    fprintf(stderr, "_open_counter tid %d\n", tid);
#endif

    ct_ctx = (counter_ctx_t *)malloc(sizeof(counter_ctx_t));

    ct_ctx->tid = tid;
    ct_ctx->counter = 0;

    *th_ctx = (ch_t)ct_ctx;
}


void _incr_counter
#ifdef NIDL_PROTOTYPES
(
    ch_t            th_ctx      /* [in] Counter context handle */
)
#else
(th_ctx)
    ch_t            th_ctx;     /* [in] Counter context handle */
#endif

{
    counter_ctx_t *ct_ctx;
    int i;

    TRY
        cma_mutex_lock(&mutex);
        if (++concurrency_count > concurrency_max)
            concurrency_max = concurrency_count;
        cma_mutex_unlock(&mutex);
    CATCH_ALL
        fprintf(stderr, "Error in _incr_counter() critical section 1\n");
        print_exception(THIS_CATCH);
        return;
    ENDTRY

    ct_ctx = (counter_ctx_t *)th_ctx;
#ifdef DEBUG
    fprintf(stderr, "_incr_counter tid %d\n", ct_ctx->tid);
#endif

    ct_ctx->counter++;

    /* Spin for awhile to help achieve concurrency. */

#ifdef DEBUG
    fprintf(stderr, "tid %d: begin spin cycle\n", ct_ctx->tid);
#endif
    for (i = 0 ; i < SPIN_CYCLE ; i++)
        ;
#ifdef DEBUG
    fprintf(stderr, "tid %d:   end spin cycle\n", ct_ctx->tid);
#endif

    TRY
        cma_mutex_lock(&mutex);
        concurrency_count--;
        cma_mutex_unlock(&mutex);
    CATCH_ALL
        fprintf(stderr, "Error in _incr_counter() critical section 2\n");
        print_exception(THIS_CATCH);
    ENDTRY
}


ndr_long_int _close_counter
#ifdef NIDL_PROTOTYPES
(
    ch_t            *th_ctx     /* [io] Counter context handle */
)
#else
(th_ctx)
    ch_t            *th_ctx;    /* [io] Counter context handle */
#endif

{
    long s;
    counter_ctx_t *ct_ctx;

    ct_ctx = (counter_ctx_t *)*th_ctx;
#ifdef DEBUG
    fprintf(stderr, "_close_counter tid %d\n", ct_ctx->tid);
#endif

    s = ct_ctx->counter;
    free(ct_ctx);

    *th_ctx = NULL;
    return s;
}


void ch_t_rundown
#ifdef NIDL_PROTOTYPES
(
    ch_t            th_ctx      /* [in] Counter context handle */
)
#else
(th_ctx)
    ch_t            th_ctx;     /* [in] Counter context handle */
#endif

{
    if (th_ctx != NULL)
        free(th_ctx);
}


globaldef threads_v0_0_epv_t threads_v0_0_m_epv = {
    _th_init,
    _th_cleanup,
    _open_counter,
    _incr_counter,
    _close_counter
};
