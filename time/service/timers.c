/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: timers.c,v $
 * Revision 1.1.9.2  1996/02/17  23:35:57  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:52:11  marty]
 *
 * Revision 1.1.9.1  1995/12/08  18:14:23  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/10/03  16:50 UTC  sommerfeld
 * 	Fix DCE_SVC_DEBUG macro invocation; previous one was Just Busted.
 * 	[1995/09/27  13:59 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/26  20:04 UTC  pare
 * 	Merge bl18 changes
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/18  14:12 UTC  pare
 * 	Merge changes from Hpdce02
 * 	[1995/12/08  17:26:20  root]
 * 
 * Revision 1.1.7.8  1994/09/23  20:00:21  tom
 * 	Back out change of unsigned/signed64 to typedef hyper
 * 	[1994/09/23  19:27:07  tom]
 * 
 * Revision 1.1.7.6  1994/08/23  20:32:42  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:53:13  cbrooks]
 * 
 * Revision 1.1.7.5  1994/08/16  18:13:15  cbrooks
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:32  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  17:55 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.7.3  1994/05/16  18:52:06  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:41  rhw]
 * 
 * Revision 1.1.7.2  1994/04/21  22:06:20  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:00:12  cbrooks]
 * 
 * Revision 1.1.7.1  1994/01/25  17:16:56  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/25  17:15:53  rsalz]
 * 
 * Revision 1.1.3.3  1993/09/17  17:44:20  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/17  17:43:46  pare]
 * 
 * 	Bug 7794 - Use lrand48() instead of rand() to get better random
 * 	numbers in RandomizeInterval().
 * 	[1993/07/14  20:47:00  tom]
 * 
 * Revision 1.1.4.3  1993/09/15  17:49:04  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.3.2  1993/07/16  12:38:22  truitt
 * 	OT7794: Use lrand48() instead of rand() to get better random numbers
 * 	        in the RandomizeInterval() function.
 * 	[1993/07/16  12:36:53  truitt]
 * 
 * Revision 1.1.2.2  1992/12/30  16:40:21  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:33:30  htf]
 * 
 * Revision 1.1.1.10  1993/05/04  22:40:47  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1  1992/01/19  15:34:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module TIMERS.C
 *
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
 *	This module implements the interval timers for DTSS timed events, such
 *	as the synchronization timer and time request timeouts.
 *
 * ENVIRONMENT:	portable
 *
 * AUTHORS:
 *		Brian Bailey
 *		Distributed Systems Services (DSS)
 *
 * DATE:	September 1990
 *
 */

#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>

#include <sys/time.h>

#if CONFIG_THREADS

/*
 * shmPtr needed for tracing flags
 */
extern ShmBuf	*shmPtr;


/*
 * DTSS timers
 * 
 * DTSS timers are used to generate events for the main thread to process
 * through the synchronization state machine.  Each timer is itself a state
 * machine, driven by its own timer events.  Each timer is managed by a separate
 * thread.  All timer state is kept in a timer descriptor structure, which is
 * manipulated only by routines in this module.
 *
 * When a timer expires, it generates an event for the main thread to process.
 * Each timer uses an event block to communicate the event information to the
 * main thread.  For simplicity, we use only one event block for each timer and
 * we assume that an expired timer will not be reset before the main thread has
 * processed the event block.  We can make this assumption because the main
 * thread is the only thread that sets timers.  It only resets a timer after it
 * processes the event block.
 *
 * The timer mechanism must be portable (VMS and U*ix) and interruptible in a
 * threaded environment.  For this reason, we've chosen a timed wait on a thread
 * condition variable.  This allows a timer to be cancelled or reset while it
 * is running.  This timer module is portable because the threads package is
 * portable and all time manipulation is done by the DTSS portable API library.
 *
 * Timer operations that are exported by this module are:
 *
 * CreateTimer		creates the timer thread and initializes the timer
 *			state machine.  Called by InitEvent when DTSS starts.
 *			Timer threads last for the life of the DTSS process.
 *
 * InitiateTimer	set a timer to expire after a certain interval.
 *
 * CancelTimer		cancel a timer.
 *
 * UpdateTimer		update all running timers by changing the expiration
 *			times by a given interval.  This is called when DTSS
 *			changes the system clock.
 *
 */

