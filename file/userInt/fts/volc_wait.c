/*
 * HISTORY
 * $Log: fts_wait.c,v $
 * $EndLog$
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/common_data.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/stds.h>
#include <dcedfs/basicincludes.h>
#include <dcedfs/volume.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/rep_data.h>
#include <dcedfs/osi_dfserrors.h>

#include <dcedfs/fldb_data.h>
#include <dcedfs/flserver.h>
#include <dcedfs/ftserver_data.h>
#include <dcedfs/ftserver.h>
#include <dcedfs/rep_errs.h>

#include <volc.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftimac.h>
#include <dfsftisvc.h>
#include <dfsftimsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/userInt/fts/RCS/volc_wait.c,v 1.3 1996/03/04 19:16:40 ota Exp $")

/*
 * Module for implementing a waiting action for fts actions which may
 * want to wait for a repserver to complete its update.
 */

/*
 * Attempt to wait for all replication sites to receive the new version
 * of data from the relevant source fileset.
 *
 * The only trigger that we have for kicking the repserver is
 * REP_UpdateSelf() calls.
 *
 * The only information that we can obtain about a given replica managed
 * by some repserver is returned with REP_GetOneRepStatus() in a
 * "struct repStatus".
 *
 * We do not want to assume too much about the internal mechanisms of the
 * repserver, but we need to look at some of the data inside the
 * "struct repStatus".  In determining whether a replica update is finished,
 * we examine:
 *
 * repStatus.srcVV
 * repStatus.curVV
 *
 * When both of these are non-zero, and the curVV >= VV of the source
 * fileset, then we are done.
 *
 * The tricky part is determining whether a repserver is likely to be
 * able to successfully update the replica.  There are 2 situations
 * which are indistinguishable.
 *
 * 1. The repserver tries to retrieve the new fileset data (with
 *    FTSERVER_Forward), but the source fileset is busy because another
 *    repserver is currently forwarding the fileset.
 *
 * 2. The repserver tries to retrieve the new fileset data, but the
 *    retrieval fails for some other reason (for example, insufficient
 *    space on the destination aggregate).
 *
 * In order to work around this problem, we deploy several heuristics
 * which should allow us to wait until all replicas are updated if
 * everything is functioning properly, but bail out if not.
 *
 * Other failures, such as not even beginning to retrieve the fileset,
 * are somewhat easier to detect.
 *
 * The fields in the repStatus relevant to this are
 *
 * repStatus.whenTried
 * repStatus.whenToTryAgain
 *
 * The whenTried field former is assumed to be set when the repserver
 * is about to retrieve the fileset from the source ftserver managing
 * the source fileset.
 */

/*
 * The state machine progression for asking the repserver to update a
 * replica.
 */
#define SITE_STATE_START	0
#define SITE_STATE_DOBINDING	1
#define SITE_STATE_DOUPDATE	2
#define SITE_STATE_INPROGRESS	3
#define SITE_STATE_COMPLETE	4

/*
 * A grace period to control how long we will wait for a site to update
 * a replica if the retrieval is long running.  If a site appears to be
 * executing a fileset retrieval longer than this, then we will set the
 * GIVE_UP flag.  If the retrieval fails, then we will not try this site
 * again.
 */
#define MAX_RETRIEVAL_GRACE	(15*60)

/*
 * This is the longest time that we will wait for whenToTryAgain.  After
 * this many seconds have past, we will redo the REP_UpdateSelf() call.
 */
#define MAX_WHEN_TO_TRY		(2*60)

/*
 * The amount of time we will wait between some failure and when to try
 * again.
 */
#define FAIL_WAIT_TIME		(2*60)

/*
 * The maximum number of attempts we will try to get a site to retrieve
 * a replica.
 */
#define MAX_ATTEMPTS		5

/*
 * Maximum number of failures for various RPC calls, etc.
 */
#define MAX_FAILURES		5

/*
 * Various flags for ftsSiteStatus.flags
 */

/*
 * The repserver is retrieving a fileset from the source file set
 */
#define IS_RETRIEVING	0x1

/*
 * Defer giving up on this replication site until any pending retrievals
 * are completed, whether successful or not.
 */
#define GIVE_UP		0x2

