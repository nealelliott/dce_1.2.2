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
 * lwps.c -- restore fileset and disk
 *
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */

#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

#include <sys/wait.h>
#include <errno.h>

#include <dcedfs/ktime.h>				/* PA */
#include <dcedfs/aggr.h>
#include <dcedfs/common_data.h>
#include <dcedfs/compat.h>
#include <dcedfs/lock.h>
#include <dcedfs/fldb_data.h>
#include <dcedfs/flserver.h>
#include <dcedfs/volc.h>
#include <dcedfs/tcdata.h>
#include <dcedfs/bubasics.h>				/* PA */
#include <dcedfs/budb_errs.h>
#include <dcedfs/async_queue.h>
#include <com_err.h>
#include "error_macros.h"

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/butc/RCS/lwps.c,v 10.42 1996/05/31 00:24:16 hyer Exp $")

#ifdef AFS_AIX31_ENV
/*
 * In AIX, the  sys/m_wait.h redefines these, so we remove
 * that definition  and get the definition we want back.
 */
#undef WIFEXITED
#undef WEXITSTATUS
#define	WIFEXITED(stat)  	((int)((stat)&0xFF) == 0)
#define	WEXITSTATUS(stat) 	((int)(((stat)>>8)&0xFF))
#endif /* AFS_AIX31_ENV */


extern int errno;

/* GLOBAL CONFIGURATION PARAMETERS */
extern int tapeblocks;
extern int dump_namecheck;
extern int queryoperator;
extern int tapemounted;
extern char *opencallout;
extern char *closecallout;
extern int autoQuery;
extern int sockFlag;

/* notes
 *	Need to re-write to:
 *	1) non-interactive tape handling (optional)
 *	2) compute tape and volume sizes for the database
 *	3) compute and use tape id's for tape tracking (put on tape label)
 *	4) status management
 */


#define	LABEL_CHECKS		1
#define RESTORE_NBUFS		64

/* Abort checks are done after each  BIGCHUNK of data transfer */
#define	BIGCHUNK (512*1024)

#define SetError(no,fl,er,la)				\
{							\
        u_long taskId;					\
        lock_ObtainRead(&((no)->lock));			\
	taskId = (no)->dumpID;				\
        lock_ReleaseRead(&((no)->lock));		\
	setStatus(taskId, fl);				\
	error = er;					\
	goto la;					\
}

#define SetAndReturn(no,fl,er,dev,resP)			\
{							\
        u_long taskId;					\
        lock_ObtainRead(&((no)->lock));			\
	taskId = (no)->dumpID;				\
        lock_ReleaseRead(&((no)->lock));		\
	setStatus(taskId, fl);				\
	LeaveDeviceQueue(dev);				\
        LogError("task %u: Unable to restore fileset %lu,,%lu", \
		 er, taskId, AFS_HGETBOTH(resP->vid)); \
        dce_svc_printf(BAK_S_UNABLE_TO_RESTORE_FILESET_MSG, \
		       taskId, AFS_HGETBOTH(resP->vid), \
		       dfs_dceErrTxt(er));			\
	return er;					\
}

#ifdef notdef
#define dfprintf(x) { fprintf x ; fflush(stdout); }
#else
#define dfprintf(x)
#endif

char *whoami = "butc";

/* each volume is treated as a file of data. This file is
 * preceeded by a header and ended with a trailer. These are
 * stripped before being passed to the ftserver.
 */

extern int debugLevel;
extern void LogError();
extern struct tapeConfig globalTapeConfig;
extern struct deviceSyncNode *deviceLatch;
extern char *globalCellName;		/* cell name from which we get data */
extern osi_dlock_t statusQueueLock;
struct timeval tp;
struct timezone tzp;
int ExtractTrailer();

/* first proc called by anybody who intends to use the device */
void EnterDeviceQueue(devLatch)
struct deviceSyncNode *devLatch;
{
  lock_ObtainWrite(&(devLatch->deviceLock));
  devLatch->flags = TC_DEVICEINUSE;
}

/* last proc called by anybody finishing using the device */
void LeaveDeviceQueue(devLatch)
struct deviceSyncNode *devLatch;
{
  devLatch->flags = 0;
  lock_ReleaseWrite(&(devLatch->deviceLock));
}

/*
 * State data for fileset restores. There are two types of state
 * information. The restore state data tracks the progress of the
 * fileset restore. The pipe state data tracks the progress of
 * the transfer of fileset data to the server.
 *
 * Three buffers are used to read the dump from tape. We use a three
 * buffer window on the tape to efficiently search for the
 * end of each fileset. We read data until hit the FILE_END mark.
 * Once we reach the FILE_END mark, the two buffers preceeding the
 * FILE_END mark contain the volume trailer. We need to
 * check two previous buffers in case the volume trailer spans
 * block boundaries. As long as we haven't read the FILE_END mark,
 * we know the oldest buffer in the three buffer window does not
 * contain the volume trailer, so it is safe to send it to the server.
 */

struct restoreRock {

    /* restore state data */

    char TapeName[TC_MAXTAPELEN];
    char VolName[TC_MAXNAMELEN];
    char OldVolName[TC_MAXNAMELEN];
    afsNetAddr Server;
    unsigned long Part, FromDate, Magic;
    unsigned long Frag;
    afs_hyper_t Volid, OrigVid;
    unsigned long DumpID,ParentID,EndTime;
    struct tc_dumpDesc *Dump;
    struct tc_restoreDesc *Restore;
    struct butm_tapeInfo *TapeInfo;
    struct dumpNode *Node;

    /* pipe state data */

    asyncQueue_t *asyncP;	/* async buffer queue */
    asyncElement_t *curElemP;	/* current async queue element */
    asyncElement_t *nextElemP;	/* next async queue element */
    asyncElement_t *lastElemP;	/* last async queue element */
    unsigned long chunkSize;	/* bytes written since abort check */
    unsigned long residBytes;	/* residual bytes for KB calculation */
    int newTape;		/* new tape mounted */
    int needNewTape;		/* need new tape mounted */
};

#define BELLTIME 60 	/* 60 seconds before a bell rings */
#define BELLCHAR 7 	/* ascii for bell */

/* wait for FILE *afile to have at least one byte of input available, either
   in buffer or in the associated file descriptor.  Return after BELLTIME
   seconds with error code 2 if no input by then; otherwise, return 0 if
   there is input available */

FWait(afile, awaitTime)
long awaitTime;
FILE *afile;
{ /*FWait*/

    register long code;
    long in, out, except;
    struct timeval tp;

    tp.tv_sec = awaitTime;
    tp.tv_usec = 0;

    while (1) {
	if (afile->_cnt > 0)
	    break;
	in = (1<<(fileno(afile)));
	out = except = 0;
	code = select (32, (fd_set *)&in, (fd_set *)&out, (fd_set *)&except, &tp);
	if (code > 0)
	    break;
	if(code == 0){			/* timed out on select */
	    return 2;
	}
    }
    return(0);

} /*FWait*/

static long FInputExists(afile)
    FILE *afile;

{
    register long code;
    long in, out, except;
    struct timeval tp;

    tp.tv_sec = 0;
    tp.tv_usec = 0;

    if (afile->_cnt > 0)
	return 1;
    in = (1<<(fileno(afile)));
    out = except = 0;
    code = select(32, (fd_set *)&in, (fd_set *)&out, (fd_set *)&except, &tp);
    if (code == 1) return 1;
    return 0;


}

/* FFlushInput
 *	flush all input
 * notes:
 *	only external clients are in recoverDb.c. Was static. PA
 */
FFlushInput(afile)
FILE *afile;
{
    while(1){
	if(FInputExists(afile))
	    getchar();
	else return;
    }

}

/* display the <node> information, if <flag> is 1, associated dump
 * is proceeding normally, else it is being aborted on request
 */
void ShowDumpNode(flag,node)
long flag;
struct dumpNode *node;
{
    if(!debugLevel)
	return;
    osi_gettimeofday(&tp,&tzp);
    dce_svc_printf(BAK_S_STATUS_HEADER_MSG);
    printf("%s",ctime((long *)&(tp.tv_sec)));
    dce_svc_printf(BAK_S_TAPE_COORD_ACTIVE_MSG);
    if(flag)
	dce_svc_printf(BAK_S_CONTINUE_DUMP_RESTORE_MSG);
    else
	dce_svc_printf(BAK_S_ABORTING_DUMP_RESTORE_ON_REQUEST_MSG);
    dce_svc_printf(BAK_S_DUMP_CREATED_MSG,node->dumpID,ctime(&(node->creationTime)));
    if(node->opcode & DUMP) {
	dce_svc_printf(BAK_S_OPCODE_DUMP_MSG);
	dce_svc_printf(BAK_S_DUMPSET_PARENT_TIME_LEVEL_MSG,node->dumpSetName,node->parent,node->level);
	dce_svc_printf(BAK_S_FILESET_VOLUME_MSG,node->volumeBeingDumped);
    }
    else dce_svc_printf(BAK_S_OPCODE_RESTORE_MSG);
    dce_svc_printf(BAK_S_BYTES_TRANSFERED_MSG,node->bytesDumped);
    printf("------------\n");
    fflush(stdout);
    return;
}


/* check to see if anyone has requested an abort */
int
ShouldAbort(newNode, residBytes)
     struct dumpNode *newNode;
     long residBytes;
{
    statusP statusPtr;
    u_long taskId;
    int abortFlag = 0;

    extern statusP findStatus();
    lock_ObtainWrite(&(newNode->lock));
    taskId = newNode->dumpID;
    lock_ObtainShared(&statusQueueLock);
    statusPtr = findStatus(taskId);
    if ( statusPtr != 0 )
    {
        lock_UpgradeSToW(&statusQueueLock);
        statusPtr->nKBytes += residBytes / 1024;
	abortFlag = (statusPtr->flags & ABORT_REQUEST);
	if ( abortFlag )
	{
	    statusPtr->flags &= ~ABORT_REQUEST;
	    statusPtr->flags |= ABORT_DONE;
	}
	lock_ReleaseWrite(&statusQueueLock);
    }
    else {
	com_err("Restorer", 0, "Unable to locate status node\n");
	lock_ReleaseShared(&statusQueueLock);
    }

    if ( abortFlag == 0 )
    {
	newNode->status |= INPROGRESS;
	ShowDumpNode(1,newNode);
        lock_ReleaseWrite(&(newNode->lock));
	return 0; /*dont abort */
    }
    else
    {
	/* someone has signalled me to abort */
	LogError("Abort of task ID %d requested\n", 0, newNode->dumpID);
	newNode->status |= ABORTED;
	newNode->status &= ~INPROGRESS;
	ShowDumpNode(0,newNode);
        lock_ReleaseWrite(&(newNode->lock));
	return 1;
    }
}


/* verify that <tapeName> matches that on <tapeLabel> */
long CheckLabel(tapeLabel,tapeName,dumpID)
struct butm_tapeLabel *tapeLabel;
char *tapeName;
long dumpID;
{
	/*should this check for expiration ?*/
	if ((!strcmp(tapeLabel->name,tapeName)) &&
	    /* Either there is no dumpID (we are running an older version
	     * of bak) or the dump ids match.
	     */
	    ((dumpID == 0) ||
	     (tapeLabel->dumpid == dumpID)))
		return 0;	/* label matches tapename */
	else
	    return 1;	/* does not match */
}

