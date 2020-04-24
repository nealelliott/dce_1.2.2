/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: transport_rpc.c,v $
 * Revision 1.1.12.2  1996/02/17  23:35:59  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:52:13  marty]
 *
 * Revision 1.1.12.1  1995/12/08  18:14:26  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/6  1995/09/11  12:36 UTC  pare
 * 	Merge changes
 * 
 * 	HP revision /main/HPDCE02/pare_MOTHRA/1  1995/09/08  13:03 UTC  pare
 * 	Free lanProfileName string -- CHFts16224
 * 
 * 	HP revision /main/HPDCE02/5  1995/05/18  20:34 UTC  pare
 * 	Merge fix for CHFts14912
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/1  1995/05/18  20:32 UTC  pare
 * 	Fix for CHFts14912
 * 
 * 	HP revision /main/HPDCE02/4  1994/10/10  17:49 UTC  pare
 * 	Merge bl21 changes.
 * 	[1995/12/08  17:26:23  root]
 * 
 * Revision 1.1.10.11  1994/10/05  16:09:49  tom
 * 	Bug 12498 - Move audit init ever later in the process, right
 * 	before we do rpc listen, since it requires interfaces to be registered.
 * 	[1994/10/05  16:06:38  tom]
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/26  20:10 UTC  pare
 * 	Merge bl18 changes
 * 
 * Revision 1.1.10.10  1994/08/23  20:32:44  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:57:17  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:53:17  cbrooks]
 * 
 * Revision 1.1.10.9  1994/08/16  18:13:17  cbrooks
 * 	More 64bit changes for 32bit machines
 * 	[1994/08/15  18:53:52  cbrooks]
 * 
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:34  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/18  14:15 UTC  pare
 * 	Merge changes from Hpdce02
 * 
 * Revision 1.1.10.8  1994/08/05  20:47:53  cbrooks
 * 	CR11240 - add dced config functionality
 * 	[1994/08/05  20:23:56  cbrooks]
 * 
 * Revision 1.1.10.7  1994/07/29  18:27:16  cbrooks
 * 	CR11466 - new functions for read/write bindings file
 * 	[1994/07/29  18:25:15  cbrooks]
 * 
 * Revision 1.1.10.6  1994/07/11  18:27:44  cbrooks
 * 	CR11223 - better error message for timeout
 * 	[1994/07/11  18:27:08  cbrooks]
 * 
 * Revision 1.1.10.5  1994/06/30  19:45:37  cbrooks
 * 	CR11112 new serviceability messages required
 * 	[1994/06/30  19:44:07  cbrooks]
 * 
 * 	CR11112 - new serviceability messages required
 * 	[1994/06/28  17:49:05  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  17:58 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/2  1994/02/03  13:57  pare
 * 	merge kk and hpdce01
 * 
 * 	HP revision /main/HPDCE01/pare_kk/1  1994/02/02  18:16  pare
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.7.4  1993/09/17  17:31:04  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/17  17:30:08  pare]
 * 
 * Revision 1.1.8.3  1993/09/15  17:49:07  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.7.3  1993/07/15  17:59:31  truitt
 * 	OT5146: Put the KeyMgmtThread() function under DTS_ENTITY_PRINCIPAL
 * 	        ifdef.
 * 	OT7016: Don't let dtsd complain about type already registered during
 * 	        an enable of a global server.
 * 	[1993/07/15  17:59:05  truitt]
 * 
 * Revision 1.1.10.4  1994/05/16  18:52:08  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:43  rhw]
 * 
 * Revision 1.1.10.3  1994/04/28  21:09:04  cbrooks
 * 	DTS Code cleanup
 * 	[1994/04/27  19:41:59  cbrooks]
 * 
 * Revision 1.1.10.2  1994/04/21  22:06:22  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:00:15  cbrooks]
 * 
 * 	Checkpoint:4.15.94
 * 	[1994/04/15  21:15:56  cbrooks]
 * 
 * 	Checkpoint
 * 	[1994/03/28  19:47:18  cbrooks]
 * 
 * Revision 1.1.10.1  1994/01/25  17:16:59  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/25  17:15:56  rsalz]
 * 
 * Revision 1.1.8.2  1993/07/14  20:23:23  tom
 * 	Fix typo.
 * 	[1993/07/06  14:36:50  tom]
 * 
 * 	Bug 5146: Put KeyMgmtThread() under ifdef DTS_ENTITY_PRINCIPAL.
 * 	[1993/04/29  20:40:45  tom]
 * 
 * 	Bug 7016: Don't complain about type already registered during an
 * 	enable of a global server.
 * 	[1993/04/29  20:28:22  tom]
 * 
 * Revision 1.1.7.2  1993/07/08  20:28:47  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:28:15  root]
 * 
 * Revision 1.1.5.2  1993/03/24  14:17:05  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/24  14:16:02  truitt]
 * 
 * Revision 1.1.1.8  1993/03/11  22:35:48  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.3  1993/01/08  15:08:00  smythe
 * 	fixed dtsd authentication bug by moving CreateSecurityCntxt call
 * 	from dtss_service_ncl.c to transport_rpc.c (OSF OT 5050 and 6763)
 * 	[1993/01/08  15:05:41  smythe]
 * 
 * Revision 1.1.5.5  1993/01/04  19:35:35  yankes
 * 	This fixes OT5050 (dtsd loses authentication) by moving the call to
 * 	CreateSecurityCntxt into transport_rpc.c so it can be called before
 * 	making any RPC calls.
 * 	[1993/01/04  19:13:01  yankes]
 * 
 * Revision 1.1.5.4  1992/12/30  16:40:29  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:35:48  htf]
 * 
 * Revision 1.1.3.2  1992/12/22  00:18:48  smythe
 * 	added server_listen change for OSF OT 5043 fix
 * 	[1992/12/21  20:35:25  smythe]
 * 
 * Revision 1.1.5.3  1992/12/18  17:03:01  tom
 * 	Bug 5043 - close sync window between dtsd and dtscp.
 * 	verify that the rpc_server_listen() thread is really listening.
 * 	[1992/12/18  17:02:35  tom]
 * 
 * Revision 1.1.5.2  1992/11/06  20:27:13  yankes
 * 	This checkin fixes OT 3080 by adding the #if CONFIG_SECURITY conditional
 * 	   around the prototyping of ValidateServerIdentity.
 * 	[1992/11/06  17:03:40  yankes]
 * 
 * Revision 1.1.1.7  1992/09/21  18:57:25  osfrcs
 * 	    Loading drop Dce1_0_1fnl
 * 
 * Revision 1.1.2.3  1992/05/11  14:54:40  ko
 * 	fix ot 2150, use OLD_LOG_EVENT
 * 	[1992/05/07  15:44:40  ko]
 * 
 * Revision 1.1.2.2  1992/04/21  17:39:42  comuzzi
 * 	Fixed double equal sign in assignment, added type casts
 * 	to eliminate warnings>>>
 * 	[1992/04/21  15:45:44  comuzzi]
 * 
 * Revision 1.1  1992/01/19  15:34:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * 	Module TRANSPORT_RPC.C
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
 */

#include <dce/dce.h>
#include <dts.h>
#include <dtss_includes.h>
#include <dtss_config.h>
#include <dtss_service.h>
#include <dce/binding.h>
#include <dce/pgo.h>
#include <dce/secidmap.h>
#include <dce/keymgmt.h>
#include <dcedtssvc.h>
#include <dcedtsmsg.h>
#include <dcedtsmac.h>

#ifndef HPDCE_FIX_CHFts14912
#include <dce/assert.h>
#include <dtssmacros.h>
#endif

#define K_LISTEN_SLEEP        5   /* Wait 5 seconds between listen polls */
#define K_MAX_LISTEN_COUNT    30   /* Poll up to 30 times if server_listen */

extern RpcBuf *rpcBufPtr;
extern ShmBuf	*shmPtr;

/*
 * One port descriptor for RPC.  This is used only to indicate that a given
 * server's transport type is RPC.  All RPC binding information is actually
 * contained in the binding descriptor linked to the server's cache element.
 */
PortDsc		*rpcPortPtr;

static pthread_t    TSthread;

