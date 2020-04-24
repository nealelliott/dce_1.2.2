/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: time_request.c,v $
 * Revision 1.1.9.2  1996/02/17  23:35:55  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:52:10  marty]
 *
 * Revision 1.1.9.1  1995/12/08  18:14:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:26:19  root]
 * 
 * Revision 1.1.7.9  1994/09/23  20:00:19  tom
 * 	Back out change of unsigned/signed64 to typedef hyper
 * 	[1994/09/23  19:27:02  tom]
 * 
 * Revision 1.1.7.7  1994/08/16  18:13:14  cbrooks
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:31  cbrooks]
 * 
 * Revision 1.1.7.6  1994/07/27  17:53:49  cbrooks
 * 	CR11154 timer thread expires before synch thread
 * 	[1994/07/20  21:11:58  cbrooks]
 * 
 * Revision 1.1.7.5  1994/06/30  19:45:35  cbrooks
 * 	CR11112 - new serviceability messages required
 * 	[1994/06/28  17:49:02  cbrooks]
 * 
 * Revision 1.1.7.4  1994/05/16  18:52:05  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:40  rhw]
 * 
 * Revision 1.1.7.3  1994/04/28  21:09:02  cbrooks
 * 	DTS Code Cleanup
 * 	[1994/04/28  17:35:26  cbrooks]
 * 
 * Revision 1.1.7.2  1994/04/21  22:06:18  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:00:10  cbrooks]
 * 
 * Revision 1.1.7.1  1994/01/25  17:16:55  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/25  17:15:52  rsalz]
 * 
 * Revision 1.1.5.2  1993/07/28  17:07:22  tom
 * 	Bug 8373 - If timestamp versions don't match up in ProcessTimeResponseRPC,
 * 	get the node name correctly and check for failures.
 * 	[1993/07/28  17:06:39  tom]
 * 
 * Revision 1.1.2.2  1992/12/30  16:40:10  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:25:04  htf]
 * 
 * Revision 1.1  1992/01/19  15:34:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module TIME_REQUEST.C
 *
 * Copyright (c) 1990, 1991 by
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
 *	This module contains routines used to request times from servers using
 *	both RPC and message-based transports.
 *
 * ENVIRONMENT:	portable
 *
 * AUTHORS:
 *		Brian Bailey
 *		Distributed Processing Engineering (DPE)
 *
 * DATE:	October 1990
 *
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

/*
 * Macros
 */

/*
 * External variables
 */

extern Sdb sdb;			/* synchronization database */

extern ShmBuf *shmPtr;		/* shared state area */

extern PortDsc *rpcPortPtr;	/* pointer to the RPC port descriptor */

/*
 * Internal prototypes
 */
static ResponseElement *ProcessTimeResponseRPC (EventBlk *);

static void RequestTimeRPC (ResponseElement *,
			    ServerSetType);


pthread_addr_t GetTimeFromServerRPC (pthread_addr_t);

static StatusBlk AllocTimeReqEvent (TimeRequestDsc *,
				    EventBlk **);

static void AddDiffEpochElement(epoch_t , ResponseElement *);

void RPC_ClerkRequestTime ( handle_t , struct utc *, idl_ulong_int *, 
			   error_status_t *) ;

void RPC_ServerRequestTime ( handle_t , struct utc *, idl_ulong_int *,
			    idl_long_int *, idl_long_int *, error_status_t *);

void RPC_ClerkRequestGlobalTime ( handle_t , struct utc *, idl_ulong_int *,
				 error_status_t *);

void RPC_ServerRequestGlobalTime ( handle_t , struct utc *, idl_ulong_int *,
				  idl_long_int *, error_status_t *);


/*
 *++
 *  RequestTimeFromServers ()
 *	Request times from servers in the given server set (local or global).
 *
 *  Inputs:
 *	setPtr		is the server set.
 *      servSetType     server set type (SRVlocal or SRVglobal)
 *
 *--
 */
