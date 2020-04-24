/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: provider_interface.c,v $
 * Revision 1.1.11.2  1996/02/17  23:35:50  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:52:05  marty]
 *
 * Revision 1.1.11.1  1995/12/08  18:14:15  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/26  20:00 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:26:14  root]
 * 
 * Revision 1.1.9.8  1994/08/31  20:30:57  cbrooks
 * 	CR11964 - auditing calls cause dtsd to dump core
 * 	[1994/08/31  20:21:33  cbrooks]
 * 
 * Revision 1.1.9.7  1994/08/16  18:13:13  cbrooks
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:29  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/18  14:05 UTC  pare
 * 	Merge changes from Hpdce02
 * 
 * Revision 1.1.9.6  1994/08/05  20:47:49  cbrooks
 * 	CR11240 - add dced config functionality
 * 	[1994/08/05  20:23:52  cbrooks]
 * 
 * Revision 1.1.9.5  1994/06/30  19:45:33  cbrooks
 * 	CR11112 - new serviceability messages required
 * 	[1994/06/28  17:49:00  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  17:52 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.8.3  1993/09/17  17:51:50  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/17  17:51:13  pare]
 * 
 * Revision 1.1.9.3  1993/09/15  17:49:02  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.8.2  1993/07/08  20:27:23  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:26:53  root]
 * 
 * Revision 1.1.9.4  1994/05/16  18:52:02  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:38  rhw]
 * 
 * Revision 1.1.9.3  1994/04/28  21:08:58  cbrooks
 * 	DTS Code Cleanup
 * 	[1994/04/28  21:07:40  cbrooks]
 * 
 * Revision 1.1.9.2  1994/04/21  22:06:17  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:00:08  cbrooks]
 * 
 * Revision 1.1.9.1  1994/01/25  17:16:53  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/25  17:15:50  rsalz]
 * 
 * Revision 1.1.7.2  1993/06/10  19:33:25  sommerfeld
 * 	Added rpc_binding_free in exit path of CreateTPbinding
 * 	to fix memory leak in attempted synchronize.
 * 	[1993/04/29  18:56:27  truitt]
 * 
 * Revision 1.1.6.2  1993/03/24  14:13:17  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/24  14:12:32  truitt]
 * 
 * Revision 1.1.1.8  1993/03/11  22:35:35  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.2.3  1992/12/30  16:39:58  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:23:38  htf]
 * 
 * Revision 1.1.2.2  1992/12/18  18:19:40  marcyw
 * 	Defect 4998: adding disclaimer comment
 * 	[1992/12/18  18:13:29  marcyw]
 * 
 * Revision 1.1.4.2  1992/12/04  20:46:26  smythe
 * 	added rpc_binding_free in exit path of CreateTPbinding
 * 	to fix memory leak in attempted synchronize  CHFts06335
 * 	[1992/12/04  20:45:20  smythe]
 * 
 * Revision 1.1.1.7  1992/09/21  18:57:15  osfrcs
 * 	    Loading drop Dce1_0_1fnl
 * 
 * Revision 1.1  1992/01/19  15:34:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module PROVIDER_INTERFACE.C
 *
 * Copyright (c) 1989,1990 by
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
 *	This module contains the DTSS routines that handle communication with
 *	a timer provider program.
 *
 *	Some of these routines were taken from DTSS_SERVICE_STATE.C
 */

#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>
#include <string.h>
#include <dtsd_macros.h>
#include <dce/dce.h>
#include <dcedtssvc.h>
#include <dcedtsmsg.h>
#include <dcedtsmac.h>

#if	defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <aud_evtsp.h>
#endif	/* defined(DCE_AUD_POINTS) */


/*
 * External Variables
 */
extern ShmBuf *shmPtr;                 /* Ptr to shared memory
                                        * all modules have shmPtr defined */
extern Sdb sdb;			       /* synchronization database */

extern RpcBuf *rpcBufPtr;

#if	defined(DCE_AUD_POINTS)
extern dce_aud_trail_t dts_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */

/*
 * Internal Static Variables
 */

/*
 * Time provider thread variables.
 */

static pthread_t	TPthread;

static Que		TPeventQ;
static pthread_mutex_t	TPmutex;
static pthread_cond_t	TPcond;

/*
 * Time provider communication variables
 */
#define K_TP_BINDING_NAME	"ncadg_ip_udp:127.0.0.1"
#define K_TP_MAX_BINDING_NAME	100


/*
 * Prototypes
 */

pthread_addr_t GetTimeFromProvider (pthread_addr_t );

static StatusBlk AllocTPevent (TPrespDsc *, EventBlk **);

static void FreeTPevent (EventBlk *);

static void GenTPevent (EventTypes event, EventBlk *);

