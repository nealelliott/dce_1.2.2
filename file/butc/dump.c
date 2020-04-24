/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1996, 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * dump.c -- dump filesets to tape
 *
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */

#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/lock.h>
#include <dcedfs/fldb_data.h>
#include <dcedfs/flserver.h>
#include <dcedfs/volc.h>
#include <dcedfs/tcdata.h>
#include <dcedfs/ktime.h>                                  /* PA */
#include <dcedfs/budb.h>
#include <dcedfs/budb_errs.h>
#include <error_macros.h>
#include <dcedfs/butc.h>
#include <dcedfs/bc.h>
#include <dcedfs/async_queue.h>
#include <com_err.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/butc/RCS/dump.c,v 10.17 1996/07/08 14:02:40 madhuc Exp $")

#ifdef notdef
#define dfprintf(x) {fprintf x ; fflush(stdout); }
#else
#define dfprintf(x)
#endif

#define BIGCHUNK	(512*1024)
#define DUMP_NBUFS	64

/* TBD
 *
 * Done 1) dump id generation
 * Done xx) volume fragment number accounting !!  I think.
 * 2) check abort - check after subroutine calls
 * Done 3) trailer anomaly
 * 4) trailer damage indicator after partial dumps ( affects scandump )
 * Done 5) Ensure mount failure logged
 * 6) Ensure bucoord status calls work
 *
 * notes
 * pass 3:
 *	keep token timeout. If no user reponse (idle time > some period)
 *	and tokens about to time out, terminate dump. This provides at
 *	least something usable.
 */

/* dump states */

#define	DUMP_FAILED	1		/* outright failed */
#define	DUMP_PARTIAL	2		/* partial on tape */
#define	DUMP_SUCCESS	3		/* all ok */

/* GLOBAL CONFIGURATION PARAMETERS */
extern int dump_namecheck;
extern int tapeblocks;
extern int queryoperator;
extern int isafile;
extern int sockFlag;
extern int autoQuery;

struct dumpRock
{
    dlqlinkT	vols_toDump;
    dlqlinkT	vol_current;
    dlqlinkT	vols_done;
    dlqlinkT	vols_failed;

    /* statistics & status */
    u_long	volBytes;
    u_long	tapeBytes;			/* residual */
    u_long	tapeKBytes;
    u_long	dumpMBytes;

    /* status only */
    int		tapeSeq;
    char	tapeName[TC_MAXTAPENAMELEN];
    int		totalVolumes;
    int		volumesDone;
    int		passNumber;
    int		curVolumeStatus;		/* more explicit dump state */
    u_long	databaseDumpId;			/* real dump id, for db */

    /* tape management */
     struct butm_tapeInfo tapeInfo;

    /* database information */
    struct budb_dumpEntry dump;			/* current dump */
    struct budb_tapeEntry tape;			/* current tape */

    /* links to existing info */
    struct dumpNode *node;

    /* pipe state data */
    int startVolume;
    int endVolume;
    asyncQueue_t *asyncP;
    asyncElement_t *elemP;
    int chunkSize;
    int fragmentNumber;
    int oldPosition;
};

struct volumeToDump
{
    dlqlinkT		links;			/* links */
    struct tc_dumpDesc	*dumpDescPtr;		/* handle on volume */
};

/* configuration variables */
int maxpass = 3;
int deletePartialDumps = 0;
extern int debugLevel;
Dumper(nodePtr)
     struct dumpNode *nodePtr;
{
    struct dumpRock *rockPtr = 0;
    struct butm_tapeInfo *tapeInfoPtr = 0;
    int pass;
    long code = 0, unusedCode;
    /* for volume setup */
    struct volumeToDump *vtdPtr;
    struct tc_dumpDesc *dumpDescPtr;
    int i;
    extern struct deviceSyncNode *deviceLatch;
    extern struct tapeConfig globalTapeConfig;
    extern long dumpPass();
    extern long createDump();
    extern printvtd();

    EnterDeviceQueue(deviceLatch);
    clearStatus(nodePtr->dumpID, DRIVE_WAIT);

    /* setup the dump rock */
    rockPtr = (struct dumpRock *)osi_Alloc(sizeof(struct dumpRock));
    if ( rockPtr == 0 )
	ERROR_EXIT(TC_NOMEMORY);
    bzero((char *)rockPtr, sizeof(*rockPtr));
    dlqInit(&rockPtr->vols_toDump);
    dlqInit(&rockPtr->vol_current);
    dlqInit(&rockPtr->vols_done);
    dlqInit(&rockPtr->vols_failed);

    /* setup the volume list */
    dumpDescPtr = nodePtr->dumps;
    for ( i = 1; i <= nodePtr->arraySize; i++ )
    {
	vtdPtr = (struct volumeToDump *)osi_Alloc(sizeof(struct volumeToDump));
	if ( vtdPtr == 0 )
	    ERROR_EXIT(-1);
	vtdPtr->dumpDescPtr = dumpDescPtr;
	dlqLinkb(&rockPtr->vols_toDump, (dlqlinkP) vtdPtr);
	rockPtr->totalVolumes++;
	dumpDescPtr++;
    }

    rockPtr->node = nodePtr;
    /* instantiate the tape module */
    tapeInfoPtr = &rockPtr->tapeInfo;
    tapeInfoPtr->structVersion = CUR_BUTM_VERSION;
    code = butm_file_Instantiate(tapeInfoPtr, &globalTapeConfig);
    if ( code )
	ERROR_EXIT(code);

    /* check if abort requested */
    if ( checkAbort(nodePtr) )
	goto error_exit;

    lock_ObtainWrite(&(nodePtr->lock));
    nodePtr->status |= INPROGRESS;
    lock_ReleaseWrite(&(nodePtr->lock));
    /*set status to initializing; */

    /* if no dumps */
    if ( dlqEmpty(&rockPtr->vols_toDump) )
    {
	LogTape("Task %u: dump (%s) No filesets - dump ignored\n",
		nodePtr->dumpID,
		nodePtr->dumpSetName);
	goto error_exit;
    }
    LogTape("\n");
    LogTape("Task %u: dump (%s) Started\n", nodePtr->dumpID,
	    nodePtr->dumpSetName);
    dce_svc_printf(BAK_S_TASK_DUMP_STARTED_MSG, nodePtr->dumpID,
	    nodePtr->dumpSetName);
    /* createDump logs errors */
    code = createDump(rockPtr);			/* enter dump into database */
    if ( code )
	ERROR_EXIT(code);


    /* mount the tape ; getTape logs errors */
    code = getTape(rockPtr, autoQuery);
    if ( checkAbort(nodePtr) ) {
      (void) bcdb_deleteDumpById(rockPtr->databaseDumpId);
	goto error_exit;
    }

    if ( code )
    {
	LogError("Task %u: Unable to mount tape %s", code,
		 nodePtr->dumpID, rockPtr->tapeName);
      (void) bcdb_deleteDumpById(rockPtr->databaseDumpId);
	ERROR_EXIT(code);
    }

    pass = 1;
    while ( pass <= maxpass )
    {
	rockPtr->passNumber = pass;
	if ( checkAbort(nodePtr) )
	  goto error_exit;
	code = dumpPass(rockPtr, pass);
	/* if no failed volumes, we're done */
	if ( dlqCount(&rockPtr->vols_failed) == 0 )
	     break;
	/* move failed volumes back */
	dlqMoveb(&rockPtr->vols_failed, &rockPtr->vols_toDump);
	/* check if user requested an abort */
	pass++;
    }

    code = butm_WriteEOT(tapeInfoPtr);
    if ( code )
    {
	LogTape("Task %u: dump (%s) failed to write EOT on tape; %s\n",
		nodePtr->dumpID, nodePtr->dumpSetName,
		dfs_dceErrTxt(code));
	LogError("Task %u: dump (%s) failed to write EOT on tape",
		 code, nodePtr->dumpID, nodePtr->dumpSetName);
	ERROR_EXIT(code);
    }

    /* print a summary */

