/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftu_syscalls.c,v $
 * Revision 1.1.18.1  1996/10/02  17:48:47  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:37  damon]
 *
 * Revision 1.1.13.3  1994/07/13  22:28:16  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:38:00  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:20  mbs]
 * 
 * Revision 1.1.13.2  1994/06/09  14:09:18  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:23:57  annie]
 * 
 * Revision 1.1.13.1  1994/02/04  20:19:49  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:39  devsrc]
 * 
 * Revision 1.1.11.2  1994/01/20  18:43:15  annie
 * 	added copyright header
 * 	[1994/01/20  18:39:30  annie]
 * 
 * Revision 1.1.11.1  1993/12/07  17:25:45  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:06:43  jaffe]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1995, 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <dcedfs/syscall.h>

#include <ftutil.h>
#include <ftu_private.h>
#include <ftu_syscalls.h>
#include <ftu_trace.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ftutil/ftu_syscalls.c,v 1.1.18.1 1996/10/02 17:48:47 damon Exp $")

struct traceInfo {
    u_long	flags;
    long*	normalCodes;
};

#define ABNORMAL_ONLY	0x1

#define TRACE_ENTRY(volOp, traceInfoArray)	\
    (((traceInfoArray)[(volOp)].flags & ABNORMAL_ONLY) == 0)


PRIVATE int
TraceExit(volOp, code, traceInfoArray)
    int			volOp;
    long		code;
    struct traceInfo	traceInfoArray[];
{
    struct traceInfo*	tiP = &traceInfoArray[volOp];
    int	trace = 1;

    if (code == 0) {
	if (tiP->flags & ABNORMAL_ONLY)
	    trace = 0;
    } else {
	int	i;

	if (tiP->normalCodes != 0) {
	    for (i = 0; tiP->normalCodes[i] != 0; i++) {
		if (code == tiP->normalCodes[i]) {
		    trace = 0;
		    break;
		}
	    }
	}
    }

    return trace;
}	/* TraceExit() */


PRIVATE long seekCodes[] = {VOL_ERR_EOF, 0};
PRIVATE long scanCodes[] = {VOL_ERR_EOF, VOL_ERR_DELETED, 0};
PRIVATE long loopCodes[] = {ELOOP, 0};
PRIVATE long readdirCodes[] = {VOL_ERR_EOF, 0};

