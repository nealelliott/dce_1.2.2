/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: fsprobe.c,v $
 * Revision 1.1.71.1  1996/10/02  17:47:16  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:37:42  damon]
 *
 * Revision 1.1.66.3  1994/08/01  18:50:44  ruby
 * 	Implement partial S12Y (Internationaliztion) for DFS commands and daemons
 * 	[1994/08/01  18:42:18  ruby]
 * 
 * Revision 1.1.66.2  1994/06/09  14:08:08  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:23:01  annie]
 * 
 * Revision 1.1.66.1  1994/02/04  20:18:56  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:10  devsrc]
 * 
 * Revision 1.1.64.1  1993/12/07  17:24:29  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:24:28  jaffe]
 * 
 * Revision 1.1.5.6  1993/01/21  19:36:26  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  19:50:13  zeliff]
 * 
 * Revision 1.1.5.5  1992/12/09  20:07:14  jaffe
 * 	Transarc delta: vijay-ot6061-fsprobe-incr-stack-size 1.1
 * 	  Selected comments:
 * 
 * 	    fsprobe thread needs a bigger thread stack on the pmax.
 * 	[1992/12/03  22:25:19  jaffe]
 * 
 * Revision 1.1.5.4  1992/11/24  17:48:36  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  17:59:47  bolinger]
 * 
 * Revision 1.1.5.3  1992/10/02  21:13:32  toml
 * 	Clean up #includes.
 * 	[1992/10/02  18:59:00  toml]
 * 
 * Revision 1.1.5.2  1992/08/31  19:46:39  jaffe
 * 	Transarc delta: cfe-ot3884-unlimited-listaggr 1.4
 * 	  Selected comments:
 * 	    Fix use of vols_ListAggregates in fsprobe/scout.
 * 	    Call fts's own cover procedure for vols_ListAggregates.
 * 	    Also, use all 16 disks in the afsStatistics structure.  (Though this
 * 	    could well be changed so as not to bother with its 16-disk limit...)
 * 	Transarc delta: jaffe-ot4705-fix-dfs-102-143-compilation-errors 1.4
 * 	  Selected comments:
 * 	    Cleanup compilation errors from the dfs-102-1.43 build.
 * 	    rename print_hostname_from_binding to compat_print_hostname_from_binding
 * 	[1992/08/30  02:28:43  jaffe]
 * 
 * Revision 1.1.3.4  1992/04/21  13:37:28  mason
 * 	Transarc delta: tu-1143-error-mapping 1.8
 * 	  Files modified:
 * 	    cm: cm.h, cm_cell.c, cm_conn.c, cm_daemons.c, cm_dcache.c
 * 	    cm: cm_dnamehash.c, cm_init.c, cm_pioctl.c, cm_rrequest.c
 * 	    cm: cm_scache.c, cm_server.c, cm_subr.c, cm_tokens.c
 * 	    cm: cm_vnodeops.c, cm_volume.c; fsprobe: fsprobe.c
 * 	    libafs/RIOS: dfscore.exp; osi: Makefile, osi_dfserrors.h
 * 	    osi.klib/OSF1: Makefile; osi.klib/RIOS: Makefile
 * 	    osi/OSF1: Makefile, osi_dfs2osferr.c, osi_osf2dfserr.c
 * 	    osi/RIOS: Makefile, osi_aix2dfserr.c, osi_dfs2aixerr.c
 * 	    px: px_init.c, px_intops.c; rep: rep_main.c
 * 	    userInt/fts: volc_tokens.c
 * 	  Selected comments:
 * 	    An enhancement to allow the DFS server to map returned kernel errors to the
 * 	    canonical ones and to allow the CM to map the canonical ones to the
 * 	    appropriate local kernel errors.
 * 	    Please refer to the design memo, Error Conversion Protocol, for more detail.
 * 	    The same as above.
 * 	    Made osi_dfserrors.h and osi_dfs2aixerr.c osi_aix2dfserr.c osi_osf2dfserr.c and
 * 	    osi_dfs2osferr.c avaliable in the user space. So, whoever references AFS_xxx
 * 	    APIs should use the error conversion protocol.
 * 	    fixed some typo errors and others
 * 	    Changes did not compile on the PMAX.
 * 	    DFS servers now maps errors to the canonical ones before returning to theirs
 * 	    clients. Therefore, those clients who use AFS_xxx rpc calls must do the
 * 	    error decoding work to get the appropriate error back.
 * 	    Add  osi_initDecodeTable() and   osi_errDecode() to decode the returned
 * 	    error from DFS servers.
 * 	    CFE adds osi_initDecodeTable() and osi_errDecode() to repserver and fts.
 * 	[1992/04/20  22:33:20  mason]
 * 
 * Revision 1.1.3.3  1992/01/24  03:14:10  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:13:00  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:48:41  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:33:26  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:16:40  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*
 * (C) Copyright Transarc Corporation 1989
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

