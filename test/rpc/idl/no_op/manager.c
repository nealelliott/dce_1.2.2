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
 * Revision 1.1.4.2  1996/02/17  23:15:10  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:11  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:25:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:13:53  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:59:42  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:35:18  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:32:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <stdlib.h>
#include <no_op.h>

static ndr_boolean verbose = false;

static char *BUFFERFILE = "buffer";

static ndr_boolean op0
#ifdef IDL_PROTOTYPES
(
    /*in*/     handle_t     h,
    /*in,out*/ no_op_pipe_t *inoutpipe
)
#else
(h, inoutpipe)
    /*in*/     handle_t     h;
    /*in,out*/ no_op_pipe_t *inoutpipe;
#endif
{
    unsigned long   count, totalin, totalout;
    ndr_byte        buffer[BUFSIZ];
    FILE            *file;
    char *s;

    if (getenv("NO_OP_VERBOSE"))
	verbose = 1;

    if (s = getenv("NO_OP_S_FILENAME"))
	BUFFERFILE = s;

    if (verbose)
        printf("In no_op_op0().\n");

    totalin = totalout = 0;

    if (verbose)
        printf("In no_op_op0(), buffer file is \"%s\".\n", BUFFERFILE);

    if (NULL == (file = fopen(BUFFERFILE, "w"))) {
        perror(BUFFERFILE);
        exit(1);
    }

    do {
        inoutpipe->pull(inoutpipe->state, buffer, BUFSIZ, &count);
        if (verbose)
            printf("Pulled a chunk of %d bytes into %x.\n", count, buffer);
        if (count != fwrite(buffer, sizeof(buffer[0]), count, file)) {
            fprintf(stderr, "Bad write to \"%s\".\n", BUFFERFILE);
            exit(1);
        }
        totalin += count;
    } while (count);

    if (verbose)
        printf("In no_op_op0(), totalin == %d\n", totalin);

    fclose(file);

    if (NULL == (file = fopen(BUFFERFILE, "r"))) {
        perror(BUFFERFILE);
        exit(1);
    }

    do {
        count = fread(buffer, sizeof(buffer[0]), BUFSIZ, file);
        if (verbose)
            printf("About to push a chunk of %d bytes from %x.\n", count, buffer);
        inoutpipe->push(inoutpipe->state, buffer, count);
        if (verbose)
            printf("Pushed a chunk of %d bytes from %x.\n", count, buffer);
        totalout += count;
    } while (count);

    if (verbose)
        printf("In no_op_op0(), totalout == %d\n", totalout);

    return (ndr_boolean)(totalin == totalout);
}

globaldef no_op_v0_0_epv_t no_op_v0_0_m_epv = {
    op0
};
