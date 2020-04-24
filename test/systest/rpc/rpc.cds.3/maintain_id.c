/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: maintain_id.c,v $
 * Revision 1.1.16.2  1996/02/17  23:27:27  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:51  marty]
 *
 * Revision 1.1.16.1  1995/12/11  22:42:22  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/08  20:33 UTC  bissen
 * 	merge from HPDCE01
 * 	[1995/12/11  22:19:42  root]
 * 
 * Revision 1.1.13.2  1994/02/23  21:43:53  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:39:46  pellis]
 * 
 * Revision 1.1.13.1  1993/12/22  20:51:13  mhickey
 * 	no_changes
 * 	[1993/12/22  20:50:11  mhickey]
 * 
 * 	Changed the kill_threads func to cleanup func
 * 	[1993/12/20  19:50:36  mhickey]
 * 
 * Revision 1.1.11.2  1993/11/02  22:27:47  ohara
 * 	svr4 porting changes
 * 	[1993/11/02  21:21:25  ohara]
 * 
 * Revision 1.1.11.1  1993/09/14  15:01:05  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/14  15:00:28  cmckeen]
 * 
 * Revision 1.1.8.2  1993/08/24  13:44:17  eheller
 * 	Added include of rptest.h
 * 	[1993/08/23  18:17:33  eheller]
 * 
 * Revision 1.1.2.2  1993/03/12  04:26:07  mhickey
 * 	Final checkin for initial submit to 1.0.2 backing tree
 * 
 * 	Defect 5887.
 * 	[1993/03/11  21:38:32  mhickey]
 * 
 * $EndLog$
 */

/*---*
 *---* History from before the rename 
 *---*
 * Revision 1.1.1.2  1993/01/29  20:32:52  mhickey
 * *** empty log message ***
 *
 * Revision 1.1.1.4  1993/01/21  21:05:24  mhickey
 * 	added a warning message if the wakeup time indicates
 * 	we are not getting enough timeslice.
 * 
 * Revision 1.1.1.3  1993/01/21  16:58:13  mhickey
 * 	Added a call to rpcsec2_rep_gen before exit if we
 * 	can't validate id.
 * 
 * Revision 1.1.1.2  1993/01/19  20:39:37  mhickey
 * 	Initial checkin of extracted id maintainence routine.
 * 
 *---*
 *---*/

/*---*
 *---* maintain_id.c - uses pthread_cond_timedwait to time
 *---*                           id refreshes.
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
 *---*    between id refreshes and validations.  Updates count of refreshes
 *---*    and last refresh time variables, and if unsuccessful, calls the
 *---*    routine pointed to by rep_gen_func_p to print a report and then
 *---*    calls the function pointed to by cleanup_func_p to kill any
 *---*    outstanding threads managed by the application before exiting.
 *---*
 *---* CALLED BY
 *---*    Main program threads.
 *---* 
 *---* CALLS INTO 
 *---*    'C' library
 *---*    kill_thds_func_p, and rep_gen_func_p
 *---*
 *---* PREREQUISITES
 *---*    Assumes the existence of a pointer to a void function called
 *---*    "kill_thds_func_p" that kills all but the calling thread (which is
 *---*    passed as an arguement), and a pointer to a void function called
 *---*    "rep_gen_func_p" that takes an int as an arguement.  Also assumes 
 *---*    that the variables
 *---*    defined in the section "the following are expected..." below are
 *---*    defined externally (they are not passed in order to keep the 
 *---*    the arg list reasonalble.  It would also be easy to define a struct
 *---*    containing all of the variables now defined as extern as well as 
 *---*    the lifetime, and to pass it into the routine instead of just the 
 *---*    lifetime.
 *---*/

#pragma HP_ALIGN HPUX_WORD

/*---*
 *---* Includes
 *---*/
#include <stdio.h>
#include <sys/time.h>
#include <dce/dce.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <pthread.h>
#include "../rpc.sec.2/rpctest.h"

#include <err_track.h>
#include <ctime_nonl.h>

#ifdef SNI_SVR4
#include <rpc.cds.3_defs.h>
#endif

/*---*
 *---* maintain_id(interval)
 *---*
 *---* Try to refresh our identity a minute before it expires
 *---*/

/*---* the following are assumed to be defined elsewhere *---*/
extern void *sec_key_p;
extern char des_key[];
extern err_stat_p_t err_stats[];
extern int nerr_stats;
extern int debug;
extern time_t last_id_refr;
extern int id_refr_cnt;
extern sec_login_handle_t login_ctxt; /*---* dce login ctxt for auth rpc *---*/
extern pthread_mutex_t refr_id_mutex;
extern pthread_cond_t refr_id_cond;
extern void (*rep_gen_func_p)();
extern void (*cleanup_func_p)();



