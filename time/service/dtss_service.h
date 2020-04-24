
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss_service.h,v $
 * Revision 1.1.6.2  1996/02/18  23:12:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:37:25  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:13:45  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/26  19:24 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:25:48  root]
 * 
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/18  12:41:36  pare]
 * 
 * Revision 1.1.4.5  1994/08/23  20:32:25  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:57:00  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:53:00  cbrooks]
 * 
 * Revision 1.1.4.4  1994/08/16  18:13:02  cbrooks
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:16  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  15:20 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDC01 into HPDCE02
 * 
 * Revision 1.1.4.3  1994/05/16  18:51:46  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:19  rhw]
 * 
 * Revision 1.1.4.2  1994/04/28  21:08:44  cbrooks
 * 	DTS Code Cleanup
 * 	[1994/04/27  21:26:00  cbrooks]
 * 
 * Revision 1.1.4.1  1994/04/21  22:06:00  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:59:45  cbrooks]
 * 
 * 	Checkpoint:4.15.94
 * 	[1994/04/15  21:14:37  cbrooks]
 * 
 * 	Checkpoint
 * 	[1994/03/28  20:28:32  cbrooks]
 * 
 * Revision 1.1.6.3  1993/09/15  17:48:31  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.5.2  1993/07/08  20:16:58  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:16:26  root]
 * 
 * Revision 1.1.3.2  1993/06/09  14:31:24  truitt
 * 	Oops.  Make sure prototype for InitGlobalSet() is known.
 * 	[1993/06/09  14:30:36  truitt]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/30  19:22:44  htf]
 * 
 * Revision 1.1.1.10  1993/05/04  22:40:07  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1  1992/01/19  15:34:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module DTSS_SERVICE.H
 */
/*
 * Copyright (c) 1989, 1990 by
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
 * ABSTRACT:    This is the generic header file for the DTSS_SERVICE module.
 *
 * ENVIRONMENT:	portable
 *
 * AUTHORS:
 *		Wick Nichols
 *		Distributed Processing Engineering (DPE)
 *
 * DATE:	January 1989
 *
 */
#ifndef _DTSS_SERVICE_H
#define _DTSS_SERVICE_H 1

#include <dtss_attr_ranges.h>
/*
 *  performance logging
 */
#if CONFIG_LOGPT
#include <logdts.h>       
#endif


#include <dtss_rpc.h>
#include <dtsprovider.h>   /* Time Provider Interface definitions */
#include <dtsstime_req.h>  /* Time Service RPC interface definitions for */
                           /* servers in the local set.                  */
#include <dtss_globaltime_req.h> /* Time Service RPC interface definitions */
                                 /* for servers in the global set          */
#include <lan_services.h>        /* LAN services interface UUID */


/*
 *  Define time outs for time provider queries.
 *  All values are in seconds.
 */
#define	 K_MAX_TP_POLL	(31*24*60*60) /* maximum 31 days to poll tp          */	
#define	 K_MIN_TP_POLL  (1)        /* minimum 1 second between TP polls      */
#define  K_MAX_TP_TMO   (5*60)     /* maximum 5 minutes to wait for TP resp. */
#define  K_MIN_TP_TMO   (1)	   /* minimum 1 second to wait for TP resp.  */	

#define	 K_DLI_READS_PER_PORT	(5)

/*
 * Define the state machine input stimuli.
 * Note that the array nextState is initialized according to this
 * ordering in DTSS_SERVICE_STATE.C.
 */
typedef enum INstimuli
{
    INrcvdTimeResponse,		/* received time response pkt		*/
    INrcvdSolicitResponse,	/* received solicit response pkt	*/
    INsynchTimer,		/* time for next synchronization	*/
    INqueryTimeout,		/* some query request timed out		*/
    INrcvdTPresponse,		/* received time provider response	*/
    INlinkReady,		/* an active link has been accepted	*/
    INstimuliNumber		/* # of possible input stimuli		*/
} INstimuli;

/*
 * Define the event types returned by NextEvent()
 */
