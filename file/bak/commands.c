/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1996, 1994 Transarc Corporation
 * All Rights Reserved
 */
/*
 * commands.c -- UI command functions
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <dcedfs/cmd.h>
#include <com_err.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/fldb_data.h>
#include <dcedfs/flclient.h>
#include <dcedfs/flserver.h>
#include <dcedfs/volc.h>
#include <dcedfs/volume.h>
#include <dcedfs/ftserver_data.h>
#include <dcedfs/ktime.h>
#ifdef AFS_AIX31_ENV
#include <time.h>
#endif /* AFS_AIX31_ENV */
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/butc.h>
#include <dcedfs/tcdata.h>
#include <dcedfs/budb_errs.h>
#include <dcedfs/common_def.h>
#include <bc.h>
#include <error_macros.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

extern int nonInteractiveBak;
extern struct bc_config *bc_globalConfig;
extern char *ktime_GetDateUsage();
extern int bc_FindVolumeSet();
extern int bc_FindDumpSchedule();
extern struct bc_dumpTask bc_dumpTasks[BC_MAXSIMDUMPS];
extern struct ubik_client *cstruct;
extern int bc_Dumper();		/* function to do dumps */
extern int bc_Restorer();	/* function to do restores */
extern long ScanDumpHdr();
extern long ScanTapeVolume();
extern rpc_binding_handle_t bc_GetConn();
extern char *whoami;
static char mn[] = "commands";			/*Module name*/
char *tailCompPtr();

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/bak/RCS/commands.c,v 4.153 1996/07/15 17:45:29 gait Exp $")

/*
 * Utility routines
 */
bc_SafeATOI(anum)
register char *anum; {
    register long total;
    register int tc;

    total = 0;
    while(tc = *anum) {
	if (!strcmp(anum, "g") || !strcmp(anum, "G")) {
	    total *= 1024*1024;
	    break;
	}
	if (!strcmp(anum, "m") || !strcmp(anum, "M")) {
	    total *= 1024;
	    break;
	}
	if (tc < '0' || tc > '9') return -1;
	total *= 10;
	total += (tc - '0');
	anum++;
    }
    return total;
}

/* make a copy of a string so that it can be freed later */
char *bc_CopyString(astring)
register char *astring; {
    register long tlen;
    register char *tp;

    if (!astring)
    	return (char *) 0;		/* propagate null strings easily */
    tlen = strlen(astring);
    tp = (char *) osi_Alloc(tlen+1);	/* don't forget the terminating null */
    strcpy(tp, astring);
    return tp;
}

/* printIfStatus
 *	print out an interface status node as received from butc
 */

printIfStatus(statusPtr)
    tciStatusS *statusPtr;
{

    dce_printf(bak_s_taskname, statusPtr->taskName);
    if ( (statusPtr->info.opType == TCOP_DUMP ||
	  statusPtr->info.opType == TCOP_RESTORE) ) {
/*      if (statusPtr->info.u.vol.nKBytes )*/
      dce_printf(bak_s_kbytes_transfered, statusPtr->info.u.vol.nKBytes);
      if ( strlen((char *)statusPtr->info.u.vol.volumeName) != 0 )
	dce_printf(bak_s_filesetname, statusPtr->info.u.vol.volumeName);
    }
    if ( statusPtr->flags & ABORT_REQUEST )
    {
	dce_printf(bak_s_abort_req_rcvd);
    }
    if ( statusPtr->flags & ABORT_DONE )
    {
	dce_printf(bak_s_abort_complete);
    }
    if ( statusPtr->flags & OPR_WAIT )
    {
	dce_printf(bak_s_operator_wait);
    }

     if ( statusPtr->flags & CALL_WAIT )
     {
	  dce_printf(bak_s_callout);
     };


    if ( statusPtr->flags & DRIVE_WAIT )
    {
	dce_printf(bak_s_drive_wait);
    }
    if ( statusPtr->flags & TASK_DONE )
    {
	dce_printf(bak_s_done_brack);
    }
    dce_printf(bak_s_newline);
}

/* get a connection to the tape controller */
rpc_binding_handle_t
bc_GetConn(aconfig, aport)
     struct bc_config *aconfig;
     long aport;
{
    rpc_binding_handle_t tconn;
    unsigned long host;
    unsigned short port;
    struct bc_hostEntry *te;
    error_status_t st;

    if (aconfig == (struct bc_config *)NULL ||
	(aport < 0) || (aport > BC_MAXPORTOFFSET))
      return (rpc_binding_handle_t)NULL;

    lock_ObtainRead(&(aconfig->hostLock));
    if (!(te=aconfig->tapeHosts))  {
      lock_ReleaseRead(&(aconfig->hostLock));
      return( (rpc_binding_handle_t )NULL);
    }
    while ( te != 0 )  {
      if ( te->portOffset == aport ) {
	/* found the right port */
	rpc_binding_copy (te->binding, &tconn,(unsigned32 *) &st);
#ifdef DEBUG
	compat_print_binding("\nbc_GetConn binding: ", tconn);
	fflush(stdout); fflush(stderr);
#endif /* DEBUG */
	lock_ReleaseRead(&(aconfig->hostLock));
	if (st == rpc_s_ok)
	  return(tconn);
	else
	  return ((rpc_binding_handle_t)NULL);
      }
      te = te->next;
    }
    lock_ReleaseRead(&(aconfig->hostLock));
    return( (rpc_binding_handle_t )NULL );
}

/*
 * bc_FreeConn
 *	free the binding handle passed in
 */
bc_FreeConn(aconfig, aconn)
     struct bc_config *aconfig;
     rpc_binding_handle_t aconn;
{
  error_status_t st;
  rpc_binding_free((rpc_binding_handle_t *)&aconn, &st);
  return st;
}

/* CheckTCVersion
 *	make sure we are talking to a compatible butc process.
 */
CheckTCVersion(tconn)
  rpc_binding_handle_t tconn;
{
    struct tc_tcInfo tci;
    long code;

    code = BUTC_TCInfo(tconn, &tci);
    if ( code )
      return(code);
    if ( tci.tcVersion != CUR_BUTC_VERSION )
      return(BC_VERSIONFAIL);
    return(0);
}

/* bc_GetTapeStatusCmd
 *	display status of all tasks on a particular tape coordinator
 */
bc_GetTapeStatusCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    register long code;
    long index, dumpID;
    rpc_binding_handle_t tconn;
    char *tp;
    long portOffset;
    long st = 0;
    int ntasks;
    unsigned long flags;
    unsigned long taskId;
    tciStatusS status;

    if (as->parms[0].items) {
	portOffset = bc_SafeATOI(tp=as->parms[0].items->data);
	if (portOffset < 0 ) {
	    com_err(whoami, BC_BADTCID,
		    "Unable to decode tcid '%s'", tp);
	    return BC_BADTCID;
	}
	if(portOffset > BC_MAXPORTOFFSET){
	    com_err(whoami, BC_BADTCID,
		    "%u exceeds max port offset %u",
		    portOffset, BC_MAXPORTOFFSET);
	    return BC_BADTCID;
	}
    }
    else
      portOffset = 0;

    tconn = bc_GetConn(bc_globalConfig, portOffset);
    if ( tconn == (rpc_binding_handle_t)NULL ) {
      code = BC_NOCONN;
      com_err(whoami, code,
	      "; Unable to connect to tape coordinator at port %d",
	      portOffset);
      return code;
    }

    code = CheckTCVersion(tconn);
    if ( code )
    {
        com_err(whoami, code,
		"; Unable to contact tape coordinator at tcid %d",
		portOffset);
	bc_FreeConn(bc_globalConfig, tconn);
	return code;
    }

    flags = TSK_STAT_FIRST;
    taskId = 0;
    ntasks = 0;

    while ( (flags & TSK_STAT_END) == 0 )
    {
	code = BUTC_ScanStatus(tconn, &taskId, &status, &flags);
	if ( IS_COMM_ERROR(code) )
	{
	    if ( code == TC_NOTASKS || code == TC_BADTASK )
		break;

	    com_err(whoami, code,
		    "; Unable to obtain status information at tcid %d",
		    portOffset);
	    bc_FreeConn(bc_globalConfig, tconn);
	    return code;
	}
	flags &= ~TSK_STAT_FIRST; /* turn off flag */
	dce_printf(bak_s_task_id, status.taskId);
	printIfStatus(&status);
	ntasks++;
    }

    if ( ntasks == 0 )
	dce_svc_printf(BAK_S_TAPE_COORD_IDLE_MSG);
    bc_FreeConn(bc_globalConfig, tconn);
    return(0);
}

/* bc_WaitForJobs
 *	wait for all jobs to terminate
 */
bc_WaitForJob()
{
    register struct bc_dumpTask *td;
    register int i;
    int usefulJobRunning;
    struct timespec delayTime;
    long code = 0;
    extern dlqlinkT statusHead;
    statusP statusPtr=(statusP)NULL;

    while (1) {
	usefulJobRunning=0;
	/*
	 * Check both the dumpTask and statusQueue to catch all cases
	 */
	for( td = bc_dumpTasks, i=0; i<BC_MAXSIMDUMPS; i++, td++) {
	    if (td->flags & BC_DI_INUSE)
		usefulJobRunning++;
	    else if (td->flags & BC_DI_ERROR) { /* failed before RPC start */
		td->flags &= ~BC_DI_ERROR;
		return 1;
	    }
	}

	lock_ObtainShared(&statusQueueLock);
	if (statusPtr) {
	    if ((statusPtr->flags & (TASK_ERROR|ABORT_DONE)) ||
		statusPtr->defunct) { 		    /* task failed */
		lock_UpgradeSToW(&statusQueueLock);
		deleteStatusNode(statusPtr);
		lock_ReleaseWrite(&statusQueueLock);
		return 1;
	    }
	    else if (statusPtr->flags & TASK_DONE) {/* task succeeded */
		lock_UpgradeSToW(&statusQueueLock);
		deleteStatusNode(statusPtr);
		lock_ReleaseWrite(&statusQueueLock);
		return 0;
	    }
	    else
		/* else still running */
		usefulJobRunning++;
	}
	else {
	    if(dlqCount(&statusHead) > 0) {
		statusPtr = (statusP)dlqFront(&statusHead);
		usefulJobRunning++;
	    }
	}
	lock_ReleaseShared(&statusQueueLock);

	if (!usefulJobRunning)
	    break;
	/* otherwise we found at least one non-aborted running job,
	 * so we wait a few seconds and check again.
	 */
	delayTime.tv_sec = 5;
	delayTime.tv_nsec = 0;
	code = pthread_delay_np (&delayTime);
	if (code < 0)
	    dce_svc_printf(BAK_S_PTHREAD_FAILED_MSG, whoami);
    }
    return(0);
}