typedef struct secMgmt_t
{
    unsigned long threadState;
    pthread_t     secThread_h;
} secMgmt_t;

/*
 * Values for threadState of key management and identity refresh
 * threads.
 */
#define SEC_MGMT_THREAD_CREATED	1
#define SEC_MGMT_THREAD_STOPPED	0

#define TEN_MINUTES (10 * 60)

#ifndef MIN
#define MIN(a, b)  (a) > (b) ? (b):(a)
#endif

/*
 * these macros check security-returned status
 */

#define GOOD_STATUS(stp)   ((*stp) == error_status_ok)
#define BAD_STATUS(stp)    ((*stp) != error_status_ok)

/*
 * Declare prototypes
 */

pthread_addr_t CreateRPClisten (pthread_addr_t dummyArg);

pthread_addr_t refreshIdentThread (pthread_addr_t argPtr);

#if DTS_ENTITY_PRINCIPAL
pthread_addr_t KeyMgmtThread (pthread_addr_t );
#endif

#if CONFIG_SECURITY
void ValidateServerIdentity (sec_login_handle_t ,
			     unsigned_char_t    *,
			     unsigned32         *);

#endif

void UnexportMgmtService (void);

void dts_write_bindings (error_status_t *);
void dts_remove_bindings_file (void);
static void ExportMgtServiceToNS( unsigned32 * ) ;

error_status_t mgt_acl_init (void);

void mgt_unregister_acl_mgr_if ( rpc_binding_vector_t * , uuid_vector_t * ,
	error_status_t * );

void mgt_unexport_acl_mgr_if ( unsigned_char_t *, uuid_vector_t *,
			error_status_t *);

void mgt_register_acl_mgr_if ( rpc_binding_vector_t *, uuid_vector_t *,
     error_status_t *);


void mgt_export_acl_mgr_if ( unsigned_char_t *, rpc_binding_vector_t *,
			    uuid_vector_t *, error_status_t *);

void get_security_config_info (sec_id_t *, sec_id_t *,error_status_t *);

#if     defined(DCE_AUD_POINTS)
void dts_aud_init(void);
#endif /* defined(DCE_AUD_POINTS) */

globalref time_service_v1_0_epv_t time_service_epv;
globalref gbl_time_service_v1_0_epv_t gbl_time_service_epv;
globalref time_control_v2_0_epv_t time_control_epv;
globalref serviceability_v1_0_epv_t dts_svc_ep;


/*
 *++
 *  InitServerRPC ()
 *    This routine is responsible for creating the thread to do one-time
 *    Initializations at startup.  The thread will also listen for
 *    remote calls to any of the procedures supported by the interfaces
 *    registered with the RPC run-time library.
 *--
 */
void InitServerRPC (void)
{
    unsigned32 listen_status;
    int        count;
    boolean32 is_listening = false ;
    extern pthread_attr_t thread_attribute;

    /* begin */

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	   "InitServerRPC:  Initializing Service RPC\n"));

    /*
     * Create a separate thread to initalize DTSS with the RPC run-time
     * library and to listen for remote calls.
     */

    if (pthread_create (&TSthread,
			thread_attribute,
			(pthread_startroutine_t)CreateRPClisten,
			(pthread_addr_t)NULL) < 0)
    {
	BugCheckOSstatus (errno);	/* for debugging */
    }

    /*
     * Make sure the server is listening before we
     * return.
     */

    count = 0;
    do {
	struct timespec t = { (unsigned long)K_LISTEN_SLEEP, 0L } ;
	int v ;
	v = pthread_delay_np( &t ) ;
	/*sleep(K_LISTEN_SLEEP);*/
	is_listening = 
	    rpc_mgmt_is_server_listening( (rpc_binding_handle_t) NULL,
					 &listen_status);
	count++;
    } while (!is_listening && listen_status == rpc_s_ok &&
	     count < K_MAX_LISTEN_COUNT);

    /*
     * If we are not listening, we are in the grasp of some sort of
     * compute loop or RPC bug.
     */

    /* 
     * either: is_listening OR listen_status != rpc_s_ok OR
     * count >= K_MAX_LISTEN_COUNT
     */

    if (! is_listening)		/* we aren't listening yet  */
    {
	 
        /* rpc_mgmt_is_server... returns FALSE with error message */
	if (listen_status != rpc_s_ok) {

	    dce_error_string_t tempString;
	    dce_svc_printf( DTS_S_MGMT_SRVR_LISTENING_MSG, 
			   dce_dts_error_text( listen_status,tempString));
	}
	else 
	{
	    dce_svc_printf( DTS_S_MGMT_SRVR_LISTEN_TIMEOUT_MSG, K_MAX_LISTEN_COUNT * K_LISTEN_SLEEP);
	}
#ifndef HPDCE_FIX_CHFts14912
	dce_assert((struct dce_svc_handle_s_t *)0, is_listening);
#else
	BugCheck();
#endif
    }
    /*
     * ...else rpc_server_listen is O.K.
     */
} /* end of routine InitServerRPC  */


/*
 *++
 *  CreateRPClisten ()
 *      This thread is called created by InitServerRPC.  Its function
 *      is to register the Control Program (CP) requests server with the RPC
 *      and the protocol sequences supported by the CP and Time Service
 *      interfaces.  This thread also calls rpc_server_listen to start
 *      accepting remote calls.
 *  Inputs:
 *	dummyArg :  Sanity check argument.  Not needed in this routine
 *                  but we pass it to the thread to be consistent with
 *                  Pthreads creation mechanism.
 *--
 */
/*ARGSUSED*/
pthread_addr_t CreateRPClisten(
    pthread_addr_t dummyArg
)
{
    unsigned32      st = rpc_s_ok ;

    /* begin */

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		   "CreateRPCListen: registering interfaces...\n"));

    /* Register protocol sequences on which DTS will listen for RPCs. */

    rpc_server_use_all_protseqs(10, &st);

    /* Register interfaces. */

    rpc_server_register_if(time_control_v2_0_s_ifspec,
	(uuid_t *)NULL, (rpc_mgr_epv_t)&time_control_epv, &st);
    if (st != rpc_s_ok)
    {
	dce_error_string_t tempString ;
	dce_svc_printf( DTS_S_SRVR_CONTROLV2_MSG,
		       dce_dts_error_text( st, tempString));
    }
    

    rpc_server_register_if(serviceability_v1_0_s_ifspec, NULL,
	(rpc_mgr_epv_t)&dts_svc_ep, &st);
    if (st != rpc_s_ok)
    {
	dce_error_string_t tempString ;

	dce_svc_printf( DTS_S_SRVR_SERVICEABILITY_MSG, 
		       dce_dts_error_text( st, tempString));
    }
    

    rpc_server_inq_bindings(&rpcBufPtr->bindingVector, &st);
    if (st != rpc_s_ok)
    {
	dce_error_string_t tempString ;	

	dce_svc_printf( DTS_S_INQ_BINDINGS_MSG, 
		       dce_dts_error_text( st, tempString ));
    }

    /* Register endpoints. */

    rpc_ep_register(time_control_v2_0_s_ifspec, rpcBufPtr->bindingVector,
	(uuid_vector_p_t)NULL, (unsigned char *)"Time Service Control", &st);
    if (st != rpc_s_ok)
    {
	dce_error_string_t tempString ;	

	dce_svc_printf( DTS_S_EP_REGISTER_CONTROLV2_MSG, 
		       dce_dts_error_text( st, tempString ));
    }
    

    rpc_ep_register(serviceability_v1_0_s_ifspec, rpcBufPtr->bindingVector,
	&rpcBufPtr->objUUIDvector, (unsigned_char_p_t)"Time SVC", &st);
    if (st != rpc_s_ok)
    {
	dce_error_string_t tempString ;

	dce_svc_printf( DTS_S_EP_REGISTER_SERVICEABILITY_MSG, 
		       dce_dts_error_text( st, tempString ));
    }
    

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		   "CreateRPCListen: write bindings...\n"));

    dts_write_bindings( &st ) ;
    if (st != error_status_ok)  
    {
	dce_error_string_t tempstring;
	dce_svc_printf( DTS_S_WRITE_BINDINGS_MSG, 
		       (st == dts_s_binding_file_write_error) ? 
		       strerror( errno ) : dce_dts_error_text( st, tempstring ));
    }

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		   "CreateRPCListen: Export Control Interface to NSI\n"));

    ExportMgtServiceToNS( &st ) ;
    if (st != rpc_s_ok) 
    {
	dce_error_string_t tempString ;

	dce_svc_printf( DTS_S_EXPORT_CONTROL_MSG, 
		       dce_dts_error_text( st, tempString ));
    }

