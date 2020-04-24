/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtstimed.c,v $
 * Revision 1.1.8.2  1996/02/17  23:35:10  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:21  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:12:52  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/30  22:44 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/2  1994/11/22  13:26 UTC  pare
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/1  1994/11/22  13:23 UTC  pare
 * 	Wait for shared memory id to be created by dtsd
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  12:50 UTC  pare
 * 	Merge hpdce02 (DCE1.1 beta) and HPDCE01 into HPDCE02
 * 	[1995/12/08  17:25:14  root]
 * 
 * Revision 1.1.6.1  1994/06/10  20:52:28  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:53  devsrc]
 * 
 * Revision 1.1.6.2  1993/07/08  19:54:46  root
 * 	Initial King Kong branch
 * 	[1993/07/08  19:54:17  root]
 * 
 * Revision 1.1.4.5  1993/05/28  13:08:14  truitt
 * 	Corrected bonehead mistake in version string.
 * 	[1993/05/28  13:07:43  truitt]
 * 
 * Revision 1.1.4.4  1993/05/25  12:45:37  truitt
 * 	Add new version control string.
 * 	[1993/05/25  12:45:19  truitt]
 * 
 * Revision 1.1.4.3  1993/05/10  18:25:50  truitt
 * 	Add version control string.
 * 	[1993/05/10  18:25:07  truitt]
 * 
 * Revision 1.1.4.2  1993/03/24  18:49:16  truitt
 * 	Initial GAMERA submit.
 * 	[1993/03/24  18:48:36  truitt]
 * 
 * Revision 1.1.2.3  1992/12/07  14:43:57  smythe
 * 
 * 	changed file name from adjtimed.c to dtstimed.c
 * 	changed daemon name from adjtimed to dtstimed
 * 	added SIGINT terminate handler for debug mode
 * 	changed > to >= and < to <= in minimum change check
 * 	added pending_change accumulator for small requests
 * 	CHFts06362 and CHFts06361
 * 	[1992/12/07  14:42:30  smythe]
 * 
 * Revision 1.1.2.2  1992/12/07  13:47:28  jake
 * 
 * 	First entry of dtstimed.c for adjtimed.c
 * 	jake for the smythe-man
 * 	[1992/12/07  13:46:49  jake]
 * 
 * Revision 1.1.2.4  1992/10/30  19:48:05  smythe
 * 
 * 	rewritten and simplified for hpux dce dts
 * 	[1992/10/30  19:44:47  smythe]
 * 
 * Revision 1.1.2.3  1992/07/09  17:21:31  smythe
 * 
 * 	changed #include "adjtime.h" to #include <adjtime.h>
 * 	added protections against large time or rate changes
 * 	[1992/07/09  17:21:03  smythe]
 * 
 * Revision 1.1.2.2  1992/06/26  20:14:31  smythe
 * 
 * 	adjtime daemon which emulates the adjtime call on hpux
 * 	[1992/06/26  20:13:45  smythe]
 * 
 * $EndLog$
 */

/*************************************************************************/
/* (c) Copyright Tai Jin, 1988.  All Rights Reserved.                    */
/*     Hewlett-Packard Laboratories.                                     */
/*                                                                       */
/* Permission is hereby granted for unlimited modification, use, and     */
/* distribution.  This software is made available with no warranty of    */
/* any kind, express or implied.  This copyright notice must remain      */
/* intact in all versions of this software.                              */
/*                                                                       */
/* The author would appreciate it if any bug fixes and enhancements were */
/* to be sent back to him for incorporation into future versions of this */
/* software.  Please send changes to tai@iag.hp.com or ken@sdd.hp.com.   */
/*************************************************************************/