/* bc_JobsCmd
 *	print status on running jobs
 * parameters
 *	ignored
 */
bc_JobsCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    register int i;
    register struct bc_dumpTask *td;
    dlqlinkP ptr;
    statusP statusPtr;

    extern dlqlinkT statusHead;

    lock_ObtainRead(&statusQueueLock);
    ptr = (&statusHead)->dlq_next;
    while ( ptr != &statusHead )
    {
	statusPtr = (statusP) ptr;
	dce_printf(bak_s_job_task_pair, statusPtr->jobNumber, statusPtr->taskName);
	if (statusPtr->opType == TCOP_DUMP||statusPtr->opType == TCOP_RESTORE){
	  if ( statusPtr->nKBytes )
	    dce_printf(bak_s_kbytes, statusPtr->nKBytes);
	  if ( strlen(statusPtr->volumeName) != 0 )
	    dce_printf(bak_s_fileset_name, statusPtr->volumeName);
	}
	else if (statusPtr->opType == TCOP_READLABEL ||
		 statusPtr->opType == TCOP_LABELTAPE)
	  dce_printf(bak_s_tape_name_and_size, strcmp(statusPtr->tapeName, "")?statusPtr->tapeName:"<NULL>",
		 statusPtr->tapeSize);
	if ( statusPtr->flags & CONTACT_LOST )
	    dce_printf(bak_s_butc_contact_lost);

	if ( statusPtr->flags & ABORT_REQUEST )
	    dce_printf(bak_s_abort_request);

	if ( statusPtr->flags & ABORT_SENT )
	    dce_printf(bak_s_abort_sned);

	if ( statusPtr->flags & OPR_WAIT )
	    dce_printf(bak_s_operator_wait);

	if ( statusPtr->flags & CALL_WAIT )
	    dce_printf(bak_s_callout);

	if ( statusPtr->flags & DRIVE_WAIT )
	    dce_printf(bak_s_drive_wait);
	dce_printf(bak_s_newline);
	ptr = ptr->dlq_next;
    }
    lock_ReleaseRead(&statusQueueLock);

    return 0;
}

bc_KillCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    register long i;
    long slot;
    register struct bc_dumpTask *td;
    register char *tp;
    char tbuffer[256];
    long code;
    dlqlinkP ptr;
    statusP statusPtr;
    extern dlqlinkT statusHead;
    extern statusP findStatus();

    tp = as->parms[0].items->data;
    if (strchr(tp, '.') == (char *)0)
    {
	slot = bc_SafeATOI(tp);
	if (slot == -1)
	{
	    code = BC_BADJOB;
	    com_err(whoami, code,
		    "; Unable to parse job number '%s'", tp);
	    return code;
	}

	lock_ObtainWrite(&statusQueueLock);
	ptr = (&statusHead)->dlq_next;
	while ( ptr != &statusHead )
	{
	    statusPtr = (statusP) ptr;
	    if ( statusPtr->jobNumber == slot )
	    {
		statusPtr->flags |= ABORT_REQUEST;
		lock_ReleaseWrite(&statusQueueLock);
		return(0);
	    }
	    ptr = ptr->dlq_next;
	}
	lock_ReleaseWrite(&statusQueueLock);

	dce_svc_printf(BAK_S_JOB_NOT_FOUND_MSG, slot);
	return BC_NOJOB;
    }
    else
    {
	/* vol.dump */
	td = bc_dumpTasks;
	for(i=0;i<BC_MAXSIMDUMPS;i++, td++) {
	    if (td->flags & BC_DI_INUSE) {
		/* compute name */
		strcpy(tbuffer, td->volSetName);
		strcat(tbuffer, ".");
		strcat(tbuffer, (char *) tailCompPtr(td->dumpName));
		if (strcmp(tbuffer, tp) == 0)
		{
		    break;
		}
	    }
	}
	if (i >= BC_MAXSIMDUMPS) {
	    code = BC_NOJOB;
	    com_err(whoami, code,
		    "; Unable to find job '%s'", tp);
	    return code;
	}

	lock_ObtainShared(&statusQueueLock);
	statusPtr = findStatus(td->dumpID);
	if ( statusPtr == 0 ) {
	  code = BC_INTERNALERROR;
	  com_err(whoami, code,
		  "; Unable to locate status information for job '%s'", tp);
	  lock_ReleaseShared(&statusQueueLock);
	  return code;
	}
	lock_UpgradeSToW(&statusQueueLock);
	statusPtr->flags |= ABORT_REQUEST;
	lock_ReleaseWrite(&statusQueueLock);
    }
    return 0;
}

/* restore a volume or volumes */
bc_VolRestoreCmd(as, arock)
struct cmd_syndesc *as;
char *arock;
{
    /*
     * parm 0 is the new server to restore to
     * parm 1 is the new partition to restore to
     * parm 2 is fileset(s) to restore
     * parm 3 is the new extension, if any, for the fileset name.
     * parm 4 gives the new volume # to restore this volume as.
     * parm 5 date is a string representing the date
     *
     * We handle four types of restores.  If old is set, then we restore the
     * volume with the same name and ID.  If old is not set, we allocate
     * a new volume ID for the restored volume.  If a new extension is
     * specified, we add that extension to the volume name of the restored
     * volume.
     */
    register char *partName;
    register char *servName;
    register struct cmd_item *ti;
    register long code;
    struct bc_volumeEntry tvolumeEntry;		/*entry within the volume set*/
    struct bc_volumeDump *volsToRestore;	/*volumes to restore */
    struct bc_volumeDump *lastVol;
    struct bc_volumeDump *tvol;			/*temp for same */
    struct sockaddr_in destServ;		/*machine to which to restore*/
    long destPartition;				/*aggr to which to restore*/
    long fromDate;
    int oldFlag;
    int i;
    char *newExt;
    long aport = 0;
    pthread_t helper;
    pthread_attr_t threadAttr;

    if (as->parms[0].items) {
	servName = as->parms[0].items->data;
	/* specified other destination host */
	bzero((char *)&destServ, sizeof(destServ));
	if (!bc_hostNameToAddr(servName, &destServ)) {
	  code = BC_NOHOST;
	  com_err(whoami, code,
		  "; Unable to locate destination host '%s'",
		  servName);
	  return code;
	}
    }

    if (as->parms[1].items) {
	/* specified other destination partition */
	partName = as->parms[1].items->data;
	code = bc_GetPartitionID(partName, servName, &destPartition);
	if (code) {
	    com_err(whoami, code,
		    "; Unable to parse destination aggregate '%s'",
		    partName);
	    return code;;
	}
    }
    else {	/* use original partition */
	com_err(whoami, 0,
		"Restoreft must specify aggregate to restore filesets");
	return BC_NOPARTITION;
    }
    volsToRestore = lastVol = (struct bc_volumeDump *) 0;
    for(ti=as->parms[2].items; ti; ti=ti->next) {
	/* build list of volume items */
	tvol =(struct bc_volumeDump *)osi_Alloc(sizeof (struct bc_volumeDump));
	if (tvol == (struct bc_volumeDump *)NULL) {
	  com_err(whoami, BC_NOMEM, "; Unable to begin restoreft");
	  return BC_NOMEM;
	}
	bzero((char *)tvol, sizeof(struct bc_volumeDump));
	if (lastVol){
	    lastVol->next = tvol;
	    lastVol = tvol;
	}
	else {
	    lastVol = volsToRestore = tvol;
	}
	tvol->name = (char *)osi_Alloc(BU_MAXNAMELEN+1);
	strncpy(tvol->name, ti->data, BU_MAXNAMELEN);
	tvol->partition = destPartition;
	bcopy((char *)&destServ, (char *)&(tvol->server),
	      sizeof(struct sockaddr_in));
	tvol->entry = &tvolumeEntry;
    }

    if (as->parms[4].items) {
	code = ktime_DateToLong(as->parms[4].items->data, &fromDate);
	if (code) {
	    com_err(whoami, code,
		    "; Unable to parse restore date '%s'\n'%s'",
		    as->parms[4].items->data, ktime_GetDateUsage());
	    return code;
	}
    }
    else
	fromDate = 0x7fffffff;	/* latest one */
    if (as->parms[3].items)  {
      newExt = as->parms[3].items->data;

      if (!strcmp(newExt, ".backup") || !strcmp(newExt, ".readonly")) {

	dce_svc_printf(BAK_S_RESERVED_EXTENSIONS_MSG);
	return (BC_BADARG);
      }
    }

    else
	newExt = (char *) 0;	/* no new extension specified */
    oldFlag = 0;
    if(as->parms[5].items) aport = bc_SafeATOI(as->parms[5].items->data);
    if (aport < 0) {
        code = BC_BADTCID;
	com_err(whoami, code,
		"; Unable to parse tcid '%s'", as->parms[5].items->data);
	return code;
    }
    if(aport > BC_MAXPORTOFFSET){
        code = BC_BADTCID;
	com_err(whoami, code,
		"; tcid %u exceeds max tcid %u",aport,
		BC_MAXPORTOFFSET);
	return code;
    }

    for(i=0;i<BC_MAXSIMDUMPS;i++)
	if (!(bc_dumpTasks[i].flags & BC_DI_INUSE)) break;
    if (i >= BC_MAXSIMDUMPS) {
        code = BC_MAXDUMPS;
	com_err(whoami, code,
		"; Unable to start fileset restore at tcid %d",
		aport);
	return code;
    }
    bzero((char *)&(bc_dumpTasks[i]), sizeof(struct bc_dumpTask));
    bc_dumpTasks[i].config = bc_globalConfig;
    bc_dumpTasks[i].volumes = volsToRestore;
    bc_dumpTasks[i].flags = BC_DI_INUSE;
    bc_dumpTasks[i].dumpName = bc_CopyString("fileset");
    bc_dumpTasks[i].volSetName = bc_CopyString("restore");
    bc_dumpTasks[i].newExt = bc_CopyString(newExt);
    bc_dumpTasks[i].dumpLevel = 0;
    bc_dumpTasks[i].parentDumpID = 0;
    bc_dumpTasks[i].oldFlag = oldFlag;
    bc_dumpTasks[i].fromDate = fromDate;
    bc_dumpTasks[i].destPartition = destPartition;
    bc_dumpTasks[i].portOffset = aport;
    bc_dumpTasks[i].expDate = 0;
    bc_dumpTasks[i].expType = 0;
    if(as->parms[6].items) {
	bc_dumpTasks[i].dontExecute = (char)1;
	dce_svc_printf(BAK_S_START_RESTORE_MSG);
    }
    else
	bc_dumpTasks[i].dontExecute = (char)0;
    bcopy((char *)&destServ, (char *)&(bc_dumpTasks[i].destServer),
	  sizeof(struct sockaddr_in));

    if (pthread_attr_create(&threadAttr) != 0) {
	bc_dumpTasks[i].flags |= BC_DI_ERROR;
	bc_dumpTasks[i].flags &= ~BC_DI_INUSE;
	return(BC_THREAD);
    }
    if (pthread_attr_setstacksize(&threadAttr, BC_DEFAULT_STACK_SIZE) != 0) {
	bc_dumpTasks[i].flags |= BC_DI_ERROR;
	bc_dumpTasks[i].flags &= ~BC_DI_INUSE;
	return(BC_THREAD);
    }
    code = pthread_create((pthread_t *) &helper,
			  threadAttr,
			  (pthread_startroutine_t)bc_Restorer,
			  (pthread_addr_t)i);
    if (code) {
      com_err(whoami,BC_THREAD,"Could not start thread to do backup");
      bc_dumpTasks[i].flags |= BC_DI_ERROR;
      bc_dumpTasks[i].flags &= ~BC_DI_INUSE;
      return BC_THREAD;
    }
    return 0;
}