#if     defined(DCE_AUD_POINTS)
    dts_aud_init();
#endif  /* defined(DCE_AUD_POINTS) */

    /* Listen for RPC calls from clients.  There are two types of
     * clients that can send RPC requests to a DTSS daemon, depending on
     * whether the Time Service was configured as a clerk or a server.
     * Requests from a Control Program client are acknowleged by both
     * clerks and servers.  Time requests from a DTSS clients are acknowledged
     * ONLY by time servers. */

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		   "CreateRPCListen: calling rpc_server_listen\n"));
    rpc_server_listen(K_RPC_MAX_CALLS, &st);
    /*
     * assume at this point that we have received an
     * rpc_mgmt_stop_server_listening() call
     * 
     * behave as if we had seen a "dts stop" command or a SIGTERM
     */
     GenEvent(ETterminate) ;
     /* Kill the function and the thread will die */
}

/*
 *++
 *  RegisterServerRPC ()
 *      This routine is executed as a result of an ENABLE command
 *      when the Time Service is running as a server.  It registers
 *      the interface with the RPC rtl to make it available to
 *      clients that are trying to use it.
 *--
 */
StatusBlk RegisterTServerRPC (void)
{
    StatusBlk     statusBlk;
    uuid_t        DTSS_uuid;          /* UUID for this interface */
    unsigned32    status;

    statusBlk.status = DTSS_SUCCESS;  /* initially assume success */

	DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "DTSS server registering with RPC rtl...\n"));

    uuid_create_nil (&DTSS_uuid, &status);

    /*
     * Register the Time Request interface for local servers with the
     * RPC run-time library
     */
    rpc_server_register_if (time_service_v1_0_s_ifspec,
			    &DTSS_uuid,
			    (rpc_mgr_epv_t) &time_service_epv,
			    &status);
    if (status != rpc_s_ok)
    {
	statusBlk.status = K_DCE_ERROR;
	statusBlk.sysStatus = status;
	return (statusBlk);
    }

    /*
     * Register the Time Request interface for global servers with the
     * RPC run-time library, if this server has already been advertised.
     */

    if (shmPtr->mgtParams.servType == K_SERVER_GLOBAL)
    {
	rpc_server_register_if (gbl_time_service_v1_0_s_ifspec,
				(uuid_p_t)NULL,
				(rpc_mgr_epv_t) &gbl_time_service_epv,
				&status);
	/*
	 * Don't complain if this was done (via an advertise command) 
	 * when we were disabled.
	 */
	if (status != rpc_s_ok && 
	    status != rpc_s_type_already_registered)
	{
	    statusBlk.status = K_DCE_ERROR;
	    statusBlk.sysStatus = status;
	    return (statusBlk);
	}

    }

    return(statusBlk);
}

#if CONFIG_SECURITY
/*
 *++
 *  CreateSecurityCntxt ()
 *--
 */

StatusBlk CreateSecurityCntxt (void)
{
    StatusBlk     	    statusBlk;
    unsigned32    	    secStatus;
    static unsigned_char_t  *principalName;
    sec_login_handle_t      temp_context;

    statusBlk.status = DTSS_SUCCESS;
    principalName = (unsigned_char_t *)NULL;

#if DTS_ENTITY_PRINCIPAL
    /*
     * NOTE: This code is included only if the dts server is running with a
     *       name /.:/hosts/<node>/dts-entity (not under the machine principal)
     *       See note in dtss_rpc.h
     */

    if (IsServer())
    {
	/*
	 * check if key management or refresh identity threads are already
	 * running. If so, cancel the threads and detach them (to release
	 * allocated memory). The only case in which this condition occurrs is
	 * if  CreateSecurityCntxt() is called again, as a result of a
	 * management action that changed the server's principal name.
	 * Normally, this action does not occurr.
	 */

	if (keyMgmt.threadState == SEC_MGMT_THREAD_CREATED)
	{
	    pthread_addr_t join_status;

	    if (pthread_cancel (keyMgmt.secThread_h))
		BugCheckOSstatus (errno);	/* for debugging */

	    if (pthread_join (keyMgmt.secThread_h, &join_status))
		BugCheckOSstatus (errno);	/* for debugging */

	    if (pthread_detach (&keyMgmt.secThread_h))
		BugCheckOSstatus (errno);	/* for debugging */
	}

	/*
	 * Check state of refresh identity thread.
	 */
	if (refreshIdent.threadState == SEC_MGMT_THREAD_CREATED)
	{
	    pthread_addr_t join_status;

	    if (pthread_cancel (refreshIdent.secThread_h))
		BugCheckOSstatus (errno);	/* for debugging */

	    if (pthread_join (refreshIdent.secThread_h, &join_status))
		BugCheckOSstatus (errno);	/* for debugging */

	    if (pthread_detach (&refreshIdent.secThread_h))
		BugCheckOSstatus (errno);	/* for debugging */
	}
    }

#endif /* DTS_ENTITY_PRINCIPAL */

    /*
     * Select the principal name for a server or a clerk.  If the Time Service
     * entity is running as a server, get the principal name assigned in the
     * security namespace.  If it is a clerk , get the machine's principal
     * name.
     * For a server, allocate memory, since it will be passed down to the
     * identity refresh thread, as an argument.
     *
     */
    if (IsServer())
    {
	if (principalName != NULL)
	    DTSSfree((VOID *)principalName);

	principalName =
	   (unsigned_char_t *)DTSSmalloc (
			strlen((char *)shmPtr->mgtParams.servPrincName)+1);
	strcpy((char *)principalName,
	       (char *)shmPtr->mgtParams.servPrincName);
    }
    else
	principalName = rpcBufPtr->host_principal.name;

    /*
     * If this condition is true, the login context was probably already
     * allocated by security.  That means that CreateSecurityCntxt() is
     * being called again because of a change in the principal name.  The
     * old login context is no longer valid, so it will be purged.  Set
     * the current login context to null in case the time provider
     * thread access it at the instant memory is being deallocated.
     *
     * NOTE: Since a principal name can only be changed on a server entity, this
     *       condition will not occurr if the time service is running as
     *       a clerk.
     */

    if (rpcBufPtr->loginContext_h != NULL)
    {
	temp_context = rpcBufPtr->loginContext_h;
	rpcBufPtr->loginContext_h = (sec_login_handle_t)NULL;

#if DTS_ENTITY_PRINCIPAL
        /*
         * If dts is configured to run as /dts-entity, purge the context,
	 * which also destroys the network credentials.  Otherwise, if dts
	 * is configured to run as the machine principal, just release the
	 * context so that the credentials are kept.
	 */
	sec_login_purge_context (&temp_context, &secStatus);
#else
	sec_login_release_context (&temp_context, &secStatus);
#endif
    }

#if DTS_ENTITY_PRINCIPAL
    /*
     * Set up the network entity's identity.
     */

    sec_login_setup_identity (principalName,
			      sec_login_no_flags,
			      &rpcBufPtr->loginContext_h,
			      &secStatus);
    }
    if (BAD_STATUS(&secStatus))
    {
        statusBlk.status = K_DCE_ERROR;
        statusBlk.sysStatus = secStatus;
        return(statusBlk);
    }

    /*
     * Validate the identity for this principal.
     */
    ValidateServerIdentity (rpcBufPtr->loginContext_h,
			    principalName,
			    &secStatus);

    if (BAD_STATUS(&secStatus))
    {
	statusBlk.status = K_DCE_ERROR;
	statusBlk.sysStatus = secStatus;
	return(statusBlk);
    }

    /*
     * Set the network credentials to those specified in the
     * login context above.
     */

    sec_login_set_context (rpcBufPtr->loginContext_h,
			   &secStatus);
    if (BAD_STATUS(&secStatus))
    {
	statusBlk.status = K_DCE_ERROR;
	statusBlk.sysStatus = secStatus;
	return(statusBlk);
    }

    /*
     * NOTE: This code is included only if the dts server is running with a
     *       name /.:/hosts/<node>/dts-entity (not under the machine principal)
     *       See note in dtss_rpc.h
     */

    if (shmPtr->mgtParams.variant == VTserver)
    {
	/*
	 * Register authentication information to be used with the
	 * RPC runtime.
	 */
	rpc_server_register_auth_info( shmPtr->mgtParams.servPrincName,
				       rpc_c_authn_dce_private,
				      (rpc_auth_key_retrieval_fn_t)NULL,
				      (idl_void_p_t)NULL,
				       &secStatus);
	if (BAD_STATUS(&secStatus))
	{
	    statusBlk.status = K_DCE_ERROR;
	    statusBlk.sysStatus = secStatus;
	    return(statusBlk);
	}

	/*
	 * Create a seperate thread which will change the principal's
	 * key on a regular basis, determined by the local cell's policy.
	 * Set the flag to indicate that a key management will be ruinning.
	 */

	keyMgmt.threadState = SEC_MGMT_THREAD_CREATED;
	if (pthread_create (&keyMgmt.secThread_h,
			    thread_attribute,
			    (pthread_startroutine_t)KeyMgmtThread,
			    (pthread_addr_t)NULL) < 0){
	    BugCheckOSstatus (errno);	/* for debugging */
	}

	/*
	 * Create a seperate thread which refreshes the server't identity
	 * periodically.  The period of time when its identity needs to be
	 * refreshed is determined by administrative policy set up for the
	 * account corresponding to this principal.
	 */
	refreshIdent.threadState = SEC_MGMT_THREAD_CREATED;

	if (pthread_create (&refreshIdent.secThread_h,
			    thread_attribute,
			    (pthread_startroutine_t)refreshIdentThread,
			    (pthread_addr_t)principalName) < 0){
	    BugCheckOSstatus (errno);	/* for debugging */
	}
    } /* end if */

