/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: status.c,v $
 * Revision 1.1.89.1  1996/10/02  16:58:55  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:58  damon]
 *
 * Revision 1.1.83.4  1994/08/25  17:34:43  mckeen
 * 	Added s12y calls
 * 	[1994/08/25  17:23:15  mckeen]
 * 
 * Revision 1.1.83.3  1994/07/13  22:27:18  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:20:44  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:53:14  mbs]
 * 
 * Revision 1.1.83.2  1994/06/09  13:50:39  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:59  annie]
 * 
 * Revision 1.1.83.1  1994/02/04  20:04:43  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:05:59  devsrc]
 * 
 * Revision 1.1.81.1  1993/12/07  17:11:46  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:45:17  jaffe]
 * 
 * Revision 1.1.5.5  1993/01/18  19:51:43  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:26:21  cjd]
 * 
 * Revision 1.1.5.4  1993/01/13  15:24:17  shl
 * 	Transarc delta: jaffe-cleanup-2.3-compiler-warnings 1.8
 * 	  Selected comments:
 * 	    Fix compiler warnings found when moving to OSF/1 1.1.1.
 * 	    add casts for calls to bzero.
 * 	    Remove the function dacl_SetOwnerControl.  It is not used.
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
 * 	[1993/01/12  17:36:40  shl]
 * 
 * Revision 1.1.5.3  1992/11/24  15:38:04  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:05:37  bolinger]
 * 
 * Revision 1.1.5.2  1992/08/31  18:01:12  jaffe
 * 	Transarc delta: jess-ot2869-com_err-usage-cleanup 1.5
 * 	  Selected comments:
 * 	    The use of com_err in dfs is inconsistent and in some cases
 * 	    incorrect.
 * 	    1) there are many sources which include <com_err.h> but don't
 * 	    use any definitions from it.
 * 	    2) there are sources which use com_err, but don't include the
 * 	    <com_err.h>
 * 	    3) there are subdirectories which do not need libcom_err.a for
 * 	    any reason that link with it.
 * 	    This delta is created to clean up the usage of com_err in dfs.
 * 	    Delete line "#include <com_err.h>
 * 	[1992/08/28  20:24:45  jaffe]
 * 
 * Revision 1.1.3.5  1992/05/05  01:19:30  mason
 * 	delta vijay-butc-compile-error 1.2
 * 	[1992/04/24  21:16:20  mason]
 * 
 * Revision 1.1.3.4  1992/01/25  20:43:39  zeliff
 * 	dfs6.3 merge, part2
 * 	[1992/01/25  19:35:24  zeliff]
 * 
 * $EndLog$
 */
/*
 * status.c -- task status management
 */ 

#include <dcedfs/stds.h>  /* include appropriate standards file */

#include <dcedfs/compat.h>
#include <sys/types.h>
#include <bc.h>
#include <error_macros.h>
#include <pthread.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

extern int nonInteractiveBak;
extern dlqlinkT statusHead;		/* chain of status blocks */
osi_dlock_t statusQueueLock;       

#define dfprintf(x)

initStatus()
{
    lock_Init(&statusQueueLock);
    lock_ObtainWrite(&statusQueueLock);
    TRY {
      dlqInit(&statusHead);
    } FINALLY {
      lock_ReleaseWrite(&statusQueueLock);
    } ENDTRY
}

/* general */

clearStatus(taskId, flags)
     u_long taskId;
     u_long flags;
{
     statusP ptr;

     extern statusP findStatus();
     lock_ObtainShared(&statusQueueLock);
     ptr = findStatus(taskId);
     if ( ptr == 0 ) {
       lock_ReleaseShared(&statusQueueLock);
       return;
     }
     lock_UpgradeSToW(&(statusQueueLock));
     ptr->flags &= ~flags;
     lock_ReleaseWrite(&(statusQueueLock));
 }

