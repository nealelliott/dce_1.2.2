/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss_service_state.c,v $
 * Revision 1.1.12.2  1996/02/17  23:35:39  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:53  marty]
 *
 * Revision 1.1.12.1  1995/12/08  18:13:56  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/26  19:39 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:25:56  root]
 * 
 * Revision 1.1.10.6  1994/08/23  20:32:34  cbrooks
 * 	CR11495 - remove fprintfs in code; use serviceability
 * 	[1994/08/19  19:44:41  cbrooks]
 * 
 * Revision 1.1.10.5  1994/08/16  18:13:07  cbrooks
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:21  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/18  13:57 UTC  pare
 * 	Merege changes from Hpdce02
 * 
 * Revision 1.1.10.4  1994/08/02  19:05:29  cbrooks
 * 	CR11514 - non-null args to utc_pointtime
 * 	[1994/08/02  18:54:52  cbrooks]
 * 
 * 	CR11514 - add non-null arg to utc_getnextzone()
 * 	[1994/08/02  14:55:58  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  17:21 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.7.5  1993/09/18  12:33:14  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/18  12:31:38  pare]
 * 
 * Revision 1.1.8.3  1993/09/15  17:48:44  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.7.4  1993/07/16  12:51:39  truitt
 * 	OT4697: Log a local clock fault when sync'ing with a Time Provider
 * 	        that provides a time that does not intersect with our clock.
 * 	[1993/07/16  12:51:08  truitt]
 * 
 * Revision 1.1.10.3  1994/05/16  18:51:53  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:26  rhw]
 * 
 * Revision 1.1.10.2  1994/04/21  22:06:08  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:59:55  cbrooks]
 * 
 * 	Checkpoint:4.15.94
 * 	[1994/04/15  21:15:11  cbrooks]
 * 
 * 	Checkpoint
 * 	[1994/03/28  20:28:47  cbrooks]
 * 
 * Revision 1.1.10.1  1994/01/25  17:16:47  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/25  17:15:45  rsalz]
 * 
 * Revision 1.1.8.2  1993/07/14  21:33:05  tom
 * 	Bug 4697 - Log an local clock fault when syncing with a TP
 * 	That provides a time that does not intersect with our clock.
 * 	[1993/06/16  21:26:47  tom]
 * 
 * Revision 1.1.7.3  1993/07/15  13:00:27  truitt
 * 	Added missing default case to switch statement.  If default
 * 	is hit, will call bugcheck function.
 * 	[1993/07/15  12:59:53  truitt]
 * 
 * Revision 1.1.7.2  1993/07/08  20:21:53  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:21:10  root]
 * 
 * Revision 1.1.5.2  1993/04/05  17:16:13  truitt
 * 	Recreate GAMERA branch.
 * 	[1993/04/05  17:15:08  truitt]
 * 
 * Revision 1.1.5.2  1993/03/19  15:25:55  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/19  15:24:58  truitt]
 * 
 * Revision 1.1.1.7  1993/03/11  22:35:12  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.2  1992/12/22  00:18:07  smythe
 * 	added OSF OT 5105 fix to set inaccuracy properly in some clock set cases
 * 	added OSF OT 5104 fix to get server to handle single time provider stamp
 * 	added OSF OT 2907 to properly expand macro under all compiler settings
 * 	[1992/12/19  19:22:03  smythe]
 * 
 * Revision 1.1.5.5  1993/02/03  21:14:10  tom
 * 	Bug 7118 - Add call to LogEventTPtimout to log TP timeouts.
 * 	[1993/02/03  21:11:04  tom]
 * 
 * Revision 1.1.5.4  1992/12/30  16:39:16  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:21:23  htf]
 * 
 * Revision 1.1.5.3  1992/12/15  12:48:06  yankes
 * 		This submit covers OTs 2907, 5105 and 5104.
 * 	[1992/12/11  20:54:06  yankes]
 * 
 * Revision 1.1.5.2  1992/09/29  20:44:10  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:07:14  sekhar]
 * 
 * Revision 1.1.2.2  1992/05/14  18:30:54  ko
 * 	Fix ot - 2116 : uninitialized variable warnings.
 * 	[1992/05/14  16:54:46  ko]
 * 
 * Revision 1.1  1992/01/19  15:34:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * 	Module DTSS_SERVICE_STATE.C
 *
 * Copyright (c) 1989,1990,1991 by
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
 */
/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */

#include <math.h>
#include <stdlib.h>

#ifdef ultrix
#include <sys/time.h>
#endif

#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>
#include <dce/dce.h>
#include <dcedtssvc.h>
#include <dcedtsmsg.h>
#include <dcedtsmac.h>

/*
 * External Variables
 */
extern Que portQhd;    /* Port Queue header */

extern ShmBuf *shmPtr; /* Ptr to shared memory all modules have shmPtr defined */

/*
 * flag to determine that a new epoch was declared  but no time given.
 * Therefore this server has its inaccuracy set to infinity.
 */
extern newEpochNoTime;

extern RpcBuf *rpcBufPtr;

/*
 * Global variables
 */

Sdb sdb;			       /* synchronization database.
				        * exported to the control module */

static utc_t cacheFlushInt;
static utc_t nextCacheFlush;
static UTCValue nextCheckInt = UTCinit(0,0);
static UTCValue nextSynchTime = UTCinit(0,0);



/*
 * State table design for the synchronization process
 */

/*
 * Define the synchronization states
 */
typedef enum STsynchStates
{
/*
 * The following are pseudo-states for the transition engine
 */
    STbad = 0,                  /* illegal state transition 		*/
    STignore,                   /* ignore this transition 		*/
/*
 * The following states are real
 */
    STdoCkInt,			/* perform a checkInt operation		*/
    STgblInit,			/* set up for global request		*/
    STgblLinks,			/* request global time request links	*/
    STgblLinksWait,		/* wait for global time request links	*/
    STgblSend,			/* send global time request		*/
    STgblTimer,			/* request timed out	      		*/
    STgblWait,			/* wait for global response		*/
    STnone,			/* not synchronizing  			*/
    STprvdrSync,		/* synchronizing w/ TP.                 */
    STprvdrTMO,			/* time provider request timed out	*/
    STprvdrWait,		/* wait for TP response			*/
    STprvdrWTMO,		/* Wait for TP initial response		*/
    STrqInit,	       		/* init time request(s)			*/
    STrqSend,	       		/* send time request(s)			*/
    STrqTimer,			/* request timed out			*/
    STrqWait,			/* wait for response(s)			*/
    STstartSynch,		/* start synchronizing 			*/
    STstCkInt,			/* begin a checkInt			*/
    STstPrvdrSync,		/* begin TP synchronization	        */
    STstServSynch,		/* Start a server synchronization	*/
    STsolInit,			/* set up for solicitation request	*/
    STsolSend,			/* send solicitation request		*/
    STsolTimer,			/* request timed out			*/
    STsolWait,			/* wait for solicitation response	*/
    STsynch,			/* perform a server synchronization	*/
    STnoSynch,			/* server synchronization failed	*/
    STcleanup,			/* clean up and sched next synch	*/
    STnoPrvdr,			/* couldn't contact a provider		*/

/*
 * STates_Number assumes that the states start at zero
 */
    STates_Number  		/* # of possible states	     		*/
} STsynchStates;

static STsynchStates synchState;  /* current state of state machine */



/*
 * The state transition table:
 *   Stimuli are arrayed across the page, INstimuli defined in DTSS_SERVICE.H
 *   Old states are arrayed down the page, STsynchStates defined above
 *   New states are at the intersections.
 *
 *  recvTime,  recvdSol, syncTime,   qryTmo,      recvTP,  linkReady
 */
STsynchStates nextState[STates_Number][INstimuliNumber] =
{
{ STbad,     STbad,       STbad,     STbad,       STbad   ,	STbad},         /* STbad      */
{ STbad,     STbad,       STbad,     STbad,       STbad   ,	STbad}, 	/* STignore   */
{ STignore,  STignore,    STbad,     STbad,       STignore,	STignore}, 	/* STdoCkInt  */
{ STignore,  STignore,    STbad,     STbad,       STignore,	STignore}, 	/* STgblInit  */
{ STignore,  STignore,    STbad,     STbad,       STignore,	STignore}, 	/* STgblLinks */
{ STignore,  STignore,    STbad,     STignore,    STignore,	STgblLinksWait},/* STgblLinksWait*/
{ STignore,  STignore,    STbad,     STbad,       STignore,	STignore}, 	/* STgblSend  */
{ STignore,  STignore,    STbad,     STbad,       STignore,	STignore}, 	/* STgblTimer */
{ STgblWait, STignore,    STbad,  STgblTimer,     STignore,	STignore}, 	/* STgblWait  */
{ STignore,  STignore, STstartSynch, STbad,       STignore,	STignore}, 	/* STnone     */
{ STignore,  STignore,    STbad,     STbad,       STignore,	STignore}, 	/* STprvdrSync*/
{ STignore,  STignore,    STbad,     STbad,       STignore,	STignore}, 	/* STprvdrTMO */
{ STignore,  STignore,    STbad,  STprvdrTMO,  STprvdrWait,	STignore}, 	/* STprvdrWait*/
{ STignore,  STignore,    STbad,   STnoPrvdr,  STprvdrWTMO}, /* STprvdrWTMO*/
{ STignore,  STignore,    STbad,     STbad,       STignore}, /* STrqInit   */
{ STignore,  STignore,    STbad,     STbad,       STignore}, /* STrqSend   */
{ STignore,  STignore,    STbad,     STbad,       STignore}, /* STrqTimer  */
{ STrqWait,  STignore,    STbad,   STrqTimer,     STignore}, /* STrqWait   */
{ STignore,  STignore,    STbad,     STbad,       STignore}, /* STstartSync*/
{ STignore,  STignore,    STbad,     STbad,       STignore}, /* STstCkInt  */
{ STignore,  STignore,    STbad,     STbad,       STignore}, /* STstPrvdrSy*/
{ STignore,  STignore,    STbad,     STbad,       STignore}, /* STstServSyn*/
{ STignore,  STignore,    STbad,     STbad,       STignore}, /* STsolInit  */
{ STignore,  STignore,    STbad,     STbad,       STignore}, /* STsolSend  */
{ STignore,  STignore,    STbad,     STbad,       STignore}, /* STsolTimer */
{ STignore,  STsolWait,   STbad,  STsolTimer,     STignore}, /* STsolWait  */
{ STignore,  STignore,    STbad,     STbad,       STignore}, /* STsynch    */
{ STignore,  STignore,    STbad,     STbad,       STignore}, /* STnoSynch  */
{ STignore,  STignore,    STbad,     STbad,       STignore}, /* STcleanup  */
{ STignore,  STignore,	  STbad,     STbad,       STignore}  /* STnoPrvdr  */
};

#ifdef	DCE_DEBUG
#define MAYBEPRINTSTATE(p)	\
    if (DCE_SVC_DEBUG_ATLEAST(dts_svc_handle, dts_s_states, 1)) \
	PrintState(p, (char *)NULL)
#else
#define MAYBEPRINTSTATE(p)	/* NULL */
#endif
#define Next(nstate)				\
{						\
    synchState = (nstate);			\
    MAYBEPRINTSTATE("Next ");			\
    break;							\
}
#define Wait(nstate)				\
{						\
    synchState = (nstate);			\
    MAYBEPRINTSTATE("Wait ");			\
    return; 							\
}



/*
 * Internal static prototypes
 */

static void PrintInaccuracyTimes (UTCValue *,
	    ExtInacc *,
	    BaseTimes *);

static void PrintState (char *, char *);

static char *GetStimulus (INstimuli );

static void PrintSynchTimes (UTCValue *, ExtInacc *,
	    UTCValue *, ExtInacc *);

static void Qrand (Que *);

static void FlushRespQs ( void )   ;

static void RemoveResponseElement (ResponseElement *);

static void SelectServers (ServerSet *,
	    int );

static int DecrAndPruneServers (ServerSet *, int );

static void DoSynch (ExtInacc *);

static int IsErrorToleranceExceeded (UTCValue *,
	    ExtInacc *,
	    UTCValue *,
	    ExtInacc *);

static void UpdateServerCacheStatus (ServerSet *,
	    UTCValue *,
	    ExtInacc *);

static void UpdateCourierStatus ( void ) ;

static int compareUUIDs (uuid_t *, uuid_t *);


static int CvtSecondsToUTCrel (unsigned long ,
	    UTCValue *);

static void 	WriteInaccTraceRecord (UTCValue *,
	    ExtInacc *,
	    UTCValue *,
	    ExtInacc *,
	    int );

static unsigned long CollectServerTimes (ServerSet *,
	    UTCValue *,
	    UTCValue *,
	    EndPointElement **);

static STsynchStates CalcSynchNeeds( void ) ;


/*
 *++
 *  ReInitState()
 *	Disable and then reinitialize the state machine.
 *	Cancel timers, End the clock adjustment, InitState.
 *  Implicit Inputs:
 *	ShmPtr, sdb.
 *	TSstate may not be equal to anything but OFF.
 *  Side Effects:
 *	Cancels timer events, flushes servers queues, reinits machine.
 *--
 */
void ReInitState(void)
{
    CacheElement *sp;

    if ( !IsState(K_STATE_OFF) )
    {
	BugCheck();
    }

    /*
     * Cancel any current clock adjustments, no not complete the
     * adjustment if NTP is running.
     */
    if (!(sdb.TPvalid && sdb.TPnoClockSet))
        AdjustClkEnd();

    /*
     * Cancel all DTSS timers.
     */
    CancelTimer( K_TIMER_NEXT_SYNCH );
    CancelTimer( K_TIMER_QUERY_TMO );
    CancelTimer( K_TIMER_ADVERTISE );

    /*
     * Flush the response queues.
     */
    FlushRespQs ();

    /*
     * Flush the server caches.
     */
    QloopBegin (&sdb.localSet.cacheQ, &sp)
        RemoveServer ((ServerSet *)&sdb.localSet, sp);
    QloopEnd

    QloopBegin (&sdb.globalSet.cacheQ, &sp)
	RemoveServer ((ServerSet *)&sdb.globalSet, sp);
    QloopEnd;
    sdb.NScontacted = FALSE;

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
             "Flushed cache of known servers\n"));

    /*
     * Clear the state machine.
     */
    InitState();
}


/*
 *++
 *  InitState ()
 *	Inits the state machine, called by main () at startup and
 *	by RestartState to restart the machine.
 *  Value Returned:
 * 	StatusBlk
 *  Side Effects:
 *	changes the state of the state machine
 *--
 */
