/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mgtrpc.c,v $
 * Revision 1.1.12.2  1996/02/17  23:35:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:52:03  marty]
 *
 * Revision 1.1.12.1  1995/12/08  18:14:12  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/26  19:56 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:26:09  root]
 * 
 * Revision 1.1.10.10  1994/09/23  20:00:16  tom
 * 	Use macro U64_TO_HYPER from dce64.h
 * 	[1994/09/23  19:27:00  tom]
 * 
 * Revision 1.1.10.8  1994/08/31  20:30:55  cbrooks
 * 	CR11964 - auditing calls cause dtsd to dump core
 * 	[1994/08/31  20:21:30  cbrooks]
 * 
 * Revision 1.1.10.7  1994/08/23  20:32:38  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:57:12  cbrooks]
 * 
 * Revision 1.1.10.6  1994/08/16  18:13:11  cbrooks
 * 	More 64bit changes for 32bit machines
 * 	[1994/08/15  18:52:33  cbrooks]
 * 
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:26  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/18  14:02 UTC  pare
 * 	Merge changes from Hpdce02
 * 
 * Revision 1.1.10.5  1994/08/05  20:47:47  cbrooks
 * 	CR11240 - add dced config functionality
 * 	[1994/08/05  20:23:49  cbrooks]
 * 
 * Revision 1.1.10.4  1994/06/30  19:45:31  cbrooks
 * 	CR11112 - new serviceability messages required
 * 	[1994/06/28  17:48:55  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  17:46 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.10.3  1994/05/16  18:52:00  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:36  rhw]
 * 
 * Revision 1.1.10.2  1994/04/21  22:06:16  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:00:06  cbrooks]
 * 
 * 	Checkpoint:4.15.94
 * 	[1994/04/15  21:15:44  cbrooks]
 * 
 * 	Checkpoint
 * 	[1994/03/28  20:28:53  cbrooks]
 * 
 * Revision 1.1.10.1  1994/01/25  17:16:51  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/25  17:15:48  rsalz]
 * 
 * Revision 1.1.8.2  1993/07/14  20:32:46  tom
 * 	Bug 7017: show {local,global} servers returning 'invalid bound'
 * 	because serverCount wasn't initialzed to zero.
 * 	[1993/04/29  20:27:34  tom]
 * 
 * Revision 1.1.3.3  1993/09/17  17:59:54  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/17  17:59:22  pare]
 * 
 * 	This fixes OT5454 (memory being freed twice) by eliminating
 * 	the redundant call to DTSSfree.
 * 	[1992/12/30  20:09:46  yankes]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  22:23:24  htf]
 * 
 * 	This fixes OT5454 (memory being freed twice) by eliminating the
 * 	the redundant call to DTSSfree.
 * 	[1992/12/30  20:09:46  yankes]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  22:23:24  htf]
 * 
 * Revision 1.1.4.3  1993/09/15  17:48:58  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.3.2  1993/07/15  18:32:36  truitt
 * 	OT7017: The dtscp show local/global server command fails with the
 * 	        wrong error message when the create command has not yet
 * 	        been given.  Must initialize the serverCount variable
 * 	        to zero before use.
 * 	[1993/07/15  18:31:41  truitt]
 * 
 * Revision 1.1.8.2  1993/07/14  20:32:46  tom
 * 	Bug 7017: show {local,global} servers returning 'invalid bound'
 * 	because serverCount wasn't initialzed to zero.
 * 	[1993/04/29  20:27:34  tom]
 * 
 * Revision 1.1.2.3  1992/06/09  20:27:40  ko
 * 	Fix OT 3916 - dtsscp printing wrong version numbers...
 * 	mgtrpc was not passing along the support level -eg 1.2.3, 3 is support
 * 	level; it was hardcoded to 0 for some reason.
 * 	Pass it along, use K_* literals to pass version numbers.
 * 	[1992/06/04  16:23:51  ko]
 * 
 * 	fix ot 2827, 2150; use LogDCEevent, do not double dealloc reqPtr on error.
 * 	[1992/05/07  15:50:41  ko]
 * 
 * Revision 1.1.1.10  1993/05/04  22:40:40  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1  1992/01/19  15:34:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
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
 */

#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>
#include <dce/dce.h>
#include <dcedtssvc.h>
#include <dcedtsmsg.h>
#include <dcedtsmac.h>
#if	defined(DCE_AUD_POINTS)
#include <aud_evtsp.h>
#include <dce/audit.h>
#endif	/* defined(DCE_AUD_POINTS) */

#define INITSTATUSBLK( statusBlk, component, compError ) \
	 (statusBlk)->status = component;		 \
	 (statusBlk)->sysStatus = compError;

/* Since the data structures of both StatusBlk_t and StatusBlk  
 * are not the same, the following line is not working correctly
 * on the 64-bit platform.  
 * memcpy((VOID *)statusBlk,(VOID *)&reqPtr->statusBlk,sizeof(StatusBlk));
 * Define a new macro to replace this command here.	
 */

#define STATUSBLKCOPY(statusBlk,returnstatus)			\
	(statusBlk)->status = (returnstatus)->status;		\
	(statusBlk)->sysStatus = (returnstatus)->sysStatus;

/*
 * server Set structure used with SHOW LOCAL SERVERS and SHOW
 * GLOBAL SERVERS commands.
 */
typedef struct serverSet_t
{
    idl_long_int  *servCount;
    ServerID   *servSet;
} serverSet_t;

/*
 * Control Program database.
 *
 * These variables are owned by the control program thread and shared with the
 * main thread.  CPmutex must be locked when accessing these variables.  When
 * a control program thread receives a request, it places it in an event block
 * and inserts it in the queue CPeventQ.  It then wakes the main thread and
 * waits on the condition CPcond.  The main thread fetches the event block by
 * calling GetNextCPevent and processes it by calling ProcessControlMsg.  This
 * routine writes a response message, then wakes any waiting CP thread by
 * calling WakeCPwithResp, which broadcasts CPcond.  The CP thread then sends
 * its response message to the control program.
 */

static pthread_mutex_t	CPmutex;
static pthread_cond_t	CPcond;
static Que		CPeventQ;

/*
 * The variables below keep count of the number of control program requests
 * currently in progress.  If a control program tells DTSS to shut down (with
 * a DELETE command) while other control programs requests are active on this
 * server, we want to wait until these RPC routines have hada chance to return
 * results to their control programs before we exit the DTSS process (which
 * terminates all threads).
 *
 * When a CP command RPC routine starts a request, it will call BeginCPcall
 * to increment CPcallsInProgress.  When it exits, it will call EndCPcall,
 * which decrements CPcallsInProgress and signals CPcallCond.  When the
 * main thread exits its event processing loop, it calls ShutdownCPinterface,
 * which waits on CPcallCond until CPcallsInProgress goes to zero, then it
 * exits.
 */

static int		CPcallsInProgress;
static pthread_mutex_t	CPcallMutex;
static pthread_cond_t	CPcallCond;

/*
 * External variables
 */
extern ShmBuf *shmPtr;                 /* Ptr to shared memory
                                        * all modules have shmPtr defined */

extern Sdb sdb;			       /* synchronization database */

extern RpcBuf *rpcBufPtr;

#if	defined(DCE_AUD_POINTS)
extern dce_aud_trail_t dts_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */

/*
 * This is the entry-point vector that contains the name of the
 * server routines for the control program interface.
 */

globaldef time_control_v2_0_epv_t time_control_epv =
{
    CreateCmd,
    DeleteCmd,
    EnableCmd,
    DisableCmd,
    UpdateCmd,
    ChangeCmd,
    SynchronizeCmd,
    AdvertiseCmd,
    UnadvertiseCmd,
    SetDefaultCmd,
    SetAttrCmd,
    ShowAttrCmd,
    ShowAllCharsCmd,
    ShowAllStatusCmd,
    ShowAllCntrsCmd,
    ShowLocServersCmd,
    ShowGblServersCmd
};

/*
 * Forward static procedure definitions
 */
static int ProcessCPreq (CPreq_t  *);

static StatusBlk GenCPevent (CPreq_t    *, EventBlk  **);

static StatusBlk WaitForCPeventResp (EventBlk *, int *);

static StatusBlk AllocCPevent (CPreq_t   *, EventBlk  **);

static void FreeCPevent (EventBlk *);

static void BeginCPcall (void) ;

static void EndCPcall (void);

static void ShowServersCmd (serverSet_t *, int );

static void ShowServersSubCmd (ServerSet *,
	    int ,
	    ServerID [],
            int *);

StatusBlk NCLtoDCEerror(unsigned long );



/*
 *++
 *  InitCPinterface ()
 *
 *  Functional Description:
 *
 *	This routine initializes the control program interface.  It creates the
 *	necessary thread synchronization variables.
 *
 *  Inputs:
 *	none
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *	none
 *
 *  Implicit Outputs:
 *      CPeventQ	gets initialized to empty.
 *	CPmutex		the CP mutex object gets created
 *	CPcond 		the CP condition variable gets created.
 *	CPcallsInProgress gets zeroed.
 *	CPcallMutex	gets created.
 *	CPcallCond	gets created.
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *      none
 *
 *--
 */