static error_status_t CreateTPbinding (rpc_binding_handle_t * );

void dive( int ) ;

/*
 *++
 *  InitTPinterface ()
 *	This routine initializes the time provider interface.  It creates the
 *	necessary thread synchronization variables.
 *
 *	TBD - When we RPCify, we might have to set up to listen for RPC requests
 *	here.
 *
 *  Implicit Outputs:
 *      TPeventQ	gets initialized to empty.
 *	TPmutex		the mutex object gets created
 *	TPcond 		the condition variable gets created.
 *
 *--
 */
void InitTPinterface (void)
{
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "Main:  Initializing TP interface\n"));

    Qinit (&TPeventQ);
    dtss_mutex_init (&TPmutex);
    dtss_cond_init (&TPcond);
} /* end of routine InitTPinterface */

/*
 *++
 *  ShutdownTPinterface ()
 *	This routine shuts down the time provider interface.  The main thread
 *	calls it after exiting the main processing loop.
 *
 *	TBD - right now this routine terminates the TP listening IPC.  When we
 *	RPCify, this routine may go away altogether, or it may have to perform
 *	some RPC-specific one-time shutdown stuff.
 *
 *  Implicit Inputs:
 *	TPlisteningIPCdesc
 *
 *  Side Effects:
 *      We stop listening for TP IPC messages.
 *
 *--
 */
void ShutdownTPinterface (void)
{

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "Main:  Shutting down TP interface\n"));
} /* end of routine ShutdownTPinterface */

/*
 *++
 *  ContactTimeProvider()
 *	This routine is called by the main thread to attempt to obtain time from
 *	a provider.  It creates a time provider thread running the routine
 *	GetTimeFromProvider.
 *
 *  Implicit Inputs:
 *	TPthread	is the time provider thread handle.
 *
 *  Value Returned:
 *  	TRUE if the thread was created successfully, FALSE otherwise.
 *
 *  Side Effects:
 *	A TP thread is created and begins executing GetTimeFromProvider.
 *
 *--
 */
int ContactTimeProvider(void)
{
    TPrespDsc	*rdPtr;
    extern pthread_attr_t thread_attribute;

    rdPtr = DTSSmallocStruct (TPrespDsc);
    rdPtr->respPtr = (char *)NULL;
    rdPtr->syncID = sdb.synchSerialNo;
    rdPtr->release = FALSE;

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "Main:  Creating TP thread\n"));

    if (pthread_create (&TPthread,
			thread_attribute,
			GetTimeFromProvider,
			(pthread_addr_t)rdPtr) < 0) {
	DTSSfree ((VOID *)rdPtr);
	return (FALSE);
    }
    else {
	return (TRUE);
    }
} /* end of routine ContactTimeProvider */

/*
 *++
 *  ReleaseTimeProvider()
 *  	This routine tells the TP thread to release the time provider.  It sets
 *	the release flag in the TP response descriptor for the TP event block
 *	currently being processed.
 *
 *  Inputs:
 *	eblkPtr		is the provider event block currently being processed.
 *
 *  Implicit Inputs:
 *      sdb.TPvalid is current status of TP synchronization.
 *
 *  Implicit Outputs:
 *      shmPtr->mgtParams.hasProvider gets set to the value of sdb.TPvalid.
 *
 *--
 */
void ReleaseTimeProvider (
    EventBlk	*eblkPtr
)
{
    TPrespDsc	*rdPtr;

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "Main:  Releasing TP thread\n"));

    shmPtr->mgtParams.hasProvider = sdb.TPvalid;

    if (eblkPtr != NULL) {
	rdPtr = (TPrespDsc *)eblkPtr->ioDsc.ptr;
	rdPtr->release = TRUE;
    }

    /*
     * Detach the TP thread handle so we can use it again.  Ignore an ESRCH
     * error, which means that the thread has already terminated.
     */
    if ((pthread_detach (&TPthread) < 0) && (errno != ESRCH))
	BugCheckOSstatus (errno);
} /* End of routine ReleaseTimeProvider */

/*
 *++
 *  GetNextTPevent()
 *	Get the next time provider event.  This routine returns the next event
 *	event block from the TP event queue by calling GetNextThreadEvent.  If
 *	the queue is empty, it returns NULL.
 *
 *  Implicit Inputs:
 *	TPeventQ	is the queue of TP event blocks waiting to be processed.
 *	TPmutex		is the TP event queue mutex.
 *
 *  Implicit Outputs:
 *      TPeventQ	if this queue is not empty, the first event block is
 *			is removed.
 *
 *  Value Returned:
 *	eblkPtr		is a pointer to the element removed from the queue.  If
 *			the queue is empty, this is NULL.
 *
 *--
 */
