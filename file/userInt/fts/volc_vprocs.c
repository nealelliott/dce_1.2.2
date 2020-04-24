/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <ctype.h>
#include <dcedfs/stds.h>
#include <dcedfs/vol_errs.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/ftserver_data.h>
#include <dcedfs/fldb_data.h>
#include <dcedfs/flserver.h>
#include <dcedfs/ftserver.h>
#include <dcedfs/ftutil.h>
#include <dcedfs/queue.h>
#include <dcedfs/volume.h>
#include <dcedfs/ftserver_trans.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/dfsauth.h>
#include <dcedfs/lock.h>
#include <dcedfs/bomb.h>
#include <dcedfs/ftutil.h>
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */
#include <dcedfs/tkm_tokens.h>

#include <volc.h>

#ifndef BUFSIZ
#define BUFSIZ 1024
#endif /* BUFSIZ */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftimac.h>
#include <dfsftisvc.h>
#include <dfsftimsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/userInt/fts/RCS/volc_vprocs.c,v 4.385 1996/10/16 12:19:34 gait Exp $")

/* forward declarations */
static long CheckLocalMount _TAKES((rpc_binding_handle_t,
				     struct sockaddr *,
				     afs_hyper_t *,
				     unsigned long,
				     char *));

static long RenameEntry _TAKES((struct vldbentry *,
				 long,
				 long,
				 int *,
				 int *));

static int CloneVolume _TAKES((rpc_binding_handle_t,
				struct sockaddr *,
				u_char *,
				unsigned long,
				afs_hyper_t *,
				char *,
				long,
				afs_hyper_t *,
				int,
				int,
				int));

static int CopyCloneVolume _TAKES((rpc_binding_handle_t,
				    unsigned long,
				    afs_hyper_t *,
				    afs_hyper_t *,
				    char *,
				    afs_hyper_t *,
				    rpc_binding_handle_t,
				    unsigned long,
				    struct vldbentry *,
				    struct sockaddr *,
				    u_char *,
				    struct sockaddr *,
				    u_char *,
				    unsigned long,
				    long *,
				    long *,
				    int *,
				    int,
				    int,
				    unsigned long,
				    afs_hyper_t *,
				    afs_hyper_t *,
				    char *,
				    int));

static int ForwardVolume _TAKES((struct vldbentry *,
				  rpc_binding_handle_t,
				  unsigned long,
				  afs_hyper_t *,
				  afs_hyper_t *,
				  afs_hyper_t *,
				  struct ftserver_Date *,
				  rpc_binding_handle_t,
				  struct sockaddr *,
				  long *,
				  u_char *,
				  unsigned long,
				  int));

static long SetMoveTags _TAKES((rpc_binding_handle_t,
				 long *,
				 struct sockaddr *,
				 afs_hyper_t *,
				 unsigned long,
				 int));
#define	MOVETAG_SOURCE	0
#define	MOVETAG_TARGET	1
#define	MOVETAG_FETCH	2

static long SetMoveBaseStatus _TAKES((rpc_binding_handle_t,
				       long,
				       unsigned long,
				       unsigned long));

static long FinishMoveTags _TAKES((struct vldbentry *,
				    rpc_binding_handle_t,
				    struct sockaddr *,
				    unsigned long,
				    afs_hyper_t *));

static long ClearMoveTags _TAKES((struct sockaddr *,
				   afs_hyper_t *,
				   unsigned long,
				   int,
				   int));

static long SetFlags _TAKES((rpc_binding_handle_t,
			      unsigned long,
			      afs_hyper_t *,
			      unsigned long));

static long SwapIntoPlace _TAKES((rpc_binding_handle_t,
				   long,
				   afs_hyper_t *,
				   afs_hyper_t *,
				   unsigned long,
				   unsigned long,
				   int,
				   afs_hyper_t *,
				   afs_hyper_t *,
				   struct sockaddr *));

static long threeIDsFromOne _TAKES((afs_hyper_t *,
				    struct sockaddr *,
				   bulkIDs *newIDsp));

static void SetupDest(destp, servAddrp, pname)
    struct ftserver_dest *destp;
    struct sockaddr *servAddrp;
    u_char *pname;
{
    /* XXX: configuration */
    bzero((char *)destp, sizeof(struct ftserver_dest));
    destp->destSSID = 1;
    *((struct sockaddr *) &destp->address) = *servAddrp;
    strncpy((char *)&destp->prinName[0], (char *)pname,
	    sizeof(destp->prinName));
}

#define MAXFTCONNS 35 /* XXX needs to be dynamic */

static struct lock_data connLock;
static struct serverConn {
    u_long ipaddr;
    rpc_binding_handle_t tconn;
    int needAuth;
    int maxTimeoutValue;
    int serverKind;
    u_long timeUsed;
} serverConns[MAXFTCONNS];
static int serverConnInited = 0;
static pthread_once_t connInitedBlock = pthread_once_init;

static void initServerConns() {
    lock_Init(&connLock);
    bzero((char *)&serverConns[0], sizeof(serverConns));
    /* Initialize the table used for decoding the error returned by DFS calls */
    fts_InitDecoder();
    serverConnInited = 1;
}

static void checkInitConns() {
    if (serverConnInited == 0) {
	if (pthread_once(&connInitedBlock, &initServerConns) != 0) {
	    VOLSERLOG(0, "Cannot init conns with pthread_once: %d\n", errno);
	}
    }
}

SHARED osi_dlock_t transKALock;
static pthread_once_t transKALockBlock = pthread_once_init;
static int inited_transKALock = 0;

static void initTransKALock() {
    lock_Init(&transKALock);
    inited_transKALock = 1;
}

static void checkKALockInit() {
    if (inited_transKALock == 0) {
	if (pthread_once(&transKALockBlock, &initTransKALock) != 0)
	    VOLSERLOG(0, "Cannot init lock with pthread_once: %d\n", errno);
    }
}

long CacheServerConnection(servAddrP, bindingHandle, needAuth)
    struct sockaddr *	servAddrP;
    rpc_binding_handle_t bindingHandle;
    int needAuth;
{
    long				rtnVal = 0;
    error_status_t		rpcStatus = error_status_ok;
    register int			index, minIndex;
    register struct sockaddr_in *	sin = (struct sockaddr_in *)servAddrP;
    u_long		minTime;
    static char			routineName[] = "CacheServerConnection";

    /* initialize, if needed */
    checkInitConns();

    /* all the cached connections are encountered before the zeroes */
    lock_ObtainWrite(&connLock);
    for (index = 0; index < MAXFTCONNS; index++) {
	if (serverConns[index].ipaddr == sin->sin_addr.s_addr) {
	    serverConns[index].timeUsed = (u_long) osi_Time();
	    break;
	}
	else if (serverConns[index].ipaddr == 0) {
	    /* insert the connection info here */
	    serverConns[index].ipaddr = sin->sin_addr.s_addr;
	    serverConns[index].timeUsed = (u_long) osi_Time();
	    rpc_binding_copy(bindingHandle,
			     &(serverConns[index].tconn),
			     &rpcStatus);
	    serverConns[index].needAuth = needAuth;
	    serverConns[index].serverKind = SERVERKIND_ANY;
	    serverConns[index].maxTimeoutValue = -1;
	    if (rpcStatus != error_status_ok) {
		VOLSERLOG(1, "%s: error copying rpc binding handle (%ld): %s\n",
			  routineName, rpcStatus, dfs_dceErrTxt(rpcStatus));
		rtnVal = (long)rpcStatus;
		bzero((char *)&(serverConns[index]),
		      sizeof(serverConns[index]));
	    }
	    break;
	}
    }
    /* use lru algorithm to maintain cached server connections */
    if (index >= MAXFTCONNS) {
	minIndex = 0; minTime = serverConns[0].timeUsed;
	for (index = 1; index < MAXFTCONNS; ++index) {
	    if (serverConns[index].timeUsed < minTime) {
		minIndex = index;
		minTime = serverConns[index].timeUsed;
	    }
	}
	/* overwrite the oldest entry */
	rpc_binding_free(&(serverConns[index].tconn), &rpcStatus);
	bzero((char *)&(serverConns[index]), sizeof(serverConns[index]));
	/* insert the connection info here */
	serverConns[index].ipaddr = sin->sin_addr.s_addr;
	serverConns[index].timeUsed = (u_long) osi_Time();
	serverConns[index].needAuth = needAuth;
	serverConns[index].serverKind = SERVERKIND_ANY;
	serverConns[index].maxTimeoutValue = -1;
	rpc_binding_copy(bindingHandle,
			 &(serverConns[index].tconn),
			 &rpcStatus);
	if (rpcStatus != error_status_ok) {
	    VOLSERLOG(1, "%s: error copying rpc binding handle (%ld): %s\n",
		      routineName, rpcStatus, dfs_dceErrTxt(rpcStatus));
	    rtnVal = (long)rpcStatus;
	    bzero((char *)&(serverConns[index]),
		  sizeof(serverConns[index]));
	}
    }
    lock_ReleaseWrite(&connLock);
    return rtnVal;
}

rpc_binding_handle_t connToServer(servAddrp, prinName, serverKind)
struct sockaddr *servAddrp;
u_char *prinName;
int serverKind;
{
    register struct sockaddr_in *sin;
    rpc_binding_handle_t tconn;
    error_status_t		rpcStatus = error_status_ok;
    register unsigned index, minIndex;
    u_long minTime;
    char pnameBuf[600];
    struct siteDesc site;
    long code;
    long		dfsauthCode=0;
    char		errorBuffer[1000];
    static char		routineName[] = "connToServer";

    checkInitConns();
    sin = (struct sockaddr_in *) servAddrp;
    lock_ObtainWrite(&connLock);
    /* valid addresses are before the zeros */
    for (index = 0; index < MAXFTCONNS; ++index) {
	if (serverConns[index].ipaddr == sin->sin_addr.s_addr
	    && (serverConns[index].serverKind == serverKind
		|| serverConns[index].serverKind == SERVERKIND_ANY)) {
	    /* we have this one already */
	    serverConns[index].serverKind = serverKind;
	    break;
	}
	else if (serverConns[index].ipaddr == 0) {
	    serverConns[index].ipaddr = sin->sin_addr.s_addr;
	    if (rpcx_binding_from_sockaddr((struct sockaddr *)sin,
					  &serverConns[index].tconn) != 0) {
		bzero((char *)&(serverConns[index]),
		      sizeof(serverConns[index]));
		lock_ReleaseWrite(&connLock);
		return (rpc_binding_handle_t)NULL;
	    }
	    serverConns[index].needAuth = 1;
	    serverConns[index].serverKind = serverKind;
	    serverConns[index].maxTimeoutValue = -1;
	    break;
	}
    }
    if (index >= MAXFTCONNS) {
	/* use lru caching, overwrite the oldest entry */
	minIndex = 0; minTime = serverConns[0].timeUsed;
	for (index = 1; index < MAXFTCONNS; ++index) {
	    if (serverConns[index].timeUsed < minTime) {
		minIndex = index;
		minTime = serverConns[index].timeUsed;
	    }
	}
	rpc_binding_free(&(serverConns[index].tconn), &rpcStatus);
	bzero((char *)&(serverConns[index]), sizeof(serverConns[index]));
	/* insert the connection info here */
	serverConns[index].ipaddr = sin->sin_addr.s_addr;
	if (rpcx_binding_from_sockaddr((struct sockaddr *)sin,
				      &serverConns[index].tconn) != 0) {
	    bzero((char *)&(serverConns[index]),
		  sizeof(serverConns[index]));
	    lock_ReleaseWrite(&connLock);
	    return (rpc_binding_handle_t)NULL;
	}
	serverConns[index].needAuth = 1;
	serverConns[index].serverKind = serverKind;
	serverConns[index].maxTimeoutValue = -1;
    }
    if (serverConns[index].needAuth) {
	if (prinName == NULL) {
	    code = vldb_GetSiteInfo(sin, &site);
	    if (code == 0)
		prinName = &site.KerbPrin[0];
	}
	if (prinName != NULL) {
	    sprintf(pnameBuf, "%s/%s/dfs-server", currentCellName, prinName);
	    dfsauthCode = dfsauth_client_InitBindingAuth(
						 &(serverConns[index].tconn),
						 useNoAuth, useLocalAuth,
						 pnameBuf);
	    if (dfsauthCode) {
		dfs_copyDceErrTxt(dfsauthCode, errorBuffer,
				  sizeof(errorBuffer));
		dce_svc_printf(FTS_S_VPROCS_CONNTOSERVER_AUTH_MSG,
			       MapSockAddr(sin), useNoAuth, useLocalAuth,
			       pnameBuf, serverKind, errorBuffer);
		/* return dfsauthCode; */ /* FALL THROUGH */
	    }
	}
	else
	    dce_svc_printf(FTS_S_VPROCS_CONNTOSERVER_NO_PRINCIPAL_MSG,
			   MapSockAddr(sin), serverKind, dfs_dceErrTxt(code));
	serverConns[index].needAuth = 0;
    }
    serverConns[index].timeUsed = (u_long) osi_Time();
    if (serverConns[index].maxTimeoutValue < 0) {
	rpc_mgmt_set_com_timeout(serverConns[index].tconn,
				 rpc_c_binding_default_timeout-1,
				 &rpcStatus);
	if (rpcStatus != error_status_ok) {
	    dfs_copyDceErrTxt(rpcStatus, errorBuffer, sizeof(errorBuffer));
	    dce_svc_printf(FTS_S_VPROCS_CONNTOSERVER_COM_TIMEOUT_MSG,
			   MapSockAddr(sin), rpcStatus, errorBuffer);
	} else {
	    serverConns[index].maxTimeoutValue =
	      rpc_c_binding_default_timeout - 1;
	}
    }
    lock_ReleaseWrite(&connLock);
    return serverConns[index].tconn;
}

/* Make the timeout value at least as large as the argument. */
long fts_UseThisTimeout(handlep, timeoutValue)
rpc_binding_handle_t handlep;
int timeoutValue;
{
    register int ix;
    char errorBuffer[1000];
    error_status_t st;

    for (ix = 0; ix < MAXFTCONNS; ++ix) {
	if (serverConns[ix].tconn == handlep)
	    break;
    }
    if (ix >= MAXFTCONNS)
	return -1;	/* no match in the table */
    if (serverConns[ix].maxTimeoutValue < timeoutValue) {
	rpc_mgmt_set_com_timeout(serverConns[ix].tconn,
				 timeoutValue, &st);
	if (st != error_status_ok) {
	    dfs_copyDceErrTxt(st, errorBuffer, sizeof(errorBuffer));
	    VOLSERLOG(0, "fts_UseThisTimeout: error setting com_timeout \
(%ld): %s\n",
		      st, errorBuffer);
	} else {
	    serverConns[ix].maxTimeoutValue = timeoutValue;
	}
    }
    return 0;
}

void fts_GetIndices(entryp, xl)
struct vldbentry *entryp;
int *xl;
{
    int j, k;
    for (j = 0; j < MAXTYPES; ++j) xl[j] = -1;
    for (k = 0; k < MAXVOLTYPES && !AFS_hiszero(entryp->VolIDs[k]); ++k) {
	for (j = 0; j < MAXTYPES; ++j) {
	    if (entryp->VolTypes[k] == VOLTIX_TO_VOLTYPE(j)) {
		xl[j] = k;
		break;
	    }
	}
    }
}

/*
 * Common calling routine for all VC_* calls that require connection to a server
 */
volc_Call(proc, servAddrp, prinName, p2, p3, p4, p5, p6, p7, p8, p9)
    int (*proc)();
    struct sockaddr *servAddrp;
    u_char *prinName;
    char *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;
{
    register rpc_binding_handle_t  connp;
    register int code;
    static char	routineName[] = "volc_Call";

    if (servAddrp) {
	connp = connToServer(servAddrp, prinName, SERVERKIND_FT);
	if (connp == (rpc_binding_handle_t)NULL)
	  return FTS_ERR_BADCONN;

	code = (*proc)(connp, servAddrp, p2, p3, p4, p5, p6, p7, p8, p9);

	/* rpc_binding_free(&connp, &st); */ /* NO--connToServer caches this */
    } else {
	code = (*proc)(p2, p3, p4, p5, p6, p7, p8, p9);
    }

    return code;
}


/*
 * Create a volume in [servAddrp, aggrId] with the name volNamep; On a
 * successful return VolIdp contains the new volume's Id.
 */
long VC_CreateVolume(connp, servAddrp, aggrId, volNamep, volIdp)
    register rpc_binding_handle_t connp;
    struct sockaddr *servAddrp;
    unsigned long aggrId;
    char *volNamep;
    afs_hyper_t *volIdp;
{
    long transId, code;
    afs_hyper_t readonlyId, backupId;
    struct ftserver_status ftstat;
    bulkIDs newIDs;

    checkInitConns();
    if (AFS_hiszero(*volIdp)) {
	/* Get three new fileset IDs in reference to this server */
	code = vldb_GetNewVolumeIds(3, servAddrp, &newIDs);
	if (code != 0) return code;
	if (newIDs.bulkIDs_len != 3) {
	    VOLSERLOG(1,
		      "Internal inconsistency: asked for 3 IDs, but got %d\n",
		      newIDs.bulkIDs_len);
	    return VL_BADID; /* a more meaningful error */
	}
    } else {
	/* One ID was given by the caller; get three free ones. */
	code = threeIDsFromOne(volIdp, servAddrp, &newIDs);
	if (code != 0)
	    return code;
    }
    bzero((char *)&ftstat, sizeof(ftstat));
    *volIdp = newIDs.bulkIDs_val[0];
    ftstat.vsd.cloneId = newIDs.bulkIDs_val[1];
    ftstat.vsd.backupId = newIDs.bulkIDs_val[2];

    /*
     * The RPC call to the Volume server to actually create the volume; note
     * that the volume, by default, stays offline after its creation.
     */
    if (code = vols_CreateVolume(connp, aggrId, volNamep,
				 VOLTIX_TO_VOLTYPE(RWVOL),
				 FLAGS_ENCODE(FTSERVER_OP_SETSTATUS |
					      FTSERVER_OP_DELETE,
					      VOLERR_TRANS_CREATEFILESET),
				 &osi_hZero, volIdp, &transId)) {
	return code;
    }
    /* Bring new volume on-line. */
    ftstat.vss.states = VOL_TYPE_RW | VOL_RW | VOL_REP_NONE;
    if (code = vols_SetStatus(connp, transId,
			       VOL_STAT_STATES
			       | VOL_STAT_BACKUPID | VOL_STAT_CLONEID
			       | VOL_STAT_PARENTID
			       | VOL_STAT_LLBACKID | VOL_STAT_LLFWDID,
			       &ftstat)) {
	(void) vols_DeleteVolume(connp, transId);
	(void) vols_AbortTrans(connp, transId);
	return code;
    }
    VOLSERLOG(1, "Fileset %s (%lu,,%lu) created and brought online\n",
	      volNamep, AFS_HGETBOTH(*volIdp));
    /*
     * Create a new VLDB entry to represent the newly created volume.
     */
    if (code = vldb_CreateVldbEntry(volNamep, servAddrp, aggrId,
				    VLF_RWEXISTS,
				    volIdp, &ftstat.vsd.cloneId,
				    &ftstat.vsd.backupId, 0, 0)) {
	VOLSERLOG(1, "Creating vldb entry failed; deleting the newly created \
fileset %lu,,%lu\n",
		  AFS_HGETBOTH(*volIdp));
	(void) vols_DeleteVolume(connp, transId);
	(void) vols_AbortTrans(connp, transId);
	return code;
    }
    code = vols_DeleteTrans(connp, transId);
    return code;
}

/*
 * Delete the volume volIdp at [servAddrp, aggrId]
 */
long VC_DeleteVolume(connp, servAddrp, aggrId, volIdp)
    rpc_binding_handle_t connp;
    struct sockaddr *servAddrp;
    unsigned long aggrId;
    afs_hyper_t *volIdp;
{
    long code, tcode;
    struct vldbentry entry;
    int xl[MAXTYPES];
    int rwIx, roIx, tokenDesc;

    checkInitConns();
    /*
     * First, lock vldb entry for the duration of the call
     */
    if (code = vldb_SetLock(volIdp, -1, VLOP_DELETE))
	return code;
    /*
     * Now Get hold of the vldb entry for the volume to remove
     */
    if (code = vldb_GetEntryByID(volIdp, -1, &entry, 0))
	return (vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL));

    fts_GetIndices(&entry, xl);

    if (xl[BACKVOL] >= 0 && (AFS_hsame(*volIdp, entry.VolIDs[xl[BACKVOL]])) &&
	(entry.flags & VLF_BACKEXISTS)) {
	/*
	 * It's the backup volume: delete it and remove it from the VLDB entry
	 */
	if (CheckLocalMount(connp, servAddrp, volIdp, aggrId, "deleted") > 0) {
	    (void) vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	    return FTSERVER_ERR_LCLMOUNT;
	}
	code = fts_DeleteVolume(connp, aggrId, volIdp, 0, 0);
	if (code == 0 || code == ENOENT || code == ENODEV ||
	    code == FTSERVER_ERR_NOVOL)
	    return (vldb_RemoveVolume(&entry, servAddrp, aggrId, BACKVOL,
				      volIdp));
    } else if (xl[ROVOL] >= 0 && (AFS_hsame(*volIdp, entry.VolIDs[xl[ROVOL]])) &&
	       (entry.flags &VLF_ROEXISTS)) {
	/*
	 * It's the RO volume: Make sure its site matches the one in VLDB and
	 * then delete it and remove it from the VLDB entry.
	 */
	roIx = vldb_FindRepSite(&entry, servAddrp, aggrId,
				((unsigned32)VLSF_ZEROIXHERE) >> xl[ROVOL]);
	if (roIx >= 0) {
	    if (entry.flags & VLF_LAZYREP) {
		rwIx = vldb_FindRepSite(&entry, 0, 0,
				((unsigned32)VLSF_ZEROIXHERE) >> xl[RWVOL]);
		if (rwIx >= 0) {
		    if (bcmp((char *)&entry.siteAddr[roIx],
			     (char *)&entry.siteAddr[rwIx],
			     sizeof(afsNetAddr)) != 0) {
			dce_svc_printf(FTS_S_VPROCS_RMSITE_NOT_DELETE_MSG);
			code = vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
			return (code ? code : FTS_ERR_NOVOL);
		    }
		}
	    }
	    vldb_SaveOldEntry(&entry);
	    if (CheckLocalMount(connp, servAddrp, volIdp, aggrId, "deleted")
		> 0) {
		(void) vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
		return FTSERVER_ERR_LCLMOUNT;
	    }
	    code = fts_DeleteVolume(connp, aggrId, volIdp, 0, 0);
	    if (code == 0 || code == ENOENT || code == ENODEV ||
		code == FTSERVER_ERR_NOVOL) {
		code = vldb_RemoveVolume(&entry, servAddrp, aggrId, ROVOL,
					 volIdp);
		if (!code) vldb_TellRepAboutNewEntry(&entry, 0, 1);
		return code;
	    }
	} else {
	    code = FTS_ERR_NOVOL;
	}
    } else if (xl[RWVOL] >= 0 && (AFS_hsame(*volIdp, entry.VolIDs[xl[RWVOL]])) &&
	       (entry.flags &VLF_RWEXISTS)) {
	/*
	 * It's the RW volume itself: Make sure its site matches the one in
	 * the FLDB; then delete it and remove it from the VLDB entry along
	 * with the its backup volume.
	 */

	/* don't delete if a RO exists */
	
	/* does RO exist? */
	
	if (entry.flags & VLF_ROEXISTS) {
		/* yes, return error */
	    VOLSERLOG(0,"There is a RO copy in existence - please \
'rmsite' before 'delete'.\n");
		vldb_ReleaseLock(volIdp,-1,LOCKREL_ALL);
	
		return FTS_ERR_NOVOL;
	}
	
	/* no, carry on */

	if (vldb_FindRepSite(&entry, servAddrp, aggrId,
			    ((unsigned32)VLSF_ZEROIXHERE) >> xl[RWVOL]) != -1) {
	    if (CheckLocalMount(connp, servAddrp, volIdp, aggrId, "deleted")
		> 0) {
		(void) vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
		return FTSERVER_ERR_LCLMOUNT;
	    }
	    if (entry.flags & VLF_BACKEXISTS) {
		if (CheckLocalMount(connp, servAddrp,
				    &entry.VolIDs[xl[BACKVOL]],
				    aggrId, "deleted") > 0) {
		    (void) vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
		    return FTSERVER_ERR_LCLMOUNT;
		}
	    }
	    /* Get all CMs to notice that this fileset is being deleted. */
	    code = fts_GetToken((afsNetAddr *)servAddrp, NULL, volIdp,
				TKN_UPDATE /* | TKN_LOCK_WRITE */,
				AFS_FLAG_FORCEVOLQUIESCE |
				AFS_FLAG_FORCEREVOCATIONS,
				&tokenDesc);
	    if (code == 0 || code == VOLERR_PERS_DELETED) {
		code = fts_DeleteVolume(connp, aggrId, volIdp, 0, 0);
		if (code == ENOENT || code == ENODEV ||
		    code == FTSERVER_ERR_NOVOL)
		    code = 0;
		if (!code) {
		    if (entry.flags & VLF_BACKEXISTS) {
			code = BOMB_EXEC("VC_DeleteVolume.1",
					 fts_DeleteVolume(connp, aggrId,
					    &entry.VolIDs[xl[BACKVOL]], 0, 0));
			if (code == ENOENT || code == ENODEV
			    || code == FTSERVER_ERR_NOVOL)
			    code = 0;
			if (!code)
			  code = vldb_RemoveVolume(&entry, servAddrp, aggrId,
						     BACKVOL, volIdp);
		    } else {
			code = 0;
		    }
		    if (!code) {
			code = vldb_RemoveVolume(&entry, servAddrp, aggrId,
						 RWVOL, volIdp);
			if (!code) vldb_TellRepAboutNewEntry(&entry, 0, 1);
			(void) fts_ReleaseToken(tokenDesc);
			return code;
		    }
		}
		(void) fts_ReleaseToken(tokenDesc);
	    }
	} else {
	    code = FTS_ERR_NOVOL;
	}
    } else {
      /* XX not neccessary */
	dce_svc_printf(FTS_S_VPROCS_FILESET_NOT_EXIST_MSG,
		       AFS_HGETBOTH(*volIdp));
	code = FTS_ERR_NOVOL;
    }
    /*
     * All done; release the lock
     */
    tcode = vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
    return (code ? code : tcode);
}


/*
 * Make a new backup of volume <volIdp> on [servAddr, aggrId]
 * if one already exists, update it
 */
