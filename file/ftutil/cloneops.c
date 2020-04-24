/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cloneops.c,v $
 * Revision 1.1.75.1  1996/10/02  17:48:18  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:17  damon]
 *
 * Revision 1.1.70.5  1994/08/30  15:55:58  sasala
 * 	DFS S12Y general cleanup - rename ftutil and tools cmd catalogs, add ftutil msgs to xaggr dfsexport, and episode salvage and vnops subcomponents
 * 	[1994/08/30  00:19:38  sasala]
 * 
 * Revision 1.1.70.4  1994/08/17  15:39:27  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/16  20:29:01  ruby]
 * 
 * Revision 1.1.72.2  1994/06/29  11:35:18  devsrc
 * 	merged with bl-10
 * 
 * Revision 1.1.72.1  1994/04/28  17:35:41  mbs
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:09  mbs]
 * 
 * Revision 1.1.70.2  1994/06/09  14:09:00  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:23:40  annie]
 * 
 * Revision 1.1.70.1  1994/02/04  20:19:25  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:28  devsrc]
 * 
 * Revision 1.1.68.1  1993/12/07  17:25:21  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:28:45  jaffe]
 * 
 * Revision 1.1.3.3  1993/01/21  19:37:55  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  19:51:59  zeliff]
 * 
 * Revision 1.1.3.2  1992/11/24  17:50:21  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:03:51  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:51:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1990-1993 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 */

#define AFSDEBUG_DECLARE 1	/* Declare AFS-related debug code */

#include <dcedfs/param.h>		/* General system parameters */
#include <dcedfs/sysincludes.h>		/* Basic kernel include files */
#include <dcedfs/basicincludes.h>	/* Another basic include file set */
#include <dcedfs/osi.h>
#include <pthread.h>
#include <dcedfs/afs4int.h>		/* afsFid */
#include <dcedfs/volume.h>
#include <dcedfs/vol_init.h>		/* volser's syscall */
#include <dcedfs/ag_init.h>		/* aggregate syscall */
#include <dcedfs/bomb.h>		/* BOMB_EXEC macro */
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */

#include <ftutil.h>
#include <ftu_private.h>
#include <ftu_syscalls.h>
#include <ftu_trace.h>
  
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftlmac.h>
#include <dfsftlsvc.h>
#include <dfsftlmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ftutil/cloneops.c,v 1.1.75.1 1996/10/02 17:48:18 damon Exp $")

/* Define some convenient handles on the volume and aggregate ops */



EXPORT void ftutil_ThreadYield()
{/* Wrapper for pthread_yield() to dodge implementation limits. */
    static unsigned long lastTime = 0;
    register unsigned long Now;
    static long pid = 0;
    static unsigned long SigToUse = 0;
    struct itimerval sigTimer;
    int code;
    struct timespec delaytime;

    /* Need to dodge the limitations of CMA for kernel-bound processes. */
    /*
     * The default thread scheduling policy is to run until the user-space
     * time-slice expires, which for threads that are spending most of their
     * time in the kernel, can take a huge amount of real time.  In addition,
     * we have to ensure that pending I/O is checked so that the RPC listener
     * thread will be made runnable.  Generally, we want to do these in the
     * opposite order, recognizing pending I/O and then doing a context swap.
     *
     * The resources are these:
     *     - pthread_yield(), which used to check pending I/O but no longer
    *       does in CMA;
     *     - pthread_delay_np(), which will do a context swap no matter what
     *        the scheduling algorithm;
     *     - pthread_cond_timedwait(), which will do something comparable;
     *     - sending ourselves a VTALRM signal (defined in the non-exported
     *        cma_timer.h file--thanks), which will cut short our time slice
     *        and generally check for pending I/O;
     *     - sending ourselves a VIRT signal, which is an AIX version of the
     *        same thing.
     *
     * Limitations:
     *     - has to work in non-threaded (non-RPC) applications;
     *     - has to work with threading mechanisms other than CMA.
     */
    /* Solution:
     *      - for AIX, send a signal to this process;
     *      - for other CMA, call pthread_yield() to let pending I/O make threads
     *         runnable, and then delay for a nanosecond to get the runnable
     *         threads to run;
     *      - for non-CMA, just call pthread_yield().
     */

      /* Do this heavyweight stuff at most once a second. */
      Now = osi_Time();
      if (Now != lastTime) {
#ifdef _CMA_VENDOR_
	  /* First, make I/O-dependent threads runnable. */
	  /* Find out what signal to use: SIGVIRT or SIGVTALRM. */
	  if (SigToUse == 0) {
	      SigToUse = SIGVTALRM;
#if defined(ITIMER_VIRT) && defined(SIGVIRT)
	      bzero(&sigTimer, sizeof(sigTimer));
	      code = getitimer(ITIMER_VIRT, &sigTimer);
	      if (code != 0) {
		  dce_svc_printf(FTL_S_CO_GETITIMER_MSG, strerror(errno));
	      } else if (timerisset(&sigTimer.it_interval))
		  SigToUse = SIGVIRT;
#endif /* defined(ITIMER_VIRT) && defined(SIGVIRT) */
	  }
	  /* Find out what pid to send it to. */
	  if (pid == 0)
	      pid = getpid();
	  kill(pid, SigToUse);
	  /* Second, ensure that other runnable processes are run. */
	  bzero((char *)&delaytime, sizeof(delaytime));
	  delaytime.tv_nsec = 1;
	  pthread_delay_np(&delaytime);
#else /* _CMA_VENDOR_ */
	  pthread_yield();
#endif /* _CMA_VENDOR_ */
	  /* Record when this was last done. */
	  lastTime = Now;
      }
}