typedef enum EventTypes
{
    ETreceivedMsg,              /* A net msg arrived from another srvc */

    ETtimeRespRPC,		/* Recv'd response to RPC time request */

    ETcontrol,                  /* A mbx msg arrived from ctrl prog    */
    ETdtsNCL,			/* A mgmt msg arrived from NCL	       */
    ETprovider,                 /* A mbx msg arrived from time provider*/
#if CONFIG_THREADS
    ETprvdrFail,		/* Failed to get response from provider*/
#endif
    ETsynchronize,              /* Time to synchronize now             */
    ETadvertise,                /* Time to advertise now               */
    ETtdfChange,                /* Time to change TDF now              */
    ETqueryTimeout,             /* A query has timed out               */
    ETautoTDF,			/* Time to change TDF now              */
    ETterminate,                /* Stop DTSS Service                   */
    ETupdateTimeDone,		/* Sched. next synch & clr update flag */
    ETdliError,                 /* Detected a dli error                */
    ETdliRestart,               /* Time to recover a dli error         */
    ETlinkReady,		/* Link to global server is ready      */
    ETeventNumber		/* number of event types enumerated    */
} EventTypes;


/*
 * The event block is the primary data structure of the DTSS scheduler:
 * nextEvent(). nextEvent returns an event block each time it is called.
 * The event block has all the information describing the next event to
 * be processed.
 */
typedef struct EventBlk
{
    struct IntQue Que;       	/* For queuing on interlocked queues   */
                                /* with IntQue... routines             */
    UTCValue UTCtime;           /* Time stamp added by interrupt rtn   */
    EventTypes event;           /* Event type, one of eventTypes       */

#if CONFIG_THREADS
    volatile int busy;		/* TRUE while event is being processed */
#endif

    IOdsc ioDsc;            	/* Buffer descriptor for I/O routines  */
} EventBlk;

typedef struct eblk_list_t
{
    EventBlk *eblkptr;
    char     *next_p;
} eblk_list_t;


/*
 * Timer indices
 */                                        
#define K_TIMER_NEXT_SYNCH	0       /* time to synchronize */
#define K_TIMER_ADVERTISE	1       /* time to advertize */
#define K_TIMER_QUERY_TMO	2       /* current query timed out */

#define K_NUM_TIMERS 		3

#define K_MAX_SERVER_TEXT_NAME 100
/*
 * InitiateRandomTimer arguments
 */
typedef enum
{
    RThalfTo1,
    RT34To54
} RandomTimerFactor;


/*
 * Storage definitions for the service
 */


/*
 * ServerBindingDsc - Server RPC binding descriptor
 *
 *	This structure describes an active RPC binding to a remote DTSS server.
 *	It is linked to the cache element for the server and created when the
 *	cache element is created in AddServer.
 *
 *	A time request thread references the binding while making its RPC call.
 *	The main thread references the descriptor during the life of the server
 *	cache element.  Multiple RPC time requests may be made over the lifetime
 *	of a server cache element, but once the cache element is removed, no
 *	more time requests will be made and the binding is released and
 *	deallocated.
 *
 *	The descriptor carries a reference count to make sure the RPC binding
 *	is not deleted (by removing the server from the cache) while a time
 *	request thread is using it in the middle of an RPC call.  This case
 *	is not likely, but it is possible:  If the RPC call hangs and the query
 *	timer expires during the call, the main thread will remove the server
 *	from the cache while the time request thread is still using the binding.
 *	The descriptor may not be deleted until the reference count drops to 
 *	zero.
 *
 *	Since two threads access the reference count, we must use a mutex to
 *	protect it.  The reference count can be in the binding descriptor
 *	(without fear of deallocating it while a thread is waiting on it)
 *	because when the last thread finishes referencing the descriptor
 *	(reference count is zero), then all threads are finished referencing it.
 *	Thus, we are free to destroy and deallocate the mutex along with the
 *	binding descriptor.
 *
 * The fields are:
 *
 *	refCount	is the number of threads that are currently using this
 *			descriptor.  There may be at most two:  the main thread
 *			and a thread created to request time from the server
 *			using this binding.  When a thread starts using this
 *			this descriptor, it increments this field.  When it is
 *			finished, it decrements the field and, if it is zero,
 *			releases the binding and deallocates this descriptor.
 *
 *	mutex		is the mutex protecting this descriptor.  It must be
 *			locked when accessing any of the fields.
 *
 *	handle		is the handle for the binding.
 *
 */