StatusBlk InitState (void)
{
    StatusBlk statusBlk;
    utc_t now;
    struct reltimespec flushIntsp;
    struct timespec zeroInaccsp;
    UTCValue randomSeed;
    static int firstCall = TRUE;

    /*
     * Init the synchronization state machine
     */
    synchState = STnone;

    /*
     * Init the synchronization database queues
     */
    Qinit (&sdb.localSet.respQ);
    Qinit (&sdb.globalSet.respQ);
    Qinit (&sdb.TPset.respQ);
    Qinit (&sdb.localSet.cacheQ);
    Qinit (&sdb.globalSet.cacheQ);
    Qinit (&sdb.epochSet);
    sdb.NScontacted = FALSE;
    sdb.localSolDone= FALSE;

    sdb.doLANsolicit = FALSE;

    /*
     * Get the current time.
     */
    (void) utc_gettime (&now);

    /*
     * Init the cache flush value.
     */
    if (firstCall)
    {
	firstCall = FALSE;
	flushIntsp.tv_sec = K_CACHE_FLUSH_INTERVAL;
	flushIntsp.tv_nsec = 0;
	zeroInaccsp.tv_sec = 0;
	zeroInaccsp.tv_nsec = 0;
	(void) utc_mkbinreltime (&cacheFlushInt, &flushIntsp, &zeroInaccsp);
    }

    (void) utc_addtime (&nextCacheFlush, &now, &cacheFlushInt);

    /*
     * Init the random number generator and start the message ID
     * serial numbers with a random number to reduce the chance of
     * accepting an old message that's been knocking around the net
     * since before we restarted.  The random number seed will be the low-order
     * word of the current time value.
     */
    (void) utc_comptime (&randomSeed, (UTCValue *)NULL, (long *)NULL, &now);
    (void) srand( UTClow(&randomSeed) );
    sdb.msgIdSerial = rand ();
    sdb.synchSerialNo = rand ();

    ReturnDTSSstatus (DTSS_SUCCESS, statusBlk);
}

/*
 *++
 *  Synchronize ()
 *	The state machine that governs message traffic and time
 *	synchronization. Called only from main loop subroutines.
 *	This routine plus the two-dimensional state array is the logic that
 *	controls DTSS's message traffic that leads up to a synchronization.
 *	The machine will not synchronize if the state is set to OFF.
 *	To abort a synchronization, change TSstate to anything other
 *	than Synchronizing.
 *  Inputs:
 *         stimulus - one of the stimulus types defined in DTSS_SERVICE.H
 *         eblkPtr  - pointer to event block describing the stimulus event
 *  Implicit Inputs:
 *         synchState - the current state of the machine
 *	   shmPtr...TSstate - the current mode of the Service
 *  Implicit Outputs:
 *         synchState - the current state of the machine
 *  Side Effects:
 *         sends messages, synchs time, etc.
 *         caller is responsible for reposting reads and restarting timers
 *--
 */

void Synchronize (
  INstimuli stimulus,
  EventBlk *eblkPtr
)
{
    ExtInacc Icomp, *IcompPtr;
    STsynchStates newState;
    UTCValue now;
    utc_t nowutc;
    enum utc_cmptype timeRelation;
    ResponseElement *rp;
    int targetRespLen;
    static int solRetriesLeft;

    /*
     * Get the new state from state machine table, based on old state
     * and stimulus. If the new state is STignore, then the stimulus is
     * extraneous (eg, an extra datagram replying to a time query) and
     * we ignore the message. If the state is STbad, then there is a bug
     * somewhere and we report the bad state transition.
     */
    /*
     * Do not synchronize if the current state if OFF.
     * Someone got a disable command in before we had time to
     * disable the synchronization timer.
     */
    if ( IsState( K_STATE_OFF ) )
	return;

    /*
     * assume no synch completes
     */
    IcompPtr = NULL;
/*
 * Portability issue: (int) cast "synchState" and "stimulus" because pcc
 * does not currently allow the use of enumerated types in array indexing.
 */

    newState = nextState[(int)synchState][(int)stimulus];
    /*
     * Special pseudo-states. These do not change the current
     * synchronization sub-state.
     */
    if (newState == STignore)
    {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
	         "Ignoring message (STignore) %s\n", GetStimulus(stimulus)));
        return;
    }
    if (newState == STbad)
    {
	PrintState("bad state transition from %s ", GetStimulus(stimulus));
        BugCheck ();
    }
    synchState = newState;
#ifdef	DCE_DEBUG
    if (DCE_SVC_DEBUG_ATLEAST(dts_svc_handle, dts_s_states, 1))
	     PrintState("%s ", GetStimulus(stimulus));
