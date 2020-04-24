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
 * Revision 1.1.6.2  1996/02/17  23:16:19  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:46  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:27:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:51  root]
 * 
 * Revision 1.1.4.1  1993/10/14  17:52:40  ganni
 * 	OT 7426 - malloc the buffers to avoid the alignment problems.
 * 	[1993/10/14  17:51:55  ganni]
 * 
 * Revision 1.1.2.2  1993/01/07  20:41:19  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:39:45  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:33:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <test_common.h>
#include <pipetest2_cancel.h>

typedef unsigned char byte;

typedef struct client_pipe_state_t {
    int element_count;
} client_pipe_state_t;

client_pipe_state_t client_ip_state = {0};
client_pipe_state_t client_iop_state = {0};
client_pipe_state_t client_op_state = {0};

#define CLIENT_PIPE_ELEMENT_COUNT 999                      
#define CLIENT_BUFFER_SIZE 2048

static volatile long in_pipe_done = idl_false;
static volatile long thread_terminated = idl_false;

void client_alloc_float (ndr_void_p_t state,
		   unsigned long bsize,
		   float **buf,
		   unsigned long *bcount)
{
    *buf = (float *)malloc(CLIENT_BUFFER_SIZE);
    *bcount = CLIENT_BUFFER_SIZE;
}

void client_alloc_pt2 (ndr_void_p_t state,
		   unsigned long bsize,
		   pt2_struct **buf,
		   unsigned long *bcount)
{
    *buf = (pt2_struct *)malloc(CLIENT_BUFFER_SIZE);
    *bcount = CLIENT_BUFFER_SIZE;
}

void client_ip_pull (rpc_ss_pipe_state_t state_arg,
		     float *buf,
		     unsigned long esize,
		     unsigned long *ecount)
{
    int i;
    int current_count;
    client_pipe_state_t *state = state_arg;

    current_count = state->element_count;
    if (current_count == CLIENT_PIPE_ELEMENT_COUNT)
    {
        *ecount = 0;
        in_pipe_done = idl_true;
        return;
    }
        
    for (i=1; i<=esize; i++)
    {
        current_count++;
        buf[i-1] = (float)(2 * current_count);
        if (current_count == CLIENT_PIPE_ELEMENT_COUNT) 
        {
            state->element_count = current_count;
            *ecount = i;
            return;
        }
    }
    state->element_count = current_count;
    *ecount = esize;
}

void client_iop_pull (rpc_ss_pipe_state_t state_arg,
		      pt2_struct *buf,
		      unsigned long esize,
		      unsigned long *ecount)
{
    int i;
    int current_count;
    client_pipe_state_t *state = state_arg;

    current_count = state->element_count;
    if (current_count == CLIENT_PIPE_ELEMENT_COUNT)
    {
        *ecount = 0;
        return;
    }
        
    for (i=1; i<=esize; i++)
    {
        current_count++;
        buf[i-1].pt2_long = 3 * current_count;
        buf[i-1].pt2_double = (double)(4 * current_count);
        if (current_count == CLIENT_PIPE_ELEMENT_COUNT)
        {
            state->element_count = current_count;
            *ecount = i;
            return;
        }
    }
    state->element_count = current_count;
    *ecount = esize;
}
                
pt2_struct iop_checksums = { 0, 0.0 };
int iop_j = 1;
void client_iop_push (rpc_ss_pipe_state_t state_arg,
		      pt2_struct *buf,
		      unsigned long ecount)
{
    int i;
    pt2_struct *iop_ptr;
    unsigned long *p_as_word;
    ndr_long_float wk_double;
    client_pipe_state_t *state = state_arg;

    if (ecount == 0)
    {
        printf("Client checksums for iop_struct: %d %.1f\n",
               iop_checksums.pt2_long, iop_checksums.pt2_double);
        iop_j = 1;
    }
    else
    {
        for (i=0; i<ecount; i++)
        {
            if (iop_j*5 != buf[i].pt2_long)
            {
                printf("j = %d, pt2_long = %d\n", iop_j, buf[i].pt2_long);
            }
            wk_double = iop_j * 6.0;
            if (wk_double != buf[i].pt2_double)
            {
                printf("j = %d, wk_double = %.8f, pt2_double = %.8f\n", iop_j, 
                       wk_double, buf[i].pt2_double);
                p_as_word = (unsigned long *)&buf[i].pt2_double;
                printf("%08x %08x should be ", *p_as_word, *(p_as_word+1));
                p_as_word = (unsigned long *)&wk_double;
                printf("%08x %08x\n", *p_as_word, *(p_as_word+1));
            }
            iop_checksums.pt2_long += buf[i].pt2_long;
            iop_checksums.pt2_double += buf[i].pt2_double;
            iop_j++;
        }
    }
}
                