    code = bcdb_FinishDump(&rockPtr->dump);
    if (code) {
      LogTape("Task %u: dump (%s) failed to mark dump as completed; %s\n",
	      nodePtr->dumpID, nodePtr->dumpSetName,
	      dfs_dceErrTxt(code));
      LogError("Task %u: dump (%s) failed to mark dump as completed",
	       code, nodePtr->dumpID, nodePtr->dumpSetName);
    }
error_exit:
    /* error and normal exit */
    if (  unusedCode = butm_Dismount(tapeInfoPtr) )
    {
	LogTape("Task %u: dump (%s) could not dismount tape %s\n",
		 nodePtr->dumpID, nodePtr->dumpSetName,
		 rockPtr->tapeName, dfs_dceErrTxt(unusedCode));
	LogError("Task %u: dump (%s) could not dismount tape %s", unusedCode,
		 nodePtr->dumpID, nodePtr->dumpSetName,
		 rockPtr->tapeName);
    }
    if ( checkAbort(nodePtr) ) {
        abortDump(nodePtr);
	LogTape("Task %u: dump (%s) aborted by request\n", nodePtr->dumpID,
		nodePtr->dumpSetName);
	dce_svc_printf(BAK_S_TASK_DUMP_ABORTED_MSG, nodePtr->dumpID,
 nodePtr->dumpSetName);
    }
    else
    {
	LogTape("Task %u: dump (%s) Finished\n", nodePtr->dumpID,
		nodePtr->dumpSetName);
	dce_svc_printf(BAK_S_TASK_DUMP_FINISHED_MSG, nodePtr->dumpID,
 nodePtr->dumpSetName);
    }


    /* pass back the number of volumes we failed to dump */
    /* Make sure you follow the locking order, node before status queue */
    lock_ObtainWrite(&(nodePtr->lock));
    lock_ObtainWrite(&statusQueueLock);
    nodePtr->statusNodePtr->volsFailed = dlqCount(&rockPtr->vols_toDump);
    lock_ReleaseWrite(&statusQueueLock);
    lock_ReleaseWrite(&(nodePtr->lock));
    if (nodePtr->statusNodePtr->volsFailed == 0) {
	LogTape("Task %u: dump (%s) successful : %d filesets dumped\n", nodePtr->dumpID,
		nodePtr->dumpSetName, rockPtr->totalVolumes);

	dce_svc_printf(BAK_S_TASK_DUMP_SUCCESS_MSG, nodePtr->dumpID,
		nodePtr->dumpSetName, rockPtr->totalVolumes);
    } else {

	LogTape("Task %u: dump (%s) : %d fileset dumps succeeded, %d fileset dumps failed \n",
	       nodePtr->dumpID, nodePtr->dumpSetName,
	       rockPtr->totalVolumes - nodePtr->statusNodePtr->volsFailed,
	       nodePtr->statusNodePtr->volsFailed);

	dce_svc_printf(BAK_S_DUMP_FILESET_SUCCESS_MSG, nodePtr->dumpID, nodePtr->dumpSetName,
	       rockPtr->totalVolumes - nodePtr->statusNodePtr->volsFailed,
	       nodePtr->statusNodePtr->volsFailed);
    }

    if ( code )
	setStatus(nodePtr->dumpID, TASK_ERROR);
    setStatus(nodePtr->dumpID, TASK_DONE);

    /* consolidate volume chains */
    dlqMoveb(&rockPtr->vols_toDump, &rockPtr->vols_done);

    /* free volume chains */
    vtdPtr = (struct volumeToDump *)dlqUnlinkf(&rockPtr->vols_done);
    while ( vtdPtr )
    {
	(void)osi_Free(vtdPtr, sizeof(struct volumeToDump));
	vtdPtr = (struct volumeToDump *) dlqUnlinkf(&rockPtr->vols_done);
    }

    /* free dump rock */
    (void)osi_Free(rockPtr, sizeof(struct dumpRock));

    /* free the dump node */
    FreeNode(nodePtr->dumpID);

    LeaveDeviceQueue(deviceLatch);
    return(code);
}

#define BELLTIME 60     /* 60 seconds before a bell rings */
#define BELLCHAR 7      /* ascii for bell */


/* retryPrompt
 * 	prompt the user to decide how to handle a failed volume dump. The
 *	volume parameters describe the volume that failed
 * entry:
 *	volumeName - name of volume
 *	high, low : volid
 *	taskId - for job contrl
 * fn return:
 *	character typed by user, one of r, o or a
 */

char
retryPrompt(u_char *volumeName, u_int32 vhigh, u_int32 vlow, u_int32 taskId)
{
  int32 start;
  char ch;
  int32 code = 0;

  setStatus(taskId, OPR_WAIT);
  printf("\nDump of volume %s (%lu,,%lu) failed\n\n", volumeName, vhigh, vlow);

  printf("Please select action to be taken for this volume\n");

 again:
  printf("r - retry, try dumping this volume again\n");
  printf("o - omit,  this volume from this dump\n");
  printf("a - abort, the entire dump\n");

  while (1) {
    FFlushInput(stdin);
    putchar(BELLCHAR);
    fflush(stdout);

    start = time(0);
    while(1)
      {
	code = FWait(stdin,	/* check every --> seconds */ 5);
	if (code == 0)
	  break;		/* input is available */

	if ( checkAbortByTaskId(taskId) )
	  {
	    clearStatus(taskId, OPR_WAIT);
	    printf("This tape operation has been aborted by the coordinator\n");
	    return 'a';
	  }

	if (time(0) > start + BELLTIME) break;
      }
    /* otherwise, we should beep again, check for abort and go back,
     * since the FWait timed out
     */
    if (code == 0)
      break;			/* input is available */
  }
  clearStatus(taskId, OPR_WAIT);
  ch = getchar();
  if ( ch != 'r' && ch != 'o' && ch != 'a' )
    {
      printf("Please select one of the 3 options, r, o or a\n");
      goto again;
    }

  return ch;
}