/*
 * timer_dsc -- Timer descriptor
 *
 * Each timer has a descriptor with the following components:
 *
 * expirationTime	the expiration time of the timer, if the timer is
 *			running.  This is an absolute time represented as a
 *			POSIX timespec structure.
 *
 * running		the state of the timer (boolean variable).
 *
 * timerEvent		the event that causes the timer to change state, either
 *			SET, CANCEL, or EXPIRE.  These events are local to this
 *			timer module and are separate from the general
 *			synchronization events represented by event blocks.
 *
 * timerEventCond	condition variable that gets signalled when we want a
 *			timer to change its state.
 *
 * expired		gets set to TRUE when the timer expires.  A timer uses
 *			this flag to tell the main thread that it has generated
 *			a synchronization event and it should process the event
 *			block for this timer.
 *
 * eblkPtr		is a pointer to the event block associated with the
 *		        timer.  The main thread processes this when the timer
 *			expires.
 *
 * mutex		mutex protecting the timer descriptor.  Must be locked
 *			wnen accessing any descriptor fields, except the thread
 *			handle.
 *
 * thread		is the handle for the timer thread.
 *
 */
typedef struct timer_dsc
{
    volatile struct timespec	expirationTime;
    volatile int	        running;
    volatile int		expired;
    volatile int		timerEvent;
    pthread_cond_t      	timerEventCond;
    EventBlk			*eblkPtr;
    pthread_mutex_t		mutex;
    pthread_t			thread;
} timer_dsc;

/*
 * Array of timer descriptors - one per timer.
 */
static timer_dsc	timers[K_NUM_TIMERS+1];


/*
 * Timer events - possible values of timer_dsc.timerEvents
 *
 * These events are internal to the timer module and should not be confused with
 * the synchronization events that the main thread processes.
 */
#define K_TIMER_EVENT_NULL	0
#define K_TIMER_EVENT_SET	1
#define K_TIMER_EVENT_CANCEL	2
#define K_TIMER_EVENT_EXPIRE	3

/*
 * Local Prototypes
 */

static void Timer (int);

static char *  PrintTimerEvent (int, int);


/*
 *
 * CreateTimer () -
 *
 * Description:
 *
 *	Create a timer.  Initialize the descriptor for the given timer, then
 *	create a thread running the routine Timer, passing it a pointer to its
 *      timer descriptor.
 *
 * Inputs:
 *
 *	t_index - index of the timer in timers[]
 *
 * Implicit Inputs:
 *
 *	timers - array of timer descriptors
 *
 * Outputs:
 *
 *	none
 *
 * Implicit Outputs:
 *	timers[t_index] descriptor fields are initialized and the mutex and
 *	condition variable are created.
 *
 * Return Value:
 *
 *	none
 *
 */
void CreateTimer (
    int	t_index
)
{
    extern pthread_attr_t thread_attribute;

     DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
 	     "Main:  Creating timer %d\n", t_index));

    timers[t_index].running = FALSE;
    timers[t_index].expired = FALSE;
    timers[t_index].timerEvent = K_TIMER_EVENT_NULL;

    if (!(timers[t_index].eblkPtr = DTSSmallocStruct (EventBlk)))
	BugCheck ();
    memset( (VOID *)timers[t_index].eblkPtr, 0, sizeof(EventBlk));
    if (t_index == K_TIMER_NEXT_SYNCH)
	timers[t_index].eblkPtr->event = ETsynchronize;
    else if (t_index == K_TIMER_ADVERTISE)
	timers[t_index].eblkPtr->event = ETadvertise;
    else if (t_index == K_TIMER_QUERY_TMO)
	timers[t_index].eblkPtr->event = ETqueryTimeout;
    else
	BugCheck ();

    dtss_mutex_init (&timers[t_index].mutex);
    dtss_cond_init (&timers[t_index].timerEventCond);
    if (pthread_create (&timers[t_index].thread,
			thread_attribute,
			(pthread_startroutine_t) Timer,
			(pthread_addr_t) t_index) < 0)
	BugCheckOSstatus (errno);


} /* end of routine CreateTimer */