/* prompt the operator to see if we want to continue with the
 * rest of the restores, even thought the current restore on
 * volume <volname> seems to have failed due to a tape module
 * error */
static int ShouldContinue(volname)
char *volname;
{
    register long code;
    long start;
    int inchr;

    while (1) {
	FFlushInput(stdin);
	putchar(BELLCHAR);
	dce_svc_printf(BAK_S_RESTORE_TAPE_ERROR_MSG,volname);
	dce_svc_printf(BAK_S_DO_YOU_WANT_TO_CONTINUE_MSG);
	fflush(stdout);
	start = time(0);
	while(1) {
	    code = FWait(stdin, 5); /* check every 5 seconds */
	    if (code == 0) break;	/* input is available */
	    if (time(0) > start + BELLTIME) break;
	}
	/* otherwise, we should print the prompt out again, check for abort
	   and go back, since the FWait timed out */
	if (code == 0) break;	/* input is available */
    }
    inchr = getchar();
    if(inchr == 'y' || inchr == 'Y'){
	dce_svc_printf(BAK_S_THANKS_WILL_TRY_MSG);
	return 1;
    }
    else{
	dce_svc_printf(BAK_S_ABANDONING_THE_RESTORE_MSG);
	return 0;
    }
}



/*
 * UnmountTape
 *     Unmounts a tape and prints a warning if it can't unmount it.
 *     Regardless of error, the closecallout routine will be called
 *     (unless a tape is not mounted in the first place).
 */
UnmountTape(taskId, tapeInfoPtr)
    int32 taskId;
    struct butm_tapeInfo  *tapeInfoPtr;
{
    int32 code;
    int32          wcode;
    int cpid, status, rcpid, inchr;

    code = butm_Dismount(tapeInfoPtr);
    if (code && (code !=  BUTM_NOMOUNT))
      LogError("Warning: Can't close tape\n");

    if (tapemounted && closecallout)
    {
        setStatus(taskId, CALL_WAIT);

        cpid = CallOutRoutine(taskId, globalTapeConfig.device, CLOSEOPCODE, "", 0, 1);
	while (cpid)         /* Wait until return */
	{
	    status = 0;
	    rcpid = waitpid(cpid, &status, WNOHANG);
	    if (rcpid != 0)
	    {
	        tapemounted = 0;
		break;
	    }

	    if ( checkAbortByTaskId(taskId) )
	    {
		LogTape("Task %u : Callout routine has been aborted\n", taskId);
		fprintf(stderr, "Callout routine has been aborted\n");
	        if (kill(cpid, SIGKILL))	/* Cancel callout */
		    LogError("Kill of callout process %d failed\n", cpid);
		break;
	    }
	}
    }
    clearStatus(taskId, CALL_WAIT);
}


int CallOutRoutine(taskId, tapePath, flag, name, dbDumpId, tapecount)
   int32  taskId;
   char   *tapePath;
   int    flag;
   char   *name;
   u_int32 dbDumpId;
   int tapecount;
{
  int32 count;

  int32 code = 0;
  int  pid;

  char StapePath[256];
  char ScallOut[256];
  char Scount[10];
  char Sopcode[10];
  char Sdumpid[15];
  char Stape[40];
  char *callOut;

  char *CO_argv[10];
  char *CO_envp[1];

  pid = fork();
  if (pid < 0)
    {
      LogError("Can't call to outside routine\n");
      pid = 0;
    }
  else if (pid == 0)
    {
      callOut = opencallout;

      if (!callOut) exit(2);

      switch ( flag )
	{
	case READOPCODE:
	  strcpy(Sopcode, "restore");
	  break;
	case APPENDOPCODE:
	  strcpy(Sopcode, "appenddump");
	  break;
	case WRITEOPCODE:
	  strcpy(Sopcode, "dump");
	  break;
	case LABELOPCODE:
	  strcpy(Sopcode, "labeltape");
	  break;
	case READLABELOPCODE:
	  strcpy(Sopcode, "readlabel");
	  break;
	case SCANOPCODE:
	  strcpy(Sopcode, "scantape");
	  break;
	case RESTOREDBOPCODE:
	  strcpy(Sopcode, "restoredb");
	  break;
	case SAVEDBOPCODE:
	  strcpy(Sopcode, "savedb");
	  break;
	case CLOSEOPCODE:
	  strcpy(Sopcode, "unmount");
	  callOut = closecallout;
	  break;
	default:
	  strcpy(Sopcode, "unknown");
	  break;
	}

      strcpy(ScallOut, callOut);
      CO_argv[0] = ScallOut;

      strcpy(StapePath, tapePath);
      CO_argv[1] = StapePath;

      CO_argv[2] = Sopcode;

      if (flag == CLOSEOPCODE)
	{
	  CO_argv[3] = (char *)0;
	}
      else
	{
	  sprintf(Scount, "%d", tapecount);
	  CO_argv[3] = Scount;

	  /* The tape label name - special case labeltape */
	  if ( !name || (strcmp(name,"") == 0) )
	    strcpy(Stape, "none");
	  else
	    strcpy(Stape, name);
	  CO_argv[4] = Stape;

	  /* The tape id */
	  if (!dbDumpId)
	    strcpy(Sdumpid, "none");
	  else
	    sprintf(Sdumpid, "%u", dbDumpId);
	  CO_argv[5] = Sdumpid;

	  CO_argv[6] = (char *)0;
	}

      CO_envp[0] = (char *)0;

      /* Should not return */
      code = execve(callOut, CO_argv, CO_envp);
      LogError("Call to %s outside routine %s failed\n",
	       Sopcode, callOut, errno);
      exit(2);
    }

  return(pid);
}

/* PrintPrompt
 *	print out prompt to operator
 * calledby:
 *	PromptForTape only.
 */

void static
PrintPrompt(flag, name, dumpID)
int flag;
char *name;
{
    dce_svc_printf(BAK_S_OPERATOR_ATTENTION_MSG);
    dce_svc_printf(BAK_S_DEVICE_NAME_MSG,globalTapeConfig.device);

    switch ( flag )
    {
      case READOPCODE:
      case RESTOREDBOPCODE:
	/* mount for reading */
	if (dumpID == 0)
	    dce_svc_printf(BAK_S_TAKE_OUT_TAPE_MSG, name);
	else
	    dce_svc_printf(BAK_S_TAKE_OUT_TAPE_AND_REPLACE_FOR_ID_MSG, name, dumpID);
	break;

      case WRITEOPCODE:
      case SAVEDBOPCODE:
	/* mount for writing */
        dce_svc_printf(BAK_S_TAKE_OUT_TAPE_AND_REPLACE_WRITE_MSG, name);
	break;

      case LABELOPCODE:
	/* mount for labelling */
	dce_printf(bak_s_tape_to_be_labelled);
	if(strcmp(name,""))
		dce_svc_printf(BAK_S_REPLACE_WRITE_MSG,name);
	else
		printf(" <NULL> \n");
	break;

      case READLABELOPCODE:
	/* mount for labelling */
	dce_svc_printf(BAK_S_TAPE_TO_BE_READ_LABEL_MSG);
	break;

      case SCANOPCODE:
	if ( strcmp(name, "") == 0 )
		dce_svc_printf(BAK_S_TAPE_TO_BE_SCANNED_MSG);
	else
		dce_svc_printf(BAK_S_PUT_IN_TAPE_FOR_SANNING_MSG, name);
	break;

      default:
	break;
    }
    dce_svc_printf(BAK_S_HIT_RETURN_WHEN_DONE_MSG);
}

/* PromptForTape
 *	prompt the operator to change the tape. if flag == 0, tape <name>
 *	is to be mounted for reading,if 1, for writing, else for labelling
 * notes:
 *	only external clients are in recoverDb.c. Was static PA
 *      dump.c also has a call to this.  (Dave Carr)
 */
int PromptForTape(flag, name, dumpID, taskId, tapecount)
     int flag;
     char *name;
     long dumpID;
     u_long taskId;
     int tapecount;
{
     register long code = 0;
     long start;
     int           CallOut;
     int           cpid, status, rcpid;
     int           wcode;

     CallOut = (opencallout ? 1 : 0);

     if (CallOut) {

	  status  = 0;
	  setStatus(taskId, CALL_WAIT);

	  cpid = CallOutRoutine(taskId, globalTapeConfig.device, flag, name,
				dumpID, tapecount);
	  if (cpid == 0) {
	       CallOut = 0;
	       goto forw;	/* prompt at screen */
	  }

	  rcpid = waitpid(cpid, &status, 0);

	  if ( checkAbortByTaskId(taskId) )
	  {

	       dce_svc_printf(BAK_S_ABORTED_BY_COORDINATOR_MSG);
	       if (kill(cpid, SIGKILL)) /* Cancel callout */
		    LogError("PromptforTape : Task Id %u : Kill of callout process %d failed\n",
			     taskId, cpid);

	       ERROR_EXIT(TC_ABORTEDBYREQUEST);

	  }



	  if  (rcpid != cpid) wcode = -1;
	  else
	       if (!WIFEXITED(status))
		    wcode = -1;
	       else
		    wcode = WEXITSTATUS(status);


	  if (wcode == 0)	/* Succesful exit */
	       goto forw;
	  else if (wcode == 1)
	  {
	       ERROR_EXIT(TC_ABORTEDBYREQUEST); /* Abort */
	  }
	  else
	  {			/* Switch to Keyboard prompt */
	       LogTape( "Task %u : Callout routine has exited with code %d: will prompt\n",
		       taskId,wcode);
	       CallOut = 0;	/* Switch to keyboard input */
	       goto forw;
	  }
     }

   forw :

     if (!CallOut) {
	  clearStatus(taskId, CALL_WAIT);
	  setStatus(taskId, OPR_WAIT);

	  while (1) {
	       FFlushInput(stdin);
	       putchar(BELLCHAR);
	       PrintPrompt(flag, name, dumpID);
	       fflush(stdout);
	       start = time(0);
	       while(1) {
		    code = FWait(stdin,	/* check every --> seconds */ 5);
		    if (code == 0) break; /* input is available */

		    if ( checkAbortByTaskId(taskId) )
		    {
			 printf("This tape operation has been aborted by the coordinator.\n");
			 return (TC_ABORTEDBYREQUEST);
		    }

		    if (time(0) > start + BELLTIME) break;
	       }
	       /* otherwise, we should print the prompt out again, check for abort
		  and go back, since the FWait timed out */
	       if (code == 0) break; /* input is available */
	  };
	  getchar();
     };

     clearStatus(taskId, OPR_WAIT);

    dce_printf(bak_s_thanks_proceeding);

     switch (flag)
     {
     case READOPCODE:
     case RESTOREDBOPCODE:
	  dce_printf(bak_s_reading);
	  break;

     case WRITEOPCODE:
     case SAVEDBOPCODE:
	  dce_printf(bak_s_writing);
	  break;

     case LABELOPCODE:
	  dce_printf(bak_s_labelling);
	  break;

     case READLABELOPCODE:
	  dce_printf(bak_s_label_reading);
	  break;

     case SCANOPCODE:
	  dce_printf(bak_s_scanning);
	  break;

     default:
	  dce_printf(bak_s_unknown);
	  break;
     }

    dce_svc_printf(BAK_S_OPERATION_MSG);

     if (!CallOut)
	  printf("**********************************\n");

     tapemounted = 1;
 error_exit:
     clearStatus(taskId, (OPR_WAIT | CALL_WAIT));
     return(code);

}