/* EXPORT */
long ftutil_VolCloneAux(srcftd, destftd, errBuf, keepAliveProc, keepAliveRock)
int srcftd, destftd;
char *errBuf;
void (*keepAliveProc)();
void *keepAliveRock;
{
/*
 * Assume that srcftd and destftd are local open fileset descriptors.
 * Assume that destftd is a new fileset, just created.
 * Clone all the indices in srcftd into destftd, making destftd's fileset be
 * a copy-on-write version of srcftd's fileset.
 * If ``errBuf'' is non-zero, it's assumed to point to a buffer in which
 * could be written a reasonably-sized error message.
 */
    long code;

    icl_Trace4(_ftu_iclSetP, FTU_TRACE_ENTER_FTUTIL_CLONE_PROC,
	       ICL_TYPE_STRING, "ftutil_VolCloneAux",
	       ICL_TYPE_LONG, srcftd, ICL_TYPE_LONG, destftd,
	       ICL_TYPE_POINTER, errBuf);
    icl_Trace2(_ftu_iclSetP, FTU_TRACE_ENTER_FTUTIL_CLONE_PROC2,
	       ICL_TYPE_POINTER, keepAliveProc,
	       ICL_TYPE_POINTER, keepAliveRock);

    /* Iterate through everything in the source */
    /* Most of the iteration happens in the kernel itself! */
    for (;;) {
	code = BOMB_EXEC("ftutil_VolCloneAux.1", VOL_CLONE(destftd, srcftd));
	if (code == 0 || code != ELOOP)
	    break;
	/* Awaken pthreads implementations that count only VTALRM time */
	if (keepAliveProc == NULL)
	    ftutil_ThreadYield();
	else
	    (*keepAliveProc)(keepAliveRock);
    }

    if (code != 0 && errBuf)
	{
	char *dcesptr;
	dcesptr = (char *) dce_sprintf(ftl_s_co_cant_clone,
		srcftd, destftd, dfs_dceErrTxt(code));
	strcpy(errBuf , dcesptr);
	free(dcesptr);
	}
    icl_Trace2(_ftu_iclSetP, FTU_TRACE_EXIT_FTUTIL_CLONE_PROC,
	       ICL_TYPE_STRING, "ftutil_VolCloneAux", ICL_TYPE_LONG, code);

    /* Error (err-code in ``code'') or at end-of-source. */
    return code;
}

/* EXPORT */
long ftutil_VolClone(srcftd, destftd, errBuf)
int srcftd, destftd;
char *errBuf;
{
    return ftutil_VolCloneAux(srcftd, destftd, errBuf, NULL, NULL);
}

/* EXPORT */
long ftutil_VolUnCloneAux(int ftd, int backftd, char *errBuf,
			  void (*keepAliveProc)(), void *keepAliveRock)
{/* Unclone all the *nodes in the fileset opened via ftd. */
    long code;

    icl_Trace4(_ftu_iclSetP, FTU_TRACE_ENTER_FTUTIL_CLONE_PROC,
	       ICL_TYPE_STRING, "ftutil_VolUnCloneAux",
	       ICL_TYPE_LONG, ftd, ICL_TYPE_LONG, backftd,
	       ICL_TYPE_POINTER, errBuf);
    icl_Trace2(_ftu_iclSetP, FTU_TRACE_ENTER_FTUTIL_CLONE_PROC2,
	       ICL_TYPE_POINTER, keepAliveProc,
	       ICL_TYPE_POINTER, keepAliveRock);

    for (;;) {
	code = BOMB_EXEC("ftutil_VolUnCloneAux.1", VOL_UNCLONE(ftd, backftd));
	if (code == 0 || code != ELOOP)
	    break;
	/* Awaken pthreads implementations that count only VTALRM time */
	if (keepAliveProc == NULL)
	    ftutil_ThreadYield();
	else
	    (*keepAliveProc)(keepAliveRock);
    }

    if (code != 0 && errBuf)
	{
	char *dcesptr;
	dcesptr = (char *) dce_sprintf(ftl_s_co_cant_unclone,
		ftd, backftd, dfs_dceErrTxt(code));
	strcpy(errBuf , dcesptr);
	free(dcesptr);
	}

    icl_Trace2(_ftu_iclSetP, FTU_TRACE_EXIT_FTUTIL_CLONE_PROC,
	       ICL_TYPE_STRING, "ftutil_VolUnCloneAux", ICL_TYPE_LONG, code);

    /* Got here either via error or on end-of-index */
    return code;
}

