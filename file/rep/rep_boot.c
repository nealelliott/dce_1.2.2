/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rep_boot.c,v $
 * $EndLog$
 */
/*
 * (C) Copyright 1996, 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

/* rep_boot.c: startup/reboot code. */

#include <dcedfs/param.h>		/* General system parameters */
#include <dcedfs/osi.h>
#include <dcedfs/basicincludes.h>	/* Another basic include file set */
#include <dcedfs/compat.h>
#include <dcedfs/lock.h>
#include <dcedfs/tkm_tokens.h>		/* token types */
#include <dcedfs/nubik.h>
#include <dcedfs/afs4int.h>		/* afsFid */
#include <dcedfs/tkn4int.h>		/* tkn-revocation interface */
#include <dcedfs/fldb_data.h>
#include <dcedfs/fldb_proc.h>
#include <dcedfs/flserver.h>		/* flserver interface */
#include <dcedfs/vol_init.h>		/* volser's syscall */
#include <dcedfs/vol_errs.h>
#include <dcedfs/ag_init.h>		/* aggregate syscall */
#include <dcedfs/ftserver.h>
#include <dcedfs/ftserver_data.h>
#include <dcedfs/ftserver_proc.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/ftu_syscalls.h>
#include <dcedfs/ftutil.h>

/* SAMS support */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsrpsmac.h>
#include <dfsrpssvc.h>
#include <dfsrpsmsg.h>

#include <pthread.h>
#include <dcedfs/ktime.h>
#if defined(AFS_HPUX_ENV)
#include <fcntl.h>
#endif /* defined(AFS_HPUX_ENV) */
#include <dce/rpc.h>
#include <dcedfs/rep_errs.h>		/* error code values */
#include <dcedfs/rep_data.h>		/* repser RPC structures and interface */
#include <dcedfs/rep_proc.h>

#include <repser.h>			/* internal structures */
#include <rep_trace.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/rep/RCS/rep_boot.c,v 4.83 1996/07/15 17:48:50 gait Exp $")