/*  This dtstime daemon (dtstimed) makes gradual clock corrections
 *  on hpux 8.0 and 9.0.  It works like the adjtime system call, which
 *  hpux does not currently support but may support in the future.  The
 *  calling stub communicates with the daemon through hpux sys5 message
 *  queues.  The daemon uses an undocumented kernel variable, so it may
 *  not work on future hpux releases.
 *
 *  The daemon has been rewritten and simplified for hpux dce dts.
 *  No leftovers from the previous correction are returned.  The rate
 *  of correction is fixed at four percent.  Accuracy is limited to
 *  about half a millisecond, and finer corrections are saved and
 *  applied to future corrections above this adjustment threshold.
 *  The maximum correction is ~2000 seconds (giving a maximum correction
 *  period of ~14 hours).  Requests for larger corrections yield only the
 *  maximum correction.  In debug mode (-d), the daemon runs unforked in
 *  the foreground and prints diagnostic information.  Error information
 *  is always posted to syslog.  Kill allows the daemon to clean up and
 *  reset the clock increment, but kill -9 may leave the clock rate fast
 *  or slow.  Interference will result if more than one program which
 *  attempts to adjust the time (dts, ntp, or timed) is run concurrently.
 *
 *  The system oscillator produces regular timing interrupt ticks.
 *  Kernel software advances the clock at each tick by a quantity of
 *  microseconds stored in a kernel increment variable.  Changing 
 *  the kernel increment variable for a number of ticks (counted
 *  by a kernel tick timer) advances the clock at a different rate,
 *  gradually accomplishing the requested change.
 */

#include <sys/param.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <signal.h>
#include <nlist.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <syslog.h>
#include "dtstime.h"

static void adjust_clock();
static void set_increment();
static void reset_increment();
static void terminate();

/* defined in <sys/param.h>:  HZ  ticks per second (machine dependent) */

#define adjustment_rate  40000L  /* microseconds per second */
#define maximum_change    2000L  /* seconds */

static int mqid;
static int kmem;
static int debug = 0; 
static int terminating = 0;
static struct nlist nl[] = { { "old_tick" }, { "" } };
static long tick_period = 1000000L / (long) HZ;  /* microseconds per tick */

void main(argc, argv)
  int argc;
  char **argv;
{
  msgbuf msg;
  struct sigvec vec;
  long increment;
  long mask;

  if (argc > 1) {
    if (argc == 2 && !strcmp(argv[1], "-d")) debug = 1;
    else {
      fprintf(stderr, "usage: dtstimed -d \n");
      exit(-1);
    }
  }
         
  if (debug) {
    setvbuf(stdout, NULL, _IONBF, NULL);
    setvbuf(stderr, NULL, _IONBF, NULL);
  } else {
    switch (fork()) {
      case 0:
        close(fileno(stdin));
        close(fileno(stdout));
        close(fileno(stderr));
        setpgrp();
        break;
      case -1:
        perror("dtstimed: main: fork");
        exit(-1);
      default:
        exit(0);
    }
  }
  sleep(60);
  openlog("dtstimed", LOG_PID, LOG_LOCAL0);
  syslog(LOG_INFO, "main: start");
  if (debug) printf("dtstimed: main: start \n");

  if ((mqid = msgget(msgkey, 0)) != -1 &&
    msgctl(mqid, IPC_RMID, (struct msqid_ds *) NULL) == -1) {
    syslog(LOG_ERR, "main: msgctl: %m");
    perror("dtstimed: main: msgctl");
    terminate();
  }
  if (msgget(msgkey, IPC_CREAT|IPC_EXCL) == -1) {
    syslog(LOG_ERR, "main: msgget: %m");
    perror("dtstimed: main: msgget");
    terminate();
  }
  if ((mqid = msgget(msgkey, 0)) == -1) {
    syslog(LOG_ERR, "main: mqid: %m");
    perror("dtstimed: main: mqid");
    terminate();
  }

  if ((kmem = open("/dev/kmem", O_RDWR)) == -1) {
    syslog(LOG_ERR, "main: open: %m");
    perror("dtstimed: main: open");
    terminate();
  }
  if (nlist("/hp-ux", nl) == -1) {
    syslog(LOG_ERR, "main: nlist: %m");
    perror("dtstimed: main: nlist");
    terminate();
  }
  if (nl[0].n_type == 0) {
    syslog(LOG_ERR, "main: ntype");
    fprintf(stderr, "dtstimed: main: ntype \n");
    terminate();
  }

  if (lseek(kmem, (long) nl[0].n_value, 0) == -1L) {
    syslog(LOG_ERR, "main: lseek: %m");
    perror("dtstimed: main: lseek");
    terminate();
  }
  mask = sigblock(sigmask(SIGALRM));
  if (read(kmem, (caddr_t)&increment, sizeof(long)) != sizeof(long)) {
    sigsetmask(mask);
    syslog(LOG_ERR, "main: read: %m");
    perror("dtstimed: main: read");
    terminate();
  }
  sigsetmask(mask);
  if (debug) printf("dtstimed: main: original increment  %d \n", increment);

  (void)signal(SIGHUP, SIG_IGN);
  if (debug) (void)signal(SIGINT, terminate);
  else (void)signal(SIGINT, SIG_IGN);
  (void)signal(SIGQUIT, SIG_IGN);
  (void)signal(SIGTERM, terminate);
  vec.sv_handler = reset_increment;
  vec.sv_flags = 0;
  vec.sv_mask = ~0;
  if (sigvector(SIGALRM, &vec, (struct sigvec *) NULL) == -1) {
    syslog(LOG_ERR, "main: sigvector: %m");
    perror("dtstimed: main: sigvector");
    terminate();
  }

  for (;;) {
    if (msgrcv(mqid, &msg, msgsize, msgkey, 0) == -1) {
      if (errno == EINTR) continue;
      syslog(LOG_ERR, "main: msgrcv: %m");
      perror("dtstimed: main: msgrcv");
      terminate();
    }
    adjust_clock(&msg.delta);
  }
}