statusP
createStatusNode(refCount)
  int refCount;	/* refCount to be placed on status node */
{
    statusP ptr;

    if((ptr = (statusP) osi_Alloc(sizeof(statusT))) == (statusP)NULL)
      return((statusP)0);

    bzero((char *)ptr, sizeof(statusT));
    /* link it onto the chain of status entries */
    lock_ObtainWrite(&(statusQueueLock));
    TRY {
      dlqLinkb(&statusHead, (dlqlinkP) ptr);
      /*
       * refCount is a way of making sure all entities that want read the
       * status structure and then delete them, do so in a synchronized manner.
       * deleteStatusNode decrements refCount, and deleted when refCount == 0.
       * refCount set to 3 in bak for
       *	- this creation
       *	- statusWatcher
       *	- bc_WaitForJobs (only in non-interactive mode)
       * refCount set to 2 in butc for
       *  - this creation
       *  - BUTC_EndStatus
       */
      ptr->refCount = refCount;
      ptr->flags = STARTING;
      dfprintf(("createStatusNode: task refcount %d\n", ptr->refCount));
    } FINALLY {
      lock_ReleaseWrite(&(statusQueueLock));
    } ENDTRY
    return(ptr);
}

/* 
 * holdStatusNode -- increment the reference count. Call with statusQueueLock 
 * held for write.
 */
holdStatusNode(ptr)
     statusP ptr;
{
  ptr->refCount++;
}

/* 
 * releStatusNode -- decrement the reference count. Call with statusQueueLock 
 * held for write.
 */
releStatusNode(ptr)
     statusP ptr;
{
  ptr->refCount++;
}

/* 
 * deleteStatusNode
 *	Orderly release of status node
 *	Grab the statusQueueLock (write) before calling this routine, and 
 *	release it after calling it.
 */
deleteStatusNode(ptr)
     statusP ptr;
{
  dfprintf(("deleteStatusNode: task %s:%d refcount %d\n", 
	    ptr->taskName,ptr->taskId,ptr->refCount));
    ptr->refCount--;
    if (ptr->refCount == 0) { /* no references to this node */
      dlqUnlink( (dlqlinkP) ptr);
      (void)osi_Free(ptr, sizeof(statusT));
    }
}

/*
 * zapStatusNode
 *	Blow away the status node irrespective of the refCount value
 *	Grab the statusQueueLock (write) before calling this routine, and 
 *	release it after calling it.
 */
zapStatusNode(ptr)
  statusP ptr;
{
  dfprintf(("zapStatusNode: task %s:%d refcount %d\n", 
	    ptr->taskName,ptr->taskId,ptr->refCount));
  dlqUnlink( (dlqlinkP) ptr);
  (void)osi_Free(ptr, sizeof(statusT));
}

/* Grab the statusQueueLock (read) before calling this routine, and release it	  after calling it
 */
statusP
findStatus(taskId)
     u_long taskId;
{
    statusP ptr = (statusP)NULL;
    dlqlinkP dlqPtr;

    dlqPtr = statusHead.dlq_next;
    while ( dlqPtr != &statusHead )
    {
        if ( ((statusP) dlqPtr)->taskId == taskId )
        {
            ptr = (statusP) dlqPtr;
            break;
        }
        dlqPtr = dlqPtr->dlq_next;
    }
    if (ptr)
      dfprintf(("findStatus: task %s:%d refcount %d\n", ptr->taskName,ptr->taskId,ptr->refCount));
    return(ptr);
}

setStatus(taskId, flags)
     u_long taskId;
     u_long flags;
{
     statusP ptr;

     lock_ObtainShared(&statusQueueLock);
     ptr = findStatus(taskId);
     if ( ptr == 0 ) {
       lock_ReleaseShared(&statusQueueLock);
       return;
     }
     dfprintf(("setStatus: task %s:%d refcount %d\n", ptr->taskName,ptr->taskId,ptr->refCount));
     lock_UpgradeSToW(&(statusQueueLock));
     ptr->flags |= flags;
     lock_ReleaseWrite(&(statusQueueLock));
}