#else  /* The server is configured to run as the machine principal */

    /*
     * get the current context, which it is assumed to be the machine's
     * principal context.  The daemon sec_clientd must already be running.
     */
    sec_login_get_current_context(&rpcBufPtr->loginContext_h,
				  &secStatus);

    if (GOOD_STATUS(&secStatus)) {
            /*
             * Make sure we get the real default machine context - so
             * release the context we just picked up.  (The current default
             * context may belong to an interactive user who has a DCE
             * identity established.)  By releasing the context and
             * re-obtaining the default context we are guaranteed to
             * fall back to getting the machine context (this is only
             * true because we are running as the local privileged user).
             */
            sec_login_release_context(&rpcBufPtr->loginContext_h,
				      &secStatus);
            sec_login_get_current_context(&rpcBufPtr->loginContext_h,
					  &secStatus);
    	    if (BAD_STATUS(&secStatus))
    	    {
                statusBlk.status = K_DCE_ERROR;
        	statusBlk.sysStatus = secStatus;
        	return(statusBlk);
    	    }
    }
    else if (BAD_STATUS(&secStatus))
    {
        statusBlk.status = K_DCE_ERROR;
        statusBlk.sysStatus = secStatus;
        return(statusBlk);
    }

#endif  /* DTS_ENTITY_PRINCIPAL */

    return (statusBlk);
} /* End of CreateSecurityCntxt */


#if DTS_ENTITY_PRINCIPAL
/*
 *++
 *  KeyMgmtThread ()
 *     This thread calls the key management routine sec_key_mgmt_manage_key
 *     whose primary function is to change the principal's key on a regular
 *     basis as determined by the local cell's policy.  This function runs
 *     indefinitely and, under normal operation, does not terminate.  If the
 *     network manager decides to change the server's principal name while
 *     the Time Service is running, this thread will be cancelled and another
 *     thread created with the new principal name.
 *  Inputs:
 *	dummyArg :  Sanity check argument.  Not needed in this routine
 *                  but we pass it to the thread to be consistent with
 *                  Pthreads creation mechanism.
 *--
 */
/*ARGSUSED*/
pthread_addr_t KeyMgmtThread ( 
    pthread_addr_t dummyArg
)
{
    unsigned32 secStatus;

    sec_key_mgmt_manage_key (rpc_c_authn_dce_private,
			     NULL,
			     shmPtr->mgtParams.servPrincName,
			     &secStatus);
    if (BAD_STATUS(&secStatus))
    {
	
	dce_error_string_t tempstring;
	dce_svc_printf( DTS_S_SEC_KEY_MGMT_MSG, 
		       dce_dts_error_text( secStatus, tempstring));
    }

    return ((pthread_addr_t)NULL);
}
#endif


/*
 *++
 *  refreshIdentThread ()
 *    This thread refreshes the server's identity periodically, according
 *    to the expiration policy set up for the account corresponding to
 *    this servers's principal.  The thread determines when the identity
 *    expires by calling sec_login_get_expiration
 *  Inputs:
 *	dummyArg :  Sanity check argument.  Not needed in this routine
 *                  but we pass it to the thread to be consistent with
 *                  Pthreads creation mechanism.
 *--
 */
/*ARGSUSED*/
pthread_addr_t refreshIdentThread (
    pthread_addr_t argPtr
)
{
    unsigned32           interval;
    signed32		 identExpiration;
    error_status_t       status;
    unsigned_char_t      *principalName;
    struct timespec      currTime, timer;
    utc_t                now;

    principalName = (unsigned_char_t *)argPtr;

    status = error_status_ok;

    while (GOOD_STATUS(&status))
    {
	utc_gettime(&now);
	utc_bintime(&currTime, (struct timespec *)0, (long *)0, &now);

	/*
	 * Obtain identity expiration time for this server principal.
	 * If the identity has no expiration time, it will return zero.
	 */

	sec_login_get_expiration (rpcBufPtr->loginContext_h, &identExpiration,
				  &status);
	if (GOOD_STATUS(&status) || status == sec_login_s_not_certified)
	{
	    if (identExpiration == 0)
	    {
	       /*
		* Set up default timer to 10 minutes.  If identity has no
		* expiration date,  we don't want to stall the thread forever,
		* since expiration policy might be changed by the administrator
		* to a finite time.  Instead,  check every 10 minutes and
		* refresh identity if needed.
		*/
		timer.tv_sec = TEN_MINUTES;
		timer.tv_nsec = 0;
	    }
	    else
	    {
		interval = identExpiration - currTime.tv_sec;
		timer.tv_sec = (interval- MIN(TEN_MINUTES,(signed32)interval/2));
	    }

	    if (timer.tv_sec > 0)
		pthread_delay_np(&timer);

	    sec_login_refresh_identity (rpcBufPtr->loginContext_h,
					&status);
	    if (GOOD_STATUS(&status))
	    {
	        ValidateServerIdentity (rpcBufPtr->loginContext_h,
				        principalName,
				        &status);
		if (BAD_STATUS(&status))
		{
		    dce_error_string_t tempString ;

		    dce_svc_printf( DTS_S_VALIDATE_SRVR_ID_MSG, 
				   dce_dts_error_text( status, tempString ));
		}
	    }
	    else
	    {
		dce_error_string_t tempString ;

		dce_svc_printf( DTS_S_REFRESH_IDENTITY_MSG, 
			       dce_dts_error_text( status, tempString ));
	    }

	}
	else
	{
	    dce_error_string_t tempString ;
	    

	    dce_svc_printf( DTS_S_GET_EXPIRATION_MSG,
			   dce_dts_error_text( status, tempString ));
	}
	
    } /* end while */

    return ((pthread_addr_t)NULL);
}


/*
 *++
 *  ValidateServerIdentity ()
 *   This routine extracts the key (or password) from the local key file,
 *   and validates this principal's identity.  The key is retrieved from
 *   the key file by looping through it until a key for this principal is
 *   successfully validated or until there are no more keys to try.
 *--
 */

