/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tcprocs.c,v $
 * Revision 1.1.73.1  1996/10/02  17:06:18  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:30  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1996, 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * tcprocs.c -- BUTC RPC manager
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */


/* procedures invoked by the rpc stub */

#include <dcedfs/param.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/signal.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <dirent.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/lock.h>
#include <dcedfs/nubik.h>
#include <dcedfs/tcdata.h>
#include <error_macros.h>
#include <dcedfs/bc.h>
#include <dcedfs/common_data.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/butc/tcprocs.c,v 1.1.73.1 1996/10/02 17:06:18 damon Exp $")

#define dfprintf(x)

/* 
 * XXXX
 * Need to protect the butc against malicious client thread cancels.
 * Add code in such a way that genuine cancels from bak are not affected.
 */


extern int debugLevel;
extern struct deviceSyncNode *deviceLatch;
static int CopyRestoreDesc(), CopyDumpDesc(), CopyTapeSetDesc();
/* -------------------------
 * butc - interface routines - alphabetic order
 * -------------------------
 */

error_status_t
BUTC_AbortDump(acid, dumpID)
rpc_binding_handle_t acid;
long dumpID;
{
    struct dumpNode *myDumpNode;
    long code;
    int gotLock = 0;
    char *pid;
    pthread_t watchDog;
    pthread_attr_t threadAttr;
    extern void *WatchDog();

    /* get the node from global list of dumpNodes */
    code = GetNode(dumpID, &myDumpNode);
    if(code) return code; /* no node found ! */
    lock_ObtainWrite(&(myDumpNode->lock));
    gotLock = 1;
     if(myDumpNode->status & INPROGRESS) {
	myDumpNode->status |= ABORT_FLAG; /* tell the thread to abort */
	/* tell the thread that someone is waiting for it to wind up */
	myDumpNode->status |= TC_WAITING;
	lock_ReleaseWrite(&(myDumpNode->lock));
	gotLock = 0;
	/* create & init attr values needed by the threads */
	if (pthread_attr_create(&threadAttr) != 0) 
	{
	    dce_svc_printf(BAK_S_ERROR_CREATING_THREAD_ATTRIBUTE_MSG);
	    return(EINVAL);
	}
	if (pthread_attr_setstacksize(&threadAttr,TC_DEFAULT_STACK_SIZE) != 0) 
	{
	    dce_svc_printf(BAK_S_ERROR_SETTING_STACK_SIZE_MSG,
			   TC_DEFAULT_STACK_SIZE);
	    return (EINVAL);
	}
	/* We have to grab the lock before starting up WatchDog, otherwise
	   we may miss the wakeup */
	lock_ObtainWrite(&(myDumpNode->lock));
	code = pthread_create(&watchDog, threadAttr,
			      (pthread_startroutine_t)WatchDog, 
			      (pthread_addr_t)myDumpNode);
	if(code) 
	  return code;
	osi_SleepW((opaque)&(myDumpNode->lock), 
		     (osi_dlock_t *)&(myDumpNode->lock));
	gotLock = 0;
    }
    if(!gotLock) {/*would I be able to get a lock if the process is hung after locking ?*/
        lock_ObtainWrite(&(myDumpNode->lock));
	gotLock = 1;
    }
    if((myDumpNode->status & ABORTED) || (myDumpNode->status & DONE) ||
       (myDumpNode->status & ERROR_FLAG)){
        lock_ReleaseWrite(&(myDumpNode->lock));
	FreeNode(dumpID);
	return 0;
    }
    else {
        lock_ReleaseWrite(&(myDumpNode->lock));
	return TC_ABORTFAILED;	/* abort is queued, at least */
    }
}

