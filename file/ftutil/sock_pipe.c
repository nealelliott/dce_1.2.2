/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sock_pipe.c,v $
 * $EndLog$
*/
/*
 * sock_pipe.c
 *
 * Socket realization of pipe abstraction for ftserver.
 *
 * (C) Copyright 1996 Transarc Corporation
 * All Rights Reserved
 */
#if !defined(OSF_NO_SOCKET_DUMP)
#include <dcedfs/param.h>
#include <dcedfs/common_data.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_dfserrors.h>
#include <sys/errno.h>
#include <dce/rpc.h>
#include <dcedfs/compat.h>

#include <pipe.h>
#include <sock_pipe.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/ftutil/RCS/sock_pipe.c,v 1.19 1996/10/12 18:13:32 madhuc Exp $")

/*
 * This package implements data transfers over TCP or Unix domain sockets.
 * A two stage protocol is used to establish the data connections to ensure
 * that the connection is established between the client and the server
 * before any data is transferred.
 *
 * In the first stage, the client listens on a socket and sends a request
 * for the server to establish a connection to the client. The server
 * connects and returns the addresses of the connection endpoints to
 * the client along with an identifier that the client can use to refer
 * to the connection in subsequent RPC calls.
 *
 * In the second stage, the client checks the connection endpoints returned
 * by the server against the connection endpoints of its connection. If
 * the two match, then the client is connected to the server, otherwise
 * the client is connected to some third party, and the connection is aborted.
 * If the connection endpoints check out, then the client is able to use
 * the connection for a bulk data transfer by passing the connection ID
 * as a parameter in an RPC call. The server times out connections if the
 * client does not respond within the timeout interval.
 *
 * Two types of sockets are supported, TCP sockets and Unix domain sockets.
 * TCP connections are not secure and should be used with caution.
 *
 * Unix domain sockets are as secure as the processor they are on,
 * and provide for high performance bulk data transfers between two
 * processes on the same machine.
 */

/*
 * SERVER SIDE ROUTINES
 */

/*
 * readSockPipe
 *
 * This is the read op inside the generic afsPipe structure.  It reads in the
 * specified number of bytes (if possible); a short read indicates an EOF
 * condition (the EOF flag is also set in this case.)
 *
 * -1 return indicates an error.
 */

static int readSockPipe(
    afsPipe_t *obj,
    char *buf,
    unsigned size)
{
    srvrSock_t *srvrSockP;
    long len;
    long cnt;

    srvrSockP = (srvrSock_t *)obj->private_data;

    if (obj->pipe_error) {
	srvrSockP->status = obj->pipe_error;
	return -1;
    }

    if (!(obj->flags & PFL_READ)) {
	srvrSockP->status = EOPNOTSUPP;
	return -1;
    }

    if (obj->flags & (PFL_CLOSED|PFL_EOF)) {
	srvrSockP->status = EBADF;
	return -1;
    }

    /* Use any data we have in the pipe buffer */
    cnt = srvrSockP->dataBufSize - srvrSockP->dataBufOff;
    if (cnt > 0) {
	if (cnt > size) cnt = size;
	bcopy(srvrSockP->dataBuf + srvrSockP->dataBufOff, buf, cnt);
	srvrSockP->dataBufOff += cnt;
	if ( srvrSockP->dataBufOff == srvrSockP->dataBufSize) {
	    srvrSockP->dataBufOff = 0;
	    srvrSockP->dataBufSize = 0;
	}
    }

    while (cnt < size && !(obj->flags & PFL_EOF)) {
	if (size - cnt >= SOCK_BUFFER_SIZE) {
	    /* read the directly into the caller's buffer */
	    len = read(srvrSockP->sock, buf + cnt, size - cnt);
	    if (len <= 0) {
		if (len < 0) srvrSockP->status = errno;
		obj->flags |= PFL_EOF;
	    } else cnt += len;
	} else {
	    /* refill the buffer, copy what we need */
	    len = read(srvrSockP->sock, srvrSockP->dataBuf, SOCK_BUFFER_SIZE);
	    if (len <= 0) {
		if (len < 0) srvrSockP->status = errno;
		obj->flags |= PFL_EOF;
		break;
	    }
	    srvrSockP->dataBufSize = len;
	    if (len > size - cnt) len = size - cnt;
	    bcopy(srvrSockP->dataBuf, buf + cnt, len);
	    srvrSockP->dataBufOff = len;
	    cnt += len;
	}
    }

    return cnt;
}

/*
 * writeSockPipe
 *
 * This is the write op inside the generic afsPipe structure.  It writes in
 * the specified number of bytes (if possible).
 *
 * -1 indicates an error.
 */