#endif
    /*
     * Perform any processing required by the new state.
     */
    for (;;)
    {
	switch (synchState)
	{
	    /*
	     * This state is used as an exit point for the synchronization
	     * process. Initiate a timer for the next synchronization and
	     * exit.
	     */

	    case STnone:
		if ((IsState(K_STATE_SYNCH_ADJUST) ||
		     IsState(K_STATE_SYNCH_SET))   &&
		    !StateChange( K_STATE_ON ))
		    BugCheck();

		Wait (STnone);

	    /*
	     * The synchronization timer has elapsed and it is time to
	     * synchronize.  If we are a server and have a provider, then
	     * synchronize with the provider, else synchronize with
	     * other servers.
	     */
	    case STstartSynch:
		/*
		 *  Common Initialization.
	 	 */
		/*
 		 * If the mode is updating, then log an update
		 * done.  This is the only place in DTSS that
		 * we have a state transition out of update.
		 */
  		/* Set the synch state.  Note that we try and synch
		 * by adjust first and then by set.  If the synch
		 * command set the synch_set state then the _SET will
		 * succeed, else the _ADJUST should succeed.   If both
		 * fail, then we have scheduled a synch while the service
		 * is off or updating - fatal.
		 */
		if ( !StateChange( K_STATE_SYNCH_ADJUST) &&
		     !StateChange( K_STATE_SYNCH_SET))
		    BugCheck();
		/*
		 * Increment the synchronization serial number, it can
		 * not be zero since the synch serial number of
		 * servers is initialized to zero.
		 */
		sdb.synchSerialNo++;
		if ( sdb.synchSerialNo == 0 )
		    sdb.synchSerialNo++;

                sdb.localSet.numRecvd = sdb.globalSet.numRecvd = 0;
		sdb.TPset.numRecvd = sdb.TPset.numResp = 0;

                /*
                 * save a copy of params that the control program
                 * might change, so we will have consistent values
                 * during the synchronization.
                 */
                sdb.mgtParams = shmPtr->mgtParams;
		sdb.checkIntTime = sdb.TPvalid = FALSE;
		sdb.TPnextPoll = 0;

		sdb.globalSet.numServers = sdb.globalSet.numServerSet;
		sdb.localSet.numServers = sdb.localSet.numServerSet;

		/*
		 * If this synch is a check-int go do it
		 */
		if (shmPtr->mgtParams.hasProvider) {
		    if (utc_comptime(&now, (UTCValue *)NULL, (long *)NULL,
                                     (utc_t *)NULL)) {
			BugCheck();
		    }
		    if (IsUTClt(&nextCheckInt, &nextSynchTime) &&
			IsUTClt(&nextCheckInt, &now))
		        Next(STstCkInt);
		}
		/*
		 * If we are a server and we are able to contact
		 * a time provider, then synchronize with it.
		 */
		if (IsServer())
		    Next( STstPrvdrSync );

		/*
		 * Else do a server sync.
		 */
		shmPtr->mgtParams.hasProvider = FALSE;
                sdb.TPvalid = FALSE;
		UTCzero (&nextCheckInt);
		Next( STstServSynch );

	    /*
	     * We are about to start a new synchronization. We are
	     * synchronizing with servers only.
	     */
	    case STstServSynch:

                if ((! IsQempty (sdb.localSet.respQ )) ||
		    (! IsQempty (sdb.globalSet.respQ)) ||
		    (! IsQempty (sdb.TPset.respQ    )) )
                    BugCheck ();
                /*
                 * Calculate how many servers we need to contact.
                 * Clerks contact the minServers as the manager told them.
                 * Servers contact one fewer, including themselves as one.
		 *
		 * If this is a server and a new epoch was declared, but no
		 * time specified, set minserver = 1 (in reality, minservers
		 * is 2, since it uses its time plus another server's to
		 * calculate the correct time). Its current inacc is infinite and
		 * we must make sure this server completes this synchronization.
                 */

		if (IsServer() && newEpochNoTime)
		    sdb.minServers = 1;
		else
		    sdb.minServers = IsClerk () ? sdb.mgtParams.minServers :
			                          sdb.mgtParams.minServers - 1;

                /*
                 * See if we've passed the server cache flush time.
                 */
		(void) utc_gettime (&nowutc);
		(void) utc_cmpmidtime (&timeRelation, &nowutc, &nextCacheFlush);
		if (timeRelation == utc_greaterThan)
                {
		    /*
		     * Flush the server caches and calculate the time for the
		     * next server cache flush.
		     */
                    (void)utc_addtime(&nextCacheFlush, &nowutc, &cacheFlushInt);
                    FlushServerCache ();
                }
                /*
		 * Contact the name service, if need be, to fill the
		 * cache of global servers. We must contact DNS before
		 * we contact any servers (local or global), because
		 * enumerating a directory can take quite a while and
		 * we don't want that time included in the
		 * synchronization.
		 *
		 * sdb.NScontacted is cleared at startup and in
		 * FlushServers.
                 */
		if ((! sdb.NScontacted)
		    ||
		    (sdb.globalSet.numServers == 0)
		    )
		{

		    PopulateGlobalCache ();

		    /*
		     * set the ns contacted to true only
		     * if the global directory was indeed
		     * set.
		     */
		    sdb.NScontacted = TRUE;
		}

                /*
                 * Rearrange the cache queues into a new random order.
                 * SelectServers will take the servers it needs from the
                 * head of the lists.
                 */
                Qrand (&sdb.localSet.cacheQ );
                Qrand (&sdb.globalSet.cacheQ);
		/*
		 * Stop any clock adjustment now is in progress.
		 */
		if (!(sdb.TPvalid && sdb.TPnoClockSet))
		    AdjustClkEnd();

                /*
                 * If this is a clerk, then enable a new local solicit
		 * on every synchonrization, see CalcSynchNeeds.
		 *
		 * If we have RPC transport for the local set, we allow servers
		 * to solicit for new RPC servers, which don't use broadcast
		 * messages to discover each other.
		 *
                 */
		sdb.localSolDone = FALSE;

                /*
                 * Figure out what is needed for a synchronization.
                 */
		Next ( CalcSynchNeeds () );

    /*
     * The following 5 states handle synchronization with
     * the time provider.
     */
	    case STstPrvdrSync:

		/*
		 * Stop any clock adjustment which is in progress.
		 */
		AdjustClkEnd();

                /*
		 * Send a request to the Time provider process,
		 * and then wait for that response.
		 */
		if (!ContactTimeProvider ()) {
		    (void)LogEventTPfailure();
		    Next(STstServSynch);
		}
		InitiateTimer(K_TIMER_QUERY_TMO,	  /* index of timer */
			      &sdb.mgtParams.LANtimeOut); /* O.K. for now */
		Wait( STprvdrWTMO );

	    /*
	     * Wait for the TP to issue an initial response.
	     * The response will contain the TP control information,
	     * nextPoll and timeout.
	     */
            case STprvdrWTMO:
	    {
		unsigned long timeout;
		unsigned long nextPoll;
		UTCValue tmoInt;
		TPmsgStatus msgStatus;
		unsigned long noSetBit;

		/*
		 * If the message Serial Number is valid, the time
		 * provider state is TPsuccessful and the message
		 * is of correct format, then get the timeout value
		 * into sdb.TPtimeout.
		 */
		msgStatus = ProcessTPtmoResponse(eblkPtr,
						 &timeout,
						 &nextPoll,
						 &noSetBit);
                /*
		 * Wait for a valid message.
		 */
		if ( msgStatus == TPmsgSyncSerMismatch )
		{
		    Wait(STprvdrWTMO);
		}

		CancelTimer(K_TIMER_QUERY_TMO);

		/*
		 * If we got no response at all, then we assume there is no TP
		 */
		if (msgStatus == TPmsgNoResponse) {
		    ReleaseTimeProvider (eblkPtr);
		    Next( STstServSynch );
		}
		/*
		 * If the TP hardware is bad, or the time out value is
		 * out of bounds, synch with peers.
		 */
		if ( msgStatus == TPmsgTypeMismatch ||
		     msgStatus == TPmsgInvalid  )
		{
		    (void)LogEventTPfailure();
		    ReleaseTimeProvider(eblkPtr);
		    Next(STstServSynch);
		}

		/*
		 * Convert the user input to a UTC relative time.  If
		 * an overflow occurs in the process abort this
		 * synchronization with the TP and go synch with your
		 * peers.
		 */
		if (CvtSecondsToUTCrel( timeout, &tmoInt))
		{
		    (void)LogEventTPfailure();
		    ReleaseTimeProvider (eblkPtr);
		    Next(STstServSynch);
		}
		sdb.TPnextPoll = nextPoll;
                sdb.TPnoClockSet = noSetBit;
		InitiateTimer( K_TIMER_QUERY_TMO, &tmoInt );

		Wait(STprvdrWait);
	    }

	    /*
	     * Wait for the TP to respond to our time query.
	     * This state is a place holder in the state table
	     * while we wait for the time stamps.
	     */
	    case STprvdrWait:
	    {
		TPmsgStatus msgStatus;

		/*
		 * We get here because a time provider message arrived.
		 * If it is a valid TP message then sync, else
		 * resume the wait.
		 *
		 * If the message id is invalid, wait for a valid one.
                 * If the message is bad, release the TP and continue sync.
		 */
		msgStatus = ProcessTPresponse( eblkPtr );
		if ( msgStatus == TPmsgInvalid ||
		     msgStatus == TPmsgTypeMismatch )
		{
		    (void)LogEventTPfailure();
		    CancelTimer(K_TIMER_QUERY_TMO);
		    ReleaseTimeProvider( eblkPtr );
		    Next(STstServSynch);
		}

		/*
	         * Will wait only if the synchronization serial numbers
		 * do not match.
		 */
		if ( msgStatus == TPmsgSyncSerMismatch )
	 	{
		    Wait( STprvdrWait );
		}

		/*
		 * Success !
		 * We are assured that the TP is valid. Cancel the timer,
		 * and do the real work, sync the clock with the TP data.
		 */
                Next( STprvdrSync );
	    }

	    case STprvdrSync :
                /*
		 * Sync with the time provider info, cleanup, reinitiate
		 * the sync timer and go to the top of the state machine.
		 */
		CancelTimer(K_TIMER_QUERY_TMO);
		/*
		 * Note the provider success.
		 */
		sdb.TPvalid = TRUE;

                /*
		 * Do the actual synchronization.
		 * If a TDF occured then skip the synchronization and
		 * schedule the next synch.
		 */
		if ( IsState (K_STATE_SYNCH_ADJUST ) ||
		     IsState (K_STATE_SYNCH_SET) ) {
		    DoSynch (&Icomp);
		}
		/*
		 * Synchronization is done, so it's OK for the name
		 * service to start updating its database.  We set
		 * this flag at the end of every synch, but it only
		 * matters at the end of the first synch: DNS waits
		 * for the first synch to complete before allowing
		 * database updates on startup.
		 */
		sdb.synchDoneFlag = TRUE;

		DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
			 "TP Synchronization complete\n"));
                /*
		 * Release the time provider.
		 */
		ReleaseTimeProvider (eblkPtr);

		/*
                 * Flush the response queue
                 */
                FlushRespQs ();

		ScheduleNextSynch ((ExtInacc *)NULL);	/* don't need Icomp */

		/*
		 * Since we have a time provider attatched, make sure
		 * our acting courier role attribute is reflected
		 * properly.
		 */
		UpdateCourierStatus();

		Next (STnone);

	    case STnoPrvdr:
		/*
		 * The timer has expired, the TP has not been able
		 * to respond in time.  Mark the TP as invalid and
		 * sync with the other servers.  Don't log an event, since
		 * this state may be reached if there is no TP on this node.
		 */
		sdb.TPvalid = FALSE;
		/* cancel the TP thread here when cancellation is implemented */
		/* by RPC. */
		ReleaseTimeProvider ((EventBlk *)NULL);
		LogEventTPtimeout();
		Next(STstServSynch);

	    case STprvdrTMO:
		/*
		 * The timer has expired, the TP has not been able
		 * to respond in time.  Mark the TP as invalid and
		 * sync with the other servers.  Log a TP failure event.
		 */
		sdb.TPvalid = FALSE;
		(void)LogEventTPfailure();
		/* cancel the TP thread here when cancellation is implemented */
		/* by RPC. */
		ReleaseTimeProvider ((EventBlk *)NULL);
		Next(STstServSynch);

    /*
     * The following 2 states handle CheckInt.
     * Request times from all our peers and detect any faults.
     */
  	    case STstCkInt:
		DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
			"Starting CheckInt\n"));
                /*
		 * If there are any known servers then perform a
		 * checkint with them, otherwise just fall through and
		 * wait for the next checkInt.  If the server has not
		 * done a local solicit yet, go do it first.
		 */
		sdb.checkIntTime = TRUE;

		if (! sdb.localSolDone )
		    Next( STsolInit );

		if (!IsQempty(sdb.localSet.cacheQ) )
		    Next( STrqInit );

		Next( STdoCkInt );

            case STdoCkInt:
		/*
		 * Log all faulty servers by calling DoSynch.
		 * An actual synchronization does not occur since
		 * we are doing a checkInt.
		 */
		if ( sdb.localSet.numRecvd != 0 )
		    DoSynch(&Icomp);

		/*
		 * Synchronization is done, so it's OK for the name
		 * service to start updating its database.  We set
		 * this flag at the end of every synch, but it only
		 * matters at the end of the first synch: DNS waits
		 * for the first synch to complete before allowing
		 * database updates on startup.
		 */
		sdb.synchDoneFlag = TRUE;

                /*
                 *   Log the different epochs
		 */
		(void)LogEventDiffEpoch( (BadEpochSet *)&sdb.epochSet );

		/*
		 * Flush the response queues.
		 */
		FlushRespQs ();

		DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
			"Finished CheckInt\n"));
		ScheduleNextSynch((ExtInacc *)NULL);

		sdb.checkIntTime = FALSE;

		Next( STnone );

    /*
     * The following 4 states handle requesting the time from
     * servers in the local set.
     */
            case STrqInit:
		/*
		 * Figure out how many local servers we should contact.
		 */
		if ( IsServer () )
		    /*
		     * A server always queries all other servers in
		     * its local set
		     */
		    targetRespLen = sdb.localSet.numServers;
		else
		{
		    /*
		     * Look for at least K_LOCAL_SERVERS_GOAL servers
		     * (that's 3), even if minServers is less than that.
		     *
		     * SelectServers doesn't mind if we ask for more
		     * than there are in the cache, it will return
		     * what is available.
		     */

		    targetRespLen = sdb.minServers > K_LOCAL_CLERKS_GOAL ?
			            sdb.minServers : K_LOCAL_CLERKS_GOAL;
		}

                /*
                 * Select local servers from the cache to replace any
                 * in the response queue that we have not yet heard from.
                 * On the first time through here for each synchronization,
                 * this will mean all response entries, on subsequent times,
                 * only those presumed dead.
                 */
		SelectServers ((ServerSet *)&sdb.localSet, targetRespLen);
                Next (STrqSend);

	    case STrqSend:
                /*
                 * (re)send to those servers that haven't yet responded
                 */
		RequestTimeFromServers ((ServerSet *)&sdb.localSet, SRVlocal);
                InitiateTimer (K_TIMER_QUERY_TMO,         /* index of timer  */
                               &sdb.mgtParams.LANtimeOut);/* ptr to UTCValue */
		Wait (STrqWait);

	    /*
	     * This state provides a holding point while we are waiting for
	     * responses to be returned. Merge the received message into
	     * the synchronization database.
	     */
	    case STrqWait:
                /*
                 * Find message's msgIdSerial in response queue.
                 * If not found, ignore the message.
                 * (Caller will repost read.)
                 */
		if ( rp = ProcessTimeResponse (eblkPtr) )
		{
		    DisconnectLink ( rp->serverPtr->portPtr,
				    (int *)&rp->serverPtr->linkId);
                    if (++sdb.localSet.numRecvd >= sdb.localSet.numResp)
                    {
                        /*
                         * All the outstanding requests have been answered.
                         */
                        CancelTimer (K_TIMER_QUERY_TMO);
			/*
			 * If this state was entered from CheckInt, go on with
			 * the CheckInt
			 */
			if (sdb.checkIntTime)
			    Next(STdoCkInt);
			Next ( CalcSynchNeeds () );
                    }
                }
		Wait (STrqWait);

	    /*
	     * The request response timer has expired for local
	     * queries. Try again to contact more local servers unless
	     * the retry count has expired.
	     */
	    case STrqTimer:
		/*
		 * Decrement retrans counter for all servers that
		 * haven't yet responded.  Remove those whose retrans
		 * counter has reached zero.  If any servers still
		 * have non-zero retrans counters, send to them.
		 */
                if (DecrAndPruneServers ((ServerSet *)&sdb.localSet,
				         K_SERVER_LOCAL))
		    Next (STrqSend);
		/*
		 * If this state was entered from CheckInt, go on with
		 * the CheckInt
		 */
		if (sdb.checkIntTime)
		    Next(STdoCkInt);
		Next ( CalcSynchNeeds () );

    /*
     * The following 4 states handle soliciting new servers from
     * the LAN.
     */

	    /*
	     * Set up to solicit for new local servers.
	     */
	    case STsolInit:
		/*
		 * Load up the non-autoconfigured part of the local
		 * cache since there are fewer than minServers servers in
		 * the local cache.
		 */
		PopulateLocalCache ();
	    	sdb.localSolDone = TRUE;

		/*
		 * If the configuration allows it, do a LAN solicit.  If we're
		 * a server, only do the LAN solicitation once, when we start
		 * up.
		 */
		if (sdb.doLANsolicit) {
		    if (IsServer())
			sdb.doLANsolicit = FALSE;

		    solRetriesLeft = sdb.mgtParams.repetitions;
		    Next (STsolSend);
		}

		if ( sdb.checkIntTime )
		    Next( STstCkInt );
		Next( CalcSynchNeeds () );

	    /*
	     * Send a multicast message on each LAN connection to solicit
	     * for more servers.
	     */
	    case STsolSend:
                InitiateTimer (K_TIMER_QUERY_TMO,         /* index of timer  */
                               &sdb.mgtParams.LANtimeOut);/* ptr to UTCValue */
		Wait (STsolWait);

	    /*
	     * This state provides a holding point while we wait for a single
	     * solicitation response. Merge the information from the response
	     * into the local cache and then retry contacting any new servers
	     * that we have heard about.
	     */
	    case STsolWait:
	        /*
	         * See if this reponse is the first on its port.
	         * If not, ignore the message.
	         * If so, merge its server list into the local cache
	         * and if we now have responses on all ports, return true.
	         */
		Wait (STsolWait);

	    /*
	     * The solicit response timer has expired. Try again to solicit
	     * for more servers unless the retry count has expired.
	     */
	    case STsolTimer:
		if (--solRetriesLeft > 0)
		    Next ( STsolSend );

		if ( sdb.checkIntTime )
		    Next( STstCkInt );

		Next( CalcSynchNeeds () );

    /*
     * The following states handle contacting global servers.
     */

	    /*
	     * Choose one or more global servers from which to request time.
	     */
	    case STgblInit:
		/*
		 * Figure out how many global servers we should contact.
		 * We wouldn't be here if there were no globals to
		 * contact, so the min should be one.
		 * A server should contact all globals, a clerk should
		 * contact min(minServers, number of globals).
		 */
		targetRespLen = sdb.minServers - sdb.localSet.numResp;
		if (targetRespLen < 1)
		    targetRespLen = 1;

                /*
                 * Select global servers from the cache to replace any
                 * in the response queue that we have not yet heard from.
                 * On the first time through here for each synchronization,
                 * this will mean all response entries, on subsequent times,
                 * only those presumed dead.
                 */
		SelectServers ((ServerSet *)&sdb.globalSet, targetRespLen);
		Next (STgblLinks);

	    /*
	     * Set up any required transport links to the selected servers.
	     */
	    case STgblLinks:
		/*
		 * Try to open links on connnectionful transports
		 * before sending messages so the open time will not
		 * be included in the round-trip time.  For servers
		 * reached by a connectionless xport, this is a no-op.
		 * If the links are not immediately opened, wait for them
		 * in the state STgblLinksWait.  This allows other events and
		 * messages to be processed if the links take a long time to
		 * establish.
		 */
		Next (STgblSend);
	    /*
	     * Wait here while links are being established to global servers
	     * that require connectionful transport.
	     */
	    case STgblLinksWait:

		Next (STgblSend);

	    /*
	     * Send time requests to the global servers we've selected.
	     */
	    case STgblSend:
		/*
		 * Now send a time request to each server in the
		 * global request queue.
		 */
		RequestTimeFromServers ((ServerSet *)&sdb.globalSet,
					SRVglobal);
                InitiateTimer (K_TIMER_QUERY_TMO,         /* index of timer  */
                               &sdb.mgtParams.WANtimeOut);/* ptr to UTCValue */
		Wait (STgblWait);

	    /*
	     * This state provides a holding point while we are waiting for
	     * a response from the global server.
	     */
	    case STgblWait:
		/*
		 * If this was an expected message, process it, count it,
		 * and then see if we are expecting any more.
		 */
		if ( rp = ProcessTimeResponse (eblkPtr) )
		{
		    DisconnectLink ( rp->serverPtr->portPtr,
				    (int *)&(rp->serverPtr->linkId));
                    if (++sdb.globalSet.numRecvd >= sdb.globalSet.numResp)
                    {
                        /*
                         * All the outstanding requests have been answered.
                         */
                        CancelTimer (K_TIMER_QUERY_TMO);
                        Next ( CalcSynchNeeds () );
                    }
                }
                Wait (STgblWait);      /* Wrong message, wait for another */

	    /*
	     * The global response timer has expired. Disconnect the link
	     * to the global server, flush it's element from the global cache
	     * and try to contact another global server.
	     */
	    case STgblTimer:
		/*
		 * Decrement retrans counter for all servers that
		 * haven't yet responded.  Remove those whose retrans
		 * counter has reached zero.  If any servers still
		 * have non-zero retrans counters, send to them.
		 */
                if (DecrAndPruneServers ((ServerSet *)&sdb.globalSet,
					 K_SERVER_GLOBAL))
		    Next (STgblLinks);
		Next ( CalcSynchNeeds () );

	    /*
	     * We've done all we can to contact minServers. If we got
	     * enough responses, do a synchronization, else just clean
	     * up and schedule another synchronization after the
	     * normal time.
	     */
	    case STsynch:
		/*
		 * Assume we don't complete the synch so we don't have a
		 * computed inaccuracy for scheduling the next synch.
		 */
		IcompPtr = NULL;

#ifdef DEBUG
		/*
		 * Make sure all the dead servers are removed.
		 */
	    {
		ResponseElement *rp;

		QloopBegin (&sdb.localSet.respQ, &rp)
		{
		    if ( ! rp->responseRecvd )
		    {
			DCE_SVC_DEBUG((dts_svc_handle, dts_s_states,
			    svc_c_debug1,
	"******* Bug: Local resp has unresponsive servers at STsynch\n"));
		    }
		}
		QloopEnd;
		QloopBegin (&sdb.globalSet.respQ, &rp)
		{
		    if ( ! rp->responseRecvd )
		    {
			DCE_SVC_DEBUG((dts_svc_handle, dts_s_states,
			    svc_c_debug1,
	"****** Bug: Global resp has unresponsive servers at STsynch\n"));
		    }
		}
		QloopEnd;
	    }
#endif
		/*
		 * We have minServers, do the synchronization, unless
		 * minServers was 1 and this node is the 1 server.
		 * (For debugging, we will allow a synch if we are the
		 * only server.)
		 *
		 * If there was no TDF change in the middle of this
		 * synchronization then complete the operation else
		 * schedule the next synch.
		 */
#ifndef DEBUG
		if (sdb.localSet.numRecvd + sdb.globalSet.numRecvd > 0)