error_status_t
BUTC_LabelTape(acid, label, taskId)
rpc_binding_handle_t acid;
     tc_tapeLabel *label;
     unsigned long *taskId;
{
    char *pid;
    tc_tapeLabel *mylabel;
    struct labelTapeIf *ptr;
    statusP statusPtr;
    long code;
    pthread_t labeller;
    pthread_attr_t threadAttr;

    extern void *Labeller();
    extern statusP createStatusNode();
    extern long allocDumpId();
    long len;

    ptr = (struct labelTapeIf *) osi_Alloc(sizeof(struct labelTapeIf));
    if ( ptr == (struct labelTapeIf *)NULL )
        return(TC_INTERNALERROR);
    ptr->label = *label;	/* structure assignment */

    /* set up the status node */
    *taskId = allocDumpId();                    /* for bucoord */
    ptr->taskId = *taskId;

    /* LabelTape does not create a dump node. So we go ahead and grab the
       status q lock. Make sure this never tries to grab a node lock. */
    statusPtr = createStatusNode(TC_REFCOUNT);
    lock_ObtainWrite(&(statusQueueLock));
    statusPtr->taskId = *taskId;
    strcpy((char *)statusPtr->taskName, "Labeltape");
    statusPtr->lastPolled = time(0);
    statusPtr->opType = TCOP_LABELTAPE;

    strncpy((char *)statusPtr->tapeName, (char *)label->name, BU_MAXTAPELEN);

    if ((size_t)(strlen((const char *)label->name) + 1) > BU_MAXTAPELEN)
      statusPtr->tapeName[BU_MAXTAPELEN] = '\0';

    statusPtr->tapeSize = label->size;
    statusPtr->tapeSize_ext = label->size_ext;
    lock_ReleaseWrite(&(statusQueueLock));

    setStatus(statusPtr->taskId, DRIVE_WAIT);
    clearStatus(statusPtr->taskId, STARTING);           /* ok to examine */

    /* create the thread to do the real work behind the scenes */
    /* create & init attr values needed by the threads */
    if ((code = pthread_attr_create(&threadAttr)) != 0) {
      dce_svc_printf(BAK_S_BUTC_ERROR_CREATING_THREAD_ATTR_MSG, code, dfs_dceErrTxt(TC_INTERNALERROR));
      LogError("BUTC_LabelTape: error creating thread attribute\n",
               TC_INTERNALERROR);
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      (void)osi_Free(ptr, sizeof(struct labelTapeIf));
      return(TC_INTERNALERROR);
    }
     if ((code = pthread_attr_setstacksize(&threadAttr,
                                          TC_DEFAULT_STACK_SIZE)) != 0) {
      dce_svc_printf(BAK_S_BUTC_ERROR_SETTING_STACK_SIZE_MSG, code, dfs_dceErrTxt(TC_INTERNALERROR));
      LogError("BUTC_LabelTape: error setting stack size\n", TC_INTERNALERROR);
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      (void)osi_Free(ptr, sizeof(struct labelTapeIf));
      return (TC_INTERNALERROR);
    }
    dfprintf(("BUTC_LabelTape: task %s:%d start labelling \n", statusPtr->taskName, statusPtr->taskId));
    code = pthread_create(&labeller, threadAttr,
                          (pthread_startroutine_t)Labeller,
                          (pthread_addr_t)ptr);
    if (code != 0) {
      dce_svc_printf(BAK_S_BUTC_ERROR_CREATING_LABBEL_TH_MSG, code, dfs_dceErrTxt(TC_INTERNALERROR));
      LogError("BUTC_LabelTape: error creating labeller thread\n",
               TC_INTERNALERROR);
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      (void)osi_Free(ptr, sizeof(struct labelTapeIf));
      return (TC_INTERNALERROR);
    }
    lock_ObtainWrite(&statusQueueLock);
    dfprintf(("BUTC_LabelTape: ref count before deleting is %d\n", statusPtr->re
fCount));
    deleteStatusNode(statusPtr);
    dfprintf(("BUTC_LabelTape: ref count after deleting is %d\n", statusPtr->ref
Count));
    lock_ReleaseWrite(&statusQueueLock);
    return (0);
}

/* BUTC_PerformDump
 *	Tape coordinator server routine to do a dump
 */