void RequestTimeFromServers (
    ServerSet	*setP,
    ServerSetType   servSetType
)
{
    ResponseElement     *rp;

    QloopBegin (&setP->respQ, &rp) {
	if (!rp->responseRecvd) {
            /*
             * msgIdSerial is unsigned: let it wrap
             */
            rp->msgIdSerial = sdb.msgIdSerial++;

            if (rp->serverPtr->portPtr->socketType == K_SOCKET_DTS_RPC)
		RequestTimeRPC (rp, servSetType);
	    }
	}
    QloopEnd
} /* end of routine RequestTimeFromServers */

/*
 *++
 *  ProcessTimeResponse ()
 *	find the corresponding element in the response queue and copy
 *	the response values from the message into it.
 *
 *  Inputs:
 *	reqPtr		is a pointer to the time request/response descriptor,
 *			which contains the time information and request
 *			serial number.
 *
 *  Implicit Inputs:
 *	response queue
 *
 *  Implicit Outputs:
 *	time values stored in the response queue element
 *
 *  Value Returned:
 *	ReponseElement *if success, NULL if failed to find corresponding
 *	response element. We can fail to find a corresponding reponse
 *	element if this is a late response to a request that has been
 *	retransmitted.  Only the most recent request is kept, so that
 *	the round-trip timer will be correct.
 *
 *--
 */
ResponseElement *ProcessTimeResponse (
    EventBlk *eblkPtr
)
{

    if (eblkPtr->event == ETtimeRespRPC)
	return(ProcessTimeResponseRPC (eblkPtr));
    else
	BugCheck();
    /*NOTREACHED*/
} /* end of routine ProcessTimeResponse */

/*
 *++
 *  ProcessTimeResponseRPC ()
 *	find the corresponding element in the response queue and copy
 *	the response values from the message into it.
 *
 *  Inputs:
 *	eblkPtr
 *
 *  Implicit Inputs:
 *	response queue
 *
 *  Implicit Outputs:
 *	time values stored in the response queue element
 *
 *  Value Returned:
 *	ReponseElement *if success, NULL if failed to find corresponding
 *	response element. We can fail to find a corresponding reponse
 *	element if this is a late response to a request that has been
 *	retransmitted.  Only the most recent request is kept, so that
 *	the round-trip timer will be correct.
 *
 *--
 */
static ResponseElement *ProcessTimeResponseRPC (
    EventBlk *eblkPtr
)
{
    TimeRequestDsc	*reqPtr;
    ResponseElement	*rp;
    ServerName		server;
    unsigned_char_p_t	servername;
    error_status_t 	status;

    reqPtr = (TimeRequestDsc *)eblkPtr->ioDsc.ptr;

    if (!(rp = FindResponseElement (reqPtr->synchSerialNo,
				    reqPtr->msgIdSerial)))
	return ((ResponseElement *)NULL);

    /*
     * Check epoch, log if different
     */

    if (IsServer() && (reqPtr->epochNumber != sdb.mgtParams.epochNumber)) {
	AddDiffEpochElement (reqPtr->epochNumber, rp);
	return ((ResponseElement *)NULL);
    }

    /*
     * Check version of incoming timestamp, log if different
     */

    if (utc_bintime( (timespec_t *)NULL, (timespec_t *)NULL, (long *)NULL,
                     &reqPtr->serverTime))
	{

        rpc_mgmt_inq_server_princ_name (rp->serverPtr->bindPtr->handle,
				       rpc_c_authn_default,
				       &servername,
				       &status);

	if ((status != rpc_s_ok) || (servername == NULL)) {
	    strcpy(server.serverName.serverTextName, "<unknown>");
	} else {
            strncpy(server.serverName.serverTextName, 
		    (char *)servername, 
		    K_MAX_SERVER_TEXT_NAME - 1);
	    rpc_string_free (&servername, &status);
	}

        server.selector = SNtext;
 	LogEventTimeRep( &server );

	}

    /*
     * Put response info from the request descriptor into the response element.
     */
    rp->responseRecvd = TRUE;
    rp->sendTime = reqPtr->sendTime;
    rp->serverTime = reqPtr->serverTime;
    rp->processingDelay = reqPtr->processingDelay;
    rp->recvTime = reqPtr->recvTime;
    if (reqPtr->servSetType == SRVlocal)
	rp->serverPtr->courierRole = reqPtr->courierRole;

#ifdef	DCE_DEBUG
    if (DCE_SVC_DEBUG_ATLEAST(dts_svc_handle, dts_s_msgs, 1))
	 PrintResponseTimes(rp);
#endif

    return (rp);
} /* end of routine ProcessTimeResponseRPC */