static int writeSockPipe(
    afsPipe_t *obj,
    char *buf,
    unsigned size)
{
    srvrSock_t *srvrSockP;
    long rc;
    long cnt;

    srvrSockP = (srvrSock_t *)obj->private_data;

    if (obj->pipe_error) {
	srvrSockP->status = obj->pipe_error;
	return -1;
    }

    if (!(obj->flags & PFL_WRITE)) {
	srvrSockP->status = EOPNOTSUPP;
	return -1;
    }

    if (obj->flags & (PFL_CLOSED|PFL_EOF)) {
	srvrSockP->status = EBADF;
	return -1;
    }

    /* if we already have a partially full buffer, use it */
    if (srvrSockP->dataBufSize) {
	cnt = SOCK_BUFFER_SIZE - srvrSockP->dataBufSize;
	if (cnt > size) cnt = size;
	bcopy(buf, srvrSockP->dataBuf + srvrSockP->dataBufOff, cnt);
	srvrSockP->dataBufOff += cnt;
	srvrSockP->dataBufSize += cnt;
	
	/* transmit the buffer if it is full */
	if (srvrSockP->dataBufSize == SOCK_BUFFER_SIZE) {
	    rc = write(srvrSockP->sock, srvrSockP->dataBuf, SOCK_BUFFER_SIZE);
	    if (rc < 0) {
		srvrSockP->status = errno;
		return -1;
	    }
	    srvrSockP->dataBufOff = 0;
	    srvrSockP->dataBufSize = 0;
	}
    } else cnt = 0;

    if (size - cnt >= SOCK_BUFFER_SIZE) {
	/* send the remainder directly */
	rc = write(srvrSockP->sock, buf + cnt, size - cnt);
	if (rc < 0) {
	    srvrSockP->status = errno;
	    return -1;
	}
	return size;
    } else if (size) {
	/* copy the rest into the pipe buffer */
	bcopy(buf + cnt, srvrSockP->dataBuf + srvrSockP->dataBufOff, size - cnt);
	srvrSockP->dataBufOff += size - cnt;
	srvrSockP->dataBufSize += size - cnt;
    }

    return size;
}

/*
 * holdSockPipe
 *
 * this is the hold op inside the generic afsPipe structure.  It simply
 * bumps the reference count.
 *
 */

static void holdSockPipe(
    afsPipe_t *obj)
{
    obj->refCount++;
}

/*
 * releaseSockPipe
 *
 * this is the release op inside the generic afsPipe structure.  It
 * decrements the reference count, and when the count goes to zero it
 * deletes the pipe resources.  Future references to the pipe are not
 * allowed
 */

static void releaseSockPipe(
    afsPipe_t *obj)
{
    srvrSock_t *srvrSockP;

    obj->refCount--;
    if (obj->refCount)
	return;
    /* last reference, delete */
    srvrSockP = (srvrSock_t *)obj->private_data;

    if (!(obj->flags & PFL_CLOSED))
	(*(obj->pipe_close))(obj, (obj->flags & PFL_DUPLEX));

    termSrvrSock(srvrSockP);
}

/*
 * closeSockPipe
 *
 * This is the close op inside the generic afsPipe structure.  It marks the
 * pipe as closed.  In the read pipe case, it drains all the remaining
 * input from the pipe.
 */

static int closeSockPipe(
    afsPipe_t *obj,
    unsigned flags)
{
    srvrSock_t *srvrSockP;
    int rc;

    if(obj->flags & PFL_NOCLOSE) /* do nothing */
	return 0;

    srvrSockP = (srvrSock_t *)obj->private_data;

    if (flags != (obj->flags & PFL_DUPLEX)) {
	srvrSockP->status = EINVAL;
	return -1;
    }

    if (obj->pipe_error) {
	srvrSockP->status = obj->pipe_error;
	return -1;
    }

    if ((flags & PFL_READ) && (obj->flags & PFL_EOF) == 0) {
	/* read remaining data */
	do {
	    rc = read(srvrSockP->sock, srvrSockP->dataBuf, SOCK_BUFFER_SIZE);
	} while(rc > 0);
    }

    if (flags & PFL_WRITE) {
	/* flush remaining data */
	if (srvrSockP->dataBufSize > 0) {
	    rc = write(srvrSockP->sock, srvrSockP->dataBuf, srvrSockP->dataBufSize);
	}
    }

    /* close the connection */
    close(srvrSockP->sock);
    srvrSockP->sock = -1;

    obj->flags |= (PFL_CLOSED|PFL_EOF);

    return 0;
 }

/*
 * statSockPipe
 *
 * this code returns the current error status value of the given afsPipe
 * object.
 */
static int statSockPipe(
    afsPipe_t *obj,
    unsigned type,
    long *errVal)
{
    srvrSock_t *srvrSockP;

    srvrSockP = (srvrSock_t *)obj->private_data;

    *errVal = srvrSockP->status;

    return 0;
}

/*
 * crashSockPipe
 *
 * Abort use of this pipe.
 */
static int crashSockPipe(
    afsPipe_t *obj,
    unsigned errorCode)
{
    srvrSock_t *srvrSockP;

    obj->pipe_error = errorCode;

    return 0;
}

/* exported routines */

/*
 * createSockPipe
 *
 * This routine takes a pointer to an socket pipe structure, an afs pipe
 * structure, and a flags field (which tells which type of pipe this is)
 *
 * It initializes the afsPipeP structure to allow future use for manipulating
 * this pipe using the standard pipe operators.
 *
 * Side effects: this sets the reference count to afsPipeP.
 *
 * Locking: no locking is done within this package.  The caller must do any
 * locking necessary.
 *
 */