/*------------------------------------------------------------------------
 * fsprobe.c
 *
 * Description:
 *	Implementation of the AFS FileServer probe facility.
 *
 * Author:
 *	Ed. Zayas
 *	Transarc Corporation
 *------------------------------------------------------------------------*/

#include <dcedfs/osi.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/fldb_data.h>
#include <dcedfs/ftserver.h>
#include <dcedfs/ftserver_data.h>
#include <dcedfs/ftserver_proc.h>
#include <dcedfs/volc.h>
#include <dcedfs/osi_dfserrors.h>
#include <fsprobe.h>			/*Interface for this module*/

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsfspmac.h>
#include <dfsfspsvc.h>
#include <dfsfspmsg.h>

#if defined(AFS_HPUX_ENV)
#define THREAD_STACK_SIZE	(16 * 1024)
#elif defined(AFS_OSF_ENV)
#define THREAD_STACK_SIZE	(150000)
#elif defined(AFS_AIX_ENV)
#define THREAD_STACK_SIZE       (32 * 1024)
#else /* defined(AFS_HPUX_ENV) */
#define THREAD_STACK_SIZE	(8 * 1024)
#endif /* defined(AFS_HPUX_ENV) */

/* define a servicability handle for the dce_svc_printf facility */

DCE_SVC_DEFINE_HANDLE(fsp_svc_handle, fsp_svc_table, "fsp")

/*
 * Routines we need that don't have explicit include file definitions.
 */

extern char *hostutil_GetNameByINet();	/*Host parsing utility*/

/*
 * Exported variables.
 */
int fsprobe_numServers;				 /*Num servers connected*/
unsigned char **fsprobe_servers;                 /*server names */
struct fsprobe_ConnectionInfo *fsprobe_ConnInfo; /*Ptr to connection array*/
struct fsprobe_ProbeResults fsprobe_Results;	 /*Latest probe results*/
int fsprobe_ProbeFreqInSecs;			 /*Probe freq. in seconds*/

/*
 * Private globals.
 */
static int fsprobe_initflag = 0;		/*Was init routine called?*/
static int fsprobe_debug = 0;			/*Debugging output enabled?*/
static int (*fsprobe_Handler)();		/*Probe handler routine*/
static pthread_t probeTHREAD_ID;
static pthread_attr_t threadAttr;
static int fsprobe_statsBytes;			/*Num bytes in stats block*/
static int fsprobe_probeOKBytes;		/*Num bytes in probeOK block*/

/*------------------------------------------------------------------------
 * [private] fsprobe_CleanupInit
 *
 * Description:
 *	Set up for recovery after an error in initialization (i.e.,
 *	during a call to fsprobe_Init.
 *
 * Arguments:
 *	None.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	This routine is private to the module.
 *
 * Side Effects:
 *	Zeros out basic data structures.
 *------------------------------------------------------------------------*/

static int fsprobe_CleanupInit()

{ /*fsprobe_CleanupInit*/

    long code;			/*Return code from callback stubs*/

    fsprobe_ConnInfo = (struct fsprobe_ConnectionInfo *)0;
    bzero((char *)&fsprobe_Results, sizeof(struct fsprobe_ProbeResults));

    return(0);

} /*fsprobe_CleanupInit*/