#endif
		    if ( IsState(K_STATE_SYNCH_ADJUST ) ||
			 IsState(K_STATE_SYNCH_SET ))
		    {
			DoSynch(&Icomp);
			/*
			 * Do not corrupt the value of IcompPtr.  At this
			 * instant, IcompPtr == NULL, which causes the
			 * the current inaccuracy to be used.
			 */
			IcompPtr = &Icomp;
		    }

		DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
			 "Synchronization complete\n"));
		Next (STcleanup);

	    /*
	     * We've done all we can to contact minServers and it just
	     * wasn't enough. Just clean up and schedule another
	     * synchronization after the normal time.
	     */
	    case STnoSynch:
		/*
		 * Assume we don't complete the synch so we don't have a
		 * computed inaccuracy for scheduling the next synch.
		 */
		IcompPtr = NULL;

		/*
		 * We've tried all sources, log an event and wait for
		 * next synchronization.
		 */
		(void)LogEventTooFewServers((unsigned long)
				(sdb.localSet.numServers  +
				    sdb.globalSet.numServers +
				    (IsServer () ? 1 : 0)),
				  (unsigned long) sdb.mgtParams.minServers );
		DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
					 "Synchronization not complete\n"));
		Next (STcleanup);

	    /*
	     * Clean up after this synchronization and prepare for the next.
	     */
	    case STcleanup:
		/*
		 * Synchronization is done as best we can right now,
		 * so it's OK for the name service to start updating
		 * its database.  We set this flag at the end of every
		 * synch attempt, but it only matters at the end of
		 * the first synch attempt: DNS waits for the first
		 * synch to complete before allowing database updates
		 * on startup.
		 */
		sdb.synchDoneFlag = TRUE;

		/*
		 * clear this flag in case it was set due to an epoch change.
		 */
		newEpochNoTime = 0;

                /*
                 * Set a timer for the next synchronization.
                 */
		ScheduleNextSynch (IcompPtr);

		/*
		 * Log all the different epochs detected.
		 */
		(void)LogEventDiffEpoch( (BadEpochSet *)&sdb.epochSet );

                /*
                 * The synchronization is complete, clean up
                 */

                FlushRespQs ();

		/*
		 * since the response queues were flushed, better
		 * update the courier status.
		 */
		UpdateCourierStatus();

		Next (STnone);

	    default:
		BugCheck ();
	}
    }
}


/*
 *++
 *  CalcSynchNeeds()
 *	   Calculate what servers we need to hear from to complete
 *         this synchronization. Return the next state for the state
 *         machine so as to query those servers.
 *  Implicit Inputs:
 *         the sdb (synch data base)
 *  Value Returned:
 *         next state, caller expected to call in Next macro.
 *--
 */

static STsynchStates CalcSynchNeeds (void)
{
    int targetNum;
    /*
     * only attempt to contact servers with a non-zero timeout.
     */
     if (IsUTCzero(&sdb.mgtParams.WANtimeOut))
	 sdb.globalSet.numServers = 0;

    if (IsUTCzero(&sdb.mgtParams.LANtimeOut))
	sdb.localSet.numServers = 0;

    /*
     * If we've not yet heard from enough local servers, try to
     * contact some local servers.
     *
     * Servers Always contact at least sdb.minServers.
     * Clerks  Always attempt to contact at least K_LOCAL_CLERKS_GOAL
     * servers, but never fewer than sdb.minServers.
     */
    if (IsServer())
	targetNum =
	    sdb.minServers > sdb.localSet.numServers ?
	    sdb.minServers : sdb.localSet.numServers;
    else
    {
	if (sdb.minServers < K_LOCAL_CLERKS_GOAL)
	{
	    if (sdb.localSet.numServers >= K_LOCAL_CLERKS_GOAL)
		targetNum = K_LOCAL_CLERKS_GOAL;
	    else
	    {
		if (sdb.localSolDone)
		    targetNum = sdb.localSet.numServers;
		else
		    targetNum = K_LOCAL_CLERKS_GOAL > sdb.minServers ?
			K_LOCAL_CLERKS_GOAL : sdb.minServers;
	    }
	}
	else
	    targetNum = sdb.minServers;
    }

    if (sdb.localSet.numRecvd < targetNum)
    {
	/*
	 * If we know of any local servers that we haven't heard from
	 * yet, and if the total of all locals and all known globals
	 * would be enough to make up the expected number of
	 * responses, then go to STrqInit to send out some more local
	 * requests.
	 *
	 * sdb.globalSet was updated in STstartSynch with a
	 * call to PopulateGlobalCache ().
	 */
	if ((sdb.localSet.numServers > sdb.localSet.numRecvd) &&
	    (sdb.localSet.numServers +
	     sdb.globalSet.numServers >= sdb.minServers))
	{
	    return (STrqInit);
	}

	/*
	 * Lets see if we can find some locals.
	 * Note that both clerk and server will find localSolDone
	 * to be false the first time. Only the clerk ever gets
	 * the flag cleared after that.
	 */
	if ( ! sdb.localSolDone )
	    return (STsolInit);
    }

    /*
     * We've done all we can with the local set, let's try the
     * global set if need be: if total local and global responses we've
     * received don't add up to minServers, try for more globals.
     */
    if ((sdb.localSet.numRecvd + sdb.globalSet.numRecvd) < sdb.minServers)
    {
	/*
	 * If there are global servers we haven't heard from yet, and
	 * total local and global we've heard from isn't sufficient,
	 * and there would be enough if we heard from all globals,
	 * then go query some globals.
	 */
	if ((sdb.globalSet.numServers > sdb.globalSet.numRecvd) &&
	    (sdb.localSet.numRecvd +
	     sdb.globalSet.numServers >= sdb.minServers))
	{
	    if ( IsServer () )
		UpdateCourierStatus ();
	    return (STgblInit);
	}
	/*
	 * Else there we don't know of enough servers to satisfy
	 * minServers
	 */
	return (STnoSynch);
    }

    /*
     * We have enough servers, if we haven't heard from a global, then
     * let's see if this node is a courier and thus must contact at
     * least one global set server.
     */
    if (( IsServer () ) &&
	(sdb.globalSet.numRecvd == 0))
    {
	UpdateCourierStatus ();
	if ( IsCourier () )
	{
	    /*
	     * If there are any globals to contact, go get 'em, else
	     * tell the world of our inability to find a global.
	     */
	    if (sdb.globalSet.numServers > 0)
		return (STgblInit);

	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
		     "***No globals contacted\n"));

	    (void)LogEventNoGlobalServers();

	}
    }

    /*
     * Go do a synchronization based on the servers we've heard from.
     */
    return (STsynch);
}



/*
 *++
 *  Qrand (qHeadPtr)
 *	Rearranges a queue into a random order
 *  Inputs:
 *	Pointer to a queue head (struct Que)
 *  Side Effects:
 *	changes the order of the elements in the input queue
 *
 *--
 */
static void Qrand ( 
    Que *qHeadPtr
)
{
#define ARRAYSIZE 53                   /* a prime number to avoid cycles */

    Que *(elemPtrArr[ARRAYSIZE]);
    Que *elemPtr;
    unsigned int r;

    /*
     * initialize the array to be empty
     */
    for (r=0 ; r<ARRAYSIZE ; r++)
        elemPtrArr[r] = 0;

    /*
     * Remove each element of the queue and put it in a singly-linked list
     * hanging off a randomly-selected bucket in the array. If multiple
     * elements hit the same bucket in the array, put the new one at the head
     * of the bucket list. This reduces some of the randomness, not enough to
     * be worth more complexity, though.
     */

    QloopBegin (qHeadPtr,&elemPtr)
        Qremove (elemPtr);
        r = rand () % ARRAYSIZE;
        elemPtr->next = elemPtrArr[r];
        elemPtrArr[r] = elemPtr;
    QloopEnd

    /*
     * Scan the array, enqueuing each element as we find it. This builds
     * up the queue again, in its new random order.
     */

    for (r=0 ; r<ARRAYSIZE ; r++)
    {
	Que *ep;

        elemPtr = elemPtrArr[r];
        while (ep = elemPtr)
        {
            elemPtr = elemPtr->next;
            QinsertLast (qHeadPtr, ep);
        };
    }
}

/*
 *++
 *  SelectServers ()
 *	Selects the local or global servers to be used in the next
 *	synchronization.
 *  Implicit Inputs:
 *	Assumes that caller has called Qrand (&sdb.xxxSet.cacheQ).
 *	shmPtr - ptr to shared memory.
 *  Implicit Outputs:
 *	Sets up the synchronization database response queue
 *  Side Effects:
 *	Builds and inits the response queue
 *	Moves the Q header around the sdb.xxxSet.cacheQ (really a ring) so
 *	it points just after the elements used in the response Q.
 *--
 */
static void SelectServers ( 
    ServerSet *setPtr,
    int targetRespLen
)
{
    int i;                             /* number of resp elems to enqueue */
    CacheElement *lp;                  /* pointer into cache q */
    ResponseElement *rp;               /* pointer into response q */
    /*
     * Quit now if there are no servers in the cache
     */
    /*
     * Quit now if there are no servers in the cache
     * Check numServers in case it is zero.  CalcSynchNeeds
     * sets numServers to zero to let us know not to contact
     * any servers at all.
     *
     */
    if ( IsQempty (setPtr->cacheQ) || setPtr->numServers == 0 )
        return;

    /*
     * Fill the response queue with the servers we expect to contact.
     * This routine is called once at the start of each synchronization.
     * It will be called again if one or more of the servers originally
     * selected fails to respond. On the second and subsequent calls, this
     * routine will replace the dead servers with new ones from the cache.
     *
     * Note that the cache queue is rearranged into a new random order
     * (Qrand) at the start of each synchronization, so here we just
     * have to pick them off the head of the list.
     *
     * The cache is a circular, doubly-linked list, as are all the
     * lists in this program. setPtr->cacheQ is the list header. It
     * always points to the first available server if there is one.
     * The first time through here on each synchronization, the
     * response queue is empty, so the first server in the cache list
     * is by definition available.  After we use some servers for the
     * response queue, we move the setPtr->cacheQ header around the
     * circular list to point after the last server we used. Thus, the
     * next time through, we will find a new server or there are none
     * available. New servers discovered from advertisements or
     * solicitation responses are always added to the end of the cache
     * list. Since the queue header precesses around the list, we need
     * some way to find which servers we've used. We mark each one
     * with the current synchronization serial number when we use it.
     * Then, before using a server, we check its sync serial number to
     * see if we've used it already. If we find one we've used
     * already, then we've come all the way around the list and there
     * are no more available.
     * The setPtr->numServers is a count of the number of servers which
     * can participate in the synchronization.  RemoveServer decrements
     * this value when a server becomes unavailable.  setPtr->numServerSet
     * is the actual count of known servers.  A nonresponsive server
     * can be left on the server queue since select servers will not
     * choose a server twice.  setPtr->numServerSet allows us to remember
     * how many of those servers there are (numServerSet - numServers).
     * The synchronization algorithm uses the numservers parameter to
     * determine if there are enough servers to synch with.
     */
    i = targetRespLen - setPtr->numResp;
    if ( i <= 0 )
        return;
    QloopBegin (&setPtr->cacheQ, &lp)
    {
	/* If we have enough servers, break
	 */
        if ( i-- <= 0 )
            break;
        /*
	 * If there are no more servers we haven't already used, break
	 */
        if ( lp->synchSerialNo == sdb.synchSerialNo )
            break;
        /*
         * build a new response queue element
	 */
        rp = DTSSmallocStruct (ResponseElement);
        /*
         * Point the new response element to the selected cache element
	 */
        rp->serverPtr = lp;
        rp->responseRecvd = FALSE;
	rp->differingEpoch= FALSE;
	rp->threadValid = FALSE;
	/*
	 * Set the number of retries to allow for this new server. If
	 * we reach it by way of raw datagrams, try several times. If
	 * we reach it by a virtual circuit (DECnet, TP4) then only
	 * allow one try at this level.
	 */
	if (lp->portPtr->datagram)
	    rp->retriesLeft = sdb.mgtParams.repetitions;
	else
	    rp->retriesLeft = 1;

        QinsertLast (&setPtr->respQ, rp);
        setPtr->numResp++;
        /*
         * Mark the cache element so we'll know we've used it
	 */
        lp->synchSerialNo = sdb.synchSerialNo;
    }
    QloopEnd;
    /*
     * Point the cache queue header to the first unused server,
     * so the next call will find an unused server if there is one.
     */
    Qremove (&setPtr->cacheQ);
    QinsertLast (lp, &setPtr->cacheQ);
}

/*
 *++
 *  FlushRespQs ()
 *	Flushes both response queues.
 *	Called at end of synchronization to clean up for the next one.
 *--
 */
static void FlushRespQs (void)
{
    ResponseElement *rp;

    /*
     * Flush the local response queue
     */
    QloopBegin (&sdb.localSet.respQ, &rp)
    {
	DisconnectLink ( rp->serverPtr->portPtr,
			(int *)&(rp->serverPtr->linkId));
	RemoveResponseElement (rp);
    }
    QloopEnd;
    sdb.localSet.numResp = 0;

    /*
     * Flush the global response queue
     */
    QloopBegin (&sdb.globalSet.respQ, &rp)
    {
	DisconnectLink ( rp->serverPtr->portPtr,
			(int *)(&rp->serverPtr->linkId));
	RemoveResponseElement (rp);
    }
    QloopEnd;
    sdb.globalSet.numResp = 0;

   /*
    * Flush the Time Provider response queue
    */
    QloopBegin (&sdb.TPset.respQ, &rp)
    {
        Qremove  (rp);
        DTSSfree ((VOID *) rp);
    }
    QloopEnd;
    sdb.TPset.numResp = 0;
}

/*
 *++
 *  RemoveResponseElement ()
 *	Removes a response element from a response queue.  Releases the time
 *	request thread for this element and deallocates the element.
 *  Side Effects:
 *	removes a reponse element.
 *--
 */
static void RemoveResponseElement (
    ResponseElement *rp
)
{

    ReleaseTimeReqThread (rp);
    Qremove  (rp);
    DTSSfree ((VOID *) rp);
} /* end of routine RemoveResponseElement */

/*
 *++
 *  RemoveServer ()
 *	Removes specified server from cache (either local or global)
 *  Inputs:
 *      pointer to server set
 *	pointer to cache element
 *  Implicit Inputs:
 *	** Does not decrement sdb.localSet.numServers or sdb.numGlobals **
 *	shmPtr - ptr to shared memory
 *  Side Effects:
 *	decrements the number of servers in the indicated set
 *--
 */
void RemoveServer (setPtr, sp)
ServerSet *setPtr;
CacheElement *sp;
{
    /*
     * Close any connectionful links that may still be open for this server
     */
    DisconnectLink (sp->portPtr, (int *)&(sp->linkId));

    /*
     * If this is an RPC server, release its binding
     */
    if (sp->portPtr->socketType == K_SOCKET_DTS_RPC)
	ReleaseServerBinding (sp->bindPtr);

    /*
     * Remove server from cache (local or global)
     */
    Qremove (sp);

    /*
     * Free cache element.
     *
     * Note that the CacheElement and the name string are freed
     * together here and malloc'd together in AddServer. If
     * you change one, change them both!
     */
    DTSSfree ((VOID *) sp);

    /*
     * Decrement the number of servers in this set
     */
    setPtr->numServers--;
    setPtr->numServerSet--;
}