long createSockPipe(
    srvrSock_t *srvrSockP,
    afsPipe_t *afsPipeP,
    unsigned flags)
{
    unsigned noCloseFlag;
    long error;

    if (!srvrSockP || !afsPipeP) {
	error = EINVAL;
	goto error_exit;
    }

    noCloseFlag = flags & PFL_NOCLOSE;
    flags &= ~PFL_NOCLOSE;

    /* unidirectional pipes only */
    if (flags != PFL_READ && flags != PFL_WRITE) {
	error = EINVAL;
	goto error_exit;
    }

    srvrSockP->dataBufOff = 0;
    srvrSockP->dataBufSize = 0;
    srvrSockP->status = 0;

    afsPipeP->refCount = 1;
    afsPipeP->flags = flags | noCloseFlag;
    afsPipeP->pipe_error = 0;
    afsPipeP->pipe_read = readSockPipe;
    afsPipeP->pipe_write = writeSockPipe;
    afsPipeP->pipe_hold = holdSockPipe;
    afsPipeP->pipe_rele = releaseSockPipe;
    afsPipeP->pipe_close = closeSockPipe;
    afsPipeP->pipe_stat = statSockPipe;
    afsPipeP->pipe_crash = crashSockPipe;
    afsPipeP->private_data = (char *)srvrSockP;
    return 0;

error_exit:
    termSrvrSock(srvrSockP);
    return error;
}

/*
 * getLocalInetAddr
 *
 * Get a local IP address, if the value of hostAddr zero, then the
 * result is the address any enabled network interface. If the value of
 * hostAddr is non-zero, then the result will be set to the value of
 * hostAddr only if an enabled network interface is found matching the
 * hostAddr specified (tests whether hostAddr is local).
 */
long getLocalInetAddr(
    unsigned32 hostAddr,
    unsigned32 *resultP)
{
    long code;
    int sock;
    int cnt;
    int rc;
    int i;
    int ifc_len;
    struct ifconf ifc;
    struct ifreq *ifrp;
    unsigned32 lclAddr;
    unsigned32 ifFlags;
    unsigned32 ifMask;

    code = 0;
    ifc.ifc_buf = NULL;
    sock = -1;
    *resultP = 0;

    /*
     * open a TCP socket and retrieve a list of IP addresses
     * for each Internet Protocol network interface
     */

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
	code = errno;
	goto exit;
    }

    ifc_len = SOCK_MAXHOSTADDR * sizeof(struct ifreq);
    ifc.ifc_len = ifc_len;
    ifc.ifc_buf = (char *)osi_Alloc(ifc.ifc_len);
    if (!ifc.ifc_buf) {
	code = ENOMEM;
	goto exit;
    }

    rc = ioctl(sock, SIOCGIFCONF, &ifc);
    if (rc < 0) {
	code = errno;
	goto exit;
    }
    cnt = ifc.ifc_len / sizeof(struct ifreq);

    /*
     * loop through the network interfaces looking for a match.
     * Ignore interfaces that are configured down, and do not
     * return the loopback interface if no hostAddr was specified.
     */
    for (i = 0, ifrp = ifc.ifc_req ; i < cnt ; i++, ifrp++) {
	rc = ioctl(sock, SIOCGIFADDR, ifrp);
	if (rc < 0) {
	    code = errno;
	    goto exit;
	}
	lclAddr = ntohl(((struct sockaddr_in *)
		        &(ifrp->ifr_addr))->sin_addr.s_addr);
	rc = ioctl(sock, SIOCGIFFLAGS, ifrp);
	if (rc < 0) {
	    code = errno;
	    goto exit;
	}
	if (ifrp->ifr_flags & IFF_UP) {
	    if (hostAddr == lclAddr) {
		*resultP = hostAddr;
		goto exit;
	    }
	    if (!(ifrp->ifr_flags & IFF_LOOPBACK)) {
		*resultP = lclAddr;
		if (!hostAddr) {
		    goto exit;
		}
	    }
	}
    }

exit:
    if (sock >= 0)
	close(sock);
    if (ifc.ifc_buf != NULL)
	osi_Free(ifc.ifc_buf, ifc_len);
    return code;
}

/*
 * connectTcpSock
 *
 * establish a TCP socket to the client and return the servers
 * TCP port and IP address so the client can verify its endpoint.
 * The output socket ID is mapped to a pipe by calling createSockPipe.
 */