typedef struct ftsSiteStatus {
    struct ftsSiteStatus *nextSite;
    int siteState;
    struct sockaddr_in *repAddr;
    rpc_binding_handle_t repHandle;
    error_status_t code;
    time_t updateTime; /* Time of last REP_UpdateSelf call */
    time_t lastTried; /* Copy of repStatus.whenTried */
    time_t nextCheck; /* When next do something for some replica site */
    int nAttempts;    /* Number of retrieval attempts */
    int nFailures;
    u_long flags;
    byte repMsg[REPMSG_SIZE]; /* Copy of the last repserver message */
} ftsSiteStatus;

typedef struct ftsSiteList {
    ftsSiteStatus root;
    ftsSiteStatus *tail;
    int nSites;
    int nComplete;
    int anyFailed;
} ftsSiteList;

static void fts_FreeStatusList _TAKES((ftsSiteList *));

static void fts_InsertSiteStatus _TAKES((ftsSiteList *, ftsSiteStatus *));

static void fts_ProcessRepSite _TAKES((ftsSiteStatus *,
				       afs_hyper_t *,
				       afs_hyper_t *));

static void fts_AnalyzeRepProgress _TAKES((ftsSiteList *,
					   afs_hyper_t *,
					   int));

/*
 * Attempt to wait for a list of repserver hosts for a repica to receive
 * their updates.
 *
 * entryp points to the FLDB entry for the volume given by rwVolIdp.
 * goalVVp contains the minimum VV that we wish the repserver to have
 * for the replica associated with rwVolIdp.  If servAddrs is non-NULL,
 * then it specifies a subset of servers in entryp for which we wish
 * to wait for updates.  If servAddrs is NULL, then we wait for all of
 * the replication sites indicated by entryp.
 *
 * Most of the work is done by fts_ProcessRepSite() and
 * fts_AnalyzeRepProgress().
 *
 * Returns:
 * 0	if all repservers successfully receive the update.
 * FTS_ERR_BADRELEASE if any repservers fail to receive the update.
 */