void InitCPinterface (void)
{
    error_status_t	status;

    Qinit (&CPeventQ);
    dtss_mutex_init (&CPmutex);
    dtss_cond_init (&CPcond);

    CPcallsInProgress = 0;
    dtss_mutex_init (&CPcallMutex);
    dtss_cond_init (&CPcallCond);

#if CONFIG_SECURITY
    status = mgt_acl_init ();
    if (status != error_status_ok)
    {
	dce_error_string_t tempString ;
	
	dce_svc_printf( DTS_S_INIT_MGMT_ACL_MSG,
		       dce_dts_error_text( status, tempString ));
    }

    /*
     * Register authentication information with RPC for the host's principal
     * name.  This is the identity used by clients (like the acl editor)
     * calling the acl manager interface.
     */
    rpc_server_register_auth_info (rpcBufPtr->host_principal.name,
				   rpc_c_authn_dce_private,
				   (rpc_auth_key_retrieval_fn_t)NULL,
				   (idl_void_p_t)NULL,
				   &status);
    if (status != rpc_s_ok)
    {
	dce_error_string_t tempString ;
	
	dce_svc_printf( DTS_S_SERVER_REG_AUTH_INFO_MSG, 
		       dce_dts_error_text( status, tempString));
    }

    /*
     * register an authentication function for the 
     * rpc_mgmt_stop_server_listening_call 
     */

    rpc_mgmt_set_authorization_fn( dts_dtsd_auth_function, &status) ;

#endif /* CONFIG_SECURITY */
} /* end of routine InitCPinterface */


/*
 *++
 *  ShutdownCPinterface ()
 *
 *  Functional Description:
 *
 *	This routine shuts down the control program interface.  The main thread
 *	calls it after exiting the main processing loop.  It sets the CPshutdown
 *	flag to TRUE and then broadcasts on the CPcond condition variable to
 *	wake up waiting control program threads, so they may complete their RPC
 *	calls.
 *
 *	This routine then waits on the CPcallCond condition until
 *	CPcallsInProgress drops to zero, indicating that all control program
 *	RPC calls have terminated.
 *
 *  Inputs:
 *	none
 *
 *  Implicit Inputs:
 *	CPeventQ	is the queue of event blocks for control program RPC
 *			calls in progress.
 *
 *	CPcond		is the condition variable on which control program RPC
 *			threads are waiting for the main thread to process their
 *			event blocks.
 *
 *	CPcallsInProgress	is the number of control program RPC calls in
 *				progress.  When this drops to zero, we can
 *				finish shutting down the DTSS process.
 *
 *	CPcallCond		is the condition variable on which this routine
 *				waits for CPcallsInProgress to become zero.
 *
 *  Outputs:
 *	none
 *
 *  Implicit Outputs:
 *	reqPtr->shutdown gets set to TRUE for each event block waiting on
 *	CPeventQ.
 *
 *  Locking:
 *	CPmutex is locked while setting shutdown and broadcasting CPcond.
 *
 *	CPcallMutex is locked while waiting on CPcallCond.
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *      Control prgram RPC threads waiting on CPcond are awakened.  They finish
 *	their RPC calls, then signal CPcallCond.
 *
 *--
 */
void ShutdownCPinterface (void)
{
    EventBlk		*eblkPtr;
    CPreq_t		*reqPtr;

    /*
     * Tell the CP threads to shut down
     */
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	      "Main:  Shutting down CP interface\n"));

    dtss_mutex_lock (&CPmutex);

    QloopBegin (&CPeventQ, &eblkPtr) {
	reqPtr = (CPreq_t *)(eblkPtr->ioDsc.ptr);
	reqPtr->shutdown = TRUE;
    }
    QloopEnd
    dtss_cond_broadcast (&CPcond);

    dtss_mutex_unlock (&CPmutex);

    /*
     * If there are any control program RPC calls in progress, wait for them
     * to terminate.
     */
    dtss_mutex_lock (&CPcallMutex);
    while (CPcallsInProgress > 0)
	dtss_cond_wait (&CPcallCond, &CPcallMutex);
    dtss_mutex_unlock (&CPcallMutex);

    /*
     * Yield here to give the RPC runtime system time to send the results of the
     * control program calls back to the control programs.  We do this because
     * at the point where the RPC routine decrements CPcallsInProgress, the
     * RPC runtime still has not yet sent back the reply to the waiting control
     * programs.  We want to give those threads one last chance to send the
     * results before we finish shutting down the DTSS process.
     */
    pthread_yield ();

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	      "Main:  CP interface has shut down\n"));
} /* end of routine ShutdownCPinterface */

/*
 *++
 *  CreateCmd ()
 *
 *  Functional Description:
 *
 *      Create the Time Service (as a Server or a Clerk).
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h   :  A binding handle used in the RPC call.
 *      servType :  Service Type (server or clerk).
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      statusBlk:  Error status from the Time Service.
 *      comStatus:  Communication status between the client and the RPC
 *                  server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void CreateCmd (
    handle_t       bind_h,
    idl_long_int   *servType,
    StatusBlk_t    *statusBlk,
    error_status_t *comStatus
)
{
    CPreq_t   *reqPtr;

#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
    dce_aud_ev_info_t ev_info;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_CREATE_CMD, bind_h, 0, 
	aud_c_esl_cond_unknown, &ard, &audStatus);
    if (ard) {
	ev_info.format = aud_c_evt_info_long_int;
	ev_info.data.long_int = *servType;
	dce_aud_put_ev_info(ard, ev_info, &audStatus);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since successful status is set
     * by the executing thread.
     */

    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has write access to the management interface
     */
    if ( bind_h) 
    {
	if (!mgt_test_write_access (bind_h, statusBlk))
	{
#if	defined(DCE_AUD_POINTS)
	    if (ard)
		dce_aud_commit(dts_aud_trail, ard, 0, 0, 
			       aud_c_esl_cond_denial, &audStatus);
#endif /* defined(DCE_AUD_POINTS) */
	    EndCPcall ();
	    return;
	}
    }
    
#endif /* CONFIG_SECURITY */


    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->inArgPtr  = (VOID *)servType;
    reqPtr->func = CtlCreate;

    ProcessCPreq ( reqPtr );

    /*
     * Copy resulting statusBlk from executed Process Control Request
     * thread into returning argument.
     */

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk)
		? aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */

    EndCPcall ();
} /* end of routine CreateCmd */



/*
 *++
 *  DeleteCmd ()
 *
 *  Functional Description:
 *
 *      Delete the Time Service.  This command stops the process.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h   :  A binding handle used in the RPC call.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      statusBlk:  Error status from the Time Service.
 *      comStatus:  Communication status between the client and the RPC
 *                  server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void DeleteCmd (
    handle_t       bind_h,
    StatusBlk_t    *statusBlk,
    error_status_t *comStatus
)
{
    CPreq_t   *reqPtr;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_DELETE_CMD, bind_h, 0, 
	aud_c_esl_cond_unknown, &ard, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since successful status is set
     * by the executing thread.
     */

    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has write access to the management interface
     */
    if (!mgt_test_write_access (bind_h, statusBlk))
    {
#if	defined(DCE_AUD_POINTS)
	if (ard)
	    dce_aud_commit(dts_aud_trail, ard, 0, 0, 
		aud_c_esl_cond_denial, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	EndCPcall ();
	return;
    }
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->func = CtlDelete;

    ProcessCPreq ( reqPtr );

    /*
     * Copy resulting statusBlk from executed Process Control Request
     * thread into returning argument.
     */

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk)
		? aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine DeleteCmd */


/*
 *++
 *  EnableCmd ()
 *
 *  Functional Description:
 *
 *      Enable the Time Service.  Start accepting time requests from
 *      clients.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h   :  A binding handle used in the RPC call.
 *      clockSet :  Flag to tell the Time Service whether or not to
 *                  set the clock during the first synchronization.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      statusBlk:  Error status from the Time Service.
 *      comStatus:  Communication status between the client and the RPC
 *                  server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void EnableCmd (
    handle_t       bind_h,
    idl_long_int   *clockSet,
    StatusBlk_t    *statusBlk,
    error_status_t *comStatus
)
{

    CPreq_t   *reqPtr;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
    dce_aud_ev_info_t ev_info;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_ENABLE_CMD, bind_h, 0, 
	aud_c_esl_cond_unknown, &ard, &audStatus);
    if (ard) {
	ev_info.format = aud_c_evt_info_long_int;
	ev_info.data.long_int = *clockSet;
	dce_aud_put_ev_info(ard, ev_info, &audStatus);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */

    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has write access to the management interface
     */
    if ( bind_h ) 
    {
	if (!mgt_test_write_access (bind_h, statusBlk))
	{
#if	defined(DCE_AUD_POINTS)
	    if (ard)
		dce_aud_commit(dts_aud_trail, ard, 0, 0, 
			       aud_c_esl_cond_denial, &audStatus);
#endif /* defined(DCE_AUD_POINTS) */
	    EndCPcall ();
	    return;
	}
    }
    
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->inArgPtr  = (VOID *)clockSet;
    reqPtr->func = CtlEnable;

    ProcessCPreq ( reqPtr );

    /*
     * Copy resulting statusBlk from executed Process Control Request
     * thread into returning argument.
     */

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk)
		? aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine EnableCmd */




/*
 *++
 *  DisableCmd ()
 *
 *  Functional Description:
 *
 *      Disable the Time Service.  Make the service unavailable to clients.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h   :  A binding handle used in the RPC call.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      statusBlk:  Error status from the Time Service.
 *      comStatus:  Communication status between the client and the RPC
 *                  server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void DisableCmd  (
    handle_t       bind_h,
    StatusBlk_t    *statusBlk,
    error_status_t *comStatus
)
{

    CPreq_t   *reqPtr;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_DISABLE_CMD, bind_h, 0, 
	aud_c_esl_cond_unknown, &ard, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */

    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has write access to the management interface
     */
    if (!mgt_test_write_access (bind_h, statusBlk))
    {
#if	defined(DCE_AUD_POINTS)
	if (ard)
	    dce_aud_commit(dts_aud_trail, ard, 0, 0, 
		aud_c_esl_cond_denial, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	EndCPcall ();
	return;
    }
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->func = CtlDisable;

    ProcessCPreq ( reqPtr );

    /*
     * Copy resulting statusBlk from executed Process Control Request
     * thread into returning argument.
     */

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk) ?
		aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine DisableCmd */


