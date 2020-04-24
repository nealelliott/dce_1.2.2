/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bc_status.c,v $
 * Revision 1.1.88.1  1996/10/02  16:57:47  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:42  damon]
 *
 * Revision 1.1.82.4  1994/08/25  17:34:27  mckeen
 * 	Added s12y calls
 * 	[1994/08/25  17:22:53  mckeen]
 * 
 * Revision 1.1.82.3  1994/07/13  22:27:03  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:16:43  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:52:55  mbs]
 * 
 * Revision 1.1.82.2  1994/06/09  13:50:04  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:31  annie]
 * 
 * Revision 1.1.82.1  1994/02/04  20:04:21  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:05:45  devsrc]
 * 
 * Revision 1.1.80.1  1993/12/07  17:11:25  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:42:33  jaffe]
 * 
 * Revision 1.1.6.6  1993/01/18  19:50:17  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:24:04  cjd]
 * 
 * Revision 1.1.6.5  1993/01/13  15:23:07  shl
 * 	Transarc delta: vijay-ot6381-correct-handling-of-device-queue-lock 1.4
 * 	  Selected comments:
 * 
 * 	    The aim of this delta is to get bak restore to succeed. The problems in restore
 * 	    were some spurious thread exceptions and assertions that made bak dump core.
 * 	    Upon investigation it turned out that there were some locking problems in
 * 	    backup and butc. The delta cleans up locking in backup and butc.
 * 	    The delta is not ready for export. Much more needs to be done to get all the
 * 	    locking issues right. This is just a checkpoint.
 * 	    clean up locking in the status node manager.
 * 	    Second set of changes to get locking issues in order. This time the changes
 * 	    are mostly in bakserver. The changes introduced are
 * 	    1. Establish a global lock so that two RPCs do not interfere with each other.
 * 	    This was introduced because there are some global data structures in
 * 	    backup that might get affected. These global structures now have
 * 	    individual locks to serialize changes. The global lock in temporary, and
 * 	    will go away once we have serialized all accesses to all global data
 * 	    structures.
 * 	    2. Disable cancels in bakserver RPCs. Care is taken to not disable cancels in
 * 	    operations that can be cancelled. There is some more work that needs to be
 * 	    done in this area.
 * 	    3. Accesses to the database are controlled by memoryDB.lock. The accesses are
 * 	    by means of dbread and dbwrite. The hash tables too are protected by this
 * 	    mechanism.
 * 	    4. Changes to the backup database dump code to simplify the dump operation. In
 * 	    particular, the Unix pipe mechanism is used to synchronize reader and
 * 	    writer without need for condition variables.
 * 	    5. Get rid of any pthread_mutex and pthread_cond operations. Use afslk and
 * 	    afsos routines instead.
 * 	    6. Continue the work described by the previous revision of the delta in bak
 * 	    and butc. This should be it for changes in bak and butc.
 * 	    Fix compilation problems on the RIOS.
 * 	    This is hopefully the final revision of this delta. The fixes here are
 * 	    1. Changes to the bakserver and butc RPC interfaces.
 * 	    The RPCs that handle variable size arrays now use conformant arrays.
 * 	    This avoids allocating lots of memory on the client thread stack which
 * 	    was the cause of the exceptions in restoreft. The server allocates
 * 	    memory which is freed by the server stub on the way out. The client thread
 * 	    frees the memory allocated by the client stub.
 * 	    2. get database dump and restore to handle status correctly.
 * 	    3. Get the locking hierarchy right in bakserver, bak and butc.
 * 	    4. Other minor nits.
 * 	    There is still a problem with scantape -dbadd that has to be addressed. With
 * 	    this delta, all backup commands should work when used in a straightforward
 * 	    manner. Some error cases are still not handled properly though. Subsequent
 * 	    deltas would fix those.
 * 	[1993/01/12  17:25:18  shl]
 * 
 * Revision 1.1.6.4  1992/12/09  19:19:24  jaffe
 * 	Transarc delta: comer-ot6129-user-level-icl-dumping 1.5
 * 	  Selected comments:
 * 	    Added code to ICL trace logs at user level.  The guinea pig for this
 * 	    change was dfsbind, also modified as part of this delta.  Most of the
 * 	    actual tracing and some of the ICL work was done at IBM by Jean
 * 	    Pehkonen.
 * 	    Force the program to dump its log: kill -30 <pid>
 * 	    Added dfsbind to EXPINC_SUBDIRS
 * 	    In files where both osi.h and compat.h are included, the reference
 * 	    to osi.h must be first.  In each of these files, osi.h was implicitly
 * 	    included, so I added an explicit reference before compat.h.
 * 	    See above.
 * 	    Installed message catalog.
 * 	[1992/12/03  20:42:27  jaffe]
 * 
 * Revision 1.1.6.3  1992/11/24  15:36:56  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:04:31  bolinger]
 * 
 * Revision 1.1.6.2  1992/08/31  18:00:26  jaffe
 * 	Transarc delta: jaffe-ot4300-fix-compilation-warnings 1.2
 * 	  Selected comments:
 * 	    Fix compilation warnings
 * 	    cast arguments to strcpy to (char *)
 * 	[1992/08/28  20:22:55  jaffe]
 * 
 * Revision 1.1.3.5  1992/05/05  00:53:06  mason
 * 	delta vijay-butc-compile-error 1.2
 * 	[1992/04/24  21:08:25  mason]
 * 
 * Revision 1.1.3.4  1992/01/25  20:43:11  zeliff
 * 	dfs6.3 merge, part2
 * 	[1992/01/25  19:32:55  zeliff]
 * 
 * $EndLog$
 */
