/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: events.c,v $
 * Revision 1.1.52.2  1996/02/17  23:35:42  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:56  marty]
 *
 * Revision 1.1.52.1  1995/12/08  18:14:04  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/26  19:50 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:26:02  root]
 * 
 * Revision 1.1.50.6  1994/08/16  18:13:09  cbrooks
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:14  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  17:28 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.7.3  1993/09/17  19:27:43  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/17  19:25:53  pare]
 * 
 * Revision 1.1.8.3  1993/09/15  17:48:49  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.7.2  1993/07/08  20:23:19  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:22:51  root]
 * 
 * Revision 1.1.5.2  1993/03/23  19:35:54  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/23  19:35:13  truitt]
 * 
 * Revision 1.1.1.8  1993/03/11  22:35:21  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.50.5  1994/05/16  18:51:55  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:31  rhw]
 * 
 * Revision 1.1.50.4  1994/04/28  21:08:52  cbrooks
 * 	DTS Code cleanup
 * 	[1994/04/27  19:41:56  cbrooks]
 * 
 * Revision 1.1.50.3  1994/04/21  22:06:11  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:59:59  cbrooks]
 * 
 * Revision 1.1.50.2  1994/01/21  22:00:48  rsalz
 * 	Merged with changes from 1.1.50.1
 * 	[1994/01/21  22:00:36  rsalz]
 * 
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/21  21:07:30  rsalz]
 * 
 * Revision 1.1.50.1  1994/01/21  21:08:35  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/21  21:07:30  rsalz]
 * 
 * Revision 1.1.3.4  1993/01/12  17:07:45  tom
 * 	Call signal handler initialization after main event mutex
 * 	has been created. Part of fix for bug #6617.
 * 	[1993/01/12  17:06:34  tom]
 * 
 * Revision 1.1.3.3  1992/12/30  16:39:24  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:22:06  htf]
 * 
 * Revision 1.1.3.2  1992/12/22  00:18:23  smythe
 * 	added OSF fix of WakeMainWithEvent call to get
 * 	dtsd to respond gracefully to kill signal
 * 	[1992/12/19  16:52:01  smythe]
 * 
 * Revision 1.1.3.2  1992/12/15  14:35:34  tom
 * 	Bug 6298 - dtsd was ignoring signals due to a missing
 * 	call to WakeMainWithEvent() in GenEvent().
 * 	[1992/12/14  21:05:06  tom]
 * 
 * Revision 1.1.1.7  1992/09/21  18:57:03  osfrcs
 * 	    Loading drop Dce1_0_1fnl
 * 
 * Revision 1.1  1992/01/19  15:34:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * 	Module EVENTS.C
 *	Version X0.6
 */
/*
 * Copyright (c) 1990 by
 * Digital Equipment Corporation, Maynard, Mass.
 *
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:
 *
 *	This module contains routines used to communicate synchronization and
 *      other events between the main thread (the consumer thread) and the
 *	producer threads, such as control program, time provider, NCL, and
 *	network message threads.
 *
 * ENVIRONMENT:	portable
 *
 * AUTHOR:
 *              Brian Bailey
 *		Distributed Processing Engineering (DPE)
 *
 * DATE:	September 1990
 *
 */

#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>
#include <dce/dce.h>
#include <dcedtssvc.h>
#include <dcedtsmsg.h>

/*
 * External Variables
 */

extern ShmBuf	*shmPtr;	/* pointer to global state containing the */
				/* trace flags */

/*
 * Thread synchronization variables.  These variables synchronize the event
 * processing loop of the main thread with the event generation activities of
 * the other threads (such as the control program and time provider threads).
 *
 * When generating an event to be processed by the main thread, a thread must:
 *
 *	1) generate the event
 *	2) call WakeMainWithEvent
 *
 * The main thread will continue to fetch events by calling GetNextEvent and
 * process them with ProcessEvent.  GetNextEvent will:
 *
 *	1) attempt to fetch an event
 *	2) if there is an event, return it
 *	3) if there is no event, call WaitForMainEvent
 *
 * WakeMainWithEvent and WaitForMainEvent use the synchronization variables
 * defined below to implement the waiting and wakeup functions.  Other locks
 * or condition variables may be required that are specific to the event itself
 * (in step 1 of the generating thread and step 1 of GetNextEvent).
 *
 */