/*------------------------------------------------------------------------
 * [exported] fsprobe_Cleanup
 *
 * Description:
 *	Clean up our memory and connection state.
 *
 * Arguments:
 *	int a_releaseMem : Should we free up malloc'ed areas?
 *
 * Returns:
 *	0 on total success,
 *	-1 if the module was never initialized, or there was a problem
 *		with the fsprobe connection array.
 *
 * Environment:
 *	fsprobe_numServers should be properly set.  We don't do anything
 *	unless fsprobe_Init() has already been called.
 *
 * Side Effects:
 *	Shuts down Rx connections gracefully, frees allocated space
 *	(if so directed).
 *------------------------------------------------------------------------*/

int fsprobe_Cleanup(a_releaseMem)
    int a_releaseMem;

{ /*fsprobe_Cleanup*/

    static char rn[] = "fsprobe_Cleanup";	/*Routine name*/
    int code;					/*Return code*/
    int conn_idx;				/*Current connection index*/
    struct fsprobe_ConnectionInfo *curr_conn;	/*Ptr to fsprobe connection*/
    unsigned32 status;				/*Throwaway status*/

    /*
     * Assume the best, but check the worst.
     */
    if (!fsprobe_initflag) {
      dce_svc_printf(FSP_S_FP_MODULE_NOT_INITIALIZED_MSG, rn);
      return(-1);
    }
    else
      code = 0;

    /*
     * Take care of all Rx connections first.  Check to see that the
     * server count is a legal value.
     */
    if (fsprobe_numServers <= 0) {
      dce_svc_printf(FSP_S_FP_ILLEGAL_NUMBER_SERVERS_MSG, rn, fsprobe_numServers);
      code = -1;
    }
    else {
      if (fsprobe_ConnInfo != (struct fsprobe_ConnectionInfo *)0) {
	/*
	 * The fsprobe connection structure array exists.  Go through it
	 * and close up any Rx connections it holds.
	 */
	curr_conn = fsprobe_ConnInfo;
	for (conn_idx = 0; conn_idx < fsprobe_numServers; conn_idx++) {
	  if (curr_conn->h != (rpc_binding_handle_t) 0) {
	      rpc_binding_free(&curr_conn->h,&status);
	      curr_conn->h = (rpc_binding_handle_t )0;
	  }
	  curr_conn++;
	} /*for each fsprobe connection*/
      } /*fsprobe connection structure exists*/
    } /*Legal number of servers*/

    /*
     * Now, release all the space we've allocated, if asked to.
     */
    if (a_releaseMem) {
      if (fsprobe_ConnInfo != (struct fsprobe_ConnectionInfo *)0)
	free(fsprobe_ConnInfo);
      if (fsprobe_Results.stats != (afsStatistics *)0)
	free(fsprobe_Results.stats);
      if (fsprobe_Results.probeOK != (int *)0)
	free(fsprobe_Results.probeOK);
    }

    /*
     * Return the news, whatever it is.
     */
    return(code);

} /*fsprobe_Cleanup*/

