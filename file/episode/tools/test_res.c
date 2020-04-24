/* test_res.c */

/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY
 * $Log: test_res.c,v $
 * Revision 1.1.46.1  1996/10/02  17:42:11  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:54  damon]
 *
 * $EndLog$
 */
/*
 * (C) Copyright 1994, 1992 Transarc Corporation - All Rights Reserved
 */
/*
 * Test program for testing the reservation manager code.
 */

#include <stdio.h>
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/stds.h>
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>
#include <dce/dce_error.h>

#include "fifo.h"
#include "epi_thread.h"
#include "reserve.h"

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/tools/test_res.c,v 1.1.46.1 1996/10/02 17:42:11 damon Exp $")

/* offsets for the command interface stuff */

#define REPEAT_OPT 4
#define INITRES_OPT 5
#define OVERRES_OPT 6
#define ADJUSTSIZE_OPT 7
#define ALLOC_OPT 8
#define NTHREADS_OPT 9
#define MTHREADS_OPT 10
#define QUIET_OPT 11
#define SLEEP_OPT 12

/* run-time defaults */
#ifndef REPEAT_DEF
#define REPEAT_DEF 25
#endif
#ifndef INITRES_DEF
#define INITRES_DEF 10
#endif
#ifndef ADJUSTSIZE_DEF
#define ADJUSTSIZE_DEF 4
#endif
#ifndef ALLOC_DEF
#define ALLOC_DEF 2
#endif 
#ifndef NTHREADS_DEF
#define NTHREADS_DEF 5
#endif
#ifndef MTHREADS_DEF
#define MTHREADS_DEF 1
#endif

#define Printf if (!quiet) printf


struct epis_ReservationData testReservation;
unsigned runners;
osi_dlock_t runnersLock;
int quiet = 0;

struct pidInfo {
    long alloc;				/* # of allocs and deallocs to do */
    unsigned loops;			/* # of times to do it each time */
    long failOK;			/* count failure loops? */
};

/*
 * ProcOne - reserve and un-reserve single (1) elements.
 */
void *ProcOne(void *arg)
  {
      struct pidInfo *piP = arg;
      long code;

      while (piP->loops) {
	  code = epis_Reserve(&testReservation,piP->alloc,1,0);
	  pthread_yield();
	  if (code) {
	      Printf("ProcOne: reserved %d\n",piP->alloc);
	      code=epis_Reserve(&testReservation,-(piP->alloc),1,0);
	      afsl_Assert(code);
	      piP->loops--; /* one fewer successful loops required now */
	      pthread_yield();
	  } else {
	      Printf("ProcOne: failed to reserve %d\n",piP->alloc);
	      if (!piP->failOK) exit (1);
	      piP->loops--;
	  }
      }
      lock_ObtainWrite(&runnersLock);
      runners--;
      osi_Wakeup(&runners);
      lock_ReleaseWrite(&runnersLock);
      pthread_exit(0);
  }

/*
 * ProcTwo - frobnicate with the overall reservation
 */ 
void * ProcTwo(void *arg)
  {
      struct pidInfo *piP = arg;
      long code;
      long res1,res2;
      long orig_res;

      orig_res = epis_QueryTotalUnits(&testReservation);
      while (piP->loops--) {
	  code = piP->alloc;
	  while (code) {
	      code -= epis_AdjustReservationUnits(&testReservation, code);
	      pthread_yield();
	  }
	  Printf("ProcTwo: bump reservation %d\n",piP->alloc);
	  code = -(piP->alloc);
	  while (code) {
	      code -= epis_AdjustReservationUnits(&testReservation, code);
	      pthread_yield();
	  }
      }
      lock_ObtainWrite(&runnersLock);
      runners--;
      osi_Wakeup(&runners);
      lock_ReleaseWrite(&runnersLock);
      pthread_exit(0);
  }

void StartConsumerThreads(int nthreads, int alloc_size, int repeat, int failOK)
{
      long code;
      struct pidInfo *piP;
      pthread_t pid;

      lock_ObtainWrite(&runnersLock);
      runners += nthreads;
      lock_ReleaseWrite(&runnersLock);
      /*
       * we want a large number of threads running through the allocation
       * code simultaneously.
       */
      while (nthreads--) {
	  piP = (struct pidInfo *)osi_Alloc(sizeof(struct pidInfo));
	  piP->alloc = alloc_size;
	  piP->loops = repeat;
	  piP->failOK = failOK;
	  code = epi_ThreadCreate(ProcOne, 16000, EPI_NORMAL_PRIORITY,
				   (void *)piP, &pid);
	  afsl_MBZ(code);
      }
}

void StartManagerThreads(int nthreads, int adjust_size, int repeat)
{
      long code;
      struct pidInfo *piP;
      pthread_t pid;

      lock_ObtainWrite(&runnersLock);
      runners += nthreads;
      lock_ReleaseWrite(&runnersLock);
      while (nthreads--) {
	  piP = (struct pidInfo *)osi_Alloc(sizeof(struct pidInfo));
	  piP->alloc = adjust_size;
	  piP->loops = repeat;
	  piP->failOK = 0;
	  code = epi_ThreadCreate(ProcTwo, 16000, EPI_NORMAL_PRIORITY,
				   (void *)piP, &pid);
	  afsl_MBZ(code);
      }
}

