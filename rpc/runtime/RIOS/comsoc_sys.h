/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: comsoc_sys.h,v $
 * Revision 1.1.55.2  1996/02/18  22:55:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:27  marty]
 *
 * Revision 1.1.55.1  1995/12/08  00:16:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:04  root]
 * 
 * Revision 1.1.53.2  1994/01/22  22:49:49  cbrooks
 * 	Code Cleanup - CR 9797
 * 	[1994/01/22  21:22:24  cbrooks]
 * 
 * Revision 1.1.53.1  1994/01/21  22:33:25  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  20:59:43  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  22:57:15  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:57:14  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  20:10:46  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:28:39  zeliff]
 * 
 * Revision 1.1.2.3  1992/06/26  05:57:15  jim
 * 	Various changes for AIX 3.2 and BSD 4.4 sockets.
 * 	[1992/05/29  18:26:53  jim]
 * 
 * Revision 1.1.2.2  1992/05/01  16:05:03  rsalz
 * 	 10-mar-1992 pf      Initial revision
 * 	[1992/05/01  00:18:17  rsalz]
 * 	Revision 1.1.3.3  1992/06/05  22:33:54  jim
 * 	Changes to insure compatibility with AIX 3.1.5.
 * 
 * Revision 1.1.2.2  1992/05/01  16:05:03  rsalz
 * 	 10-mar-1992 pf      Initial revision
 * 	[1992/05/01  00:18:17  rsalz]
 * 
 * $EndLog$
 */
#ifndef COMSOC_SYS_H
#define COMSOC_SYS_H
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      comsoc_osf.h (src/rpc/runtime/<OSF/1-LIKE-TARGET>/comsoc_sys.h)
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  The platform-specific portion of the internal network "socket" object
**  interface.  See abstract in "comsoc.h" for details.
**
**  For OSF/1 systems.  
**
**
*/

#if defined(__osf__) || defined(__OSF__) 
# include <sys/time.h>
#endif

/*
 * A handle to a socket.  The implementation of this type is considered
 * to be private to this package.
 */

typedef int rpc_socket_t;                       /* a UNIX socket handle */

/*
 * A public function for comparing two socket handles.
 */

#define RPC_SOCKET_IS_EQUAL(s1, s2)             (s1 == s2)


/*
 * This package's error type and values.  The implementation of this
 * type is considered to be private to this package.
 */

typedef int rpc_socket_error_t;                 /* a UNIX errno */

/*
 * The maximum number of iov elements which can be sent through
 * sendmsg is MSG_IOVLEN-1.
 */
#define RPC_C_MAX_IOVEC_LEN ( MSG_MAXIOVLEN - 1)

/*
 * The maximum number of connections which can be queued on a socket.
 */
#define RPC_C_LISTEN_BACKLOG SOMAXCONN

/*
 * Public error constants and functions for comparing errors.
 * The _ETOI_ (error-to-int) function converts a socket error to a simple
 * integer value that can be used in error mesages.
 */

