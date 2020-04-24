/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * queue.h -- 
 */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: queue.h,v $
 * Revision 1.1.8.1  1996/10/17  18:18:05  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:55  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:25:27  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:57:26  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  16:10:40  jaffe
 * 	Transarc delta: fred-add-cc-tests 1.2
 * 	  Selected comments:
 * 	    Added Mike Kazar's cache consistency tests to the test tree.
 * 	    Forgot to put a EndLog marker in the header comment.
 * 	    Added EndLog marker.
 * 	[1992/10/27  17:54:24  jaffe]
 * 
 * $EndLog$
 */

#ifndef	_QUEUEH_
#define	_QUEUEH_

/* 
 * "Simple Queues" implemented with both pointers and short offsets into 
 * a disk file. 
 */
struct squeue {
    struct squeue *next;
    struct squeue *prev;
};

/*
 * Operations on circular queues implemented with pointers.  Note: these queue objects 
 * are always located at the beginning of the structures they are linking.
 */
#define	QInit(q)    ((q)->prev = (q)->next = (q))
#define	QAdd(q,e)   ((e)->next=(q)->next,(e)->prev=(q),(q)->next->prev=(e),(q)->next=(e))
#define	QAddT(q,e)  ((e)->next=(q), (e)->prev=(q)->prev, (q)->prev->next=(e), (q)->prev=(e))
#define	QRemove(e)  ((e)->next->prev = (e)->prev, (e)->prev->next = (e)->next)
#define	QNext(e)    ((e)->next)
#define QPrev(e)    ((e)->prev)

#endif /* _QUEUEH_ */