static void adjust_clock(delta)
  struct timeval *delta;    /* encoded requested change */
{
  struct itimerval period;        /* encoded adjustment period */
  long requested_change;          /* microseconds */
  static long pending_change = 0; /* microseconds */
  long adjusted_increment;        /* microseconds per tick */
  long adjustment_ticks;          /* ticks */
  static long increment_change =  /* microseconds per tick */
    adjustment_rate / (long) HZ;

  if (debug) printf("dtstimed: adjust_clock  delta  %d sec  %d usec \n",
    (long) delta->tv_sec, delta->tv_usec);
  if ((long) delta->tv_sec > maximum_change )
    delta->tv_sec = (unsigned long) maximum_change;
  if ((long) delta->tv_sec < -maximum_change )
    delta->tv_sec = (unsigned long) -maximum_change;
  requested_change = ((long) delta->tv_sec * 1000000L)
    + delta->tv_usec + pending_change;
  pending_change = requested_change % increment_change;
  if (debug) printf("dtstimed: adjust_clock  pending change  %d usec \n",
    pending_change);
  if (requested_change >= increment_change) {
    adjustment_ticks = requested_change / increment_change;
    adjusted_increment = tick_period + increment_change;
  } else if (requested_change <= -increment_change) {
    adjustment_ticks = (-requested_change) / increment_change;
    adjusted_increment = tick_period - increment_change;
  } else {
    adjustment_ticks = 0;
    adjusted_increment = tick_period;
  };
  period.it_value.tv_sec = (unsigned long) (adjustment_ticks / (long) HZ);
  period.it_value.tv_usec = (adjustment_ticks % (long) HZ) * tick_period;
  period.it_interval.tv_sec = (unsigned long) 0L;
  period.it_interval.tv_usec = 0L;
  if (debug) printf("dtstimed: adjust_clock  period  %d sec  %d usec \n",
    (long) period.it_value.tv_sec, period.it_value.tv_usec);
  set_increment(adjusted_increment);
  setitimer(ITIMER_REAL, &period, (struct itimerval *) NULL);
}

static void set_increment(increment)
  long increment;
{
  long mask;

  if (debug) printf("dtstimed: set_increment  %d \n", increment);
  if (lseek(kmem, (long) nl[0].n_value, 0) == -1L) {
    syslog(LOG_ERR, "set_increment: lseek: %m");
    perror("dtstimed: set_increment: lseek");
    if (terminating) return; else terminate();
  }
  mask = sigblock(sigmask(SIGALRM));
  if (write(kmem, (caddr_t)&increment, sizeof(long)) != sizeof(long)) {
    sigsetmask(mask);
    syslog(LOG_ERR, "set_increment: write: %m");
    perror("dtstimed: set_increment: write");
    if (terminating) return; else terminate();
  }
  sigsetmask(mask);
}

static void reset_increment()
{
  struct itimerval it;

  if (debug) printf("dtstimed: reset_increment \n");
  it.it_value.tv_sec = (unsigned long) 0L;
  it.it_value.tv_usec = 0L;
  setitimer(ITIMER_REAL, &it, (struct itimerval *) NULL);
  set_increment(tick_period); 
} 

static void terminate()
{
  if (debug) printf("dtstimed: terminate \n");
  syslog(LOG_INFO, "terminate");
  terminating = 1;
  reset_increment();
  close(kmem);
  msgctl(mqid, IPC_RMID, (struct msqid_ds *) NULL); 
  closelog();
  exit(-1);
}