int RunTest(struct cmd_syndesc *as, char *arock)
{
      long code;
      int repeat, initial_res,over_res,adjust_size, alloc_size, nthreads;
      int mthreads;
      struct pidInfo *piP;

      repeat = as->parms[REPEAT_OPT].items ?
	  atoi(as->parms[REPEAT_OPT].items->data) : REPEAT_DEF;

      initial_res = as->parms[INITRES_OPT].items ?
	  atoi(as->parms[INITRES_OPT].items->data) : INITRES_DEF;

      over_res = (as->parms[OVERRES_OPT].items != 0);

      adjust_size = as->parms[ADJUSTSIZE_OPT].items ?
	  atoi(as->parms[ADJUSTSIZE_OPT].items->data) : ADJUSTSIZE_DEF;

      alloc_size = as->parms[ALLOC_OPT].items ?
	  atoi(as->parms[ALLOC_OPT].items->data) : ALLOC_DEF;

      nthreads = as->parms[NTHREADS_OPT].items ?
	  atoi(as->parms[NTHREADS_OPT].items->data) : NTHREADS_DEF;

      mthreads = as->parms[MTHREADS_OPT].items ?
	  atoi(as->parms[MTHREADS_OPT].items->data) : MTHREADS_DEF;
	  

      quiet = (as->parms[QUIET_OPT].items != 0);

      if (as->parms[SLEEP_OPT].items != 0)
	  afsl_tr_global.enabledTypes |= (1<<AFSL_TR_SLEEP);

      Printf("test_res run -repeat %d -initres %d%s -adjustsize %d -alloc %d -nthreads %d%s\n",
	     repeat, initial_res, (over_res ? " -overres":""), adjust_size,
	     alloc_size, nthreads, as->parms[SLEEP_OPT].items ? " -sleep":"");

      /*
       * initialization code
       */
      epis_InitReservationData(&testReservation, initial_res, 0);
      lock_Init(&runnersLock);

      if (over_res) {

	  /* We have a single thread which tries to reserve "too much" all the
	   * time, where "too much" is the total reservation pool plus one.
	   * This, interacting with the Manager can give us interesting
           * results.  This thread will succeed when the manager thread has
           * bumped the total up and fail when it is down.
           */
	  StartConsumerThreads(1, initial_res + 1, repeat, 1);
	  /*
	   * Very aggressive - we have a single thread which always
	   * over-allocates.  Note that we set the fail-OK bit.
	   */
	  StartConsumerThreads(1, initial_res + adjust_size + 1, repeat, 1);
      }

      if (adjust_size) {
	  /* for now, we only allow one manager thread */
	  StartManagerThreads(mthreads,adjust_size, repeat);
      }

      StartConsumerThreads(nthreads, alloc_size, repeat, 0);

      /*
       * Now we wait until all processing is complete.
       */

      lock_ObtainWrite(&runnersLock);
      while (runners) {
	  osi_SleepW(&runners, &runnersLock);
	  Printf("main thread: wakeup (%d)\n",runners);
	  lock_ObtainWrite(&runnersLock);
      }

      /*
       * Check to see if the reservation structure looks correct
       */
      if (epis_QueryTotalUnits(&testReservation) != initial_res)
	  printf("total units count wrong at end of test (%d)\n",
		 epis_QueryTotalUnits(&testReservation));
      if (epis_QueryFreeUnits(&testReservation) != initial_res)
	  printf("free units count wrong at end of test (%d)\n",
		 epis_QueryFreeUnits(&testReservation));
      if (epis_QueryUnusedUnits(&testReservation) !=
	  epis_QueryFreeUnits(&testReservation))
	  printf("unused units count wrong at end of test (%d)\n",
		 epis_QueryUnusedUnits(&testReservation));
      return 0;
  }

char *btool_log_name;
char *btool_test_name;

int main(int argc, char **argv)
{
      long code;
      static char BtoolLogname[1024];
      struct cmd_syndesc *syntax;

      btool_test_name = "test_res";
      (void) sprintf(BtoolLogname, "btool.dmp.%d", getpid());
      btool_log_name = BtoolLogname;

      syntax = cmd_CreateSyntax((char *)0, RunTest, 0,
				"Run reservation package test");
      (void)cmd_IndexedAddParm(syntax, "-repeat",CMD_SINGLE,CMD_OPTIONAL,
			       "# of iterations to run", REPEAT_OPT);
      (void)cmd_IndexedAddParm(syntax, "-initres",CMD_SINGLE,CMD_OPTIONAL,
			       "initial resource reservation",INITRES_OPT);
      (void)cmd_IndexedAddParm(syntax, "-overres",CMD_FLAG, CMD_OPTIONAL,
			       "run thread which tries to over-reserve",
			       OVERRES_OPT);
      (void)cmd_IndexedAddParm(syntax, "-adjustsize",CMD_SINGLE,CMD_OPTIONAL,
			       "amount by which to adjust resource limits",
			       ADJUSTSIZE_OPT);
      (void)cmd_IndexedAddParm(syntax, "-alloc",CMD_SINGLE,CMD_OPTIONAL,
			       "default allocation amount", ALLOC_OPT);
      (void)cmd_IndexedAddParm(syntax, "-nthreads",CMD_SINGLE,CMD_OPTIONAL,
			       "number of allocator threads to run",
			       NTHREADS_OPT);
      (void)cmd_IndexedAddParm(syntax, "-mthreads",CMD_SINGLE,CMD_OPTIONAL,
			       "number of manager threads to run",
			       MTHREADS_OPT);
      (void)cmd_IndexedAddParm(syntax, "-quiet",CMD_FLAG,CMD_OPTIONAL,
			       "suppress output", QUIET_OPT);
      (void)cmd_IndexedAddParm(syntax, "-sleep",CMD_FLAG,CMD_OPTIONAL,
			       "trace sleeping", SLEEP_OPT);

      code = cmd_Dispatch(argc, argv);
      if (code) {
	  com_err("test_res",code, (char *)0);
      }

      /* done */
      Printf("test done\n");
#ifdef DOBTOOL
      BtoolExit(code);
#endif /* DOBTOOL */
      return code;
}
