/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volc_queue.c,v $
 * Revision 1.1.125.1  1996/10/02  21:15:27  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:47  damon]
 *
 * $EndLog$
*/
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/common_data.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/stds.h>
#include <dcedfs/basicincludes.h>
#include <dce/rpc.h>
#include <dcedfs/fldb_data.h>
#include <dcedfs/ftserver_data.h>
#include <dcedfs/flserver.h>
#include <dcedfs/ftserver.h>

#include <dcedfs/volume.h>
#include <dcedfs/aggr.h>
#include <volc.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftimac.h>
#include <dfsftisvc.h>
#include <dfsftimsg.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/userInt/fts/volc_queue.c,v 1.1.125.1 1996/10/02 21:15:27 damon Exp $")

static int GroupEntries();

/*
 * The queue structure used by this module
 */
struct qvolEntry {
    char		name[FTSERVER_MAXFSNAME];
    afs_hyper_t		ids[MAXTYPES];
    afsTimeval		copyDate[MAXTYPES];
    int			isValid[MAXTYPES];
    u_long		vStates[MAXTYPES];
    struct		qvolEntry *next;
};


/*
 * Head of the queue
 */
struct qHead {
    int			count;
    int                 fsetBusy;
    struct qvolEntry	*next;
};

/*
 * Initialize queue pointed by <headp>
 */
static q_Init(headp)
    struct qHead *headp;
{
    headp->count = 0;
    headp->next = (struct qvolEntry *)0;
    headp->fsetBusy = 0;
}


static int q_GetTypeFromStatus(statusp)
ftserver_status *statusp;
{
    register unsigned long typeFld;

    typeFld = statusp->vss.states & VOL_TYPEFIELD;
    if (typeFld) {
	switch (typeFld) {
	    case VOL_TYPE_RW:
		return RWVOL;
	    case VOL_TYPE_RO:
		return ROVOL;
	    case VOL_TYPE_BK:
		return BACKVOL;
	}
    }
    if (statusp->vss.type == VOL_RW)
	return RWVOL;
    /* No sub-type given.  Maybe a raw Episode fileset.  If R/W, this will be OK. */
    if (statusp->vss.states & VOL_RW)
	return RWVOL;
    return -1;	/* can't guess! */
}

/*
 * Returns 1 if it finds queue entry with a newType volume equal to <volIdp>,
 * else 0
 */
static q_Find(headp, volIdp, elempp, newType)
    struct qHead *headp;
    afs_hyper_t *volIdp;
    struct qvolEntry **elempp;
    long newType;
{   
    struct qvolEntry *qvolp = headp->next;

    while (qvolp) {
	if (!AFS_hcmp(qvolp->ids[newType], *volIdp)) {
	    *elempp = qvolp;
	    return 1;
	}
	qvolp = qvolp->next;
    }
    return 0;
}



/*
 * Add <elem> in front of queue <headp> 
 */
static q_Add(headp, qvolp)
    struct qHead *headp;
    struct qvolEntry *qvolp;
{
    struct qvolEntry *tqvolp;

    if (!headp->count) {
	headp->next = qvolp;
	qvolp->next = (struct qvolEntry *)0;
    } else {
	tqvolp = headp->next;
	headp->next = qvolp;
	qvolp->next = tqvolp;
    }
    headp->count++;
}


/*
 * return the element in the beginning of the queue <headp>, free
 * the space used by that element; 1 is returned if removal was ok, else 0.
 */
static q_Remove(headp, qvolp)
    struct qHead *headp;
    struct qvolEntry *qvolp;
{
    int i;
    struct qvolEntry *tqvolp;

    if (headp->count > 0) {
	headp->count--;
	tqvolp = headp->next;
	headp->next = headp->next->next;
	strncpy(qvolp->name, tqvolp->name, FTSERVER_MAXFSNAME);
	for (i = 0; i < MAXTYPES; i++) {
	    qvolp->ids[i] = tqvolp->ids[i];
	    qvolp->copyDate[i] = tqvolp->copyDate[i];
	    qvolp->isValid[i] = tqvolp->isValid[i];
	    qvolp->vStates[i] = tqvolp->vStates[i];
	}
	qvolp->next = (struct qvolEntry *)0;
	free(tqvolp);
	return 1;
    }
    return 0;
}


/*
 * Print out the entries for the queue, headp.
 */
static q_Traverse(headp)
    struct qHead *headp;
{
    int count = headp->count, i;
    struct qvolEntry *old = headp->next, *new;

    if (old)
      new = old->next;
    else
      new = NULL;