static volatile int 	eventToProcess;
static pthread_mutex_t 	mainEventMutex;
static pthread_cond_t	mainEventCond;

/*
 * All-purpose event block - used to indicate termination or other events that
 * don't need event blocks of their own.
 */

static EventBlk		miscellaneousEblk;

/*
 * Termination Flag
 */
char	TerminateDTSS = FALSE;

/*
 * Message event variables.  These variables are used for message events and
 * RPC response events
 */
Que		MsgEventQ;
pthread_mutex_t	MsgEventMutex;


/*
 * Internal Prototypes
 */
static void WaitForMainEvent( void ) ;


/*
 *++
 *  InitEvent ()
 *
 *  Functional Description:
 *	Initialize the thread synchronization variables used to communicate
 *	events between the main thread and other threads.
 *
 *  Inputs:
 *	none
 *
 *  Implicit Inputs:
 *	All condition variables and mutexes used by the main thread.
 *
 *  Outputs:
 *	none
 *
 *  Implicit Outputs:
 *	All condition variables and mutexes are created.
 *
 *  Value Returned:
 *	StatusBlk
 *
 *  Side Effects:
 *	Mutexes are created in the unlocked state.
 *
 *--
 */

StatusBlk InitEvent (void)
{
    StatusBlk statusBlk;
    int i;

    /*
     * Initialize the main event processing variables
     */
    eventToProcess = FALSE;
    dtss_mutex_init (&mainEventMutex);
    dtss_cond_init (&mainEventCond);

    /*
     * Initialize the system-dependent event stuff
     * MUST be after mainEventMutex is created.
     */
    statusBlk = InitSysdepEvent ();
    if (IsError(statusBlk))
	BugCheck ();

    /*
     * Initialize the message/RPC event processing variables
     */
    Qinit (&MsgEventQ);
    dtss_mutex_init (&MsgEventMutex);

    /*
     * Initialize the timers
     */
    for (i = 0; i < K_NUM_TIMERS; i++)
	CreateTimer (i);

    /*
     * Initialize the time provider interface
     */
    InitTPinterface ();

    /*
     * Initialize the control program interface
     */
    InitCPinterface ();

    /*
     * Initialize RPC-dependent code of the time service
     */
    InitServerRPC ();

    return (statusBlk);
} /* end of routine InitEvent */

/*
 *++
 *  GetNextEvent()
 *
 *  Functional Description:
 *
 *    GetNextEvent returns the next event that the DTSS service is to
 *    process. Come here when ready to process an event. Called only from
 *    the main loop.
 *
 *    If no events are ready, wait for one by calling WaitForMainEvent.
 *
 *  Inputs:
 *
 *    none
 *
 *  Implicit Inputs:
 *
 *    none
 *
 *  Outputs:
 *
 *    none
 *
 *  Implicit Outputs:
 *
 *    Caller is responsible for reposting eblk for read if it was
 *    a message event, initiating timer if it was a timer event.
 *
 *  Value Returned:
 *
 *    pointer to the event block to be processed next
 *
 *  Side Effects:
 *    Forgets the event returned; returns the message or clears the timer.
 *
 *--
 */
