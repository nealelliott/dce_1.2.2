/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.6.2  1996/02/17  23:16:03  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:34  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:27:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:39  root]
 * 
 * Revision 1.1.3.3  1993/01/07  20:39:11  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:38:40  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:21:57  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:47:07  weisman]
 * 
 * Revision 1.1  1992/01/19  03:26:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#ifdef VMS
#include <file.h>
#elif defined(SNI_SVR4)
#include <fcntl.h>
#else
#include <sys/file.h>
#endif
#include <pipefault.h>
#include <dce/rpc.h>
#include <dce/cma.h>
#include <dce/rpcexc.h>

typedef unsigned char byte;

typedef struct client_pipe_state_t {
    ndr_boolean file_open;
    int file_handle;
} client_pipe_state_t;

client_pipe_state_t client_in_pipe_state = {false, 0};
client_pipe_state_t client_out_pipe_state = {false, 0};

char *filename = "pipefault.idl";
static char *pipe_out = "pt.out";
static char *s;

void client_pull (rpc_ss_pipe_state_t state_arg,
		  byte *buf,
		  unsigned long esize,
		  unsigned long *ecount)
{
    client_pipe_state_t * state = state_arg;

    if ( ! state->file_open )
    {
	if (s = getenv("PIPEFAULT_FILE"))
	    filename=s;

        state->file_handle = open(filename,O_RDONLY);
        if (state->file_handle == -1)
        {
            printf("Client couldn't open %s\n", filename);
            exit(0);
        }
        state->file_open = true;
    }
    *ecount = read( state->file_handle, buf, esize );
    if (*ecount == 0)
    {
        close(state->file_handle);
    }
}

void client_push (rpc_ss_pipe_state_t state_arg,
		  byte *buf,
		  unsigned long ecount)
{
    client_pipe_state_t * state = state_arg;

    if ( ! state->file_open )
    {
	if (s = getenv("PIPEFAULT_PIPEOUT"))
	    filename=s;
        state->file_handle = open(pipe_out,
                                  O_CREAT | O_TRUNC | O_WRONLY, 0644 );
        if (state->file_handle == -1)
        {
            printf("Couldn't create %s\n", pipe_out);
            exit(0);
        }
        state->file_open = true;
    }
    if (ecount == 0)
    {
        close(state->file_handle);
    }
    else write( state->file_handle, buf, ecount );
}
                      
#define CLIENT_BUFFER_SIZE 2048
byte client_buffer[CLIENT_BUFFER_SIZE];

void client_alloc (rpc_ss_pipe_state_t state,
		   unsigned long bsize,
		   byte **buf,
		   unsigned long *bcount)
{
    *buf = client_buffer;
    *bcount = CLIENT_BUFFER_SIZE;
}


uuid_t Uuid;

handle_t my_handle_bind(my_handle m)
{
    printf("Binding my_handle\n");
    return( i_h );
}

void my_handle_unbind(my_handle m,  handle_t h)
{
    printf("Unbinding my_handle\n");
}

int client_aux(handle_t h, int passes)
{
    pipe_of_chars client_in_pipe;
    pipe_of_chars client_out_pipe;
    long outval;
    error_status_t st;
    my_handle m;
    char command[100];
    int failures = 0;
                   
    client_in_pipe.pull = client_pull;
    client_in_pipe.alloc = client_alloc;
    client_in_pipe.state = &client_in_pipe_state;
    client_out_pipe.push = client_push;
    client_out_pipe.alloc = client_alloc;
    client_out_pipe.state = &client_out_pipe_state;

    i_h = h;

    TRY
    {
        client_in_pipe_state.file_open = false;
        client_out_pipe_state.file_open = false;
        my_oper1 ( 'X', client_in_pipe, client_out_pipe, &outval );
        printf("***FAILURE***: Missing exception\n");
        failures++;
    }
    CATCH (rpc_x_ss_pipe_empty)
        printf("Pipe empty exception\n");
    CATCH_ALL
        printf("***FAILURE***: Unexpected exception\n");
        failures++;
        print_exception(THIS_CATCH);
    ENDTRY

    TRY
    {
        client_in_pipe_state.file_open = false;
        client_out_pipe_state.file_open = false;
	my_oper2 ( &i_h, 'X', client_in_pipe, client_out_pipe, &outval );
        printf("***FAILURE***: Missing exception\n");
        failures++;
    }
    CATCH (rpc_x_ss_pipe_order)
        printf("Pipe order exception\n");
    CATCH_ALL
        printf("***FAILURE***: Unexpected exception\n");
        failures++;
        print_exception(THIS_CATCH);
    ENDTRY

    TRY
    {
        client_in_pipe_state.file_open = false;
        client_out_pipe_state.file_open = false;
        my_oper3 ( m, 'X', client_in_pipe, client_out_pipe, &outval );
        printf("***FAILURE***: Missing exception\n");
        failures++;
    }
    CATCH (rpc_x_ss_pipe_closed)
        printf("Pipe closed exception\n");
    CATCH_ALL
        printf("***FAILURE***: Unexpected exception\n");
        failures++;
        print_exception(THIS_CATCH);
    ENDTRY
    TRY
    {
        client_in_pipe_state.file_open = false;
        client_out_pipe_state.file_open = false;
        my_oper4 ( &m, 'X', client_in_pipe, client_out_pipe, &outval );
        printf("***FAILURE***: Missing exception\n");
        failures++;
    }
    CATCH (rpc_x_ss_pipe_discipline_error)
        printf("Pipe discipline exception\n");
    CATCH_ALL
        printf("***FAILURE***: Unexpected exception\n");
        failures++;
        print_exception(THIS_CATCH);
    ENDTRY

    printf("Test Complete\n");
    return failures;
}
