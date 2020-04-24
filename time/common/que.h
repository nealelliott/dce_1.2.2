/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: que.h,v $
 * Revision 1.1.6.2  1996/02/18  23:11:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:42  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:07:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:56  root]
 * 
 * Revision 1.1.4.1  1994/05/12  15:17:19  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:35  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  20:39:46  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:08:24  htf]
 * 
 * Revision 1.1  1992/01/19  15:31:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 ****************************************************************************
 *									    *
 *  COPYRIGHT (c) 1988, 1989 BY						    *
 *  DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASSACHUSETTS.		    *
 *  ALL RIGHTS RESERVED.						    *
 * 									    *
 *  THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND COPIED   *
 *  ONLY IN  ACCORDANCE WITH  THE  TERMS  OF  SUCH  LICENSE  AND WITH THE   *
 *  INCLUSION OF THE ABOVE COPYRIGHT NOTICE. THIS SOFTWARE OR  ANY  OTHER   *
 *  COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY   *
 *  OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE IS  HEREBY   *
 *  TRANSFERRED.							    *
 * 									    *
 *  THE INFORMATION IN THIS SOFTWARE IS  SUBJECT TO CHANGE WITHOUT NOTICE   *
 *  AND  SHOULD  NOT  BE  CONSTRUED AS  A COMMITMENT BY DIGITAL EQUIPMENT   *
 *  CORPORATION.							    *
 * 									    *
 *  DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE  OR  RELIABILITY OF ITS   *
 *  SOFTWARE ON EQUIPMENT WHICH IS NOT SUPPLIED BY DIGITAL.		    *
 * 									    *
 *									    *
 ****************************************************************************
 */

/*
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:
 *
 *	This module contains genric queue macros.
 *
 * ENVIRONMENT: 	portable
 *
 * AUTHOR:		Carol Frampton
 *			(adapted from routines written by Neal Jacobson for
 *			 VAX Teamdata)
 *
 * CREATION DATE: 	January 1989
 *
 */

#ifndef _QUE_H
#define _QUE_H

/* 
 * Generalized Queue structure 
 */
typedef volatile struct Que
{
    volatile struct Que *next;
    volatile struct Que *prior;
} Que;


/* 
 * IsQEmpty
 * -----------
 *	Determines whether or not a queue is empty.  Returns true/false.
 *
 *	Also, "qhd" must be a simple; "simple" meaning that "qhd" must NOT
 *	contain any side-effect expressions (such as "++", "--", "+=", some
 *	functions, etc).
 *
 *	qhd		: Que		: read	: value
 *			: Queue head.
 *
 *	RETURNS: true	- queue is empty
 *	         false	- queue is not empty
 *
 *	Example:
 *	-------
 *	FOO	* foo_ptr;	! contains a field for the que head
 *	if (IsQEmpty ( foo_ptr->que )) ...
 *
 */
#define	IsQempty(qhd)							\
    ( (qhd).next == &(qhd) )

/* 
 * Qinit
 * ----------
 *	Initializes a queue header (points to itself).
 *
 *	ent		: Que		: mod	: ref
 *			: Queue head to initialize.
 */
#define	Qinit(qhd)							\
    {									\
	Que *_qhd; 							\
									\
	_qhd = (Que *) (qhd);						\
	_qhd->next = _qhd->prior = _qhd;				\
    }


/* 
 * Qfirst / Qnext
 * ------------------------
 *	Gets the first/next entry on a non-empty queue
 *
 *	qhd		: Que		: read	: ref
 *			: Queue head
 *
 *	ent		: entry		: write	: ref (ref)
 *			: Returns first/next entry on queue.
 */
#define	Qfirst(qhd,ent)							\
    {									\
	Que *_qhd, **_ent; 						\
									\
	_qhd= (Que *) (qhd); 						\
	_ent= (Que * *)(ent); 						\
	(* _ent) = _qhd->next;						\
    }

#define	Qnext Qfirst

/*
 * Qlast / Qprior
 * ------------------------
 *	Gets the last/prior entry on a non-empty queue
 *
 *	qhd		: Que		: read	: ref
 *			: Queue head.
 *
 *	ent		: entry		: write	: ref (ref)
 *			: Returns last/prior entry on queue.
 *
 */
#define	Qlast(qhd,ent)						\
    {									\
	Que *_qhd, **_ent; 						\
									\
	_qhd = (Que *) (qhd);						\
	_ent = (Que **) (ent); 						\
	(* _ent) = _qhd->prior;						\
    }

#define	Qprior Qlast


/*
 * Qinsert
 * ------------
 * QinsertFirst
 * --------------
 *	Inserts an entry on a queue.  The entry is inserted
 *	after the "qhd" entry, to become the last entry in the queue
 *
 *	qhd		: Que		: mod	: ref
 *			: Queue head.
 *
 *	ent		: entry		: mod	: ref
 *			: Entry to insert.
 *
 * QinsertLast
 * --------------
 *	Inserts an entry on a queue.  The entry is inserted
 *	prior to the "qhd" entry, to become the last entry in the queue.
 *
 *	qhd		: Que head	: mod	: ref
 *			: Queue head.
 *
 *	ent		: Que entry	: mod	: ref
 *			: Enty to insert.
 *
 * 	*** VARIABLES MUST BE UNIQue WHEN CALLING OTHER MACROS! ***
 */
