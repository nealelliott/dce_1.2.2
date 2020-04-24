/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dlq.c,v $
 * Revision 1.1.52.1  1996/10/02  16:58:01  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:47  damon]
 *
 * Revision 1.1.46.4  1994/08/25  17:34:32  mckeen
 * 	Added s12y calls
 * 	[1994/08/25  17:23:00  mckeen]
 * 
 * Revision 1.1.46.3  1994/07/13  22:27:08  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:16:47  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:53:00  mbs]
 * 
 * Revision 1.1.46.2  1994/06/09  13:50:14  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:40  annie]
 * 
 * Revision 1.1.46.1  1994/02/04  20:04:26  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:05:49  devsrc]
 * 
 * Revision 1.1.44.1  1993/12/07  17:11:33  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:43:27  jaffe]
 * 
 * Revision 1.1.3.4  1993/01/18  19:50:44  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:24:44  cjd]
 * 
 * Revision 1.1.3.3  1992/11/24  15:37:11  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:04:49  bolinger]
 * 
 * Revision 1.1.3.2  1992/09/25  17:48:02  jaffe
 * 	Transarc delta: jaffe-ot5416-cleanup-RCS-log-entries 1.1
 * 	  Selected comments:
 * 	    Cleanup extra RCS log entries.  There should only be one per file, and
 * 	    it should be closed with an EndLog comment.
 * 	[1992/09/23  18:07:38  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:56:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (C) 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * dlq.c -- doubly linked queue mainpulation
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bak/dlq.c,v 1.1.52.1 1996/10/02 16:58:01 damon Exp $")

#include <pthread.h>
#include <dlq.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

#define	DLQ_ASSERT_HEAD(headptr)				\
        if ( (headptr)->dlq_type != DLQ_HEAD )			\
	{							\
	    dce_svc_printf(BAK_S_INVALID_QUEUE_HEAD_MSG, \
		   __FILE__, __LINE__);				\
	    exit(1);						\
	}


/* dlqEmpty
 * exit:
 *	1 - queue is empty
 *	0 - items on queue
 */

dlqEmpty(headptr)
     dlqlinkP	headptr;
{
    DLQ_ASSERT_HEAD(headptr);
    if ( headptr->dlq_next == headptr )
	return(1);
    return(0);
}

dlqInit(headptr)
     dlqlinkP	headptr;
{
    headptr->dlq_next = headptr;
    headptr->dlq_prev = headptr;
    headptr->dlq_type = DLQ_HEAD;
    return(0);
}

/* dlqLinkf
 *	link item to front of chain
 */
dlqLinkf(headptr, entryptr)
     dlqlinkP	headptr;
     dlqlinkP	entryptr;
{
    DLQ_ASSERT_HEAD(headptr);
    /* link in as first item in chain */
    entryptr->dlq_next = headptr->dlq_next;
    headptr->dlq_next->dlq_prev = entryptr;
    entryptr->dlq_prev = headptr;
    headptr->dlq_next = entryptr;
    return(0);
}	

/* dlqLinkb
 *	link item to end of chain
 */

dlqLinkb(headptr, entryptr)
     dlqlinkP	headptr;
     dlqlinkP	entryptr;
{
    DLQ_ASSERT_HEAD(headptr);
    entryptr->dlq_next = headptr;
    entryptr->dlq_prev = headptr->dlq_prev;

    headptr->dlq_prev = entryptr;
    entryptr->dlq_prev->dlq_next = entryptr;
    return(0);
}

/* dlqMoveb
 *	move all the items on the fromptr and append to the toptr's list
 */

dlqMoveb(fromptr, toptr)
     dlqlinkP	fromptr;
     dlqlinkP	toptr;
{
    dlqlinkP	tailptr;

    DLQ_ASSERT_HEAD(fromptr);
    DLQ_ASSERT_HEAD(toptr);

    if ( dlqEmpty(fromptr) )
	return(0);

    tailptr = toptr->dlq_prev;

    tailptr->dlq_next = fromptr->dlq_next;
    tailptr->dlq_next->dlq_prev = tailptr;

    /* now fix up the last item in the new chain */
    tailptr = fromptr->dlq_prev;

    tailptr->dlq_next = toptr;
    toptr->dlq_prev = tailptr;

    fromptr->dlq_next = fromptr;
    fromptr->dlq_prev = fromptr;
}

/* dlqUnlinkb
 *	unlink the last item on the queue
 */

dlqlinkP
dlqUnlinkb(headptr) 
     dlqlinkP	headptr;
{
    dlqlinkP	ptr;
    DLQ_ASSERT_HEAD(headptr);

    if ( dlqEmpty(headptr) )
	return(0);

    ptr = headptr->dlq_prev;
    ptr->dlq_prev->dlq_next = headptr;
    headptr->dlq_prev = ptr->dlq_prev;
    
    ptr->dlq_next = ptr;
    ptr->dlq_prev = ptr;
    return(ptr);
}

/* dlqUnlinkf
 *	unlink the item on the front of the queue
 */

dlqlinkP
dlqUnlinkf(headptr)
     dlqlinkP	headptr;
{
    dlqlinkP	ptr;
    DLQ_ASSERT_HEAD(headptr);

    if ( dlqEmpty(headptr) )
	return(0);

    ptr = headptr->dlq_next;

    headptr->dlq_next = ptr->dlq_next;
    ptr->dlq_next->dlq_prev = headptr;

    ptr->dlq_next = ptr;
    ptr->dlq_prev = ptr;
    return(ptr);
}

/* dlqUnlink
 *	unlink the specified item from the queue.
 */

dlqUnlink(ptr)
     dlqlinkP    ptr;
{
    /* must not be the queue head */
    if ( ptr->dlq_type == DLQ_HEAD )
    {
	dce_svc_printf(BAK_S_INVALID_UNLINK_MSG);
	exit(1);
    }

    ptr->dlq_prev->dlq_next = ptr->dlq_next;
    ptr->dlq_next->dlq_prev = ptr->dlq_prev;

    ptr->dlq_next = 0;
    ptr->dlq_prev = 0;
}

/* dlqFront
 *	return point to item at front of queuen
 */

dlqlinkP
dlqFront(headptr)
     dlqlinkP	headptr;
{
    DLQ_ASSERT_HEAD(headptr);

    if ( dlqEmpty(headptr) )
	return(0);

    return(headptr->dlq_next);
}

int
dlqCount(headptr)
     dlqlinkP	headptr;
{
    dlqlinkP    ptr;
    int count = 0;

    DLQ_ASSERT_HEAD(headptr);

    ptr = headptr->dlq_next;
    while ( ptr != headptr )
    {
	ptr = ptr->dlq_next;
	count++;
    }
    return(count);
}

dlqTraverseQueue(headptr, fn)
     dlqlinkP	headptr;
     int (*fn)();
{
    dlqlinkP    ptr;

    DLQ_ASSERT_HEAD(headptr);

    ptr = headptr->dlq_next;
    while ( ptr != headptr )
    {
	(*fn)(ptr);
	ptr = ptr->dlq_next;
    }
    return(0);
}