/*------------------------------------------------------------------------
 * [private] fsprobe_THREAD
 *
 * Description:
 *	This THREAD iterates over the server connections and gathers up
 *	the desired statistics from each one on a regular basis.  When
 *	the sweep is done, the associated handler function is called
 *	to process the new data.
 *
 * Arguments:
 *	None.
 *
 * Returns:
 *	Nothing.
 *
 * Environment:
 *	Started by fsprobe_Init(), uses global sturctures.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/


static pthread_startroutine_t fsprobe_THREAD()

{ /*fsprobe_THREAD*/

    static char rn[] = "fsprobe_THREAD";		/*Routine name*/
    register long code;				/*Results of calls*/
    struct timeval tv;				/*Time structure*/
    int conn_idx;				/*Connection index*/
    struct fsprobe_ConnectionInfo *curr_conn;	/*Current connection*/
    afsStatistics *curr_stats;		        /*Current stats region*/
    int *curr_probeOK;				/*Current probeOK field*/

        /* introduce some auxilliary per-server info */

        int ii;
        struct
        {
                struct sockaddr servAddr;
                int valid;
        }
        server_info[256];

        /* make the info not valid */

        for(ii=0;ii<256;++ii)
                server_info[ii].valid=1;

	vldb_InitUbikHandle(0);

	for(conn_idx = 0; conn_idx < fsprobe_numServers; conn_idx++)
        {
		/* looked up this server yet? */
		
		if(server_info[conn_idx].valid)
		{
		        /* nope */
		
		          if (!GetServer(fsprobe_servers[conn_idx],
		                   &(server_info[conn_idx].servAddr)))
		              exit(1);
		
		        /* mark the lookup valid */
		
		        server_info[conn_idx].valid=0;
		}
        }

    while (1) { /*Service loop*/
	/*
	 * Iterate through the server connections, gathering data.
	 * Don't forget to bump the probe count and zero the statistics
	 * areas before calling the servers.
	 */
      if (fsprobe_debug)
	dce_svc_printf(FSP_S_FP_COLLECTING_DATA_MSG, rn, fsprobe_numServers);
      curr_conn    = fsprobe_ConnInfo;
      curr_stats   = fsprobe_Results.stats;
      curr_probeOK = fsprobe_Results.probeOK;
      fsprobe_Results.probeNum++;
      bzero((char *)fsprobe_Results.stats, fsprobe_statsBytes);
      bzero((char *)fsprobe_Results.probeOK, fsprobe_probeOKBytes);

      for (conn_idx = 0; conn_idx < fsprobe_numServers; conn_idx++) {
	  /*
	   * Grab the statistics for the current FileServer, if the
	   * connection is valid.
	   */
	if (fsprobe_debug)
	  dce_svc_printf(FSP_S_FP_CONTACTING_SERVER_MSG, rn, curr_conn->hostName);
	if (curr_conn->h != (rpc_binding_handle_t)0) {
	  if (fsprobe_debug)
	    dce_svc_printf(FSP_S_FP_CONNECTION_VALID_MSG, rn);
	  code = AFS_GetStatistics(curr_conn->h, curr_stats);
	  *curr_probeOK = osi_errDecode(code);
	  if (*curr_probeOK) {
	      unsigned32 dontcare;

	      if (fsprobe_debug) {
		  dce_svc_printf(FSP_S_FP_AFS_GETSTATISTICS_FAILED_MSG, dfs_dceErrTxt(*curr_probeOK));
		  fflush(stderr);
	      }
	      rpc_binding_reset(curr_conn->h, &dontcare);
	      curr_conn++;
	      curr_stats++;
	      curr_probeOK++;
	      continue;
	  }
	  /* contact ftserver to get disk stats */
      {
	  long code = 0, i, numAggrs;
	  struct sockaddr servAddr;
	  struct ftserver_aggrInfo aggrInfo;
	  struct ftserver_aggrList *aggrListp;
	  register struct ftserver_aggrList *aggrlp;
	  afsDisk *disk;

	  /* from fsprobe_servers[conn_idx] get the cellname,
	     use the cell name to do vldb_InitUbikHandle to
	     get a ubik handle for each cell */
/*
	  vldb_InitUbikHandle(0);
	  if (!GetServer(fsprobe_servers[conn_idx],
			 &servAddr))
	      exit(1);
*/

	servAddr=server_info[conn_idx].servAddr;

	  aggrListp = 0;
	  code = volc_Call(fts_GetAllAggrs, &servAddr, NULL, &numAggrs, &aggrListp);

	  if (!code) {
	      for (i = 0, aggrlp = aggrListp; i < numAggrs; ++i,  ++aggrlp) {
		  code = volc_Call(vols_AggregateInfo, &servAddr, NULL, 
				   aggrlp->Id, &aggrInfo);
		  if (!code) {
		      if(fsprobe_debug) {
			  dce_svc_printf(FSP_S_FP_FREE_SPACE_MSG);
			  dce_svc_printf(FSP_S_FP_AGGREGATE_DISK_BLOCKS_MSG, aggrInfo.name, aggrInfo.curFree, aggrInfo.totalUsable,
				 aggrInfo.minFree);
		      }
		      /* copy the aggregate info onto stats */
		      disk = NULL;
		      switch (i) {
			case 0: disk = &curr_stats->Disk1; break;
		  	case 1: disk = &curr_stats->Disk2; break;
			case 2: disk = &curr_stats->Disk3; break;
			case 3: disk = &curr_stats->Disk4; break;
			case 4: disk = &curr_stats->Disk5; break;
			case 5: disk = &curr_stats->Disk6; break;
			case 6: disk = &curr_stats->Disk7; break;
			case 7: disk = &curr_stats->Disk8; break;
			case 8: disk = &curr_stats->Disk9; break;
			case 9: disk = &curr_stats->Disk10; break;
			case 10: disk = &curr_stats->Disk11; break;
			case 11: disk = &curr_stats->Disk12; break;
			case 12: disk = &curr_stats->Disk13; break;
			case 13: disk = &curr_stats->Disk14; break;
			case 14: disk = &curr_stats->Disk15; break;
			case 15: disk = &curr_stats->Disk16; break;
		      }
		      if (disk) {
			  disk->BlocksAvailable = aggrInfo.curFree;
			  disk->TotalBlocks = aggrInfo.totalUsable;
			  strcpy ((char *)disk->Name, (char *)aggrInfo.name);
		      }
		  }
	      }
	  }
	  if (aggrListp)
	      free(aggrListp);
      }
      } /*Valid Rx connection*/

	/*
	 * Advance the fsprobe connection pointer & stats pointer.
	 */
	curr_conn++;
	curr_stats++;
	curr_probeOK++;

      } /*For each fsprobe connection*/

      /*
       * All (valid) connections have been probed.  Now, call the
       * associated handler function.  The handler does not take
       * any explicit parameters, rather gets to the goodies via
       * some of the objects exported by this module.
       */
      if (fsprobe_debug)
	dce_svc_printf(FSP_S_FP_POLLING_COMPLETE_MSG, rn);
      code = fsprobe_Handler();
      if (code)
	dce_svc_printf(FSP_S_FP_HANDLER_ERROR_MSG, rn, code);

      /*
       * Fall asleep for the prescribed number of seconds.
       */
      tv.tv_sec  = fsprobe_ProbeFreqInSecs;
      tv.tv_usec = 0;
      if (fsprobe_debug)
	dce_svc_printf(FSP_S_FP_SLEEP_SECONDS_MSG, rn, fsprobe_ProbeFreqInSecs);

      code = select(0,0,0,0,&tv);
      if (code)
	  dce_svc_printf(FSP_S_FP_SELECT_RETURN_CONDE_MSG, rn, code);
    } /*Service loop*/

} /*fsprobe_THREAD*/


