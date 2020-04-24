#ifndef _ICL_H__ENV_
#define _ICL_H__ENV_ 1

/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*  
 * HISTORY
 * $Log: icl.h,v $
 * Revision 1.1.62.1  1996/10/02  17:52:00  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:45  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1994 Transarc Corporation - All rights reserved. */
/* $Header: /u0/rcs_trees/dce/rcs/file/icl/icl.h,v 1.1.62.1 1996/10/02 17:52:00 damon Exp $ */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/lock.h>
#include <dcedfs/icl_macros.h>

/* locking hierarchy: lock icl_set first, then icl_log, then the
 * low-level icl_lock.
 */

/* Some of the procedures in this module have names with an "_r" suffix.  This
 * indicates that they must be called from a preemptive environment.  See the
 * comment preceding the declaration of osi_Alloc_r in osi.h for details. */

extern osi_mutex_t icl_lock;	/* lock for log and set refcounts */
extern int icl_inited;
extern struct icl_log *icl_allLogs;
extern struct icl_set *icl_allSets;

/* define an in-core logging package */

/* Make "states" the first word since all code compiled with icl_TraceN macros
 * references this field.  Doing so allows us to reorganize the following
 * fields without recompiling the entire system. */

struct icl_set {
    long states;		/* state flags */
    int refCount;		/* reference count */
    osi_mutex_t lock;		/* lock */
    struct icl_set *nextp;	/* next dude in all known tables */
    char *name;			/* name of set */
    struct icl_log *logs[ICL_LOGSPERSET];	/* logs */

    /* These two fields are referenced by icl_Event4 without locking for
     * performance reasons, so code that modifies them (there is none
     * presently) must do so carefully. */

    long nevents;		/* count of events */
    char *eventFlags;		/* pointer to event flags */

    /* Next four fields are for per-thread log mode */

    int nThreads;			/* number of threads; size of arrays */
    long *tidHT;			/* hash table of threadps */
    long *tid;				/* array of thread ID's */
    struct icl_log **perThreadLog;      /* array of logs */
};

/* descriptor of the circular log.  Note that it can never be 100% full
 * since we break the ambiguity of head == tail by calling that
 * state empty.
 */
struct icl_log {
    int refCount;		/* reference count */
    int setCount;		/* number of non-FREED sets pointing to us */
    osi_mutex_t lock;		/* lock */
    char *name;			/* log name */
    struct icl_log *nextp;	/* next log in system */
    long logSize;		/* allocated # of elements in log */
    long logElements;		/* # of elements in log right now */
    long *datap;		/* pointer to the data */
    long firstUsed;		/* first element used */
    long firstFree;		/* index of first free dude */
    unsigned long baseCookie;	/* cookie value of first entry */
    long states;		/* state bits */
    unsigned long lastTS;	/* last timestamp written to this log */
};

extern int icl_Event0(struct icl_set *setp, long id, long descr);
extern int icl_Event1(struct icl_set *setp, long id, long descr, long p1);
extern int icl_Event2(struct icl_set *setp, long id, long descr,
		      long p1, long p2);
extern int icl_Event3(struct icl_set *setp, long id, long descr,
		      long p1, long p2, long p3);
extern int icl_Event4(struct icl_set *setp, long id, long descr,
		      long p1, long p2, long p3, long p4);
extern int icl_CreateSet(
    char *name,
    struct icl_log *baseLog,
    struct icl_log *fatalLog,
    struct icl_set **set
);
extern int icl_CopyOut(
    struct icl_log *log,
    long *buf,
    long *bufSize,
    unsigned long *cookie,
    long *flags
);

extern int icl_CreateSetWithFlags _TAKES((char *name,
					  struct icl_log *baseLog,
					  struct icl_log *fatalLog,
					  unsigned long flags,
					  struct icl_set **set));
extern void icl_CreateSetWithFlags_r _TAKES((char *name,
					  struct icl_log *baseLog,
					  struct icl_log *fatalLog,
					  unsigned long flags,
					  struct icl_set **set));

extern int icl_CreateLog(char *name, long size, struct icl_log **olog);
extern int icl_CreateLogWithFlags(
    char *name,
    long size,
    u_long flags,
    struct icl_log **olog
);
extern void icl_CreateLogWithFlags_r(
    char *name,
    long size,
    u_long flags,
    struct icl_log **olog
);

extern int icl_GetLogParms(
    struct icl_log *log,
    long *maxSize,
    long *curSize
);

extern int icl_SetEnable(struct icl_set *set, long eventID, int val);
extern int icl_GetEnable(struct icl_set *set, long eventID, int *setp);
extern struct icl_log *icl_FindLog(char *name);
extern struct icl_set *icl_FindSet(char *name);
extern int icl_EnumerateLogs(
    int (*proc)(char *, char *, struct icl_log *),
    char *rock
);
extern int icl_EnumerateSets(
    int (*proc)(char *, char *, struct icl_set *),
    char *rock
);
extern int icl_SetSetStat(struct icl_set *setp, int op);
extern int icl_SetSetStat_r(struct icl_set *setp, int op);
extern int icl_LogSetSize(struct icl_log *logp, long logSize);
extern int icl_ZeroLog(struct icl_log *logp);
extern int icl_ZeroSet(struct icl_set *setp);

extern void icl_LogRele(struct icl_log *logp);
extern void icl_LogRele_r(struct icl_log *logp);
extern void icl_LogReleNL_r(struct icl_log *logp);
extern void icl_SetRele(struct icl_set *setp);
extern void icl_LogHold(struct icl_log *logp);
extern void icl_SetHold(struct icl_set *setp);
extern void icl_LogUse_r(struct icl_log *logp);
extern void icl_LogFreeUse_r(struct icl_log *logp);
extern void icl_ZapLog_r(struct icl_log *logp);
extern void icl_ZapSet_r(struct icl_set *setp);

/* Because we use native mutexes we should drop the preemption lock in icl
 * routines in case we call some blocking routine.  We also decide to allocate
 * and free memory in a preemptive environment (allocation, at least can block
 * for an extended period).
 *
 * The main exception to this is that the icl_Trace routines are ignorant of
 * the preemption_lock and if called with it held they may briefly block while
 * holding it.  This is probably a minor (negligible) performance hit. */

#define icl_MakePreemptionRight() osi_MakePreemptionRight()
#define icl_UnmakePreemptionRight() osi_UnmakePreemptionRight()

/* Measure performance of logging an event. */
#if ICL_MEASURE_LOG
#define ICL_LOG_TIME_NBINS 25
extern struct icl_log_times {
    u_long records, time, min, max;
    u_long bin[ICL_LOG_TIME_NBINS];
} icl_log_times;
#endif

#endif /* _ICL_H__ENV_ */