error_status_t
BUTC_PerformDump(acid, tcdiPtr, tc_dumpArrayPtr, dumpID)
rpc_binding_handle_t acid;
     struct tc_dumpInterface *tcdiPtr;
     tc_dumpArray *tc_dumpArrayPtr;
     long *dumpID;
{
    struct dumpNode *newNode;
    statusP	statusPtr;
    long code = 0;
    pthread_t dumper;
    pthread_attr_t threadAttr;
    extern void *Dumper();
    extern statusP createStatusNode();

    /* should be verifying parameter validity */

    /* this creates a node in list, alots an id for it and prepares it for
     * locking
     */
    CreateNode(&newNode); /* newNode->lock is held on return */
    /*set up the parameters in the node, to be used by thread */
    strcpy(newNode->dumpSetName, (char *)tcdiPtr->dumpName);
    newNode->dumpName = (char *) osi_Alloc(strlen((char *)tcdiPtr->dumpPath)+1);
    strcpy(newNode->dumpName, (char *)tcdiPtr->dumpPath);
    newNode->volumeSetName=(char *)osi_Alloc(strlen((char *)tcdiPtr->volumeSetName)+1);
    strcpy(newNode->volumeSetName, (char *)tcdiPtr->volumeSetName);
    CopyTapeSetDesc(&(newNode->tapeSetDesc), &tcdiPtr->tapeSet);
    /* 
     * If there are no filesets to dump (tc_dumpArrayPtr->tc_dumpArray_len==0),
     * we don't want to call osi_Alloc with a size of zero.  This will cause
     * an assertion failure later in osi_Free.  Just leave newNode->dumps as
     * nil.
     */
    if (tc_dumpArrayPtr->tc_dumpArray_len > 0) {
        newNode->dumps = (struct tc_dumpDesc *)osi_Alloc(sizeof(struct tc_dumpDesc) * tc_dumpArrayPtr->tc_dumpArray_len);
        CopyDumpDesc(newNode->dumps, (tc_dumpArray *)tc_dumpArrayPtr);
    }
    newNode->arraySize = tc_dumpArrayPtr->tc_dumpArray_len;
    *dumpID = newNode->dumpID;
    newNode->opcode = DUMP;
    newNode->parent = tcdiPtr->parentDumpId;
    newNode->level = tcdiPtr->dumpLevel;

    /* create the status node */
    statusPtr = createStatusNode(TC_REFCOUNT);
    lock_ObtainWrite(&statusQueueLock);
    statusPtr->taskId = newNode->dumpID;
    strncpy(statusPtr->taskName, "Dump", sizeof(statusPtr->taskName));
    statusPtr->lastPolled = time(0);
    statusPtr->opType = TCOP_DUMP;
    newNode->statusNodePtr = statusPtr;    
    lock_ReleaseWrite(&statusQueueLock);
    setStatus(newNode->dumpID, DRIVE_WAIT);
    clearStatus(newNode->dumpID, STARTING);	/* ok to examine */

    /* create the thread to do the real work behind the scenes */
    /* create & init attr values needed by the threads */
    if (pthread_attr_create(&threadAttr) != 0) {
     dce_svc_printf(BAK_S_BUTC_PD_ERROR_CR_THREAD_MSG);
     lock_ReleaseWrite(&(newNode->lock)); /* grabbed in CreateNode */
     lock_ObtainWrite(&statusQueueLock);
     deleteStatusNode(statusPtr);
     lock_ReleaseWrite(&statusQueueLock);
     return(EINVAL);
    }
    if (pthread_attr_setstacksize(&threadAttr, TC_DEFAULT_STACK_SIZE) != 0) {
      dce_svc_printf(BAK_S_BUTC_PD_ERROR_SETTING_STACK_SIZE_MSG,
		     TC_DEFAULT_STACK_SIZE);
      lock_ReleaseWrite(&(newNode->lock)); /* grabbed in CreateNode */
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      return (EINVAL);
    }
    code = pthread_create(&dumper, threadAttr,
			  (pthread_startroutine_t)Dumper, 
			  (pthread_addr_t)newNode);
    lock_ReleaseWrite(&(newNode->lock)); /* grabbed in CreateNode */
    if ( code )
      /* error exit - failed to create thread to do the dump. Deallocate the
       * node and return the error code
       */
      FreeNode(newNode->dumpID);
    lock_ObtainWrite(&statusQueueLock);
    deleteStatusNode(statusPtr);
    lock_ReleaseWrite(&statusQueueLock);
    return(code);
}