EventBlk *GetNextTPevent (void)
{
    EventBlk	*eblkPtr;

    eblkPtr = GetNextThreadEvent (&TPeventQ, &TPmutex);

#ifdef	DCE_DEBUG
    if (eblkPtr) {
	char *p;
	if (eblkPtr->event == ETprovider)
	    p = "ETprovider";
	else if (eblkPtr->event == ETprvdrFail)
	    p = "ETprvdrFail";
	else
	    p = "BOGUS TP";
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	    "Main:  Got %s event\n", p));
    }
#endif

    return (eblkPtr);
} /* end of routine GetNextTPevent */

/*
 *++
 *  WakeTPthreadWithResp ()
 *	This routine is called by the main thread to respond to an event from
 *	the TP thread.  It wakes the TP thread.
 *
 *  Implicit Inputs:
 *	TPmutex		is the TP database mutex
 *	TPcond		is the condition variable on which the TP thread is
 *			waiting.
 *
 *  Side Effects:
 *	The TP thread is awakened and resumes execution.
 *
 *--
 */
void WakeTPthreadWithResp (
    EventBlk	*eblkPtr
)
{

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "--> Waking TP thread\n"));

    WakeThreadWithResp (&TPmutex, &TPcond, eblkPtr);
} /* end of routine WakeTPthreadWithResp */

/*
 *++
 *  ProcessTPtmoResponse()
 *  Process the initial TP response to a Time Service Time Request.
 *  The routine will check the validity of the message identification
 *  and user supplied input of the Time Provider Control Message.
 *
 *  Inputs:
 *		eblkPtr - The address of the Event Block returned from
 *			  GetNextEvent.
 *
 *  Implicit Inputs:
 *
 *		sdb.SerialNo.
 *
 *  Outputs:
 *
 *		nextPoll    - The value specified in the time service
 *			      message.
 *		timeout     - The value specified in the time service
 *			      message.
 *
 *
 *  Value Returned:
 *
 *		TPmsgInvalid if TPstatus == TPunsuccessful, or we didn't get
 *		a response from the TP.
 *		TPmsgSyncSerMismatch if sdb.SerialNo != message synch ID
 *		TPmsgInvalid if timeout or nextPoll values are out of range.
 *		TPmsgValid otherwise.
 *
 *
 *--
 */
TPmsgStatus ProcessTPtmoResponse(
    EventBlk  *eblkPtr,
    unsigned  long *timeout,
    unsigned  long *nextPoll,
   unsigned  long *noSetBit
)
{
    TPrespDsc	*respDsc;
    TPctlMsg    *rspMsg;

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "Main:  Processsing TP control message\n"));

    respDsc = (TPrespDsc *)eblkPtr->ioDsc.ptr;

    /*
     * Do the sync serial numbers match ?
     */
    if ( sdb.synchSerialNo != respDsc->syncID )
    {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
			 "Bad TP response, found Serial %d , expected %d",
			 respDsc->syncID, sdb.synchSerialNo));
	return( TPmsgSyncSerMismatch );
    }

    /*
     * Did we get a response from the TP at all?
     */
    if (eblkPtr->event == ETprvdrFail) {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
			 "No response from TP on request %d\n",
			 sdb.synchSerialNo));
	return( respDsc->respStatus );
    }

    rspMsg = (TPctlMsg *)respDsc->respPtr;

    /*
     * Is this the correct response Type?
     */
    if ( respDsc->respType != K_TP_RESP_CONTROL )
    {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
		 	"Bad TP response, found TIME type, expected CTL"));
	return(TPmsgTypeMismatch);
    }

    /*
     * Is the time provider in a good state?
     */
    if ( rspMsg->status != K_TPI_SUCCESS )
    {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
		    "Bad TP CTL message, TPUnsuccessful\n"));
        return( TPmsgInvalid );
    }

    /*
     * Is the user specified timeout within bounds?
     */
    *timeout = rspMsg->timeout;

    if (*timeout > K_MAX_TP_TMO  || *timeout < K_MIN_TP_TMO )
    {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
			 "Bad TP message, timeout value %d out of range\n",
			 rspMsg->timeout));
	return( TPmsgInvalid );
    }

    /*
     * Is the user supplied poll rate within bounds.
     */
    *nextPoll = rspMsg->nextPoll;

    if (*nextPoll > K_MAX_TP_POLL  ||  *nextPoll < K_MIN_TP_POLL )
    {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
			 "Bad TP message, nextPoll value %d out of range",
			 rspMsg->nextPoll));
	return( TPmsgInvalid );
    }

    /*
     * get the noSetBit
     */
    *noSetBit = rspMsg->noClockSet & 1;

    return( TPmsgValid );
} /* End of routine ProcessTPtmoResponse */

