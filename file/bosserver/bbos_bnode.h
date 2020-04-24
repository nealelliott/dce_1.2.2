/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bbos_bnode.h,v $
 * Revision 1.1.12.1  1996/10/02  17:03:47  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:46  damon]
 *
 * Revision 1.1.6.1  1994/06/09  13:52:00  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:25:35  annie]
 * 
 * Revision 1.1.4.2  1993/05/11  12:44:39  jaffe
 * 	Transarc delta: comer-ot5972-return-error-on-create-for-bad-executable 1.1
 * 	  Selected comments:
 * 	    Now, if you specify a bad command to execute in a "bos create", the
 * 	    bosserver will return an error message instead of creating the bnode
 * 	    and trying merrily to executed the non-existent file.
 * 	    Added a new bnode op to do validation so that this can be tried before
 * 	    actually creating the bnode.
 * 	[1993/05/10  16:38:41  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/18  19:55:00  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:30:48  cjd]
 * 
 * Revision 1.1.2.2  1992/10/02  20:50:18  toml
 * 	#define TRUE, #define bnode bbos_bnode for better portability.
 * 	[1992/10/02  18:39:26  toml]
 * 
 * Revision 1.1  1992/01/19  02:49:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  bbos_bnode.h -- declarations and macros for dealing with bosserver bnodes
 *
 *	(based on AFS 3 code):
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */


#define	BOP_TIMEOUT(bnode)	((*(bnode)->ops->timeout)((bnode)))
#define	BOP_GETSTAT(bnode, a)	((*(bnode)->ops->getstat)((bnode),(a)))
#define	BOP_SETSTAT(bnode, a)	((*(bnode)->ops->setstat)((bnode),(a)))
#define	BOP_DELETE(bnode)	((*(bnode)->ops->delete)((bnode)))
#define	BOP_PROCEXIT(bnode, a)	((*(bnode)->ops->procexit)((bnode),(a)))
#define	BOP_GETSTRING(bnode, a, b)	((*(bnode)->ops->getstring)((bnode),(a), (b)))
#define	BOP_GETPARM(bnode, n, b, l)	((*(bnode)->ops->getparm)((bnode),(n),(b),(l)))
#define	BOP_RESTARTP(bnode)	((*(bnode)->ops->restartp)((bnode)))
#define BOP_HASCORE(bnode)	((*(bnode)->ops->hascore)((bnode)))

/* remove any chance of a namespace conflict with OS header files */
#define bnode bbos_bnode

#ifndef	TRUE
#define TRUE 1
#endif

struct bnode_ops {
    struct bnode *(*create)(/* variable args */);
    int (*timeout) (/* bnode */);
    int (*getstat) (/* bnode, status */);
    int (*setstat) (/* bnode, status */);
    int (*delete) (/* bnode */);
    int (*procexit) (/* bnode, proc */);
    int (*getstring) (/* bnode, buffer, bufLen */);
    int (*getparm) (/* bnode, parmIndex, buffer, bufLen */);
    int	(*restartp) (/*	bnode */);
    int (*hascore) (/* bnode */);
    int (*isvalid) (/* variable args */);
};

struct bnode_type {
    struct bnode_type *next;
    char *name;
    struct bnode_ops *ops;
};

struct bnode_token {
    struct bnode_token *next;
    char *key;
};

struct bnode {
    struct bnode *next;		    /* next pointer in top-level's list */
    char *name;			    /* instance name */
    long nextTimeout;		    /* next time this guy should be woken */
    long period;		    /* period between calls */
    long rsTime;		    /* time we started counting restarts */
    long rsCount;		    /* count of restarts since rsTime */
    struct bnode_type *type;	    /* type object */
    struct bnode_ops *ops;	    /* functions implementing bnode class */
    long procStartTime;		    /* last time a process was started */
    long procStarts;		    /* number of process starts */
    long lastAnyExit;		    /* last time a process exited, for any reason */
    long lastErrorExit;		    /* last time a process exited unexpectedly */
    long errorCode;		    /* last exit return code */
    long errorSignal;		    /* last proc terminating signal */
    char *lastErrorName;	    /* name of proc that failed last */
    short refCount;		    /* reference count */
    short flags;		    /* random flags */
    char goal;			    /* 1=running or 0=not running */
    char fileGoal;		    /* same, but to be stored in file */
};

struct bnode_proc {
    struct bnode_proc *next;	    /* next guy in top-level's list */
    struct bnode *bnode;	    /* bnode creating this process */
    char *comLine;		    /* command line used to start this process */
    char *coreName;		    /* optional core file component name */
    long pid;			    /* pid if created */
    long lastExit;		    /* last termination code */
    long lastSignal;		    /* last signal that killed this guy */
    long flags;			    /* flags giving process state */
};

/* bnode flags */
#define	BNODE_NEEDTIMEOUT	    1	    /* timeouts are active */
#define	BNODE_ACTIVE		    2	    /* in generic lists */
#define	BNODE_WAIT		    4	    /* someone waiting for status change */
#define	BNODE_DELETE		    8	    /* delete this bnode asap */
#define	BNODE_ERRORSTOP		    0x10    /* stopped due to errors */

/* flags for bnode_proc */
#define	BPROC_STARTED		    1	    /* ever started */
#define	BPROC_EXITED		    2	    /* exited */

/* status values for bnodes, and goals */
#define	BSTAT_SHUTDOWN		    0	    /* shutdown normally */
#define	BSTAT_NORMAL		    1	    /* running normally */
#define	BSTAT_SHUTTINGDOWN	    2	    /* normal --> shutdown */
#define	BSTAT_STARTINGUP	    3	    /* shutdown --> normal */

/* calls back up to the generic bnode layer */
extern int bnode_SetTimeout(/* bnode, timeout */);
extern int bnode_Init(/* bnode, bnodeops */);
extern int bnode_Activate(/* bnode */);
extern int bnode_NewProc(/* bnode, execstring, corename, procaddr */);
extern int bnode_Init(/* no parms */);
extern long bnode_Create();
extern struct bnode *bnode_FindInstance();

typedef struct bnode_stats {
    int weirdPids;
} bnode_stats_t;