/*
 *++
 *  UpdateCmd ()
 *
 *  Functional Description:
 *
 *      Update the servers's time by synchronizing with the time provided
 *      in the input argument.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h   :  A binding handle used in the RPC call.
 *      timeVal  :  new time value.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      statusBlk:  Error status from the Time Service.
 *      comStatus:  Communication status between the client and the RPC
 *                  server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void UpdateCmd (
    handle_t       bind_h,
    utc_t          *timeVal,
    StatusBlk_t    *statusBlk,
    error_status_t *comStatus
)
{
    CPreq_t   *reqPtr;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok;
    dce_aud_rec_t ard = 0 ;
    dce_aud_ev_info_t ev_info;
    utc_t	cur_time;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_UPDATE_CMD, bind_h, 0, 
	aud_c_esl_cond_unknown, &ard, &audStatus);
    if (ard) {
	ev_info.format = aud_c_evt_info_utc;
	ev_info.data.utc = *timeVal;
	dce_aud_put_ev_info(ard, ev_info, &audStatus);
	utc_gettime(&cur_time);
	ev_info.data.utc = cur_time;
	dce_aud_put_ev_info(ard, ev_info, &audStatus);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */

    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has write access to the management interface
     */
    if (!mgt_test_write_access (bind_h, statusBlk))
    {
#if	defined(DCE_AUD_POINTS)
	if (ard)
	    dce_aud_commit(dts_aud_trail, ard, 0, 0, aud_c_esl_cond_denial,
		&audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	EndCPcall ();
	return;
    }
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->inArgPtr  = (VOID *)timeVal;
    reqPtr->func = CtlUpdate;

    ProcessCPreq ( reqPtr );

    /*
     * Copy resulting statusBlk from executed Process Control Request
     * thread into returning argument.
     */

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk) ?
		aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine UpdateCmd */


/*
 *++
 *  ChangeCmd ()
 *
 *  Functional Description:
 *
 *      Change the epoch number of the server and optionally the time.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h    :  A binding handle used in the RPC call.
 *      changedir :  Change directive structure.  New epoch and time.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      statusBlk:  Error status from the Time Service.
 *      comStatus:  Communication status between the client and the RPC
 *                  server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void ChangeCmd (
    handle_t       bind_h,
    Change_t       *changeDir,
    StatusBlk_t    *statusBlk,
    error_status_t *comStatus
)
{
    CPreq_t   *reqPtr;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
    dce_aud_ev_info_t ev_info;
    utc_t cur_time;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_DELETE_CMD, bind_h, 0, 
	aud_c_esl_cond_unknown, &ard, &audStatus);
    if (ard) {
	ev_info.format = aud_c_evt_info_long_int;
	ev_info.data.long_int = shmPtr->mgtParams.epochNumber;
	dce_aud_put_ev_info(ard, ev_info, &audStatus);
	ev_info.data.long_int = (epoch_t)changeDir->newEpoch;
	dce_aud_put_ev_info(ard, ev_info, &audStatus);
	ev_info.format = aud_c_evt_info_utc;
	utc_gettime(&cur_time);
	if (changeDir->newTimePresent) {
	     ev_info.data.utc = changeDir->newTime;
	} else {
	     ev_info.data.utc = cur_time;
	}
	dce_aud_put_ev_info(ard, ev_info, &audStatus);
	ev_info.data.utc = cur_time;
	dce_aud_put_ev_info(ard, ev_info, &audStatus);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */

    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has write access to the management interface
     */
    if (!mgt_test_write_access (bind_h, statusBlk))
    {
#if	defined(DCE_AUD_POINTS)
	if (ard)
	    dce_aud_commit(dts_aud_trail, ard, 0, 0, aud_c_esl_cond_denial,
		&audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	EndCPcall ();
	return;
    }
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->inArgPtr  = (VOID *)changeDir;
    reqPtr->func = CtlChange;

    ProcessCPreq ( reqPtr );

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk) ?
		aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine ChangeCmd */


/*
 *++
 *  SynchronizeCmd ()
 *
 *  Functional Description:
 *
 *      This command causes the Time Service to synchronize.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h   :  A binding handle used in the RPC call.
 *      clockSet :  Sets the clock to the new time if this flag is
 *                  set.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      statusBlk:  Error status from the Time Service.
 *      comStatus:  Communication status between the client and the RPC
 *                  server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void SynchronizeCmd (
    handle_t       bind_h,
    idl_long_int   *setClock,
    StatusBlk_t    *statusBlk,
    error_status_t *comStatus
)
{

    CPreq_t   *reqPtr;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
    dce_aud_ev_info_t ev_info;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_SYNCHRONIZE_CMD, bind_h, 0, 
	aud_c_esl_cond_unknown, &ard, &audStatus);
    if (ard) {
	ev_info.format = aud_c_evt_info_long_int;
	ev_info.data.long_int = *setClock;
    }
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */

    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has write access to the management interface
     */
    if (!mgt_test_write_access (bind_h, statusBlk))
    {
#if	defined(DCE_AUD_POINTS)
	if (ard)
	    dce_aud_commit(dts_aud_trail, ard, 0, 0, aud_c_esl_cond_denial,
		&audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	EndCPcall ();
	return;
    }
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->inArgPtr  = (VOID *)setClock;
    reqPtr->func = CtlSynch;

    ProcessCPreq ( reqPtr );

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk) ?
		aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine SynchronizeCmd */


/*
 *++
 *  AdvertiseCmd ()
 *
 *  Functional Description:
 *
 *      Advertise this server as a member of the global set.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h   :  A binding handle used in the RPC call.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      statusBlk:  Error status from the Time Service.
 *      comStatus:  Communication status between the client and the RPC
 *                  server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void AdvertiseCmd (
    handle_t       bind_h,
    StatusBlk_t    *statusBlk,
    error_status_t *comStatus
)
{
    
    CPreq_t   *reqPtr;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_ADVERTISE_CMD, bind_h, 0, 
	aud_c_esl_cond_unknown, &ard, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */
    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has write access to the management interface
     */
    if (bind_h) 
    {
	if (!mgt_test_write_access (bind_h, statusBlk))
	{
#if	defined(DCE_AUD_POINTS)
	    if (ard)
		dce_aud_commit(dts_aud_trail, ard, 0, 0, aud_c_esl_cond_denial,
			       &audStatus);
#endif /* defined(DCE_AUD_POINTS) */
	    EndCPcall ();
	    return;
	}
    }
#endif


    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->func = CtlAdver;

    ProcessCPreq ( reqPtr );

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk) ?
		aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine AdvertiseCmd */



/*
 *++
 *  UndvertiseCmd ()
 *
 *  Functional Description:
 *
 *      Remove this server as a member of the Global Servers set (The CELL
 *      Profile).
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h   :  A binding handle used in the RPC call.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      statusBlk:  Error status from the Time Service.
 *      comStatus:  Communication status between the client and the RPC
 *                  server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void UnadvertiseCmd (
    handle_t       bind_h,
    StatusBlk_t    *statusBlk,
    error_status_t *comStatus
)
{
    CPreq_t   *reqPtr;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_UNADVERTISE_CMD, bind_h, 0, aud_c_esl_cond_unknown,
	&ard, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */
    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has write access to the management interface
     */
    if (!mgt_test_write_access (bind_h, statusBlk))
    {
#if	defined(DCE_AUD_POINTS)
	if (ard)
	    dce_aud_commit(dts_aud_trail, ard, 0, 0, aud_c_esl_cond_denial,
		&audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	EndCPcall ();
	return;
    }
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->func = CtlUnadver;

    ProcessCPreq ( reqPtr );

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk) ?
		aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine UnadvertiseCmd */


/*
 *++
 *  SetDefaultCmd ()
 *
 *  Functional Description:
 *
 *      Set an attribute to its default value.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h   :  A binding handle used in the RPC call.
 *      setAttr  :  Attribute type to be set.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      statusBlk:  Error status from the Time Service.
 *      comStatus:  Communication status between the client and the RPC
 *                  server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void SetDefaultCmd (
    handle_t       bind_h,
    CtlSet_t       *setAttr,
    StatusBlk_t    *statusBlk,
    error_status_t *comStatus
)
{

    CPreq_t   *reqPtr;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
    dce_aud_ev_info_t ev_info;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_SET_DEFAULT_CMD, bind_h, 0, 
	aud_c_esl_cond_unknown, &ard, &audStatus);
    if (ard) {
	ev_info.format = aud_c_evt_info_long_int;
	ev_info.data.long_int = setAttr->attribute;
	dce_aud_put_ev_info(ard, ev_info, &audStatus);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */
    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has write access to the management interface
     */
    if (!mgt_test_write_access (bind_h, statusBlk))
    {
#if	defined(DCE_AUD_POINTS)
	if (ard)
	    dce_aud_commit(dts_aud_trail, ard, 0, 0, aud_c_esl_cond_denial,
		&audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	EndCPcall ();
	return;
    }
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->inArgPtr  = (VOID *)setAttr;
    reqPtr->func = CtlSetDef;

    ProcessCPreq ( reqPtr );

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk) ?
		aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine SetDefaultCmd */