void ValidateServerIdentity ( 
    sec_login_handle_t loginContext,
    unsigned_char_t    *principalName,
    unsigned32         *secStatus
)
{

    idl_void_p_t	cursor, keyData;
    unsigned32          key_vno = 0;
    unsigned_char_t     *name;
    unsigned32          st, status, valStatus;
    sec_passwd_type_t   keyType = sec_passwd_none;
    boolean32           reset_passwd = 0;
    sec_login_auth_src_t authSrc = 0;

    name = (unsigned_char_t *)NULL;
    st = status = error_status_ok;

    /*
     * Mark the beginning of the search for this principal.
     */
    sec_key_mgmt_initialize_cursor(rpc_c_authn_dce_private,
				   NULL,
				   (unsigned char *)principalName,
				   &keyType,
				   &cursor,
				   &status);
    if (GOOD_STATUS(&status))
    {
	while (GOOD_STATUS(&status))
	{
	    /*
	     * Get the "best" for this principal.  Normally this means,
	     * the latest key that was added to the keytab file.  Try
	     * to validate the identity with the new key.  Loop until a
	     * key succeeds or there are no more keys to try.
	     */
	    sec_key_mgmt_get_next_key(cursor,
				      &name,
				      &key_vno,
				      &keyData,
				      &status );
	    if(status != sec_key_mgmt_e_key_unavailable && GOOD_STATUS(&status))
	    {
		sec_login_validate_identity (loginContext,
					     keyData,
					     &reset_passwd,
					     &authSrc,
					     &valStatus);
		free(name);
		sec_key_mgmt_free_key(keyData, &st);
		if (GOOD_STATUS(&valStatus))
		{
		    *secStatus = valStatus;
		    sec_key_mgmt_release_cursor(&cursor, &st);
		    return;
		}
	    }
	} /* end while */

	sec_key_mgmt_release_cursor(&cursor, &st);
    }

    *secStatus = status;
}

#endif /* CONFIG_SECURITY */


/*
 *++
 *  ExportTServiceToNS ()
 *    This routine exports this server to the name-service data base
 *    making it available to clients on the LAN that send time requests.
 *    First, the server adds address information of its binding handles
 *    to the endpoint mapper database.  Then the server exports its interface
 *    to the name-service database, making it a member of the local set of
 *    time servers.  Configuration of the local set is by means of RPC
 *    profiles.  Server entries are added to the LAN profile, which is
 *    a CDS (Cell Directory Service) entry that contains the names of
 *    distributed services offered throughout the LAN.  First we look up
 *    the name of the LAN profile, beginning with the node's initial or
 *    default profile.  Assuming that there exists a well-known interface
 *    UUID associated with LAN services, we get the name of the LAN
 *    profile.  Subsequently, perform an RPC binding export to this
 *    LAN profile.
 *  Value Returned:
 *	StatusBlk
 *--
 */

StatusBlk ExportTServiceToNS (void)
{
    StatusBlk     statusBlk;
    unsigned32    status, RPCstatus;

    unsigned_char_p_t  lanProfileName;
    rpc_if_id_t        serverInterfaceID;

    statusBlk.status = DTSS_SUCCESS;
    lanProfileName = (unsigned_char_p_t)NULL;

    /*
     * Find the name of the LAN profile to which the server's bindings
     * and name will be exported.
     */

    GetProfileName (lan_profile_v1_0_c_ifspec,
		    &lanProfileName,
		    &RPCstatus);

    if (RPCstatus != rpc_s_ok)
    {
	/*
	 * Profile loop detected, profile lacking a default is
	 * found or profile found but default entry does not refer to
	 * another profile.
	 */

	LogEventNoLANProfile();
	statusBlk.status = K_DCE_ERROR;
	statusBlk.sysStatus = RPCstatus;
#ifndef HPDCE_FIX_CHFts16224
        rpc_string_free (&lanProfileName, &status);
#endif
        return(statusBlk);
    }

    /*
     * Add server address information to the endpoint mapper database.
     */
    rpc_ep_register (time_service_v1_0_s_ifspec,
		     rpcBufPtr->bindingVector,
		     &rpcBufPtr->objUUIDvector,
		     (unsigned_char_p_t)"Time Service",
		     &status);

    if (status != rpc_s_ok)
    {
	dce_error_string_t tempString ;

	dce_svc_printf( DTS_S_TSERVICEV1_EP_REGISTER_MSG, 
		       dce_dts_error_text( status, tempString ));

	statusBlk.status    = K_DCE_ERROR;
	statusBlk.sysStatus = status;
#ifndef HPDCE_FIX_CHFts16224
        rpc_string_free (&lanProfileName, &status);
#endif
	return( statusBlk );
    }

    /*
     * First, attempt to remove from the nameservice database
     * all binding handles corresponding to the time service
     * interface UUID.
     */
    rpc_ns_binding_unexport (rpc_c_ns_syntax_dce,
			     rpcBufPtr->TServiceEntryName,
			     time_service_v1_0_s_ifspec,
			     &rpcBufPtr->objUUIDvector,
			     &status);


    /*
     * Export the server's bindings to the node entry.
     */

    rpc_ns_binding_export (rpc_c_ns_syntax_dce,
			   rpcBufPtr->TServiceEntryName,
			   time_service_v1_0_s_ifspec,
			   rpcBufPtr->bindingVector,
			   &rpcBufPtr->objUUIDvector,
			   &status);

    if (status != rpc_s_ok)
    {
	dce_error_string_t tempString ;

	dce_svc_printf( DTS_S_EXPORT_TSERVICEV1_MSG, 
		       dce_dts_error_text( status, tempString ));
	statusBlk.status    = K_DCE_ERROR;
	statusBlk.sysStatus = status;
#ifndef HPDCE_FIX_CHFts16224
        rpc_string_free (&lanProfileName, &status);
#endif
	return( statusBlk );
    }

    /*
     * Create entry in the LAN profile for this server.
     */

    rpc_if_inq_id (time_service_v1_0_s_ifspec,
		   &serverInterfaceID,
		   &status);

    rpc_ns_profile_elt_add (rpc_c_ns_syntax_dce,
			    lanProfileName,
			    &serverInterfaceID,
			    rpc_c_ns_syntax_dce,
			    rpcBufPtr->TServiceEntryName,
			    0,
			    (unsigned_char_p_t)"Time Server entry",
			    &status);
    if (status != rpc_s_ok &&
	status != rpc_s_profile_element_replaced)
    {
	dce_error_string_t tempString ;

	dce_svc_printf( DTS_S_TSERVICEV1_PROFILE_ADD_MSG, lanProfileName, 
		       dce_dts_error_text( status, tempString));
	statusBlk.status    = DTSS_ERROR;
	statusBlk.sysStatus = status;
    }

    /*
     * Deallocate storage for strings
     */

    rpc_string_free (&lanProfileName, &status);

    return (statusBlk);
}

/*
 * ExportMgtServiceToNS() 
 * export the bindings for the control interface to the Name Server.
 * 
 */

static void ExportMgtServiceToNS( unsigned32 * statusp)
{
    unsigned32   localstatus = rpc_s_ok ;


    /* begin */

    /*
     * make sure the name is initialized 
     */ 

    strcpy((char *)rpcBufPtr->TServiceEntryName,
	   (char *)CELL_RELATIVE_PREFIX);

    strcat ((char *)rpcBufPtr->TServiceEntryName,
            (char *)shmPtr->mgtParams.servEntryName);


    rpc_ns_binding_unexport (rpc_c_ns_syntax_dce,
			     rpcBufPtr->TServiceEntryName,
			     time_control_v2_0_s_ifspec,
			     &rpcBufPtr->objUUIDvector,
			     &localstatus);

    /* ignore error if we can't unexport the name */

    rpc_ns_binding_export (rpc_c_ns_syntax_dce,
			   rpcBufPtr->TServiceEntryName,
			   time_control_v2_0_s_ifspec,
			   rpcBufPtr->bindingVector,
			   &rpcBufPtr->objUUIDvector,
			   statusp);

    if (*statusp != rpc_s_ok)
    {
	dce_error_string_t tempString ;

	dce_svc_printf( DTS_S_EXPORT_TSERVICEV2_MSG, 
		       dce_dts_error_text( *statusp, tempString ));
    }
    return ;
}


