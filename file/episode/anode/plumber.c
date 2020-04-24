/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: plumber.c,v $
 * Revision 1.1.43.1  1996/10/02  17:18:37  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:28:36  damon]
 *
 * Revision 1.1.37.2  1994/06/09  13:58:37  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:33:49  annie]
 * 
 * Revision 1.1.37.1  1994/02/04  20:11:47  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:09:27  devsrc]
 * 
 * Revision 1.1.35.1  1993/12/07  17:17:28  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:45:00  jaffe]
 * 
 * Revision 1.1.2.4  1993/01/18  22:06:53  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  21:46:57  cjd]
 * 
 * Revision 1.1.2.3  1992/11/24  16:21:31  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:19:17  bolinger]
 * 
 * Revision 1.1.2.2  1992/11/09  21:34:53  jaffe
 * 	Transarc delta: ota-ot3804-new-block-security 1.1
 * 	  Selected comments:
 * 	    This is a super-delta comprising changes necessary to accomplish both
 * 	    deltas which are a very tough import and are really intertwined anyway.
 * 	    Transarc delta: mason-ot5508-wrong-comparison-in-WaitForTranFree 1.4
 * 	    Transarc delta: ota-new-block-security-2467 1.9
 * 	[1992/11/09  18:12:12  jaffe]
 * 
 * $EndLog$
 */

/* Copyright (C) 1992 Transarc Corporation - All rights reserved. */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

#include <dcedfs/episode/fifo.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */


RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/plumber.c,v 1.1.43.1 1996/10/02 17:18:37 damon Exp $")

/* plumber.c -- This package is a wrapper for osi_Alloc and osi_Free intended
 *     to catch heap trashing problem.  It is has not been used to actual look
 *     for memory leaks so the name "plumber" may not be especially
 *     appropriate.  It has only been used in user space (since it calls
 *     "malloc" to do the actual allocation).
 *
 *     The basic idea is to allocate space for a header and trailer on every
 *     block and keep them on a fifo of allocated blocks.  Periodically, this
 *     list can be checked for consistency.  Free's insist that the blocks were
 *     actually allocated by expecting them to be on the fifo.  Further it sets
 *     the contents of freed blocks to a bogus value in attempt to catch
 *     references to a block after it is freed.
 *
 *     The format of the header is a obvious.
 *
 *     The format of the trailer is complicated by the desire the insert a
 *     recognizable pattern immediately followig the requested bytes to detect
 *     any overruns, even those below the granualarity of a word.  We insist on
 *     at least 4 bytes of magic trailer plus enough bytes to pad the requested
 *     length to a 4 byte multiple.  Thus we could have anywhere from 4 to 7
 *     bytes of magic tailer.  These are bcopied from the structure "magic34"
 *     into the allocated block immediately after the requested space.  The
 *     check uses a similar call to bcmp.  This is done using the MAGIC34
 *     macro.  The header ptr in the trailer just provides a testable
 *     consistency between the header and trailer of the same block. */

#define MAGIC1 0x9d0bcf25
#define MAGIC2 0xed2d5d49
#define MAGIC3 0x64dd6934
#define MAGIC4 0x5e700934

static struct { long magic3; long magic4; } magic34 = {MAGIC3, MAGIC4};

struct header {
    long magic1;
    struct fifo_Thread thread;
    int size;
    long magic2;
};
struct trailer {
    /* The first zero to three bytes of magic34 precede this word. */
    long magic;
    struct header *h;
};

PRIVATE struct fifo blocks;
PRIVATE init;

#define ROUNDUP(x) ((x+3)&~3)
#define SIZEOF(x) (ROUNDUP(x) + sizeof(struct header) + sizeof(struct trailer))

#define MAGIC34(h) /* bcmp or bcopy arglist */ \
    (&magic34, (char *)((h)+1) + (h)->size, ROUNDUP((h)->size) - (h)->size + 4)

#define H_MAGIC_OK(h) ((h->magic1 == MAGIC1) && (h->magic2 == MAGIC2))
#define T_MAGIC_OK(h,t) ((t->h == (h)) && (bcmp MAGIC34(h) == 0))

/* on average (the log of) how many entries to traverse on each call. */
/* Set to 31 to check on every call, set to zero to never check. */
PRIVATE int logEntriesPerCheck = 4;	/* i.e. 2^4 = 16 */

/* Check -- periodically check the list of allocated blocks for damage to the
 *     header and trailer. */

PRIVATE void Check()
{
    static long calls = 0;		/* calls made */
    static long entries = 0;		/* entries traversed */
    struct header *h;

    if (calls++ < (entries >> logEntriesPerCheck)) return;
    entries += blocks.length;

    for (h = FIFO_LookAtOldest(&blocks, offsetof(struct header, thread));
	 h;
	 h = FIFO_NextOldest(&blocks, h, &h->thread)) {
	char *b = (char *)(h+1);
	struct trailer *t = (struct trailer *)(b + ROUNDUP(h->size));
	afsl_Assert (init && H_MAGIC_OK(h) && T_MAGIC_OK(h,t));
    }
}

opaque osi_Alloc (x)
  long x;
{
    char *b;
    struct header *h;
    struct trailer *t;
    int size = SIZEOF(x);

    if (!init) {
	init = 1;
	FIFO_INIT (&blocks, /*thread offset*/4);
    }

    Check ();

    h = (struct header *) malloc (size);
    h->magic1 = MAGIC1;
    h->magic2 = MAGIC2;
    h->size = x;
    FIFO_ADD (&blocks, &h->thread);
    b = (char *)(h + 1);
    t = (struct trailer *)(b+ROUNDUP(x));
    t->h = h;
    bcopy MAGIC34(h) ;

    { /* init the data to garbage */
	u_int i;
	for (i=0; i<x; i++) b[i] = (u_char)i;
    }

    return b;
}

void osi_Free (b, x)
  IN opaque b;
  IN long x;
{
    struct header *h = (struct header *)((char *)b-sizeof(struct header));
    struct trailer *t = (struct trailer *)((char *)b+ROUNDUP(x));
    int size = SIZEOF(x);

    Check();
    afsl_Assert (H_MAGIC_OK(h) && (h->size == x) && T_MAGIC_OK(h,t));
    FIFO_REMOVE (&blocks, &h->thread);
    {   int i;
	int words = size>>2;
	long *w = (long *)h;
	for (i=0;i<words;i++) {
	    *(w++) = 0xfdfe0102;
	}
    }
    free (h);
}

#ifdef MAIN

main ()
{
    char *blks[10];
    int sizes[10];
    int i,x;

    srandom(time(0));

    for (i=0; i<10; i++) {
	blks[i] = osi_Alloc (sizes[i] = random() % 100000);
    }

    for (i=0; i<100; i++) {
	while (!blks[(x = random() % 10)]);
	osi_Free (blks[x],sizes[x]);
	blks[x] = 0;
	if (i < 90) 
	    blks[x] = osi_Alloc (sizes[x] = random() % 100000);
    }
    return 0;
}

#endif