/*
 * FindResponseElement ()
 *
 * 	Searches the local and global set response queues for the response
 *	element corresponding to the given request descriptor.  Called by
 *	ProcessTimeResponseRPC and ProcessTimeResponseMsg.
 *
 * Inputs:
 *	reqPtr		is the time request descriptor that contains the
 *			request and response information.
 *
 * Value Returned:
 *      pointer to the server's response element if found, else NULL
 *
 */

ResponseElement *FindResponseElement (
    unsigned int	synchSerialNo,
    unsigned int	msgIdSerial
)
{
    ResponseElement *rp;

    /*
     * First make sure that this is a response to the current synchronization
     */
    if (synchSerialNo != sdb.synchSerialNo) {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_msgs, svc_c_debug1,
	   "Main:  Wrong synch no: %d, expected %d\n",
	   synchSerialNo, sdb.synchSerialNo));
	return ((ResponseElement *)NULL);
    }

    /*
     * Look in the local set
     */
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "Main:  Searching local set for response\n"));
    QloopBegin (&sdb.localSet.respQ, &rp) {
	if (msgIdSerial == rp->msgIdSerial) {
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		   "Main:  Found response in local set\n"));
	    return (rp);
	}
    }
    QloopEnd;

    /*
     * Not in the local set, look in the global set
     */
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "Main:  Searching global set for response\n"));
    QloopBegin (&sdb.globalSet.respQ, &rp) {
	if (msgIdSerial == rp->msgIdSerial) {
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		"Main:  Found response in global set\n"));
	    return (rp);
	}
    }
    QloopEnd;

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "Main:  Failed to find response\n"));
    return ( (ResponseElement *) NULL ); /* Didn't find it */
} /* end of routine FindResponseElement */

/*
 *++
 *  AddDiffEpochElement ()
 *  	Adds a differing epoch event logging element onto the sdb bad epoch
 *  	queue for a server reporting an epoch different than ours.
 *
 *  Inputs:
 *      epoch	the epoch reported by the server.
 *
 *	rp	the server's response element.
 *
 *  Implicit Inputs:
 *	sdb, local/global/badepoch server cache
 *--
 */
static void AddDiffEpochElement (
    epoch_t		epoch,
    ResponseElement	*rp
)
{
    BadEpochElement *bePtr;

    /*
     * Make sure we only add it once to the list
     */
    QloopBegin ( &sdb.epochSet.serverQ, &bePtr ) {
	if ( bePtr->serverId == rp ) {
	    return;
	}
    }
    QloopEnd;

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "Main:  Found different epoch\n"));
    /*
     * allocate some memory and place the element into
     * the bad epoch queue.
     */
    bePtr = DTSSmallocStruct (BadEpochElement);
    bePtr->serverId = rp;
    bePtr->epochNumber= epoch;

    /*
     * fill in the server name, either it is a full name, a text
     * string, an RPC binding string, or an ID802.
     */

    if (rp->serverPtr->nodeFnamePtr != NULL)
    {
    }
    else if (rp->serverPtr->nodeNamePtr != NULL)
    {
	strncpy(bePtr->server.serverName.serverTextName,
		rp->serverPtr->nodeNamePtr,
		K_MAX_SERVER_TEXT_NAME - 1);
	bePtr->server.serverName.serverTextName[K_MAX_SERVER_TEXT_NAME]='\0';
	bePtr->server.selector = SNtext;
    }
    else
    {
	bePtr->server.serverName.serverNodeName = rp->serverPtr->nodeAddr;
	bePtr->server.selector = SN802;
    }

    QinsertLast( &sdb.epochSet.serverQ, bePtr );
} /* end of routine AddDiffEpochElement */
/*
 *++
 *  PrintResponseTimes ()
 *	Prints the times in a response element. Called for tracing when
 * 	a time response message arrives.
 *  Inputs:
 *	rp - pointer to a response element
 *  Side Effects:
 *	prints
 *--
 */