/*
 *++
 *  GetProfileName ()
 *     This routine obtains the name of a profile given the interface UUID.
 *     The possible profiles returned are either the LAN or CELL services
 *     profiles.
 *  Inputs:
 *      InterfaceUUID :  The interface specification of the service sought.
 *  Outputs:
 *      profileName: Name of the profile that matches the interface UUID.
 *	RPCstatus :  status value returned by RPC NSI calls.
 *--
 */

void GetProfileName (
    rpc_if_handle_t     interfaceID,
    unsigned_char_p_t  *profileName,
    unsigned32         *RPCstatus
)
{

    unsigned32              status;
    rpc_ns_handle_t         profContext;
    unsigned_char_p_t       currProfileList[64];
    rpc_if_id_t             UUID, tempUUID;
    int                     i, pCounter;
    unsigned32              priority;
    unsigned32              expirationAge;
    unsigned short          profileFound;
    unsigned_char_t         nodeInitialProfile[K_MAX_ENTRY_LENGTH];
    char                    *nodeProfile;

    pCounter = 0;

    /*
     * obtain the name of the default profile where the search will start.
     */
    dce_cf_profile_entry_from_host ((char *)NULL,
				    &nodeProfile, RPCstatus);
    if (*RPCstatus != error_status_ok)
    {
	dce_error_string_t tempString ;

	dce_svc_printf( DTS_S_CF_PROFILE_NAME_MSG, 
		       dce_dts_error_text( *RPCstatus, tempString));

      return;
    }

    strcpy((char *)nodeInitialProfile, (char *)CELL_RELATIVE_PREFIX);
    strcat((char *)nodeInitialProfile, nodeProfile);

    currProfileList[pCounter] = (unsigned_char_p_t)nodeInitialProfile;
    profileFound   = FALSE;

    /*
     * Obtain the UUID part of the interface specification.
     */

    rpc_if_inq_id (interfaceID,
		   &UUID,
		   &status);

    while (! profileFound)
    {
	/*
	 * Traverse the current profile looking for the name that
	 * matches the UUID.
	 */
	rpc_ns_profile_elt_inq_begin (rpc_c_ns_syntax_dce,
				      currProfileList[pCounter],
				      rpc_c_profile_match_by_if,
				      &UUID,
				      rpc_c_vers_all,
				      rpc_c_ns_syntax_dce,
				      NULL,
				      &profContext,
				      RPCstatus);
	if (*RPCstatus != rpc_s_ok)
	{
	    dce_error_string_t tempString ;	    

	    dce_svc_printf( DTS_S_PROFILE_BEGIN_MSG, 
			   dce_dts_error_text( *RPCstatus, tempString));
	    return;
	}

	/*
	 * disable CDS caching
	 */
	expirationAge = 0;
	rpc_ns_mgmt_handle_set_exp_age (profContext,
					expirationAge,
					&status);
	/*
	 * retrieve the next element in the profile and see if it
	 * corresponds to the UUID.
	 */
	rpc_ns_profile_elt_inq_next (profContext,
				     &tempUUID,
				     profileName,
				     &priority,
				     NULL,
				     RPCstatus);

	rpc_ns_profile_elt_inq_done ( &profContext,
				      &status);

	if (*RPCstatus == rpc_s_ok)
	    profileFound = TRUE;

	else if (*RPCstatus == rpc_s_no_more_elements)
	{
	    /*
	     * See if there is a default profile
	     */
	    rpc_ns_profile_elt_inq_begin (rpc_c_ns_syntax_dce,
					  currProfileList[pCounter],
					  rpc_c_profile_default_elt,
					  (rpc_if_id_p_t)NULL,
					  0,
					  rpc_c_ns_syntax_dce,
					  NULL,
					  &profContext,
					  RPCstatus);
	    /*
	     * if failure, profile does not exist or can't access it.
	     */

	    if (*RPCstatus != rpc_s_ok)
		goto CLEANUP;

	    /*
	     * Otherwise we have a default profile. No need to re-disable
	     * CDS caching since we have current data.  Get name of default
	     * profile.
	     */

	    rpc_ns_profile_elt_inq_next (profContext,
					 &tempUUID,
					 &currProfileList[++pCounter],
					 &priority,
					 NULL,
					 RPCstatus);

	    rpc_ns_profile_elt_inq_done (&profContext,
					 &status);

	    /*
	     * if failure, the profile does not have a default entry;
	     * we are at the top of the heap.
	     */
	    if (*RPCstatus != rpc_s_ok)
		goto CLEANUP;

	    /*
	     * Search the list for the default profile name we just obtained.
	     * If found in the list, a cycle is present in the profiles.
	     * Return an rpc_s_cycle_detected error.
	     */

	    if (*RPCstatus == rpc_s_ok)
	    {
		unsigned_char_p_t currProfilePtr;
		int               count;

		currProfilePtr = currProfileList[pCounter];

		for (count=0; count < pCounter;)
		{
		    /*
		     * NOTE: strcmp() is used here only temporarily. When
		     *       the profile_compare routines are ready, we will
		     *       use those instead.
		     */

		    if (strcmp( (char *)currProfileList[count++],
                                (char *)currProfilePtr) == 0)
		    {
			*RPCstatus = rpc_s_cycle_detected;
			goto CLEANUP;
		    }
		}
	    }

	} /* end if */

	else
	{
	    /*
	     * There was an error in NSI while attempting to obain the
	     * next element in a profile during the first pass.
	     */
	    goto CLEANUP;
	}

    } /* end while */

/*
 * CLEAN UP: free memory allocated for profile names
 */

    CLEANUP: ;

    /*
     * Start at i=1 because the first entry in the array was obtained
     * "manually" from the Node Entry Name.  Subsequenent entries are
     * returned by RPC, so memory needs to be deallocated.
     */
    for (i=1; i <= pCounter; )
	rpc_string_free (&currProfileList[i++], &status);

    free(nodeProfile);
} /* end of GetProfileName */


#if CONFIG_SECURITY
/*
 *++
 *  GetTServicePrincUUID ()
 *      Return the time service principal uuid, given the principal name.
 *  Inputs:
 *      servPrincName	the server principal name.
 *--
 */

StatusBlk GetTServicePrincUUID (
    unsigned_char_t   *servPrincName,
    uuid_t            *servPrincUUID
)
{
    StatusBlk        statusBlk;
    unsigned32       secStatus;

    statusBlk.status = DTSS_SUCCESS;

    /*
     * Get this server's UUID created by security.
     */

    sec_rgy_pgo_name_to_id (rpcBufPtr->secContext,
			    sec_rgy_domain_person,
			    (unsigned char *)servPrincName,
			    servPrincUUID,
			    &secStatus);

    if (BAD_STATUS(&secStatus))
    {
	statusBlk.status = K_DCE_ERROR;
	statusBlk.sysStatus = secStatus;
	return (statusBlk);
    }

    return (statusBlk);
}
#endif /* CONFIG_SECURITY */

/*
 *++
 *  UnregisterServerRPC ()
 *      This routine is executed as a result of a DISABLE command
 *      when the Time Service is running as a server.  It unregisters
 *      the interface with the RPC rtl so that it is no longer available
 *      to clients currently using it.
 *--
 */
StatusBlk UnregisterTServerRPC (void)
{
    StatusBlk     statusBlk;
    unsigned32    status;

    statusBlk.status = DTSS_SUCCESS;

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "Unregistering DTSS server with RPC rtl...\n"));

    rpc_server_unregister_if (time_service_v1_0_s_ifspec,
			      NULL,
			      &status);
    if (status != rpc_s_ok)
    {
	statusBlk.status    = K_DCE_ERROR;
	statusBlk.sysStatus = status;
    }

    if (shmPtr->mgtParams.servType == K_SERVER_GLOBAL)
    {
	rpc_server_unregister_if (gbl_time_service_v1_0_s_ifspec,
				  NULL,
				  &status);
	if (status != rpc_s_ok)
	{
	    statusBlk.status    = K_DCE_ERROR;
	    statusBlk.sysStatus = status;
	}

    }

    return(statusBlk);
}


/*
 *++
 *  InitRPCport ()
 *	This routine initializes an RPC port.  It creates the port descriptor.
 *	It's called when initializing the transport layer (InitTransport).
 *  Implicit Outputs:
 *      rpcPortPtr	points to the initialized RPC port.
 *  Side Effects:
 *      the RPC port descriptor is allocated.
 *--
 */