    dce_fprintf(stderr, fts_s_queue_internal_queue);
    while (count >  0) {
	dce_fprintf(stderr, fts_s_queue_old_name, old->name);
	for (i = 0; i < MAXTYPES; ++i) {
	    dce_fprintf(stderr, fts_s_queue_voltypes, 
			volTypes[i], AFS_HGETBOTH(old->ids[i]));
	    if (old->isValid[i])
		dce_fprintf(stderr, fts_s_queue_valid);
	    else 
		dce_fprintf(stderr, fts_s_queue_invalid);
	}
	dce_fprintf(stderr, fts_s_queue_newline_newline);
	old = new;
	if (count != 1) 
	    new = new->next;
	count--;
    }
}

static long  match(entryp, qvolp)
    struct qvolEntry *qvolp;
    struct vldbentry *entryp;
{
    int i,j;

    for (i = 0; i < MAXTYPES; i++)
    {
	for (j = 0; j < MAXVOLTYPES; j++)
	{
	    if (AFS_hsame(qvolp->ids[i], entryp->VolIDs[j]) &&
		(VOLTIX_TO_VOLTYPE(i) == entryp->VolTypes[j]))
	    {
		return 1;
	    }
	}
    }
    return (0);
}

static int InQ(entry, headp)
    struct vldbentry *entry;
    struct qHead *headp;
{
    struct qvolEntry *q = headp->next;

    while (q != NULL)
    {
	if (match(entry,q))
	    return 1;
	q = q->next;
    }
    return 0;
}

/*
 * Delete FLDB entries if the fileset is not in the queue
 */
static long deleteFLDBsNotInQ(headp, servAddrp, aggrId,numDid, numFailed)
    struct qHead *headp;
    struct sockaddr *servAddrp;
    unsigned long aggrId;
    unsigned int *numDid;
    unsigned int *numFailed;
{
    struct VldbListByAttributes attributes;
    bulkentries Entries;
    u_long cookie, nextcookie, flg;
    struct vldbentry entry;
    int i, j;
    long code, nentries;

    *numDid = *numFailed = 0;
    VOLSERLOG(1, "Deleting fileset entries for filesets not found on aggr %d of server %s\n",
	      aggrId, MapSockAddr(servAddrp));
    /* 
     * First get the list of filesets that the FLDB thinks live here 
     */
    attributes.partition = aggrId;
    *((struct sockaddr *) &attributes.site) = *servAddrp;
    attributes.Mask = VLLIST_SITE | VLLIST_PARTITION;

    for (cookie = flg = 0; (flg & 1) == 0; cookie = nextcookie) {

	bzero((char *)&Entries, sizeof(bulkentries));
	if (code = vldb_ListByAttributes(&attributes, cookie, &nentries,
					 &Entries, &nextcookie, &flg)) {
	    if (code == VL_ENDOFLIST) break;
	    dce_svc_printf(FTS_S_QUEUE_NO_FLDB_ENTRIES_MSG);
	    return code;
	}
	for (j = 0; j < nentries; ++j) {
	    int xl[MAXTYPES];
	    entry= Entries.bulkentries_val[j];
	    if (!InQ(&entry, headp)){
		fts_GetIndices(&entry,xl);
		VOLSERLOG(1, "Removing this location from %s's entry\n",
			  entry.name);
		(*numDid)++;
		for (i = 0; i < MAXTYPES; ++i) {
		    if (xl[i] >=0) {
			if (code = vldb_RemoveVolume(&entry, 
						     servAddrp, aggrId, i, 
						     &(entry.VolIDs[xl[i]]))) 
			{
			    dce_svc_printf(FTS_S_QUEUE_REMOVE_FAILED_MSG,
					   code);
			    (*numFailed)++;
			}
		    }
		}
	    }
	}
    }
}

/*
 * Called by VC_SyncVldb to put all volumes in a queue for processing.
 * GroupEntries and ProcessEntries (see below) do all the dirty work.
 */
static long ProcessEntries(); /* forward declaration */

q_HandleEntries(connp, servAddrp, aggrId, statusp, count, agtype)
    register rpc_binding_handle_t connp;
    unsigned long aggrId;
    register struct sockaddr *servAddrp;
    ftserver_status *statusp;
    long count, agtype;
{
    long code;
    struct qHead myQueue;

    q_Init(&myQueue);
    if (code = GroupEntries(connp, statusp, count, &myQueue, aggrId))
	return code;
    code = ProcessEntries(&myQueue, servAddrp, aggrId, agtype);
    return code;
}


/* Implement a procedure by which the code for ``fts syncfldb'' can pass in a handful of
    fileset status entries at a time, and process them when they're all added. */
static struct qHead localQueue;

int q_StartEntryGroup()
{
    q_Init(&localQueue);
    return 0;
}