void PrintResponseTimes (
    ResponseElement *rp
)
{
    char timestr[80];
    UTCValue roundTrip;
    UTCValue utcTime, utcInacc;

    UTCsub (&rp->recvTime, &rp->sendTime, &roundTrip);
    (void) utc_comptime(&utcTime, &utcInacc, (long *)NULL, &rp->serverTime);
    utc_ascgmtime(timestr,80,&rp->serverTime);

#if LONG_BIT < 64
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_msgs, svc_c_debug1,
	"\tsrv's time %s, inacc %u %lu,\n\tproc delay %lu %lu, round-trip %lu %lu\n",
	timestr,
	(int) utcInacc.hi, utcInacc.lo,
	rp->processingDelay.hi, rp->processingDelay.lo,
	roundTrip.hi, roundTrip.lo));
#else
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_msgs, svc_c_debug1,
        "\tsrv's time %s, inacc %lu,\n\tproc delay %lu, round-trip %lu\n",
        timestr,
        utcInacc,
        rp->processingDelay,
        roundTrip));
#endif
}

/*
 *++
 *  RequestTimeRPC ()
 *	Create a thread to request time from a DTSS server using RPC.
 *
 *  Inputs:
 *	rp	     is the response element for this server.
 *      servSetType  server set type (SRVlocal or SRVglobal)
 *
 *--
 */
static void RequestTimeRPC (
    ResponseElement     *rp,
    ServerSetType       setType 
)
{
    TimeRequestDsc	*reqPtr;
    extern pthread_attr_t thread_attribute;
    /*
     * Allocate and fill in a descriptor for this request.
     */

    reqPtr = DTSSmallocStruct (TimeRequestDsc);
    memset((VOID *)reqPtr, 0, sizeof(TimeRequestDsc));
    reqPtr->synchSerialNo = sdb.synchSerialNo;
    reqPtr->msgIdSerial = rp->msgIdSerial;
    reqPtr->servSetType = setType;
    reqPtr->isServer = IsServer();
    reqPtr->bindPtr = rp->serverPtr->bindPtr;

    /*
     * Acquire the binding on behalf of the time request thread.  This ensures
     * that the binding can't be deallocated before the time request thread
     * is finished using it.
     */
    AcquireServerBinding (reqPtr->bindPtr);

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
       "Main:  Creating thread to request time from %s\n",
       rp->serverPtr->nodeNamePtr));

    /*
     * Create the thread to request time from the server
     */
    if (pthread_create (&rp->requestThread,
			thread_attribute,
			GetTimeFromServerRPC,
			(pthread_addr_t)reqPtr) < 0) {
	BugCheckOSstatus (errno);
	/* DTSSfree ((VOID *)reqPtr); */
    }
    else
	rp->threadValid = TRUE;
} /* end of routine RequestTimeRPC */

/*
 *++
 *  ReleaseTimeReqThread ()
 *	Release the time request thread for a given server, if it is in use.
 *
 *  Inputs:
 *	rp	is the response element for this server.
 *
 *  Implicit Outputs:
 *	rp->threadValid gets set to FALSE.
 *
 *  Side Effects:
 *	the thread is detached
 *
 *--
 */