#define RPC_C_SOCKET_OK           0             /* a successful error value */
#define RPC_C_SOCKET_EWOULDBLOCK  EWOULDBLOCK   /* operation would block */
#define RPC_C_SOCKET_EINTR        EINTR         /* operation was interrupted */
#define RPC_C_SOCKET_EIO          EIO           /* I/O error */
#define RPC_C_SOCKET_EADDRINUSE   EADDRINUSE    /* address was in use (see bind) */
#define RPC_C_SOCKET_ECONNRESET   ECONNRESET    /* connection reset by peer */
#define RPC_C_SOCKET_ETIMEDOUT    ETIMEDOUT     /* connection request timed out*/
#define RPC_C_SOCKET_ECONNREFUSED ECONNREFUSED  /* connection request refused */
#define RPC_C_SOCKET_ENOTSOCK     ENOTSOCK      /* descriptor was not a socket */
#define RPC_C_SOCKET_ENETUNREACH  ENETUNREACH   /* network is unreachable*/
#define RPC_C_SOCKET_ENOSPC       ENOSPC        /* no local or remote resources */
#define RPC_C_SOCKET_ENETDOWN     ENETDOWN      /* network is down */
#define RPC_C_SOCKET_ETOOMANYREFS ETOOMANYREFS  /* too many remote connections */
#define RPC_C_SOCKET_ESRCH        ESRCH         /* remote endpoint not found */
#define RPC_C_SOCKET_EHOSTDOWN    EHOSTDOWN     /* remote host is down */
#define RPC_C_SOCKET_EHOSTUNREACH EHOSTUNREACH  /* remote host is unreachable */
#define RPC_C_SOCKET_ECONNABORTED ECONNABORTED  /* local host aborted connect */
#define RPC_C_SOCKET_ECONNRESET   ECONNRESET    /* remote host reset connection */
#define RPC_C_SOCKET_ENETRESET    ENETRESET     /* remote host crashed */
#define RPC_C_SOCKET_ENOEXEC      ENOEXEC       /* invalid endpoint format for remote */
#define RPC_C_SOCKET_EACCESS      EACCES        /* access control information */
                                                /* invalid AT remote node */
#define RPC_C_SOCKET_EPIPE        EPIPE         /* a write on a pipe */
                                                /* or SOCKET for which there */
                                                /* is NO process to */
                                                /* read THE data. */
#define RPC_C_SOCKET_EAGAIN       EAGAIN        /* no more processes */
#define RPC_C_SOCKET_EALREADY     EALREADY      /* operation already */
                                                /* in PROGRESS */
#define RPC_C_SOCKET_EDEADLK      EDEADLK       /* resource deadlock */
                                                /* would occur */
#define RPC_C_SOCKET_EINPROGRESS  EINPROGRESS   /* operation now in */
                                                /* progress */
#define RPC_C_SOCKET_EISCONN      EISCONN       /* socket is already */
                                                /* connected */

/*
 * A macro to determine if an socket error can be recovered from by
 * retrying.
 */
#define RPC_SOCKET_ERR_IS_BLOCKING(s) \
    ((s == RPC_C_SOCKET_EAGAIN) || (s == RPC_C_SOCKET_EWOULDBLOCK) || (s == RPC_C_SOCKET_EINPROGRESS) || \
     (s == RPC_C_SOCKET_EALREADY) || (s == RPC_C_SOCKET_EDEADLK))

#define RPC_SOCKET_ERR_EQ(serr, e)  ((serr) == e)
#define RPC_SOCKET_IS_ERR(serr)     (! RPC_SOCKET_ERR_EQ(serr, RPC_C_SOCKET_OK))
#define RPC_SOCKET_ETOI(serr)       (serr)

/*
 * Macro to paper over the difference between 4.4 and 4.3 sockaddrs.
 */
#if _BSD == 44
#define RPC_SOCKET_FIX_ADDRLEN(addrp) \
     if(addrp != NULL) \
	do { ((struct sockaddr *)(&((addrp)->sa)))->sa_len = 0; } while(0)
#define MSG_ACCRIGHTS msg_control
#define MSG_ACCRIGHTSLEN msg_controllen
#else
#define RPC_SOCKET_FIX_ADDRLEN(addrp)
#endif

/*
 * Macros for performance critical operations.
 */