long q_AddToGroup(connp, servAddrp, aggrId, statusp, count, agtype)
    register rpc_binding_handle_t connp;
    unsigned long aggrId;
    register struct sockaddr *servAddrp;
    ftserver_status *statusp;
    long count, agtype;
{
    return GroupEntries(connp, statusp, count, &localQueue, aggrId);
}

long q_ProcessGroup(connp, servAddrp, aggrId, statusp, count, agtype)
    register rpc_binding_handle_t connp;
    unsigned long aggrId;
    register struct sockaddr *servAddrp;
    ftserver_status *statusp;
    long count, agtype;
{
    long code;

    code = ProcessEntries(&localQueue, servAddrp, aggrId, agtype);
    q_Init(&localQueue);	/* re-init */
    return code;
}


/*
 * Useful macros for the following functions
 */
#define q_AssignType(queuep, newType, statusp) \
    (queuep)->isValid[(newType)] = 1; \
    (queuep)->ids[(newType)] = (statusp)->vss.volId; \
    (queuep)->copyDate[(newType)] = (statusp)->vsd.copyDate; \
    (queuep)->vStates[(newType)] = (statusp)->vss.states;

static void q_FillEntry(queuep, newType, statusp) 
    struct qvolEntry *queuep;
    int newType;
    ftserver_status *statusp;     
{
    q_AssignType(queuep, newType, statusp);
    if (newType != RWVOL) {
	if (!AFS_hiszero(statusp->vss.parentId))
	    queuep->ids[RWVOL] = statusp->vss.parentId;
    }
    if (newType != BACKVOL &&
	!AFS_hiszero(statusp->vsd.backupId) &&
	AFS_hiszero(queuep->ids[BACKVOL]))
	queuep->ids[BACKVOL] = statusp->vsd.backupId;
    if (newType != ROVOL &&
	!AFS_hiszero(statusp->vsd.cloneId) &&
	AFS_hiszero(queuep->ids[ROVOL]))
	queuep->ids[ROVOL] = statusp->vsd.cloneId;
    if (statusp->vss.volName[0] != '/')
	strcpy(queuep->name, ExtractSuffix((char *)statusp->vss.volName));
    queuep->next = (struct qvolEntry *)0; 
}

#ifdef undef
/* Remove all entries corresponding to a particular aggregate Id */
void q_RemoveAggrFsets(aggrId)
     unsigned long aggrId;
{
  int count = localQueue.count, i;
  struct qvolEntry *curr = localQueue.next, *prev = NULL, *qtemp ;

  
  if (verbose)
    fprintf(stderr, "Deleting all filesets on aggregate id %lu \n", aggrId);
  while (count > 0)  {
    if (curr->aggrid == aggrId) {
      /* delete this fileset from list */
      if (verbose) {
	fprintf(stderr, "%s AggrID = %lu ", curr->name, curr->aggrid);
	for (i = 0; i < MAXTYPES; ++i) {
	  fprintf(stderr, "%sID %lu,,%lu",
		  volTypes[i], AFS_HGETBOTH(curr->ids[i]));
	  if (curr->isValid[i])
	    fprintf(stderr, " valid ");
	  else 
	    fprintf(stderr, " invalid ");
	}
      };

      if (prev == NULL) 
	localQueue.next = curr->next;
      else 
	prev->next = curr->next;

      qtemp = curr->next; 
      (void) free (curr);
      curr = qtemp;

      localQueue.count--;  
    } else {
      prev = curr;
      curr = curr->next;
    };
      
    count--;
  }
}
#endif