long fts_WaitForRepHostUpdates(entryp, rwVolIdp, goalVVp, servAddrs, nAddrs)
struct vldbentry *entryp;
afs_hyper_t *rwVolIdp;
afs_hyper_t *goalVVp;
afsNetAddr *servAddrs;
int nAddrs;
{
    ftsSiteList siteList;
    int ix1, ix2;
    ftsSiteStatus *sitep;
    int xl[MAXTYPES];
    int localIx;
    int newState;
    afs_hyper_t roVolId;
    u_long roFlag;
    int releaseStyle;
    time_t now;
    int nServers;
    afsNetAddr *addrs;

    /*
     * It must be a replicated fileset
     */
    if ((entryp->flags & VLF_LAZYREP) == 0) {
	dce_svc_printf(FTS_S_VPROCS_SET_REPLICATION_TYPE_MSG,
		       AFS_HGETBOTH(*rwVolIdp));
	return(FTS_ERR_BADOP);
    }

    fts_GetIndices(entryp, xl);

    /*
     * There must be a R/W fileset
     */
    if (xl[RWVOL] < 0 || AFS_hiszero(entryp->VolIDs[xl[RWVOL]])) {
	dce_svc_printf(FTS_S_VPROCS_NO_READ_WRITE_FILESET_MSG,
		       AFS_HGETBOTH(*rwVolIdp));
	return(FTS_ERR_NEED_RW);
    }

    /*
     * The R/W fileset given in rwVolIdp must match the one in the
     * FLDB entry given by entryp.
     */
    if (!AFS_hsame(entryp->VolIDs[xl[RWVOL]], *rwVolIdp)) {
	dce_svc_printf(FTS_S_VPROCS_NOT_READ_WRITE_FILESET_MSG,
		       AFS_HGETBOTH(*rwVolIdp));
    }

    /*
     * There must be a readonly site
     */
    if (xl[ROVOL] < 0 || AFS_hiszero(entryp->VolIDs[xl[ROVOL]])) {
	dce_svc_printf(FTS_S_VPROCS_NO_READ_ONLY_FILESET_MSG,
		       AFS_HGETBOTH(*rwVolIdp));
    }
    roFlag = ((unsigned32)VLSF_ZEROIXHERE) >> xl[ROVOL];

    releaseStyle = ((entryp->flags & VLF_LAZYRELE) == VLF_LAZYRELE);
    
    /*
     * Find the index of the staging replica for release-style replication
     */

    if (releaseStyle) {
	localIx = fts_releaseRepSite(entryp);

	if (localIx < 0) {
	    switch (localIx) {
	      case -1:
		dce_svc_printf(FTS_S_VPROCS_NOT_RELEASE_REPLICATED_MSG,
			       AFS_HGETBOTH(*rwVolIdp));
		break;
	      case -2:
		dce_svc_printf(FTS_S_VPROCS_NO_READ_WRITE_ID_MSG,
			       AFS_HGETBOTH(*rwVolIdp));
		break;
	      case -3:
		dce_svc_printf(FTS_S_VPROCS_NO_READ_ONLY_STAGING_SITE_MSG,
			       AFS_HGETBOTH(*rwVolIdp));
		break;
	    }
	    return(FTS_ERR_BADOP);
	}
    } else {
	/*
	 * There is no staging replica for scheduled replication
	 */
	localIx = -1;
    }


    roVolId = entryp->VolIDs[xl[ROVOL]];
    
    VOLSERLOG(1,
	      "Attempting to bring sites for fileset %lu,,%lu up to VV %lu,,%lu\n",
	      AFS_HGETBOTH(roVolId), AFS_HGETBOTH(*goalVVp));

    siteList.root.nextSite = NULL;
    siteList.tail = &siteList.root;
    siteList.nSites = 0;
    siteList.nComplete = 0;
    siteList.anyFailed = 0;

    /*
     * Fill in the site list, based on the given site addresses, or all
     * if no list.
     */
    if (servAddrs != NULL) {
	addrs = servAddrs;
	nServers = nAddrs;
    } else {
	addrs = entryp->siteAddr;
	nServers = entryp->nServers;
    }

    for (ix1 = 0; ix1 < nServers; ix1++) {
	if (servAddrs != NULL) {
	    /*
	     * See if this is a valid server in entryp
	     */
	    for (ix2 = 0; ix2 < entryp->nServers; ix2++) {
		if (ix2 == localIx) {
		    /* Skip the staging replica */
		    continue;
		}
		if (entryp->siteFlags[ix2] & VLSF_SAMEASPREV) {
		    continue;
		}
		if (bcmp((void *)&addrs[ix1], (void *)&entryp->siteAddr[ix2],
			 sizeof(afsNetAddr)) == 0) {
		    break;
		}
	    }

	    if (ix2 >= entryp->nServers) {
		dce_svc_printf(FTS_S_WAIT_NO_REPLICA_DEFINED_MSG,
			       AFS_HGETBOTH(*rwVolIdp),
			       MapSockAddr(&addrs[ix1]));
		continue;
	    }
	} else {
	    if (ix1 == localIx) {
		/* Skip the staging replica */
		continue;
	    }
	    if (entryp->siteFlags[ix1] & VLSF_SAMEASPREV) {
		continue;
	    }
	    if ((entryp->siteFlags[ix1] & roFlag) == 0) {
		continue;
	    }
	    ix2 = ix1;
	}

	VOLSERLOG(1, "Adding %lu,,%lu on %s (aggr %ld)\n",
		  AFS_HGETBOTH(roVolId),
		  MapSockAddr((struct sockaddr *)&entryp->siteAddr[ix2]),
		  entryp->sitePartition[ix2]);

	sitep = (ftsSiteStatus *)malloc(sizeof(ftsSiteStatus));
	if (sitep == NULL) {
	    fts_FreeStatusList(&siteList);
	    return(ENOMEM);
	}

	sitep->nextSite = NULL;
	sitep->siteState = SITE_STATE_START;
	sitep->repAddr = (struct sockaddr_in *)&entryp->siteAddr[ix2];
	sitep->repHandle = NULL;
	sitep->code = 0;
	sitep->updateTime = 0;
	sitep->lastTried = 0;
	sitep->nextCheck = 0;
	sitep->nAttempts = 0;
	sitep->nFailures = 0;
	sitep->flags = 0;
	sitep->repMsg[0] = '\0';

	dce_svc_printf(FTS_S_WAIT_WILL_WAIT_FOR_RO_FILESET_MSG,
		       AFS_HGETBOTH(roVolId), MapSockAddr(sitep->repAddr));

	fts_InsertSiteStatus(&siteList, sitep);
    }

    (void) vldb_GetRemoteCellID();

    while (1) {
	for (sitep = siteList.root.nextSite; sitep != NULL;
	     sitep = sitep->nextSite) {

	    if (sitep->siteState == SITE_STATE_COMPLETE) {
		continue;
	    }

	    fts_ProcessRepSite(sitep, &roVolId, goalVVp);

	    if (sitep->siteState == SITE_STATE_COMPLETE) {
		siteList.nComplete++;
		if (sitep->code != 0) {
		    siteList.anyFailed = 1;
		}
		fts_AnalyzeRepProgress(&siteList, &roVolId, 1);
	    }
	}

	/*
	 * 0 or more may have change since we last processed the list.
	 * See if we need to give up on anything.
	 */
	fts_AnalyzeRepProgress(&siteList, &roVolId, 0);

	if (siteList.nSites == siteList.nComplete) {
	    break;
	}

	sleep(1);
    }

    fts_FreeStatusList(&siteList);

    return(siteList.anyFailed ? FTS_ERR_BADRELEASE : 0);
}

