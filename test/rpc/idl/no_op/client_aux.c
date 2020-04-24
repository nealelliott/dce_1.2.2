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
 * Revision 1.1.4.2  1996/02/17  23:15:09  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:10  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:25:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:13:52  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:59:35  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:35:14  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:32:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdlib.h>
#include <stdio.h>
#include <no_op.h>

ndr_boolean verbose = false;

static char *INPUTFILE  = "no_op.h";
static char *OUTPUTFILE = "outputfile";

static void caller_alloc
#ifdef IDL_PROTOTYPES
(
    rpc_ss_pipe_state_t state,
    ndr_ulong_int       bsize,
    ndr_byte            **buf,
    ndr_ulong_int       *bcount
)
#else
(state, bsize, buf, bcount)
    rpc_ss_pipe_state_t state;
    ndr_ulong_int       bsize;
    ndr_byte            **buf;
    ndr_ulong_int       *bcount;
#endif
{
    static ndr_byte buffer[BUFSIZ];

    *buf    = buffer;
    *bcount = BUFSIZ;
}

static void caller_pull
#ifdef IDL_PROTOTYPES
(
    rpc_ss_pipe_state_t state,
    ndr_byte            *buf,
    ndr_ulong_int       esize,
    ndr_ulong_int       *ecount
)
#else
(state, buf, esize, ecount)
    rpc_ss_pipe_state_t state;
    ndr_byte            *buf;
    ndr_ulong_int       esize;
    ndr_ulong_int       *ecount;
#endif
{
    if (0 == (*ecount = fread(buf, sizeof(*buf), esize, *((FILE **)state)))) {
        fclose(*((FILE **)state));
        if (NULL == (*((FILE **)state) = fopen(OUTPUTFILE, "w")))
            perror(OUTPUTFILE);
    }
}

static void caller_push
#ifdef IDL_PROTOTYPES
(
    rpc_ss_pipe_state_t state,
    ndr_byte            *buf,
    ndr_ulong_int       ecount
)
#else
(state, buf, ecount)
    rpc_ss_pipe_state_t state;
    ndr_byte            *buf;
    ndr_ulong_int       ecount;
#endif
{
    if (verbose)
        printf("In caller_push(), ecount = %d, buf = %x\n", ecount, (int)buf);

    if (ecount != fwrite(buf, sizeof(*buf), ecount, *((FILE **)state)))
        fprintf(stderr, "Bad fwrite() at caller.\n");
}


int client_aux
#ifdef IDL_PROTOTYPES
(handle_t h, int passes)
#else
(h, passes)
    handle_t h;
    int passes;
#endif
{
    long            failures = 0;
    no_op_pipe_t    *inoutpipe;
    static FILE     *file = NULL;
    char *s;

    if (getenv("NO_OP_VERBOSE"))
	verbose = 1;

    if (s = getenv("NO_OP_INFILE"))
	INPUTFILE = s;

    if (s = getenv("NO_OP_OUTFILE"))
	OUTPUTFILE = s;

    if (verbose)
        printf("In no_op_op0(), input file is \"%s\".\n", INPUTFILE);

    if (NULL == (inoutpipe = malloc(sizeof(*inoutpipe)))) {
        fprintf(stderr, "Cannot allocate pipe in caller.\n");
        exit(1);
    }

    if (NULL == (file = fopen(INPUTFILE, "r"))) {
        perror(INPUTFILE);
        exit(1);
    }

    inoutpipe->alloc = caller_alloc;
    inoutpipe->pull  = caller_pull;
    inoutpipe->push  = caller_push;
    inoutpipe->state = &file;

    if (! no_op_op0(h, inoutpipe))
        ++failures;

    fclose(file);
    free(inoutpipe);

    return failures;
}