static int q_CheckRepInfo(entryp, qvolp)
    register struct vldbentry *entryp;
    register struct qvolEntry *qvolp;
{
    register int ix;
    int xl[MAXTYPES];
    unsigned long rwflag, roflag;
    u_long totalStates;
    u_long minSiteAge, maxSiteAge;
    int repStyle, minAgeIx, maxAgeIx;

    /* If we already have replication info, no problem. */
    if (entryp->flags & VLF_LAZYREP) return 0;

    /* See if it needs replication info. */
    fts_GetIndices(entryp, xl);
    if (xl[ROVOL] < 0) return 0;

    roflag = ((unsigned32)VLSF_ZEROIXHERE) >> xl[ROVOL];

    for (ix = 0; ix < entryp->nServers; ++ix) {
	if (entryp->siteFlags[ix] & roflag)
	    break;
    }
    if (ix >= entryp->nServers) {
	/* No sites marked as R/O, but check the global flags. */
	if ((entryp->flags & VLF_ROEXISTS) == 0)
	    return 0;	/* OK--no need. */
    }
    /*
     * We've now determined that the FLDB entry has no replication
     * information, but that there's either one or more R/O sites defined,
     * or the VLF_ROEXISTS flag is set.
     * Try extracting what we can from the qvolp.
     */
    if (xl[RWVOL] >= 0) {
	/* Get an index to which we can refer; if no R/W, the R/O is OK */
	rwflag = ((unsigned32)VLSF_ZEROIXHERE) >> xl[RWVOL];
	for (ix = 0; ix < entryp->nServers; ++ix) {
	    if (entryp->siteFlags[ix] & rwflag)
		break;
	}
    }
    VOLSERLOG(0, "Fileset entry '%s' has read-only data, but no\n",
	      entryp->name);
    VOLSERLOG(0, "  replication information has been set in the FLDB.\n");
    totalStates = 0;
    for (ix = 0; ix <= BACKVOL; ++ix) {
	totalStates |= qvolp->vStates[ix];
    }
    if ((totalStates & VOL_REPFIELD) == VOL_REP_RELEASE) {
	VOLSERLOG(0, "Re-instating release-style replication, but with default parameters.\n");
	VOLSERLOG(0, "Use ``fts setrepinfo'' to reset these parameters.\n");
	entryp->flags |= VLF_LAZYREP | VLF_LAZYRELE;
	entryp->maxTotalLatency = 7200;	/* two hours */
	entryp->hardMaxTotalLatency = 86400;	/* 24 hours */
	entryp->minimumPounceDally = 300;	/* 5 minutes */
	entryp->defaultMaxReplicaLatency = 1800;	/* 30 minutes */
	entryp->reclaimDally = 64800;	/* 18 hours */
	for (ix = 0; ix < entryp->nServers; ++ix) {
	    if (entryp->siteFlags[ix] & VLSF_SAMEASPREV) continue;
	    if ((entryp->siteFlags[ix] & roflag) == 0) continue;
	    entryp->sitemaxReplicaLatency[ix] = 1800; /* 30 minutes */
	}
	return 1;
    } else if ((totalStates & VOL_REPFIELD) == VOL_REP_LAZY) {
	VOLSERLOG(0, "The fileset had earlier been marked for scheduled replication.\n");
	/* Figure min/max maxsiteage fields for those entries that have values. */
	minAgeIx = maxAgeIx = -1;
	for (ix = 0; ix < entryp->nServers; ++ix) {
	    if (entryp->siteFlags[ix] & VLSF_SAMEASPREV) continue;
	    if ((entryp->siteFlags[ix] & roflag) == 0) continue;
	    if (entryp->sitemaxReplicaLatency[ix] != 0) {
		if (minAgeIx < 0 ||
		    (entryp->sitemaxReplicaLatency[ix] < minSiteAge)) {
		    minAgeIx = ix;
		    minSiteAge = entryp->sitemaxReplicaLatency[ix];
		}
		if (maxAgeIx < 0 ||
		    (entryp->sitemaxReplicaLatency[ix] > maxSiteAge)) {
		    maxAgeIx = ix;
		    maxSiteAge = entryp->sitemaxReplicaLatency[ix];
		}
	    }
	}
	if (minAgeIx < 0 || (maxSiteAge < 7200 && minSiteAge > 300)) {
	    VOLSERLOG(0, "Re-instating scheduled replication, but with default parameters.\n");
	    VOLSERLOG(0, "Use ``fts setrepinfo'' to reset these parameters.\n");
	    entryp->flags |= VLF_LAZYREP;
	    entryp->flags &= ~VLF_LAZYRELE;
	    entryp->maxTotalLatency = 7200;	/* two hours */
	    entryp->hardMaxTotalLatency = 86400;	/* 24 hours */
	    entryp->minimumPounceDally = 300;	/* 5 minutes */
	    entryp->defaultMaxReplicaLatency = 1800;	/* 30 minutes */
	    entryp->reclaimDally = 64800;	/* 18 hours */
	    for (ix = 0; ix < entryp->nServers; ++ix) {
		if (entryp->siteFlags[ix] & VLSF_SAMEASPREV) continue;
		if ((entryp->siteFlags[ix] & roflag) == 0) continue;
		if (entryp->sitemaxReplicaLatency[ix] == 0)
		    entryp->sitemaxReplicaLatency[ix] = 1800; /* 30 minutes */
	    }
	    return 1;
	} else {
	    VOLSERLOG(0, "Cannot use default parameters for scheduled replication.\n");
	    VOLSERLOG(0, "Please use the 'setrepinfo' subcommand on this fileset.\n");
	    return 0;
	}
    }
    VOLSERLOG(0, "There is no clearcut replication marking on the fileset.\n");
    VOLSERLOG(0, "Please use the 'setrepinfo' subcommand on this fileset.\n");
    return 0;
}