/*
 *++
 *  ProcessTPresponse()
 *  Process a message which has been recieved from the time provider
 *  process.  Fill the TP response queue if the message is valid.
 *
 *  Inputs:
 *
 *	eblkPtr - event block pointer.
 *
 *  Implicit Outputs:
 *
 *	The time provider response queue fields of the SDB are
 *	filled.
 *
 *  Value Returned:
 *
 *   	TPmsgSyncSerMismatch if sdb.synchSerialNo != synchID of message
 *	TPmsgInvalid if status of msg is TPunsuccessful or if data fields
 *		values are out of range or if the TP didn't respond at all.
 *	TPmsgTypeMismatch if msg != TPtimeMessage
 *	TPmsgValid Otherwise.
 *
 *
 *
 *--
 */
TPmsgStatus ProcessTPresponse( 
    EventBlk  *eblkPtr
)
{
    TPrespDsc		*respDsc;
    TPtimeMsg   	*rspMsg;
    ResponseElement	*rp;
    int			i;

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
	     "Main:  Processsing TP times message\n"));

    respDsc = (TPrespDsc *)eblkPtr->ioDsc.ptr;

    /*
     * Do the sync serial numbers match ?
     */
    if ( sdb.synchSerialNo != respDsc->syncID )
    {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
			 "Bad TP message, found Serial %d , expected %d",
			 respDsc->syncID, sdb.synchSerialNo));
	return( TPmsgSyncSerMismatch );
    }

    /*
     * Did we get a response from the TP at all?
     */
    if (eblkPtr->event == ETprvdrFail) {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
			 "No response from TP on request %d\n",
			 sdb.synchSerialNo));
	return( TPmsgInvalid );
    }

    rspMsg = (TPtimeMsg *)respDsc->respPtr;

    /*
     * Is this the correct response Type?
     */
    if ( respDsc->respType != K_TP_RESP_TIMES )
    {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
		 	"Bad TP response, found CTL type, expected TIME"));
	return(TPmsgTypeMismatch);
    }

    /*
     * Is the time provider in a good state?
     */
    if (rspMsg->status == K_TPI_FAILURE )
    {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
				"Bad TP DATA message, TPunsuccessful\n"));
	  return( TPmsgInvalid );
    }

    /*
     * Is the timStampCount of the response valid?
     */
    if ( (rspMsg->timeStampCount > K_MAX_TIMESTAMPS) ||
         (rspMsg->timeStampCount < K_MIN_TIMESTAMPS)   )
    {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
		      "Bad TP DATA message, Time Stamp value %d out of range\n",
			 rspMsg->timeStampCount));
	return( TPmsgInvalid );
    }

    sdb.TPset.numResp = rspMsg->timeStampCount;
    for ( i = 0; i < rspMsg->timeStampCount; i++)
    {
    utc_t beforeTime, afterTime;

	rp = DTSSmallocStruct(ResponseElement);
        (void)memcpy( (VOID *)&beforeTime,
	        (VOID *)&rspMsg->timeStampList[i].beforeTime,
	        sizeof( beforeTime ));
	(void) utc_comptime(&rp->sendTime, (ExtInacc *)NULL, (long *)NULL,
                            &beforeTime);

        (void)memcpy( (VOID *)&rp->serverTime,
	        (VOID *)&rspMsg->timeStampList[i].TPtime,
	        sizeof(rp->serverTime));

        (void)memcpy((VOID *)&afterTime,
	       (VOID *)&rspMsg->timeStampList[i].afterTime,
               sizeof( afterTime ));
	(void) utc_comptime(&rp->recvTime, (ExtInacc *)NULL, (long *)NULL,
			    &afterTime);

	rp->responseRecvd = TRUE;
        UTCzero (&(rp->processingDelay));

	rp->serverPtr = 0;
	QinsertLast( &sdb.TPset.respQ, rp );
    }

    return( TPmsgValid );
} /* End of routine ProcessTPresponse */

/*
 *++
 *  GetTimeFromProvider()
 *  	This routine obtains timestamps from a time provider.  It runs in its
 *	own thread, created when the main thread calls ContactTimeProvider,
 *	which is done at each synchronization.  This thread lasts for the
 *	duration of one request sequence with the time provider.
 *
 *	It first requests control information from the time provider.  It passes
 *	this control information in an ETprovider event block to the main
 *	thread and waits.  The main thread processes the event block and returns
 *	it by calling WakeTPthreadWithResp.  This routine regains control and
 *	requests timestamp information from the time provider, again passing
 *	this information to the main thread in an ETprovider event block and
 *	waiting.  The main thread processes the timestamp data and again returns
 *	the event block by calling WakeTPthreadWithResp.  This routine regains
 *	control, deallocates the event block and returns, causing the thread to
 *	terminate.
 *
 *	If this routine encounters an error when making either of the two calls
 *	to the time provider, it passes an ETprvdrFail event block to the main
 *	thread.
 *
 *	When the main thread discovers that it is finished making time provider
 *	calls, either because it has successfully finished synchronizing, or
 *	because an error has occured, it will set the release flag in the TP
 *	response information structure attached to the event block before
 *	returning it to the time provider thread.
 *
 *  Implicit Inputs:
 *	TPmutex		is the mutex protecting the TP database.
 *
 *	TPcond		is the condition variable on which we wait for the
 *			main thread to process events.
 *
 *--
 */