static void fts_FreeStatusList(siteListP)
ftsSiteList *siteListP;
{
    ftsSiteStatus *sitep, *tsitep;
    error_status_t st;

    for (sitep = siteListP->root.nextSite; sitep != NULL;) {
	tsitep = sitep;
	sitep = sitep->nextSite;
	free((void *)tsitep);
    }
}

static void fts_InsertSiteStatus(siteListP, sitep)
ftsSiteList *siteListP;
ftsSiteStatus *sitep;
{
    siteListP->tail->nextSite = sitep;
    siteListP->tail = sitep;
    siteListP->nSites++;
}

/*
 *
 * The basic mode of operation is as follows.
 *
 * Begin in SITE_STATE_START.  Currently there is no action associated
 * with this state, so we move to state SITE_STATE_DOBINDING.
 *
 * In SITE_STATE_DOBINDING, we try allocate an RPC binding handle.
 * If this fails, we wait for FAIL_WAIT_TIME and try again.  Once we
 * have processed one iteration of the list of repservers, we call
 * fts_AnalyzeRepProgress().  In here, if there are no working repservers
 * and the number of failures is >= MAX_FAILURES, then we give up.  This
 * is a trade off between giving up early and hoping that the situation
 * will improve sometime along the way.  Once we successfully obtain an
 * RPC binding handle, we move to SITE_STATE_DOUPDATE.
 *
 * In SITE_STATE_DOUPDATE, we attempt to send a REP_UpdateSelf() RPC to
 * a repserver.  If this succeeds, then we clear the number of failures
 * and move to SITE_STATE_INPROGRESS.  If this fails, it could be because
 * this is no longer a valid replication site, in which case we are done.
 * This is not treated as a release error since the repserver is as up to
 * date as it is going to be.  Under any other failure case, we increment
 * the number of failures and handle the analysis in fts_AnalyzeRepProgress()
 * just as with SITE_STATE_DOBINDING failures.
 *
 * In SITE_STATE_INPROGRESS, we basically do the following.  Do a
 * REP_GetOneRepStatus() for the (repserver, cell, volume-id) triple.
 * If this fails, it could be that this is no longer a valid replication
 * site, so we consider this complete and not a release error, just as
 * when REP_UpdateSelf() is called.  With any other error, we increment
 * the number of failures and determine whether we should give up in
 * fts_AnalyzeRepProgress() after processing the list of repservers.
 *
 * If the REP_GetOneRepStatus() succeeds, then we are done if the notion
 * of source volume version (VV) and current VV at the repserver is non-zero,
 * equal and at least as large as the goal VV passed to
 * fts_WaitForRepHostsUpdates().
 *
 * In any other case, we need to look at the whenToTryAgain and whenTried
 * and determine if the repserver appears to be making progress.  One of
 * the assumptions is that the action of forwarding the fileset from the
 * source to the replication site will typically be the longest running
 * application.  When whenToTryAgain is 0, the repserver is in the process
 * of doing something.  If whenTried is greater than the last value we
 * stored for this repserver, then we credit the repserver as retrieving
 * the fileset and set the IS_RETRIEVING flag.  If the whenTried value
 * is less than our saved value, then we assume that something odd has
 * occurred on the repserver and we give up.
 *
 * If whenToTryAgain is non-zero, this could be for a few reasons.  One
 * is that the repserver failed doing something and has set a wake up
 * time for the future.  Another, is that somehow the repserver has
 * an internal consitency problem (the wake up time has no relation to
 * the whenToTryAgain time).  A third is that either by us, or another
 * repserver via a REP_AllCheckReplicationConfig(), a REP_UpdateSelf()
 * RPC has been set, which has the side effect of resetting whenToTryAgain
 * even when the repserver is processing a replica.  Another issue with
 * whenToTryAgain is that if a replica has large replication parameters,
 * such as site age, there could be a long lag between update attempts
 * when a fileset retrieval fails because the source is busy.
 *
 * Under the condition that whenToTryAgain is non-zero, we first see if
 * whenTried is greater than the last value we saved.  If it is, then
 * we credit the repserver as retrieving a fileset.  If whenToTryAgain
 * is beyond or past now by more than MAX_WHEN_TO_TRY seconds, then
 * we go back to SITE_STATE_DOUPDATE and see if kicking the repserver
 * again will move us along.  If whenTried does not appear to be advancing,
 * then the repserver is probably not even getting to the point of trying
 * to forward a fileset.  In this case, since we have been sending
 * periodic REP_UpdateSelf() calls, we give up if whenTried is more than
 * 2 * MAX_WHEN_TO_TRY seconds in the past.  If whenTried is less than
 * the previous stored value, maybe the clock on the repserver went
 * backwards, or the repserver restarted.  We count this as a failure.
 */
