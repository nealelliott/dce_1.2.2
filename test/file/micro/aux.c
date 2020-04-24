/* 
 * Copyright (C) 1996, 1993  Transarc Corporation - All rights reserved 
 */
/*
 * Functions to help microtests
 */

#include "aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#if 	defined(SUNOS5)
#include <sys/times.h>
#include <limits.h>
#endif
#include <fcntl.h>

int	in_subdir = 0;	/* Flag to tell if we are in subdir */
struct timeval tv_start, tv_end;
#if	defined(SUNOS5)
static struct tms r_buf_start, r_buf_end;
#else	/* SUNOS5 */
static struct rusage r_buf_start, r_buf_end;
#endif	/* SUNOS5 */

/*
 * Set up a test directory with files (filled with data if provided)
 * Creates TDIR in ".", cd's into it and creates test files inside.
 */
int make_testdir (files, buf, bsize)
	int files;
	char *buf;
	int bsize;
{
    int fd, i;
    char name[MAXPATHLEN], str[MAXPATHLEN];
    struct stat statb;

    /*
     * See if a remnant test directory exists, if so zap it.
     */
    if (stat(TDNAME, &statb) == 0) {
	sprintf(str, "rm -r %s", TDNAME);
	if (system(str) != 0) {
	    perror("system: can't remove old test directory");
	    return(1);
	}
    }

    /*
     * Make test directory
     */
    if (mkdir(TDNAME, TDMODE) < 0) {
	perror("mkdir: can't create test directory");
	return(1);
    }
    if (chdir(TDNAME) < 0) {
	perror("chdir to test directory");
	return(1);
    }
    in_subdir = 1;

    /*
     * Now create all the files in the test directory
     */
    for (i=0; i<files; i++) {
	sprintf(name, "%s.%d", TFNAME, i);
	if ((fd = open(name, O_RDWR|O_CREAT, TFMODE)) < 0) {
	    perror("open");
	    return(1);
	}
	if (buf) {
	    if (write(fd, buf, bsize) != bsize) {
		perror("write after creat failed");
		return(1);
	    }
	}

	if (close(fd) < 0) {
	    perror("close");
	    return(1);
	}
    }
    return(0);
}

/*
 * Remove all the files in the test directory, and then cd .. and remove
 * the test directory itself.
 */
int remove_testdir(files, ignore_errors)
    int files;
    int ignore_errors;
{
    int i;
    char name[MAXPATHLEN];

    for (i=0; i<files; i++) {
	sprintf(name, "%s.%d", TFNAME, i);
	if (unlink(name) < 0 && !ignore_errors) {
	    perror("unlink");
	    return(1);
	}
    }

    if (chdir("..") < 0) {
	perror("chdir .. failed");
	return(1);
    }

    if (rmdir(TDNAME) < 0 && !ignore_errors) {
	perror("rmdir");
	return(1);
    }
    return(0);
}

void timer_value(tv)
    struct timeval *tv;
{
    if (tv_end.tv_usec < tv_start.tv_usec) {
	tv_end.tv_sec--;
	tv_end.tv_usec += 1000000;
    }
    tv->tv_usec = tv_end.tv_usec - tv_start.tv_usec;
    tv->tv_sec = tv_end.tv_sec - tv_start.tv_sec;
}

#if	!defined(__hpux)
/*
 * Compute net usage
 */
#if 	defined(SUNOS5)
void aux_net_usage(utp, stp)
    double	*utp, *stp;
{
    int 	u_tcks, s_tcks;
    double 	ticks_per_second;

    ticks_per_second = CLK_TCK;
    /*
     * Get end time values for user and system ticks.
     */
    u_tcks = r_buf_end.tms_utime;
    s_tcks = r_buf_end.tms_stime;

    /*
     * Subtract off start time vaules.
     */
    u_tcks -= r_buf_start.tms_utime;
    s_tcks -= r_buf_start.tms_stime;

    /*
     * Represent as floating point seconds
     */
    *utp = (double)u_tcks / ticks_per_second;
    *stp = (double)s_tcks / ticks_per_second;
    return;
}
#else /* NOT SUNOS5 */
void aux_net_usage(utp, stp)
    double	*utp, *stp;
{
    int 	u_usec, s_usec;

    /*
     * Get end time rusage values for user and system microseconds
     */
    u_usec = (r_buf_end.ru_utime.tv_sec * 1000000) +
	r_buf_end.ru_utime.tv_usec;
    s_usec = (r_buf_end.ru_stime.tv_sec * 1000000) +
	r_buf_end.ru_stime.tv_usec;

    /*
     * Subtract off start time rusage vaules for user
     * and system microseconds.
     */
    u_usec -= ((r_buf_start.ru_utime.tv_sec * 1000000) +
	r_buf_start.ru_utime.tv_usec);
    s_usec -= ((r_buf_start.ru_stime.tv_sec * 1000000) +
	r_buf_start.ru_stime.tv_usec);

    /*
     * Represent as floating point seconds
     */
    *utp = (double)u_usec / 1000000;
    *stp = (double)s_usec / 1000000;
    return;
}
#endif	/* SUNOS5 */

/*
 * Start usage timer
 */
int aux_usage_start()
{
#if	defined(SUNOS5)
    if (times(&r_buf_start) < 0)
#else
    if (getrusage(RUSAGE_SELF, &r_buf_start) < 0)
#endif
	return(1);
    else
	return(0);
}

/*
 * Stop usage timer
 */
int aux_usage_end()
{
#if	defined(SUNOS5)
    if (times(&r_buf_end) < 0)
#else
    if (getrusage(RUSAGE_SELF, &r_buf_end) < 0)
#endif
	return(1);
    else
	return(0);
}
#endif	/* !hpux */