EventBlk *GetNextEvent(void)
{
    EventBlk      *eblkPtr;
    StatusBlk     statusBlk;

    statusBlk.status = DTSS_SUCCESS;

    for ( ; ; ) {

	if (TerminateDTSS) {
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		      "Main:  got event ETterminate\n"));
	    eblkPtr = &miscellaneousEblk;
	    eblkPtr->event = ETterminate;
	    return(eblkPtr);
	}

	/*
	 * First, try to get a message event, since they're the most important.
	 * (RPC events come in on this queue, too)
	 */
	if (eblkPtr = GetNextThreadEvent (&MsgEventQ, &MsgEventMutex)) {
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		     "Got message or RPC event\n"));
	    return (eblkPtr);
	}

	if (eblkPtr = GetNextTPevent ())
	    return (eblkPtr);

	if (eblkPtr = GetNextCPevent ())
	    return (eblkPtr);

	if (eblkPtr = GetNextTimerEvent ())
	    return (eblkPtr);

	/*
	 * No events so far.  Try for a message one more time...
	 */
	if (eblkPtr = GetNextThreadEvent (&MsgEventQ, &MsgEventMutex)) {
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		     "Got message or RPC event"));
	    return (eblkPtr);
	}

	/*
	 * No events available.  Wait for one.
	 */
	WaitForMainEvent ();

    } /* while (TRUE) */
} /* end of routine GetnextEvent() */

/*
 *++
 *  WaitForMainEvent ()
 *
 *  Functional Description:
 *
 *	Called only from GetNextEvent in the main loop.  If there is an event to
 *	process, then just return.  If there is none, we waits on the main
 *	thread condition variable mainEventCond for other threads to generate
 *	events and wake us by calling WakeMainWithEvent.
 *
 *  Inputs:
 *	None
 *
 *  Implicit Inputs:
 *	eventToProcess		TRUE if there is an event to process.
 *
 *	mainEventCond		condition that gets signalled when there is
 *				an event to process.
 *
 *	mainEventMutex		mutex that protects eventToProcess and
 *				eventCond.
 *
 *  Outputs:
 *	None
 *
 *  Implicit Outputs:
 *	eventToProcess		gets set to FALSE.
 *
 *  Locking:
 *      mainEventMutex gets locked before waiting on mainEventCond and while
 *	accessing eventToProcess.
 *
 *  Value Returned:
 *	None
 *
 *  Side Effects:
 *	The main thread stalls while waiting on mainEventCond.
 *
 *--
 */
static void WaitForMainEvent (void)
{

    dtss_mutex_lock (&mainEventMutex);
    while (!eventToProcess) {
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		 "Main:  waiting for event\n"));
        dtss_cond_wait (&mainEventCond, &mainEventMutex);
    }
    eventToProcess = FALSE;
    dtss_mutex_unlock (&mainEventMutex);

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "Main:  awakened with event\n"));
} /* end of routine WaitForEvent */

/*
 *++
 *  WakeMainWithEvent ()
 *
 *  Functional Description:
 *
 *	Threads call this routine to wake the main thread when they have
 *	generated an event for it to process.  It sets eventToProcess to
 *	indicate that an event has been generated.  It then signals
 *	mainEventCond, which wakes the main thread if it is waiting in
 *	WaitForMainEvent.
 *
 *  Inputs:
 *	None
 *
 *  Implicit Inputs:
 *	mainEventCond		the condition variable on which the main thread
 *				is waiting for events.
 *
 *	mainEventMutex		mutex that protects eventToProcess and
 *				mainEventCond.
 *
 *  Outputs:
 *	None
 *
 *  Implicit Outputs:
 *	eventToProcess		gets set to TRUE.
 *
 *  Locking:
 *      mainEventMutex gets locked while setting eventToProcess and signalling
 *	mainEventCond and unlocked before returning.
 *
 *  Value Returned:
 *	None
 *
 *  Side Effects:
 *	Wakes the main thread if it is waiting on mainEventCond.
 *
 *--
 */
void WakeMainWithEvent (void)
{

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "--> Waking Main Thread\n"));

    dtss_mutex_lock (&mainEventMutex);
    eventToProcess = TRUE;
    dtss_cond_signal (&mainEventCond);
    dtss_mutex_unlock (&mainEventMutex);
} /* end of routine WakeMainWithEvent */

/*
 *++
 *  GenEvent()
 *
 *  Functional Description:
 *	Generates an event of the specified type.  This is used to generate
 *	timer-related events or a termination event.
 *
 *  Inputs:
 *	eventType	is the event type (ETxxx)
 *
 *  Implicit Inputs:
 *      none
 *
 *  Outputs:
 *      none
 *
 *  Implicit Outputs:
 *      none
 *
 *  Value Returned:
 *      none
 *
 *  Side Effects:
 *      May start or reset a timer.
 *--
 */