/*
 *++
 *  SetAttrCmd ()
 *
 *  Functional Description:
 *
 *      Set an attribute to a value specified by the user.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h     :  A binding handle used in the RPC call.
 *      setAttrVal :  Attribute type and value to be set.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      statusBlk:  Error status from the Time Service.
 *      comStatus:  Communication status between the client and the RPC
 *                  server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void SetAttrCmd (
    handle_t       bind_h,
    CtlSetAttr_t   *setAttrVal,
    StatusBlk_t    *statusBlk,
    error_status_t *comStatus
)
{

    CPreq_t   *reqPtr;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
    dce_aud_ev_info_t ev_info;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_SET_ATTR_CMD, bind_h, 0, 
	aud_c_esl_cond_unknown, &ard, &audStatus);

    if (ard) {
	ev_info.format = aud_c_evt_info_ulong_int;
	ev_info.data.ulong_int = setAttrVal->setAttr.attribute;
	dce_aud_put_ev_info(ard, ev_info, &audStatus);

	switch((int)setAttrVal->setAttr.attribute)
	{
	case K_SERVER_PRINCIPAL_NAME:
	case K_SERVER_ENTRY_NAME:
	case K_SERVER_GROUP_NAME:
	    ev_info.format = aud_c_evt_info_char_string;
	    ev_info.data.char_string = 
		       setAttrVal->attrValue.Data.setData.userString.buffer;
	    dce_aud_put_ev_info(ard, ev_info, &audStatus);
	    break;

	default:
	    /* use pickling if we want to include anything here. */
	    break;
	}
    }
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */
    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has write access to the management interface
     */
    if ( bind_h ) 
    {
	if (!mgt_test_write_access (bind_h, statusBlk))
	{
#if	defined(DCE_AUD_POINTS)
	    if (ard)
		dce_aud_commit(dts_aud_trail, ard, 0, 0, aud_c_esl_cond_denial,
			       &audStatus);
#endif /* defined(DCE_AUD_POINTS) */
	    EndCPcall ();
	    return;
	}
    }
    
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->inArgPtr  = (VOID *)setAttrVal;
    reqPtr->func = CtlSet;

    ProcessCPreq ( reqPtr );

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk) ?
		aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine SetAttrCmd */


/*
 *++
 *  ShowAttrCmd ()
 *
 *  Functional Description:
 *
 *      Show the current value of a specific attribute.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h     :  A binding handle used in the RPC call.
 *      attribute  :  The particular attribute being imquired about.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      attrValue:  Current value of this attribute.
 *      statusBlk:  Error status from the Time Service.
 *      comStatus:  Communication status between the client and the RPC
 *                  server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void ShowAttrCmd (
    handle_t       bind_h,
    idl_long_int   *attribute,
    AttrVal_t      *attrValue,
    StatusBlk_t    *statusBlk,
    error_status_t *comStatus
    )
{

    CPreq_t   *reqPtr;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok;
    dce_aud_rec_t ard = 0 ;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_SHOW_ATTR_CMD, bind_h, 0, aud_c_esl_cond_unknown, &ard,
	&audStatus);
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */
    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has read access to the management interface
     */
    if (bind_h) 
    {
	if (!mgt_test_read_access (bind_h, statusBlk))
	{
#if	defined(DCE_AUD_POINTS)
	    if (ard)
		dce_aud_commit(dts_aud_trail, ard, 0, 0, aud_c_esl_cond_denial,
			       &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	    EndCPcall ();
	    return;
	}
    }
    
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->inArgPtr  = (VOID *)attribute;
    reqPtr->outRspPtr = (VOID *)attrValue;
    reqPtr->func = CtlShow;

    ProcessCPreq ( reqPtr );

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk) ?
		aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine ShowAttrCmd */


/*
 *++
 *  ShowAllCharsCmd ()
 *
 *  Functional Description:
 *
 *      Show the values of all the characteristic attributes.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h     :  A binding handle used in the RPC call.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      showAllChar:  Characteristics structure.
 *      statusBlk:    Error status from the Time Service.
 *      comStatus:    Communication status between the client and the RPC
 *                    server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void ShowAllCharsCmd (
    handle_t       bind_h,
    CtlShowAllChar *showAllChar,
    StatusBlk_t    *statusBlk,
    error_status_t *comStatus
)
{

    CPreq_t   *reqPtr;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_SHOW_ALL_CHARS_CMD, bind_h, 0, aud_c_esl_cond_unknown,
	&ard, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */
    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has read access to the management interface
     */
    if (!mgt_test_read_access (bind_h, statusBlk))
    {
#if	defined(DCE_AUD_POINTS)
	if (ard)
	    dce_aud_commit(dts_aud_trail, ard, 0, 0, aud_c_esl_cond_denial,
		&audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	EndCPcall ();
	return;
    }
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->outRspPtr = (VOID *)showAllChar;
    reqPtr->func = CtlShowChar;

    ProcessCPreq ( reqPtr );

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk) ?
		aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine ShowAllCharsCmd */


/*
 *++
 *  ShowAllStatusCmd ()
 *
 *  Functional Description:
 *
 *      Show the values of all the status attributes.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h     :  A binding handle used in the RPC call.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      showAllStatus:  Status structure.
 *      statusBlk:      Error status from the Time Service.
 *      comStatus:      Communication status between the client and the RPC
 *                      server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void ShowAllStatusCmd (
    handle_t         bind_h,
    CtlShowAllStatus *showAllStatus,
    StatusBlk_t      *statusBlk,
    error_status_t   *comStatus
)
{

    CPreq_t   *reqPtr;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_SHOW_ALL_STATUS_CMD, bind_h, 0, aud_c_esl_cond_unknown,
	&ard, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */
    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has read access to the management interface
     */
    if (!mgt_test_read_access (bind_h, statusBlk))
    {
#if	defined(DCE_AUD_POINTS)
	if (ard)
	    dce_aud_commit(dts_aud_trail, ard, 0, 0, aud_c_esl_cond_denial,
		&audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	EndCPcall ();
	return;
    }
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->outRspPtr = (VOID *)showAllStatus;
    reqPtr->func = CtlShowStatus;

    ProcessCPreq ( reqPtr );

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk) ?
		aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine ShowAllStatusCmd */


/*
 *++
 *  ShowAllCntrsCmd ()
 *
 *  Functional Description:
 *
 *      Show the values of all the counters.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h     :  A binding handle used in the RPC call.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      showAllCntrs:   Counters structure.
 *      statusBlk:      Error status from the Time Service.
 *      comStatus:      Communication status between the client and the RPC
 *                      server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void ShowAllCntrsCmd (
    handle_t         bind_h,
    CtlShowAllCntrs  *showAllCntrs,
    StatusBlk_t      *statusBlk,
    error_status_t   *comStatus
)
{

    CPreq_t   *reqPtr;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_SHOW_ALL_CNTRS_CMD, bind_h, 0, 
	aud_c_esl_cond_unknown, &ard, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */
    *comStatus = rpc_s_ok;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has read access to the management interface
     */
    if (!mgt_test_read_access (bind_h, statusBlk))
    {
#if	defined(DCE_AUD_POINTS)
	if (ard)
	    dce_aud_commit(dts_aud_trail, ard, 0, 0, 
		aud_c_esl_cond_denial, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	EndCPcall ();
	return;
    }
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);

    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->outRspPtr = (VOID *)showAllCntrs;
    reqPtr->func = CtlShowCntrs;

    ProcessCPreq ( reqPtr );

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk) ?
		aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine ShowAllCntrsCmd */


/*
 *++
 *  ShowLocServersCmd ()
 *
 *  Functional Description:
 *
 *      Show all the local servers.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h     :  A binding handle used in the RPC call.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      showLServers:   Set of Local Servers.
 *      statusBlk:      Error status from the Time Service.
 *      comStatus:      Communication status between the client and the RPC
 *                      server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void ShowLocServersCmd (
    handle_t          bind_h,
    idl_long_int          *maxTservers,
    ServerID           localServers[],
    idl_long_int          *serverCount,
    CtlFunc           func,
    StatusBlk_t       *statusBlk,
    unsigned32        *comStatus
)
{

    CPreq_t      *reqPtr;
    serverSet_t  *showLservers;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_SHOW_LOC_SERVERS_CMD, bind_h, 0, aud_c_esl_cond_unknown,
	&ard, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */
    *comStatus = rpc_s_ok;
    *serverCount = 0;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has read access to the management interface
     */
    if (!mgt_test_read_access (bind_h, statusBlk))
    {
#if	defined(DCE_AUD_POINTS)
	if (ard)
	    dce_aud_commit(dts_aud_trail, ard, 0, 0, aud_c_esl_cond_denial,
		&audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	EndCPcall ();
	return;
    }
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);
    *serverCount = 0;

    showLservers = DTSSmallocStruct (serverSet_t);
    showLservers->servCount = serverCount;
    showLservers->servSet = (ServerID *)localServers;
    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->outRspPtr = (VOID *)showLservers;
    reqPtr->func = CtlShowLservers;

    ProcessCPreq ( reqPtr );

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);
    DTSSfree ((VOID *)showLservers);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk) ?
		aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine ShowLocServersCmd */


/*
 *++
 *  ShowGblServersCmd ()
 *
 *  Functional Description:
 *
 *      Show all the global servers.
 *      This procedure is executed through a Remote Procedure Call from
 *      a client Control Program.
 *
 *  Inputs:
 *
 *	bind_h     :  A binding handle used in the RPC call.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *
 *      showGServers:   Set of global servers.
 *      statusBlk:      Error status from the Time Service.
 *      comStatus:      Communication status between the client and the RPC
 *                      server.
 *
 *  Implicit Outputs:
 *
 *     none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *     none
 *
 *--
 */