pthread_addr_t GetTimeFromProvider (
    pthread_addr_t	argPtr
)
{

    TPctlMsg		*ctlMsgPtr;
    TPtimeMsg		*timeMsgPtr;
    TPrespDsc		*respDscPtr;
    EventBlk		*eblkPtr;
    rpc_binding_handle_t TPbinding;
    int			TPbindingValid;
    error_status_t	 rpcStatus;
    StatusBlk 		statusBlk;
    int			TPcontacted;
#if	defined(DCE_AUD_POINTS)
    error_status_t      audStatus = error_status_ok ;
    dce_aud_rec_t 	ard1 = NULL;
    dce_aud_rec_t 	ard2 = NULL;
#endif	/* defined(DCE_AUD_POINTS) */

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "TP:  thread starting\n"));

    respDscPtr = (TPrespDsc *)argPtr;
    TPcontacted = FALSE;

    /*
     * Allocate an event block
     */
    statusBlk = AllocTPevent (respDscPtr, &eblkPtr);
    if (IsError(statusBlk)) {
	DTSSfree ((VOID *)respDscPtr);
	return ((pthread_addr_t)NULL);
    }

    respDscPtr->respStatus = TPmsgInvalid;
    TPbindingValid = FALSE;

    ctlMsgPtr = DTSSmallocStruct (TPctlMsg);
    /*
     * Bind to the time provider
     */
    rpcStatus = CreateTPbinding (&TPbinding);
    if (rpcStatus != rpc_s_ok)
    {
	respDscPtr->respStatus = TPmsgNoResponse;
	GenTPevent (ETprvdrFail, eblkPtr);
    }
    else {
	TPbindingValid = TRUE;
	respDscPtr->respPtr = (char *)ctlMsgPtr;
	respDscPtr->respType = K_TP_RESP_CONTROL;
#if	defined(DCE_AUD_POINTS)
    	dce_aud_start_with_server_binding(EVT_CONTACT_PROVIDER, 
		TPbinding, 0, aud_c_esl_cond_unknown, &ard1, &audStatus);
    	dce_aud_start_with_server_binding(EVT_REQUEST_PROVIDER_TIME, 
		TPbinding, 0, aud_c_esl_cond_unknown, &ard2, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	/*
	 * Request control information from the time provider through an
	 * RPC call.  If we fail, generate an ETprvdrFail event.
	 * Otherwise, generate an ETprovider event.
	 */
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		 "TP:  Contacting time provider\n"));

	ContactProvider (TPbinding, ctlMsgPtr, &rpcStatus);

	if (rpcStatus == rpc_s_ok)
	{
	    TPcontacted = TRUE;
	    respDscPtr->respStatus = TPmsgValid;
	    GenTPevent (ETprovider, eblkPtr);
#if	defined(DCE_AUD_POINTS)
	    if (ard1)
		dce_aud_commit(dts_aud_trail, ard1, 0, 0,
		    aud_c_esl_cond_success, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	}
	else {
#ifdef	DCE_DEBUG
	    int _i;
	    unsigned char       buf[dce_c_error_string_len];

	    dce_error_inq_text((rpcStatus), buf, &_i);
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		"TP:  RPC error contacting provider, %s", buf));
#endif
	    respDscPtr->respStatus = TPmsgNoResponse;
	    GenTPevent (ETprvdrFail, eblkPtr);
