/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1995, 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * tcstatus.c -- status RPCs
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */

#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/lock.h>
#include <dcedfs/tcdata.h>
#include <dcedfs/butc.h>
#include <error_macros.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/butc/RCS/tcstatus.c,v 4.21 1995/07/26 16:08:13 blake Exp $")

#define dfprintf(x)

/* tape coordinator - task status management */
extern osi_dlock_t statusQueueLock;
dlqlinkT	statusHead;

/* BUTC_GetStatus
 *	get the status of a task
 * entry:
 *	taskId - task for which status required
 * exit:
 *	statusPtr - filled in with task status
 */
error_status_t
BUTC_GetStatus(call, taskId, statusPtr)
  rpc_binding_handle_t call;
     unsigned long taskId;
     tciStatusS *statusPtr;
{
    statusP ptr;
    int retval = 0;

    extern statusP findStatus();

    dfprintf(("BUTC_GetStatus: fetching status for task %d\n",taskId));
    bzero((char *)statusPtr, sizeof(tciStatusS));
    lock_ObtainShared(&statusQueueLock);
    ptr = findStatus(taskId);
    if ( ptr )
    {
        statusPtr->info.opType = ptr->opType;
	statusPtr->flags = ptr->flags;
	strcpy((char *)statusPtr->taskName, ptr->taskName);
	statusPtr->lastPolled = ptr->lastPolled;
        lock_UpgradeSToW(&statusQueueLock);
	ptr->lastPolled = time(0);
	lock_ReleaseWrite(&statusQueueLock);

	switch (ptr->opType) {
	case TCOP_READLABEL:
	case TCOP_LABELTAPE:
	  dfprintf(("BUTC_GetStatus: task %s:%d is op %d label %s size %lu\n", ptr->taskName, taskId, ptr->opType, ptr->tapeName, ptr->tapeSize));
	  statusPtr->info.u.label.tapeLabel.nameLen = strlen(ptr->tapeName)+1;
	  strcpy((char *)statusPtr->info.u.label.tapeLabel.name,ptr->tapeName);
	  statusPtr->info.u.label.tapeLabel.size = ptr->tapeSize;
	  statusPtr->info.u.label.tapeLabel.size_ext = ptr->tapeSize_ext;
	  break;
	case TCOP_DUMP:
	case TCOP_RESTORE:
	  dfprintf(("BUTC_GetStatus: task %s:%d is op %d volname %s nkbytes %lu\n", ptr->taskName, taskId, ptr->opType, ptr->volumeName, ptr->nKBytes));
	  strcpy((char *)statusPtr->info.u.vol.volumeName, ptr->volumeName);
	  statusPtr->info.u.vol.nKBytes = ptr->nKBytes;
	  statusPtr->info.u.vol.volsFailed = ptr->volsFailed;
	  break;
	default:
	  dfprintf(("BUTC_GetStatus: task %s:%d is op %d \n", ptr->taskName, taskId, ptr->opType));
	  break;
	}
    }
    else {
      dfprintf(("BUTC_GetStatus: task %d not found\n", taskId));
	retval = TC_NODENOTFOUND;
	lock_ReleaseShared(&statusQueueLock);
    }

    return(retval);
}
error_status_t
BUTC_EndStatus(call, taskId )
  rpc_binding_handle_t call;
     u_long taskId;
{
    statusP ptr;
    int retval = 0;
    extern statusP findStatus();

    dfprintf(("BUTC_EndStatus: finishing up task %d\n", taskId));
    lock_ObtainShared(&statusQueueLock);
    ptr = findStatus(taskId);
    if ( ptr ) {
        lock_UpgradeSToW(&statusQueueLock);
	dfprintf(("BUTC_EndStatus: setting defunct and deleting task %d\n", taskId));
	ptr->defunct=1;
	dfprintf(("BUTC_EndStatus: task %s:%d refcount before deleting is %d\n", ptr->taskName,taskId, ptr->refCount));
	deleteStatusNode(ptr);
	dfprintf(("BUTC_EndStatus: task %s:%d refcount after deleting is %d\n", ptr->taskName,taskId, ptr->refCount));
	lock_ReleaseWrite(&statusQueueLock);
    }
    else {
      dfprintf(("BUTC_EndStatus: task %d not found\n", taskId));
	retval = TC_NODENOTFOUND;
	lock_ReleaseShared(&statusQueueLock);
    }
    return(retval);
}
error_status_t
BUTC_RequestAbort(call,  taskId )
  rpc_binding_handle_t call;
     unsigned long taskId;
{
    statusP ptr;
    int retval = 0;
    extern statusP findStatus();

    lock_ObtainWrite(&statusQueueLock);
    ptr = findStatus(taskId);
    if ( ptr )
	ptr->flags |= ABORT_REQUEST;
    else
	retval = TC_NODENOTFOUND;
    lock_ReleaseWrite(&statusQueueLock);
    return(retval);
}