/*------------------------------------------------------------------------
 * [exported] fsprobe_Init
 *
 * Description:
 *	Initialize the fsprobe module: set up Rx connections to the
 *	given set of servers, start up the probe and callback THREADs,
 *	and associate the routine to be called when a probe completes.
 *
 * Arguments:
 *	int a_numServers		  : Num. servers to connect to.
 *	struct sockaddr_in *a_socketArray : Array of server sockets.
 *	int a_ProbeFreqInSecs		  : Probe frequency in seconds.
 *	int (*a_ProbeHandler)()		  : Ptr to probe handler fcn.
 *	int a_debug;			  : Turn debugging output on?
 *
 * Returns:
 *	0 on success,
 *	-2 for (at least one) connection error,
 *	THREAD process creation code, if it failed,
 *	-1 for other fatal errors.
 *
 * Environment:
 *	*** MUST BE THE FIRST ROUTINE CALLED FROM THIS PACKAGE ***
 *	Also, the server security object CBsecobj MUST be a static,
 *	since it has to stick around after this routine exits.
 *
 * Side Effects:
 *	Sets up just about everything.
 *------------------------------------------------------------------------*/

int fsprobe_Init(a_numServers, a_servers, a_bindingVector, a_ProbeFreqInSecs, a_ProbeHandler, 
		 a_debug)
    int a_numServers;
    char *a_servers[];
    rpc_binding_vector_t *a_bindingVector;
    int a_ProbeFreqInSecs;
    int (*a_ProbeHandler)();
    int a_debug;