/*
 *++
 *  AddServer ()
 *	Adds a new server to the local or global cache
 *	NOTE: There is a VMS version AND a non VMS version, there need
 *            not be, but separating the two makes for cleaner, more
 *            understandable code.
 *  Inputs:
 *      ServerSet       *setPtr;        pointer to local or global set in sdb
 *	SysIdType        nodeId;	unique id of new server
 *	NodeAddr         nodeAddr;	Ethernet address of new server
 *      char            *nodeNamePtr;   Node name (gets copied so caller can
 *                                      release), ignored if (char *) 0
 *	int		nameLen;	length of the name passed
 *	int		 courierRole;   Its courier role (yes, no, backup)
 *	void		*portPtr;       The port we reach it through
 *  Value Returned:
 *	pointer to new CacheElement, or zero if failed
 *  Side Effects:
 *	the cache queue may grow
 *--
 */
CacheElement * AddServer (
    ServerSet       *setPtr,
    SysIdType       *nodeId,
    NodeAddr        *nodeAddr,
    char            *nodeNamePtr,
    VOID            *nodeFnamePtr,
    size_t		nameLen,
    int		courierRole,
    PortDsc		*portPtr
)
{
    CacheElement 	*sp;
    unsigned		allocSize;

    allocSize = sizeof (CacheElement) + nameLen + 1;

    sp = (CacheElement *) DTSSmalloc (allocSize);
    (void) memset ((VOID *)sp, 0, allocSize);
    sp->nodeId        = *nodeId;
    sp->nodeAddr      = *nodeAddr;
    sp->courierRole   = courierRole;
    sp->portPtr       = portPtr;
    sp->linkId        = -1;

    if (nameLen != 0)
    {
	if ( nodeNamePtr != NULL )
	{
	    sp->nodeNamePtr = ((char *) sp) + sizeof (CacheElement);
	    (void) memcpy ((VOID *)sp->nodeNamePtr,
			    (VOID *)nodeNamePtr, nameLen);
	}

    }

    /*
     * Must insert at the beginning of the queue here, so
     * SelectServers will find it.
     */
    QinsertFirst (&setPtr->cacheQ, sp);
    setPtr->numServers++;
    setPtr->numServerSet++;
    return (sp);
}


/*
 *++
 *  DecrAndPruneServers ()
 *      Decrements retriesLeft for each server in the request queue.
 *	Removes unresponsive servers from the cache.
 *      Called when query timer expires.
 *  Inputs:
 *	serverSet - address of the server set queue which contains all servers.
 *      serverType- K_SERVER_GLOBAL or K_SERVER_LOCAL, nonresponsive global
 *                  servers are logged as not responding.
 *  Implicit Outputs:
 *	The server not responding event could be logged.
 *  Value Returned:
 *	TRUE if one or more servers needs a request, else FALSE
 *  Side Effects:
 *	Removes dead servers from the cache and logs an event for each.
 *--
 */
static int DecrAndPruneServers (
    ServerSet *setPtr,
    int       serverType
)
{
    ResponseElement *rp;
    int retval;
    ServerName server;

    retval = FALSE;

    /*
     * Decrement the retry count on each server that hasn't responded.
     * If a server's count has gone to zero, remove it from the cache.
     */
    QloopBegin (&setPtr->respQ, &rp)
	if ( ! rp->responseRecvd )
	{
	    if (--rp->retriesLeft > 0)
	    {
		retval = TRUE;
	    }
	    else
	    {
		if ( serverType == K_SERVER_GLOBAL &&
		     !rp->differingEpoch)
		{
		    if ( rp->serverPtr->nodeFnamePtr != NULL )
		    {
		    }
		    else if ( rp->serverPtr->nodeNamePtr != NULL ) {
			server.selector = SNtext;
			strncpy(server.serverName.serverTextName,
				rp->serverPtr->nodeNamePtr,
				K_MAX_SERVER_TEXT_NAME - 1);
		        server.serverName.serverTextName[K_MAX_SERVER_TEXT_NAME] = '\0';
		    }
		    else {
			server.selector = SN802;
			server.serverName.serverNodeName = rp->serverPtr->nodeAddr;
		    }
		    (void)LogEventServerNotResp( &server, 0, (char *) 0 );
		}
		/*
		 * Remove servers which did not register a response
		 * for reasons other than a differing epoch.
		 */
		if (!rp->differingEpoch)
	 	{
		    RemoveServer (setPtr, rp->serverPtr);
		}
		else
		{
		/*
		 * Decrement the count of servers which can take
		 * part in this particular synchronization.
		 */
		    setPtr->numServers--;
		}
		RemoveResponseElement (rp);
		setPtr->numResp--;
	    }
	}
    QloopEnd;

    return (retval);
}

/*
 *++
 *  FlushServerCache ()
 *	Flushes cache of known servers.
 *	See comment in body of code for more details.
 *  Implicit Inputs:
 *	shmPtr - ptr to shared memory
 *  Value Returned:
 *	StatusBlk
 *  Side Effects:
 *	flushes the caches
 *      sdb.NScontacted = FALSE;
 *--
 */
void FlushServerCache (void)
{
    CacheElement *sp;
    /*
     * If this is a clerk, flush the local set cache.
     * If this is a server, don't flush the autoconfigured (802.3)
     * nodes in the cache, since that would cause a solicit and if
     * servers were to solicit from each other then stale data could
     * persist forever. Servers are allowed to solicit when they first
     * come up to speed the process of joining the local set.
     *
     * Flush the global set cache in any case.
     */
    QloopBegin (&sdb.localSet.cacheQ, &sp)
    {
	if ((IsClerk ()) || (sp->portPtr->socketType != K_SOCKET_DTS_8023))
	{
            RemoveServer ((ServerSet *)&sdb.localSet, sp);
	}
    }
    QloopEnd;

    /*
     * If we're using non-broadcast transports for server in the local set we
     * want to enable a solicit to refill the nodes we just flushed from the
     * local set.
     */
    sdb.localSolDone = FALSE;

    /*
     * Flush the entire global set in all cases.
     */
    QloopBegin (&sdb.globalSet.cacheQ, &sp)
    {
	RemoveServer ((ServerSet *)&sdb.globalSet, sp);
    }
    QloopEnd;

    sdb.NScontacted = FALSE;

#if CONFIG_SECURITY

    /*
     * If the local host is running as a server, check that it still
     * is a member of the Time Servers security group.  If that is
     * not the case, Unexport bindings from its name server database
     * so that no new clients will send time requests to it.  The
     * clients that have this server in their caches will continue
     * to contact it until their next cache refresh.
     */

    if (shmPtr->mgtParams.variant == VTserver)
    {
	if (! serverInSecurityGroup( shmPtr->mgtParams.servPrincName ) )
	{
	    int hostServer = 1;
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		       "Host Server %s is not a member of Time Servers group\n",
			shmPtr->mgtParams.servPrincName));
	    /*
	     *  Log event ->  <Local Server Not In Group>
	     *  and unexport bindings.
	     */
	    LogEventServerNotInGroup((char *)shmPtr->mgtParams.servPrincName,
				     (char *)shmPtr->mgtParams.groupName,
				     hostServer);
	    UnexportTimeService();
	}
    }
#endif /* CONFIG_SECURITY */

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
             "Flushed cache of known servers\n"));
}



/*
 *++
 *  ScheduleNextSynch()-
 *	Schedule the time of the next synch.  If we are a clerk or a server
 *	without a TP, use the algorithm in Section 5.3.  If we are a server
 *	with a TP use nextPoll, specified by the TP.
 *	Do not schedule a future synchronization if we are updating.
 *  Inputs:
 *	[IcompInPtr] - pointer to the computed inaccuracy - this should
 *		       be specified if we're a clerk or a server without a
 *		       TP and we just did a synch - if we are a clerk or
 *		       a server w/o a TP and we just finished updating the
 *		       time or we couldn't complete a sync there is no Icomp
 *		       so if Icomp isn't specified the current inaccuracy is
 *		       used.
 *  Implicit Inputs:
 *	sdb.mgtParams - the local copy of the management params
 *	sdb.TPvalid   - TRUE if we are scheduling for a TP synch,
 *			FALSE otherwise.
 *	sdb.TPnextPoll - the value of that the TP returned (in seconds)
 *			for the next sync if this is called for a TP.
 *  Implicit Outputs:
 *	the next synch timer, K_TIMER_NEXT_SYNCH,  is set for the time of
 *	the next synch
 *--
 */
void ScheduleNextSynch (
    ExtInacc *IcompInPtr
)
{
    ExtInacc		D, *IcompPtr, Icomp;
    StatusBlk		statusBlk;
    long                maxDriftReciprocal;
    UTCValue		nextSynch;
    UTCValue 		curTime;
    UTCValue            randomInterval;

    /*
     * If the service is not in the middle of an update operation, then
     * we can schedule a synchronization.
     */
    if ( IsState( K_STATE_UPDATING ) )
   	return;

    if (utc_comptime(&curTime, (UTCValue *)NULL, (long *)NULL,
		     (utc_t *)NULL))
	BugCheck();

    /*
     * Determine the time for the next synchronization.  For clerks, and
     * server's which synchronized without a TP, use the time at which
     * the inaccuracy will reach maxInacc, but only if it is greater than
     * the base time + synchHold.
     *
     * If (maxInacc < (Icomp + syncHold * maxDrift))
     *     set the random timer between 3/4 and 5/4 of syncHold
     *
     * else
     *	   calculate the time when the inaccuracy will reach maxInacc which is
     *	       D = (maxInacc - Icomp) / maxDrift
     *	   set the random timer between 1/2 and 1 of the calculated time
     */
    if ( ! sdb.TPvalid && ! sdb.checkIntTime )
    {
	/*
	 * If Icomp wasn't specified, use the current inaccuracy.  Icomp
	 * won't be specified if we didn't complete the synch or if we are
	 * scheduling the next synch because the update time adjustment is done.
	 * If we can't figure out the inaccuracy assume it is infinite.
	 */
	if (IcompInPtr == 0)
	{
	    if (utc_comptime((UTCValue *)NULL, &Icomp, (long *)NULL,
			     (utc_t *)NULL))
	    {
		UTCassign (&Icomp,K_INFINITE_INACC_LO,K_INFINITE_INACC_HI);
	    }
	    IcompPtr = &Icomp;
	}
	else
	{
	    /*
	     * We just synched so use the Icomp we calculated.
	     */
	    IcompPtr = IcompInPtr;
	}

	/*
         * compute a maxdrift reciprocal, clockParams.clockDrift is
         * stored as parts per 10**9(billion), dividing 10**9 by the drift
         * generates the ratio
         */
	maxDriftReciprocal = K_ONE_BILLION/shmPtr->clockParams.clockDrift;

	if (IsInfiniteInacc (IcompPtr) ||
	    IsUTClt (&sdb.mgtParams.maxInacc, IcompPtr))
	{
	    statusBlk = InitiateRandomTimer (K_TIMER_NEXT_SYNCH,
					     &sdb.mgtParams.syncHold,
					     RT34To54);
	}
	else
	{
	    /*
	     * We know maxInacc - Icomp is positive (we just tested
	     * it), so we don't have to worry about the unsigned
	     * multiplication here.
	     */
	    UTCsub (&sdb.mgtParams.maxInacc, IcompPtr, &D);
	    UTCiMul (&D, maxDriftReciprocal, &D);
	    if (IsUTClt (&D, &sdb.mgtParams.syncHold))
	    {
		statusBlk = InitiateRandomTimer (K_TIMER_NEXT_SYNCH,
						 &sdb.mgtParams.syncHold,
						 RT34To54);
	    }
	    else
	    {
		statusBlk = InitiateRandomTimer (K_TIMER_NEXT_SYNCH,
						 &D,
						 RThalfTo1);
	    }
	}
	if (IsError (statusBlk))
	    BugCheckOSstatus (statusBlk.sysStatus);

    }
    else if (sdb.TPvalid)
    {
	/* For servers that synched with a TP, schedule the next
	 * synch for the time specified by the TP (nextPoll).
	 *
	 * Convert the TP input from seconds to a UTC relative
	 * time. This time was checked in the state machine and
	 * is guaranteed to fall within a valid range.  If an
	 * overflow occurs, then bugcheck.
	 */
	if(CvtSecondsToUTCrel( sdb.TPnextPoll, &nextSynch )) {
	    BugCheck();
	}
	/*
	 * Schedule the next synch to be either a checkint or a
	 * true synch, depending upon which must occur first.
	 */
        UTCadd(&curTime, &nextSynch, &nextSynchTime);
	    if (IsUTCgt(&nextSynchTime, &nextCheckInt)) {
		UTCsub(&nextCheckInt, &curTime, &nextSynch);
		if (IsUTCneg(&nextSynch)) {
		    UTCzero (&nextSynch);
		}
	    }

	statusBlk = InitiateTimer( K_TIMER_NEXT_SYNCH, &nextSynch);
	if (IsError(statusBlk))
	    BugCheckOSstatus (statusBlk.sysStatus);
    }
    else if (sdb.checkIntTime) {
	RandomizeInterval(&randomInterval,
			  &shmPtr->mgtParams.checkInt,
			  RT34To54);
        UTCadd(&curTime, &randomInterval, &nextCheckInt);
	if (IsUTCgt(&nextSynchTime, &nextCheckInt)) {
	    UTCsub(&nextCheckInt, &curTime, &nextSynch);

	}
	else {
	    UTCsub(&nextSynchTime, &curTime, &nextSynch);
	}
	if (IsUTCneg(&nextSynch)) {
	    UTCzero (&nextSynch);
	}
	statusBlk = InitiateTimer( K_TIMER_NEXT_SYNCH, &nextSynch);
	if (IsError(statusBlk))
	    BugCheckOSstatus (statusBlk.sysStatus);
    }
}