PRIVATE struct traceInfo volOpTraceInfo[] = {
    {0, 0},				/* 0-3 are unsed */
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},				/* 4 (VOLOP_OPEN) */
    {ABNORMAL_ONLY, seekCodes},		/* 5 (VOLOP_SEEK) */
    {0, 0},				/* 6 (VOLOP_TELL) */
    {ABNORMAL_ONLY, scanCodes},		/* 7 (VOLOP_SCAN) */
    {0, 0},				/* 8 (VOLOP_CLOSE) */
    {0, 0},				/* 9 (VOLOP_DESTROY) */
    {0, 0},				/* 10 (VOLOP_GETSTATUS) */
    {0, 0},				/* 11 (VOLOP_SETSTATUS) */
    {ABNORMAL_ONLY, 0},			/* 12 (VOLOP_CREATE) */
    {ABNORMAL_ONLY, 0},			/* 13 (VOLOP_READ) */
    {ABNORMAL_ONLY, 0},			/* 14 (VOLOP_WRITE) */
    {ABNORMAL_ONLY, 0},			/* 15 (VOLOP_TRUNCATE) */
    {ABNORMAL_ONLY, 0},			/* 16 (VOLOP_DELETE) */
    {ABNORMAL_ONLY, 0},			/* 17 (VOLOP_GETATTR) */
    {ABNORMAL_ONLY, 0},			/* 18 (VOLOP_SETATTR) */
    {ABNORMAL_ONLY, 0},			/* 19 (VOLOP_GETACL) */
    {ABNORMAL_ONLY, 0},			/* 20 (VOLOP_SETACL) */
    {ABNORMAL_ONLY, loopCodes},		/* 21 (VOLOP_CLONE) */
    {ABNORMAL_ONLY, loopCodes},		/* 22 (VOLOP_RECLONE) */
    {0, 0},				/* 23 (VOLOP_VGET) */
    {0, 0},				/* 24 (VOLOP_ROOT) */
    {0, 0},				/* 25 (VOLOP_ISROOT) */
    {ABNORMAL_ONLY, loopCodes},		/* 26 (VOLOP_UNCLONE) */
    {0, 0},				/* 27 (VOLOP_FCLOSE) */
    {ABNORMAL_ONLY, 0},			/* 28 (VOLOP_SETVV) */
    {0, 0},				/* 29 (VOLOP_SWAPVOLIDS) */
    {ABNORMAL_ONLY, 0},			/* 30 (VOLOP_COPYACL) */
    {0, 0},				/* 31 (VOLOP_AGOPEN) */
    {0, 0},				/* 32 (VOLOP_SYNC) */
    {0, 0},				/* 33 (VOLOP_PUSHSTATUS) */
    {ABNORMAL_ONLY, 0},			/* 34 (VOLOP_PROBE) */
    {0, 0},				/* 35 (VOLOP_LOCK) */
    {0, 0},				/* 36 (VOLOP_UNLOCK) */
    {ABNORMAL_ONLY, readdirCodes},	/* 37 (VOLOP_READDIR) */
    {ABNORMAL_ONLY, 0},			/* 38 (VOLOP_APPENDDIR) */
    {0, 0},				/* 39 (VOLOP_BULKSETSTATUS) */
    {0, 0},				/* 40 (VOLOP_GETZLC) */
    {0, 0},				/* 41 (VOLOP_GETNEXTHOLES) */
    {ABNORMAL_ONLY, loopCodes},		/* 42 (VOLOP_DEPLETE) */
};

#define TRACE_VOLOP_ENTRY(volOp)	TRACE_ENTRY(volOp, volOpTraceInfo)
#define TRACE_VOLOP_EXIT(volOp, code)	TraceExit(volOp, code, volOpTraceInfo)


EXPORT long
ftu_AggrSyscall(aggrOp, p1, p2, p3, p4)
    int	aggrOp;
    int	p1, p2, p3, p4;
{
    int	code;

    icl_Trace4(_ftu_iclSetP, FTU_TRACE_ENTER_AGGR_SYSCALL,
	       ICL_TYPE_LONG, aggrOp, ICL_TYPE_LONG, p1,
	       ICL_TYPE_LONG, p2, ICL_TYPE_LONG, p3);
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_ENTER_AGGR_SYSCALL2,
	       ICL_TYPE_LONG, p4);

    if ((code = afs_syscall(AFSCALL_AGGR, aggrOp, p1, p2, p3, p4)) == -1)
	code = errno;

    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_AGGR_SYSCALL,
	       ICL_TYPE_LONG, code);
    return code;
}	/* ftu_AggrSyscall() */


EXPORT long
ftu_VolSyscall(volOp, p1, p2, p3, p4)
    int	volOp;
    int	p1, p2, p3, p4;
{
    int	code;

    if (TRACE_VOLOP_ENTRY(volOp)) {
	icl_Trace4(_ftu_iclSetP, FTU_TRACE_ENTER_VOL_SYSCALL,
		   ICL_TYPE_LONG, volOp, ICL_TYPE_LONG, p1,
		   ICL_TYPE_LONG, p2, ICL_TYPE_LONG, p3);
	icl_Trace1(_ftu_iclSetP, FTU_TRACE_ENTER_VOL_SYSCALL2,
		   ICL_TYPE_LONG, p4);
    }

    if ((code = afs_syscall(AFSCALL_VOL, volOp, p1, p2, p3, p4)) == -1)
	code = errno;

    if (TRACE_VOLOP_EXIT(volOp, code)) {
	icl_Trace2(_ftu_iclSetP, FTU_TRACE_EXIT_VOL_SYSCALL,
		   ICL_TYPE_LONG, volOp, ICL_TYPE_LONG, code);
    }
    return code;
}	/* ftu_VolSyscall() */