/* EXPORT */
long ftutil_VolUnClone(int ftd, int backftd, char *errBuf)
{
    return ftutil_VolUnCloneAux(ftd, backftd, errBuf, NULL, NULL);
}

/* EXPORT */
long ftutil_VolReCloneAux(int ftd, int backftd, char *errBuf,
			  void (*keepAliveProc)(), void *keepAliveRock)
{/* Reclone all the *nodes in the fileset opened via ftd.   Any COW pointers
   are to the fileset opened via backftd. */
    long code;

    icl_Trace4(_ftu_iclSetP, FTU_TRACE_ENTER_FTUTIL_CLONE_PROC,
	       ICL_TYPE_STRING, "ftutil_VolReCloneAux",
	       ICL_TYPE_LONG, ftd, ICL_TYPE_LONG, backftd,
	       ICL_TYPE_POINTER, errBuf);
    icl_Trace2(_ftu_iclSetP, FTU_TRACE_ENTER_FTUTIL_CLONE_PROC2,
	       ICL_TYPE_POINTER, keepAliveProc,
	       ICL_TYPE_POINTER, keepAliveRock);

    for (;;) {
	code = BOMB_EXEC("ftutil_VolReCloneAux.1", VOL_RECLONE(ftd, backftd));
	if (code == 0 || code != ELOOP)
	    break;
	/* Awaken pthreads implementations that count only VTALRM time */
	if (keepAliveProc == NULL)
	    ftutil_ThreadYield();
	else
	    (*keepAliveProc)(keepAliveRock);
    }

    if (code != 0 && errBuf)
	{
	char *dcesptr;
	dcesptr = (char *) dce_sprintf(ftl_s_co_cant_reclone,
		ftd, backftd, dfs_dceErrTxt(code));
	strcpy(errBuf , dcesptr);
	free(dcesptr);
	}

    icl_Trace2(_ftu_iclSetP, FTU_TRACE_EXIT_FTUTIL_CLONE_PROC,
	       ICL_TYPE_STRING, "ftutil_VolReCloneAux", ICL_TYPE_LONG, code);

    /* Got here either via error or on end-of-index */
    return code;
}

/* EXPORT */
long ftutil_VolReClone(int ftd, int backftd, char *errBuf)
{
    return ftutil_VolReCloneAux(ftd, backftd, errBuf, NULL, NULL);
}

/* EXPORT */
long ftutil_VolDepleteAux(int ftd, char *errBuf, void (*keepAliveProc)(), void *keepAliveRock)
{/* Deplete the fileset opened by ftd. */
    long code;

    icl_Trace4(_ftu_iclSetP, FTU_TRACE_ENTER_FTUTIL_VOL_DEPLETE_AUX,
	       ICL_TYPE_LONG, ftd, ICL_TYPE_POINTER, errBuf,
	       ICL_TYPE_POINTER, keepAliveProc,
	       ICL_TYPE_POINTER, keepAliveRock);

    for (;;) {
	code = BOMB_EXEC("ftutil_VolDepleteAux.1", VOL_DEPLETE(ftd));
	if (code == 0 || code != ELOOP)
	    break;
	/* Awaken pthreads implementations that count only VTALRM time */
	if (keepAliveProc == NULL)
	    ftutil_ThreadYield();
	else
	    (*keepAliveProc)(keepAliveRock);
    }

    if (code != 0 && errBuf)
	{
	char *dcesptr;
	dcesptr = (char *) dce_sprintf(ftl_s_co_cant_deplete,
				       ftd, dfs_dceErrTxt(code));
	strcpy(errBuf , dcesptr);
	free(dcesptr);
	}

    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_FTUTIL_VOL_DEPLETE_AUX,
	       ICL_TYPE_LONG, code);

    /* Got here either via error or on end-of-index */
    return code;
}

/* EXPORT */
long ftutil_VolDeplete(int ftd, char *errBuf)
{
    return ftutil_VolDepleteAux(ftd, errBuf, NULL, NULL);
}