/* restore a whole partition or server */

/* bc_DiskRestoreCmd
 *	restore a whole partition
 * params:
 *	first, reqd - machine (server) to restore
 *	second, reqd - partition to restore
 *	various optional
 */

bc_DiskRestoreCmd(as, arock)
struct cmd_syndesc *as;
char *arock; {
    register char *tp;
    register char *servName = (char *)0;
    register long code;
    struct bc_volumeSet tvolumeSet;	/* temp volume set for EvalVolumeSet */
    struct bc_volumeEntry tvolumeEntry;	/* entry within the volume set */
    struct bc_volumeDump *volsToRestore;/* list of volumes to restore */
    struct bc_volumeDump *tvol;
    struct sockaddr_in destServ;	/* machine to restore volumes */
    long destPartition;			/* partition to restore volumes */
    char *vsetName="TempVolumeSet";
    int oldFlag;
    long fromDate;
    char *newExt;
    long aport = 0;
    int i;
    pthread_t helper;
    pthread_attr_t threadAttr;

    /*create volume set corresponding to the volume pattern we've been given */
    bzero((char *)&tvolumeSet, sizeof(struct bc_volumeSet));
    bzero((char *)&tvolumeEntry, sizeof(struct bc_volumeEntry));
    tvolumeSet.name = vsetName;
    tvolumeSet.ventries = &tvolumeEntry;
    tvolumeEntry.serverName = as->parms[0].items->data;
    tvolumeEntry.partname = as->parms[1].items->data;
    code = bc_GetPartitionID(tvolumeEntry.partname, tvolumeEntry.serverName,
			     &tvolumeEntry.partition);
    if (code) {
      com_err(whoami, code,
	      "; Unable to parse aggregate name '%s'",
	      tvolumeEntry.partname);
      return code;
    }
    if (!bc_hostNameToAddr(tvolumeEntry.serverName,
		     (struct sockaddr_in *)&(tvolumeEntry.server))) {
        code = BC_NOHOST;
	com_err(whoami, code,
		"; Unable to parse host name specification '%s'",
		tvolumeEntry.serverName);
	return code;
    }

    if (as->parms[8].items) {
      servName = as->parms[8].items->data;
      /* specified other destination host */
      if (!bc_hostNameToAddr(servName, &destServ)) {
	code = BC_NOHOST;
	com_err(whoami, code,
		"; Unable to parse destination host name specification '%s'",
		servName);
	return code;
      }
    }
    else	/* use destination host == original host */
    {
	bcopy((char *)&(tvolumeEntry.server), (char *)&destServ,
	      sizeof(destServ));
    }
    if (as->parms[9].items) {
	/* specified other destination partition */
	tp = as->parms[9].items->data;
	if (servName)
	    code = bc_GetPartitionID(tp, servName, &destPartition);
	else
	    code=bc_GetPartitionID(tp,tvolumeEntry.serverName,&destPartition);
	if (code) {
	  com_err(whoami, code,
		  "; Unable to parse destination aggregate '%s'",
		  tp);
	    return code;
	}
    }
    else {/* use original partition */
      if (servName)
	code = bc_GetPartitionID(tvolumeEntry.partname,
				 servName, &destPartition);
      else
	code=bc_GetPartitionID(tvolumeEntry.partname,
			       tvolumeEntry.serverName,&destPartition);
      if (code) {
	com_err(whoami, code,
		"; Unable to validate destination aggregate '%s'",
		tvolumeEntry.partname);
	return code;
      }
    }
    tvolumeEntry.name = ".*";/*match all volumes (this should be a parameter)*/

    if(as->parms[2].items) aport = bc_SafeATOI(as->parms[2].items->data);
    if (aport < 0) {
        code = BC_BADTCID;
	com_err(whoami, code,
		"; Unable to parse tcid '%s'",
		as->parms[2].items->data);
	return code;
    }
    if(aport > BC_MAXPORTOFFSET){
        code = BC_BADTCID;
	com_err(whoami, code,
		"; %u exceeds max port offset %u", aport, BC_MAXPORTOFFSET);
	return code;
    }
    /*
     * Expand out the volume set into its component list of volumes,
     * by calling FLDB server.
     */
    code = bc_EvalVolumeSet(bc_globalConfig, &tvolumeSet, &volsToRestore,
			    cstruct);
    if (code) {
	com_err(whoami, code,
		"; Unable to enumerate filesets to restore");
	return code;
    }

    /* Now overwrite with the new server and partition. One could check to
       see if this is really necessary, but that doesn't save any time
       anyway  */

    tvol = volsToRestore;

    while (tvol) {
	  bcopy((char *)&destServ, (char *)&(tvol->server),
		sizeof(struct sockaddr_in));
	  tvol->partition = destPartition;
	  tvol = tvol->next;
    }

    fromDate = 0x7fffffff;	/* last one before this date */
    newExt = (char *) 0;	/* no new extension */
    oldFlag = 1;		/* do restore same volid (and name) */
    /*
     * Perform the call to start the dump.
     */
    for(i=0;i<BC_MAXSIMDUMPS;i++)
	if (!(bc_dumpTasks[i].flags & BC_DI_INUSE)) break;
    if (i >= BC_MAXSIMDUMPS) {
        code = BC_MAXDUMPS;
	com_err(whoami, code,
		"; Unable to start fileset restore at tcid %d",
		aport);
	return code;
    }

    bzero((char *)&(bc_dumpTasks[i]), sizeof(struct bc_dumpTask));
    bc_dumpTasks[i].config = bc_globalConfig;
    bc_dumpTasks[i].volumes = volsToRestore;
    bc_dumpTasks[i].flags = BC_DI_INUSE;
    bc_dumpTasks[i].dumpName = bc_CopyString("disk");
    bc_dumpTasks[i].volSetName = bc_CopyString("restore");
    bc_dumpTasks[i].newExt = bc_CopyString(newExt);
    bc_dumpTasks[i].dumpLevel = 0;
    bc_dumpTasks[i].parentDumpID = 0;
    bc_dumpTasks[i].oldFlag = oldFlag;
    bc_dumpTasks[i].fromDate = fromDate;
    bc_dumpTasks[i].destPartition = destPartition;
    bc_dumpTasks[i].portOffset = aport;
    bc_dumpTasks[i].expDate = 0;
    bc_dumpTasks[i].expType = 0;
    if(as->parms[10].items) {
	bc_dumpTasks[i].dontExecute = (char)1;
	dce_svc_printf(BAK_S_START_RESTORE_MSG);
    }
    else
	bc_dumpTasks[i].dontExecute = (char)0;
        bcopy((char *)&destServ, (char *)&(bc_dumpTasks[i].destServer),
	  sizeof(struct sockaddr_in));

    if (pthread_attr_create(&threadAttr) != 0) {
	bc_dumpTasks[i].flags |= BC_DI_ERROR;
	bc_dumpTasks[i].flags &= ~BC_DI_INUSE;
	return(BC_THREAD);
    }
    if (pthread_attr_setstacksize(&threadAttr, BC_DEFAULT_STACK_SIZE) != 0) {
	bc_dumpTasks[i].flags |= BC_DI_ERROR;
	bc_dumpTasks[i].flags &= ~BC_DI_INUSE;
	return(BC_THREAD);
    }
    code = pthread_create((pthread_t *) &helper,
			  threadAttr,
			  (pthread_startroutine_t)bc_Restorer,
			  (pthread_addr_t)i);
    if (code) {
      com_err(whoami,BC_THREAD,"; Failed to queue restore");
      bc_dumpTasks[i].flags |= BC_DI_ERROR;
      bc_dumpTasks[i].flags &= ~BC_DI_INUSE;
      return BC_THREAD;
    }
    return 0;
}

/*
 * bc_VolSetRestoreCmd : restore a filesetfamily
 * restoreftfamily -family <family name> -file <file name> -tcid <port number> -noaction
 */

