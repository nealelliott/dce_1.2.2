/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: perfp.c,v $
 * Revision 1.1.3.2  1996/02/17  23:23:54  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:20  marty]
 *
 * Revision 1.1.3.1  1995/12/11  20:15:13  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/30  17:44 UTC  psn
 * 	Add definition of MIN for non-HP platform.
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/06  15:47 UTC  tatsu_s
 * 	Submitted the pipe test.
 * 
 * 	HP revision /main/tatsu_s_func_test_b1/2  1994/10/24  17:19 UTC  tatsu_s
 * 	Added more pipe tests.
 * 
 * 	HP revision /main/tatsu_s_func_test_b1/1  1994/10/10  13:57 UTC  tatsu_s
 * 	Pipe test
 * 	[1995/12/11  19:48:07  root]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1994 by
**      Hewlett-Packard Company, Palo Alto, Ca.
**
**
**  NAME
**
**      rpcp.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  Server manager routines for performance and system execiser
**  auxiliary interface. This interface is generic.
**
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/file.h>

#include <perfp.h>

#include <perf_c.h>
#include <perf_p.h>


#if !defined(MIN) && !defined(__hpux)
#define MIN(a,b)  ((a) > (b)? (b) : (a))
#endif

#define MANAGER_BUFFER_SIZE 2048

/***************************************************************************/

void my_perfp_op1
#ifdef IDL_PROTOTYPES
(
    /* [in] */ handle_t handle,
    /* [in] */ idl_ulong_int seconds,
    /* [in] */ pipe_of_chars in_pipe,
    /* [out] */ idl_ulong_int *out_long
)
#else
(handle, seconds, in_pipe, out_long)
    /* [in] */ handle_t handle;
    /* [in] */ idl_ulong_int seconds;
    /* [in] */ pipe_of_chars in_pipe;
    /* [out] */ idl_ulong_int *out_long;
#endif
{
    unsigned long data_count;
    unsigned long total = 0;
    char manager_buffer[MANAGER_BUFFER_SIZE];

    print_binding_info ("my_perfp_op1", handle);

    while (true)
    {
        (*in_pipe.pull)(in_pipe.state, (idl_char *)manager_buffer,
                        sizeof(manager_buffer), &data_count);
        if (data_count == 0)
            break;

        total += data_count;

        if (seconds != 0)
        {
            printf("+ Sleeping for %lu seconds...\n", seconds);
            SLEEP(seconds);
            printf("    ...awake!\n");
        }
    }

    printf("    ...done!\n");

    *out_long = total;
}

extern void my_perfp_op2
#ifdef IDL_PROTOTYPES
(
    /* [in] */ handle_t handle,
    /* [in] */ idl_ulong_int seconds,
    /* [in] */ idl_ulong_int in_long,
    /* [out] */ pipe_of_chars out_pipe
)
#else
(handle, seconds, in_long, out_pipe)
    /* [in] */ handle_t handle;
    /* [in] */ idl_ulong_int seconds;
    /* [in] */ idl_ulong_int in_long;
    /* [out] */ pipe_of_chars out_pipe;
#endif
{
    unsigned long data_count;
    idl_boolean forever = (in_long == 0);
    char manager_buffer[MANAGER_BUFFER_SIZE];

    print_binding_info ("my_perfp_op2", handle);

    while (true)
    {
	if (forever)
	    data_count = sizeof(manager_buffer);
	else
	    data_count = MIN(in_long, sizeof(manager_buffer));

        (*out_pipe.push)(out_pipe.state, (idl_char *)manager_buffer,
                         data_count);

        if (data_count == 0)
	    break;

	if (!forever)
	    in_long -= data_count;

        if (seconds != 0)
        {
            printf("+ Sleeping for %lu seconds...\n", seconds);
            SLEEP(seconds);
            printf("    ...awake!\n");
        }
    }

    printf("    ...done!\n");
}

void my_perfp_op3
#ifdef IDL_PROTOTYPES
(
    /* [in] */ handle_t handle,
    /* [in] */ idl_ulong_int seconds,
    /* [in] */ pipe_of_chars in_pipe,
    /* [out] */ pipe_of_chars out_pipe,
    /* [out] */ idl_ulong_int *out_long
)
#else
(handle, seconds, in_pipe, out_pipe, out_long)
    /* [in] */ handle_t handle;
    /* [in] */ idl_ulong_int seconds;
    /* [in] */ pipe_of_chars in_pipe;
    /* [out] */ pipe_of_chars out_pipe;
    /* [out] */ idl_ulong_int *out_long;
#endif
{
    int f = -1;
    int n;
    unsigned long i;
    char *t;
    char TempFileName[] = "/tmp/perfpXXXXXX";
    unsigned long data_count;
    unsigned long total = 0;
    char manager_buffer[MANAGER_BUFFER_SIZE];

    print_binding_info ("my_perfp_op3", handle);

    t = (char *) mktemp(TempFileName);
    printf("+ Writing file \"%s\"\n", t);
    f = open(t, (O_TRUNC | O_WRONLY | O_CREAT), 0777);
    if (f < 0)
    {
        perror("Can't create temp file");
        goto DONE;
    }

    while (true)
    {
        (*in_pipe.pull)(in_pipe.state, (idl_char *)manager_buffer,
                        sizeof(manager_buffer), &data_count);
        if (data_count == 0)
        {
            close(f);
            break;
        }
        n = write(f, manager_buffer, data_count);
        if (n != data_count)
        {
            perror("Write failed");
            goto DONE;
        }

        if (seconds != 0)
        {
            printf("+ Sleeping for %lu seconds...\n", seconds);
            SLEEP(seconds);
            printf("    ...awake!\n");
        }
    }

    printf("+ Reading file \"%s\"\n", t);
    f = open(t, O_RDONLY);
    if (f < 0)
    {
        perror("Can't copen temp file");
        goto DONE;
    }

    while (true)
    {
        data_count = read(f, manager_buffer, sizeof(manager_buffer));
        (*out_pipe.push)(out_pipe.state, (idl_char *)manager_buffer,
                         data_count);
        if (data_count < 0)
        {
            perror("Read failed");
            goto DONE;
        }
        if (data_count == 0)
        {
            close(f);
            break;
        }
        total += data_count;

        if (seconds != 0)
        {
            printf("+ Sleeping for %lu seconds...\n", seconds);
            SLEEP(seconds);
            printf("    ...awake!\n");
        }
    }

DONE:
    printf("    ...done!\n");
    if (f >= 0)
        close(f);
    unlink(t);

    *out_long = total;
}

/***************************************************************************/

perfp_v1_0_epv_t perfp_epv =
{
    my_perfp_op1,
    my_perfp_op2,
    my_perfp_op3
};