/*
 *
 * InitiateTimer () -
 *
 * Description:
 *
 *	Set a timer to expire after the specified interval of time has
 *	passed.  If a negative relative time comes in, the timer will expire
 *	immediately (this is a crock).
 *
 *	The timing mechanism requires an absolute expiration time, which is
 *	computed from the given intval.  This means that whenever the system
 *	clock is changed, UpdateTimer must be called to adjust the expiration
 *	times of all DTSS timers.  On VMS, only DTSS can change the system
 *	clock, so we can account for all clock changes.  On U*ix, we cannot
 *	account for someone changing the clock behind our backs.  In this case,
 *	DTSS timers will be incorrect.  DTSS cannot currently solve this problem
 *	until it can somehow be notified that the system clock was changed.
 *
 * Inputs:
 *
 *	t_index 	is the index of the timer in timers[]
 *
 *	intval		is the expiration interval for the timer (a UTC relative
 *			time value)
 *
 * Implicit Inputs:
 *
 *	timers		is the array of timer descriptors
 *
 * Outputs:
 *
 *	none
 *
 * Return Value in StatusBlk.status:
 *
 *	DTSS_SUCCESS - timer initiated
 *	DTSS_ERROR - error initiating timer
 *
 */
StatusBlk InitiateTimer(
    int 		t_index,
    UTCValue 	*intval
)
{
    StatusBlk 	statusBlk;
    utc_t	timedInterval, expiration;
    timespec_t  temp_timespec;

     DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
 	    "--> Initiating timer %d\n", t_index));

    /*
     * Compute the absolute expiration time by adding the interval to the
     * current time.  We convert all times to utc format for portable time
     * manipulation.
     */
    (void) utc_mkcomptime (&timedInterval, intval, (ExtInacc *)NULL, 0L);
    (void) utc_addtime (&expiration, (utc_t *)NULL, &timedInterval);

    /*
     * Now convert the expiration time into the form required by the timer
     * mechanism (a POSIX timespec) and place it in the timer descriptor.
     */
    (void) utc_bintime (&temp_timespec, (timespec_t *)NULL, (long *)NULL,
			&expiration);

    dtss_mutex_lock (&timers[t_index].mutex);
    timers[t_index].expirationTime = temp_timespec;

    /*
     * Signal the timer thread to set the timer.
     */
    timers[t_index].timerEvent = K_TIMER_EVENT_SET;
    dtss_cond_signal (&timers[t_index].timerEventCond);

    dtss_mutex_unlock (&timers[t_index].mutex);
	   
    statusBlk.status = DTSS_SUCCESS;

    return (statusBlk);
}

/*
 *
 * UpdateTimer () -
 *
 * Description:
 *
 *	Updates timer database to reflect the fact that the system's clock
 *	is being changed. Alas, all the other applications in the system
 *	will just misbehave in strange and marvelous ways.
 *
 *  	This update is necessary because the portable interruptible timing
 *	mechanism (a call to pthread_cond_timedwait) only accepts an absolute
 *	time as its timeout parameter.  Thus, when the system clock changes by
 *	a given interval, timer expiration times must be adjusted by the same
 *	amount in order to correctly time the originally chosen interval.
 *
 *	For each running timer, the adjustment interval is added to the timer's
 *	absolute expiration time to produce a new absolute expiration time.
 *	All times are converted to utc format for portable time manipulation.
 *
 * Inputs:
 *
 *	interval	is the adjustment interval for the timer (relative time)
 *
 * Implicit Inputs:
 *
 *	timers		is the array of timer descriptors
 *
 * Outputs:
 *
 *	none
 *
 * Return Value in StatusBlk.status:
 *
 *	DTSS_SUCCESS
 *
 */
