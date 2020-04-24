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
 * Revision 1.1.4.2  1996/02/17  23:16:10  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:38  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:27:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:43  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:40:02  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:39:03  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <pipes.h>
#include <stdio.h>
#include <test_common.h>

#define BUFFSIZE 512

static ndr_long_int copy(h, dst, stream, checksum)
    handle_t h;
    char *dst;
    pipe_t *stream;
    long checksum;
{
    FILE *f = fopen(dst, "w");
    char buffer[BUFFSIZE];
    int i, count;
    ndr_ulong_int ecount, check2;

    if (f == NULL)
    {
        fprintf(stderr, "Couldn't open %s for writing\n", dst);
        return;
    }

#ifdef DEBUG
    printf("Copying to %s\n", dst);
#endif

    do
    {
        stream->pull(stream->state, (ndr_byte *)buffer, BUFFSIZE, &ecount);
        fwrite(buffer, 1, ecount, f);
        for (i = 0; i < ecount; i++) check2 += (int)buffer[i];
    } while (ecount);

    fclose(f);

#ifdef DEBUG
    printf("Computing checksum...\n");
#endif
    if ((f = fopen(dst, "r")) == NULL)
    {
        fprintf(stderr, "Couldn't open %s for reading\n", dst);
        return;
    }
    check2 = 0;
    while (count = fread(buffer, 1, BUFFSIZE, f))
        for (i = 0; i < count; i++) check2 += buffer[i];
    fclose(f);

#ifdef DEBUG
    printf("Checksum is %d\n", check2);
#endif

    return (check2 == checksum) ? 0 : -1;
}

globaldef pipes_v0_0_epv_t pipes_v0_0_m_epv = {copy};