/*
 * bc_status.c -- bak coordinator status management
 */ 
/*
 * Copyright (C) 1991 Transarc Corporation
 * All Rights Reserved
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
#include <dcedfs/osi.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <sys/types.h>
#include <com_err.h>
#include <dcedfs/tcdata.h>
#include <bc.h>
#include <error_macros.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

#define dfprintf(x)

#define	SET_FLAG(set)				\
    lock_ObtainWrite(&statusQueueLock);	\
    curPollPtr->flags |= (set);			\
    lock_ReleaseWrite(&statusQueueLock);      

#define CLEAR_FLAG(clear)			\
    lock_ObtainWrite(&statusQueueLock);	\
    curPollPtr->flags &= ~(clear);		\
    lock_ReleaseWrite(&statusQueueLock);      

/* globals for bak coordinator status management */
dlqlinkT statusHead;			/* chain of status blocks */

/* nextItem
 *	get next item for status interrogation, if any.
 */
static statusP
nextItem(linkPtr)
     statusP linkPtr;
{
    dlqlinkP	ptr;

    ptr = (dlqlinkP) linkPtr;

    /* if last known item has terminated, reset ptr */
    if ( ptr == (dlqlinkP)NULL )
    {
	ptr = &statusHead;
	if ( dlqEmpty(ptr) )
	    return(0);
    }

    ptr = ptr->dlq_next;

    /* if we're back at the head again */
    if ( ptr ==  &statusHead )
	return(0);
    return((statusP) ptr);
}