long connectTcpSock(
    unsigned16 clntPort,
    unsigned32 clntAddr,
    unsigned16 *srvrPortP,
    unsigned32 *srvrAddrP,
    srvrSock_t **srvrSockPP)
{
    srvrSock_t *srvrSockP = NULL;
    struct sockaddr_in lclAddr;
    struct sockaddr_in remAddr;
    long startTime;
    int sockFlags;
    int optLen;
    int optVal;
    int error = 0;
    int rc;

    if (!srvrPortP || !srvrAddrP || !srvrSockPP)
	return EINVAL;

    srvrSockP = (srvrSock_t *)osi_Alloc(sizeof(srvrSock_t));
    if (!srvrSockP) {
	error = ENOMEM;
	goto error_exit;
    }
    srvrSockP->sockFile = NULL;
    srvrSockP->sock = -1;
    srvrSockP->dataBuf = osi_Alloc(SOCK_BUFFER_SIZE);
    if (!srvrSockP->dataBuf) {
	error = ENOMEM;
	goto error_exit;
    }

    /*
     * create a socket socket to use for the connection
     */
    srvrSockP->sock = socket(PF_INET, SOCK_STREAM, 0);
    if (srvrSockP->sock < 0) {
	error = errno;
	goto error_exit;
    }

    /*
     * put  the socket into non blocking mode so the connect
     * won't block the process.
     */
    sockFlags = fcntl(srvrSockP->sock, F_GETFL);
    if (sockFlags < 0) {
	error = errno;
	goto error_exit;
    }
    sockFlags |= O_NONBLOCK;
    rc = fcntl(srvrSockP->sock, F_SETFL, sockFlags);
    if (rc < 0) {
	error = errno;
	goto error_exit;
    }

    /*
     * bind to any available port
     */
    bzero((char *)&lclAddr, sizeof(lclAddr));
    lclAddr.sin_family = AF_INET;
    lclAddr.sin_port = 0;
    lclAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    rc = bind(srvrSockP->sock, (struct sockaddr *)&lclAddr, sizeof(lclAddr));
    if (rc < 0) {
        error = errno;
        goto error_exit;
    }

    /*
     * format the client's network address
     */
    bzero((char *)&remAddr, sizeof(remAddr));
    remAddr.sin_family = AF_INET;
    remAddr.sin_port = htons(clntPort);
    remAddr.sin_addr.s_addr = htonl(clntAddr);
     
    /*
     * connect to the remote host
     */
    startTime = osi_Time();
    while (1) {
	rc = connect(srvrSockP->sock, (struct sockaddr *)&remAddr,
		     sizeof(remAddr));
	if (rc == 0 || errno == EISCONN) break;
	else if (errno != EALREADY &&
		 errno != EWOULDBLOCK &&
		 errno != EINPROGRESS &&
		 errno != EAGAIN) {
	    error = errno;
	    goto error_exit;
	}
	osi_Pause(1);
	if ( osi_Time() - startTime > SOCK_CONNECT_TIMEOUT) {
	    error = ETIMEDOUT;
	    goto error_exit;
	}
    }

    /*
     * find out what port we are using
     */
    optLen = sizeof(lclAddr);
    rc = getsockname(srvrSockP->sock, (struct sockaddr *)&lclAddr, &optLen);
    if (rc < 0) {
	    error = errno;
	    goto error_exit;
    }
    *srvrPortP = ntohs(lclAddr.sin_port);
    *srvrAddrP = ntohl(lclAddr.sin_addr.s_addr);

    /*
     * set the socket send and receive buffer sizes
     */
    optVal = SOCK_RCVBUF_SIZE;
    rc = setsockopt(srvrSockP->sock, SOL_SOCKET, SO_RCVBUF,
		    (char *)&optVal, sizeof(int));
    if (rc < 0) {
	error = errno;
	goto error_exit;
    }
    optVal = SOCK_SNDBUF_SIZE;
    rc = setsockopt(srvrSockP->sock, SOL_SOCKET, SO_SNDBUF,
		    (char *)&optVal, sizeof(int));
    if (rc < 0) {
	error = errno;
	goto error_exit;
    }

    /*
     * put the socket into blocking mode for the duration
     */
    sockFlags &= ~O_NONBLOCK;
    rc = fcntl(srvrSockP->sock, F_SETFL, sockFlags);
    if (rc < 0) {
	error = errno;
	goto error_exit;
    }

    *srvrSockPP = srvrSockP;
    return 0;

error_exit:
    termSrvrSock(srvrSockP);
    return error;
}

/*
 * connectUnixSock
 *
 * establish a UNIX socket to the client and return the servers
 * address so the client can verify its endpoint. The output socket ID
 * is mapped to a pipe by calling createSockPipe.
 */