typedef struct ServerBindingDsc
{
    int			refCount;
    pthread_mutex_t	mutex;
    rpc_binding_handle_t handle;
} ServerBindingDsc;


typedef struct CacheElement
{
    Que          que;                  /* for ...CacheQ */
    SysIdType	 nodeId;               /* node id (unique even multihomed) */
    NodeAddr     nodeAddr;             /* node address (Ph IV or ethernet) */
    int		 courierRole;	       /* courier, not, or backup */
    PortDsc     *portPtr;              /* the port descriptor */
    char        *nodeNamePtr;          /* ptr to malloc'd node name string */
    VOID        *nodeFnamePtr;         /* this field not used in RPC version */
    unsigned int synchSerialNo;        /* last synch this element used in */
    utc_t       sync ;
    utc_t       serv ;
    int         linkId;  	       /* Open link to this node */

    ServerBindingDsc *bindPtr;	       /* ptr to malloc'd RPC binding info */
    uuid_t           serverUUID;       /* this servers' entry UUID         */


} CacheElement;

typedef struct ResponseElement
{
    Que que;			       /* for ...RespQ */
    CacheElement *serverPtr;           /* ptr to server's cache element */
    unsigned int msgIdSerial;          /* resend identifying number */
    UTCValue sendTime;                 /* time we sent request */
    utc_t serverTime;                  /* server's time */
    UTCValue processingDelay;          /* server's processing delay */
    UTCValue recvTime;                 /* time we recv'd response */
    char responseRecvd;                /* TRUE iff resp has been rec'v'd  */
    UTCValue Tserv;		       /* Serv Time est at Tsync */
    UTCValue Iserv;                    /* Serv Inacc est at Isync */
    int retriesLeft;		       /* before server declared dead */
#if CONFIG_THREADS
    pthread_t requestThread;	       /* handle for time request thread */
    char threadValid;		       /* TRUE if this thread is active */
#endif
    int	differingEpoch;	               /* flag:T if servers epoch diff than 
					  ours; server only */
} ResponseElement;

typedef struct ServerSet
{
    Que          cacheQ;	/* Qhd for list servers */
    Que          respQ;		/* Qhd for list of server responses */
    unsigned int numServers;	/* # servers in cache */
    unsigned int numServerSet;  /* # servers allowed to take part in synch */
    unsigned int numResp;	/* # servers in response queue */
    unsigned int numRecvd;	/* # responses received from servers */
    int		 linksPending;	/* # links waiting to be opened */
} ServerSet;

/*
 * Storage definition for epoch number mismatch set
 */
typedef enum
{
    SNdnsFull,
    SN802,
    SNtext
} SelServName;
 
typedef struct ServerName
{
    SelServName selector;
    union {
        NodeAddr         serverNodeName;             /* Server Name as 802  */
        char             serverTextName[K_MAX_SERVER_TEXT_NAME+1];
                                                     /* Server Name as Text */
    } serverName;
} ServerName;

typedef struct BadEpochElement      /* set of servers with bad epochs */ 
{
    Que          que;               /* Flink and Blink */
    ServerName   server;            /* server this entry represents */
    ResponseElement *serverId;      /* server id number */
    epoch_t      epochNumber;       /* The server's epoch number */
} BadEpochElement;

typedef struct BadEpochSet
{
    Que  serverQ;                       /* queue head */

} BadEpochSet;


/*
 * sdb, the synchronization database collects all the variables that
 * persist for the duration of a synchronization. At the start of each
 * synchronization, we init all the variables, including copying
 * management parameters from shmem to sdb. The synchronization
 * routines use the sdb copy instead of shmem parameters so we will
 * have a consistent set of values during the synchronization even if
 * the control program changes them in the middle.
 */