void ShowGblServersCmd (
    handle_t          bind_h,
    idl_long_int          *maxTservers,
    ServerID           globalServers[],
    idl_long_int          *serverCount,
    CtlFunc           func,
    StatusBlk_t       *statusBlk,
    unsigned32        *comStatus
    )
{
    CPreq_t   *reqPtr;
    serverSet_t  *showGservers;
#if	defined(DCE_AUD_POINTS)
    error_status_t audStatus = error_status_ok ;
    dce_aud_rec_t ard = 0 ;
#endif	/* defined(DCE_AUD_POINTS) */

#if	defined(DCE_AUD_POINTS)
    dce_aud_start(EVT_SHOW_GBL_SERVERS_CMD, bind_h, 0, 
	aud_c_esl_cond_unknown, &ard, &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */

    BeginCPcall ();

    /*
     * Initialize status parameters.
     * The value of comStatus indicates that this RPC call was executed.
     * StatusBlk is initialized to failure, since  successful status is set
     * by the executing thread.
     */
    *comStatus = rpc_s_ok;
    *serverCount = 0;

#if CONFIG_SECURITY
    /*
     * Make sure the caller has read access to the management interface
     */
    if (!mgt_test_read_access (bind_h, statusBlk))
    {
#if	defined(DCE_AUD_POINTS)
	if (ard)
	    dce_aud_commit(dts_aud_trail, ard, 0, 0, aud_c_esl_cond_denial,
		&audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
	EndCPcall ();
	return;
    }
#endif

    INITSTATUSBLK( statusBlk, DTSS_ERROR, DTSS_FAILED);
    *serverCount = 0;

    showGservers = DTSSmallocStruct (serverSet_t);
    showGservers->servCount = serverCount;
    showGservers->servSet = (ServerID *)globalServers;
    reqPtr = DTSSmallocStruct (CPreq_t);
    memset((VOID *)reqPtr, 0, sizeof(CPreq_t));
    reqPtr->outRspPtr = (VOID *)showGservers;
    reqPtr->func = CtlShowGservers;

    ProcessCPreq ( reqPtr );

    STATUSBLKCOPY(statusBlk, &(reqPtr->statusBlk));

    DTSSfree ((VOID *)reqPtr);
    DTSSfree ((VOID *)showGservers);

#if	defined(DCE_AUD_POINTS)
    if (ard)
	dce_aud_commit(dts_aud_trail, ard, aud_c_evt_commit_sync, 0,
	    IsSuccess(*statusBlk) ?
		aud_c_esl_cond_success : aud_c_esl_cond_failure,
	    &audStatus);
#endif	/* defined(DCE_AUD_POINTS) */
    EndCPcall ();
} /* end of routine ShowGblServersCmd */

/*
 *++
 *  BeginCPcall()
 *
 *  Functional Description:
 *
 *	This routine is called by the control program RPC routines (xxxxCmd)
 *	to initiate a control program request.  It increments CPcallsInProgress,
 *	so that the main thread doesn't exit the DTSS process before this
 *	request returns its result to its control program.
 *
 *  Inputs:
 *	none
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *	none
 *
 *  Implicit Outputs:
 *      CPcallsInProgress gets incremented.
 *
 *  Locking:
 *	CPcallMutex is locked while incrementing.
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *      none
 *
 *--
 */

static void BeginCPcall (void)
{

    dtss_mutex_lock (&CPcallMutex);
    CPcallsInProgress++;
    dtss_mutex_unlock (&CPcallMutex);

} /* end of routine BeginCPcall */
/*
 *++
 *  EndCPcall()
 *
 *  Functional Description:
 *
 *	This routine is called by the control program RPC routines (xxxxCmd)
 *	to terminate a control program request.  It decrements CPcallsInProgress
 *	and signals CPcallCond, allowing the main thread to exit the DTSS
 *	process if CPcallsInProgress is zero.
 *
 *  Inputs:
 *	none
 *
 *  Implicit Inputs:
 *	CPcallCond	the CP call condition variable.
 *
 *  Outputs:
 *	none
 *
 *  Implicit Outputs:
 *      CPcallsInProgress gets incremented.
 *
 *  Locking:
 *	CPcallMutex is locked while decrementing and signalling.
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *      If the main thread is waiting on CPcallCond, it is awakened, and may
 * 	exit the DTSS process if CPcallsInProgress is zero.
 *
 *--
 */
static void EndCPcall (void)
{

    dtss_mutex_lock (&CPcallMutex);
    CPcallsInProgress--;
    dtss_cond_signal (&CPcallCond);
    dtss_mutex_unlock (&CPcallMutex);
} /* end of routine EndCPcall */


/*
 *++
 *  GetNextCPevent()
 *
 *  Functional Description:
 *
 *	Get the next control program event.  This routine returns the next event
 *	event block from the CP event queue by calling GetNextThreadEvent.  If
 *	the queue is empty, it returns NULL.
 *
 *  Inputs:
 *	none
 *
 *  Implicit Inputs:
 *	CPeventQ	is the queue of CP event blocks waiting to be processed.
 *	CPmutex		is the CP event queue mutex.
 *
 *  Outputs:
 *	none
 *
 *  Implicit Outputs:
 *      CPeventQ	if this queue is not empty, the first event block is
 *			is removed.
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
EventBlk *GetNextCPevent (void)
{
    EventBlk	*eblkPtr;

    eblkPtr = GetNextThreadEvent (&CPeventQ, &CPmutex);

    if (eblkPtr) {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		 "Main:  got event ETcontrol\n"));
    }

    return (eblkPtr);
} /* end of routine GetNextCPevent */

/*
 *++
 *  ProcessCPreq()
 *
 *  Functional Description:
 *
 *	Process a control program request message and return a control program
 *	response message.  This routine makes some consistency checks on the
 *	message, sets up the response message header, then generates a control
 *	program event for the main thread and waits for the main thread to
 *	process the event and fill in the body of the response message.  If the
 *	main thread fails to process the event, this routine indicates the error
 *	in the response message.
 *
 *  Inputs:
 *	reqPtr:	        pointer to the CP request structure.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *      reqPtr:         for commands that required outputs, they are
 *                      returned in this structure.
 *
 *  Implicit Outputs:
 *      none
 *
 *  Value Returned:
 *	shutdown	gets set to TRUE if the main thread indicates that the
 *			control program thread should exit.
 *
 *  Side Effects:
 *	Generates a control program event for the main thread.
 *
 *--
 */
static int ProcessCPreq ( 
    CPreq_t  *reqPtr
)
{
    EventBlk	*eblkPtr;
    int		shutdown;
    StatusBlk	statusBlk;

    shutdown = FALSE;

    /*
     * Ultrix will use a privledged socket to send these messages,
     * so it is not necessary to check the privs here.
     */
    reqPtr->statusBlk.status = DTSS_SUCCESS;

    /*
     * Generate a control program event.
     */
    statusBlk = GenCPevent (reqPtr, &eblkPtr);
    if (IsError(statusBlk)) {
	reqPtr->statusBlk = statusBlk;
	return (shutdown);
    }

    /*
     * Wait for the main thread to process the event.  If it didn't successfully
     * process it (it produced no response message), return an error response
     * message.
     */

    statusBlk = WaitForCPeventResp (eblkPtr, &shutdown);
    if (IsError(statusBlk))
	reqPtr->statusBlk = statusBlk;

    return (shutdown);
} /* end of routine ProcessCPreq */


/*
 *++
 *  GenCPevent(void)
 *
 *  Functional Description:
 *
 *	Generate a control program event.  Allocate an event block and CP
 *	request structure and link the control and response message buffers
 *	to it.  Call GenThreadEvent to place the event block on the event queue
 *	CPeventQ and wake the main thread.
 *
 *  Inputs:
 *      reqPtr          pointer to the CP request structure.
 *
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *	eblkPtr		pointer to the event block created.
 *
 *  Implicit Outputs:
 *	CPeventQ	an event block is placed on this queue, if one can be
 *			allocated.
 *
 *  Locking:
 *	CPmutex is locked while accessing CPeventQ.
 *
 *  Return Value:
 *	statusBlk	is DTSS_SUCCESS if we could allocate a CP event block
 *			and DTSS_ERROR otherwise.
 *
 *  Side Effects:
 *	The main thread is awakened.
 *
 *--
 */
static StatusBlk GenCPevent (
    CPreq_t    *reqPtr,
    EventBlk  **eblkPtr
)
{
    StatusBlk	statusBlk;

    /*
     * Allocate an event block to hold the request and response messages
     */
    statusBlk = AllocCPevent (reqPtr, eblkPtr);
    if (IsError (statusBlk))
	return (statusBlk);

    /*
     * Put the event block on CPeventQ and wake the main thread
     */
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	      "CP:  generating ETcontrol event\n"));
    GenThreadEvent (&CPeventQ, &CPmutex, *eblkPtr);

    return (statusBlk);
} /* end of routine GenCPevent */

/*
 *++
 *  WaitForCPeventResp()
 *
 *  Functional Description:
 *
 *	Calls WaitForEventResp to wait for the main thread to process the given
 *	control program event.  When the main thread wakes us by calling
 *	WakeThreadWithResp from ProcessControlMsg, we check the request
 *	structure to see if the request was processed and if the main thread
 *	is shutting down the CP interface.  Then, deallocate the event block for
 *	this event.  If the request wasn't processed, return an error.
 *
 *  Inputs:
 *	eblkPtr		pointer to the event block.
 *
 *  Implicit Inputs:
 *	eblkPtr->ioDsc.ptr	is the request structure (a CPreq_t structure)
 *	CPcond			is the condition variable to wait on.
 *	CPmutex 		protects the event block and CPcond.
 *
 *  Outputs:
 *	shutdownFlag	gets set to TRUE if the main thread told us to shut down
 *
 *  Implicit Outputs:
 *	none
 *
 *  Locking:
 *
 *  Return Value:
 *	statusBlk	is DTSS_SUCCESS if the message was processed and
 *			DTSS_ERROR if it wasn't.
 *
 *  Side Effects:
 *	The CP thread stalls until until the main thread calls WakeCPwithResp
 *	or ShutdownCPinterface.
 *
 *--
 */