void ReleaseTimeReqThread (
    ResponseElement     *rp
)
{

    if (rp->threadValid) {

	DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	    "Main:  Releasing time request thread\n"));

	if (pthread_detach (&rp->requestThread) < 0) {
	    BugCheckOSstatus (errno);
	}

    }

    rp->threadValid = FALSE;
} /* end of routine ReleaseTimeReqThread */

/*
 *++
 *  GetTimeFromServerRPC ()
 *	Request time from a DTSS server using RPC
 *
 *  Inputs:
 *	argPtr		is the descriptor for this time request.
 *
 *--
 */
pthread_addr_t GetTimeFromServerRPC (
    pthread_addr_t	argPtr
)
{
    TimeRequestDsc	*reqPtr;
    ServerBindingDsc	*bindPtr;
    EventBlk		*eblkPtr;
    idl_ulong_int	processingDelay;
    idl_long_int	response_epoch;
    error_status_t	rpcStatus;
    StatusBlk		statusBlk;
    struct utc          timeNow;

    reqPtr = (TimeRequestDsc *)argPtr;

    statusBlk = AllocTimeReqEvent (reqPtr, &eblkPtr);
    if (IsError (statusBlk)) {
	DTSSfree ((VOID *)reqPtr);
	return ((pthread_addr_t)NULL);
    }

    bindPtr = reqPtr->bindPtr;

    SHOW_BINDING(dts_s_threads, "Sending Time Request to ",
	 bindPtr->handle);

    /*
     * Take a time stamp right before we send a time request
     * to the server.
     */
    (void)utc_gettime (&timeNow);
    (void)utc_comptime (&reqPtr->sendTime, (UTCValue *)NULL,
			(long *)NULL, &timeNow);

    if (reqPtr->isServer)
    {
	if (reqPtr->servSetType == SRVlocal)
	    ServerRequestTime (bindPtr->handle,
			       &reqPtr->serverTime,
			       &processingDelay,
			       &response_epoch,
			       &reqPtr->courierRole,
			       &rpcStatus);
	else  /* SRVglobal */
	    ServerRequestGlobalTime (bindPtr->handle,
				     &reqPtr->serverTime,
				     &processingDelay,
			             &response_epoch,
				     &rpcStatus);
    }

    else
    {
	if (reqPtr->servSetType == SRVlocal)
	    ClerkRequestTime (bindPtr->handle,
			      &reqPtr->serverTime,
			      &processingDelay,
			      &rpcStatus);
	else  /* SRVglobal */
	    ClerkRequestGlobalTime (bindPtr->handle,
				    &reqPtr->serverTime,
				    &processingDelay,
				    &rpcStatus);
    }

    /*
     * Take a time stamp right after the RPC time response.
     */
    (void)utc_gettime (&timeNow);
    (void)utc_comptime (&reqPtr->recvTime, (UTCValue *)NULL,
			(long *)NULL, &timeNow);

    /*
     * Copy epoch to data structure
     */
    reqPtr->epochNumber = response_epoch;

#ifdef	DCE_DEBUG
    if (rpcStatus == rpc_s_ok) {
	char buf[UTC_MAX_STR_LEN];

	(void)utc_ascgmtime (buf, sizeof buf, &reqPtr->serverTime);
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	    "Times from RPC: %s\n", buf));
    }