/* PositionTape
 *	position tape at the beginning of the volume <VolName> if
 *	it exists.  If Volid is 0, we don't check it.
 */
long
PositionTape(TapeInfo,curVolName,curVolid)
     struct butm_tapeInfo *TapeInfo;
     char *curVolName;
     afs_hyper_t curVolid;
{
    struct volumeHeader tapeVolHeader, hostVolHeader;
    long nbytes;
    long code = 0;
    char databuf[BUTM_BLOCKSIZE];

    while(1) {
	afs_hyper_t volId;

	code = butm_ReadFileBegin(TapeInfo);
	if(code == BUTM_EOT) {
	    break;
	}
	if(code) return code;

	bzero((char *)&tapeVolHeader,sizeof(struct volumeHeader));
	code = butm_ReadFileDataCopy(TapeInfo,&tapeVolHeader,
				     sizeof(tapeVolHeader), &nbytes);
	if(code)
		return code;

	code = VolHeaderToHost(&hostVolHeader, &tapeVolHeader);
	if ( code )
		return(code);
	DFSH_MemFromNetHyper (volId, hostVolHeader.volumeID);

	if((strcmp(hostVolHeader.volumeName,curVolName) == 0 ) &&
	   (!AFS_hgetlo(curVolid) ||
	    AFS_hsame(curVolid, volId))
	  )
	{
	    /*found the volume */
	    return 0;
	}
	/* skip all the file data till EOF seen */
	nbytes = 1;
	while(nbytes) {
	    code = butm_ReadFileData(TapeInfo,databuf,BUTM_DATASIZE,&nbytes);
	    if(code && code != BUTM_STATUS_EOF)
	      return code;
	    pthread_yield();
	}
	code = butm_ReadFileEnd(TapeInfo);
	if(code) return code;
	pthread_yield();
    }
    return TC_VOLUMENOTONTAPE;


}

/* ResetTapeState
 *	reset the state of the tape device by closing and opening it.
 *	In many cases this is used merely to rewind the tape. It is
 *	also used when the user is asked to change tapes.
 * notes:
 *	users are the Labeller and database dump code
 */

ResetTapeState(TapeInfo, TapeName)
     struct butm_tapeInfo *TapeInfo;
     char *TapeName;
{
    char *whoi = "Dumper: ResetTapeState:";
    long code;

    dfprintf((stdout, "resetTapeState: dismounting tape\n"));
    code = butm_Dismount(TapeInfo);
    if(code)
    {
      dfprintf((stdout, "resetTapeState: dismounting tape failed, code %d\n", code));
	LogError("%s can't dismount tape\n", code, whoi);
	return(code);
    }
    dfprintf((stdout, "resetTapeState: mounting tape\n"));
    code = butm_Mount(TapeInfo,TapeName);
    if ( code == BUTM_NOTAPE )
    {
	/* no tape loaded or drive offline */
	dce_svc_printf(BAK_S_NO_TAPE_MSG,globalTapeConfig.device);
	return(code);
    }
    else if(code)
    {
      dfprintf((stdout, "resetTapeState: mounting tape failed, code %ld\n",code));
	LogError("%s can't mount tape %s\n", code, whoi);
	return(code);
    }
    return(0);
}

/* VerifyLabel
 *	mount a tape, read label and verify it matches tapename.  If tape
 *	name is wrong, set awrongTape to be true, otherwise false. */

long static
VerifyLabel(TapeInfo, TapeName, DumpId, rockPtr, awrongTape)
     struct butm_tapeInfo *TapeInfo;
     int *awrongTape;
     char *TapeName;
     long DumpId;
     struct restoreRock *rockPtr;
{
    long error;
    long code;
    static struct butm_tapeLabel oldTapeLabel;

    error = 0;
    code = 0;
    *awrongTape = 0;

    code = butm_Mount(TapeInfo,TapeName);
    if ( code == BUTM_NOTAPE )
    {
	/* no tape loaded or drive offline */
	dce_svc_printf(BAK_S_NO_TAPE_MSG,globalTapeConfig.device);
	error = TC_NOTAPE;
	goto vlfail;
    }
    else if(code) {
	LogError("Restorer: VerifyLabel: task %u: can't mount the tape %s\n",
		 code,rockPtr->Node->dumpID,TapeName);
	SetError(rockPtr->Node,TASK_ERROR,code,vlfail);
    }

    if(ShouldAbort(rockPtr->Node, 0)) {
	/*the dumper wakes up anyone waiting */
	/*do any cleanup */
	code = butm_Dismount(TapeInfo);/*TapeInfo is the stuff got by init proc */
	if(code) {
	    LogError("Restorer:VerifyLabel: task %u: can't unmount tape %s\n",
		     code,rockPtr->Node->dumpID,TapeName);
	    SetError(rockPtr->Node,TASK_ERROR,code,vlfail);
	}
	SetError(rockPtr->Node,ABORT_DONE,TC_ABORTEDBYREQUEST,vlfail);
    }
    code = butm_ReadLabel(TapeInfo,&oldTapeLabel);
    if(code) {/* no label exists */
	LogError("Restorer:VerifyLabel:task %u: can't read label on tape %s\n",
		 code,rockPtr->Node->dumpID,TapeName);
	if(!debugLevel)
	    dce_svc_printf(BAK_S_CANT_READ_LABEL_MSG,TapeName);
	*awrongTape = 1;
	error = TC_WRONGTAPE;
	goto vlfail;
    }
    code = CheckLabel(&oldTapeLabel,TapeName,DumpId);
    /*should check if the label corresponds to the tape name */
    if(code) {/* wrong tape */
	if(!strcmp("",oldTapeLabel.name)){
	    LogError("Restorer:VerifyLabel %u: label expected %s; label seen <NULL>. dump id expected %u; dump id seen %u.\n",
		 TC_WRONGTAPE,rockPtr->Node->dumpID,TapeName,DumpId,oldTapeLabel.dumpid);
	    if(!debugLevel)
		dce_svc_printf(BAK_S_LABEL_EXPECTED_LABEL_SEEN_MSG, TapeName, "NULL", DumpId, oldTapeLabel.dumpid);
	}
	else{
	    LogError("Restorer:VerifyLabel %u: label expected %s; label seen %s. dump id expected %u; dump id seen %u.\n",
		 TC_WRONGTAPE,rockPtr->Node->dumpID,TapeName,oldTapeLabel.name,DumpId,oldTapeLabel.dumpid);
	    if(!debugLevel)
		dce_svc_printf(BAK_S_LABEL_EXPECTED_LABEL_SEEN_MSG, TapeName,oldTapeLabel.name,DumpId,oldTapeLabel.dumpid);
	}
	*awrongTape = 1;
	error = TC_WRONGTAPE;
    }
vlfail:return error;
}


/* GetHeader
 *	seek to approp position and read out the volume header
 */
long static
GetHeader(TapeInfo,Restore,volTrailer,trailerSize,rockPtr)
     struct butm_tapeInfo *TapeInfo;
     struct volumeHeader *volTrailer;
     long trailerSize;
     struct restoreRock *rockPtr;
     struct tc_restoreDesc *Restore;
{
    long error;
    long code;
    long nbytes;

    nbytes = 0;
    code = 0;
    error = 0;

    code = butm_Seek(TapeInfo,Restore->position);
    if(code) {
	LogError("Restorer:GetHeader: task %u: can't seek to %u on tape %s\n",
		 code,rockPtr->Node->dumpID,Restore->position,rockPtr->TapeName);
	SetError(rockPtr->Node,TASK_ERROR,code,ghfail);
    }
    code = butm_ReadFileBegin(TapeInfo);
    if(code) {
	LogError("Restorer:GetHeader: task %u: can't read FileBegin on tape %s\n",
		 code,rockPtr->Node->dumpID,rockPtr->TapeName);
	SetError(rockPtr->Node,TASK_ERROR,code,ghfail);
    }
    code = butm_ReadFileDataCopy(TapeInfo,volTrailer,trailerSize,&nbytes);
    if(code) {
	LogError("Restorer:GetHeader: task %u: can't read FileData on tape %s\n",
		 code,rockPtr->Node->dumpID,rockPtr->TapeName);
	SetError(rockPtr->Node,TASK_ERROR,code,ghfail);
    }
    if(nbytes == 0){
	SetError(rockPtr->Node,0,TC_PREMATUREEOF,ghfail);
    }
ghfail:return error;
}

/*
 * fill the restore buffers by reading from tape.
 */
