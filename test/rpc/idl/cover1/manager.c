/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.6.2  1996/02/17  23:13:23  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:15  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:00:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:42  root]
 * 
 * Revision 1.1.3.3  1993/01/07  19:50:24  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:30:39  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:21:06  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:46:25  weisman]
 * 
 * Revision 1.1  1992/01/19  03:31:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
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
#include <test_common.h>

typedef unsigned char byte;

static char *filename="cover1.in";
static char *s;

#define MANAGER_BUFFER_SIZE 2048
byte manager_buffer[MANAGER_BUFFER_SIZE];

void his_in_pipe_op(h,in_pipe)
    /* [in] */handle_t h;
    /* [in] */pipe_of_chars in_pipe;
{
    int file_handle;
    unsigned long data_count;

    if (s = getenv("COVER1_S_FILENAME"))
	    filename = s;

    file_handle = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644 );
    if (file_handle == -1)
    {
        printf("Cannot create %s\n", filename);
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

}

void his_out_pipe_op(h, out_pipe)
    /* [in] */handle_t h;
    /* [out] */pipe_of_chars out_pipe;
{
    int file_handle;
    unsigned long data_count;

    if (s = getenv("COVER1_S_FILENAME"))
	    filename = s;


    file_handle = open(filename, O_RDONLY);
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

}

void his_sp_pa_op( h, p_in_tc_link, p_p_out_tc_link )
/*[in]*/ handle_t h;
/*[in]*/ tc_link_t *p_in_tc_link;
/*[out]*/ tc_link_t **p_p_out_tc_link;
{
    tc_link_t *p_out_tc_link, *p_new_tc_link;
    
    p_out_tc_link = NULL;
    for (;
        p_in_tc_link != NULL;
        p_in_tc_link = p_in_tc_link->p_link)
    {
        p_new_tc_link = (tc_link_t *)malloc(sizeof(tc_link_t));
        p_new_tc_link->p_link = p_out_tc_link;
        p_new_tc_link->p_tc = p_in_tc_link->p_tc;
        p_out_tc_link = p_new_tc_link;
    }
    *p_p_out_tc_link = p_out_tc_link;
}

void his_dtb_op(h, dtbp, status)
    handle_t h;
    dtb_param_struct *dtbp;
    error_status_t *status;
{
    int i;

    dtbp->count = DTB_ELT_COUNT;
    for ( i=0; i<DTB_ELT_COUNT; i++ )
    {
        dtbp->elts[i].a = i * i;
        dtbp->elts[i].b = i * i * i;
    }
    *status = 0x12345678;
}

globaldef cover1_v0_0_epv_t cover1_v0_0_m_epv = {his_in_pipe_op,
                                                   his_out_pipe_op,
                                                   his_sp_pa_op,
                                                 his_dtb_op};