bc_VolSetRestoreCmd (as, arock)
    struct cmd_syndesc *as;
    char *arock;
{
  struct bc_volumeSet	 *volsetPtr; /* Ptr to list of generated volume info */
  struct bc_volumeDump *volsToRestore, *lastVol; /* list of volumes to restore */
  struct sockaddr_in    destServ; /* machine to which to restore volume */
  register long code;
  long port = 0;
  pthread_t helper;
  pthread_attr_t threadAttr;
  long fromDate;
  char *volsetName;
  long destPartition;
  int i;
  int oldFlag;
  char *newExt;



  if (as->parms[0].items)
    {
      if (as->parms[1].items)
	{
	  com_err(whoami, 0, "Can't have both -family and -file options");
	  return(-1);
	}


      volsetName = as->parms[0].items->data;
      code  = bc_FindVolumeSet(bc_globalConfig, volsetName, &volsetPtr);
      if (code || !volsetPtr)
	{
	  com_err(whoami,0,"Can't find fileset family  '%s' in backup database", volsetName);
	  return(-1);
	}

      /* Expand fileset family into component filesets */
      code = bc_EvalVolumeSet(bc_globalConfig, volsetPtr, &volsToRestore, cstruct);
      if (code)
	{
	  com_err(whoami,code,"; Failed to evaluate volume set");
	  return(-1);
	}
    }
  else if (as->parms[1].items)
    {
      /* Open file and read restore data from there */

      FILE *fd;
      char line[256];
      char server[50], partition[50], volume[50], rest[256];
      long count;
      struct bc_volumeDump *tvol;


      fd = fopen(as->parms[1].items->data, "r");
      if (!fd)
	{
	  com_err(whoami,errno,"Cannot open file '%s'", as->parms[1].items->data);
	  return(-1);
	}

      volsToRestore = lastVol = (struct bc_volumeDump *)0;
      while ( fgets(line, 255, fd) )
	{
	  count = sscanf(line, "%s %s %s %s", server, partition, volume, rest);

	  if (count <= 0) continue;
	  if (count < 3)
	    {
	      printf("Invalid filesetfamily file format: Wrong number of arguments: Ignoring\n");
	      printf("     %s", line);
	      continue;
	    }

	  bzero((char *)&destServ, sizeof(destServ));
	  if (!bc_hostNameToAddr(server, &destServ)) {
	    code = BC_NOHOST;
	    com_err(whoami, code,
		    "; Unable to locate destination host '%s'",
		    server);
	    return code;
	  }

	  if ( bc_GetPartitionID(partition, server, &destPartition) )
	    {
	      com_err(whoami,0,"Failed to parse destination partition '%s'", partition);
	      continue;
	    }

	  /* Allocate a volumeDump structure and link it in */
	  tvol = (struct bc_volumeDump *) malloc(sizeof(struct bc_volumeDump));
	  bzero(tvol, sizeof(struct bc_volumeDump));
	  tvol->name = (char *) malloc(BU_MAXNAMELEN + 1);
	  strncpy(tvol->name, volume, BU_MAXNAMELEN);

	  bcopy((char *)&destServ, (char *)&(tvol->server),
		sizeof(struct sockaddr_in));

	  tvol->partition = destPartition;

	  if (lastVol)
	    {
	      lastVol->next = tvol;
	      lastVol       = tvol;
	    }
	  else
	    {
	      volsToRestore = tvol;
	      lastVol       = tvol;
	    }
	}
      fclose(fd);
    }
  else
    {
      com_err(whoami,errno,"-volumeset or -volumesetfile option required");
      return(-1);
    }

  if(as->parms[2].items) {
    port = bc_SafeATOI(as->parms[2].items->data);
    if (port == -1) {
      code = BC_BADTCID;
      com_err(whoami, code,
	      "; Unable to parse tcid '%s'",
	      as->parms[2].items->data);
      return code;
    }
    if (port > BC_MAXPORTOFFSET){
      code = BC_BADTCID;
      com_err(whoami, code,
	      "; %u exceeds max port offset %u", port, BC_MAXPORTOFFSET);
      return code;
    }
  }
  else
    port = 0;

  fromDate = 0x7fffffff;	/* last one before this date */
  newExt = (char *) 0;		/* no new extension */
  oldFlag = 1;			/* do restore same volid (and name) */

  for(i=0;i<BC_MAXSIMDUMPS;i++)
    if (!(bc_dumpTasks[i].flags & BC_DI_INUSE)) break;
  if (i >= BC_MAXSIMDUMPS) {
    code = BC_MAXDUMPS;
    com_err(whoami, code,
	    "; Unable to start fileset restore at tcid %d",
	    port);
    return code;
  }


  bzero((char *)&(bc_dumpTasks[i]), sizeof(struct bc_dumpTask));
  bc_dumpTasks[i].config = bc_globalConfig;
  bc_dumpTasks[i].volumes = volsToRestore;
  bc_dumpTasks[i].flags = BC_DI_INUSE;
  bc_dumpTasks[i].dumpName = bc_CopyString("disk");
  bc_dumpTasks[i].volSetName = bc_CopyString("restore");
  bc_dumpTasks[i].newExt = bc_CopyString(newExt);
  bc_dumpTasks[i].dumpLevel = 0;
  bc_dumpTasks[i].parentDumpID = 0;
  bc_dumpTasks[i].oldFlag = oldFlag;
  bc_dumpTasks[i].fromDate = fromDate;
  bc_dumpTasks[i].destPartition = 0;
  bc_dumpTasks[i].portOffset = port;
  bc_dumpTasks[i].expDate = 0;
  bc_dumpTasks[i].expType = 0;

  if(as->parms[3].items) {
    bc_dumpTasks[i].dontExecute = (char)2; /* Special for restoreftfamily */
    printf("Starting restore\n");
  }
  else
    bc_dumpTasks[i].dontExecute = (char)0;

  if (pthread_attr_create(&threadAttr) != 0) {
    bc_dumpTasks[i].flags |= BC_DI_ERROR;
    bc_dumpTasks[i].flags &= ~BC_DI_INUSE;
    return(BC_THREAD);
  }
  if (pthread_attr_setstacksize(&threadAttr, BC_DEFAULT_STACK_SIZE) != 0) {
    bc_dumpTasks[i].flags |= BC_DI_ERROR;
    bc_dumpTasks[i].flags &= ~BC_DI_INUSE;
    return(BC_THREAD);
  }
  code = pthread_create((pthread_t *) &helper,
			threadAttr,
			(pthread_startroutine_t)bc_Restorer,
			(pthread_addr_t)i);
  if (code) {
    com_err(whoami,BC_THREAD,"; Failed to queue restore");
    bc_dumpTasks[i].flags |= BC_DI_ERROR;
    bc_dumpTasks[i].flags &= ~BC_DI_INUSE;
    return BC_THREAD;
  }
  return 0;

}


/*-----------------------------------------------------------------------------
 * bc_DumpCmd
 *
 * Description:
 *	Perform a dump of the set of volumes provided.
 *	user specifies: -volumeset .. -dump .. [-portoffset ..] [-n]
 *
 * Arguments:
 *	as	: Parsed command line information.
 *	arock	: Ptr to misc stuff; not used.
 *
 * Returns:
 *	-1 on errors,
 *	The result of bc_StartDump() otherwise
 *
 * Environment:
 *	Nothing special.
 *
 * Side Effects:
 *	As advertised.
 *---------------------------------------------------------------------------
 */

int bc_DumpCmd(as, arock)
    struct cmd_syndesc *as;
    char *arock;

