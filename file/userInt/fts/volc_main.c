/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volc_main.c,v $
 * $EndLog$
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <dcedfs/debug.h>
#include <dcedfs/sysincludes.h>
#include <stdio.h>
#include <time.h>

#include <dcedfs/stds.h>
#include <dcedfs/compat.h>
#include <dcedfs/ftserver_proc.h>
#include <dcedfs/dfsauth.h>
#include <dcedfs/osi_dfserrors.h>

#include <dcedfs/queue.h> 
#include <dcedfs/volume.h>
#include <dcedfs/ioctl.h>
#include <dcedfs/nubik.h>
#include <dcedfs/fldb_data.h>
#include <dcedfs/fldb_proc.h>
#include <dcedfs/flserver.h>
#include <dcedfs/ftserver.h>
#include <dcedfs/ftserver_trans.h>
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>
#include <dcedfs/aggr.h>
#include <dcedfs/rep_data.h>
#include <dcedfs/rep_errs.h>
#include <dcedfs/ftutil.h>
#include <dcedfs/cm.h>
#include <volc.h>
#include <fts_userint.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftimac.h>
#include <dfsftisvc.h>
#include <dfsftimsg.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/userInt/fts/RCS/volc_main.c,v 12.12 1996/10/16 12:14:05 gait Exp $")

long fts_debug_flag = 0x0;
#define FTS_DEBUG_PARSE_MASK	1

#define	MAXSIZE	2048
PRIVATE char blob_space[MAXSIZE+1];

PRIVATE int doUbikCleanup = 0;   /* really a boolean */

dce_svc_handle_t   fts_svc_handle;