#ifdef AIX32
#define RPC_SOCKET_SENDMSG(sock, iovp, iovlen, addrp, ccp, serrp) \
    { \
        struct msghdr msg; \
        RPC_SOCKET_FIX_ADDRLEN(addrp); \
sendmsg_again:\
        RPC_LOG_SOCKET_SENDMSG_NTR; \
        if ((addrp) != NULL)\
        {\
            msg.msg_name = (caddr_t) &(addrp)->sa; \
            msg.msg_namelen = (addrp)->len; \
        }\
        else\
        {\
            msg.msg_name = (caddr_t) NULL;\
        }\
        msg.msg_iov = (struct iovec *) iovp; \
        msg.msg_iovlen = iovlen; \
        msg.MSG_ACCRIGHTS = NULL; \
        msg.MSG_ACCRIGHTSLEN = 0; \
	msg.msg_flags = 0; \
        *(ccp) = sendmsg ((int) sock, (struct msghdr *) &msg, 0); \
        *(serrp) = (*(ccp) == -1) ? errno : RPC_C_SOCKET_OK; \
        RPC_LOG_SOCKET_SENDMSG_XIT; \
        if (*(serrp) == EINTR)\
        {\
            goto sendmsg_again;\
        }\
    }

#else /* not AIX32 */

#define RPC_SOCKET_SENDMSG(sock, iovp, iovlen, addrp, ccp, serrp) \
    { \
        struct msghdr msg; \
        RPC_SOCKET_FIX_ADDRLEN(addrp); \
sendmsg_again:\
        RPC_LOG_SOCKET_SENDMSG_NTR; \
        if ((addrp) != NULL)\
        {\
            msg.msg_name = (caddr_t) &(addrp)->sa; \
            msg.msg_namelen = (addrp)->len; \
        }\
        else\
        {\
            msg.msg_name = (caddr_t) NULL;\
        }\
        msg.msg_iov = (struct iovec *) iovp; \
        msg.msg_iovlen = iovlen; \
        msg.msg_accrights = NULL; \
        msg.msg_accrightslen = 0; \
        *(ccp) = sendmsg ((int) sock, (struct msghdr *) &msg, 0); \
        *(serrp) = (*(ccp) == -1) ? errno : RPC_C_SOCKET_OK; \
        RPC_LOG_SOCKET_SENDMSG_XIT; \
        if (*(serrp) == EINTR)\
        {\
            goto sendmsg_again;\
        }\
    }

#endif /* AIX32 */


#define RPC_SOCKET_RECVFROM(sock, buf, buflen, from, ccp, serrp) \
    { \
recvfrom_again:\
        RPC_SOCKET_FIX_ADDRLEN(from); \
        RPC_LOG_SOCKET_RECVFROM_NTR; \
        *(ccp) = recvfrom ((int) sock, (char *) buf, (int) buflen, (int) 0, \
            (struct sockaddr *) (&(from)->sa), (int *) (&(from)->len)); \
        *(serrp) = (*(ccp) == -1) ? errno : RPC_C_SOCKET_OK; \
        RPC_LOG_SOCKET_RECVFROM_XIT; \
        RPC_SOCKET_FIX_ADDRLEN(from); \
        if (*(serrp) == EINTR)\
        {\
            goto recvfrom_again;\
        }\
    }

#define RPC_SOCKET_RECVMSG(sock, iovp, iovlen, addrp, ccp, serrp) \
    { \
        struct msghdr msg; \
         \
recvmsg_again:\
        RPC_LOG_SOCKET_RECVMSG_NTR; \
        if ((addrp) != NULL)\
        {\
            RPC_SOCKET_FIX_ADDRLEN(addrp); \
            msg.msg_name = (caddr_t) &(addrp)->sa; \
            msg.msg_namelen = (addrp)->len; \
        }\
        else\
        {\
            msg.msg_name = (caddr_t) NULL;\
        }\
        msg.msg_iov = (struct iovec *) iovp; \
        msg.msg_iovlen = iovlen; \
        msg.MSG_ACCRIGHTS = NULL; \
        *(ccp) = recvmsg ((int) sock, (struct msghdr *) &msg, 0); \
        if ((addrp) != NULL)\
        {\
            (addrp)->len = msg.msg_namelen; \
        }\
        *(serrp) = (*(ccp) == -1) ? errno : RPC_C_SOCKET_OK; \
        RPC_LOG_SOCKET_RECVMSG_XIT; \
        if (*(serrp) == EINTR)\
        {\
            goto recvmsg_again;\
        }\
    }

#endif
