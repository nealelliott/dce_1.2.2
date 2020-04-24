/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cm_conn.h,v $
 * Revision 1.1.70.1  1996/10/02  17:07:28  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:41  damon]
 *
 * $EndLog$
 */
/*
 *      Copyright (C) 1996, 1990 Transarc Corporation
 *      All rights reserved.
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/cm/cm_conn.h,v 1.1.70.1 1996/10/02 17:07:28 damon Exp $ */

#ifndef	_CM_CONNH_
#define _CM_CONNH_

#include <dcedfs/krpc_pool.h>
#include <cm_server.h>
#include <cm_rrequest.h>
#include <cm_volume.h>


/*
 * This represents a single connection of an authenticated (or non-
 * authenticated) user to a particular server; a server is usually
 * the exporter itself but it could also be a database server such
 * as the VLDB server.
 *
 * NOTE: llock is used to protect all fields of cm_conn except refCount and
 *       next. They are protected by the cm_connlock.
 */
struct cm_conn {
    struct cm_conn *next;		/* Next dude same server. */
    unsigned long authIdentity[2];      /* [pag, uid] for authentication */
    handle_t  connp;			/* RPC conn to file server */
    struct sockaddr_in addr;            /* RPC conn's server-address */
    struct cm_server *serverp;		/* server associated with this conn */
    unsigned long service;		/* service ID for this connection */
    struct lock_data lock;		/* lock for each connp entry */
    time_t  lifeTime;			/* tgt's expiration time */
    short refCount;			/* reference count for allocation */
    short callCount;			/* # of outstanding calls. */
    u_char states;			/* conn flags */
    u_char currAuthn;		/* authn level in use */
    u_char currAuthz;		/* authz service in use */
    u_char expiryState;		/* how far it's expired */
};

struct cm_tgt {
    struct cm_tgt	*next;		/* next login session on the CM */
    unsigned32	pag;		/* PAG for the login session */
    time_t		tgtTime;	/* TGT's expiration time */
    short		tgtFlags;	/* any flags */
};
#define CM_TGT_EXPIRED	1

/* 
 * Conn's state bits 
 */
#define	CN_FORCECONN	0x1		/* Reconnect to the server */
#define	CN_INPROGRESS	0x2		/* creating a binding in progress */
#define	CN_EXPIRED	0x4		/* this conn caches notice of a bad PAG */
#define CN_SETCONTEXT  	0x8		/* about to setup connection */
#define CN_CALLCOUNTWAIT	0x10	/* waiting for callcount to zero */

/*
 * General initialization constants.
 */
#define CM_CONN_MAXCLIENTCALLS	15	/* max client calls active at once */
#define CM_CONN_MAXCALLSPERUSER	4	/* max calls to create per auth user */
#define CM_CONN_UNAUTHLIFETIME	(60 * 60 * 24)	/* default unauth lifetime */

/* 
 * Exported by cm_conn.c 
 */
extern struct lock_data cm_connlock;
extern struct lock_data cm_tgtlock;
extern struct cm_tgt *cm_tgtList, *FreeTGTList;
extern struct cm_conn *FreeConnList;

extern unsigned32 cm_ConnInit _TAKES((int));

extern struct cm_conn *cm_Conn _TAKES((afsFid *, unsigned long,
				       struct cm_rrequest *,
				       struct cm_volume **, long *));

extern struct cm_conn *cm_ConnByHost _TAKES((struct cm_server *,
					     unsigned long,
					     afs_hyper_t *,
					     struct cm_rrequest *,
					     long));

extern struct cm_conn *cm_ConnByMHosts _TAKES((struct cm_server **,
					       unsigned long,
					       afs_hyper_t *,
					       struct cm_rrequest *,
					       struct cm_volume *,
					       struct cm_cell *,
					       long *));

extern struct cm_conn *cm_ConnByAddr _TAKES((struct sockaddr_in *,
					     struct cm_server *, 
					     unsigned long, afs_hyper_t *,
					     struct cm_rrequest *,
					     struct cm_volume *, long, int *));

extern void cm_PutConn _TAKES((struct cm_conn *));
extern void cm_ResetUserConns _TAKES((unsigned32));
extern void cm_DeleteConns _TAKES((struct cm_server *));
extern long cm_TGTLifeTime _TAKES((unsigned32, int *));
extern void cm_ReactToBindAddrChange(struct cm_conn *, int);

/* return values for the int* parameter */
#define	CM_TGTEXP_NOTEXPIRED	0
#define	CM_TGTEXP_PAGEXPIRED	1
#define	CM_TGTEXP_NOTGT		2
extern int cm_MarkExpired _TAKES((unsigned32));
#define	CM_BADCONN_ALLCONNS	(-6)
extern void cm_MarkBadConns _TAKES((struct cm_server *, unsigned32));
extern struct krpc_pool cm_concurrentCallPool;
#endif /* _CM_CONNH_ */