long connectUnixSock(
    char *clntAddr,
    char *srvrAddr,
    srvrSock_t **srvrSockPP)
{
    srvrSock_t *srvrSockP = NULL;
    long startTime;
    struct sockaddr_un lclAddr;
    struct sockaddr_un remAddr;
    int sockFlags;
    int optLen;
    int optVal;
    int error = 0;
    int rc;

    if (!clntAddr || !srvrAddr || !srvrSockPP)
	return EINVAL;

    srvrSockP = (srvrSock_t *)osi_Alloc(sizeof(srvrSock_t));
    if (!srvrSockP) {
	error = ENOMEM;
	goto error_exit;
    }
    srvrSockP->sockFile = tempnam(SOCK_FILE_DIR, SOCK_FILE_PREFIX);
    if (!srvrSockP->sockFile) {
	error = errno;
	goto error_exit;
    }
    srvrSockP->sock = -1;
    srvrSockP->dataBuf = osi_Alloc(SOCK_BUFFER_SIZE);
    if (!srvrSockP->dataBuf) {
	error = ENOMEM;
	goto error_exit;
    }

    /*
     * create a socket socket to use for the connection
     */
    srvrSockP->sock = socket(PF_UNIX, SOCK_STREAM, 0);
    if (srvrSockP->sock < 0) {
	error = errno;
	goto error_exit;
    }

    /*
     * set the socket send and receive buffer sizes
     */
    optVal = SOCK_RCVBUF_SIZE;
    rc = setsockopt(srvrSockP->sock, SOL_SOCKET, SO_RCVBUF,
		    (char *)&optVal, sizeof(int));
    if (rc < 0) {
	error = errno;
	goto error_exit;
    }
    optVal = SOCK_SNDBUF_SIZE;
    rc = setsockopt(srvrSockP->sock, SOL_SOCKET, SO_SNDBUF,
		    (char *)&optVal, sizeof(int));
    if (rc < 0) {
	error = errno;
	goto error_exit;
    }

    /*
     * put  the socket into non blocking mode so the connect
     * won't block the process.
     */
    sockFlags = fcntl(srvrSockP->sock, F_GETFL);
    if (sockFlags < 0) {
	error = errno;
	goto error_exit;
    }
    sockFlags |= O_NONBLOCK;
    rc = fcntl(srvrSockP->sock, F_SETFL, sockFlags);
    if (rc < 0) {
	error = errno;
	goto error_exit;
    }

    /*
     * bind to a unique temporary file name
     */
    bzero((char *)&lclAddr, sizeof(lclAddr));
    lclAddr.sun_family = AF_UNIX;
    strcpy(lclAddr.sun_path, srvrSockP->sockFile);
    rc = bind(srvrSockP->sock, (struct sockaddr *)&lclAddr, sizeof(lclAddr));
    if (rc < 0) {
        error = errno;
        goto error_exit;
    }

    /*
     * format the client's network address
     */
    bzero((char *)&remAddr, sizeof(remAddr));
    remAddr.sun_family = AF_UNIX;
    strcpy(remAddr.sun_path, clntAddr);
     
    /*
     * connect to the remote host
     */
    startTime = osi_Time();
    while (1) {
	rc = connect(srvrSockP->sock, (struct sockaddr *)&remAddr,
		     sizeof(remAddr));
	if (rc == 0 || errno == EISCONN) break;
	else if (errno != EALREADY &&
		 errno != EWOULDBLOCK &&
		 errno != EINPROGRESS &&
		 errno != EAGAIN) {
	    error = errno;
	    goto error_exit;
	}
	osi_Pause(1);
	if ( osi_Time() - startTime > SOCK_CONNECT_TIMEOUT) {
	    error = ETIMEDOUT;
	    goto error_exit;
	}
    }
    strcpy(srvrAddr, srvrSockP->sockFile);

    /*
     * put the socket into blocking mode for the duration
     */
    sockFlags &= ~O_NONBLOCK;
    rc = fcntl(srvrSockP->sock, F_SETFL, sockFlags);
    if (rc < 0) {
	error = errno;
	goto error_exit;
    }

    *srvrSockPP = srvrSockP;
    return 0;

error_exit:
    termSrvrSock(srvrSockP);
    return error;
}

/*
 * termSrvrSock
 *
 * this routine removes the specified socket from the ready list
 * and frees all resources.
 */

long termSrvrSock(
    srvrSock_t *srvrSockP)
{
    if (!srvrSockP) {
	return(EINVAL);
    }
    if (srvrSockP->dataBuf) osi_Free(srvrSockP->dataBuf, SOCK_BUFFER_SIZE);
    if (srvrSockP->sockFile) {
	unlink(srvrSockP->sockFile);
	free(srvrSockP->sockFile);
    }
    if (srvrSockP->sock >= 0) close(srvrSockP->sock);
    osi_Free(srvrSockP, sizeof(srvrSock_t));
    return(0);
}

/*
 * srvrSockFromClnt
 *
 * This routine converts a client socket into a server socket. Once a
 * server has connected to another server using the client side routines,
 * the server calls this routine so it can use the data connection with
 * the server side routines.
 */

long srvrSockFromClnt(
    clntSock_t *clntSockP,
    srvrSock_t **srvrSockPP)
{
    long error = 0;
    srvrSock_t *srvrSockP = NULL;

    if (!clntSockP || !srvrSockPP) {
	error = EINVAL;
	goto error_exit;
    }

    /*
     * allocate the server socket descriptor
     */
    srvrSockP = (srvrSock_t *)osi_Alloc(sizeof(srvrSock_t));
    if (!srvrSockP) {
	error = ENOMEM;
	goto error_exit;
    }
    srvrSockP->sockFile = NULL;
    srvrSockP->sock = -1;
    srvrSockP->dataBuf = osi_Alloc(SOCK_BUFFER_SIZE);
    if (!srvrSockP->dataBuf) {
	error = ENOMEM;
	goto error_exit;
    }

    /*
     * convert the client descriptor to a server descriptor
     */
    srvrSockP->sockFile = clntSockP->sockFile;
    clntSockP->sockFile = NULL;
    srvrSockP->sock = clntSockP->sock;
    clntSockP->sock = -1;

    /*
     * free the client descripor
     */
    termClntSock(clntSockP);
    clntSockP = NULL;

    /*
     * return the new server descriptor
     */
    *srvrSockPP = srvrSockP;
    return 0;

error_exit:
    if (srvrSockP) {
	termSrvrSock(srvrSockP);
    }
    if (clntSockP) {
	termClntSock(clntSockP);
    }
    return error;
}

/*
 * CLIENT SIDE ROUTINES
 */

/*
 * clntSockThreadRtn
 *
 * thread that handles the client end of a socket pipe
 */