long EnumerateVLDB(againP)
int *againP;
{/* Scan the VLDB for entries pertinent to our work. */
    struct replica *rp, *temprp;
    long code;
    int addrIx, nents, Ix;
    int32 mySite;
    bulkentries VEntries;
    struct vldbentry *ep;
    struct VldbListByAttributes AttributeChoice;
    struct vldbRepInfo vldbInfo;
    vlconf_cell myCellInfo;
    char cellWithFS[MAXCELLCHARS+100];
    u_long cookie, nextcookie, flg;
    unsigned long st;

    *againP = 1;
    strcpy(cellWithFS, localCellName);
    code = strlen(cellWithFS);
    dfs_GetJunctionName(localCellName, 0, &cellWithFS[code],
			 sizeof(cellWithFS) - code - 1, &st);
    if (st != 0) {
	sprintf(ErrBuff, "Can't determine the DFS junction for the local cell name: %s.  Exiting.",
		dfs_dceErrTxt(st));
	Log("%s", ErrBuff);
	*againP = 0;
	return (long)st;
    }
    code = ubik_ClientInit(NULL, 
			   (unsigned_char_t *)cellWithFS, 
			   (unsigned_char_t *)"subsys/dce/dfs-fs-servers", 0, 1,
			    1, (struct ubik_client **)&VLDBcstruct);
    if (code) {
	sprintf(ErrBuff, "%s: ubik client init failed (%d, %s).\n",
		WhoIAm, code, dfs_dceErrTxt(code));
	Log("%s", ErrBuff);
	*againP = 0;
	return code;
    }
    icl_Trace0(rep_iclSetp, REP_FLDB_CONN_GET);
    
    /* Client-side RPC initialization is all done. */

    for (;;) {
	/* Init the localCellID here since the EnumerateVLDB caller will do retries. */
	code = ubik_Call(VL_GetCellInfo, VLDBcstruct, 0, &myCellInfo);
	if (code == 0)
	    break;
	if (code != 0) {
	    if (code == UNOQUORUM
		|| code == UNOSERVERS
		|| code == VL_INITNEEDSWRITE) {
		sprintf(ErrBuff, "%s: FL server not yet up for VL_GetCellInfo: %s",
			WhoIAm, dfs_dceErrTxt(code));
		Log("%s", ErrBuff);
	    } else {
		sprintf(ErrBuff, "%s: trouble with VL_GetCellInfo: %s",
			WhoIAm, dfs_dceErrTxt(code));
		Log("%s", ErrBuff);
	    }
	    if (code == UNOQUORUM
		|| code == VL_INITNEEDSWRITE) {
		sleep(30);
	    } else {
		*againP = 0;
		return code;
	    }
	}
    }
    localCellID = myCellInfo.CellID;

    for (addrIx = 0; addrIx < numMyNetAddrs; ++addrIx) {
	/* discover all volumes in the VLDB on myNetAddrs[addrIx] */
	bzero((char *)&AttributeChoice, sizeof(AttributeChoice));
	AttributeChoice.site = myNetAddrs[addrIx];
	AttributeChoice.flag = VLF_LAZYREP;	/* must be on (others OK too) */
	AttributeChoice.Mask = VLLIST_SITE | VLLIST_FLAG;

	for (cookie = flg = 0; (flg & 1) == 0; cookie = nextcookie) {
	    bzero((char *)&VEntries, sizeof(VEntries));	/* hint to stub to allocate space */
	    code = ubik_Call(VL_ListByAttributes, VLDBcstruct, 0, &AttributeChoice, cookie,
			     &nents, &VEntries, &nextcookie, &flg);
	    if (code != error_status_ok) {
		if (code == VL_ENDOFLIST) break;
		if (code == UNOQUORUM
		    || code == UNOSERVERS
		    || code == VL_INITNEEDSWRITE) {
		    sprintf(ErrBuff, "%s: FL server not yet up for VL_ListByAttributes: %s",
			WhoIAm, dfs_dceErrTxt(code));
		} else {
		    sprintf(ErrBuff, "%s: trouble with VL_ListByAttributes: %s",
			WhoIAm, dfs_dceErrTxt(code));
		}
		Log("%s", ErrBuff);
		if (code != UNOQUORUM
		    && code != VL_INITNEEDSWRITE) {
		    *againP = 0;
		}
		return code;
	    }
	    ep = (struct vldbentry *) &VEntries.bulkentries_val[0];
	    for (Ix = 0; Ix < nents; ++Ix, ++ep) {
		/* ep points to a VLDB entry that's on this server somewhere. */
		/* Incorporate it into what we know we should be doing. */
		code = AddVLDBEntry(ep, &vldbInfo, &mySite);
		if (code == REP_ERR_NOSTORAGE) {
		    sprintf(ErrBuff, "%s: no memory to copy a VLDB structure",
			WhoIAm);
		    Log(ErrBuff);
		    *againP = 0;
		    return code;
		}
		if (code == 0 && mySite >= 0) {
		    icl_Trace1(rep_iclSetp, REP_VLDB_ENT_FOUND, 
			       ICL_TYPE_HYPER, &vldbInfo.VolIDOfSecondary);
		    temprp = NewReplicaStruct();
		    if (temprp == NULL) {
			sprintf(ErrBuff, "%s: no memory for a new replica struct", WhoIAm);
			Log("%s", ErrBuff);
			*againP = 0;
			return -1;
		    }
		    code = VLDBCopy(&vldbInfo, temprp, mySite, ErrBuff, 0);
		    osi_Free((opaque) vldbInfo.Reps, vldbInfo.MaxReps * sizeof(struct RepSite));
		    if (code != 0) {
			osi_Free((opaque) temprp, sizeof(struct replica));
			Log("%s", ErrBuff);
			if (code == ENOMEM) {
			    *againP = 0;
			    return -1;
			}
			else continue;
		    }
		    /* See if we have this replica going already */
		    lock_ObtainShared(&RepTableLock);
		    for (rp = RepRoot; rp != NULL; rp = rp->Next) {
			if (rp->MyReplicaAggregate == temprp->MyReplicaAggregate
			    && AFS_hsame(rp->v.VolIDOfSecondary, temprp->v.VolIDOfSecondary)) {
			    /* This one is already in the to-do list. */
			    if (temprp->v.Reps != NULL)
				osi_Free((opaque) temprp->v.Reps,
					 temprp->v.MaxReps * sizeof(struct RepSite));
			    osi_Free((opaque) temprp, sizeof(struct replica));
			    icl_Trace0(rep_iclSetp, REP_ALREADY_HANDLING);
			    break;
			}
		    }
		    if (rp == NULL) {	/* wasn't a duplicate; add it. */
			lock_UpgradeSToW(&RepTableLock);
			AFS_hzero(temprp->VVOnSource);
			temprp->Next = RepRoot;
			RepRoot = temprp;
			++numReplicas;
			temprp->RefCount = 1;
			lock_ReleaseWrite(&RepTableLock);
			Log("FLDB finds: '%s', %lu,,%lu, aggr %lu",
			    temprp->v.mainVolName,
			    AFS_HGETBOTH(temprp->v.VolIDOfSecondary),
			    temprp->MyReplicaAggregate);
		    } else {
			lock_ReleaseShared(&RepTableLock);
		    }
		}
	    }
	}
    }
    return 0;
}

