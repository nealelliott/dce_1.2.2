/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dlq.h,v $
 * Revision 1.1.11.1  1996/10/02  16:58:03  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:48  damon]
 *
 * Revision 1.1.5.1  1994/06/09  13:50:16  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:42  annie]
 * 
 * Revision 1.1.3.3  1993/01/18  19:50:47  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:24:51  cjd]
 * 
 * Revision 1.1.3.2  1992/09/25  17:48:09  jaffe
 * 	Transarc delta: jaffe-ot5416-cleanup-RCS-log-entries 1.1
 * 	  Selected comments:
 * 	    Cleanup extra RCS log entries.  There should only be one per file, and
 * 	    it should be closed with an EndLog comment.
 * 	[1992/09/23  18:08:19  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:56:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (C) 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * dlq.h -- doubly linked queue structures
 * 
 * $Header: /u0/rcs_trees/dce/rcs/file/bak/dlq.h,v 1.1.11.1 1996/10/02 16:58:03 damon Exp $
 */

struct dlqlink
{
    struct dlqlink	*dlq_next;
    struct dlqlink	*dlq_prev;
    long		dlq_type;
    char *		dlq_structPtr;		/* enclosing structure */
};

typedef	struct dlqlink	dlqlinkT;
typedef	dlqlinkT	*dlqlinkP;


/* invariants */
#define	DLQ_HEAD	1

/* user defined */

/* macros */

/* simple assertion - for testing, don't halt processing */

#define	DLQASSERT(condition)					\
	if ( (condition) == 0 )					\
	{							\
	    printf("DLQASSERT: %s failed\n", #condition);	\
	}

/* function typing */
extern dlqlinkP dlqFront();
extern dlqlinkP dlqUnlinkb();
extern dlqlinkP dlqUnlinkf();


