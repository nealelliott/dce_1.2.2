/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pipe_aux.c,v $
 * Revision 1.1.11.2  1996/03/09  20:51:11  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:02  marty]
 *
 * Revision 1.1.11.1  1995/12/13  21:55:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:12  root]
 * 
 * Revision 1.1.6.2  1993/07/09  12:46:18  root
 * 	Initial King Kong branch
 * 	[1993/07/09  12:45:52  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:28:21  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:55:22  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:02:42  mcfarland]
 * 
 * 	Reorganize util routines
 * 	[1992/04/15  15:59:39  rps]
 * 
 * Revision 1.1.1.2  1992/05/28  15:13:29  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:36:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <pipe.h>
#include <util.h>

typedef unsigned char byte;

typedef struct client_pipe_state_t {
    int element_count;
} client_pipe_state_t;

client_pipe_state_t client_ip_state = {0};
client_pipe_state_t client_iop_state = {0};
client_pipe_state_t client_op_state = {0};

#define CLIENT_PIPE_ELEMENT_COUNT 999                      
#define CLIENT_BUFFER_SIZE 2048
byte client_buffer[CLIENT_BUFFER_SIZE];

void client_alloc (state,bsize,buf,bcount)
    ndr_void_p_t state;
    unsigned long bsize;
    byte **buf;
    unsigned long *bcount;
{
    *buf = client_buffer;
    *bcount = CLIENT_BUFFER_SIZE;
}

void client_ip_pull (state,buf,esize,ecount)
    client_pipe_state_t * state;
    float *buf;
    unsigned long esize;
    unsigned long *ecount;
{
    int i;
    int current_count;
                      
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

void client_iop_pull (state,buf,esize,ecount)
    client_pipe_state_t * state;
    pt2_struct *buf;
    unsigned long esize;
    unsigned long *ecount;
{
    int i;
    int current_count;
                      
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
void client_iop_push (state,buf,ecount)
    client_pipe_state_t *state;
    pt2_struct *buf;
    unsigned long ecount;
{
    int i;
    pt2_struct *iop_ptr;
    unsigned long *p_as_word;
    ndr_long_float wk_double;
    char buffer[128];

    if (ecount == 0)
    {
        sprintf(buffer, "Client checksums for iop_struct: %d %.1f\n",
               iop_checksums.pt2_long, iop_checksums.pt2_double);
	xx_log(buffer);
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
void client_op_push (state,buf,ecount)
    client_pipe_state_t *state;
    float *buf;
    unsigned long ecount;
{
    int i;
    unsigned long *p_as_word;
    ndr_short_float wk_single;
    char buffer[128];

    if (ecount == 0)
    {
        sprintf(buffer, "Client checksum for op_float: %.1f\n", op_checksum);
	xx_log(buffer);
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

handle_t my_handle_bind( m )
my_handle m;
{
    printf("Binding my_handle\n");
    return( s_h );
}

void my_handle_unbind(m,h)
   my_handle m;
    handle_t h;
{
    printf("Unbinding my_handle\n");
}

int client_aux(h,passes)
rpc_binding_handle_t h;
int passes;
{
    error_status_t st;
    pipe_of_float op_float;
    pipe_of_struct iop_struct;
    pipe_of_float ip_float;
    ndr_boolean correct_to_server;
    int failures = 0;

    s_h = h;

    ip_float.pull = client_ip_pull;
    ip_float.alloc = client_alloc;
    ip_float.state = (rpc_ss_pipe_state_t)&client_ip_state;
    iop_struct.pull = client_iop_pull;
    iop_struct.push = client_iop_push;
    iop_struct.alloc = client_alloc;
    iop_struct.state = (rpc_ss_pipe_state_t)&client_iop_state;
    op_float.push = client_op_push;
    op_float.alloc = client_alloc;
    op_float.state = (rpc_ss_pipe_state_t)&client_op_state;
                                     
    client_ip_state.element_count = 0;
    client_iop_state.element_count = 0;
    client_op_state.element_count = 0;
    iop_checksums.pt2_long = 0;
    iop_checksums.pt2_double = 0.0;
    op_checksum = 0.0;

   my_oper2 ( h, &op_float, &iop_struct, &ip_float, &correct_to_server); 

    if ( !correct_to_server ) failures = 1;
    if ( iop_checksums.pt2_long != 2497500) failures = 1;                
    if ( iop_checksums.pt2_double != 2997000.0 ) failures = 1;
    if ( op_checksum != 1998000.0 ) failures = 1;

    return(failures);
}
