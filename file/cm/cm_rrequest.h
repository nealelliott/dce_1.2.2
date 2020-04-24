/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cm_rrequest.h,v $
 * Revision 1.1.63.1  1996/10/02  17:08:25  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:52  damon]
 *
 * $EndLog$
 */
/*
 *      Copyright (C) 1995, 1990 Transarc Corporation
 *      All rights reserved.
 */


#ifndef	_CM_REQUESTH_
#define _CM_REQUESTH_

/* 
 * Structure used for user's remote calls 
 */
struct cm_rrequest {
    unsigned long pag;          /* user's Process Authentication Group */
    unsigned long uid;		/* effective user id */
#if 0
    u_long timeWentBusy;	/* when this rreq first went busy */
#endif /* 0 */
    struct cm_server *serverp;	/* server we got token from */
    char initd;			/* if non-zero, error fields meaningful */
    char accessError;		/* flags for overriding error return code */
    char volumeError;		/* encountered a missing volume */
    char networkError;		/* encountered network problems */
    char readOnlyError;		/* read-only file system */
    char interruptedError;		/* sleep was interrupted */
    char connFlags;			/* any flags to control conn backoff */
};

#define CM_UNAUTH_USERNUM	((uid_t) -2)
#define CM_UNUSED_USERNUM	((uid_t) -3)

#define CM_RREQFLAG_NOVOLWAIT	1
#define CM_RREQFLAG_NOAUTH      2

/*
 * Related macros
 */
#define	CM_RREQ_SLEEP	20		/* time to sleep */
#if 0
#define	CM_RREQ_TIMEOUT	400		/* VLDB timeout period */
#endif /* 0 */

#define cm_RPCError(x)		(((x) >= rpc_s_mod && (x) < rpc_s_mod+4096) ||\
				 (x) == -1)

/* used to pass server reference down to cm_MergeStatus */
#define cm_SetReqServer(rreqp,sp)	((rreqp)->serverp = (sp))
#define cm_GetReqServer(rreqp)		((rreqp)->serverp)

/* used to pass flags down to cm_ConnByMHosts */
#define cm_SetConnFlags(rreqp, fv)	((rreqp)->connFlags |= (fv))
#define cm_ClearConnFlags(rreqp, fv)	((rreqp)->connFlags &= ~(fv))
#define cm_GetConnFlags(rreqp)		((rreqp)->connFlags)

/* 
 * Exported by cm_rrequest.c 
 */
extern void cm_InitReq _TAKES((struct cm_rrequest *, osi_cred_t *));
extern void cm_InitUnauthReq _TAKES((struct cm_rrequest *));
extern void cm_FinalizeReq _TAKES((struct cm_rrequest *));
extern void cm_CopyError _TAKES((struct cm_rrequest *, struct cm_rrequest *));
extern int cm_CheckError _TAKES((long, struct cm_rrequest *));

#endif /* _CM_REQUESTH_ */
