/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
*/
/*
 * (C) Copyright Transarc Corporation 1995, 1989
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */
/*
 * $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/fsprobe/RCS/fsprobe.h,v 4.6 1995/07/26 16:08:44 blake Exp $
 */

#ifndef TRANSARC_FSPROBE_H
#define	TRANSARC_FSPROBE_H  1

/*------------------------------------------------------------------------
 * fsprobe.h
 *
 * Interface to the AFS FileServer probe facility.  With the routines
 * defined here, the importer can gather statistics from the given set
 * of FileServers at regular intervals, or force immediate collection.
 *
 * Author:
 *	Ed. Zayas
 *	Transarc Corporation
 *------------------------------------------------------------------------*/

#include <dcedfs/sysincludes.h>
#include <dcedfs/osi_net.h>
#include <dcedfs/afs4int.h>		/*AFS FileServer interface*/
#include <dce/rpc.h>

/*
  * Connection information per FileServer host being probed.
  */
struct fsprobe_ConnectionInfo {
#ifdef notdef
    struct sockaddr_in skt;		/*Socket info*/
    struct rx_connection *rxconn;	/*Rx connection*/
#endif
    rpc_binding_handle_t h;             /* connection handle */

    char hostName[256];			/*Computed hostname*/
};

/*
  * The results of a probe of the given set of FileServers.  The ith
  * entry in the stats array corresponds to the ith connected server.
  */
struct fsprobe_ProbeResults {
    int probeNum;			/*Probe number*/
    long probeTime;			/*Time probe initiated*/
    afsStatistics *stats;	        /*Ptr to stats array for servers*/
    int *probeOK;			/*Array: was latest probe successful?*/
};

extern int fsprobe_numServers;				/*# servers connected*/
extern struct fsprobe_ConnectionInfo *fsprobe_ConnInfo;	/*Ptr to connections*/
extern int numCollections;				/*Num data collections*/
extern struct fsprobe_ProbeResults fsprobe_Results;	/*Latest probe results*/

extern int fsprobe_Init();
    /*
     * Summary:
     *    Initialize the fsprobe module: set up Rx connections to the
     *	  given set of servers, start up the probe and callback THREADs,
     *	  and associate the routine to be called when a probe completes.
     *
     * Args:
     *	  int a_numServers		    : Num. servers to connect to.
     *	  struct sockaddr_in *a_socketArray : Array of server sockets.
     *	  int a_ProbeFreqInSecs		    : Probe frequency in seconds.
     *	  int (*a_ProbeHandler)()	    : Ptr to probe handler fcn.
     *	  int a_debug			    : Turn debugging output on?
     *
     * Returns:
     *	  0 on success,
     *	  Error value otherwise.
     */

extern int fsprobe_ForceProbeNow();
    /*
     * Summary:
     *    Force an immediate probe to the connected FileServers.
     *
     * Args:
     *	  None.
     *
     * Returns:
     *	  0 on success,
     *	  Error value otherwise.
     */

extern int fsprobe_Cleanup();
    /*
     * Summary:
     *    Clean up our memory and connection state.
     *
     * Args:
     *	  int a_releaseMem : Should we free up malloc'ed areas?
     *
     * Returns:
     *	  0 on total success,
     *	  -1 if the module was never initialized, or there was a problem
     *		with the fsprobe connection array.
     */

#endif /* TRANSARC_FSPROBE_H */
