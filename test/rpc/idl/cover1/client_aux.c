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
 * Revision 1.1.434.2  1996/02/17  23:13:21  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:14  marty]
 *
 * Revision 1.1.434.1  1995/12/11  20:00:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:40  root]
 * 
 * Revision 1.1.431.1  1994/01/21  22:40:14  cbrooks
 * 	RPC Code Cleanup-Initial Submission
 * 	[1994/01/21  20:15:49  cbrooks]
 * 
 * Revision 1.1.3.3  1993/01/07  19:50:12  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:30:31  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:20:58  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:46:20  weisman]
 * 
 * Revision 1.1  1992/01/19  03:31:42  devrcs
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
#include <cover1.h>
#define STUBBASE_H
#include <errno.h>
#include <dce/cma.h>
#include <dce/rpc.h>
#include <test_common.h>

#ifdef apollo
#include <signal.h>
#define SYS_CALL(result, function) \
    do \
        result = function; \
    while (result == SIGVTALRM)
#else
#define SYS_CALL(result, function) \
    do \
        result = function; \
    while (result == -1  &&  errno == EINTR)
#endif

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

typedef unsigned char byte;

typedef struct client_pipe_state_t {
    boolean32 file_open;
    int file_handle;
} client_pipe_state_t;

client_pipe_state_t client_in_pipe_state = {false, 0};
client_pipe_state_t client_out_pipe_state = {false, 0};

static char *in_filename = "cover1.h";
static char *out_filename ="cover1.out";
static char *s;

void client_pull
#ifdef _DCE_PROTOTYPE_ 
(
#ifdef SNI_SVR4
    client_pipe_state_t * state,
#else
    rpc_ss_pipe_state_t state_arg,
#endif
    byte *buf,
    unsigned long esize,
    unsigned long *ecount
)
#else
(state, buf, esize, ecount)
#ifdef SNI_SVR4
    client_pipe_state_t * state;
#else
    rpc_ss_pipe_state_t state_arg;
#endif
    byte *buf;
    unsigned long esize;
    unsigned long *ecount;
#endif

{
#ifndef SNI_SVR4
    client_pipe_state_t *state = state_arg;
#endif

    if ( ! state->file_open )
    {
	if (s = getenv("COVER1_C_INFILE"))
		in_filename = s;

        state->file_handle = open(in_filename,O_RDONLY);
        if (state->file_handle == -1)
        {
            printf("Client couldn't open %s\n", in_filename);
            exit(0);
        }
        state->file_open = true;
    }
    *ecount = read( state->file_handle, buf, esize );
    if (*ecount == 0)
    {
        close(state->file_handle);
        state->file_open = false;
        state->file_handle = 0;
    }
}

void client_push
#ifdef _DCE_PROTOTYPE_ 
(
#ifdef SNI_SVR4
    client_pipe_state_t * state,
#else
    rpc_ss_pipe_state_t state_arg,
#endif
    byte *buf,
    unsigned long ecount
)
#else
(state, buf, ecount)
#ifdef SNI_SVR4
    client_pipe_state_t * state;
#else
    rpc_ss_pipe_state_t state_arg;
#endif
    byte *buf;
    unsigned long ecount;
#endif

{
#ifndef SNI_SVR4
    client_pipe_state_t *state = state_arg;
#endif

    if ( ! state->file_open )
    {
	if (s = getenv("COVER1_C_OUTFILE"))
		out_filename = s;


#ifdef VMS
        delete(out_filename);
#endif
        state->file_handle = open(out_filename,
                                  O_CREAT | O_TRUNC | O_WRONLY, 0x1FF );
        if (state->file_handle == -1)
        {
            printf("Couldn't create %s\n", out_filename);
            exit(0);
        }
        state->file_open = true;
    }
    if (ecount == 0)
    {
        close(state->file_handle);
        state->file_open = false;
        state->file_handle = 0;
    }
    else write( state->file_handle, buf, ecount );
}
                      
#define CLIENT_BUFFER_SIZE 2048
byte client_buffer[CLIENT_BUFFER_SIZE];

void client_alloc
#ifdef _DCE_PROTOTYPE_
(
    ndr_void_p_t state,
    unsigned long bsize,
    byte **buf,
    unsigned long *bcount
)
#else
(state, bsize, buf, bcount)
    ndr_void_p_t state;
    unsigned long bsize;
    byte **buf;
    unsigned long *bcount;
#endif

{
    *buf = client_buffer;
    *bcount = CLIENT_BUFFER_SIZE;
}


#ifdef VMS
#define SUCCESS_STATUS(x) (((x) & 1) == 1)  /* LBS */
#else
#define SUCCESS_STATUS(x) ((x) == 0)
#endif

int client_aux(handle_t h)
{
    pipe_of_chars client_in_pipe;
    pipe_of_chars client_out_pipe;
    error_status_t st;
   char command[100];
    tc_link_t *p_in_tc_link, *p_new_tc_link, *p_out_tc_link;
    int i;
    int status;
    char result[100];
    dtb_param_struct dtb;

    client_in_pipe.pull = client_pull;
    client_in_pipe.alloc = client_alloc;
#ifdef SNI_SVR4
    client_in_pipe.state = (rpc_ss_pipe_state_t)&client_in_pipe_state;
#else
    client_in_pipe.state = &client_in_pipe_state;
#endif
    client_out_pipe.push = client_push;
    client_out_pipe.alloc = client_alloc;
#ifdef SNI_SVR4
    client_out_pipe.state = (rpc_ss_pipe_state_t)&client_out_pipe_state;
#else
    client_out_pipe.state = &client_out_pipe_state;
#endif

    in_pipe_op(h, client_in_pipe);
    out_pipe_op(h, client_out_pipe);
   
   if ( ! file_diff(out_filename,in_filename))
    {
        return(1);
    }
    p_in_tc_link = NULL;
    for (i=0; i<26; i++)
    {
        p_new_tc_link = malloc(sizeof(tc_link_t));
        p_new_tc_link->p_link = p_in_tc_link;
        p_new_tc_link->p_tc = malloc(sizeof(twochars_t));
        p_new_tc_link->p_tc->upper = 'A' + i;
        p_new_tc_link->p_tc->lower = 'a' + i;
        p_in_tc_link = p_new_tc_link;
    }
    sp_pa_op( h, p_in_tc_link, &p_out_tc_link );
    result[0] = '\0';
    for ( ; 
        p_out_tc_link != NULL;
        p_out_tc_link = p_out_tc_link->p_link)
    {
        sprintf( &result[strlen(result)], "%c%c", p_out_tc_link->p_tc->upper,
                        p_out_tc_link->p_tc->lower );
    }
    if ( strcmp(result, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz")
                            != 0 ) return(1);

    dtb_op( h, &dtb, &st );
    if ( dtb.count != DTB_ELT_COUNT )
    {
        printf("Error: dtb.count = %d\n", dtb.count);
        return(1);
    }
    for (i=0; i<DTB_ELT_COUNT; i++)
    {
        if (dtb.elts[i].a != i * i)
        {
            printf("Error: dtb.elts[%d].a = %d\n", i, dtb.elts[i].a);
            return(1);
        }
        if (dtb.elts[i].b != i * i * i)
        {
            printf("Error: dtb.elts[%d].b = %d\n", i, dtb.elts[i].b);
            return(1);
        }
    }
    if (st != 0x12345678)
    {
        printf("Error: st = %08lx\n", st);
        return(1);
    }

    return(0);
}
