/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>

#include <machine/inline.h>

#include "random-pool.h"

#define NTRIALS 1000000

main()
{
    rand_pool_t rp;
    rand_src_t rs;
    
    int i,j,pj;
    int nbits = 0;
    rand_pool_init(&rp);
    rand_src_init(&rs);

    for (i=0,j=0; i<100000; i++) {
	int nbits;

	j += 40;
	if ((i&7)==5)
	    j += 100;

	j+= i&1;

	nbits = rand_pool_add_timer (&rp, &rs, j);
	pj = j;
    }
    
    printf("bits: %d\n", nbits);
}

