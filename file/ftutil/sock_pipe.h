/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sock_pipe.h,v $
 * $EndLog$
 */
/*
*/
/*
 * sock_pipe.h
 *
 * (C) Copyright 1996 Transarc Corporation
 * All Rights Reserved.
 *
 */

#ifndef _TRANSARC_SOCK_PIPE_H
#define _TRANSARC_SOCK_PIPE_H
#if !defined(OSF_NO_SOCKET_DUMP)

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <net/if.h>

#if defined(_IBMR2)
#include <sys/time.h>
#include <sys/ioctl.h>
#else /* defined(_IBMR2) */
#include <sys/sockio.h>
#endif /* defined(_IBMR2) */

#include <dcedfs/lock.h>
#include <dcedfs/pipe.h>

#define SOCK_MAXHOSTADDR	64
#define SOCK_BUFFER_SIZE	(4096)
#define SOCK_CONNECT_TIMEOUT	(30)
#define SOCK_ALLOC_SIZE		(65536)
#define SOCK_SNDBUF_SIZE	(32768)
#define SOCK_RCVBUF_SIZE	(32768)
#define SOCK_FILE_DIR		"/tmp"
#define SOCK_FILE_PREFIX	"dfs."

/*
 * state for server side of SOCK pipe
 */

typedef struct srvrSock {
    int sock;				/* socket used for data transfer */
    char *sockFile;			/* filename for Unix sockets */
    char *dataBuf;			/* socket data buffer */
    long dataBufOff;			/* current buffer offset */
    long dataBufSize;			/* current buffer size */
    long status;			/* status flag */
} srvrSock_t;

/*
 * state for client side of SOCK pipe
 */

#define SOCK_CLNT_RECV		0x001	/* receive only connection */
#define SOCK_CLNT_XMIT		0x002	/* transmit only connection */
#define SOCK_CLNT_CONNECT	0x010	/* connection established */
#define SOCK_CLNT_ACCEPT	0x020	/* connection accepted */
#define SOCK_CLNT_ABORT		0x040	/* request thread to abort */
#define SOCK_CLNT_RUNNING	0x080	/* client thread running */
#define SOCK_CLNT_SRVCONN	0x100	/* server is connected */

typedef struct clntSock {
    osi_dlock_t lock;			/* controls access to structure */
    int sock;				/* socket used for data transfer */
    pipe_t *pipeP;			/* DCE pipe mapped to socket */
    int sockDomain;			/* protocol domain for socket */
    struct sockaddr_in inetAddr;	/* internet socket address */
    struct sockaddr_un unixAddr;	/* UNIX socket address */
    char *sockFile;			/* filename used for UNIX socket */
    long flags;				/* state flags */
    long error;				/* error code */
} clntSock_t;

/* declaration(s) */

/*
 * SERVER SIDE ROUTINES
 */

long createSockPipe _TAKES((srvrSock_t *, afsPipe_t *, unsigned));

long connectTcpSock _TAKES((unsigned16, unsigned32, unsigned16 *,
			    unsigned32 *, srvrSock_t **));

long connectUnixSock _TAKES((char *, char *, srvrSock_t **));

long termSrvrSock _TAKES((srvrSock_t *));

long srvrSockFromClnt _TAKES((clntSock_t *, srvrSock_t **));

/*
 * CLIENT SIDE ROUTINES
 */

long lstnTcpSock _TAKES((unsigned, pipe_t *, unsigned16 *, unsigned32 *,
			 clntSock_t **));

long acceptTcpSock _TAKES((clntSock_t *, unsigned16, unsigned32));

long lstnUnixSock _TAKES((unsigned, pipe_t *, char *, clntSock_t **));

long acceptUnixSock _TAKES((clntSock_t *, char *));

long termClntSock _TAKES((clntSock_t *));

long getLocalInetAddr _TAKES((unsigned32, unsigned32 *));

#endif /* OSF_NO_SOCKET_DUMP */
#endif /* _TRANSARC_SOCK_PIPE_H */