#endif

  /*
     * Check to see if the server is still on the response queue.
     * The server could have been removed due to the timer expiring.
     * If the server is no longer on the response queue then we
     * don't have to do a ReleaseServerBinding.
     */

    if (FindResponseElement(reqPtr->synchSerialNo, reqPtr->msgIdSerial))
       ReleaseServerBinding (bindPtr);

    if (rpcStatus != rpc_s_ok) {
	/*
	 * TBD - Do we try to recover here if we get a "wrong boot time" error?
	 */
        dce_error_string_t tempString ;
	dce_svc_printf( DTS_S_CLERK_REQUEST_MSG, 
		       dce_dts_error_text( rpcStatus, tempString ));

        FreeTimeReqEvent (eblkPtr);
        return ((pthread_addr_t)NULL);
    }

    /*
     * Processing Delay is returned from the RPC call as a 32-bit number of
     * nanoseconds.  We make our time calculations using a processing delay
     * represented as a 64-bit UTC value in 100's of nanoseconds, so we'll do
     * the conversion here.
     */
    UTCassign (&(reqPtr->processingDelay),(processingDelay/100),0);

    GenMsgEvent (ETtimeRespRPC, eblkPtr);

    return ((pthread_addr_t)NULL);
} /* end of routine GetTimeFromServerRPC */

/*
 *++
 *  AllocTimeReqEvent()
 *	Allocate and initialize a time request event block.  Link the given
 *	time request descriptor to it.
 *
 *	Currently this routine just allocates them from memory, but this routine
 *	could keep some sort of pool to avoid the overhead of memory allocation
 *	and deallocation.
 *
 *  Inputs:
 *	reqPtr	is a pointer to the time request descriptor.
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
static StatusBlk AllocTimeReqEvent (
    TimeRequestDsc	*reqPtr,
    EventBlk	**eblkPtr
)
{
    EventBlk	*eptr;
    StatusBlk	statusBlk;

    *eblkPtr = NULL;
    statusBlk.status = DTSS_ERROR;
    statusBlk.sysStatus = 0;

    eptr = DTSSmallocStruct (EventBlk);
    memset((VOID *)eptr, 0, sizeof(EventBlk));
    eptr->ioDsc.ptr = (unsigned char *)reqPtr;
    eptr->ioDsc.portPtr = rpcPortPtr;

    *eblkPtr = eptr;

    statusBlk.status = DTSS_SUCCESS;
    return (statusBlk);
} /* end of routine AllocTimeReqEvent */

/*
 *++
 *  FreeTimeReqEvent()
 *	Free a time request event block.  Currently, this routine just
 *	deallocates the event block and its associated time request descriptor.
 *
 *	TBD - If we implement a pool here, return the event block to the pool.
 *	see the description of AllocTimeReqevent.
 *
 *  Inputs:
 *	eblkPtr			is the event block.
 *
 *  Implicit Inputs:
 *	eblkPtr->ioDsc.ptr 	is the associated descriptor.
 *
 *--
 */

void FreeTimeReqEvent (
    EventBlk	*eblkPtr
)
{

    DTSSfree ((VOID *)eblkPtr->ioDsc.ptr);
    DTSSfree ((VOID *)eblkPtr);
} /* end of routine FreeTimeReqEvent */


/*
 *++
 *  RPC_ClerkRequestTime ()
 *	The RPC runtime calls this routine when it receives an RPC time request
 *	from a clerk (when that clerk calls ClerkRequestTime).  It
 *	returns the current time on this server and other information.
 *
 *  Inputs:
 *	bind_h		is the RPC binding handle
 *
 *  Outputs:
 *	timeReq		is the current time on this server.
 *
 *	procDelay	is the bound on the time needed to process the
 *			time request.  Currently this is zero, since we don't
 *			know the length of time this request spent in the RPC
 *			runtime routines.
 *
 *	status		is the status of the request.  Currently this is always
 *			rpc_s_ok.
 *
 *--
 */

void RPC_ClerkRequestTime (
    handle_t       bind_h,
    struct utc     *timeRequest,
    idl_ulong_int  *processDelay,
    error_status_t *status
)
{

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	"\n-> Local Server handling time request from clerk <-\n\n"));

    /*
     * It is not currently possible to calculate the processing delay,
     * when time requests are sent over RPC.  We return 0 for now.
     */

    *processDelay = 0L;

    /*
     * Get the current UTC time and return it to the clerk.
     */
    utc_gettime (timeRequest);

    /*
     * Return success to the client.  This acknowledges that the
     * server was successfully contacted.
     */

    *status = rpc_s_ok;
} /* end of RPC_ClerkRequestTime */


