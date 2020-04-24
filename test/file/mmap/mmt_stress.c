/*
 * Copyright (c) 1993 Transarc Corp.  All rights reserved.
 */

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <poll.h>

static char pnm[] = "mmaptest_stress";	/* program name for error msgs */

void
sig_catcher (int sigval)
{
    /*
     * We only expect to catch SIGUSR1, the signal that time's up.
     */
    exit (0);
}

/* Should make these settable */

int nwriters = 3;
int ninvalidators = 1;

void
map_and_bash (f, bufsize)
    int f;
    int bufsize;
{
    caddr_t addr = mmap (0, bufsize, PROT_WRITE | PROT_READ,
			 MAP_SHARED, f, 0);
    if (addr == (caddr_t) -1) {perror (pnm); exit (1);}

    printf ("Another process started using mapped memory\n");

    while (1) {
	char c;
	int off, rw, bs;

	/* Create random offset, 15 bits at a time */
	for (bs = bufsize, off = 0; bs >= (1 << 15); bs >>= 15) {
	    off <<= 15;
	    off += rand();
	}
	if (bs > 0) {
	    off *= bs;
	    off += ((rand() * bs) >> 15);
	}

	rw = rand() & (1 << 8);

	if (rw)
	    c = addr [off];
	else
	    addr [off] = 37;
	poll (NULL, 0, (rand() * 1000) >> 15);
    }
}

void
invalidate_map (f, bufsize)
    int f;
    int bufsize;
{
    caddr_t addr = mmap (0, bufsize, PROT_WRITE | PROT_READ,
			 MAP_SHARED, f, 0);
    if (addr == (caddr_t) -1) {perror (pnm); exit (1);}

    printf ("Another process started invalidating mapped memory\n");

    while (1) {
	poll (NULL, 0, (rand() * 1000) >> 15);
	msync (addr, bufsize, MS_ASYNC | MS_INVALIDATE);
    }
}

main()
{
    printf ("mmap stress tests\n");

    printf ("Multiple process tests\n");

    {
	char buf [8192*128];
	int bufsize = sizeof(buf);
	int f = open ("test_file_stress", O_RDWR|O_CREAT|O_TRUNC, 0777);
	long *lbuf = (long *) buf;
	int code;
	int i;
	pid_t val;

	/*
	 * Create our own process group so that when we kill all our
	 * children, we don't kill our parent too
	 */
	(void) setpgrp ();

	/* Extend file via syscall */
	for (i=0; i<(bufsize/sizeof(long)); i++)
	    lbuf[i] = (i % (1<<15)) * (i % (1<<15));
	code = write (f, buf, bufsize);
	if (code == -1) {perror (pnm); exit (1);}
	if (code < bufsize) {
	    fprintf (stderr, "%s: wrote less data than expected\n", pnm);
	    exit (1);
	}

	/* Spawn memory modifiers */
	for (i=0; i<nwriters; i++) {
	    val = fork();
	    if (val == 0) {		/* child */
		signal (SIGUSR1, sig_catcher);
		map_and_bash (f, bufsize);
		exit (1);		/* should not get here */
	    }
	}

	/* Spawn invalidators */
	for (i=0; i<ninvalidators; i++) {
	    val = fork();
	    if (val == 0) {		/* child */
		signal (SIGUSR1, sig_catcher);
		invalidate_map (f, bufsize);
		exit (1);		/* should not get here */
	    }
	}

	/* Let them run for a while */
	sleep (15 * 60);

	/* Don't get killed by our own signal */
	signal (SIGUSR1, SIG_IGN);

	/* Blow them all away */
	kill (0, SIGUSR1);

	/* Clean up */
	close (f);

	printf ("Finished multiple process stress test.\n");
    }
}