void
restoreAsyncRtn(elemP, argP)
    asyncElement_t *elemP;
    void *argP;
{
    struct ftsPipeState *statePtr = (struct ftsPipeState *)argP;
    struct restoreRock *rockPtr = (struct restoreRock *)statePtr->data;
    afs_hyper_t volId;
    long code=0;
    struct volumeHeader tapeVolTrailer, hostVolTrailer;
    unsigned long wrongTape;
    int foundTrailer;
    int tapecount = 1;
    unsigned long error=0;
    int bytesRead;

    dfprintf((stdout, "Enter restoreAsyncRtn\n"));

    /* if we need a new tape, mount it now */
    if ( rockPtr->needNewTape ) {
        /* new fileset */
        dfprintf((stdout, "Need a new tape, loading one right now\n"));
        rockPtr->needNewTape = 0;    /* reset the flag */
        rockPtr->newTape = 1;    /* we are reading from a new tape */
        while (1) {
            code = PromptForTape (READOPCODE, rockPtr->TapeName,
                                  rockPtr->DumpID,
                                  rockPtr->Node->dumpID, tapecount);
            if (code)
                goto error_exit;

            code = VerifyLabel (rockPtr->TapeInfo, rockPtr->TapeName,
                                rockPtr->DumpID, rockPtr, &wrongTape);
            if (code == 0)
                break;    /* all done */
            if (!wrongTape && code != TC_NOTAPE) {
                /* can't verify label */
                LogError("restoreAsyncRtn: task %u: can't verify label on tape %s, dump id %u.\n",
                         code, rockPtr->Node->dumpID,
                         rockPtr->TapeName, rockPtr->DumpID);
                SetError(rockPtr->Node, TASK_ERROR, code, error_exit);
            } else if ( code != TC_NOTAPE ) {
                /* wrong tape has been put in */
                dce_svc_printf(BAK_S_YOU_PUT_IN_WRONG_TAPE_MSG);
                code = butm_Dismount(rockPtr->TapeInfo);
                if (code) {
                    LogError("restoreAsyncRtn: task %u: can't dismount tape %s\n",
                             code, rockPtr->Node->dumpID,
			     rockPtr->TapeName);
                    SetError(rockPtr->Node, TASK_ERROR, code, error_exit);
                }
            }
            tapecount++;
        } /* while */
    } /* needNewTape */

    /* first time with this tape, read the volume header */
    if ( rockPtr->newTape ) {
        dfprintf((stdout, "start of a new tape\n"));
        rockPtr->newTape = 0;
  
        /* seek to the fileset */
        if (rockPtr->Restore->position) {
            code = GetHeader (rockPtr->TapeInfo, rockPtr->Restore,
                              &tapeVolTrailer, sizeof(tapeVolTrailer),
			      rockPtr);
            if (code) {
                LogError("restoreAsyncRtn: task %u: can't get header info on tape %s\n",
                         code, rockPtr->Node->dumpID, rockPtr->TapeName);
                SetError(rockPtr->Node, TASK_ERROR, code, error_exit);
            }
            code = VolHeaderToHost (&hostVolTrailer, &tapeVolTrailer);
            if (code) {
                LogError("restoreAsyncRtn: task %u: can't convert header on tape %s from net to host format\n",
                         code, rockPtr->Node->dumpID, rockPtr->TapeName);
                SetError(rockPtr->Node, TASK_ERROR, code, abort_exit);
            }
	    DFSH_MemFromNetHyper(volId, hostVolTrailer.volumeID);
            if (strcmp (hostVolTrailer.volumeName, rockPtr->OldVolName) ||
                !AFS_hsame(volId, rockPtr->OrigVid) ||
                (hostVolTrailer.magic != TC_VOLBEGINMAGIC)) {
                 /* header does not match, abandon restoring this fileset */
                 SetError(rockPtr->Node, TASK_ERROR, -1, abort_exit);
            }
        } /* Restore->position */
        else {        /* position the tape using linear scan */
            dfprintf((stdout, "positioning the tape using linear scan\n"));
            code = PositionTape (rockPtr->TapeInfo,
  			       rockPtr->VolName, rockPtr->Volid);
            if (code) {
                LogError("restoreAsyncRtn: task %u: can't find volume %s on tape %s\n",
                         code, rockPtr->Node->dumpID, rockPtr->VolName,
  		       rockPtr->TapeName);
                SetError(rockPtr->Node, TASK_ERROR, code, abort_exit);
            }
        }
    }

    /* read the next block of data into the queue element */
    dfprintf((stdout, "Reading next buffer\n"));
    code = butm_ReadFileData(rockPtr->TapeInfo, elemP->data,
                             BUTM_DATASIZE, &bytesRead);
    if (!code) {
	/*
	 * release the hold on the next element
	 */
	if (rockPtr->nextElemP && rockPtr->lastElemP) {
	    rockPtr->nextElemP->hold = 0;
	    rockPtr->nextElemP = NULL;
	}
	/* place a hold on this buffer */
	elemP->count = bytesRead;
	elemP->offset = 0;
	elemP->hold = 1;
	rockPtr->nextElemP = rockPtr->lastElemP;
	rockPtr->lastElemP = elemP;
        rockPtr->chunkSize += bytesRead;
    } else if (code == BUTM_ENDVOLUME) {
	/*
	 * we hit the end of this file, extract the volume trailer
	 * and release the holds on the previous two data buffers
	 */
	code = 0;
        dfprintf((stdout, "found the FILE_END mark\n"));
	elemP->count = 0;
	elemP->offset = 0;
	if (rockPtr->lastElemP == NULL) {
	    /* first block is end of file */
	    foundTrailer = 0;
	} else if ( rockPtr->nextElemP == NULL) {
	    /* only one block in tape file */
	    foundTrailer = ExtractTrailer(NULL, 0, NULL,
					  rockPtr->lastElemP->data+BUTM_HDRSIZE,
					  rockPtr->lastElemP->count,
					  &rockPtr->lastElemP->count,
					  &tapeVolTrailer);
	    rockPtr->lastElemP->hold = 0;
	    rockPtr->lastElemP = NULL;
	} else {
	    /* check last two blocks for file mark */
	    foundTrailer = ExtractTrailer(rockPtr->nextElemP->data+BUTM_HDRSIZE,
					  rockPtr->nextElemP->count,
					  &rockPtr->nextElemP->count,
					  rockPtr->lastElemP->data+BUTM_HDRSIZE,
					  rockPtr->lastElemP->count,
					  &rockPtr->lastElemP->count,
					  &tapeVolTrailer);
	    rockPtr->nextElemP->hold = 0;
	    rockPtr->nextElemP = NULL;
	    rockPtr->lastElemP->hold = 0;
	    rockPtr->lastElemP = NULL;
	}
        if (foundTrailer) {
             dfprintf((stdout, "trailer seems to be valid\n"));
            /* trailer seems to be valid */
            code = VolHeaderToHost (&hostVolTrailer, &tapeVolTrailer);
            if (code) {
                code = TC_MISSINGTRAILER;
                LogError("restoreAsyncRtn: task %u: no trailer on tape %s\n",
			 code, rockPtr->Node->dumpID, rockPtr->TapeName);
                SetError(rockPtr->Node, TASK_ERROR, code, abort_exit);
            }
            if (hostVolTrailer.contd == 0) {
                /* seen the last fileset fragment */
                dfprintf((stdout, "last fragment of the fileset restored\n"));
                rockPtr->Frag = hostVolTrailer.frag;
                dfprintf((stdout, "setting last flag in queue element\n"));
		elemP->last = 1;
            } else {
                /* dismount current tape and prepare to mount the next tape */
                dfprintf((stdout, "this tape over, prepare for next one\n"));
                rockPtr->Frag = hostVolTrailer.frag;
                rockPtr->Restore++;
                code = butm_Dismount (rockPtr->TapeInfo);
                if (code) {
                   LogError("restoreAsyncRtn: task %u: can't dismount %s\n",
			    code, rockPtr->Node->dumpID,
			    rockPtr->TapeName);
                   SetError(rockPtr->Node, TASK_ERROR, code, abort_exit);
                }
                strcpy (rockPtr->TapeName,
			(char *)rockPtr->Restore->tapeName);
                rockPtr->DumpID = rockPtr->Restore->realDumpId;
                rockPtr->Frag = rockPtr->Restore->frag;
                rockPtr->needNewTape = 1;
            } /* else */
        } else {
            dfprintf((stdout, "missing trailer\n"));
            code = TC_MISSINGTRAILER;
            LogError("restoreAsyncRtn: task %u: can't read trailer on %s\n",
		     code, rockPtr->Node->dumpID, rockPtr->TapeName);
            SetError(rockPtr->Node, TASK_ERROR, code, abort_exit);
        }
    } else {
        LogError("restoreAsyncRtn: task %u: error reading tape %s\n",
                 code, rockPtr->Node->dumpID, rockPtr->TapeName);
        SetError(rockPtr->Node, TASK_ERROR, code, abort_exit);
    }

 exit:
    dfprintf((stdout, "restoreAsyncRtn, no error\n"));
    if (rockPtr->chunkSize > BIGCHUNK) {
        /* time to check if anyone has signalled abort */
        rockPtr->chunkSize = 0;
        if (ShouldAbort(rockPtr->Node, rockPtr->residBytes)) {
            error = TC_ABORTEDBYREQUEST;
            code = butm_ReadFileEnd (rockPtr->TapeInfo);
            if (code) {
                LogError("restoreAsyncRtn: task %u: can't read EOF on %s\n",
			 code, rockPtr->Node->dumpID, rockPtr->TapeName);
                SetError(rockPtr->Node, TASK_ERROR, code, error_exit);
            }
            code = butm_Dismount(rockPtr->TapeInfo);
            if (code) {
                LogError("restoreAsyncRtn: task %u: can't dismount tape %s\n",
			 code, rockPtr->Node->dumpID, rockPtr->TapeName);
                SetError(rockPtr->Node, TASK_ERROR, code, error_exit);
            }
            goto error_exit;
        }
	rockPtr->residBytes %= 1024;
    }
    dfprintf((stdout, "*** leaving restoreAsyncRtn ***\n"));
    return;

 error_exit:
    dfprintf((stdout, "error_exit: error code %lu\n", code));
    statePtr->error = code;
    if(rockPtr->nextElemP) {
	rockPtr->nextElemP->hold = 0;
	rockPtr->nextElemP = NULL;
    }
    if(rockPtr->lastElemP) {
	rockPtr->lastElemP->hold = 0;
	rockPtr->lastElemP = NULL;
    }
    elemP->count = 0;
    elemP->hold = 0;
    elemP->last = 1;
    dfprintf((stdout, "*** leaving restoreAsyncRtn ***\n"));
    return;

 abort_exit:
    dfprintf((stdout, "abort_exit: error code %lu\n", code));
    code = butm_ReadFileEnd (rockPtr->TapeInfo);
    if (code) {
        LogError("restoreAsyncRtn: task %u: can't read file mark on tape %s\n",
		 code, rockPtr->Node->dumpID, rockPtr->TapeName);
        SetError(rockPtr->Node, TASK_ERROR, code, error_exit);
    }
    goto error_exit;
}

/* Allocate a buffer for the pipe used to restore filesets.
 * The tape is actually read in the allocate routine to avoid
 * data copies in the push routine.
 */
void
restoreAlloc (state, bsize, buf, nbytes)
  rpc_ss_pipe_state_t state;
  idl_ulong_int bsize;
  idl_byte **buf;
  idl_ulong_int *nbytes;
{
    struct ftsPipeState *statePtr = (struct ftsPipeState *)state;
    struct restoreRock *rockPtr = (struct restoreRock *)statePtr->data;
    static char dummybuf[4096];
    void restoreAsyncRtn();

    dfprintf((stdout, "restoreAlloc, bsize=%d\n",bsize));

    /*
     * initialize the async output queue
     */
    if ( rockPtr->asyncP == NULL && statePtr->error == 0 ) {
        rockPtr->asyncP = initAsyncQueue(RESTORE_NBUFS, BUTM_BLOCKSIZE,
			                 (void (*)())restoreAsyncRtn, NULL,
					 (void *)state);
	if ( rockPtr->asyncP == NULL ) {
	    dfprintf((stdout, "restoreAlloc: problem with async queue\n"));
	    statePtr->error = EPIPE;
	}
    }
    
    /*
     * use the dummy buffer if there is a problem with the async queue
     */

     if ( rockPtr->asyncP == NULL ) {
	*buf = (ndr_byte *)&dummybuf[0];
	*nbytes = 4096;
	if ( *nbytes > bsize ) *nbytes = bsize;
	return;
    }

    /* return empty buffers to the input queue */
    if ( rockPtr->curElemP != NULL &&
	 rockPtr->curElemP->offset == rockPtr->curElemP->count) {
	rockPtr->curElemP->offset = 0;
	rockPtr->curElemP->count = 0;
	relAsyncElement(rockPtr->asyncP, rockPtr->curElemP);
	rockPtr->curElemP = NULL;
    }

     /* if we don't have a queue element, get one */
     while ( rockPtr->curElemP == NULL ) {
	rockPtr->curElemP = getAsyncElement(rockPtr->asyncP);
	if ( rockPtr->curElemP == NULL ) {
	    dfprintf((stdout, "restoreAlloc: reached end of async queue\n"));
	    termAsyncQueue(rockPtr->asyncP);
	    rockPtr->asyncP = NULL;
	    *buf = (ndr_byte *)&dummybuf[0];
	    *nbytes = 4096;
	    if ( *nbytes > bsize ) *nbytes = bsize;
	    return;
	}
	if (rockPtr->curElemP->count == 0) {
	    relAsyncElement(rockPtr->asyncP, rockPtr->curElemP);
	    rockPtr->curElemP = NULL;
	}
    }

    /* the pipe reads directly from the queue element's buffer */
    *buf = (ndr_byte *) (rockPtr->curElemP->data) +
	   BUTM_HDRSIZE + rockPtr->curElemP->offset;
    *nbytes = rockPtr->curElemP->count - rockPtr->curElemP->offset;
    return;
}