typedef struct Sdb
{
    MgtParams mgtParams;               /* all the management parameters */
    char fill1[4 - (sizeof (MgtParams) % 4)]; /* pad to longword boundary */

    ServerSet localSet;		       /* cache of local set of servers */
    ServerSet globalSet;	       /* cache of global set of servers */
    ServerSet TPset;		       /* record of time provider responses */
    BadEpochSet  epochSet;             /* servers with invalid epoch numbers */

    unsigned long TPnextPoll;	       /* time to next sync in seconds */

    unsigned int minServers;           /* clrk: minServers, srvr: minS - 1 */
    unsigned int synchSerialNo;        /* 1'd @ StartSynch */
    unsigned int msgIdSerial;          /* next msgIdSerial to assign */
    unsigned int retriesLeft;          /* retries left before reject server */

    char localSolDone;                 /* flag: local solicit done as appr */
    char doLANsolicit;		       /* flag: do an IEEE 802 LAN solicit */
    char NScontacted;                  /* flag: NS contacted since flush */
    char checkIntTime;		       /* flag: doing checkInt	*/
    char TPvalid;		       /* flag: TP not faulty, last sync */
    char TPnoClockSet;		       /* flag: true-TP can not set the clock*/
    char synchDoneFlag;		       /* flag: Set at end of first (actually 
					* every) synch to tell DNS that it's
					* OK to update its database on startup
					*/
    char fill2[3];		       /* pad to longword boundary */
} Sdb;

#if CONFIG_THREADS

/*
 * CPreq_t - Control Program request structure
 *
 * This structure contains the information needed to process one control program
 * request.  It is linked to an ETcontrol event block and passed between control
 * program threads and the DTSS main thread.  The fields are:
 *
 *	inArgPtr	Pointer to the input parameters. Some commands do not
 *                      required input parameters.
 *
 *	outRspPtr	Pointer to output parameters.  Some commands do not 
 *                      required  output paramters.
 *
 *      statusBlk       Error Status from the Time Service.
 *      
 *      func            Directive code for corresponding command.
 *
 *	processed	The main thread sets this flag to TRUE when it is done
 *			processing the control request.
 *
 *	shutdown	The main thread sets this flag to tell the CP thread
 *			that it is shutting down.
 *
 */
typedef struct CPreq_t
{
    VOID	*inArgPtr;
    VOID	*outRspPtr;
    StatusBlk   statusBlk;
    CtlFunc     func;
    char	processed;
    char 	shutdown;
} CPreq_t;


/*
 * TPrespDsc -- Time provider response descriptor
 *
 * This structure contains the information needed to process the response to one
 * request to the time provider.  It is linked to an ETprovider or ETprvdrFail
 * event block and passed between the time provider thread and the main thread.
 * The fields are:
 *
 *	respPtr		is a pointer to the time provider response information.
 *
 *	respType	the response type, which indicates the contents of the
 *			response information buffer.  This is either a control
 *			response (K_TP_RESP_CONTROL) or a timestamp response
 *			(K_TP_RESP_TIMES).
 *
 *	syncID		the serial number of the synchronization that resulted
 *		        in this time provider response.
 *
 *	release		The main thread sets this flag when it is finished with
 *			the time provider and wants the TP thread to terminate,
 *			either because the synchronization is finished, or
 *			because it detected an error in the TP response
 *			information.
 *
 */
typedef struct TPrespDsc
{
    char		*respPtr;
    TPmsgStatus		respStatus;
    unsigned short	respType;
    unsigned long	syncID;
    char		release;
} TPrespDsc;


/*
 * Values for TPrespDsc.respType
 */
#define K_TP_RESP_CONTROL	1
#define K_TP_RESP_TIMES		2

#endif /* CONFIG_THREADS */


/*
 * TimeRequestDsc -- Time Request Descriptor
 *
 * This structure describes a server time request.  It contains information used
 * to request time from a given server and also the information obtained from
 * that server in response.
 *
 */
typedef struct TimeRequestDsc
{
    ServerBindingDsc	*bindPtr;
    unsigned int	synchSerialNo;
    unsigned int	msgIdSerial;
    char		isServer;
    UTCValue		sendTime;
    utc_t		serverTime;
    UTCValue		processingDelay;
    UTCValue		recvTime;
    ServerSetType       servSetType;
    epoch_t		epochNumber;
    idl_long_int	courierRole;
}
TimeRequestDsc;



/*
 * Include architecture specific includes (after the above structure def's)!
 */

#include <dtss_service_ultrix.h>




/*
 * Prototypes
 */


/*
 * EVENTS.C
 */
StatusBlk InitEvent (void) ;

EventBlk  *GetNextEvent (void) ;

void GenEvent (EventTypes ) ; 