float op_checksum =  0.0 ;
int op_j = 1;
void client_op_push (rpc_ss_pipe_state_t state_arg,
		     float *buf,
		     unsigned long ecount)
{
    int i;
    unsigned long *p_as_word;
    ndr_short_float wk_single;
    client_pipe_state_t *state = state_arg;

    if (ecount == 0)
    {
        printf("Client checksum for op_float: %.1f\n", op_checksum);
        op_j = 1;
    }
    else
    {
        for (i=0; i<ecount; i++)
        {
            wk_single = op_j * 4.0;
            if (wk_single != buf[i])
            {
                printf("j = %d, float = %.8f\n", op_j, buf[i]);
                p_as_word = (unsigned long *)&buf[i];
                printf("%08x should be ",*p_as_word);
                p_as_word = (unsigned long *)&wk_single;
                printf("%08x\n",*p_as_word);
            }
            op_checksum += buf[i];
            op_j++;
        }
    }
}

long thread_main
#ifdef IDL_PROTOTYPES
(
    long    tid     /* Thread ID */
)
#else
(tid)
    long    tid;    /* Thread ID */
#endif
{
    volatile error_status_t st = 1;
    pipe_of_float op_float;
    pipe_of_struct iop_struct;
    pipe_of_float ip_float;
    ndr_boolean correct_to_server;
    int failures = 0;

    TRY
    ip_float.pull = client_ip_pull;
    ip_float.alloc = (void (*) (rpc_ss_pipe_state_t,
				idl_ulong_int,
				idl_short_float **,
				idl_ulong_int *)
		      ) client_alloc_float;
    ip_float.state = &client_ip_state;
    iop_struct.pull = client_iop_pull;
    iop_struct.push = client_iop_push;
    iop_struct.alloc = (void (*) (rpc_ss_pipe_state_t,
				idl_ulong_int,
				pt2_struct **,
				idl_ulong_int *)
		      ) client_alloc_pt2;
    iop_struct.state = &client_iop_state;
    op_float.push = client_op_push;
    op_float.alloc = (void (*)(rpc_ss_pipe_state_t,
			       idl_ulong_int,
			       idl_short_float **,
			       idl_ulong_int *)
		      ) client_alloc_float;
    op_float.state = &client_op_state;
                                     
    client_ip_state.element_count = 0;
    client_iop_state.element_count = 0;
    client_op_state.element_count = 0;
    iop_checksums.pt2_long = 0;
    iop_checksums.pt2_double = 0.0;
    op_checksum = 0.0;

    my_oper2 ( &op_float, &iop_struct, &ip_float, &correct_to_server); 

    if ( !correct_to_server ) failures = 1;
    if ( iop_checksums.pt2_long != 2497500) failures = 1;                
    if ( iop_checksums.pt2_double != 2997000.0 ) failures = 1;
    if ( op_checksum != 1998000.0 ) failures = 1;

    st = 0;
    CATCH(cma_e_alerted)
        if (st == 0) printf("***ERROR***: Thread not terminated via cancel\n");
    CATCH_ALL
        printf("***ERROR***: Exception caught by client thread\n");
        print_exception(THIS_CATCH);
    ENDTRY
#ifdef DEBUG
    printf("Client thread exiting...\n");
#endif
    thread_terminated = idl_true;
    return(failures);
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
    cma_t_thread        th_handle;

    /*
     * Main thread of execution.
     */
    i_h = h;

    /*
     * Start a thread to perform the pipe
     */
    TRY
#ifdef DEBUG
        printf("Creating thread %d\n", i);
#endif
        cma_thread_create(
            &th_handle,
            &cma_c_null,
            (cma_t_start_routine)thread_main,
            (cma_t_address)i);
    CATCH_ALL
        printf("Error doing cma_thread_create, thread #%d\n", i);
        print_exception(THIS_CATCH);
        failures++;
    ENDTRY

    /* Wait until pipe is transmitted */
    for (i = 0; ((i < 1000) && !in_pipe_done) ; i++)
         cma_delay(1.0);

#ifdef DEBUG
    printf("Client Cancelling thread %d...\n", i);
#endif

    cma_delay(5.0);

    /*
     * Now thy to cancel the pipe while it is executing on the server.
     */
    TRY
        /* Should be in transmit, try to cancel it */
        cma_thread_alert(&th_handle);

        printf("Waiting for thread to get cancel...\n");

        for (i = 0; ((i < 1000) && !thread_terminated) ; i++)
            cma_delay(10.0);

        if (thread_terminated)
            printf("Thread cancelled as expected\n");
        else 
            printf("***FAILURE***: Thread not cancelled as expected\n");

    CATCH_ALL
        printf("***Error***: trying to cancel client thread!\n");
        print_exception(THIS_CATCH);
        failures++;
    ENDTRY

    return failures;
}