/*
 * Pull routine for the pipe used to restore filesets.
 * The actual work is done in the allocate routine.
 */
void
restorePull (state, buf, bsize, nbytes)
   rpc_ss_pipe_state_t state;
   idl_byte *buf;
   idl_ulong_int bsize;
   idl_ulong_int *nbytes;
{
    struct ftsPipeState *statePtr = (struct ftsPipeState *)state;
    struct restoreRock *rockPtr = (struct restoreRock *)statePtr->data;

    /* send zero length buffer to indicate end of transfer */
    if ( rockPtr->asyncP == NULL) {
        dfprintf((stdout, "restorePull, returning empty buffer\n"));
        *nbytes = 0;
    } else {
	/* update the current buffer offset with the amount transferred */
	*nbytes = rockPtr->curElemP->count - rockPtr->curElemP->offset;
	if (*nbytes > bsize) *nbytes = bsize;
	rockPtr->curElemP->offset += *nbytes;
        rockPtr->residBytes += *nbytes;
        rockPtr->Node->bytesDumped += *nbytes;
    }
    return;
}

#define free_all(TapeInfo, oldTapeLabel, newTapeLabel) \
{ \
	  osi_Free(TapeInfo, sizeof(struct butm_tapeInfo)); \
	  osi_Free(oldTapeLabel, sizeof(struct butm_tapeLabel)); \
	  osi_Free(newTapeLabel, sizeof(struct butm_tapeLabel)); \
}

/* Restorer
 *	created as a thread by the server stub, <newNode> is a pointer to all
 *	the parameters Restorer needs
 */

Restorer(newNode)
struct dumpNode *newNode;
{
    int waiting = 0;                /* anyone waiting for my signal? */
    long code, altCode;
    int needNewTape, k, RWix, agType;
    char config[256];
    struct butm_tapeLabel *oldTapeLabel, *newTapeLabel;
    struct restoreRock rock;
    int totalFrags, wrongTape;
    long moreRestores;
    int mounted = 0;
    struct budb_tapeEntry backupTape;
    statusP statusPtr;
    u_long taskId;
    pipe_t dataPipe;
    struct ftsPipeState pipeState;
    afs_hyper_t volId;
    char *volName;
    int override;
    struct sockaddr_in sa;
    unsigned long restoreFlags;
    struct vldbentry fldbEntry;
    extern u_long getStatusFlag();
    extern statusP findStatus();
    int tapecount = 1, tapeNumber = 1;
    int cont = 0;

    oldTapeLabel = (struct butm_tapeLabel *)
		   osi_Alloc(sizeof(struct butm_tapeLabel));
    if (oldTapeLabel == (struct butm_tapeLabel *)NULL)
        return(TC_NOMEMORY);
    newTapeLabel = (struct butm_tapeLabel *)
		   osi_Alloc(sizeof(struct butm_tapeLabel));
    if (newTapeLabel == (struct butm_tapeLabel *)NULL) {
        osi_Free(oldTapeLabel, sizeof(struct butm_tapeLabel));
        return(TC_NOMEMORY);
    }
    rock.TapeInfo = (struct butm_tapeInfo *)
		       osi_Alloc(sizeof(struct butm_tapeInfo));
    if (rock.TapeInfo == (struct butm_tapeInfo *)NULL) {
        osi_Free(oldTapeLabel, sizeof(struct butm_tapeLabel));
        osi_Free(newTapeLabel, sizeof(struct butm_tapeLabel));
        return (TC_NOMEMORY);
    }
    bzero((char *)rock.TapeInfo,sizeof(struct butm_tapeInfo));
    bzero((char *)oldTapeLabel,sizeof(struct butm_tapeLabel));
    bzero((char *)newTapeLabel,sizeof(struct butm_tapeLabel));
    rock.Dump = (struct tc_dumpDesc *)0;
    rock.Restore = (struct tc_restoreDesc *)0;
    rock.Node = newNode;

    EnterDeviceQueue(deviceLatch);
    clearStatus(newNode->dumpID, DRIVE_WAIT);