#if CONFIG_THREADS

void GenMsgEvent (EventTypes , EventBlk *) ;

void GenThreadEvent (Que *,pthread_mutex_t *, EventBlk *) ;

EventBlk *GetNextThreadEvent (Que *, pthread_mutex_t *) ;

void WakeMainWithEvent (void) ;

void WaitForEventResp (pthread_mutex_t *, pthread_cond_t *, EventBlk *) ;

void WakeThreadWithResp (pthread_mutex_t *, pthread_cond_t *, EventBlk *) ;

#endif /* CONFIG_THREADS */


/*
 * EVENTS_UNIX.C
 */

#if CONFIG_THREADS

StatusBlk InitSysdepEvent (void) ;

void CreateMsgEventPool (PortDsc *, int ) ;

EventBlk *AllocMsgEvent (PortDsc *) ;

void FreeMsgEvent (EventBlk *,	    PortDsc *) ;

#endif /* CONFIG_THREADS */


/*
 * TIMERS.C
 */

#if CONFIG_THREADS

EventBlk *GetNextTimerEvent (void);

void CreateTimer (int ) ;

#endif /* CONFIG_THREADS */

StatusBlk InitiateTimer (int , UTCValue *) ;

void CancelTimer (int ) ;

void RandomizeInterval (UTCValue *,
	    UTCValue *,
	    RandomTimerFactor ) ;

StatusBlk UpdateTimer (UTCValue *) ;

StatusBlk InitiateRandomTimer (int , UTCValue *, RandomTimerFactor ) ;


/*
 * PROVIDER_INTERFACE.C
 */
int ContactTimeProvider (void) ;

#if CONFIG_THREADS

TPmsgStatus ProcessTPresponse (EventBlk *) ;

TPmsgStatus ProcessTPtmoResponse (EventBlk *, unsigned long *,
	    unsigned long *, unsigned long * ) ;

void InitTPinterface (void) ;

void ShutdownTPinterface (void) ;

EventBlk *GetNextTPevent (void);

void WakeTPthreadWithResp (EventBlk *) ;

#endif /* !CONFIG_THREADS */

void ReleaseTimeProvider (EventBlk *) ;




/*
 * TRANSPORT_RPC.C
 */

void InitServerRPC (void) ;

StatusBlk InitRPCport (void) ;

StatusBlk RegisterTServerRPC (void) ;

StatusBlk UnregisterTServerRPC (void) ;

StatusBlk ExportTServiceToNS (void) ;

void UnexportTimeService (void) ;

void ShutDownRPC (void) ;

#if CONFIG_SECURITY
StatusBlk CreateSecurityCntxt (void) ;

boolean32 serverInSecurityGroup (unsigned_char_p_t );
#endif /* CONFIG_SECURITY */

StatusBlk GetTServicePrincUUID ( unsigned_char_p_t , uuid_t *);

error_status_t CreateRPCstringBinding (char *, rpc_binding_handle_t *) ;

void GetProfileName (rpc_if_handle_t, unsigned_char_p_t *, unsigned32 *) ;

ServerBindingDsc *CreateServerBinding (char *, size_t ,rpc_binding_handle_t);

void AcquireServerBinding (ServerBindingDsc *) ;

void ReleaseServerBinding (ServerBindingDsc *) ;

/*
 * TIME_REQUEST.C
 */

void RequestTimeFromServers (ServerSet     *, ServerSetType ) ;

void FreeTimeReqEvent (EventBlk *) ;

void ReleaseTimeReqThread (ResponseElement *) ;

void PrintResponseTimes (ResponseElement *) ;


ResponseElement *ProcessTimeResponse (EventBlk *) ;


#if CONFIG_THREADS

ResponseElement *ProcessTimeResponseMsg (EventBlk *) ;

ResponseElement *FindResponseElement (unsigned int, unsigned int ) ;

#endif /* CONFIG_THREADS */



/*
 * DTSS_SERVICE_EVENT_xxx.C
 */

/*
 * DTSS_SERVICE_STATE.C
 */

StatusBlk InitState (void) ;

void ReInitState (void) ;

CacheElement *AddServer (ServerSet *, SysIdType *, NodeAddr  *,
	    char *, VOID *, size_t  , int , PortDsc *) ;