void initialize_svc()
{
  error_status_t st;

  fts_svc_handle = dce_svc_register(fts_svc_table,  (idl_char *)"fts",  &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dce_msg_define_msg_table(fts__table,
        sizeof fts__table / sizeof fts__table[0], &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Warning Cannot register fts incore message table 0x%lx\n", st);
}


/*
 * Call by qsort in ListVolumes to dump volumes sorted by name
 */
PRIVATE CompareVolName(p1,p2)
    const void *p1, *p2;
{
    ftserver_status *arg1, *arg2;

    arg1 = (ftserver_status *)p1;
    arg2 = (ftserver_status *)p2;
    return (strcmp((char *)arg1->vss.volName, (char *)arg2->vss.volName));
}

/*
 * Call by qsort in ListVolumes to dump volumes sorted by Id
 */
PRIVATE CompareVolID(p1,p2)
    const void *p1, *p2;
{
    ftserver_status *arg1, *arg2;

    arg1 = (ftserver_status *)p1;
    arg2 = (ftserver_status *)p2;
    return (AFS_hcmp(arg1->vss.volId, arg2->vss.volId));
}

/*
 * All aggregate information about the server (or the individual aggregate if one is 
 * specified) is returned in entriesp.
 */
PRIVATE int GetOptAggr(as, servAddrp, aggrListp, numAggrsp, offset)
     register struct cmd_syndesc *		as;
     register struct sockaddr *			servAddrp;
     register struct ftserver_aggrList **		aggrListp;
     register long *				numAggrsp;
     register int				offset;
{
    long aggrId, code;
    char aggrName[MAX_AGGRNAME];
    register struct ftserver_aggrList *aggrp;

    if (as->parms[offset].items) {
	aggrp = (struct ftserver_aggrList *) malloc(sizeof(struct ftserver_aggrList));
	if (aggrp == NULL) {
	    PrintError(as->name, ENOMEM);
	    return -1;
	}
	if (!GetAggr(as->parms[offset].items->data, servAddrp, GETAGGR_VALIDATE,
		     &aggrp->Id, (char *)&aggrp->name[0]))
	    return -1;
	*numAggrsp = 1;
	*aggrListp = aggrp;
	code = 0;
    } else {
	code = volc_Call(fts_GetAllAggrs, servAddrp, NULL, numAggrsp, aggrListp);
	if (code) PrintError(as->name, code);
    }
    return code;
}

/*
 *  "addsite <server> <aggr> <fileset> [maxreplatency] "
 */
EXPORT int fts_DoAddSite(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    afs_hyper_t volId;
    long code;
    u_long aggrId, maxreplat;
    struct sockaddr servAddr;
    char *volumeNamep = as->parms[ADDSITE_FILESET_OFFSET].items->data;
    char *aggrp = as->parms[ADDSITE_AGGREGATE_OFFSET].items->data;
    char *serverNamep = as->parms[ADDSITE_SERVER_OFFSET].items->data;
    char aggrName[MAX_AGGRNAME];

    if (!GetServer(serverNamep, &servAddr))				/* server */
	exit(1);
    if (!GetAggr(aggrp, &servAddr, GETAGGR_VALIDATE, &aggrId, aggrName))		/* aggr */
	exit(1);
    if (!GetVolumeID(volumeNamep, &volId))				/* volume */
	exit(1);
    maxreplat = 0;
    if (as->parms[ADDSITE_MAXREPL_OFFSET].items) {
	if (fts_strToIv(as->parms[ADDSITE_MAXREPL_OFFSET].items->data, &maxreplat))
	    exit(1);
    }
    code = fts_NeedAggrType(&servAddr, aggrId, AG_TYPE_ANY_EFS_MASK, "Replication is");
    if (code) exit(1);
    if (code = vldb_AddSite(&servAddr, aggrId, &volId, maxreplat))
	exit(1);
    dce_svc_printf(FTS_S_MAIN_REP_SITE_MSG, serverNamep, aggrName,
	      volumeNamep);
    return 0;
}

/*
 * "aggrinfo <server> [aggr]"
 */
EXPORT int fts_DoAggregateInfo(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{   
    long code = 0, i, numAggrs;
    struct sockaddr servAddr;
    struct ftserver_aggrInfo aggrInfo;
    struct ftserver_aggrList *aggrListp;
    register struct ftserver_aggrList *aggrp;
    char agName[sizeof(aggrInfo.name)+1];
    char agDevName[sizeof(aggrInfo.devName)+1];

    if (!GetServer(as->parms[AGGRINFO_SERVER_OFFSET].items->data, &servAddr)) /* server */
	exit(1);
    if (code = GetOptAggr(as, &servAddr,
			   &aggrListp, &numAggrs,
			   AGGRINFO_AGGREGATE_OFFSET)) /* [aggr] */
	exit(1);

    for (i = 0, aggrp = aggrListp; i < numAggrs; ++i, ++aggrp) {
	code = volc_Call(vols_AggregateInfo, &servAddr, NULL, aggrp->Id, &aggrInfo, 0);
	if (!code) {
	    /* Allow the aggrInfo fields to be unterminated. */
	    bcopy((char *)aggrInfo.name, agName, sizeof(aggrInfo.name));
	    agName[sizeof(aggrInfo.name)] = '\0';
	    bcopy((char *)aggrInfo.devName, agDevName, sizeof(aggrInfo.devName));
	    agDevName[sizeof(aggrInfo.devName)] = '\0';
	    dce_printf(fts_s_main_agg_type, AG_TYPE_TO_STR(aggrInfo.type));
	    dce_printf(fts_s_main_agg_name, agName);
	    if (strcmp(agName, agDevName) != 0)
		dce_printf(fts_s_main_agg_devname, agDevName);
	    dce_printf(fts_s_main_k_free,
		       aggrInfo.curFree, aggrInfo.totalUsable);
	    if (aggrInfo.minFree != 0) {
		dce_printf(fts_s_main_k_reserved, aggrInfo.minFree);
	    }
	    printf("\n");
	}
    }
    if (aggrListp)
	free(aggrListp);
    return code;
}

/*
 *  "backup <fileset>"
 */
EXPORT int fts_DoBackupVolume(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    afs_hyper_t volId;
    long aggrId, volType, code;
    struct sockaddr servAddr;
    struct vldbentry entry;

    if (!GetVolumeID(as->parms[CLONE_FILESET_OFFSET].items->data, &volId)) /* volume */
	exit(1);
    if (vldb_GetVldbInfo(&volId, &servAddr, &aggrId, &volType, &entry))
	exit(1);
    code = fts_NeedAggrType(&servAddr, aggrId, AG_TYPE_ANY_EFS_MASK, "'Clone' is");
    if (code) exit(1);
    if (code = volc_Call(VC_BackupVolume, &servAddr, NULL, aggrId, &volId))
	return (PrintError(as->name, code));
    dce_svc_printf(FTS_S_MAIN_CREATED_BACKUP_MSG, as->parms[CLONE_FILESET_OFFSET].items->data); 
    return 0;
}

/*
 * "backupsys <prefix> [server] [aggr]"
 */
EXPORT int fts_DoBackSys(as, aRock)
register struct cmd_syndesc *	as;
char *				aRock;
{   
    char prefix[FTSERVER_MAXFSNAME];
    char *prefixp;

    prefixp = (char *) NULL;
    if (as->parms[CLONESYS_PREFIX_OFFSET].items) {		/* <prefix> */
	strncpy(prefix, as->parms[CLONESYS_PREFIX_OFFSET].items->data,
		FTSERVER_MAXFSNAME);
	prefixp = prefix;
    }
    if (as->parms[CLONESYS_AGGREGATE_OFFSET].items
	 && !as->parms[CLONESYS_SERVER_OFFSET].items) {
	dce_svc_printf(FTS_S_MAIN_AG_NEEDS_SERVER_MSG, programName, as->name,
		  as->parms[CLONESYS_AGGREGATE_OFFSET].items->data);
	exit(1);
    }
    if (vldb_ProcessByAttrs((as->parms[CLONESYS_SERVER_OFFSET].items ?
			      as->parms[CLONESYS_SERVER_OFFSET].items->data : (char *)0),
			     (as->parms[CLONESYS_AGGREGATE_OFFSET].items ?
			      as->parms[CLONESYS_AGGREGATE_OFFSET].items->data : (char *)0),
			     0, prefixp, 2/*BULK_BACKUP*/))
	exit(1);
    return 0;
}

/*
 *  "create <volumeName> <server> <aggr>"
 */
EXPORT int fts_DoCreateVolume(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    long code;
    u_long aggrId;
    afs_hyper_t volId;
    struct sockaddr servAddr;
    struct vldbentry entry;
    char *volumeNamep = as->parms[CREATE_FSNAME_OFFSET].items->data;
    char *aggrp = as->parms[CREATE_AGGREGATE_OFFSET].items->data;
    char *serverNamep = as->parms[CREATE_SERVER_OFFSET].items->data;
    char aggrName[MAX_AGGRNAME];

    if (!GetServer(serverNamep, &servAddr))			/* server */
	exit(1);
    if (!GetAggr(aggrp, &servAddr, GETAGGR_VALIDATE, &aggrId, aggrName))	/* aggr */
	exit(1);
    if (InvalidVolName(volumeNamep)) 			/* volumeName */
	exit(1);
    if (!vldb_GetEntryByName(volumeNamep, &entry, 1)) {
	dce_svc_printf(FTS_S_MAIN_FILESET_EXISTS_MSG, programName, as->name, volumeNamep);
	exit(1);
    }
    code = fts_NeedAggrType(&servAddr, aggrId, AG_TYPE_ANY_EFS_MASK, "'Create' is");
    if (code)
	exit(1);
    AFS_hzero(volId);
    if (code = volc_Call(VC_CreateVolume, &servAddr, NULL, aggrId, volumeNamep, &volId))
	return (PrintError(as->name, code));
    dce_svc_printf(FTS_S_MAIN_FILESET_CREATED_MSG, AFS_HGETBOTH(volId), 
	       aggrName, serverNamep);
    return 0;
}

/*
 *  "delete <fileset> <server> <aggr>"
 */
EXPORT int fts_DoDeleteVolume(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    u_long aggrId;
    long code;
    afs_hyper_t volId;
    struct sockaddr servAddr;
    char *volumeNamep = as->parms[DELETE_FILESET_OFFSET].items->data; 
    char *aggrp = as->parms[DELETE_AGGREGATE_OFFSET].items->data;
    char *serverNamep = as->parms[DELETE_SERVER_OFFSET].items->data;
    char aggrName[MAX_AGGRNAME];

    if (!GetServer(serverNamep, &servAddr))				/* server */
	exit(1);
    if (!GetAggr(aggrp, &servAddr, GETAGGR_VALIDATE, &aggrId, aggrName))		/* aggr */
	exit(1);
    if (!GetVolumeID(volumeNamep, &volId))				/* volume */
	exit(1);
    code = fts_NeedAggrType(&servAddr, aggrId, AG_TYPE_ANY_EFS_MASK,
			     "'Delete' is");
    if (code)
	exit(1);
    if (code = volc_Call(VC_DeleteVolume, &servAddr, NULL, aggrId, &volId))
	return (PrintError(as->name, code));
    dce_svc_printf(FTS_S_MAIN_FILESET_DELETED_MSG,
		   AFS_HGETBOTH(volId), aggrName, serverNamep);
    return 0;
}

/*
 * "delentry <fileset> [prefix] [server] [aggr]"
 */
EXPORT int fts_DoDeleteEntry(as, aRock)
register struct cmd_syndesc *	as;
char *				aRock;
{
    afs_hyper_t volId;
    char *prefixp;
    vldbentry vlent;
    long code, code2;

    if (as->parms[DELFLDB_FILESET_OFFSET].items) {		/* <fileset> */
	if (as->parms[DELFLDB_SERVER_OFFSET].items
	    || as->parms[DELFLDB_AGGREGATE_OFFSET].items) {
	    dce_svc_printf(FTS_S_MAIN_NEITHER_MSG, programName, as->name);
	    exit(1);
	}
	if (!GetVolumeID(as->parms[DELFLDB_FILESET_OFFSET].items->data, &volId))
	    exit(1);
	vldb_SaveOldEntry(NULL);
	code2 = vldb_GetEntryByID(&volId, -1, &vlent, 0);
	code = vldb_DeleteEntry(&volId, -1);
	if (code2 == 0) {
	    (void) vldb_TellRepAboutNewEntry(&vlent, 0, 1);
	}
	if (code)
	    exit(1);
	dce_svc_printf(FTS_S_MAIN_FLDB_ENTRY_DELETE_MSG,
		       as->parms[DELFLDB_FILESET_OFFSET].items->data,
		       AFS_HGETBOTH(volId));
	exit(0);
    }
    prefixp = (char *) NULL;
    if (as->parms[DELFLDB_PREFIX_OFFSET].items) { 			/* [prefix] */
	prefixp = as->parms[DELFLDB_PREFIX_OFFSET].items->data;
    }
    if ((prefixp == NULL || *prefixp == '\0')
	 && !as->parms[DELFLDB_SERVER_OFFSET].items
	 && !as->parms[DELFLDB_AGGREGATE_OFFSET].items) {
	dce_svc_printf(FTS_S_MAIN_NULL_PREFIX_MSG, programName, as->name);
	exit(1);
    }
    if (as->parms[DELFLDB_AGGREGATE_OFFSET].items
	 && !as->parms[DELFLDB_SERVER_OFFSET].items) {
	dce_svc_printf(FTS_S_MAIN_AG_NEEDS_SERVER_MSG, programName, as->name,
		  as->parms[DELFLDB_AGGREGATE_OFFSET].items->data);
	exit(1);
    }
    if (vldb_ProcessByAttrs((as->parms[DELFLDB_SERVER_OFFSET].items ?
			      as->parms[DELFLDB_SERVER_OFFSET].items->data : (char *)0),
			     (as->parms[DELFLDB_AGGREGATE_OFFSET].items ?
			      as->parms[DELFLDB_AGGREGATE_OFFSET].items->data : (char *)0),
			     0, prefixp, 1 /*BULK_DELETE*/))
	exit(1);
    return 0;
}

/*
 *  "dump <fileset> <fromdate> <dumpfile>"
 */
EXPORT int fts_DoDumpVolume(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{    
    afs_hyper_t volId;
    long aggrId, voltype, code;
    struct sockaddr servAddr;
    char *fileName;
    struct vldbentry entry;
    struct ftserver_Date dumpDate;
    struct tm *tmptr;
    static char *mnames[12] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    unsigned long sockFlag = PIPE_NONE;

    if (!GetVolumeID(as->parms[DUMP_FILESET_OFFSET].items->data, &volId)) /* volume */
	exit(1);
    if (!as->parms[DUMP_TIME_OFFSET].items && !as->parms[DUMP_VERSION_OFFSET].items) {
	dce_svc_printf(FTS_S_MAIN_NEED_TIME_MSG, programName, as->name);
	exit(1);
    }
    if (as->parms[DUMP_TIME_OFFSET].items && as->parms[DUMP_VERSION_OFFSET].items) {
	dce_svc_printf(FTS_S_MAIN_NOT_BOTH_TIME_MSG, programName, as->name);
	exit(1);
    }
#if !defined(OSF_NO_SOCKET_DUMP)
    if (as->parms[DUMP_SOCKFLAG_OFFSET].items) {
	sockFlag = PIPE_TCP;
    }
#endif /* OSF_NO_SOCKET_DUMP */
    if (as->parms[DUMP_TIME_OFFSET].items) {
	if (strcmp(as->parms[DUMP_TIME_OFFSET].items->data, "0") == 0) {
	    dumpDate.mask = 0;		/* full dump */
	} else {
	    dumpDate.mask = 1;		/* incremental from date */
	    code = ktime_DateToLong(as->parms[DUMP_TIME_OFFSET].items->data,
				    &dumpDate.fromDate.sec);
	    dumpDate.fromDate.usec = 0;
	    if (code != 0) {
		dce_svc_printf(FTS_S_MAIN_BAD_TIME_MSG, programName, as->name,
			  as->parms[DUMP_TIME_OFFSET].items->data,
			  ktime_GetDateUsage());
		exit(1);
	    }
	}
    } else {
	if (strcmp(as->parms[DUMP_VERSION_OFFSET].items->data, "0") == 0) {
	    dumpDate.mask = 0;		/* full dump */
	} else {
	    dumpDate.mask = 2;		/* incremental from version */
	    code = fts_IsNumeric(as->parms[DUMP_VERSION_OFFSET].items->data,
				 0, 1, &dumpDate.fromVersion);
	    if (code == 0) {
		dce_svc_printf(FTS_S_MAIN_NOT_VERSION_NUMBER_MSG, programName, as->name,
			  as->parms[DUMP_VERSION_OFFSET].items->data);
		exit(1);
	    }
	}
    }
    fileName = NULL;
    /* Check whether '-file' was present */
    if (as->parms[DUMP_FILE_OFFSET].items)		/* dumpfile */
	fileName = as->parms[DUMP_FILE_OFFSET].items->data;
    if (cmd_IsParamPresent(as, DUMP_SERVER_OFFSET))     {
	/* optional server specified */
	if (!GetServer(as->parms[DUMP_SERVER_OFFSET].items->data, &servAddr))
	    exit(1);
	if (code = vldb_GetVldbInfoByServer(&volId, &servAddr, &aggrId, &voltype, &entry))
	    exit(1);
    }
    else    {
	/* find any server */
	if (code = vldb_GetVldbInfo(&volId, &servAddr, &aggrId, &voltype, &entry))
	    exit(1);
    }
    if (code = volc_Call(VC_DumpVolume, &servAddr, NULL, &volId,  aggrId, 
			 &dumpDate, fileName, (pipe_t *)NULL, sockFlag))
	return (PrintError(as->name, code));
    dce_fprintf(stderr,
		fts_s_main_dumped_fileset, 
		as->parms[DUMP_FILESET_OFFSET].items->data);
    switch (dumpDate.mask) {
	case 1:
	    code = (long) dumpDate.fromDate.sec;
	    tmptr = localtime(&code);
	    dce_fprintf(stderr,
			fts_s_main_incr_from, 
			tmptr->tm_mday, 
			mnames[tmptr->tm_mon], tmptr->tm_year+1900,
			tmptr->tm_hour, tmptr->tm_min);
	    break;
	case 2:
	    dce_fprintf(stderr, fts_s_main_incr_from_version, 
			AFS_HGETBOTH(dumpDate.fromVersion));
	    break;
	default:
	    break;
    }
    if (fileName == NULL) {
	dce_fprintf(stderr, fts_s_main_to_stout);
    } else {
	dce_fprintf(stderr, fts_s_main_to_file, fileName);
    }
    return 0;
}

PRIVATE void defaultPath(as, pelt, oelt)
register struct cmd_syndesc *as;
int pelt, oelt;
{   /* default the path name ``.'' if nothing was given. */
    register struct cmd_item *ti;

    if (oelt >= 0 && as->parms[oelt].items != NULL) return;
    if (as->parms[pelt].items == NULL) {
	/* allocate an item representing "." */
	/* Use malloc(), not osi_Alloc, since file/tools/cmd/cmd.c uses free() */
	ti = (struct cmd_item *) malloc(sizeof(struct cmd_item));
	ti->next = (struct cmd_item *) 0;
	ti->data = (char *) malloc(2);
	strcpy(ti->data, ".");
	/* and push it into place */
	as->parms[pelt].items = ti;
    }
}
/*
 *  "lsft <fileset>"
 */
EXPORT int fts_DoLSFT(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    struct vldbentry entry;
    ftserver_status status;
    struct sockaddr servAddr;
    afs_hyper_t volId;
    long code, volType, aggrId;
    struct afsFid fid;

    defaultPath(as, EXAMINE_PATH_OFFSET, EXAMINE_FILESET_OFFSET);
    if (as->parms[EXAMINE_PATH_OFFSET].items) {
	/* have a path; use it */
	code = fts_PathToFid(as->name,
			     as->parms[EXAMINE_PATH_OFFSET].items->data,
			     &fid);
	if (code) exit(1);
	volId = fid.Volume;
    } else {
	if (!GetVolumeID(as->parms[EXAMINE_FILESET_OFFSET].items->data, &volId)) /* volume */
	exit(1);
    }
    if (cmd_IsParamPresent(as, EXAMINE_SERVER_OFFSET))     {
	/* optional server specified */
	if (!GetServer(as->parms[EXAMINE_SERVER_OFFSET].items->data, &servAddr))
	    exit(1);
	if (code = vldb_GetVldbInfoByServer(&volId, &servAddr, &aggrId, &volType, &entry))
	    exit(1);
    }
    else     {
	/* find any server */
	if (code = vldb_GetVldbInfo(&volId, &servAddr, &aggrId, &volType, &entry))
	    exit(1);
    }
    if (code = volc_Call(VC_VolumeStatus, &servAddr, NULL, aggrId, &volId, &status)) {
	if (code == ENODEV) {
	    dce_svc_printf(FTS_S_MAIN_NO_FILESET_AT_SITE_MSG,
			   programName, as->name);
	    vldb_EnumerateEntry(&entry, "", 0);
	}
	return (PrintError(as->name, code));
    }
    VolumeStats(&status, &entry, &servAddr, aggrId, volType);
    return 0;
}

/*
 * "lock <fileset>"
 */
EXPORT int fts_DoLockEntry(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    afs_hyper_t volId;

    if (!GetVolumeID(as->parms[LOCK_FILESET_OFFSET].items->data, &volId)) /* volume */
	exit(1);
    if (vldb_SetLock(&volId, -1, VLOP_DELETE))
	exit(1);
    dce_svc_printf(FTS_S_MAIN_LOCKE_FLDB_MSG, as->parms[LOCK_FILESET_OFFSET].items->data);
    return 0;
}

/*
 *  "lsaggr <server>"
 */
EXPORT int fts_DoListAggregates(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    long code, i, numAggs;
    struct sockaddr servAddr;
    struct ftserver_aggrList *aggrListp;
    register struct ftserver_aggrList *aggrp;

    if (!GetServer(as->parms[LSAGGR_SERVER_OFFSET].items->data, &servAddr)) /* server */
	exit(1);
    if (code = volc_Call(fts_GetAllAggrs, &servAddr, NULL, &numAggs, &aggrListp))
	return (PrintError(as->name, code));
    dce_svc_printf(FTS_S_MAIN_N_AGGREGATES_MSG, (numAggs == 1 ? "is" : "are"),
	    numAggs,
	    (numAggs == 1 ? "" : "s"),
	    as->parms[LSAGGR_SERVER_OFFSET].items->data,
	    MapSockAddr(&servAddr));
    aggrp = aggrListp;
    for (i = 0, aggrp = aggrListp; i < numAggs; ++i, ++aggrp) {
	dce_printf(fts_s_main_aggr_names, aggrp->name);
	if (strcmp((char *)aggrp->name, (char *)aggrp->dvname) != 0)
	    dce_printf(fts_s_main_dvname, aggrp->dvname);
	dce_printf(fts_s_main_aggrp_id, 
		   aggrp->Id, 
		   AG_TYPE_TO_STR(aggrp->type));
    }
    if (aggrListp)
	free(aggrListp);
    return 0;
}

/*
 * listvldb <fileset> [server] [aggr] [locked]"
 */
EXPORT int fts_DoListFLDB(as, aRock)
register struct cmd_syndesc *	as;
char *				aRock;
{   
    long lock;
    afs_hyper_t volId;

    if (as->parms[LSFLDB_FILESET_OFFSET].items) {			/* <fileset> */
	if (as->parms[LSFLDB_SERVER_OFFSET].items
	    || as->parms[LSFLDB_AGGREGATE_OFFSET].items
	    || as->parms[LSFLDB_LOCKED_OFFSET].items) {
	    dce_svc_printf(FTS_S_MAIN_NOT_SERVER_AGG_MSG, programName, as->name);
	    exit(1);
	}
	if (!GetVolumeID(as->parms[LSFLDB_FILESET_OFFSET].items->data, &volId))
	    exit(1);
	vldb_DisplayVolId(&volId);
	exit(0);
    }
    lock = 0;
    if (as->parms[LSFLDB_LOCKED_OFFSET].items) 				/* [locked] */
	lock = 1;
    if (as->parms[LSFLDB_AGGREGATE_OFFSET].items
	 && !as->parms[LSFLDB_SERVER_OFFSET].items) {
	dce_svc_printf(FTS_S_MAIN_AGG_NEEDS_SERVER_MSG, programName, as->name,
		  as->parms[LSFLDB_AGGREGATE_OFFSET].items->data);
	exit(1);
    }
    if (vldb_ProcessByAttrs((as->parms[LSFLDB_SERVER_OFFSET].items ?
			      as->parms[LSFLDB_SERVER_OFFSET].items->data : (char *)0),
			     (as->parms[LSFLDB_AGGREGATE_OFFSET].items ?
			      as->parms[LSFLDB_AGGREGATE_OFFSET].items->data : (char *)0),
			     lock, NULL, 4 /*BULK_LIST*/))
	exit(1);
    return 0;
}

/*
 *  "listvol <server> [aggr] [-fast] [-long]"
 */
EXPORT int fts_DoListVolumes(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{  
    long longf = 0, fast = 0;
    unsigned long allcount;
    long code = 0, count, i, numAggrs;
    struct sockaddr servAddr;
    ftserver_status *statusp;
    struct ftserver_aggrList *aggrListp;
    register struct ftserver_aggrList *aggrp;

    if (!GetServer(as->parms[LSFS_SERVER_OFFSET].items->data, &servAddr)) /* server */
	exit(1);
    if (code = GetOptAggr(as, &servAddr, &aggrListp, &numAggrs,
			   LSFS_AGGREGATE_OFFSET)) /* [aggr] */
	exit(1);

    if (as->parms[LSFS_FAST_OFFSET].items)				/* [-fast] */
	fast = 1;
    if (as->parms[LSFS_LONG_OFFSET].items)			/* [-long] */
	longf = 1;
    allcount = 0;
    for (i = 0, aggrp = aggrListp; i < numAggrs; ++i, ++aggrp) {
	if (code = volc_Call(VC_ListVolumes, &servAddr, NULL, aggrp->Id,
			     !fast, &statusp, &count)) {
	    if (statusp) free(statusp);
	    PrintError(as->name, code);
	    break;
	}
	if (i > 0 && !fast) dce_svc_printf(FTS_S_MAIN_NEWLINE_MSG);
	if (count == 0) {
	    dce_svc_printf(FTS_S_MAIN_NO_FILESETS_MSG, aggrp->name, aggrp->Id,
		   as->parms[LSFS_SERVER_OFFSET].items->data);
	} else {
	    if (count > 1) {
		if (!fast) 
		    qsort((char *)statusp, count, sizeof(ftserver_status),
			  CompareVolName);
		else 
		    qsort((char *)statusp, count, sizeof(ftserver_status),
			  CompareVolID);
	    }
	    if (!fast) {
		dce_svc_printf(FTS_S_MAIN_TOTAL_FILESETS_MSG, as->parms[LSFS_SERVER_OFFSET].items->data,
		       aggrp->name, aggrp->Id, count);
	    }
	    DisplayVolumes(&servAddr, aggrp->Id, statusp, count, longf,
			   fast);
	    allcount += count;
	}
    }
    if (code == 0 && !as->parms[LSFS_AGGREGATE_OFFSET].items) {
	dce_svc_printf(FTS_S_MAIN_TOTAL_NUMBER_FILESETS_MSG, as->parms[LSFS_SERVER_OFFSET].items->data, allcount);
    }
    if (aggrListp)
	free(aggrListp);
    return code;
}

/*
 *  "move <fileset> <fromserver> <fromaggr> <toserver> <toaggr>"
 */
EXPORT int fts_DoMoveVolume(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    afs_hyper_t volId;
    u_long faggrId, taggrId;
    long code;
    int sockFlag = 0;
    struct sockaddr fservAddr, tservAddr;
    char *volumeNamep = as->parms[MOVE_FILESET_OFFSET].items->data;
    char *fserverNamep = as->parms[MOVE_FROMSERVER_OFFSET].items->data;
    char *faggrp = as->parms[MOVE_FROMAGGREGATE_OFFSET].items->data;
    char *tserverNamep = as->parms[MOVE_TOSERVER_OFFSET].items->data;
    char *taggrp = as->parms[MOVE_TOAGGREGATE_OFFSET].items->data;
    char faggrName[MAX_AGGRNAME], taggrName[MAX_AGGRNAME];

#if !defined(OSF_NO_SOCKET_DUMP)
    if (as->parms[MOVE_SOCKFLAG_OFFSET].items)		/* [-socket] */
        sockFlag = 1;
#endif /* OSF_NO_SOCKET_DUMP */
    if (!GetVolumeID(volumeNamep, &volId))		/* volume */
	exit(1);
    if (!GetServer(fserverNamep, &fservAddr))		/* fromserver */
	exit(1);
    if (!GetAggr(faggrp, &fservAddr, GETAGGR_VALIDATE, &faggrId, faggrName))
							/* fromaggr */
	exit(1);
    if (!GetServer(tserverNamep, &tservAddr))		/* toserver */
	exit(1);
    if (!GetAggr(taggrp, &tservAddr, GETAGGR_VALIDATE, &taggrId, taggrName))		/* toaggr */
	exit(1);
    code = fts_NeedAggrType(&fservAddr, faggrId, AG_TYPE_ANY_EFS_MASK,
			    "Moving from is");
    if (code)
	exit(1);
    code = fts_NeedAggrType(&tservAddr, taggrId, AG_TYPE_ANY_EFS_MASK,
			    "Moving to is");
    if (code)
	exit(1);
    if (code = volc_Call(VC_MoveVolume, 0, NULL, &volId, &fservAddr,
			 faggrId, &tservAddr, taggrId, sockFlag))
	return (PrintError(as->name, code));
    dce_svc_printf(FTS_S_MAIN_FILESET_MOVED_MSG, AFS_HGETBOTH(volId),
	       fserverNamep, faggrName, tserverNamep, taggrName);
    return 0;
}

/*
 *  "release <fileset>"
 */
EXPORT int fts_DoReleaseVolume(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    long aggrId, volType, code;
    struct sockaddr servAddr;
    struct vldbentry entry;
    afs_hyper_t volId;
    int synchronous;

    if (!GetVolumeID(as->parms[RELEASE_FILESET_OFFSET].items->data, &volId)) /* volume */
	exit(1);

    synchronous = 0;
    if (as->parms[RELEASE_WAIT_OFFSET].items)
	synchronous = 1;

    if (code = vldb_GetVldbInfo(&volId, &servAddr, &aggrId, &volType, &entry))
	exit(1);
    if ((entry.flags & VLF_LAZYREP) == 0) {
	dce_svc_printf(FTS_S_MAIN_SET_REP_TYPE_MSG, programName, as->name, entry.name);
	exit(1);
    }
    if ((entry.flags & VLF_LAZYRELE) == 0) {
	dce_svc_printf(FTS_S_MAIN_RELEASE_NOT_SUPPORTED_MSG, programName, as->name);
	exit(1);
     }
    code = fts_NeedAggrType(&servAddr, aggrId, AG_TYPE_ANY_EFS_MASK, "Replication is");
    if (code)
	exit(1);
    if (code = volc_Call(VC_ReleaseVolume, 0, NULL, &volId, &servAddr, aggrId, synchronous))
	return (PrintError(as->name, code));
    dce_svc_printf(FTS_S_MAIN_FILESET_RELEASED_MSG, as->parms[RELEASE_FILESET_OFFSET].items->data);
    return 0;
}

/*
 *  "unlock <fileset>"
 */
EXPORT int fts_DoReleaseEntry(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    afs_hyper_t volId;
    long code;

    if (!GetVolumeID(as->parms[UNLOCK_FILESET_OFFSET].items->data, &volId))			/* volume */
	exit(1);
    if (code = vldb_ReleaseLock(&volId, -1, LOCKREL_ALL))		
	return (PrintError(as->name, code));
    dce_svc_printf(FTS_S_MAIN_ENTRY_UNLOCKED_MSG, as->parms[UNLOCK_FILESET_OFFSET].items->data);
    return 0;
}

/*
 *  "rename <oldvolumename> <newvolumename>"
 */ 
EXPORT int fts_DoRenameVolume(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    long code1, code2, code;
    struct vldbentry entry, nentry;
    char *oldname = as->parms[RENAME_OLDNAME_OFFSET].items->data;
    char *newname = as->parms[RENAME_NEWNAME_OFFSET].items->data;

    code1 = vldb_GetEntryByName(oldname, &entry, 1);	      	/* oldvolumename */
    code2 = vldb_GetEntryByName(newname, &nentry, 1);	/* newvolumename */
    if ((!code1) && (!code2)) {
	dce_svc_printf(FTS_S_MAIN_FILESET_EXISTS_MSG, programName, as->name, newname);
	exit(1);
    }
    if (code1 && code2) {
	dce_svc_printf(FTS_S_MAIN_NO_FILESET_ENTRY_MSG, programName, as->name,
		  oldname, newname, code1, code2);
	exit(1);
    }
    if (InvalidVolName(oldname) || InvalidVolName(newname))
	exit(1);
    if (code = volc_Call(VC_RenameVolume, 0, NULL, &entry, oldname, newname)) {
	PrintError(as->name, code);
	exit(1);
    }
    dce_svc_printf(FTS_S_MAIN_FILESET_RENAMED_MSG, oldname, newname); 
    return 0;
}

/*
 *  "restore <dumpfile> <server> <aggr> <volumeName> <fileset>"
 */
EXPORT int fts_DoRestoreVolume(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{    
    afs_hyper_t volId;
    long agType, code, override = 1;
    u_long aggrId;
    struct sockaddr servAddr;
    struct vldbentry entry;
    char *fileName;
    char *volNamep = as->parms[RESTORE_FSNAME_OFFSET].items->data;
    char *serverNamep = as->parms[RESTORE_SERVER_OFFSET].items->data;
    char *aggrp = as->parms[RESTORE_AGGREGATE_OFFSET].items->data;
    char aggrName[MAX_AGGRNAME];
    int Overwrite;
    unsigned long restoreFlags;
    int RWix, k, fatalError, siteIx;
    unsigned long sockFlag = PIPE_NONE;

    fileName = NULL;			/* dumpfile */
    if (as->parms[RESTORE_FILE_OFFSET].items) {	/* if file name given */
	fileName = as->parms[RESTORE_FILE_OFFSET].items->data;
	if (!FileExists(fileName)) {
	    exit(1);
	}
    }
    if (!GetServer(serverNamep, &servAddr))		/* server */
	exit(1);
    if (!GetAggr(aggrp, &servAddr, GETAGGR_VALIDATE, &aggrId, aggrName))/* aggr */
	exit(1);
    if (InvalidVolName(volNamep))			 /* volumeName */
	exit(1);
    if (as->parms[RESTORE_FSID_OFFSET].items) {		/* volume */
	if (!GetVolumeID(as->parms[RESTORE_FSID_OFFSET].items->data, &volId))
	    exit(1);
    } else
	AFS_hzero(volId);
    Overwrite = 0;
    if (as->parms[RESTORE_OVERWRITE_OFFSET].items) {		/* -overwrite */
	Overwrite = 1;
    }
    restoreFlags = 0;
    if (as->parms[RESTORE_DISJOINT_OFFSET].items) {		/* -disjoint */
	restoreFlags = FTU_RESTORE_DISJOINT_OK;
    }
#if !defined(OSF_NO_SOCKET_DUMP)
    if (as->parms[RESTORE_SOCKFLAG_OFFSET].items) {		/* -disjoint */
	sockFlag = PIPE_TCP;
    }
#endif /* OSF_NO_SOCKET_DUMP */
    fatalError = 0;
    code = vldb_GetEntryByName(ExtractSuffix(volNamep), &entry, 1);
    if (code != 0 && code != VL_NOENT) {
	VOLSERLOG(0, "%s %s: Cannot determine if fileset %s is in the FLDB: %s\n",
		  programName, as->name,
		  volNamep, dfs_dceErrTxt(code));
	fatalError = 1;
    } else if (code == 0) {
	RWix = -1;
	for (k = 0; k < MAXVOLTYPES && !AFS_hiszero(entry.VolIDs[k]); ++k) {
	    if (entry.VolTypes[k] == VOLTIX_TO_VOLTYPE(RWVOL)) {
		RWix = k;
		break;
	    }
	}
	dce_svc_printf(FTS_S_MAIN_FILESET_IN_FLDB_MSG, programName, as->name,
		  volNamep, AFS_HGETBOTH(volId));
	if (Overwrite == 0) {
	    dce_svc_printf(FTS_S_MAIN_USER_OVERWRITE_MSG, programName, as->name);
	    fatalError = 1;
	}
	if (RWix < 0) {
	    VOLSERLOG(0, "%s %s: No R/W fileset ID is defined for fileset %s\n",
		  programName, as->name,
		  volNamep);
	    fatalError = 1;
	} else {
	    if (AFS_hiszero(volId))
		volId = entry.VolIDs[RWix];
	    else if (!AFS_hiszero(entry.VolIDs[RWix]) &&
		     !AFS_hsame(entry.VolIDs[RWix], volId)) {
		VOLSERLOG(0, "%s %s: Cannot change the ID of fileset %s to the given %lu,,%lu\n",
			  programName, as->name,
			  volNamep, AFS_HGETBOTH(volId));
		fatalError = 1;
	    }
	    siteIx = vldb_FindRepSite(&entry, 0, -1,
				      ((unsigned32)VLSF_ZEROIXHERE) >> RWix);
	    if (siteIx < 0) {
		VOLSERLOG(0, "%s %s: Fileset %s needs a R/W fileset ID\n",
			  programName, as->name,
			  volNamep);
	    } else {
		if (!vldb_IsSiteHere(&entry, siteIx, &servAddr, -1)) {
		    VOLSERLOG(0, "%s %s: Restoration cannot move %s from %s to %s\n",
			      programName, as->name, volNamep,
			      MapSockAddr(&entry.siteAddr[siteIx]),
			      serverNamep);
		    fatalError = 1;
		} else if (aggrId != entry.sitePartition[siteIx]) {
		    VOLSERLOG(0, "%s %s: Restoration cannot move %s from aggr %d to aggr %d\n",
			      programName, as->name, volNamep,
			      entry.sitePartition[siteIx], aggrId);
		    fatalError = 1;
		}
	    }
	}
	if (fatalError == 0) {
	    dce_svc_printf(FTS_S_MAIN_OVERWRITING_MSG, programName, as->name);
	}
    } else { /* doesn't exist, create it first */
      if (code = fts_GetAggrType(&servAddr, aggrId, &agType, 1))
	return (PrintError("restore/create", code));
      if (! AG_TYPE_SUPPORTS_EFS(agType)) {
	dce_svc_printf(FTS_S_MAIN_NO_FILESET_LFS_MSG, volNamep,
		       getAggrName(&servAddr, aggrId, 0),
		       MapSockAddr(&servAddr));
	exit(1);
      }
      else {
        dce_svc_printf(FTS_S_MAIN_CREATING_FILESET_MSG, volNamep,
		       getAggrName(&servAddr, aggrId, 0),
		       MapSockAddr(&servAddr));
	if (code = volc_Call(VC_CreateVolume, &servAddr, NULL, aggrId, 
			     volNamep, &volId))
	    return (PrintError("restore/create", code));
	dce_svc_printf(FTS_S_MAIN_FILESET_CREATED_MSG,
		       AFS_HGETBOTH(volId), aggrName, serverNamep);
      }
    }
    if (fatalError != 0)
	exit(1);
    if (code = volc_Call(VC_RestoreVolume, &servAddr, NULL, aggrId, &volId,
			 volNamep, override, restoreFlags, fileName, 
			 (pipe_t *)NULL, sockFlag))
	return (PrintError(as->name, code));
    dce_svc_printf(FTS_S_MAIN_FILESET_RESTORED_MSG, 
		   volNamep, 
		   serverNamep, 
		   aggrName,
		   ((fileName == NULL) ? "standard input" : fileName));
    return 0;
}

/*
 *  "rmsite <fileset> <server> <aggr>"
 */
EXPORT int fts_DoRemoveSite(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{ 
    afs_hyper_t volId;
    u_long aggrId;
    long code;
    struct sockaddr servAddr;
    char *volumeNamep = as->parms[RMSITE_FILESET_OFFSET].items->data;
    char *aggrp = as->parms[RMSITE_AGGREGATE_OFFSET].items->data;
    char *serverNamep = as->parms[RMSITE_SERVER_OFFSET].items->data;
    char aggrName[MAX_AGGRNAME];

    if (!GetServer(serverNamep, &servAddr))				/* server */
	exit(1);
    if (!GetAggr(aggrp, &servAddr, GETAGGR_NUMERICOK, &aggrId, aggrName))		/* aggr */
	exit(1);
    if (!GetVolumeID(volumeNamep, &volId))				/* volume */
	exit(1);
    if (code = vldb_RemoveSite(&servAddr, aggrId, &volId))
	exit(1);
    dce_svc_printf(FTS_S_MAIN_SITE_REMOVED_MSG, serverNamep, aggrId,
	      volumeNamep);

    return 0;
}

/*
 *  "setrepinfo -id <vid> [-maxtotlat <d>] [-hardmaxtotlat <d>] [-minpounce <d>] [-hardmaxreplat <d>]
	  [-reclaimdally <d>] [-maxreplatency <d>] [-combi][-lazyrep][-release]"
 */
EXPORT int fts_DoSetRepInfo(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    struct vldbentry entry, possEntry;
    struct sockaddr servAddr;
    afs_hyper_t volId, ROvolId;
    long code, aggrId;
    int OK, repStyle, oldStyle, realoldStyle, sawChange;
    u_long volType;
    u_long currmaxrep, currminrep;
    int maxrepix, minrepix;
#define rv_MAXT 0
#define rv_HMT 1
#define rv_RECL 2
#define rv_MINPOUNCE 3
#define rv_DMR 4
#define rv_LEN 5
    u_long CurrV[rv_LEN];
    static char *rvNames[rv_LEN] = {
	"maxage", "failage", "reclaimwait", "minrepdelay", "defaultsiteage"
    };
    static int rvOffsets[rv_LEN] = {
	SETREPL_MAXAGE_OFFSET,
	SETREPL_FAILAGE_OFFSET,
	SETREPL_RECLAIMWAIT_OFFSET,
	SETREPL_MINREPDELAY_OFFSET,
	SETREPL_DFLTSITEAGE_OFFSET,
    };
#define rvf_Derived 0
#define rvf_Existing 1
#define rvf_Given 2
#define rvf_Initial 3
#define rvf_Default 4
    static char *fromNames[] = {
	"derived value", "preexisting value", "given value", "initial value", "default value"
    };
    static char *capFromNames[] = {
	"Derived value", "Preexisting value", "Given value", "Initial value", "Default value"
    };
    char rvFrom[rv_LEN];
    register int ix;
    int RWix, StageIx, err, lcl, nonlcl, oldStageIx, possStageIx;
    u_long RWmask, ROmask;

    if (!GetVolumeID(as->parms[SETREPL_FILESET_OFFSET].items->data, &volId)) /* volume */
	exit(1);
    if (code = vldb_GetVldbInfo(&volId, &servAddr, &aggrId, &volType, &entry))
	exit(1);
    if (code = vldb_SetLock(&volId, volType, VLOP_RELEASE))
	exit(1);
    if (as->parms[SETREPL_SCHEDULED_OFFSET].items
	 && as->parms[SETREPL_RELEASE_OFFSET].items) {
	dce_svc_printf(FTS_S_MAIN_ONLY_ONE_MSG, programName, as->name);
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }
    /* Get the existing parameters from the vldb entry. */
    realoldStyle = oldStyle = -1;
    if ((entry.flags & VLF_LAZYREP) != 0) {
	if ((entry.flags & VLF_LAZYRELE) != 0) {
	    realoldStyle = 0;
	} else {
	    realoldStyle = 1;
	}
    }
    sawChange = (as->parms[SETREPL_CHANGE_OFFSET].items ? 1 : 0);
    if (sawChange) {
	if (realoldStyle < 0) {
	    dce_svc_printf(FTS_S_MAIN_CHANGE_INAPPROPRIATE_MSG, programName, as->name, entry.name);
	    (void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	    exit(1);
	}
    } else {
	/* use existing unless '-change' was specified */
	oldStyle = realoldStyle;
    }
    repStyle = -1;
    if (as->parms[SETREPL_SCHEDULED_OFFSET].items) repStyle = 1;	/* -scheduled */
    else if (as->parms[SETREPL_RELEASE_OFFSET].items) repStyle = 0;	/* -release */
    if (repStyle < 0 && sawChange) {
	dce_svc_printf(FTS_S_MAIN_MUST_CHANGE_MSG, programName, as->name);
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }
    /* Catch a whole collection of uses here */
    if (repStyle >= 0 && realoldStyle >= 0 && (!sawChange || repStyle == realoldStyle)) {
	    dce_svc_printf(FTS_S_MAIN_FILESET_REPLICATED_MSG, 
		      programName, as->name,
		      entry.name, (realoldStyle == 0 ? "release" : "scheduled"));
    if (repStyle == realoldStyle) {
	    dce_svc_printf(FTS_S_MAIN_NO_MORE_SPECS_MSG);
	} else {
	    dce_svc_printf(FTS_S_MAIN_USE_CHANGE_MSG);
	}
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }
    if (repStyle < 0) {
	if (oldStyle < 0) {
	    dce_svc_printf(FTS_S_MAIN_SET_REP_FIRST_TYPE_MSG, programName, as->name, entry.name);
	    (void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	    exit(1);
	}
	VOLSERLOG(1, "%s %s: Assuming the existing `-%s' replication style for fileset `%s'.\n",
		  programName, as->name,
		  (oldStyle == 0 ? "release" : "scheduled"), entry.name);
	repStyle = oldStyle;
    }
    RWmask = ROmask = 0;
    AFS_hzero(ROvolId);
    for (ix = 0; ix < MAXNSERVERS && !AFS_hiszero(entry.VolIDs[ix]); ++ix) {
	if (entry.VolTypes[ix] == VOLTIX_TO_VOLTYPE(RWVOL)) {
	    RWmask = ((unsigned32)VLSF_ZEROIXHERE) >> ix;
	} else if (entry.VolTypes[ix] == VOLTIX_TO_VOLTYPE(ROVOL)) {
	    ROmask = ((unsigned32)VLSF_ZEROIXHERE) >> ix;
	    ROvolId = entry.VolIDs[ix];
	}
    }
    if (AFS_hiszero(ROvolId)) {
	dce_svc_printf(FTS_S_MAIN_CANT_SET_REP_INFOR_MSG, programName, as->name, entry.name);
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }
    RWix = -1;
    for (ix = 0; ix < entry.nServers; ++ix) {
	if ((entry.siteFlags[ix] & RWmask) != 0) {
	    RWix = ix;
	    break;
	}
    }
    if (RWix < 0) {
	dce_svc_printf(FTS_S_MAIN_NO_RW_SITE_MSG, programName, as->name, entry.name);
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }
    code = fts_NeedAggrType(&entry.siteAddr[RWix], entry.sitePartition[RWix],
			     AG_TYPE_ANY_EFS_MASK, "Replication is");
    if (code) {
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }
    /* other non-0 ``code'' values indicate failure in finding the type of the aggr,
      and we ignore them. */
    /* If repstyle wasn't given in the old entry, or '-clear' option named, clear out values. */
    if (realoldStyle < 0 || as->parms[SETREPL_CLEAR_OFFSET].items) {
	for (ix = 0; ix < rv_LEN; ++ix) rvFrom[ix] = rvf_Initial;
	bzero((char *)&CurrV[0], sizeof(CurrV));
    } else {
	/* copy in the old values to edit here. */
	for (ix = 0; ix < rv_LEN; ++ix) rvFrom[ix] = rvf_Existing;
	CurrV[rv_MAXT] = entry.maxTotalLatency;
	CurrV[rv_HMT] = entry.hardMaxTotalLatency;
	CurrV[rv_MINPOUNCE] = entry.minimumPounceDally;
	CurrV[rv_DMR] = entry.defaultMaxReplicaLatency;
	CurrV[rv_RECL] = entry.reclaimDally;
    }
    oldStageIx = fts_releaseRepSite(&entry);
    possEntry = entry;
    possEntry.flags |= VLF_LAZYREP | VLF_LAZYRELE;
    possStageIx = fts_releaseRepSite(&entry);
    if (repStyle) {
	possEntry.flags &= ~(VLF_LAZYRELE);	/* tag as sched-rep */
    } else {
	possEntry.flags |= VLF_LAZYRELE;	/* tag as release-rep */
    }
    StageIx = fts_releaseRepSite(&possEntry);
    /* Now pick up the command option values for these intervals. */
    err = 0;
    for (ix = 0; ix < rv_LEN; ++ix) {
	if (as->parms[rvOffsets[ix]].items) {
	    if (fts_strToIv(as->parms[rvOffsets[ix]].items->data, &CurrV[ix]))
		err = 1;
	    rvFrom[ix] = rvf_Given;
	}
    }
    if (err) {	/* err msg was given by fts_strToIv() */
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }
    if (repStyle == 0) {
	/* Setting release-style replication */
	lcl = nonlcl = 0;
	for (ix = 0; ix < entry.nServers; ++ix) {
	    if (entry.siteFlags[ix] & VLSF_SAMEASPREV) continue;
	    if ((entry.siteFlags[ix] & ROmask) == 0) continue;
	    if (ix == StageIx)
		++lcl;
	    else
		++nonlcl;
	}
	if (nonlcl > 0 && lcl == 0) {
	    dce_svc_printf(FTS_S_MAIN_NEED_LOCAL_REP_MSG,
			   programName, as->name);
	    (void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	    exit(1);
	}
    }
    /* Derive values for missing parameters. */
    if (repStyle == 1) {
	/* Setting scheduled replication. */
	if (rvFrom[rv_HMT] == rvf_Initial
	    && rvFrom[rv_DMR] == rvf_Initial
	    && rvFrom[rv_MINPOUNCE] == rvf_Initial) {
	    /* maxage: 2 hour default */
	    if (rvFrom[rv_MAXT] == rvf_Initial) {
		CurrV[rv_MAXT] = 2 * 60 * 60;	/* raw default: two hours */
		rvFrom[rv_MAXT] = rvf_Default;
	    }
	    /* reclaimwait: 18 hour default */
	    if (rvFrom[rv_RECL] == rvf_Initial) {
		CurrV[rv_RECL] = 18 * 60 * 60;	/* raw default: 18 hours */
		rvFrom[rv_RECL] = rvf_Default;
	    }
	    /* failage: the greater of 1 day or twice the maxage */
	    CurrV[rv_HMT] = CurrV[rv_MAXT] * 2;
	    if (CurrV[rv_HMT] < (24*60*60)) CurrV[rv_HMT] = 24*60*60;
	    rvFrom[rv_HMT] = rvf_Derived;
	    /* defaultsiteage: maxage/4 */
	    CurrV[rv_DMR] = CurrV[rv_MAXT] / 4;
	    rvFrom[rv_DMR] = rvf_Derived;
	    /* minpounce: the lesser of 5 minutes or defaultsiteage/4 */
	    CurrV[rv_MINPOUNCE] = CurrV[rv_DMR] / 4;
	    if (CurrV[rv_MINPOUNCE] > (5*60)) CurrV[rv_MINPOUNCE] = 5*60;
	    rvFrom[rv_MINPOUNCE] = rvf_Derived;
	}
	/* Complain if we don't have values for everything */
	if (rvFrom[rv_MAXT] == rvf_Initial || rvFrom[rv_HMT] == rvf_Initial
	    || rvFrom[rv_MINPOUNCE] == rvf_Initial || rvFrom[rv_RECL] == rvf_Initial) {
	    dce_svc_printf(FTS_S_MAIN_SPECIFY_ALL_PARAMETERS_MSG, programName, as->name,
		      rvNames[rv_HMT], rvNames[rv_MINPOUNCE], rvNames[rv_DMR]);
	    (void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	    exit(1);
	}
    } else {
	/* repStyle == 0: release replication */
	if (rvFrom[rv_HMT] == rvf_Initial) {
	    /* maxage: 2 hour default */
	    if (rvFrom[rv_MAXT] == rvf_Initial) {
		CurrV[rv_MAXT] = 2 * 60 * 60;	/* raw default: two hours */
		rvFrom[rv_MAXT] = rvf_Default;
	    }
	    /* failage: the greater of 1 day or twice the maxage */
	    CurrV[rv_HMT] = CurrV[rv_MAXT] * 2;
	    if (CurrV[rv_HMT] < (24*60*60)) CurrV[rv_HMT] = 24*60*60;
	    rvFrom[rv_HMT] = rvf_Derived;
	    /* reclaimwait: 18 hour default */
	    if (rvFrom[rv_RECL] == rvf_Initial) {
		CurrV[rv_RECL] = 18 * 60 * 60;	/* raw default: 18 hours */
		rvFrom[rv_RECL] = rvf_Default;
	    }
	}
	/* Complain if we don't have values for everything */
	if (rvFrom[rv_MAXT] == rvf_Initial || rvFrom[rv_HMT] == rvf_Initial
	    || rvFrom[rv_RECL] == rvf_Initial) {
	    dce_svc_printf(FTS_S_MAIN_MUST_SPECIFY_MSG, programName, as->name,
		      rvNames[rv_MAXT], rvNames[rv_RECL], rvNames[rv_HMT]);
	    (void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	    exit(1);
	}
    }
    /* Check the parameters for reasonableness. */
    OK = 1;
    /* First, support changing rep-style from release to scheduled, where the
     * maxsiteage is suddenly meaningful.  If we are switching and some
     * maxsiteage values are zero, try to use the default maxsiteage value to
     * fill them in.
     */
    /* On second thought, this fldb entry might have been created wholesale by
     * syncfldb, and there might be all kinds of reasons for the absence of a
     * maxsiteage.  Just fill in any existing zero values with the default.
     */
    nonlcl = 0;	/* Really, number of zero-maxsiteage entries */
    if (repStyle == 1)			/* Scheduled replication */
	for (ix = 0; ix < entry.nServers; ++ix) {
	    if (entry.siteFlags[ix] & VLSF_SAMEASPREV) continue;
	    if ((entry.siteFlags[ix] & ROmask) == 0) continue;	/* not this server */
	    if (entry.sitemaxReplicaLatency[ix] == 0) ++nonlcl;
	}
    if (nonlcl > 0) {	/* any zero-maxage entries? */
	dce_svc_printf(FTS_S_MAIN_ZERO_MAXSITEAGE_MSG, 
		       programName, as->name, nonlcl,
		       (nonlcl == 1 ? " has a" : "s have"),
		       (nonlcl == 1 ? "" : "s"));
	for (ix = 0; ix < entry.nServers; ++ix) {
	    if (entry.siteFlags[ix] & VLSF_SAMEASPREV) continue;
	    if ((entry.siteFlags[ix] & ROmask) == 0) continue;	/* not this server */
	    if (ix == StageIx) continue;		/* not to worry about staging site */
	    if (entry.sitemaxReplicaLatency[ix] == 0) {
		dce_svc_printf(FTS_S_MAIN_AGGREGATE_X_MSG,
			       MapSockAddr(&entry.siteAddr[ix]),
			       getAggrName(&entry.siteAddr[ix],
					   entry.sitePartition[ix], 0), 0);
		if (CurrV[rv_DMR] != 0)
		    entry.sitemaxReplicaLatency[ix] = CurrV[rv_DMR];
	    }
	}
	if (CurrV[rv_DMR] != 0) {
	    dce_svc_printf(FTS_S_MAIN_GIVING_THE_FOR_MSG, 
			   programName, as->name,
			   (nonlcl == 1 ? "it" : "them"),
			   fts_ivToString(CurrV[rv_DMR]),
			   fromNames[rvFrom[rv_DMR]], rvNames[rv_DMR]);
	} else {
	    dce_svc_printf(FTS_S_MAIN_NO_TO_GIVE_MSG, 
			   programName, as->name, rvNames[rv_DMR],
			   (nonlcl == 1 ? "this site" : "these sites"));
	    OK = 0;
	}
    }
    /* Find the max and min per-site maxsiteage values. */
    maxrepix = minrepix = -1;
    for (ix = 0; ix < entry.nServers; ++ix) {
	if (entry.siteFlags[ix] & VLSF_SAMEASPREV) continue;
	if ((entry.siteFlags[ix] & ROmask) == 0) continue;	/* not this server */
	if (repStyle == 0 && ix == StageIx) continue;	/* don't count staging site */
	if (maxrepix < 0 || currmaxrep < entry.sitemaxReplicaLatency[ix]) {
	    currmaxrep = entry.sitemaxReplicaLatency[ix];
	    maxrepix = ix;
	}
	if (minrepix < 0 || currminrep > entry.sitemaxReplicaLatency[ix]) {
	    currminrep = entry.sitemaxReplicaLatency[ix];
	    minrepix = ix;
	}
    }
    if (CurrV[rv_MAXT] < (2*60)) {
	dce_svc_printf(FTS_S_MAIN_ERROR_MUST_BE_MSG, programName, as->name,
		  capFromNames[rvFrom[rv_MAXT]], rvNames[rv_MAXT],
		  fts_ivToString(CurrV[rv_MAXT]));
	dce_svc_printf(FTS_S_MAIN_AT_LEAST_MSG, fts_ivToString(2*60));
	OK = 0;
    }
    if (CurrV[rv_HMT] < CurrV[rv_MAXT]) {
	dce_svc_printf(FTS_S_MAIN_NO_SMALLER_MSG, programName, as->name,
		  fts_ivToString(CurrV[rv_HMT]),
		  fromNames[rvFrom[rv_HMT]], rvNames[rv_HMT]);
	dce_svc_printf(FTS_S_MAIN_THAN_TO_FOR_MSG, fts_ivToString(CurrV[rv_MAXT]),
		  fromNames[rvFrom[rv_MAXT]], rvNames[rv_MAXT]);
	OK = 0;
    }
    if (CurrV[rv_RECL] < (90*60)) {	/* valid constraint for release-style replication, also */
	dce_svc_printf(FTS_S_MAIN_MUST_BE_MSG, programName, as->name,
		  capFromNames[rvFrom[rv_RECL]], rvNames[rv_RECL],
		  fts_ivToString(CurrV[rv_RECL]));
	dce_svc_printf(FTS_S_MAIN_AT_LEAST_MSG, fts_ivToString(90*60));
	OK = 0;
    }
    if (repStyle == 1 && minrepix >= 0 && CurrV[rv_MINPOUNCE] >= currminrep) {
	dce_svc_printf(FTS_S_MAIN_FOR_MUST_BE_MSG, programName, as->name,
		  capFromNames[rvFrom[rv_MINPOUNCE]], rvNames[rv_MINPOUNCE],
		  fts_ivToString(CurrV[rv_MINPOUNCE]));
	dce_svc_printf(FTS_S_MAIN_SMALLER_MAXSITEAGE_MSG, fts_ivToString(currminrep),
		  MapSockAddr(&entry.siteAddr[minrepix]), entry.sitePartition[minrepix]);
	OK = 0;
    }
    if (repStyle == 1 && maxrepix >= 0 && CurrV[rv_MAXT] <= currmaxrep) {
	dce_svc_printf(FTS_S_MAIN_FOR_MUST_BE_MSG, programName, as->name,
		  capFromNames[rvFrom[rv_MAXT]], rvNames[rv_MAXT],
		  fts_ivToString(CurrV[rv_MAXT]));
	dce_svc_printf(FTS_S_MAIN_LARGER_MAXSITEAGE_MSG, fts_ivToString(currmaxrep),
		  MapSockAddr(&entry.siteAddr[maxrepix]), entry.sitePartition[maxrepix]);
	OK = 0;
    }
    if (repStyle == 1 && CurrV[rv_DMR] != 0) {
	if (CurrV[rv_MINPOUNCE] >= CurrV[rv_DMR]) {
	    dce_svc_printf(FTS_S_MAIN_FOR_MUST_BE_MSG, programName, as->name,
		      capFromNames[rvFrom[rv_MINPOUNCE]], rvNames[rv_MINPOUNCE],
		      fts_ivToString(CurrV[rv_MINPOUNCE]));
	    dce_svc_printf(FTS_S_MAIN_SMALLER_THAN_MSG, fromNames[rvFrom[rv_DMR]], rvNames[rv_DMR],
		      fts_ivToString(CurrV[rv_DMR]));
	    OK = 0;
	}
	if (CurrV[rv_DMR] >= CurrV[rv_MAXT]) {
	    dce_svc_printf(FTS_S_MAIN_MUST_BE_MSG, programName, as->name,
		      capFromNames[rvFrom[rv_DMR]], rvNames[rv_DMR],
		      fts_ivToString(CurrV[rv_DMR]));
	    dce_svc_printf(FTS_S_MAIN_SMALLER_THANE_MSG, fromNames[rvFrom[rv_MAXT]], rvNames[rv_MAXT],
		      fts_ivToString(CurrV[rv_MAXT]));
	    OK = 0;
	}
    }
    if (!OK) {
	dce_svc_printf(FTS_S_MAIN_INCONSISTENT_REP_INFO_MSG, programName, as->name, entry.name);
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }

    /* So far so good.  If we're changing from release replication to scheduled replication, and there's
      a distinguished replica (managed by FTS), we have to nuke that now. */
    if (repStyle == 1 && realoldStyle <= 0 && possStageIx >= 0) {	/* to -sched? */
	/* Nuke any old distinguished replica. */
	code = volc_Call(VC_VolumeZap, &entry.siteAddr[possStageIx],
			 (u_char *)&entry.sitePrincipal[possStageIx].text[0],
			 entry.sitePartition[possStageIx], &ROvolId);
	if (code != 0 && code != ENOENT && code != ENODEV) {
	    dce_svc_printf(FTS_S_MAIN_OLD_DISTINGUISHED_MSG, programName, as->name, entry.name);
	    (void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	    return (PrintError(as->name, code));
	}
    }

    /* All seems fine.  Set the new parameters in the FLDB entry. */
    vldb_SaveOldEntry(&entry);
    entry.flags |= VLF_LAZYREP;
    if (repStyle) {
	entry.flags &= ~(VLF_LAZYRELE);	/* tag as sched-rep */
    } else {
	entry.flags |= VLF_LAZYRELE;	/* tag as release-rep */
    }
#if !defined(OSF_NO_SOCKET_DUMP)
    if (as->parms[SETREPL_SOCKFLAG_OFFSET].items) {
	entry.flags |= VLF_SOCKFLAG;
    } else if (as->parms[SETREPL_CLEAR_OFFSET].items) {
	entry.flags &= ~(VLF_SOCKFLAG);
    }
#endif /* OSF_NO_SOCKET_DUMP */
    for (ix = 0; ix < rv_LEN; ++ix) {
	if (repStyle == 1 || (ix != rv_MINPOUNCE && ix != rv_DMR)) {
	    if (rvFrom[ix] == rvf_Derived || rvFrom[ix] == rvf_Given || rvFrom[ix] ==rvf_Default) {
		dce_svc_printf(FTS_S_MAIN_USING_MSG, programName, as->name,
			  fromNames[rvFrom[ix]], rvNames[ix], fts_ivToString(CurrV[ix]));
	    }
	}
    }
    entry.maxTotalLatency = CurrV[rv_MAXT];
    entry.hardMaxTotalLatency = CurrV[rv_HMT];
    entry.minimumPounceDally = CurrV[rv_MINPOUNCE];
    entry.defaultMaxReplicaLatency = CurrV[rv_DMR];
    entry.reclaimDally = CurrV[rv_RECL];
    if (code = vldb_ReplaceEntry(&entry, &volId, volType, LOCKREL_ALL)) {
	if (repStyle == 1 && realoldStyle == 0 && oldStageIx >= 0) {
	    dce_svc_printf(FTS_S_MAIN_FORMER_DISTINGUISHED_MSG, programName, as->name);
	}
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }
    (void) vldb_TellRepAboutNewEntry(&entry, 0, 1);
    return(0);
}

/*
 *  "status <server>"
 */
EXPORT int fts_DoVolserStatus(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{   
    long code;
    register int i;
    struct sockaddr servAddr;
    ftserver_transEntries oneStatus;

    if (!GetServer(as->parms[STATUS_SERVER_OFFSET].items->data, &servAddr)) /* server */
	exit(1);
    if (code = volc_Call(VC_VolserStatus, &servAddr, NULL, &oneStatus))
	return (PrintError(as->name, code));
    if (oneStatus.ftserver_tranStatus_len == 0) 
	dce_svc_printf(FTS_S_MAIN_NO_ACTIVE_TRANS_MSG, as->parms[STATUS_SERVER_OFFSET].items->data);
    else
	dce_svc_printf(FTS_S_MAIN_TOTAL_TRANS_MSG, oneStatus.ftserver_tranStatus_len);
    for (i = 0; i < oneStatus.ftserver_tranStatus_len; ++i)
	PrintVolserStatus(&oneStatus.ftserver_transStatus_val[i]);
    return 0;
}

/*
 *  "syncfldb <server> [aggr]"
 */
EXPORT int fts_DoSyncVldb(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    long code;
    u_long aggrId = (unsigned long) -1;
    struct sockaddr servAddr;
    char *serverNamep = as->parms[SYNCFLDB_SERVER_OFFSET].items->data;
    int	isAggrPresent = (as->parms[SYNCFLDB_AGGREGATE_OFFSET].items !=
			 (struct cmd_item *)NULL);
    char *aggrp;
    char aggrName[MAX_AGGRNAME];

    aggrp = (isAggrPresent ?
	     as->parms[SYNCFLDB_AGGREGATE_OFFSET].items->data : (char *)NULL);

    if (!GetServer(serverNamep, &servAddr))				/* server */
	exit(1);
    if (isAggrPresent) {						/* [aggr] */
	if (!GetAggr(aggrp, &servAddr, GETAGGR_VALIDATE, &aggrId, aggrName))
	    exit(1);
    }
    if (code = volc_Call(VC_SyncVldb, &servAddr, NULL, aggrId))
	return (PrintError(as->name, code));
    if (aggrId != (u_long) -1)
	dce_svc_printf(FTS_S_MAIN_FLDB_SYNCHED_MSG, serverNamep, aggrName);
    else 
	dce_svc_printf(FTS_S_MAIN_FLDB_SERVER_SYNCHED_MSG, serverNamep);
    return 0;
}

/*
 *  "syncserv <server> [aggr]"
 */
EXPORT int fts_DoSyncServer(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    long code;
    u_long aggrId = (unsigned long) -1;
    struct sockaddr servAddr;
    char *serverNamep = as->parms[SYNCSERV_SERVER_OFFSET].items->data;
    int	isAggrPresent = (as->parms[SYNCSERV_AGGREGATE_OFFSET].items !=
			 (struct cmd_item *)NULL);
    char *aggrp;
    char aggrName[MAX_AGGRNAME];

    aggrp = (isAggrPresent ?
	     as->parms[SYNCSERV_AGGREGATE_OFFSET].items->data : (char *)NULL);

    if (!GetServer(serverNamep, &servAddr))				/* server */
	exit(1);
    if (isAggrPresent) {						/* [aggr] */
	if (!GetAggr(aggrp, &servAddr, GETAGGR_VALIDATE, &aggrId, aggrName))
	    exit(1);
    }
    if (code = volc_Call(VC_SyncServer, &servAddr, NULL, aggrId))
	return (PrintError(as->name, code));
    if (aggrId != -1) 
	dce_svc_printf(FTS_S_MAIN_SERVER_AGG_SYNCHED_MSG, serverNamep, aggrName);
    else 
	dce_svc_printf(FTS_S_MAIN_SERVER_FLDB_SYNCH_MSG, serverNamep);
    return 0;
}

/*
 * "unlockfldb [server] [aggr]"
 */
EXPORT int fts_DoUnlockFLDB(as, aRock)
register struct cmd_syndesc *	as;
char *				aRock;
{

    if (as->parms[UNLOCKFLDB_AGGREGATE_OFFSET].items
	 && !as->parms[UNLOCKFLDB_SERVER_OFFSET].items) {
	dce_svc_printf(FTS_S_MAIN_AGG_NEEDS_SERVER_MSG, programName, as->name,
		  as->parms[UNLOCKFLDB_AGGREGATE_OFFSET].items->data);
	exit(1);
    }
    if (vldb_ProcessByAttrs((as->parms[UNLOCKFLDB_SERVER_OFFSET].items ?
			      as->parms[UNLOCKFLDB_SERVER_OFFSET].items->data : (char *)0),
			     (as->parms[UNLOCKFLDB_AGGREGATE_OFFSET].items ?
			      as->parms[UNLOCKFLDB_AGGREGATE_OFFSET].items->data :
			      (char *)0),
			     1, NULL, 3 /* BULK_UNLOCK */))
	exit(1);
    return 0;
}

/*
 * "update <id> <server> <aggr>"
 */
EXPORT int fts_DoRepUpdate(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    struct vldbentry entry;
    struct sockaddr servAddr;
    afs_hyper_t volId, *roVolIDp;
    char *volumeNamep = as->parms[UPDATE_FILESET_OFFSET].items->data;
    char *serverNamep;
    char aggrName[MAX_AGGRNAME];
    rpc_binding_handle_t serverconn;
    long code, aggrId;
    struct sockaddr_in *sinp;
    int numSent, allFlag;
    u_long ROflag, volType;
    register int ix;
    extern afs_hyper_t remoteCellID;
    extern long REP_UpdateSelf();
    u_long spare4, spare5;
    unsigned long st;

    fts_InitDecoder();
    if (!GetVolumeID(volumeNamep, &volId))			/* volume */
	exit(1);
    if (code = vldb_GetVldbInfo(&volId, &servAddr, &aggrId, &volType, &entry))
	exit(1);
    if ((entry.flags & VLF_LAZYREP) == 0) {
	dce_svc_printf(FTS_S_MAIN_SET_REP_TYPE_MSG, programName, as->name);
	exit(1);
    }
#if 0
    if ((entry.flags & VLF_LAZYRELE) != 0) {
	dce_svc_printf(FTS_S_MAIN_UPDATE_REP_MSG, programName, as->name);
	exit(1);
    }
#endif /* 0 */
    serverNamep = NULL;
    allFlag = 0;
    if (as->parms[UPDATE_SERVER_OFFSET].items) {
	serverNamep = as->parms[UPDATE_SERVER_OFFSET].items->data;
	if (!GetServer(serverNamep, &servAddr))		/* server */
	    exit(1);
    }
    if (as->parms[UPDATE_ALL_OFFSET].items)
	allFlag = 1;
    if (allFlag == 0 && serverNamep == NULL) {
	dce_svc_printf(FTS_S_MAIN_ALL_OR_SERVER_MSG, programName, as->name);
	exit(1);
    }
    if (allFlag != 0 && serverNamep != NULL) {
	dce_svc_printf(FTS_S_MAIN_IGNORE_SERVER_MSG, programName, as->name);
	serverNamep = NULL;
    }

    roVolIDp = NULL;
    for (ix = 0; ix < MAXNSERVERS && !AFS_hiszero(entry.VolIDs[ix]); ++ix) {
	if (entry.VolTypes[ix] == VOLTIX_TO_VOLTYPE(ROVOL)) {
	    roVolIDp = &entry.VolIDs[ix];
	    ROflag = ((unsigned32)VLSF_ZEROIXHERE) >> ix;
	    break;
	}
    }
    if (roVolIDp == NULL) {
	dce_svc_printf(FTS_S_MAIN_NO_RO_ID_MSG,
		       programName, as->name, AFS_HGETBOTH(volId));
	exit(1);
    }
    (void) vldb_GetRemoteCellID();
    numSent = 0;
    for (ix = 0; ix < entry.nServers; ++ix) {
	if ((entry.siteFlags[ix] & ROflag) != 0
	    && (serverNamep == NULL
		|| bcmp((char *)&entry.siteAddr[ix], (char *)&servAddr, sizeof(servAddr)) == 0)) {
	    sinp = (struct sockaddr_in *) &entry.siteAddr[ix];
	    serverconn = connToServer((struct sockaddr *) sinp,
				      (u_char *)(&entry.sitePrincipal[ix].text[0]),
				      SERVERKIND_REP);
	    if (serverconn != NULL) {
		(void) fts_UseThisTimeout(serverconn, rpc_c_binding_default_timeout);
		rpc_mgmt_set_call_timeout(serverconn, 150 /* sec */, &st);
		if (st != error_status_ok) {
		    dce_svc_printf(FTS_S_MAIN_UNSET_TIMEOUT_MSG, MapSockAddr(sinp), dfs_dceErrTxt(st));
		    code = st;
		} else {
		    VOLSERLOG(1, "Calling REP_UpdateSelf(%s, {%lu,,%lu}, {%lu,,%lu}, ...)...\n",
			      MapSockAddr(sinp), AFS_HGETBOTH(remoteCellID),
			      AFS_HGETBOTH(*roVolIDp));
		    code = REP_UpdateSelf(serverconn, &remoteCellID, roVolIDp, 0, 0, 0, 0,
					  &spare4, &spare5);
		    code = osi_errDecode(code);
		    if (code == REP_ERR_NOVOLUME)
			code = 0;		/* authoritative answer from repserver */
		    if (code != 0) {
			dce_svc_printf(FTS_S_MAIN_NO_UPDATE_REPSERVER_MSG,
				       programName, as->name,
				       MapSockAddr(sinp), dfs_dceErrTxt(code));
		    } else {
			dce_svc_printf(FTS_S_MAIN_REPSERVER_UPDATE_MSG, programName, as->name,
			       MapSockAddr(sinp), AFS_HGETBOTH(*roVolIDp));
		    }
		}
		/* rpc_binding_free((rpc_binding_handle_t *)&serverconn, &st); */
	    } else {
		dce_svc_printf(FTS_S_MAIN_NO_REPSERVER_CONNECT_MSG, programName, as->name, MapSockAddr(sinp));
	    }
	    ++numSent;
	}
    }
    if (numSent == 0) {
	if (serverNamep) {
	    dce_svc_printf(FTS_S_MAIN_NO_IP_IN_FLDB_MSG,
			   programName, as->name, AFS_HGETBOTH(volId),
			   entry.name, MapSockAddr(&servAddr), aggrId);
	} else {
	    dce_svc_printf(FTS_S_MAIN_NO_RO_IN_FLDB_MSG, programName, as->name,
			   AFS_HGETBOTH(volId), entry.name);
	}
	exit(1);
    }
    return 0;
}

/*
 *  "zap <server> <aggr> <fileset>"
 */
EXPORT int fts_DoVolumeZap(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    afs_hyper_t volId;
    long code;
    u_long aggrId;
    struct sockaddr servAddr;
    char *volumeNamep = as->parms[ZAP_FSID_OFFSET].items->data;
    char *aggrp = as->parms[ZAP_AGGREGATE_OFFSET].items->data;
    char *serverNamep = as->parms[ZAP_SERVER_OFFSET].items->data;
    char aggrName[MAX_AGGRNAME];

    if (!GetServer(serverNamep, &servAddr))				/* server */
	exit(1);
    if (!GetAggr(aggrp, &servAddr, GETAGGR_VALIDATE, &aggrId, aggrName))		/* aggr */
	exit(1);
    if (fts_IsNumeric(volumeNamep, 0, 1, &volId) == 0) {			/* vol ID */
	dce_svc_printf(FTS_S_MAIN_NEED_NUMERIC_ID_MSG, programName, as->name);
	exit(1);
    }
    code = fts_NeedAggrType(&servAddr, aggrId, AG_TYPE_ANY_EFS_MASK, "Zapping is");
    if (code)
	exit(1);
    if (code = volc_Call(VC_VolumeZap, &servAddr, NULL, aggrId, &volId))
	return (PrintError(as->name, code));
    dce_svc_printf(FTS_S_MAIN_FILESET_AGG_DELETED_MSG,
		   AFS_HGETBOTH(volId), serverNamep, aggrName);
    return 0;
}

/*
 *  "newserver  -address <addr> [-principal <s>] [-quota <d>] [-owner <s>] [-objid <s>]"
 */
EXPORT int fts_DoCrServer(as, aRock)
register struct cmd_syndesc *	as;
char *				aRock;
{
    struct siteDesc Descr;
    register long code;
    register int ix;
    afs_hyper_t dum;
    char *serverNamep = as->parms[NEWSRV_ADDRESS_OFFSET].items->data;

    bzero((char *)&Descr, sizeof(Descr));
    if (!GetServer(serverNamep, &Descr.Addr[0]))		/* server */
	exit(1);
    for (ix = 1; ix < ADDRSINSITE; ++ix) {
	Descr.Addr[ix].type = (unsigned short) -1;
    }
    if (as->parms[NEWSRV_PRINCIPAL_OFFSET].items) {
	if (strlen(as->parms[NEWSRV_PRINCIPAL_OFFSET].items->data) >= (unsigned)MAXKPRINCIPALLEN) {
	    dce_svc_printf(FTS_S_MAIN_TOO_LONG_NAME_MSG, programName, as->name,
		      as->parms[NEWSRV_PRINCIPAL_OFFSET].items->data,
		      MAXKPRINCIPALLEN-1);
	    exit(1);
	}
	strncpy((char *)&Descr.KerbPrin[0],
		as->parms[NEWSRV_PRINCIPAL_OFFSET].items->data,
		sizeof(Descr.KerbPrin));
    }
    if (as->parms[NEWSRV_QUOTA_OFFSET].items) {
	if (fts_IsNumeric(as->parms[NEWSRV_QUOTA_OFFSET].items->data,
			  0, 0, &dum) == 0) {
	    dce_svc_printf(FTS_S_MAIN_BAD_QUOTA_MSG, programName, as->name,
		      as->parms[NEWSRV_QUOTA_OFFSET].items->data);
	    exit(1);
	}
	Descr.CreationQuota = AFS_hgetlo(dum);
    }
    if (as->parms[NEWSRV_OWNER_OFFSET].items) {
	code = fts_GroupToUuid(as->parms[NEWSRV_OWNER_OFFSET].items->data,
			       &Descr.Owner);
	if (code) {
	    dce_svc_printf(FTS_S_MAIN_BAD_OWNING_GROUP_MSG, programName, as->name, 
		      as->parms[NEWSRV_OWNER_OFFSET].items->data,
		      dfs_dceErrTxt(code));
	    exit(1);
	}
    }
#if 0
    if (as->parms[NEWSRV_OBJID_OFFSET].items) {
	code = fts_strToUuid(as->parms[NEWSRV_OBJID_OFFSET].items->data,
			     &Descr.ObjID);
	if (code) {
	    dce_svc_printf(FTS_S_MAIN_FXDID_TO_UUID_MSG, programName, as->name, dfs_dceErrTxt(code));
	    exit(1);
	}
    }
#endif /* 0 */
    code = vldb_CreateServer(&Descr);
    if (code) exit(1);
    return(0);
}

/*
 * "alterserver -server <name/addr> [-deladdr] [-addaddr <name/addr>]
    [-changeaddr <name/addr>] [-principal <s>] [-quota <d>] [-owner <s>] [-objid <s>]"
 */
EXPORT int fts_DoAlterServer(as, aRock)
register struct cmd_syndesc *	as;
char *				aRock;
{
    struct siteAlter alterAttrs;
    struct afsNetAddr servAddr;
    struct afsNetAddr newAddr;
    register long code;
    register int ix;
    afs_hyper_t dum;
    char *serverNamep = as->parms[ALTSRV_ADDRESS_OFFSET].items->data;

    if (!GetServer(serverNamep, &servAddr))			/* server to modify */
	exit(1);
    if (as->parms[ALTSRV_CHANGEADDR_OFFSET].items && as->parms[ALTSRV_DELADDR_OFFSET].items) {
	dce_svc_printf(FTS_S_MAIN_DELETE_AND_CHANGE_MSG, programName, as->name);
	exit(1);
    }
    bzero((char *)&alterAttrs, sizeof(alterAttrs));
    if (as->parms[ALTSRV_SETPRINCIPAL_OFFSET].items) {
	if (strlen(as->parms[ALTSRV_SETPRINCIPAL_OFFSET].items->data) >= (unsigned)MAXKPRINCIPALLEN) {
	    dce_svc_printf(FTS_S_MAIN_NAME_TOO_LONG_MSG, programName, as->name,
		      as->parms[ALTSRV_SETPRINCIPAL_OFFSET].items->data,
		      MAXKPRINCIPALLEN-1);
	    exit(1);
	}
	strncpy((caddr_t)&alterAttrs.KerbPrin[0],
		as->parms[ALTSRV_SETPRINCIPAL_OFFSET].items->data,
		sizeof(alterAttrs.KerbPrin));
	alterAttrs.Mask |= SITEALTER_PRINCIPAL;
    }
    if (as->parms[ALTSRV_SETQUOTA_OFFSET].items) {
	if (fts_IsNumeric(as->parms[ALTSRV_SETQUOTA_OFFSET].items->data,
			  0, 0, &dum) == 0) {
	    dce_svc_printf(FTS_S_MAIN_BAD_QUOTA_MSG, programName, as->name,
		      as->parms[ALTSRV_SETQUOTA_OFFSET].items->data);
	    exit(1);
	}
	alterAttrs.CreationQuota = AFS_hgetlo(dum);
	alterAttrs.Mask |= SITEALTER_CREATIONQUOTA;
    }
    if (cmd_IsParamPresent(as, ALTSRV_NOOWNER_OFFSET)) {
	/* remove ownership: uuid is already set to all 0's */
	alterAttrs.Mask |= SITEALTER_OWNER;
    }
    if (as->parms[ALTSRV_SETOWNER_OFFSET].items) {
	code = fts_GroupToUuid(as->parms[ALTSRV_SETOWNER_OFFSET].items->data,
			       &alterAttrs.Owner);
	if (code) {
	    dce_svc_printf(FTS_S_MAIN_GROUP_TO_UUID_MSG, programName, as->name, 
		      as->parms[ALTSRV_SETOWNER_OFFSET].items->data,
		      dfs_dceErrTxt(code));
	    exit(1);
	}
	alterAttrs.Mask |= SITEALTER_OWNER;
    }
#if 0
    if (as->parms[ALTSRV_SETOBJID_OFFSET].items) {
	code = fts_strToUuid(as->parms[ALTSRV_SETOBJID_OFFSET].items->data,
			     &alterAttrs.ObjID);
	if (code) {
	    dce_svc_printf(FTS_S_MAIN_FXDID_TO_UUID_MSG, programName, as->name, dfs_dceErrTxt(code));
	    exit(1);
	}
	alterAttrs.Mask |= SITEALTER_OBJID;
    }
#endif /* 0 */
    if (alterAttrs.Mask != 0) {
	code = vldb_AlterServer(&servAddr, &alterAttrs);
	if (code) exit(1);
    }
    if (as->parms[ALTSRV_CHANGEADDR_OFFSET].items) {
	if (!GetServer(as->parms[ALTSRV_CHANGEADDR_OFFSET].items->data, &newAddr))
	    exit(1);
	code = vldb_ChangeAddress(&servAddr, &newAddr);
	if (code) exit(1);
	servAddr = newAddr;
    }
    newAddr.type = (unsigned short) -1;
    if (as->parms[ALTSRV_ADDADDR_OFFSET].items) {
	if (!GetServer(as->parms[ALTSRV_ADDADDR_OFFSET].items->data, &newAddr))
	    exit(1);
    }
    if (as->parms[ALTSRV_DELADDR_OFFSET].items) {
	if (newAddr.type != (unsigned short) -1) {
	    code = vldb_ChangeAddress(&servAddr, &newAddr);
	    if (code) exit(1);
	    servAddr = newAddr;
	    newAddr.type = (unsigned short) -1;
	} else {
	    code = vldb_RemoveAddress(&servAddr);
	    if (code) exit(1);
	}
    }
    if (newAddr.type != (unsigned short) -1) {
	code = vldb_AddAddress(&servAddr, &newAddr);
	if (code) exit(1);
    }
    return(0);
}

/*
 * "lsserver -address <name/addr>"
 */
EXPORT int fts_DoLsServer(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    register int ix;
    struct siteDesc Descr;
    struct afsNetAddr servAddr;
    int allFlag;
    char *serverNamep;
    register long code;
    u_long start, nextstart;
    u_long nsites;
    bulkSites Sites;

    allFlag = 0;
    if (as->parms[SHOWSRV_ALL_OFFSET].items) allFlag = 1;
    serverNamep = NULL;
    if (as->parms[SHOWSRV_ADDRESS_OFFSET].items) {
	serverNamep = as->parms[SHOWSRV_ADDRESS_OFFSET].items->data;
	if (!GetServer(serverNamep, &servAddr))		/* server to look at */
	    exit(1);
    }

    if (allFlag) {
	start = 0;
	for (;;) {
	    code = vldb_GenerateSites(start, &nextstart, &Sites, &nsites);
	    if (code == VL_ENDOFLIST || nsites == 0) break;
	    if (code != 0) exit(1);
	    for (ix = 0; ix < nsites; ++ix) {
		if (start != 0 || ix != 0) dce_svc_printf(FTS_S_MAIN_NEWLINE_MSG);
		PrintSiteInfo(&Sites.Sites[ix], NULL);
	    }
	    start = nextstart;
	}
    } else {
	code = vldb_GetSiteInfo(&servAddr, &Descr);
	if (code) exit(1);
	PrintSiteInfo(&Descr, serverNamep);
    }
    return 0;
}

/*
 * "delserverentry -server <name/addr> [-cell <name>]
 */
EXPORT int fts_DoDelServer (as, aRock)
     register struct cmd_syndesc *    as;
     char *                           aRock;
{
    struct siteAlter alterAttrs;
    struct afsNetAddr servAddr;
    register long code;
    char *serverNamep = as->parms[DELSRV_ADDRESS_OFFSET].items->data;

    if (!GetServer(serverNamep, &servAddr))
      exit(1);
    bzero((char *)&alterAttrs, sizeof(alterAttrs));
    alterAttrs.Mask |= SITEALTER_DELETEME;

    code = vldb_AlterServer(&servAddr, &alterAttrs);
    if (code) exit(1);
    return 0;
}

#if 0
/*
 * "reserveids [-count <d>] [-server <name/addr>]"
 */
EXPORT int fts_DoReserveIDs(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    struct afsNetAddr servAddr;
    long code, num;
    register int ix;
    unsigned long hi, lo;
    afs_hyper_t dum;
    bulkIDs IDs;

    num = 1;	/* default value */
    if (as->parms[RSVIDS_COUNT_OFFSET].items) {
	if (fts_IsNumeric(as->parms[RSVIDS_COUNT_OFFSET].items->data,
			  0, 0, &dum) == 0) {
	    dce_svc_printf(FTS_S_MAIN_BAD_COUNT_MSG, programName, as->name,
		      as->parms[RSVIDS_COUNT_OFFSET].items->data);
	    exit(1);
	}
	num = AFS_hgetlo(dum);
    }
    if (num > MAXBULKLEN) {
	dce_svc_printf(FTS_S_MAIN_NOT_RESERVE_ID_MSG, programName, as->name, MAXBULKLEN, num);
	exit(1);
    }
    if (num <= 0) {
	dce_svc_printf(FTS_S_MAIN_NEED_NEW_IDS_MSG, programName, as->name);
	exit(1);
    }
    servAddr.type = (unsigned short) -1;
    if (as->parms[RSVIDS_SERVER_OFFSET].items) {
	if (!GetServer(as->parms[RSVIDS_SERVER_OFFSET].items->data, &servAddr))
	    exit(1);
    }
    code = vldb_GetNewVolumeIds(num, &servAddr, &IDs);
    if (code) exit(1);

    if (IDs.bulkIDs_len == 1) {
	AFS_hget64(hi, lo, IDs.bulkIDs_val[0]);
	dce_svc_printf(FTS_S_MAIN_ALLOCATED_FILESET_ID_MSG, hi, lo);
    } else {
	for (ix = 0; ix < IDs.bulkIDs_len; ++ix) {
	    AFS_hget64(hi, lo, IDs.bulkIDs_val[ix]);
	    dce_svc_printf(FTS_S_MAIN_ALLOC_FILESET_ID_MSG, ix+1, hi, lo);
	}
    }
    return 0;
}
#endif /* 0 */

/*
 * "getrepstatus -fileset [-all] [-server <name/addr>]"
 */
EXPORT int fts_DoGetRepStatus(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    struct afsNetAddr servAddr;
    afs_hyper_t volId, roVolId;
    long code;
    int numPrinted, Errs, stagingIx;
    struct vldbentry ventry;
    unsigned long cookie, nextcookie, flags, spare4, spare5, howMany;
    register int ix, A;
    char *volumeNamep = as->parms[GETRSTAT_FILESET_OFFSET].items->data;
    char *serverNamep;
    char *aggrp;
    char aggrName[MAX_AGGRNAME];
    struct repStatus oneStat;
    rpc_binding_handle_t serverconn;
    struct sockaddr_in *sinp;
    unsigned long ROflag, st;
    extern afs_hyper_t remoteCellID;

    fts_InitDecoder();
    if (!GetVolumeID(volumeNamep, &volId))			/* volume */
	exit(1);
    /* server address (optional) */
    servAddr.type = (unsigned short) -1;
    if (as->parms[GETRSTAT_SERVER_OFFSET].items) {
	if (!GetServer(as->parms[GETRSTAT_SERVER_OFFSET].items->data, &servAddr))
	    exit(1);
    }
    code = vldb_GetEntryByID(&volId, -1, &ventry, 0);
    if (code != 0) exit(1);
    ROflag = 0;
    for (A = 0; A < MAXVOLTYPES && !AFS_hiszero(ventry.VolIDs[A]); ++A) {
	if (ventry.VolTypes[A] == VOLTIX_TO_VOLTYPE(ROVOL)) {
	    ROflag = ((unsigned32)VLSF_ZEROIXHERE) >> A;
	    roVolId = ventry.VolIDs[A];
	}
    }
    if ((ventry.flags & VLF_LAZYREP) == 0 || ROflag == 0) {
	dce_svc_printf(FTS_S_MAIN_NOT_REPLICATD_MSG, programName, as->name, volumeNamep);
	exit(1);
    }
    stagingIx = fts_releaseRepSite(&ventry);	/* < 0 for non-release-replicas */
    numPrinted = Errs = 0;
    (void) vldb_GetRemoteCellID();
    if (as->parms[GETRSTAT_ALLFLAG_OFFSET].items) {
	for (A = 0; A < ventry.nServers; ++A) {
	    if (ventry.siteFlags[A] & VLSF_SAMEASPREV) continue;
	    if (stagingIx == A) continue;	/* don't probe the staging replica */
	    if (ventry.siteFlags[A] & ROflag) {
		for (ix = 0; ix < A; ++ix) {
		    if (ventry.siteFlags[ix] & VLSF_SAMEASPREV) continue;
		    if (stagingIx == ix) continue;	/* didn't probe the staging replica */
		    if ((ventry.siteFlags[ix] & ROflag) == 0) continue;
		    if (bcmp((char *)&ventry.siteAddr[ix],
			     (char *)&ventry.siteAddr[A],
			     sizeof(afsNetAddr)) == 0)
			break;
		}
		if (ix < A) continue;		/* we already saw this server */
		servAddr = ventry.siteAddr[A];
		sinp = (struct sockaddr_in *) &servAddr;
		serverconn = connToServer((struct sockaddr *) sinp,
					  (u_char *)(&ventry.sitePrincipal[A].text[0]),
					  SERVERKIND_REP);
		if (serverconn != NULL) {
		    (void) fts_UseThisTimeout(serverconn, rpc_c_binding_default_timeout);
		    rpc_mgmt_set_call_timeout(serverconn, 150 /* sec */, &st);
		    if (st != error_status_ok) {
			dce_svc_printf(FTS_S_MAIN_CANT_SET_TIMEOUT_MSG, MapSockAddr(sinp), dfs_dceErrTxt(st));
			code = st;
		    } else {
			code = flags = 0;
			bzero((char *)&oneStat, sizeof(struct repStatus));
			VOLSERLOG(1, "Calling REP_GetOneRepStatus(%s, {%lu,,%lu}, {%lu,,%lu}, ..)...\n",
				  MapSockAddr(sinp),
				  AFS_HGETBOTH(remoteCellID),
				  AFS_HGETBOTH(roVolId));
			code = REP_GetOneRepStatus(serverconn, &remoteCellID, &roVolId,
						   0, 0, 0, &oneStat, &flags, &spare4, &spare5);
			code = osi_errDecode(code);
			if (code == 0) {
			  dce_printf(fts_s_main_on_sockaddr,
				     (numPrinted ? "\n" : ""),
				     MapSockAddr(&servAddr));
			  PrintRepStatus(&oneStat, &servAddr);
			  ++numPrinted;
			} else {
			    dce_svc_printf(FTS_S_MAIN_NO_REP_STATUS_MSG, programName, as->name,
				      MapSockAddr(&servAddr), dfs_dceErrTxt(code));
			    ++Errs;
			}
		    }
		    /* rpc_binding_free(&serverconn, &st); */
		} else {
		    dce_svc_printf(FTS_S_MAIN_NO_REP_CONN_MSG, programName, as->name, MapSockAddr(&servAddr));
		    ++Errs;
		}
	    }
	}
	if (numPrinted == 0 && Errs == 0)
	    dce_svc_printf(FTS_S_MAIN_NO_REPLICAS_MSG, programName, as->name, volumeNamep);
    } else {
	/* the case where the server was indicated */
	sinp = (struct sockaddr_in *) &servAddr;
	serverconn = connToServer((struct sockaddr *) sinp, NULL,
				  SERVERKIND_REP);
	if (serverconn != NULL) {
	    (void) fts_UseThisTimeout(serverconn, rpc_c_binding_default_timeout);
	    rpc_mgmt_set_call_timeout(serverconn, 150 /* sec */, &st);
	    if (st != error_status_ok) {
		dce_svc_printf(FTS_S_MAIN_NO_REP_TIMEOUT_MSG, MapSockAddr(sinp), dfs_dceErrTxt(st));
		code = st;
	    } else {
		code = flags = 0;
		bzero((char *)&oneStat, sizeof(struct repStatus));
		VOLSERLOG(1, "Calling REP_GetOneRepStatus(%s, {%lu,,%lu}, {%lu,,%lu}, ..)...\n",
			  MapSockAddr(sinp), AFS_HGETBOTH(remoteCellID),
			  AFS_HGETBOTH(roVolId));
		code = REP_GetOneRepStatus(serverconn, &remoteCellID, &roVolId,
					   0, 0, 0, &oneStat, &flags, &spare4, &spare5);
		code = osi_errDecode(code);
		if (code == 0) {
		  dce_printf(fts_s_main_on_sockaddr,
			     (numPrinted ? "\n" : ""),
			     MapSockAddr(&servAddr));
		  PrintRepStatus(&oneStat, &servAddr);
		  ++numPrinted;
		} else {
		    dce_svc_printf(FTS_S_MAIN_NO_REP_STATUS_MSG, programName, as->name,
			      MapSockAddr(&servAddr), dfs_dceErrTxt(code));
		    ++Errs;
		}
	    }
	    /* rpc_binding_free(&serverconn, &st); */
	} else {
	    dce_svc_printf(FTS_S_MAIN_NO_REP_CONN_MSG, programName, as->name, MapSockAddr(&servAddr));
	    ++Errs;
	}
	if (numPrinted == 0 && Errs == 0)
	    dce_svc_printf(FTS_S_MAIN_NO_FILESETS_MATCH_MSG, programName, as->name,
		      volumeNamep, MapSockAddr(&servAddr));
    }
    return(0);
}

/*
 * "getrepserverstatus -server <name/addr>"
 */
EXPORT int fts_DoGetRepServerStatus(as, aRock)
register struct cmd_syndesc *	as;
char *				aRock;
{
    struct sockaddr servAddr;
    int numPrinted, Errs;
    long code;
    u_long volType;
    register int ix;
    char *serverNamep = as->parms[GETRSSTAT_SERVER_OFFSET].items->data;
    struct repStatuses Stats;
    rpc_binding_handle_t serverconn;
    struct sockaddr_in *sinp;
    unsigned long cookie, nextcookie, flags, spare4, spare5, howMany, st;
    repserverStatus serverStatus;

    fts_InitDecoder();
    if (!GetServer(as->parms[GETRSSTAT_SERVER_OFFSET].items->data, &servAddr))
	exit(1);

    /* the case where the server was indicated */
    sinp = (struct sockaddr_in *) &servAddr;
    serverconn = connToServer((struct sockaddr *) sinp, NULL,
			      SERVERKIND_REP);
    if (serverconn != NULL) {
	(void) fts_UseThisTimeout(serverconn, rpc_c_binding_default_timeout);
	rpc_mgmt_set_call_timeout(serverconn, 150 /* sec */, &st);
	if (st != error_status_ok) {
	    dce_svc_printf(FTS_S_MAIN_NO_REP_TIMEOUT_MSG, MapSockAddr(sinp), dfs_dceErrTxt(st));
	    code = st;
	} else {
	    VOLSERLOG(1, "Calling REP_GetRepServerStatus(%s, ...)...\n",
		      MapSockAddr(sinp));
	    code = REP_GetRepServerStatus(serverconn, 0, 0, 0, &serverStatus, &spare4, &spare5);
	    code = osi_errDecode(code);
	    if (code != 0) {
		dce_svc_printf(FTS_S_MAIN_NO_REP_STATUS_MSG, programName, as->name,
			  MapSockAddr(sinp), dfs_dceErrTxt(code));
		exit(1);
	    }
	    PrintRepServerStatus(&serverStatus,
				 as->parms[GETRSSTAT_SERVER_OFFSET].items->data,
				 &servAddr);
	    if (as->parms[GETRSSTAT_LONG_OFFSET].items) {
		numPrinted = Errs = 0;
		code = flags = 0;
		for (cookie = 0; code == 0 && (flags & 1) == 0; cookie = nextcookie) {
		    bzero((char *)&Stats, sizeof(struct repStatuses));
		    VOLSERLOG(1, "Calling REP_GetRepStatus(%s, %d, ..)...\n",
			      MapSockAddr(sinp), cookie);
		    code = REP_GetRepStatus(serverconn, cookie, 0, 0, 0, &nextcookie, &Stats,
					    &howMany, &flags, &spare4, &spare5);
		    code = osi_errDecode(code);
		    if (code == 0) {
			if (howMany > Stats.repStatuses_len)
			    howMany = Stats.repStatuses_len;
			for (ix = 0; ix < howMany; ++ix) {
			    if (numPrinted) dce_svc_printf(FTS_S_MAIN_NEWLINE_MSG);
			    else dce_svc_printf(FTS_S_MAIN_ACTIVE_REPLICAS_MSG, as->parms[GETRSSTAT_SERVER_OFFSET].items->data);
			    PrintRepStatus(&Stats.repStatuses_val[ix], &servAddr);
			    ++numPrinted;
			}
		    } else {
			if (code != REP_ERR_EOF) {
			    dce_svc_printf(FTS_S_MAIN_NO_REP_STATUS_MSG, programName, as->name,
				      MapSockAddr(&servAddr), dfs_dceErrTxt(code));
			    ++Errs;
			}
		    }
		}
		if (numPrinted == 0 && Errs == 0)
		    dce_svc_printf(FTS_S_MAIN_NO_REPS_ON_SERVER_MSG, programName, as->name,
			      MapSockAddr(&servAddr));
	    }
	}
	/* rpc_binding_free(&serverconn, &st); */
    } else {
	dce_svc_printf(FTS_S_MAIN_NO_REP_BIND_MSG, programName, as->name, MapSockAddr(&servAddr));
    }
    return(0);
}

/*
 *  "crfldbentry <volumeName> <server> <aggr>"
 */
EXPORT int fts_DoCrFLDBEntry(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    long code, agType;
    u_long aggrId;
    struct sockaddr servAddr;
    struct vldbentry entry;
    char *volumeNamep = as->parms[CRFLDB_FTNAME_OFFSET].items->data;
    char *aggrp = as->parms[CRFLDB_AGGRID_OFFSET].items->data;
    char *serverNamep = as->parms[CRFLDB_SERVER_OFFSET].items->data;
    char aggrName[MAX_AGGRNAME];
    afs_hyper_t rwId, roId, bkId;
    bulkIDs newIDs;

    if (!GetServer(serverNamep, &servAddr))				/* server */
	exit(1);
    if (!GetAggr(aggrp, &servAddr, GETAGGR_NUMERICONLY, &aggrId, aggrName))		/* aggrid */
	exit(1);
    if (InvalidVolName(volumeNamep)) 				/* volumeName*/
	exit(1);
    AFS_hzero(rwId); AFS_hzero(roId); AFS_hzero(bkId);
    if (!vldb_GetEntryByName(volumeNamep, &entry, 1)) {
	dce_svc_printf(FTS_S_MAIN_FILESET_EXISTS_MSG, programName, as->name, volumeNamep);
	exit(1);
    }
    code = fts_GetAggrType(&servAddr, aggrId, &agType, 1);
    if (code == 0 && AG_TYPE_SUPPORTS_EFS(agType)) {
	dce_svc_printf(FTS_S_MAIN_CRFLDBENTRY_MSG, programName, as->name);
    }
    if (AFS_hiszero(rwId)) {
	/* Get three new fileset IDs in reference to this server */
	code = vldb_GetNewVolumeIds(3, &servAddr, &newIDs);
	if (code != 0) exit(1);
	if (newIDs.bulkIDs_len != 3) {
	    dce_svc_printf(FTS_S_MAIN_NUMBER_IDS_MSG, programName, as->name,
		      newIDs.bulkIDs_len);
	    exit(1);
	}
	rwId = newIDs.bulkIDs_val[0];
	roId = newIDs.bulkIDs_val[1];
	bkId = newIDs.bulkIDs_val[2];
    }
    /* 
     * Create a new VLDB entry
     */
    code = vldb_CreateVldbEntry(volumeNamep, &servAddr, aggrId, VLF_RWEXISTS, 
				 &rwId, &roId, &bkId, 0, 1);
    if (code) exit(1);
    dce_printf(fts_s_main_entry_for_fileset,
	       volumeNamep, AFS_HGETBOTH(rwId), aggrId, serverNamep);
    return 0;
}

EXPORT int fts_DoLsQuota(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
    int	rtnVal = 0;
    int	isFilesetPresent;
    int	isPathPresent;
    cmd_dataIter_t	nameIter;
    char *thisNameP;
    int	nameOffset;
    struct vldbentry entry;
    ftserver_status status;
    ftserver_aggrInfo aggrStatus;
    afs_hyper_t volId;
    long code, volType, aggrId;
    struct sockaddr servAddr;
    unsigned long xfree;
    struct afsFid fid;
    int anySeen, Pct, Warn, Warn1;
    afs_hyper_t hDum;
    unsigned long visQuotaLimit, visQuotaUsage, allocLimit, allocUsage;
    char vnbuf[AFS_NAMEMAXLEN+14];

/* &&& comment from Jean & Linda:
        can the you make sure the output shows:
               fileset name
               fileset quota
               amount of quota used
               percent of quota used
               server name
               aggregate name
               percent of aggregate used
               number of blocks used on aggregate
 */

    rtnVal = 0;

    anySeen = 0;
    defaultPath(aSyntax, LSQUOTA_PATH_OFFSET, LSQUOTA_FILESET_OFFSET);
    isFilesetPresent = cmd_IsParamPresent(aSyntax, LSQUOTA_FILESET_OFFSET);
    isPathPresent = cmd_IsParamPresent(aSyntax, LSQUOTA_PATH_OFFSET);
    nameOffset = (isFilesetPresent ?
		   LSQUOTA_FILESET_OFFSET :
		   LSQUOTA_PATH_OFFSET);
    for (thisNameP = cmdIter_FirstData(&nameIter, nameOffset, aSyntax);
	  thisNameP != (char *)NULL;
	  thisNameP = cmdIter_NextData(&nameIter)) {
	dmprintf(fts_debug_flag, FTS_DEBUG_PARSE_MASK,
		 ("%s name = %s", isFilesetPresent ? "fileset" : "path", thisNameP));
	if (isPathPresent) {
	    code = fts_PathToFid(aSyntax->name, thisNameP, &fid);
	    if (code) {rtnVal = code; continue;}
	    volId = fid.Volume;
	} else {
	    if (!GetVolumeID(thisNameP, &volId)) /* volume */ {
		rtnVal = 1;
		continue;
	    }
	}
	if (code = vldb_GetVldbInfo(&volId, &servAddr, &aggrId, &volType, &entry)) {
	    rtnVal = code;
	    continue;
	}
	if (code = volc_Call(VC_VolumeStatus, &servAddr, NULL, aggrId, &volId,
			     &status)) {
	    if (code == ENODEV)
		dce_svc_printf(FTS_S_MAIN_NOT_AT_SITE_MSG, 
			       programName, aSyntax->name,
			  (isPathPresent ? "CM" : "FLDB"));
	    PrintError(aSyntax->name, code);
	    rtnVal = code;
	    continue;
	}
	/* Get block quantities in 32-bit integers */
	hDum = status.vsd.allocLimit;
	AFS_hrightshift(hDum, 10);
	allocLimit = AFS_hgetlo(hDum);
	hDum = status.vsd.allocUsage;
	AFS_hrightshift(hDum, 10);
	allocUsage = AFS_hgetlo(hDum);
	hDum = status.vsd.visQuotaLimit;
	AFS_hrightshift(hDum, 10);
	visQuotaLimit = AFS_hgetlo(hDum);
	hDum = status.vsd.visQuotaUsage;
	AFS_hrightshift(hDum, 10);
	visQuotaUsage = AFS_hgetlo(hDum);
	if (code = volc_Call(vols_AggregateInfo, &servAddr, NULL, aggrId, &aggrStatus, 0)) {
	    bzero((char *)&aggrStatus, sizeof(aggrStatus));
	}
	if (anySeen == 0) {
	    dce_printf(fts_s_main_agg_fileset_quota, "Fileset Name", "Quota", "Used", "% Us");
	    anySeen = 1;
	}
	Warn = 0;
	/* Test for size/maxQuota really being over 90.000%, not just (truncated) Pct > 90 */
	Warn1 = 0;
	if ((visQuotaUsage * 10) > (visQuotaLimit * 9)) {
		Warn = Warn1 = 1;
	}
	vnbuf[0] = '\0';
	if (status.vss.volName[0] == '/' || (code == 0 && !AG_TYPE_SUPPORTS_EFS(aggrStatus.type))) {
	    strncpy(vnbuf, (caddr_t)entry.name, AFS_NAMEMAXLEN);
	    if (volType == ROVOL) strcat(vnbuf, ".readonly");
	    else if (volType == BACKVOL) strcat(vnbuf, ".backup");
	}
	Pct = visQuotaLimit ? ((visQuotaUsage * 100)/visQuotaLimit) : 0;
	dce_printf(fts_s_main_quotas,
		       (vnbuf[0] != '\0' ? vnbuf : (caddr_t)status.vss.volName),
		       visQuotaLimit, visQuotaUsage, Pct, (Warn1 ? "<<" : "  "));
	if (code == 0) {
	    /* oops--totalUsable already is diminished by minFree */
	    xfree = (aggrStatus.totalUsable - aggrStatus.curFree);
	    Pct = (aggrStatus.totalUsable ? ((xfree * 100) / aggrStatus.totalUsable) : 0);
	    /* Test for xfree/totalUsable really being over 97.0%, not just (truncated) Pct > 97 */
	    Warn1 = 0;
	    if ((xfree * 100) > (aggrStatus.totalUsable * 97)) {
		Warn = Warn1 = 1;
	    }
	    dce_printf(fts_s_main_equals,
			   Pct, 
			   (Warn1 ? "<<" : ""), 
			   xfree, 
			   aggrStatus.totalUsable);
#if 0
	    switch (aggrStatus.type) {
		case AG_TYPE_UFS:
		    dce_svc_printf(FTS_S_MAIN_NON_LFS_MSG); break;
		case AG_TYPE_EPI:
		    dce_svc_printf(FTS_S_MAIN_LFS_MSG); break;
		case AG_TYPE_AIX3:
		    dce_printf(fts_s_main_AIX3); break;
		default:
		    dce_svc_printf(FTS_S_MAIN_NON_LFS_TYPE_MSG, aggrStatus.type);
	    }
#else /* 0 */
	    dce_printf(fts_s_main_default_type, AG_TYPE_TO_STR(aggrStatus.type));
#endif /* 0 */
	}
	if (Warn)
	  dce_printf(fts_s_main_warning);
	else
	  dce_printf(fts_s_main_no_warning);
    }
    if (rtnVal == 0 && anySeen == 0) {
	dce_svc_printf(FTS_S_MAIN_NO_ARGS_MSG, programName, aSyntax->name);
	exit(1);
    }
    if (rtnVal != 0) exit(1);
    return 0;
}

EXPORT int fts_DoLsMount(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
    register long rtnVal;
    struct afs_ioctl blob;
    int anyError;
    char orig_name[1024+1];		/*Original name, may be modified*/
    char true_name[1024+1];		/*``True'' dirname (e.g., symlink target)*/
    char parent_dir[1024+1];		/*Parent directory of true name*/
    register char *last_component;	/*Last component of true name*/
    struct stat statbuff;		/*Buffer for status info*/
    int link_chars_read;		/*Num chars read in readlink()*/
    int	thru_symlink;			/*Did we get to a mount point via a symlink?*/
    cmd_dataIter_t dirIter;
    char *dirNameP;
    
    anyError = 0;
    rtnVal = 0;

    if (!fts_PioctlWorks()) {
	dce_svc_printf(FTS_S_MAIN_NO_CACHE_MANAGER_MSG, programName, aSyntax->name);
	return 1;
    }

    /* once per file on cmd line */
    for (dirNameP = cmdIter_FirstData(&dirIter, LSMOUNT_DIR_OFFSET, aSyntax);
	  dirNameP != (char *)NULL;
	  dirNameP = cmdIter_NextData(&dirIter)) {

	dmprintf(fts_debug_flag, FTS_DEBUG_PARSE_MASK,
		 ("dir name = %s", dirNameP));
	thru_symlink = 0;
	sprintf(orig_name, "%s%s",
		(dirNameP[0] == '/') ? "" : "./",
		dirNameP);

	/* Allow trailing slash in input, though pioctl hates it */
	while (orig_name[0] != '\0' && orig_name[strlen(orig_name)-1] == '/')
	    orig_name[strlen(orig_name)-1] = '\0';

	if (lstat(orig_name, &statbuff) < 0) {
	    /* if lstat fails, we should still try the pioctl, since it
		may work (for example, lstat will fail, but pioctl will
		    work if the volume of offline (returning ENODEV). */
	    statbuff.st_mode = S_IFDIR; /* lie like pros */
	}

	/*
	 * The lstat succeeded.  If the given file is a symlink, substitute
	 * the file name with the link name.
	 */
	if ((statbuff.st_mode & S_IFMT) == S_IFLNK) {
	    thru_symlink = 1;
	    /*
	     * Read name of resolved file.
	     */
	    link_chars_read = readlink(orig_name, true_name, sizeof(true_name)-1);
	    if (link_chars_read <= 0) {
		dce_svc_printf(FTS_S_MAIN_NOT_LINK_MSG, programName, aSyntax->name, orig_name);
		exit(1);
	    }

	    /*
	     * Add a trailing null to what was read, bump the length.
	     */
	    true_name[link_chars_read++] = 0;

	    /*
	     * If the symlink is an absolute pathname, we're fine.  Otherwise, we
	      * have to create a full pathname using the original name and the
	      * relative symlink name.  Find the rightmost slash in the original
	      * name (we know there is one) and splice in the symlink value.
	      */
	    if (true_name[0] != '/') {
		last_component = rindex(orig_name, '/');
		strcpy(++last_component, true_name);
		strcpy(true_name, orig_name);
	    }
	}
	else
	    strcpy(true_name, orig_name);

	/* Allow trailing slash in input, though pioctl hates it */
	while (true_name[0] != '\0' && true_name[strlen(true_name)-1] == '/')
	    true_name[strlen(true_name)-1] = '\0';

	/*
	 * Find rightmost slash, if any.
	     */
	last_component = rindex(true_name, '/');
	if (last_component) {
	    /*
	     * Found it.  Designate everything before it as the parent directory,
	     * everything after it as the final component.
	     */
	    strncpy(parent_dir, true_name, last_component - true_name);
	    parent_dir[last_component - true_name] = 0;
	    last_component++;   /*Skip the slash*/
	}
	else {
	    /*
	     * No slash appears in the given file name.  Set parent_dir to the current
	     * directory, and the last component as the given name.
	     */
	    strcpy(parent_dir, ".");
	    last_component = true_name;
	}

	if (strcmp(last_component, ".") == 0 || strcmp(last_component, "..") == 0) {
	    dce_svc_printf(FTS_S_MAIN_MAY_NOT_USE_MSG, programName, aSyntax->name);
	    dce_svc_printf(FTS_S_MAIN_LAS_COMPONENT_MSG, programName, aSyntax->name);
	    anyError = 1;
	    continue;
	}

	blob.in = last_component;
	blob.in_size = strlen(last_component)+1;
	blob.out_size = MAXSIZE;
	blob.out = blob_space;
	bzero(blob_space, sizeof(blob_space));

	rtnVal = pioctl(parent_dir, VIOC_AFS_STAT_MT_PT, &blob, 1);

	if (rtnVal == 0) {
	    dce_svc_printf(FTS_S_MAIN_MOUNT_POINT_MSG, dirNameP,
		   (thru_symlink ? "symbolic link, leading to a " : ""),
		   blob_space);
	}

	else {
	    anyError = 1;
	    if (errno == EINVAL)
		dce_svc_printf(FTS_S_MAIN_NOT_MOUNT_POINT_MSG, dirNameP);
	    else {
		dce_svc_printf(FTS_S_MAIN_ERROR_FOR_FILE_MSG,
			       programName, aSyntax->name,
			       dirNameP, strerror(errno));
	    }
	}
    }
    return anyError;
}

EXPORT int fts_DoMkMount(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
    register long rtnVal = 0;
    struct vldbentry vldbEntry;
    struct afs_ioctl blob;
    char *tp;
    struct cm_CreateMountPoint *mountp;
    extern char remoteCellName[];
    char dirNameBuff[1024+1];
    char *dirP, *finalP;
    char linkBuff[1024+1];
    char cellBuf[500];
    char *dirNameP = cmd_FirstData(aSyntax, CRMOUNT_DIR_OFFSET);
    char *filesetP;
    int isCellPresent = cmd_IsParamPresent(aSyntax, CRMOUNT_CELL_OFFSET);
    char *cellP = ((cmd_IsParamPresent(aSyntax, CRMOUNT_CELL_OFFSET)) ?
		    cmd_FirstData(aSyntax, CRMOUNT_CELL_OFFSET) :
		    (char *)NULL);
    int isRwPresent = cmd_IsParamPresent(aSyntax, CRMOUNT_RW_OFFSET);
    int isFastPresent = cmd_IsParamPresent(aSyntax, CRMOUNT_FAST_OFFSET);
    int isGlobalPresent = cmd_IsParamPresent(aSyntax, CRMOUNT_GLOBAL_OFFSET);

    rtnVal = 0;

    filesetP = ((cmd_IsParamPresent(aSyntax, CRMOUNT_FILESET_OFFSET)) ?
		    cmd_FirstData(aSyntax, CRMOUNT_FILESET_OFFSET) :
		    (char *)NULL);

    dmprintf(fts_debug_flag, FTS_DEBUG_PARSE_MASK,
	      ("dirName = %s, fileset = %s, cell = %s, rw? %s, fast? %s, global? %s",
	       dirNameP,
	       filesetP ? filesetP : "(nofs)",
	       isCellPresent ? cellP : "(none)",
	       isRwPresent ? "YES" : "NO",
	       isFastPresent ? "YES" : "NO",
	       isGlobalPresent ? "YES" : "NO"));

    if (!fts_PioctlWorks()) {
	dce_svc_printf(FTS_S_MAIN_NO_CACHE_MANAGER_MSG, programName, aSyntax->name);
	return 1;
    }

    strcpy(dirNameBuff, dirNameP);
    tp = rindex(dirNameBuff, '/');
    if (tp) {
	*tp = '\0';
	dirP = dirNameBuff;
	finalP = tp+1;
    } else {
	dirP = ".";
	finalP = dirNameBuff;
    }
    cellBuf[0] = '\0';
    if ((dirP[0] == '\0') || !fts_InDFS(dirP, cellBuf)) {
	dce_svc_printf(FTS_S_MAIN_NO_MOUNT_POINTS_MSG, programName, aSyntax->name);
	return 1;
    }

    if (!isFastPresent && !isGlobalPresent) {
	/* not fast, not global; check name with VLDB */
	if (cellP == 0)
	    cellP = cellBuf;	/* get cell name from cell in which parent dir exists. */
	rtnVal = fts_openCellFldb(cellP);
	if (rtnVal) {
	    dce_svc_printf(FTS_S_MAIN_CONTINUE_MSG, programName, aSyntax->name);
	} else {
	    rtnVal = vldb_GetEntryByName(filesetP, &vldbEntry, 1);
	    /* Don't complain if there are problems talking to the remote servers--
	      just if there's some authoritative indication that the requested volume
	     doesn't exist. */
            switch(rtnVal){
	        case 0:
                     break;
	        case VL_NOENT:
		    dce_svc_printf(FTS_S_MAIN_NOT_IN_CELL_MSG, programName, aSyntax->name, filesetP, cellP);
                    rtnVal=0;
                    break;
	        case VL_BADNAME:
                    dce_svc_printf(FTS_S_MAIN_ILLEGAL_NAME_MSG, filesetP);
                    break;
                default:
		    dce_svc_printf(FTS_S_MAIN_NO_CHECK_FILESET_MSG, programName, aSyntax->name, filesetP, cellP,
			  dfs_dceErrTxt(rtnVal));
                    rtnVal=0;
	       }
	    if (rtnVal)
		return(rtnVal);
	}
    }

    linkBuff[0] = (isGlobalPresent ? '!' : (isRwPresent ? '%' : '#'));
    linkBuff[1] = '\0';
    if (isCellPresent) {
	/* cell was specified; put cell prefix in buffer */
	strcat(linkBuff, cellP ? cellP : currentCellName);
	strcat(linkBuff, ":");
    }
    if (!isGlobalPresent)
	strcat(linkBuff, filesetP);	/* append volume name (if there is one) */
    strcat(linkBuff, ".");		/* stupid convention; these end with a period. */
    /* See if the arguments will fit inside the ioctl blob. */
    bzero(blob_space, sizeof(blob_space));
    mountp = (struct cm_CreateMountPoint *) blob_space;
    mountp->nameOffset = sizeof(struct cm_CreateMountPoint);
    mountp->nameLen = strlen(finalP);
    mountp->nameTag = 0;
    mountp->pathOffset = mountp->nameOffset + mountp->nameLen + 1;
    mountp->pathLen = strlen(linkBuff);
    mountp->pathTag = 0;
    if ((mountp->pathOffset + mountp->pathLen) > MAXSIZE) {
	dce_svc_printf(FTS_S_MAIN_OVERSIZE_MOUNT_POINT_MSG,
		       programName, aSyntax->name, dirNameP);
    }
    strncpy(&blob_space[mountp->nameOffset], finalP, mountp->nameLen);
    strncpy(&blob_space[mountp->pathOffset], linkBuff, mountp->pathLen);
    blob.out_size = 0;
    blob.out = 0;
    blob.in = blob_space;
    blob.in_size = mountp->pathOffset + mountp->pathLen + 1;
    rtnVal = pioctl(dirP, VIOC_AFS_CREATE_MT_PT, &blob, 1);
    if (rtnVal) {
	dce_svc_printf(FTS_S_MAIN_MAKING_MOUNT_POINT_MSG,
		       programName, aSyntax->name, dirNameP, strerror(errno));
	return 1;
    }
    return rtnVal;
}

EXPORT int fts_DoDelMount(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
    register long rtnVal = 0;
    cmd_dataIter_t dirIter;
    char *dirNameP = (char *)NULL;
    struct afs_ioctl blob;
    char tbuffer[1024+1];
    register char *tp;
    int dirLen;
    int code = 0;

    rtnVal = 0;
 
    if (!fts_PioctlWorks()) {
	dce_svc_printf(FTS_S_MAIN_NO_CACHE_MANAGER_MSG, programName, aSyntax->name);
	return 1;
    }

    /* per-file processing */
    for (dirNameP = cmdIter_FirstData(&dirIter, RMMOUNT_DIR_OFFSET, aSyntax);
	  dirNameP != (char *)NULL;
	  dirNameP = cmdIter_NextData(&dirIter)) {
	dmprintf(fts_debug_flag, FTS_DEBUG_PARSE_MASK,
		 ("dir name = %s", dirNameP));

	/* Allow trailing slash in input, though pioctl hates it */
	while (dirNameP[0] != '\0' && dirNameP[strlen(dirNameP)-1] == '/')
	    dirNameP[strlen(dirNameP)-1] = '\0';

	blob.out_size = 0;
	blob.out = 0;
	tp = rindex(dirNameP, '/');
	if (tp) {
	    dirLen = tp - dirNameP;
	    strncpy(tbuffer, dirNameP, dirLen);  /* the dir name */
	    tbuffer[dirLen] = '\0';
	    tp++;   /* skip the slash */
	}
	else {
	    strcpy(tbuffer, ".");
	    tp = dirNameP;
	}
	blob.in = tp;
	blob.in_size = strlen(tp)+1;
	errno = 0;
	if (tbuffer[0] == '\0')
	    errno = EINVAL;
	else 
	   rtnVal = pioctl(tbuffer, VIOC_AFS_DELETE_MT_PT, &blob, 1);
	if (rtnVal || errno ) {
	    code = 1;
	    dce_svc_printf(FTS_S_MAIN_REMOVING_MOUNT_POINT_MSG,
			   programName, aSyntax->name,
			   dirNameP, strerror(errno));
	}
    }
    return code;
}

EXPORT int fts_DoSetQuota(aSyntax, aRock)
struct cmd_syndesc *	aSyntax;
char *			aRock;
{
    int	rtnVal = 0;
    char *cellNameP = ((cmd_IsParamPresent(aSyntax, FTS_CELL_OFFSET)) ?
			     cmd_FirstData(aSyntax, FTS_CELL_OFFSET) : (char *)NULL);
    int isCellPresent = cmd_IsParamPresent(aSyntax, FTS_CELL_OFFSET);
    int isNoauthPresent = cmd_IsParamPresent(aSyntax, FTS_NOAUTH_OFFSET);
    int isLocalAuthPresent = cmd_IsParamPresent(aSyntax, FTS_LOCALAUTH_OFFSET);
    int isFilesetPresent = cmd_IsParamPresent(aSyntax, SETQUOTA_FILESET_OFFSET);
    int isPathPresent = cmd_IsParamPresent(aSyntax, SETQUOTA_PATH_OFFSET);
    char *sizeP = cmd_FirstData(aSyntax, SETQUOTA_SIZE_OFFSET);
    int nameOffset;
    cmd_dataIter_t nameIter;
    char *	nameP = (char *)NULL;
    struct vldbentry entry;
    ftserver_status status;
    afs_hyper_t volId;
    long code, volType, aggrId;
    struct sockaddr servAddr;
    unsigned newQuota;
    struct afsFid fid;
    afs_hyper_t dum;
    int anySeen;

    rtnVal = 0;

    while (*sizeP && isspace(*sizeP)) ++sizeP;
    if (fts_IsNumeric(sizeP, 0, 0, &dum) == 0) {
	dce_svc_printf(FTS_S_MAIN_NOT_NUMERIC_TO_SIZE_MSG, programName, aSyntax->name, sizeP);
	exit(1);
    }
    newQuota = AFS_hgetlo(dum);

    /* the parser will make sure that one of the two entries is present */
    nameOffset = isFilesetPresent ? SETQUOTA_FILESET_OFFSET : SETQUOTA_PATH_OFFSET;
    anySeen = 0;
    for (nameP = cmdIter_FirstData(&nameIter, nameOffset, aSyntax);
	  nameP != (char *)NULL;
	  nameP = cmdIter_NextData(&nameIter)) {
	dmprintf(fts_debug_flag, FTS_DEBUG_PARSE_MASK,
		 ("%s name = %s", isFilesetPresent ? "fileset" : "path", nameP));
	if (isPathPresent) {
	    code = fts_PathToFid(aSyntax->name, nameP, &fid);
	    if (code) {rtnVal = code; continue;}
	    volId = fid.Volume;
	} else {
	    if (!GetVolumeID(nameP, &volId)) /* volume */ {
		rtnVal = -1;
		continue;
	    }
	}
	if (code = vldb_GetVldbInfo(&volId, &servAddr, &aggrId, &volType, &entry)) {
	    rtnVal = code;
	    continue;
	}
	if (volType != VOLTIX_TO_VOLTYPE(RWVOL)) {
	    rtnVal = EINVAL;
	    VOLSERLOG(0, "Setting fileset quota is permitted only on read-write filesets\n");
	    continue;
	}
	code = fts_NeedAggrType(&servAddr, aggrId, AG_TYPE_ANY_EFS_MASK,
				"Setting fileset quota is");
	if (code) {
	    rtnVal = code;
	    continue;
	}
	if (code = volc_Call(VC_SetQuota, &servAddr, NULL, aggrId, &volId, newQuota)) {
	    if (code == ENODEV)
		dce_svc_printf(FTS_S_MAIN_NOT_AT_SITE_FLDB_MSG, programName, aSyntax->name);
	    PrintError(aSyntax->name, code);
	    rtnVal = code;
	    continue;
	}
	anySeen = 1;
    }
    if (rtnVal == 0 && anySeen == 0) {
	dce_svc_printf(FTS_S_MAIN_NO_COMMAND_ARGS_MSG, programName, aSyntax->name);
	exit(1);
    }
    if (rtnVal != 0) exit(1);
    return 0;
}

/*
 *  "setprotectlevels -id <vid> [-minlocalprotectlevel <ident>]
 *     [-maxlocalprotectlevel <ident>] [-minremoteprotectlevel <ident>]
 *     [-maxremoteprotectlevel <ident>]"
 */
EXPORT int fts_DoSetProtectLevels(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{
    struct vldbentry entry;
    struct sockaddr servAddr;
    afs_hyper_t volId;
    long code, aggrId;
    int sawAny;
    u_long volType;
    unsigned32 minLclLevel, maxLclLevel;
    unsigned32 minRmtLevel, maxRmtLevel;

    if (!GetVolumeID(as->parms[SETPROT_FILESET_OFFSET].items->data, &volId)) /* volume */
	exit(1);
    if (code = vldb_SetLock(&volId, -1, VLOP_RELEASE))
	exit(1);
    if (code = vldb_GetVldbInfo(&volId, &servAddr, &aggrId, &volType, &entry)) {
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }

    minLclLevel = entry.charSpares[VLAUTHN_MINLCLLEVEL];
    maxLclLevel = entry.charSpares[VLAUTHN_MAXLCLLEVEL];
    minRmtLevel = entry.charSpares[VLAUTHN_MINRMTLEVEL];
    maxRmtLevel = entry.charSpares[VLAUTHN_MAXRMTLEVEL];

    sawAny = 0;
    if (as->parms[SETPROT_MINLCLAUTHN_OFFSET].items) {
	sawAny = 1;
	if (compat_nameToAuthnLevel(
				 as->parms[SETPROT_MINLCLAUTHN_OFFSET].items->data,
				 &minLclLevel)) {
	    dce_svc_printf(FTS_S_BADMINLCLAUTHN_MSG,
			   as->parms[SETPROT_MINLCLAUTHN_OFFSET].items->data);
	    (void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	    exit(1);
	}
    }
    if (as->parms[SETPROT_MAXLCLAUTHN_OFFSET].items) {
	sawAny = 1;
	if (compat_nameToAuthnLevel(
				 as->parms[SETPROT_MAXLCLAUTHN_OFFSET].items->data,
				 &maxLclLevel)) {
	    dce_svc_printf(FTS_S_BADMAXLCLAUTHN_MSG,
			   as->parms[SETPROT_MAXLCLAUTHN_OFFSET].items->data);
	    (void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	    exit(1);
	}
    }
    if (as->parms[SETPROT_MINRMTAUTHN_OFFSET].items) {
	sawAny = 1;
	if (compat_nameToAuthnLevel(
				as->parms[SETPROT_MINRMTAUTHN_OFFSET].items->data,
				&minRmtLevel)) {
	    dce_svc_printf(FTS_S_BADMINRMTAUTHN_MSG,
			   as->parms[SETPROT_MINRMTAUTHN_OFFSET].items->data);
	    (void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	    exit(1);
	}
    }
    if (as->parms[SETPROT_MAXRMTAUTHN_OFFSET].items) {
	sawAny = 1;
	if (compat_nameToAuthnLevel(
				as->parms[SETPROT_MAXRMTAUTHN_OFFSET].items->data,				&maxRmtLevel)) {
	    dce_svc_printf(FTS_S_BADMAXRMTAUTHN_MSG,
			   as->parms[SETPROT_MAXRMTAUTHN_OFFSET].items->data);
	    (void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	    exit(1);
	}
    }

    if (minLclLevel != rpc_c_protect_level_default
	&& maxLclLevel != rpc_c_protect_level_default
	&& minLclLevel > maxLclLevel) {
	dce_svc_printf(FTS_S_LCLPROTOVERLAP_MSG,
		       minLclLevel, maxLclLevel);
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }

    if (minRmtLevel != rpc_c_protect_level_default
	&& maxRmtLevel != rpc_c_protect_level_default
	&& minRmtLevel > maxRmtLevel) {
	dce_svc_printf(FTS_S_RMTPROTOVERLAP_MSG,
		       minRmtLevel, maxRmtLevel);
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }

    if (minLclLevel > rpc_c_protect_level_pkt_privacy
	|| maxLclLevel > rpc_c_protect_level_pkt_privacy) {
	dce_svc_printf(FTS_S_BADLCLPACKING_MSG,
		       minLclLevel, maxLclLevel);
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }

    if (minRmtLevel > rpc_c_protect_level_pkt_privacy
	|| maxRmtLevel > rpc_c_protect_level_pkt_privacy) {
	dce_svc_printf(FTS_S_BADRMTPACKING_MSG,
		       minRmtLevel, maxRmtLevel);
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }

    if (!sawAny) {
	dce_svc_printf(FTS_S_SETPROT_EMPTY_MSG, entry.name);
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }

    entry.charSpares[VLAUTHN_MINLCLLEVEL] = minLclLevel;
    entry.charSpares[VLAUTHN_MAXLCLLEVEL] = maxLclLevel;
    entry.charSpares[VLAUTHN_MINRMTLEVEL] = minRmtLevel;
    entry.charSpares[VLAUTHN_MAXRMTLEVEL] = maxRmtLevel;

    /* All seems fine.  Set the new parameters in the FLDB entry. */
    vldb_SaveOldEntry(&entry);
    if (code = vldb_ReplaceEntry(&entry, &volId, volType, LOCKREL_ALL)) {
	(void) vldb_ReleaseLock(&volId, volType, LOCKREL_ALL);
	exit(1);
    }
    (void) vldb_TellRepAboutNewEntry(&entry, 0, 1);
    return(0);
}

#if 0
/*
 *  "etrans <server> <transId>"
 */
EXPORT int fts_DoEndTrans(as, aRock)
     register struct cmd_syndesc *	as;
     char *				aRock;
{ 
    long transId, code;
    struct sockaddr servAddr;

    if (!GetServer(as->parms[ETRANS_SERVER_OFFSET].items->data, &servAddr)) /* server */
	exit(1);
    transId = atol(as->parms[ETRANS_TRANID_OFFSET].items->data);	/* transId */
    code = volc_Call(VC_EndTrans, &servAddr, NULL, transId);
    return code;
}
#endif /* 0 */

/*
 * Handle some default conf arguments
 */
PRIVATE MyBeforeProc(as, rockp)
    struct cmd_syndesc *as;
    char *rockp; 
{
    extern char 	localCellName[];
    register char 	*tcell = (char *)localCellName;
    register long 	code;

    useNoAuth = 0;
    useLocalAuth = 0;
    if (as->parms[FTS_LOCALAUTH_OFFSET].items)	/* -localauth specified */
	useLocalAuth = 1;
    if (as->parms[FTS_NOAUTH_OFFSET].items)	/* -noauth specified */
	useNoAuth = 1;

    /*
     * It shouldn't hurt anything (& it will help) to init security here, if 
     * we're in a situation in which we may want & need a network identity, but
     * not have one yet.
     */
    code = dfsauth_client_InitAuthentication((rpc_binding_handle_t *)NULL,
					     (char *)NULL,
					     useNoAuth, useLocalAuth);
    if (code != 0) {
      dce_svc_printf(FTS_S_MAIN_LOCALAUTH_FAILED_MSG, programName, dfs_dceErrTxt(code));
      exit(1);
    }
    
    /* Something of a cross-level story here...don't init FLDB for 
       trivial commands. */
    if (strncmp(as->name, "help", strlen(as->name)) != 0 /* fts help */
	 && strncmp(as->name, "-help", strlen(as->name)) != 0 /* fts -help */
	 && strncmp(as->name, "apropos", strlen(as->name)) != 0/* fts apropos */
	 && as->parms[CMD_HELPPARM].items == 0 /* fts cmd -help */) {

	(void) getLocalCellInfo();
	if (as->parms[FTS_CELL_OFFSET].items) {	/* if -cell specified */
	    tcell = as->parms[FTS_CELL_OFFSET].items->data;
	    if (tcell == NULL || *tcell == '\0') {
		dce_svc_printf(FTS_S_MAIN_MISSING_CELL_ARG_MSG, programName);
		exit(1);
	    }
	}
	vldb_SetRemoteCellName(tcell);

	/* this figures out which servers have the vldb for the cell */
	code = fts_openCellFldb(tcell);
	if (code)
	    exit(1);

	/*
	 *  Set a boolean flag to indicate we need to do ubik cleanup.
	 *  This just saves all the re-checking of paramters at cleanup time.
	 */
	doUbikCleanup = 1;

    }
    verbose = (as->parms[FTS_VERBOSE_OFFSET].items ? 1 : 0);
    (void) vldb_SaveOldEntry(0);

    /*
     * as a workaround to the DG private sockets/pipe argument problem
     * (transarc DB 6731), we disallow private DG sockets.  Do it here,
     * after we know the DCE RPC is initialized, as the last thing we do
     * before starting real work.
     */
#ifndef AFS_AIX_ENV
    rpc__dg_disa_priv_sockets();
#endif /* AFS_AIX_ENV */

    return 0;
}

/*
 *  MyAfterProc() --
 *    Cleanup function.
 */
PRIVATE int MyAfterProc(cmdDataP, myDataP)
  IN struct cmd_syndesc *cmdDataP;
  IN char *myDataP;
{
    long returnValue;

    /*
     *  Cleanup ubik if we have to.
     */
    if (doUbikCleanup)
    {
	if ((returnValue = ubik_ClientCleanup()) != 0)
	    PrintError("MyAfterProc (ubik)", returnValue);
    }

    /*
     *  Since we always call the dfsauth_client_InitAuthentication()
     *  function in MyBeforeProc, we just always call the cleanup function
     *  here.  It really only helps when the -localauth switch is used,
     *  but it doesn't hurt to call it anyway.  Yes, this function is
     *  also called by ubik_ClientCleanup(), but we're not supposed to
     *  know that.  We're just trying to be a good application.
     *
     *  Note that we don't consider the "client not initialized" error
     *  a true error.
     */
    if (((returnValue = dfsauth_client_CleanupAuthentication()) != 0) &&
	(returnValue != DAUT_ERROR_CLIENT_NOT_INITIALIZED))
    {
	PrintError("MyAfterProc (dfsauth)", returnValue);
    }

    return(0);
}


/*
 * Main volume client routine.
 */
main(argc, argv)
    int argc;
    char **argv; 
{
    register long code;

    osi_setlocale(LC_ALL, "");
    (void)strncpy(programName, argv[0], BUFSIZ);

    initialize_svc();

    (void) cmd_SetBeforeProc(MyBeforeProc, (char *) 0);
    (void) cmd_SetAfterProc(MyAfterProc, (char *) 0);

    fts_ui_SetUpSyntax();

    code = cmd_Dispatch(argc, argv);

    exit (code);
}