statusWatcher()
{
    volatile rpc_binding_handle_t tconn;
    volatile statusP curPollPtr = 0;
    struct timespec delayTime;
    volatile struct tciStatusS statusPtr;

    /* task information */
    volatile u_long taskFlags;
    volatile u_long localTaskFlags;
    u_long temp;			/* for flag manipulation */
    volatile long jobNumber;
    volatile long taskId;
    volatile long port;
	
    error_status_t code = 0;
    extern struct bc_config *bc_globalConfig;
    extern rpc_binding_handle_t bc_GetConn();
    while ( 1 )
    { /*w*/
        lock_ObtainWrite(&statusQueueLock);      
	curPollPtr = nextItem(curPollPtr);
	if (curPollPtr)
	  dfprintf(("StatusWatcher: now handling task %s:%d\n", 
		    curPollPtr->taskName, curPollPtr->taskId));
	if ( curPollPtr == 0 || curPollPtr->defunct || 
	    (curPollPtr->flags & TASK_DONE)) {
	  if (curPollPtr) {
	    if (curPollPtr->flags & TASK_DONE)
	      dfprintf(("StatusWatcher: task %s:%d done, skipping...\n",
			curPollPtr->taskName, curPollPtr->taskId));
	    else
	      dfprintf(("StatusWatcher: task %s:%d defunct, skipping...\n",
			curPollPtr->taskName, curPollPtr->taskId));
	  }
	  /* This node will soon be freed, so reset pointer */
	  curPollPtr = (statusP)NULL;
	  lock_ReleaseWrite(&statusQueueLock);      
	  delayTime.tv_sec = 5;
	  delayTime.tv_nsec = 0;
	  pthread_delay_np(&delayTime);
	  continue;
	}
	/* save useful information */
	lock_ConvertWToS(&statusQueueLock);      
	TRY {
	  localTaskFlags = curPollPtr->flags;
	  taskId = curPollPtr->taskId;
	  port = curPollPtr->port;
	  jobNumber = curPollPtr->jobNumber;
	  dfprintf(("StatusWatcher: task %s:%d, flags %d, port %d, job # %d\n",
		  curPollPtr->taskName,taskId,localTaskFlags, port,jobNumber));
	} FINALLY {
	  lock_ReleaseShared(&statusQueueLock);      
	} ENDTRY
	/* reset certain flags; local kill; */
	CLEAR_FLAG(ABORT_LOCAL);

	if ( localTaskFlags & STARTING )		/* ignore it */
	    continue;
	if ( localTaskFlags & ABORT_LOCAL )
	{
	    /* kill the local task */
	    if ( (localTaskFlags & CONTACT_LOST) != 0 )
	    {
		dce_svc_printf(BAK_S_CONTACT_WITH_BUTC_MSG, jobNumber, port);
		dce_svc_printf(BAK_S_LOCAL_KILL_IGNORED_MSG, jobNumber);
	    }
	}
	tconn = bc_GetConn(bc_globalConfig, port);
	if ( tconn == (rpc_binding_handle_t)NULL ) {
	  SET_FLAG(CONTACT_LOST);
	  continue;
	}
	if ( CheckTCVersion(tconn) ) {
	  SET_FLAG(CONTACT_LOST);
	  bc_FreeConn(bc_globalConfig, tconn);
	  continue;
	}
	CLEAR_FLAG(CONTACT_LOST);

	if ( localTaskFlags & ABORT_REQUEST ) {
	    code = BUTC_RequestAbort(tconn, taskId);
	    if ( IS_COMM_ERROR(code) ) {
		com_err("statusWatcher", code, "; Can't post abort request");
	        com_err("statusWatcher", 0, "Deleting job");
		lock_ObtainWrite(&statusQueueLock);
		TRY {
		  curPollPtr->defunct=1;
		  deleteStatusNode(curPollPtr);
		  curPollPtr = (statusP)NULL;
		} FINALLY {
		  lock_ReleaseWrite(&statusQueueLock);
		} ENDTRY
		bc_FreeConn(bc_globalConfig, tconn);
		continue;
	    }
	    else  {
	      lock_ObtainWrite(&statusQueueLock);
	      TRY {
		curPollPtr->flags &= ~ABORT_REQUEST;
		curPollPtr->flags |= ABORT_SENT;
	      } FINALLY {
		lock_ReleaseWrite(&statusQueueLock);
	      } ENDTRY
	    }
	}

	/* otherwise just get the status */
	dfprintf(("StatusWatcher: calling BUTC_GetStatus on task %s: %d\n",
		  curPollPtr->taskName, taskId));
	bzero((char *)&statusPtr, sizeof(struct tciStatusS));
	code = BUTC_GetStatus(tconn, taskId, (struct tciStatusS *)&statusPtr);
	dfprintf(("StatusWatcher: returning code %d\n", code));
	if ( IS_COMM_ERROR(code) )
	{
	    if ( code == TC_NODENOTFOUND )
	    {
		/* delete this status node */
		dce_svc_printf(BAK_S_NO_SUCH_TASK_MSG, jobNumber, curPollPtr->taskName, port);
		lock_ObtainWrite(&statusQueueLock);
		TRY {
		  /*
		   * We don't have to call releStatusNode because we are going
		   * to delete the node, which would first decrement the refcnt
		   * Anyone else holding a reference to this status node will
		   * soon release it because defunct is set.
		   */
		  curPollPtr->defunct = 1;
		  deleteStatusNode(curPollPtr);
		  curPollPtr = (statusP)NULL;
		} FINALLY {
		  lock_ReleaseWrite(&statusQueueLock);
		} ENDTRY
		bc_FreeConn(bc_globalConfig, tconn);
		continue;
	    }
	    SET_FLAG(CONTACT_LOST);
	    bc_FreeConn(bc_globalConfig, tconn);
	    continue;
	}
	CLEAR_FLAG(CONTACT_LOST);
	/* extract useful status */
	taskFlags = statusPtr.flags;

	/* update local status */
	lock_ObtainWrite(&statusQueueLock);
	TRY {
	  /* update local value of the selected flags (in temp) */
	  temp = (TASK_DONE|DRIVE_WAIT|OPR_WAIT|CALL_WAIT);
	  curPollPtr->flags &= ~temp;			/* clear */
	  curPollPtr->flags |= (taskFlags & temp);	/* update */
	  assert(curPollPtr->opType == statusPtr.info.opType);
	  curPollPtr->lastPolled = statusPtr.lastPolled;
	  switch (statusPtr.info.opType) {
	  case TCOP_DUMP:
	  case TCOP_RESTORE:
	    dfprintf(("StatusWatcher: op %d, volume name %s\n", 
		      statusPtr.info.opType, 
		      statusPtr.info.u.vol.volumeName));	  
	    curPollPtr->nKBytes = statusPtr.info.u.vol.nKBytes;
	    strcpy(curPollPtr->volumeName, 
		   (char *)statusPtr.info.u.vol.volumeName);
	    curPollPtr->volsFailed = statusPtr.info.u.vol.volsFailed;
	    break;
	  case TCOP_READLABEL:
	  case TCOP_LABELTAPE:
	    dfprintf(("StatusWatcher: op %d, tape name %s\n", 
		      statusPtr.info.opType, 
		      statusPtr.info.u.label.tapeLabel.name));
	    strncpy((char *)curPollPtr->tapeName, 
		    (char *)statusPtr.info.u.label.tapeLabel.name, 
		    statusPtr.info.u.label.tapeLabel.nameLen - 1);
	    curPollPtr->tapeSize = statusPtr.info.u.label.tapeLabel.size;
	    curPollPtr->tapeSize_ext=statusPtr.info.u.label.tapeLabel.size_ext;
	    break;
	  default:
	    dfprintf(("StatusWatcher: op %d\n", statusPtr.info.opType));
	    break;
	  }
	} FINALLY {
	  lock_ReleaseWrite(&statusQueueLock);
	} ENDTRY

	if ( taskFlags & TASK_DONE )
	{ /*done*/
	    /* Task has finished. Check the termination conditions */
	  dfprintf(("StatusWatcher: task %s:%d done\n", curPollPtr->taskName, 
		    taskId));	  
	    if ( (taskFlags & (ABORT_DONE|TASK_ERROR)) == 0 )
	    {
	      dfprintf(("StatusWatcher: task %s:%d no abort or error\n", 
			curPollPtr->taskName, taskId));
	        curPollPtr->defunct=0;
		if ( (localTaskFlags & SILENT) == 0 )
		{
		    dce_printf(bak_s_job_finished, jobNumber, 
			   curPollPtr->taskName);
		    fflush(stdout);
		    switch (curPollPtr->opType) {
		    case TCOP_DUMP:
		      if ( curPollPtr->volsFailed )
			dce_printf(bak_s_filesets_failed, curPollPtr->volsFailed, curPollPtr->volsTotal);
		      break;
		    case TCOP_RESTORE:
		      break;
		    case TCOP_READLABEL:
		      break;
		    case TCOP_LABELTAPE:
		      break;
		    case TCOP_SCANTAPE:
		      break;
		    default:
		      break;
		    }
		    dce_printf(bak_s_newline);
		}
		
	    }
	    else
	    if ( taskFlags & ABORT_DONE )
	    {
	        curPollPtr->defunct=1;
		dce_printf(bak_s_job_aborted, jobNumber, curPollPtr->taskName);
		if ( taskFlags & TASK_ERROR )
		    dce_printf(bak_s_with_errors);
		dce_printf(bak_s_newline);
		fflush(stdout);
	    }
	    else
	    if ( taskFlags & TASK_ERROR )
	    {
	        curPollPtr->defunct=1;
		dce_svc_printf(BAK_S_FAILED_WITH_ERRORS_MSG, jobNumber);
		fflush(stdout);
	    }
	    else
	    {
	        curPollPtr->defunct=1;
		dce_svc_printf(BAK_S_INTERNAL_ERROR_MSG);
		fflush(stdout);
	      }		
	    /* make call to destroy task on server */
	  dfprintf(("StatusWatcher: calling BUTC_EndStatus for task %s:%d\n",
		    curPollPtr->taskName,taskId));
	    code = BUTC_EndStatus(tconn, taskId);
	    if ( code )
	    {
	        curPollPtr->defunct=1;
		dce_svc_printf(BAK_S_ERROR_IN_CLEANUP_MSG, jobNumber, curPollPtr->taskName);
	    }
	    /* unlink and destroy local task */
	    lock_ObtainWrite(&statusQueueLock);
	    TRY {
	      /*
	       * We don't have to call releStatusNode because we are going
	       * to delete the node, which would first decrement the refcnt.
	       */
	      dfprintf(("StatusWatcher: task %s: %d done, deleting...\n", 
			curPollPtr->taskName, taskId));
	      deleteStatusNode(curPollPtr);
	      curPollPtr = (statusP)NULL;
	    } FINALLY {
	      lock_ReleaseWrite(&statusQueueLock);
	    } ENDTRY
	} /*done*/
	bc_FreeConn(bc_globalConfig, tconn);
    } /*w*/
}

/* bc_jobNumber
 *	Allocate a job number. Computes the maximum of all the job numbers
 *	and then returns the maximum+1.
 *	If no jobs are found, returns 1.
 * locks:
 *	This should be called with statusQueueLock already held for write
 */

long
bc_jobNumber()
{
    long retval = 0;
    dlqlinkP ptr;

    ptr = statusHead.dlq_next;
    while ( ptr != &statusHead )
    {
	/* compute max of all job numbers */
	if ( ((statusP) ptr)->jobNumber > retval )
	    retval = ((statusP) ptr)->jobNumber;

	ptr = ptr->dlq_next;
    }
    retval++;
    return(retval);
}