long VC_BackupVolume(connp, servAddrp, aggrId, volIdp)
    rpc_binding_handle_t connp;
    struct sockaddr *servAddrp;
    unsigned long aggrId;
    afs_hyper_t *volIdp;
{
    afs_hyper_t backupId;
    long code, tcode, backupExists;
    char volName[FTSERVER_MAXFSNAME];
    struct vldbentry entry;
    int xl[MAXTYPES];
    int weLockedIt = 0, mustLock = 0, changedFldb = 0;
    int ix;

    checkInitConns();
    /*
     * Get an unlocked copy of the vldb entry for the volume to backup.
     */
    if (code = vldb_GetEntryByID(volIdp, RWVOL, &entry, 0))
	goto backup;
    fts_GetIndices(&entry, xl);

    /* Determine whether we must lock this entry first. */

    for (;;) {
	/* Loop if we have to try again. */
	if (mustLock
	    || (entry.flags & VLOP_ALLOPERS) != 0 /* already locked */
	    || (entry.WhenLocked != 0) /* already locked */
	    || (entry.flags & VLF_BACKEXISTS) == 0 /* no bk */
	    || xl[BACKVOL] < 0	/* no room for bk */
	    || AFS_hiszero(entry.VolIDs[xl[BACKVOL]]) /* no bk ID */) {
	    /*
	     * It's locked, or there's no backup fileset.
	     * We have to lock it and create our own.
	     */
	    if (code = vldb_SetLock(volIdp, RWVOL, VLOP_BACKUP))
		return code;
	    VOLSERLOG(1, "Locked %s fldb entry for backup.\n",
		      entry.name);
	    weLockedIt = 1;
	    /* Having locked it, re-fetch in case it had changed. */
	    if (code = vldb_GetEntryByID(volIdp, RWVOL, &entry, 0))
		goto backup;
	    fts_GetIndices(&entry, xl);
	    if (xl[BACKVOL] < 0) {
		/* Need to allocate a slot for the backup ID. */
		for (ix = 0;
		     ix < MAXVOLTYPES && !AFS_hiszero(entry.VolIDs[ix]);
		     ++ix) {
		}
		if (ix < MAXVOLTYPES) {
		    xl[BACKVOL] = ix;
		} else {
		    VOLSERLOG(0, "No room for a backup ID in fldb entry \
for %lu,,%lu.\n",
			      AFS_HGETBOTH(*volIdp));
		    code = FTS_ERR_NEED_BK;
		    goto backup;
		}
	    }
	    /* Get the backup fileset ID, allocating one if necessary. */
	    code = vldb_GetVolumeId(&entry.VolIDs[xl[BACKVOL]], &backupId,
					(char *)0, &tcode, servAddrp);
	    if (tcode)
		changedFldb = 1;
	    if (code)
		goto backup;
	} else {
	    VOLSERLOG(1, "Optimistically NOT locking %s entry for backup.\n",
		      entry.name);
	    backupId = entry.VolIDs[xl[BACKVOL]];
	}

	/*
	 * Check if the backup volume already exists; set backupExists, if so
	 */
	if (code = fts_VolumeExists(connp, aggrId, &backupId)) {
	    if (code != EEXIST)
		goto backup;
	    backupExists = 1;
	} else {
	    /* But we can't just create one without having locked. */
	    if (weLockedIt == 0) {
		/* Loop back, and lock the entry this time. */
		VOLSERLOG(1, "No actual backup vol for %s (%lu,,%lu): \
looping back.\n",
			  entry.name, AFS_HGETBOTH(backupId));
		mustLock = 1;
		continue;
	    }
	    backupExists = 0;
	}
	break;
    }
    /*
     * Clone the new backup volume with the proper name.
     */
/* XXX: backup name should come from cell profile. */
#ifndef FTS_BACKUP_NAME
#define FTS_BACKUP_NAME ".backup"
#endif
    if ((strlen((char *)entry.name) + strlen(FTS_BACKUP_NAME)) <=
	(unsigned)FTSERVER_MAXFSNAME) {
	strcpy(volName, (char *)entry.name); strcat(volName, FTS_BACKUP_NAME);
	code = CloneVolume(connp, servAddrp, NULL, aggrId, volIdp, volName,
			   VOLTIX_TO_VOLTYPE(BACKVOL), &backupId,
			   backupExists, 1, 0);
    }
    else code = FTS_ERR_BADNAME;

    if (!code) {
	/*
	 * Bring new backup volume Online
	 */
	code = VOL_TYPE_BK | VOL_READONLY;
	if (entry.flags & VLF_LAZYREP) {
	    if (entry.flags & VLF_LAZYRELE) {
		code |= VOL_REP_RELEASE;
	    } else {
		code |= VOL_REP_LAZY;
	    }
	}
	code = SetFlags(connp, aggrId, &backupId, code);
	if (!code && weLockedIt) {
	    /*
	     * Add the new backup site info to the vldb entry
	     */
	    changedFldb = 1;	/* this can make FLDB changes */
	    code = vldb_AddVolume(&entry, servAddrp, aggrId, BACKVOL,
				  &backupId);
	    if (code == 0)
		changedFldb = 0;	/* changes now written */
	}
    }
backup:
    /*
     * All done, release the lock
     */
    if (weLockedIt) {
	if (changedFldb) {
	    tcode = vldb_ReplaceEntry(&entry, volIdp, RWVOL, LOCKREL_ALL);
	} else {
	    tcode = vldb_ReleaseLock(volIdp, RWVOL, LOCKREL_ALL);
	}
	if (!code) code = tcode;
    }
    return (code);
}


/*
 * This routine sets a flag in the state field of the
 * volume on the specified aggregate/server.
 */
long VC_SetStateBits(connp, servAddrp, aggrId, volIdp, flags, set)
    register rpc_binding_handle_t connp;
    struct sockaddr *servAddrp;
    unsigned long aggrId;
    afs_hyper_t *volIdp;
    unsigned long flags;
    int set;
{
    long transId;
    long code;
    long tcode = 0;
    struct ftserver_status ftstat;

    checkInitConns();
    code = vols_CreateTrans(connp, volIdp, aggrId,
			     FLAGS_ENCODE(FTSERVER_OP_SETSTATUS |
					  FTSERVER_OP_GETSTATUS,
					  VOLERR_TRANS_SETSTATUS),
			     &transId, 0);

    if (!code)
    {
	code = vols_GetStatus(connp, transId, &ftstat);
	if (!code)
	{
	    if (set)
	    {
		if ((ftstat.vss.states | flags) != ftstat.vss.states)
		{
		    /* not already set */
		    ftstat.vss.states |= flags;
		    code = vols_SetStatus(connp, transId,
					  VOL_STAT_STATES, &ftstat);
		}
	    }
	    else
	    {
		if ((ftstat.vss.states & ~flags) != ftstat.vss.states)
		{
		    /* is really set */
		    ftstat.vss.states &= ~flags;
		    code = vols_SetStatus(connp, transId,
					  VOL_STAT_STATES, &ftstat);
		}
	    }
	}
	tcode = vols_DeleteTrans(connp, transId);
    }
    return (code ? code : tcode);
}

/*
 * This routine provides ncs space for dump/restore operations
 */

static void fts_DumpAlloc(state, bsize, bufP, outSizeP)
    rpc_ss_pipe_state_t state;
    unsigned long bsize;
    ndr_byte **bufP;
    ndr_ulong_int *outSizeP;
{
    static char buffer[BUFSIZ];

    *bufP = (ndr_byte *)buffer;
    *outSizeP = (ndr_ulong_int)sizeof(buffer);
}

/*
 * This routine handles the calls from ncs pipe code to get more bits.
 */
static void fts_RestoreRead(state, buf, inSize, outSizeP)
    rpc_ss_pipe_state_t state;
    unsigned char *buf;
    unsigned long inSize;
    unsigned long *outSizeP;
{
    int outs;
    ftsPipeState *pipeStateP = (struct ftsPipeState *)state;
    int fd = (int)*(pipeStateP->data);

    for (;;) {
	errno = 0;
	outs = read(fd, buf, inSize);
	if (outs >= 0) break;
	if (errno != EINTR) {
	  if (!pipeStateP->error)
	    pipeStateP->error = errno;
	  /* XX a bug here? If errno != EINTR, *outSizeP will be set to a
	   * negative value instead of zero
	   */
	  if (verbose)
	    dce_svc_printf(FTS_S_VPROCS_FTS_RESTOREREAD_READ_FAILED_MSG,
			   fd, buf, inSize, errno,
			   strerror(errno));
	  break;
	}
	/* otherwise, call read() again. */
    }
    *outSizeP = outs;
    if (verbose) {
	dce_svc_printf(FTS_S_VPROCS_FTS_RESTOREREAD_READ_MSG, *outSizeP, *buf);
    }
}

/*
 * Handles calls from ncs stub routines to store bits from remote.
 */
static void fts_DumpWrite(state, buf, inSize)
    rpc_ss_pipe_state_t state;
    ndr_byte *buf;
    ndr_ulong_int inSize;
{
    register ndr_byte *bp;
    register ndr_ulong_int isiz;
    register int res;
    ftsPipeState *pipeStateP = (struct ftsPipeState *)state;
    int fd = (int)*(pipeStateP->data);


    bp = buf;
    isiz = inSize;
    while (isiz > 0) {
	errno = 0;
	res = write(fd, (char *)bp, isiz);
	if (res <= 0) {
	  if (verbose)
	    dce_svc_printf(FTS_S_VPROCS_FTS_DUMPWRITE_WRITE_FAILED_MSG,
			   fd, bp, isiz, errno,
			   strerror(errno));
	  if (!pipeStateP->error)
	    pipeStateP->error = errno;
	  break;
	}
	bp += res;
	isiz -= res;
	/* go back and check for completion, in case it was a partial write. */
    }
}

/*
 * Dump the volume <volIdp> starting from <dumpDatep>. <dumpFunction> does the
 * real work behind the scenes (its parameter <filename> is actually the
 * filename where to dump it)
 */
long VC_DumpVolume(tconn, servAddrp, volIdp,  aggrId, dumpDatep,
	      filename, dataPipeP, sockFlag)
    volatile rpc_binding_handle_t tconn;
    struct sockaddr * volatile servAddrp;
    afs_hyper_t * volatile volIdp;
    volatile unsigned long aggrId;
    struct ftserver_Date *dumpDatep;
    char * volatile filename;		/* dump to this entity */
    pipe_t * volatile dataPipeP;	/* alternate destination */
    unsigned sockFlag;			/* protocol domain for sockets */
{
    volatile error_status_t code, code2, code3;
    error_status_t code4;
    volatile pipe_t filePipe;
    volatile int fd, tokenDesc;
    volatile long transId;
    char *excNameP;
    char excBuff[50];
    extern char * ExceptionToString();
    volatile ftsPipeState pipeState;

#if defined(OSF_NO_SOCKET_DUMP)
    assert(sockFlag == 0);
#endif
    checkKALockInit();
    checkInitConns();
    /* XXXX WHY Lock the VLDB ENTRY if we don't do anything with it!!! XXXX
     * XXXX because it wants to be a read lock? XXXXX
     */
    code = vldb_SetLock(volIdp, -1, VLOP_DUMP);
    if (code)
	return code;

    if (dataPipeP != (pipe_t *)NULL)
      filePipe = *dataPipeP;
    else {
      if (filename != (char *)NULL) {
	/* open dump file */
	fd = open((char *)filename, O_CREAT|O_TRUNC|O_WRONLY, 0666);
	if (fd == -1) {
	    dce_svc_printf(FTS_S_VPROCS_CANT_CREATE_FILE_MSG,
			   filename, errno, strerror(errno));
	}
      } else {
	fd = fileno(stdout);
	if (fd < 0) {
	    dce_svc_printf(FTS_S_VPROCS_NOT_STANDARD_OUTPUT_MSG);
	    errno = EFAULT;
	}
      }
      if (fd < 0) {
	vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	return errno;
      }
      /* set up pipe parameters */
      if ((pipeState.data = (char *)malloc(sizeof(int))) == (char *)NULL) {
	vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	return (FTS_ERR_NOMEM);
      }
      *(pipeState.data) = (int)fd;
      pipeState.error = 0;
      filePipe.alloc = fts_DumpAlloc;
      filePipe.state = (rpc_ss_pipe_state_t)&pipeState;
      filePipe.push = fts_DumpWrite;
      filePipe.pull = 0;
    }

    /*
     * Call in all the cached changes that might have happened to this fileset.
     */
    code = fts_GetToken((afsNetAddr *)servAddrp, NULL,
			(afs_hyper_t *)volIdp, TKN_READ,
			AFS_FLAG_FORCEVOLQUIESCE, (int *)&tokenDesc);
    if (code != 0) {
	if (filename != NULL) close(fd);
	vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	if (dataPipeP == NULL) free(pipeState.data);
	return code;
    }

    code2 = code3 = 0;
    code = vols_CreateTrans(tconn, (afs_hyper_t *)volIdp,
	(unsigned long)aggrId, FLAGS_ENCODE(
	FTSERVER_OP_DUMP | FTSERVER_OP_GETSTATUS, VOLERR_TRANS_DUMP),
	(long *)&transId, 0);
    if (!code) {
	TRY {
	    /* Start up a periodic keep-alive function too. */
	    code = SetMoveTags(tconn, (long *)&transId,
			(struct sockaddr *)servAddrp,
			(afs_hyper_t *)volIdp,
			(unsigned long)aggrId, MOVETAG_FETCH);
	    if (code == 0)
		code = vols_Dump(tconn, (long)transId,
					dumpDatep, (pipe_t *)&filePipe,
					sockFlag);
	} CATCH_ALL {
	    excNameP = ExceptionToString(THIS_CATCH);
	    if (excNameP == NULL) {
		sprintf(excBuff, "[[unk: %#lx]]", THIS_CATCH);
		excNameP = excBuff;
	    }
	    dce_svc_printf(FTS_S_VPROCS_CAUGHT_FTSERVER_EXCEPTION_MSG, excNameP);
	    code = FTS_ERR_PIPE;
	} ENDTRY
	pipeState = *((ftsPipeState *) filePipe.state);
	if (pipeState.error) {
	    dce_svc_printf(FTS_S_VPROCS_ERROR_FILESET_DUMP_MSG,
			   pipeState.error, strerror(pipeState.error));
	    if (!code)
		code = FTS_ERR_PIPE;
	}
	(void) ClearMoveTags((struct sockaddr *)servAddrp,
		(afs_hyper_t *)volIdp, (unsigned long)aggrId, 1, 0);
	lock_ObtainWrite(&transKALock);
	if (code)
	    (void) vols_AbortTrans(tconn, transId);
	else
	    code = vols_DeleteTrans(tconn, transId);
	transId = -1;
	lock_ReleaseWrite(&transKALock);
    }
    /* Release our promise about changes. */
    (void) fts_ReleaseToken(tokenDesc);

    if (filename != NULL) {
	code3 = (error_status_t)close(fd);
	if (code3 != 0)
	    code3 = errno;
    }
    code4 = vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
    
    if (dataPipeP == (pipe_t *)NULL)
	free(pipeState.data);

    if (code)
	return code;
    if (code2)
	return code2;
    if (code3)
	return code3;
    return code4;
}

/*
 * Restore  volume <volIdp> <volNamep> on [toservAddrp, toaggrId] from
 * the dump file specified in <filename>.
 */
long VC_RestoreVolume(tconn, toservAddrp, aggrId, volIdp, volNamep, override,
		      restoreFlags, filename, dataPipeP, sockFlag)
    volatile rpc_binding_handle_t tconn;
    struct sockaddr * volatile toservAddrp;
    volatile unsigned long aggrId;
    afs_hyper_t * volatile volIdp;
    char * volatile volNamep;
    int override;			/* not used */
    unsigned long restoreFlags;
    char * volatile filename;		/* source of input for restoration */
    pipe_t * volatile dataPipeP;	/* alternate source of input */
    unsigned sockFlag;			/* protocol domain for socket */
{
    long newIdFlag;
    struct ftserver_status status;
    volatile int fd, tokenDesc, siteIx, bksiteIx;
    volatile long transId;
    struct stat fstatus;
    volatile error_status_t code;
    error_status_t code2;
    volatile ftsPipeState pipeState;
    volatile pipe_t filePipe;
    struct ftserver_status ftstat;
    volatile struct vldbentry entry;
    char *excNameP;
    char excBuff[50];
    rpc_binding_handle_t connp;
    volatile int xl[MAXTYPES];
    volatile unsigned long rwSiteFlag;
    volatile unsigned long srepSiteFlag;
    volatile struct sockaddr fromservAddr;
    volatile unsigned long fromaggrId;
    volatile afs_hyper_t backupId;
    volatile char updateFldb = (char)0;
    extern char * ExceptionToString();

#if defined(OSF_NO_SOCKET_DUMP)
    assert(sockFlag == 0);
#endif
    checkKALockInit();
    checkInitConns();
    /*
     * First, lock vldb entry for the duration of the call
     */
    if (code = vldb_SetLock(volIdp, -1, VLOP_RESTORE))
	return code;

    newIdFlag = 0;
    connp = (rpc_binding_handle_t)NULL;
    if (code = vldb_GetVolumeId(volIdp, volIdp, volNamep, &newIdFlag,
				toservAddrp)) {
      vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
      return code;
    }

    /*
     * We have two sources for input. One is a pipe that is setup by the caller
     * of this RPC. The other is the name of a file from which to read data.
     * We first check for a non-NULL pipe, and if present, we go ahead and use
     * it, ignoring the filename. If the pipe is NULL, then we use the filename
     * string. If the filename is NULL too, we use stdin.
     */

    if (dataPipeP != (pipe_t *)NULL)
      filePipe = *dataPipeP;
    else {
      if (filename != (char *)NULL) {
	/* open dump file for read*/
	fd = open((char *)filename, 0, 0);
	if ( (fd < 0) || (fstat(fd, &fstatus) == -1) ) {
	  dce_svc_printf(FTS_S_VPROCS_CANT_ACCESS_FILE_MSG,
			 filename, errno, strerror(errno));
	  vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	  return errno;
	}
      } else {
	fd = fileno(stdin);
	if (fd < 0) {
	  dce_svc_printf(FTS_S_VPROCS_NO_STANDARD_INPUT_MSG);
	  vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	  return FTS_ERR_IO;
	}
      }
      /* set up pipe parameters */
      if ((pipeState.data = (char *)malloc(sizeof(int))) == (char *)NULL) {
	vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	return (FTS_ERR_NOMEM);
      }
      *(pipeState.data) = (int)fd;
      pipeState.error = 0;
      filePipe.alloc = fts_DumpAlloc;
      filePipe.state = (rpc_ss_pipe_state_t)&pipeState;
      filePipe.pull = fts_RestoreRead;
      filePipe.push = 0;
    }

    if (code = vldb_GetEntryByID(volIdp, -1, &entry, 0)) {
      if (filename != (char *)NULL) close(fd);
      if (dataPipeP == NULL) free(pipeState.data);
      vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
      return code;
    }
    rwSiteFlag = 0;
    srepSiteFlag = 0;
    fts_GetIndices(&entry, xl);	/* refresh this */

    /*
     * We cannot safely restore a R/W volume if there are replicas. Fileset
     * replication uses incremental dumps, and require that the R/W
     * fileset's version number increases monotonically. Restoring a R/W
     * fileset usually means setting the fileset version to a lower number
     * than the version at the replica sites. For now, refuse to restore
     * if the volume is known to be replicated.
     */
    if ((entry.flags & VLF_ROEXISTS) &&
	(xl[ROVOL] >= 0) &&
	(vldb_FindRepSite(&entry, 0, -1,
			  ((unsigned32)VLSF_ZEROIXHERE) >> xl[RWVOL]) >= 0)) {
      dce_svc_printf(FTS_S_VPROCS_FILESET_REPLICATED_MSG,
		     volNamep,AFS_HGETBOTH(*volIdp));
      if (filename != (char *)NULL) close(fd);
      if (dataPipeP == NULL) free(pipeState.data);
      vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
      return FTS_ERR_FSET_REPLICATED;
    }
    
    /* 
     * If we are overwriting a fileset, we should make sure that the FLDB
     * entry description is consistent with the location we are to restore the
     * fileset. If the FLDB entry differs from our restoration site, and if
     * the fileset *does* exist in the location pointed to by the FLDB, we fail
     * to do the restore.
     */
    if (xl[RWVOL] >= 0) {
      /* where's the R/W instance, according to FLDB? */
      rwSiteFlag = ((unsigned32)VLSF_ZEROIXHERE) >> xl[RWVOL];
      siteIx = vldb_FindRepSite(&entry, 0, -1, rwSiteFlag);
      if (siteIx >= 0) {
	if (!vldb_IsSiteHere(&entry, siteIx, toservAddrp, aggrId)) {
	  /* a change required for the FLDB entry */
	  updateFldb = (char)1;
	  connp = connToServer((struct sockaddr *)&entry.siteAddr[siteIx], 
			      (u_char *)(&entry.sitePrincipal[siteIx].text[0]),
			       SERVERKIND_FT);
	  if (connp == (rpc_binding_handle_t)NULL) {
	    if (filename != (char *)NULL) close(fd);
	    if (dataPipeP == NULL) free(pipeState.data);
	    vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	    return FTS_ERR_BADCONN;
	  }
	  code = vols_GetOneVolStatus(connp, (afs_hyper_t *)volIdp, 
				      entry.sitePartition[siteIx], 
				      &ftstat, 1);
	  if (code == 0) {
	    if (filename != (char *)NULL) close(fd);
	    if (dataPipeP == NULL) free(pipeState.data);
	    vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	    dce_svc_printf(FTS_S_VPROCS_FILESET_EXISTS_MSG,
			   volNamep, AFS_HGETBOTH(*volIdp),
			   getAggrName(&entry.siteAddr[siteIx], 
				       entry.sitePartition[siteIx], 0), 
			   MapSockAddr(&entry.siteAddr[siteIx]));
	    return FTS_ERR_FSET_EXISTS;
	  }
	  else if (code==ENOENT||code==ENODEV||code==FTSERVER_ERR_NOVOL||
		   code==FTU_E_NO_SUCH_FSET ) {
	    /* no fileset, go ahead with the restore */
	    ;
	  }
	  else {
	    if (filename != (char *)NULL) close(fd);
	    if (dataPipeP == NULL) free(pipeState.data);
	    vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	    return code;
	  }
	}
	/* else we would just overwrite the fileset */
      }
      else {
	if (filename != (char *)NULL) close(fd);
	vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	return FTS_ERR_NEED_RW;
      }
    }
    else {
      if (filename != (char *)NULL) close(fd);
      if (dataPipeP == NULL) free(pipeState.data);
      vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
      return FTS_ERR_NEED_RW;
    }

    code = vols_GetOneVolStatus(tconn, (afs_hyper_t *)volIdp,
				(unsigned long)aggrId, &ftstat, 1);
    if ( code == ENOENT || code == ENODEV || code == FTSERVER_ERR_NOVOL ||
	 code == FTU_E_NO_SUCH_FSET ) {
      bzero((char *)&ftstat, sizeof(ftstat));
      if (xl[RWVOL] < 0) {
	if (filename != (char *)NULL) close(fd);
	vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	return VL_BADID;
      }
      else {
	*volIdp = entry.VolIDs[xl[RWVOL]];
	if (xl[ROVOL] >= 0)
	    ftstat.vsd.cloneId = entry.VolIDs[xl[ROVOL]];
	if (xl[BACKVOL] >= 0)
	    ftstat.vsd.backupId = entry.VolIDs[xl[BACKVOL]];
      }
      /* fileset does not exist, create it using the FLDB entry info */
      dce_svc_printf(FTS_S_VPROCS_FILESET_NOT_EXISTS_MSG,
		     AFS_HGETBOTH(*volIdp),
		     getAggrName(toservAddrp, aggrId, 0),
		     MapSockAddr(toservAddrp));
      if (code = vols_CreateVolume(tconn, (unsigned long)aggrId,
				   (char *)volNamep,
				   VOLTIX_TO_VOLTYPE(RWVOL),
				   FLAGS_ENCODE((FTSERVER_OP_SETSTATUS
						 | FTSERVER_OP_GETSTATUS
						 | FTSERVER_OP_DELETE),
						VOLERR_TRANS_RESTORE),
				   &osi_hZero, volIdp,
				   (long *) &transId)) {
	if (filename != (char *)NULL) close(fd);
	if (dataPipeP == NULL) free(pipeState.data);
	vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	return code;
      }
      /* Bring new volume on-line. */
      ftstat.vss.states = VOL_TYPE_RW | VOL_RW | VOL_REP_NONE;
      if (code = vols_SetStatus(tconn, transId,
				VOL_STAT_STATES
				| VOL_STAT_BACKUPID | VOL_STAT_CLONEID
				| VOL_STAT_PARENTID
				| VOL_STAT_LLBACKID | VOL_STAT_LLFWDID,
				&ftstat)) {
	(void) vols_DeleteVolume(tconn, transId);
	(void) vols_AbortTrans(tconn, transId);
	if (dataPipeP == NULL) free(pipeState.data);
	if (filename != (char *)NULL) close(fd);
	vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	return code;
      }
      VOLSERLOG(1, "Fileset %s (%lu,,%lu) created and brought online\n",
		volNamep, AFS_HGETBOTH(*volIdp));

      /* Have to close the fileset, get the necessary tokens and then reopen
       * the fileset. This procedure is required because getting the tokens
       * with the fileset open causes failure in OSF/1 but not on AIX.
       */
      if (code = vols_DeleteTrans(tconn, transId)) {
	(void) vols_DeleteVolume(tconn, transId);
	(void) vols_AbortTrans(tconn, transId);
	if (dataPipeP == NULL) free(pipeState.data);
	if (filename != (char *)NULL) close(fd);
	vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	return code;
      }
    }
    else if (code) {
      if (filename != (char *)NULL) close(fd);
      if (dataPipeP == NULL) free(pipeState.data);
      vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
      return code;
    }

    /* Prevent concurrent access to this fileset */
    if ( code = fts_GetToken((afsNetAddr *)toservAddrp,
			     NULL, (afs_hyper_t *)volIdp,
			     TKN_UPDATE
			     /* | TKN_OPEN_WRITE | TKN_OPEN_EXCLUSIVE |
				TKN_OPEN_PRESERVE */,
			     AFS_FLAG_FORCEVOLQUIESCE, (int *)&tokenDesc) ) {
      if (filename != (char *)NULL) close(fd);
      if (dataPipeP == NULL) free(pipeState.data);
      vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
      return code;
    }
    /*
     * Do the actual volume restoring from filename
     */
    code = vols_CreateTrans(tconn, (afs_hyper_t *)volIdp,
			    (unsigned long)aggrId,
			    FLAGS_ENCODE(FTSERVER_OP_RESTORE
					 | FTSERVER_OP_GETSTATUS
					 | FTSERVER_OP_SETSTATUS,
					 VOLERR_TRANS_RESTORE),
			    (long *)&transId, 0);
    if (code) {
      if (filename != (char *)NULL) close(fd);
      if (dataPipeP == NULL) free(pipeState.data);
      (void) fts_ReleaseToken(tokenDesc);
      vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
      return code;
    }

    TRY {
	/* Start up a periodic keep-alive function too. */
	code = SetMoveTags(tconn, (long *)&transId,
			   (struct sockaddr *)toservAddrp,
			   (afs_hyper_t *)volIdp,
			   (unsigned long)aggrId, MOVETAG_FETCH);
	if (code == 0)
	    code = vols_Restore((rpc_binding_handle_t)tconn, transId, 
			restoreFlags, (pipe_t *)&filePipe, sockFlag);
	if (code == 0 || code == FTU_E_RESTORED_INCONSISTENT_FSET) {
	    /* get a fresh copy of the states bits */
	    code2 = vols_GetStatus(tconn, transId, &ftstat);
	    if (code2 == 0 &&
		(ftstat.vss.states & (VOL_OUTOFSERVICE | VOL_OFFLINE)) != 0) {
		ftstat.vss.states &= ~(VOL_OUTOFSERVICE | VOL_OFFLINE);
		code2 = vols_SetStatus(tconn, transId, VOL_STAT_STATES,
				      &ftstat);
	    }
	    if (code2 != 0) code = code2;
	}
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	    sprintf(excBuff, "[[unk: %#lx]]", THIS_CATCH);
	    excNameP = excBuff;
	}
	dce_svc_printf(FTS_S_VPROCS_FTSERVER_WRITE_EXCEPTION_MSG, excNameP);
	code = FTS_ERR_PIPE;
    } ENDTRY

    pipeState = *((ftsPipeState *) filePipe.state);
    if (pipeState.error) {
	dce_svc_printf(FTS_S_VPROCS_FILESET_RESTORE_ERROR_MSG,
		       pipeState.error, strerror(pipeState.error));
	if (!code)
	    code = pipeState.error;
    }
    code2 = code;	/* some errors don't need an abort */
    if (code == FTSERVER_ERR_DISJOINT_DATE
	|| code == FTSERVER_ERR_DISJOINT_VV
	|| code == FTSERVER_ERR_DISJOINT2_DATE
	|| code == FTSERVER_ERR_DISJOINT2_VV)
      code2 = 0;
    (void) ClearMoveTags((struct sockaddr *)toservAddrp,
		(afs_hyper_t *)volIdp, (unsigned long)aggrId, 1, 0);
    lock_ObtainWrite(&transKALock);
    if (code2) {
	code2 = vols_AbortTrans(tconn, transId);
	if (code == FTU_E_RESTORED_INCONSISTENT_FSET) {
	    code = code2;
	    dce_svc_printf(FTS_S_VPROCS_FILESET_INCONSISTENT_MSG);
	}
    } else {
	code2 = vols_DeleteTrans(tconn, transId);
	if (code == 0) code = code2;
    }
    transId = -1;
    lock_ReleaseWrite(&transKALock);
    /* update the FLDB entry for this fileset, if necessary */
    if (updateFldb != (char)0 && code == 0 && code2 == 0) {
      /* save backup fileset's parameters for deletion purposes */
      vldb_SaveOldEntry(&entry);
      bksiteIx = -1;
      if (xl[BACKVOL] >= 0) {
	bksiteIx = vldb_FindRepSite(&entry, 0, -1, 
				 ((unsigned32)VLSF_ZEROIXHERE) >> xl[BACKVOL]);
	if (bksiteIx >= 0) {
	  fromservAddr = *((struct sockaddr *) &entry.siteAddr[bksiteIx]);
	  fromaggrId = entry.sitePartition[bksiteIx];
	  backupId = entry.VolIDs[xl[BACKVOL]];
	  connp = connToServer((struct sockaddr *)&entry.siteAddr[bksiteIx], 
			       (u_char *)&entry.sitePrincipal[bksiteIx].text[0],
			       SERVERKIND_FT);
	}
      }
      VOLSERLOG(1,"Updating the FLDB entry for %s(%lu,,%lu) to be resident in \
aggregate %s of server %s...",
		volNamep, AFS_HGETBOTH(*volIdp),
		getAggrName(toservAddrp, aggrId, 0), 
		MapSockAddr(toservAddrp));
      /* Make sure that ReplaceRepSite finds the R/W instance to move */
      if (!vldb_ReplaceRepSite(&entry, &entry.siteAddr[siteIx],
			  entry.sitePartition[siteIx], 
			  toservAddrp, aggrId, rwSiteFlag))
	VOLSERLOG(1, "failed; fldb entry for %s not found\n", volNamep);
      else {
	if (code = vldb_ReplaceEntry(&entry, volIdp, RWVOL, LOCKREL_ALL))
	  VOLSERLOG(1, "failed; %s\n", dfs_dceErrTxt(code));
	else
	  VOLSERLOG(1,"done\n");
      }
      /* 
       * Delete the RW's backup volume on the original site (if one exists) 
       */
      if (bksiteIx >= 0) {
	VOLSERLOG(1, 
	   "Deleting backup of fileset %s(%lu,,%lu) in aggr %s of server %s\n",
		  volNamep, AFS_HGETBOTH(backupId), 
		  getAggrName(&fromservAddr, fromaggrId, 0), 
		  MapSockAddr(&fromservAddr));
	if (connp != (rpc_binding_handle_t)NULL)
	    (void)fts_DeleteVolume(connp, (unsigned long)fromaggrId,
				(afs_hyper_t *)&backupId, 0, 0);
	(void)vldb_RemoveVolume(&entry, (struct sockaddr *)&fromservAddr, 
				(unsigned long)fromaggrId, 
				BACKVOL, (afs_hyper_t *)&backupId);
      }
      vldb_TellRepAboutNewEntry(&entry, 0, 1);
    }

    /* Allow accesses again. */
    (void) fts_ReleaseToken(tokenDesc);
    vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);

    code2 = 0;
    if (filename != NULL) {
	errno = 0;
	code2 = (error_status_t)close(fd);
	if (code2 != 0)
	    code2 = errno;
    }
    if (code == 0) code = code2;
    if (dataPipeP == NULL) free(pipeState.data);

    return code;
}