StatusBlk UpdateTimer (
    UTCValue	*interval
)
{
    StatusBlk	statusBlk;
    int 	i;
    utc_t	expiration, newExpiration, adjustment;
    timespec_t  temp_timespec;


    (void) utc_mkcomptime (&adjustment, interval, (ExtInacc *)NULL, 0L);

    for (i = 0; i < K_NUM_TIMERS; i++) {

	dtss_mutex_lock (&timers[i].mutex);

	if (timers[i].running) {

 	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
 		   "--> Updating timer %d\n", i));

	    /*
	     * Compute a new expiration time by adding the adjustment to the
	     * existing expiration time.
	     */
	    temp_timespec = timers[i].expirationTime;
	    (void) utc_mkbintime (&expiration, &temp_timespec,
				  (timespec_t *)NULL, 0L);
	    (void) utc_addtime (&newExpiration, &expiration, &adjustment);
	    (void) utc_bintime (&temp_timespec, (timespec_t *)NULL,
				(long *)NULL, &newExpiration);
	    timers[i].expirationTime = temp_timespec;

	    /*
	     * Signal the timer thread to reset the timer
	     */
	    timers[i].timerEvent = K_TIMER_EVENT_SET;
	    dtss_cond_signal (&timers[i].timerEventCond);

	}
	dtss_mutex_unlock (&timers[i].mutex);
    }

    statusBlk.status = DTSS_SUCCESS;

    return (statusBlk);
}

/*
 *
 * CancelTimer () -
 *
 * Description:
 *
 *	Cancel the specified timer request.
 *
 * Inputs:
 *
 *	t_index		is the index of the timer in timers[] to cancel
 *
 * Implicit Inputs:
 *
 *	timers		is the array of timer descriptors
 *
 * Outputs:
 *
 *	none
 *
 * Return Value:
 *
 *	none
 *
 */
void CancelTimer (
    int	t_index
)
{

     DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
 		   "--> Cancelling timer %d\n", t_index));

    dtss_mutex_lock (&timers[t_index].mutex);

    timers[t_index].timerEvent = K_TIMER_EVENT_CANCEL;
    timers[t_index].expired = FALSE;
    dtss_cond_signal (&timers[t_index].timerEventCond);

    dtss_mutex_unlock (&timers[t_index].mutex);
    
} /* end of routine CancelTimer */




/*
 * RandomizeInterval () -
 *
 * Description:
 *
 *	Tries to select a random number
 *	in the range (intval*3/4 to intval*5/4) or (intval*1/2 to intval).
 *	Note that both ranges are intval/2 wide, they just start differently.
 *
 *	Random timers used to assure that multiple DTSS services will send their
 *	requests and advertisements at different times. 
 *
 * Inputs:
 *
 *	newval - address of UTCValue to return.
 *	intval - expiry interval for the timer (relative time)
 *	minFactor - RThalfTo1 to choose from intval/2 to intval
 *                  RT34To54 to choose from intval*3/4 to intval*5/4
 *
 * Implicit Inputs:
 *
 *	timers - array of timer descriptors
 *
 * Outputs:
 *
 *	newval - randomized interval.
 *
 * Return Value in StatusBlk.status:
 *
 *	DTSS_SUCCESS - timer initiated
 *	DTSS_ERROR   - error initaiting timer
 *
 */