/*
 *++
 *  RPC_ServerRequestTime()
 *	The RPC runtime calls this routine when it receives an RPC time request
 *	from another server (when that server calls ServerRequestTime).  It
 *	returns the current time on this server and other information.
 *
 *  Inputs:
 *	bind_h		is the RPC binding handle
 *
 *  Implicit Inputs:
 *	shmPtr->mgtParams.epochNumber
 *	shmPtr->mgtParams.courierRole
 *
 *  Outputs:
 *	timeReq		is the current time on this server.
 *
 *	procDelay	is the bound on the time needed to process the
 *			time request.  Currently this is zero, since we don't
 *			know the length of time this request spent in the RPC
 *			runtime routines.
 *
 *	epoch		is the epoch of this server.
 *
 *	courierRole	is the courier role of this server.
 *
 *	status		is the status of the request.  Currently this is always
 *			rpc_s_ok.
 *
 *--
 */

void RPC_ServerRequestTime (
    handle_t         bind_h,
    struct utc      *timeReq,
    idl_ulong_int   *procDelay,
    idl_long_int    *epoch,
    idl_long_int    *courierRole,
    error_status_t  *status
)
{
    epoch_t  epoch1, epoch2;
    long     courierRole1, courierRole2;
    int      valuesDiff;

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
      "\n-> Local Server handling time request from server <-\n\n"));

/*
 *
 * Do a poor man's lock:
 * We read both the epoch number and the courier role values twice and check
 * for consistency.  If the values are different, read them again.  This is
 * done to shield against the case of "word tearing" by an interrupting
 * management command in a different thread that would change (perhaps
 * partially) the contents of either of these two attributes.
 *
 */
    do
    {
 	epoch1 = shmPtr->mgtParams.epochNumber;
 	epoch2 = shmPtr->mgtParams.epochNumber;
	valuesDiff = (epoch1 == epoch2) ? 0 : 1;
    }while (valuesDiff);

    *epoch = epoch1;

    do
    {
	courierRole1 = shmPtr->mgtParams.courierRole;
	courierRole2 = shmPtr->mgtParams.courierRole;
	valuesDiff = (courierRole1 == courierRole2) ? 0 : 1;
    }while (valuesDiff);

    *courierRole = courierRole1;

    /*
     * It is not currently possible to calculate the processing delay,
     * when time requests are sent over RPC.  We return 0 for now.
     */
    *procDelay = 0L;

    /*
     * Get the current UTC time to be returned to the requesting server.
     * Do it as late in the process as possible.
     */

    utc_gettime( timeReq );

    *status = rpc_s_ok;
} /* end of RPC_ServerRequestTime */


/*
 *++
 *  RPC_ClerkRequestGlobalTime ()
 *	The RPC runtime calls this routine when it receives an RPC time request
 *	from a clerk (when that clerk calls ClerkRequestTime) to get the time
 *      from a global server. This server has advertised itself as a member of
 *      the global set. The server returns the current time and other
 *      information in the argument list.
 *
 *  Inputs:
 *	bind_h		is the RPC binding handle
 *
 *  Outputs:
 *	timeReq		is the current time on this server.
 *
 *	procDelay	is the bound on the time needed to process the
 *			time request.  Currently this is zero, since we don't
 *			know the length of time this request spent in the RPC
 *			runtime routines.
 *
 *	status		is the status of the request.  Currently this is always
 *			rpc_s_ok.
 *
 *--
 */