{ /*bc_DumpCmd*/

    static char	rn[] = "bc_DumpCmd";	    /*Routine name*/
    char *dumpPath, *vsName;		    /*Ptrs to various names*/
    struct bc_volumeSet	*tvs;		    /*Ptr to list of generated volume info*/
    struct bc_dumpSchedule *tds, *ptrds;   /*Ptr to dump schedule node*/
    struct bc_dumpSchedule baseds;
    struct bc_volumeDump *tve, *volsToDump; /*Ptr to individual vols to be dumped*/
    struct budb_dumpEntry dumpEntry;	    /* dump entry */
    long parent = 0;			    /* parent dump */
    long level = 0;			    /* this dump's level # */

    long problemFindingDump;		    /* can't find parent(s) */
    long aport = 0;
    int haveInfo;			    /* flags for dump searching */
    register long code;	                    /*Return code*/

    struct bc_dumpTask *dumpTaskPtr;	    /* for dump thread */
    long dumpTaskSlot;
    char *cDate;
    pthread_t helper;
    pthread_attr_t threadAttr;
    extern struct bc_dumpTask bc_dumpTasks[];
    extern long bcdb_FindLastVolClone();
    extern long volImageTime();

    vsName = as->parms[0].items->data;		/* get volume set name */
    dumpPath = as->parms[1].items->data;    	/* get dump path */

    dce_svc_printf(BAK_S_START_DUMP_FAMILY_MSG, vsName, dumpPath);

    /* get the port number, if one was specified */
    if(as->parms[2].items)
    	aport = bc_SafeATOI(as->parms[2].items->data);
    else
    	aport = 0;

    if (aport < 0)
    {
        code = BC_BADTCID;
	com_err(whoami, code,
		"; Unable to parse tcid '%s'", as->parms[2].items->data);
	return code;
    }

    if(aport > BC_MAXPORTOFFSET)
    {
        code = BC_BADTCID;
	com_err(whoami, code,
		"; %u exceeds max port offset %u",
		aport,BC_MAXPORTOFFSET);
	return code;
    }

    /* Expand out the volume set into its component list of volumes. */
    lock_ObtainRead(&(bc_globalConfig->vsetLock));
    code = bc_FindVolumeSet(bc_globalConfig, vsName, &tvs);
    if (code || !tvs)
    {
	com_err(whoami,code,
		"; Unable to find fileset family '%s'", vsName);
	lock_ReleaseRead(&(bc_globalConfig->vsetLock));
	return code;
    }
    code = bc_EvalVolumeSet(bc_globalConfig, tvs, &volsToDump, cstruct);
    if (code)
    {
	com_err(whoami, code,
		"; Unable to enumerate filesets to dump");
	lock_ReleaseRead(&(bc_globalConfig->vsetLock));
	return code;
    }
    lock_ReleaseRead(&(bc_globalConfig->vsetLock));

    lock_ObtainRead(&(bc_globalConfig->dschedLock));
    code = bc_FindDumpSchedule(bc_globalConfig, dumpPath, &ptrds);
    if (code || !ptrds)
    {
	com_err(whoami, code,
		"; Unable to find dump schedule '%s'", dumpPath);
	lock_ReleaseRead(&(bc_globalConfig->dschedLock));
	return code;
    }
    /* Now look in dump schedule and get the parent dump name.  Find out if
     * there's a parent dump instance.  If there isn't, keep looking up the
     * dump schedule tree to see if there's any dump instance that we can base
     * this incremental off of.  At worst, we'll have to do a full dump.
     */
    tds = ptrds;
    haveInfo = 0;		/* haven't found a usable dump instance yet */
    problemFindingDump = 0;	/* haven't encountered any missing dumps yet */
    while(tds->parent)
    {
	/* find the most recent dump of the specified volumeset and dumpPath */

	code = bcdb_FindLatestDump(vsName, tds->parent->name, &dumpEntry);
	if (code)
	{
	    dce_svc_printf(BAK_S_NO_PARENT_DUMP_MSG, vsName, tailCompPtr(tds->parent->name));
	    problemFindingDump = 1;
	    tds = tds->parent;
	    continue;
	}
	/* Now check if this is the correct, latest dump level otherwise to do
	 * a dump relative to.
	 */

	if (dumpEntry.id > parent) { /* is it latest ? */
	  if (parent) problemFindingDump = 1;
	  parent = dumpEntry.id;
	  level = dumpEntry.level+1;
	}
	tds = tds->parent;
      }

    /*
     * We release the dsched lock here. To avoid surprises we copy the names.
     * When using the parent of sibling pointers in the dump schedule struct
     * please be careful because they may no longer be valid with some
     * concurrent add/delete/modify operation. If you want to be sure when
     * using these ptrs, grab the dschedLock and verify.
     */
    bcopy((char *)ptrds, (char *)&baseds, sizeof(struct bc_dumpSchedule));
    baseds.name = (char *)osi_Alloc(strlen(ptrds->name)+1);
    if (baseds.name == (char *)NULL) {
      lock_ReleaseRead(&(bc_globalConfig->dschedLock));
      return BC_NOMEM;
    }
    strcpy(baseds.name, ptrds->name);
    lock_ReleaseRead(&(bc_globalConfig->dschedLock));

    if (parent)
      printf("Found parent: dump id %u\n", parent);

    /* print appropriate warning */
    if (problemFindingDump)
    {
	dce_svc_printf(BAK_S_DOING_HIGHER_LEVEL_MSG, level);
    }
    /* Now, on a per volume basis, we determine the incremental time for
     * the dump. For full dumps the time is 0.
     */
    for ( tve = volsToDump; tve != 0; tve = tve->next )
    { /*tve*/
	/* For each volume in the volume expansion,
	 * get the clone time of the volume
	 */
	code = volImageTime(tve);
	if ( code )
	{
	  com_err(whoami, code,
		  "; Unable to determine clone time of fileset '%s' - using 0",
		  tve->name);
	}

	/* find the clone time from the previous dump of this volume, looking
	 * of course only in dumps up the selected tree
	 */
	code = bcdb_FindClone(parent, tve->name, &tve->date);
	if ( code )
	{
	  com_err(whoami, code,
		  "; Unable to find an older dump of fileset '%s'",
		  tve->name);
	  tve->date = 0;
	}

	/* now issue warnings if the volume's time stamp is unchanged from
	 * the last dump of that volume. For full dumps need to do additional
	 * lookups
	 */
	if ( baseds.parent == 0 )
	{
	  /* full dump */
	  Date fullCloneDate=0;
	  code = bcdb_FindLastVolClone(vsName, baseds.name, tve->name,
				       &fullCloneDate);
	  if ( (code == 0)
	      &&   (fullCloneDate == tve->cloneDate)
	      )
	    dce_svc_printf(BAK_S_TIMESTAMP_UNCHAGED_MSG, tve->name);
	}
	else {
	  /* incremental dump */
	  if ( tve->cloneDate == tve->date )
	    dce_svc_printf(BAK_S_TIMESTAMP_UNCHANGED_MSG, tve->name);
	}
    } /*tve*/

    /* List out the expanded volume names and IDs. */
    /* check the no execute flag i.e. -n */
    if(as->parms[3].items) {
	dce_svc_printf(BAK_S_WOULD_HAVE_DUMPED_MSG);
	for( tve=volsToDump; tve; tve=tve->next ) {
	  cDate = ctime((time_t *)&(tve->cloneDate));
	  if (cDate)
	    cDate[strlen(cDate)-1] = '\0'; /*strip the \n*/
	  dce_svc_printf(BAK_S_VOL_ID_AND_NAME_MSG, tve->name,
			 AFS_HGETBOTH(tve->vid), (cDate?cDate:""));
	}
	osi_Free(baseds.name, strlen(baseds.name)+1);
	return 0;
    }
    dce_svc_printf(BAK_S_PREP_TO_DUMP_MSG);
    for(tve=volsToDump; tve; tve=tve->next) {
      cDate = ctime((time_t *)&(tve->cloneDate));
      if (cDate)
	cDate[strlen(cDate)-1] = '\0'; /*strip the \n*/
      dce_svc_printf(BAK_S_VOL_ID_AND_NAME_MSG,
		     tve->name, AFS_HGETBOTH(tve->vid), (cDate?cDate:""));
    }

    /* now start the dump */
    for ( dumpTaskSlot = 0; dumpTaskSlot < BC_MAXSIMDUMPS; dumpTaskSlot++ )
    {
	if ( (bc_dumpTasks[dumpTaskSlot].flags & BC_DI_INUSE) == 0 )
    		break;
    }
    if ( dumpTaskSlot >= BC_MAXSIMDUMPS)
    {
        code = BC_MAXDUMPS;
	com_err(whoami, code,
		"; Unable to start fileset dump at tcid %d",
		aport);
	osi_Free(baseds.name, strlen(baseds.name)+1);
	return code;
    }

    dumpTaskPtr = &bc_dumpTasks[dumpTaskSlot];
    bzero((char *)dumpTaskPtr, sizeof(*dumpTaskPtr));
    dumpTaskPtr->config = bc_globalConfig;
    dumpTaskPtr->volumes = volsToDump;
    dumpTaskPtr->flags = BC_DI_INUSE;
    dumpTaskPtr->dumpName = bc_CopyString(dumpPath);
    dumpTaskPtr->volSetName = bc_CopyString(vsName);
    dumpTaskPtr->dumpLevel = level;
    dumpTaskPtr->parentDumpID = parent;
    dumpTaskPtr->portOffset = aport;
    dumpTaskPtr->expDate = baseds.expDate;
    dumpTaskPtr->expType = baseds.expType;
    if (pthread_attr_create(&threadAttr) != 0) {
      com_err(whoami, BC_THREAD,
	      "failed to create thread attribute");
      dumpTaskPtr->flags |= BC_DI_ERROR;
      dumpTaskPtr->flags &= ~BC_DI_INUSE;
      osi_Free(baseds.name, strlen(baseds.name)+1);
      return(BC_THREAD);
    }
    if (pthread_attr_setstacksize(&threadAttr, BC_DEFAULT_STACK_SIZE) != 0) {
	com_err(whoami, BC_THREAD, "failed to set stack size to %d",
		BC_DEFAULT_STACK_SIZE);
	dumpTaskPtr->flags |= BC_DI_ERROR;
	dumpTaskPtr->flags &= ~BC_DI_INUSE;
	osi_Free(baseds.name, strlen(baseds.name)+1);
	return (BC_THREAD);
    }
    code = pthread_create(&helper, threadAttr,
			  (pthread_startroutine_t)bc_Dumper,
			  (pthread_addr_t)dumpTaskPtr);
    if (code) {
      com_err(whoami,BC_THREAD,"Unable to start dump; (Thread start failed)");
      dumpTaskPtr->flags |= BC_DI_ERROR;
      dumpTaskPtr->flags &= ~BC_DI_INUSE;
      osi_Free(baseds.name, strlen(baseds.name)+1);
      return(BC_THREAD);
    }
    dce_svc_printf(BAK_S_STARTING_DUMP_MSG);
    osi_Free(baseds.name, strlen(baseds.name)+1);
    return(0);
} /*bc_DumpCmd*/


/* bc_QuitCmd
 *	terminate the bak process.
 * parameters:
 *	ignored
 */

bc_QuitCmd(as, arock)
struct cmd_syndesc *as;
char *arock; {
    exit(0);
}

bc_dumpInfoCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    long dumpid;
    long detailFlag;
    long ndumps;
    long dumpInfo();

    if( as->parms[0].items && as->parms[0].items->data ) {
      if (as->parms[2].items)
	dce_svc_printf(BAK_S_VERBOSE_SWITCH_IGNORE_MSG);
      ndumps = atoi(as->parms[0].items->data);
      if (ndumps > 0)
	(void)printRecentDumps(ndumps);
      else {
	com_err(whoami, BC_BADARG, "; count of dumps '%s' invalid",
		as->parms[0].items->data);
	return 1;
      }
    }
    else if(as->parms[1].items) {
      dumpid = atoi(as->parms[1].items->data);
      if(as->parms[2].items)
	detailFlag = 1;				/* detailed listing */
      else
	detailFlag = 0;
      if (dumpid > 0)
	(void)dumpInfo(dumpid, detailFlag);
      else {
	com_err(whoami, BC_BADARG, "; dump ID '%s' invalid",
		as->parms[1].items->data);
	return 1;
      }
      return(0);
    }
    else {
      if (as->parms[2].items)
	dce_svc_printf(BAK_S_VERBOSE_SWITCH_IGNORE_MSG );
      (void)printRecentDumps(10);
    }
    return(0);
}

/* bc_LabelTapeCmd
 *	Labels a tape i.e. request the tape coordinator to perform this
 *	operation
 */

