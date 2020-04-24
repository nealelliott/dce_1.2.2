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
 * Revision 1.1.6.2  1996/02/17  23:16:11  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:40  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:27:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:46  root]
 * 
 * Revision 1.1.3.4  1993/01/13  21:49:55  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:39:13  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:22:08  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:47:18  weisman]
 * 
 * Revision 1.1  1992/01/19  03:26:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#ifdef VMS
#include <file.h>
#elif defined (SNI_SVR4)
#include <fcntl.h>
#else
#include <sys/file.h>
#endif
#include <pipetest.h>
#include <test_common.h>

typedef unsigned char byte;

typedef struct client_pipe_state_t {
    ndr_boolean file_open;
    int file_handle;
} client_pipe_state_t;

client_pipe_state_t client_in_pipe_state = {false, 0};
client_pipe_state_t client_out_pipe_state = {false, 0};
char *filename = "pipetest.h";
char *pipe_in = "pt.in";
char *pipe_out = "pt.out";
char *s;

int file_diff(char *a, char *b)
{
      FILE *af = fopen(a,"r");
      FILE *bf = fopen(b,"r");
      char ab[1000],bb[1000];
      int num = 0;
      int status = 1;

      while (1)
      {
          char *aval,*bval;          
          num++;
          aval = fgets(ab,1000,af);
          bval = fgets(bb,1000,bf);
          if ((aval == 0) && (bval==0)) break;
          if ((aval == 0) && (bval!=0)) {
              fprintf(stderr,"Files %s and (eof) %s differ in size\n",a,b);
              status = 0;
	      break;
              }
          if ((aval != 0) && (bval==0)) {
              fprintf(stderr,"Files (eof) %s and %s differ in size\n",a,b);
              status = 0;
	      break;
              }


          if (strcmp(ab,bb) != 0) {
              fprintf(stderr,"file_diff: %s <> %s @ %d\n%s:%s\n%s:%s\n",
                  a,b,num,a,ab,b,bb);
              status = 0;
	      break;
              }
      }

      fclose(af);
      fclose(bf);
      return status;
}


void client_pull (rpc_ss_pipe_state_t state_arg,
		  byte *buf,
		  unsigned long esize,
		  unsigned long *ecount)
{
    client_pipe_state_t * state = state_arg;

    if ( ! state->file_open )
    {
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
        state->file_open = false;
    }
}

void client_push (rpc_ss_pipe_state_t state_arg,
		  byte *buf,
		  unsigned long ecount)
{
    client_pipe_state_t * state = state_arg;

    if ( ! state->file_open )
    {
        state->file_handle = open(pipe_out, O_CREAT | O_TRUNC | O_WRONLY, 0777);
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
        state->file_open = false;
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

int client_aux(handle_t h, int passes)
{
    int failures = 0;
    pipe_of_chars client_in_pipe;
    pipe_of_chars client_out_pipe;
    long outval;
    error_status_t st;

    if (s = getenv("PIPETEST_FILENAME"))
	    filename = s;
    if (s = getenv("PIPETEST_PIPEIN"))
	    pipe_in = s;
    if (s = getenv("PIPETEST_PIPEOUT"))
	    pipe_out = s;

    client_in_pipe.pull = client_pull ;
    client_in_pipe.alloc = client_alloc;
    client_in_pipe.state = &client_in_pipe_state;
    client_out_pipe.push = client_push;
    client_out_pipe.alloc = client_alloc;
    client_out_pipe.state = &client_out_pipe_state;

   my_oper2 ( h, 'X', client_in_pipe, client_out_pipe, &outval );
   if (outval != 1024)
      printf("***FAILURE***: Returned to client with value %d, expecte 1024\n", outval);
                     
   if (!file_diff(filename,pipe_in)) failures++;
   if (!file_diff(filename,pipe_out)) failures++;
#ifdef VMS
   delete(pipe_in);
   delete(pipe_out);
#endif

   return failures;
}