StatusBlk InitRPCport (void)
{
    StatusBlk	statusBlk;

    statusBlk.status = DTSS_ERROR;
    statusBlk.sysStatus = DTSS_FAILED;

    rpcPortPtr = DTSSmallocStruct (PortDsc);
    memset((VOID *)rpcPortPtr, 0, sizeof(PortDsc));
    rpcPortPtr->socketType = K_SOCKET_DTS_RPC;
    rpcPortPtr->owner_id = (1<<DTSS_SERVICE);
    rpcPortPtr->broadcast = FALSE;
    rpcPortPtr->datagram = FALSE;

    statusBlk.status = DTSS_SUCCESS;
    return (statusBlk);
}

/*
 *++
 *  CreateServerBinding ()
 *	This routine creates an RPC binding to a DTSS server.  It's called by
 *	AddServer when adding a server to a cache.  It creates a binding
 *	descriptor and then creates the binding for the given server name.
 *  Inputs:
 *	name		is the binding name.
 *	nameLen		is the length of the binding name.
 *  Value Returned:
 *	bindPtr		is a pointer to the initialized binding descriptor. If
 *			this routine fails, this will be NULL.
 *  Side Effects:
 *      If the call succeeds, bindPtr->handle is a valid RPC binding handle and
 *	we can use it to make RPC calls to this server.
 *
 *--
 */
ServerBindingDsc *CreateServerBinding (
    char	              *name,
    size_t	              nameLen,
    rpc_binding_handle_t  binding_h
)
{
    ServerBindingDsc	*bp;
    error_status_t	rpcStatus;

    /*
     * Note that the binding descriptor and binding name are allocated
     * together here and are deallocated together in ReleaseServerBinding
     */
    bp = DTSSmallocStruct (ServerBindingDsc);
    bp->refCount = 1;	/* by creating the binding, we're making the
			       first reference to it */
    dtss_mutex_init (&bp->mutex);
    rpc_binding_copy (binding_h,
		      &bp->handle,
		      &rpcStatus);

    return (bp);
}

/*
 *++
 *  AcquireServerBinding ()
 *	This routine is called when a thread wants to access an RPC binding
 *	belonging to a particular server.  Time request threads do this in
 *	GetTimeFromServerRPC because they need to make sure that the main thread
 *	doesn't release the binding (because of a cache flush) while it is in
 *	the middle of an RPC call.  This routine protects the binding by
 *	incrementing the reference count in the binding's descriptor.
 *  Inputs:
 *	bindPtr		is the binding descriptor.
 *  Implicit Outputs:
 *      bindPtr->refCount gets incremented.
 *  Locking:
 *	bindPtr->mutex is locked while incrementing the refCount
 *--
 */
void AcquireServerBinding (
    ServerBindingDsc	*bindPtr
)
{

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	     "RPCTIME:  Acquiring binding \n"));

    dtss_mutex_lock (&bindPtr->mutex);
    bindPtr->refCount++;
    dtss_mutex_unlock (&bindPtr->mutex);
} /* end of routine AcquireServerBinding */

/*
 *++
 *  ReleaseServerBinding ()
 *	This routine is called when a thread is finished using a server's RPC
 *	binding.  It decrements the refCount of the given binding descriptor.
 *	If the refCount is zero, it then releases the binding and deallocates
 *	the descriptor.
 *  Inputs:
 *	bindPtr		is the binding descriptor.
 *  Implicit Outputs:
 *      bindPtr->refCount gets decremented.  The descriptor is deallocated if
 *	the refCount is zero.
 *  Locking:
 *	bindPtr->mutex is locked while decrementing the refCount and while
 *	releasing the binding.
 *--
 */
void ReleaseServerBinding (
    ServerBindingDsc	*bindPtr
)
{
    error_status_t	rpcStatus;

    dtss_mutex_lock (&bindPtr->mutex);
    bindPtr->refCount--;
    if (bindPtr->refCount > 0) {
	dtss_mutex_unlock (&bindPtr->mutex);
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		 "--> Releasing binding \n"));
    }
    else {
	/*
	 * Release the binding, then the mutex. Finally, free the descriptor.
	 */
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		 "--> Deallocating binding \n"));
	rpc_binding_free (&bindPtr->handle, &rpcStatus);
	dtss_mutex_unlock (&bindPtr->mutex);
	dtss_mutex_destroy (&bindPtr->mutex);
	DTSSfree ((VOID *)bindPtr);
    }
} /* end of routine ReleaseServerBinding */


/*
 *++
 *  ShutDownRPC ()
 *     Unregister the Control Program interface from the RPC run-time
 *     library.  Unregister Control Program interfaces from the endpoint mapper.
 *     Unregister the Time Service from RPC and namespace and deallcate
 *     memory previously allocated for RPC functions.
 *--
 */

void ShutDownRPC (void)
{
    unsigned32    status;

    /*
     * Tell the RPC to shut down
     */
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	      "Main:  Shutting down RPC \n"));

    /*
     * Unregister serviceability.
     */

    rpc_ep_unregister (serviceability_v1_0_s_ifspec,
		       rpcBufPtr->bindingVector,
		       &rpcBufPtr->objUUIDvector,
		       &status);
    if (status != rpc_s_ok)
    {
	dce_error_string_t tempString ;

	dce_svc_printf( DTS_S_SVC_EP_UNREGISTER_MSG, 
		       dce_dts_error_text( status, tempString ));
    }

    rpc_server_unregister_if (serviceability_v1_0_s_ifspec,
			    NULL,
			    &status);

    /*
     * Unregister Control Program service from RPC
     */

    rpc_ep_unregister (time_control_v2_0_s_ifspec,
		       rpcBufPtr->bindingVector,
		       (uuid_vector_p_t)NULL,
		       &status);
    if (status != rpc_s_ok)
    {
	dce_error_string_t tempString ;

	dce_svc_printf( DTS_S_CONTROL_EP_UNREGISTER_MSG,
		       dce_dts_error_text( status, tempString ));
    }
    

    rpc_server_unregister_if (time_control_v2_0_s_ifspec,
			      NULL,
			      &status);

    UnexportMgmtService() ;

    /* unregister acl mgr here XXX */

    if (shmPtr->mgtParams.variant == VTserver)
	UnexportTimeService();

    /*
     * Free up memory allocated by rpc
     */

    if (rpcBufPtr->lanProfile != NULL)
	rpc_string_free (&rpcBufPtr->lanProfile, &status);

    if (rpcBufPtr->hostName != NULL)
	rpc_string_free (&rpcBufPtr->hostName, &status);

    if (rpcBufPtr->bindingVector != NULL)
	rpc_binding_vector_free (&rpcBufPtr->bindingVector, &status);

#if CONFIG_SECURITY

#if DTS_ENTITY_PRINCIPAL
        /*
         * If dts is configured to run as /dts-entity, purge the context,
	 * which also destroys the network credentials.  Otherwise, if dts
	 * is configured to run as the machine principal, just release the
	 * context so that the credentials are kept.
	 */
	sec_login_purge_context (&temp_context, &secStatus);
#else
    sec_login_release_context (&rpcBufPtr->loginContext_h,
#endif
			       &status);
    sec_rgy_site_close (rpcBufPtr->secContext, &status);

#endif /* CONFIG_SECURITY */

    if (pthread_detach (&TSthread) < 0)
	BugCheckOSstatus (errno);

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	      "Main:  RPC has shut down\n"));
} /* end of routine ShutDownRPC  */


void UnexportMgmtService( void ) 
{
    unsigned32   status;

    /* begin */

    rpc_ns_binding_unexport (rpc_c_ns_syntax_dce,
			     rpcBufPtr->TServiceEntryName,
			     time_control_v2_0_s_ifspec,
			     &rpcBufPtr->objUUIDvector,
			     &status);
    if (status != rpc_s_ok)
    {
	
	dce_error_string_t tempString ;

	dce_svc_printf( DTS_S_CONTROL_UNEXPORT_MSG, 
		       dce_dts_error_text( status, tempString));
    }
    
    dts_remove_bindings_file();
    return ;
}