error_status_t
BUTC_PerformRestore(acid,  ndumpSetName, arestores, dumpID)
rpc_binding_handle_t acid;
     butcNameString_t *ndumpSetName;
     tc_restoreArray *arestores;
     long *dumpID;
{
    struct dumpNode *newNode;
    statusP statusPtr;
    long code = 0;
    char *dumpSetName;
    extern void *Restorer();
    extern statusP createStatusNode();
    pthread_t restorer;
    pthread_attr_t threadAttr;

    *dumpID = 0L;
    dumpSetName = (char *)osi_Alloc(strlen((char *)ndumpSetName)+1);
    if (dumpSetName == (char *)NULL)
      return (TC_NOMEMORY);
    strcpy(dumpSetName, (char *)ndumpSetName);
    /* should  verify parameter validity */
    CreateNode(&newNode);/* this creates a node in list, alots an id for it and
			    prepares it for locking, newNode->lock held */
    /*set up the parameters in the node, to be used by thread */
    strcpy(newNode->dumpSetName,dumpSetName);
    newNode->restores=(struct tc_restoreDesc *)osi_Alloc(sizeof(struct tc_restoreDesc) * arestores->tc_restoreArray_len);
    newNode->arraySize = arestores->tc_restoreArray_len;
    (void)CopyRestoreDesc(newNode->restores,(tc_restoreArray *)arestores);
    *dumpID = newNode->dumpID;
    newNode->opcode = RESTORE;
    /* should log the intent */

    /* create the status node */
    statusPtr = createStatusNode(TC_REFCOUNT);
    lock_ObtainWrite(&statusQueueLock);
    statusPtr->taskId = newNode->dumpID;
    strncpy(statusPtr->taskName, "Restore", sizeof(statusPtr->taskName));
    statusPtr->lastPolled = time(0);
    statusPtr->opType = TCOP_RESTORE;
    newNode->statusNodePtr = statusPtr;
    lock_ReleaseWrite(&statusQueueLock);
    setStatus(newNode->dumpID, DRIVE_WAIT);
    clearStatus(newNode->dumpID, STARTING);     /* ok to examine */

    /* create the thread to do the real work behind the scenes */
    /* create & init attr values needed by the threads */
    if (pthread_attr_create(&threadAttr) != 0) {
      dce_svc_printf(BAK_S_BUTC_PR_ERROR_CR_THR_ATTR_MSG);
      osi_Free(dumpSetName, strlen(dumpSetName)+1);
      lock_ReleaseWrite(&(newNode->lock));
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      return(EINVAL);
    }
    if (pthread_attr_setstacksize(&threadAttr, TC_DEFAULT_STACK_SIZE) != 0) {
      dce_svc_printf(BAK_S_BUTC_PR_ERROR_SET_ST_SZ_MSG, TC_DEFAULT_STACK_SIZE);
      osi_Free(dumpSetName, strlen(dumpSetName)+1);
      lock_ReleaseWrite(&(newNode->lock));
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      return (EINVAL);
    }
    code = pthread_create(&restorer, threadAttr,
			  (pthread_startroutine_t)Restorer, 
			  (pthread_addr_t)newNode);
    lock_ReleaseWrite(&(newNode->lock));
    osi_Free(dumpSetName, strlen(dumpSetName)+1);
    if (code)
      FreeNode(newNode->dumpID);
    lock_ObtainWrite(&statusQueueLock);
    deleteStatusNode(statusPtr);
    lock_ReleaseWrite(&statusQueueLock);
    return(code);
}