long
dumpPass(rockPtr, passNumber)
    struct dumpRock *rockPtr;
     int passNumber;
{
    struct tc_dumpDesc *curDump;
    dlqlinkP	dlqPtr;
    pipe_t dataPipe;
    ftsPipeState pipeState;
    afs_hyper_t volId;
    ftserver_Date dumpDateDesc;
    int action;
    struct dumpNode *nodePtr = rockPtr->node;
    struct butm_tapeInfo *tapeInfoPtr = &rockPtr->tapeInfo;
    asyncQueue_t *qPtr;
    long code = 0;
    long mask;
    int ix, jx, volType;
    struct vldbentry vldbEntry;
    struct ftserver_status status;
    struct timeval tp;
    struct timezone tzp;
    char ch;
    void dumpAlloc(), dumpPush();

    /*
    add dump to database;
    add tape to database (including tape id );
    */

    LogTape("\n");
    LogTape("Starting pass %d\n", passNumber);

    dlqPtr =  dlqUnlinkf(&rockPtr->vols_toDump);

    /* while there are more volumes to dump */
    while ( dlqPtr != 0 )
    { /*w*/
	/* save in the current volume position */
	dlqLinkb(&rockPtr->vol_current, dlqPtr);
	curDump = (( struct volumeToDump * ) dlqPtr)->dumpDescPtr;

	/* set name of current volume being dumped */
	/* Make sure you follow the locking order, node before status queue */
	lock_ObtainWrite(&(nodePtr->lock));
	lock_ObtainWrite(&statusQueueLock);
	strcpy(nodePtr->statusNodePtr->volumeName, (char *)curDump->name);
	lock_ReleaseWrite(&statusQueueLock);
	lock_ReleaseWrite(&(nodePtr->lock));

	if ( checkAbort(nodePtr) ) {
	  code = TC_ABORTEDBYREQUEST;
	  goto error_exit;
	}

	/* Check if volume has moved,  and get new clone date */

	if (passNumber > 1) {

	  code = vldb_GetEntryByID(curDump->vid, -1, &vldbEntry, 1);
	  if (code) {

	    LogTape("Can't find fileset %s(%lu,,%lu) in FLDB: %s\n",
		    curDump->name, AFS_HGETBOTH(curDump->vid),
		    dfs_dceErrTxt(code));

	    fprintf(stderr,
		    "Can't find fileset %s(%lu,,%lu) in FLDB: %s\n",
		    curDump->name, AFS_HGETBOTH(curDump->vid),
		    dfs_dceErrTxt(code));

	    goto cont;
	  }

	  /* Search for that VolID */
	  for (ix = 0; ix < MAXVOLTYPES && !AFS_hiszero(vldbEntry.VolIDs[ix]); ++ix) {
	    if (AFS_hsame(curDump->vid, vldbEntry.VolIDs[ix])) {
	      volType = vldbEntry.VolTypes[ix];
	      break;
	    };
	  };

	  if (ix == MAXVOLTYPES || AFS_hiszero(vldbEntry.VolIDs[ix])) {
	    LogTape("Can't find fileset %s(%lu,,%lu) in FLDB\n",
		    curDump->name, AFS_HGETBOTH(curDump->vid));

	    fprintf(stderr, "Can't find fileset %s(%lu,,%lu) in FLDB\n",
		    curDump->name, AFS_HGETBOTH(curDump->vid));
	    goto cont;
	  };

	  mask  = ((unsigned32)VLSF_ZEROIXHERE) >> ix;

	  /* Search for that entry. Get first RO */
	  for (jx = 0; jx < vldbEntry.nServers; ++jx) {
	    if (vldbEntry.siteFlags[jx] & mask)  {
	      break;
	    }
	  };

	  if (jx == vldbEntry.nServers) {
	    LogTape("Can't find fileset %s(%lu,,%lu) in FLDB\n",
		    curDump->name, AFS_HGETBOTH(curDump->vid));

	    fprintf(stderr,
		    "Can't find fileset %s(%lu,,%lu) in FLDB\n",
		    curDump->name, AFS_HGETBOTH(curDump->vid));
	    goto cont;
	  }

	  /* Update address and partition */
	  curDump->hostAddr = vldbEntry.siteAddr[jx];
	  curDump->partition = vldbEntry.sitePartition[jx];

	  /* Update Clone date */

	  switch (volType) {

	  case VOLTIX_TO_VOLTYPE(RWVOL):
	    osi_gettimeofday(&tp, &tzp);
	    curDump->cloneDate = tp.tv_sec;
	    break;

	  default:
		    code = volc_Call(VC_VolumeStatus, (struct sockaddr *)&curDump->hostAddr,
				     NULL, curDump->partition, &curDump->vid, &status);

	    if (code)		/* Don't update date */
	      break;

	    curDump->cloneDate = status.vss.cloneTime.sec;
	    break;
	}
      }
      cont:
	rockPtr->curVolumeStatus = DUMP_FAILED;

	/* Initialize the state data for the volume dump. The state structure
 	 * is passed with each call to the pipe allocate and push routines.
	 * The error field in the state is used to return errors detected
	 * during the transfer.
	 */

	rockPtr->startVolume = 1;
	rockPtr->endVolume = 0;
        rockPtr->asyncP = NULL;
        rockPtr->elemP = NULL;
	rockPtr->chunkSize = 0;
	rockPtr->fragmentNumber = 1;
	pipeState.data= (char *)rockPtr;
	pipeState.error= 0;
        dataPipe.state = (rpc_ss_pipe_state_t)&pipeState;
	dataPipe.push  = (void (*)())dumpPush;
	dataPipe.pull  = 0;
	dataPipe.alloc = (void (*)())dumpAlloc;

	volId = curDump->vid;

	bzero((char *)&dumpDateDesc, sizeof(ftserver_Date));
	if (curDump->date) { /* incremental dump */
	  dumpDateDesc.mask = 1; /* from date with dirs */
	  dumpDateDesc.fromDate.sec = curDump->date;
	}
	code = volc_Call(VC_DumpVolume,
			 (struct sockaddr *)&(curDump->hostAddr),
			 (u_char *)NULL,
			 (char *)&volId,
			 (char *)curDump->partition,
			 (char *)&dumpDateDesc,
			 (char *)NULL,		/* file to dump */
			 (char *)&dataPipe,	/* pipe to dump */
			 sockFlag);

	/*
	 * Determine whether the dump succeeded or failed. There are three
	 * parameters by which we could decide this.
	 *	- returned code
	 *	- any pipe errors when calling push/pull routines
	 *	- curVolumeStatus field
	 * The dump succeeds only if all the above three indicate success. If
	 * there is clear evidence of failure, then we mark dump as failed. If
	 * both the above conditions don't match, then the dump is partial.
	 */
	if ( code == 0 && pipeState.error == 0 &&
	    rockPtr->curVolumeStatus == DUMP_SUCCESS)
	  action = DUMP_SUCCESS;
	else if ( code || pipeState.error ||
		 rockPtr->curVolumeStatus == DUMP_FAILED)
	  action = DUMP_FAILED;
	else
	  action = DUMP_PARTIAL;

	if ( checkAbort(nodePtr) ) {
	  code = TC_ABORTEDBYREQUEST;
	  goto error_exit;
	}
	switch ( action )
	{
	  case DUMP_SUCCESS:
	    dce_svc_printf(BAK_S_PASS_FILESET_SUCCESS_DUMP_MSG, passNumber,
			   curDump->name, AFS_HGETBOTH(curDump->vid));
	    LogTape("Pass %d: Fileset %s(%u,,%u) successfully dumped\n",
		    passNumber, curDump->name, AFS_HGETBOTH(curDump->vid));

	    dlqPtr = dlqUnlinkb(&rockPtr->vol_current);
	    if ( dlqPtr == 0 )
		ERROR_EXIT(-1);
	    dlqLinkb(&rockPtr->vols_done, dlqPtr);

	    rockPtr->tapeBytes += rockPtr->volBytes;
	    rockPtr->tapeKBytes += (rockPtr->tapeBytes/1024);
	    rockPtr->tapeBytes = rockPtr->tapeBytes % 1024;
	    rockPtr->volumesDone++;
	    break;

	  case DUMP_PARTIAL:
	    dce_svc_printf(BAK_S_PASS_FILESET_FAILED_MSG, passNumber,
			   curDump->name, AFS_HGETBOTH(curDump->vid),
			   dfs_dceErrTxt(code?code:pipeState.error));
	    LogTape(
		"Pass %d: Fileset %s(%lu,,%lu) failed; partially dumped; %s\n",
		passNumber, curDump->name, AFS_HGETBOTH(curDump->vid),
		dfs_dceErrTxt(code?code:pipeState.error));
	   LogError(
		"Pass %d: Fileset %s(%lu,,%lu) failed; partially dumped",
		(code?code:pipeState.error), passNumber, curDump->name,
		AFS_HGETBOTH(curDump->vid));


		if (passNumber == maxpass)
		{
		    if (!queryoperator)
		        ch = 'o';
		    else
		        ch = retryPrompt(curDump->name,
					 AFS_HGETBOTH(curDump->vid),
					 nodePtr->dumpID);

		    switch ( ch )
		    {
		       case 'r':                                    /* retry */
			   continue;
			case 'o':                                    /* omit */
			   LogTape("TaskId %u : Fileset %s (%lu,,%lu) omitted\n",
				   nodePtr->dumpID, curDump->name,
				   AFS_HGETBOTH(curDump->vid));
			   break;
			case 'a':                                    /* abort */
			   LogTape("TaskId %u : Dump of Fileset %s (%lu,,%lu) aborted\n",
				   nodePtr->dumpID, curDump->name,
				   AFS_HGETBOTH(curDump->vid));
			   code = TC_ABORTEDBYREQUEST;
			   goto error_exit;
			default:
			   code = TC_INTERNALERROR;
			   goto error_exit;
		    }
		  }

	    /* move volume to failed list */
	    dlqPtr = dlqUnlinkb(&rockPtr->vol_current);
	    if ( dlqPtr == 0 )
		ERROR_EXIT(-1);
	    dlqLinkb(&rockPtr->vols_failed, dlqPtr);
	    break;


	  case DUMP_FAILED:
	    dce_svc_printf(BAK_S_PASS_FILESET_FAILED_2_MSG,
			   passNumber, curDump->name,
			   AFS_HGETBOTH(curDump->vid),
			   dfs_dceErrTxt((code?code:pipeState.error)));
	    LogTape("Pass %d: Fileset %s(%u,,%u) failed; %s\n",
		    passNumber, curDump->name, AFS_HGETBOTH(curDump->vid),
		    dfs_dceErrTxt((code?code:pipeState.error)));
	    LogError("Pass %d: Fileset %s(%u,,%u) failed",
		     (code?code:pipeState.error), passNumber,
		     curDump->name, AFS_HGETBOTH(curDump->vid));

		if (passNumber == maxpass)
		{
		    if (!queryoperator)
		        ch = 'o';
		    else
		        ch = retryPrompt(curDump->name,
					 AFS_HGETBOTH(curDump->vid),
					 nodePtr->dumpID);

		    switch ( ch )
		    {
		       case 'r':                                    /* retry */
			   continue;
			case 'o':                                    /* omit */
			   LogTape("TaskId %u : Fileset %s (%lu,,%lu) omitted\n",
				   nodePtr->dumpID, curDump->name,
				   AFS_HGETBOTH(curDump->vid));
			   break;
			case 'a':                                    /* abort */
			   LogTape("TaskId %u : Dump of Fileset %s (%lu,,%lu) aborted\n",
				   nodePtr->dumpID, curDump->name,
				   AFS_HGETBOTH(curDump->vid));
			   code = TC_ABORTEDBYREQUEST;
			   goto error_exit;
			default:
			   code = TC_INTERNALERROR;
			   goto error_exit;
		    }
		  }

	    /* move volume to failed list */
	    dlqPtr = dlqUnlinkb(&rockPtr->vol_current);
	    if ( dlqPtr == 0 )
		ERROR_EXIT(-1);
	    dlqLinkb(&rockPtr->vols_failed, dlqPtr);
	    break;

	  default:
	    ERROR_EXIT(-1);
	    break;
	}

	dlqPtr =  dlqUnlinkf(&rockPtr->vols_toDump);
    } /*w*/

    /* print a summary of this pass */
    dce_svc_printf(BAK_S_END_OF_PASS_MSG, passNumber, dlqCount(&rockPtr->vols_failed));
    LogTape("End of pass %d: Filesets remaining = %d\n",
	    passNumber, dlqCount(&rockPtr->vols_failed));

error_exit:
    return(code);
}