#define	QinsertFirst(qhd, ent)						\
    {									\
	Que *_qhd = (Que *) (qhd);					\
	Que *_ent = (Que *) (ent);					\
									\
	(_ent) -> prior = _qhd;						\
	(_ent -> next = _qhd -> next) -> prior = _ent;			\
	_qhd -> next = _ent;						\
    }

#define QinsertLast(qhd,ent) 						\
    {									\
	Que *__qhd; 							\
									\
	__qhd=(Que *)(qhd);						\
	QinsertFirst (__qhd->prior, (ent) );				\
    }

#define Qinsert	QinsertFirst


/* 
 * Qremove
 * ------------
 *	Removes an arbitrary entry from a queue.
 *
 *	ent		: entry		: mod	: ref
 *			: Entry to remove.
 *
 * QremoveFirst
 * ------------------
 * Removes, and returns, the first entry from a queue.  The caller is
 * responsible for making sure that the queue is NOT empty (i.e. IsQEmpty).
 *
 *	qhd		: Que		: mod	: ref
 *			: Queue head.
 *
 *	ent		: Que entry	: write	: ref (ref)
 *			: First entry on queue.
 * *** VARIABLES MUST BE UNIQue WHEN CALLING OTHER MACROS! ***
 *
 *
 * QremoveLast
 * -----------------
 * Removes, and returns, the last entry from a queue.  The caller is
 * responsible for making sure that the queue is NOT empty (i.e. IsQEmpty).
 *
 *	qhd		: Que		: mod	: ref
 *			: Queue head.
 *
 *	ent		: Que entry	: write	: ref (ref)
 *			: Last entry on queue.
 *
 * *** VARIABLES MUST BE UNIQue WHEN CALLING OTHER MACROS! ***
 */
#define Qremove(ent) 							\
    {									\
	Que *_ent; 							\
									\
	_ent = (Que *) (ent);						\
    	(_ent->prior->next = _ent->next)->prior = _ent->prior;		\
    }

#define	QremoveFirst(qhd, ent) 						\
    {									\
	Que *__qhd, *__ent, **__entx;					\
									\
        __qhd = (Que *) (qhd); 						\
	Qfirst (__qhd, & __ent); 					\
	Qremove (__ent);						\
    	__entx = (Que **) (ent); 					\
	(* __entx) = __ent; 						\
    }

#define	QremoveLast(qhd, ent) 						\
    {									\
	Que *__qhd, *__ent, **__entx;					\
									\
    	__qhd = (Que *) (qhd); 						\
	Qlast (__qhd, & __ent); 					\
	Qremove (__ent);						\
    	__entx = (Que **) (ent); 					\
	(* __entx) = __ent; 						\
    }


/* 
 *			Queue Loop Macros
 *			=================
 *
 * QloopBegin / QloopEnd	(bracketed around code)
 * --------------   --------------
 *	Loops through an absolute queue.
 *
 *	NOTES:
 *	    o	The queue head ("qhd") is NOT considered a queue entry.  The
 *		first queue entry is the one that "qhd" points to.
 *	    o	You can use the "break" and "continue" statements from within
 *		the ABSQ_LOOP sequences.
 *
 *	NOTE
 *
 *	qhd		: Que		: read	: ref
 *			: Queue head.
 *
 *	ent		: entry pointer	: write	: ref (ref)
 *			: Receives each (next) entry on the queue.
 *
 *	Example:
 *	-------
 *	Que		* fooq_ptr;
 *	struct foo	* foo_ptr;
 *
 *	QloopBegin (& fooq_ptr->foo_que, & foo_ptr)
 *	    foo_ptr->bar = 5;
 *		.....
 *	QloopEnd
 */
#define	QloopBegin(qhd,ent) 						\
    {									\
	Que *_qhd, *_next, **_ent; 					\
									\
	_qhd = (Que *) (qhd); 						\
	_ent = (Que **) (ent);						\
	_next = _qhd -> next;						\
	while (_next != _qhd)						\
	{								\
	    (* _ent)=_next; 						\
	    _next = _next -> next; 					\
	    {

#define	QloopEnd							\
	    }								\
	}								\
    }


#endif  /* #ifndef _QUE_H */
/* DEC/CMS REPLACEMENT HISTORY, Element QUE.H*/
/* *4    13-SEP-1991 10:56:00 B_BAILEY "Make queue definition volatile"*/
/* *3     1-AUG-1989 16:30:34 C_CORTES "converted from variable length to Stream_LF"*/
/* *2    22-MAR-1989 18:34:12 NICHOLS "change macro names to fit DTSS conventions"*/
/* *1     1-FEB-1989 10:15:13 FRAMPTON "Queue manipulation macros - non-interlocked"*/
/* DEC/CMS REPLACEMENT HISTORY, Element QUE.H*/
