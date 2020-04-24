/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * com.h -- 
 *    Declarations for communication structures and functions.
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
 * $Log: com.h,v $
 * Revision 1.1.8.1  1996/10/17  18:17:45  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:46  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:25:10  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:56:35  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  16:01:34  jaffe
 * 	Transarc delta: fred-add-cc-tests 1.2
 * 	  Selected comments:
 * 	    Added Mike Kazar's cache consistency tests to the test tree.
 * 	    Forgot to put a EndLog marker in the header comment.
 * 	    Added EndLog marker.
 * 	[1992/10/27  17:04:11  jaffe]
 * 
 * $EndLog$
 */

#ifndef _COM_H_ENV_
#define _COM_H_ENV_ 1

#include <sys/types.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "queue.h"

#ifndef _TAKES
#if defined(__STDC__) || defined(mips) || defined(_IBMR2)
#define _TAKES(x) x
#else /* __STDC__ */
#define _TAKES(x) ()
#endif /* __STDC__ */
#endif /* _TAKES */

#define WLR_BUFFERSIZE	16384
/* define max # of server file descriptors to use; artificially low for
 * testing purposes; probably more reasonable to keep about 4.
 */
#define WLR_SERVERFDS	1
#define wlr_ClientNext(x)	((struct wlr_client *)((x)->q.next))

/* caller of wlr_ACall/wlr_Call must fill in all but xid fields,
 * with the long fields in host order, but the buffer already in
 * marshalled network byte order.
 */
struct wlr_packet {
    struct wlr_packet *nextp;		/* next pointer when in (free) list */
    struct wlr_packetData {
	long flags;			/* call or return */
	long cid;			/* connection ID (relative to FD) */
	long xid;			/* call ID */
	long opcode;			/* what function to invoke */
	long code;			/* return code */
	long len;			/* bytes in buffer */
	char buffer[WLR_BUFFERSIZE];	/* user data */
    } d;				/* stuff send over the wire */
};

/* wlr_packet flags */
#define WLR_PACKET_TYPEMASK	0xf	/* mask for type in flags */
#define WLR_PACKET_TYPECALL	1	/* a call packet */
#define WLR_PACKET_TYPERETURN	2	/* a return packet */

/* structure representing user interface to a remote client */
struct wlr_client {
    struct squeue q;
    struct sockaddr_in remoteAddr;
    struct wlr_call *callp;
    char *name;
    long flags;
    long userFD;
};

/* client *set* */
struct wlr_clientSet {
    struct squeue q;
    long flags;
};

#define wlr_CallOf(acp)	((acp)->callp)		/* get call from client */

/* for user convenience */
#define wlr_GetFD(clp)	((clp)->userFD)
#define wlr_SetFD(clp,val)	((clp)->userFD = (val))

#define wlr_SendBuf(aclientp)	((aclientp)->s)
#define wlr_RecvBuf(aclientp)	((aclientp)->r)

/* structure representing a single call at the client */
/* states are 0 (before call), BUSY after call has started, BUSY|DONE
 * after call has completed and results are waiting, and then 0 after
 * the client has read out the results or waited for the call (discarding
 * returned parameters).
 */
#define WLR_CALL_BUSY		1	/* call has started */
#define WLR_CALL_DONE		2	/* call has completed */
#define WLR_CALL_BAD		4	/* FD has problems (ignore) */
#define WLR_CALL_SERVER		8	/* server connection */
#define WLR_CALL_ACTIVE		0x10	/* call is in the active list */
struct wlr_call {
    long xid;			/* request ID we want */
    long cid;			/* client ID */
    int refCount;		/* reference count */
    int socket;			/* socket */
    struct wlr_call *nextp;	/* next call to deal with */
    struct wlr_client *clientp;	/* client handling this call */
    char *mp;			/* marshalling ptr; pts to r/s buffer */
    struct wlr_packet *r;	/* server:request, client:response */
    struct wlr_packet *s;	/* server:response (client req done directly) */
    long states;
};

#define COM_ERR_NOTIDLE		100	/* call not idle at starte */

/* global list of client connections */
extern struct wlr_client *wlr_clientListp;
extern int wlr_Inited;

/* prototypes */

/* com.c */
extern struct wlr_packet *wlr_GetPacket _TAKES((void));
extern wlr_FreePacket _TAKES((struct wlr_packet *pkt));
extern wlr_CallBad _TAKES((struct wlr_call *call));
extern wlr_Init _TAKES((void));
static struct wlr_call *wlr_FindCall _TAKES((int fd, long cid, int isClient));
extern struct wlr_call *wlr_GetCall _TAKES((void));
extern wlr_FreeCall _TAKES((struct wlr_call *callp));
extern struct wlr_call *wlr_NewServerCall _TAKES((int afd, long cid));
extern wlr_ServerListen _TAKES((int as, int (*procp)()));
extern wlr_AsyncServerListen _TAKES((int as, int (*procp)()));
extern wlr_ACall _TAKES((struct wlr_call *callp, long opcode));
extern wlr_ABegin _TAKES((struct wlr_client *clientp,
			  struct wlr_call **callpp));
extern wlr_Hold _TAKES((struct wlr_call *callp));
extern wlr_Rele _TAKES((struct wlr_call *callp));
extern wlr_Activate _TAKES((struct wlr_call *callp));
extern wlr_Deactivate _TAKES((struct wlr_call *callp));
extern wlr_ResetCall _TAKES((struct wlr_call *callp));
extern wlr_AWaitCall _TAKES((struct wlr_call *callp));
extern wlr_AWait _TAKES((struct wlr_client *cp));
extern wlr_AWaitAll _TAKES((struct wlr_clientSet *setp));
extern wlr_ntohPacket _TAKES((struct wlr_packet *am));
extern wlr_htonPacket _TAKES((struct wlr_packet *am));
extern wlr_panic _TAKES((char *msg));
extern wlr_read _TAKES((int fd, char *buffer, long bufferSize));
static Listen _TAKES((struct wlr_call *callp));
extern wlr_ResetAll _TAKES((struct wlr_clientSet *cp));
extern wlr_AfterServerCall _TAKES((struct wlr_call *callp, long code));

/* client.c */

extern wlr_NewClient _TAKES((struct sockaddr_in *sockp, char *name,
	      struct wlr_client **newClientpp));
extern wlr_ReverseBind _TAKES((struct wlr_call *callp,
			       struct wlr_client **newcpp));
extern wlr_ClientSetInit _TAKES((struct wlr_clientSet *setp));
extern long wlr_Count _TAKES((struct wlr_clientSet *setp));
extern struct wlr_client *wlr_ClientSetFirst
    _TAKES((struct wlr_clientSet *setp));
extern struct wlr_client *wlr_ClientSetNext _TAKES((struct wlr_clientSet *setp,
					    struct wlr_client *clientp));
extern wlr_ClientSetRotate _TAKES((struct wlr_clientSet *setp));
extern wlr_ClientSetAdd _TAKES((struct wlr_clientSet *setp,
				struct wlr_client *cp));
extern wlr_ClientSetRemove _TAKES((struct wlr_clientSet *setp,
				   struct wlr_client *cp));
extern wlr_ClientSetSplit _TAKES((struct wlr_clientSet *baseSetp,
			  struct wlr_clientSet *newSetp,
			  int fraction,
			  int min));
extern wlr_ClientSetCopy _TAKES((struct wlr_clientSet *isp,
				 struct wlr_clientSet *osp));
extern wlr_ClientSetClear _TAKES((struct wlr_clientSet *sp));

#endif /* _COM_H_ENV_ */