/*
 *++
 *  DoSynch()-
 *	Any clock adjustment which was in progress has already been
 *	stopped.  We know we have the necessary numbers of server to
 *	synchronize.  Determine the sychronization instant.  Translate
 *	each server's time to the synch instant.  Compute the best
 *	correct time from the time values obtained from the servers.
 *	Do faulty clock detection and log an error if the local clock
 *	is faulty.
 *  NOTE:
 *	This routine assumes all dead servers have already been cleared
 *	from the local and the global response queues.
 *  Inputs:
 * 	IcompPtr - pointer to the returned computed inaccuracy
 *  Implicit Inputs:
 *	the localRespQ and the globalRespQ from the synch sdb, sdb
 *	the local copy of the management params from sdb - need error tolerance
 *	TSstate     -   SYNCH_ADJUST = synchronize by adjusting,
 *			SYNCH_SET    = synchronize by setting.
 *	sdb.TPvalid -   TRUE if this is a TP synch, false otherwise.
 *  Outputs:
 * 	IcompPtr - pointer to the computed inaccuracy so that it can be used
 *	  	   to schedule the next synch
 *  Implicit Outputs:
 *	the clock may have been adjusted or reset if the error tolerance was
 *	    exceeded
 *  Side Effects:
 *	may log an event if the local clock is faulty
 *--
 */
static void DoSynch (
    ExtInacc	*IcompPtr
)
{
    StatusBlk		statusBlk;
    EndPointElement	*endPointPtr, *endPointArr, *minPointPtr, *maxPointPtr;
    BaseTimes		*baseTimesPtr;
    unsigned long	status;
    UTCValue		Tsync, Tcomp;
    ExtInacc		Isync;
    int			numElements, M, f, TsyncIntersectsTcomp;
    int			mostMinIntersects, mostMaxIntersects;
    int			notAllSrvTimesIntersectTcomp;
    int                 numServersUsedinSynch;
    utc_t               curTime;

    /*
     * initialize the inaccuracy & serverintersectTcomp for errors
     */
    UTCassign (IcompPtr,K_INFINITE_INACC_LO,K_INFINITE_INACC_HI);
    notAllSrvTimesIntersectTcomp = FALSE;

    /*
     * The following line is for automatic parsing of the trace file.
     * It should be at the start of the synchronization record and should
     * not be changed.
     */
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	     "******** start of synch record ********\n"));
    /*
     * Determine the synchronization instant.
     */
    baseTimesPtr = &shmPtr->baseTimes;


    if (utc_gettime (&curTime))
	status = DTSS_ERROR;
    else
    {
	(void) utc_comptime(&Tsync, &Isync, (long *)NULL, &curTime);
	status = DTSS_SUCCESS;
    }

    if (status != DTSS_SUCCESS)
    {
	/**** user manually set the time back - log event ****/

	dce_svc_printf( DTS_S_USER_RESET_MSG) ;
	return;
    }
#ifdef	DCE_DEBUG
    if (DCE_SVC_DEBUG_ATLEAST(dts_svc_handle, dts_s_arith, 1))
	 PrintInaccuracyTimes(&Tsync, &Isync, baseTimesPtr);
#endif
    /*
     * Allocate an array of EndPointElements large enough to hold all
     * the end points. M = the number of servers involved in the
     * synchronization.  If we're a server or a TP we're going to
     * include ourself in the calculation.
     */
    M = sdb.localSet.numResp  +
	sdb.globalSet.numResp +
	(IsServer() ? sdb.TPset.numResp + 1 : 0 );
    numElements = M * 2;
    endPointArr = (EndPointElement *)
	DTSSmalloc ((unsigned)(sizeof (EndPointElement) * numElements));

    /*
     * Loop through the lists of servers that have responded.  Translate
     * each server's time to the synch instant.  If we can't estimate
     * a server's time, abort the current synchronization.
     */
    endPointPtr = endPointArr;
    status = CollectServerTimes ((ServerSet *)&sdb.localSet,
				 &Tsync,
				 &Isync,
				 &endPointPtr);
    if (status != DTSS_SUCCESS)
	goto DoSynchCleanUpAndReturn;

    status = CollectServerTimes ((ServerSet *)&sdb.globalSet,
				 &Tsync,
				 &Isync,
				 &endPointPtr);
    if (status != DTSS_SUCCESS)
	goto DoSynchCleanUpAndReturn;

    /*
     * If this is a server then use any TP information available. Also
     * include its time into the endpoint list.
     */
    if ( IsServer() )
    {
        ResponseElement *rp;
        UTCValue	utcTime,utcInacc;

	QloopBegin(&sdb.TPset.respQ, &rp) {
	    utc_comptime(&utcTime,
                         &utcInacc,
                         (long *)NULL,
			 &rp->serverTime);
	    if (!IsInfiniteInacc(&utcInacc))
	    {
	        UTCiAdd(&utcInacc,                            /* inacc   */
                    shmPtr->clockParams.clockResolution/100,  /* resol   */
                    &utcInacc);                               /* result  */
            }
	    utc_mkcomptime(&rp->serverTime,
			   &utcTime,
			   &utcInacc,
			   0L);
	} QloopEnd;

	status = CollectServerTimes ((ServerSet *)&sdb.TPset,
				     &Tsync,
				     &Isync,
				     &endPointPtr);
	if (status != DTSS_SUCCESS)
	    goto DoSynchCleanUpAndReturn;

	/*
	 * Add this server's time to the end point list.
	 */
        minPointPtr = endPointPtr++;
        maxPointPtr = endPointPtr++;

	CalcIntervalEndPoints (&Tsync, &Isync, minPointPtr, maxPointPtr);
     }

    /*
     * Sort the list of end points.
     */

    qsort (
	(void *) endPointArr,		/* (IN) base of array */
	(size_t) numElements,		/* (IN) number of elements in array */
	(size_t) sizeof(EndPointElement),/* (IN) size of each element */
	CompareIntervalEndPoints);	/* (IN) comparision function */

    /*
     * Find the min. and the max. of the correct time interval.  If we're a
     * clerk protect ourself from up to minServer/2 faulty servers.
     */
    f = IsServer() ? 0 : sdb.mgtParams.minServers / 2;

    mostMinIntersects = ComputedTimeMinimum (
	M,				/* (IN) number of time values */
	f,				/* (IN) max. # faulty srvs tolerated */
	endPointArr,			/* (IN) end points already sorted */
	&minPointPtr);			/* (OUT) lower end of the interval */

    mostMaxIntersects = ComputedTimeMaximum (
	M,				/* (IN) number of time values */
	f,				/* (IN) max. # faulty srvs tolerated */
	endPointArr,			/* (IN) end points already sorted */
	&maxPointPtr);			/* (OUT) lower end of the interval */

    if ( mostMinIntersects != M || mostMaxIntersects != M )
        notAllSrvTimesIntersectTcomp = TRUE;
    else
        notAllSrvTimesIntersectTcomp = FALSE;

    /*
     * Calculate the computed time interval.
     */
    CalcMidpointAndInacc (minPointPtr, maxPointPtr, &Tcomp, IcompPtr);

    /* Check for faulty servers.  If the computed time doesn't intersect
     * with our time, then we are faulty so log LOCAL FAULT DETECTED.
     * If we are faulty and the error tolerance is exceeded then we
     * force a clock set rather than a clock adjustment.
     *
     * Additionally, if we are a server without a TP we have to check
     * for other faulty servers.  Log FAULTY SEVERS DETECTED if not
     * all the servers' times intersect the computed time but our time
     * does intersect the computed time so we know it's not us that is faulty.
     *
     * In the case of a TP performing a checkInt operation then we
     * Log FAULTY SERVERS DETECTED assuming that our clock is not
     * faulty.  Note that in the case of a checkInt no clock adjustment
     * is made.
     */
    {
	utc_t compTime, oldTime;
	utc_t junkTime;		/* never used */

	(void) utc_mkcomptime(&compTime, &Tcomp, IcompPtr, 0L);
	(void) utc_getnextzone((utc_t *)&junkTime, &compTime);

	(void) utc_mkcomptime(&oldTime, &Tsync, &Isync, 0L);
	(void) utc_getnextzone((utc_t *)&junkTime, &oldTime);

	TsyncIntersectsTcomp = IsIntervalIntersection (
			&Tcomp,				/* (IN) time 1  */
			IcompPtr,			/* (IN) inacc 1 */
			&Tsync,				/* (IN) time 2  */
			&Isync);			/* (IN) inacc 2 */

	if (!TsyncIntersectsTcomp && !sdb.checkIntTime)
	{
	    (void)LogEventLocalFault(&compTime,
                                     &oldTime,
			             (unsigned long)( !(sdb.TPvalid &&
                                                    sdb.TPnoClockSet) &&
				        (IsErrorToleranceExceeded (&Tcomp,
                                                  IcompPtr, &Tsync, &Isync))
			             ));
	}
	else
	{
	    if (IsServer() && notAllSrvTimesIntersectTcomp)
	    {
		/*
		 * So, not all the server times intersected
		 * we need to report who may be wrong:
		 * the server out there somewhere, or our local clock.
		 */
		if (!sdb.TPvalid || sdb.checkIntTime )
	    	    (void)LogEventFaultyServer( &compTime );
		else
		if (sdb.TPvalid)
			(void)LogEventLocalFault(&compTime, &oldTime,
                                     (unsigned long)( !sdb.TPnoClockSet &&
                                        (IsErrorToleranceExceeded (&Tcomp,
                                                  IcompPtr, &Tsync, &Isync))
                                     ));
	    }
	}

    }
    /*
     * Update the server caches for SHOW SERVERS
     */
    UpdateServerCacheStatus ((ServerSet *)&sdb.localSet,  &Tsync, &Isync);
    UpdateServerCacheStatus ((ServerSet *)&sdb.globalSet, &Tsync, &Isync);

    /*
     * If this is a checkInt then skip any clock adjustments.
     */
     if ( sdb.checkIntTime )
	goto DoSynchCleanUpAndReturn;

    /*
     * Initiate either a clock adjustment or a clock set.  The clock is
     * normally adjusted so that it remains monotonic but in the case
     * where the local clock is faulty it is sometimes reset.
     *
     * First, calc number of servers we used for the two log event calls.
     */
    numServersUsedinSynch = (sdb.localSet.numRecvd  +
			     sdb.globalSet.numRecvd +
			     (IsServer() ? 1 : 0)    );

    if (!(sdb.TPvalid && sdb.TPnoClockSet))
    {
	utc_t compTime, oldTime;
	utc_t junkTime ;

	(void) utc_mkcomptime(&compTime, &Tcomp, IcompPtr, 0L);
	(void) utc_getnextzone((utc_t *)&junkTime, &compTime);

	(void) utc_mkcomptime(&oldTime, &Tsync, &Isync, 0L);
	(void) utc_getnextzone((utc_t *)&junkTime, &oldTime);

	if (IsErrorToleranceExceeded (&Tcomp, IcompPtr, &Tsync, &Isync) ||
	    IsState( K_STATE_SYNCH_SET ))
	{
	    statusBlk = SetClock(&Tcomp, IcompPtr, &Tsync, (int *)NULL);
	    if (IsError(statusBlk))
	    {
		/*
		 * We should not bugcheck here.  Instead, we should report
		 * the failure and set the local inaccuracy to infinite.
		 */
		BugCheckOSstatus(statusBlk.sysStatus);
	    }

	    (void)LogEventSynchCompleted(&compTime, &oldTime,
                                         numServersUsedinSynch);
	    (void)LogEventClockSet(&compTime, &oldTime);

	}
	else /* synch by adjust */
	{
	    statusBlk = AdjustClock (&Tsync,	    /* (IN) Tsync */
				     &Isync,	    /* (IN) Isync */
				     &Tcomp,	    /* (IN) Tcomp */
				     IcompPtr);    /* (IN) Icomp */

	    if (!IsError(statusBlk))
	    {
		(void)LogEventSynchCompleted(&compTime, &oldTime,
					     numServersUsedinSynch);
	    }
	    else
	    {
		/*
		 * The user probably did a reset of the clock using SET TIME.
		 */
		/*** log an error? ***/
	    }
	}
    }
    else {

	/* NOTE: this does nothing to the clock as arg3 is subtracted from
	 * arg1 to get the actual adjustment.
	 */
	statusBlk = SetClock(&Tcomp, IcompPtr, &Tcomp, (int *)NULL);
	if (IsError(statusBlk))
	{
	    BugCheckOSstatus(statusBlk.sysStatus);
	}
    }

/*
 * Exit here
 * Complete synch record in trace
 * Free the end point element array
 */
DoSynchCleanUpAndReturn:

#ifdef	DCE_DEBUG
    if (DCE_SVC_DEBUG_ATLEAST(dts_svc_handle, dts_s_arith, 1))
	 PrintSynchTimes (&Tsync, &Isync, &Tcomp, IcompPtr);
#endif
    /*
     * The following line is for automatic parsing of the trace file.
     * It should be at the end of the synchronization record and should
     * not be changed.
     */
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	      "******** end of synch record ********\n"));
    /*
     * If we are producing output for the inaccuracy graph, write out a
     * record.
     */
    if (shmPtr->mgtParams.traceFlags & M_TRACE_GRAPH)
    {
	WriteInaccTraceRecord (&Tsync,
			       &Isync,
			       &Tcomp,
			       IcompPtr,
			       (IsServer () ?
				notAllSrvTimesIntersectTcomp :
				0) );
    }
    DTSSfree ((VOID *) endPointArr);

}


/*
 *++
 *  CollectServerTimes ()
 *	Loop over response elements, building the endpoint array that
 *      will later be sorted and scanned to determine the intersection
 *      that will be the calculated time.
 *  Inputs:
 * 	IcompPtr - pointer to the returned computed inaccuracy
 *  Outputs:
 * 	IcompPtr - pointer to the computed inaccuracy so that it can be used
 *	  	   to schedule the next synch
 *--
 */
static unsigned long CollectServerTimes (
    ServerSet *setPtr,
    UTCValue *TsyncPtr,
    UTCValue *IsyncPtr,
    EndPointElement	**endPointPP
)
{
    unsigned long   status;
    ResponseElement *rp;
    EndPointElement *minPointPtr, *maxPointPtr;
    UTCValue utcTime; 
    ExtInacc utcInacc;
/*  
    Inaccuracy inacc;
*/

    status = DTSS_SUCCESS;

    QloopBegin (&setPtr->respQ, &rp)
    {
	(void) utc_comptime(&utcTime, &utcInacc, (long *)NULL, &rp->serverTime);

/*  Don't have to reassigne utcInacc, because in EstimateServerTime() *
 *  function, inacc is reassigned to a ExtInacc type variable.  Also  *
 *  it is difficult to handle the following conversion, since the     *
 *  the struct of inacc has two members on 32-bit machine, and        *
 *  utcInacc is a unsigned long variable in the 64-bits machine.      *
 *  The following files are changed too:			      *
 *  		clock_adjust.c 					      *
 *		dtss_service.h					      */

/*        inacc.hi = utcInacc.hi;	*/
/*        inacc.lo = utcInacc.lo;	*/
/*	Above comments can be deleted after code review.	*/
/*		York Lo						*/

	status = EstimateServerTime (
		&rp->sendTime,		/* time request was sent */
		&utcTime,		/* time in response */

/*   change the following line to &utcInacc			*/
/*   these comments can be deleted after code review.		*/
/*		York Lo						*/
/*		&inacc,						*/

		&utcInacc,		/* inacc in response */
		&rp->processingDelay,	/* w */
		&rp->recvTime,		/* time resp received back*/
		TsyncPtr,		/* my time at sync instant */
		IsyncPtr,		/* my inacc at sync instant */
		&rp->Tserv,		/* OUT: srv time at sync instant */
		&rp->Iserv);		/* OUT: srv inacc at sync instant */

	if (status != DTSS_SUCCESS)
	    return (status);

	minPointPtr = (*endPointPP)++;
	maxPointPtr = (*endPointPP)++;

	CalcIntervalEndPoints (&rp->Tserv,
			       &rp->Iserv,
			       minPointPtr,
			       maxPointPtr);
    }
    QloopEnd;
    return (status);
}



