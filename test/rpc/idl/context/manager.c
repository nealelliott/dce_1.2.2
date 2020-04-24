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
 * Revision 1.1.6.2  1996/02/17  23:13:17  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:11  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:00:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:32  root]
 * 
 * Revision 1.1.3.3  1993/01/07  19:49:23  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:30:10  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:20:52  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:46:15  weisman]
 * 
 * Revision 1.1  1992/01/19  03:24:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#ifdef VMS
#include <file.h>
#elif SNI_SVR4
#include <fcntl.h>
#else
#include <sys/file.h>
#endif
#include <context.h>
#include <test_common.h>


static char *filename = "remotefile.tmp"; /* Default output filename */
static char *s;
static int file_handle;

static void wr_open(h,p_wr_ctx)
handle_t h;
ndr_void_p_t *p_wr_ctx;
{
#ifdef DEBUG
    printf("Entered wr_open\n");
#endif

	if (s = getenv("S_REMOTEFILE"))
		filename = s;

        file_handle = open(filename, 
                                  O_CREAT | O_TRUNC | O_WRONLY, 0644 );
        if (file_handle == -1)
        {
            printf("Couldn't create %s for writing\n",filename);
            exit(0);
        }
        *p_wr_ctx = (ndr_void_p_t)&file_handle;
}

static void wr_write(wr_ctx,count,buffer)
ndr_void_p_t wr_ctx;
long count;
ndr_byte buffer[BUFFSIZE];
{
#ifdef DEBUG
    printf("Entered wr_write\n");
#endif

    if (write(*(int *)wr_ctx,buffer,count) != count)
    {
        printf("Error writing to %s\n",filename);
        exit(0);
    }
}

static void wr_close(p_wr_ctx)
ndr_void_p_t *p_wr_ctx;
{
#ifdef DEBUG
    printf("Entered wr_close\n");
#endif

    if (close(*(int *)*p_wr_ctx) == -1)
    {
        printf("Error closing %s after writing\n",filename);
        exit(0);
    }
    *p_wr_ctx = NULL;
}

static void rd_open(h,p_rd_ctx)
handle_t h;
ndr_void_p_t *p_rd_ctx;
{
#ifdef DEBUG
    printf("Entered rd_open\n");
#endif

	if (s = getenv("S_REMOTEFILE"))
		filename = s;

        file_handle = open(filename,O_RDONLY);
        if (file_handle == -1)
        {
            printf("Couldn't open %s for reading\n", filename);
            exit(0);
        }
        *p_rd_ctx = (ndr_void_p_t)&file_handle;
}

static void rd_read(rd_ctx,p_count,buffer)
ndr_void_p_t rd_ctx;
long *p_count;
ndr_byte buffer[BUFFSIZE];
{
#ifdef DEBUG
    printf("Entered rd_read\n");
#endif

   *p_count = read(*(int *)rd_ctx, buffer, BUFFSIZE);
}

static void rd_close(p_rd_ctx)
ndr_void_p_t *p_rd_ctx;
{
#ifdef DEBUG
    printf("Entered rd_close\n");
#endif

    if (close(*(int *)*p_rd_ctx) == -1)
    {
        printf("Error closing %s after reading\n",filename);
        exit(0);
    }
    *p_rd_ctx = NULL;
}

globaldef context_v0_0_epv_t context_v0_0_m_epv = {
    wr_open,
    wr_write,
    wr_close,
    rd_open,
    rd_read,
    rd_close
};
