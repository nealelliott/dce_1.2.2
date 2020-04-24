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
 * Revision 1.1.9.2  1996/02/17  23:27:56  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:07  marty]
 *
 * Revision 1.1.9.1  1995/12/11  22:43:17  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  19:42 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  17:58  giza
 * 	merge kk & hpdce01
 * 	[1995/12/11  22:20:17  root]
 * 
 * Revision 1.1.4.2  1993/08/24  18:45:36  giza
 * 	Added global file pointer for error logging.
 * 	[1993/08/24  18:45:07  giza]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/04  22:10:28  cjd]
 * 
 * Revision 1.1.1.5  1993/05/04  22:21:52  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.5.2  1993/08/24  14:01:15  giza
 * 	Added error file logging.
 * 	[1993/08/23  20:10:39  giza]
 * 
 * Revision 1.1.1.5  1993/05/04  22:21:52  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.2.3  1993/02/05  15:21:46  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:42:12  cjd]
 * 
 * Revision 1.1.2.2  1992/09/25  19:22:02  eperkins
 * 	Initial version
 * 	[1992/09/25  19:21:42  eperkins]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <test_common.h>
#include <pipetest.h>
#include <sys/time.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>

#define GTM(tm) \
{ \
	struct timezone tz; \
 \
	gettimeofday(tm, &tz); \
}

typedef unsigned char byte;
typedef struct client_pipe_state_t {
    int element_count;
} client_pipe_state_t;

client_pipe_state_t client_iop_state = {0};

#define CLIENT_PIPE_ELEMENT_COUNT 999                      
#define CLIENT_BUFFER_SIZE 2048
byte client_buffer[CLIENT_BUFFER_SIZE];

extern FILE *fp;
extern void dce_error_inq_text();

void client_alloc (ndr_void_p_t state,
		   unsigned long bsize,
		   byte **buf,
		   unsigned long *bcount)
{
    *buf = client_buffer;
    *bcount = CLIENT_BUFFER_SIZE;
}


void client_iop_pull (rpc_ss_pipe_state_t state_arg,
		      pt_struct *buf,
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
        buf[i-1].pt_long = 3 * current_count;
        buf[i-1].pt_double = (double)(4 * current_count);
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
                
pt_struct iop_checksums = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0 };
int iop_j = 1;
void client_iop_push (rpc_ss_pipe_state_t state_arg,
		      pt_struct *buf,
		      unsigned long ecount)
{
    int i;
    ndr_long_float wk_double;
    client_pipe_state_t *state = state_arg;

    if (ecount == 0)
    {
        fprintf(fp, "Client checksums for iop_struct: %d %.1f\n",
               iop_checksums.pt_long, iop_checksums.pt_double);
        iop_j = 1;
    }
    else
    {
        for (i=0; i<ecount; i++)
        {
            if (iop_j*5 != buf[i].pt_long)
            {
                fprintf(fp, "j = %d, pt_long = %d\n", iop_j, buf[i].pt_long);
            }
            wk_double = iop_j * 6.0;
            if (wk_double != buf[i].pt_double)
            {
                fprintf(fp, "j = %d, wk_double = %.8f, pt_double = %.8f\n", iop_j, 
                       wk_double, buf[i].pt_double);
            }
            iop_checksums.pt_long += buf[i].pt_long;
            iop_checksums.pt_double += buf[i].pt_double;
            iop_j++;
        }
    }
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
struct msec_time
{
    unsigned long msec;
    unsigned short usec;
};


static void timing( start_time, avg_time )
struct timeval *start_time;
struct msec_time *avg_time;
{
        struct timeval elapsed_time;
        unsigned long elapsed_usec;

	GTM(&elapsed_time);
	if ( elapsed_time.tv_usec < start_time->tv_usec)
	{
  	         elapsed_time.tv_sec--;
       		 elapsed_time.tv_usec += 1000000;
        }
        elapsed_time.tv_usec -= start_time->tv_usec;
        elapsed_time.tv_sec  -= start_time->tv_sec;

        elapsed_usec = (elapsed_time.tv_sec * 1000000) + elapsed_time.tv_usec;
        avg_time->msec = (unsigned long) (elapsed_usec / 1000);
        avg_time->usec = (unsigned long) (elapsed_usec % 1000);

}




int client_aux(rpc_binding_handle_t h, int passes)
{
    error_status_t st;
    int dcest;
    pipe_of_struct iop_struct;
    ndr_boolean correct_to_server;
    int failures = 0;
    struct timeval start_time;
    struct msec_time avg_time;
    unsigned char *ebuff;

    s_h = h;
    ebuff = (unsigned char*)malloc(dce_c_error_string_len);

    iop_struct.pull = client_iop_pull;
    iop_struct.push = client_iop_push;
    iop_struct.alloc = (void (*) (rpc_ss_pipe_state_t,
				  idl_ulong_int,
				  pt_struct **,
				  idl_ulong_int *)
			) client_alloc;
    iop_struct.state = &client_iop_state;

    client_iop_state.element_count = 0;
    iop_checksums.pt_long = 0;
    iop_checksums.pt_double = 0.0;
    GTM(&start_time);

    my_oper2 ( &iop_struct, &correct_to_server, &st); 
    if (st != 0){
	dce_error_inq_text(st, ebuff, &dcest); 
    	if ( dcest == 0)
		fprintf(fp, "%s\n",ebuff);
    }

    timing(&start_time, &avg_time);
    fprintf(fp, "call elapsed ms: %lu.%03u\n", avg_time.msec, avg_time.usec);

    if ( !correct_to_server ) {
        failures = 1;
	fprintf(fp, "Error in Manager checksums for iop_struct returned\n");
    }
    if ( iop_checksums.pt_long != 2497500) {
	failures = 1;                
	fprintf(fp, "Error in client checksums: iop_checksums.pt_long\n");
    }
    if ( iop_checksums.pt_double != 2997000.0 ) {
        failures = 1;
        fprintf(fp, "Error in client checksums: iop_checksums.pt_double\n");
    }
    return(failures);
}

