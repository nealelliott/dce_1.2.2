/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1989, 1990 Transarc Corporation - All rights reserved */

/*
 * HISTORY
 * $Log: osi_net_osf.c,v $
 * Revision 1.1.9.1  1996/10/02  17:58:30  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:59  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:15:44  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:11  annie]
 * 
 * Revision 1.1.2.2  1993/05/12  13:56:02  delgado
 * 	Create a new osi_Wait and osi_CancelProc for OSF.  We no longer
 * 	use any of the routines in osi/osi_net.c.
 * 	[1993/05/12  13:52:18  delgado]
 * 
 * $EndLog$
 */

#ifdef KERNEL
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/osi.h>

/*
 * This waitV is here because of the way the osi_Wait is implemented.
 * We would really prefer to have distinct channels for sleeping 
 * purposes, however the ahandle which is passed to osi_Wait can
 * sometimes be NULL.  Though it seems that tpq, a primary user of
 * these routines, always passes in a non-NULL ahandle, other
 * users, e.g, the cache mgr, do not.
 *
 * This is how we've implemented it:
 *    if ahanle is non null set it to some unique per-thread 
 *       structure and sleep on that.  The wakeup will occur on that
 *       unique channel as well.  
 *
 *    If ahandle is null then do the sleep and wakup on the global
 *       waitV; note this might have the side effect of  waking up
 *       other threads that one did not intend to wakeup.
 */

char waitV;

/* 
 * Wait for wakeup on ahandle, or ams ms later. ahandle may be null.  Returns
 * 0 if timeout and 2 if signalled 
 *
 * Usage would indicate that callers which pass in a NULL ahandle 
 * don't expect to be purposely awoken by something like osi_CancelWait.
 * These callers expect to sleep the duration of the specified timeout
 * period.
 */

int osi_Wait(ams, ahandle, aintok)
    struct osi_WaitHandle *ahandle;
    int aintok;
    long ams; 
{
    register long code, endTime, sec;
    int pri;
    caddr_t chan;

    sec = ams/1000;
    endTime =  osi_Time() +sec;
    if (ahandle)  /* use something thread unique instead of u.u_procp */
	chan = ahandle->proc  = (caddr_t) u.np_uthread;
    else
        chan = &waitV;
    do {
	code = tsleep(chan, (aintok? PCATCH|PZERO+1 :
		PZERO-1), "osi_wait", (int) (sec*hz + 1));
        /* 
         * tsleep returns 0 if awakened and EWOULDBLOCK if
         * the timeout occurs.
         */
        switch (code){
        case 0:
                 code = 2;
                 if (ahandle && (ahandle->proc == NULL)){
                    /* we were intentionally awakened by osi_CancelWait */
                     return(code);
        	 }
                 /* we were "inadvertently" woken up */
                 break;
	 case EWOULDBLOCK:    /* EWOULDBLOCK - might be time to get up */
                code = 0;   /* we're supposed to return 0 if we timeout */
                break;
         default:
                return(code);   /* something might be wrong or  */
                                /* in the case of interruptible we got */
                                /* ERESTART or EINTR (a signal came in) */
	 }

        
    } while (osi_Time() < endTime);

    /*
     * Okay why do we have to check the time time here anyway? Because
     * there are callers who call osi_Wait for very different reasons
     * and don't pass in an ahandle.  Therefore they're all sleeping
     * on the same channel and a wakeup on that channel intended for
     * one of them could wakeup all of them, even those who want to
     * sleep for the entire specified timeout period.
     *
     * examples : cm_CheckSize wants to do an osi_Wait for 5 min when
     *            it's run out of cache space.
     *            
     *            cm_dcache wants to do an osi_Wait for 5 min when it's
     *            out of entries.
     *
     *            afscall_cm_setpag wants to sometimes do an osi_Wait
     *            for some reason unknown to mankind.
     */

    return(code);

  }

/*
 * Wakeup someone who's gone to sleep via osi_Wait
 */

void 
osi_CancelWait(ahandle)
    struct osi_WaitHandle *ahandle; 
{

    caddr_t proc;

    if (ahandle){
        if (ahandle->proc == (caddr_t)0) 
            return;
        proc = ahandle->proc;
        ahandle->proc = (caddr_t) 0; /* so dude can figure out he was signalled */   
        wakeup(proc);
    }
    else
        wakeup(&waitV);
  }
#endif /* KERNEL */