/* allocate routine for pipe used to transfer fileset dumps */
void
dumpAlloc (state, bsize, buf, nbytes)
  rpc_ss_pipe_state_t state;
  ndr_ulong_int bsize;
  ndr_byte **buf;
  ndr_ulong_int *nbytes;
{
    struct ftsPipeState *statePtr = (struct ftsPipeState *)state;
    struct dumpRock *rockPtr = (struct dumpRock *)statePtr->data;
    static char dummybuf[4096];
    void dumpAsyncRtn();

    /*
     * initialize the async output queue
     */
    if ( rockPtr->asyncP == NULL && statePtr->error == 0 ) {
        rockPtr->asyncP = initAsyncQueue(DUMP_NBUFS, BUTM_BLOCKSIZE,
			                 (void (*)())dumpAsyncRtn, NULL,
					 (void *)state);
	if ( rockPtr->asyncP == NULL ) {
	    dfprintf((stdout, "dumpAlloc: problem with async queue\n"));
	    statePtr->error = EPIPE;
	}
    }
    
    /*
     * use the dummy buffer if there is a problem with the async
     * queue. We are obligated to drain the pipe.
     */

     if ( rockPtr->asyncP == NULL ) {
	*buf = (ndr_byte *)&dummybuf[0];
	*nbytes = 4096;
	if ( *nbytes > bsize ) *nbytes = bsize;
	return;
     }

     /* if we don't have a queue element, get one */
     if ( rockPtr->elemP == NULL ) {
	rockPtr->elemP = getAsyncElement(rockPtr->asyncP);
	if ( rockPtr->elemP == NULL ) {
	    dfprintf((stdout, "dumpAlloc: problem with async queue\n"));
	    termAsyncQueue(rockPtr->asyncP);
	    rockPtr->asyncP = NULL;
	    if ( !statePtr->error ) statePtr->error = EPIPE;
	    *buf = (ndr_byte *)&dummybuf[0];
	    *nbytes = 4096;
	    if ( *nbytes > bsize ) *nbytes = bsize;
	    return;
	}
    }

    /* the pipe writes directly into the queue element's buffer */
    *buf = (ndr_byte *)
	   (rockPtr->elemP->data) + BUTM_HDRSIZE + rockPtr->elemP->count;
    *nbytes = BUTM_DATASIZE - rockPtr->elemP->count;
    return;
}

/*
 * Push routine for pipe used to transfer dumps from the server
 */

static void
dumpPush (state, buf, bsize)
  rpc_ss_pipe_state_t state;
  char *buf;
  long bsize;
{
    struct ftsPipeState *statePtr = (struct ftsPipeState *)state;
    struct dumpRock *rockPtr = (struct dumpRock *)statePtr->data;

    /* if we don't have an async queue, do nothing */
    if ( rockPtr->asyncP == NULL ) return;

    if ( bsize == 0 ) {
        /* a zero length buffer indicates the end of the stream */
	rockPtr->elemP->last = 1;
	relAsyncElement(rockPtr->asyncP, rockPtr->elemP);
	termAsyncQueue(rockPtr->asyncP);
	rockPtr->asyncP = NULL;
	rockPtr->elemP = NULL;
	return;
    } else {
        /* update the current count and send the element if it is full */
        rockPtr->elemP->count += bsize;
        if ( rockPtr->elemP->count == BUTM_DATASIZE ) {
	    relAsyncElement(rockPtr->asyncP, rockPtr->elemP);
	    rockPtr->elemP = NULL;
        }
    }

    return;
}

/*
 * Routine to process queue elements off of the async queue
 */