static long GetOneVolStat(vidp, agid, statp)
afs_hyper_t *vidp;
unsigned long agid;
struct vol_status *statp;
{/* Get volume status from vidp to statp for initial looking around. */
    int volD;
    long code;
    u_long TimeoutTime;
    struct timeval waittime;

    /* At initialization time, we have to wait for a busy fileset to clear. */
    TimeoutTime = osi_Time() + 12*60;	/* wait 12 minutes at most */
    for (;;) {
	code = ftu_AggrOpenFset(agid, vidp, VOL_SYS_GETSTATUS,
				VOLERR_TRANS_REPGETSTATUS, &volD);
	if (code == 0)
	    break;
	icl_Trace2(rep_iclSetp, REP_CANT_OPEN_LVOL, 
		   ICL_TYPE_HYPER, vidp,
		   ICL_TYPE_STRING, dfs_dceErrTxt(code));
	/* If it's a normal 8-bit code, it's not recoverable. */
	if ((code & ~(0xff)) == 0 || code == FTU_E_NO_SUCH_FSET)
	    return code;
	/* Check for timeout. */
	if (osi_Time() > TimeoutTime) {
	    sprintf(ErrBuff,
		    "GetOneVolStat: can't open local vol %lu,,%lu: %s",
		    AFS_HGETBOTH(*vidp), dfs_dceErrTxt(code));
	    Log("%s", ErrBuff);
	    return code;
	}
	sprintf(ErrBuff, "GetOneVolStat: can't open local vol %lu,,%lu: %s; sleeping...",
		AFS_HGETBOTH(*vidp), dfs_dceErrTxt(code));
	Log("%s", ErrBuff);
	bzero((char *)&waittime, sizeof(waittime));
	waittime.tv_sec = 30;	/* wait 30 seconds and try again */
	if (select(0, 0, 0, 0, &waittime))
	    sleep(1);
    }
    code = ftu_FsetGetStatus(volD, statp);
    if (code != 0) {
	icl_Trace2(rep_iclSetp, REP_CANT_GETSTAT_LVOL, 
		   ICL_TYPE_HYPER, vidp,
		   ICL_TYPE_STRING, dfs_dceErrTxt(code));
	(void) ftu_AggrAbortFsetOpen(volD);
	return code;
    }
    code = ftu_AggrCloseFset(volD);
    if (code != 0) {
	icl_Trace2(rep_iclSetp, REP_CANT_CLOSE_LVOL, 
		   ICL_TYPE_HYPER, vidp,
		   ICL_TYPE_STRING, dfs_dceErrTxt(code));
    } else {
	icl_Trace2(rep_iclSetp, REP_GOT_STATES,
		   ICL_TYPE_HYPER, vidp,
		   ICL_TYPE_LONG, statp->vol_st.states);
    }
    return (code);
}

