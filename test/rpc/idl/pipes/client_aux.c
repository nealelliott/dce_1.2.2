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
 * Revision 1.1.4.2  1996/02/17  23:16:08  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:37  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:27:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:42  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:39:56  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:38:59  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <pipes.h>
#include <test_common.h>

extern char *error_text();

#define BUFFSIZE 1024

static void alloc(state, bsize, buf, bcount)
    rpc_ss_pipe_state_t state;
    ndr_ulong_int bsize;
    ndr_byte **buf;
    ndr_ulong_int *bcount;
{
    static char buffer[BUFFSIZE];

    *buf = (ndr_byte *)buffer;
    *bcount = BUFFSIZE;
}

static void pull(state, buf, esize, ecount)
    rpc_ss_pipe_state_t state;
    ndr_byte *buf;
    ndr_ulong_int esize;
    ndr_ulong_int *ecount;
{
    *ecount = fread(buf, 1, esize, (FILE *)state);
}

client_aux(h,passes)
      handle_t h;
      int passes;
{
    int failures = 0;
    char *srcfile,*dstfile;
    char *s;
    error_status_t st;
    pipe_t *stream = (pipe_t *)malloc(sizeof(struct pipe_t));
    ndr_ulong_int checksum;
    int i, count;
    ndr_char dst[32];
    char buf[BUFFSIZE];

    srcfile = "pipes.h";
    dstfile = "pipes.out";
    
    if (s = getenv("PIPES_SRCFILE"))
	srcfile = s;

    if (s = getenv("PIPES_DSTFILE"))
	dstfile = s;

    if ((stream->state = fopen(srcfile, "r")) == NULL)
    {
        fprintf(stderr, "Couldn't open %s for reading\n", srcfile);
        exit(2);
    }

    printf("Computing checksum...\n");
    checksum = 0;
    while (count = fread(buf, 1, BUFFSIZE, ((FILE *)stream->state)))
        for (i = 0; i < count; i++) checksum += buf[i];
#ifdef DEBUG
    printf("Checksum is %d\n", checksum);
#endif
    rewind((FILE *)stream->state);

    printf("Copying from %s\n", srcfile);

    stream->alloc = alloc;
    stream->pull = pull;
    strcpy(dst, dstfile);

    if (pipes_copy(h, dst, stream, checksum))
    {
      printf("Copy failed.\n");
      failures++;
    }
    else printf("Copy succeeded.\n");

    return failures;
}
