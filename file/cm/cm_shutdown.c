/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *      Copyright (C) 1996, 1990 Transarc Corporation
 *      All rights reserved.
 */

#include <dcedfs/param.h>			/* Should be always first */
#include <dcedfs/sysincludes.h>		/* Standard vendor system headers */
#include <cm.h>				/* Cm-based standard headers */
#include <cm_cell.h>
#include <cm_bkg.h>
#include <cm_volume.h>
#include <cm_dcache.h>
#include <cm_conn.h>
#include <cm_vcache.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/cm/RCS/cm_shutdown.c,v 4.40 1996/03/04 18:55:04 ota Exp $")

struct lock_data cm_termStateLock;

static void shutdown_BKG _TAKES((void));
static void shutdown_TKN _TAKES((void));
static void shutdown_Vnodes _TAKES((void));
static void shutdown_Daemons _TAKES((void));
static void cm_WaitForMount _TAKES((void));

/* count of active CM subprocesses for shutting down cleanly */
static long cm_sdVnodeCount = 0;
static long cm_sdTKNCount = 0;
static long cm_sdBKGCount = 0;
static long cm_sdDaemonCount = 0;
static int cm_shuttingDown = 0; 

/* called to idle DFS, so that we can do unmount of disks and
 * know that DFS client code won't start writing to UFS, or allow
 * other process to make DFS vnode calls, or start other DFS activity.
 * Waits for current activity to stop.
 */
cm_shutdown() 
{
    int i, rc;
    struct cm_dcache *dcp;
    struct cm_scache *scp;
    int dowrite;

    icl_Trace0(cm_iclSetp, CM_TRACE_SHUTDOWN);

    lock_ObtainWrite(&cm_termStateLock);
    if (cm_shuttingDown) {
	lock_ReleaseWrite(&cm_termStateLock);
	return 0;
    }
    cm_shuttingDown = 1;
    osi_Wakeup(&cm_shuttingDown);
    shutdown_Vnodes();	/* shutdown new lookups on /... first */
    /* 
     * Write through the modified dcache entries
     */
    lock_ObtainWrite(&cm_dcachelock);
    for (i = 0; i < cm_cacheFiles; i++) {
	dcp = cm_GetDSlot(i, 0);
	osi_assert(dcp != NULL);

	dowrite = cm_indexFlags[i] & DC_IFENTRYMOD;
	/*
	 * If the entry is online with no references, then mark it 
	 * offline and clear its tokenID field.  This will keep 
	 * cm_ClearOnlineState from clearing the state and updating the 
	 * modTime on the entry, which will cause it to be invalidated 
	 * on the next cache manager startup.  It is ok to update 
	 * the entry here without updating the entry's modTime since 
	 * the online state does not alter the entry's identity, 
	 * and will be reset on cache manager restart anyway.
	 */
	if (dcp->refCount == 1) {
	    if ((dcp->f.states & DC_DONLINE) && 
		(scp = cm_FindSCache(&dcp->f.fid))) {
		dcp->f.versionNo = scp->m.DataVersion;
		cm_PutSCache(scp);
	    }
	    dcp->f.states &= ~DC_DONLINE;
	    AFS_hzero(dcp->f.tokenID);
	    dowrite = 1;
	}
	if (dowrite)
	    cm_WriteDCache(dcp, cm_indexFlags[i] & DC_IFENTRYMOD);

	cm_indexFlags[i] &= ~DC_IFENTRYMOD;
	dcp->refCount--;
    }
    lock_ReleaseWrite(&cm_dcachelock);
    /* 
     * Try to Idle the scache pool.  If the pool is still busy
     * then abort the shutdown and return EBUSY.
     */
    rc = cm_IdleSCaches();
    if (rc == 0) {
	cm_ServerTokenMgt();    /* return queued tokens to servers */
	shutdown_TKN();  	/* stop handling revokes */
	shutdown_BKG(); 	/* stop background worker bees */
	shutdown_Daemons();	/* stop periodic work */
	if (cm_cacheFilep && !cm_diskless) {
	    /* Close CacheItems file so the cache fs can be unmouted. */
	    osi_Close(cm_cacheFilep);
	    cm_cacheFilep = NULL;
	}
    } else {
	cm_shuttingDown = 0;
	osi_Wakeup(&cm_shuttingDown);	/* restart held operations */
    }
    lock_ReleaseWrite(&cm_termStateLock);
    return rc;
}


/* Called to restart system after cm_shutdown idled it.  Called
 * from basic mount code.  There, unfortunately, no useful mount
 * command here.
 */
void cm_UnShutdown()
{
    lock_ObtainWrite(&cm_termStateLock);
    if (cm_shuttingDown) {
	/* was shutdown, restart waiting processes */
	/* Open up the Cacheitems file */
	if (!cm_diskless && (cm_cacheFilep == NULL)) {
#ifdef AFS_VFSCACHE
	    if (!(cm_cacheFilep = osi_Open(cm_cache_mp, &cm_cacheHandle)))
#else
	    if (!(cm_cacheFilep = osi_Open(&cacheDev, cm_cacheInode)))
#endif
		panic("cacheitems open");
	}
	cm_shuttingDown = 0;
	osi_Wakeup(&cm_shuttingDown);
    }
    lock_ReleaseWrite(&cm_termStateLock);
}