/* BUTC_ScanStatus
 *	Get status of all tasks on the butc, successively. Initial call
 *	should come in with TSK_STAT_FIRST flag set to initialize the
 *	scan.
 * entry:
 *	taskId - specifies the task whose status is to be returned
 *		(unless TSK_STAT_FIRST set in which case it is ignored)
 * exit:
 *	taskId - id of next task in the list
 *	flags - TSK_STAT_END will be set when one reaches the end of
 *		the task list. taskId is not updated in this case.
 * return values:
 *	0 - normal
 *	TC_NOTASKS - no tasks active
 */
error_status_t
BUTC_ScanStatus(call, taskId, statusPtr, flags)
  rpc_binding_handle_t call;
     unsigned long *taskId;
     tciStatusS *statusPtr;
     unsigned long *flags;
{
    statusP ptr = 0;
    statusP nextPtr = 0;
    dlqlinkP dlqPtr;
    extern statusP findStatus();

    dfprintf(("BUTC_ScanStatus: scanning status for task %d\n", *taskId));
    bzero((char *)statusPtr, sizeof(tciStatusS));
    lock_ObtainRead(&statusQueueLock);
    if ( *flags & TSK_STAT_FIRST )
    {
	/* find first status node */
	dlqPtr = statusHead.dlq_next;
	if ( dlqPtr == &statusHead )
	{
	    /* no status nodes */
	    lock_ReleaseRead(&statusQueueLock);
	    return(TC_NOTASKS);
	}
	ptr = (statusP) dlqPtr;
    }
    else
    {
	ptr = findStatus(*taskId);
	if ( ptr == 0 )
	{
	  /* in the event that the set of tasks has changed, just
	   * finish, letting the caller retry
	   */
	    dfprintf(("BUTC_ScanStatus: cannot find task %d, retry\n",*taskId));
	    *flags |= (TSK_STAT_NOTFOUND | TSK_STAT_END);
	    lock_ReleaseRead(&statusQueueLock);
            return(TC_BADTASK);
	}
    }

    /* ptr is now set to the status node we wish to return. Determine
     * what the next node will be
     */

    if ( ptr->link.dlq_next == &statusHead )
	*flags |= TSK_STAT_END;
    else
	*taskId = ((statusP) ptr->link.dlq_next)->taskId;

    statusPtr->flags = ptr->flags;
    strcpy((char *)statusPtr->taskName, ptr->taskName);
    statusPtr->lastPolled = ptr->lastPolled;
    statusPtr->info.opType = ptr->opType;
     statusPtr->taskId = ptr->taskId;
    switch (ptr->opType) {
    case TCOP_READLABEL:
    case TCOP_LABELTAPE:
      dfprintf(("BUTC_ScanStatus: task %s:%d label %s, size %lu\n", ptr->taskName, ptr->taskId, ptr->tapeName, ptr->tapeSize));
      strcpy((char *)statusPtr->info.u.label.tapeLabel.name, ptr->tapeName);
      statusPtr->info.u.label.tapeLabel.size = ptr->tapeSize;
      statusPtr->info.u.label.tapeLabel.size_ext = ptr->tapeSize_ext;
      break;
    case TCOP_DUMP:
    case TCOP_RESTORE:
      dfprintf(("BUTC_ScanStatus: task %s:%d volumename %s, nKBytes %lu\n", ptr->taskName, ptr->taskId, ptr->volumeName, ptr->nKBytes));
      strcpy((char *)statusPtr->info.u.vol.volumeName, ptr->volumeName);
      statusPtr->info.u.vol.nKBytes = ptr->nKBytes;
      statusPtr->info.u.vol.volsFailed = ptr->volsFailed;
      break;
    default:
      dfprintf(("BUTC_ScanStatus: task %s:%d \n", ptr->taskName, ptr->taskId));
      break;
    }
    lock_ReleaseRead(&statusQueueLock);
    return(0);
}


/* ---------------------------------
 * misc. status management routines
 * ---------------------------------
 */

/* checkAbortByTaskId
 * exit:
 *	0 - continue
 *	n - abort requested
 */

checkAbortByTaskId(taskId)
     u_long taskId;
{
    statusP statusPtr;
    int retval = 0;

    extern statusP findStatus();

    lock_ObtainRead(&statusQueueLock);
    statusPtr = findStatus(taskId);
    if ( statusPtr )
    {
	retval = statusPtr->flags & ABORT_REQUEST;
    }
    lock_ReleaseRead(&statusQueueLock);
    return(retval);
}

/* getStatusFlag
 * 	For backwards compatibility. Queries flag status
 * exit:
 *	0 - flag clear
 *	n - flag set
 */

u_long
getStatusFlag(nodePtr, flag)
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
        retval = statusPtr->flags & flag;
    }
    lock_ReleaseRead(&statusQueueLock);
    lock_ReleaseRead(&(nodePtr->lock));
    return(retval);
}