error_status_t
BUTC_ReadLabel(acid, taskId)
  rpc_binding_handle_t acid;
  unsigned long *taskId;
{
    char *pid;
    long code;
    statusP statusPtr;
    unsigned32 *ptr;
    pthread_t readlabel;
    pthread_attr_t threadAttr;
    extern int ReadLabel();
    extern long allocDumpId();
    extern statusP createStatusNode();

    *taskId = 0;
    ptr = (unsigned32 *) osi_Alloc(sizeof(unsigned32));
    if ( ptr == 0 )
	return(TC_NOMEMORY);
    /* set up the status node */
    *ptr = *taskId = allocDumpId();			/* for bucoord */

    /* ReadLabel does not create a dump node. So we go ahead and grab the
       status q lock. Make sure this never tries to grab a node lock. */
    statusPtr = (statusP)createStatusNode(TC_REFCOUNT);
    lock_ObtainWrite(&(statusQueueLock));
    statusPtr->taskId = *taskId;
    strcpy((char *)statusPtr->taskName, "ReadLabel");
    statusPtr->lastPolled = time(0);
    statusPtr->opType = TCOP_READLABEL;
    lock_ReleaseWrite(&(statusQueueLock));
    setStatus(statusPtr->taskId, DRIVE_WAIT);
    clearStatus(statusPtr->taskId, STARTING);		/* ok to examine */

    /* create the thread to do the real work behind the scenes */
    /* create & init attr values needed by the threads */
    if ((code = pthread_attr_create(&threadAttr)) != 0) {
      dce_svc_printf(BAK_S_BUTC_RL_ERROR_CR_THR_ATTR_MSG, code, dfs_dceErrTxt(TC_INTERNALERROR));
      LogError("BUTC_ReadLabel: error creating thread attribute\n", 
	       TC_INTERNALERROR);
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      return(TC_INTERNALERROR);
    }
    if ((code = pthread_attr_setstacksize(&threadAttr, 
					  TC_DEFAULT_STACK_SIZE)) != 0) {
      dce_svc_printf(BAK_S_BUTC_RL_ERROR_SET_STACK_SIZE_MSG, code, dfs_dceErrTxt(TC_INTERNALERROR));
      LogError("BUTC_ReadLabel: error setting stack size\n", TC_INTERNALERROR);
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      return (TC_INTERNALERROR);
    }
    dfprintf(("BUTC_ReadLabel: task %s:%d start reading label\n", statusPtr->taskName, statusPtr->taskId));
    code = pthread_create(&readlabel, threadAttr,
			  (pthread_startroutine_t)ReadLabel, 
			  (pthread_addr_t)ptr);
    if (code != 0) {
      dce_svc_printf(BAK_S_BUTC_RL_START_READ_LABEL_MSG, code, dfs_dceErrTxt(TC_INTERNALERROR));
      LogError("BUTC_ReadLabel: error creating readlabel thread\n", 
	       TC_INTERNALERROR);
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      return (TC_INTERNALERROR);
    }
    lock_ObtainWrite(&statusQueueLock);
    dfprintf(("BUTC_ReadLabel: ref count before deleting is %d\n", statusPtr->refCount));
    deleteStatusNode(statusPtr);
    dfprintf(("BUTC_ReadLabel: ref count after deleting is %d\n", statusPtr->refCount));
    lock_ReleaseWrite(&statusQueueLock);
    return code;
}

/* BUTC_RestoreDb
 *	restore the backup database from tape
 */

error_status_t
BUTC_RestoreDb(acid, taskId)
rpc_binding_handle_t acid;
unsigned32 *taskId;
{
    char *pid;
    statusP statusPtr;
    long code = 0;
    unsigned32 *restoreDbTaskId;
    pthread_t restoreDb;
    pthread_attr_t threadAttr;
    extern void *restoreDbFromTape();
    extern statusP createStatusNode();
    extern long allocDumpId();
    
    restoreDbTaskId = (unsigned32 *)osi_Alloc(sizeof(unsigned32));
    *taskId = *restoreDbTaskId = allocDumpId();
    /* create the status node */
    statusPtr = createStatusNode(TC_REFCOUNT);
    lock_ObtainWrite(&statusQueueLock);
    statusPtr->taskId = *restoreDbTaskId;
    strncpy(statusPtr->taskName, "RestoreDb", sizeof(statusPtr->taskName));
    statusPtr->lastPolled = time(0);
    statusPtr->opType = TCOP_RESTOREDB;
    lock_ReleaseWrite(&statusQueueLock);
    clearStatus(*restoreDbTaskId, STARTING);     /* ok to examine */
    setStatus(*restoreDbTaskId, DRIVE_WAIT);
    /* create & init attr values needed by the threads */
    if (pthread_attr_create(&threadAttr) != 0) {
      dce_svc_printf(BAK_S_BUTC_RD_ERR_CR_THR_ATTR_MSG);
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      return(EINVAL);
    }
    if (pthread_attr_setstacksize(&threadAttr, TC_DEFAULT_STACK_SIZE) != 0) {
      dce_svc_printf(BAK_S_BUTC_RD_ERR_SETTING_STACK_SIZE_MSG,
		     TC_DEFAULT_STACK_SIZE);
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      return (EINVAL);
    }
    code = pthread_create(&restoreDb, threadAttr,
			  (pthread_startroutine_t)restoreDbFromTape,
			  (pthread_addr_t)restoreDbTaskId);
    lock_ObtainWrite(&statusQueueLock);
    deleteStatusNode(statusPtr);
    lock_ReleaseWrite(&statusQueueLock);
    return(code);
}

/* BUTC_SaveDb
 *	save the backup database to tape
 */