/*
 *++
 *  UpdateServerCacheStatus()
 *	Update the server cache elements with time of use data.
 *  Inputs:
 * 		TsyncPtr - Time of the synchronization
 *		IsyncPtr - Inaccuracy at the synchronization
 *  Implicit Inputs:
 *		sdb - synchronization database
 *  Implicit Outputs:
 *		The server cache elements are updated.
 *--
 */
static void UpdateServerCacheStatus(
    ServerSet *setPtr,
    UTCValue  *TsyncPtr,
    ExtInacc  *IsyncPtr
)
{
    ResponseElement *rp;
    long theTDF ;

    /* begin */

    (void)utc_localzone((char *)0, 100, &theTDF, (int *)0, (utc_t *)0);
    
    QloopBegin(&setPtr->respQ, &rp)
    {
	if (rp->differingEpoch)
	{
	    rp->serverPtr->synchSerialNo = sdb.synchSerialNo - 1 ;
	}
	else
	{
	    rp->serverPtr->synchSerialNo = sdb.synchSerialNo;
#ifdef v1_code 
	    rp->serverPtr->Tsync = *TsyncPtr;
	    rp->serverPtr->Isync = *IsyncPtr;
	    rp->serverPtr->Tserv = rp->Tserv;
	    rp->serverPtr->Iserv = rp->Iserv;
#else
	    utc_mkcomptime(&rp->serverPtr->sync, TsyncPtr, IsyncPtr, theTDF) ;
	    utc_mkcomptime(&rp->serverPtr->serv, &rp->Tserv, &rp->Iserv, theTDF);
#endif
	}
    }
    QloopEnd;
}

/* End of routine UpdateServerCacheStatus */



/*
 *++
 *  UpdateCourierStatus()
 *	Update my courier status, based on the management courier parameter and,
 *	if that is CRbackupCourier, the local set cache.
 *  Implicit Inputs:
 *		sdb - synchronization database
 *  Implicit Outputs:
 *		Updates shmPtr->mgtParams.myCourierRole
 *  Side Effects:
 *		Updates shmPtr->mgtParams.myCourierRole
 *--
 */
static void UpdateCourierStatus (void)
{
    CacheElement *sp, *minsp;
    int myCourierRole;

    myCourierRole = K_BACKUP_ROLE_NONCOURIER;  /* default behavior */

    /*
     * If the synchronization has occured with a time provider,
     * then this is not a courier.
     */
    if (sdb.TPvalid) {
	shmPtr->mgtParams.myCourierRole = K_BACKUP_ROLE_NONCOURIER;
	return;
    }

    switch (sdb.mgtParams.courierRole)
    {
    case K_ROLE_COURIER:
	myCourierRole = K_BACKUP_ROLE_COURIER;
	break;
    case K_ROLE_NONCOURIER:
        myCourierRole = K_BACKUP_ROLE_NONCOURIER;
        break;

    case K_ROLE_BACKUPCOURIER:
	/* see comment in if statement below Qloopend - search for
	   "once upon " molson 28-nov-89 */

        myCourierRole = K_BACKUP_ROLE_NONCOURIER;

	/*
	 * This node is a backup courier. Search the local set cache for
	 * another server that claims to be a courier. If found then this
	 * node is not a courier. If no other node claims to be a courier,
	 * then if this node has the lowest node id it is the elected courier.
	 */
	minsp = (CacheElement *)0;
	QloopBegin(&sdb.localSet.cacheQ, &sp)
	{
	    switch (sp->courierRole)
	    {
		/* ignore CRnonCourier servers */

	    case K_ROLE_COURIER:
		shmPtr->mgtParams.myCourierRole = K_BACKUP_ROLE_NONCOURIER;
		return;

	    case K_ROLE_BACKUPCOURIER:

		/*
		 * If this server's UUID is the smallest yet, save a
		 * pointer to it.
		 */
		if ((minsp == 0) ||
		    (compareUUIDs (&sp->serverUUID,
				  &minsp->serverUUID) < 0) )
		{
		    minsp = sp;
		}
		break;

	    }
	}
	QloopEnd;

	/*
	 * No other node is an appointed courier (CRcourier).
	 * See if this node is elected from among the backup couriers
	 * (has the lowest nodeId for the DECNET case or the smallest UUID
	 * for the RPC-Security case).
	 * minsp is null if all the roles were non-courier
	 * or there were no servers in the cache, in which case
	 * it's time to become the courier.
	 */

	if (((minsp) &&
	    (compareUUIDs (&rpcBufPtr->TServicePrincUUID,
			   &minsp->serverUUID) < 0))   ||
	    (minsp == NULL) )
	{
	    myCourierRole = K_BACKUP_ROLE_COURIER;
	}

	/* once upon a time, a line here read:
	 * else
	 *    shmPtr->myCourierRole=K_ROLE_COURIER;
	 * but it stubbornly refused to compile on the pmax.
	 * Moving this line to the top of the case statement, making
	 * CRcourier the default, fixed the problem. This smells like
	 * a compiler bug.
	 * molson 28-nov-89
	 */

	break;
        } /* end case */

	shmPtr->mgtParams.myCourierRole = myCourierRole;
}

/* End of routine UpdateCourierStatus */



/*
 *++
 *  compareUUIDs()
 *    This function compares two UUIDS (uuid1 and uuid2) and determines
 *    its lexical order.  The UUIDs are converted to strings and each
 *    string is broken up in tokens of 4 hexadecimal characters.  The
 *    tokens are compared based on their numerical value.
 *    NOTE: We use this function instead of rpc's API uuid_compare()
 *          function, since the latter is not guarenteed to return
 *          the same results on different architectures.
 *  Inputs:
 *	    uuid1 - First UUID to be compared.
 *          uuid2 - Second UUID to be compared.
 *  Value Returned:
 *             -1 if uuid1 < uuid2
 *              0 if uuid1 = uuid2
 *              1 if uuid1 > uuid2
 *--
 */

static int compareUUIDs (
    uuid_t *uuid1,
    uuid_t *uuid2
)
{

#define BASE  (16)   /* Hexadecimal base */

    unsigned32    status;
    unsigned long value1, value2;
    char       uuidStr1[40], uuidStr2[40];
    unsigned_char_p_t strPtr1, strPtr2;
    char       token1[5], token2[5];
    int        i;

    /*
     * Initialize all strings to zeros and assign addresses to
     * character pointers.
     */

    memset( (VOID *)uuidStr1, 0, 40 );
    memset( (VOID *)uuidStr2, 0, 40 );
    memset( (VOID *)token1, 0, 5 );
    memset( (VOID *)token2, 0, 5 );
    strPtr1 = (unsigned_char_p_t)&uuidStr1[0];
    strPtr2 = (unsigned_char_p_t)&uuidStr2[0];

    /*
     * convert uuids to character strings.
     */
    uuid_to_string (uuid1, &strPtr1, &status);
    uuid_to_string (uuid2, &strPtr2, &status);

    /*
     * Scan the converted strings breaking them up in tokens of
     * four hex characters. Each UUID consists of 8 sets of tokens.
     * A typical UUID string looks like this:
     *
     *           e073d860-6830-11c9-a60d-08002b0dea7a
     */

    for (i=0; i<8; i++)
    {
	strncpy( token1, (char *)strPtr1, 4);
	strncpy( token2, (char *)strPtr2, 4);

	/*
	 * convert each token to a numerical value and compare them
	 * to determine which one is smaller.
	 */

	value1 = strtoul (token1, (char **)NULL, BASE);
	value2 = strtoul (token2, (char **)NULL, BASE);

	if (value1 < value2)
	    return (-1);

	if (value1 > value2)
	    return (1);

	/*
	 * The tokens are equal. Check for separator in the strings
	 * and advance pointers accordingly.
	 */
	if (*(strPtr1+4) == '-' && *(strPtr2+4) == '-'){
	    strPtr1 +=5;
	    strPtr2 +=5;
	}
	else{
	    strPtr1 +=4;
	    strPtr2 +=4;
	}
    }

    /*
     * The UUID strings have been scanned and compared.
     * They are equal
     */
    return (0);
}



/*
 *++
 *  IsErrorToleranceExceeded()
 *	Determine whether the error tolerance is exceeded.  The alogorithm
 *      is from Section 3.9 in the architecture.
 *  Inputs:
 * 	TcompPtr, IcompPtr - pointers to the computed time interval
 * 	TsyncPtr, IsyncPtr - pointers to the synchronization time interval
 *	errorTolPtr - pointer to the error tolerance
 *  Implicit Inputs:
 *	the local copy of the management params from sdb - need error tolerance
 *  Value Returned:
 *	0 = error tolerance is not exceeded
 *	1 = error tolerance is execeeded
 *--
 */
static int IsErrorToleranceExceeded (
    UTCValue	*TcompPtr,
    ExtInacc	*IcompPtr,
    UTCValue	*TsyncPtr,
    ExtInacc	*IsyncPtr
)
{
    UTCValue	timeDiff, inaccDiff;

    /*
     * Error tolerance exceeded if:
     *
     * 	   |Tcomp - Tsync| - Icomp - Isync > errorTolerance
     */

    /*
     * If the error tolerance is infinite, then it can't be exceeded.
     */
    if (IsInfiniteInacc (&sdb.mgtParams.errorTolerance))
	return (0);

    /*
     * Otherwise, if either inaccuracy is infinite, then error tolerance
     * isn't exceeded.
     */
    if (IsInfiniteInacc (IcompPtr) || IsInfiniteInacc (IsyncPtr))
	return (0);

    UTCsub (TsyncPtr, TcompPtr, &timeDiff);	/* Tsync - Tcomp */
    UTCabsVal (&timeDiff, &timeDiff);		/* |Tsync - Tcomp| */

    UTCadd (IcompPtr, IsyncPtr, &inaccDiff);	/* Icomp + Isync */

    /*
     * If the inaccuracies are greater than the times, then the error
     * tolerance isn't exceeded.  Since the comparisions are unsigned
     * we have to special case this.
     */
    if (IsUTCgt (&inaccDiff, &timeDiff))
	return (0);

    UTCsub (&timeDiff, &inaccDiff, &timeDiff);

    return (IsUTCgt (&timeDiff, &sdb.mgtParams.errorTolerance));
}



/*
 *++
 *  CvtSecondsToUTCrel()
 *  Convert Seconds into its UTC relative time equivalent.
 *  Inputs:
 *		numSeconds - the number of seconds
 *  Outputs:
 *		intVal - UTC relative time representing 'numSeconds' time.
 *  Value Returned:
 *		TRUE if an arithmetic overflow occured, FALSE otherwise.
 *
 *--
 */
static int CvtSecondsToUTCrel( 
    unsigned long numSeconds,
    UTCValue *intVal
)
{
    uemul(numSeconds, K_100NS_PER_SEC, intVal);
    return (0);              /* This can not overflow! */
}


/*
 * PrintInaccuracyTimes() -
 *	Print the base times and inaccuracies.
 */
static void PrintInaccuracyTimes (
    UTCValue	*Tptr,
    ExtInacc	*Iptr,
    BaseTimes	*baseTimesPtr
)
{
    ExtInacc	*IbasePtr;
    UTCValue	*TbasePtr;
    char	Tbasebuf[32];
    char	Tptrbuf[32];
    char	Ibasebuf[32];
    char	Iptrbuf[32];


    TbasePtr = &baseTimesPtr->Tbase;
    IbasePtr = &baseTimesPtr->Ibase;

    CvtUTCValueToStr (TbasePtr, Tbasebuf, sizeof(Tbasebuf));
    if (IsInfiniteInacc (IbasePtr))
	(void)strcpy(Ibasebuf, "infinite");
    else
	CvtUTCValueToStr (IbasePtr, Ibasebuf, sizeof(Ibasebuf));
    CvtUTCValueToStr(Tptr, Tptrbuf, sizeof(Tptrbuf));
    if (IsInfiniteInacc (Iptr))
	(void)strcpy(Iptrbuf, "infinite");
    else
	CvtUTCValueToStr(Iptr, Iptrbuf, sizeof(Iptrbuf));

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	"***CalcInaccuracy Times***\n"));
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	"Tbase: %s\tIbase %s\n", Tbasebuf, Ibasebuf));
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	"T: %s\tI: %s\n", Tptrbuf, Iptrbuf));
}



/*
 *++
 *  PrintState ()
 *	print out the synchronization state
 *  Implicit Inputs:
 *	synchState
 *--
 */