void RPC_ClerkRequestGlobalTime (
    handle_t       bind_h,
    struct utc     *timeRequest,
    idl_ulong_int  *processDelay,
    error_status_t *status
)
{

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	"\n-> Gbl Server handling time request from clerk  <-\n\n"));

    /*
     * It is not currently possible to calculate the processing delay,
     * when time requests are sent over RPC.  We return 0 for now.
     */

    *processDelay = 0L;

    /*
     * Get the current UTC time and return it to the clerk.
     */
    utc_gettime (timeRequest);

    /*
     * Return success to the client.  This acknowledges that the
     * server was successfully contacted.
     */

    *status = rpc_s_ok;
} /* end of RPC_ClerkRequestGlobalTime */


/*
 *++
 *  RPC_ServerRequestGlobalTime()
 *	The RPC runtime calls this routine when it receives an RPC time request
 *	from another server (when that server calls ServerRequestTime) to get
 *      the time from a global server.  This server has advertised itself as
 *      a member of the global set.  The server returns the current time
 *      and other information in the argument list.
 *
 *  Inputs:
 *	bind_h		is the RPC binding handle
 *
 *  Implicit Inputs:
 *	shmPtr->mgtParams.epochNumber
 *
 *  Outputs:
 *	timeReq		is the current time on this server.
 *
 *	procDelay	is the bound on the time needed to process the
 *			time request.  Currently this is zero, since we don't
 *			know the length of time this request spent in the RPC
 *			runtime routines.
 *
 *	epoch		is the epoch of this server.
 *
 *	status		is the status of the request.  Currently this is always
 *			rpc_s_ok.
 *
 *--
 */

void RPC_ServerRequestGlobalTime (
    handle_t         bind_h,
    struct utc      *timeReq,
    idl_ulong_int   *procDelay,
    idl_long_int    *epoch,
    error_status_t  *status
)
{
    epoch_t  epoch1, epoch2;
    int      valuesDiff;

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	"\n-> Gbl Server handling time request from server <-\n\n"));

/*
 *
 * Do a poor man's lock:
 * We read both the epoch number and the courier role values twice and check
 * for consistency.  If the values are different, read them again.  This is
 * done to shield against the case of "word tearing" by an interrupting
 * management command in a different thread that would change (perhaps
 * partially) the contents of either of these two attributes.
 *
 */
    do
    {
 	epoch1 = shmPtr->mgtParams.epochNumber;
 	epoch2 = shmPtr->mgtParams.epochNumber;
	valuesDiff = (epoch1 == epoch2) ? 0 : 1;
    }while (valuesDiff);

    *epoch = epoch1;

    /*
     * It is not currently possible to calculate the processing delay,
     * when time requests are sent over RPC.  We return 0 for now.
     */
    *procDelay = 0L;

    /*
     * Get the current UTC time to be returned to the requesting server.
     * Do it as late in the process as possible.
     */

    utc_gettime( timeReq );

    *status = rpc_s_ok;
} /* end of RPC_ServerRequestGlobalTime */

/*
 *
 * Entry Point Vector (EPV) of dispatch routines for the procedures
 * above.  This EPV contains the address of the functions that will
 * be executed on an RPC call.
 *
 * NOTE:
 *
 * The names of the functions declared above and in the EPV differ from
 * the definitions in the IDL file. The names in this module are preceded
 * with the RPC_ prefix.  This is done in order to successfully build an RPC
 * server application that will also be a client. The stubs declare these
 * procedures with the names they are given in the IDL file. Therefore, in this
 * module, we make the names different so the linker does not get confused and
 * generate "multiple declaration" errors.  RPC uses the EPV as a dispatcher
 * table to the procedures invoked, but the caller ( a client) invokes the
 * routine with the name declared in the IDL file (dtsstime_req.idl).
 *
 */

/*
 * EPV for local set servers.
 */

globaldef time_service_v1_0_epv_t time_service_epv =
{
    RPC_ClerkRequestTime,
    RPC_ServerRequestTime
};

/*
 * EPV for global set servers.
 */

globaldef gbl_time_service_v1_0_epv_t gbl_time_service_epv =
{
    RPC_ClerkRequestGlobalTime,
    RPC_ServerRequestGlobalTime
};
