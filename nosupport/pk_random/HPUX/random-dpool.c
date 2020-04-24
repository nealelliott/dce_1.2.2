/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
#include "random-pool.c"

/* queue a sample.  in reality, as this is time-critical,
 * this should be inlined; it's about 6 instructions on the PA.
 */
   

void rand_pool_deferred_add(unsigned32 input)
{
    unsigned32 *ptr = dpool.ptr;
    unsigned32 *end = dpool.end;

    if (ptr<end)
	*ptr++ = input;
    dpool.ptr = ptr;
}

void rand_pool_deferred_process(void)
{
    unsigned32 *ptr = dpool.start;
    unsigned32 *end = dpool.ptr;

    dpool.last = end;
    dpool.ptr = ptr;

    for (; ptr < end; ptr++) {
	/* this should perhaps be inlined.. */
	rand_pool_add_timer (&gpool, &gsrc, *ptr); 
    }
}


void random_pool_deferred_init(unsigned32 *buf, unsigned32 *end)
{
    dpool.ptr = buf;
    dpool.start = buf;
    dpool.end = end;
    rand_pool_init(&gpool);
    rand_src_init(&gsrc);
}