static StatusBlk WaitForCPeventResp (
    EventBlk	*eblkPtr,
    int		*shutdownFlag
)
{
    CPreq_t	*reqPtr;
    StatusBlk	statusBlk;

    statusBlk.status = DTSS_SUCCESS;

    /*
     * Wait on the CP condition variable for the main thread to either set the
     * processed flag in the request structure (telling us it's processed the
     * message) or set CPshutdown (telling us to shut down).
     */
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	"CP:  Waiting for main to process event\n"));

    WaitForEventResp (&CPmutex, &CPcond, eblkPtr);

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	      "CP:  awakened with event response\n"));

    /*
     * If the shutdown occured before the message could be processed,
     * return an error.
     */

    reqPtr = (CPreq_t *)(eblkPtr->ioDsc.ptr);
    *shutdownFlag = reqPtr->shutdown;
    if (!reqPtr->processed) {
	statusBlk.status    = DTSS_ERROR;
	statusBlk.sysStatus = DTSS_TS_SHUTDOWN;
    }

    /*
     * Free the event block
     */
    FreeCPevent (eblkPtr);

    return (statusBlk);
} /* end of routine WaitForCPeventResp */

/*
 *++
 *  AllocCPevent(void)
 *
 *  Functional Description:
 *	Allocate and initialize a control program event block and its associated
 *	request structure.  Link the given request and response message buffers
 *	to the request structure.
 *
 *	Currently this routine just allocates them from memory, but when we
 *	allow multiple CP requests, this could keep some sort of pool to avoid
 *	the overhead of memory allocation and deallocation.
 *
 *  Inputs:
 *	ctlMsgPtr	is a pointer to the request message.
 *
 *	respMsgPtr	is a pointer to the request message buffer.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *	eblkPtr		is a pointer to the event block.  This is NULL
 *			if this routine fails.
 *
 *  Implicit Outputs:
 *	none
 *
 *  Value Returned:
 *	statusBlk.status 	is DTSS_SUCCESS if we succeeded.
 *				is DTSS_ERROR otherwise.
 *
 *	statusBlk.sysStatus	TBD - put some value here to indicate "no
 *				resources" or something similar.
 *
 *  Side Effects:
 *	none (TBD - if we implement a pool, we have to lock it with CPmutex)
 *
 *--
 */
static StatusBlk AllocCPevent (
    CPreq_t	        *reqPtr,
    EventBlk	**eblkPtr
)
{

    StatusBlk	statusBlk;

    statusBlk.status = DTSS_ERROR;
    statusBlk.sysStatus = 0;

    *eblkPtr = DTSSmallocStruct (EventBlk);
    memset((VOID *)*eblkPtr, 0, sizeof(EventBlk));
    (*eblkPtr)->event = ETcontrol;
    (*eblkPtr)->ioDsc.ptr = (unsigned char *)reqPtr;

    reqPtr->processed = FALSE;
    reqPtr->shutdown = FALSE;

    statusBlk.status = DTSS_SUCCESS;
    return (statusBlk);
} /* end of routine AllocCPevent */

/*
 *++
 *  FreeCPevent()
 *
 *  Functional Description:
 *	Free a control program event block.  Currently, this routine just
 *	deallocates the event block and its associated request structure.
 *	When we go to multiple CP requests, this could keep a pool of such
 *	structures to avoid the overhead of memory allocation and deallocation.
 *
 *  Inputs:
 *	eblkPtr			is the control program event block.
 *
 *  Implicit Inputs:
 *	eblkPtr->ioDsc.ptr	is the associated request structure.
 *
 *  Outputs:
 *	none
 *
 *  Implicit Outputs:
 *	none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *	none
 *
 *--
 */
static void FreeCPevent (
    EventBlk	*eblkPtr
)
{

    DTSSfree ((VOID *)eblkPtr);
} /* end of routine FreeCPevent */

/*
 *++
 *  ProcessControlMsg () --- THREADED VERSION
 *
 *  Functional Description:
 *
 *	This routine processes all messages from the control program.
 *
 *  Inputs:
 *	pointer to event block describing message received
 *
 *  Implicit Inputs:
 *	shmPtr - ptr to mapped and locked shared memory
 *
 *  Outputs:
 *      none
 *
 *  Implicit Outputs:
 *      many functions reply to the control program.
 *	caller is expected to repost read to K_CHANNEL_CPTS.
 *
 *  Value Returned:
 *	StatusBlk
 *
 *  Side Effects:
 *	many, depending on the function
 *
 *--
 */