bc_LabelTapeCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    char tapename[256];
    long size;
    long code;
    long port;

    if(as->parms[0].items)
	strcpy(tapename,as->parms[0].items->data);
    else
	strcpy(tapename,"");

    if(as->parms[1].items) {
	size = bc_SafeATOI(as->parms[1].items->data);
	if (size == -1) {
	    code = BC_BADARG;
	    com_err(whoami, code,
		    "; Unable to parse tape size '%s'",
		    as->parms[1].items->data);
	    return code;
	}
    }
    else
	size = 0;
    if(as->parms[2].items) {
	port = bc_SafeATOI(as->parms[2].items->data);
	if (port == -1) {
	    code = BC_BADTCID;
	    com_err(whoami, code,
		    "; Unable to parse tcid '%s'",
		    as->parms[2].items->data);
	    return code;
	}
    if (port > BC_MAXPORTOFFSET){
        code = BC_BADTCID;
	com_err(whoami, code,
		"; %u exceeds max port offset %u", port, BC_MAXPORTOFFSET);
	return code;
    }

    }
    else
	port = 0;
    code = bc_LabelTape(tapename, size, bc_globalConfig, port);
    if (code)
	return code;
    return 0;
}

/* bc_ReadLabelCmd
 *	read the label on a tape
 * params:
 *	optional port number
 */

bc_ReadLabelCmd(as, arock)
struct cmd_syndesc *as;
char *arock;
{
    long code;
    long port;

    if(as->parms[0].items) {
      port = bc_SafeATOI(as->parms[0].items->data);
      if (port == -1) {
	code = BC_BADTCID;
	com_err(whoami, code,
		"; Unable to parse tcid '%s'", as->parms[0].items->data);
	return code;
      }
      if (port > BC_MAXPORTOFFSET){
	code = BC_BADTCID;
	com_err(whoami, code,
		"; %u exceeds max port offset %u", port, BC_MAXPORTOFFSET);
	return code;
      }
    }
    else
      port = 0;
    code = bc_ReadLabel(bc_globalConfig, port);
    if (code) return code;
    return 0;
}

/* bc_ScanDumpsCmd
 *	read content information from dump tapes, and if user desires,
 *	add it to the database
 */

bc_ScanDumpsCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    long port = 0;
    long dbAddFlag = 0;
    long code;

    /* check for flag */
    if ( as->parms[0].items != 0 )		/* add scan to database */
      dbAddFlag++;
    /* check for port */
   if ( as->parms[1].items != 0 )  {
     port = bc_SafeATOI(as->parms[1].items->data);
     if ( port == -1 ) {
       code = BC_BADTCID;
       com_err(whoami, code,
	       "; Unable to parse  port '%s'",
	       as->parms[1].items->data);
       return code;
     }
     if (port > BC_MAXPORTOFFSET){
       code = BC_BADTCID;
       com_err(whoami, code,
	       "; %u exceeds max port offset %u", port, BC_MAXPORTOFFSET);
       return code;
     }
   }
    code = bc_ScanDumps(bc_globalConfig, dbAddFlag, port);
    return (code);
}


/* clone time fixes PA */

/*
 * MapHostToNetwork
 * 	maps the host entries in <entry> which are present in host byte order
 * 	to network byte order
 */
MapHostToNetwork(entry)
struct vldbentry *entry;
{
    int i,count;

    count = entry->nServers;
    if(count < MAXNSERVERS) count++;
    for(i = 0; i < count; i++) {
	entry->siteAddr[i].type = htons(entry->siteAddr[i].type);
    }
}

/* set <server> and <part> to the correct values depending on
 * <voltype> and <entry> */
static void GetServerAndPart(entry,voltype,server,part)
struct vldbentry *entry;
long *server,*part;
int voltype;
{
    int i, xl[MAXTYPES];
    struct sockaddr_in *sin;

    *server = -1;
    *part = -1;

    fts_GetIndices( entry, xl );
    if((voltype == RWVOL)|| (voltype == BACKVOL)){
        for(i = 0; i < entry->nServers ; i++){
            if(xl[RWVOL] && (entry->siteFlags[i] &
			     ((unsigned)VLSF_ZEROIXHERE >> xl[RWVOL]))){
		sin = (struct sockaddr_in *) &entry->siteAddr[i];
                *server = sin->sin_addr.s_addr;
                *part = entry->sitePartition[i];
            }
        }
    }
    if(voltype == ROVOL){
        for(i = 0; i < entry->nServers ; i++){
            if(xl[ROVOL] && (entry->siteFlags[i] &
			     ((unsigned)VLSF_ZEROIXHERE >> xl[ROVOL]))){
		sin = (struct sockaddr_in *) &entry->siteAddr[i];
                *server = sin->sin_addr.s_addr;
                *part = entry->sitePartition[i];
            }
        }
    }
    return;
}

/* volImageTime
 *	Determine the time stamp to be recorded with the backup of this
 *	volume. For backup and r/o volumes this is the clone time, for
 *	r/w volumes, this is the current time. This timestamp is stored
 *	directly into the cloneDate field of the bc_volumeDump structure
 * exit:
 *	0 - success
 *	-1 - failed to get information. Sets cloneDate to 0.
 */

long
volImageTime( vdptr )
     struct bc_volumeDump *vdptr;
{
    struct ftserver_status *viptr;
    long statSize;
    long code,i;
    struct timeval tp;
    struct timezone tzp;

    error_status_t theCommStatus;
    struct ftserver_status status;

    code = volc_Call(VC_VolumeStatus, (struct sockaddr *)&vdptr->server, NULL,
		     vdptr->partition, &vdptr->vid, &status);
    if (code) {
      com_err(whoami, code,
	      "; Unable to get clone time for fileset '%s' - using 0",
	      vdptr->name);
      goto error;
    }

    /* volume types from vol/voldefs.h */
    switch ( status.vss.type )
    {
    case VOL_RW:
	/* For a r/w volume there may not be any foolproof way of
	 * preventing anomalies in the backups. Use the current time;
	 */

        osi_gettimeofday(&tp, &tzp);
	vdptr->cloneDate = tp.tv_sec;
	break;

    case VOL_READONLY:
	/* clone time is the creation time */
	vdptr->cloneDate = status.vss.cloneTime.sec;
	break;

    default:
	com_err(whoami, BC_INTERNALERROR,
		"; Unknown fileset type for fileset '%s'",
		vdptr->name);
	return(BC_INTERNALERROR);
    }
    return(0);

error:
    vdptr->cloneDate = 0;
    return code;
}


/* database lookup command and routines */
/* bc_dblookupCmd
 *	Currently a single option, volumename to search for. Reports
 *	all dumps containing the specified volume
 */

bc_dblookupCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    extern long DBLookupByVolume();
    return(DBLookupByVolume(as->parms[0].items->data));
}

bc_dbVerifyCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    long status;
    long orphans;
    long host;

    struct hostent *hostPtr;
    long detail = 0;
    long code = 0;

    extern struct udbHandleS udbHandle;

    if ( as->parms[0].items )
	detail++;			/* print more details */


    code = ubik_Call(BUDB_DbVerify, udbHandle.uh_client, 0,
		     &status, &orphans, &host);

    if ( code ) {
      com_err(whoami, code, "; Unable to verify database");
      return code;
    }
    /* verification call succeeded */
    if ( status == 0 )
      dce_svc_printf(BAK_S_DATABASE_OK_MSG);
    else
      dce_svc_printf(BAK_S_DATABASE_NOT_OK_MSG);
    fflush(stdout);

    if ( detail ) {
      dce_svc_printf(BAK_S_ORPHAN_BLOCK_COUNT_MSG, orphans);
#if 0
      hostPtr = gethostbyaddr((char *) &host, sizeof(host), AF_INET);
      if (hostPtr == 0 )
	dce_svc_printf(BAK_S_HOST_LOOKUP_FAILED_MSG);
      else
	dce_svc_printf(BAK_S_DATABASE_CHECKER_HOST_MSG, hostPtr->h_name);
#endif /* 0 */
    }
    return(0);
}

/* bc_deleteDumpCmd
 *	Delete a specified dump from the database
 * entry:
 *	dump id - single required arg as param 0.
 */

bc_deleteDumpCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    u_long dumpid;
    long code = 0;

    extern struct udbHandleS udbHandle;

    dumpid = atoi(as->parms[0].items->data);
    if (dumpid <= 0) {
      com_err(whoami, BC_BADARG,
	      "; Unable to parse dump id '%s'", as->parms[0].items->data);
      return(BC_BADARG);
    }
    code = ubik_Call(BUDB_DeleteDump, udbHandle.uh_client, 
		     UF_SYNC_SITE, dumpid);
    if ( code ) {
      com_err(whoami, code,
	      "; Unable to delete dump id '%s'",
	      as->parms[0].items->data);
      return(code);
    }
    return(0);
}

bc_saveDbCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    rpc_binding_handle_t tconn;
    long portOffset = 0;
    statusP statusPtr;
    u_long taskId;
    long code=0;
    long st = 0;
    extern statusP createStatusNode();

    /* create status monitor block */
    statusPtr = createStatusNode(BC_REFCOUNT);
    if ( as->parms[0].items ){
	portOffset = bc_SafeATOI(as->parms[0].items->data);
	if ( portOffset < 0 ) {
	    code = BC_BADTCID;
	    com_err(whoami, code,
		    "; Unable to parse tcid '%s'",
		    as->parms[0].items->data);
	    zapStatusNode(statusPtr);
	    return code;
	}
	if (portOffset > BC_MAXPORTOFFSET){
	  code = BC_BADTCID;
	  com_err(whoami, code,
		  "; %u exceeds max port offset %u",
		  portOffset, BC_MAXPORTOFFSET);
	  return code;
	}
    }
    tconn = bc_GetConn(bc_globalConfig, portOffset);
    if ( tconn == 0 ) {
        code = BC_NOCONN;
	com_err(whoami, code,
		"; Unable to connect to tape coordinator at port %d",
		portOffset);
	zapStatusNode(statusPtr);
	return code;
    }
    code = CheckTCVersion(tconn);
    if ( code ) {
	com_err(whoami, code,
		"; Unable to contact tape coordinator at port %d",
		portOffset);
	goto exit;
      };

    code = BUTC_SaveDb(tconn, &taskId);
    if ( code ) {
        com_err(whoami, code,
		"; Unable to save the backup database at tcid %d",
		portOffset);
	goto exit;
    }

    lock_ObtainWrite(&statusQueueLock);
    statusPtr->taskId = taskId;
    statusPtr->port = portOffset;
    statusPtr->jobNumber =  bc_jobNumber();
    statusPtr->opType = TCOP_SAVEDB;
    sprintf(statusPtr->taskName, "SaveDb");
    lock_ReleaseWrite(&statusQueueLock);
    clearStatus(taskId, STARTING);		/* ok to examine */
    lock_ObtainWrite(&statusQueueLock);
    deleteStatusNode(statusPtr);		/* drop the refCount */
    lock_ReleaseWrite(&statusQueueLock);