struct moveClone {
    afs_hyper_t id;
    afs_hyper_t tempId;
    unsigned long type;
};

/*
 * Move volume, volIdp from [fromservAddrp, fromaggrId] to [toservAddrp,
 * toaggrId].
 */
long VC_MoveVolume(volIdp, fromservAddrp, fromaggrId, toservAddrp,
		   toaggrId, sockFlag)
    afs_hyper_t *volIdp;
    struct sockaddr *fromservAddrp;
    unsigned long fromaggrId;
    struct sockaddr *toservAddrp;
    unsigned long toaggrId;
    int sockFlag;
{
    rpc_binding_handle_t toconn = 0, fromconn = 0;
    afs_hyper_t backupId, dumpCloneId, targetInitialVolId;
    char volName[FTSERVER_MAXFSNAME + 30];
    char targetInitialVolName[FTSERVER_MAXFSNAME + 30];
    register long code, tcode;
    struct vldbentry entry;
    int xl[MAXTYPES];
    int nukeFClone, nukeFrom, nukeTo;
    struct siteDesc fromSite, toSite;
    bulkIDs newIDs;
    int wasBackup, sameServer, entryLocked, siteIx;
    unsigned long preservedStates, rwSiteFlag;
    struct ftserver_status ftstatus;
    int readTokDesc, thereTokDesc;
    long fromTrans, toTrans;
    int fromFlIx, toFlIx;
    int isInconsistent = 0;
    afs_hyper_t srepId;
    afs_hyper_t srepTmpId;
    unsigned long srepSiteFlag;
    int fromSrepFlIx;
    unsigned long fromSrepAggrId;
    int toSrepFlIx;
    long srepTrans;
    int numNewIDs;
    char srepTmpName[FTSERVER_MAXFSNAME + 30];
    u_long srepRepLat;

#if defined(OSF_NO_SOCKET_DUMP)
    assert(sockFlag == 0);
#endif
    checkInitConns();
    fromTrans = toTrans = -1;
    readTokDesc = thereTokDesc = -1;
    nukeFClone = nukeFrom = nukeTo = -1;
    entryLocked = 0;
    checkKALockInit();
    sameServer = BOMB_EXEC("movevolume#12", (bcmp((char *)fromservAddrp,
						(char *)toservAddrp,
						sizeof(struct sockaddr)) == 0));
    if (sameServer && (fromaggrId == toaggrId))
	return 0;
    code = BOMB_EXEC("movevolume#1", vldb_GetSiteInfo(fromservAddrp,&fromSite));
    if (code != 0)
	return code;
    if (sameServer) {
	toSite = fromSite;	/* structure assignment */
    } else {
	code = BOMB_EXEC("movevolume#2", vldb_GetSiteInfo(toservAddrp,&toSite));
	if (code != 0)
	    return code;
	/* Compare canonicalized versions of the sites */
	if (bcmp((char *)&fromSite.Addr[0], (char *)&toSite.Addr[0],
		 sizeof(fromSite.Addr)) == 0
	    && bcmp((char *)&fromSite.KerbPrin[0], (char *)&toSite.KerbPrin[0],
		    sizeof(fromSite.KerbPrin)) == 0
	    && bcmp((char *)&fromSite.Owner, (char *)&toSite.Owner,
		    sizeof(fromSite.Owner)) == 0
	    && bcmp((char *)&fromSite.ObjID, (char *)&toSite.ObjID,
		    sizeof(fromSite.ObjID)) == 0) {
	    sameServer = BOMB_EXEC("movevolume#13", 1);	/* it's the same site */
	    if (fromaggrId == toaggrId)
		return 0;
	}
    }
    /*
     * First, lock vldb entry for the duration of the call
     */
    if (code = BOMB_EXEC("movevolume#3", vldb_SetLock(volIdp, RWVOL,VLOP_MOVE)))
	return code;
    entryLocked = 1;
    rwSiteFlag = 0;
    /*
     * Now get hold of the vldb entry for the volume to move
     */
    if (code = BOMB_EXEC("movevolume#4", vldb_GetEntryByID(volIdp, RWVOL,
							   &entry, 0)))
	goto move;
    vldb_SaveOldEntry(&entry);
    fts_GetIndices(&entry, xl);
    AFS_hzero(backupId);
    if (xl[BACKVOL] >= 0) backupId = entry.VolIDs[xl[BACKVOL]];

    BOMB_IF("movevolume#5") {
	xl[RWVOL] = -1;
    }
    if (xl[RWVOL] < 0) {
	dce_svc_printf(FTS_S_VPROCS_MISSING_RW_FILESET_ID_MSG,
		       AFS_HGETBOTH(*volIdp));
	vldb_PrintRepSites(&entry, -1, 0, 0);
	code = FTS_ERR_NEED_RW;
	goto move;
    }
    rwSiteFlag = ((unsigned32)VLSF_ZEROIXHERE) >> xl[RWVOL];
    targetInitialVolId = *volIdp;

    fromconn = connToServer(fromservAddrp, fromSite.KerbPrin, SERVERKIND_FT);
    BOMB_IF("movevolume#6") {fromconn = NULL;}
    if (fromconn == (rpc_binding_handle_t)NULL) {
	(void) BOMB_EXEC("movevolume#60", vldb_ReleaseLock(volIdp, -1,
							   LOCKREL_ALL));
	code = FTS_ERR_BADCONN;
	goto move;
    }
    toconn = connToServer(toservAddrp, toSite.KerbPrin, SERVERKIND_FT);
    BOMB_IF("movevolume#7") {toconn = NULL;}
    if (toconn == (rpc_binding_handle_t)NULL) {
	(void) BOMB_EXEC("movevolume#61", vldb_ReleaseLock(volIdp, -1,
							   LOCKREL_ALL));
	code = FTS_ERR_BADCONN;
	goto move;
    }
    wasBackup = BOMB_EXEC("movevolume#10", (entry.flags & VLF_BACKEXISTS));
    /*
     * This applies the check-for-local-mount restriction ONLY if it's a
     * cross-server move.
     */
    if (!sameServer) {
	if (CheckLocalMount(fromconn, fromservAddrp, volIdp, fromaggrId,
			    "moved") > 0) {
	    code = FTSERVER_ERR_LCLMOUNT;
	    goto move;
	}
	if (wasBackup) {
	    if (CheckLocalMount(fromconn, fromservAddrp, &backupId, fromaggrId,
				"moved") > 0) {
		code = FTSERVER_ERR_LCLMOUNT;
		goto move;
	    }
	}
    }

    /* Check whether this is a resumption. */
    /* See which site the FLDB entry points to. */
    fromFlIx = BOMB_EXEC("movevolume#14",
			 vldb_FindRepSite(&entry, fromservAddrp, fromaggrId,
					  rwSiteFlag));
    toFlIx = BOMB_EXEC("movevolume#15",
		       vldb_FindRepSite(&entry, toservAddrp, toaggrId,
					rwSiteFlag));
    if (fromFlIx < 0 && toFlIx < 0) {
	/* The FLDB doesn't point to either location. */
	dce_svc_printf(FTS_S_VPROCS_NOT_ON_SITE_MSG,
		       AFS_HGETBOTH(*volIdp));
	vldb_PrintRepSites(&entry, rwSiteFlag, 0, 0);
	code = FTS_ERR_MOVED;
	goto move;
    }
    if (fromFlIx >= 0 && toFlIx >= 0) {
	/* Inconsistent FLDB entry */
	dce_svc_printf(FTS_S_VPROCS_INCONSISTENT_FLDB_MSG,
		       AFS_HGETBOTH(*volIdp));
	vldb_PrintRepSites(&entry, rwSiteFlag, 0, 0);
	code = FTS_ERR_MOVED;
	goto move;
    }
    /*
     * Discover replication-related info.  We're mostly interested in filesets
     * that are release replicated, because we want to move the staging
     * replica along with the primary.
     */
    AFS_hzero(srepId);
    srepSiteFlag = 0;
    fromSrepFlIx = -1;
    toSrepFlIx = -1;
    srepTrans = -1;
    if (entry.flags & VLF_LAZYREP) {
	if (entry.flags & VLF_LAZYRELE) {
	    /* Fileset is release-replicated */
	    preservedStates = VOL_REP_RELEASE;
	    if (entry.flags & VLF_ROEXISTS) {
		if (xl[ROVOL] < 0) {
		    VOLSERLOG(0, "The FLDB entry for %lu,,%lu is missing its \
RO fileset ID\n",
			      AFS_HGETBOTH(*volIdp));
		    code = FTS_ERR_NEED_RO;
		    goto move;
		}
		srepId = entry.VolIDs[xl[ROVOL]];
		srepSiteFlag = ((unsigned32)VLSF_ZEROIXHERE) >> xl[ROVOL];
		if (sameServer) {
		    fromSrepFlIx = vldb_FindRepSite(&entry,
						    fromservAddrp, fromaggrId,
						    srepSiteFlag);
		    fromSrepAggrId = fromaggrId;
		} else {
		    fromSrepFlIx = vldb_FindRepSite(&entry,
						    fromservAddrp, -1,
						    srepSiteFlag);
		}
		if (fromSrepFlIx != -1) {
		    fromSrepAggrId = entry.sitePartition[fromSrepFlIx];
		    srepRepLat = entry.sitemaxReplicaLatency[fromSrepFlIx];
		}
		if (!sameServer) {
		    toSrepFlIx = vldb_FindRepSite(&entry,
						  toservAddrp, -1,
						  srepSiteFlag);
		}
	    }
	} else {
	    preservedStates = VOL_REP_LAZY;
	}
    } else {
	preservedStates = 0;
    }

    if (toSrepFlIx >= 0) {
	VOLSERLOG(0, "You must remove the replication site at the destination \
before moving\n");
	code = FTS_ERR_BADOP;
	goto move;
    }

    if (fromFlIx >= 0) {
	/* The FLDB points to the source site. */
	if (sameServer) {
	    code = FLAGS_ENCODE(FTSERVER_OP_GETSTATUS, VOLERR_TRANS_MOVE);
	} else {
	    code = FLAGS_ENCODE(FTSERVER_OP_GETSTATUS | FTSERVER_OP_DELETE,
				VOLERR_TRANS_MOVE);
	}
	code = BOMB_EXEC("movevolume#81",
			 vols_CreateTrans(toconn, volIdp, toaggrId, code,
					  &toTrans, 1));
	if (code) toTrans = -1;
	if (code && code != ENOENT && code != ENODEV
	    && code != FTSERVER_ERR_NOVOL
	    && (!sameServer || code != EXDEV)) {
	    dce_svc_printf(FTS_S_VPROCS_ON_EXISTENCE_CHECK_MSG,
			   AFS_HGETBOTH(*volIdp),
			   MapSockAddr(toservAddrp),
			   getAggrName(toservAddrp, toaggrId, 0),
			   dfs_dceErrTxt(code));
	    goto move;
	}
	if (code == 0) {
	    code = BOMB_EXEC("movevolume#82",
			     vols_GetStatus(toconn, toTrans, &ftstatus));
	    if (code) {
		dce_svc_printf(FTS_S_VPROCS_CANT_GET_STATUS_MSG,
			       AFS_HGETBOTH(*volIdp),
			       MapSockAddr(toservAddrp),
			       getAggrName(toservAddrp, toaggrId, 0),
			       dfs_dceErrTxt(code));
		goto move;
	    }
	    if (ftstatus.vss.states & VOL_MOVE_SOURCE) {
		dce_svc_printf(FTS_S_VPROCS_DESTINATION_IS_SOURCE_MSG);
		code = FTS_ERR_BADOP;
		goto move;
	    }
	    if (sameServer) {
		/* The target is the good one!  It's been moved already. */
		if (ftstatus.vss.states & (VOL_ZAPME | VOL_CLONEINPROG |
					   VOL_OFFLINE | VOL_OUTOFSERVICE |
					   VOL_DEADMEAT)) {
		    dce_svc_printf(FTS_S_VPROCS_BAD_TARGET_STATUS_MSG,
				   AFS_HGETBOTH(*volIdp),
				   MapSockAddr(toservAddrp),
				   getAggrName(toservAddrp, toaggrId, 0),
				   ftstatus.vss.states);
		    code = FTS_ERR_BADOP;
		    goto move;
		}
		code = BOMB_EXEC("movevolume#84",
				 vols_DeleteTrans(toconn, toTrans));
		toTrans = -1;
		if (code != 0) {
		    dce_svc_printf(FTS_S_VPROCS_BAD_TARGET_CLOSE_MSG,
				   AFS_HGETBOTH(*volIdp),
				   MapSockAddr(toservAddrp),
				   getAggrName(toservAddrp, toaggrId, 0),
				   dfs_dceErrTxt(code));
		    goto move;
		}
		/* 
		 * Update VLDB so that volume's location is set to its new
		 * home in destination site. Also reflect the fact that the
		 * backup volume (if any) is gone too.
		 */
		BOMB_POINT("movevolume#130");
		if (wasBackup)
		    vldb_DeleteVolume(&entry, fromservAddrp, fromaggrId,
				      BACKVOL, &backupId, NULL);
		if (fromSrepFlIx != -1)
		    vldb_DeleteVolume(&entry, fromservAddrp, fromSrepAggrId,
				      ROVOL, &srepId, NULL);
		vldb_DeleteVolume(&entry, fromservAddrp, fromaggrId, RWVOL,
				  volIdp, NULL);
		vldb_EnsureSite(&entry, VLF_RWEXISTS, rwSiteFlag, toservAddrp,
				toaggrId, -1, 0);
		if (fromSrepFlIx != -1)
		    vldb_EnsureSite(&entry, VLF_ROEXISTS, srepSiteFlag,
				    toservAddrp, toaggrId, -1, srepRepLat);
		code = BOMB_EXEC("movevolume#131",
				 vldb_ReplaceEntry(&entry, volIdp, -1,
						   LOCKREL_ALL));
		if (code)
		    goto move;
		nukeFrom = 2;
		entryLocked = 0;
		vldb_TellRepAboutNewEntry(&entry, 0, 1);
		toFlIx = fromFlIx;
		fromFlIx = -1;
		code = BOMB_EXEC("movevolume#133",
				 FinishMoveTags(&entry, toconn, toservAddrp,
						toaggrId, volIdp));
		goto move;
	    } else {
		struct ftserver_status tempStatus;

		code = vols_GetOneVolStatus(fromconn, volIdp, fromaggrId,
					    &tempStatus, 1);
		if (code != 0) {
		    VOLSERLOG(0, "Cannot get status of source fileset %lu,,%lu \
on %s/%s: %s\n",
			      AFS_HGETBOTH(*volIdp),
			      MapSockAddr(fromservAddrp),
			      getAggrName(fromservAddrp, fromaggrId, 0),
			      dfs_dceErrTxt(code));
		    goto move;
		}

		if (tempStatus.vss.states & (VOL_DELONSALVAGE | VOL_DEADMEAT |
					     VOL_ZAPME)) {
		    VOLSERLOG(0, "The source fileset (%lu,,%lu) is in an \
inconsistent state on %s/%s\n",
			      AFS_HGETBOTH(*volIdp),
			      MapSockAddr(fromservAddrp),
			      getAggrName(fromservAddrp, fromaggrId, 0));
		    code = FTS_ERR_INCONSISTENT;
		    goto move;
		}

		code = BOMB_EXEC("movevolume#83",
				 vols_DeleteVolume(toconn, toTrans));
		if (code != 0) {
		    dce_svc_printf(FTS_S_VPROCS_BAD_TARGET_DELETE_MSG,
				   AFS_HGETBOTH(*volIdp),
				   MapSockAddr(toservAddrp),
				   getAggrName(toservAddrp, toaggrId, 0),
				   dfs_dceErrTxt(code));
		    goto move;
		}
		code = BOMB_EXEC("movevolume#84",
				 vols_DeleteTrans(toconn, toTrans));
		toTrans = -1;
		if (code != 0) {
		    dce_svc_printf(FTS_S_VPROCS_BAD_OLD_TARGET_DELETE_MSG,
				   AFS_HGETBOTH(*volIdp),
				   MapSockAddr(toservAddrp),
				   getAggrName(toservAddrp, toaggrId, 0),
				   dfs_dceErrTxt(code));
		    goto move;
		}
	    }
	}

	if (fromSrepFlIx != -1) {
	    if (sameServer)
		code = FLAGS_ENCODE(FTSERVER_OP_GETSTATUS, VOLERR_TRANS_MOVE);
	    else
		code = FLAGS_ENCODE(FTSERVER_OP_GETSTATUS | FTSERVER_OP_DELETE,
				    VOLERR_TRANS_MOVE);
	    code = vols_CreateTrans(toconn, &srepId, toaggrId, code,
				    &srepTrans, 1);
	    if (code) srepTrans = -1;
	    if (code && code != ENOENT && code != ENODEV
		&& code != FTSERVER_ERR_NOVOL
		&& (!sameServer || code != EXDEV)) {
		VOLSERLOG(0, "Cannot check for an existing target staging \
fileset %lu,,%lu on %s/%s: %s\n",
			  AFS_HGETBOTH(srepId),
			  MapSockAddr(toservAddrp),
			  getAggrName(toservAddrp, toaggrId, 0),
			  dfs_dceErrTxt(code));
		goto move;
	    }
	    if (code == 0) {
		code = vols_GetStatus(toconn, srepTrans, &ftstatus);
		if (code) {
		    VOLSERLOG(0, "Cannot get status of existing target staging \
fileset %lu,,%lu on %s/%s: %s\n",
			      AFS_HGETBOTH(srepId),
			      MapSockAddr(toservAddrp),
			      getAggrName(toservAddrp, toaggrId, 0),
			      dfs_dceErrTxt(code));
		    goto move;
		}
		if (sameServer) {
		    /* The target is the good one!  It's been moved already. */
		    if (ftstatus.vss.states & (VOL_ZAPME | VOL_CLONEINPROG |
					       VOL_OFFLINE | VOL_OUTOFSERVICE |
					       VOL_DEADMEAT |
					       VOL_REPSERVER_MGD)) {
			VOLSERLOG(0, "Bad status on the existing target \
staging fileset %lu,,%lu on %s/%s: %#lx\n",
				  AFS_HGETBOTH(srepId),
				  MapSockAddr(toservAddrp),
				  getAggrName(toservAddrp, toaggrId, 0),
				  ftstatus.vss.states);
			code = FTS_ERR_BADOP;
			goto move;
		    }
		    VOLSERLOG(1,
			      "Found good staging replica on destination\n");
		    code = vols_DeleteTrans(toconn, srepTrans);
		    srepTrans = -1;
		    if (code != 0) {
			VOLSERLOG(0, "Error closing existing target staging \
fileset %lu,,%lu on %s/%s: %s\n",
				  AFS_HGETBOTH(srepId),
				  MapSockAddr(toservAddrp),
				  getAggrName(toservAddrp, toaggrId, 0),
				  dfs_dceErrTxt(code));
			goto move;
		    }

		    if (AFS_hiszero(ftstatus.vsd.llFwdId)) {
			VOLSERLOG(0, "Target staging replica is not linked to \
a R/W fileset: %lu,,%lu on %s/%s\n",
				  AFS_HGETBOTH(srepId),
				  MapSockAddr(toservAddrp),
				  getAggrName(toservAddrp, toaggrId, 0));
			code = FTS_ERR_BADOP;
			goto move;
		    }
		    
		    code = FLAGS_ENCODE(FTSERVER_OP_GETSTATUS
					| FTSERVER_OP_SETSTATUS
					| FTSERVER_OP_SWAPIDS
					| FTSERVER_OP_DELETE, 
					VOLERR_TRANS_MOVE);
		    code = vols_CreateTrans(fromconn, volIdp, fromaggrId, code,
					    &fromTrans, 0);
		    if (code) {
			fromTrans = -1;
			VOLSERLOG(0, "Failed to open source fileset %lu,,%lu \
on %s/%s: %s\n",
				  AFS_HGETBOTH(*volIdp),
				  MapSockAddr(toservAddrp),
				  getAggrName(toservAddrp, toaggrId, 0),
				  dfs_dceErrTxt(code));
			goto move;
		    }

		    VOLSERLOG(1, "Finishing move by swapping RW into place\n");
		    /* not sure where swap will leave us */
		    nukeFrom = nukeTo = 0;
		    code = SwapIntoPlace(fromconn, fromTrans, volIdp,
					 &ftstatus.vsd.llFwdId,
					 fromaggrId, toaggrId, 0, NULL, NULL,
					 fromservAddrp);
		    if (code)
			(void)vols_AbortTrans(fromconn, fromTrans);
		    else
			code = vols_DeleteTrans(fromconn, fromTrans);
		    fromTrans = -1;
		    if (code)
			goto move;
		    
		    /* 
		     * Update VLDB so that volume's location is set to its new
		     * home in destination site. Also reflect the fact that the
		     * backup volume (if any) is gone too.
		     */
		    if (wasBackup)
			vldb_DeleteVolume(&entry, fromservAddrp, fromaggrId,
					  BACKVOL, &backupId, NULL);
		    if (fromSrepFlIx != -1)
			vldb_DeleteVolume(&entry, fromservAddrp,
					  fromSrepAggrId, ROVOL, &srepId,
					  NULL);
		    vldb_DeleteVolume(&entry, fromservAddrp, fromaggrId, RWVOL,
				      volIdp, NULL);
		    vldb_EnsureSite(&entry, VLF_RWEXISTS, rwSiteFlag,
				    toservAddrp, toaggrId, -1, 0);
		    if (fromSrepFlIx != -1)
			vldb_EnsureSite(&entry, VLF_ROEXISTS, srepSiteFlag,
					toservAddrp, toaggrId, -1, srepRepLat);
		    code =  vldb_ReplaceEntry(&entry, volIdp, -1, LOCKREL_ALL);
		    if (code)
			goto move;
		    nukeFrom = 2;
		    entryLocked = 0;
		    vldb_TellRepAboutNewEntry(&entry, 0, 1);
		    toFlIx = fromFlIx;
		    fromFlIx = -1;
		    code = FinishMoveTags(&entry, toconn,
					  toservAddrp, toaggrId, volIdp);
		    goto move;
		} else {
		    code = vols_DeleteVolume(toconn, srepTrans);
		    if (code != 0) {
			VOLSERLOG(0, "Cannot delete old target staging \
replica %lu,,%lu on %s/%s: %s\n",
				  AFS_HGETBOTH(srepId),
				  MapSockAddr(toservAddrp),
				  getAggrName(toservAddrp, toaggrId, 0),
				  dfs_dceErrTxt(code));
			goto move;
		    }
		    code = vols_DeleteTrans(toconn, srepTrans);
		    srepTrans = -1;
		    if (code != 0) {
			VOLSERLOG(0, "Error closing existing target staging \
fileset %lu,,%lu on %s/%s: %s\n",
				  AFS_HGETBOTH(srepId),
				  MapSockAddr(toservAddrp),
				  getAggrName(toservAddrp, toaggrId, 0),
				  dfs_dceErrTxt(code));
			goto move;
		    }
		}
	    }
	}

	if (!sameServer) {
	    BOMB_IF("movevolume#17")
		{toSite.CreationUses = toSite.CreationQuota;}
	    /* don't strain our recovery protocol */
	    if (toSite.CreationQuota != 0 &&
		toSite.CreationUses >= toSite.CreationQuota) {
		dce_svc_printf(FTS_S_VPROCS_MAX_FLDB_ENTRIES_MSG);
		return VL_OUTOFQUOTA;
	    }
	}
	/* Turn these bits off so that we (and token management) can work. */
	code = FLAGS_ENCODE(FTSERVER_OP_GETSTATUS | FTSERVER_OP_SETSTATUS, 
			    VOLERR_TRANS_MOVE);
	code = BOMB_EXEC("movevolume#18",
			 vols_CreateTrans(fromconn, volIdp, fromaggrId, code,
					  &fromTrans, 0));
	if (code) fromTrans = -1;
	if (code && code != ENOENT && code != ENODEV
	    && code != FTSERVER_ERR_NOVOL
	    && (!sameServer || code != EXDEV)) {
	    dce_svc_printf(FTS_S_VPROCS_CANNOT_REPAIR_FILESET_MSG,
			   AFS_HGETBOTH(*volIdp),
			   MapSockAddr(fromservAddrp),
			   getAggrName(fromservAddrp, fromaggrId, 0),
			   dfs_dceErrTxt(code));
	    goto move;
	}
	if (code == 0) {
	    code = BOMB_EXEC("movevolume#19",
			     vols_GetStatus(fromconn, fromTrans, &ftstatus));
	    if (code == 0) {
		if (((ftstatus.vss.states & (VOL_OFFLINE | VOL_OUTOFSERVICE |
					     VOL_MOVE_SOURCE)) != 0)
		     || ftstatus.vss.volMoveTimeout != 0) {
		    ftstatus.vss.states &= ~(VOL_OFFLINE | VOL_OUTOFSERVICE |
					     VOL_MOVE_SOURCE);
		    ftstatus.vss.volMoveTimeout = 0;
		    code = BOMB_EXEC("movevolume#20",
				     vols_SetStatus(fromconn, fromTrans,
						    VOL_STAT_STATES |
						    VOL_STAT_VOLMOVETIMEOUT,
						    &ftstatus));
		    }
	    }
	    if (code)
		(void) BOMB_EXEC("movevolume#62",
				 vols_AbortTrans(fromconn, fromTrans));
	    else
		code = BOMB_EXEC("movevolume#21",
				 vols_DeleteTrans(fromconn, fromTrans));
	    fromTrans = -1;
	    if (code)
		goto move;
	}

	preservedStates |= (ftstatus.vss.states
			    & (VOL_IS_REPLICATED|VOL_NOEXPORT));

	/*
	 * Clone the RW volume on the source server.
	 * Get a new volume ID for the local temp clone.  If we move to another
	 * aggregate on the same server, get another new volume ID for the
	 * target volume.  We'll swap the ID numbers when we're done.
	 */
	numNewIDs = 1;
	if (sameServer) {
	    numNewIDs = 2;
	    if (fromSrepFlIx != -1)
		numNewIDs = 3;
	}
	code = BOMB_EXEC("movevolume#22", vldb_GetNewVolumeIds(numNewIDs,
							       fromservAddrp,
							       &newIDs));
	if (code)
	    goto move;
	dumpCloneId = newIDs.bulkIDs_val[0];
	strcpy(targetInitialVolName, (char *)entry.name);
	AFS_hzero(srepTmpId);
	if (sameServer) {
	    targetInitialVolId = newIDs.bulkIDs_val[1];
	    strcat(targetInitialVolName, ".move-dest");
	    if (fromSrepFlIx != -1) {
		srepTmpId = newIDs.bulkIDs_val[2];
		strcpy(srepTmpName, (char *)entry.name);
		strcat(srepTmpName, ".MT.move-dest");
	    }
	} else {
	    targetInitialVolId = *volIdp;
	    if (fromSrepFlIx != -1) {
		srepTmpId = srepId;
		strcpy(srepTmpName, (char *)entry.name);
		strcat(srepTmpName, ".readonly");
	    }
	}
	strcpy(volName, (char *)entry.name);
	strcat(volName, ".move-temp");
	nukeFClone = 0;
	if (code = BOMB_EXEC("movevolume#23",
			     CloneVolume(fromconn, fromservAddrp,
					 fromSite.KerbPrin, fromaggrId, volIdp,
					 volName, VOLTIX_TO_VOLTYPE(3),
					 &dumpCloneId, 0, 0, 0)))
	    goto move;
	nukeFClone = 1;

	if (wasBackup && sameServer) {
	    /*
	     * Delete the RW's backup volume on the from site (if one exists)
	     */
	    VOLSERLOG(1, "Deleting backup of original fileset %lu,,%lu on \
aggr %lu\n",
		      AFS_HGETBOTH(backupId), fromaggrId);
	    code = BOMB_EXEC("movevolume#24",
			     fts_DeleteVolume(fromconn, fromaggrId, &backupId,
					      0, 0));
	    if (code && code != ENOENT && code != ENODEV &&
		code != FTSERVER_ERR_NOVOL)
		goto move;
	    vldb_DeleteVolume(&entry, fromservAddrp, fromaggrId,
			      BACKVOL, &backupId, NULL);
	    code = vldb_ReplaceEntry(&entry, &backupId, BACKVOL, 0);
	    if (code)
		goto move;
	}

	nukeFClone = 0;
	nukeTo = 1;
	code = BOMB_EXEC("movevolume#25",
			 CopyCloneVolume(fromconn, fromaggrId, volIdp,
					 &targetInitialVolId,
					 targetInitialVolName, &dumpCloneId,
					 toconn, toaggrId, &entry,
					 fromservAddrp, fromSite.KerbPrin,
					 toservAddrp, toSite.KerbPrin,
					 preservedStates, &fromTrans, &toTrans,
					 &readTokDesc, sameServer,
					 (fromSrepFlIx != -1), fromSrepAggrId,
					 &srepId, &srepTmpId, srepTmpName,
					 sockFlag));
	if (code == FTU_E_RESTORED_INCONSISTENT_FSET) {
	    isInconsistent = 1;
	    code = 0;
	    dce_svc_printf(FTS_S_VPROCS_FILESET_WAS_INCONSISTENT_MSG);
	} else if (code != 0)
	    goto move;
	nukeFClone = -1;

	/*
	 * Now adjust the flags so that the new volume becomes official
	 */
	if (!sameServer) {
	    /* turn off VOL_ZAPME this way */
	    if (code = BOMB_EXEC("movevolume#26",
				 SetMoveBaseStatus(toconn, toTrans,
						   /* states bits to set */
						   preservedStates |
						   (VOL_TYPE_RW | VOL_RW),
						   /* states bits to clear */
						   (VOL_REPFIELD |
						    VOL_OUTOFSERVICE |
						    VOL_OFFLINE |
						    VOL_ZAPME)))) {
		goto move;
	    }
	} else {	/* set the flags appropriately, here and now */
	    /* else turn off VOL_ZAPME like this */
	    ftstatus.vss.states = preservedStates | (VOL_TYPE_RW | VOL_RW);
	    code = BOMB_EXEC("movevolume#27",
			     vols_SetStatus(toconn, toTrans, VOL_STAT_STATES,
					    &ftstatus));
	    if (code)
		goto move;
	}

	/* New fileset is completely set on the destination--just busy. */
	lock_ObtainWrite(&transKALock);
	if (isInconsistent)
	    code = vols_AbortTrans(toconn, toTrans);
	else
	    code = BOMB_EXEC("movevolume#28",
			     vols_DeleteTrans(toconn, toTrans));
	toTrans = -1;
	lock_ReleaseWrite(&transKALock);
	if (code)
	    goto move;

	if (!sameServer) {
	    /* New fileset is intact, with VOL_MOVE_TARGET set and kept alive.*/
	    /* Mark the source (while busy) as VOL_MOVE_SOURCE. */
	    code = BOMB_EXEC("movevolume#29",
			     SetMoveTags(fromconn, &fromTrans, fromservAddrp,
					 volIdp, fromaggrId, MOVETAG_SOURCE));
	    if (code)
		goto move;
	}

	if (sameServer) {
	    /* We're moving between aggregates of the same server. */
	    /* All done.  No CM notification necessary. */
	    /* Swap IDs and delete the old guy. */
	    /* Afterwards, we'll update the FLDB. */
	    VOLSERLOG(1, "Swapping fileset IDs: exchanging source %lu,,%lu/%u \
with target %lu,,%lu/%lu, then deleting old source\n",
		      AFS_HGETBOTH(*volIdp), fromaggrId,
		      AFS_HGETBOTH(targetInitialVolId), toaggrId);
	    nukeFrom = nukeTo = 0; /* can't be sure where swap will leave us */
	    code = BOMB_EXEC("movevolume#53",
			     SwapIntoPlace(toconn, fromTrans,
					   volIdp, &targetInitialVolId,
					   fromaggrId, toaggrId,
					   (fromSrepFlIx != -1), &srepId,
					   &srepTmpId, fromservAddrp));
	    if (code)
		(void) BOMB_EXEC("movevolume#66",
				 vols_AbortTrans(toconn, fromTrans));
	    else
		code = BOMB_EXEC("movevolume#54",
				 vols_DeleteTrans(toconn, fromTrans));
	    BOMB_IF("movevolume#54.1") { code = ENOTTY; }
	    fromTrans = -1;
	    if (code)
		goto move;
	    nukeFrom = -1; nukeTo = -1;	/* move is complete */
	}
	/* 
	 * Update VLDB so that volume's location is set to its new
	 * home in destination site. Also reflect the fact that the
	 * backup volume (if any) is gone too.
	 */
	BOMB_POINT("movevolume#30");
	if (wasBackup && !sameServer)
	    vldb_DeleteVolume(&entry, fromservAddrp, fromaggrId,
			      BACKVOL, &backupId, NULL);
	if (fromSrepFlIx != -1)
	    vldb_DeleteVolume(&entry, fromservAddrp, fromSrepAggrId, ROVOL,
			      &srepId, NULL);
	vldb_DeleteVolume(&entry, fromservAddrp, fromaggrId, RWVOL, volIdp,
			  NULL);
	vldb_EnsureSite(&entry, VLF_RWEXISTS, rwSiteFlag, toservAddrp,
			toaggrId, -1, 0);
	if (fromSrepFlIx != -1)
	    vldb_EnsureSite(&entry, VLF_ROEXISTS, srepSiteFlag,
			    toservAddrp, toaggrId, -1, srepRepLat);
	if (!sameServer) {
	    nukeFrom = nukeTo = 0;
	}
	code = BOMB_EXEC("movevolume#31",
			 vldb_ReplaceEntry(&entry, volIdp, -1, LOCKREL_ALL));
	if (code)
	    goto move;
	entryLocked = 0;
	vldb_TellRepAboutNewEntry(&entry, 0, 1);
	if (!sameServer) {
	    nukeFrom = 1; nukeTo = -1;
	}
	toFlIx = fromFlIx;
	fromFlIx = -1;
	if (sameServer)
	    goto move;	/* we're done */
	/* fromTrans is left busy; toTrans is completed. */
    } else {
	/*
	 * FLDB points to the target already.  Open the source fileset for
	 * updating.
	 */
	code = FLAGS_ENCODE(FTSERVER_OP_SETFLAGS
			    | FTSERVER_OP_GETSTATUS
			    | FTSERVER_OP_SETSTATUS
			    | FTSERVER_OP_NOACCESS
			    | FTSERVER_OP_SWAPIDS
			    | FTSERVER_OP_DELETE,
			    VOLERR_TRANS_MOVE);
	code = BOMB_EXEC("movevolume#32",
			 vols_CreateTrans(fromconn, volIdp, fromaggrId, code,
					  &fromTrans, 0));
	if (code) fromTrans = -1;
	if (code == ENOENT || code == ENODEV || code == FTSERVER_ERR_NOVOL
	    || (sameServer && code == EXDEV)) {
	    /* Already moved, and the source is gone. */
	    dce_svc_printf(FTS_S_VPROCS_FILESET_ALREADY_ON_SERVER_MSG,
			   AFS_HGETBOTH(*volIdp),
			   MapSockAddr(toservAddrp),
			   getAggrName(toservAddrp, toaggrId, 0));
	    /* Clear VOL_MOVE_TARGET if it's still set on the target. */
	    /* Know that source would have been deleted only after token
	     obtained or timeout expired. */
	    code = BOMB_EXEC("movevolume#33",
			     FinishMoveTags(&entry, toconn, toservAddrp,
					    toaggrId, volIdp));
	    goto move;
	}
	if (code)
	    goto move;	/* some other problem */
	code = BOMB_EXEC("movevolume#34",
			 vols_GetStatus(fromconn, fromTrans, &ftstatus));
	if (code)
	    goto move;	/* yet another problem */
	if (ftstatus.vss.states & VOL_LCLMOUNT) {
	    dce_svc_printf(FTS_S_VPROCS_FILESET_ON_AGGREGATE_MSG,
			   AFS_HGETBOTH(*volIdp),
			   getAggrName(fromservAddrp, fromaggrId, 0),
			   MapSockAddr(fromservAddrp));
	    dce_svc_printf(FTS_S_VPROCS_MOUNTED_LOCALLY_MSG);
	    code = FTSERVER_ERR_LCLMOUNT;
	    goto move;
	}
	if (ftstatus.vss.states & VOL_MOVE_TARGET) {
	    dce_svc_printf(FTS_S_VPROCS_SOURCE_MARKED_DESTINATION_MSG);
	    code = FTS_ERR_BADOP;
	    goto move;
	}
	if (ftstatus.vss.states & VOL_ZAPME) {
	    /* Again, can just destroy the thing and finish. */
	    nukeFrom = 2;
	    dce_svc_printf(FTS_S_VPROCS_FILESET_ON_SERVER_DELETING_MSG,
			   AFS_HGETBOTH(*volIdp),
			   MapSockAddr(toservAddrp),
			   getAggrName(toservAddrp, toaggrId, 0));
	    code = BOMB_EXEC("movevolume#35",
			     vols_DeleteVolume(fromconn, fromTrans));
	    if (code)
		(void) BOMB_EXEC("movevolume#63",
				 vols_AbortTrans(fromconn, fromTrans));
	    else
		code = BOMB_EXEC("movevolume#36",
				 vols_DeleteTrans(fromconn, fromTrans));
	    fromTrans = -1;
	    if (code)
		goto move;
	    nukeFrom = -1;
	    /* Clear VOL_MOVE_TARGET if it's still set on the target. */
	    /* Know that source would have been deleted only after token
	     obtained or timeout expired. */
	    code = BOMB_EXEC("movevolume#37",
			     FinishMoveTags(&entry, toconn, toservAddrp,
					    toaggrId, volIdp));
	    goto move;
	}
	/* Well, we have to re-do the post-move protocol. */
	if (!sameServer) {
	    /* Ensure that VOL_MOVE_TARGET is kept alive on the target. */
	    code = FLAGS_ENCODE(FTSERVER_OP_SETSTATUS
				| FTSERVER_OP_SETFLAGS
				| FTSERVER_OP_GETSTATUS,
				VOLERR_TRANS_MOVE);
	    code = BOMB_EXEC("movevolume#38",
			     vols_CreateTrans(toconn, volIdp, toaggrId, code,
					      &toTrans, 0));
	    if (code) {
		toTrans = -1;
		goto move;
	    }
	    code = BOMB_EXEC("movevolume#39",
			     vols_GetStatus(toconn, toTrans, &ftstatus));
	    if (code)
		goto move;	/* yet another problem */
	    if (ftstatus.vss.states & VOL_MOVE_SOURCE) {
		dce_svc_printf(FTS_S_VPROCS_DESTINATION_MARKED_SOURCE_MSG);
		code = FTS_ERR_BADOP;
		goto move;
	    }
	    if (!(ftstatus.vss.states & VOL_MOVE_TARGET)) {
		/* Target bit is clear.  Move was about done.  Just finish up.*/
		code = BOMB_EXEC("movevolume#40",
				 vols_DeleteTrans(toconn, toTrans));
		toTrans = -1;
		if (code)
		    goto move;
		dce_svc_printf(FTS_S_VPROCS_ALREADY_MOVED_MSG,
			       AFS_HGETBOTH(*volIdp),
			       MapSockAddr(toservAddrp),
			       getAggrName(toservAddrp, toaggrId, 0));
		code = BOMB_EXEC("movevolume#41",
				 vols_DeleteVolume(fromconn, fromTrans));
		if (code)
		    (void) BOMB_EXEC("movevolume#64",
				     vols_AbortTrans(fromconn, fromTrans));
		else
		    code = BOMB_EXEC("movevolume#42",
				     vols_DeleteTrans(fromconn, fromTrans));
		fromTrans = -1;
		if (code)
		    goto move;
		/* Clear VOL_MOVE_TARGET if it's still set on the target. */
		/* Know that source would have been deleted only after token
		 obtained or timeout expired. */
		code = BOMB_EXEC("movevolume#43",
				 FinishMoveTags(&entry, toconn, toservAddrp,
						toaggrId, volIdp));
		goto move;
	    }
	    /* Have to to continue in the middle */
	    code = BOMB_EXEC("movevolume#43a",
			     SetMoveTags(toconn, &toTrans, toservAddrp, volIdp,
					 toaggrId, MOVETAG_TARGET));
	    if (code)
		goto move;
	    if (code = BOMB_EXEC("movevolume#44",
				 SetMoveBaseStatus(toconn, toTrans,
					 /* states bits to set */
					 preservedStates
					 | (VOL_TYPE_RW | VOL_RW),
					 /* states bits to clear first */
					 (VOL_REPFIELD | VOL_OUTOFSERVICE
					  | VOL_OFFLINE | VOL_ZAPME)))) {
		goto move;
	    }
	    /* New fileset is completely set on the destination--just busy. */
	    lock_ObtainWrite(&transKALock);
	    code = BOMB_EXEC("movevolume#45",
			     vols_DeleteTrans(toconn, toTrans));
	    toTrans = -1;
	    lock_ReleaseWrite(&transKALock);
	    if (code)
		goto move;
	    /* Target fileset is intact, with VOL_MOVE_TARGET set and kept
	     * alive.
	     */
	    /* Mark the source (while busy) as VOL_MOVE_SOURCE. */
	    code = BOMB_EXEC("movevolume#46",
			     SetMoveTags(fromconn, &fromTrans, fromservAddrp,
					 volIdp, fromaggrId, MOVETAG_SOURCE));
	    if (code)
		goto move;
	}
    }

    /* Common code, whether initiating or resuming a move. */

    if (!sameServer) {
	/* turn on VOL_OFFLINE on the source in this way */
	if (code = BOMB_EXEC("movevolume#47",
			     SetMoveBaseStatus(fromconn, fromTrans,
				     /* states bits to set */
				     VOL_OFFLINE,
				     /* states bits to clear first */
				     (VOL_OUTOFSERVICE | VOL_ZAPME)))) {
	    goto move;
	}
	/* Close the source fileset, exposing its VOL_MOVE_SOURCE attribute. */
	lock_ObtainWrite(&transKALock);
	code = BOMB_EXEC("movevolume#48",
			 vols_DeleteTrans(fromconn, fromTrans));
	fromTrans = -1;
	lock_ReleaseWrite(&transKALock);
	if (code)
	    goto move;

	/*
	 * Obtain tokens that will force all CMs to switch their focus to the
	 * new site.
	 *
	 * AFS_FLAG_MOVE_SOURCE_OK permits token-grants while
	 * VOL_MOVE_SOURCE and VOL_OFFLINE are set.
	 */
	code = BOMB_EXEC("movevolume#48a",
			 fts_GetToken((afsNetAddr *)fromservAddrp,
				      (unsigned char *)fromSite.KerbPrin,
				      volIdp, TKN_SPOT_THERE,
				      AFS_FLAG_MOVE_SOURCE_OK |
				      AFS_FLAG_FORCEVOLQUIESCE |
				      AFS_FLAG_FORCEREVOCATIONS,
				      &thereTokDesc));
	if (code)
	    goto move;
	nukeFrom = 2;	/* Can delete the source since we got the token */

	/* Declare victory: */
	/* Clear out the VOL_MOVE_TARGET bit now */
	code = BOMB_EXEC("movevolume#49",
			 ClearMoveTags(toservAddrp, volIdp, toaggrId, 0, 0));
	if (code)
	    goto move;

#if 0
	/* eventually flush out all old tokens this way, maybe */
	{int flushTokDesc;

	if (BOMB_EXEC("movevolume#50", fts_GetToken(fromservAddrp,
			 (unsigned char *)fromSite.KerbPrin,
			 volIdp,
			 TKN_LOCK_WRITE | TKN_OPEN_DELETE,
			 AFS_FLAG_MOVE_SOURCE_OK | AFS_FLAG_FORCEREVOCATIONS,
			 &flushTokDesc)) == 0) {
	    (void) BOMB_EXEC("movevolume#65", fts_ReleaseToken(flushTokDesc));
	}
	}
#endif /* 0 */

	if (wasBackup) {
	    /*
	     * Delete the RW's backup volume on the from site (if one exists)
		*/
	    VOLSERLOG(1, "Deleting backup of original fileset %lu,,%lu on \
aggr %lu\n",
		      AFS_HGETBOTH(backupId), fromaggrId);
	    code = BOMB_EXEC("movevolume#51",
			     fts_DeleteVolume(fromconn, fromaggrId, &backupId,
					      0, 0));
	    if (code && code != ENOENT && code != ENODEV
		&& code != FTSERVER_ERR_NOVOL
		&& (!sameServer || code != EXDEV))
		goto move;
	}

	/*
	 * All done; all relevant CMs notified.
	 * Delete the original RW volume on the from site.
	 */
	VOLSERLOG(1, "Deleting original fileset %lu,,%lu on aggr %lu\n",
		  AFS_HGETBOTH(*volIdp), fromaggrId);
	code = BOMB_EXEC("movevolume#52",
			ClearMoveTags(fromservAddrp, volIdp, fromaggrId, 0, 1));
	if (code)
	    goto move;
	if (fromSrepFlIx != -1) {
	    code = fts_DeleteVolume(fromconn, fromSrepAggrId, &srepId, 0, 0);
	    if (code)
		goto move;
	}
	nukeFrom = -1; nukeTo = -1;	/* move is complete */
    } else {
	/* We're moving between aggregates of the same server. */
	/* All done.  No CM notification necessary. */
	/* Swap IDs and delete the old guy. */
	VOLSERLOG(1, "Swapping fileset IDs; exchanging source %lu,,%lu/%u \
with target %lu,,%lu/%lu, then deleting old source\n",
		  AFS_HGETBOTH(*volIdp), fromaggrId,
		  AFS_HGETBOTH(targetInitialVolId), toaggrId);
	nukeFrom = nukeTo = 0;	/* not sure about effect of swap */
	code = BOMB_EXEC("movevolume#53",
			 SwapIntoPlace(toconn, fromTrans, volIdp,
				       &targetInitialVolId, fromaggrId,
				       toaggrId, (fromSrepFlIx != -1), &srepId,
				       &srepTmpId, fromservAddrp));
	if (code)
	    (void) BOMB_EXEC("movevolume#66",
			     vols_AbortTrans(toconn, fromTrans));
	else
	    code = BOMB_EXEC("movevolume#54",
			     vols_DeleteTrans(toconn, fromTrans));
	fromTrans = -1;
	if (code)
	    goto move;
	nukeFrom = -1; nukeTo = -1;	/* move is complete */
    }

move:
    if (!sameServer) {
	/* Turn off the periodic updating without clearing any bits. */
	(void) BOMB_EXEC("movevolume#67",
			 ClearMoveTags(fromservAddrp, volIdp, fromaggrId, 1,0));
	(void) BOMB_EXEC("movevolume#68",
			 ClearMoveTags(toservAddrp, &targetInitialVolId,
				       toaggrId, 1, 0));
    }
    if (toTrans >= 0) {
	lock_ObtainWrite(&transKALock);
	if (code)
	    (void) BOMB_EXEC("movevolume#69", vols_AbortTrans(toconn, toTrans));
	else
	    code = BOMB_EXEC("movevolume#70",
			     vols_DeleteTrans(toconn, toTrans));
	toTrans = -1;
	lock_ReleaseWrite(&transKALock);
    }
    if (fromTrans >= 0) {
	lock_ObtainWrite(&transKALock);
	if (code)
	    (void) BOMB_EXEC("movevolume#71",
			     vols_AbortTrans(fromconn, fromTrans));
	else
	    code = BOMB_EXEC("movevolume#72",
			     vols_DeleteTrans(fromconn, fromTrans));
	fromTrans = -1;
	lock_ReleaseWrite(&transKALock);
    }
    if (srepTrans >= 0) {
	lock_ObtainWrite(&transKALock);
	if (code)
	    (void) vols_AbortTrans(toconn, srepTrans);
	else
	    code = vols_DeleteTrans(toconn, srepTrans);
	srepTrans = -1;
	lock_ReleaseWrite(&transKALock);
    }
    if (thereTokDesc >= 0) {
	(void) BOMB_EXEC("movevolume#73", fts_ReleaseToken(thereTokDesc));
	thereTokDesc = -1;
    }
    if (readTokDesc >= 0) {
	(void) BOMB_EXEC("movevolume#74", fts_ReleaseToken(readTokDesc));
	readTokDesc = -1;
    }
    /* nukeFClone is 1 if we know we have the clone, 0 if maybe, -1 if unknown
     */
    if (nukeFClone >= 0) {
	(void) BOMB_EXEC("movevolume#75",
			 fts_DeleteVolume(fromconn, fromaggrId, &dumpCloneId, 2,
					  0));
    }
#if 0
/* Do this only if we have two copies of the R/W with the correct ID, which is
    not the case in a single-server move. */
/* More horribly, the FLDB might have been updated OK, the updating call
  might have returned an error, and the read call might return old data.... */
    if (!sameServer && nukeFrom == 0 && nukeTo == 0 && rwSiteFlag != 0) {
	/* read FLDB entry and decide which to keep */
	if (BOMB_EXEC("movevolume#55",
		      vldb_GetEntryByID(volIdp, RWVOL, &entry, 0)) == 0) {
	    /* set nukeFrom and nukeTo to the decision result */
	    fts_GetIndices(&entry, xl);	/* refresh this */
	    if (xl[RWVOL] >= 0) {
		/* where's the R/W instance, according to FLDB? */
		siteIx = BOMB_EXEC("movevolume#56",
				   vldb_FindRepSite(&entry, 0, -1, rwSiteFlag));
		if (siteIx >= 0) {
		    if (vldb_IsSiteHere(&entry, siteIx, fromservAddrp,
					fromaggrId)) {
			/* ultimately, no movement */
			nukeFrom = -1; nukeTo = 1;
		    } else if (vldb_IsSiteHere(&entry, siteIx, toservAddrp,
					       toaggrId)) {
			/* move completed */
			nukeFrom = 1; nukeTo = -1;
		    }
		}
	    }
	}
    }
#endif /* 0 */
    /* If nukeFrom is 1, then we have to get the TKN_SPOT_THERE token before
      deleting the from fileset.  Punt on that for now; the user can
      re-execute the command just as effectively. */
    /* If nukeFrom is 2, then we've gotten the TKN_SPOT_THERE token
       and this can go ahead and destroy the old fileset. */
    if (nukeFrom > 1) {
	(void) BOMB_EXEC("movevolume#76",
			 fts_DeleteVolume(fromconn, fromaggrId, volIdp, 2, 0));
	if (sameServer)
	    (void) BOMB_EXEC("movevolume#77",
			     fts_DeleteVolume(fromconn, fromaggrId,
					      &targetInitialVolId, 2, 0));
	if (fromSrepFlIx != -1) {
	    (void) fts_DeleteVolume(fromconn, fromSrepAggrId, &srepId, 2, 0);
	    if (sameServer)
		(void)fts_DeleteVolume(fromconn, fromSrepAggrId, &srepTmpId,
				       2, 0);
	}
    }
    if (nukeTo > 0) {
	(void) BOMB_EXEC("movevolume#78",
			 fts_DeleteVolume(toconn, toaggrId, volIdp, 2, 0));
	if (sameServer)
	    (void) BOMB_EXEC("movevolume#79",
			     fts_DeleteVolume(toconn, toaggrId,
					      &targetInitialVolId, 2, 0));
	if (fromSrepFlIx != -1) {
	    (void) fts_DeleteVolume(toconn, toaggrId, &srepId, 2, 0);
	    if (sameServer)
		(void)fts_DeleteVolume(toconn, toaggrId, &srepTmpId, 2, 0);
	}
    }
    /*
     * All done, release the lock
     */
    if (entryLocked) {
	tcode = BOMB_EXEC("movevolume#57",
			  vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL));
	if (code == 0)
	    code = tcode;
    }
    return (code);
}