void GenEvent( 
    EventTypes	eventType
)
{
    UTCValue intval;

    UTCzero (&intval);

    switch (eventType) {

    case ETsynchronize:
    	InitiateTimer(K_TIMER_NEXT_SYNCH, &intval);
	break;

    case ETadvertise:
      	InitiateTimer(K_TIMER_ADVERTISE, &intval);
	break;

    case ETqueryTimeout:
   	InitiateTimer(K_TIMER_QUERY_TMO, &intval);
	break;

    case ETterminate:
      	TerminateDTSS = TRUE;
	break;

    default:
	BugCheck ();
	break;
    }

    WakeMainWithEvent();
} /* end of routine GenEvent */

/*
 *++
 *  GenMsgEvent()
 *
 *  Functional Description:
 *
 *	Generate a message or RPC event.  This routine calls GenThreadEvent to
 *	place an event block containing a DTSS message or an RPC time response
 *	on the message queue.
 *
 *  Inputs:
 *	event		the event to generate (ETreceivedMsg or ETtimeRespRPC).
 *
 *	eblkPtr		is the event block to add to the queue.
 *
 *  Implicit Inputs:
 *	MsgEventQ	is the message event queue.
 *
 *	MsgEventMutex	is the mutex that protects the event queue.
 *
 *  Outputs:
 *	none
 *
 *  Implicit Outputs:
 *	none
 *
 *  Return Value:
 *	none
 *
 *  Side Effects:
 *	none
 *
 *--
 */
void GenMsgEvent (
    EventTypes	event,
    EventBlk	*eblkPtr
) 
{

#ifdef	DCE_DEBUG
    {
	char *p;

	if (event == ETreceivedMsg)
	    p = "ETreceivedMsg";
	else if (event == ETtimeRespRPC)
	    p = "ETtimeRespRPC";
	else
	    p = "ETlinkReady";
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	"MSG: generating %s event\n", p));
    }
#endif

    eblkPtr->event = event;
    GenThreadEvent (&MsgEventQ, &MsgEventMutex, eblkPtr);
} /* end of routine GenMsgEvent */

/*
 *++
 *  GenThreadEvent()
 *
 *  Functional Description:
 *
 *	Generate a thread event.  This routine is a tool for generating events
 *	for threads that use queues of event blocks to communicate with the
 *	main thread.  This routine sets the busy flag in the given event block,
 *	places it on the event queue, then wakes the main thread by calling
 *	WakeMainWithEvent.
 *
 *	In processing the event, the main thread may clear the busy flag (by
 *	calling WakeThreadWithResp) if the generating thread is waiting for a
 *	response to the event (by calling WaitForEventResp).
 *
 *  Inputs:
 *	eQhd		is the queue of events of the type we're generating
 *
 *	eQmutex		is the mutex that protects the event queue.
 *
 *	eblkPtr		is the event block to add to the queue.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *	none
 *
 *  Implicit Outputs:
 *	eblkPtr->busy	gets set to TRUE.
 *	eQhd		an event block is placed on the tail of this queue.
 *
 *  Locking:
 *	eQmutex is locked while accessing the event queue.
 *
 *  Return Value:
 *	none
 *
 *  Side Effects:
 *	The main thread is awakened.
 *
 *--
 */
void GenThreadEvent (
    Que		*eQhd,
    pthread_mutex_t	*eQmutex,
    EventBlk	*eblkPtr
)
{

    eblkPtr->busy = TRUE;
    dtss_mutex_lock (eQmutex);
    QinsertLast (eQhd, eblkPtr);
    dtss_mutex_unlock (eQmutex);
    WakeMainWithEvent ();
} /* end of routine GenThreadEvent */