void clntSockThreadRtn(
    void *argP)
{
    clntSock_t *clntSockP = (clntSock_t *)argP;
    int rc;
    int newsock;
    int sockFlags;
    long error = 0;
    idl_byte *buf;
    idl_ulong_int nbytes;
    idl_ulong_int bsize;
    struct sockaddr *remAddr;
    int addrLen;

    /*
     * put the socket into non blocking mode so the accept
     * won't block the process if the server hasn't connected.
     */
    sockFlags = fcntl(clntSockP->sock, F_GETFL);
    if (sockFlags < 0) {
	error = errno;
	goto exit;
    }
    sockFlags |= O_NONBLOCK;
    rc = fcntl(clntSockP->sock, F_SETFL, sockFlags);
    if (rc < 0) {
	error = errno;
	goto exit;
    }

    /*
     * wait for the server to connect.
     */
    while (1) {
	lock_ObtainWrite(&clntSockP->lock);
	if (clntSockP->flags & SOCK_CLNT_ABORT) {
	    lock_ReleaseWrite(&clntSockP->lock);
	    goto exit;
	}
	if (clntSockP->flags & SOCK_CLNT_SRVCONN) {
	    lock_ReleaseWrite(&clntSockP->lock);
	    error = ENOTCONN;
	    goto exit;
	}
	lock_ReleaseWrite(&clntSockP->lock);
	if (clntSockP->sockDomain == PF_INET) {
	    remAddr = (struct sockaddr *)&clntSockP->inetAddr;
	    addrLen = sizeof(struct sockaddr_in);
	} else {
	    remAddr = (struct sockaddr *)&clntSockP->unixAddr;
	    addrLen = sizeof(struct sockaddr_un);
	}
        newsock = accept(clntSockP->sock, remAddr, &addrLen);
        if (newsock >= 0) {
	    close(clntSockP->sock);
	    clntSockP->sock = newsock;
	    break;
	} else if (errno != EWOULDBLOCK && errno != EAGAIN) {
	    error = errno;
	    goto exit;
        }
	osi_Pause(1);
    }

    /*
     * put the new socket into blocking mode for the duration.
     */
    sockFlags = fcntl(clntSockP->sock, F_GETFL);
    if (sockFlags < 0) {
	error = errno;
	goto exit;
    }
    sockFlags &= ~O_NONBLOCK;
    rc = fcntl(clntSockP->sock, F_SETFL, sockFlags);
    if (rc < 0) {
	error = errno;
	goto exit;
    }

    /*
     * notify the client that we are connected and wait for the client
     * to validate the servers address before transferring any data.
     * If the pipe pointer is NULL, then wait to be converted to a 
     * server connection, this thread will not manage the data transfer.
     */
    lock_ObtainWrite(&clntSockP->lock);
    clntSockP->flags |= SOCK_CLNT_CONNECT;
    osi_Wakeup(clntSockP);
    while(!clntSockP->pipeP || !(clntSockP->flags & SOCK_CLNT_ACCEPT)) {
	osi_SleepW(clntSockP, &clntSockP->lock);
	lock_ObtainWrite(&clntSockP->lock);
	if (clntSockP->flags & SOCK_CLNT_ABORT) {
	    lock_ReleaseWrite(&clntSockP->lock);
	    goto exit;
	}
    }
    sockFlags = clntSockP->flags;
    lock_ReleaseWrite(&clntSockP->lock);

    /*
     * do the data transfer
     */

    while (1) {
	if (sockFlags & SOCK_CLNT_RECV) {
	    /* allocate a receive buffer */
	    (*(clntSockP->pipeP->alloc))(clntSockP->pipeP->state,
					 SOCK_ALLOC_SIZE, &buf, &nbytes);
	    /* read some data into the client's buffer */
	    rc = read(clntSockP->sock, (char *)buf, (int)nbytes);
	    if (rc < 0) {
		(*(clntSockP->pipeP->push))(clntSockP->pipeP->state,
					    NULL, 0);
		error = errno;
		goto exit;
	    }

	    /* push the data to the client */
	    (*(clntSockP->pipeP->push))(clntSockP->pipeP->state,
					buf, (idl_ulong_int)rc);
	    /* zero length buffer indicates EOF */
	    if (rc == 0)
		break;
	} else {
	    /* allocate a transmit buffer */
	    (*(clntSockP->pipeP->alloc))(clntSockP->pipeP->state,
					 SOCK_ALLOC_SIZE, &buf, &nbytes);

	    /* pull some data into the client's buffer */
	    (*(clntSockP->pipeP->pull))(clntSockP->pipeP->state,
					buf, nbytes, &nbytes);

	    /* zero length buffer indicates EOF */
	    if (nbytes == 0)
		break;

	    /* send the data */
	    rc = write(clntSockP->sock, (char *)buf, (int)nbytes);
	    if (rc < 0) {
		error = errno;
		/* drain the client's data */
		do {
		  (*(clntSockP->pipeP->alloc))(clntSockP->pipeP->state,
					       SOCK_ALLOC_SIZE, &buf,
					       &nbytes);
		  (*(clntSockP->pipeP->pull))(clntSockP->pipeP->state,
					      buf, nbytes, &nbytes);
		} while (nbytes != 0);
		goto exit;
	    }
	}
    }

exit:
    /*
     * close the connection
     */
    if (clntSockP->sock >= 0) {
	rc = close(clntSockP->sock);
	if (rc < 0 && error == 0)
	    error = errno;
	clntSockP->sock = -1;
    }
    
    /*
     * notify the client that we are done
     */
    lock_ObtainWrite(&clntSockP->lock);
    clntSockP->error = error;
    clntSockP->flags &= ~SOCK_CLNT_RUNNING;
    osi_Wakeup(clntSockP);
    lock_ReleaseWrite(&clntSockP->lock);
    return;
}

/*
 * begin listening on a TCP socket
 */

