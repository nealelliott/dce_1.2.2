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
 * Revision 1.1.6.2  1996/02/17  23:16:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:49  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:27:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:55  root]
 * 
 * Revision 1.1.4.1  1993/10/14  17:52:45  ganni
 * 	OT 7426 - malloc the buffers to avoid the alignment problems.
 * 	[1993/10/14  17:52:01  ganni]
 * 
 * Revision 1.1.2.2  1993/01/07  20:42:01  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:40:03  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:34:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <pipetest2_ool.h>
#include <test_common.h>

typedef unsigned char byte;

typedef struct client_pipe_state_t {
    int element_count;
} client_pipe_state_t;

client_pipe_state_t client_p1_state = {0};
client_pipe_state_t client_ip_state = {0};
client_pipe_state_t client_iop_state = {0};
client_pipe_state_t client_op_state = {0};

#define CLIENT_PIPE_ELEMENT_COUNT 999                      
#define CLIENT_BUFFER_SIZE 2048

void client_alloc_float (ndr_void_p_t state,
		   unsigned long bsize,
		   float **buf,
		   unsigned long *bcount)
{
     *buf = (float *)malloc(CLIENT_BUFFER_SIZE);
     *bcount = CLIENT_BUFFER_SIZE;
}

void client_alloc_p1 (ndr_void_p_t state,
		    unsigned long bsize,
		    p1_struct **buf,
		    unsigned long *bcount)
{
     *buf = (p1_struct *)malloc(CLIENT_BUFFER_SIZE);
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

void client_p1_pull (rpc_ss_pipe_state_t state_arg,
		     p1_struct *buf,
		     unsigned long esize,
		     unsigned long *ecount)
{
    int i;
    int current_count;
    client_pipe_state_t * state = state_arg;
                      
    current_count = state->element_count;
    if (current_count == CLIENT_PIPE_ELEMENT_COUNT)
    {
        *ecount = 0;
        return;
    }
        
    for (i=1; i<=esize; i++)
    {
        current_count++;
        buf[i-1].p1_long = 3 * current_count;
        buf[i-1].p1_d.d = (double)(4 * current_count);
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
                
p1_struct p1_checksums = { 0, 0.0 };
int p1_j = 1;
void client_p1_push (rpc_ss_pipe_state_t state_arg,
		     p1_struct *buf,
		     unsigned long ecount)
{
    int i;
    p1_struct *iop_ptr;
    unsigned long *p_as_word;
    ndr_long_float wk_double;
    client_pipe_state_t *state = state_arg;
    if (ecount == 0)
    {
        printf("Client checksums for p1_struct: %d %.1f\n",
               p1_checksums.p1_long, p1_checksums.p1_d.d);
        p1_j = 1;
    }
    else
    {
        for (i=0; i<ecount; i++)
        {
            if (p1_j*5 != buf[i].p1_long)
            {
                printf("j = %d, p1_long = %d\n", p1_j, buf[i].p1_long);
            }
            wk_double = p1_j * 6.0;
            if (wk_double != buf[i].p1_d.d)
            {
                printf("j = %d, wk_double = %.8f, pt2_double = %.8f\n", p1_j, 
                       wk_double, buf[i].p1_d.d);
                p_as_word = (unsigned long *)&buf[i].p1_d.d;
                printf("%08x %08x should be ", *p_as_word, *(p_as_word+1));
                p_as_word = (unsigned long *)&wk_double;
                printf("%08x %08x\n", *p_as_word, *(p_as_word+1));
            }
            p1_checksums.p1_long += buf[i].p1_long;
            p1_checksums.p1_d.d += buf[i].p1_d.d;
            p1_j++;
        }
    }
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
    client_pipe_state_t * state = state_arg;

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

uuid_t Uuid; 
   handle_t s_h;

handle_t my_handle_bind(my_handle m)
{
    printf("Binding my_handle\n");
    return( s_h );
}

void my_handle_unbind(my_handle m, handle_t h)
{
    printf("Unbinding my_handle\n");
}

int client_aux(rpc_binding_handle_t h, int passes)
{
    error_status_t st;
    pipe_of_p1 iop1;
    pipe_of_float op_float;
    pipe_of_struct iop_struct;
    pipe_of_float ip_float;
    ndr_boolean correct_to_server;
    int failures = 0;

    s_h = h;

    iop1.pull = client_p1_pull;
    iop1.push = client_p1_push;
    iop1.alloc = (void (*) (rpc_ss_pipe_state_t,
			    idl_ulong_int,
			    p1_struct **,
			    idl_ulong_int *)
		  ) client_alloc_p1;
    iop1.state = &client_p1_state;
    client_p1_state.element_count = 0;
    p1_checksums.p1_long = 0;
    p1_checksums.p1_d.d = 0.0;

    my_oper1 ( &iop1, &correct_to_server); 

    if ( !correct_to_server ) failures++;
    if ( p1_checksums.p1_long != 2497500) failures++;
    if ( p1_checksums.p1_d.d != 2997000.0 ) failures++;


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
    op_float.alloc = (void (*) (rpc_ss_pipe_state_t,
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

    if ( !correct_to_server ) failures++;
    if ( iop_checksums.pt2_long != 2497500) failures++;
    if ( iop_checksums.pt2_double != 2997000.0 ) failures++;
    if ( op_checksum != 1998000.0 ) failures++;

    return(failures);
}