exit:
    if (code)
	zapStatusNode(statusPtr);
    bc_FreeConn(bc_globalConfig, tconn);
    return(code);
}

bc_restoreDbCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    rpc_binding_handle_t tconn;
    long portOffset = 0;
    statusP statusPtr;
    u_long taskId;
    long code=0;
    long st = 0;
    extern statusP createStatusNode();

    /* create status monitor block */
    statusPtr = createStatusNode(BC_REFCOUNT);
    if ( as->parms[0].items ) {
	portOffset = bc_SafeATOI(as->parms[0].items->data);
	if ( portOffset < 0 )
	{
	    code = BC_BADTCID;
	    com_err(whoami, code,
		    "; Unable to parse tcid '%s'",
		    as->parms[0].items->data);
	    zapStatusNode(statusPtr);
	    return code;
	}
	if (portOffset > BC_MAXPORTOFFSET){
	  code = BC_BADTCID;
	  com_err(whoami, code,
		  "; %u exceeds max port offset %u",
		  portOffset, BC_MAXPORTOFFSET);
	  return code;
	}
    }
    tconn = bc_GetConn(bc_globalConfig, portOffset);
    if ( tconn == 0 ) {
        code = BC_NOCONN;
	com_err(whoami, code,
		"; Unable to connect to tape coordinator at port %d",
		portOffset);
	zapStatusNode(statusPtr);
	return code;
    }
    code = CheckTCVersion(tconn);
    if ( code ) {
        com_err(whoami, code,
		"; Unable to contact tape coordinator at port %d",
		portOffset);
	goto exit;
    }
    code = BUTC_RestoreDb(tconn, &taskId);
    if ( code ) {
	com_err(whoami, code, "; Unable to restore database at tcid %d",
		portOffset);
	goto exit;
    }
    lock_ObtainWrite(&statusQueueLock);
    statusPtr->taskId = taskId;
    statusPtr->port = portOffset;
    statusPtr->jobNumber =  bc_jobNumber();
    statusPtr->opType = TCOP_RESTOREDB;
    sprintf(statusPtr->taskName, "RestoreDb");
    lock_ReleaseWrite(&statusQueueLock);
    clearStatus(taskId, STARTING);		/* ok to examine */
    lock_ObtainWrite(&statusQueueLock);
    deleteStatusNode(statusPtr);		/* drop the refCount */
    lock_ReleaseWrite(&statusQueueLock);

exit:
    if (code)
	zapStatusNode(statusPtr);
    bc_FreeConn(bc_globalConfig, tconn);
    return(code);
}

/* ----------------------------------
 * supporting routines for database examination
 * ----------------------------------
 */

/* structures and defines for DBLookupByVolume */
/*
 * dumpedVol - saves interesting information so that we can print it out
 *	later
 */

struct dumpedVol
{
    struct dumpedVol *next;
    long dumpID;
    char tapeName[BU_MAXTAPELEN];
    long level;
    long parent;
    long createTime;
    long incTime;			/* actually the clone time */
};

/* compactDateString
 *	print out a date in compact format, 14 chars, format is
 *	mm/dd/yy hh:mm
 * entry:
 *	date_long - ptr to a long containing the time
 * exit:
 *	ptr to a string containing a representation of the date
 */

char *
compactDateString(date_long)
     long *date_long;
{
    static char buffer[256];
    struct tm *ltime;

    ltime = localtime(date_long);

    /* prints date in U.S. format of mm/dd/yy */
    sprintf(buffer, "%02d/%02d/%02d %02d:%02d",
            ltime->tm_mon+1,
            ltime->tm_mday,
            ltime->tm_year % 100,
            ltime->tm_hour,
            ltime->tm_min);

    return(&buffer[0]);
}

/* -----------------------------------------
 * routines for examining the database
 * -----------------------------------------
 */



/* DBLookupByVolume
 *	Lookup the volumename in the bak database and print the results
 * entry:
 *	volumeName - volume to lookup
 */

long
DBLookupByVolume (volumeNam)
  char *volumeNam;
{
    budb_dumpEntry dumpEntry;
    budb_volumeList *volumeList;
    budb_volumeEntry *volumeEntries;
    long numEntries;
    long last,next;

    struct dumpedVol *dvptr = 0;
    struct dumpedVol *tempPtr = 0;
    long code;
    int i;

    last = 0;
    next = 0;


    while ( next != -1 )
    { /*w*/
        volumeList = (budb_volumeList *)NULL;
	code = bcdb_LookupVolume(volumeNam,
				 last,
				 &next,
				 &volumeList);
	if (code) {
	  volumeList = (budb_volumeList *)NULL;
	  goto error;
	}

	numEntries = volumeList->budb_volumeList_len;
	volumeEntries = volumeList->budb_volumeList_val;
	/*
	 * Remember to free the volumeEntries list before making another
	 * call to bcdb_LookupVolume, or on any further error
	 */
	/* add the volumes to the list */
	for ( i = 0; i < numEntries; i++ )
	{ /*f*/
	    struct dumpedVol *insPtr, **prevPtr;

	    tempPtr = (struct dumpedVol *)osi_Alloc(sizeof(struct dumpedVol));
	    if ( tempPtr == 0 ) {
	      code = BC_NOMEM;
	      goto error;
	    }
	    bzero((char *)tempPtr, sizeof(*tempPtr));

	    tempPtr->incTime = volumeEntries[i].clone;
	    tempPtr->dumpID = volumeEntries[i].dump;
	    strcpy((char *)tempPtr->tapeName, (char *)volumeEntries[i].tape);

	    /* check if we need to null terminate it - just for safety */
	    if ( strlen((char *)volumeEntries[i].tape) >= (unsigned)BU_MAXTAPELEN )
	    	tempPtr->tapeName[BU_MAXTAPELEN-1] = 0;

	    code = bcdb_FindDumpByID(tempPtr->dumpID, &dumpEntry);
	    if ( code ) {
	      osi_Free(tempPtr, sizeof(*tempPtr));
	      goto error;
	    }
	    tempPtr->parent = dumpEntry.parent;
	    tempPtr->level = dumpEntry.level;
	    tempPtr->createTime = dumpEntry.created;

	    /* add volume to list in reverse chronological order */
	    prevPtr = &dvptr;
	    insPtr = dvptr;

	    while ( (insPtr != 0)
	    &&	    (insPtr->createTime > tempPtr->createTime)
		  ) {
	      prevPtr = &insPtr->next;
	      insPtr = insPtr->next;
	    }
	    /* now at the right place - insert the block */
	    tempPtr->next = *prevPtr;
	    *prevPtr = tempPtr;
	} /*f*/
	last = next;
	/* Free the storage allocated by the rpc runtime before repeating */
	(void)free(volumeList);
	volumeList = (budb_volumeList *)NULL;
    } /*w*/

    tempPtr = dvptr;
    dce_svc_printf(BAK_S_DUMP_HEADER_MSG);
    while ( tempPtr != 0 )
    {
	/* beware the static items in compactDateString */
	dce_printf(bak_s_table_item1, tempPtr->dumpID, tempPtr->parent, tempPtr->level,
		compactDateString(&tempPtr->createTime));
	dce_svc_printf(BAK_S_TABLE_ITEM2_MSG, compactDateString(&tempPtr->incTime),
		tempPtr->tapeName);
	tempPtr = tempPtr->next;
    }

error:
    if (volumeList != (struct budb_volumeList *)NULL)
      /* Free the storage allocated by the rpc runtime before leaving */
      (void)free((char *)volumeList);
    tempPtr = dvptr;
    while ( tempPtr != 0 )
    {
	dvptr = dvptr->next;
	osi_Free((char *)tempPtr, sizeof(*tempPtr));
	tempPtr = dvptr;
    }
    return(code);
}




/* structures for dumpInfo */

struct volumeLink
{
    struct volumeLink *nextVolume;
    struct budb_volumeEntry volumeEntry;
};

struct tapeLink
{
    struct tapeLink *nextTape;
    struct budb_tapeEntry tapeEntry;
    struct volumeLink *firstVolume;
};


/* dumpInfo
 *	print information about a dump and all its tapes and volumes.
 */