error_status_t
BUTC_SaveDb(acid, taskId)
rpc_binding_handle_t acid;
unsigned32 *taskId;
{
    char *pid;
    statusP statusPtr;
    long code = 0;
    unsigned32 *saveDbTaskId;
    pthread_t saveDb;
    pthread_attr_t threadAttr;
    extern void *saveDbToTape();
    extern statusP createStatusNode();
    extern long allocDumpId();

    saveDbTaskId = (unsigned32 *)osi_Alloc(sizeof(unsigned32));
    *taskId = *saveDbTaskId = allocDumpId();
    /* create the status node */
    statusPtr = createStatusNode(TC_REFCOUNT);
    lock_ObtainWrite(&statusQueueLock);
    statusPtr->taskId = *saveDbTaskId;
    strncpy(statusPtr->taskName, "SaveDb", sizeof(statusPtr->taskName));
    statusPtr->lastPolled = time(0);
    statusPtr->opType = TCOP_SAVEDB;
    lock_ReleaseWrite(&statusQueueLock);
    clearStatus(*saveDbTaskId, STARTING);     /* ok to examine */
    setStatus(*saveDbTaskId, DRIVE_WAIT);

    /* create & init attr values needed by the threads */
    if (pthread_attr_create(&threadAttr) != 0) {
      dce_svc_printf(BAK_S_BUTC_SD_ERR_CR_THR_ATTR_MSG);
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      return(EINVAL);
    }
    if (pthread_attr_setstacksize(&threadAttr, TC_DEFAULT_STACK_SIZE) != 0) {
      dce_svc_printf(BAK_S_BUTC_SD_ERR_SET_STACK_SIZE_MSG, 
		     TC_DEFAULT_STACK_SIZE);
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      return (EINVAL);
    }	
    code = pthread_create(&saveDb, threadAttr,
			  (pthread_startroutine_t)saveDbToTape,
			  (pthread_addr_t)saveDbTaskId);
error_exit:
    lock_ObtainWrite(&statusQueueLock);
    deleteStatusNode(statusPtr);
    lock_ReleaseWrite(&statusQueueLock);
    return(code);
}
	

/* BUTC_ScanDumps
 * 	read a dump (maybe more than one tape), and print out a summary
 *	of its contents. If the flag is set, add to the database.
 * entry:
 *	addDbFlag - if set, the information will be added to the database
 */

error_status_t
BUTC_ScanDumps(acid, addDbFlag, taskId)
rpc_binding_handle_t acid;
     long addDbFlag;
     u_long *taskId;
{
    long pid;
    struct scanTapeIf *ptr;
    statusP statusPtr;
    long code = 0;
    pthread_t thr;
    pthread_attr_t threadAttr;
    extern void *ScanDumps();
    extern long allocDumpId();
    extern statusP createStatusNode();

    *taskId = allocDumpId();

    ptr = (struct scanTapeIf *)osi_Alloc(sizeof(*ptr));
    if ( ptr == 0 )
	return(TC_INTERNALERROR);

    ptr->addDbFlag = addDbFlag;
    ptr->taskId = *taskId;

    /* create the status node */
    statusPtr = createStatusNode(TC_REFCOUNT);
    lock_ObtainWrite(&statusQueueLock);
    statusPtr->taskId = *taskId;
    strncpy(statusPtr->taskName, "Scantape", sizeof(statusPtr->taskName));
    statusPtr->lastPolled = time(0);
    statusPtr->opType = TCOP_SCANTAPE;
    lock_ReleaseWrite(&statusQueueLock);
    clearStatus(taskId, STARTING);     /* ok to examine */
    setStatus(taskId, DRIVE_WAIT);

    /* create & init attr values needed by the threads */
    if (pthread_attr_create(&threadAttr) != 0) {
      dce_svc_printf(BAK_S_BUTC_SCD_ERR_CR_THR_ATTR_MSG);
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      return(EINVAL);
    }
    if (pthread_attr_setstacksize(&threadAttr, TC_DEFAULT_STACK_SIZE) != 0) {
      dce_svc_printf(BAK_S_BUTC_SCD_ERR_SET_ST_SZ_MSG, TC_DEFAULT_STACK_SIZE);
      lock_ObtainWrite(&statusQueueLock);
      deleteStatusNode(statusPtr);
      lock_ReleaseWrite(&statusQueueLock);
      return (EINVAL);
    }
    code = pthread_create (&thr, threadAttr,
			   (pthread_startroutine_t)ScanDumps, 
			   (pthread_addr_t)ptr);
    lock_ObtainWrite(&statusQueueLock);
    deleteStatusNode(statusPtr);
    lock_ReleaseWrite(&statusQueueLock);
    return code;
}