long lstnTcpSock(
    unsigned flags,
    pipe_t *pipeP,
    unsigned16 *sockPortP,
    unsigned32 *sockAddrP,
    clntSock_t **clntSockPP)
{
    int rc;
    int optLen;
    long error = 0;
    clntSock_t *clntSockP = NULL;
    unsigned32 lclHost;
    struct sockaddr_in lclAddr;
    struct hostent *hep;

    if (!sockPortP || !clntSockPP)
	return EINVAL;

    /* unidirectional connections only */
    if (flags != SOCK_CLNT_RECV && flags != SOCK_CLNT_XMIT)
	return EINVAL;

    /*
     * get a valid IP address for this machine
     */
    error = getLocalInetAddr(0, &lclHost);
    if (error)
	return error;

    /* allocate the client state structure */
    clntSockP = osi_Alloc(sizeof(clntSock_t));
    if (!clntSockP) {
	error = ENOMEM;
	goto error_exit;
    }
    lock_Init(&clntSockP->lock);
    clntSockP->sockDomain = PF_INET;
    clntSockP->flags = flags;
    clntSockP->pipeP = pipeP;
    clntSockP->sock = -1;
    clntSockP->sockFile = NULL;
    clntSockP->error = 0;

    /*
     * create a socket to use for the connection
     */
    clntSockP->sock = socket(PF_INET, SOCK_STREAM, 0);
    if (clntSockP->sock < 0) {
	error = errno;
	goto error_exit;
    }

    /*
     * bind the local socket to any available port
     */
    bzero((char *)&lclAddr, sizeof(lclAddr));
    lclAddr.sin_family = AF_INET;
    lclAddr.sin_port = 0;
    lclAddr.sin_addr.s_addr = htonl(lclHost);
    rc = bind(clntSockP->sock, (struct sockaddr *)&lclAddr, sizeof(lclAddr));
    if (rc < 0) {
	error = errno;
	goto error_exit;
    }

    /*
     * get the port we will be listening on
     */
    optLen = sizeof(lclAddr);
    rc = getsockname(clntSockP->sock, (struct sockaddr *)&lclAddr, &optLen);
    if (rc < 0) {
	error = errno;
	goto error_exit;
    }
    *sockPortP = ntohs(lclAddr.sin_port);
    *sockAddrP = lclHost;

    /*
     * begin listening for connections
     */
    rc = listen(clntSockP->sock, 1);
    if (rc < 0) {
	error = errno;
	goto error_exit;
    }

    /*
     * create a thread to handle this connection
     */
    clntSockP->flags |= SOCK_CLNT_RUNNING;
    osi_ThreadCreate(clntSockThreadRtn, (void *)clntSockP, 0, 0,
		     "tcp", error);
    if (error) {
	clntSockP->flags &= ~SOCK_CLNT_RUNNING;
	goto error_exit;
    }

    *clntSockPP = clntSockP;
    return 0;

error_exit:
    if (clntSockP)
        termClntSock(clntSockP);
    return error;
}

/*
 * accept a connection on a TCP socket
 */

long acceptTcpSock(
    clntSock_t *clntSockP,
    unsigned16 srvrPort,
    unsigned32 srvrAddr)
{
    int rc;
    long error;

    if (!clntSockP)
	return EINVAL;

    /*
     * synchronize with the client thread
     */
    lock_ObtainWrite(&clntSockP->lock);
    clntSockP->flags |= SOCK_CLNT_SRVCONN;
    osi_Wakeup(clntSockP);
    while (!(clntSockP->flags & SOCK_CLNT_CONNECT)) {
	if (!(clntSockP->flags & SOCK_CLNT_RUNNING)) {
	    lock_ReleaseWrite(&clntSockP->lock);
	    error = clntSockP->error;
	    goto error_exit;
	}
	osi_SleepW(clntSockP, &clntSockP->lock);
	lock_ObtainWrite(&clntSockP->lock);
    }
    lock_ReleaseWrite(&clntSockP->lock);

    /*
     * make sure the caller's address matches the address server's address
     */
    if ( ntohs(clntSockP->inetAddr.sin_port) != srvrPort ||
	 ntohl(clntSockP->inetAddr.sin_addr.s_addr) != srvrAddr) {
	error = ECONNREFUSED;
	goto error_exit;
    }

    /*
     * inform the thread that the connection has been accepted
     */
    lock_ObtainWrite(&clntSockP->lock);
    clntSockP->flags |= SOCK_CLNT_ACCEPT;
    osi_Wakeup(clntSockP);
    lock_ReleaseWrite(&clntSockP->lock);

    return 0;

error_exit:
    if (clntSockP)
	termClntSock(clntSockP);
    return error;
}

/*
 * begin listening on a Unix domain socket
 */

