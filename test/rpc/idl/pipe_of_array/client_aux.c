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
 * Revision 1.1.4.2  1996/02/17  23:16:00  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:31  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:34  root]
 * 
 * Revision 1.1.2.3  1993/01/13  21:49:45  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:38:23  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:32:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <pipe_of_array.h>

typedef unsigned char byte;

typedef struct client_pipe_state_t {
    int element_count;
} client_pipe_state_t;

#define CLIENT_PIPE_ELEMENT_COUNT 100
#define CLIENT_BUFFER_SIZE 2048
byte client_buffer[CLIENT_BUFFER_SIZE];

void client_alloc (
    rpc_ss_pipe_state_t state,
    unsigned long bsize,
    array_t **buf,
    unsigned long *bcount
)
{
    *buf = (array_t *)client_buffer;
    *bcount = CLIENT_BUFFER_SIZE;
}

long checksum;

client_pipe_state_t client_in_state = {0};
client_pipe_state_t client_out_state = {0};

void client_pull (
    rpc_ss_pipe_state_t state_arg,
    array_t *buf,
    unsigned long esize,
    unsigned long *ecount
)
{
    int i,j;
    int current_count;
      
    client_pipe_state_t *state = (client_pipe_state_t *)state_arg;
    current_count = state->element_count;
    if (current_count == CLIENT_PIPE_ELEMENT_COUNT)
    {
        *ecount = 0;
        return;
    }
        
    for (i=1; i<=esize; i++)
    {
        current_count++;
        for (j=0; j<ARRAYSIZE/2; j++)
            buf[i-1][j] = current_count * 100 - j;
        for (j=ARRAYSIZE/2; j<ARRAYSIZE; j++)
            buf[i-1][j] = current_count * 100 + j - ARRAYSIZE / 2;
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
                
void client_push (
    rpc_ss_pipe_state_t state_arg,
    array_t *buf,
    unsigned long ecount
)
{
    int i,j;
    client_pipe_state_t *state = (client_pipe_state_t *)state_arg;
    if (ecount == 0)
    {
        printf("Client checksum %d\n", checksum);
    }
    else
    {
        for (i=0; i<ecount; i++)
        {
            for (j=0; j<ARRAYSIZE; j++)
                checksum += buf[i][j];
        }
    }
}


/* Routines for pipe2_t */

client_pipe_state_t client_in_p2_state = {0};
client_pipe_state_t client_out_p2_state = {0};

void client_p2_pull (
    rpc_ss_pipe_state_t state_arg,
    il_a_t *buf,
    unsigned long esize,
    unsigned long *ecount
)
{
    int i,j,k;
    int current_count;
    client_pipe_state_t *state = (client_pipe_state_t *)state_arg;
                      
    current_count = state->element_count;
    if (current_count == CLIENT_PIPE_ELEMENT_COUNT)
    {
        *ecount = 0;
        return;
    }
        
    for (i=1; i<=esize; i++)
    {
        current_count++;
        for (j=0; j<ILASIZE/2; j++)
        {
            for (k=0; k<OOLASIZE; k++)
                buf[i-1][j][k] = current_count * 100 - j * OOLASIZE - k;
        }
        for (j=0; j<ILASIZE/2; j++)
        {
            for (k=0; k<OOLASIZE; k++)
                buf[i-1][j+ILASIZE/2][k] = current_count * 100 
                                                     + j * OOLASIZE + k;
        }
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
                
void client_p2_push (
    rpc_ss_pipe_state_t state_arg,
    il_a_t *buf,
    unsigned long ecount
)
{
    int i,j,k;
    client_pipe_state_t *state = (client_pipe_state_t *)state_arg;

    if (ecount == 0)
    {
        printf("Client checksum %d\n", checksum);
    }
    else
    {
        for (i=0; i<ecount; i++)
        {
            for (j=0; j<ILASIZE; j++)
                for(k=0; k<OOLASIZE; k++)
                    checksum += buf[i][j][k];
        }
    }
}

int client_aux(h,passes)
rpc_binding_handle_t h;
int passes;
{
    error_status_t st;
    pipe_t in_pipe;
    pipe_t out_pipe;
    pipe2_t in_p2_pipe;
    pipe2_t out_p2_pipe;
    int failures = 0;
    ndr_long_int manager_checksum;

    in_pipe.pull = client_pull;
    in_pipe.alloc = client_alloc;
    in_pipe.state = (rpc_ss_pipe_state_t)&client_in_state;
    out_pipe.push = client_push;
    out_pipe.alloc = client_alloc;
    out_pipe.state = (rpc_ss_pipe_state_t)&client_out_state;
                                     
    client_in_state.element_count = 0;
    client_out_state.element_count = 0;
    checksum = 0;

    op0 ( h, &in_pipe, &manager_checksum, &out_pipe );

    if ( manager_checksum != 10100000 )
    {
        printf("Bad manager checksum in op0\n");
        failures = 1;
    }
    if ( checksum != 10100000 )
    {
        printf("Bad client checksum in op0\n");
        failures = 1;
    }

    in_p2_pipe.pull = client_p2_pull;
    in_p2_pipe.alloc
	    = (void(*)(rpc_ss_pipe_state_t,idl_ulong_int,
		       il_a_t **,idl_ulong_int *)) client_alloc;
    in_p2_pipe.state = (rpc_ss_pipe_state_t)&client_in_p2_state;
    out_p2_pipe.push = client_p2_push;
    out_p2_pipe.alloc = in_p2_pipe.alloc;
    out_p2_pipe.state = (rpc_ss_pipe_state_t)&client_out_p2_state;
                                     
    client_in_p2_state.element_count = 0;
    client_out_p2_state.element_count = 0;
    checksum = 0;

    op1 ( h, &in_p2_pipe, &manager_checksum, &out_p2_pipe );

    if ( manager_checksum != 10100000 )
    {
        printf("Bad manager checksum in op0\n");
        failures = 1;
    }
    if ( checksum != 10100000 )
    {
        printf("Bad client checksum in op0\n");
        failures = 1;
    }


    return(failures);
}
