/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: timing.c,v $
 * Revision 1.1.7.2  1996/02/17  23:26:20  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:12  marty]
 *
 * Revision 1.1.7.1  1995/12/11  21:52:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:54:07  root]
 * 
 * Revision 1.1.3.5  1993/03/04  20:43:43  cuti
 * 	Add defined(__hpux) in front of defined(SNI_SVR4_POSIX) 'cause they are all POSIX.
 * 	[1993/03/04  20:01:54  cuti]
 * 
 * Revision 1.1.3.4  1993/02/01  21:33:42  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  21:04:54  hinman]
 * 
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  16:30:37  hinman]
 * 
 * Revision 1.1.3.3  1992/12/31  17:36:04  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:59:47  htf]
 * 	Revision 1.1.4.2  1993/01/11  16:15:52  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.3.2  1992/09/29  21:24:59  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/15  14:56:19  sekhar]
 * 
 * Revision 1.1  1992/01/19  04:08:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  timing.c V=3 04/29/91 //littl/prgy/src/client/tests_dir
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * 
 * Timing Module:
 *  Used by some tests to determine the amount of time taken for certain
 *  remote operations.
 */ 
#ifdef SNI_SVR4
#include <unistd.h>
#endif /* SNI_SVR4 */
#include <timing.h>
#include <un_maloc.h>
#include <stdio.h>

struct TIMINGinfo {
    char *blurb;
    int  start_count, stop_count;
    struct timeval timer;
    struct timeval sys_timer;
    struct timeval total_time;
    struct timeval wall_time;
    struct timeval total_wall_time;
    double  last_print_sys;
    double  last_print_wall;
    int     last_print_count;
};

#define TICKS 1000000

static int TIMINGdisplay = 0;

#define dprintf if (TIMINGdisplay) printf

void TIMINGecho
#ifndef __STDC__
    ( flag ) 
    int flag;
#else
  (
    int flag
  )
#endif
{
    TIMINGdisplay = flag;
}

struct TIMINGinfo *
TIMINGregister
#ifndef __STDC__
    (blurb)
    char *blurb;
#else
  (
    char *blurb
  )
#endif
{
    struct TIMINGinfo *t;

    t = (struct TIMINGinfo *) malloc(sizeof(*t));
    if (t != NULL) {
        t->start_count = t->stop_count = 0;
        t->blurb = blurb;
        t->total_time.tv_sec = 0;
        t->total_time.tv_usec = 0;
        t->total_wall_time.tv_sec = 0;
        t->total_wall_time.tv_usec = 0;
        t->last_print_sys = 0.0;
        t->last_print_wall = 0.0;
        t->last_print_count = 0;
    }
    return t;
}

#if defined(__hpux) || defined(SNI_SVR4_POSIX)


void TIMINGstart
#ifndef __STDC__
    (timer)
    struct TIMINGinfo *timer;
#else
  (
    struct TIMINGinfo *timer
  )
#endif
{
    struct timezone tz;
    struct tms ru;
    unsigned int ticks_svr4;

    timer->start_count++;
#if defined(__hpux)
    gettimeofday(&timer->wall_time, &tz);

    ticks_svr4 = sysconf(CLK_TCK);
#else 
    gettimeofday(&timer->wall_time);

    /* get ticks per second */

    ticks_svr4 = sysconf(_SC_CLK_TCK);
#endif

    times(&ru);

    /*
     * tms values are reported in clock ticks.
     * TICKS is the number of microseconds per second,
     * ticks_svr4 is the number of clock ticks per second -- rcc
     */

    timer->timer.tv_sec = ru.tms_utime / ticks_svr4;
    timer->timer.tv_usec = (ru.tms_utime % ticks_svr4) * TICKS / ticks_svr4;

    timer->sys_timer.tv_sec = ru.tms_stime / ticks_svr4;
    timer->sys_timer.tv_usec = (ru.tms_stime % ticks_svr4) * TICKS / ticks_svr4;
}


#else

void TIMINGstart
#ifndef __STDC__
    (timer)
    struct TIMINGinfo *timer;
#else
  (
    struct TIMINGinfo *timer
  )
#endif
{
    struct timezone tz;
    struct rusage ru;

    timer->start_count++;
    gettimeofday(&timer->wall_time, &tz);
    getrusage(RUSAGE_SELF, &ru);
    timer->timer = ru.ru_utime;
    timer->sys_timer = ru.ru_stime;
}
#endif /* SNI_SVR4_POSIX */

void TIMINGstop
#ifndef __STDC__
    (timer)
    struct TIMINGinfo *timer;
#else
  (
    struct TIMINGinfo *timer
  )