static long KillInitVolume(vidp, agid)
afs_hyper_t *vidp;
unsigned long agid;
{/* Destroy the incomplete volume at init time, since it's under our management. */
    long volD, code;
    u_long TimeoutTime;
    struct timeval waittime;

    /* At initialization time, we have to wait for a busy fileset to clear. */
    TimeoutTime = osi_Time() + 12*60;	/* wait 12 minutes at most */
    for (;;) {
	code = ftu_AggrDestroyFset(agid, vidp);
	if (code == 0)
	    break;
	icl_Trace2(rep_iclSetp, REP_CANT_DESTROY_LVOL, 
		   ICL_TYPE_HYPER, vidp,
		   ICL_TYPE_STRING, dfs_dceErrTxt(code));
	/* If it's a normal 8-bit code, it's not recoverable. */
	if ((code & ~(0xff)) == 0 || code == FTU_E_NO_SUCH_FSET)
	    return code;
	/* Check for timeout. */
	if (osi_Time() > TimeoutTime) {
	    sprintf(ErrBuff, "KillInitVolume: can't destroy local vol %lu,,%lu: %s",
		    AFS_HGETBOTH(*vidp), dfs_dceErrTxt(code));
	    Log("%s", ErrBuff);
	    return code;
	}
	sprintf(ErrBuff, "KillInitVolume: can't destroy local vol %lu,,%lu: %s; sleeping...",
		AFS_HGETBOTH(*vidp), dfs_dceErrTxt(code));
	Log("%s", ErrBuff);
	bzero((char *)&waittime, sizeof(waittime));
	waittime.tv_sec = 30;	/* wait 30 seconds and try again */
	if (select(0, 0, 0, 0, &waittime))
	    sleep(1);
    }
    return 0;
}

