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

#define NTRIALS 10000

main()
{
    rand_pool_t rp;
    rand_src_t gtod;
    rand_src_t cr16;
    int ncrstat[NTRIALS];
    int ncrbit[NTRIALS];    
    int stats[256];
    
    int i,j;
    int nbits = 0;
    int ncrbits = 0;
    rand_pool_init(&rp);
    rand_src_init(&gtod);
    rand_src_init(&cr16);

    for (i=0; i<256;i++) {
	stats[i] = 0;
    }
    for (j=0; j<NTRIALS; j++) {
	register unsigned32 tmp;
	struct timeval tv;
#if 0
	gettimeofday(&tv, 0);
	nbits += rand_pool_add_timer(&rp, &gtod, tv.tv_usec);
#endif

	_MFCTL(16, tmp);
	ncrstat[j]=tmp;	
	ncrbit[j] = rand_pool_add_timer(&rp, &cr16, tmp);
	ncrbits += ncrbit[j];
#if 0
	if (j>POOLWORDS) {
	    for (i=0; i<POOLWORDS; i++) {
		unsigned32 pw = rp.pool[i];
		stats[pw&0xff]++;
		stats[(pw>>8)&0xff]++;
		stats[(pw>>16)&0xff]++;
		stats[(pw>>24)&0xff]++;
	    }
	}
#endif
    }
#if 0
    for (i=0; i<POOLWORDS; i++) {
	unsigned32 pw = rp.pool[i];
	stats[pw&0xff]++;
	stats[(pw>>8)&0xff]++;
	stats[(pw>>16)&0xff]++;
	stats[(pw>>24)&0xff]++;
    }
#endif
    for (i=1; i<NTRIALS; i++) {
	printf("%d, %d\n", ncrstat[i]-ncrstat[i-1], ncrbit[i]);
    }
#if 0
    for (i=0; i<256; i++) {
	if (i%8 == 0)
	    printf("\n");
	printf("%7d ", stats[i]);

    }
#endif

    printf("\n");
	
	

    printf("bits: %d\n", nbits);
    printf("crbits: %d\n", ncrbits);
}

