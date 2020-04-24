/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include "random-hpux.h"

main()
{
    struct random_stats curst, lastst;
    int delta_in, delta_out, delta_samples;
    double entpersamp;
    
    int fd = open("/dev/random", O_RDONLY, 0);

    memset(&lastst, 0, sizeof(lastst));

    printf("%8.8s %8.8s %8.8s %8.8s %8.8s %8.8s %8.8s\n",
	   "entropy", "out", "in", "samples", "ent/samp", "waiters", "dpool");
#define DELTA(field) (curst.field - lastst.field)
    for (;;) {
	if (ioctl(fd, RNDGETSTATS, &curst) < 0) {
	    perror("ioctl(RNDGETSTATS)");
	    exit(1);
	}
	delta_in = DELTA(entropy_in);
	delta_out = DELTA(entropy_out); 
	delta_samples = DELTA(input_samples);
	entpersamp = (double)(delta_in)/(double)(delta_samples);
	
	printf("%8d %8d %8d %8d %8.4f %8d %8d\n",
	       curst.ent_stored,
	       delta_out,
	       delta_in,
	       delta_samples,
	       entpersamp,
	       curst.waiters,
	       curst.deferred_last);
	lastst = curst;
	sleep(1);
    }
}

	
    