static void PrintState (
    char *text, 
    char *arg
)
{
    char *stateName;
    char  nameString[50];
    struct tm now;
    long nanonow;

    switch (synchState)
    {
        case STbad:         stateName = "STbad";          break;
        case STignore:      stateName = "STignore";       break;
        case STdoCkInt:     stateName = "STdoCkInt";      break;
        case STgblInit:     stateName = "STgblInit";      break;
        case STgblLinks:    stateName = "STgblLinks";     break;
        case STgblLinksWait:stateName = "STgblLinksWait"; break;
        case STgblSend:     stateName = "STgblSend";      break;
        case STgblTimer:    stateName = "STgblTimer";     break;
        case STgblWait:     stateName = "STgblWait";      break;
        case STnone:        stateName = "STnone";         break;
        case STprvdrSync:   stateName = "STprvdrSync";    break;
        case STprvdrTMO:    stateName = "STprvdrTMO";     break;
        case STprvdrWait:   stateName = "STprvdrWait";    break;
        case STprvdrWTMO:   stateName = "STprvdrWTMO";    break;
        case STrqInit:      stateName = "STrqInit";       break;
        case STrqSend:      stateName = "STrqSend";       break;
        case STrqTimer:     stateName = "STrqTimer";      break;
        case STrqWait:      stateName = "STrqWait";       break;
        case STstartSynch:  stateName = "STstartSynch";   break;
        case STstCkInt:     stateName = "STstCkInt";      break;
        case STstPrvdrSync: stateName = "STstPrvdrSync";  break;
        case STstServSynch: stateName = "STstServSynch";  break;
        case STsolInit:     stateName = "STsolInit";      break;
        case STsolSend:     stateName = "STsolSend";      break;
        case STsolTimer:    stateName = "STsolTimer";     break;
        case STsolWait:     stateName = "STsolWait";      break;
        case STsynch:       stateName = "STsynch";        break;
        case STnoSynch:     stateName = "STnoSynch";      break;
        case STcleanup:     stateName = "STcleanup";      break;
        case STnoPrvdr:     stateName = "STnoPrvdr";      break;
        default:
            stateName = nameString;
            (void) sprintf (stateName,
			    "*** unknown state %d ***",
			    synchState);
            break;
    }
    (void) utc_gmtime (&now,
		       &nanonow,
		       (struct tm *)  NULL,
		       (long *)       NULL,
		       (utc_t *) NULL); /* now */

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
	text, arg));
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
	"to state %s, at %d-%02d-%02d-%02d:%02d:%02d.%03d GMT\n",
	stateName,
	now.tm_year, now.tm_mon + 1, now.tm_mday,
	now.tm_hour, now.tm_min, now.tm_sec,
	nanonow / 1000000)); /*  10**6 = 10**9 / 10**3  */
}



static char *GetStimulus (
    INstimuli stimulus
)
{
    static char buf[50];

    switch (stimulus)
    {
        case INrcvdTimeResponse:
            return "INrcvdTimeResponse";
        case INrcvdSolicitResponse:
            return "INrcvdSolicitResponse";
        case INsynchTimer:
            return "INsynchTimer";
        case INqueryTimeout:
            return "INqueryTimeout";
	case INrcvdTPresponse:
	    return "INrcvdTPresponse";
	case INlinkReady:
	    return "INlinkReady";
	case INstimuliNumber:
	    return "INstimuliNumber" ;
				/* N.B fall-through */
    }
    (void) sprintf (buf, "*** unknown stimulus %d ***", stimulus);
    return buf;
}

/*
 * PrintSynchTimes() -
 *	Print the results of the synchronization.
 */
static void PrintSynchTimes ( 
    UTCValue	*TsyncPtr,
    ExtInacc	*IsyncPtr,
    UTCValue	*TcompPtr,
    ExtInacc	*IcompPtr
)
{
    char	Tsyncbuf[32];
    char	Isyncbuf[32];
    char	Tcompbuf[32];
    char	Icompbuf[32];


    CvtUTCValueToStr (TsyncPtr, Tsyncbuf, sizeof(Tsyncbuf));
    if (IsInfiniteInacc (IsyncPtr))
	strcpy(Isyncbuf, "infinite");
    else
	CvtUTCValueToStr (IsyncPtr, Isyncbuf, sizeof(Isyncbuf));

    CvtUTCValueToStr (TcompPtr, Tcompbuf, sizeof(Tcompbuf));
    if (IsInfiniteInacc (IcompPtr))
	(void)strcpy(Icompbuf, "infinite");
    else
	CvtUTCValueToStr (IcompPtr, Icompbuf, sizeof(Icompbuf));

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	"***Synch Times***\n"));
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	"Tsync: %s\n", Tsyncbuf));
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	"Isync: %s\n", Isyncbuf));
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	"Tcomp: %s\n", Tcompbuf));
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	"Icomp: %s\n", Icompbuf));
}

/*
 * WriteInaccTraceRecord() -
 *	Used to write a record out to the file named, K_INACCFILE_NAME.
 *	The record will contain times and inaccuracies which will enable
 *	us to track the clock's inaccuracy over a period of time.
 * Inputs:
 *	TsyncPtr, IsyncPtr - synchronization point
 *	TcompPtr, IcomPtr  - computed time as of the synch point
 * Implcit Inputs:
 *	sdb.localRespQ - for Tserv and Iserv from each remote server
 * Implicit Outputs:
 *	Each record has these fields:
 *	  synchronization number
 *	  one of the following pairs of fields:
 *	   "0", server address, as a hex string; or name in text
 *	   "1","Tsynch" for my record
 *	   "2","Tcomp" for Tcomp record
 *	   "3","separator" for separator between synchs
 *	  Tcomp
 *	  Tserv - Tcomp
 *	  Iserv as estimated by clerk (incl round-trip and clerk drift)
 *	  0 if fault not detected, 1 if fault detected in this synch
 *	  # of records in this synchronization
 *        Iserv as reported by server
 *        1 if global set server, else 0
 */

static void WriteInaccTraceRecord (
    UTCValue	*TsynchPtr,
    ExtInacc	*IsynchPtr,
    UTCValue	*TcompPtr,
    ExtInacc	*IcompPtr,
    int              notAllSrvTimesIntersectTcomp
)
{

#define GRAPHVERSION 4
#define GRAPHPRINTSTR "%ld\t%d\t%s\t%s\t%s\t%s\t%ld\t%ld\t%s\t%d\n"

    FILE *inaccTraceFP;
    UTCValue diff, utcTemp;
    ResponseElement *rp;
    char dStr[30];			/* difference (Tcomp - Tserv) */
    char tStr[30];			/* Tcomp */
    char iStr[30];			/* Iserv */
    char rStr[30];			/* round trip time */
    int recsInSet;
    char *namePtr;
    char hexName[30];
    char inaccFilename[K_MAX_FILENAME+1];
    BaseTimes *basePtr;

    strcpy(inaccFilename, dcelocal_path);
    strcat(inaccFilename, K_INACCFILE_NAME);

    /*
     * Make sure the file is open. We close it after every synchronization
     * so watchers can see it without stopping the service.
     */
    if ( shmPtr->mgtParams.newTraceFile )
    {
	shmPtr->mgtParams.newTraceFile = FALSE;
	/*
	 * First time, supersede old log
	 */
	inaccTraceFP = fopen (inaccFilename, "w");
	/*
	 * Put out header line in new version of trace file (if there is one)
	 */
        if (inaccTraceFP != NULL)
	{
	    (void) fprintf (inaccTraceFP, "Version %d Variant %s\n",
			    GRAPHVERSION,
			    IsServer () ? "Server" : "Clerk");
	}
    }
    else
    {
	/*
	 * Subsequent times, append to previous log
	 */
	inaccTraceFP = fopen (inaccFilename, "a+");
    }
    /*
     * If open failed, tell user, turn off trace graph flag and return
     */
    if (inaccTraceFP == NULL)
    {
	perror  (inaccFilename);
	(void) fprintf (stderr, "  Disabling trace graph\n");
	shmPtr->mgtParams.traceFlags &= ~M_TRACE_GRAPH;
	return;
    }

    /*
     * All records use the same value of Tcomp, make it into a string once.
     */
    CvtUTCValueToStr (TcompPtr, tStr, sizeof(tStr));

    recsInSet = sdb.TPset.numResp + sdb.localSet.numResp + 3;
    /*
     * Put out one record for each TP record used in this synch
     */
    QloopBegin( &sdb.TPset.respQ, &rp )
    {
	UTCsub (&rp->Tserv, TcompPtr, &diff);
	CvtUTCValueToStr (&diff, dStr, sizeof(dStr));
	CvtUTCValueToStr (&rp->Iserv, iStr, sizeof(iStr));

	(void) utc_comptime((UTCValue *)NULL, &utcTemp, (long *)NULL,
			    &rp->serverTime);
	CvtUTCValueToStr (&utcTemp, rStr, sizeof(rStr));

	(void) fprintf (inaccTraceFP, GRAPHPRINTSTR,
			sdb.synchSerialNo,
			0,
			"Time-Provider",
			tStr,
			dStr,
			iStr,
			notAllSrvTimesIntersectTcomp,
			recsInSet,
			rStr,
			0);			/* local server */
    }
    QloopEnd;

    /*
     * Put out one record for each local server used in this synch
     */
    QloopBegin( &sdb.localSet.respQ, &rp )
    {
	UTCsub (&rp->Tserv, TcompPtr, &diff);
	CvtUTCValueToStr (&diff, dStr, sizeof(dStr));
	CvtUTCValueToStr (&rp->Iserv, iStr, sizeof(iStr));

	(void) utc_comptime((UTCValue *)NULL, &utcTemp, (long *)NULL,
			    &rp->serverTime);
	CvtUTCValueToStr (&utcTemp, rStr, sizeof(rStr));

	if (rp->serverPtr->nodeNamePtr)
	    namePtr = rp->serverPtr->nodeNamePtr;
	else
	{
	    namePtr = hexName;
	    (void) sprintf (hexName, "%02x-%02x-%02x-%02x-%02x-%02x",
			    rp->serverPtr->nodeAddr.ethernetAddr.bytes[0],
			    rp->serverPtr->nodeAddr.ethernetAddr.bytes[1],
			    rp->serverPtr->nodeAddr.ethernetAddr.bytes[2],
			    rp->serverPtr->nodeAddr.ethernetAddr.bytes[3],
			    rp->serverPtr->nodeAddr.ethernetAddr.bytes[4],
			    rp->serverPtr->nodeAddr.ethernetAddr.bytes[5]);
	}
	(void) fprintf (inaccTraceFP, GRAPHPRINTSTR,
			sdb.synchSerialNo,
			0,
			namePtr,
			tStr,
			dStr,
			iStr,
			notAllSrvTimesIntersectTcomp,
			recsInSet,
			rStr,
			0);	/* local server */
    }
    QloopEnd;

    /*
     * Put out one record for each global server used in this synch
     */
    QloopBegin( &sdb.globalSet.respQ, &rp )
    {
	UTCsub (&rp->Tserv, TcompPtr, &diff);
	CvtUTCValueToStr (&diff, dStr, sizeof(dStr));
	CvtUTCValueToStr (&rp->Iserv, iStr, sizeof(iStr));

	(void) utc_comptime((UTCValue *)NULL, &utcTemp, (long *)NULL,
			    &rp->serverTime);
	CvtUTCValueToStr (&utcTemp, rStr, sizeof(rStr));

	if (rp->serverPtr->nodeNamePtr)
	{
	    namePtr = rp->serverPtr->nodeNamePtr;
        }
	else
	{
	    namePtr = hexName;
	    (void) sprintf (hexName, "%02x-%02x-%02x-%02x-%02x-%02x",
			    rp->serverPtr->nodeAddr.ethernetAddr.bytes[0],
			    rp->serverPtr->nodeAddr.ethernetAddr.bytes[1],
			    rp->serverPtr->nodeAddr.ethernetAddr.bytes[2],
			    rp->serverPtr->nodeAddr.ethernetAddr.bytes[3],
			    rp->serverPtr->nodeAddr.ethernetAddr.bytes[4],
			    rp->serverPtr->nodeAddr.ethernetAddr.bytes[5]);
	}

	(void) fprintf (inaccTraceFP, GRAPHPRINTSTR,
			sdb.synchSerialNo,
			0,
			namePtr,
			tStr,
			dStr,
			iStr,
			notAllSrvTimesIntersectTcomp,
			recsInSet,
			rStr,
			1);			/* global server */
    }
    QloopEnd;

    /*
     * Put out one record for my time before the synch (Tsynch)
     */
    UTCsub (TsynchPtr, TcompPtr, &diff);
    CvtUTCValueToStr (&diff, dStr, sizeof(dStr));
    CvtUTCValueToStr (IsynchPtr, iStr, sizeof(iStr));

    {
	namePtr = hexName;
	(void) sprintf (hexName, "%02x-%02x-%02x-%02x-%02x-%02x",
			shmPtr->myNodeId.ethernetAddr.bytes[0],
			shmPtr->myNodeId.ethernetAddr.bytes[1],
			shmPtr->myNodeId.ethernetAddr.bytes[2],
			shmPtr->myNodeId.ethernetAddr.bytes[3],
			shmPtr->myNodeId.ethernetAddr.bytes[4],
			shmPtr->myNodeId.ethernetAddr.bytes[5]);
    }
    (void) fprintf (inaccTraceFP, GRAPHPRINTSTR,
		    sdb.synchSerialNo,
		    1,
		    namePtr,
		    tStr,
		    dStr,
		    iStr,
		    notAllSrvTimesIntersectTcomp,
		    recsInSet,
		    iStr,
		    0);			/* local server */
    /*
     * Put out one record for computed time (Tcomp)
     *
     * Load up the base times structure currently in use, this is used
     * to get the current base inaccuracy (Icomp + Tcomp-Tsynch)
     */
    basePtr = &shmPtr->baseTimes;
    CvtUTCValueToStr (&basePtr->Ibase, dStr, sizeof(dStr));
    CvtUTCValueToStr (IcompPtr, iStr, sizeof(iStr));
    (void) fprintf (inaccTraceFP, GRAPHPRINTSTR,
		    sdb.synchSerialNo,
		    2,
		    "Tcomp",
		    tStr,
		    "0",
		    dStr,
		    notAllSrvTimesIntersectTcomp,
		    recsInSet,
		    iStr,
		    0);			/* local server */

    /*
     * Put out a nearly blank line as a separator between the synchs
     */
    (void) fprintf (inaccTraceFP, GRAPHPRINTSTR,
		    sdb.synchSerialNo,
		    3,
		    "separator",
		    "0",
		    "0",
		    "0",
		    0,
		    recsInSet,
		    "0",
		    0);			/* local server */

    /*
     * Close the trace file so watchers can see it without waiting for
     * the service to close the file on exit.
     */
    (void) fclose (inaccTraceFP);
}

/*
 *++
 *  UpdateStateTimers ()
 *   Update the absolute timers that are maintained by the state engine.
 *   The timers are updated when DTSS changes the system time.  The
 *   mechanism is to add a relative time to each timer.  The relative
 *   time is the amount of time the local system is being changed by.
 *  Implicit Inputs:
 *	nextChaceFlush,
 *      nextCheckInt,
 *      nextSynchTime.
 *--
 */
void UpdateStateTimers(
    UTCValue *adjustTime
)
{
    utc_t adjutc;

    if (utc_mkcomptime (&adjutc, adjustTime, (UTCValue *)NULL, 0L))
	BugCheck ();

    if (utc_addtime (&nextCacheFlush, &nextCacheFlush, &adjutc))
	BugCheck ();

    UTCadd(adjustTime, &nextCheckInt,   &nextCheckInt);
    UTCadd(adjustTime, &nextSynchTime,  &nextSynchTime);
}