    rock.TapeInfo->structVersion = CUR_BUTM_VERSION;
    code = butm_file_Instantiate (rock.TapeInfo, &globalTapeConfig);
    if (code) {
        LogError("Dumper: can't initialize the tape module for task %u\n", \
                 code,rock.Node->dumpID);

        free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);
        SetAndReturn(rock.Node,TASK_ERROR|TASK_DONE,code,deviceLatch,
		     rock.Restore);
    }

    /* do the initial parameter processing and call set up */
    if ( checkAbort(newNode) == 0 ) {
        lock_ObtainWrite(&(newNode->lock));
        newNode->status |= INPROGRESS;

        /*point to the first entry to be dumped */
        rock.Restore = newNode->restores;
        totalFrags = newNode->arraySize; /*total number of entries */
        newNode->opcode = RESTORE;
        newNode->bytesDumped = 0;
        newNode->volumeBeingDumped=  rock.Restore->vid;
        lock_ReleaseWrite(&(newNode->lock));
    } else {
        lock_ObtainWrite(&(newNode->lock));
        /* someone has signalled me to abort */
        newNode->status |= ABORTED;
        newNode->status &= ~INPROGRESS;
        if(newNode->status & TC_WAITING) waiting = 1;
        /* do any cleaning up */
        lock_ReleaseWrite(&(newNode->lock));
        setStatus(newNode->dumpID, ABORT_DONE|TASK_DONE);

        /* this should be done if someone is waiting */
        lock_ObtainWrite(&(newNode->lock));
        osi_Wakeup((caddr_t)&(newNode->lock));
        lock_ReleaseWrite(&(newNode->lock));

        waiting = 0;
        LogError("Restorer: task %u aborted by request\n",
		 0,rock.Node->dumpID);
        code = butm_Dismount(rock.TapeInfo);
        if (code) {
            LogError("Dumper: can't unmount tape %s\n",code,rock.TapeName);
            free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);
            SetAndReturn(rock.Node,TASK_ERROR | TASK_DONE,code,deviceLatch,
			 rock.Restore);
        }
        LeaveDeviceQueue(deviceLatch);
        free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);

        return 0;
    }

    /* do any other processing, going through the same loop as above,
     * set error or other info if required
     */

    strcpy(rock.TapeName, (char *)rock.Restore->tapeName);
    rock.DumpID = rock.Restore->realDumpId;
    needNewTape = 1;
    moreRestores = totalFrags;
    while(moreRestores) {

        /* process each entry , check for return errors on each call.
         * Get the connection with the site on which volume resides
         */
        rock.Server = rock.Restore->hostAddr; /* structure assignment */
        bcopy((char *)&rock.Server, (char *)&sa, sizeof(struct sockaddr_in));
        if(!(sa.sin_addr.s_addr)) {
            LogError("Restorer: task %u: illegal host ID\n",
                     0,rock.Node->dumpID);
            free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);
            SetAndReturn(rock.Node,TASK_ERROR | TASK_DONE,
                         TC_BADHOST,deviceLatch,rock.Restore);
        }
        rock.Part = rock.Restore->partition;
        rock.Volid=  rock.Restore->vid;
        strcpy(rock.VolName, (char *)rock.Restore->newName);
        strcpy(rock.OldVolName, (char *)rock.Restore->oldName);
        rock.Frag = rock.Restore->frag;
        rock.OrigVid=  rock.Restore->origVid;

        if(needNewTape) {
            needNewTape = 0;
            tapecount = 1;
            mounted = 1;
            while (1) {
                if (((tapeNumber == 1) && (autoQuery == 1))
                    || (tapeNumber > 1))
                    code = PromptForTape(READOPCODE,rock.TapeName,
					 rock.DumpID,
                                         rock.Node->dumpID, tapecount);

                if (code) {
                    free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);
                    SetAndReturn(newNode,TASK_ERROR|TASK_DONE,0,deviceLatch,
				 rock.Restore);
                }

                if ( checkAbort(rock.Node) ) {
                    free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);
                    SetAndReturn(newNode,ABORT_DONE|TASK_DONE,0,deviceLatch,
				 rock.Restore);
                }
                code = VerifyLabel(rock.TapeInfo,rock.TapeName,
				   rock.DumpID, &rock, &wrongTape);
                if (code == 0) break;
                if (!wrongTape && code != TC_NOTAPE ) {
                    /* some other error */
                    LogError("Restorer: task %u: can't verify label on tape %s, dump id %u\n",
                             0,rock.Node->dumpID,rock.TapeName,
			     rock.DumpID);
                    free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);

                    SetAndReturn(rock.Node,TASK_ERROR | TASK_DONE,
                                 code,deviceLatch, rock.Restore);
                }
                else if ( code != TC_NOTAPE ) {
                    /* wrong tape has been put in */
                    dce_svc_printf(BAK_S_YOU_PUT_IN_WRONG_TAPE_MSG);

                    code = butm_Dismount(rock.TapeInfo);
                    if(code) {
                        LogError("Restorer: task %u: can't unmount tape %s, dump id %u\n",
                                 code,rock.Node->dumpID,rock.TapeName,
				 rock.DumpID);
                        free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);

                        SetAndReturn(rock.Node,TASK_ERROR|TASK_DONE,
                                     code,deviceLatch,rock.Restore);
                    }
                }
                tapecount++;
                tapeNumber++;
            }
        }
        if((rock.Restore->position >= 2)
           && (rock.TapeInfo->position < rock.Restore->position)) {
            code = butm_Seek(rock.TapeInfo,rock.Restore->position);
            if(code) {
                LogError("Restorer: task %u: can't seek to position %u on tape %s, dump id %u\n",
                         0,rock.Node->dumpID,rock.Restore->position - 1,
                         rock.TapeName, rock.DumpID);
                if(ShouldContinue(rock.VolName)){
                    lock_ObtainWrite(&(rock.Node->lock));
                    rock.Node->status |= PARTIAL;
                    rock.Node->status &= ~ERROR_FLAG;
                    lock_ReleaseWrite(&(rock.Node->lock));
                    clearStatus(rock.Node->dumpID, TASK_ERROR);
                    goto skipto;
                }
                else{
                    altCode = butm_Dismount(rock.TapeInfo);
                    if(altCode) {
                        LogError("Restorer: task %u: can't unmount tape %s, dump id %u\n",
                                 altCode,rock.Node->dumpID,
				 rock.TapeName,rock.DumpID);
                        free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);
                        SetAndReturn(rock.Node,TASK_ERROR |TASK_DONE,
                                     altCode, deviceLatch, rock.Restore);
                    }
                    mounted = 0;
                    free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);

                    SetAndReturn(rock.Node,TASK_ERROR |TASK_DONE,
                                 code, deviceLatch, rock.Restore);
                }
            }

        }


        /* set the volume name for the current restore */
        /* follow proper locking hierarchy, node lock before status q lock */
        lock_ObtainRead(&(newNode->lock));
        taskId = newNode->dumpID;
        lock_ObtainShared(&statusQueueLock);
        statusPtr = findStatus(taskId);
        if ( statusPtr != 0 ) {
            lock_UpgradeSToW(&statusQueueLock);
            strncpy(statusPtr->volumeName, rock.VolName,
                    sizeof(statusPtr->volumeName));
            lock_ReleaseWrite(&statusQueueLock);
        } else {
            com_err("Restorer", 0, "Unable to locate status node\n");
            lock_ReleaseShared(&statusQueueLock);
        }
        lock_ReleaseRead(&(newNode->lock));

        LogTape("Task %u: Restoring fileset %s ..\n",
                rock.Node->dumpID, rock.VolName);

        if (strcmp(rock.VolName, rock.OldVolName) == 0)
            override = 1;
        else
            override = 0;

        /*
         * If the fileset to be restored already exists, we overwrite it. If
         * the fileset does not exist, we create it before we restore the data.
         */

        if (!(code = vldb_GetEntryByName(ExtractSuffix(rock.VolName),
                                         &fldbEntry, 1))){
            RWix = -1;
            for (k = 0; k < MAXVOLTYPES &&
			!AFS_hiszero(fldbEntry.VolIDs[k]); ++k) {
                if (fldbEntry.VolTypes[k] == VOLTIX_TO_VOLTYPE(RWVOL)) {
                    RWix = k;
                    break;
                }
            }
            if (AFS_hiszero(rock.Volid))
                rock.Volid=  fldbEntry.VolIDs[RWix];
            else if (!AFS_hiszero(fldbEntry.VolIDs[RWix]) &&
                     !AFS_hsame(fldbEntry.VolIDs[RWix], rock.Volid))
                rock.Volid=  fldbEntry.VolIDs[RWix];
            LogTape("Task %lu: The fileset %s (%lu,,%lu) already exists in the FLDB, overwriting it.\n",
                    rock.Node->dumpID, rock.VolName, AFS_HGETBOTH(rock.Volid));
            dce_svc_printf(BAK_S_FILESET_ALREADY_EXISTS_PVERWRITING_MSG,
			   rock.Node->dumpID, rock.VolName,
			   AFS_HGETBOTH(rock.Volid));
        } else { /* doesn't exist, create it first */
            /* Check if are creating the new fileset in LFS */
            code = fts_GetAggrType(&rock.Server, rock.Part, &agType, 1);
            if (code || !(AG_TYPE_SUPPORTS_EFS(agType))) {
                LogTape("Task %lu: The fileset %s (%lu,,%lu) does not exist, cannot create on non-LFS aggregate %s of %s\n",
                        rock.Node->dumpID, rock.VolName,
			AFS_HGETBOTH(rock.Volid),
                        getAggrName(&rock.Server, rock.Part, 0),
                        MapSockAddr(&rock.Server));
                dce_svc_printf(BAK_S_FILESET_DOES_NOT_EXIST_CAN_NOT_CREATE_MSG,
                               rock.Node->dumpID, rock.VolName,
			       getAggrName(&rock.Server, rock.Part, 0),
                               MapSockAddr(&rock.Server));
                cont = 0;

                if(!queryoperator || ShouldContinue(rock.VolName))
                    cont = 1; /* operator said yes, or no queryoperator  */

                if (cont) {
                    lock_ObtainWrite(&(rock.Node->lock));
                    rock.Node->status |= PARTIAL;
                    rock.Node->status &= ~ERROR_FLAG;
                    lock_ReleaseWrite(&(rock.Node->lock));
                    clearStatus(rock.Node->dumpID, TASK_ERROR);
                    LogError("Restorer: task %u: can't restore fileset %s\n",
                             code, rock.Node->dumpID, rock.VolName);
                    goto skipto;
                } else{
                    lock_ObtainWrite(&(rock.Node->lock));
                    osi_Wakeup((caddr_t)&(rock.Node->lock));
                    lock_ReleaseWrite(&(rock.Node->lock));
                    LogError("Restorer: task %u: can't restore fileset %s - TM error, aborting !!\n",
                             code,rock.Node->dumpID,rock.VolName);
                    free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);
                    SetAndReturn(rock.Node,TASK_ERROR|TASK_DONE,
				 code,deviceLatch,rock.Restore);
                }
            }

            AFS_hzero(rock.Volid);        /* backward compatibility */
            if (code = volc_Call(VC_CreateVolume, &rock.Server,
				 NULL, rock.Part,
                                 rock.VolName, &rock.Volid)) {
                LogError("Task %lu: Restorer: Fileset %s(%lu,,%lu) creation failed on aggregate %s of %s\n",
			 code, rock.Node->dumpID, rock.VolName,
			 AFS_HGETBOTH(rock.Volid),
                         getAggrName(&rock.Server, rock.Part, 0),
                         MapSockAddr(&rock.Server));

                dce_svc_printf(BAK_S_FILESET_CREATION_FAILED_MSG,
			       rock.Node->dumpID, rock.VolName,
			       AFS_HGETBOTH(rock.Volid),
                               getAggrName(&rock.Server, rock.Part, 0),
                               MapSockAddr(&rock.Server));

                if(ShouldContinue(rock.VolName)){
                    lock_ObtainWrite(&(rock.Node->lock));
                    rock.Node->status |= PARTIAL;
                    rock.Node->status &= ~ERROR_FLAG;
                    lock_ReleaseWrite(&(rock.Node->lock));
                    clearStatus(rock.Node->dumpID, TASK_ERROR);
                    LogError("Restorer: task %u: can't restore fileset %s\n",
                             code, rock.Node->dumpID, rock.VolName);
                    goto skipto;
                } else{
                    lock_ObtainWrite(&(rock.Node->lock));
                    osi_Wakeup((caddr_t)&(rock.Node->lock));
                    lock_ReleaseWrite(&(rock.Node->lock));
                    LogError("Restorer: task %u: can't restore fileset %s, aborting !!\n",
                             code,rock.Node->dumpID,rock.VolName);
                    free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);

                    SetAndReturn(rock.Node,TASK_ERROR|TASK_DONE,
				 code,deviceLatch,rock.Restore);
                }
            }
            LogTape("Task %lu: Fileset %s(%lu,,%lu) created on aggregate %s of %s\n",
		    rock.Node->dumpID, rock.VolName,
                    AFS_HGETBOTH(rock.Volid), getAggrName(&rock.Server,
		    rock.Part, 0), MapSockAddr(&rock.Server));

            dce_svc_printf(BAK_S_FILESET_CREATED_ON_AGGR_MSG,
			   rock.Node->dumpID, rock.VolName,
			   AFS_HGETBOTH(rock.Volid),
                           getAggrName(&rock.Server, rock.Part, 0),
                           MapSockAddr(&rock.Server));
        }

        /*
         * initialize the restore pipe state variables
         */

        rock.asyncP       = NULL;
        rock.curElemP     = NULL;
        rock.nextElemP    = NULL;
        rock.lastElemP    = NULL;
        rock.chunkSize    = 0;
        rock.residBytes   = 0;
        rock.newTape      = 1;
        rock.needNewTape  = 0;
        pipeState.data    = (char *)&rock;
        pipeState.error   = 0;
        dataPipe.state    = (rpc_ss_pipe_state_t)&pipeState;
        dataPipe.push     = 0;
        dataPipe.pull     = restorePull;
        dataPipe.alloc    = restoreAlloc;

        /* The restoreFlags holds all special flags that should be passed to
         * the ftserver restore rpc. The flag to allow disjoint restores to
         * proceed should be passed in, when we are in a position to allow
         * such restores. For now, no flags are passed in.
         */
        restoreFlags = 0;

        code = volc_Call(VC_RestoreVolume,
                         (struct sockaddr *)&rock.Server, NULL,
                         (unsigned long)rock.Part, &rock.Volid,
                         rock.VolName, override, restoreFlags,
                         (char *)NULL, (pipe_t *)&dataPipe, sockFlag);

        LogTape("Task %u: Done\n", rock.Node->dumpID);

        /*  restorePull takes care of all the related fragments spanning
         *  various tapes. On return the complete volume has been restored
         */

        if(code && checkAbort(rock.Node))
        {
            /*readData encontered an abort signal */
            lock_ObtainWrite(&(rock.Node->lock));
            osi_Wakeup((caddr_t)&(rock.Node->lock));
            lock_ReleaseWrite(&(rock.Node->lock));
            fprintf(stderr, "Restore: task %u aborted by request\n",
                    rock.Node->dumpID);
            LogError("Restore: task %u aborted by request\n",code,
                     rock.Node->dumpID);
            code = butm_Dismount(rock.TapeInfo);
            if(code) {
                LogError("Restorer: task %u: can't unmount tape %s, dump id %u\n",
                         0,rock.Node->dumpID,rock.TapeName,
			 rock.DumpID);
                free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);

                SetAndReturn(rock.Node,TASK_ERROR | ABORT_DONE | TASK_DONE,
                             code,deviceLatch,rock.Restore);
            }
            mounted = 0;
            free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);

            SetAndReturn(newNode,ABORT_DONE|TASK_DONE,code,deviceLatch,
			 rock.Restore);
        }

        if(code && getStatusFlag(rock.Node, TASK_ERROR))
        {
            fprintf(stderr, "butc: task %lu: Unable to restore fileset %s(%lu,,%lu); %s\n",
		    rock.Node->dumpID, rock.VolName, AFS_HGETBOTH(rock.Volid),
                    dfs_dceErrTxt(code));
            LogError("Restorer: task %u: can't restore fileset %s\n",
                     code, rock.Node->dumpID, rock.VolName);
            /* a tape module error took place in restorePull function*/
            if(ShouldContinue(rock.VolName)){
                lock_ObtainWrite(&(rock.Node->lock));
                rock.Node->status |= PARTIAL;
                rock.Node->status &= ~ERROR_FLAG;
                lock_ReleaseWrite(&(rock.Node->lock));
                clearStatus(rock.Node->dumpID, TASK_ERROR);
            } else{
                lock_ObtainWrite(&(rock.Node->lock));
                osi_Wakeup((caddr_t)&(rock.Node->lock));
                lock_ReleaseWrite(&(rock.Node->lock));

                LogError("Restorer: task %u: can't restore fileset %s - TM error, aborting !!\n",
                         code,rock.Node->dumpID,rock.VolName);
                free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);

                SetAndReturn(rock.Node,TASK_ERROR|TASK_DONE,
			     code,deviceLatch, rock.Restore);
            }
        }

        if(code){
            lock_ObtainWrite(&(rock.Node->lock));
            rock.Node->status |= PARTIAL;
            lock_ReleaseWrite(&(rock.Node->lock));
            fprintf(stderr, "butc: task %lu: Unable to restore fileset %s(%lu,,%lu); %s\n",
		    rock.Node->dumpID, rock.VolName,
                    AFS_HGETBOTH(rock.Volid), dfs_dceErrTxt(code));
            LogError("Restorer: task %u: can't restore fileset %s\n",
                     code,rock.Node->dumpID,rock.VolName);
        }
    skipto: /* to hell with the purists ! */
        moreRestores -= rock.Frag;
        if(moreRestores) rock.Restore++;
        if(moreRestores > 0) { /*this means rock.Restore is != NULL */
            if(strcmp((char *)rock.Restore->tapeName,rock.TapeName)){
		/*need a new tape */
                needNewTape = 1;
                if(mounted) {
                    code = butm_Dismount(rock.TapeInfo);
                    if(code) {
                        LogError("Restorer: task %u: can't dismount tape %s, dump id %u\n",
                                  code,rock.Node->dumpID,rock.TapeName,
				  rock.DumpID);
			free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);
                        SetAndReturn(rock.Node,TASK_ERROR|TASK_DONE,code,
                                     deviceLatch,rock.Restore);
                    }
                    strcpy(rock.TapeName, (char *)rock.Restore->tapeName);
                    rock.DumpID = rock.Restore->realDumpId;
                    mounted = 0;
                }
            }
        } else {
            if(mounted) {
                code = butm_Dismount(rock.TapeInfo);
                if(code) {
                    LogError("Restorer: task %u: can't dismount tape %s, dump id %u\n",
                             code,rock.Node->dumpID,rock.TapeName,
			     rock.DumpID);
                    free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);
                    SetAndReturn(rock.Node,TASK_ERROR | TASK_DONE,code,
                                 deviceLatch,rock.Restore);
                }
                mounted = 0;
            }
        }

        if( checkAbort(newNode) == 0 ) {
            lock_ObtainWrite(&(newNode->lock));
            if(rock.Restore)
                newNode->volumeBeingDumped = rock.Restore->vid;
            lock_ReleaseWrite(&(newNode->lock));
        } else {
            lock_ObtainWrite(&(newNode->lock));
            /* someone has signalled me to abort */
            newNode->status |= ABORTED;
            newNode->status &= ~INPROGRESS;
            if(newNode->status & TC_WAITING) waiting = 1;
            /* do any cleaning up */
            lock_ReleaseWrite(&(newNode->lock));
            setStatus(newNode->dumpID, ABORT_DONE|TASK_DONE);

            /* this should be done if someone is waiting ? */
            lock_ObtainWrite(&(newNode->lock));
            osi_Wakeup((caddr_t)&(newNode->lock));
            lock_ReleaseWrite(&(newNode->lock));

            waiting = 0;
            LogError("Dumper: task %u:aborted by request\n",
		     0,rock.Node->dumpID);
            code = butm_Dismount(rock.TapeInfo);
            if(code) {
                LogError("Restorer: task %u: can't dismount tape %s, dump id %u\n",
                         code,rock.Node->dumpID,rock.TapeName,
			 rock.DumpID);
                free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);

                SetAndReturn(rock.Node, TASK_ERROR|TASK_DONE,
			     code,deviceLatch,rock.Restore);
            }
            LeaveDeviceQueue(deviceLatch);
            free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);

            return 0;
        }
    }

    LogTape("Task %u: restore finished\n", rock.Node->dumpID);
    dce_svc_printf(BAK_S_RESTORE_FINISHED_MSG, rock.Node->dumpID);


    /* when finishing up, check to see if anyone is waiting, if so wake up
     * any waiting fellows
     */
    lock_ObtainWrite(&(newNode->lock));
    if(newNode->status & TC_WAITING) waiting = 1;
    newNode->status =  DONE;        /* all the other flags are cleared */
    lock_ReleaseWrite(&(newNode->lock));
    setStatus(newNode->dumpID, TASK_DONE);

    /* wake up anyone who is still waiting for me */
    if(waiting) {
        lock_ObtainWrite(&(newNode->lock));
        osi_Wakeup((caddr_t)&(newNode->lock));
        lock_ReleaseWrite(&(newNode->lock));
    }
    LeaveDeviceQueue(deviceLatch);

    free_all(rock.TapeInfo, oldTapeLabel, newTapeLabel);
    return 0;
}