static void
dumpAsyncRtn (elemP, argP, holdP)
  asyncElement_t *elemP;
  void *argP;
  int *holdP;
{
    struct ftsPipeState *statePtr = (struct ftsPipeState *)argP;
    struct dumpRock *rockPtr = (struct dumpRock *)statePtr->data;
    struct butm_tapeInfo *tapeInfoPtr = &rockPtr->tapeInfo;
    struct dumpNode *nodePtr = rockPtr->node;
    long volumeFlags;
    long code = 0;
    long kRemaining;
    struct volumeHeader hostVolumeHeader, netVolumeHeader;
    long bytesRead;
    extern long addVolume();

    /* ignore uninitialized queue elements */
    if ( elemP->count < 0 ) {
	dfprintf((stdout, "dumpAsyncRtn: ignoring uninitialized element\n"));
	elemP->count = 0;
	elemP->offset = 0;
	return;
    }

    if (statePtr->error) { /* this volume dump has aborted */
	dfprintf((stdout, "dumpAsyncRtn: ignoring volume because of error\n"));
	elemP->count = 0;
	elemP->offset = 0;
	return;
    }

    /* pull the data out of the queue element */
    bytesRead = elemP->count;
    elemP->count = 0;

    /* last flag is set on end of dump */
    if (elemP->last) {
	dfprintf((stdout, "dumpAsyncRtn: an empty buffer for write\n"));

	if (rockPtr->endVolume == 1) {
	    dfprintf((stdout, "dumpAsyncRtn: endVolume is already set\n"));
	    return;
	}
	else {
	    dfprintf((stdout,"dumpAsyncRtn: setting end volume\n"));
	    rockPtr->endVolume = 1;
	}
    }

    /* Initialize */
    if (rockPtr->startVolume) {
	dfprintf((stdout, "dumpAsyncRtn: start of a new fileset\n"));
	rockPtr->startVolume = 0;
	rockPtr->oldPosition = tapeInfoPtr->position;
        rockPtr->fragmentNumber = 1;
        rockPtr->volBytes = 0;

        /* set bytes dumped for bucoord */
	/* Make sure you follow the locking order, node before status queue */
	lock_ObtainWrite(&(nodePtr->lock));
	lock_ObtainWrite(&statusQueueLock);
        nodePtr->statusNodePtr->nKBytes = rockPtr->tapeKBytes;
	lock_ReleaseWrite(&statusQueueLock);
	lock_ReleaseWrite(&(nodePtr->lock));

	dfprintf((stdout, "dumpAsyncRtn: writing fileset header\n"));
        code = butm_WriteFileBegin(tapeInfoPtr);
        if ( code )
        {
	  LogTape("Task %u: Unable to write begin file mark on tape; %s\n",
		  nodePtr->dumpID, dfs_dceErrTxt(code));
	  LogError("Task %u: Unable to write begin file mark on tape %s",
		   code, nodePtr->dumpID, rockPtr->tapeName);
	  ERROR_EXIT(code);
        }

        makeVolumeHeader(&hostVolumeHeader, rockPtr, rockPtr->fragmentNumber);
        volumeHeader_hton(&hostVolumeHeader, &netVolumeHeader);

        code = butm_WriteFileDataCopy(tapeInfoPtr, &netVolumeHeader,
				      sizeof(netVolumeHeader));
        if ( code ) {
	  LogTape("Task %u: Unable to write fileset data on tape; %s\n",
		  nodePtr->dumpID, dfs_dceErrTxt(code));
	  LogError("Task %u: Unable to write fileset data on tape %s",
		   code, nodePtr->dumpID, rockPtr->tapeName);
	  ABORT_EXIT(code);
	}
        rockPtr->curVolumeStatus = DUMP_PARTIAL;
    }
    rockPtr->volBytes    += bytesRead;

    /* if we have a full buffer, write it to tape */
    if (bytesRead == BUTM_DATASIZE) {
	code = butm_WriteFileData(tapeInfoPtr, elemP->data, bytesRead);
        if ( code ) {
            LogTape("Task %u: Unable to write fileset data on tape; %s\n",
                    nodePtr->dumpID, dfs_dceErrTxt(code));
            LogError("Task %u: Unable to write fileset data on tape %s",
                     code, nodePtr->dumpID, rockPtr->tapeName);
            ABORT_EXIT(code);
        }
    }
    rockPtr->chunkSize += bytesRead;
    dfprintf((stdout, "dumpAsyncRtn: chunkSize %lu\n", rockPtr->chunkSize));
    if ( rockPtr->chunkSize > BIGCHUNK )
    {
	dfprintf((stdout, "dumpAsyncRtn: chunkSize %lu larger than BIGCHUNK",
		  rockPtr->chunkSize));
        rockPtr->chunkSize = 0;
        if ( checkAbort(nodePtr) ) {
	    dfprintf((stdout, "dumpAsyncRtn: aborted by request\n"));
	    ABORT_EXIT(TC_ABORTEDBYREQUEST);
	}
        /* set bytes dumped for bucoord */
	/* Make sure you follow the locking order, node before
	   status queue */
	lock_ObtainWrite(&(nodePtr->lock));
	lock_ObtainWrite(&statusQueueLock);
        nodePtr->statusNodePtr->nKBytes = rockPtr->tapeKBytes +
		                          rockPtr->volBytes/1024;
	lock_ReleaseWrite(&statusQueueLock);
	lock_ReleaseWrite(&(nodePtr->lock));
    }

    /* Check for remaining space in the tape. If not, get a new tape */
    kRemaining = butm_remainingKSpace(tapeInfoPtr);
    if( kRemaining < TC_KENDMARGIN ) {
	dfprintf((stdout, "dumpAsyncRtn: tape done, use new tape\n"));

	/* terminate tape */
	makeVolumeHeader(&hostVolumeHeader, rockPtr, rockPtr->fragmentNumber);

	/* modify for trailer format */
	hostVolumeHeader.contd = TC_VOLCONTD;
	hostVolumeHeader.magic = TC_VOLENDMAGIC;

	volumeHeader_hton(&hostVolumeHeader, &netVolumeHeader);

	/* write the trailer and terminate the tape */
	dfprintf((stdout,"dumpAsyncRtn: writing volume trailer, end of tape\n"));
        code = butm_WriteFileDataCopy(tapeInfoPtr, &netVolumeHeader,
				      sizeof(netVolumeHeader));
	if ( code ) {
	  LogTape("Task %u: Unable to write fileset data on tape; %s\n",
		  nodePtr->dumpID, dfs_dceErrTxt(code));
	  LogError("Task %u: Unable to write fileset data on tape %s",
		   code, nodePtr->dumpID, rockPtr->tapeName);
	  ABORT_EXIT(code);
	}
	code = butm_WriteFileEnd(tapeInfoPtr);
	if ( code ) {
	  LogTape("Task %u: Unable to write end file mark on tape; %s\n",
		  nodePtr->dumpID, dfs_dceErrTxt(code));
	  LogError("Task %u: Unable to write end file mark on tape %s",
		   code, nodePtr->dumpID, rockPtr->tapeName);
	  ERROR_EXIT(code);
	}

	/* add the volume to the database */
	volumeFlags = (rockPtr->fragmentNumber == 1)?BUDB_VOL_FIRSTFRAG:0;
	dfprintf((stdout, "dumpAsyncRtn: adding fileset to database\n"));
	/* addvolume logs failures to tape */
	code = addVolume(rockPtr, rockPtr->fragmentNumber, volumeFlags,
			 rockPtr->oldPosition);
	if ( code )
	  ERROR_EXIT(code);

	dfprintf((stdout, "dumpAsyncRtn: writing EOT and dismounting tape\n"));
	code = butm_WriteEOT(tapeInfoPtr);
	if ( code ) {
	  LogTape("Task %u: Unable to write end of tape mark on tape; %s\n",
		  nodePtr->dumpID, dfs_dceErrTxt(code));
	  LogError("Task %u: Unable to write end of tape mark on tape %s",
		   code, nodePtr->dumpID, rockPtr->tapeName);
	  ERROR_EXIT(code);
	}
	code = butm_Dismount(tapeInfoPtr);
	if ( code ) {
	  LogTape("Task %u: Unable to dismount tape %s; %s\n",
		  nodePtr->dumpID, rockPtr->tapeName, dfs_dceErrTxt(code));
	  LogError("Task %u: Unable to dismount tape %s",
		   code, nodePtr->dumpID, rockPtr->tapeName);
	  ERROR_EXIT(code);
	}
	/* this tape is done */
	rockPtr->tape.flags = BUDB_TAPE_WRITTEN;
	code = bcdb_FinishTape(&rockPtr->tape);
	if ( code ) {
	  LogTape("Task %u: Unable to add tape %s to dump %ld; %s\n",
		  rockPtr->tapeName,
		  nodePtr->dumpID, dfs_dceErrTxt(code));
	  LogTape("Task %u: Unable to add tape %s to dump %ld; %s\n",
		  nodePtr->dumpID, rockPtr->tapeName,
		  rockPtr->databaseDumpId, dfs_dceErrTxt(code));
	  LogError("Task %u: Unable to add tape %s to dump %ld; %s\n",
		   code, nodePtr->dumpID,
		  rockPtr->tapeName,
		  rockPtr->databaseDumpId, dfs_dceErrTxt(code));
	  ERROR_EXIT(code);
	}
	/* get the next tape, and add it into the database */
	rockPtr->tapeSeq++;
	code = getTape(rockPtr, 1);                 /* interactive */
	if ( checkAbort(nodePtr) ) {
	    dfprintf((stdout, "dumpAsyncRtn: aborted dump by request\n"));
	    ABORT_EXIT(TC_ABORTEDBYREQUEST);
	}
	if ( code )
	    ERROR_EXIT(code);

	dfprintf((stdout, "dumpAsyncRtn: writing header for new tape\n"));
	/* Get oldpos  position for this fragment */
	rockPtr->oldPosition = tapeInfoPtr->position;

	code = butm_WriteFileBegin(tapeInfoPtr);
	if ( code )
	{
	  LogTape("Task %u: Unable to write begin file mark on tape; %s\n",
		  nodePtr->dumpID, dfs_dceErrTxt(code));
	  LogError("Task %u: Unable to write begin file mark on tape %s",
		   code, nodePtr->dumpID, rockPtr->tapeName);
	  ERROR_EXIT(code);
	}

	/* add the volume header for the continuing volume dump */
	rockPtr->fragmentNumber++;
	makeVolumeHeader(&hostVolumeHeader, rockPtr, rockPtr->fragmentNumber);

	/* mark as continuation of a volume dump */
	hostVolumeHeader.contd = TC_VOLCONTD;
	volumeHeader_hton(&hostVolumeHeader, &netVolumeHeader);

        code = butm_WriteFileDataCopy(tapeInfoPtr, &netVolumeHeader,
				      sizeof(netVolumeHeader));
	if ( code ) {
	  LogTape("Task %u: Unable to write fileset data on tape; %s\n",
		  nodePtr->dumpID, dfs_dceErrTxt(code));
	  LogError("Task %u: Unable to write fileset data on tape %s",
		   code, nodePtr->dumpID, rockPtr->tapeName);
	  goto abort_exit;
	}

	/* clear the statistics for this new tape - clearing the
	 * volBytes is necessary if the database is going to reflect
	 * the actual number of bytes on tape. Further work is reqd. if
	 * the user wants to see the cumulative # of bytes dumped for
	 * the volume.
	 */

	rockPtr->tapeBytes = 0;
	rockPtr->tapeKBytes = 0;
	rockPtr->volBytes = 0;

	if ( checkAbort(nodePtr) ) {
	    dfprintf((stdout, "dumpAsyncRtn: aborted dump by request\n"));
	    ABORT_EXIT(TC_ABORTEDBYREQUEST);
	}
    }

    /* append a volume trailer and finish the dump */
    if (rockPtr->endVolume) {
	dfprintf((stdout, "dumpAsyncRtn: this volume over\n"));

	/* partial data buffer */
	makeVolumeHeader(&hostVolumeHeader, rockPtr, rockPtr->fragmentNumber);

	/* modify for trailer format */
	hostVolumeHeader.contd = 0;
	hostVolumeHeader.magic = TC_VOLENDMAGIC;
	hostVolumeHeader.endTime = time(0);

	volumeHeader_hton(&hostVolumeHeader, &netVolumeHeader);

	/* write the volume trailer */
	if (sizeof(netVolumeHeader) <= BUTM_DATASIZE - bytesRead)
	{
	    dfprintf((stdout,"dumpvolume: writing data and trailer\n"));
	    bcopy((char *)&netVolumeHeader,
		  elemP->data + BUTM_HDRSIZE + bytesRead,
		  sizeof(netVolumeHeader));
	    bytesRead += sizeof(netVolumeHeader);
	    dfprintf((stdout,"dumpAsyncRtn: writing last buffer, size %lu\n",
		      bytesRead));
	    code = butm_WriteFileData(tapeInfoPtr, elemP->data, bytesRead);
	    if ( code ) {
	      LogTape("Task %u: Unable to write fileset data on tape; %s\n",
		      nodePtr->dumpID, dfs_dceErrTxt(code));
	      LogError("Task %u: Unable to write fileset data on tape %s",
		       code, nodePtr->dumpID, rockPtr->tapeName);
	      ABORT_EXIT(code);
	    }
	}
	else
	{
	    dfprintf((stdout, "dumpAsyncRtn: volume trailer does not fit\n"));
	    bcopy ((char *)&netVolumeHeader,
		   elemP->data + BUTM_HDRSIZE + bytesRead,
		   BUTM_DATASIZE - bytesRead);
	    code = butm_WriteFileData(tapeInfoPtr, elemP->data, BUTM_DATASIZE);
	    if (code) {
	      LogTape("Task %u: Unable to write fileset data on tape; %s\n",
		      nodePtr->dumpID, dfs_dceErrTxt(code));
	      LogError("Task %u: Unable to write fileset data on tape %s",
		       code, nodePtr->dumpID, rockPtr->tapeName);
	      ABORT_EXIT(code);
	    }
	    dfprintf((stdout, "dumpAsyncRtn: writing rest of trailer\n"));
	    code = butm_WriteFileDataCopy(tapeInfoPtr,
					  ((char *)&netVolumeHeader) +
					  (BUTM_DATASIZE - bytesRead),
					  sizeof(netVolumeHeader) -
					  (BUTM_DATASIZE - bytesRead));
	    if (code) {
	      LogTape("Task %u: Unable to write fileset data on tape; %s\n",
		      nodePtr->dumpID, dfs_dceErrTxt(code));
	      LogError("Task %u: Unable to write fileset data on tape %s",
		      code, nodePtr->dumpID, rockPtr->tapeName);
	      ABORT_EXIT(code);
	    }
	}

	dfprintf((stdout, "dumpAsyncRtn: Done with dumping fileset\n"));
	code = butm_WriteFileEnd(tapeInfoPtr);
	if ( code ) {
	  LogTape("Task %u: Unable to write end file mark on tape; %s\n",
		  nodePtr->dumpID, dfs_dceErrTxt(code));
	  LogError("Task %u: Unable to write end file mark on tape %s",
		   code, nodePtr->dumpID, rockPtr->tapeName);
	  ERROR_EXIT(code);
	}

	/* dump complete, add volume to the database */
	volumeFlags = BUDB_VOL_LASTFRAG;
	if( rockPtr->fragmentNumber == 1)
	    volumeFlags |= BUDB_VOL_FIRSTFRAG;

	dfprintf((stdout, "dumpAsyncRtn: adding fileset to database\n"));
	code = addVolume(rockPtr, rockPtr->fragmentNumber, volumeFlags,
			 rockPtr->oldPosition);
	if (code)
	    ERROR_EXIT(code);
	rockPtr->curVolumeStatus = DUMP_SUCCESS;
    }

exit:
    return;

error_exit:
    dfprintf((stdout, "dumpAsyncRtn: aborting dump because of an error\n"));
    statePtr->error = code;
    goto exit;

abort_exit:
    /* this is used when an error occurs part way into a volume dump. Need
     * to clean up the tape state before returning
     */
    butm_WriteFileEnd(tapeInfoPtr);
    goto error_exit;
}