void 
maintain_id(int *lifetime)
{
   unsigned32           status;      /*---* for function returns        *---*/
   signed32             id_exp_time; /*---* when id expires             *---*/
   int                  refreshes;   /*---* number of refreshes         *---*/
   struct timespec      now,         /*---* the current time            *---*/
                        wakeup;      /*---* time when we wnat to wakeup *---*/
   char                 *gcp;        /*---* g-eneric c-har p-ointer     *---*/
   boolean32            reset_pw;    /*---* did password get reset      *---*/
   sec_login_auth_src_t auth_src;    /*---* Returns auth source         *---*/
   int pid=getpid();
   int i;

   memcpy(
         (char *)(((sec_passwd_rec_t *)sec_key_p)->key.tagged_union.des_key),
         des_key, 8);

   if (debug)
      dump_key(sec_key_p);

   /*---* get the current time... *---*/
   getclock(TIMEOFDAY, &now);
   id_exp_time = (signed32)(now.tv_sec + *lifetime);
   wakeup.tv_sec = id_exp_time;
   wakeup.tv_nsec = 0;

   CTIME_NO_NL(gcp, &(now.tv_sec))
   printf("maintain id(%d): Starting id refreshes at %s...\n", pid, gcp);
   CTIME_NO_NL(gcp, &id_exp_time)
   printf("maintain id(%d): First id update at %s...\n", pid, gcp);
   pthread_mutex_lock(&refr_id_mutex);
   while(1)
   {
      while ( now.tv_sec < id_exp_time )
      {
         /*---* timedwait waits till we need to refresh *---*/
         pthread_cond_timedwait(&refr_id_cond, &refr_id_mutex, &wakeup);

         /*---* and find out what time it is for the while condition *---*/
         getclock(TIMEOFDAY, &now);
       
#if defined(MAINTAIN_ID_REFR_DEBUG)
         /*---* 
          *---* warn of impending doom if we woke up much later than we wanted
          *---* to
          *---*/
         i = now.tv_sec - id_exp_time;
         CTIME_NO_NL(gcp, &(now.tv_sec))
         printf("maintain id(%d):  %s: woke up at %s, %d seconds late...\n",
                   pid, i < 60? "trouble": "I think we are going down", 
                   gcp, i);
#endif /*---*  defined(MAINTAIN_ID_REFR_DEBUG) *---*/
      }
      
#if defined(MAINTAIN_ID_REFR_DEBUG)
      id_exp_time = now.tv_sec;
#endif /*---*  defined(MAINTAIN_ID_REFR_DEBUG) *---*/

      /*---* 
       *---* at this point, we are at or within a one minute buffer before 
       *---* identity expiration.  Do the refresh first...
       *---*/
      if (debug)
      {
         CTIME_NO_NL(gcp, &(now.tv_sec))
         printf("maintain id(%d): Trying to refresh our identity at %s...", 
                                                                pid, gcp);
      }
      /*---* we need to refresh our identity. Give it a shot *---*/
      sec_login_refresh_identity(login_ctxt, &status);
      /*---* if we failed... *---*/
      if (rpcsec2_check_stat(status, "refreshing identity", err_stats,
                                                     nerr_stats, debug, 1))
      {
         /*---* We're shutout, flat busted, croaked *---*/
         printf("maintain id(%d): Can't refresh identity!!\n",pid);
      }
      else
      {
         if (debug)
            printf("maintain id(%d): Aaaah! we are REFRESHED.\n",pid);
         refreshes++;
   
         /*---*
          *---* we may need to validate our id, so just in case
          *---*/
         if (debug)
	 {
            printf("maintain id(%d): validating login context...", pid);
            dump_key((sec_passwd_rec_t *)sec_key_p);
            printf("maintain id(%d): des_key = %s\n",pid,des_key);
	 }
         if (memcmp(des_key, 
           (void *)(((sec_passwd_rec_t *)sec_key_p)->key.tagged_union.des_key),
           8))
	 {
            if (debug)
               printf("maintain id(%d): des_key is gone.\n",pid);
            memcpy(
           (char *)(((sec_passwd_rec_t *)sec_key_p)->key.tagged_union.des_key),
            des_key, 8);
            if (debug)
               dump_key((sec_passwd_rec_t *)sec_key_p);
         }
         sec_login_validate_identity(login_ctxt, sec_key_p, &reset_pw,
                                                        &auth_src, &status);
         /*---* 
          *---* get the time again to figure next update as closely as possible
          *---*/
         getclock(TIMEOFDAY, &now);

         /*---*
          *---* ???? need to do something about expired passwd here ????
          *---*/
         if (rpcsec2_check_stat(status, "validating login context",
                                         err_stats, &nerr_stats, debug, 1)
                                && status != sec_login_s_already_valid  )
	 {
            CTIME_NO_NL(gcp, &(now.tv_sec));
            printf("maintain id(%d): Couldn't validate identity(%s).\n",pid,
                                                                         gcp);
            (*rep_gen_func_p)(-2);
            (*cleanup_func_p)(pthread_self());
            exit(1);
	 }
#if  defined(MAINTAIN_ID_REFR_DEBUG)
         CTIME_NO_NL(gcp, &(now.tv_sec));
         printf("maintain id(%d): refresh complete at %s, (%d sec elapsed)\n",
                                        pid, gcp, now.tv_sec - id_exp_time);
#endif /*---* defined(MAINTAIN_ID_REFR_DEBUG) *---*/
         /*---* update id refresh tracking variables *---*/
         id_refr_cnt++;
         last_id_refr = (time_t)now.tv_sec;
         id_exp_time = now.tv_sec + *lifetime;
         wakeup.tv_sec = id_exp_time;
         wakeup.tv_nsec = 0;
      }
   }
}