#if	defined(DCE_AUD_POINTS)
	    if (ard1)
		dce_aud_commit(dts_aud_trail, ard1, 0, 0, 
			aud_c_esl_cond_failure, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	}
    }

    /*
     * Wait for the main thread to process the TP control event.
     */
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	 "TP:  Waiting for main to process TP ctrl\n"));
    WaitForEventResp (&TPmutex, &TPcond, eblkPtr);

    /*
     * If the main thread has not released us, then the control information from
     * the time provider must be valid and the synchronization state machine is
     * now waiting for provider timestamps.  Request them from the provider.  If
     * it responds with valid timestamps, generate an ETprovider event,
     * otherwise generate an ETprvdrFail event.  Then, wait for the main thread
     * to process them.
     */
    if (TPcontacted && !respDscPtr->release) {
	respDscPtr->respStatus = TPmsgInvalid;
	timeMsgPtr = DTSSmallocStruct (TPtimeMsg);
	respDscPtr->respPtr = (char *)timeMsgPtr;
	respDscPtr->respType = K_TP_RESP_TIMES;

	DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	    "TP:  Getting times from time provider\n"));

	ServerRequestProviderTime (TPbinding, timeMsgPtr, &rpcStatus);
	if (rpcStatus == rpc_s_ok) {
	    respDscPtr->respStatus = TPmsgValid;
	    GenTPevent (ETprovider, eblkPtr);
#if	defined(DCE_AUD_POINTS)
	    if (ard2)
		dce_aud_commit(dts_aud_trail, ard2, 0, 0,
		    aud_c_esl_cond_success, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	}
	else {
#ifdef	DCE_DEBUG
	    int _i;
	    unsigned char       buf[dce_c_error_string_len];

	    dce_error_inq_text((rpcStatus), buf, &_i);
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		"TP:  RPC error contacting provider, %s", buf));
#endif
	    GenTPevent (ETprvdrFail, eblkPtr);
#if	defined(DCE_AUD_POINTS)
	    if (ard2)
		dce_aud_commit(dts_aud_trail, ard2, 0, 0, 
		    aud_c_esl_cond_failure, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	}

	DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	 "TP:  Waiting for main to process TP data\n"));
	WaitForEventResp (&TPmutex, &TPcond, eblkPtr);

	DTSSfree ((VOID *)timeMsgPtr);
    }

    /*
     * At this point, we're released, whether the timestamps were successfully
     * received by the synchronization state machine or not.  Free the TP event
     * structures and any TP response buffers we used.
     */

    /*
     * release memory used by the RPC binding handle.
     */
    if (TPbindingValid)
	rpc_binding_free (&TPbinding, &rpcStatus);

    /*
     * release other allocated memory.
     */

    if (ctlMsgPtr)
	DTSSfree ((VOID *)ctlMsgPtr);

    FreeTPevent (eblkPtr);

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "TP:  thread terminating\n"));

    return ((pthread_addr_t)NULL);
} /* end of routine GetTimeFromProvider */


/*
 *++
 *  CreateTPbinding()
 *	Create an RPC binding to the time provider program.
 *
 *  Outputs:
 *	binding_h : binding handle return by the RPC binding routines.
 *
 *  Value Returned:
 *	status : status returned by RPC calling routines
 *                  rpc_s_ok if success
 *
 *--
 */

static error_status_t CreateTPbinding (
    rpc_binding_handle_t *binding_h
)
{

    unsigned_char_p_t     strBindName;
    rpc_binding_handle_t  tempBinding;
    rpc_binding_handle_t  bindingVector[10];
    error_status_t        status, tempStatus;
    rpc_if_handle_t       providerInterface;
    rpc_if_id_t           providerInterfaceID;
    unsigned_char_p_t     providerPrincName;
    rpc_ep_inq_handle_t   epContext;
    int                   index;
    int                   i;

    providerPrincName = (unsigned_char_p_t)NULL;

    /*
     * Invoke this macro to select a protocol sequence from the vector
     * of available protseqs on this node. Protocol sequences are registered
     * by the Time Provider daemon, if there is one running on this host.
     */

    SelectStrBinding (status, strBindName);

    if (status != rpc_s_ok)
    {

	dce_error_string_t tempstring;
	dce_svc_printf( DTS_S_SELECT_STRBIND_MSG, 
		       dce_dts_error_text( status, tempstring ));
	return (status);
    }

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
       "Selected binding for TP -> %s\n", strBindName));

    rpc_binding_from_string_binding (strBindName,
				     &tempBinding,
				     &status);

    rpc_string_free (&strBindName, &tempStatus);

    if (status != rpc_s_ok){

	dce_error_string_t tempstring;

	dce_svc_printf( DTS_S_TP_BINDING_MSG, 
		       dce_dts_error_text( status, tempstring ));
	return( status );
    }

    /*
     * Obtain the principal name of the time provider daemon running on
     * this node.  This is done by calling rpc_mgmt_inq_server_princ_name.
     * First we must obtain a fully-bound handle in order for this call
     * to complete.  Obtain all binding handles, if any, that have been
     * registered by the time provider daemon.
     */

    providerInterface = time_provider_v1_0_c_ifspec;
    rpc_if_inq_id (providerInterface, &providerInterfaceID, &status);

    rpc_mgmt_ep_elt_inq_begin (tempBinding,
			       rpc_c_ep_match_by_if,
			       &providerInterfaceID,
			       rpc_c_vers_all,
			       NULL,
			       &epContext,
			       &status);
    if (status != rpc_s_ok){
	dce_error_string_t tempstring;

	dce_svc_printf( DTS_S_TP_EP_BEGIN_MSG, 
		       dce_dts_error_text( status, tempstring ));
	return(status);
    }

    index=0;
    tempStatus = rpc_s_ok;

    while (tempStatus == rpc_s_ok)
    {
	rpc_mgmt_ep_elt_inq_next (epContext,
				  &providerInterfaceID,
				  &bindingVector[index],
				  NULL,
				  NULL,
				  &tempStatus);
	if (tempStatus != rpc_s_ok)
	{
	    if (tempStatus != rpc_s_no_more_elements){

		dce_error_string_t tempstring;

		dce_svc_printf( DTS_S_TP_EP_NEXT_MSG, 
			       dce_dts_error_text( status, tempstring ));
	    }
	    break;
	}
	else
	    index++;
    }

    rpc_mgmt_ep_elt_inq_done (&epContext, &status);

    if (index == 0)
    {
	rpc_binding_free (&tempBinding, &status);
	return(tempStatus);
    }