static void fts_ProcessRepSite(sitep, volIdp, goalVVp)
ftsSiteStatus *sitep;
afs_hyper_t *volIdp;
afs_hyper_t *goalVVp;
{
    int state;
    struct sockaddr_in *sinp;
    error_status_t code, code2;
    u_long spare4, spare5;
    struct repStatus oneStat;
    u_long statFlags, newFlags;
    afs_hyper_t *curVV, *srcVV;
    time_t whenToTry;
    time_t lastTried;
    time_t now, nextTime;
    time_t interval;

    now = nextTime = osi_Time();

    if (now < sitep->nextCheck) {
	/*
	 * Not time to do anything yet.
	 */
	return;
    }

    state = sitep->siteState;
    sinp = sitep->repAddr;
    newFlags = sitep->flags;
    code = 0;

    VOLSERLOG(1, "Process %lu,,%lu at %s in state %d\n",
	      AFS_HGETBOTH(*volIdp), MapSockAddr(sinp), state);

    switch (state) {
      case SITE_STATE_START:

	state = SITE_STATE_DOBINDING;
	break;

      case SITE_STATE_DOBINDING:

	sitep->repHandle = connToServer((struct sockaddr *) sinp, NULL,
					SERVERKIND_REP);
	code = 0;
	if (sitep->repHandle) {
	    (void) fts_UseThisTimeout(sitep->repHandle,
				      rpc_c_binding_default_timeout);
	    rpc_mgmt_set_call_timeout(sitep->repHandle, 150 /* sec */, &code2);
	    state = SITE_STATE_DOUPDATE;
	    sitep->nFailures = 0;
	} else {
	    VOLSERLOG(1, "Unable to obtain an rpc binding handle for %s\n",
		      MapSockAddr(sinp));
	    nextTime += FAIL_WAIT_TIME;
	    sitep->nFailures++;
	}
	break;

      case SITE_STATE_DOUPDATE:

	/*
	 * First, see if we need to do this
	 */
	spare4 = spare5 = 0;

	code = REP_UpdateSelf(sitep->repHandle, &remoteCellID, volIdp,
			      0, /* flags, unused */
			      0, 0, 0, /* spare1, spare2, spare3 */
			      &spare4, &spare5);

	code = osi_errDecode(code);

	/*
	 * Reset times
	 */
	now = nextTime = osi_Time();

	if (code == 0) {
	    state = SITE_STATE_INPROGRESS;
	    sitep->updateTime = now;
	    sitep->nFailures = 0;
	    nextTime += 10;
	} else {

	    VOLSERLOG(1,
		      "REP_UpdateSelf for %lu,,%lu on %s failed: %s\n",
		      AFS_HGETBOTH(*volIdp),
		      MapSockAddr(sinp), dfs_dceErrTxt(code));

	    if (code == REP_ERR_NOVOLUME
		|| code == REP_ERR_NOTREPLICA
		|| code == REP_ERR_UNKNOWNREPLICA) {
		/* This is no longer a replication site */
		code = 0;
		state = SITE_STATE_COMPLETE;
	    } else {
		nextTime += FAIL_WAIT_TIME;
		sitep->nFailures++;
	    }
	}
	break;

      case SITE_STATE_INPROGRESS:

	/*
	 * At this point, we have sent the REP_UpdateSelf call, but we
	 * need to determine if the repserver is done, or is making
	 * or attempting to make progress.
	 *
	 * We know the repserver is done if the fileset is no longer
	 * managed by the repserver (REP_ERR_NOVOLUME, REP_ERR_NOTREPLICA)
	 *
	 * We know the repserver has at least gotten this version of
	 * the fileset if the current VV on the repserver is at least
	 * as large as the goalVV.
	 *
	 * Now, the hard part is determining if the repserver is making
	 * progress.
	 *
	 */

	bzero((void *)&oneStat, sizeof(oneStat));
	statFlags = 0;
	spare4 = spare5 = 0;
	code = REP_GetOneRepStatus(sitep->repHandle, &remoteCellID, volIdp,
				   0, 0, 0, /* spare1, spare2, spare3 */
				   &oneStat, &statFlags,
				   &spare4, &spare5);
	code = osi_errDecode(code);

	/*
	 * Reset times
	 */
	now = nextTime = osi_Time();

	if (code != 0) {
	    VOLSERLOG(1,
		      "REP_GetOneRepStatus for %lu,,%lu on %s failed: %s\n",
		      AFS_HGETBOTH(*volIdp), MapSockAddr(sinp),
		      dfs_dceErrTxt(code));
	    if (code == REP_ERR_NOVOLUME
		|| code == REP_ERR_NOTREPLICA
		|| code == REP_ERR_UNKNOWNREPLICA) {
		/* This is no longer a replication site */
		code = 0;
		state = SITE_STATE_COMPLETE;
	    } else {
		/*
		 * This may have been a transient failure, use a smaller
		 * time.
		 */
		nextTime += 60;
		sitep->nFailures++;
		/*
		 * Let the global processing handle whether to
		 * give up on this, but clear IS_RETRIEVING since
		 * we can not be sure.
		 */
		newFlags &= ~(IS_RETRIEVING);
	    }
	    break; /* Out of the case */
	} else {
	    sitep->nFailures = 0;
	}

	(void) strncpy((char *)sitep->repMsg, (char *)oneStat.RepMsg,
		       REPMSG_SIZE);

	/* Look at oneStat and see if we are done */

	curVV = &oneStat.curVV;
	srcVV = &oneStat.srcVV;

	VOLSERLOG(1,
		  "%lu,,%lu on %s goalVV %lu,,%lu srcVV %lu,,%lu curVV %lu,,%lu\n",
		  AFS_HGETBOTH(*volIdp), MapSockAddr(sinp),
		  AFS_HGETBOTH(*goalVVp), AFS_HGETBOTH(*srcVV),
		  AFS_HGETBOTH(*curVV));

	if (!AFS_hiszero(*goalVVp) && !AFS_hiszero(*srcVV)
	    && AFS_hcmp(*curVV, *goalVVp) >= 0) {
	    /*
	     * The repserver has a VV at least as large as the one for
	     * which we were trying.
	     */
	    state = SITE_STATE_COMPLETE;
	    break; /* Out of the case */
	}

	/*
	 * Try to determine if we are making progress
	 */
	whenToTry = oneStat.whenToTryAgain.sec;
	/*
	 * This is the time of the last fileset dump attempt
	 */
	lastTried = oneStat.whenTried.sec;


	VOLSERLOG(1, "%lu,,%lu at %s last %ld, again %ld\n",
		  AFS_HGETBOTH(*volIdp), MapSockAddr(sinp),
		  lastTried, whenToTry);
	VOLSERLOG(1, "%lu,,%lu at %s msg: %s\n",
		  AFS_HGETBOTH(*volIdp), MapSockAddr(sinp), sitep->repMsg);

	if (whenToTry == 0) {
	    /*
	     * The repserver may be in the process of doing something
	     */
	    nextTime += 10;
	    if (lastTried > sitep->lastTried) {
		/*
		 * The repserver has tried a fileset dump since we last
		 * checked.
		 */
		sitep->lastTried = lastTried;
		sitep->nAttempts++;
		newFlags |= IS_RETRIEVING;
	    } else if (lastTried == sitep->lastTried) {
		/*
		 * Re-assert this in case there were earlier failures
		 * with REP_GetOneRepStatus()
		 */
		newFlags |= IS_RETRIEVING;
	    } else if (lastTried < sitep->lastTried) {
		/*
		 * Either the clock rolled backwards, or we just can not
		 * tell what is happening, treat this as a failure.
		 */
		newFlags &= ~(IS_RETRIEVING);
		sitep->nFailures++;
	    }
	} else {
	    /*
	     * An update attempt is scheduled either for the future, or in
	     * the past.  Bound how long we will wait before kicking this
	     * again.
	     */
	    interval = ((whenToTry > now)
			? (whenToTry - now) : (now - whenToTry));

	    if (interval > MAX_WHEN_TO_TRY) {
		/*
		 * It is a long time in the future, or past, so kick it
		 * again.  This may have the side effect of advancing
		 * whenToTryAgain, but if this repserver is still working,
		 * the REP_UpdateSelf() call should otherwise be a no-op.
		 * Avoid doing the REP_UpdateSelf() too frequently.
		 */
		state = SITE_STATE_DOUPDATE;
		nextTime += MAX_WHEN_TO_TRY;
	    } else {
		/*
		 * Check again soon
		 */
		nextTime += 15;
	    }

	    /*
	     * We do not necessarily know if it is currently retrieving
	     * or not.  Claim that it is if the lastTried time is greater.
	     */
	    if (lastTried > sitep->lastTried) {
		sitep->lastTried = lastTried;
		sitep->nAttempts++;
		newFlags |= IS_RETRIEVING;
	    } else if (lastTried == 0
		       || (lastTried < sitep->updateTime)) {
		/*
		 * The repserver appears to not have tried a fileset
		 * retrieval since our last REP_UpdateSelf(), track this.
		 * Give up if it appears to be too long.  Give it about
		 * 2 REP_UpdateSelf() chances.
		 */
		newFlags &= ~(IS_RETRIEVING);
		if ((lastTried + (2 * MAX_WHEN_TO_TRY)) < sitep->updateTime) {
		    newFlags |= GIVE_UP;
		}
	    }
	}
	break;

      case SITE_STATE_COMPLETE:
	break;

      default:
	/* This should not happen */
	break;
    }

    if (state == SITE_STATE_COMPLETE) {
	if (code == 0) {
	    dce_svc_printf(FTS_S_WAIT_REPLICATION_DONE_MSG,
			   AFS_HGETBOTH(*volIdp), MapSockAddr(sinp));
	} else {
	    dce_svc_printf(FTS_S_WAIT_REPLICATION_SITE_FAILED_MSG,
			   AFS_HGETBOTH(*volIdp),
			   MapSockAddr(sinp), dfs_dceErrTxt(code));
	}
    } else if ((newFlags & (IS_RETRIEVING|GIVE_UP)) == GIVE_UP) {
	dce_svc_printf(FTS_S_WAIT_GIVING_UP_ON_MSG,
		       AFS_HGETBOTH(*volIdp), MapSockAddr(sinp),
		       ((sitep->code != 0) ? ": " : ""),
		       ((sitep->code != 0)
			? (char *)dfs_dceErrTxt(sitep->code) : ""));

	if (sitep->repMsg[0] != '\0') {
	    dce_svc_printf(FTS_S_WAIT_LAST_REPSEVER_MESSSAGE_MSG,
			   sitep->repMsg);
	}
	state = SITE_STATE_COMPLETE;
	/*
	 * This should be as good as any other error
	 */
	code = FTS_ERR_BADRELEASE;
    }

    VOLSERLOG(1, "Will check %lu,,%lu at %s in %ld seconds\n",
	      AFS_HGETBOTH(*volIdp), MapSockAddr(sinp),
	      (nextTime - now));
    VOLSERLOG(1,
	      "Process end %lu,,%lu at %s att=%d fail=%d state=%d flags=%lu\n",
	      AFS_HGETBOTH(*volIdp), MapSockAddr(sinp),
	      sitep->nAttempts, sitep->nFailures, state, newFlags);

    sitep->siteState = state;
    sitep->code = code;
    sitep->flags = newFlags;
    sitep->nextCheck = nextTime;
}