void RandomizeInterval (
    UTCValue 		*newval,
    UTCValue 		*intvalPtr,
    RandomTimerFactor	minFactor
)
{
    UTCValue r;
    
#if LONG_BIT >= 64
    if ( *intvalPtr >= 0x100000000 ) {	/* if intval >= 2**32 */
#else
    if ( intvalPtr->hi ) {              /* if intval >= 2**32 */
#endif

	int shiftCnt;
	unsigned long hiBit;
	UTCValue q;
	
        r = q = *intvalPtr;
        /*
         * subtract 1/2 or 1/4 of the time
         */
        if (minFactor == RThalfTo1) {
	    UTCshiftRight (&q, 1);
	    }
        else
            UTCshiftRight (&q, 2);
        UTCsub (intvalPtr, &q, newval);
        /*
         * Shift value right so that it all fits in low 32 bits
         * to simplify arithmetic. We shift some significant bits
         * off the right end of the quadword, but this is not very
         * important: as the timer gets longer, the details about
         * when it fires become less important.
         */
	shiftCnt = 32;
#if LONG_BIT >= 64
        hiBit = 0x8000000000000000;
        while ( (r & hiBit) == 0 ) {
#else
	hiBit = 0x80000000UL;
	while ( (r.hi & hiBit) == 0 ) {
#endif
	    shiftCnt--;
	    hiBit = hiBit >> 1;
	    }
	UTCshiftRight (&r, shiftCnt);
	UTClow(&r) = lrand48 () % UTClow(&r);
	/*
	 * Shift left again, but one bit less so the result is
	 * in the range 0 to intval/2
	 */
	if (--shiftCnt > 0)
            UTCshiftLeft (&r, shiftCnt);
	}
    else {                              /* else intval < 2**32 */
        /*
         * subtract intval/2 from intval to make intval*1/2 or
         * subtract intval/4 from intval to make intval*3/4
         */
        *newval = *intvalPtr;
        if (minFactor == RThalfTo1)
            UTClow(newval) -= UTClow(intvalPtr) / 2;
        else
	    UTClow(newval) -= UTClow(intvalPtr) / 4;
        /*
         * pick a random number in range 0 to intval/2
         */
#if LONG_BIT < 64
        r.hi = 0;
#endif
        UTClow(&r) = lrand48 () % (UTClow(intvalPtr) / 2);
	}
    /*
     * add random interval (0 to intval/2) to newval (3*intval/4)
     */
    UTCadd (newval, &r, newval);
    /*
     * start the timer and return its status code
     */
#if LONG_BIT >= 64
     DCE_SVC_DEBUG((dts_svc_handle, dts_s_msgs, svc_c_debug1,
                   "Random timer,\tRequested:  %u,\n\t\tRandomized: %u\n",
                            *intvalPtr,
                            *newval));
#else
     DCE_SVC_DEBUG((dts_svc_handle, dts_s_msgs, svc_c_debug1,
		   "Random timer,\tRequested:  %u %u,\n\t\tRandomized: %u %u\n",
			    intvalPtr->hi, intvalPtr->lo,
			    newval->hi, newval->lo));
#endif

} /* end of routine RandomizeInterval */

/*
 *
 * InitiateRandomTimer () -
 *
 * Description:
 *
 *	Set a timer to expire after a random approximation of the specified
 *	interval of time has passed. Tries to select a random number
 *	in the range (intval*3/4 to intval*5/4) or (intval*1/2 to intval).
 *	Note that both ranges are intval/2 wide, they just start differently.
 *
 *	Random timers used to assure that multiple DTSS services will send their
 *	requests and advertisements at different times. 
 *
 * Inputs:
 *
 *	t_index - index of the timer in timers[]
 *	intval - expiry interval for the timer (relative time)
 *	minFactor - RThalfTo1 to choose from intval/2 to intval
 *                  RT34To54 to choose from intval*3/4 to intval*5/4
 *
 * Implicit Inputs:
 *
 *	timers - array of timer descriptors
 *
 * Outputs:
 *
 *	none
 *
 * Return Value in StatusBlk.status:
 *
 *	DTSS_SUCCESS - timer initiated
 *	DTSS_ERROR   - error initaiting timer
 *
 */

StatusBlk InitiateRandomTimer (
    int               	t_index,
    UTCValue          	*intvalPtr,
    RandomTimerFactor	minFactor
)
{
    UTCValue newval;

    /*
     * Randomize the interval and initiate the timer.
     */
    RandomizeInterval(&newval, intvalPtr, minFactor);
    return (InitiateTimer(t_index, &newval));

} /* end of routine InitiateRandomTimer */


/*
 * GetNextTimerEvent ()
 *
 * Functional Description:
 *
 *	This routine is called by GetNextEvent to look for synchronization
 *	events generated by expired timers.  It traverses the array of timer
 * 	descriptors and returns a pointer to the event block belonging to the
 *	first expired timer.  If no timers are expired, it returns NULL.
 *
 *	Note that this algorithm may result in timer events being processed in
 *	a different order in which they expired, if the difference between the
 *	expiration times is less than the resolution of the thread scheduling
 *	algorithm.  These differences are not considered significant for DTSS.
 *
 */
EventBlk *GetNextTimerEvent (void)
{
    EventBlk	*eblkPtr;
    int		i;


    for (i = 0; i < K_NUM_TIMERS; i++) {

	dtss_mutex_lock (&timers[i].mutex);

	if (timers[i].expired) {
	    timers[i].expired = FALSE;
	    eblkPtr = timers[i].eblkPtr;
	    dtss_mutex_unlock (&timers[i].mutex);
 
 	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
 			   "Main:  Got timer %d event\n", i));
 
	    return (eblkPtr);
	}

	dtss_mutex_unlock (&timers[i].mutex);

    }

    return (NULL);

} /* end of routine GetNextTimerEvent */