void RemoveServer (ServerSet *, CacheElement *) ;

void Synchronize (INstimuli , EventBlk *) ;

void ScheduleNextSynch (ExtInacc *) ;

void UpdateStateTimers (UTCValue *);


void FlushServerCache (void);

/*
 * DTSS_SERVICE_CTL.C
 */

#if CONFIG_THREADS

void InitCPinterface (void);

void ShutdownCPinterface (void);

EventBlk *GetNextCPevent (void);

#endif /* CONFIG_THREADS */


StatusBlk ProcessControlMsg (EventBlk *) ; 

#if CONFIG_SECURITY
/*
 * MGTACL.C
 */
error_status_t mgt_acl_init (void);

void mgt_register_acl_mgr_if (rpc_binding_vector_t *,
	    uuid_vector_t		*,
	    error_status_t		*) ;

void mgt_export_acl_mgr_if (unsigned_char_t		*,
	    rpc_binding_vector_t	*,
	    uuid_vector_t		*,
	    error_status_t		*) ;

boolean32 mgt_test_read_access (rpc_binding_handle_t , StatusBlk_t *) ;

boolean32 mgt_test_write_access (rpc_binding_handle_t , StatusBlk_t *) ;

#endif /* CONFIG_SECURITY */

/*
 * DTSS_SERVICE_STUBS.C
 */

StatusBlk ProcessTimeRequest (EventBlk *eblkPtr) ;

StatusBlk Advertise (void) ;


/*
 * CLOCK_ADJUST.C
 */

int IsGreaterThanOrEqualNextLS (UTCValue *, ExtInacc *, UTCValue *) ;

void CalcIntervalEndPoints (UTCValue *, ExtInacc *, EndPointElement *, 
			    EndPointElement *) ; 
void CalcMidpointAndInacc (EndPointElement *, EndPointElement *, 
			   UTCValue *, ExtInacc *) ;

/*
 * Some declarations of qsort are broken
 * so we don't declare this function here
 */

int	CompareIntervalEndPoints (const VOID *, const VOID *) ;


int	ComputedTimeMaximum (int , int , EndPointElement *, 
			     EndPointElement **) ;

int	ComputedTimeMinimum (int , int , EndPointElement *, EndPointElement **) ;

unsigned long EstimateServerTime (UTCValue *,
	    UTCValue *, ExtInacc *, UTCValue *, UTCValue *,
	    UTCValue *, ExtInacc *, UTCValue *, ExtInacc *) ;

int IsInterval1InInterval2 (UTCValue *, ExtInacc *, UTCValue *,
	    ExtInacc *) ;

int	IsIntervalIntersection (UTCValue *, ExtInacc *, UTCValue *,
	    ExtInacc *) ;

StatusBlk SetClock (UTCValue *, ExtInacc *, UTCValue *, int *) ;

StatusBlk AdjustClock (UTCValue *, ExtInacc *, UTCValue *, ExtInacc *);

StatusBlk AdjustClkEnd (void);


void CalcNextLS (UTCValue *, ExtInacc *, UTCValue *);

void PrintServerTimes (UTCValue *, UTCValue *, ExtInacc *, ExtInacc *, UTCValue *,
	    UTCValue *, ExtInacc *, UTCValue *, ExtInacc *);



StatusBlk ReadHWclkParams (unsigned long *, unsigned long *);


/*
 * TRANSPORT.C
 */

StatusBlk InitTransport (void) ;

void RecvMsg (EventBlk *) ;

StatusBlk OpenLink (CacheElement *) ;

void DisconnectLink (PortDsc *, int *) ;

StatusBlk SendMsg (VOID *, int , NodeAddr *, VOID *, /* either full or char ptrs */
		   PortDsc *, int *) ;

void PrintNodeNameAddr(char *, VOID *, NodeAddr *) ; /* trace function */

void SprintNodeAddr (char *, NodeAddr *, PortDsc *) ; /* trace function */

void PrintNodeAddr (char *, NodeAddr *) ; /* trace function */


/*
 *  DTSS_SERVICE_DLI_xxx.C
 */

StatusBlk InitDLIports (int , int ) ;

void SendDLImsg (VOID *, int , EthernetAddr *, PortDsc *) ;

