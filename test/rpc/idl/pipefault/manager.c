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
 * $Log: manager.c,v $
 * Revision 1.1.6.2  1996/02/17  23:16:05  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:35  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:27:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:39  root]
 * 
 * Revision 1.1.3.4  1993/01/13  21:49:49  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:38:44  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:22:03  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:47:13  weisman]
 * 
 * Revision 1.1  1992/01/19  03:26:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <stdlib.h>
#ifdef VMS
#include <file.h>
#elif defined (SNI_SVR4)
#include <fcntl.h>
#else
#include <sys/file.h>
#endif
#include <pipefault.h>     

typedef unsigned char byte;

#define MANAGER_BUFFER_SIZE 2048
byte manager_buffer[MANAGER_BUFFER_SIZE];

static char *filename = "pipefault.idl";
static char *pipe_in = "pt.in";
static char *s;

void his_oper1 (
    /* [in] */ndr_char in_char,
    /* [in] */pipe_of_chars in_pipe,
    /* [out] */pipe_of_chars out_pipe,
    /* [out] */ndr_long_int *out_long
)
{
    int file_handle;
    unsigned long data_count;

    if (in_char != 'X') {
        printf("Server received character -%c- from client\n",in_char);
        fflush(stdout);
        }

    if (s = getenv("PIPEFAULT_PIPEIN"))
	    pipe_in=s;

    file_handle = open(pipe_in, O_CREAT | O_TRUNC | O_WRONLY, 0644 );
    if (file_handle == -1)
    {
        printf("Cannot create %s\n", pipe_in);
        exit(0);
    }
    while (true)
    {
        (*in_pipe.pull)(in_pipe.state,manager_buffer,MANAGER_BUFFER_SIZE,
                        &data_count);
        if (data_count == 0)
        {
            close(file_handle);
            break;
        }
        write(file_handle,manager_buffer,data_count);
    }
    /* Read from empty pipe */
        (*in_pipe.pull)(in_pipe.state,manager_buffer,MANAGER_BUFFER_SIZE,
                        &data_count);

    if (s = getenv("PIPEFAULT_FILE"))
	    filename=s;

    file_handle = open(filename,O_RDONLY);
    if (file_handle == -1)
    {
        printf("Server cannot open %s\n", filename); 
        exit(0);
    }
    while (true)
    {
        data_count = read(file_handle,manager_buffer,MANAGER_BUFFER_SIZE);
        (*out_pipe.push)(out_pipe.state,manager_buffer,data_count);
        if (data_count == 0)
        {
            close(file_handle);
            break;
        }
    }

    *out_long = 1024;

}

void his_oper2 (
    /* [in] */handle_t *p_h,
    /* [in] */ndr_char in_char,
    /* [in] */pipe_of_chars in_pipe,
    /* [out] */pipe_of_chars out_pipe,
    /* [out] */ndr_long_int *out_long
)
{
    int file_handle;
    unsigned long data_count;

    if (in_char != 'X') {
        printf("Server received character -%c- from client\n",in_char);
            fflush(stdout);
            }

    /* Write [out] pipe with [in] pipe not drained */

    if (s = getenv("PIPEFAULT_FILE"))
	    filename=s;

    file_handle = open(filename,O_RDONLY);
    if (file_handle == -1)
    {
        printf("Server cannot open %s\n", filename); 
        exit(0);
    }
    while (true)
    {
        data_count = read(file_handle,manager_buffer,MANAGER_BUFFER_SIZE);
        (*out_pipe.push)(out_pipe.state,manager_buffer,data_count);
        if (data_count == 0)
        {
            close(file_handle);
            break;
        }
    }

    *out_long = 1024;

}

void his_oper3 (
    /* [in] */my_handle m,
    /* [in] */ndr_char in_char,
    /* [in] */pipe_of_chars in_pipe,
    /* [out] */pipe_of_chars out_pipe,
    /* [out] */ndr_long_int *out_long
)
{
    int file_handle;
    unsigned long data_count;

    if (in_char != 'X') {
        printf("Server received character -%c- from client\n",in_char);
                fflush(stdout);
                }

    if (s = getenv("PIPEFAULT_PIPEIN"))
	    filename=s;

    file_handle = open(pipe_in, O_CREAT | O_TRUNC | O_WRONLY, 0644 );
    if (file_handle == -1)
    {
        printf("Cannot create %s\n", pipe_in);
        exit(0);
    }
    while (true)
    {
        (*in_pipe.pull)(in_pipe.state,manager_buffer,MANAGER_BUFFER_SIZE,
                        &data_count);
        if (data_count == 0)
        {
            close(file_handle);
            break;
        }
        write(file_handle,manager_buffer,data_count);
    }


    if (s = getenv("PIPEFAULT_FILE"))
	    filename=s;

    file_handle = open(filename,O_RDONLY);
    if (file_handle == -1)
    {
        printf("Server cannot open %s\n", filename); 
        exit(0);
    }
    while (true)
    {
        data_count = read(file_handle,manager_buffer,MANAGER_BUFFER_SIZE);
        (*out_pipe.push)(out_pipe.state,manager_buffer,data_count);
        if (data_count == 0)
        {
            close(file_handle);
            break;
        }
    }

    /* Write to closed pipe */
        (*out_pipe.push)(out_pipe.state,manager_buffer,data_count);

    *out_long = 1024;

}

void his_oper4
(
    /* [in] */my_handle *p_m,
    /* [in] */ndr_char in_char,
    /* [in] */pipe_of_chars in_pipe,
    /* [out] */pipe_of_chars out_pipe,
    /* [out] */ndr_long_int *out_long
)
{
    int file_handle;
    unsigned long data_count;


    if (in_char != 'X') {
        printf("Server received character -%c- from client\n",in_char);
                    fflush(stdout);
                    }

    if (s = getenv("PIPEFAULT_PIPEIN"))
	    filename=s;

    file_handle = open(pipe_in, O_CREAT | O_TRUNC | O_WRONLY, 0644 );
    if (file_handle == -1)
    {
        printf("Cannot create %s\n", pipe_in);
        exit(0);
    }
    while (true)
    {
        (*in_pipe.pull)(in_pipe.state,manager_buffer,MANAGER_BUFFER_SIZE,
                        &data_count);
        if (data_count == 0)
        {
            close(file_handle);
            break;
        }
        write(file_handle,manager_buffer,data_count);
    }

    /* Return from call with [out] pipe not filled */

    *out_long = 1024;

}

globaldef pipefault_v0_0_epv_t pipefault_v0_0_m_epv = {
    his_oper1,
    his_oper2,
    his_oper3,
    his_oper4
};