/*
 *
 * Timer()
 *
 * Description:
 *
 *	This routine runs in the thread for each DTSS timer.
 *
 * Inputs:
 *
 *	timerPtr	is a pointer to the descriptor for this timer
 *
 * Implicit Inputs:
 *
 *	none
 *
 * Outputs:
 *
 *	none
 *
 * Return Value:
 *
 *	none
 *
 */

static void Timer (
    int	t_index
)
{
    timer_dsc		*timerPtr;
    struct timespec	expirationTime;

    timerPtr = &timers[t_index];

    dtss_mutex_lock (&timerPtr->mutex);

    for(;;) {

	if (timerPtr->running) {

	    /*
	     * The timer's been set.  Get the expiration time and wait until
	     * it expires or until someone cancels the timer or sets it again.
	     */

	    expirationTime = timerPtr->expirationTime;
	    while (timerPtr->timerEvent == K_TIMER_EVENT_NULL) {
 		DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
 			       "Timer %d:  running\n", t_index));

		if (pthread_cond_timedwait (&timerPtr->timerEventCond,
					    &timerPtr->mutex,
					    &expirationTime) < 0) {
		    if (errno == EAGAIN) {
			if (timerPtr->timerEvent == K_TIMER_EVENT_NULL)
			    timerPtr->timerEvent = K_TIMER_EVENT_EXPIRE;
		    }
		    else
			BugCheck();
		}
	    }
	}
	else {

	    /*
	     * The timer's been stopped.  Wait until someone performs another
	     * timer operation.
	     */

	    while (timerPtr->timerEvent == K_TIMER_EVENT_NULL) {
 
 		DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
 			       "Timer %d:  stopped\n", t_index));

		dtss_cond_wait (&timerPtr->timerEventCond, &timerPtr->mutex);
	    }
	}

	/*
	 * Some timer event has occured.  If it was a SET, mark the timer as
	 * running.  If it was EXPIRE, mark the timer as expired.
	 */

 	DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
 		       "%s\n", PrintTimerEvent (t_index, timerPtr->timerEvent)));

	timerPtr->running = (timerPtr->timerEvent == K_TIMER_EVENT_SET);
	timerPtr->expired = (timerPtr->timerEvent == K_TIMER_EVENT_EXPIRE);
	timerPtr->timerEvent = K_TIMER_EVENT_NULL;

	/*
	 * If the timer expired, wake the main thread so it processes the timer
	 * event block.
	 */

	if (timerPtr->expired) {
	    WakeMainWithEvent ();
	}

    }

    /*
     * If we ever decide to terminate this thread in an orderly fashion (and
     * break out of the infinite loop), then here we have to do:
     *
     * dtss_mutex_unlock (&timerPtr->mutex);
     *
     */


} /* end of routine Timer */

/*
 * PrintTimerEvent
 *
 */
static char * PrintTimerEvent (
    int	t_index,
    int	t_event
)
{
    char	*name;

    switch (t_event) {

    case K_TIMER_EVENT_NULL:
	name = "Null event";
	break;

    case K_TIMER_EVENT_SET:
	name = "Set";
	break;

    case K_TIMER_EVENT_CANCEL:
	name = "Cancelled";
	break;

    case K_TIMER_EVENT_EXPIRE:
	name = "Expired";
	break;

    default:
	name = "*** BOGUS TIMER EVENT ***";
	break;

    }
    return name ;
}

#endif /* CONFIG_THREADS */