void SendMultiCastMsg (int , VOID *, int ) ;

StatusBlk RecvDliMsg (EventBlk *) ;

/*
 * DTSS_SERVICE_GLOBAL_SET.C
 */
void PopulateGlobalCache (void) ;

void PopulateLocalCache (void) ;



int UnexportGblTServiceFromNS (StatusBlk *);

int ExportGblTServiceToNS (StatusBlk *);

/*
 * DTSS_DECNET_xxx.C
 */

/*
 * DTSS_SERVICE_NCL.C
 */

unsigned long DirectiveCreate (unsigned long , int , StatusBlk *statusBlkPtr ) ;

unsigned long DirectiveDisable (unsigned long , StatusBlk *statusBlkPtr ) ;

unsigned long DirectiveEnable (unsigned long ,
	    unsigned long ,
            StatusBlk *statusBlkPtr ) ;

unsigned long DirectiveDelete (unsigned long ,
	    StatusBlk *statusBlkPtr ) ;

unsigned long DirectiveSynchronize (unsigned long ,
	    unsigned long ,
            StatusBlk *statusBlkPtr ) ;

unsigned long DirectiveShow (unsigned long ,
            unsigned long ,
            struct TransBuf *,
            StatusBlk *);

unsigned long DirectiveSet (unsigned long ,
            unsigned long ,
            struct TransBuf *,
	    int ,
            StatusBlk *);

unsigned long StateChange (int nextState );

unsigned long DirectiveUpdate (unsigned long ,
	    utc_t *,
	    unsigned long ,
            StatusBlk *statusBlkPtr ) ;

unsigned long DirectiveDeclare (unsigned long ,
	    utc_t *,
	    epoch_t ,
	    StatusBlk *) ;

unsigned long DirectiveAdvertise (unsigned long ,
	    VOID *,
	    unsigned long ,
	    StatusBlk *) ;

unsigned long DirectiveUnadvertise (StatusBlk *) ;

CacheElement *FindGSname (void);

CacheElement *FindLSname (void);

/*
 * LOGEVENT_xxx.C
 */

void LogEventClockSet (utc_t *, utc_t *);

void LogEventLocalFault (utc_t *, utc_t *, unsigned long );

void LogEventFaultyServer (utc_t *);

void LogEventSynchCompleted (utc_t *, utc_t *, int );

void LogEventUpdateTime (utc_t *, utc_t *);

void LogEventTooFewServers (unsigned long , unsigned long );

void LogEventStartupCompleted (unsigned long );

void LogEventDiffEpoch (BadEpochSet *);

void LogEventProtmismatch (ServerName *);

void LogEventTooManyServers (int , int );

void LogEventShutdownComplete (void);

void LogEventTimeRep (ServerName *);

void LogEventNoGlobalServers (void);

void LogEventServerNotResp (ServerName *, unsigned int , char *);

void LogEventInsfVirtMem (void);

void LogEventEpochDeclared (epoch_t , epoch_t , utc_t *, utc_t *);

void LogEventTPfailure (void);

void LogEventSystemError (int );

StatusBlk InitSysdepEvent(void) ;

void LogEventClockSet( utc_t *, utc_t *) ;

void LogEventLocalFault( utc_t *, utc_t *, unsigned long) ;

void LogEventFaultyServer( utc_t * ) ;

void LogEventSynchCompleted( utc_t *, utc_t *, int);

void LogEventUpdateTime( utc_t *, utc_t *) ;

void LogEventTooFewServers( unsigned long, unsigned long) ;

void LogEventStartupCompleted( unsigned long ) ;

void LogEventDiffEpoch( BadEpochSet * ) ;

void LogEventShutdownComplete( void ) ;

void LogEventDCEerror (error_status_t , char * );

void LogEventTimeRep( ServerName *) ;

void LogEventServerNotResp( ServerName *, unsigned int, char *) ;

void LogEventEpochDeclared( epoch_t , epoch_t , utc_t *, utc_t *) ;

void LogEventTPfailure( void ) ;

void LogEventTPtimeout(void) ;

void LogEventSystemError( int ) ;

void LogEventNoLANProfile (void);

void LogEventServerNotInGroup (char *, char *, int  );

#endif /* ifdef DTSS_SERVICE_H */