StatusBlk ProcessControlMsg ( 
    EventBlk *eblkPtr
)
{
    StatusBlk		statusBlkResult;
    CPreq_t		*reqPtr;
    unsigned long 	status;
    struct TransBuf 	showBuffer;

    struct TransBuf 	setBuffer;
    int 		i;

    /*
     * Make a few debugging sanity checks
     */
    if (!(reqPtr = (CPreq_t *)eblkPtr->ioDsc.ptr))
	BugCheck ();

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	      "Main:  Processing a control message\n"));
    INITSTATUSBLK (&reqPtr->statusBlk, DTSS_ERROR, DTSS_FAILED);

    /*
     * Interpret and process the message
     */

    switch (reqPtr->func) {

	/*
	 * Create Directive
	 */
	case CtlCreate:
        {
	    long int TimeServType;

	    TimeServType = *(long int *)reqPtr->inArgPtr;

	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		     "CTLcreateEntity\n"));
	    status = DirectiveCreate( 0L, (int)TimeServType,
				      &statusBlkResult );
	    if (status == K_NCL_RSP_SYSTEM_ERROR)
		reqPtr->statusBlk = statusBlkResult;
	    else
		reqPtr->statusBlk = NCLtoDCEerror(status);

	    break;
	}

	/*
	 * Delete Directive
	 */
	case CtlDelete:
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		     "CtlDelete\n"));
	    /*
	     * Don't log a shutdown event here, we do that in
	     * the MAIN module during the cleanup
	     */
	    status = DirectiveDelete((long)0, &statusBlkResult);
	    if (status == K_NCL_RSP_SYSTEM_ERROR)
		reqPtr->statusBlk = statusBlkResult;
	    else
		reqPtr->statusBlk = NCLtoDCEerror(status);
	    /*
	     * If the Delete succeeded, then tell the CP interface to shutdown
	     */
	    if (IsSuccess (reqPtr->statusBlk))
		reqPtr->shutdown = TRUE;
	    break;

	/*
	 * Change Directive
	 */
	case CtlChange:
        {
	    Change_t *changeDir;

	    changeDir = (Change_t *)reqPtr->inArgPtr;

	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		     "CtlChange\n"));
	    /*
	     * Declare a new epoch. Use the the new time and inaccuracy,
	     * if specified.
	     */
	    status = DirectiveDeclare(0L, changeDir->newTimePresent ?
				      &changeDir->newTime:
				      NULL,
				      (epoch_t)changeDir->newEpoch,
				      &statusBlkResult);
	    if (status == K_NCL_RSP_SYSTEM_ERROR)
		reqPtr->statusBlk = statusBlkResult;
	    else
		reqPtr->statusBlk = NCLtoDCEerror(status);
	    /*
	     * set up the response status;
	     */
	    break;
        }

	/*
	 * Disable Directive
	 */
	case CtlDisable:
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		     "CTLdisable\n"));

	    status = DirectiveDisable( 0L, &statusBlkResult );
	    if (status == K_NCL_RSP_SYSTEM_ERROR)
		reqPtr->statusBlk = statusBlkResult;
	    else
		reqPtr->statusBlk = NCLtoDCEerror(status);

	    break;

	/*
	 * Enable Directive
	 */
	case CtlEnable:
        {
	    unsigned long clockSet;

	    clockSet = *(unsigned long *)reqPtr->inArgPtr;

	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		     "CtlEnable\n"));

	    status = DirectiveEnable( 0L,
				      clockSet,
				      &statusBlkResult);
	    if (status == K_NCL_RSP_SYSTEM_ERROR)
		reqPtr->statusBlk = statusBlkResult;
	    else
		reqPtr->statusBlk = NCLtoDCEerror(status);

	    break;
        }

	/*
	 * Synchronize Directive
	 */
	case CtlSynch:
        {
	    unsigned long clockSet;

	    clockSet = *(unsigned long *)reqPtr->inArgPtr;

	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		     "CTLsync\n"));

	    status = DirectiveSynchronize( 0L,
					  (unsigned long)clockSet,
					  &statusBlkResult);
	    if (status == K_NCL_RSP_SYSTEM_ERROR)
		reqPtr->statusBlk = statusBlkResult;
	    else
		reqPtr->statusBlk = NCLtoDCEerror(status);

	    break;
        }

	/*
	 * Update Directive
	 */
	case CtlUpdate:
        {
	    utc_t *timeVal;

	    timeVal = (utc_t *)reqPtr->inArgPtr;

	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		     "CTLupdate\n"));

	    status = DirectiveUpdate( 0L,
				      timeVal,
				     (unsigned long)FALSE,
				     &statusBlkResult);
	    if (status == K_NCL_RSP_SYSTEM_ERROR)
		reqPtr->statusBlk = statusBlkResult;
	    else
		reqPtr->statusBlk = NCLtoDCEerror(status);

	    break;
        }

	/*
	 * Advertise Directive
	 */
	case CtlAdver:
        {

	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		    "CtlAdver\n"));
	    status = DirectiveAdvertise(0L,
					(VOID *)NULL,
					0L,
					&statusBlkResult);

	    if (status == K_NCL_RSP_SYSTEM_ERROR)
		reqPtr->statusBlk = statusBlkResult;
	    else
		reqPtr->statusBlk = NCLtoDCEerror(status);

	    break;
        }

	/*
	 * Unadvertise Directive
	 */
	case CtlUnadver:
        {

	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		    "Ctlunadver\n"));

	    status = DirectiveUnadvertise( &statusBlkResult );

	    if (status == K_NCL_RSP_SYSTEM_ERROR)
		reqPtr->statusBlk = statusBlkResult;
	    else
		reqPtr->statusBlk = NCLtoDCEerror(status);

	    break;
        }

	/*
         * Set Default.  This case is generated by a SET directive,
	 * except that the attribute will be set to its default value.
	 */
	case CtlSetDef:
        {
	    CtlSet_t      *setAttr;

	    setAttr = (CtlSet_t *)reqPtr->inArgPtr;
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		     "CtlSet\n"));

	    status = DirectiveSet(0L,
				  (unsigned long)setAttr->attribute,
				  &setBuffer,
				  (int)setAttr->useDefault,
				  &statusBlkResult);

	    if (status == K_NCL_RSP_SYSTEM_ERROR)
		reqPtr->statusBlk = statusBlkResult;
	    else
		reqPtr->statusBlk = NCLtoDCEerror(status);

	    break;

        }

	/*
	 * Set Directive.  Set an attribute to a value given by the user.
	 */
	case CtlSet:
        {
	    CtlSetAttr_t  *setAttrVal;

	    setAttrVal = (CtlSetAttr_t *)reqPtr->inArgPtr;

	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		     "CtlSet\n"));

	    if ( MAX_TRANSBUF_SIZE < setAttrVal->attrValue.attrSize){
		INITSTATUSBLK(&reqPtr->statusBlk, DTSS_ERROR, DTSS_FAILED);
	    }
	    else
	    {
		switch((int)setAttrVal->setAttr.attribute)
		{
		  case K_SERVER_PRINCIPAL_NAME:
		  case K_SERVER_ENTRY_NAME:
		  case K_SERVER_GROUP_NAME:
		    memcpy((VOID *)setBuffer.transbuf_t_data,
                           (VOID *)
                           setAttrVal->attrValue.Data.setData.userString.buffer,
		           (size_t)setAttrVal->attrValue.attrSize);
		    break;

		  default:
		    memcpy((VOID *)setBuffer.transbuf_t_data,
			   (VOID *)&setAttrVal->attrValue.Data.setData,
			   (size_t)setAttrVal->attrValue.attrSize);
		    break;
		}

		setBuffer.transbuf_w_size = setAttrVal->attrValue.attrSize;
		status = DirectiveSet(0L,
				      (unsigned long)
				      setAttrVal->setAttr.attribute,
				      &setBuffer,
				      (int)setAttrVal->setAttr.useDefault,
				      &statusBlkResult);
		if (status == K_NCL_RSP_SYSTEM_ERROR)
		    reqPtr->statusBlk = statusBlkResult;
		else
		    reqPtr->statusBlk = NCLtoDCEerror(status);
	    }
	    break;
        }

	/*
	 * Show Directive
	 */
	case CtlShow:
        {
	    unsigned long attribute;
	    AttrVal_t *attrValue;

	    attribute = *(unsigned long *)reqPtr->inArgPtr;
	    attrValue =  (AttrVal_t *)reqPtr->outRspPtr;

	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		     "CTLshow\n"));
	    status = DirectiveShow(0L,
				   attribute,
				   &showBuffer,
				   &statusBlkResult);
	    reqPtr->statusBlk = NCLtoDCEerror(status);

	    if ( status == K_NCL_RSP_SUCCESS ) {
		if ( MAX_TRANSBUF_SIZE < showBuffer.transbuf_w_size ) {
		    INITSTATUSBLK(&reqPtr->statusBlk, DTSS_ERROR, DTSS_FAILED);
		}
		else
		{
		    switch (showBuffer.transbuf_w_type)
		    {
			case K_TYPE_STRING:

		        memcpy((VOID *)attrValue->Data.setData.userString.buffer,
			       (VOID *)showBuffer.transbuf_t_data,
			       showBuffer.transbuf_w_size);

			    break;

			default:

			memcpy ((VOID *)&attrValue->Data.setData,
			        (VOID *)showBuffer.transbuf_t_data,
			         showBuffer.transbuf_w_size);
		    }
		    attrValue->attributeType = showBuffer.transbuf_w_type;
		    attrValue->attrSize = showBuffer.transbuf_w_size;
		}
	    }
	    break;

        }

	/*
	 * Show All counters
	 */
	case CtlShowCntrs:
        {
	    CtlShowAllCntrs *showAllCntrs;

	    /*
	     * check state.
	     */

	    if (IsState(K_STATE_NOT_CREATED))
	    {
		reqPtr->statusBlk =
		    NCLtoDCEerror((unsigned long)K_NCL_RSP_NOT_CREATED);
		break;
	    }

	    showAllCntrs = (CtlShowAllCntrs *)reqPtr->outRspPtr;

	    showAllCntrs->variant = (long int)shmPtr->mgtParams.variant;
	    showAllCntrs->creationTime = shmPtr->mgtParams.creationTime;
	    for ( i = 0; i < K_NUM_COUNTERS; i++)
		U64_TO_HYPER(showAllCntrs->counters[i], shmPtr->counters[i]);
	    INITSTATUSBLK(&reqPtr->statusBlk, DTSS_SUCCESS, DTSS_SUCCESS);
	    break;
        }

	/*
	 * Show All Characteristics
	 */
	case CtlShowChar:
        {
	    CtlShowAllChar   	*allChar;
	    utc_t currTime, nextChange;
	    long ourTDF ;

	    /*
	     * check state.
	     */

	    if (IsState(K_STATE_NOT_CREATED))
	    {
		reqPtr->statusBlk =
		    NCLtoDCEerror((unsigned long)K_NCL_RSP_NOT_CREATED);
		break;
	    }

	    /*
	     * get next TDF change before everything else
	     */
	    utc_gettime(&currTime);
	    utc_bintime((timespec_t *)0, (timespec_t *)0, &ourTDF, &currTime) ;
	    utc_getnextzone(&nextChange, &currTime);

	    /*
	     * Get all the attributes
	     */

	    allChar = (CtlShowAllChar *)reqPtr->outRspPtr;

#define SET_UTC_FROM_UTCVALUE(u,uv,tdf)\
	    utc_mkcomptime(&(u), &(uv), (UTCValue *)0, tdf);


	    SET_UTC_FROM_UTCVALUE(allChar->checkInt,
				  shmPtr->mgtParams.checkInt,ourTDF);
	    SET_UTC_FROM_UTCVALUE(allChar->maxInacc,
				  shmPtr->mgtParams.maxInacc,ourTDF);
	    SET_UTC_FROM_UTCVALUE(allChar->errorTolerance,
				  shmPtr->mgtParams.errorTolerance,ourTDF);
	    SET_UTC_FROM_UTCVALUE(allChar->LANtimeOut,
				  shmPtr->mgtParams.LANtimeOut,ourTDF);
	    SET_UTC_FROM_UTCVALUE(allChar->WANtimeOut,
				  shmPtr->mgtParams.WANtimeOut,ourTDF);
	    SET_UTC_FROM_UTCVALUE(allChar->syncHold,
				  shmPtr->mgtParams.syncHold,ourTDF);

	    allChar->serverTDF = ourTDF ;

	    allChar->nextTDFchange   = nextChange;

	    (void)memcpy((VOID *)allChar->serverPrincName.buffer,
			 (VOID *)shmPtr->mgtParams.servPrincName,
		         strlen((char *)shmPtr->mgtParams.servPrincName)+1);

	    (void)memcpy((VOID *)allChar->serverEntryName.buffer,
			 (VOID *)shmPtr->mgtParams.servEntryName,
		         strlen((char *)shmPtr->mgtParams.servEntryName)+1);

	    (void)memcpy((VOID *)allChar->TSgroupName.buffer,
			 (VOID *)shmPtr->mgtParams.groupName,
		         strlen((char *)shmPtr->mgtParams.groupName)+1);

	    allChar->minServers     = shmPtr->mgtParams.minServers;
	    allChar->queryAttempts  = shmPtr->mgtParams.repetitions;
	    allChar->courierRole     = shmPtr->mgtParams.courierRole;
	    allChar->actingCourierRole = shmPtr->mgtParams.myCourierRole;
	    allChar->epochNumber = shmPtr->mgtParams.epochNumber;
	    allChar->variant     = (long int)shmPtr->mgtParams.variant;
	    allChar->hasProvider = (long int)shmPtr->mgtParams.hasProvider;
	    allChar->clockAdjustment = (K_100NS_PER_SEC*100)/K_TICKADJ_RATE;
	    allChar->clockResolution = shmPtr->clockParams.clockResolution;
	    allChar->driftRate = shmPtr->clockParams.clockDrift;
	    allChar->autoTDF = shmPtr->mgtParams.autoTDF;

	    allChar->timeVersion[K_VER_LET] = K_TIME_VERSION_SUPPORT;
	    allChar->timeVersion[K_VER_MAJ] = K_TIME_VERSION_MAJOR;
	    allChar->timeVersion[K_VER_MIN] = K_TIME_VERSION_MINOR;
	    allChar->timeVersion[K_VER_SUB] = K_TIME_VERSION_LEVEL;

	    allChar->DNAversion[K_VER_LET] = K_DNA_VERSION_SUPPORT;
	    allChar->DNAversion[K_VER_MAJ] = K_DNA_VERSION_MAJOR;
	    allChar->DNAversion[K_VER_MIN] = K_DNA_VERSION_MINOR;
	    allChar->DNAversion[K_VER_SUB] = K_DNA_VERSION_LEVEL;

	    allChar->DECdts[K_VER_LET] = K_DECDTS_VERSION_SUPPORT;
	    allChar->DECdts[K_VER_MAJ] = K_DECDTS_VERSION_MAJOR;
	    allChar->DECdts[K_VER_MIN] = K_DECDTS_VERSION_MINOR;
	    allChar->DECdts[K_VER_SUB] = K_DECDTS_VERSION_LEVEL;

	    INITSTATUSBLK(&reqPtr->statusBlk, DTSS_SUCCESS, DTSS_SUCCESS);
	    break;

        }

	/*
	 * Show All Status
	 */
	case CtlShowStatus:
        {
	    CtlShowAllStatus *showAllStatus;

	    /*
	     * check state.
	     */

	    if (IsState(K_STATE_NOT_CREATED))
	    {
		reqPtr->statusBlk =
		    NCLtoDCEerror((unsigned long)K_NCL_RSP_NOT_CREATED);
		break;
	    }

	    showAllStatus = (CtlShowAllStatus *)reqPtr->outRspPtr;

	    showAllStatus->variant = (long int)shmPtr->mgtParams.variant;
	    (void)memcpy((VOID *)&showAllStatus->uid,
			 (VOID *)&shmPtr->mgtParams.uid, sizeof(uuid_t));

	    (void) utc_mkcomptime(&showAllStatus->lastSynch,
				  &shmPtr->baseTimes.Tbase,
				  (UTCValue *)NULL, 0L);
	    showAllStatus->state = shmPtr->mgtParams.TSstate;
	    INITSTATUSBLK(&reqPtr->statusBlk, DTSS_SUCCESS, DTSS_SUCCESS);
	    break;
	}

	/*
	 * Show all local servers
	 */
	case CtlShowLservers:
        {
	    serverSet_t *showLServers;

	    /*
	     * check state.
	     */

	    if (IsState(K_STATE_NOT_CREATED))
	    {
		reqPtr->statusBlk =
		    NCLtoDCEerror((unsigned long)K_NCL_RSP_NOT_CREATED);
		break;
	    }

	    showLServers = (serverSet_t *)reqPtr->outRspPtr;

	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		     "CtlShowLservers\n"));

	    ShowServersCmd (showLServers, K_SERVER_LOCAL );
	    INITSTATUSBLK(&reqPtr->statusBlk, DTSS_SUCCESS, DTSS_SUCCESS);
	    break;
	}

	/*
	 * Show All global servers
	 */
	case CtlShowGservers:
        {
	    serverSet_t *showGServers;

	    /*
	     * check state.
	     */

	    if (IsState(K_STATE_NOT_CREATED))
	    {
		reqPtr->statusBlk =
		    NCLtoDCEerror((unsigned long)K_NCL_RSP_NOT_CREATED);
		break;
	    }

	    showGServers = (serverSet_t *)reqPtr->outRspPtr;

	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		     "CtlShowGservers\n"));
	    ShowServersCmd (showGServers, K_SERVER_GLOBAL );
	    INITSTATUSBLK(&reqPtr->statusBlk, DTSS_SUCCESS, DTSS_SUCCESS);
	    break;
        }

	/*
	 * Otherwise
	 */
	default:
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_ctlmsgs, svc_c_debug1,
		  "Bad func from CP: %d\n", reqPtr->func));
	INITSTATUSBLK(&reqPtr->statusBlk, DTSS_ERROR, DTSS_FAILED);
	break;

    } /* end switch (reqPtr->func) */

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	      "--> Waking CP Threads\n"));

    reqPtr->processed = TRUE;
    WakeThreadWithResp (&CPmutex, &CPcond, eblkPtr);

    ReturnDTSSstatus (DTSS_SUCCESS, statusBlkResult);
} /* End of routine ProcessControlMsg -- THREADED VERSION */