/*
 * Release volume <volIdp> on [fromservAddrp, fromaggrId]
 * If the synchronous flag is 1, then wait for all of the repservers
 * managing sites for this volume to pick up the new release.
 */
long VC_ReleaseVolume(volIdp, fromservAddrp, fromaggrId, synchronous)
    afs_hyper_t *volIdp;
    struct sockaddr *fromservAddrp;
    unsigned long fromaggrId;
    int synchronous;
{
    rpc_binding_handle_t connp = 0;
    long totid = -1;
    afs_hyper_t tempVolId, pubROId;
    unsigned long toaggrId;
    long code, tcode;
    struct ftserver_status tstatus;
    struct ftserver_Date dumpDate;
    struct vldbentry entry;
    char volName[FTSERVER_MAXFSNAME];
    char cloneName[FTSERVER_MAXFSNAME];
    int cloneisreadonly;        /* IF a RO sites on the same partition */
    int xl[MAXTYPES];
    bulkIDs newIDs;
    int localIx;
    int nukeLocalClone, nukeStage, anyRepChange;
    struct siteDesc fromSite;
    int replicaExists;
    afs_hyper_t stagingVV;
    int sockFlag = 0;

    checkKALockInit();
    checkInitConns();
    /*
     * First, lock vldb entry for the duration of the call.
     */
    if (code = vldb_SetLock(volIdp, RWVOL, VLOP_RELEASE)) {
	return code;
    }
    nukeLocalClone = nukeStage = -1;
    anyRepChange = 0;
    AFS_hzero(tempVolId);
    AFS_hzero(stagingVV);
    /*
     * Now get hold of the vldb entry for the volume to release
     */
    if (code = vldb_GetEntryByID(volIdp, RWVOL, &entry, 0))
	goto release_end;
    fts_GetIndices(&entry, xl);

    if ((entry.flags & VLF_LAZYREP) == 0) {
	dce_svc_printf(FTS_S_VPROCS_SET_REPLICATION_TYPE_MSG,
		       AFS_HGETBOTH(*volIdp));
	code = FTS_ERR_BADOP;
	goto release_end;
    }
    if ((entry.flags & VLF_LAZYRELE) == 0) {
	dce_svc_printf(FTS_S_VPROCS_RELEASE_NOT_SUPPORTED_MSG,
		       AFS_HGETBOTH(*volIdp));
	code = FTS_ERR_BADOP;
	goto release_end;
    }
#if !defined(OSF_NO_SOCKET_DUMP)
    if ((entry.flags & VLF_SOCKFLAG) == 0) {
	sockFlag = 0;
    } else {
	sockFlag = 1;
    }
#endif /* OSF_NO_SOCKET_DUMP */
    if (xl[RWVOL] < 0 || AFS_hiszero(entry.VolIDs[xl[RWVOL]])) {
	dce_svc_printf(FTS_S_VPROCS_NO_READ_WRITE_FILESET_MSG,
		       AFS_HGETBOTH(*volIdp));
	code = FTS_ERR_NEED_RW;
	goto release_end;
    }
    if (xl[ROVOL] < 0 || AFS_hiszero(entry.VolIDs[xl[ROVOL]])) {
	dce_svc_printf(FTS_S_VPROCS_NO_READ_ONLY_FILESET_MSG,
		       AFS_HGETBOTH(*volIdp));
	code = FTS_ERR_NEED_RW;
	goto release_end;
    }
    if (!AFS_hsame(entry.VolIDs[xl[RWVOL]], *volIdp)) {
	 dce_svc_printf(FTS_S_VPROCS_NOT_READ_WRITE_FILESET_MSG,
			AFS_HGETBOTH(*volIdp));
	 code = FTS_ERR_BADOP;
	 goto release_end;
    }
    localIx = fts_releaseRepSite(&entry);
    if (localIx < 0) {
	switch (localIx) {
	    case -1:
		dce_svc_printf(FTS_S_VPROCS_NOT_RELEASE_REPLICATED_MSG,
			       AFS_HGETBOTH(*volIdp));
		break;
	    case -2:
		dce_svc_printf(FTS_S_VPROCS_NO_READ_WRITE_ID_MSG,
			       AFS_HGETBOTH(*volIdp));
		break;
	    case -3:
		dce_svc_printf(FTS_S_VPROCS_NO_READ_ONLY_STAGING_SITE_MSG,
			       AFS_HGETBOTH(*volIdp));
		break;
	}
	code = FTS_ERR_BADOP;
	goto release_end;
    }
    cloneisreadonly = (entry.sitePartition[localIx] == fromaggrId);

    code = vldb_GetSiteInfo(fromservAddrp, &fromSite);
    if (code) {
	dce_svc_printf(FTS_S_VPROCS_CANNOT_GET_SITE_INFO_MSG, MapSockAddr(fromservAddrp), dfs_dceErrTxt(code));
	goto release_end;
    }

    pubROId = entry.VolIDs[xl[ROVOL]];
    connp = connToServer(fromservAddrp, fromSite.KerbPrin, SERVERKIND_FT);
    if (connp == (rpc_binding_handle_t)NULL) {
	code = FTS_ERR_BADCONN;
	goto release_end;
    }
    strcpy(volName, (char *)entry.name); strcat(volName, ".readonly");
    if (cloneisreadonly) {				/* make a new clone */
	/*
	 * Need to delete the old clone and make a new one
	 */
	VOLSERLOG(1, "RO fileset on same aggregate as RW fileset; just \
cloning it.\n");
	replicaExists = 0;
	if (code = fts_VolumeExists(connp, fromaggrId, &pubROId)) {
	    if (code != EEXIST)
		goto release_end;
	    replicaExists = 1;
	}
	anyRepChange = 1;
	if (code = CloneVolume(connp, fromservAddrp, NULL, fromaggrId,
			       volIdp, volName, VOLTIX_TO_VOLTYPE(ROVOL),
			       &pubROId, replicaExists, 0, 1))
	    goto release_end;
	if (code = SetFlags(connp, fromaggrId, &pubROId,
			    VOL_REP_RELEASE | VOL_TYPE_RO | VOL_READONLY))
	    goto release_end;
	if (synchronous) {
	    if ((code = vols_GetOneVolStatus(connp, &pubROId,
					     fromaggrId, &tstatus,
					     0)) != 0) {
		goto release_end;
	    } else {
		stagingVV = tstatus.vsd.volversion;
	    }
	}
	    
    } else {       /* ro volume on different aggregate from rw volume */
	VOLSERLOG(1, "Copying local RO across aggregates (%lu -> %lu).\n",
		  fromaggrId, entry.sitePartition[localIx]);
	if (code = vldb_GetNewVolumeIds(1, fromservAddrp, &newIDs))
	    goto release_end;
	tempVolId = newIDs.bulkIDs_val[0];
	strcpy(cloneName, (char *)entry.name); strcat(cloneName, ".rel-tmp");
	nukeLocalClone = 0;
	if (code = CloneVolume(connp, fromservAddrp, NULL, fromaggrId,
			       volIdp, cloneName,
			       VOLTIX_TO_VOLTYPE(3),
			       &tempVolId, 0, 0, 1))
	    goto release_end;
	nukeLocalClone = 1;
    }
    /*
     * Get the replication sites (server, aggregate, volid of the latest r-o
     * volume) from the VLDB.
     */
    toaggrId = entry.sitePartition[localIx];
    if (cloneisreadonly) {
	/* we did clone above. */
    } else {
	/*
	 * Get connection to the replication site
	 */
	bzero((char *)&dumpDate, sizeof(dumpDate));
	code = vols_CreateTrans(connp, &pubROId, toaggrId,
				FLAGS_ENCODE(FTSERVER_OP_RESTORE |
					     FTSERVER_OP_GETSTATUS |
					     FTSERVER_OP_SETSTATUS |
					     FTSERVER_OP_DELETE,
					     VOLERR_TRANS_RELEASE),
				&totid, 0);
	if (code) totid = -1;
	if (code && code != ENOENT && code != ENODEV &&
	    code != FTSERVER_ERR_NOVOL) {
	    dce_svc_printf(FTS_S_VPROCS_CANNOT_CHECK_REPLICATION_SITE_MSG,
			   MapSockAddr(fromservAddrp),
			   getAggrName(fromservAddrp, toaggrId, 0),
			   dfs_dceErrTxt(code));
	    goto release_end;
	} else if (code == ENOENT || code == ENODEV ||
		   code == FTSERVER_ERR_NOVOL) {
	    /*
	     * The volume does not exist; create a new volume instead
	     */
	    VOLSERLOG(1, "Creating a new fileset %lu,,%lu on the replication \
site...\n", 
		      AFS_HGETBOTH(pubROId));
	    nukeStage = 0;
	    if (code = vols_CreateVolume(connp, toaggrId, volName,
					 VOLTIX_TO_VOLTYPE(ROVOL),
				FLAGS_ENCODE(FTSERVER_OP_RESTORE |
					     FTSERVER_OP_GETSTATUS |
					     FTSERVER_OP_SETSTATUS |
					     FTSERVER_OP_DELETE,
					     VOLERR_TRANS_RELEASE),
					 &entry.VolIDs[xl[RWVOL]], &pubROId,
					 &totid)) {
		VOLSERLOG(1, "**failure\n");
		totid = -1;
		/* we can tell at least some cases of getting nowhere. */
		if (code == FTSERVER_ERR_BADAGGREGATE
		    || code == DAUT_ERROR_ACCESS_DENIED)
		    nukeStage = -1;
		goto release_end;
	    }
	    VOLSERLOG(1, "done\n");
	    nukeStage = 1;
	    dumpDate.mask = 0;		/* Dump whole volume */
	    anyRepChange = 1;
	    code = ForwardVolume(&entry, connp, fromaggrId,
				 volIdp, &tempVolId, &pubROId,
				 &dumpDate, connp, fromservAddrp, &totid,
				 fromSite.KerbPrin, toaggrId, sockFlag);
	    if (synchronous && code == 0) {
		if ((tcode = vols_GetStatus(connp, totid, &tstatus)) == 0) {
		    stagingVV = tstatus.vsd.volversion;
		}
	    }
	    if (code == 0) {
		lock_ObtainWrite(&transKALock);
		code = vols_DeleteTrans(connp, totid);
		totid = -1;
		lock_ReleaseWrite(&transKALock);
	    }
	    if (code)
		goto release_end;
	    nukeStage = -1;
	} else {	/* volume exists */
	    /*
	     * Get the creation date of latest RO volume on the
	     * replication site
	     */
	    if (code = vols_GetStatus(connp, totid, &tstatus)) {
		lock_ObtainWrite(&transKALock);
		(void) vols_AbortTrans(connp, totid);	/* XXXX */
		totid = -1;
		lock_ReleaseWrite(&transKALock);
		goto release_end;
	    }
	    dumpDate.fromVersion = tstatus.vsd.volversion;
	    dumpDate.mask = 4;	/* dump from the given volume version */
	    nukeStage = 0;		/* may be made inconsistent */
	    if (tstatus.vss.states & VOL_DELONSALVAGE) {
		/* currently inconsistent--get a full dump */
		dumpDate.mask = 0;
		nukeStage = 1;	/* if we can't restore here */
	    }
	    anyRepChange = 1;
	    VOLSERLOG(1, "About to bring the existing ro fileset %lu,,%lu up \
to date\n", 
		      AFS_HGETBOTH(pubROId));
	    code = ForwardVolume(&entry, connp, fromaggrId, volIdp,
				 &tempVolId, &pubROId, &dumpDate,
				 connp, fromservAddrp, &totid,
				 fromSite.KerbPrin, toaggrId, sockFlag);

	    if (synchronous && code == 0) {
		if ((tcode = vols_GetStatus(connp, totid, &tstatus)) == 0) {
		    stagingVV = tstatus.vsd.volversion;
		}
	    }
	    if (code) {
		tcode = code;
		if (code == FTSERVER_ERR_DISJOINT_DATE
		    || code == FTSERVER_ERR_DISJOINT_VV
		    || code == FTSERVER_ERR_DISJOINT2_DATE
		    || code == FTSERVER_ERR_DISJOINT2_VV)
		    tcode = 0;
		if (tcode == 0 && nukeStage == 0) {
		    /* just let it continue to exist with its old fileset vsn */
		    lock_ObtainWrite(&transKALock);
		    tcode = vols_DeleteTrans(connp, totid);
		    totid = -1;
		    lock_ReleaseWrite(&transKALock);
		    if (tcode == 0)
			nukeStage = -1;	/* was OK and wasn't changed--leave it*/
		    anyRepChange = 0;
		}
		goto release_end;
	    }
	    lock_ObtainWrite(&transKALock);
	    code = vols_DeleteTrans(connp, totid);
	    totid = -1;
	    lock_ReleaseWrite(&transKALock);
	    if (code != 0)
		goto release_end;
	    nukeStage = -1;	/* all better now */
	}
    }
    if ((entry.flags & VLF_ROEXISTS) == 0) {
	entry.flags |= VLF_ROEXISTS;
	anyRepChange = 1;
	if (code = vldb_ReplaceEntry(&entry, volIdp, RWVOL, 0)) /* keep lock */
	    goto release_end;
    }
release_end:
    if (code) {
	dce_svc_printf(FTS_S_VPROCS_CANNOT_RELEASE_FILESET_MSG,
		       AFS_HGETBOTH(*volIdp), dfs_dceErrTxt(code));
    }
    if (totid >= 0) {
	if (nukeStage >= 0) {
	    dce_svc_printf(FTS_S_VPROCS_DELETE_DAMAGED_READ_ONLY_MSG,
			   AFS_HGETBOTH(pubROId));
	    if (vols_DeleteVolume(connp, totid) == 0)
		nukeStage = -1;
	}
	lock_ObtainWrite(&transKALock);
	if (code)
	    (void) vols_AbortTrans(connp, totid);
	else
	    code = vols_DeleteTrans(connp, totid);
	totid = -1;
	lock_ReleaseWrite(&transKALock);
    }
    if (anyRepChange) {
	vldb_SaveOldEntry(NULL);
	vldb_TellRepAboutNewEntry(&entry, 1, 1);
    }

    /*
     * We do this under the FLDB lock to reduce the problems caused by
     * allowing fileset movement, site deletion, etc.
     */
    if (synchronous && anyRepChange) {
	code = fts_WaitForRepHostUpdates(&entry, volIdp, &stagingVV,
				  (afsNetAddr *)NULL, 0);
    }

    if (nukeLocalClone >= 0) {
	VOLSERLOG(1, "Deleting the releaseClone %lu,,%lu\n",
		  AFS_HGETBOTH(tempVolId));
	(void) fts_DeleteVolume(connp, fromaggrId, &tempVolId, 2, 0);
    }
    if (nukeStage >= 0) {
	dce_svc_printf(FTS_S_VPROCS_VPROCS_DELETE_DAMAGED_READ_ONLY_MSG,
		       AFS_HGETBOTH(pubROId));
	(void) fts_DeleteVolume(connp, toaggrId, &pubROId, 2, 0);
    }
    /*
     * All done, release the lock
     */
    tcode = vldb_ReleaseLock(volIdp, RWVOL, LOCKREL_ALL);
    /* leave out rpc_binding_free() call; connToServer caches conns. */
    /* if (connp)
	rpc_binding_free(&connp, &st); */

    return (code ? code : tcode);
}