/* called by token revoke thread when it starts doing something
 * useful, i.e. revokes a token.  Tracks whether tkn revoke
 * threads are idle.
 */
int cm_EnterTKN()
{
    lock_ObtainWrite(&cm_termStateLock);
    if (cm_shuttingDown) {
	lock_ReleaseWrite(&cm_termStateLock);
	return EBUSY;
    }
    cm_sdTKNCount++;
    lock_ReleaseWrite(&cm_termStateLock);
    return 0;
}

/* called when revoke thread is about to return to RPC runtime,
 * i.e. when thread is about to become idle.
 */
void cm_LeaveTKN()
{
    lock_ObtainWrite(&cm_termStateLock);
    if (--cm_sdTKNCount < 0) panic("cm_LeaveTKN");
    if (cm_shuttingDown) osi_Wakeup(&cm_shuttingDown);
    lock_ReleaseWrite(&cm_termStateLock);
}

/* called with shuttingdown lock held.  Wait until there are no
 * active token revokes, and then return.  No new ones will start since
 * we're in the shutdown phase.
 */
static void shutdown_TKN() 
{
    /* force token revokes to stop */
    while(cm_sdTKNCount > 0) {
	osi_SleepW((char *) &cm_shuttingDown, &cm_termStateLock);
	lock_ObtainWrite(&cm_termStateLock);
    }
}


/* Same sets of functions as TKN, only for shutting down background
 * daemons.
 */

void cm_EnterBKG()
{
    lock_ObtainWrite(&cm_termStateLock);
    cm_WaitForMount();
    cm_sdBKGCount++;
    lock_ReleaseWrite(&cm_termStateLock);
}

void cm_LeaveBKG()
{
    lock_ObtainWrite(&cm_termStateLock);
    if (--cm_sdBKGCount < 0) panic("cm_LeaveBKG");
    if (cm_shuttingDown) osi_Wakeup(&cm_shuttingDown);
    lock_ReleaseWrite(&cm_termStateLock);
}

/* similar to shutdown_TKN, but has a different way of ensuring that
 * the background daemons notice that we're shutting down immediately.
 */
static void shutdown_BKG()
{
    osi_Wakeup(&cm_bkgDaemons);
    while(cm_sdBKGCount > 0) {
	osi_SleepW((char *) &cm_shuttingDown, &cm_termStateLock);
	lock_ObtainWrite(&cm_termStateLock);
    }
}


/* Same sets of code as above, only to ensure that there are no
 * threads currently executing vnode calls.
 */

int cm_EnterVnode()
{
    lock_ObtainWrite(&cm_termStateLock);
    if (cm_shuttingDown) {
	lock_ReleaseWrite(&cm_termStateLock);
	return EBUSY;
    }
    cm_sdVnodeCount++;
    lock_ReleaseWrite(&cm_termStateLock);
    return 0;
}

void cm_LeaveVnode()
{
    lock_ObtainWrite(&cm_termStateLock);
    if (--cm_sdVnodeCount < 0) panic("cm_LeaveVnode");
    if (cm_shuttingDown) osi_Wakeup(&cm_shuttingDown);
    lock_ReleaseWrite(&cm_termStateLock);
}

static void shutdown_Vnodes()
{
    while(cm_sdVnodeCount > 0) {
	osi_SleepW((char *) &cm_shuttingDown, &cm_termStateLock);
	lock_ObtainWrite(&cm_termStateLock);
    }
}

/* wait for a shutdown to occur */
void cm_WaitForShutdown()
{
    lock_ObtainWrite(&cm_termStateLock);
    while(!cm_shuttingDown) {
	osi_SleepW(&cm_shuttingDown, &cm_termStateLock);
	lock_ObtainWrite(&cm_termStateLock);
    }
    lock_ReleaseWrite(&cm_termStateLock);
}

/* tell system we're trying to start daemon up again */
void cm_EnterDaemon()
{
    lock_ObtainWrite(&cm_termStateLock);
    cm_WaitForMount();
    cm_sdDaemonCount++;
    lock_ReleaseWrite(&cm_termStateLock);
}

/* tell system we've shutdown the daemon properly */
void cm_LeaveDaemon()
{
    lock_ObtainWrite(&cm_termStateLock);
    cm_sdDaemonCount--;
    if (cm_shuttingDown)
	osi_Wakeup(&cm_shuttingDown);
    lock_ReleaseWrite(&cm_termStateLock);
}

/* same as above, but for the daemon count.
 * for daemon to acknowledge shutdown request.  Note that the caller has
 * already woken up anyone waiting for cm_shuttingDown to change (by
 * doing an osi_Wakeup(&cm_shuttingDown)).
 */
static void shutdown_Daemons()
{
    while(cm_sdDaemonCount > 0) {
	osi_SleepW((char *) &cm_shuttingDown, &cm_termStateLock);
	lock_ObtainWrite(&cm_termStateLock);
    }
}


/* wait for later mount.  When restart done, we return, and daemons
 * will start handling requests again.
 */
static void cm_WaitForMount()
{
    while (cm_shuttingDown) {
	osi_SleepW((char*) &cm_shuttingDown, &cm_termStateLock);
	lock_ObtainWrite(&cm_termStateLock);
    }
}
