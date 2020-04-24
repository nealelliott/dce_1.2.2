/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: comsoc_sys.c,v $
 * Revision 1.1.8.2  1996/02/17  23:59:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:19  marty]
 *
 * Revision 1.1.8.1  1995/12/08  00:13:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:31  root]
 * 
 * Revision 1.1.6.2  1994/05/27  15:35:32  tatsu_s
 * 	Fixed the corrupted iov in rpc__socket_recvmsg().
 * 	[1994/05/05  21:17:44  tatsu_s]
 * 
 * 	DG multi-buffer fragments.
 * 	Added rpc__socket_recvmsg().
 * 	[1994/05/03  18:33:54  tatsu_s]
 * 
 * Revision 1.1.6.1  1994/01/21  22:31:04  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:38:20  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  22:34:53  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:50:11  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  19:36:11  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:47:12  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  17:54:25  rsalz
 * 	"Changed as part of rpc6 drop."
 * 	[1992/05/01  17:49:37  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:15:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**
**  NAME:
**
**      comsoc_osc.c
**
**  FACILITY:
**
**      Example Kernel Port Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Implementations of various socket abstraction interfaces used by the
**  kernel runtime version of the macros in com/comsoc.h
**
**  Basically, these are just kernel callable versions of the standard
**  user invoked kernel interfaces.  This is based on the BSD version of
**  socket code.  To make this into a working version, the select
**  code in rpc__socket_select needs to be added; typically this is done
**  by starting with code extracted from select() in the operating system
**  being ported to.
**  Instead of passing file descriptors, a kernel socket handle is used.
**  Additionally, errors are returned through an error argument.
**
*/

/*
 * Copyright (c) 1982, 1986, 1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	Base: src/rpc/kruntime/BSD44_TEMPLATE/comsoc_sys.c, , dce_osf, dce.75d (Berkeley) 8/22/91
 */

#include <commonp.h>
#include <com.h>
#include <comprot.h>
#include <comnaf.h>
#include <comp.h>

#include <sys/param.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/file.h>

#include <sys/mbuf.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/domain.h>
#include <sys/protosw.h>

/*
 * Includes and declarations for in-kernel select implementation.
 */
#include <sys/time.h>               /* for timeval */
#include <sys/systm.h>              /* for selwait */
extern struct timeval time;

/* ======================================================================== */

/*
 * What we think a socket's buffering is in case rpc__socket_set_bufs()
 * fails miserably.  This is system dependent if you want it to be correct.
 */

#define RPC_C_SOCK_GUESSED_RCVBUF    (4 * 1024)
#define RPC_C_SOCK_GUESSED_SNDBUF    (4 * 1024)

/* ======================================================================== */

INTERNAL rpc_socket_error_t getsockopt_nck
    _DCE_PROTOTYPE_((
        rpc_socket_t sock,
        int level, 
        int name,
        char *val,
        int *valsize
    ));

INTERNAL rpc_socket_error_t setsockopt_nck
    _DCE_PROTOTYPE_((
        rpc_socket_t sock,
        int level, 
        int name,
        char *val,
        int valsize
    ));

INTERNAL rpc_socket_error_t sockargs_nck
    _DCE_PROTOTYPE_((
        struct mbuf **aname,
        struct sockaddr *name,
        int namelen,
        int type
    ));

/* ========================================================================= */

INTERNAL rpc_socket_error_t getsockopt_nck(sock, level, name, val, avalsize)
rpc_socket_t sock;
int level;
int name;
char *val;
int *avalsize;
{
    struct mbuf *m = NULL;
    rpc_socket_error_t error;
    int valsize;

    if (val)
        valsize = *avalsize;
    else
        valsize = 0;
    error = sogetopt(sock, level, name, &m);
    if (error)
        goto bad;
    if (val && valsize && m != NULL) {
        if (valsize > m->m_len)
            valsize = m->m_len;
        /*b_copy(mtod(m, caddr_t), val, (u_int)valsize);*/
        memmove( val, mtod(m, caddr_t), (u_int)valsize);*/
        *avalsize = valsize;
    }
bad:
    if (m != NULL)
        (void) m_free(m);
    return (error);
}

INTERNAL rpc_socket_error_t setsockopt_nck(sock, level, name, val, valsize)
rpc_socket_t sock;
int level, name;
char *val;
int valsize;
{
    struct mbuf *m = NULL;
    rpc_socket_error_t error;

    if (valsize > MLEN)
        return(EINVAL);
    if (val) {
        m = m_get(M_WAIT, MT_SOOPTS);
        if (m == NULL)
            return(ENOBUFS);
        /*b_copy(val, mtod(m, caddr_t), (u_int)valsize);*/
        memmove(mtod(m, caddr_t), val, (u_int)valsize);
        m->m_len = valsize;
    }
    error = sosetopt(sock, level, name, m);
    return (error);
}

INTERNAL rpc_socket_error_t sockargs_nck(aname, name, namelen, type)
struct mbuf **aname;
struct sockaddr *name;
int namelen, type;
{
    struct mbuf *m;

    if (namelen > MLEN)
        return (EINVAL);
    m = m_get(M_WAIT, type);
    if (m == NULL)
        return (ENOBUFS);
    m->m_len = namelen;
    /*b_copy(name, mtod(m, caddr_t), (u_int)namelen);*/
    memmove( mtod(m, caddr_t), name, (u_int)namelen) ;
    *aname = m;
    if (type == MT_SONAME) {
        register struct sockaddr *sa = mtod(m, struct sockaddr *);
#if defined(COMPAT_43) && BYTE_ORDER != BIG_ENDIAN
        if (sa->sa_family == 0 && sa->sa_len < AF_MAX)
            sa->sa_family = sa->sa_len;
#endif
        sa->sa_len = namelen;
    }
    return (rpc_c_socket_ok);
}

/* ======================================================================== */

/*
 * R P C _ _ S O C K E T _ O P E N
 *
 * Create a new socket for the specified Protocol Sequence.
 * The new socket has blocking IO semantics.
 *
 * (see BSD UNIX socket(2)).
 */

PRIVATE rpc_socket_error_t rpc__socket_open(protseq, sock)
rpc_protseq_id_t protseq;
rpc_socket_t *sock;
{
    rpc_socket_error_t error;

    error = socreate(
                (int) RPC_PROTSEQ_INQ_NAF_ID(protseq),
                sock,
                (int) RPC_PROTSEQ_INQ_NET_IF_ID(protseq),
                (int) RPC_PROTSEQ_INQ_NET_PROT_ID(protseq)
            );
    if (error)
        RPC_DBG_GPRINTF(("(rpc__socket_open) error = %d\n", error));
    return (error);
}


/*
 * R P C _ _ S O C K E T _ O P E N _ B A S I C
 *
 * A special version of socket_open that is used *only* by 
 * the low level initialization code when it is trying to 
 * determine what network services are supported by the host OS.
 */

PRIVATE rpc_socket_error_t rpc__socket_open_basic(naf, net_if, net_prot, sock)
rpc_naf_id_t naf;
rpc_network_if_id_t net_if;
rpc_network_protocol_id_t net_prot;
rpc_socket_t *sock;
{
    rpc_socket_error_t error;

    error = socreate((int) naf, sock, (int) net_if, (int) net_prot);
    return (error);
}


/*
 * R P C _ _ S O C K E T _ C L O S E
 *
 * Close (destroy) a socket.
 *
 * (see BSD UNIX close(2)).
 */

PRIVATE rpc_socket_error_t rpc__socket_close(sock)
rpc_socket_t sock;
{
    rpc_socket_error_t error;

    error = soclose(sock);
    if (error)
        RPC_DBG_GPRINTF(("(rpc__socket_open) error = %d\n", error));
    return (error);
}


/*
 * R P C _ _ S O C K E T _ B I N D
 *
 * Bind a socket to a specified local address.
 *
 * (see BSD UNIX bind(2)).
 */

PRIVATE rpc_socket_error_t rpc__socket_bind(sock, addr)
rpc_socket_t sock;
rpc_addr_p_t addr;
{
    struct mbuf *nam;
    rpc_socket_error_t error;

    error = sockargs_nck(&nam, (struct sockaddr *)&addr->sa, addr->len, MT_SONAME);
    if (error)
        goto bad;

    error = sobind(sock, nam);
    m_freem(nam);
bad:
    if (error)
        RPC_DBG_GPRINTF(("(rpc__socket_bind) error = %d\n", error));
    return (error);
}


/*
 * R P C _ _ S O C K E T _ S E N D M S G
 *
 * Send a message over a given socket.  An error code as well as the
 * actual number of bytes sent are returned.  Notice that this routine
 * just uses sosend() (which *always* copies the data to a mbuf) so you
 * can safely send stack based (automatic variables) data buffers.
 *
 * THIS VERSION ISN'T REQUIRED TO WORK AT INTERRUPT LEVEL.
 *
 * (see BSD UNIX sendmsg(2)).
 */

PRIVATE rpc_socket_error_t rpc__socket_sendmsg(sock, iov, iov_len, addr, cc)
rpc_socket_t sock;
rpc_socket_iovec_p_t iov;   /* array of bufs of data to send */
int iov_len;                /* number of bufs */
rpc_addr_p_t addr;          /* addr of receiver */
int *cc;                    /* returned number of bytes actually sent */
{
    rpc_socket_error_t error;
    struct mbuf *to = NULL;
    struct uio auio;
    struct iovec aiov[MSG_MAXIOVLEN];	/* XXX use malloc */
    long len;
    int i;

    if ((u_int)iov_len >= MSG_MAXIOVLEN) {
        error = EMSGSIZE;
        goto bad;
    }


    /* don't corrupt the passed iov */
    /*b_copy((caddr_t)iov, (caddr_t)aiov, iov_len * sizeof (struct iovec));*/
    memmove( (caddr_t)aiov, (caddr_t)iov, iov_len * sizeof (struct iovec));

    auio.uio_iov = aiov;
    auio.uio_iovcnt = iov_len;
    auio.uio_segflg = UIO_SYSSPACE;
    auio.uio_rw = UIO_WRITE;
    auio.uio_offset = 0;        /* XXX */
    auio.uio_resid = 0;
    for (i = 0; i < iov_len; i++, iov++) {
        if (iov->len < 0)
            return(EINVAL);
        auio.uio_resid += iov->len;
    }

    error = sockargs_nck(&to, (struct sockaddr *)&addr->sa, addr->len, MT_SONAME);
    if (error)
        goto bad;
    len = auio.uio_resid;

    /*
     * Typically, qsave is setup at this point to deal with EINTR vs RESTARTSYS.
     * For now I'm assuming that this isn't an issue since all our sockets
     * *should* be set to non-blocking mode, hence we shouldn't sleep.
     */
    error = sosend(sock, to, &auio, 0 /* flags */, 0 /* rights */, 0 /* control */);
    *cc = len - auio.uio_resid;
bad:
    if (to)
        m_freem(to);
    if (error)
        RPC_DBG_GPRINTF(("(rpc__socket_sendmsg) error = %d\n", error));
    return (error);
}


/*
 * R P C _ _ S O C K E T _ R E C V F R O M
 *
 * Recieve the next buffer worth of information from a socket.  A
 * rpc_addr_t appropriate for the NAF corresponding to this socket must
 * be provided.  addr.len must set to the actual size of addr.sa.  This
 * operation fills in addr.sa and sets addr.len to the new size of the
 * field.  An error status as well as the actual number of bytes received
 * are also returned.
 * 
 * THIS VERSION ISN'T REQUIRED TO WORK AT INTERRUPT LEVEL.
 *
 * (see BSD UNIX recvfrom(2)).
 */

PRIVATE rpc_socket_error_t rpc__socket_recvfrom(sock, buf, len, addr, cc)
rpc_socket_t sock;
byte_p_t buf;           /* buf for rcvd data */
int len;                /* len of above buf */
rpc_addr_p_t addr;      /* addr of sender; really in/out */
int *cc;                /* returned number of bytes actually rcvd */
{
    rpc_socket_error_t error;
    struct mbuf *from = NULL;
    rpc_socket_iovec_t iov;
    struct uio auio;

    iov.base = buf;
    iov.len = len;
    auio.uio_iov = (struct iovec *)&iov;
    auio.uio_iovcnt = 1;
    auio.uio_segflg = UIO_SYSSPACE;
    auio.uio_rw = UIO_READ;
    auio.uio_offset = 0;        /* XXX */
    auio.uio_resid = len;

    /*
     * Typically, qsave is setup at this point to deal with EINTR vs RESTARTSYS.
     * For now I'm assuming that this isn't an issue since all our sockets
     * *should* be set to non-blocking mode, hence we shouldn't sleep.
     */
    error = soreceive(sock, &from, &auio, 
            NULL /* flagsp */, NULL /* rightsp */, NULL /* controlp */);
    if (error)
        goto bad;
    *cc = len - auio.uio_resid;

#ifdef COMPAT_43
        mtod(from, struct osockaddr *)->sa_family =
            mtod(from, struct sockaddr *)->sa_family;
#endif
    if (addr != NULL) {
        if (addr->len > from->m_len)      /* ??? */
            addr->len = from->m_len;
        /*b_copy(mtod(from, caddr_t), (caddr_t)&addr->sa, (unsigned)addr->len);*/
        memmove((caddr_t)&addr->sa, mtod(from, caddr_t), (unsigned)addr->len);*/
    }

bad:
    if (from)
        m_freem(from);
    if (error && (error != EWOULDBLOCK))
        RPC_DBG_GPRINTF(("(rpc__socket_recvfrom) error = %d\n", error));
    return (error);
}

/*
 * R P C _ _ S O C K E T _ R E C V M S G
 *
 * Recieve the next buffers worth of information from a socket.  A
 * rpc_addr_t appropriate for the NAF corresponding to this socket must
 * be provided.  addr.len must set to the actual size of addr.sa.  This
 * operation fills in addr.sa and sets addr.len to the new size of the
 * field.  An error status as well as the actual number of bytes received
 * are also returned.
 * 
 * THIS VERSION ISN'T REQUIRED TO WORK AT INTERRUPT LEVEL.
 *
 * (see BSD UNIX recvfrom(2)).
 */

PRIVATE rpc_socket_error_t rpc__socket_recvmsg(sock, iov, iov_len, addr, cc)
rpc_socket_t sock;
rpc_socket_iovec_p_t iov;   /* array of bufs for rcvd data */
int iov_len;                /* number of bufs */
rpc_addr_p_t addr;          /* addr of sender; really in/out */
int *cc;                    /* returned number of bytes actually rcvd */
{
    rpc_socket_error_t error;
    struct mbuf *from = NULL;
    struct uio auio;
    struct iovec aiov[MSG_MAXIOVLEN];	/* XXX use malloc */
    long len;
    int i;

    if ((u_int)iov_len >= MSG_MAXIOVLEN) {
        error = EMSGSIZE;
        goto bad;
    }

    /* don't corrupt the passed iov */
    /*b_copy((caddr_t)iov, (caddr_t)aiov, iov_len * sizeof (struct iovec));*/
    memmove( (caddr_t)aiov, (caddr_t)iov, iov_len * sizeof (struct iovec));

    auio.uio_iov = aiov;
    auio.uio_iovcnt = iov_len;
    auio.uio_segflg = UIO_SYSSPACE;
    auio.uio_rw = UIO_READ;
    auio.uio_offset = 0;        /* XXX */
    auio.uio_resid = 0;
    for (i = 0; i < iov_len; i++, iov++) {
        if (iov->len < 0)
            return(EINVAL);
        auio.uio_resid += iov->len;
    }

    len = auio.uio_resid;

    /*
     * Typically, qsave is setup at this point to deal with EINTR vs RESTARTSYS.
     * For now I'm assuming that this isn't an issue since all our sockets
     * *should* be set to non-blocking mode, hence we shouldn't sleep.
     */
    error = soreceive(sock, &from, &auio, 
            NULL /* flagsp */, NULL /* rightsp */, NULL /* controlp */);
    if (error)
        goto bad;
    *cc = len - auio.uio_resid;

#ifdef COMPAT_43
        mtod(from, struct osockaddr *)->sa_family =
            mtod(from, struct sockaddr *)->sa_family;
#endif
    if (addr != NULL) {
        if (addr->len > from->m_len)      /* ??? */
            addr->len = from->m_len;
        /*b_copy(mtod(from, caddr_t), (caddr_t)&addr->sa, (unsigned)addr->len);*/
        memmove((caddr_t)&addr->sa, mtod(from, caddr_t), (unsigned)addr->len);*/
    }

bad:
    if (from)
        m_freem(from);
    if (error && (error != EWOULDBLOCK))
        RPC_DBG_GPRINTF(("(rpc__socket_recvfrom) error = %d\n", error));
    return (error);
}

/*
 * R P C _ _ S O C K E T _ I N Q _ A D D R
 *
 * Return the local address associated with a socket.  A rpc_addr_t
 * appropriate for the NAF corresponding to this socket must be provided.
 * addr.len must set to the actual size of addr.sa.  This operation fills
 * in addr.sa and sets addr.len to the new size of the field.
 *
 * !!! NOTE: You should use rpc__naf_desc_inq_addr() !!!
 *
 * This routine is indended for use only by the internal routine:
 * rpc__naf_desc_inq_addr().  rpc__socket_inq_addr() only has the
 * functionality of BSD UNIX getsockname() which doesn't (at least not
 * on all systems) return the local network portion of a socket's address.
 * rpc__naf_desc_inq_addr() returns the complete address for a socket.
 *
 * (see BSD UNIX getsockname(2)).
 */

PRIVATE rpc_socket_error_t rpc__socket_inq_endpoint(sock, addr)
rpc_socket_t sock;
rpc_addr_p_t addr;  /* really in/out */
{
    struct mbuf *m = NULL;
    rpc_socket_error_t error;

    m = m_getclr(M_WAIT, MT_SONAME);
    if (m == NULL) {
        error = ENOBUFS;
        goto bad1;
    }
    error = (*sock->so_proto->pr_usrreq)(sock, PRU_SOCKADDR,
                        (struct mbuf *)0, m, (struct mbuf *)0);
    if (error) {
        goto bad;
    }
    if (addr->len > m->m_len)
        addr->len = m->m_len;
#ifdef COMPAT_43
        mtod(m, struct osockaddr *)->sa_family =
            mtod(m, struct sockaddr *)->sa_family;
#endif
    /*b_copy(mtod(m, caddr_t), (caddr_t)&addr->sa, (u_int)addr->len);*/
    memmove( (caddr_t)&addr->sa, mtod(m, caddr_t), (u_int)addr->len);
bad:
    m_freem(m);
bad1:
    if (error)
        RPC_DBG_GPRINTF(("(rpc__socket_inq_addr) error=%d\n", error));
    return (error);
}


/*
 * R P C _ _ S O C K E T _ S E T _ B R O A D C A S T
 *
 * Enable broadcasting for the socket (as best it can).
 * Used only by Datagram based Protocol Services.
 */

PRIVATE rpc_socket_error_t rpc__socket_set_broadcast(sock)
rpc_socket_t sock;
{
    rpc_socket_error_t error = rpc_c_socket_ok;
#ifdef SO_BROADCAST
    int setsock_val = 1;

    error = setsockopt_nck(sock, SOL_SOCKET, SO_BROADCAST, 
            (char *)&setsock_val, sizeof(setsock_val));
    if (error) 
        RPC_DBG_GPRINTF(("(rpc__socket_set_broadcast) error = %d\n", error));
#endif
    return (error);
}


/*
 * R P C _ _ S O C K E T _ S E T _ B U F S
 *
 * Set the socket's send and receive buffer sizes and return the new
 * values.  Typically, it does the best job that it can upon failures.
 *
 * If for some reason your system is screwed up and defines SOL_SOCKET
 * and SO_SNDBUF, but doesn't actually support the SO_SNDBUF and SO_RCVBUF
 * operations AND using them would result in nasty behaviour (i.e. they
 * don't just return some error code), define NO_SO_SNDBUF.
 */

PRIVATE rpc_socket_error_t rpc__socket_set_bufs(sock, txsize, rxsize, ntxsize, nrxsize)
rpc_socket_t sock;
unsigned32 txsize;
unsigned32 rxsize;
unsigned32 *ntxsize;
unsigned32 *nrxsize;
{
    int sizelen;
    rpc_socket_error_t error;

#if (defined (SOL_SOCKET) && defined(SO_SNDBUF)) && !defined(NO_SO_SNDBUF)
    /*
     * Set the new sizes.
     */

    error = setsockopt_nck(sock, SOL_SOCKET, SO_SNDBUF, (char *)&txsize, sizeof(txsize));
    if (error) {
        RPC_DBG_GPRINTF(("(rpc__socket_set_bufs) WARNING: set sndbuf (%d) failed - error = %d\n", 
            txsize, error));
    }

    error = setsockopt_nck(sock, SOL_SOCKET, SO_RCVBUF, (char *)&rxsize, sizeof(rxsize));
    if (error) {
        RPC_DBG_GPRINTF(("(rpc__socket_set_bufs) WARNING: set rcvbuf (%d) failed - error = %d\n", 
            rxsize, error));
    }

    /*
     * Get the new sizes.  If this fails, just return some guessed sizes.
     */

    sizelen = sizeof *ntxsize;
    error = getsockopt_nck(sock, SOL_SOCKET, SO_SNDBUF, (char *)ntxsize, &sizelen);
    if (error) {
        RPC_DBG_GPRINTF(("(rpc__socket_set_bufs) WARNING: get sndbuf failed - error = %d\n", 
            error));
        *ntxsize = rpc_c_sock_guessed_sndbuf;
    }

    sizelen = sizeof *nrxsize;
    error = getsockopt_nck(sock, SOL_SOCKET, SO_RCVBUF, (char *)nrxsize, &sizelen);
    if (error) {
        RPC_DBG_GPRINTF(("(rpc__socket_set_bufs) WARNING: get rcvbuf failed - error = %d\n", 
            error));
        *nrxsize = rpc_c_sock_guessed_rcvbuf;
    }
#else
    *ntxsize = rpc_c_sock_guessed_sndbuf;
    *nrxsize = rpc_c_sock_guessed_rcvbuf;
#endif
    return (rpc_c_socket_ok);
}


/*
 * R P C _ _ S O C K E T _ S E T _ N B I O
 *
 * Set a socket to non-blocking mode.
 * 
 * Return rpc_c_socket_ok on success, otherwise an error value.
 */

PRIVATE rpc_socket_error_t rpc__socket_set_nbio(sock)
rpc_socket_t sock;
{
    rpc_socket_error_t error = rpc_c_socket_ok;

    sock->so_state |= SS_NBIO;
    return (error);
}


/*
 * R P C _ _ S O C K E T _ S E T _ C L O S E _ O N _ E X E C
 *
 *
 * Set a socket to a mode whereby it is not inherited by a spawned process
 * executing some new image. This is possibly a no-op on some systems.
 *
 * Return rpc_c_socket_ok on success, otherwise an error value.
 *
 * This is a no-op because rpc_socket_t's aren't part of a process'
 * state (i.e. they are not in the file table)
 */

PRIVATE rpc_socket_error_t rpc__socket_set_close_on_exec(sock)
rpc_socket_t sock;
{
    return (rpc_c_socket_ok);
}

/*
 * R P C _ _ S O C K E T _ G E T P E E R N A M E
 *
 * Get name of connected peer.
 * This is used only by Connection oriented Protocol Services.
 *
 */

PRIVATE rpc_socket_error_t rpc__socket_getpeername (sock, addr)
rpc_socket_t sock;
rpc_addr_p_t addr;
{
    DIE("(rpc__socket_getpeername) Not Supported");
}

/*
 * R P C _ _ S O C K E T _ G E T _ I F _ I D
 *
 * Get socket network interface id (socket type).
 *
 */

PRIVATE rpc_socket_error_t rpc__socket_get_if_id (sock, network_if_id)
rpc_socket_t        sock;
rpc_network_if_id_t *network_if_id;
{
    int optlen;
    
    optlen = sizeof(rpc_network_if_id_t);
    
    return (getsockopt_nck (sock,
                        SOL_SOCKET,
                        SO_TYPE,
                        (char *)network_if_id,
                        &optlen) == -1  ? -1 : rpc_c_socket_ok);
}

/*
 * R P C _ _ S O C K E T _ S E L E C T
 * 
 *
 * Perform a 'select' operation on the list of sockets defined by the
 * soc array.  Return the active sockets in the "found" fsoc[] array
 * and the found count as a return value.
 *
 * This select stuff should look very similar to standard BSD processing.
 */

PRIVATE rpc_socket_error_t rpc__socket_select(nsoc, soc, nfsoc, fsoc, tmo)
int nsoc;
rpc_socket_sel_t *soc;
int *nfsoc;
rpc_socket_sel_t *fsoc;
struct timeval *tmo;
{ 
    int err = rpc_c_socket_ok;
    int timo;
    int s;
    struct timeval atv;
    label_t lqsave;
    int ncoll;
    extern int nselcoll;     /* used by selwakeup */

    if(tmo) 
    {
        /*b_copy(tmo, &atv, sizeof (struct timeval));*/
        memmove(&atv, tmo, sizeof (struct timeval));*/

        if(itimerfix(&atv))
            return (EINVAL);

        s = splhigh(); 
        timevaladd(&atv, &time); 
        splx(s);
        timo = hzto(&atv);
    } else
        timo = 0;


retry:
    ncoll = nselcoll;
    u.u_procp->p_flag |= SSEL;

    *nfsoc = rpc__socscan(nsoc, soc, fsoc);
    if (*nfsoc)
        goto done;

    s = splhigh();
    if (tmo && (time.tv_sec > atv.tv_sec ||
        time.tv_sec == atv.tv_sec && time.tv_usec >= atv.tv_usec)) 
    {
        splx(s);   
        /* return err == ok; nfds == 0 (i.e. select timed out) */
        goto done; 
    }

    if ((u.u_procp->p_flag & SSEL) == 0 || nselcoll != ncoll) {
        splx(s);
        goto retry;
    }
    u.u_procp->p_flag &= ~SSEL;

    err = tsleep((caddr_t)&selwait, PSOCK | PCATCH, "rpc__socket_select", timo);
    splx(s);

    if (err == 0)
        goto retry;

done:
    u.u_procp->p_flag &= ~SSEL;
    return(err);      
}


int
rpc__socscan(nsoc, soc, fsoc)
int nsoc;
rpc_socket_sel_t *soc;
rpc_socket_sel_t *fsoc;
{
    int i;
    short revents;
    int nfnd;

    /*
     * "Select" on the indicated sockets.
     */
    for(i = 0, nfnd = 0; i < nsoc; i++, soc++) 
    {
        if (rpc__soo_select(soc, fsoc))
        {
            nfnd++; 
            fsoc++;
        }
    }
    return(nfnd);
}

rpc__soo_select(ssel, fssel)
rpc_socket_sel_t *ssel;
rpc_socket_sel_t *fssel;
{
        rpc_socket_t    so = ssel->so;
        short           events = ssel->events;
        short           *revents = &fssel->events;
        register int s = splnet();

        fssel->so = so;
        *revents = 0;

        if (events & POLLNORM) {
                if (soreadable(so)) 
                    *revents |= POLLNORM;
                else
                    sbselqueue(&so->so_rcv);
        }

        if (events & POLLOUT) {
                if (sowriteable(so))
                        *revents |= POLLOUT;
                else
                        sbselqueue(&so->so_snd);
        }

        if (events & POLLPRI) {
                if (so->so_oobmark ||
                    (so->so_state & SS_RCVATMARK))
                        *revents |= POLLPRI;
                else {
                        sbselqueue(&so->so_rcv);
                }
        }

        splx(s);
        return (*revents);
}

/*
 * R P C _ _ S O C K E T _ N O W R I T E B L O C K _ W A I T
 *
 * Wait until the a write on the socket should succeed without
 * blocking.  If tmo is NULL, the wait is unbounded, otherwise
 * tmo specifies the max time to wait. rpc_c_socket_etimedout
 * if a timeout occurs.  This operation in not cancellable.
 */

PRIVATE rpc_socket_error_t rpc__socket_nowriteblock_wait(so, tmo)
rpc_socket_t so;
struct timeval *tmo;
{
    rpc_socket_sel_t    ssel;
    rpc_socket_sel_t    fssel;
    int                 nfnd;
    rpc_socket_error_t  error;

    ssel.so = so;
    ssel.events = POLLOUT;

    error = rpc__socket_select(1, &ssel, &nfnd, &fssel, tmo);
    if (error)
        return error;

    if (nfnd == 0)
        return rpc_c_socket_etimedout;

    return rpc_c_socket_ok;
}