/*
 * List all the volumes on [servAddrp, aggrId].
 * OBSOLETE? [If allFlag = 1, then all the relevant fields of the volume are
 * also returned]
 */
long VC_ListVolumes(connp, servAddrp, aggrId, allFlag, resultPtr, sizep)
    rpc_binding_handle_t connp;
    struct sockaddr *servAddrp;
    unsigned long aggrId;
    int allFlag;				/* XXXX IGNORED XXXX */
    struct ftserver_status **resultPtr;
    long *sizep;
{
    long code = 0, incookie = 0, outcookie;
    ftserver_statEntries volInfo;
    struct ftserver_status *currset;
    long curralloc = 12 + 3 * FTSERVER_MAXSTATENTRIES;
    long currused = 0;

    checkInitConns();
    currset = (ftserver_status *)malloc(curralloc * sizeof(ftserver_status));

    /*
     * Get a bunch (i.e. DEFLISTVOLS) of volumes at a time from the volume
     * server; "outcookie" indicates where we're left off last time and
     * VOL_ERR_EOF indicates we reached the last volume.
     */
    /*
     * XXX: need to extend this scheme to allow more than DEFLISTVOLS
     * entries
     */
    while (1) {
	code = vols_ListVolumes(connp, aggrId, FTSERVER_MAXSTATENTRIES,
				incookie, &outcookie, &volInfo);
	if (code && (code != VOL_ERR_EOF))
	    break;
	/* Ensure there's room for the set just returned */
	if ((currused + volInfo.ftserver_status_len) > curralloc) {
	    /* grow to 3/2 the required size, plus 5 for slop */
	    curralloc = 5 + (((currused + volInfo.ftserver_status_len) * 3)
			     >> 1);
	    /*
	     * Since we normally sort all received volumes, we need to keep them
	     * in a single block, although the following code could fail if we
	     * deal with really huge number of volumes
	     */
	    currset = (ftserver_status *) realloc(currset, curralloc *
						  sizeof(ftserver_status));
	    if (currset == NULL) {
	      /* XX only in verbose mode */
		dce_svc_printf(FTS_S_VPROCS_NO_MEMORY_FOR_FTSERVER_STATUS_MSG,
			       curralloc);
		code = ENOMEM;
		currused = 0;
		break;
	    }
	}
	/* Copy the new set into place */
	bcopy((char *)volInfo.ftserver_status_val,
	      (char *)&currset[currused],
	      volInfo.ftserver_status_len * sizeof(ftserver_status));
	currused += volInfo.ftserver_status_len;
	if ((code == VOL_ERR_EOF) || (incookie == outcookie)) {
	    code = 0;
	    break;
	}
	incookie = outcookie;
    }
    if (currset != NULL && currused != curralloc) {
	/* cut this allocated block down to only the size required */
	curralloc = currused;
	if (curralloc == 0) curralloc = 1;	/* don't allocate zero bytes */
	currset = (ftserver_status *) realloc(currset, curralloc *
					      sizeof(ftserver_status));
	if (currset == NULL) {	/* should never happen! */
	  /* XX again, only in verbose mode */
	    dce_svc_printf(FTS_S_VPROCS_STILL_NO_MEMORY_FOR_FTSERVER_STATUS_MSG,
			   curralloc);
	    code = ENOMEM;
	    currused = 0;
	}
    }
    *resultPtr = currset;
    *sizep = currused;
    return code;
}


/*
 * Get all the information about volume <volIdp> on [servAddrp, aggrId]
 */
long VC_VolumeStatus(connp, servAddrp, aggrId, volIdp, statusp)
    rpc_binding_handle_t connp;
    struct sockaddr *servAddrp;
    unsigned long aggrId;
    afs_hyper_t *volIdp;
    struct ftserver_status *statusp;
{
    checkInitConns();
    return vols_GetOneVolStatus(connp, volIdp, aggrId, statusp, 0);
}


/*
 * Synchronise vldb with the file server [servAddrp, aggrId] (if it's not -1).
 * else synchronise with all the valid aggregates on <servAggr>.
 */
long VC_SyncVldb(connp, servAddrp, aggrId)
    rpc_binding_handle_t connp;
    struct sockaddr *servAddrp;
    unsigned long aggrId;
{
    long code = 0, ccode, numAggrs;
    register long i;
    ftserver_statEntries volInfo;
    struct ftserver_aggrList *aggrListp;
    register struct ftserver_aggrList *aggrp;
    long incookie, outcookie;
    struct ftserver_aggrList oneAggr;	/* buffer for a specified aggregate */

    checkInitConns();
    if (aggrId == -1) {
	/*
	 * Get all server's aggregates in aggrListp
	 */
	if (code = fts_GetAllAggrs(connp, servAddrp, &numAggrs, &aggrListp))
	    return code;
    } else {
	/* Dummy up a one-aggregate answer */
	numAggrs = 1;
	aggrListp = &oneAggr;
	oneAggr.Id = aggrId;
	oneAggr.type = -1;		/* this isn't real */
	/* Maybe we can make it real */
	(void) fts_GetAggrType(servAddrp, oneAggr.Id, &oneAggr.type, 0);
    }
    for (i = 0, aggrp = aggrListp; i < numAggrs; ++i, ++aggrp) {
	incookie = 0;
	volInfo.ftserver_status_len = 0;
	q_StartEntryGroup();
	for (ccode = 0; ccode == 0;) {
	    /* Get the next few volume descriptors and work on them. */
	    code = vols_ListVolumes(connp, aggrp->Id, FTSERVER_MAXSTATENTRIES,
				    incookie, &outcookie, &volInfo);
	    if ((code != 0) && (code != VOL_ERR_EOF))
		break;
	    if (volInfo.ftserver_status_len != 0) {
		/*
		 * The following call adds this batch of entries to a
		 * volc_queue queue.
		 */
		ccode = q_AddToGroup(connp, servAddrp, aggrp->Id,
				     &volInfo.ftserver_status_val[0],
				     volInfo.ftserver_status_len, aggrp->type);
	    }
	    if ((code == VOL_ERR_EOF) || (incookie == outcookie)) {
		code = 0;	/* end of the filesets on this aggr. */
		break;
	    }
	    incookie = outcookie;
	  }
	if (code == 0 && ccode == 0) {
	    /* The following call handles the whole synchronization process */
	    ccode = q_ProcessGroup(connp, servAddrp, aggrp->Id,
				    &volInfo.ftserver_status_val[0],
				    volInfo.ftserver_status_len, aggrp->type);
	}
    }
    if (aggrId == -1 && aggrListp != NULL)
	free(aggrListp);
    return code;
}


/*
 * rename volume <oldname> to <newname>, changing the names of the related
 * readonly and backup volumes. This operation is also idempotent; salvager is
 * capable of recovering from rename operation crapping out halfway.
 * To recover run syncserver on affected machines, it will force renaming to
 * completion.
 */
long VC_RenameVolume(entryp, oldNamep, newNamep)
    struct vldbentry *entryp;
    char *oldNamep;			/* XXXXXX UNUSED XXXXXXXXXXXX */
    char *newNamep;
{
    long code, tcode, t2code;
    int vlChanged, diskChanged;
    int xl[MAXTYPES];
    char realOldName[FTSERVER_MAXFSNAME];

    checkInitConns();
    /*
     * First, lock RW vldb entry for the duration of the call
     */
    fts_GetIndices(entryp, xl);
    if (xl[RWVOL] < 0) return FTS_ERR_NEED_RW;
    if (NameTooLong(entryp->name))
	return EINVAL;
    if (NameTooLong(newNamep))
	return EINVAL;
    if (code = vldb_SetLock(&entryp->VolIDs[xl[RWVOL]], -1, VLOP_ADDSITE))
	return code;

    /*
     * Copy the new volume name in the vldb entry and update it in the vldb
     */
    /* save the old first */
    strncpy(realOldName, (char *)entryp->name, FTSERVER_MAXFSNAME);
    strncpy((char *)entryp->name, newNamep, FTSERVER_MAXFSNAME);
    code = vldb_ReplaceEntry(entryp, &entryp->VolIDs[xl[RWVOL]], RWVOL, 0);
    if (!code) {
	VOLSERLOG(1, "Recorded the new name %s in FLDB\n", newNamep);
	/*
	 * Rename the volume in the volume server itself
	 */
	code = RenameEntry(entryp, 0, 0, &vlChanged, &diskChanged);
	if (code) {
	    /* attempt recovery.  Put the old name back. */
	    strncpy((char *)entryp->name, realOldName, FTSERVER_MAXFSNAME);
	    tcode = vldb_ReplaceEntry(entryp, &entryp->VolIDs[xl[RWVOL]],
				      RWVOL, 0);
	    t2code = RenameEntry(entryp, 0, 0, &vlChanged, &diskChanged);
	    if (tcode) {
		dce_svc_printf(FTS_S_VPROCS_FAILED_TO_REVERT_NAME_MSG, dfs_dceErrTxt(tcode));
	    }
	    if (t2code) {
		dce_svc_printf(FTS_S_VPROCS_VPROCS_FAILED_TO_REVERT_NAMES_MSG,
			       dfs_dceErrTxt(t2code));
	    }
	}
    }
    /*
     * All done, release the lock
     */
    tcode = vldb_ReleaseLock(&entryp->VolIDs[xl[RWVOL]], -1, LOCKREL_ALL);
    return (code ? code : tcode);
}



