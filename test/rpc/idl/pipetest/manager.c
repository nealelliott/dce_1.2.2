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
 * Revision 1.1.6.2  1996/02/17  23:16:13  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:41  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:27:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:46  root]
 * 
 * Revision 1.1.3.3  1993/01/07  20:40:26  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:39:17  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:22:12  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:47:23  weisman]
 * 
 * Revision 1.1  1992/01/19  03:26:27  devrcs
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

#define MANAGER_BUFFER_SIZE 2048
byte manager_buffer[MANAGER_BUFFER_SIZE];

static char *filename = "pipetest.h";
static char *pipe_in = "pt.in";
static char *s;

void his_oper2 (
    /* [in] */handle_t h,
    /* [in] */ndr_char in_char,
    /* [in] */pipe_of_chars in_pipe,
    /* [out] */pipe_of_chars out_pipe,
    /* [out] */ndr_long_int *out_long
)
{
    int file_handle;
    unsigned long data_count;

    if (s = getenv("PIPETEST_FILENAME"))
	    filename = s;
    if (s = getenv("PIPETEST_PIPEIN"))
	    pipe_in = s;

#ifdef DEBUG
    printf("Server received character -%c- from client\n",in_char);
#endif

    file_handle = open(pipe_in, O_CREAT | O_TRUNC | O_WRONLY, 0777 );
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

globaldef pipetest_v0_0_epv_t pipetest_v0_0_m_epv = {his_oper2};