/*
 *++
 *  UnexportTimeService()
 *     Unregister The Time Service from the endpoint mapper; unexport
 *     bindings from the Time Service entry and remove entry for
 *     this server from the LAN and possibly CELL profiles.  This
 *     is done either at shut down time of if the server detects that
 *     it is no longer a member of the Time Servers security group.
 *--
 */
void UnexportTimeService (void)
{
    unsigned32   status;
    StatusBlk    statusBlk;
    rpc_if_id_t  interfaceID;

    /*
     * Unregister this server from the endpoint mapper.
     */
    rpc_ep_unregister (time_service_v1_0_s_ifspec,
		       rpcBufPtr->bindingVector,
		       &rpcBufPtr->objUUIDvector,
		       &status);
    if (status != rpc_s_ok)
    {
	dce_error_string_t tempString ;	

	dce_svc_printf( DTS_S_SERVICE_UNEXPORT_MSG, 
		       dce_dts_error_text( status, tempString ));
    }
    
    /*
     * Unexport bindings from the Time Service entry in the nameserver
     * database.
     */
    rpc_ns_binding_unexport (rpc_c_ns_syntax_dce,
			     rpcBufPtr->TServiceEntryName,
			     time_service_v1_0_s_ifspec,
			     /*&rpcBufPtr->objUUIDvector,*/
			     (uuid_vector_t *)0,
			     &status);
    if (status != rpc_s_ok)
    {
	dce_error_string_t tempString ;	

	dce_svc_printf( DTS_S_SERVICE_UNEXPORT_MSG, 
		       dce_dts_error_text( status, tempString));
    }
    
    /*
     * Remove this server as a member of the LAN services profile.
     */

    rpc_if_inq_id (time_service_v1_0_s_ifspec,
		   &interfaceID,
		   &status);

    if (rpcBufPtr->lanProfile != NULL)
    {
	rpc_ns_profile_elt_remove (rpc_c_ns_syntax_dce,
				   rpcBufPtr->lanProfile,
				   &interfaceID,
				   rpc_c_ns_syntax_dce,
				   rpcBufPtr->TServiceEntryName,
				   &status);
	if (status != rpc_s_ok)
	{
	    dce_error_string_t tempString ;

	    dce_svc_printf( DTS_S_TSERVER_PROFILE_REMOVE_MSG, 
			   rpcBufPtr->lanProfile, 
			   dce_dts_error_text( status, tempString));
	}

    }
    
    /*
     * 4-15-94 CLBrooks 
     * blow off the acl manager interface as well
     */

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		       "UnExportTimeService: mgt acl interface..."));
    

    mgt_unregister_acl_mgr_if( 
			      rpcBufPtr->bindingVector,
			      &rpcBufPtr->objUUIDvector,
			      &status);
    if (status != rpc_s_ok) 
    {
	dce_error_string_t tempString ;	

	dce_svc_printf( DTS_S_UNREGISTER_ACL_MGR_MSG, 
			   dce_dts_error_text( status, tempString));
    }
    
    mgt_unexport_acl_mgr_if ( 
			     rpcBufPtr->TServiceEntryName, 
			     (uuid_vector_t *)0,
			     &status);
    if (status != rpc_s_ok) 
    {
	dce_error_string_t tempString ;

	dce_svc_printf( DTS_S_UNEXPORT_ACL_MGR_MSG, 
			   dce_dts_error_text( status, tempString));
    }
    
    
    /*
     * If this server is also a member of the global set, make it
     * unavailable.  Normally the network manager will remove this
     * server from the CELL profile through the UNADVERTISE command.
     *
     */

    if (shmPtr->mgtParams.servType == K_SERVER_GLOBAL)
    {
	(void) UnexportGblTServiceFromNS (&statusBlk);
    }
    return ;
}



/*
 *++
 *  serverInSecurityGroup()
 *        This function checks if the server principal is a member of the
 *        security group.
 *  Inputs:
 *        serverPrincname : principal name of server to be checked.
 *  Implicit Outputs:
 *         rpcBufPtr->secContext :
 *  Value Returned:
 *        true  : if host is in security group
 *        false : otherwise
 *--
 */

#if CONFIG_SECURITY
boolean32 serverInSecurityGroup (
    unsigned_char_p_t serverPrincName
)
{

    unsigned32        secStatus;
    boolean32         isMember;

    isMember = (boolean32)0;

    isMember = sec_rgy_pgo_is_member(rpcBufPtr->secContext,
				     sec_rgy_domain_group,
				     shmPtr->mgtParams.groupName,
				     serverPrincName,
				     &secStatus);
    if (BAD_STATUS(&secStatus))
    {
	dce_error_string_t tempString ;	

	dce_svc_printf( DTS_S_PGO_IS_MEMBER_MSG, 
		       dce_dts_error_text( secStatus, tempString));
    }
    
    /*
     * Close security site context
     */

    return (isMember);
}

/*
 *++
 *  get_security_config_info ()
 *      Get the security-relevant configuration information from the DCE
 *	configuration file and the security registry.
 *  Implicit Inputs:
 *	rpcBufPtr->hostName	is the host name (e.g. /hosts/harlqn)
 *  Outputs:
 *	cell_id		is the security id (name and uuid) of the cell
 *			(e.g. /.../my_cell)
 *
 *	host_id		is the security id (name and uuid) of the host principal
 *			(e.g. /hosts/harlqn/self).
 *
 *	status		is the completion status (a DCE status).
 *--
 */
void get_security_config_info (
    sec_id_t		*host_id,
     sec_id_t		*cell_id,
     error_status_t	*status
)
{
    StatusBlk               statusBlk;

    /*
     * Get the host machine's principal name, which is a well-known name.
     * It is constructed by appending the constant "/self" to the host
     * name obtained from the configuration file.
     */

    dce_cf_prin_name_from_host ((char *)rpcBufPtr->hostName,
                                (char **)&host_id->name,
                                status);

    if (host_id->name == (idl_char *)NULL)
    {
	dce_error_string_t tempString ;

	dce_svc_printf( DTS_S_PRIN_HOST_MSG, 
		       dce_dts_error_text( *status, tempString));
	return;
    }

    /*
     * Get the cell name.
     */
    dce_cf_get_cell_name ((char **)&cell_id->name, status);
    if (*status != dce_cf_st_ok)
    {
	dce_error_string_t tempString ;
	dce_svc_printf( DTS_S_CELL_NAME_MSG, 
		       dce_dts_error_text( *status, tempString));
	return;
    }

    statusBlk = CreateSecurityCntxt ();
    if (IsError(statusBlk))
    {
        *status = statusBlk.sysStatus;
        return;
    }

    sec_rgy_site_open (cell_id->name, &rpcBufPtr->secContext, status);

    /*
     * Get the host principal and cell uuid's from the rgy database.  If we
     * can't get these, then return, because the acl for the control program
     * interface can't be initialized.
     */

    if (BAD_STATUS(status))
    {
	dce_error_string_t tempString ;
	dce_svc_printf( DTS_S_RGY_SITE_OPEN_MSG, dce_dts_error_text( *status, tempString));
	return;
    }

    /*
     * Get the cell uuid
     */
    sec_id_parse_name (rpcBufPtr->secContext,
		       (idl_char * )cell_id->name,
		       (idl_char *)NULL,
		       &cell_id->uuid,
		       (idl_char*)NULL,
		       (uuid_t *)NULL,
		       status);
    if (BAD_STATUS(status))
    {
	dce_error_string_t tempString ;
	dce_svc_printf( DTS_S_ID_PARSE_NAME_MSG, dce_dts_error_text( *status, tempString));
    }
    else
    {

	/*
	 * Get the host principal uuid
	 */
	sec_rgy_pgo_name_to_id (rpcBufPtr->secContext,
				sec_rgy_domain_person,
				(idl_char *)host_id->name,
				&host_id->uuid,
				status);
	if (BAD_STATUS(status))
	{
	    dce_error_string_t tempString ;

	    dce_svc_printf( DTS_S_PGO_NAME_ID_MSG, 
			   dce_dts_error_text( *status, tempString));
	}
    }
}
#endif /* CONFIG_SECURITY */