/*
 * Ensure that <entry> matches with the info on file servers
 */
static long CheckVldb(entryp)
    struct vldbentry *entryp;
{
    long code, tcode;
    int vlChanged, diskChanged;
    register int vtype;
    struct vldbentry oldEntry;
    int xl[MAXTYPES];

    fts_GetIndices(entryp, xl);
    for (vtype = RWVOL; vtype <= BACKVOL; ++vtype)
	if (xl[vtype] >= 0) break;	/* find first used slot */
    if (xl[vtype] < 0) return FTS_ERR_NEED_RW;
    oldEntry = *entryp;
    tcode = 0;
    if (NameTooLong(entryp->name))
	return EINVAL;
    code = vldb_SetLock(&entryp->VolIDs[xl[vtype]], vtype, VLOP_DELETE);
    if (!code) {
	vlChanged = diskChanged = 0;
	code = RenameEntry(entryp, 0, 1, &vlChanged, &diskChanged);
	tcode = vldb_ReleaseLock(&entryp->VolIDs[xl[vtype]], vtype,
				 LOCKREL_ALL);
	if (code != 0 || vlChanged != 0) {
	    if (vlChanged) {
		vldb_EnumerateEntry(&oldEntry, "status before:", 0);
		vldb_EnumerateEntry(entryp, "status after:", 0);
	    } else {
		vldb_EnumerateEntry(entryp, "status:", 0);
	    }
	}
	if (code) {
	    dce_svc_printf(FTS_S_VPROCS_COULD_NOT_PROCESS_FLDB_MSG, entryp->name, code);
	} else if (tcode) {
	    dce_svc_printf(FTS_S_VPROCS_COULD_NOT_UNLOCK_FLDB_MSG, entryp->name, tcode);
	    code = tcode;
	}
    }
    return PrintError(0, code);
}


/*
 * Synchronize [servAddrp, aggrId] (if it's not -1) with the vldb, else
 * synchronize all the valid partitions on <servAddr>.
 */
long VC_SyncServer(connp, servAddrp, aggrId)
    rpc_binding_handle_t connp;
    struct sockaddr *servAddrp;
    unsigned long aggrId;
{
    long code, failures = 0, nentries, i, j, subrun, numAggrs;
    struct VldbListByAttributes attributes;
    bulkentries Entries;
    register struct vldbentry *entryp;
    struct ftserver_aggrList *aggrListp;
    register struct ftserver_aggrList *aggrp;
    u_long cookie, nextcookie, flags;
    struct ftserver_aggrList oneAggr;	/* buffer for a specified aggregate */

    checkInitConns();
    if (aggrId == -1) {
	/*
	 * Get all server's aggregates in aggrListp
	 */
	if (code = fts_GetAllAggrs(connp, servAddrp, &numAggrs, &aggrListp))
	    return code;
    } else {
	/* Dummy up a one-aggregate answer */
	numAggrs = 1;
	oneAggr.Id = aggrId;
	aggrListp = &oneAggr;
    }
    for (i = 0, aggrp = aggrListp; i < numAggrs; ++i, ++aggrp) {
	attributes.partition = aggrp->Id;
	*((struct sockaddr *) &attributes.site) = *servAddrp;
	attributes.Mask = VLLIST_SITE | VLLIST_PARTITION;
	VOLSERLOG(1, "Processing FLDB entries on aggregate Id %d...\n",
		  aggrp->Id);
	failures = 0;
	subrun = 0;
	for (cookie = flags = 0; (flags & 1) == 0; cookie = nextcookie) {
	    ++subrun;
	    bzero((char *)&Entries, sizeof(Entries));
	    if (code = vldb_ListByAttributes(&attributes, cookie, &nentries,
					     &Entries, &nextcookie, &flags)) {
		if (code == VL_ENDOFLIST) break;
		return code;
	    }
	    for (j = 0; j < nentries; ++j) {
		entryp = &Entries.bulkentries_val[j];
		VOLSERLOG(1, "Processing FLDB entry %d of total %u in \
run %u...\n", j+1, nentries, subrun);
		if (code = CheckVldb(entryp))
		    failures++;
	    }
	}
	VOLSERLOG(1, ".. done.\nTotal entries : %u; Failed to process %d\n",
		  nentries, failures);
    }
    if (aggrId == -1 && aggrListp != NULL)
	free(aggrListp);
    return 0;
}


/*
 * Report on all the active transactions on volser
 */
long VC_VolserStatus(connp, servAddrp, rstatusp)
    rpc_binding_handle_t connp;
    struct sockaddr *servAddrp;
    ftserver_transEntries *rstatusp;
{
    checkInitConns();
    rstatusp->ftserver_tranStatus_len = 0;
    /*
     * Call volume server to get all active transanctions ion  transInfo
     */
    return vols_Monitor(connp, rstatusp);
}


/*
 * Delete the volume without interacting with the vldb 
 * XXX:
 * We can do WITHOUT this redundant layer and call fts_DeleteVolume directly
 */
long VC_VolumeZap(connp, servAddrp, aggrId, volIdp)
    rpc_binding_handle_t connp;
    struct sockaddr *servAddrp;
    unsigned long aggrId;
    afs_hyper_t *volIdp;
{
    checkInitConns();
    if (CheckLocalMount(connp, servAddrp, volIdp, aggrId, "zapped") > 0) {
	return FTSERVER_ERR_LCLMOUNT;
    }
    return (fts_DeleteVolume(connp, aggrId, volIdp, 1, 0));
}

long VC_EndTrans(connp, servAddrp, transId)
    rpc_binding_handle_t connp;
    struct sockaddr *servAddrp;
    long transId;
{
    long code;

    checkInitConns();
    code = vols_DeleteTrans(connp, transId);
    return code;
}

long VC_SetQuota(connp, servAddrp, aggrId, volIdp, newQuota)
     rpc_binding_handle_t connp;
     struct sockaddr *servAddrp;
     unsigned long aggrId;
     afs_hyper_t *volIdp;
     unsigned long newQuota;
  {
      struct ftserver_status status;
      long code, code2;
      long transId;

    checkInitConns();
      AFS_hset64(status.vsd.visQuotaLimit, 0, newQuota);
      AFS_hleftshift(status.vsd.visQuotaLimit, 10); /* cvt K to bytes */
      code = vols_CreateTrans(connp, volIdp, aggrId,
			       FLAGS_ENCODE(FTSERVER_OP_SETSTATUS,
					    VOLERR_TRANS_SETQUOTA),
			       &transId, 0);
      if (!code) {
	  code = vols_SetStatus(connp, transId, VOL_STAT_VISLIMIT, &status);
	  if (code)
	      (void) vols_AbortTrans(connp, transId);
	  else
	      code = vols_DeleteTrans(connp, transId);
      }
      return code;
  }

/*
 * Static routines used by the module
 */


/*
 * Simple "dummy" Create/Delete transaction calls on the volume to determine
 * if it exists.  Return EEXIST if it seems that it exists.
 */
long fts_VolumeExists(connp, aggrId, volIdp)
    rpc_binding_handle_t connp;
    unsigned long aggrId;
    afs_hyper_t *volIdp;
{
    long transId;
    long code;

    checkInitConns();
    code = vols_CreateTrans(connp, volIdp, aggrId,
			     FLAGS_ENCODE(0, VOLERR_TRANS_FILESETEXISTS),
			     &transId, 1);
    if (!code) {
	code = vols_DeleteTrans(connp, transId);
	if (!code)
	    return EEXIST;
	return code;
    }
    if (code == ENOENT || code == ENODEV || code == FTSERVER_ERR_NOVOL) {
	code = 0;
    }
    return code;
}


/*
 * Change the volume's flags
 */
static long SetFlags(connp, aggrId, volIdp, setFlags)
    rpc_binding_handle_t connp;
    unsigned long aggrId;
    afs_hyper_t *volIdp;
    unsigned long setFlags;
{
    long transId;
    long code;

    code = vols_CreateTrans(connp, volIdp, aggrId,
			     FLAGS_ENCODE(FTSERVER_OP_SETFLAGS,
					  VOLERR_TRANS_SETFLAGS),
			     &transId, 0);
    if (!code) {
	code = vols_SetFlags(connp, transId, setFlags);
	if (code)
	    (void) vols_DeleteTrans(connp, transId);
	else
	    code = vols_DeleteTrans(connp, transId);
    }
    return code;
}

/*
 * Special routine for handling volume id change.
 */
static long SwapIntoPlace(connp, fromTransID, origIdp, tovolIdp, fromaggrId,
			  toaggrId, swapSrep, fromSrepIdp, toSrepIdp,
			  servAddrp)
     rpc_binding_handle_t connp;
     long fromTransID;
     afs_hyper_t *origIdp;
     afs_hyper_t *tovolIdp;
     unsigned long fromaggrId;
     unsigned long toaggrId;
     int swapSrep;
     afs_hyper_t *fromSrepIdp, *toSrepIdp;
     struct sockaddr *servAddrp;
{
      long code, tcode;
      long toTransID;
      struct ftserver_status ftstat;
      long fromSrepTrans;
      long toSrepTrans;
      int moveTagSet = 0;

      if (swapSrep) {
	  code = FLAGS_ENCODE(FTSERVER_OP_SWAPIDS|FTSERVER_OP_DELETE,
			      VOLERR_TRANS_MOVE);
	  code = BOMB_EXEC("swapintoplace.srep.1",
			   vols_CreateTrans(connp, fromSrepIdp, fromaggrId,
					    code, &fromSrepTrans, 0));
	  if (code)
	      return code;
	  
	  code = FLAGS_ENCODE(FTSERVER_OP_SWAPIDS, VOLERR_TRANS_MOVE);
	  code = BOMB_EXEC("swapintoplace.srep.2",
			   vols_CreateTrans(connp, toSrepIdp, toaggrId, code,
					    &toSrepTrans, 0));
	  if (code) {
	      (void) vols_AbortTrans(connp, fromSrepTrans);
	      return code;
	  }
	  
	  code = BOMB_EXEC("swapintoplace.srep.3",
			   vols_SwapIDs(connp, fromSrepTrans, toSrepTrans));
	  if (code) {
	      (void) vols_AbortTrans(connp, fromSrepTrans);
	      (void) vols_AbortTrans(connp, toSrepTrans);
	      return code;
	  }

	  BOMB_IF("swapintoplace.srep.3.5") { code = 1; }

	  if (!code) {
	      code = BOMB_EXEC("swapintoplace.srep.4",
			       vols_DeleteTrans(connp, toSrepTrans));
	  } else {
	      (void) vols_AbortTrans(connp, toSrepTrans);
	      (void) vols_AbortTrans(connp, fromSrepTrans);
	      return code;
	  }

	  if (!code)
	      code = BOMB_EXEC("swapintoplace.srep.5",
			       vols_DeleteTrans(connp, fromSrepTrans));
	  else
	      (void) vols_AbortTrans(connp, toSrepTrans);

	  if (code)
	      return code;
      }

      /*
       * First set the parentId of the source fileset to be that of the base
       * entry.
       */
      bzero((char *)&ftstat, sizeof(ftstat));
      ftstat.vss.parentId = *origIdp;
      code = BOMB_EXEC("swapintoplace#10",
		       vols_SetStatus(connp, fromTransID, VOL_STAT_PARENTID,
				      &ftstat));
      if (code)
	  return code;
      bzero((char *)&ftstat, sizeof(ftstat));
      code = FLAGS_ENCODE(FTSERVER_OP_SWAPIDS | FTSERVER_OP_SETSTATUS
			  | FTSERVER_OP_GETSTATUS,
			   VOLERR_TRANS_MOVE);
      code = BOMB_EXEC("swapintoplace#1",
		       vols_CreateTrans(connp, tovolIdp, toaggrId, code,
					&toTransID, 0));
      if (code) {
	  return code;
      }
      code = BOMB_EXEC("swapintoplace#2",
		       vols_SwapIDs(connp, fromTransID, toTransID));
      /*
       * the transactions stay connected to the same volIDs, so they actually
       * follow the swap
       */
      if (code == 0) {
	  tcode = BOMB_EXEC("swapintoplace#3",
			    vols_SetStatus(connp, fromTransID,
					   VOL_STAT_PARENTID, &ftstat));
	  code = SetMoveTags(connp, &fromTransID, servAddrp, origIdp,
			     toaggrId, MOVETAG_FETCH);
	  if (!code) {
	      moveTagSet = 1;
	      code = BOMB_EXEC("swapintoplace#4",
			       vols_DeleteVolume(connp, toTransID));
	      BOMB_IF("swapintoplace#4.5") { code = ETIMEDOUT; };
	      if (code == 0)
		  code = tcode;
	  }
      }

      if (code)
	  (void) BOMB_EXEC("swapintoplace#6",
			   vols_AbortTrans(connp, toTransID));
      else
	  code = BOMB_EXEC("swapintoplace#5",
			   vols_DeleteTrans(connp, toTransID));
      if (swapSrep && !code)
	  (void) fts_DeleteVolume(connp, fromaggrId, toSrepIdp, 2, 0);

      BOMB_POINT("swapintoplace#7");

      if (moveTagSet) {
	  tcode = ClearMoveTags(servAddrp, origIdp, toaggrId, 0, 0);
	  if (!code)
	      code = tcode;
      }

      return code;
  }


static long CSetStatus(connp, transId, volNamep, volType, parIdp, cloneIdp,
		       backupIdp, statesToSet, statesToClear)
    rpc_binding_handle_t connp;
    long transId;
    char *volNamep;
    long volType;
    afs_hyper_t *parIdp, *cloneIdp, *backupIdp;
    unsigned long statesToSet, statesToClear;
{
    long code;
    unsigned long newtype;
    unsigned long mask;
    unsigned long newstates;
    struct ftserver_status status;

    mask = 0;
    bzero((char *)&status, sizeof(status));
    if (statesToSet || statesToClear || (volType != -1)) {
	/* We'll have to get the status and possibly set it */
	switch (volType) {
	    case RWVOL:
		newtype = VOL_TYPE_RW; break;
	    case ROVOL:
		newtype = VOL_TYPE_RO; break;
	    case BACKVOL:
		newtype = VOL_TYPE_BK; break;
	    default:
		newtype = 0; break;
	}
	code = vols_GetStatus(connp, transId, &status);
	if (code != 0) return code;
	newstates = status.vss.states;
	newstates &= ~statesToClear;
	newstates |= statesToSet;
	if (status.vss.states != newstates) {
	    status.vss.states = newstates;
	    mask |= VOL_STAT_STATES;
	}
	if (newtype != 0
	    && ((status.vss.states & VOL_TYPEFIELD) != newtype)) {
	    status.vss.states &= ~VOL_TYPEFIELD;
	    status.vss.states |= newtype;
	    mask |= VOL_STAT_STATES;
	}
    }
    if (volNamep) {
	mask |= VOL_STAT_VOLNAME;
	strcpy((char *)status.vss.volName, volNamep);
    }
    if (volType != -1) {
	mask |= VOL_STAT_TYPE;
	status.vss.type = (volType == RWVOL ? VOL_RW : VOL_READONLY);
    }
    if (parIdp) {
	mask |= VOL_STAT_PARENTID;
	status.vss.parentId = *parIdp;
    }
    if (backupIdp) {
	mask |= VOL_STAT_BACKUPID;
	status.vsd.backupId = *backupIdp;
    }
    if (cloneIdp) {
	mask |= VOL_STAT_CLONEID;
	status.vsd.cloneId = *cloneIdp;
    }
    return vols_SetStatus(connp, transId, mask, &status);
}


static long LocSetStatus(connp, aggrId, volIdp, volNamep, volType, parIdp,
			 cloneIdp, backupIdp, bringOnLine)
    rpc_binding_handle_t connp;
    unsigned long aggrId;
    afs_hyper_t *volIdp;
    char *volNamep;
    long volType;
    afs_hyper_t *parIdp, *cloneIdp, *backupIdp;
    long bringOnLine;
{
    long transId, code, tcode;

    code = vols_CreateTrans(connp, volIdp, aggrId,
			    FLAGS_ENCODE(FTSERVER_OP_GETSTATUS |
					 FTSERVER_OP_SETSTATUS,
					 VOLERR_TRANS_SETSTATUS),
			     &transId, 0);
    if (!code) {
	code = CSetStatus(connp, transId, volNamep, volType, parIdp, cloneIdp,
			  backupIdp, 0,
			  (bringOnLine ?
			   (VOL_OFFLINE|VOL_OUTOFSERVICE)
			   : 0));
	if (code)
	    (void) vols_AbortTrans(connp, transId);
	else
	    code = vols_DeleteTrans(connp, transId);
    }
    return code;
}


/*
 * Delete a volume from the server
 */
long fts_DeleteVolume(connp, aggrId, volIdp, createTransFlag, servAddrp)
    rpc_binding_handle_t connp;
    unsigned long aggrId;
    afs_hyper_t *volIdp;
    long createTransFlag;
    struct sockaddr *servAddrp;
{
    long transId;
    long code;

    checkInitConns();
    if (servAddrp)
	connp = connToServer(servAddrp, NULL, SERVERKIND_FT);

    if (connp == (rpc_binding_handle_t)NULL)
	return FTS_ERR_BADCONN;
    if (createTransFlag == 0)
	VOLSERLOG(1, "Deleting fileset %lu,,%lu\n", AFS_HGETBOTH(*volIdp));
    if (code = vols_CreateTrans(connp, volIdp, aggrId,
				 FLAGS_ENCODE(FTSERVER_OP_DELETE,
					      VOLERR_PERS_DELETED),
				 &transId, createTransFlag)) {
	if (createTransFlag == 1)
	    return code;
	if (code == ENOENT || code == ENODEV || code == FTSERVER_ERR_NOVOL) {
	    if (createTransFlag != 0)
		dce_svc_printf(FTS_S_VPROCS_FTS_DELETEVOLUME_NO_EXIST_MSG,
			       AFS_HGETBOTH(*volIdp));
	    return 0;
	} else if (code == EXDEV) {
	    dce_svc_printf(FTS_S_VPROCS_FTS_DELETEVOLUME_NOT_ON_AGGR_MSG,
			   AFS_HGETBOTH(*volIdp), aggrId);
	} else {
	    dce_svc_printf(FTS_S_VPROCS_FTS_DELETEVOLUME_UNATTACHABLE_MSG,
			   AFS_HGETBOTH(*volIdp), code);
	}
	return code;
    }
    if (!code)
	code = vols_DeleteVolume(connp, transId);
    if (code)
	(void) vols_AbortTrans(connp, transId);
    else
	code = vols_DeleteTrans(connp, transId);

    return code;
}


/*
 * (Re)Clone volume specified by volIdp on the same aggregate
 */
static int CloneVolume(connp, srvAddrp, srvPrinp, aggrId, volIdp, volNamep,
		       volType, newVolIdp, recloneFlag, buFlag, nobogusFlag)
    rpc_binding_handle_t connp;
    struct sockaddr *srvAddrp;
    u_char *srvPrinp;
    unsigned long aggrId;
    afs_hyper_t *volIdp;
    char *volNamep;
    long volType;
    afs_hyper_t *newVolIdp;
    int recloneFlag, buFlag, nobogusFlag;
{
    unsigned long mask;
    long code, code2, tcode, transId;
    struct ftserver_status status;
    int tokenDesc;
    unsigned long flags = 0;

    VOLSERLOG(1, "Cloning fileset %lu,,%lu to %lu,,%lu (name %s, Reclone=%d)\n",
	       AFS_HGETBOTH(*volIdp), AFS_HGETBOTH(*newVolIdp),
	       volNamep, recloneFlag);
    /* Call in all the changes by getting read-style tokens. */
    code = BOMB_EXEC("clonevolume#1",
		     fts_GetToken((afsNetAddr *)srvAddrp, srvPrinp, volIdp,
				  TKN_READ, AFS_FLAG_FORCEVOLQUIESCE,
				  &tokenDesc));
    if (code)
	return code;
    if (recloneFlag) {
	flags = FLAGS_ENCODE(FTSERVER_OP_RECLONE | FTSERVER_OP_GETSTATUS |
		     FTSERVER_OP_SETSTATUS, VOLERR_TRANS_RECLONE);
    } else {
	flags = FLAGS_ENCODE(FTSERVER_OP_CLONE | FTSERVER_OP_GETSTATUS |
		     FTSERVER_OP_SETSTATUS, VOLERR_TRANS_CLONE);
    }

    code = BOMB_EXEC("clonevolume#2",
		     vols_CreateTrans(connp, volIdp, aggrId, flags, &transId,
				      0));
    if (!code) {
	code = BOMB_EXEC("clonevolume#3",
			 vols_GetStatus(connp, transId, &status));
	if (nobogusFlag && code == 0) {
	    status.vss.states = BOMB_EXEC("clonevolume#4", status.vss.states);
	    if ((status.vss.states & VOL_DELONSALVAGE) != 0) {
		code = FTS_ERR_INCONSISTENT;
	    }
	}
	tcode = code2 = 0;
	if (code == 0) {
	    if (recloneFlag)
		code = BOMB_EXEC("clonevolume#5",
				 vols_ReClone(connp, transId, newVolIdp));
	    else
		code = BOMB_EXEC("clonevolume#6",
				 vols_Clone(connp, transId, volType, volNamep,
					    newVolIdp));
	    if (code == 0) {
		mask = 0;
		if (buFlag && !AFS_hsame(status.vsd.backupId, *newVolIdp)) {
		    status.vsd.backupId = *newVolIdp;
		    mask = VOL_STAT_BACKUPID;
		}
		if (mask != 0) {
		    code2 = BOMB_EXEC("clonevolume#7",
				      vols_SetStatus(connp, transId, mask,
						     &status));
		}
	    }
	}
	if (code || code2)
	    (void) BOMB_EXEC("clonevolume#8", vols_AbortTrans(connp, transId));
	else
	    tcode = BOMB_EXEC("clonevolume#7",
			      vols_DeleteTrans(connp, transId));
	if (!code)
	    code = code2;
	if (!code)
	    code = tcode;
    }
    (void) BOMB_EXEC("clonevolume#10", fts_ReleaseToken(tokenDesc));
    return code;
}


/*
 * This routine does the whole copying of a cloned volume in [fconnp, faggrId]
 * to [tconnp, taggrId]
 */