{ /*fsprobe_Init*/

    static char rn[] = "fsprobe_Init";		/*Routine name*/
    register long code;				/*Return value*/

    int arg_errfound;				/*Argument error found?*/
    int curr_srv;				/*Current server idx*/
    struct fsprobe_ConnectionInfo *curr_conn;	/*Ptr to current conn*/
    char *hostNameFound;			/*Ptr to returned host name*/
    int conn_err;				/*Connection error?*/
    int PortToUse;				/*Callback port to use*/
    error_status_t st;                          /* status for servicability */

    /*
     * If we've already been called, snicker at the bozo, gently
     * remind him of his doubtful heritage, and return success.
    */
    if (fsprobe_initflag) {
      dce_svc_printf(FSP_S_FP_MULTIPLE_CALLS_MSG, rn);
      return(0);
    }
    else
     fsprobe_initflag = 1; 

    /* Register fsp messages with the dce_printf facility */

    dce_msg_define_msg_table(fsp__table,
			     sizeof fsp__table / sizeof fsp__table[0],
			     &st);

    if (st != svc_s_ok)

      fprintf(stderr,
	      "Warning Cannot register fsp_table as an incore message table 0x%lx\n",
	      st);

    /*
     * Check the parameters for bogosities.
     */
    arg_errfound = 0;
    if (a_numServers <= 0) {
      dce_svc_printf(FSP_S_FP_NUMBER_SERVERS_ILLEGAL_MSG, rn, a_numServers);
      arg_errfound = 1;
    }
    if (a_bindingVector == (rpc_binding_vector_t *)0) {
	dce_svc_printf(FSP_S_FP_NULL_SERVER_BINDINGS_MSG, rn);
	arg_errfound = 1;
    }
    if (a_ProbeFreqInSecs <= 0) {
      dce_svc_printf(FSP_S_FP_ILLEGAL_PROBE_FREQUENCY_MSG, rn, a_ProbeFreqInSecs);
      arg_errfound = 1;
    }
    if (a_ProbeHandler == (int (*)())0) {
      dce_svc_printf(FSP_S_FP_NULL_PROBE_HANDLER_MSG, rn);
      arg_errfound = 1;
    }
    if (arg_errfound)
      return(-1);

    /*
     * Record our passed-in info.
     */
    fsprobe_debug	    = a_debug;
    fsprobe_numServers      = a_numServers;
    fsprobe_servers         = (unsigned char **)a_servers;
    fsprobe_Handler	    = a_ProbeHandler;
    fsprobe_ProbeFreqInSecs = a_ProbeFreqInSecs;

    /*
     * Get ready in case we have to do a cleanup - basically, zero
     * everything out.
     */
    fsprobe_CleanupInit();

    /*
     * Initialize the error mapping table used for decoding the error returned
     * from DFS calls. 
     */
    osi_initDecodeTable();

    /*
     * Allocate the necessary data structures and initialize everything
     * else.
     */
    fsprobe_ConnInfo =
      (struct fsprobe_ConnectionInfo *)
	malloc(a_numServers * sizeof(struct fsprobe_ConnectionInfo));
    if (fsprobe_ConnInfo == (struct fsprobe_ConnectionInfo *)0) {
      dce_svc_printf(FSP_S_FP_CANT_ALLOCATE_CONNECTION_INFO_MSG, rn, a_numServers,
	      (a_numServers * sizeof(struct fsprobe_ConnectionInfo)));
      return(-1);	/*No cleanup needs to be done yet*/
    }
    fsprobe_statsBytes = a_numServers * sizeof(afsStatistics);
    fsprobe_Results.stats = (afsStatistics *)
      malloc(fsprobe_statsBytes);
    if (fsprobe_Results.stats == (afsStatistics *)0) {
      dce_svc_printf(FSP_S_FP_CANT_ALLOCATE_STATISTICS_STRUCT_MSG, rn, a_numServers, fsprobe_statsBytes);
      fsprobe_Cleanup(1); /*Delete already-malloc'ed areas*/
      return(-1);
    }
    else
      if (fsprobe_debug)
	dce_svc_printf(FSP_S_FP_ALLOCATED_STATISTICS_STRUCT_MSG, rn, fsprobe_statsBytes);

    fsprobe_probeOKBytes = a_numServers * sizeof(int);
    fsprobe_Results.probeOK = (int *) malloc(fsprobe_probeOKBytes);
    if (fsprobe_Results.probeOK == (int *)0) {
      dce_svc_printf(FSP_S_FP_CANT_ALLOCATE_PROBE_ARRAY_MSG, rn, a_numServers, fsprobe_probeOKBytes);
      fsprobe_Cleanup(1); /*Delete already-malloc'ed areas*/
      return(-1);
    }
    else
      if (fsprobe_debug)
	dce_svc_printf(FSP_S_PF_ALLOCATED_PROBE_ARRAY_MSG, rn, fsprobe_probeOKBytes);

    fsprobe_Results.probeNum  = 0;
    fsprobe_Results.probeTime = 0;
    bzero((char *)fsprobe_Results.stats,
	  (a_numServers * sizeof(afsStatistics)));

    curr_conn = fsprobe_ConnInfo;
    conn_err = 0;

    for (curr_srv = 0; curr_srv < a_numServers; curr_srv++) {
	/*
	 * Copy in the socket info for the current server, resolve its
	 * printable name if possible.
	 */
      bcopy ((char *)&a_bindingVector->binding_h[curr_srv],
	     (char *)&(curr_conn->h),
	     sizeof(rpc_binding_handle_t));

      hostNameFound = compat_print_hostname_from_binding(curr_conn->h);
      if (hostNameFound == (char *)0) {
	  dce_svc_printf(FSP_S_FP_CANT_GET_NAME_MSG , rn);
	  curr_conn->hostName[0] = '\0';
      }
      else {
	  strcpy (curr_conn->hostName, hostNameFound);
	  dce_svc_printf(FSP_S_FP_GOT_HOST_NAME_MSG, rn, curr_conn->hostName);
      }

      /*
       * Bump the current fsprobe connection to set up.
       */
      curr_conn++;

    } /*for curr_srv*/

    code = pthread_attr_create(&threadAttr);
    code = pthread_attr_setstacksize(&threadAttr, THREAD_STACK_SIZE);
    code = pthread_create(&probeTHREAD_ID, threadAttr,
			  (pthread_startroutine_t)fsprobe_THREAD, (pthread_addr_t)0);
    if (code) {
	dce_svc_printf(FSP_S_FP_CANT_CREATE_FSPROBE_THREAD_MSG, rn, code);
	fsprobe_Cleanup(1); 
	return (code);
    }
    if (fsprobe_debug)
      dce_svc_printf(FSP_S_FP_CREATED_FSPROBE_THREAD_MSG, rn, probeTHREAD_ID);

    /*
     * Return the final results.
     */
    if (conn_err)
      return(-2);
    else
      return(0);

} /*fsprobe_Init*/


/*------------------------------------------------------------------------
 * [exported] fsprobe_ForceProbeNow
 *
 * Description:
 *	Wake up the probe THREAD, forcing it to execute a probe immediately.
 *
 * Arguments:
 *	None.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	The module must have been initialized.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int fsprobe_ForceProbeNow()

{ /*fsprobe_ForceProbeNow*/

    static char rn[] = "fsprobe_ForceProbeNow";	/*Routine name*/

    /*
     * There isn't a prayer unless we've been initialized.
     */
    if (!fsprobe_initflag) {
      dce_svc_printf(FSP_S_FP_MUST_CALL_INIT_MSG, rn);
      return(-1);
    }

    /*
     * We did it, so report the happy news.
     */
    return(0);

} /*fsprobe_ForceProbeNow*/