long
dumpInfo(dumpid, detailFlag)
     long dumpid;
     long detailFlag;
{
    struct budb_dumpEntry dumpEntry;
    struct tapeLink *head = 0;
    struct tapeLink *tapeLinkPtr, *lastTapeLinkPtr;
    struct volumeLink **link, *volumeLinkPtr, *lastVolumeLinkPtr;

    budb_volumeList *vl;
    long last, next, dbTime;

    char tapeName[BU_MAXTAPELEN];
    int tapeNumber;
    int i;
    long code = 0;
    bakNameString_t emptyName;

    extern struct udbHandleS udbHandle;

    tapeLinkPtr = 0;
    lastTapeLinkPtr = 0;
    volumeLinkPtr = 0;
    lastVolumeLinkPtr = 0;
    strcpy((char *)emptyName, "");
    /* first get information about the dump */

    code = bcdb_FindDumpByID(dumpid, &dumpEntry);
    if ( code ) {
      com_err(whoami, code, "; Unable to locate dump ID %ld in database",
	      dumpid);
      ERROR(code);
    }
    /* print out the information about the dump */
    if ( detailFlag == 0 )
    {
	dce_svc_printf(BAK_S_DUMP_INFO_MSG, dumpEntry.id, dumpEntry.level, dumpEntry.nVolumes,
	       ctime((time_t *)&dumpEntry.created));
    }
    else
    {
	dce_svc_printf(BAK_S_DUMP_MSG);
	dce_svc_printf(BAK_S_SEPERATORS_4_MSG);
	printDumpEntry(&dumpEntry);
    }

    /* now get the list of tapes */
    for ( tapeNumber = 0; tapeNumber < dumpEntry.tapes.maxTapes; tapeNumber++ )
    { /*f*/
	tapeLinkPtr = (struct tapeLink *)osi_Alloc(sizeof(struct tapeLink));
	if ( tapeLinkPtr == 0 )
	    ERROR(BC_NOMEM);
	bzero((char *)tapeLinkPtr, sizeof(*tapeLinkPtr));

	tc_MakeTapeName(tapeName, (&dumpEntry.tapes), tapeNumber);
	code = bcdb_FindTape(dumpid, tapeName, &tapeLinkPtr->tapeEntry);
	if ( code )
	{
	    osi_Free(tapeLinkPtr, sizeof(*tapeLinkPtr));
	    break;
	}

	/* add this tape to  previous chain */
	if ( lastTapeLinkPtr != 0 )
	{
	    lastTapeLinkPtr->nextTape = tapeLinkPtr;
	    lastTapeLinkPtr = tapeLinkPtr;
	}

	if ( head == 0 )
	{
	    head = tapeLinkPtr;
	    lastTapeLinkPtr = head;
	}

	last = 0;
	next = 0;
	while ( next != -1 )
	{ /*wn*/
	    /* now get all the volumes in this dump. This is currently
	     * not optimal, since we'll discard volumes that are not
	     * on the current tape
	     */

	    code = ubik_Call(BUDB_GetVolumes, udbHandle.uh_client, 0,
			     CUR_BUDB_VERSION,
			     BUDB_OP_DUMPID | BUDB_OP_TAPENAME,
			     tapeLinkPtr->tapeEntry.name,        /* tape name */
			     dumpid,
			     0,				/* end */
			     last,			/* last */
			     &next,			/* nextindex */
			     &dbTime,			/* update time */
			     &vl);

	    if (code == BUDB_BADFLAGS) {
	      code = ubik_Call(BUDB_GetVolumes, udbHandle.uh_client, 0,
			       CUR_BUDB_VERSION,
			       BUDB_OP_DUMPID,
			       tapeLinkPtr->tapeEntry.name, /* tape name */
			       dumpid,
			       0, /* end */
			       last, /* last */
			       &next, /* nextindex */
			       &dbTime,	/* update time */
			       &vl);
	    }
	    if (code)  {
	        (void)free(vl);
		if ( code == BUDB_ENDOFLIST )   /* 0 volumes on tape */
		    break;
		else
		    goto error_exit;
	    }

	    /* sort the volumes onto the tape */
	    tapeLinkPtr->firstVolume = 0;

	    for ( i = 0; i < vl->budb_volumeList_len; i++ )
	    {
		/* discard volume if it is not on the tape we are currently
		 * processing
		 */
		if ( strcmp((char *)vl->budb_volumeList_val[i].tape, tapeName) != 0 )
		    continue;

		link = &tapeLinkPtr->firstVolume;

		volumeLinkPtr =
		    (struct volumeLink *)osi_Alloc(sizeof(struct volumeLink));
		if ( volumeLinkPtr == 0 )
		    ERROR(BC_NOMEM);
		bzero((char *)volumeLinkPtr, sizeof(*volumeLinkPtr));

		bcopy((char *)&vl->budb_volumeList_val[i],
		      (char *)&volumeLinkPtr->volumeEntry,
		      sizeof(struct budb_volumeEntry));

		/* now insert it onto the right place */
		while ( (*link != 0 )
		&&      (volumeLinkPtr->volumeEntry.position >
			 (*link)->volumeEntry.position)
		      )
		{
		    link = &((*link)->nextVolume);
		}

		/* now link it in */
		volumeLinkPtr->nextVolume = *link;
		*link = volumeLinkPtr;
	    }
	    /* Free the storage allocated by the rpc runtime before leaving */
	    (void)free(vl);
	} /*wn*/
    } /*f*/

    tapeLinkPtr = head;
    while ( tapeLinkPtr != 0 )
    {
	if ( detailFlag == 0 )
	{
	    dce_printf(bak_s_tape_name, tapeLinkPtr->tapeEntry.name);
	    dce_printf(bak_s_fileset_count, tapeLinkPtr->tapeEntry.nVolumes);
	    dce_printf(bak_s_fileset_create_date, compactDateString(&tapeLinkPtr->tapeEntry.written));
	    if ( tapeLinkPtr->tapeEntry.expires != 0 )
		dce_printf(bak_s_expiration_date, compactDateString(&tapeLinkPtr->tapeEntry.expires));
	    dce_printf(bak_s_2_newlines);
	}
	else
	{
	    dce_svc_printf(BAK_S_TAPE_MSG);
	    dce_svc_printf(BAK_S_SEPERATORS_4_MSG);
	    printTapeEntry(&tapeLinkPtr->tapeEntry);
	}

	/* print out all the volumes */

	/* print header for volume listing */
	if ( detailFlag == 0 )
	{
	    dce_svc_printf(BAK_S_HEADER_VOL_TABLE_MSG, "Pos", "Clone time",
			   "Nbytes", "Fileset");
	}
	volumeLinkPtr = tapeLinkPtr->firstVolume;
	while ( volumeLinkPtr )
	{
	    if ( detailFlag == 0 )
	    {
		dce_svc_printf(BAK_S_ITEM_VOL_TABLE_MSG, (volumeLinkPtr->volumeEntry.position==0)?0:
		       (volumeLinkPtr->volumeEntry.position-1),
		       compactDateString(&volumeLinkPtr->volumeEntry.clone),
		       volumeLinkPtr->volumeEntry.nBytes,
		       volumeLinkPtr->volumeEntry.name);
	    }
	    else
	    {
		dce_svc_printf(BAK_S_FILESET_MSG);
		dce_svc_printf(BAK_S_SEPERATORS_6_MSG);
		printVolumeEntry(&volumeLinkPtr->volumeEntry);
	    }

	    volumeLinkPtr = volumeLinkPtr->nextVolume;
	}
	tapeLinkPtr = tapeLinkPtr->nextTape;
    }

error_exit:
    /* free all allocated structures */

    tapeLinkPtr = head;
    while ( tapeLinkPtr )
    {
	volumeLinkPtr = tapeLinkPtr->firstVolume;
	while ( volumeLinkPtr )
	{
	    lastVolumeLinkPtr = volumeLinkPtr;
	    volumeLinkPtr = volumeLinkPtr->nextVolume;
	    osi_Free(lastVolumeLinkPtr, sizeof(*lastVolumeLinkPtr));
	}

	lastTapeLinkPtr = tapeLinkPtr;
	tapeLinkPtr = tapeLinkPtr->nextTape;
	osi_Free(lastTapeLinkPtr, sizeof(*lastTapeLinkPtr));
    }
    return(code);
}

int compareDump(ptr1, ptr2)
    const void *ptr1;
    const void *ptr2;
{
if (
 ((struct budb_dumpEntry *)ptr1)->created < ((struct budb_dumpEntry *)ptr2)->created )
	return(-1);
    else
    if ( ((struct budb_dumpEntry *)ptr1)->created >
         ((struct budb_dumpEntry *)ptr2)->created )
	return(1);
    return(0);
}

printRecentDumps(ndumps)
     int ndumps;
{
    register long code = 0;
    long        nextindex;
    long        dbTime;
    budb_dumpList *dl;
    struct budb_dumpEntry *dumpPtr;
    int i;
    bakNameString_t emptyName;
    extern struct udbHandleS udbHandle;

    strcpy((char *)emptyName, "");
    /* outline algorithm */
    code = ubik_Call (BUDB_GetDumps,  udbHandle.uh_client, 0,
                      CUR_BUDB_VERSION,
		      BUDB_OP_NPREVIOUS,
                      &emptyName,               /* no name */
                      0,	                /* start */
                      ndumps,                   /* end */
                      0,                        /* index */
                      &nextindex,
                      &dbTime,
                      &dl);
    if ( code ) {
	com_err("dumpInfo", code, "Can't get dump information");
	/* free the minimally allocated structure */
	(void)free(dl);
	return(code);
    }

    qsort((void *)dl->budb_dumpList_val, (size_t)dl->budb_dumpList_len,
	  sizeof(struct budb_dumpEntry), (int (*)(const void *, const void *))compareDump);

    if ( dl->budb_dumpList_len )
      dce_printf(bak_s_dump_table_header, "DumpID", "parentID", "lvl",
		 "    created", "nt", "nfsets");
      dce_printf(bak_s_newline);
      dce_printf(bak_s_string_newline,
	     "-------------------------------------------------------------");
    dumpPtr = dl->budb_dumpList_val;
    for ( i = 1; i <= dl->budb_dumpList_len; i++ )
    {
	dce_svc_printf(BAK_S_DUMP_TABLE_ITEM_MSG, dumpPtr->id,
	       dumpPtr->parent,
	       dumpPtr->level,
	       compactDateString(&dumpPtr->created),
	       dumpPtr->tapes.maxTapes, dumpPtr->nVolumes,
	       dumpPtr->name);

	dumpPtr++;
    }
    /* Free the storage allocated by the rpc runtime before leaving */
    (void)free(dl);
    return (code);
}

/* tailCompPtr
 *	name is a pathname style name, determine trailing name and return
 *	pointer to it
 */
char *
tailCompPtr(pathNamePtr)
     char *pathNamePtr;
{
  char *ptr;
  ptr = rindex(pathNamePtr, '/');
  if ( ptr == 0 ) {
    /* this should never happen */
    dce_svc_printf(BAK_S_NO_SLASH_IN_NAME_MSG, pathNamePtr);
    return(pathNamePtr);
  }
  else
    ptr++;					/* skip the / */
  return(ptr);
}
