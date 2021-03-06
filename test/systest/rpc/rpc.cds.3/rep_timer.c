/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rep_timer.c,v $
 * Revision 1.1.75.2  1996/02/17  23:27:33  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:54  marty]
 *
 * Revision 1.1.75.1  1995/12/11  22:42:28  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/04  20:12 UTC  bissen
 * 	merge from HPDCE01
 * 	[1995/12/11  22:19:46  root]
 * 
 * Revision 1.1.72.2  1993/10/20  19:36:48  ohara
 * 	svr4 ref port
 * 	[1993/10/20  19:35:42  ohara]
 * 
 * Revision 1.1.72.1  1993/09/14  15:01:07  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/14  15:00:29  cmckeen]
 * 
 * Revision 1.1.6.2  1993/08/19  15:42:08  bissen
 * 	Fixed for hp-ux
 * 	[1993/08/19  15:41:16  bissen]
 * 
 * Revision 1.1.2.2  1993/03/12  04:26:28  mhickey
 * 	Final checkin for initial submit to 1.0.2 backing tree
 * 
 * 	Defect 5887.
 * 	[1993/03/11  21:38:50  mhickey]
 * 
 * $EndLog$
 */

/*---*
 *---* History from before name change
 *---*
 * Revision 1.1.1.2  1993/01/29  20:34:57  mhickey
 * 	Inital check in for the rpc.cds.3 system test.
 * 	~
 *
 * Revision 1.1.2.2  1993/01/22  19:59:16  mhickey
 * 	Initial checkin
 * 	[1993/01/19  20:41:14  mhickey]
 * 
 *---*
 *---*/

/*---*
 *---* rep_timer.c - uses pthread_cond_timedwait to time report generation.
 *---*
 *---* ASSOCIATION
 *---*    This file is part of the DCE 1.0.2 System Tests
 *---*
 *---*    The DCE 1.0.2 system tests corresponds to the 
 *---*    DCE1.0.2 Core Component System Test plan.  For more information on 
 *---*    the operation and goals of the DCE 1.0.2 system tests, see the test 
 *---*    plan.
 *---* 
 *---* FUNCTION
 *---*    Takes an interval as an argument, and delays that number of seconds
 *---*    between status reports (generated by calling rep_gen_func_p()
 *---*
 *---* CALLED BY
 *---*    Program main threads.
 *---* 
 *---* CALLS INTO 
 *---*    'C' library
 *---*
 *---* PREREQUISITES
 *---*    Assumes the external definition of a pointer rep_gen_func_p to a
 *---*    routine  that takes an integer as an arguement.  Also assumes that
 *---*    the variables defined in the section "the following are expected..."
 *---*    below are defined externally (they're not passed in order to keep the
 *---*    the arg list reasonable.  It would also be easy to define a struct
 *---*    containing all of the variables now defined as extern as well as the
 *---*    interval, and to pass it into the routine instead of just the 
 *---*    interval.
 *---*/

#include <stdio.h>
#include <sys/time.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <pthread.h>
#include "../rpc.sec.2/rpctest.h"
#ifdef SNI_SVR4
#include <rpc.cds.3_defs.h>
#endif

#include <ctime_nonl.h>

/*---* 
 *---* the following are expected to be defined externally
 *---*/
extern pthread_mutex_t rep_timer_mutex;
extern pthread_cond_t rep_timer_cond;
extern int debug;
extern void (*rep_gen_func_p)();

/*---*
 *---* rep_timer(interval)
 *---*
 *---* call the report generator at the specfied interval
 *---*/
void 
rep_timer(int *interval)
{
   unsigned32           status;      /*---* for function returns        *---*/
   signed32             next;        /*---* time of first report *---*/
   struct timespec      now,         /*---* the current time            *---*/
                        wakeup;      /*---* time when we wnat to wakeup *---*/
   char                 *gcp;        /*---* g-eneric c-har p-ointer     *---*/
   int pid=getpid();

   if (debug)
      printf("rep_timer(%d): Hi from rep_timer...interval is %d minutes.\n", 
                                                                  pid,
                                                               (*interval)/60);
   /*---* get the current time... *---*/
   getclock(TIMEOFDAY, &now);
   next= (signed32)(now.tv_sec + *interval);
   wakeup.tv_sec = next;
   wakeup.tv_nsec = 0;

   CTIME_NO_NL(gcp, &(now.tv_sec))
   printf("rep_timer(%d): Starting report generation at %s...\n", pid, gcp);
   CTIME_NO_NL(gcp, &next)
   printf("rep_timer(%d): First report at %s...\n", pid, gcp);
   pthread_mutex_lock(&rep_timer_mutex);
   while(1)
   {
      while ( now.tv_sec < next )
      {
         /*---* timedwait waits till we need to refresh *---*/
         pthread_cond_timedwait(&rep_timer_cond, &rep_timer_mutex, &wakeup);

         /*---* and find out what time it is for the while condition *---*/
         getclock(TIMEOFDAY, &now);
      }
      
      /*---* 
       *---* time to report
       *---*/
      if (debug)
      {
         CTIME_NO_NL(gcp, &(now.tv_sec))
         printf("rep_timer(%d): printing status report at %s...", pid, gcp);
      }
      (*rep_gen_func_p)(0);

      if (debug)
         printf("\ndone.\n");

      /*---* reset next report time *---*/
      next = now.tv_sec + *interval;
      wakeup.tv_sec = next;

   }
}