/*
 * Attempt to analyze the global progress of the replica update
 * resetAll means that all of the non-complete sites should be given
 * a fresh oppurtunity to start.

 * After a pass processing 0 or more repservers, we call
 * fts_AnalyzeRepProgress() to examine the global state of the world.
 * Whenever fts_ProcessRepSite() marks a site as complete, we reset
 * all of the counters to give the other repservers a longer chance to
 * move along.
 *
 * In the other case, we first find the site marked as IS_RETRIEVING
 * with the longest apparent retrieval time.  If there are no working
 * sites, then we give up on any sites which have reached the maximum
 * number of attempts or failures, or who were marked with GIVE_UP flag.
 *
 * If there is at least one working site, then if its retrieval time
 * has been more than MAX_RETRIEVAL_GRACE seconds, we give it one shot
 * to complete and flag it with GIVE_UP, which is deferred until this
 * retrieval attempt is done.  The number of attempts of everyone else
 * is set to 0 since their failures may be because the fileset is busy.
 *
 * If a working site is retrieving and has reached its maximum number
 * of attempts, then we defer giviing up on this repserver and set
 * the GIVE_UP.
 *
 * Finally, if a repserver is not retrieving and has the GIVE_UP flag set,
 * then we give up on it.
 */
static void fts_AnalyzeRepProgress(listp, volIdp, resetAll)
ftsSiteList *listp;
afs_hyper_t *volIdp;
int resetAll;
{
    ftsSiteStatus *sitep;
    ftsSiteStatus *workingSitep;
    time_t now;
    time_t interval, longestWorking;
    int retrieving, giveUp;
    int state;

    now = osi_Time();
    workingSitep = NULL;

    for (sitep = listp->root.nextSite; sitep != NULL;
         sitep = sitep->nextSite) {

	if (sitep->siteState == SITE_STATE_COMPLETE) {
	    continue;
	}

	if (resetAll) {
	    /*
	     * Reset all of the counters
	     */
	    sitep->nAttempts = 0;
	    sitep->nFailures = 0;
	    continue;
	}

	/*
	 * Find the longest working site that is retrieving a fileset
	 */
	retrieving = ((sitep->siteState == SITE_STATE_INPROGRESS) &&
		      (sitep->flags & IS_RETRIEVING) == IS_RETRIEVING);

	if (retrieving) {
	    interval = now - sitep->lastTried;
	    if (interval > longestWorking) {
		longestWorking = interval;
		workingSitep = sitep;
	    }
	}
    }

    if (resetAll) {
	/*
	 * No more work to do
	 */
	return;
    }


    /*
     * Now decide if we need to give up on any sites
     */
    for (sitep = listp->root.nextSite; sitep != NULL;
         sitep = sitep->nextSite) {

	if (sitep->siteState == SITE_STATE_COMPLETE) {
	    continue;
	}

	giveUp = 0;

	if (workingSitep == NULL) {
	    /*
	     * There are no working sites, give up on any that have
	     * tried MAX_ATTEMPTS times, failed MAX_FAILURES, or are
	     * marked GIVE_UP.
	     */
	    state = sitep->siteState;

	    if (sitep->nAttempts >= MAX_ATTEMPTS
		|| sitep->nFailures >= MAX_FAILURES
		|| (sitep->flags & GIVE_UP) == GIVE_UP) {
		giveUp = 1;
	    }
	} else {
	    /*
	     * There is at least 1 retrieving site.  We need to somehow
	     * distinguish between sites which are making progress,
	     * sites which tried to retrieve the fileset and it was
	     * busy (because another site is retrieving it) and sites
	     * which are not making any progress at all.
	     */

	    retrieving = (sitep->siteState == SITE_STATE_INPROGRESS
			  && (sitep->flags & IS_RETRIEVING) == IS_RETRIEVING);

	    if (retrieving
		&& (now - workingSitep->lastTried) > MAX_RETRIEVAL_GRACE) {
		/*
		 * Give this guy one shot to complete, and reset everyone
		 * else number of attempts.
		 */
		if (sitep == workingSitep) {
		    sitep->flags |= GIVE_UP;
		} else if (retrieving) {
		    sitep->nAttempts = 0;
		}
	    } else if (retrieving && sitep->nAttempts >= MAX_ATTEMPTS) {
		/*
		 * Do not give up now, but defer until the next notice
		 * of a change in the retrieval status
		 */
		sitep->flags |= GIVE_UP;
	    } else if (!retrieving && (sitep->flags & GIVE_UP) == GIVE_UP) {
		giveUp = 1;
	    }
	}

	if (giveUp) {
	    dce_svc_printf(FTS_S_WAIT_GIVING_UP_ON_MSG,
			   AFS_HGETBOTH(*volIdp),
			   MapSockAddr(sitep->repAddr),
			   (sitep->code != 0) ? ": " : "",
			   (sitep->code != 0)
			   ? (char *)dfs_dceErrTxt(sitep->code) : "");

	    if (sitep->repMsg[0] != '\0') {
		dce_svc_printf(FTS_S_WAIT_LAST_REPSEVER_MESSSAGE_MSG,
			       sitep->repMsg);
	    }
	    sitep->siteState = SITE_STATE_COMPLETE;
	    sitep->code = FTS_ERR_BADRELEASE;
	    listp->nComplete++;
	    listp->anyFailed = 1;
	}
    }
}