#ifdef	DCE_DEBUG
    if (DCE_SVC_DEBUG_ATLEAST(dts_svc_handle, dts_s_threads, 1)) {
	for (i = 0; i < index; i++) {
	    SHOW_BINDING(dts_s_threads, "TP fully bound handle from ep_ query",
	         bindingVector[i]);
	}
     }
#endif

    /*
     * Get the principal name of the time provider running on this node, if
     * there is one.
     */

    rpc_mgmt_inq_server_princ_name (bindingVector[0],
			       	    rpc_c_authn_dce_private,
				    &providerPrincName,
				    &status);
    if (status != rpc_s_ok)
    {

	dce_error_string_t tempstring;
	dce_svc_printf( DTS_S_TP_NAME_SEARCH_MSG, 
		       dce_dts_error_text( status, tempstring ));
	goto CLEANUP;
    }

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "TP principal name: %s\n", providerPrincName));

    /*
     * NOTE: if dts is running with datagram protocol only, loop through the
     *       fully-bound handles until we find the datagram compatible
     *       binding.  This will not be the case when DCE security supports
     *       connectionful protocols.
     */

#if DATAGRAM_ONLY
    {
	unsigned_char_p_t fullBoundString;
	unsigned_char_p_t protseq;
	int Selected = 0;

	fullBoundString = (unsigned_char_p_t)NULL;
	protseq = (unsigned_char_p_t)NULL;

	i = 0;
	while((!Selected) && (i<index))
	{
	    rpc_binding_to_string_binding (bindingVector[i],
					   &fullBoundString,
					   &status);
	    if (status == rpc_s_ok)
	    {
		rpc_string_binding_parse (fullBoundString,
					  (unsigned_char_p_t *)NULL,
					  &protseq,
					  (unsigned_char_p_t *)NULL,
					  (unsigned_char_p_t *)NULL,
					  (unsigned_char_p_t *)NULL,
					  &status);

		if (strncmp((char *)protseq, (char *)"ncadg_ip_udp", 12)== 0)
		{
		    rpc_binding_copy (bindingVector[i], binding_h, &status);
		    Selected = 1;
		    SHOW_BINDING(dts_s_threads, "selected datagram handle",
			     *binding_h);
		}
	    }

	    if (protseq != NULL)
		rpc_string_free (&protseq, &tempStatus);
	    if (protseq != NULL)
		rpc_string_free (&fullBoundString, &tempStatus);
	    i++;
	}

    if (!Selected)
	goto CLEANUP;

    }

#else

    /*
     * Copy the first binding in the binding vector if we don't care
     * about a specific protocol.
     */
    rpc_binding_copy (bindingVector[0], binding_h, &status);

#endif  /* DATAGRAM_ONLY */

#if CONFIG_SECURITY
    /*
     * Set rpc binding info into the binding handle we just obtained.
     * If the Time Provider is a legitimate server, it must have registered
     * its authentication services with security and both the DTS daemon
     * and the Time Provider will exhange authenticated RPC messages.
     * The principal name under which the Time Provider process runs is
     * the time machine's principal name.
     */

    rpc_binding_set_auth_info (*binding_h,
			       providerPrincName,
			       rpc_c_authn_level_default,
			       rpc_c_authn_dce_private,
			       (rpc_auth_identity_handle_t)
			       rpcBufPtr->loginContext_h,
			       rpc_c_authz_dce,
			       &status);

    if (status != rpc_s_ok)
    {

	dce_error_string_t tempstring;

	dce_svc_printf( DTS_S_TP_AUTH_INFO_MSG, 
		       dce_dts_error_text( status, tempstring ));
	goto CLEANUP;
    }