long EnumerateLocalVolumes()
{/* Just what it says.  Describe all local volumes in our RepRoot list structure. */
    register struct replica *rp;
    register struct localvol *lv;
    struct localvol *lvRoot;
    struct localvol *llv, *nlv, *xlv;
    long agIx, maxAgg, nextIx;
    long code, numAggrs;
    struct aggrList *someAggrs;
    long  bytesUsed;
    struct io_rwDesc ioDesc;
    struct vol_status volStat;
    afs_hyper_t vidBuff;

    lvRoot = NULL;
    code = AG_ENUMERATE(sizeof(bytesUsed), &bytesUsed, &agIx);
    if (code == 0) {
	sprintf(ErrBuff, "%s: no aggregates on this server", WhoIAm);
	Log("%s", ErrBuff);
	dce_svc_printf(RPS_S_LOG_NO_EXIT_MSG,ErrBuff);
	/* don't exit--just continue, in the expectation of aggrs to appear later. */
	return 0;
    } else if (code != E2BIG) {
	sprintf(ErrBuff, "%s: unexpected AG_ENUMERATE return (%ld)", WhoIAm, code);
	Log("%s", ErrBuff);
	dce_svc_printf(RPS_S_LOG_EXIT_MSG,ErrBuff);
	return -1;
    }
    numAggrs = (bytesUsed / sizeof(struct aggrList)) + 5;	/* allow for creation race */
    someAggrs = (struct aggrList *) osi_Alloc(numAggrs * sizeof(struct aggrList));
    if (someAggrs == 0) {
	sprintf(ErrBuff, "%s: cannot allocate %ld struct aggrList buffs",
		WhoIAm, numAggrs);
	Log("%s", ErrBuff);
	dce_svc_printf(RPS_S_LOG_EXIT_MSG,ErrBuff);
	return -1;
    }
    code = AG_ENUMERATE(numAggrs * sizeof(struct aggrList), someAggrs,
			&bytesUsed);
    if (code != 0) {
	sprintf(ErrBuff, "%s: AG_ENUMERATE: %s",
		WhoIAm, dfs_dceErrTxt(code));
	Log("%s", ErrBuff);
	dce_svc_printf(RPS_S_LOG_EXIT_MSG,ErrBuff);
	return code;
    }
    maxAgg = bytesUsed / sizeof(struct aggrList);
    icl_Trace2(rep_iclSetp, REP_SCAN_AGGR,
	       ICL_TYPE_STRING, WhoIAm, 
	       ICL_TYPE_LONG, maxAgg);
    for (agIx = 0; agIx < maxAgg; ++agIx) {
	if (!AG_TYPE_SUPPORTS_EFS(someAggrs[agIx].type)) {
	    icl_Trace4(rep_iclSetp, REP_SKIP_NONLFS,
		       ICL_TYPE_STRING, WhoIAm,
		       ICL_TYPE_STRING, someAggrs[agIx].name, 
		       ICL_TYPE_LONG, someAggrs[agIx].Id,
		       ICL_TYPE_LONG,  someAggrs[agIx].type);
	    continue;
	}
	icl_Trace4(rep_iclSetp, REP_SCAN_LFS,
		   ICL_TYPE_STRING, WhoIAm,
		   ICL_TYPE_STRING, someAggrs[agIx].name, 
		   ICL_TYPE_LONG, someAggrs[agIx].Id,
		   ICL_TYPE_LONG,  someAggrs[agIx].type);
	bzero((char *)&ioDesc, sizeof(ioDesc));
	ioDesc.descId = someAggrs[agIx].Id;
	ioDesc.length = sizeof(volStat);
	ioDesc.bufferp = (char *) &volStat;
	ioDesc.position = 0;
	for (;;) {
	    code = AG_VOLENUMERATE(1, &ioDesc, &nextIx, &bytesUsed);
	    if (code != 0) {
		if (code == VOL_ERR_EOF) break;
		if (code == ENOENT) {
		    /* ioDesc.position = nextIx; */
		    ioDesc.position++;
		    continue;
		}
		if (code == ENODEV) code = FTSERVER_ERR_BADAGGREGATE;
		sprintf(ErrBuff,
			"%s: AG_VOLENUMERATE (aggr ix %ld, %s, ID %ld, type %ld) error %ld (%s)",
			WhoIAm, agIx, someAggrs[agIx].name, someAggrs[agIx].Id,
			someAggrs[agIx].type, code, dfs_dceErrTxt(code));
		Log("%s", ErrBuff);
		dce_svc_printf(RPS_S_LOG_EXIT_MSG,ErrBuff);
		return code;
	    }
	    if (ioDesc.position == nextIx)
		/* empty aggregate. */
		break;
	    if (bytesUsed != sizeof(volStat)) {
		sprintf(ErrBuff,
			"%s: AG_VOLENUMERATE (aggr ix %ld, %s, ID %ld, type %ld) returns wrong size (%ld rather than %ld bytes)",
			WhoIAm, agIx, someAggrs[agIx].name, someAggrs[agIx].Id,
			someAggrs[agIx].type, bytesUsed, sizeof(volStat));
		Log("%s", ErrBuff);
		dce_svc_printf(RPS_S_LOG_EXIT_MSG,ErrBuff);
		return -1;
	    }
	    /* The vol_st fields will be filled in pretty much OK. */
	    /* Certainly the volId and the states fields will be as they come from the disk. */
	    vidBuff = volStat.vol_st.volId;
	    if (((volStat.vol_st.states & VOL_REPSERVER_MGD) != 0)
		&& (((volStat.vol_st.states & VOL_REPFIELD) == VOL_REP_RELEASE)
		    || ((volStat.vol_st.states & VOL_REPFIELD) == VOL_REP_LAZY))
		&& (((volStat.vol_st.states & VOL_TYPEFIELD) == VOL_TYPE_RO)
		    || ((volStat.vol_st.states & VOL_TYPEFIELD) == VOL_TYPE_TEMP))) {
		/* OK, it's one of ours.  Do active management of it. */
		if (AFS_hiszero(volStat.vol_st.volId)
		    || !(volStat.vol_st.states & VOL_IS_COMPLETE)
		    || (volStat.vol_st.states &
			(VOL_OFFLINE | VOL_DELONSALVAGE
			 | VOL_OUTOFSERVICE | VOL_DEADMEAT
			 | VOL_ZAPME)) != 0) {
		    Log("Initialization destroying %lu,,%lu, aggr %lu (states %#lx)",
			AFS_HGETBOTH(vidBuff), someAggrs[agIx].Id,
			volStat.vol_st.states);
		    code = KillInitVolume(&vidBuff, someAggrs[agIx].Id);
		    if (code != 0) {
			sprintf(ErrBuff,
				"%s: VOL_DESTROY (aggr ix %ld, %s, ID %ld, type %ld, id %lu,,%lu) returns failure %d",
				WhoIAm, agIx, someAggrs[agIx].name,
				someAggrs[agIx].Id, someAggrs[agIx].type,
				AFS_HGETBOTH(vidBuff), code);
			Log("%s", ErrBuff);
			dce_svc_printf(RPS_S_LOG_NO_EXIT_MSG,ErrBuff);
			ioDesc.position = nextIx; /* skip it */
		    }
		    continue;
		}
		code = GetOneVolStat(&vidBuff, someAggrs[agIx].Id, &volStat);
		if (code != 0) {
		    sprintf(ErrBuff,
			    "%s: VOL_GETSTATUS (aggr ix %ld, %s, ID %ld, type %ld, id %lu,,%lu) returns failure %d",
			    WhoIAm, agIx, someAggrs[agIx].name, someAggrs[agIx].Id,
			    someAggrs[agIx].type, AFS_HGETBOTH(vidBuff), code);
		    Log("%s", ErrBuff);
		    dce_svc_printf(RPS_S_LOG_NO_EXIT_MSG,ErrBuff);
		    ioDesc.position = nextIx;
		    continue;
		}
		if (AFS_hsame(volStat.vol_st.volId, volStat.vol_dy.llBackId)) {
		    Log("Initialization destroying %lu,,%lu, aggr %lu, backing %lu,,%lu, fwd %lu,,%lu",
			AFS_HGETBOTH(vidBuff), someAggrs[agIx].Id,
			AFS_HGETBOTH(volStat.vol_dy.llBackId),
			AFS_HGETBOTH(volStat.vol_dy.llFwdId));
		    code = KillInitVolume(&vidBuff, someAggrs[agIx].Id);
		    if (code != 0) {
			sprintf(ErrBuff,
				"%s: VOL_DESTROY (aggr ix %ld, %s, ID %ld, type %ld, id %lu,,%lu) returns failure %d",
				WhoIAm, agIx, someAggrs[agIx].name, someAggrs[agIx].Id,
				someAggrs[agIx].type, AFS_HGETBOTH(vidBuff), code);
			Log("%s", ErrBuff);
			dce_svc_printf(RPS_S_LOG_NO_EXIT_MSG,ErrBuff);
			ioDesc.position = nextIx; /* skip it */
		    }
		    continue;
		}
		icl_Trace4(rep_iclSetp, REP_GOT_LVOL_MANG,
			   ICL_TYPE_STRING, WhoIAm,
			   ICL_TYPE_HYPER, &volStat.vol_st.volId,
			   ICL_TYPE_STRING, volStat.vol_st.volName,
			   ICL_TYPE_LONG, volStat.vol_st.states);
		lv = (struct localvol *) osi_Alloc(sizeof(struct localvol));
		if (lv == NULL) {
		    sprintf(ErrBuff, "%s: no memory for a new localvol struct.",
			    WhoIAm);
		    Log("%s", ErrBuff);
		    dce_svc_printf(RPS_S_LOG_EXIT_MSG,ErrBuff);
		    return -1;
		}
		/* set fields in *lv, then link it in. */
		bzero((char *)lv, sizeof(struct localvol));
		lv->volId = volStat.vol_st.volId;
		lv->aggr = someAggrs[agIx].Id;
		CopyLVStatus(lv, &volStat);
		lock_ObtainWrite(&RepTableLock);
		for (rp = RepRoot; rp != NULL; rp = rp->Next) {
		    if (AFS_hsame(lv->volId, rp->v.VolIDOfSecondary))
			break;
		    if (AFS_hsame(lv->llBackId, rp->v.VolIDOfSecondary))
			break;
		    if (AFS_hsame(lv->llFwdId, rp->v.VolIDOfSecondary))
			break;
		}
		if (rp != NULL) {	/* add to rep, if there is one */
		    lv->Next = rp->AllLocals;
		    rp->AllLocals = lv;
		    Log("Disk scan: attaching %lu,,%lu (aggr %lu, backing %lu,,%lu, fwd %lu,,%lu) to rep '%s'",
			AFS_HGETBOTH(lv->volId), lv->aggr,
			AFS_HGETBOTH(lv->llBackId),
			AFS_HGETBOTH(lv->llFwdId), rp->v.mainVolName);
		} else {		/* else just append to list of things to deal with later */
		    lv->Next = lvRoot;
		    lvRoot = lv;
		}
		lock_ReleaseWrite(&RepTableLock);
	    }
	    ioDesc.position = nextIx;
	}
    }
    osi_Free((opaque) someAggrs, numAggrs * sizeof(struct aggrList));
    icl_Trace1(rep_iclSetp, REP_SCAN_LVOL_FIN,
	       ICL_TYPE_STRING, WhoIAm);
    /* First, add the volumes that have a llBackId to the set of replicas to manage. */
    llv = NULL;
    for (lv = lvRoot; lv != NULL; lv = nlv) {
	nlv = lv->Next;
	if (AFS_hiszero(lv->llBackId)) {
	    /* probably somebody's junk volume--save for a minute */
	    llv = lv;
	} else {
	    /* a volume that appears to have been managed by us before. */
	    rp = NewReplicaStruct();
	    if (rp == NULL) {
		osi_Free((opaque) lv, sizeof(struct localvol));
		sprintf(ErrBuff, "%s: no memory for a new replica struct.", WhoIAm);
		Log("%s", ErrBuff);
		dce_svc_printf(RPS_S_LOG_EXIT_MSG,ErrBuff);
		return -1;
	    }
	    /* set some key fields, then link it in. */
	    icl_Trace3(rep_iclSetp, REP_ADD_DISC_VOL,
		       ICL_TYPE_STRING, WhoIAm,
		       ICL_TYPE_HYPER, &lv->volId,
		       ICL_TYPE_HYPER, &lv->llBackId);
	    rp->v.VolIDOfSecondary = lv->volId;
	    rp->MyReplicaAggregate = lv->aggr;
	    rp->Flags = 0;
	    if ((lv->itsStates & VOL_REPFIELD) == VOL_REP_RELEASE)
		rp->Flags |= repflag_ReleaseOnly;
	    rp->RefCount = 1;
	    lock_ObtainWrite(&RepTableLock);
	    rp->Next = RepRoot;
	    RepRoot = rp;
	    ++numReplicas;
	    lv->Next = rp->AllLocals;
	    rp->AllLocals = lv;
	    lock_ReleaseWrite(&RepTableLock);
	    Log("Disk finds: %lu,,%lu, aggr %lu, backing %lu,,%lu, fwd %lu,,%lu",
		AFS_HGETBOTH(rp->v.VolIDOfSecondary),
		rp->MyReplicaAggregate, AFS_HGETBOTH(lv->llBackId),
		AFS_HGETBOTH(lv->llFwdId));
	    /* delete this one from the singly-linked list */
	    if (llv == NULL) {
		lvRoot = nlv;
	    } else {
		llv->Next = nlv;
	    }
	}
    }
    /* For each leftover that doesn't have a llBackId, see if there's now a llBackId in
     the set of active replicas that matches its volId. */
    llv = NULL;
    for (lv = lvRoot; lv != NULL; lv = nlv) {
	nlv = lv->Next;
	lock_ObtainWrite(&RepTableLock);
	for (rp = RepRoot; rp != NULL; rp = rp->Next) {
	    for (xlv = rp->AllLocals; xlv != NULL; xlv = xlv->Next) {
		if (AFS_hsame(lv->volId, xlv->llBackId)) break;
	    }
	    if (xlv != NULL) break;	/* it's on the list */
	}
	if (rp == NULL) {
	    /* this one's volId doesn't match any llBackId */
	    llv = lv;			/* this gets left behind */
	} else {			/* attach it here */
	    /* this one is the backing volume of some published volume */
	    icl_Trace3(rep_iclSetp, REP_TR_ATTACH,
		       ICL_TYPE_STRING, WhoIAm,
		       ICL_TYPE_HYPER, &lv->volId,
		       ICL_TYPE_HYPER, &rp->v.VolIDOfSecondary);
	    Log("Disk scan: Attaching junk %lu,,%lu (aggr %lu, backing %lu,,%lu, fwd %lu,,%lu) to '%s'",
		AFS_HGETBOTH(lv->volId), lv->aggr,
		AFS_HGETBOTH(lv->llBackId),
		AFS_HGETBOTH(lv->llFwdId),
		rp->v.mainVolName);
	    lv->Next = rp->AllLocals;
	    rp->AllLocals = lv;
	    if (rp->JunkP == NULL) rp->JunkP = lv;
	    if (llv == NULL) {		/* and delete it from the singly-linked list */
		lvRoot = nlv;
	    } else {
		llv->Next = nlv;
	    }
	}
	lock_ReleaseWrite(&RepTableLock);
    }
    /* For the rest, create replica records for them and let VLDB lookup clean 'em up. */
    for (lv = lvRoot; lv != NULL; lv = nlv) {
	nlv = lv->Next;
	rp = NewReplicaStruct();
	if (rp == NULL) {
	    osi_Free((opaque) lv, sizeof(struct localvol));
	    sprintf(ErrBuff, "%s: no memory for a new replica struct.", WhoIAm);
	    Log("%s", ErrBuff);
	    dce_svc_printf(RPS_S_LOG_EXIT_MSG,ErrBuff);
	    return -1;
	}
	/* set some key fields, then link it in. */
	icl_Trace2(rep_iclSetp, REP_TR_ADD,
		   ICL_TYPE_STRING, WhoIAm,
		   ICL_TYPE_HYPER, &lv->volId);
	AFS_hzero(rp->VVOnSource);
	rp->v.VolIDOfSecondary = lv->volId;
	rp->MyReplicaAggregate = lv->aggr;
	rp->Flags = 0;
	if ((lv->itsStates & VOL_REPFIELD) == VOL_REP_RELEASE)
	    rp->Flags |= repflag_ReleaseOnly;
	Log("Disk finds junk: %lu,,%lu, aggr %lu, backing %lu,,%lu, fwd %lu,,%lu",
	    AFS_HGETBOTH(rp->v.VolIDOfSecondary),
	    rp->MyReplicaAggregate, AFS_HGETBOTH(lv->llBackId),
	    AFS_HGETBOTH(lv->llFwdId));
	rp->RefCount = 1;
	lock_ObtainWrite(&RepTableLock);
	rp->Next = RepRoot;
	RepRoot = rp;
	++numReplicas;
	lv->Next = rp->AllLocals;
	rp->AllLocals = lv;
	lock_ReleaseWrite(&RepTableLock);
    }
    icl_Trace1(rep_iclSetp, REP_RATNL_LVOL_FIN,
	       ICL_TYPE_STRING, WhoIAm);
    return 0;
}