/* getTape
 *	iterate until the desired tape (as specified by the dump structures)
 *	is mounted.
 * entry:
 *	interactiveFlag
 *		0 - assume the tape is there. Prompt if assumption false
 *		1 - prompt regardless
 */

getTape(rockPtr, interactiveFlag)
     struct dumpRock *rockPtr;
     int interactiveFlag;
{
    struct dumpNode *nodePtr = rockPtr->node;
    struct butm_tapeInfo *tapeInfoPtr = &rockPtr->tapeInfo;
    char *tapeName = &rockPtr->tapeName[0];
    int tapecount = 1;

    struct butm_tapeLabel *oldTapeLabel, *newTapeLabel;

    int tapeIdRequired = 0;
    long code = 0;

    extern struct tapeConfig globalTapeConfig;
    extern struct udbHandleS udbHandle;
    extern long useTape();

    oldTapeLabel = (struct butm_tapeLabel *)osi_Alloc(sizeof(struct butm_tapeLabel));
    if (oldTapeLabel == (struct butm_tapeLabel *)NULL)
      return(TC_NOMEMORY);
    newTapeLabel = (struct butm_tapeLabel *)osi_Alloc(sizeof(struct butm_tapeLabel));
    if (newTapeLabel == (struct butm_tapeLabel *)NULL) {
      osi_Free(oldTapeLabel, sizeof(struct butm_tapeLabel));
      return(TC_NOMEMORY);
    }

    tc_MakeTapeName(tapeName, &nodePtr->tapeSetDesc, rockPtr->tapeSeq);
    bzero((char *)newTapeLabel,sizeof(struct butm_tapeLabel));
    GetNewLabel(tapeInfoPtr, tapeName, newTapeLabel);

start_getTape:

    bzero((char *)oldTapeLabel,sizeof(struct butm_tapeLabel));

    if ( interactiveFlag )
    {
	code = PromptForTape(WRITEOPCODE, tapeName, rockPtr->databaseDumpId, nodePtr->dumpID,
		      tapecount);

	if (code == TC_ABORTEDBYREQUEST)
	  goto error_exit;
	if (code)
	  ERROR_EXIT(code);

	/* check to see if we should abort */
	if ( checkAbort(nodePtr) )
	    ERROR_EXIT(TC_ABORTEDBYREQUEST);

	interactiveFlag = 1;
	tapecount++;

    }

    /* open the tape device */
    code = butm_Mount(tapeInfoPtr, tapeName);

    if ( code == BUTM_NOTAPE )
    {
	/* no tape loaded or drive offline */
	dce_svc_printf(BAK_S_NO_TAPE_MSG,globalTapeConfig.device);
	interactiveFlag = 1;
	goto start_getTape;
    }
    else if ( code )
    {
      LogTape("Task %u: Unable to mount tape %s; %s\n",
	      nodePtr->dumpID, tapeName, dfs_dceErrTxt(code));
      LogError("Task %u: Unable to mount tape %s",
	       code, nodePtr->dumpID, tapeName);
      ERROR_EXIT(code);
    }

    code = butm_ReadLabel(tapeInfoPtr, oldTapeLabel);

    if ( code )
    {
	/* could not read label.
	 * Either the tape was unlabelled (blank) or the drive is not
	 * accessible. Try to write a tape label. This will distinguish
	 * the cases.
	 */
	code = butm_Create(tapeInfoPtr, newTapeLabel);

	if ( code )
	{
	    /* tape really isn't available. Reset the device and ask
	     * for operator intervention
	     */
	    interactiveFlag = 1;

	    code = butm_Dismount(tapeInfoPtr);
	    if (code) {
	      LogTape("Task %u: Unable to dismount tape %s; %s\n",
		      nodePtr->dumpID, tapeName, dfs_dceErrTxt(code));
	      LogError("Task %u: Unable to dismount tape %s",
		       code, nodePtr->dumpID, tapeName);
	      ERROR_EXIT(code);
	    }
	    goto start_getTape;
	}
	else
	{
	    /* that wasn't really the tape label we wanted so let it
	     * be overwritten by the actual label
	     */
	    ResetTapeState(tapeInfoPtr, tapeName);
	}

	/* generate a tape id */
	tapeIdRequired = 1;
      }
    else
    {
      /* Check if the tape has expired before proceeding */
      struct timeval tp;
      struct timezone tzp;
      long curTime;
  
      osi_gettimeofday(&tp,&tzp);
      curTime = tp.tv_sec;
  
      if(curTime < oldTapeLabel->expirationDate)
      {
  	/* Tape has not expired, report that one can't use this tape
  	 * and that another should be mounted.
  	 */
          LogTape("Task %u: Cannot overwrite unexpired tape %s; expiration date %s\n",
  		nodePtr->dumpID, tapeName,
  		ctime((time_t *)&(oldTapeLabel->expirationDate)));
          LogError("Task %u: Cannot overwrite unexpired tape %s; expiration date %s",
  		 0, nodePtr->dumpID, tapeName,
  		 ctime((time_t *)&(oldTapeLabel->expirationDate)));
  
      	  dce_svc_printf(BAK_S_ERROR_TAPE_NOT_EXPIRED_MSG);
	  dce_svc_printf(BAK_S_THIS_TAPE_EXPIRES_MSG, tapeName,
          ctime((time_t *)&(oldTapeLabel->expirationDate)));
	  dce_svc_printf(BAK_S_PLEASE_TRY_AGAIN_MSG);
	  interactiveFlag = 1;
	  code = butm_Dismount(tapeInfoPtr);
	  if (code)
	      ERROR_EXIT(code);
	  goto start_getTape;
       }
       if (dump_namecheck)
        /* If label does not match */
         if ( (strcmp(oldTapeLabel->name, tapeName) != 0)
 	  &&   (strcmp(oldTapeLabel->name, "") != 0)
	    )
	 {
	    /* label does not match */
	    dce_svc_printf(BAK_S_UNACCEPTABLE_TAPE_LABEL_MSG, oldTapeLabel->name);
	    LogTape("Task %u: Unacceptable tape name %s; wanted %s\n",
			nodePtr->dumpID, oldTapeLabel->name, tapeName);
	    LogError("Task %u: Unacceptable tape name %s; wanted %s",
			0, nodePtr->dumpID, oldTapeLabel->name, tapeName);
	    interactiveFlag = 1;
	    code = butm_Dismount(tapeInfoPtr);
	    if (code)
		ERROR_EXIT(code);
	    goto start_getTape;
	 }
    }

    /* now have the right tape. Compute the expiration date */
    newTapeLabel->expirationDate =
		calcExpirationDate(nodePtr->tapeSetDesc.expType,
				   nodePtr->tapeSetDesc.expDate,
				   time(0));

    /* pick up information from the old label */
    if(oldTapeLabel->size)
	newTapeLabel->size = oldTapeLabel->size;
    else
	newTapeLabel->size = globalTapeConfig.capacity;

    newTapeLabel->dumpid = rockPtr->databaseDumpId;
    newTapeLabel->useCount = oldTapeLabel->useCount + 1;

    /* get dump's full pathname */
    strcpy(newTapeLabel->dumpPath, nodePtr->dumpName);

    /* write the label on the tape */
    code = butm_Create(tapeInfoPtr, newTapeLabel);

    if ( code == BUTM_READONLY )
    {
        LogTape("Task %u: Cannot overwrite tape %s; %s\n",
		nodePtr->dumpID, tapeName, dfs_dceErrTxt(BUTM_READONLY));
	LogError("Task %u: Cannot overwrite tape %s",
		 code, nodePtr->dumpID, tapeName);

	dce_svc_printf(BAK_S_TAPE_IS_WRITE_PROT_MSG, tapeName);
	dce_svc_printf(BAK_S_PLEASE_REMOVE_MSG);

	interactiveFlag = 1;
	code = butm_Dismount(tapeInfoPtr);
	if (code)
	    ERROR_EXIT(code);
	goto start_getTape;
    }
    else
    if ( code )
    {
        LogTape("Task %u: Cannot create label on tape %s; %s\n",
		nodePtr->dumpID, tapeName, dfs_dceErrTxt(code));
	LogError("Task %u: Cannot create label on tape %s",
		 code, nodePtr->dumpID, tapeName);
	/* reset Tape device */
	code = butm_Dismount(tapeInfoPtr);
	if (code)
	    ERROR_EXIT(code);
	interactiveFlag = 1;
	goto start_getTape;
    }

    /* delete old information from the database */
    if ( oldTapeLabel->structVersion >= TAPE_VERSION_1 )
    {
	if ( oldTapeLabel->dumpid )
	{

	  code = bcdb_deleteDumpById(oldTapeLabel->dumpid);


	    /* diagnostic
	       if ( code )
	       com_err("", code, "delete dump returned code\n");
	       */

	    if (code && (code != BUDB_NOENT)) {
	      LogTape("Task %u: Unable to delete old DB entries; %s\n",
		      nodePtr->dumpID, dfs_dceErrTxt(code));
	      LogError("Task %u: Unable to delete old DB entries",
		       code, nodePtr->dumpID);
	      }
	}
    }

    code = useTape(rockPtr, newTapeLabel);
    if (code) {
      LogTape("Task %u: Unable to use tape %s; %s\n",
	      nodePtr->dumpID, rockPtr->tapeName, dfs_dceErrTxt(code));
      LogError("Task %u: Unable to use tape %s",
	       code, nodePtr->dumpID, rockPtr->tapeName);
    }
error_exit:
    osi_Free(oldTapeLabel, sizeof(struct butm_tapeLabel));
    osi_Free(newTapeLabel, sizeof(struct butm_tapeLabel));
    return(code);
}