static int q_UpdateQueueEntry(connp, qvolp, statusp, aggrId, newType)
    rpc_binding_handle_t connp;
    struct qvolEntry *qvolp;
    ftserver_status *statusp;   
    unsigned long aggrId;
    int newType;
{
    long code = 0;

    if (qvolp->isValid[newType]) {
	if (newType == RWVOL) {
	    dce_svc_printf(FTS_S_QUEUE_MULTIPLE_RW_VOL_MSG, statusp->vss.volName, aggrId);
	    code = FTS_ERR_MULTIRWVOL;
	} else {
	    /* 
	     * Do Time checks: delete superflous volume 
	     */
	    if (qvolp->copyDate[newType].sec > statusp->vsd.copyDate.sec) {   /* XXXXX */
		/*	
		 * Delete the present volume (hack!)
		 */
		code = fts_DeleteVolume(connp, aggrId, &statusp->vss.volId, 1, 0);
	    } else {
		/* 
		 * Delete the older volume after making sure that the current
		 * one is ok
		 */
		if (code = fts_VolumeExists(connp, aggrId, &qvolp->ids[newType])) {
		    if (code == EEXIST)
			code = fts_DeleteVolume(connp, aggrId, &statusp->vss.volId,
					    1, 0);	
		}
		q_AssignType(qvolp, newType, statusp);
	    }
	}
    } else {
	q_AssignType(qvolp, newType, statusp);
	if (newType != BACKVOL &&
	    !qvolp->isValid[BACKVOL] &&
	    !AFS_hiszero(statusp->vsd.backupId))
	    qvolp->ids[BACKVOL] = statusp->vsd.backupId;
	if (newType != ROVOL &&
	    !qvolp->isValid[ROVOL] &&
	    !AFS_hiszero(statusp->vsd.cloneId))
	    qvolp->ids[ROVOL] = statusp->vsd.cloneId;
    }
    return code;
}



/*
 * Group all the volume entries on <aggrId> by their parentid or by their ids
 * if the volume is rw. <count> is the number of entries to be processesd.
 * <statusp> points to the first entry. <myQueue> is the queue with entries
 * grouped 
 */
static int GroupEntries(connp, statusp, count, myQueue, aggrId)
    rpc_binding_handle_t connp;
    ftserver_status *statusp;   
    long count;
    struct qHead *myQueue;
    unsigned long aggrId;
{
    struct qvolEntry *qvolp;
    afs_hyper_t volId;
    long code = 0, fcode = 0, i, newType;

    VOLSERLOG(1, "Grouping %d fileset entries by their parent IDs on aggregate ID %d\n",
	       count, aggrId);

    for (i = 0; i < count; i++, statusp++) {
	if (statusp->vss.accError != 0) {
	    dce_svc_printf(FTS_S_QUEUE_FILESET_BUSY_MSG,
			   AFS_HGETBOTH(statusp->vss.volId),
			   dfs_dceErrTxt(statusp->vss.accError));

	    /* Flag as busy, but go on adding filesets */
	    myQueue->fsetBusy = 1;
	    continue;
#if 0
	    if (!fcode)	/* remember first error */
		fcode = statusp->vss.accError;

#endif
	} else if (statusp->vss.states & VOL_NOEXPORT) {
	    VOLSERLOG(1, "*** Omitting non-exported fileset %lu,,%lu\n",
		      AFS_HGETBOTH(statusp->vss.volId));
	} else {
	    /* 
	     * Entry is valid 
	     */
	    newType = q_GetTypeFromStatus(statusp);
	    if (newType < 0) {
		dce_svc_printf(FTS_S_QUEUE_UNKNOWN_FT_TYPE_MSG, statusp->vss.volName, statusp->vss.type,
			  statusp->vss.states, aggrId);
		continue;
	    }
	    if (newType == RWVOL) 
		volId = statusp->vss.volId;
	    else if (newType == ROVOL || newType == BACKVOL)
		volId = statusp->vss.parentId;
	    if (q_Find(myQueue, &volId, &qvolp, RWVOL)) {
		/*
		 * Update the existing queue entry
		 */
		if (code = q_UpdateQueueEntry(connp, qvolp, statusp, aggrId, newType))
		    if (!fcode) 
			fcode = code;	/* only remember 1st failure */
	    } else { 		
		/* 
		 * Create a fresh queue entry insert in the front of the queue
		 */
		qvolp = (struct qvolEntry *) malloc(sizeof(struct qvolEntry));
		bzero((char *)qvolp, sizeof(struct qvolEntry));
		q_FillEntry(qvolp, newType, statusp);
		q_Add(myQueue, qvolp);
	    }
	}
    }
    if (verbose) 
	q_Traverse(myQueue);
    return (PrintError(0, fcode));
}


/*
 * Sync vldb with all the entries on <myQueue> on <aserver> and <apart>
 */