/*
 *++
 *  NCLtoDCEerror()
 *
 *  Functional Description:
 *
 *    This routine converts an NCL status value returned from the
 *    service ncl directives into a DTS DCE error code.
 *
 *  Inputs:
 *      status - integer giving the ncl status to convert.
 *
 *  Implicit Inputs:
 *	shmPtr - ptr to dtss management parameters.
 *
 *  Outputs:
 *      none
 *
 *  Implicit Outputs:
 *
 *  Value Returned:
 *	StatusBlk
 *
 *  Side Effects:
 *
 *--
 */

StatusBlk NCLtoDCEerror( 
    unsigned long status
)
{
    StatusBlk statusBlk, *statusBlkPtr;

    statusBlkPtr = &statusBlk;

    switch ((int)status) {
	case K_NCL_RSP_SUCCESS:
	INITSTATUSBLK((statusBlkPtr),DTSS_SUCCESS, dts_s_normal);
	break;

	case K_NCL_RSP_ALREADY_EXISTS:
	INITSTATUSBLK((statusBlkPtr),DTSS_ERROR, dts_s_created);
	break;

	case K_NCL_RSP_ACCESS_DENIED:
	INITSTATUSBLK((statusBlkPtr),DTSS_ERROR, dts_s_nopriv);
	break;

	case K_NCL_RSP_NOT_CREATED:
	INITSTATUSBLK((statusBlkPtr),DTSS_ERROR, dts_s_not_created);
	break;

	case K_NCL_RSP_WRONG_STATE:
	INITSTATUSBLK((statusBlkPtr), DTSS_ERROR, dts_s_wrong_state);
	break;

	case K_NCL_RSP_EPOCH_DIFF:
	INITSTATUSBLK((statusBlkPtr), DTSS_ERROR, dts_s_epoch_range);
	break;

	case K_NCL_RSP_NOT_SUPPORTED:
	INITSTATUSBLK((statusBlkPtr), DTSS_ERROR, dts_s_not_server);
	break;

	case K_NCL_RSP_NOT_CONTAINED:
	INITSTATUSBLK((statusBlkPtr), DTSS_ERROR, dts_s_no_time_updated);
	break;

	case K_NCL_RSP_INV_ARG:
	INITSTATUSBLK((statusBlkPtr), DTSS_ERROR, dts_s_set_error);
	break;

	default:
	INITSTATUSBLK((statusBlkPtr),DTSS_ERROR, dts_s_failed);
	break;
    }
    return( statusBlk );
}

/*
 *++
 *  ShowServersCmd()                 --RPC VERSION--
 *
 *  Functional Description:
 *
 *  Process a show servers request from the control program.
 *
 *  Inputs:
 *
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *
 *  Implicit Outputs:
 *
 *
 *  Value Returned:
 *
 *
 *  Side Effects:
 *
 *
 *--
 */

static void ShowServersCmd( 
    serverSet_t *serversPtr,
    int serverType
)
{
    int count;

    count = 0;

    if ( serverType == K_SERVER_LOCAL )
	ShowServersSubCmd ((ServerSet *)&sdb.localSet,
			   FALSE,	/* not global set */
			   serversPtr->servSet,
			   &count);

    else
	ShowServersSubCmd ((ServerSet *)&sdb.globalSet,
			   TRUE,	/* global set */
			   serversPtr->servSet,
			   &count);

    *serversPtr->servCount = count;
}
/* End of routine ShowServersCmd */

/*
 *++
 *  ShowServersSubCmd()                         -- RPC VERSION --
 *
 *  Functional Description:
 *
 *      Subroutine for ShowServersCmd to load local or global set
 *      into response buffer.
 *
 *  Inputs:
 *
 *      setP - pointer to local or global set
 *      count - accumulating count of servers in response message
 *
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *
 *  Implicit Outputs:
 *
 *
 *  Value Returned:
 *
 *
 *  Side Effects:
 *
 *
 *--
 */

static void ShowServersSubCmd(
    ServerSet *setP,
    int isGlobal,
    ServerID serverIDarray[],
    int *countPtr
)
{
    CacheElement *cachePtr;
    ServerID *sIDp;

    sIDp = &serverIDarray[*countPtr];

    QloopBegin(&setP->cacheQ, &cachePtr)
    {
   	if ( *countPtr >= MAX_TSERVERS )
	    break;

	sIDp->sync = cachePtr->sync;
	sIDp->serv = cachePtr->serv;
	
        sIDp->isGlobal = isGlobal;
        sIDp->lastUsed =
	    ((cachePtr->synchSerialNo == sdb.synchSerialNo) ? 1 : 0);

	if (cachePtr->nodeNamePtr)
	    (void) strcpy ((char *)sIDp->nodeName.buffer,
			    cachePtr->nodeNamePtr);
	else
	    strcpy ((char *)sIDp->nodeName.buffer, "*none*");
	/*
	 * Now set up the tranport Type
         */
	switch (cachePtr->portPtr->socketType) {
	case K_SOCKET_DTS_8023:
		sIDp->transport = K_PROT_IEEE802;
		break;
	case K_SOCKET_DTS_DECNET:
		sIDp->transport = K_PROT_DECNET;
		break;
	case K_SOCKET_DTS_TCP:
		sIDp->transport = K_PROT_TCP_IP;
		break;
	case K_SOCKET_DTS_RPC:
		sIDp->transport = K_PROT_RPC;
		break;
	default:
		sIDp->transport = -1;
		break;
	}
	sIDp++;
	(*countPtr)++;
    }
    QloopEnd;
}