#endif /* CONFIG_SECURITY */

CLEANUP: ;

    if (providerPrincName != NULL)
	rpc_string_free (&providerPrincName, &tempStatus);

    rpc_binding_free (&tempBinding, &tempStatus);

    for (i=0; i<index;)
	rpc_binding_free (&bindingVector[i++], &tempStatus);

    return (status);
}


/*
 *++
 *  AllocTPevent()
 *	Allocate and initialize a time provider event block.  Link the
 *	associated time provider response descriptor to the event block.
 *
 *	Currently this routine just allocates from memory, but this routine
 *	could keep some sort of pool to avoid the overhead of memory allocation
 *	and deallocation.
 *
 *	There should be only one active TP request thread at any given time, so
 *	at first glance, it looks like we should need only one event block.
 *      However, if one TP request thread hangs for longer than the TP poll
 *	interval, we'd have overlapping TP threads.  This is not likely, but
 *	since the main thread has a timeout on the TP response, and this
 *	timeout causes the cancellation of the thread, and the threads
 *	package doesn't guarantee WHEN the cancellation will be delivered to a
 *      thread, it's conceivable that the TP thread could still be awaiting a
 *	cancellation delivery when it's time to make another provider request.
 *	Hence, the decision to allocate/deallocate event blocks, rather than
 * 	using a single one.
 *
 *  Inputs:
 *	respDscPtr	is a pointer to the TP response descriptor.
 *
 *  Outputs:
 *	eblkPtr		is a pointer to the event block.  This is NULL
 *			if this routine fails.
 *
 *  Value Returned:
 *	statusBlk.status 	is DTSS_SUCCESS if we succeeded.
 *				is DTSS_ERROR otherwise.
 *
 *	statusBlk.sysStatus	TBD - put some value here to indicate "no
 *				resources" or something similar.
 *
 *--
 */
static StatusBlk AllocTPevent (
    TPrespDsc	*respDscPtr,
    EventBlk	**eblkPtr
)
{
    EventBlk	*eptr;
    StatusBlk	statusBlk;

    statusBlk.status = DTSS_ERROR;
    statusBlk.sysStatus = 0;

    *eblkPtr = NULL;

    eptr = DTSSmallocStruct (EventBlk);
    memset((VOID *)eptr, 0, sizeof(EventBlk));
    eptr->ioDsc.ptr = (unsigned char *)respDscPtr;

    *eblkPtr = eptr;

    statusBlk.status = DTSS_SUCCESS;
    return (statusBlk);
} /* end of routine AllocTPevent */

/*
 *++
 *  FreeTPevent()
 *	Free a time provider event block.  Currently, this routine just
 *	deallocates the event block and its associated TP response descriptor.
 *
 *	TBD - If we implement a pool here, return the event block to the pool.
 *	see the description of AllocTPevent.
 *
 *  Inputs:
 *	eblkPtr			is the TP event block.
 *
 *  Implicit Inputs:
 *	eblkPtr->ioDsc.ptr 	is the associated TP response descriptor.
 *
 *--
 */
static void FreeTPevent (
    EventBlk	*eblkPtr
)
{

    DTSSfree ((VOID *)eblkPtr->ioDsc.ptr);
    DTSSfree ((VOID *)eblkPtr);
} /* end of routine FreeTPevent */

/*
 *++
 *  GenTPevent()
 *	Generate the given time provider event (ETprovider or ETprvdrFail).
 *	Pass it to the main thread to process.
 *
 *  Inputs:
 *	event		the event to generate.
 *
 *	eblkPtr		the event block to pass to the main thread.
 *
 *  Implicit Inputs:
 *	TPeventQ	the queue of time provider events.
 *	TPmutex		the mutex protecting TPeventQ.
 *
 *  Implicit Outputs:
 *	TPeventQ	the event block is placed on this queue, if one can be
 *			allocated.
 *
 *  Side Effects:
 *	The main thread is awakened.
 *
 *--
 */
static void GenTPevent (
    EventTypes	event,
    EventBlk	*eblkPtr
)
{

#ifdef	DCE_DEBUG
    {
	char *p;
	 if (event == ETprovider)
	      p = "ETprovider";
	 else if (event == ETprvdrFail)
	      p = "ETprvdrFail";
	 else
	      p = "BOGUS TP";
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	    "TP:  generating %s event\n", p));
    }
#endif

    eblkPtr->event = event;
    GenThreadEvent (&TPeventQ, &TPmutex, eblkPtr);
} /* end of routine GenTPevent */


void dive (
    int n
)
{
    int i;
    unsigned char foo[255];

    for (i = 0; i <= 255; i++)
	foo[i] = i;

    if (n > 0)
	dive (n-1);
}