static int CopyCloneVolume(fconnp, faggrId, origVolIdp, newVolIdp, newVolName,
			   cloneVolIdp, tconnp, taggrId, entryp, fromservAddrp,
			   fromPrinP, toservAddrp, toPrinP, xBits, fromTranP,
			   toTranP, tokDescP, sameServer, copySrep,
			   fromSrepAggrId, srepIdP, srepTmpIdP, srepTmpName,
			   sockFlag)
    rpc_binding_handle_t fconnp;
    unsigned long faggrId;
    afs_hyper_t *origVolIdp, *newVolIdp, *cloneVolIdp;
    char *newVolName;
    rpc_binding_handle_t tconnp;
    unsigned long taggrId;
    struct vldbentry *entryp;
    struct sockaddr *fromservAddrp;
    u_char *fromPrinP;
    struct sockaddr *toservAddrp;
    u_char *toPrinP;
    unsigned long xBits;
    long *fromTranP, *toTranP;
    int *tokDescP;
    int sameServer;
    int copySrep;
    unsigned long fromSrepAggrId;
    afs_hyper_t *srepIdP, *srepTmpIdP;
    char *srepTmpName;
    int sockFlag;
{
    long code, code2;
    long cloneFromTid;
    struct ftserver_dest dest;
    struct ftserver_status ftstatus;
    struct ftserver_Date dumpDate;
    int xl[MAXTYPES];
    int tokenDesc;
    unsigned long mask;
    long fromSrepTid;
    long toSrepTid;
    long dumpFromTid;
    afs_hyper_t *dumpFromVolIdp;
    struct ftserver_Date srepDate;
    

#if defined(OSF_NO_SOCKET_DUMP)
    assert(sockFlag == 0);
#endif
    /* Invalidate the reference (OUT) parameters to start with. */
    *fromTranP = -1;
    *toTranP = -1;
    *tokDescP = -1;
    cloneFromTid = -1;
    fromSrepTid = -1;
    toSrepTid = -1;

    if (copySrep) {
	code = FLAGS_ENCODE(FTSERVER_OP_DUMP | FTSERVER_OP_GETSTATUS,
			    VOLERR_TRANS_COPYCLONE);
	code = vols_CreateTrans(fconnp, srepIdP, fromSrepAggrId, code,
				&fromSrepTid, 0);
	if (code) {
	    fromSrepTid = -1;
	    goto copycloneBad;
	}

	code = BOMB_EXEC("copyclone.srep.1",
			 vols_GetStatus(fconnp, fromSrepTid, &ftstatus));
	if (code)
	    goto copycloneBad;

	srepDate.fromVersion = ftstatus.vsd.volversion;
    }

    code = FLAGS_ENCODE(FTSERVER_OP_DUMP
			 | FTSERVER_OP_DELETE
			 | FTSERVER_OP_GETSTATUS
			 | FTSERVER_OP_SETSTATUS,
			 VOLERR_TRANS_COPYCLONE);
    if (code = BOMB_EXEC("copyclone#1",
			 vols_CreateTrans(fconnp, cloneVolIdp, faggrId, code,
					  &cloneFromTid, 0))) {
	cloneFromTid = -1;
	goto copycloneBad;
    }
    code = BOMB_EXEC("copyclone#2",
		     vols_GetStatus(fconnp, cloneFromTid, &ftstatus));
    if (code)
	goto copycloneBad;
    /* save this version for the incremental dump. */
    dumpDate.fromVersion = ftstatus.vsd.volversion;

    /*
     * Create a volume on the target machine
     * First, make sure it doesn't already exist there.
     */
    code = BOMB_EXEC("copyclone#3",
		     fts_DeleteVolume(tconnp, taggrId, newVolIdp, 1, 0));
    if ((code != 0) && (code != FTSERVER_ERR_NOVOL) && (code != ENOENT) &&
	(code != ENODEV)) {
	goto copycloneBad;
    }
    VOLSERLOG(1, "Creating dest fileset %lu,,%lu...",
	      AFS_HGETBOTH(*newVolIdp));
    if (sameServer) {
	/* create the fileset off-line once and for all */
	code = FLAGS_ENCODE(FTSERVER_OP_RESTORE
			    | FTSERVER_OP_SETSTATUS
			    | FTSERVER_OP_SETFLAGS
			    | FTSERVER_OP_GETSTATUS,
			    VOLERR_TRANS_COPYCLONE);
    } else {
	/* create it, but prepare to sweep out leftover token state */
	code = FLAGS_ENCODE(FTSERVER_OP_SETSTATUS,
			    VOLERR_TRANS_COPYCLONE);
    }
    if (code = BOMB_EXEC("copyclone#4",
			 vols_CreateVolume(tconnp, taggrId, newVolName,
					   VOLTIX_TO_VOLTYPE(RWVOL), code,
					   origVolIdp, newVolIdp, toTranP))) {
	lock_ObtainWrite(&transKALock);
	*toTranP = -1;	/* ensure that the caller doesn't use it */
	lock_ReleaseWrite(&transKALock);
	goto copycloneBad;
    }
    VOLSERLOG(1, "done\n");
    if (sameServer == 0) {
	/*
	 * Sweep out any left-over token state for prior instances of the same
	 * volume.
	 * Need to make the fileset un-busy to flush these tokens.
	 * Also, need to sweep tokens clean before doing the restore, since
	 * doing the restore will invoke ZLC's token management.
	 */
	ftstatus.vss.states = xBits | (VOL_TYPE_TEMP | VOL_RW);
	code = BOMB_EXEC("copyclone#5", vols_SetStatus(tconnp, *toTranP,
				VOL_STAT_STATES, &ftstatus));
	if (code)
	    goto copycloneBad;
	lock_ObtainWrite(&transKALock);
	code = BOMB_EXEC("copyclone#5a", vols_DeleteTrans(tconnp, *toTranP));
	*toTranP = -1;
	lock_ReleaseWrite(&transKALock);
	if (code)
	    goto copycloneBad;
	code = TKN_OPEN_DELETE
	  | TKN_LOCK_READ | TKN_LOCK_WRITE
	  | TKN_DATA_READ | TKN_DATA_WRITE
	  | TKN_STATUS_READ | TKN_STATUS_WRITE;
	VOLSERLOG(1, "Sweeping tokens by getting token %#lx for dest \
fileset %lu,,%lu...\n",
		  code, AFS_HGETBOTH(*newVolIdp));
	code = BOMB_EXEC("copyclone#5b",
			 fts_GetToken((afsNetAddr *)toservAddrp, toPrinP,
				      newVolIdp, code, AFS_FLAG_FORCEVOLQUIESCE
				      | AFS_FLAG_FORCEREVOCATIONS, &tokenDesc));
	if (code) {
	    dce_svc_printf(FTS_S_VPROCS_FAILED_ON_FRESH_MOVE_MSG,
			   dfs_dceErrTxt(code));
	    (void) BOMB_EXEC("copyclone#5c",
			     fts_DeleteVolume(tconnp, taggrId, newVolIdp, 1,
					      0));
	    goto copycloneBad;
	}
	(void) BOMB_EXEC("copyclone#5d", fts_ReleaseToken(tokenDesc));
	code = FLAGS_ENCODE(FTSERVER_OP_RESTORE
			    | FTSERVER_OP_SETSTATUS
			    | FTSERVER_OP_SETFLAGS
			    | FTSERVER_OP_GETSTATUS,
			    VOLERR_TRANS_COPYCLONE);
	code = BOMB_EXEC("copyclone#5e", vols_CreateTrans(tconnp, newVolIdp,
					taggrId, code, toTranP, 0));
	if (code) {
	    lock_ObtainWrite(&transKALock);
	    *toTranP = -1;	/* ensure that the caller doesn't use it */
	    lock_ReleaseWrite(&transKALock);
	    (void) BOMB_EXEC("copyclone#5f",
			     fts_DeleteVolume(tconnp, taggrId, newVolIdp, 1,
					      0));
	    goto copycloneBad;
	}
    }
    /* Set attributes of this new volume */
    fts_GetIndices(entryp, xl);
    ftstatus.vss.states = xBits | (VOL_TYPE_RW | VOL_ZAPME | VOL_RW);
    /*
     * Set parent ID to be either the original (for surrogate ZLC tokens) or
     * irrelevant.
     */
    if (AFS_hsame(*origVolIdp, *newVolIdp)) {
	AFS_hzero(ftstatus.vss.parentId);
    } else {
	ftstatus.vss.parentId = *origVolIdp;
    }
    mask = VOL_STAT_STATES | VOL_STAT_PARENTID;
    if (xl[BACKVOL] >= 0) {
	ftstatus.vsd.backupId = entryp->VolIDs[xl[BACKVOL]];
	mask |= VOL_STAT_BACKUPID;
    }
    if (xl[ROVOL] >= 0) {
	ftstatus.vsd.cloneId = entryp->VolIDs[xl[ROVOL]];
	mask |= VOL_STAT_CLONEID;
    } 
    code = BOMB_EXEC("copyclone#5",
		     vols_SetStatus(tconnp, *toTranP, mask, &ftstatus));
    if (code)
	goto copycloneBad;
    if (BOMB_EXEC("copyclone#6", (sameServer == 0))) {
	/* do this only if we're moving from one server to another */
	code = BOMB_EXEC("copyclone#7",
			 SetMoveTags(tconnp, toTranP, toservAddrp, newVolIdp,
				     taggrId, MOVETAG_TARGET));
	if (code)
	    goto copycloneBad;
    } else {
	/*
	 * for same-server move, set up a transaction to provide keep-alive
	 * noise
	 */
	code = BOMB_EXEC("copyclone#7a",
			 SetMoveTags(tconnp, toTranP, toservAddrp, newVolIdp,
				     taggrId, MOVETAG_FETCH));
	if (code)
	    goto copycloneBad;
    }

    SetupDest(&dest, toservAddrp, toPrinP);

    if (copySrep) {
	dumpFromTid = fromSrepTid;
	dumpFromVolIdp = srepIdP;

	code = SetMoveTags(fconnp, &cloneFromTid, fromservAddrp, cloneVolIdp,
			   faggrId, MOVETAG_FETCH);
	if (code)
	    goto copycloneBad;

	VOLSERLOG(1, "Dumping from staging replica %lu,,%lu (src) to \
fileset %lu,,%lu (dest)...\n",
		  AFS_HGETBOTH(*srepIdP), AFS_HGETBOTH(*newVolIdp));
    } else {
	VOLSERLOG(1,
		  "Dumping from clone %lu,,%lu (src) to fileset %lu,,%lu \
(dest)...\n",
		  AFS_HGETBOTH(*cloneVolIdp), AFS_HGETBOTH(*newVolIdp));
	dumpFromTid = cloneFromTid;
	dumpFromVolIdp = cloneVolIdp;
    }

    /*
     * We need to setup keep-alive messages for the source.  It's possible
     * for the restore to get arbitrarily far behind the dump.  When this
     * happens the source descriptor is in danger of getting GC'ed, which will
     * be fatal when we try to delete the clone.
     */
    code = SetMoveTags(fconnp, &dumpFromTid, fromservAddrp, dumpFromVolIdp,
		       faggrId, MOVETAG_FETCH);
    if (code)
	goto copycloneBad;

    dumpDate.mask = 0;	/* the whole thing */
    code = BOMB_EXEC("copyclone#8",
		     vols_Forward(fconnp, tconnp, dumpFromTid, &dumpDate, &dest,
				  *toTranP, sockFlag));
    if (code == FTU_E_RESTORED_INCONSISTENT_FSET)
	VOLSERLOG(1, " ...dump done (dest is inconsistent)\n");
    else if (code != 0) {
	VOLSERLOG(1, " *** dump failed (%s)\n", dfs_dceErrTxt(code));
	goto copycloneBad;
    } else
	VOLSERLOG(1, " ...dump done\n");

    if (copySrep) {
	code = BOMB_EXEC("copyclone.srep.2",
			 vols_DeleteTrans(fconnp, fromSrepTid));
	fromSrepTid = -1;
	if (code)
	    goto copycloneBad;

	BOMB_POINT("copyclone.srep.2.5");

	code = fts_DeleteVolume(tconnp, taggrId, srepTmpIdP, 1, 0);
	if ((code != 0) && (code != FTSERVER_ERR_NOVOL)
	    && (code != ENOENT) && (code != ENODEV)) {
	    goto copycloneBad;
	}

	/*
	 * We need to close the destination volume to signal the system
	 * that the restore has completed successfully.  This will
	 * unset the inconsistent (VOL_DELONSALVAGE) bit.  If we didn't
	 * do this, and just went ahead with the clone, the staging
	 * replica would be marked inconsistent.
	 */
	lock_ObtainWrite(&transKALock);
	code = BOMB_EXEC("copyclone.srep.3",
			 vols_DeleteTrans(tconnp, *toTranP));
	*toTranP = -1;
	if (code) {
	    lock_ReleaseWrite(&transKALock);
	    goto copycloneBad;
	}

	code = FLAGS_ENCODE(FTSERVER_OP_CLONE
			    | FTSERVER_OP_SETSTATUS
			    | FTSERVER_OP_SETFLAGS
			    | FTSERVER_OP_GETSTATUS,
			    VOLERR_TRANS_COPYCLONE);
	code = vols_CreateTrans(tconnp, newVolIdp, taggrId, code, toTranP, 0);
	if (code) {
	    *toTranP = -1;
	    lock_ReleaseWrite(&transKALock);
	    goto copycloneBad;
	}
	lock_ReleaseWrite(&transKALock);

	VOLSERLOG(1, "Cloning %lu,,%lu into %lu,,%lu on dest to recreate the \
staging replica...\n",
		  AFS_HGETBOTH(*newVolIdp), AFS_HGETBOTH(*srepTmpIdP));

	code = BOMB_EXEC("copyclone.srep.4",
			 vols_Clone(tconnp, *toTranP, VOLTIX_TO_VOLTYPE(ROVOL),
				    srepTmpName, srepTmpIdP));
	if (code)
	    goto copycloneBad;

	VOLSERLOG(1, "Setting parent id (%lu,,%lu) on new staging replica\n",
		  AFS_HGETBOTH(*origVolIdp));

	code = FLAGS_ENCODE(FTSERVER_OP_SETSTATUS, VOLERR_TRANS_COPYCLONE);
	code = BOMB_EXEC("copyclone.srep.5",
			 vols_CreateTrans(tconnp, srepTmpIdP, taggrId, code,
					  &toSrepTid, 0));
	if (code) {
	    toSrepTid = -1;
	    goto copycloneBad;
	}

	ftstatus.vss.parentId = *origVolIdp;
	mask = VOL_STAT_PARENTID;
	
	code = BOMB_EXEC("copyclone.srep.6",
			 vols_SetStatus(tconnp, toSrepTid, mask, &ftstatus));
	if (code)
	    goto copycloneBad;

	code = BOMB_EXEC("copyclone.srep.7",
			 vols_DeleteTrans(tconnp, toSrepTid));
	toSrepTid = -1;
	if (code)
	    goto copycloneBad;

	BOMB_POINT("copyclone.srep.7.5");

	/*
	 * The ID swap that concluded the clone operation will have
	 * transferred our open rights to the target of the clone, which
	 * we don't have a descriptor for.  We need to close and reopen
	 * the destination in order to reestablish the correct set of
	 * rights.
	 */
	lock_ObtainWrite(&transKALock);
	code = BOMB_EXEC("copyclone.srep.8",
			 vols_DeleteTrans(tconnp, *toTranP));
	*toTranP = -1;
	if (code) {
	    lock_ReleaseWrite(&transKALock);
	    goto copycloneBad;
	}

	code = FLAGS_ENCODE(FTSERVER_OP_RESTORE
			    | FTSERVER_OP_SETSTATUS
			    | FTSERVER_OP_SETFLAGS
			    | FTSERVER_OP_GETSTATUS,
			    VOLERR_TRANS_COPYCLONE);
	code = BOMB_EXEC("copyclone.srep.9",
			 vols_CreateTrans(tconnp, newVolIdp, taggrId, code,
					  toTranP, 0));
	if (code) {
	    *toTranP = -1;
	    lock_ReleaseWrite(&transKALock);
	    goto copycloneBad;
	}
	lock_ReleaseWrite(&transKALock);

	VOLSERLOG(1, "Dumping from clone %lu,,%lu (src) (incr from \
VV %lu,,%lu) to fileset %lu,,%lu (dest)...\n",
		  AFS_HGETBOTH(*cloneVolIdp),
		  AFS_HGETBOTH(srepDate.fromVersion),
		  AFS_HGETBOTH(*newVolIdp));
	srepDate.mask = 4;	/* from where the staging replica left off */
	code = BOMB_EXEC("copyclone.srep.10",
			 vols_Forward(fconnp, tconnp, cloneFromTid, &srepDate,
			 &dest, *toTranP, sockFlag));
	if (code == FTU_E_RESTORED_INCONSISTENT_FSET)
	    VOLSERLOG(1, " ...dump done (dest is inconsistent)\n");
	else if (code != 0) {
	    VOLSERLOG(1, " *** dump failed (%s)\n", dfs_dceErrTxt(code));
	    goto copycloneBad;
	} else
	    VOLSERLOG(1, " ...dump done\n");
    }

    /*
     * Delete the clone, reattach to the main volume, and incrementally dump it
     */
    VOLSERLOG(1, "Deleting the clone %lu,,%lu...\n",
	      AFS_HGETBOTH(*cloneVolIdp));
    if (code2 = BOMB_EXEC("copyclone#9",
			  vols_DeleteVolume(fconnp, cloneFromTid))) {
	code = code2;
	VOLSERLOG(1, " *** deletion failed (%s)\n", dfs_dceErrTxt(code));
	goto copycloneBad;
    }
    VOLSERLOG(1, " ...deletion done\n");
    
    code2 = code;
    if (code == FTSERVER_ERR_DISJOINT_DATE
	 || code == FTSERVER_ERR_DISJOINT_VV
	 || code == FTSERVER_ERR_DISJOINT2_DATE
	 || code == FTSERVER_ERR_DISJOINT2_VV)
	code2 = 0;
    if (copySrep)
	(void) ClearMoveTags(fromservAddrp, cloneVolIdp, faggrId, 1, 0);
    (void) ClearMoveTags(fromservAddrp, dumpFromVolIdp, faggrId, 1, 0);
    if (code2) {
	code2 = BOMB_EXEC("copyclone#10",
			  vols_AbortTrans(fconnp, cloneFromTid));
	if (code == FTU_E_RESTORED_INCONSISTENT_FSET)
	    code = code2;
    } else
	code = BOMB_EXEC("copyclone#11",
			 vols_DeleteTrans(fconnp, cloneFromTid));
    cloneFromTid = -1;
    if (code)
	goto copycloneBad;

    /* Try TKN_READ rather than TKN_UPDATE here. */
    code = BOMB_EXEC("copyclone#12",
		     fts_GetToken((afsNetAddr *)fromservAddrp, fromPrinP,
				  origVolIdp, TKN_READ,
				  AFS_FLAG_FORCEVOLQUIESCE, tokDescP));
    if (code)
	goto copycloneBad;

    /*
     * Need to shut off vnode ops for setting VOL_MOVE_SOURCE to eliminate
     * races!
     */
    code = FLAGS_ENCODE(FTSERVER_OP_DUMP
			 | FTSERVER_OP_SETFLAGS
			 | FTSERVER_OP_GETSTATUS
			 | FTSERVER_OP_SETSTATUS
			 | FTSERVER_OP_NOACCESS
			 | FTSERVER_OP_SWAPIDS
			 | FTSERVER_OP_DELETE,
			 VOLERR_TRANS_MOVE);
    if (code = BOMB_EXEC("copyclone#13",
			 vols_CreateTrans(fconnp, origVolIdp, faggrId, code,
					  fromTranP, 0))) {
	* fromTranP = -1;
	goto copycloneBad;
    }
    VOLSERLOG(1, "Dumping from original %lu,,%lu (src) (incr from VV %lu,,%lu) \
to fileset %lu,,%lu (dest)...\n",
	       AFS_HGETBOTH(*origVolIdp),
	       AFS_HGETBOTH(dumpDate.fromVersion),
	       AFS_HGETBOTH(*newVolIdp));
    dumpDate.mask = 4;		/* fromVersion is assigned already */
    code = BOMB_EXEC("copyclone#14",
		     vols_Forward(fconnp, tconnp, *fromTranP, &dumpDate, &dest,
				  *toTranP, sockFlag));
    if (code == 0) {
	VOLSERLOG(1, " ...dump done\n");
    } else if (code == FTU_E_RESTORED_INCONSISTENT_FSET) {
	VOLSERLOG(1, " ...dump done (dest is inconsistent)\n");
    } else {
	VOLSERLOG(1, " *** dump failed: %s\n", dfs_dceErrTxt(code));
    }

    if (code != 0 && code != FTU_E_RESTORED_INCONSISTENT_FSET)
	goto copycloneBad;
    /* Done with what this procedure does */
    return (code);

copycloneBad:
    if (fromSrepTid >= 0) {
	if (code)
	    (void) vols_AbortTrans(fconnp, fromSrepTid);
	else
	    code = vols_DeleteTrans(fconnp, fromSrepTid);
	fromSrepTid = -1;
    }
    if (toSrepTid >= 0) {
	if (code)
	    (void) vols_AbortTrans(tconnp, toSrepTid);
	else
	    code = vols_DeleteTrans(tconnp, toSrepTid);
	toSrepTid = -1;
    }
    if (cloneFromTid >= 0) {
	if (code)
	    (void) BOMB_EXEC("copyclone#15",
			     vols_AbortTrans(fconnp, cloneFromTid));
	else
	    code = BOMB_EXEC("copyclone#16",
			     vols_DeleteTrans(fconnp, cloneFromTid));
	cloneFromTid = -1;
    }
    if (*fromTranP >= 0) {	
	lock_ObtainWrite(&transKALock);
	if (code)
	    (void) BOMB_EXEC("copyclone#17",
			     vols_AbortTrans(fconnp, *fromTranP));
	else
	    code = BOMB_EXEC("copyclone#18",
			     vols_DeleteTrans(fconnp, *fromTranP));
	*fromTranP = -1;
	lock_ReleaseWrite(&transKALock);
    }
    if (*toTranP >= 0) {	
	lock_ObtainWrite(&transKALock);
	if (code)
	    (void) BOMB_EXEC("copyclone#19", vols_AbortTrans(tconnp, *toTranP));
	else
	    code = BOMB_EXEC("copyclone#20",
			     vols_DeleteTrans(tconnp, *toTranP));
	*toTranP = -1;
	lock_ReleaseWrite(&transKALock);
    }
    if (*tokDescP >= 0) {
	(void) BOMB_EXEC("copyclone#21", fts_ReleaseToken(*tokDescP));
	*tokDescP = -1;
    }
    return (code);
}


/*
 * This routine does the whole copying of a readonly volume
 * from [fconnp, faggrId] to [tconnp, taggrId]
 */
static int ForwardVolume(entryp, fconnp, faggrId, fvolIdp, volIdp, newvolIdp,
			 dumpDatep, tconnp, toservAddrp, totidP, toPrinP,
			 toaggrId, sockFlag)
    struct vldbentry *entryp;
    rpc_binding_handle_t fconnp;
    unsigned long faggrId;
    afs_hyper_t *fvolIdp, *volIdp, *newvolIdp;
    struct ftserver_Date *dumpDatep;
    rpc_binding_handle_t tconnp;
    struct sockaddr *toservAddrp;
    long *totidP;
    u_char *toPrinP;
    unsigned long toaggrId;
    int sockFlag;
{
    long fromtid;
    long code, tcode;
    struct ftserver_dest dest;
    char volName[FTSERVER_MAXFSNAME];
    int xl[MAXTYPES];
    struct ftserver_status ftstatus;
    afs_hyper_t roId, bkId;


#if defined(OSF_NO_SOCKET_DUMP)
    assert(sockFlag == 0);
#endif
    strcpy(volName, (char *)entryp->name), strcat(volName, ".readonly");
    fts_GetIndices(entryp, xl);
    if (xl[RWVOL] < 0 || AFS_hiszero(entryp->VolIDs[xl[RWVOL]])) {
	dce_svc_printf(FTS_S_VPROCS_CANNOT_DUMP_MSG, AFS_HGETBOTH(*volIdp));
	return FTS_ERR_NEED_RW;
    }
    if (xl[ROVOL] < 0 || AFS_hiszero(entryp->VolIDs[xl[ROVOL]])) {
	dce_svc_printf(FTS_S_VPROCS_RO_IDENTIFIER_MISSING_MSG,
		       AFS_HGETBOTH(*volIdp));
	return FTS_ERR_NEED_RO;
    }
    roId = entryp->VolIDs[xl[ROVOL]];
    if (xl[BACKVOL] >= 0 && !AFS_hiszero(entryp->VolIDs[xl[BACKVOL]]))
	bkId = entryp->VolIDs[xl[BACKVOL]];
    else
	AFS_hzero(bkId);
    if (code = vols_CreateTrans(fconnp, volIdp, faggrId,
				FLAGS_ENCODE(FTSERVER_OP_GETSTATUS |
					     FTSERVER_OP_DUMP,
					     VOLERR_TRANS_FORWARD),
				&fromtid, 0)) {
	return code;
    }
    code = vols_GetStatus(fconnp, fromtid, &ftstatus);
    if (code == 0 && ((ftstatus.vss.states & VOL_DELONSALVAGE) != 0)) {
	dce_svc_printf(FTS_S_VPROCS_SOURCE_INCONSISTENT_MSG, volName,
		       AFS_HGETBOTH(*volIdp));
	code = FTS_ERR_INCONSISTENT;
    }
    if (code) {
	(void) vols_AbortTrans(fconnp, fromtid);
	return code;
    }
    /* Start up a periodic keep-alive function too. */
    code = SetMoveTags(tconnp, totidP, toservAddrp,
			newvolIdp, toaggrId, MOVETAG_FETCH);
    if (code) {
	VOLSERLOG(1, "Could not set up keep-alive function \
on %lu,,%lu/%lu: %s\n",
		  AFS_HGETBOTH(*newvolIdp), toaggrId,
		  dfs_dceErrTxt(code));
	goto forward;
    }
    SetupDest(&dest, toservAddrp, toPrinP);
    VOLSERLOG(1, "Starting the dump from %lu,,%lu to %lu,,%lu...\n",
	      AFS_HGETBOTH(*volIdp),
	      AFS_HGETBOTH(*newvolIdp));
    if (code = vols_Forward(fconnp, tconnp, fromtid, dumpDatep,
			    &dest, *totidP, sockFlag)) {
	VOLSERLOG(1, " *** dump failed (%s)\n", dfs_dceErrTxt(code));
	(void) ClearMoveTags(toservAddrp, newvolIdp, toaggrId, 1, 0);
	goto forward;
    }
    VOLSERLOG(1, " ...done\n");
    /* Use this call to mark the destination as release-replicated */
    if (code = CSetStatus(tconnp, *totidP, volName, ROVOL,
			   &entryp->VolIDs[xl[RWVOL]], &roId,
			  (AFS_hiszero(bkId) ? 0 : &bkId),
			  /* set: */ VOL_REP_RELEASE,
			  /* clear: */ (VOL_OFFLINE | VOL_OUTOFSERVICE |
					VOL_REPFIELD))) {
	(void) ClearMoveTags(toservAddrp, newvolIdp, toaggrId, 1, 0);
	goto forward;
    }
    tcode = vols_DeleteTrans(fconnp, fromtid);
    fromtid = -1;
    if (!code) 
	code = tcode;
    if (code) {
	(void) ClearMoveTags(toservAddrp, newvolIdp, toaggrId, 1, 0);
	goto forward;
    }
    entryp->flags |= VLF_ROEXISTS;
    code = vldb_ReplaceEntry(entryp, fvolIdp, RWVOL, LOCKREL_ALL);
    tcode = ClearMoveTags(toservAddrp, newvolIdp, toaggrId, 1, 0);
    if (tcode) {
	dce_svc_printf(FTS_S_VPROCS_CANT_CLEAR_KEEPALIVE_MSG, dfs_dceErrTxt(tcode));
    }
    if (code == 0) code = tcode;
    return code;
forward:
    if (fromtid >= 0) (void) vols_AbortTrans(fconnp, fromtid);
    if (code)
	dce_svc_printf(FTS_S_VPROCS_CANT_FORWARD_FILESET_MSG, dfs_dceErrTxt(code));
    return (code);
}


/*
 * Performs the actual renaming of a volume
 * If a fileset exists and OnLine is true, brings it on-line.
 * If a fileset doesn't exist and fixFlag is true, it deletes the entry from
 * the fldb.
 */
static long RenameEntry(entryp, fixFlag, OnLine, vlChangedP, diskChangedP)
    struct vldbentry *entryp;
    long fixFlag;
    long OnLine;
    int *vlChangedP;
    int *diskChangedP;
{
    long i, code;
    rpc_binding_handle_t connp;
    char volName[FTSERVER_MAXFSNAME];
    int xl[MAXTYPES];
    afs_hyper_t *rwp, *rop, *bup;

    fts_GetIndices(entryp, xl);
    if (xl[RWVOL] < 0) {
	rwp = &osi_hZero;
    } else {
	rwp = &entryp->VolIDs[xl[RWVOL]];
    }
    if (xl[ROVOL] < 0) {
	rop = &osi_hZero;
    } else {
	rop = &entryp->VolIDs[xl[ROVOL]];
    }
    if (xl[BACKVOL] < 0) {
	bup = &osi_hZero;
    } else {
	bup = &entryp->VolIDs[xl[BACKVOL]];
    }
    vldb_SaveOldEntry(entryp);
    code = 0;
    if (entryp->flags & VLF_RWEXISTS) {		/* process the rw volume */
	if (xl[RWVOL] < 0) {
	    dce_svc_printf(FTS_S_VPROCS_FLDB_RW_ID_MISSING_MSG);
	    return (FTS_ERR_NEED_RW);
	}
	if ((i = vldb_FindRepSite(entryp, 0, -1,
				  ((unsigned32)VLSF_ZEROIXHERE) >> xl[RWVOL]))
	    == -1) {
	    dce_svc_printf(FTS_S_VPROCS_FLDB_RW_NOT_IN_SITES_MSG);
	    return (FTS_ERR_VLDB);
	}
	connp = connToServer(((struct sockaddr *) &entryp->siteAddr[i]),
			     (u_char *)entryp->sitePrincipal[i].text,
			     SERVERKIND_FT);
	if (connp == (rpc_binding_handle_t)NULL)
	    return (FTS_ERR_BADCONN);
	code = LocSetStatus(connp, entryp->sitePartition[i], rwp,
			    entryp->name, RWVOL, &osi_hZero, rop, bup, OnLine);
	if (code == 0) *diskChangedP = 1;
	/* rpc_binding_free(&connp, &st); *//* leave out--connToServer caches */
	if (fixFlag && (code == ENOENT || code == ENODEV ||
			code == FTSERVER_ERR_NOVOL)) {
	    /*
	     * Assume volume doesn't exist; update the VLDB to reflect that
	     */
	    code = vldb_RemoveVolume(entryp,
				     ((struct sockaddr *) &entryp->siteAddr[i]),
				     entryp->sitePartition[i], RWVOL, rwp);
	    if (code == 0) *vlChangedP = 1;
	}
    }
    if (entryp->flags & VLF_BACKEXISTS) {	/* process the backup volume */
	if (xl[BACKVOL] < 0 || AFS_hiszero(entryp->VolIDs[xl[BACKVOL]])) {
	    dce_svc_printf(FTS_S_VPROCS_BACK_ID_MISSING_MSG);
	    return (FTS_ERR_NEED_BK);
	}
	if ((i = vldb_FindRepSite(entryp, (struct sockaddr *)0, -1,
				  ((unsigned32)VLSF_ZEROIXHERE) >> xl[BACKVOL])
	     == -1)) {
	    dce_svc_printf(FTS_S_VPROCS_BACK_BIT_NOT_FOUND_MSG);
	    return (FTS_ERR_VLDB);
	}
	connp = connToServer(((struct sockaddr *) &entryp->siteAddr[i]),
			     (u_char *)entryp->sitePrincipal[i].text,
			     SERVERKIND_FT);
	if (connp == (rpc_binding_handle_t)NULL)
	    return (FTS_ERR_BADCONN);
	strcpy(volName, (char *)entryp->name); strcat(volName, ".backup");
	code = LocSetStatus(connp, entryp->sitePartition[i], bup,  volName,
			    BACKVOL, rwp, rop, bup, OnLine);
	if (code == 0) *diskChangedP = 1;
	/* rpc_binding_free(&connp, &st); *//* leave out--connToServer caches */
	if (fixFlag && (code == ENOENT || code == ENODEV ||
			code == FTSERVER_ERR_NOVOL)) {
	    /*
	     * Assume volume doesn't exist; update the VLDB to reflect that
	     */
	    code = vldb_RemoveVolume(entryp, &entryp->siteAddr[i],
				     entryp->sitePartition[i], BACKVOL, rwp);
	    if (code == 0) *vlChangedP = 1;
	}
    }
    if (entryp->flags & VLF_ROEXISTS) {	/* process the ro volumes */
	if (xl[ROVOL] < 0 || AFS_hiszero(entryp->VolIDs[xl[ROVOL]])) {
	    dce_svc_printf(FTS_S_VPROCS_FLDB_NO_RO_ID_MSG);
	    return (FTS_ERR_NEED_RO);
	}
	for (i = 0; i < entryp->nServers; i++) {
	    if (entryp->siteFlags[i] &
		(((unsigned32)VLSF_ZEROIXHERE) >> xl[ROVOL])) {
		if (entryp->siteFlags[i] & VLSF_SAMEASPREV) continue;
		connp = connToServer(((struct sockaddr *) &entryp->siteAddr[i]),
				     (u_char *)entryp->sitePrincipal[i].text,
				     SERVERKIND_FT);
		if (connp == (rpc_binding_handle_t)NULL)
		    continue; /* go on to other servers? */
		strcpy(volName, (char *)entryp->name);
		strcat(volName, ".readonly");
		code = LocSetStatus(connp, entryp->sitePartition[i], rop,
				    volName, ROVOL, rwp, rop, bup, OnLine);
		if (code == 0) *diskChangedP = 1;
		/* rpc_binding_free(&connp, &st); *//* leave out */
		if (fixFlag && (code == ENOENT || code == ENODEV ||
				code == FTSERVER_ERR_NOVOL)) {
		    /*
		     * Assume volume doesn't exist; update the VLDB to reflect
		     * that.
		     */
		    code = vldb_RemoveVolume(entryp, &entryp->siteAddr[i],
					     entryp->sitePartition[i], ROVOL,
					     rwp);
		    if (code == 0) *vlChangedP = 1;
		}
	    }
	}
    }
    vldb_TellRepAboutNewEntry(entryp, 0, 1);
    return code;
}