/* Sleep for a while waiting for the job to terminate. */
WatchDog(newNode)
struct dumpNode *newNode;
{
    struct timespec delayTime;
    delayTime.tv_sec = TC_ABORTTIME;
    delayTime.tv_nsec = 0;
    pthread_delay_np(&delayTime);

    lock_ObtainWrite(&(newNode->lock));
    osi_Wakeup((caddr_t)&(newNode->lock));
    lock_ReleaseWrite(&(newNode->lock));
    return 0;
}

/* this is just scaffolding, creates new tape label with name <tapeName> */

GetNewLabel(TapeInfo,tapeName,tapeLabel)
struct butm_tapeInfo *TapeInfo;
char *tapeName;
struct butm_tapeLabel *tapeLabel;
{
	struct timeval tp;
	struct timezone tzp;
	long size;

	butm_GetSize(TapeInfo,&size);
	if(!size)
	        size = globalTapeConfig.capacity;
	osi_gettimeofday(&tp,&tzp);
       	tapeLabel->structVersion = CUR_TAPE_VERSION;
	strcpy(tapeLabel->name,tapeName);
	strcpy(tapeLabel->cell, globalCellName);
	strcpy(tapeLabel->comment,"DCE DFS Backup Software");
	tapeLabel->creationTime = tp.tv_sec;
	tapeLabel->size = size;
	/* for now, set expiration date to 0, since otherwise they never
	   expire */
	tapeLabel->expirationDate = 0;	/* 1970 sometime */
	tapeLabel->dumpPath[0] = 0;	/* no path name */
}

/*
 * Extract volume trailers from tape blocks. Return non-zero if volume
 * trailer is located.
 *
 * This routine looks for a valid volume trailer at the end
 * of a fileset. A volume trailer is written to indicate the end
 * of each fileset on the tape.
 * 
 * The volume trailer may span block boundaries. Each full
 * block is checked for trailer at its end. A full block with a valid volume
 * trailer at the end indicates the end of a fileset. Partial blocks also
 * indicate the end of the fileset. In a partial block, the last vhSize
 * bytes of data should be a volume trailer.
 *
 * Paramaters:
 *    prevBuf    IN  second to last tape block, if lastBuf is not
 *                   full, it may contain part of the volume trailer.
 *    prevSize   IN  Size of second to last tape block.
 *    prevBytes  OUT Number of data bytes in prevBuf.
 *    lastBuf    IN  Last tape block before FILE_END
 *    lastSize   IN  Size of last tape block.
 *    lastBytes  OUT Number of data bytes in lastBuf.
 *    volTrailer OUT Volume trailer from data blocks.
 */
int ExtractTrailer(prevBuf,prevSize,prevBytes,lastBuf,
		   lastSize,lastBytes,volTrailer)
char *prevBuf;
long prevSize;
long *prevBytes;
char *lastBuf;
long lastSize;
long *lastBytes;
struct volumeHeader *volTrailer;
{
	long prevCnt;
	long lastCnt;
	int result;

	result = 0;

	/* check whether trailer spans block boundaries */
	if ( lastSize < sizeof(struct volumeHeader) ) {
	    prevCnt = sizeof(struct volumeHeader) - lastSize;
	    lastCnt = lastSize;
	} else {
	    prevCnt = 0;
	    lastCnt = sizeof(struct volumeHeader);
	}

	/* impossible for trailer to span three blocks */
	if ( prevCnt > prevSize ) return 0;

	/* Copy trailer into temp area */
	if ( prevCnt )
	    bcopy(&prevBuf[prevSize-prevCnt],(char *)volTrailer,prevCnt);
	if ( lastCnt )
	    bcopy(&lastBuf[lastSize-lastCnt],(char *)volTrailer+prevCnt,
		  lastCnt);

	/* check for valid preamble and postamble */
	if (!strncmp(volTrailer->postamble,"T--NAME#",8) &&
	    !strncmp(volTrailer->preamble,"H++NAME#",8)) {
	    prevSize -= prevCnt;
	    lastSize -= lastCnt;
	    result = 1;
	}

	if ( prevBytes ) *prevBytes = prevSize;
	if ( lastBytes ) *lastBytes = lastSize;
	return result;
}

/* Returns true or false depending on whether the tape is expired or not */

Date ExpirationDate (tapeLabelPtr)
    struct butm_tapeLabel *tapeLabelPtr;
{
    int32                   code;
    Date                    expiration = 0;
    struct budb_dumpEntry   dumpEntry;
    struct budb_tapeEntry   tapeEntry;
    struct budb_volumeEntry volEntry;

    if (tapeLabelPtr->dumpid)
    {
        /*
	 * Get the expiration date from DB if its there.
	 */
        code = bcdb_FindTape(tapeLabelPtr->dumpid, tapeLabelPtr->name,
			     &tapeEntry);
	if (!code) expiration = tapeEntry.expires;
    }
    return(expiration);
}

int
tapeExpired (tapeLabelPtr)
    struct butm_tapeLabel *tapeLabelPtr;
{
    Date   expiration;
    struct timeval          tp;
    struct timezone         tzp;

    expiration = ExpirationDate(tapeLabelPtr);
    if (!expiration) expiration = tapeLabelPtr->expirationDate;

    osi_gettimeofday (&tp, &tzp);
    return ((expiration < tp.tv_sec) ? 1 : 0);
}


/* updateTapeLabel
 *	given the label on the tape, delete any old information from the
 *	database.

 * Deletes all entries that match the volset.dumpnode
 *	and the dump path.
 */

updateTapeLabel(labelIfPtr, TapeInfo, newLabelPtr)
     struct labelTapeIf *labelIfPtr;
     struct butm_tapeInfo *TapeInfo;
     struct butm_tapeLabel *newLabelPtr;
{
     struct butm_tapeLabel *oldLabel = NULL;
     struct timeval tp;
     struct timezone tzp;
     char *cptr;
     long dismountCode = 0;
     long code = 0;
     int tapecount = 1;
     int interactiveFlag;
     int tapeIsLabeled = 0;

     interactiveFlag = autoQuery;
     dfprintf((stdout, "updateTapeLabel: mounting tape\n"));

     while (1) {
	  if ( interactiveFlag ) {
	       code = PromptForTape(LABELOPCODE, labelIfPtr->label.name,
				    0, labelIfPtr->taskId, tapecount);
	       if (code)
		    goto error_exit;
	  }
	  interactiveFlag = 1;
	  tapecount++;

	  if ( checkAbortByTaskId(labelIfPtr->taskId) )
	  {
	       /* abort requested */
	       setStatus(labelIfPtr->taskId, ABORT_DONE);
	       goto error_exit;
	  }

	  /* mount the tape */
	  code = butm_Mount(TapeInfo, newLabelPtr->name);
	  if( code == BUTM_NOTAPE )
	  {
	       /* no tape loaded or drive offline */
	       dce_svc_printf(BAK_S_NO_TAPE_MSG,globalTapeConfig.device);
	       goto newtape;
	  }
	  else if(code)
	  {
	       dfprintf((stdout, "updateTapeLabel: mount failed, code %lu\n", code));
	       LogError("Mount failed on tape %s\n", code, newLabelPtr->name);
	       return(code);
	  }
	  dfprintf((stdout, "updateTapeLabel: mounting tape successful\n"));

	  oldLabel = (struct butm_tapeLabel *)osi_Alloc (sizeof(struct butm_tapeLabel));
	  if (oldLabel == (struct butm_tapeLabel *)NULL) {
	       LogError("Could not allocate memory for tape %s\n", code,
			newLabelPtr->name);
	       return(TC_NOMEMORY);
	  }
	  osi_gettimeofday(&tp,&tzp);

	  dfprintf((stdout, "updateTapeLabel: reading tape label\n"));
	  code  = butm_ReadLabel(TapeInfo, oldLabel);
	  if (!code) {
	       tapeIsLabeled = 1;
	       if ( !tapeExpired(&oldLabel) ) {
		    if (!queryoperator) {
			 LogTape("This tape has not expired\n");
			 goto newtape;
		    };
		    if ( Ask("This tape has not expired - proceed") == 0 )
			 goto newtape;
	       };
	  };

	  /* If we can't read label, assume none exists and go ahead anyway */
	  /* extract useful information from the old label */
	  if ( tapeIsLabeled && oldLabel->structVersion >= TAPE_VERSION_1 )
	  {
	       newLabelPtr->dumpid = 0;
	       newLabelPtr->useCount = oldLabel->useCount + 1;
	  }
	  dfprintf((stdout, "updateTapeLabel: creating new label\n"));
	  /* now write the new label */
	  code = butm_Create(TapeInfo, newLabelPtr);
	  if(code)
	  {
	       dce_svc_printf(BAK_S_COULD_NOT_LABEL_TAPE_MSG, newLabelPtr->name,
			      dfs_dceErrTxt(code));
	       LogError("updateTapeLabel: Could not label tape %s\n",
			code, newLabelPtr->name);
	       ERROR_EXIT(code);
	  }
	  dfprintf((stdout, "updateTapeLabel: creating new label successful\n"));
	  break;

     newtape:
	  UnmountTape(labelIfPtr->taskId, TapeInfo);
     }