/* BUTC_TCInfo
 *      return information about the tape coordinator. Currently this
 *      is just the version number of the interface
 */
error_status_t
BUTC_TCInfo(acid, tciptr)
rpc_binding_handle_t acid;
     struct tc_tcInfo *tciptr;
{
    tciptr->tcVersion = CUR_BUTC_VERSION;
    return(0);
}

error_status_t
BUTC_GetServerInterfaces (h, serverInterfacesP)
     handle_t h;
     dfs_interfaceList *serverInterfacesP;
{
    error_status_t st;

    dfs_GetServerInterfaces (BUTC_v4_0_s_ifspec, serverInterfacesP, &st);
    return st;
}


/* -----------------------------
 * misc. routines
 * -----------------------------
 */

static int
CopyDumpDesc(toDump, fromDump)
     struct tc_dumpDesc *toDump;
     tc_dumpArray *fromDump;
{
    struct tc_dumpDesc *toPtr, *fromPtr;
    int i;

    toPtr = toDump;
    fromPtr = fromDump->tc_dumpArray_val;
    for(i = 0 ; i < fromDump->tc_dumpArray_len; i++)
    {
	toPtr->vid = fromPtr->vid;
	toPtr->partition = fromPtr->partition;
	toPtr->date = fromPtr->date;
	toPtr->cloneDate = fromPtr->cloneDate;
	toPtr->hostAddr = fromPtr->hostAddr;	/* structure assignment */
	strcpy((char *)toPtr->name, (char *)fromPtr->name);
	fromPtr++;
	toPtr++;
    }
    return 0;
}


static int CopyRestoreDesc(toRestore, fromRestore)
struct tc_restoreDesc *toRestore;
tc_restoreArray *fromRestore;
{
    struct tc_restoreDesc *toPtr, *fromPtr;
    int i;
    
    toPtr = toRestore ;
    fromPtr = fromRestore->tc_restoreArray_val;
    for(i = 0 ; i < fromRestore->tc_restoreArray_len ; i++){
	toPtr->frag = fromPtr->frag;
	toPtr->position = fromPtr->position;
	strcpy((char *)toPtr->tapeName, (char *)fromPtr->tapeName);
	toPtr->origVid = fromPtr->origVid;
	toPtr->vid = fromPtr->vid;
	toPtr->partition = fromPtr->partition;
	toPtr->flags = fromPtr->flags;
	toPtr->hostAddr = fromPtr->hostAddr;	/* structure assignment */
	toPtr->realDumpId = fromPtr->realDumpId;
	strcpy((char *)toPtr->newName, (char *)fromPtr->newName);
	strcpy((char *)toPtr->oldName, (char *)fromPtr->oldName);
	fromPtr++;
	toPtr++;

    }
    return 0;
}

static int CopyTapeSetDesc(toPtr,fromPtr)
struct tc_tapeSet *toPtr,*fromPtr;
{
    
    toPtr->id = fromPtr->id;
    toPtr->maxTapes = fromPtr->maxTapes;
    toPtr->a = fromPtr->a;
    toPtr->b = fromPtr->b;
    strcpy((char *)toPtr->tapeServer, (char *)fromPtr->tapeServer);
    strcpy((char *)toPtr->format, (char *)fromPtr->format);

    toPtr->expDate = fromPtr->expDate;
    toPtr->expType = fromPtr->expType;
    return 0;
}

BUTC_v4_0_epv_t BUTC_v4_0_manager_epv = {
	 BUTC_PerformDump,
	 BUTC_PerformRestore,
	 BUTC_AbortDump,
	 BUTC_LabelTape,
	 BUTC_ReadLabel,
	 BUTC_ScanDumps,
	 BUTC_TCInfo,
	 BUTC_SaveDb,
	 BUTC_RestoreDb,
	 BUTC_EndStatus,
	 BUTC_GetStatus,
	 BUTC_RequestAbort,
	 BUTC_ScanStatus,
	 BUTC_GetServerInterfaces
};
