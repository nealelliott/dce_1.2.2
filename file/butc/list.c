/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: list.c,v $
 * Revision 1.1.95.1  1996/10/02  17:06:05  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:25  damon]
 *
 * Revision 1.1.89.3  1994/08/25  17:34:55  mckeen
 * 	Added s12y calls
 * 	[1994/08/25  17:21:18  mckeen]
 * 
 * Revision 1.1.89.2  1994/06/09  13:52:58  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:57  annie]
 * 
 * Revision 1.1.89.1  1994/02/04  20:06:58  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:58  devsrc]
 * 
 * Revision 1.1.87.1  1993/12/07  17:13:33  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:01:15  jaffe]
 * 
 * Revision 1.1.5.5  1993/01/18  20:48:24  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  17:33:53  cjd]
 * 
 * Revision 1.1.5.4  1993/01/13  16:04:37  shl
 * 	Transarc delta: vijay-ot6381-correct-handling-of-device-queue-lock 1.4
 * 	  Selected comments:
 * 
 * 	    The aim of this delta is to get bak restore to succeed. The problems in restore
 * 	    were some spurious thread exceptions and assertions that made bak dump core.
 * 	    Upon investigation it turned out that there were some locking problems in
 * 	    backup and butc. The delta cleans up locking in backup and butc.
 * 	    The delta is not ready for export. Much more needs to be done to get all the
 * 	    locking issues right. This is just a checkpoint.
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
 * 	[1993/01/12  18:45:12  shl]
 * 
 * Revision 1.1.5.3  1992/11/24  15:42:23  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:10:33  bolinger]
 * 
 * Revision 1.1.5.2  1992/09/25  18:02:56  jaffe
 * 	Transarc delta: jaffe-ot5416-cleanup-RCS-log-entries 1.1
 * 	  Selected comments:
 * 	    Cleanup extra RCS log entries.  There should only be one per file, and
 * 	    it should be closed with an EndLog comment.
 * 	[1992/09/23  18:45:51  jaffe]
 * 
 * Revision 1.1.3.2  1992/04/21  12:22:06  mason
 * 	Transarc delta: cburnett-ot2695-butc_changes_for_aix32 1.1
 * 	  Files modified:
 * 	    butc: list.c
 * 	  Selected comments:
 * 	    Changes for AIX 3.2 compile
 * 	    changes for AIX 3.2 compile.  Removed include of afs/lock.h.
 * 	[1992/04/20  21:57:01  mason]
 * 
 * Revision 1.1  1992/01/19  02:54:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (C) 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * list.c -- 
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
RCSID("$Header: /u0/rcs_trees/dce/rcs/file/butc/list.c,v 1.1.95.1 1996/10/02 17:06:05 damon Exp $")

#include <sys/time.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <sys/errno.h>
#include <dcedfs/compat.h>
#include <dcedfs/lock.h>
#include <dcedfs/tcdata.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

extern int debugLevel;
osi_dlock_t dumpNodeLock;	     /* lock to protect dumpNode list */
static struct dumpNode *dumpQHeader; /* ptr to head of the dumpNode list */
static struct dumpNode headNode;     /* the dummy header of the node list */
static long maxDumpID; 		     /* the largest dump Id allotted so far, 
					this is never reused */

void static DisplayNode(nodePtr)
struct dumpNode *nodePtr;
{
    dce_svc_printf(BAK_S_CREATED_DUMPNODE_MSG, nodePtr->dumpID, ctime(&(nodePtr->creationTime)));
   return;

}
/* initialize the node list used to keep track of the active dumps */
void InitNodeList()
{
    lock_Init(&dumpNodeLock);
    maxDumpID = 1; /* this is the first dump id alotted */
    headNode.dumpID = -1;
    headNode.next = (struct dumpNode *)0;
    headNode.dumps = (struct tc_dumpDesc *)0;
    headNode.restores = (struct tc_restoreDesc *)0;
    lock_ObtainWrite(&dumpNodeLock);
    dumpQHeader = &headNode;         /*no one in the list to start with */
    lock_ReleaseWrite(&dumpNodeLock);
}

/* CreateNode
 *	Create a <newNode> for the dump, put it in the list of active dumps
 *	and return a pointer to it
 * entry:
 *	newNode - for return ptr
 * exit:
 *	newNode ptr set to point to a node. Return with newNode lock held
 */

void CreateNode(newNode)
struct dumpNode **newNode;
{
    struct timeval tp;
    struct timezone tzp;