long lstnUnixSock(
    unsigned flags,
    pipe_t *pipeP,
    char *sockAddrP,
    clntSock_t **clntSockPP)
{
    int rc;
    long error = 0;
    int optLen;
    int optVal;
    struct sockaddr_un lclAddr;
    clntSock_t *clntSockP = NULL;

    if (!sockAddrP || !clntSockPP)
	return EINVAL;

    /* unidirectional connections only */
    if (flags != SOCK_CLNT_RECV && flags != SOCK_CLNT_XMIT)
	return EINVAL;

    /* allocate the client state structure */
    clntSockP = osi_Alloc(sizeof(clntSock_t));
    if (!clntSockP) {
	error = ENOMEM;
	goto error_exit;
    }
    lock_Init(&clntSockP->lock);
    clntSockP->sockDomain = PF_UNIX;
    clntSockP->flags = flags;
    clntSockP->pipeP = pipeP;
    clntSockP->sock = -1;
    clntSockP->sockFile = NULL;
    clntSockP->error = 0;

    /*
     * create a socket to use for the connection
     */
    clntSockP->sock = socket(PF_UNIX, SOCK_STREAM, 0);
    if (clntSockP->sock < 0) {
	error = errno;
	goto error_exit;
    }

    /*
     * set the socket send and receive buffer sizes
     */
    optVal = SOCK_RCVBUF_SIZE;
    rc = setsockopt(clntSockP->sock, SOL_SOCKET, SO_RCVBUF,
		    (char *)&optVal, sizeof(int));
    if (rc < 0) {
	error = errno;
	goto error_exit;
    }
    optVal = SOCK_SNDBUF_SIZE;
    rc = setsockopt(clntSockP->sock, SOL_SOCKET, SO_SNDBUF,
		    (char *)&optVal, sizeof(int));
    if (rc < 0) {
	error = errno;
	goto error_exit;
    }

    /*
     * bind the local socket to a unique file name
     */
    clntSockP->sockFile = tempnam(SOCK_FILE_DIR, SOCK_FILE_PREFIX);
    if (!clntSockP->sockFile) {
	error = errno;
	goto error_exit;
    }
    bzero((char *)&lclAddr, sizeof(lclAddr));
    lclAddr.sun_family = AF_UNIX;
    strcpy(lclAddr.sun_path, clntSockP->sockFile);
    rc = bind(clntSockP->sock, (struct sockaddr *)&lclAddr, sizeof(lclAddr));
    if (rc < 0) {
	    error = errno;
	    goto error_exit;
    }

    /*
     * begin listening for connections
     */
    rc = listen(clntSockP->sock, 1);
    if (rc < 0) {
	error = errno;
	goto error_exit;
    }

    /*
     * create a thread to handle this connection
     */
    clntSockP->flags |= SOCK_CLNT_RUNNING;
    osi_ThreadCreate(clntSockThreadRtn, (void *)clntSockP, 0, 0,
		     "unix", error);
    if (error) {
	clntSockP->flags &= ~SOCK_CLNT_RUNNING;
	goto error_exit;
    }

    strcpy(sockAddrP, clntSockP->sockFile);
    *clntSockPP = clntSockP;
    return 0;

error_exit:
    if (clntSockP)
        termClntSock(clntSockP);
    return error;
}

/*
 * accept a connection on a Unix socket
 */

long acceptUnixSock(
    clntSock_t *clntSockP,
    char *srvrAddr)
{
    int rc;
    long error;

    if (!clntSockP)
	return EINVAL;

    /*
     * synchronize with the client thread
     */
    lock_ObtainWrite(&clntSockP->lock);
    clntSockP->flags |= SOCK_CLNT_SRVCONN;
    osi_Wakeup(clntSockP);
    while (!(clntSockP->flags & SOCK_CLNT_CONNECT)) {
	if (!(clntSockP->flags & SOCK_CLNT_RUNNING)) {
	    lock_ReleaseWrite(&clntSockP->lock);
	    error = clntSockP->error;
	    goto error_exit;
	}
	osi_SleepW(clntSockP, &clntSockP->lock);
	lock_ObtainWrite(&clntSockP->lock);
    }
    lock_ReleaseWrite(&clntSockP->lock);

    /*
     * make sure the caller's address matches the address server's address
     */
    if (strcmp(clntSockP->unixAddr.sun_path, srvrAddr) != 0) {
	error = ECONNREFUSED;
	goto error_exit;
    }

    /*
     * inform the thread that the connection has been accepted
     */
    lock_ObtainWrite(&clntSockP->lock);
    clntSockP->flags |= SOCK_CLNT_ACCEPT;
    osi_Wakeup(clntSockP);
    lock_ReleaseWrite(&clntSockP->lock);

    return 0;

error_exit:
    if (clntSockP)
	termClntSock(clntSockP);
    return error;
}

/*
 * termClntSock
 *
 * Terminate a client socket and free all resources. If there is a transfer
 * in progress then wait for it to complete.
 */

long termClntSock(
    clntSock_t *clntSockP)
{
    int error;

    if (!clntSockP)
	return EINVAL;

    /*
     * wait for the thread to finish
     */
    lock_ObtainWrite(&clntSockP->lock);
    clntSockP->flags |= SOCK_CLNT_ABORT;
    osi_Wakeup(clntSockP);
    while (clntSockP->flags & SOCK_CLNT_RUNNING) {
	osi_SleepW(clntSockP, &clntSockP->lock);
	lock_ObtainWrite(&clntSockP->lock);
    }
    lock_ReleaseWrite(&clntSockP->lock);

    error = clntSockP->error;

    /*
     * free the resources
     */
    if (clntSockP->sock >= 0) close(clntSockP->sock);
    if (clntSockP->sockFile) {
	unlink(clntSockP->sockFile);
	free(clntSockP->sockFile);
    }
    osi_Free(clntSockP, sizeof(clntSock_t));

    return error;
}
#endif /* OSF_NO_SOCKET_DUMP */