/*
 *++
 *  GetNextThreadEvent()
 *
 *  Functional Description:
 *
 *	Get the next event generated by a particular thread.  This routine is a
 *	tool for threads that use queues of event blocks to communicate with the
 *	main thread.  It returns the first event block from the given event
 *	queue.  If the queue is empty, it returns NULL.
 *
 *  Inputs:
 *	eQhd	is the queue of events of the type we're generating
 *
 *	eQmutex	is the mutex that protects the event queue.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *	none
 *
 *  Implicit Outputs:
 *      eQhd		if this queue is not empty, the first event block is
 *			is removed.
 *
 *  Locking:
 *	eQmutex is locked while accessing eQhd.
 *
 *  Value Returned:
 *	eblkPtr		is a pointer to the element removed from the queue.  If
 *			the queue is empty, this is NULL.
 *
 *  Side Effects:
 *      none
 *
 *--
 */
EventBlk *GetNextThreadEvent (
    Que		*eQhd,
    pthread_mutex_t	*eQmutex
)
{
    EventBlk	*eblkPtr;

    dtss_mutex_lock (eQmutex);
    if (IsQempty (*eQhd))
	eblkPtr = NULL;
    else
	QremoveFirst (eQhd, &eblkPtr);
    dtss_mutex_unlock (eQmutex);

    return (eblkPtr);
} /* end of routine GetNextThreadEvent */

/*
 *++
 *  WaitForEventResp()
 *
 *  Functional Description:
 *
 *	Wait for the main thread to process the given event.  This routine is a
 *	tool for threads that communicate with the main thread using queues of
 *	event blocks.  It causes the generating thread to wait on the given
 *	condition variable until the main thread clears the busy flag in the
 *	given event block (by calling WakeThreadWithResp).
 *
 *  Inputs:
 *	eMutex		is the mutex protecting the event block.
 *
 *	eCond		is the condition variable for the event.
 *
 *	eblkPtr		is the event block being processed.
 *
 *  Implicit Inputs:
 *	eblkPtr->busy	gets set to FALSE when the event has been processed.
 *
 *  Outputs:
 *	none
 *
 *  Implicit Outputs:
 *	none
 *
 *  Locking:
 *	eMutex is locked before waiting on eCond and while checking
 * 	the busy flag in the event block.
 *
 *  Return Value:
 *	statusBlk	is DTSS_SUCCESS if the message was processed and
 *			DTSS_ERROR if it wasn't.
 *
 *  Side Effects:
 *	The thread waits until until the main thread signals eCond by
 *	calling WakeThreadWithResp.
 *
 *--
 */
void WaitForEventResp (
    pthread_mutex_t	*eMutex,
    pthread_cond_t	*eCond,
    EventBlk	*eblkPtr
)
{

    dtss_mutex_lock (eMutex);
    while (eblkPtr->busy) {
	dtss_cond_wait (eCond, eMutex);
    }
    dtss_mutex_unlock (eMutex);
} /* end of routine WaitForEventResp */

/*
 *++
 *  WakeThreadWithResp ()
 *
 *  Functional Description:
 *
 *	Wake a thread waiting on a given condition variable for a given event
 *	block to be processed.  The main thread calls this routine when it has
 *	processed an event block that needs to be returned to the thread that
 *	generated it.  It clears the busy flag in the event block, then
 *	broadcasts on the condition variable.  All threads waiting on that
 *	condition variable awake, but only the thread whose busy flag has been
 *	cleared proceeds.  The others resume waiting.
 *
 *  Inputs:
 *	eMutex		is the mutex protecting the event block.
 *
 *	eCond		is the condition variable for the event.
 *
 *	eblkPtr		is the event block being processed.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *	None
 *
 *  Implicit Outputs:
 *	none
 *
 *  Locking:
 *	eMutex while clearing the busy flag and broadcasting on eCond.
 *
 *  Value Returned:
 *	None
 *
 *  Side Effects:
 *	Wakes all threads waiting on eCond.
 *
 *--
 */
void WakeThreadWithResp (
    pthread_mutex_t	*eMutex,
    pthread_cond_t	*eCond,
    EventBlk	*eblkPtr
)
{

    dtss_mutex_lock (eMutex);
    eblkPtr->busy = FALSE;
    dtss_cond_broadcast (eCond);
    dtss_mutex_unlock (eMutex);
} /* end of routine WakeThreadWithResp */