makeVolumeHeader(vhptr, rockPtr, fragmentNumber)
     struct volumeHeader *vhptr;
     struct dumpRock *rockPtr;
     int fragmentNumber;
{
    struct dumpNode *nodePtr = rockPtr->node;
    struct tc_dumpDesc *curDump;

    curDump = ( (struct volumeToDump *)
	       		dlqFront(&rockPtr->vol_current))->dumpDescPtr;

    bzero((char *)vhptr, sizeof(*vhptr));
    strcpy(vhptr->volumeName, (char *)curDump->name);
    DFSH_NetFromMemHyper(vhptr->volumeID, curDump->vid);
    vhptr->server = curDump->hostAddr;	/* structure assignment */
    vhptr->part = curDump->partition;
    vhptr->from = curDump->date;
    vhptr->frag = fragmentNumber;
    vhptr->contd = 0;
    vhptr->magic = TC_VOLBEGINMAGIC;
    vhptr->dumpID = rockPtr->databaseDumpId;	/* real dump id */
    vhptr->level = nodePtr->level;
    vhptr->parentID = nodePtr->parent;
    vhptr->endTime = 0;
    vhptr->versionflags = CUR_TAPE_VERSION;

    vhptr->cloneDate = nodePtr->dumps->cloneDate;
    strcpy(vhptr->dumpSetName, nodePtr->dumpSetName);
    strcpy(vhptr->preamble,"H++NAME#");
    strcpy(vhptr->postamble,"T--NAME#");
}