static long ProcessEntries(myQueue, servAddrp, aggrId, agtype)
    struct qHead *myQueue;
    struct sockaddr *servAddrp;
    unsigned long aggrId;
    long agtype;
{   
    struct qvolEntry qvol;
    struct vldbentry entry;
    static afs_hyper_t maxdelta = AFS_HINIT(0, MAXBUMPCOUNT);
    static afs_hyper_t oldmaxdelta = AFS_HINIT(0, 100);
    afs_hyper_t maxVolid, delta;
    long i, code, fcode = 0, flags;
    unsigned int totalC = 0, totalU = 0, totalCE = 0, totalUE = 0;
    unsigned int totalG = 0, totalD, totalDE;
    int counter = 0, bumpCount;
    int xl[MAXTYPES];
    int j, k, thisIx;
    int voltype;

    /* can't delete filesets not in queue, if busy */
    if (!myQueue->fsetBusy)
	deleteFLDBsNotInQ(myQueue, servAddrp, aggrId, &totalD, &totalDE);
    if (!(totalG = myQueue->count))
	return 0;
    VOLSERLOG(1, "Processing queue entries for server %s aggregate Id %d ... \n",
	      MapSockAddr(servAddrp), aggrId);
    if (code = vldb_GetNewVolumeId(0, servAddrp, &maxVolid))
	return (code);
    while (q_Remove(myQueue, &qvol)) {
	counter++;
	VOLSERLOG(1, "-- Processing entry %d of total %d --\n", counter, totalG);
	if (!qvol.isValid[RWVOL] &&
	    !qvol.isValid[ROVOL] &&
	    !qvol.isValid[BACKVOL]) {
	    dce_svc_printf(FTS_S_QUEUE_INVALID_QUEUE_ENTRY_MSG);
	    continue;
	}
	for (i = BACKVOL; i >= RWVOL; --i) {
	    while (AFS_hcmp(maxVolid, qvol.ids[i]) <= 0) {
		delta = qvol.ids[i];
		AFS_hsub(delta, maxVolid);
		AFS_hadd32(delta, 1);
		if (AFS_hcmp(delta, maxdelta) > 0)
		    delta = maxdelta;
		bumpCount = AFS_hgetlo(delta);
		if (bumpCount == 0)
		    bumpCount = 1;
		dce_svc_printf(FTS_S_QUEUE_BUMP_MAXVOLID_MSG,
			       AFS_HGETBOTH(maxVolid), bumpCount,
			       AFS_HGETBOTH(qvol.ids[i]));
		if (code = vldb_GetNewVolumeId(bumpCount, servAddrp, &delta)) {
		    if ((code == VL_BADVOLIDBUMP) && !AFS_hsame(maxdelta, oldmaxdelta)) {
			/* Use the old maxbump value */
			VOLSERLOG(0, "Recovering: decreasing fsid increment\n");
			maxdelta = oldmaxdelta;
		    } else {
			return code;
		    }
		} else {
		    /* get a refreshed copy */
		    maxVolid = delta;
		    AFS_hadd32(maxVolid, bumpCount);
		}
	    }
	}

	for (voltype = RWVOL; voltype <= BACKVOL; voltype++) {
	    /* 
	     * Get voltype for which the ID is defined.
	     */
	    if (!AFS_hiszero(qvol.ids[voltype]))
		break;
	}

	if (voltype > BACKVOL) {
	    VOLSERLOG(0, "No defined fileset IDs!\n");
	    totalCE++;
	    code = FTS_ERR_INCONSISTENT;
	} else {
	    code = vldb_GetEntryByID(&qvol.ids[voltype], voltype, &entry, 1);
	    if (code && (code != VL_NOENT)) {
		dce_svc_printf(FTS_S_QUEUE_FETCH_FAILED_MSG,
			       AFS_HGETBOTH(qvol.ids[RWVOL]),
			       dfs_dceErrTxt(code));
		totalCE++;
	    } else if (code == VL_NOENT) {
		/* Set up a vldb entry for qvol */
		dce_svc_printf(FTS_S_QUEUE_CREATE_FILESET_ENTRY_MSG,
			       AFS_HGETBOTH(qvol.ids[RWVOL]));
		flags = 0;
		if (qvol.isValid[RWVOL]) flags |= VLF_RWEXISTS;
		if (qvol.isValid[ROVOL]) flags |= VLF_ROEXISTS;
		if (qvol.isValid[BACKVOL]) flags |= VLF_BACKEXISTS;
		if (qvol.name[0] == '\0') {
		    sprintf((char *)&qvol.name[0],
			    "SYNCFLDB-GENERATED-%lu-%lu",
			    AFS_HGETBOTH(qvol.ids[voltype]));
		}
		if (code = vldb_CreateVldbEntry(qvol.name, servAddrp, aggrId,
						flags, &qvol.ids[RWVOL],
						&qvol.ids[ROVOL],
						&qvol.ids[BACKVOL], 1, 0))
		    totalCE++;
		else {
		    totalC++;
		    if (vldb_GetEntryByID(&qvol.ids[voltype], voltype, &entry, 0) == 0) {
			vldb_SaveOldEntry(&entry);
			if (q_CheckRepInfo(&entry, &qvol)) {
			    code = vldb_ReplaceEntry(&entry, &qvol.ids[voltype],
						     voltype, LOCKREL_ALL);
			    if (code) {
				totalCE++;
				totalC--;
			    } else {
				(void) vldb_TellRepAboutNewEntry(&entry, 0, 1);
			    }
			}
		    }
		}
	    } else {
		/*  Update the existing entry */
		VOLSERLOG(1, "Trying to update the existing entry for %s ...\n",
			  entry.name);
		for (k = 0; k < MAXTYPES; ++k) xl[k] = -1;
		for (k = 0; k < MAXVOLTYPES && !AFS_hiszero(entry.VolIDs[k]); ++k) {
		    for (i = 0; i < MAXTYPES; ++i) {
			if (entry.VolTypes[k] == VOLTIX_TO_VOLTYPE(i)) {
			    xl[i] = k;
			    break;
			}
		    }
		}
		/* 'k' is left as the next vol-type slot that could be used */
		if (AG_TYPE_SUPPORTS_EFS(agtype)
		    || (qvol.name[0] != '/' && qvol.name[0] != '\0')) {
		    strncpy((char *)entry.name, qvol.name, sizeof(entry.name));
		}
		/* Assign all existing volids in the type array. */
		for (i = RWVOL; i <= BACKVOL; ++i) 
		    if (qvol.isValid[i] || !AFS_hiszero(qvol.ids[i])) {
			if (xl[i] < 0) {
			    entry.VolIDs[k] = qvol.ids[i];
			    entry.VolTypes[k] = VOLTIX_TO_VOLTYPE(i);
			    xl[i] = k++;
			    if (k < MAXVOLTYPES)
				AFS_hzero(entry.VolIDs[k]); /* terminate */
			}
			if (xl[i] >= 0
			    && !AFS_hsame(entry.VolIDs[xl[i]], qvol.ids[i])){
			    VOLSERLOG(0,
				"ID mismatch for type %s: fldb says %lu,,%lu but vol is %lu,,%lu\n",
				volTypes[i], AFS_HGETBOTH(entry.VolIDs[xl[i]]),
				      AFS_HGETBOTH(qvol.ids[i]));
			}
		    }
		/* Tackle a R/W fileset */
		if (qvol.isValid[RWVOL]) {
		    /* because of above loop, know that xl[RWVOL] is non-negative */
		    thisIx = vldb_FindRepSite(&entry, (struct sockaddr *)0, -1,
				  (((unsigned32)VLSF_ZEROIXHERE) >> xl[RWVOL]));
		    if (entry.flags & VLF_RWEXISTS) {
			/* check if sites match */
			if (thisIx < 0) {
			    /* This R/W not listed anywhere in the fldb entry */
			    dce_svc_printf(FTS_S_QUEUE_BAD_FLDB_ENTRY_MSG,
					   AFS_HGETBOTH(qvol.ids[RWVOL]));
			    vldb_EnsureSite(&entry, VLF_RWEXISTS,
					    (((unsigned32)VLSF_ZEROIXHERE) >> xl[RWVOL]),
					    servAddrp, aggrId, -1, 0);
			} else {
			    /* This R/W is listed in the fldb.  Make sure it
			     * points to this instance.
			     */
			    vldb_MoveSite(&entry, &entry.siteAddr[thisIx],
					  entry.sitePartition[thisIx],
					  servAddrp, aggrId, RWVOL);
			}
		    } else {		/* VLF_RWEXISTS is off */
			/* Check if a RW vol entry exists anyway */
			if (thisIx < 0) {
			    /* Nope: point FLDB to this instance */
			    vldb_EnsureSite(&entry, VLF_RWEXISTS,
					    (((unsigned32)VLSF_ZEROIXHERE) >> xl[RWVOL]),
					    servAddrp, aggrId, -1, 0);
			} else {
			    /* Obviously there is a R/W site: annotate the entry. */
			    vldb_AssignSite(&entry, VLF_RWEXISTS, 0, servAddrp, aggrId,
					    thisIx, 0);
			}
		    }
		} else if (entry.flags & VLF_RWEXISTS && !(myQueue->fsetBusy)
			   && !AFS_hiszero(qvol.ids[RWVOL])) {
		    /* If the fldb entry says the R/W is here, remove that pointer. */
		    code = vldb_RemoveVolume(&entry, servAddrp, aggrId, RWVOL,
					     &qvol.ids[RWVOL]);
		}
		/* Tackle a R/O fileset */
		if (qvol.isValid[ROVOL]) {
		    /* because of above loop, know that xl[ROVOL] is non-negative */
		    if (vldb_FindRepSite(&entry, servAddrp, aggrId,
					 ((unsigned32)VLSF_ZEROIXHERE) >> xl[ROVOL])
			== -1) {
			/* add this site */
			if (AFS_hsame(qvol.ids[ROVOL], entry.VolIDs[xl[ROVOL]])) {
			    vldb_EnsureSite(&entry, VLF_ROEXISTS,
					    ((unsigned32)VLSF_ZEROIXHERE) >> xl[ROVOL],
					    servAddrp, aggrId, -1, 0 /* XXX */);
			}
		    }
		}
		/* Tackle a Backup fileset */
		if (qvol.isValid[BACKVOL]) {
		    /* because of above loop, know that xl[BACKVOL] is non-negative */
		    thisIx = vldb_FindRepSite(&entry, (struct sockaddr *)0, -1,
					      ((unsigned32)VLSF_ZEROIXHERE) >> xl[BACKVOL]);
		    if (thisIx < 0) {
			/* But there's obviously an entry! */
			/* Nothing in FLDB: point FLDB to this instance */
			vldb_EnsureSite(&entry, VLF_BACKEXISTS,
					(((unsigned32)VLSF_ZEROIXHERE) >> xl[BACKVOL]),
					servAddrp, aggrId, -1, 0);
			/* Refresh our knowledge of backup location */
			thisIx = vldb_FindRepSite(&entry, (struct sockaddr *)0, -1,
					((unsigned32)VLSF_ZEROIXHERE) >> xl[BACKVOL]);
		    } else {
			if (entry.flags & VLF_BACKEXISTS) {
			    /* here's the entry--move descriptor if necessary */
			    vldb_MoveSite(&entry, &entry.siteAddr[thisIx],
					  entry.sitePartition[thisIx],
					  servAddrp, aggrId, BACKVOL);
			} else {
			    /* Entry-wide flag says that there's no backup */
			    /* Obviously there is a backup site: annotate the entry. */
			    vldb_EnsureSite(&entry, VLF_BACKEXISTS, 0, servAddrp, aggrId,
					    thisIx, 0);
			}
		    }
		    if (xl[RWVOL] >= 0 && (entry.flags & VLF_RWEXISTS) &&
			(i = vldb_FindRepSite(&entry, (struct sockaddr *)0, -1,
					      ((unsigned32)VLSF_ZEROIXHERE) >> xl[RWVOL])) != -1) {
			/* If this entry's RW isn't on this backvol's site, notify the user */
			if (!vldb_IsSiteHere(&entry, i, servAddrp, aggrId)) {
			    dce_svc_printf(FTS_S_QUEUE_SHOULD_DELETE_BACKUP_MSG,
					   qvol.name, aggrId,
					   MapSockAddr(servAddrp));
			}
		    }
		} else if (entry.flags & VLF_BACKEXISTS && !(myQueue->fsetBusy)
			   && !AFS_hiszero(qvol.ids[BACKVOL])) {
		    /* If the fldb entry says the backup is here, remove that pointer. */
		    code = vldb_RemoveVolume(&entry, servAddrp, aggrId, BACKVOL,
					     &qvol.ids[BACKVOL]);
		}
		vldb_SaveOldEntry(&entry);
		i = q_CheckRepInfo(&entry, &qvol);
		code = vldb_ReplaceEntry(&entry, &qvol.ids[voltype], voltype,
					 LOCKREL_ALL);
		if (i != 0 && code == 0)
		    (void) vldb_TellRepAboutNewEntry(&entry, 0, 1);

		if (verbose && !code) {
		    vldb_EnumerateEntry(&entry, "", 0);
		    dce_svc_printf(FTS_S_QUEUE_UPDATED_MSG);
		    totalU++;
		} else if (code) {
		    totalUE++;
		    fcode = FTS_ERR_BADOP;
		    (void) vldb_ReleaseLock(&qvol.ids[voltype], voltype, LOCKREL_ALL);
		} 
	    }
	}
	dce_svc_printf(FTS_S_QUEUE_DONE_WITH_ENTRY_MSG, counter, totalG);
    }

    VOLSERLOG(1, "Total entries created %u (failed %u); updated %u (failed %u); deleted %u (failed %u)\n",
	      totalC, totalCE, totalU, totalUE, totalD, totalDE);
    return fcode;
}