#endif
{
    struct timezone tz;
    struct timeval  wall_time;
#if defined(__hpux) || defined(SNI_SVR4_POSIX)
    struct tms ru;
    unsigned int ticks_svr4;
#else
    struct rusage   ru;
#endif /* SNI_SVR4_POSIX */
    long            seconds;
    long            micro_seconds;

#if defined(__hpux) || defined(SNI_SVR4_POSIX)
    /* get ticks per second */
#if defined(__hpux)
    ticks_svr4 = sysconf(CLK_TCK);
#else
    ticks_svr4 = sysconf(_SC_CLK_TCK);
#endif

    times(&ru);
#else
    getrusage(RUSAGE_SELF, &ru);
#endif /* SNI_SVR4_POSIX */

#ifdef SNI_SVR4
    gettimeofday(&wall_time);
#else
    gettimeofday(&wall_time, &tz);
#endif /*SNI_SVR4 */

#if defined(__hpux) || defined(SNI_SVR4_POSIX)
    seconds = (ru.tms_utime / ticks_svr4) - timer->timer.tv_sec;
    micro_seconds = ((ru.tms_utime % ticks_svr4) * TICKS / ticks_svr4)
				- timer->timer.tv_usec;
#else
    seconds = ru.ru_utime.tv_sec - timer->timer.tv_sec;
    micro_seconds = ru.ru_utime.tv_usec - timer->timer.tv_usec;
#endif /* SNI_SVR4_POSIX */

    if (micro_seconds < 0) {
        seconds--;
        micro_seconds += TICKS;
    }
    dprintf("Elapsed time: ");
    dprintf("[%d.%06d] user ", seconds, micro_seconds);


    timer->total_time.tv_sec += seconds;
    timer->total_time.tv_usec += micro_seconds;
    if (timer->total_time.tv_usec > TICKS) {
        timer->total_time.tv_sec++;
        timer->total_time.tv_usec -= TICKS;
    }

    timer->total_time.tv_sec += seconds;
    timer->total_time.tv_usec += micro_seconds;
    if (timer->total_time.tv_usec > TICKS) {
        timer->total_time.tv_sec++;
        timer->total_time.tv_usec -= TICKS;
    }

#if defined(__hpux) || defined(SNI_SVR4_POSIX)
    seconds = (ru.tms_stime / ticks_svr4) - timer->sys_timer.tv_sec;
    micro_seconds = ((ru.tms_stime % ticks_svr4) * TICKS / ticks_svr4)
			- timer->sys_timer.tv_usec;
#else
    seconds = ru.ru_stime.tv_sec - timer->sys_timer.tv_sec;
    micro_seconds = ru.ru_stime.tv_usec - timer->sys_timer.tv_usec;
#endif /* SNI_SVR4_POSIX */

    if (micro_seconds < 0) {
        seconds--;
        micro_seconds += TICKS;
    }
    dprintf("[%d.%06d] sys ", seconds, micro_seconds);

    timer->total_time.tv_sec += seconds;
    timer->total_time.tv_usec += micro_seconds;
    if (timer->total_time.tv_usec > TICKS) {
        timer->total_time.tv_sec++;
        timer->total_time.tv_usec -= TICKS;
    }

    seconds = wall_time.tv_sec - timer->wall_time.tv_sec;
    micro_seconds = wall_time.tv_usec - timer->wall_time.tv_usec;
    if (micro_seconds < 0) {
        seconds--;
        micro_seconds += TICKS;
    }
    dprintf("[%d.%06d] real time\n", seconds, micro_seconds);

    timer->total_wall_time.tv_sec += seconds;
    timer->total_wall_time.tv_usec += micro_seconds;
    if (timer->total_wall_time.tv_usec > TICKS) {
        timer->total_wall_time.tv_sec++;
        timer->total_wall_time.tv_usec -= TICKS;
    }

    timer->stop_count++;
}

void TIMINGprint
#ifndef __STDC__
    (timer)
    struct TIMINGinfo *timer;
#else
  (
    struct TIMINGinfo *timer
  )
#endif
{
    double real_sec;
    double wall_sec;
    double total_wall_sec;
    double total_real_sec;
    int period_count;

    printf("TIMING: %s [%d.%06.6d]user+sys [%d.%06.6d] real time (%d in, %d out)\n",timer->blurb,
             timer->total_time.tv_sec, timer->total_time.tv_usec,
             timer->total_wall_time.tv_sec,
             timer->total_wall_time.tv_usec,
             timer->start_count, timer->stop_count);
    total_real_sec = (double) timer->total_time.tv_sec;
    total_real_sec = total_real_sec + (((double) timer->total_time.tv_usec) / TICKS);
    real_sec = total_real_sec / timer->start_count;
    total_wall_sec = ((double) timer->total_wall_time.tv_sec);
    total_wall_sec = total_wall_sec + (((double) timer->total_wall_time.tv_usec) / TICKS);
    wall_sec = total_wall_sec / timer->start_count;
    printf("TIMING: (Per call aggregate) [%.6f]user+sys [%.6f] real time\n",
            real_sec, wall_sec);
    period_count = timer->start_count - timer->last_print_count;
    real_sec = total_real_sec - timer->last_print_sys;
    real_sec = real_sec / period_count;
    wall_sec = total_wall_sec - timer->last_print_wall;
    wall_sec = wall_sec / period_count;
    printf("TIMING: (Per call period %02.2d) [%.6f]user+sys [%.6f] real time\n",
            period_count, real_sec, wall_sec);
    timer->last_print_sys = total_real_sec;
    timer->last_print_wall = total_wall_sec;
    timer->last_print_count = timer->start_count;
}