     if ( tapeIsLabeled && oldLabel->structVersion >= TAPE_VERSION_1 )
     {
	  int i;
	  /* delete based on dump id */

	  if ( oldLabel->dumpid ) {
	       i = bcdb_deleteDumpById(oldLabel->dumpid);
	       if ( i && (i != BUDB_NOENT) ){
		    LogError("Warning: Can't delete old dump %lu from database\n", oldLabel->dumpid);
		    fprintf(stderr,
			    "Warning: Can't delete old dump %lu from database\n", oldLabel->dumpid);
	       }
	  }

     }

 error_exit:

     if (oldLabel != (struct butm_tapeLabel *) NULL)
	  (void)osi_Free(oldLabel, sizeof(struct butm_tapeLabel));
     dfprintf((stdout, "updateTapeLabel: dismounting tape\n"));
     dismountCode = butm_Dismount(TapeInfo);
     if(dismountCode)
     {
	 dce_svc_printf(BAK_S_FAILED_DISMOUNT_TAPE_MSG, newLabelPtr->name, dfs_dceErrTxt(dismountCode));
	 LogError("updateTapeLabel: Failed to dismount tape %s\n",
		  dismountCode, newLabelPtr->name);
	 return(dismountCode);
     }
     return(code);
}

/* Labeller
 *	thread created by the server stub. Labels the tape with name and size
 *	specified by <label>
 */

Labeller( labelIfPtr )
     struct labelTapeIf *labelIfPtr;
{
     struct tc_tapeLabel *label = &(labelIfPtr->label);
     statusP ptr;
     struct butm_tapeLabel *newTapeLabel;
     long Time;
     struct butm_tapeInfo *TapeInfo;
     long code = 0;
     extern statusP findStatus();

     dfprintf((stdout, "Labeller: task %d labelling tape as %s\n", labelIfPtr->taskId, label->name));
     newTapeLabel = (struct butm_tapeLabel *)osi_Alloc (sizeof(struct butm_tapeLabel));
     if(newTapeLabel == (struct butm_tapeLabel *)NULL) {
	 LogError("Can't allocate memory for labelling tape %s\n",
		  code, label->name);
	 return(TC_NOMEMORY);
     }
     TapeInfo = (struct butm_tapeInfo *)osi_Alloc (sizeof(struct butm_tapeInfo));
     if(TapeInfo == (struct butm_tapeInfo *)NULL) {
	 LogError("Can't allocate memory for labelling tape %s\n",
		  code, label->name);
	 (void)osi_Free(newTapeLabel, sizeof(struct butm_tapeLabel));
	 return(TC_NOMEMORY);
     }
     EnterDeviceQueue(deviceLatch);
     clearStatus(labelIfPtr->taskId, DRIVE_WAIT);

     bzero((char *)TapeInfo,sizeof(struct butm_tapeInfo));
     TapeInfo->structVersion = CUR_BUTM_VERSION;
     code = butm_file_Instantiate (TapeInfo, &globalTapeConfig);
     if (code)
     {
	 LogError("Can't initialize the tape module for labelling tape %s\n",
		  code, label->name);
	 ERROR_EXIT(code);
     }
     bzero((char *)newTapeLabel,sizeof(struct butm_tapeLabel));
     GetNewLabel(TapeInfo,label->name,newTapeLabel);
     if(label->size) newTapeLabel->size = label->size;
     else newTapeLabel->size = globalTapeConfig.capacity;
     lock_ObtainWrite(&statusQueueLock);
     ptr = (statusP)findStatus(labelIfPtr->taskId);
     if (ptr) {
       strcpy((char *)ptr->tapeName, newTapeLabel->name);
       ptr->tapeSize = newTapeLabel->size;
       ptr->tapeSize_ext = 0;
     }
     lock_ReleaseWrite(&statusQueueLock);
     LogTape("Labelling tape %s size %u .. ", newTapeLabel->name,
	     newTapeLabel->size);

     /* This call mounts the device, labels tape, unmounts device */
     code = updateTapeLabel(labelIfPtr, TapeInfo, newTapeLabel);

     if (code == TC_ABORTEDBYREQUEST)
       goto error_exit;

     if (code)
       ERROR_EXIT(code);
     if(!strcmp("", (char *)label->name))
	 dce_svc_printf(BAK_S_LABELLED_TAPE_NULL_MSG,newTapeLabel->size);
     else
	 dce_svc_printf(BAK_S_LABELED_TAPE_MSG,label->name,newTapeLabel->size);
     LogTape("Done\n");

error_exit:
     if ( code )
	 setStatus(labelIfPtr->taskId, TASK_ERROR);
     setStatus(labelIfPtr->taskId, TASK_DONE);
     LeaveDeviceQueue(deviceLatch);
     (void)osi_Free(labelIfPtr, sizeof(struct labelTapeIf));
     (void)osi_Free(newTapeLabel, sizeof(struct butm_tapeLabel));
     (void)osi_Free(TapeInfo, sizeof(struct butm_tapeInfo));
     return(code);
}


/* ReadLabel
 * 	Read the label from a tape.
 *	Currently prints out a "detailed" summary of the label but passes
 *	back only selected fields.
 */

ReadLabel(ptr)
     unsigned32 *ptr;
{
     struct butm_tapeLabel *newTapeLabel=0;
     struct butm_tapeInfo *TapeInfo=0;
     statusP statusPtr;
     int tapeMounted=0;
     long Time;
     long code = 0;
     unsigned long taskId;
     extern statusP findStatus();
     int interactiveFlag;
     int tapecount = 1;

     taskId = (unsigned long)*ptr;
     dfprintf((stdout, "ReadLabel: task %d reading label\n", taskId));
     newTapeLabel = (struct butm_tapeLabel *)osi_Alloc(sizeof(struct butm_tapeLabel));
     if (newTapeLabel == (struct butm_tapeLabel *)NULL)
	  return (TC_NOMEMORY);
     TapeInfo =  (struct butm_tapeInfo *)osi_Alloc(sizeof(struct butm_tapeInfo));
     if (TapeInfo == (struct butm_tapeInfo *)NULL) {
	  (void)osi_Free(newTapeLabel, sizeof(struct butm_tapeLabel));
	  return (TC_NOMEMORY);
     }
     EnterDeviceQueue(deviceLatch);
     clearStatus(taskId, DRIVE_WAIT);


     bzero((char *)TapeInfo,sizeof(struct butm_tapeInfo));
     TapeInfo->structVersion = CUR_BUTM_VERSION;
     dfprintf((stdout, "ReadLabel: instantiate butm info structure\n"));
     code = butm_file_Instantiate (TapeInfo, &globalTapeConfig);
     if(code) {
	  LogError("Can't initialize the tape module for reading tape \n",code);
	  goto error_exit;
     }
     bzero((char *)newTapeLabel,sizeof(struct butm_tapeLabel));
     interactiveFlag = autoQuery;
     LogTape("Reading tape label .. ");

     while (1) {

	  if ( interactiveFlag ) {
	       code = PromptForTape(READLABELOPCODE, "", 0, taskId, tapecount);
	       if (code) {
		    fprintf(stderr, "Can't get tape\n");
		    LogError("Can't get tape\n");
		    goto error_exit;
	       }
	  }			/* if (interactiveFlag) */

	  interactiveFlag = 1;
	  tapecount++;

	  if (checkAbortByTaskId(taskId)) {
	       setStatus(taskId, ABORT_DONE);
	       goto newtape;
	  }
	  code = butm_Mount(TapeInfo,"");
	  if ( code == BUTM_NOTAPE )
	  {
	       /* no tape loaded or drive offline */
	       dce_svc_printf(BAK_S_NO_TAPE_MSG,globalTapeConfig.device);
	       goto newtape;
	  }
	  else if(code) {
	       LogError("Mount failed on tape \n",code);
	       fprintf(stderr, "Mount failed on tape \n",code);
	       goto error_exit;
	  }
	  break;		/* go out of while loop */

     newtape:
	  UnmountTape(taskId, TapeInfo);
     }				/* while */


     dfprintf((stdout, "ReadLabel: success mounting tape\n"));
     tapeMounted=1;
     code = butm_ReadLabel(TapeInfo, newTapeLabel);
     if(code) {
	  fprintf(stderr, "Could not read tape label; %s\n",
		  dfs_dceErrTxt(code));
	  LogError("Could not read tape label, was the tape ever labelled?",
		   code);
	  goto error_exit;
     }
     dfprintf((stdout, "ReadLabel: success reading tape label\n"));
     lock_ObtainRead(&statusQueueLock);
     statusPtr = findStatus(taskId);
     strcpy(statusPtr->tapeName, newTapeLabel->name);
     statusPtr->tapeSize = newTapeLabel->size;
     statusPtr->tapeSize_ext = 0;
     lock_ReleaseRead(&statusQueueLock);
     PrintTapeLabel(newTapeLabel);
     LogTape("Done\n");

 error_exit:
     if (code)
	  setStatus(taskId, TASK_ERROR);
     setStatus(taskId, TASK_DONE);
     dfprintf((stdout, "ReadLabel: dismounting tape, code %d\n", code));
     if ( tapeMounted )  {
	  if(butm_Dismount(TapeInfo))
	       LogError("ReadLabel: tape dismount failed\n", 0);
	  else
	       dfprintf((stdout, "ReadLabel: dismount successful\n"));
	  UnmountTape(taskId, TapeInfo);

     }
     LeaveDeviceQueue(deviceLatch);
     osi_Free(newTapeLabel, sizeof(struct butm_tapeLabel));
     osi_Free(TapeInfo, sizeof(struct butm_tapeInfo));
     return(code);
}

/* compute the absolute expiration date */

long
calcExpirationDate(expType, expDate, createTime)
     long expType;
     long expDate;
     long createTime;
{
    struct ktime_date kd;
    long  Add_RelDate_to_Time();

    switch ( expType )
    {
      case BC_REL_EXPDATE:
 	/* expiration date is relative to the creation time of the dump.
	 * This is the only case that requires any work
	 */
	LongTo_ktimeRelDate(expDate, &kd);
	return(Add_RelDate_to_Time(&kd, createTime));
	break;

      case BC_ABS_EXPDATE:
	return(expDate);
	break;

      case BC_NO_EXPDATE:
      default:
	return(0);
    }
}
