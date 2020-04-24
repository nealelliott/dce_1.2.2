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
 * Revision 1.1.35.2  1996/02/18  23:46:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:14  marty]
 *
 * Revision 1.1.35.1  1995/12/08  00:14:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:05  root]
 * 
 * Revision 1.1.33.2  1994/05/27  15:35:46  tatsu_s
 * 	DG multi-buffer fragments.
 * 	Added RPC_C_MAX_IOVEC_LEN and RPC_SOCKET_RECVMSG().
 * 	[1994/05/03  17:28:51  tatsu_s]
 * 
 * Revision 1.1.33.1  1994/01/21  22:31:55  cbrooks
 * 	Code Cleanup
 * 	[1994/01/21  22:27:45  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  22:35:49  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:51:39  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:38:08  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:52:22  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:15:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef COMSOC_OSC_H
#define COMSOC_OSC_H
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**
**  %a%copyright(,**  )
**
**  NAME:
**
**      comsoc_osc.h
**
**  FACILITY:
**
**      OSF1 OSC Kernel Port Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Alternate socket abstraction definitions (still uses com/comsoc.h too).
**
**  %a%private_begin
**
**  MODIFICATION HISTORY: 
**
**  23-jul-91 sandhya       define rpc_socket_t as an int
**  08-jul-91 labossiere    add rpc_c_socket_eio
**  13-jun-91 labossiere    fix include syntax
**  08-mar-91 labossiere    select related modifications
**  02-may-90 labossiere    Initial version
**
**  %a%private_end  
**
*/

/*
 * A handle to a socket.  The implementation of this type is considered
 * to be private to this package.
 */

typedef int rpc_socket_t;       /* a kernel socket handle */

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
#define RPC_C_MAX_IOVEC_LEN (MSG_MAXIOVLEN - 1)

/*
 * Public error constants and functions for comparing errors.
 * The _ETOI_ (error-to-int) function converts a socket error to a simple
 * integer value that can be used in error mesages.
 */

#define RPC_C_SOCKET_OK           0             /* a successful error value */
#define RPC_C_SOCKET_EWOULDBLOCK  EWOULDBLOCK   /* operation would block */
#define RPC_C_SOCKET_EINTR        EINTR         /* operation was interrupted */
#define RPC_C_SOCKET_EADDRINUSE   EADDRINUSE    /* address was in use (see bind) */
#define RPC_C_SOCKET_ETIMEDOUT    ETIMEDOUT     /* connection request timed out */
#define RPC_C_SOCKET_EIO          EIO           /* "catch-all"... */

#define RPC_SOCKET_IS_ERR(serr)                 (serr != RPC_C_SOCKET_OK)
#define RPC_SOCKET_ERR_EQ(serr, e)              (serr == e)
#define RPC_SOCKET_ETOI(serr)                   (serr)


/*
 * Macros for performance critical operations.
 */

#define RPC_SOCKET_SENDMSG(sock, iovp, iovlen, addrp, ccp, serrp) \
    { \
        *(serrp) = rpc__socket_sendmsg(sock, iovp, iovlen, addrp, ccp); \
    }

#define RPC_SOCKET_RECVFROM(sock, buf, buflen, from, ccp, serrp) \
    { \
        *(serrp) = rpc__socket_recvfrom(sock, buf, buflen, (rpc_addr_p_t)from, ccp); \
    }

#define RPC_SOCKET_RECVMSG(sock, iovp, iovlen, addrp, ccp, serrp) \
    { \
        *(serrp) = rpc__socket_recvmsg(sock, iovp, iovlen, addrp, ccp); \
    }

/*
 * Some special stuff the this version of select.
 */

#include <sys/poll.h>               /* for POLL* defines */
#include <sys/uio.h>

typedef struct {
    rpc_socket_t    so;
    short           events;         /* POLL{NORM,OUT,PRI} */
} rpc_socket_sel_t, *rpc_socket_sel_p_t;


/*
 * R P C _ _ S O C K E T _ S E L E C T
 *
 * In-kernel useable version of select for sockets.
 */
PRIVATE rpc_socket_error_t rpc__socket_select
    _DCE_PROTOTYPE_ ((
        int nsoc,
        rpc_socket_sel_t *soc,
        int *nfsoc,
        rpc_socket_sel_t *fsoc,
        struct timeval *tmo
    ));


#endif  /* COMSOC_OSC_H */