static long CheckLocalMount(connp, servAddrp, vidp, aggid, whatp)
rpc_binding_handle_t connp;
struct sockaddr *servAddrp;
afs_hyper_t *vidp;
unsigned long aggid;
char *whatp;
{
/*
 * Check whether the indicated fileset is locally mounted.  Return 1 if so,
 * 0 if not, -1 if can't tell.
 */
    struct ftserver_status statblk;

    if (vols_GetOneVolStatus(connp, vidp, aggid, &statblk, 0) != 0)
	return -1;
    if ((statblk.vss.states & VOL_LCLMOUNT) == 0)
	return 0;
    dce_svc_printf(FTS_S_VPROCS_FILESET_ON_AGGREGATE_MSG,
		   AFS_HGETBOTH(*vidp),
		   getAggrName(servAddrp, aggid, 0), MapSockAddr(servAddrp));
    dce_svc_printf(FTS_S_VPROCS_LOCALLY_MOUNTED_MSG, whatp);
    return 1;
}


static long FinishMoveTags(entryp, toconn, toaddrP, toAgId, volIdp)
struct vldbentry *entryp;
rpc_binding_handle_t toconn;
struct sockaddr *toaddrP;
unsigned long toAgId;
afs_hyper_t *volIdp;
{
    struct ftserver_status ftstat;
    long code, code2;
    long trans;
    unsigned long mask;

    code = FLAGS_ENCODE(FTSERVER_OP_GETSTATUS | FTSERVER_OP_SETSTATUS,
			 VOLERR_TRANS_MOVE);
    code = BOMB_EXEC("finishmovetags#1",
		     vols_CreateTrans(toconn, volIdp, toAgId, code, &trans, 0));
    if (code == 0) {
	code = BOMB_EXEC("finishmovetags#2",
			 vols_GetStatus(toconn, trans, &ftstat));
	if (code == 0) {
	    mask = 0;
	    if (!AFS_hiszero(ftstat.vss.parentId)) {
		AFS_hzero(ftstat.vss.parentId);
		mask |= VOL_STAT_PARENTID;
	    }
	    if (ftstat.vss.states & (VOL_MOVE_TARGET | VOL_OFFLINE
				     | VOL_OUTOFSERVICE | VOL_ZAPME)) {
		ftstat.vss.states &= ~(VOL_MOVE_TARGET | VOL_OFFLINE
				       | VOL_OUTOFSERVICE | VOL_ZAPME);
		ftstat.vss.volMoveTimeout = 0;
		mask |= (VOL_STAT_STATES | VOL_STAT_VOLMOVETIMEOUT);
	    }
	    if (mask) {
		code = BOMB_EXEC("finishmovetags#3",
				 vols_SetStatus(toconn, trans, mask, &ftstat));
	    }
	}
	code2 = BOMB_EXEC("finishmovetags#4", vols_DeleteTrans(toconn, trans));
	if (code == 0) code = code2;
    }
    return code;
}

#define MAX_KA_SLOTS 4
static struct bitKeepAlive {
    rpc_binding_handle_t connp;
    long *transIdP;
    struct sockaddr servAddr;
    afs_hyper_t volId;
    unsigned long aggId;
    unsigned long StatesBits;
    unsigned long volMoveTimeoutSet;
    int isDest;
    int isFetch;
    int stillGoing;
} KATasks[MAX_KA_SLOTS];
static int numKAs = 0;
static int threadGoing = 0;

static pthread_addr_t keepAliveTask(arg)
pthread_addr_t arg;
{
    unsigned long Now;
    register struct bitKeepAlive *bkap;
    register int Ix;
    long code, transId;
    struct timeval TV;
    struct ftserver_status ftstat;

    for (;;) {
	lock_ObtainWrite(&transKALock);
	Now = osi_Time();
	for (Ix = 0, bkap = &KATasks[0]; Ix < numKAs; ++Ix, ++bkap) {
	    /* Decide if *bkap needs attention. */
	    if (bkap->stillGoing == 0) continue;
	    if (bkap->isFetch) {
		/* Fetches-only: every minute */
		if (bkap->volMoveTimeoutSet >= (Now - (90)))
		    continue;
	    } else if (bkap->isDest) {
		/* Destinations: keep 8-10 minutes in future */
		if (bkap->volMoveTimeoutSet >= ((8*60) + Now))
		    continue;
	    } else {
		/* Sources: keep 2-4 minutes in future (less than 5) */
		if (bkap->volMoveTimeoutSet >= ((2*60) + Now))
		    continue;
	    }
	    /* This one needs attention. */
	    if (*bkap->transIdP >= 0) {
		Now = osi_Time();
		if (bkap->isFetch) {
		    VOLSERLOG(1, "move KA trans %ld: fetch id %lu,,%lu; \
now %lu\n",
			      *bkap->transIdP,
			      AFS_HGETBOTH(bkap->volId), Now);
		    code = BOMB_EXEC("keepalivetask#1a",
				     vols_GetStatus(bkap->connp,
						    *bkap->transIdP, &ftstat));
		    if (code == 0) {
			/* advance when it has to be done again */
			bkap->volMoveTimeoutSet = Now;
		    }
		} else {
		    ftstat.vss.states = bkap->StatesBits;
		    ftstat.vss.volMoveTimeout = Now + (bkap->isDest ? (10*60) :
						       (4*60));
		    VOLSERLOG(1, "move KA trans %ld: %s id %lu,,%lu: \
states := %#lx, TO := %lu.\n",
			      *bkap->transIdP,
			      (bkap->isDest ? "dest" : "src"),
			      AFS_HGETBOTH(bkap->volId),
			      ftstat.vss.states, ftstat.vss.volMoveTimeout);
		    code = BOMB_EXEC("keepalivetask#1",
				     vols_SetStatus(bkap->connp,
						    *bkap->transIdP,
						    VOL_STAT_STATES |
						    VOL_STAT_VOLMOVETIMEOUT,
						    &ftstat));
		    if (code == 0) {
			/* advance when it has to be done again */
			bkap->volMoveTimeoutSet = ftstat.vss.volMoveTimeout;
		    }
		}
	    } else if (!bkap->isFetch) {
		/* Release lock for the duration */
		lock_ReleaseWrite(&transKALock);
		code = (bkap->isDest
			? FLAGS_ENCODE(FTSERVER_OP_SETSTATUS,
				       VOLERR_TRANS_MOVE)
			: FLAGS_ENCODE(FTSERVER_OP_SETSTATUS
				       | FTSERVER_OP_NOACCESS,
				       VOLERR_TRANS_MOVE));
		code = BOMB_EXEC("keepalivetask#2",
				 vols_CreateTrans(bkap->connp, &bkap->volId,
						  bkap->aggId, code, &transId,
						  0));
		if (code == 0) {
		    Now = osi_Time();
		    ftstat.vss.states = bkap->StatesBits;
		    ftstat.vss.volMoveTimeout = Now + (bkap->isDest ? (10*60) :
						       (4*60));
		    VOLSERLOG(1, "move KA: %s id %lu,,%lu: states := %#lx, \
TO := %lu.\n",
			      (bkap->isDest ? "dest" : "src"),
			      AFS_HGETBOTH(bkap->volId),
			      ftstat.vss.states, ftstat.vss.volMoveTimeout);
		    code = BOMB_EXEC("keepalivetask#3",
				     vols_SetStatus(bkap->connp, transId,
						    VOL_STAT_STATES |
						    VOL_STAT_VOLMOVETIMEOUT,
						    &ftstat));
		    if (code == 0) {
			/* advance when it has to be done again */
			bkap->volMoveTimeoutSet = ftstat.vss.volMoveTimeout;
		    }
		    if (code)
			(void) BOMB_EXEC("keepalivetask#4",
					 vols_AbortTrans(bkap->connp, transId));
		    else
			(void) BOMB_EXEC("keepalivetask#5",
					vols_DeleteTrans(bkap->connp, transId));
		}
		lock_ObtainWrite(&transKALock);
	    } else {
		/* just cancel the K-A request if the transaction is gone */
		bkap->stillGoing = 0;
	    }
	}
	lock_ReleaseWrite(&transKALock);
	bzero((char *)&TV, sizeof(TV));
	TV.tv_sec = 30;
	select(0, 0, 0, 0, &TV);
    }
    /* NOTREACHED */
}

static long startThread()
{
    pthread_t keepAliveTaskPtr;
    pthread_attr_t subthreadAttr;
    long code;

    lock_ObtainWrite(&transKALock);
    if (threadGoing == 0) {
	/* Create the keep-alive thread. */
	pthread_attr_create(&subthreadAttr);
	pthread_attr_setstacksize(&subthreadAttr, 10000);
	VOLSERLOG(1, "Creating tag keep-alive thread.\n");
	code = BOMB_EXEC("kathreadcreate#1",
			 pthread_create(&keepAliveTaskPtr, subthreadAttr,
					keepAliveTask, (pthread_addr_t)NULL));

	if (code) { /* thread creation failed */
	    dce_svc_printf(FTS_S_VPROCS_NO_KEEP_ALIVE_THREAD_MSG,
			   errno, strerror(errno));
	    exit(1);
	}
	threadGoing = 1;
    }
    lock_ReleaseWrite(&transKALock);
    return 0;
}


static long SetMoveTags(connp, transIdP, servAddrp, volIdP, aggId, moveFlag)
rpc_binding_handle_t connp;
long *transIdP;
struct sockaddr *servAddrp;
afs_hyper_t *volIdP;
unsigned long aggId;
int moveFlag;
{/* Sets appropriate values in the given fileset header and starts a daemon
   to keep them there. */
    long code, transId;
    register int Ix;
    register struct bitKeepAlive *bkap;
    struct ftserver_status ftstat;
    unsigned long openStatus;
    unsigned long Now;

    if (threadGoing == 0) {
	code = BOMB_EXEC("setmovetags#1", startThread());
	if (code)
	    return code;
    }
    lock_ObtainWrite(&transKALock);
    for (Ix = 0, bkap = &KATasks[0]; Ix < MAX_KA_SLOTS; ++Ix, ++bkap) {
	if (bkap->stillGoing == 0)
	    break;
    }
    if (Ix >= MAX_KA_SLOTS) {
	lock_ReleaseWrite(&transKALock);
	return ENFILE;
    }
    bzero((char *)bkap, sizeof(struct bitKeepAlive));
    bkap->connp = connp;
    bkap->transIdP = transIdP;
    bkap->servAddr = *servAddrp;
    bkap->volId = *volIdP;
    bkap->aggId = aggId;
    /* bkap->StatesBits yet to be determined */
    bkap->volMoveTimeoutSet = 0;
    bkap->isDest = bkap->isFetch = 0;
    bkap->stillGoing = 1;
    switch (moveFlag) {
	case MOVETAG_SOURCE:
	    break;
	case MOVETAG_TARGET:
	    bkap->isDest = 1;
	    break;
	case MOVETAG_FETCH:
	    if (*transIdP < 0) {
		dce_svc_printf(FTS_S_VPROCS_MOVETAG_FETCH_MSG);
		bkap->stillGoing = 0;
		lock_ReleaseWrite(&transKALock);
		return ENOTSOCK;
	    }
	    bkap->isFetch = 1;
	    break;
	default:
	    afsos_panic("Bad value for moveFlag");
	    exit(7);
    }
    ++Ix;
    if (Ix > numKAs)
	numKAs = Ix;
    transId = -1;
    if (*transIdP >= 0) {
	Now = osi_Time();
	code = BOMB_EXEC("setmovetags#2",
			 vols_GetStatus(connp, *transIdP, &ftstat));
	if (code == 0 && bkap->isFetch)
	    bkap->volMoveTimeoutSet = Now;
	    
    } else {
	/*
	 * Need to shut off vnode ops for setting VOL_MOVE_SOURCE to eliminate
	 * races!
	 */
	openStatus = (bkap->isDest
		      ? FLAGS_ENCODE(FTSERVER_OP_GETSTATUS |
				     FTSERVER_OP_SETSTATUS,
				   VOLERR_TRANS_MOVE)
		      : FLAGS_ENCODE(FTSERVER_OP_GETSTATUS |
				     FTSERVER_OP_SETSTATUS
				     | FTSERVER_OP_NOACCESS,
				   VOLERR_TRANS_MOVE));
	code = BOMB_EXEC("setmovetags#3",
			 vols_CreateTrans(connp, volIdP, aggId, openStatus,
					  &transId, 0));
	if (code) transId = -1;
	if (code == 0) {
	    code = BOMB_EXEC("setmovetags#4",
			     vols_GetStatus(connp, transId, &ftstat));
	}
    }
    if (code) {
	bzero((char *)bkap, sizeof(struct bitKeepAlive));
	if (transId >= 0)
	    (void) BOMB_EXEC("setmovetags#5", vols_AbortTrans(connp, transId));
	lock_ReleaseWrite(&transKALock);
	return code;
    }
    /* Finish creating the *bkap structure */
    bkap->StatesBits = ftstat.vss.states;
    if (bkap->isFetch == 0) {
	if (bkap->isDest)
	    bkap->StatesBits |= VOL_MOVE_TARGET;
	else
	    bkap->StatesBits |= VOL_MOVE_SOURCE;
	/* Now set the status in the fileset itself. */
	Now = osi_Time();
	ftstat.vss.states = bkap->StatesBits;
	ftstat.vss.volMoveTimeout = Now + (bkap->isDest ? (10*60) : (4*60));
	code = BOMB_EXEC("setmovetags#6",
			 vols_SetStatus(connp,
					(*transIdP >= 0 ? *transIdP : transId),
					VOL_STAT_STATES |
					VOL_STAT_VOLMOVETIMEOUT, &ftstat));
	if (code == 0) {
	    /* advance when it has to be done again */
	    bkap->volMoveTimeoutSet = ftstat.vss.volMoveTimeout;
	}
	if (*transIdP < 0) {
	    BOMB_EXEC("setmovetags#7", vols_AbortTrans(connp, transId));
	}
    }
    lock_ReleaseWrite(&transKALock);
    return code;
}

static long SetOneStatus(bkap, newStatus, useVolMoveTimeout, deleteInstead)
register struct bitKeepAlive *bkap;
unsigned long newStatus;
int useVolMoveTimeout, deleteInstead;
{/* We pile the deletion technology into here so that we can fold the
   OFFLINE|SOURCE -> deleted step into one indivisible transition. */
    struct ftserver_status ftstat;
    long code, transId;
    unsigned long bits;
    register int Ix;
    unsigned long Now;
	
    if (bkap->isFetch != 0 && deleteInstead == 0)
	return 0;
    if (*bkap->transIdP >= 0) {
	if (deleteInstead) {
	    code = BOMB_EXEC("setonestatus#1",
			     vols_DeleteVolume(bkap->connp, *bkap->transIdP));
	    if (code == 0)
		bkap->stillGoing = 0;	/* deallocate early */
	} else {
	    Now = osi_Time();
	    ftstat.vss.states = newStatus;
	    if (useVolMoveTimeout)
		ftstat.vss.volMoveTimeout = Now + (bkap->isDest ? (10*60) :
						   (4*60));
	    else
		ftstat.vss.volMoveTimeout = 0;
	    code = BOMB_EXEC("setonestatus#2",
			     vols_SetStatus(bkap->connp, *bkap->transIdP,
					    VOL_STAT_STATES |
					    VOL_STAT_VOLMOVETIMEOUT, &ftstat));
	    if (code == 0) {
		bkap->StatesBits = newStatus;
		/* advance when it has to be done again */
		bkap->volMoveTimeoutSet = ftstat.vss.volMoveTimeout;
	    }
	}
    } else {
	/* Release lock for the duration */
	bits = (deleteInstead
		? FLAGS_ENCODE(FTSERVER_OP_DELETE, VOLERR_PERS_DELETED)
		: (bkap->isDest
		   ? FLAGS_ENCODE(FTSERVER_OP_SETSTATUS, VOLERR_TRANS_MOVE)
		   : FLAGS_ENCODE(FTSERVER_OP_SETSTATUS | FTSERVER_OP_NOACCESS,
				  VOLERR_TRANS_MOVE)));
	code = BOMB_EXEC("setonestatus#3",
			 vols_CreateTrans(bkap->connp, &bkap->volId,
					  bkap->aggId, bits, &transId, 0));
	if (deleteInstead
	    && (code == ENOENT || code == ENODEV ||
		code == FTSERVER_ERR_NOVOL)) {
		code = 0;	/* fake a success if it's gone already */
	} else {
	    if (code == 0) {
		if (deleteInstead) {
		    /* delete the thing */
		    code = BOMB_EXEC("setonestatus#4",
				     vols_DeleteVolume(bkap->connp, transId));
		    if (code == 0)
			bkap->stillGoing = 0;	/* deallocate early */
		} else {
		    Now = osi_Time();
		    ftstat.vss.states = newStatus;
		    if (useVolMoveTimeout)
			ftstat.vss.volMoveTimeout = Now + (bkap->isDest ?
							   (10*60) : (4*60));
		    else
			ftstat.vss.volMoveTimeout = 0;
		    code = BOMB_EXEC("setonestatus#5",
				     vols_SetStatus(bkap->connp, transId,
						    VOL_STAT_STATES |
						    VOL_STAT_VOLMOVETIMEOUT,
						    &ftstat));
		    if (code == 0) {
			bkap->StatesBits = newStatus;
			/* advance when it has to be done again */
			bkap->volMoveTimeoutSet = ftstat.vss.volMoveTimeout;
		    }
		}
		if (code)
		    (void) BOMB_EXEC("setonestatus#6",
				     vols_AbortTrans(bkap->connp, transId));
		else
		    code = BOMB_EXEC("setonestatus#7",
				     vols_DeleteTrans(bkap->connp, transId));
	    }
	}
    }
    return code;
}

static long SetMoveBaseStatus(connp, transId, setBits, clearBits)
rpc_binding_handle_t connp;
long transId;
unsigned long setBits;
unsigned long clearBits;
{
    long code;
    register int Ix;
    unsigned long newstates;
    register struct bitKeepAlive *bkap;

    if (threadGoing == 0) {
	code = BOMB_EXEC("setmovebase#1", startThread());
	if (code)
	    return code;
    }
    lock_ObtainWrite(&transKALock);
    for (Ix = 0, bkap = &KATasks[0]; Ix < MAX_KA_SLOTS; ++Ix, ++bkap) {
	if (bkap->stillGoing == 0)
	    continue;
	if (connp == bkap->connp && transId == *bkap->transIdP)
	    break;
    }
    if (Ix >= MAX_KA_SLOTS) {
	lock_ReleaseWrite(&transKALock);
	return 0;	/* not there any more */
    }
    newstates = (bkap->StatesBits & ~clearBits) | setBits;
    if (bkap->StatesBits == newstates) {
	lock_ReleaseWrite(&transKALock);
	return 0;	/* states bits are already correct */
    }
    code = BOMB_EXEC("setmovebase#2", SetOneStatus(bkap, newstates, 1, 0));
    lock_ReleaseWrite(&transKALock);
    return code;
}

static long ClearMoveTags(servAddrp, volIdp, aggId, isAbort, deleteInstead)
struct sockaddr *servAddrp;
afs_hyper_t *volIdp;
unsigned long aggId;
int isAbort, deleteInstead;
{
    long code;
    register int Ix;
    register struct bitKeepAlive *bkap;
    unsigned long newStates;

    if (threadGoing == 0)
	return 0;
    lock_ObtainWrite(&transKALock);
    for (Ix = 0, bkap = &KATasks[0]; Ix < MAX_KA_SLOTS; ++Ix, ++bkap) {
	if (bkap->stillGoing == 0)
	    continue;
	if (bcmp((char *)servAddrp, (char *)&bkap->servAddr,
		 sizeof(struct sockaddr)) == 0
	    && AFS_hsame(*volIdp, bkap->volId)
	    && aggId == bkap->aggId)
	    break;
    }
    if (Ix >= MAX_KA_SLOTS) {
	lock_ReleaseWrite(&transKALock);
	return 0;	/* not there any more */
    }
    newStates = bkap->StatesBits;
    if (!isAbort) newStates &= ~(VOL_MOVE_TARGET | VOL_MOVE_SOURCE);
    code = BOMB_EXEC("clearmovetags#1",
		     SetOneStatus(bkap, newStates, 0, deleteInstead));
    if (code == 0)
	bkap->stillGoing = 0;	/* deallocate */
    lock_ReleaseWrite(&transKALock);
    return code;
}

static long threeIDsFromOne(volIdp, servAddrp, newIDsp)
afs_hyper_t *volIdp;
struct sockaddr *servAddrp;
bulkIDs *newIDsp;
{/* Get three fileset IDs from the one that was given. */
    long code;
    vldbentry ventry;
    afs_hyper_t maxVolid, idDum;
    int needed;
    register int ix, jx;
    bulkIDs myBulk;

    /* First, check whether the given ID is free. */
    code = vldb_GetEntryByID(volIdp, -1, &ventry, 1);
    if (code != VL_NOENT) {
	if (code == 0) {
	    VOLSERLOG(0, "The given fileset ID of %lu,,%lu is in use by \
fileset %s.\n",
		      AFS_HGETBOTH(*volIdp), ventry.name);
	    return FTS_ERR_FSET_EXISTS;
	} else {
	    VOLSERLOG(0, "Cannot check the given fileset ID of %lu,,%lu: %s.\n",
		      AFS_HGETBOTH(*volIdp), dfs_dceErrTxt(code));
	    return code;
	}
    }
    /* Now check if the given one is in range. */
    if (code = vldb_GetNewVolumeId(0, servAddrp, &maxVolid)) {
	VOLSERLOG(0, "Cannot check whether %lu,,%lu is in range: %s.\n",
		  AFS_HGETBOTH(*volIdp), dfs_dceErrTxt(code));
	return code;
    }
    if (AFS_hcmp(maxVolid, *volIdp) <= 0) {
	VOLSERLOG(0, "Given volid of %lu,,%lu has never been allocated in the \
FLDB.\n",
		  AFS_HGETBOTH(*volIdp));
	return VL_BADID;
    }
    /* OK, that one was good. */
    needed = 0;
    bzero(newIDsp, sizeof(*newIDsp));
    /* Copy in the first one. */
    newIDsp->bulkIDs_len = 3;
    newIDsp->bulkIDs_val[0] = *volIdp;
    /* Now find more IDs, either subsequent ones or free ones. */
    for (ix = 1; ix < 3; ++ix) {
	idDum = *volIdp;
	AFS_hadd32(idDum, ix);
	if (AFS_hcmp(maxVolid, idDum) <= 0) {
	    ++needed;
	} else {
	    code = vldb_GetEntryByID(&idDum, -1, &ventry, 1);
	    if (code && code != VL_NOENT) {
		VOLSERLOG(0, "Cannot check whether derived fileset ID %lu,,%lu \
is in use: %s\n",
			  AFS_HGETBOTH(idDum), dfs_dceErrTxt(code));
		return code;
	    } else if (code == VL_NOENT) {
		/* OK, we can use this one. */
		newIDsp->bulkIDs_val[ix] = idDum;
	    } else {
		++needed;
	    }
	}
    }
    if (needed > 0) {
	code = vldb_GetNewVolumeIds(needed, servAddrp, &myBulk);
	if (code != 0) {
	    VOLSERLOG(0, "Cannot allocate the remaining %d ID(s): %s\n",
		      needed, dfs_dceErrTxt(code));
	    return code;
	}
	if (myBulk.bulkIDs_len != needed) {
	    VOLSERLOG(1, "Internal inconsistency: asked for %d IDs but \
got %d\n",
		      needed, myBulk.bulkIDs_len);
	    return VL_BADID;
	}
	jx = 0;
	for (ix = 0; ix < 3; ++ix) {
	    if (AFS_hiszero(newIDsp->bulkIDs_val[ix])) {
		newIDsp->bulkIDs_val[ix] = myBulk.bulkIDs_val[jx];
		++jx;
	    }
	}
	if (jx != needed)
	    VOLSERLOG(0, "Internal inconsistency: asked for %d ID(s) but \
used %d\n",
		      needed, jx);
    }
    return 0;
}