volumeHeader_hton(hostPtr, netPtr)
     struct volumeHeader *hostPtr, *netPtr;
{
    /* sizes in K and fields in network order */

    strcpy(netPtr->preamble, hostPtr->preamble);
    strcpy(netPtr->postamble, hostPtr->postamble);
    strcpy(netPtr->volumeName, hostPtr->volumeName);
    netPtr->volumeID = hostPtr->volumeID; /* volumeID already in net order */
    /* server is afsNetAddr and should be converted to network order -- Is
       there a generic routine to hton a afsNetAddr structure? */
    netPtr->server = hostPtr->server;	/* structure assignment */
    netPtr->server.type = htons(netPtr->server.type);
    netPtr->part = htonl(hostPtr->part);
    netPtr->from = htonl(hostPtr->from);
    netPtr->frag = htonl(hostPtr->frag);
    netPtr->magic = htonl(hostPtr->magic);
    netPtr->contd = htonl(hostPtr->contd);
    strcpy(netPtr->dumpSetName, hostPtr->dumpSetName);
    netPtr->dumpID = htonl(hostPtr->dumpID);
    netPtr->level = htonl(hostPtr->level);
    netPtr->parentID = htonl(hostPtr->parentID);
    netPtr->endTime = htonl(hostPtr->endTime);
    netPtr->versionflags = htonl(hostPtr->versionflags);
    netPtr->cloneDate = htonl(hostPtr->cloneDate);
}

int
checkAbort(nodePtr)
     struct dumpNode *nodePtr;
{
    statusP statusPtr;
    u_long taskId;
    int retval = 0;

    extern statusP findStatus();

    /* check if we should abort */
    lock_ObtainRead(&(nodePtr->lock));
    taskId = nodePtr->dumpID;
    lock_ObtainRead(&statusQueueLock);
    statusPtr = findStatus(taskId);
    if ( statusPtr )
    {
	retval = statusPtr->flags & ABORT_REQUEST;
    }
    lock_ReleaseRead(&statusQueueLock);
    lock_ReleaseRead(&(nodePtr->lock));
    return(retval);
}

abortDump(nodePtr)
     struct dumpNode *nodePtr;
{
    clearStatus(nodePtr->dumpID, ABORT_REQUEST);
    setStatus(nodePtr->dumpID, ABORT_DONE);
    lock_ObtainWrite(&(nodePtr->lock));
    nodePtr->status |= ABORTED;
    nodePtr->status &= ~INPROGRESS;
    /* do any cleaning up */
    osi_Wakeup((caddr_t)&(nodePtr->lock));
    lock_ReleaseWrite(&(nodePtr->lock));
    return(0);
}

/* database related routines */

long
createDump(rockPtr)
     struct dumpRock *rockPtr;
{
    struct dumpNode *nodePtr = rockPtr->node;
    struct budb_dumpEntry *dumpPtr;
    long code = 0;

    dumpPtr = &rockPtr->dump;

    bzero((char *)dumpPtr, sizeof(*dumpPtr));

    /* id filled in by database */
    dumpPtr->parent = nodePtr->parent;
    dumpPtr->level = nodePtr->level;
    /* no flags */
    strcpy((char *)dumpPtr->volumeSetName, nodePtr->volumeSetName);
    strcpy((char *)dumpPtr->dumpPath, nodePtr->dumpName);
    strcpy((char *)dumpPtr->name, nodePtr->dumpSetName);
    dumpPtr->created = time(0);
    dumpPtr->incTime = 0;	                    /* not really used */
    dumpPtr->nVolumes = 0;

    dumpPtr->tapes.a = 1;
    dumpPtr->tapes.b = 1;
    dumpPtr->tapes.maxTapes = 0;
    strcpy((char *)dumpPtr->tapes.format, (char *)nodePtr->tapeSetDesc.format);

    /* principal filled in by database */

    /* now call the database to create the entry */
    code = bcdb_CreateDump(dumpPtr);

    if ( code == 0 )
    {
	/* save the generated dump id */
	rockPtr->databaseDumpId = dumpPtr->id;
    }
    else {
      LogTape("Task %lu: failed to create dump %s in database; %s\n",
	      nodePtr->dumpID, dumpPtr->name, dfs_dceErrTxt(code));
      LogError("Task %lu: failed to create dump %s in database",
	      code, nodePtr->dumpID, dumpPtr->name);
    }
    return(code);
}

long
useTape(rockPtr, labelPtr)
     struct dumpRock *rockPtr;
     struct butm_tapeLabel *labelPtr;
{
    struct budb_tapeEntry *tapePtr;
    long new;
    long code = 0;

    tapePtr = &rockPtr->tape;
    bzero((char *)tapePtr, sizeof(*tapePtr));

    strcpy((char *)tapePtr->name, rockPtr->tapeName);
    /* this flag should be set at the server, not at both ends */
    /* tapePtr->flags = BUDB_TAPE_BEINGWRITTEN; */
    tapePtr->written = time(0);

    tapePtr->expires = labelPtr->expirationDate;
    /* no bytes */
    /* no files */
    /* no volumes */
    tapePtr->seq = rockPtr->tapeSeq + 1;
    /* no tape id */
    tapePtr->useCount = labelPtr->useCount;
    tapePtr->dump = rockPtr->databaseDumpId;

    code = bcdb_UseTape(tapePtr, &new);
    return(code);
}


long
addVolume(rockPtr, fragmentNumber, flags, oldpos)
     struct dumpRock *rockPtr;
     int fragmentNumber;
     long flags;
     int oldpos;
{
    struct dumpNode *nodePtr = rockPtr->node;
    struct budb_volumeEntry volume;
    struct tc_dumpDesc *curDump;
    long code = 0;

    curDump = ( (struct volumeToDump *)
	       		dlqFront(&rockPtr->vol_current))->dumpDescPtr;

    bzero((char *)&volume, sizeof(volume));

    strcpy((char *)volume.name, (char *)curDump->name);
    volume.flags = flags;
    volume.id = curDump->vid;
    /* no server */
    /* no partition */
    /* nFrags presumably generated by db */

    /* adjust position since we've already written the fileEnd marker
     * that results in position increment
     */
    if (isafile)
      volume.position = oldpos;
    else
      volume.position = rockPtr->tapeInfo.position - 1;

    volume.clone = curDump->cloneDate;

    /* no incTime */
    /* no startByte */
    volume.nBytes = rockPtr->volBytes;
    volume.seq = fragmentNumber - 1;

    volume.dump = rockPtr->databaseDumpId;
    strcpy((char *)volume.tape, rockPtr->tapeName);

    code = bcdb_AddVolume(&volume);
    if (code) {
      LogTape("Task %u: Unable to add fileset %s(ld,,%ld) to dump %ld; %s\n",
	      volume.name, AFS_HGETBOTH(volume.id),
	      nodePtr->dumpID, dfs_dceErrTxt(code));
      LogError("Task %u: Unable to add fileset %s(ld,,%ld) to dump %ld", code,
	      volume.name, AFS_HGETBOTH(volume.id), nodePtr->dumpID);
    }
    return(code);
}

/* debug */

printvtd(ptr)
     dlqlinkP ptr;
{
    struct volumeToDump *vtdPtr;

    vtdPtr = (struct volumeToDump *) ptr;
    printf("%s\n", vtdPtr->dumpDescPtr->name);

}