    osi_gettimeofday(&tp,&tzp);

    /* get space */
    *newNode = (struct dumpNode *) (osi_Alloc (sizeof (struct dumpNode)));

    bzero((char *)*newNode, sizeof(struct dumpNode));
    lock_ObtainWrite(&dumpNodeLock);
    (*newNode)->next = dumpQHeader->next;
    dumpQHeader->next = *newNode;
    (*newNode)->dumpID = maxDumpID;
    maxDumpID++;
    (*newNode)->creationTime = tp.tv_sec;
    lock_Init(&((*newNode)->lock));
    lock_ObtainWrite(&((*newNode)->lock));
    if(debugLevel) 
      DisplayNode(*newNode);
    lock_ReleaseWrite(&dumpNodeLock);
}

/* free the space allotted to the node with <dumpID> */
void FreeNode(dumpID)
long dumpID;
{
    struct dumpNode *oldPtr,*newPtr,*curPtr;
    int done;

    lock_ObtainWrite(&dumpNodeLock);
    curPtr = dumpQHeader;
    oldPtr = dumpQHeader;
    if(curPtr) newPtr = dumpQHeader->next;
    else newPtr = (struct dumpNode *)0;
    done = 0;
    while((!done) && (curPtr != (struct dumpNode *)0)) {
      if(curPtr->dumpID == dumpID){
	done = 1;
	lock_ObtainWrite(&(oldPtr->lock));
	oldPtr->next = newPtr;
	lock_ReleaseWrite(&(oldPtr->lock));
	lock_ObtainWrite(&(curPtr->lock));
	/* free the node and its structures */
	if(curPtr->dumpName) 
	  (void)osi_Free(curPtr->dumpName, strlen(curPtr->dumpName)+1);
	if(curPtr->volumeSetName) 
	 (void)osi_Free(curPtr->volumeSetName,strlen(curPtr->volumeSetName)+1);
	if(curPtr->restores) 
	  (void)osi_Free(curPtr->restores,
			 sizeof(struct tc_restoreDesc) * curPtr->arraySize);
	if(curPtr->dumps) 
	  (void)osi_Free(curPtr->dumps, 
			 sizeof(struct tc_dumpDesc) * curPtr->arraySize);
	lock_ReleaseWrite(&(curPtr->lock));
	(void)osi_Free(curPtr, sizeof(struct dumpNode));
      }
      else {	
	oldPtr = curPtr;
	curPtr = newPtr;
	if(newPtr) newPtr = newPtr->next;
      }
    }
    lock_ReleaseWrite(&dumpNodeLock);
    return;
}

long GetNthNode(aindex, aresult)
long aindex;
long *aresult; {
    register struct dumpNode *tn;
    register int i;

    lock_ObtainRead(&dumpNodeLock);
    tn = dumpQHeader->next;
    for(i=0;;i++) {
	if (tn == (struct dumpNode *)NULL) {
	  lock_ReleaseRead(&dumpNodeLock);
	  return ENOENT;
	}
	lock_ObtainRead(&(tn->lock));
	/* see if this is the desired node ID */
	if (i == aindex) {
	    *aresult = tn->dumpID;
	    lock_ReleaseRead(&(tn->lock));
	    lock_ReleaseRead(&dumpNodeLock);
	    return 0;
	}
	/* otherwise, skip to next one and keep looking */
	lock_ReleaseRead(&(tn->lock));
	tn = tn->next;
    }
}

/* return the node with <dumpID> into <resultNode> */
long GetNode(dumpID, resultNode)
long dumpID;
struct dumpNode **resultNode;
{
    struct dumpNode *tmpPtr;
    int done;

    done = 0;
    lock_ObtainRead(&dumpNodeLock);
    tmpPtr = dumpQHeader;
    while((!done) && (tmpPtr != (struct dumpNode *)0)) {
        lock_ObtainRead(&(tmpPtr->lock));
	if(tmpPtr->dumpID == dumpID) {
	    *resultNode = tmpPtr;
	    done = 1;
	    lock_ReleaseRead(&(tmpPtr->lock));
	}
	else {
	    lock_ReleaseRead(&(tmpPtr->lock));
	    tmpPtr = tmpPtr->next;
	  }
    }
    lock_ReleaseRead(&dumpNodeLock);
    if (done) return 0;
    else return TC_NODENOTFOUND;
}

/* allocDumpId
 *	allocate a dump (task) id
 */

long
allocDumpId()
{
    return(maxDumpID++);
}
