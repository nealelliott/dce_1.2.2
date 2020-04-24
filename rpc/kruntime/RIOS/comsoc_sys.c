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
 * Revision 1.1.56.2  1996/02/18  00:00:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:38  marty]
 *
 * Revision 1.1.56.1  1995/12/08  00:14:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:04  root]
 * 
 * Revision 1.1.54.3  1994/05/27  15:35:45  tatsu_s
 * 	Fixed the corrupted iov in rpc__socket_recvmsg().
 * 	[1994/05/05  17:59:21  tatsu_s]
 * 
 * 	DG multi-buffer fragments.
 * 	Added rpc__socket_recvmsg().
 * 	[1994/05/03  17:28:35  tatsu_s]
 * 
 * Revision 1.1.54.2  1994/02/02  21:48:58  cbrooks
 * 	OT9855 code cleanup breaks KRPC
 * 	[1994/02/02  21:00:11  cbrooks]
 * 
 * Revision 1.1.54.1  1994/01/22  22:49:41  cbrooks
 * 	RPC Code Cleanup - CR 9797
 * 	[1994/01/22  22:45:54  cbrooks]
 * 
 * Revision 1.1.52.1  1993/10/05  13:41:42  root
 * 	Port the port-restriction code from user-space rpc__socket_bind
 * 	to the kernel-space version.
 * 	[1993/09/30  19:02:30  root]
 * 
 * Revision 1.1.4.3  1993/01/03  22:35:45  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:51:35  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  19:38:03  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:52:18  zeliff]
 * 
 * Revision 1.1.2.3  1992/06/26  06:06:34  jim
 * 	Changes for AIX 3.2.
 * 	[1992/06/02  15:58:20  jim]
 * 
 * Revision 1.1.2.2  1992/05/14  21:20:05  kazar
 * 	Add splnet calls to reduce crashes on RS6K
 * 	[1992/05/14  20:54:39  kazar]
 * 
 * Revision 1.1  1992/01/19  03:15:26  devrcs
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
**  %a%copyright(,**  )
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
**  %a%private_begin
**
**  MODIFICATION HISTORY: 
**
**  05-apr-91 labossiere    come up to ker3 level
**  21-nov-90 nacey         - Add _rpc__socket_getpeername and _rpc__socket_get_if_id
**                          - rpc__socket_inq_addr() -> rpc__socket_inq_endpoint()
**  02-feb-90 labossiere    Initial version
**
**  %a%private_end  
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
 *	Base: src/rpc/kruntime/RIOS/comsoc_sys.c, , dce_osf, dce.52d (Berkeley) 6/18/91
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
#include <net/spl.h>

/*
 * Includes and declarations for in-kernel select implementation.
 */
#include <sys/time.h>               /* for timeval */
#ifndef _AIX
#include <sys/systm.h>              /* for selwait */
#endif
#if defined (_AIX) && defined (_KERNEL)
#include <sys/poll.h>
#endif

extern struct file *x_ofile[];
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
    int s;

    struct file *fp;
    struct socket *so;

    if (error = x_getf(sock, &fp))
    {
        RPC_DBG_GPRINTF(("(getsockopt_nck,x_getf) error = %d\n", error));
        return (error);
    }
    so = (struct socket *)(fp->f_data);

    if (val)
        valsize = *avalsize;
    else
        valsize = 0;
    s = splnet();
    error = sogetopt(so, level, name, &m);
    splx(s);
    if (error)
        goto bad;
    if (val && valsize && m != NULL) {
        if (valsize > m->m_len)
            valsize = m->m_len;
        bcopy(mtod(m, caddr_t), val, (u_int)valsize);
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
    struct file *fp;
    struct socket *so;
    int s;

    if (error = x_getf(sock, &fp))
    {
        RPC_DBG_GPRINTF(("(setsockopt_nck,x_getf) error = %d\n", error));
        return (error);
    }
    so = (struct socket *)(fp->f_data);


    if (valsize > MLEN)
        return(EINVAL);
    if (val) {
        m = m_get(M_WAIT, MT_SOOPTS);
        if (m == NULL)
            return(ENOBUFS);
        bcopy(val, mtod(m, caddr_t), (u_int)valsize);
        m->m_len = valsize;
    }
    s = splnet();
    error = sosetopt(so, level, name, m);
    splx(s);
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
    bcopy(name, mtod(m, caddr_t), (u_int)namelen);
    *aname = m;
    if (type == MT_SONAME) {
        struct sockaddr *sa = mtod(m, struct sockaddr *);
#if defined(COMPAT_43) && BYTE_ORDER != BIG_ENDIAN
        if (sa->sa_family == 0 && sa->sa_len < AF_MAX)
            sa->sa_family = sa->sa_len;
#endif
#ifdef AIX32
        sa->sa_len = namelen;
#endif
    }
    return (RPC_C_SOCKET_OK);
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
    rpc_socket_error_t error= RPC_C_SOCKET_OK;

    *sock = x_socket((int)RPC_PROTSEQ_INQ_NAF_ID(protseq),
                      (int) RPC_PROTSEQ_INQ_NET_IF_ID(protseq),
                      (int) RPC_PROTSEQ_INQ_NET_PROT_ID(protseq)
                      );

    if (*sock == -1)
    {
        error = 1;
        RPC_DBG_GPRINTF(("(rpc__socket_open) error = %d\n", error));
    }

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
    rpc_socket_error_t error = RPC_C_SOCKET_OK;

    *sock = x_socket((int)naf, (int)net_if, (int) net_prot);
    if (*sock == -1)
    {
       error = 1;
       RPC_DBG_GPRINTF(("(rpc__socket_open_basic) error = %d\n", error));
    }
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
    struct file *fp;
    struct socket *so;

    if (error = x_getf(sock, &fp))
    {
        RPC_DBG_GPRINTF(("(rpc__socket_close,x_getf) error = %d\n", error));
        return (error);
    }
    so = (struct socket *)(fp->f_data);


    error = soclose(so);	/* does its own splnet */
    xfdfree(sock);
    if (error)
        RPC_DBG_GPRINTF(("(rpc__socket_close) error = %d\n", error));
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
    struct file *fp;
    struct socket *so;
    unsigned32 status;
    rpc_addr_p_t temp_addr=NULL;

    if (error = x_getf(sock, &fp))
    {
        RPC_DBG_GPRINTF(("(rpc__socket_close,x_getf) error = %d\n", error));
        return (error);
    }
    so = (struct socket *)(fp->f_data);


    error = sockargs_nck(&nam, (struct sockaddr *)&addr->sa, addr->len, MT_SONAME);
    if (error)
        goto bad;

    if (! RPC_PROTSEQ_TEST_PORT_RESTRICTION (addr -> rpc_protseq_id))
    {
        error = sobind(so, nam);	/* does its own splnet */
    }
    else{
              /*
         * Port restriction is in place.  If the address has a well-known
         * endpoint, then do a simple bind.
         */

        unsigned_char_t *endpoint;
        unsigned char c;

        rpc__naf_addr_inq_endpoint (addr, &endpoint, &status);

        c = endpoint[0];               /* grab first char */
        rpc_string_free (&endpoint, &status);

        if (c != '\0')       /* test for null string */
        {
            error = sobind(so, nam);	/* does its own splnet */
        }                               /* well-known endpoint */

        else
        {
            /*
             * Port restriction is in place and the address doesn't have a
             * well-known endpoint.  Try to bind until we hit a good port,
             * or exhaust the retry count.
             *
             * Make a copy of the address to work in; if we hardwire an
             * endpoint into our caller's address, later logic could infer
             * that it is a well-known endpoint.
             */

            unsigned32 i;
            boolean found;

            for (i = 0, found = false;
                 (i < RPC_PORT_RESTRICTION_INQ_N_TRIES (addr->rpc_protseq_id))
                 && !found;
                 i++)
            {
                unsigned_char_p_t port_name;

                rpc__naf_addr_overcopy (addr, &temp_addr, &status);

                if (status != rpc_s_ok)
                {
                    error = RPC_C_SOCKET_EIO;
                    break;
                }

                rpc__naf_get_next_restricted_port
                    (temp_addr -> rpc_protseq_id, &port_name, &status);

                if (status != rpc_s_ok)
                {
                    error = RPC_C_SOCKET_EIO;
                    break;
                }

                rpc__naf_addr_set_endpoint (port_name, &temp_addr, &status);

                if (status != rpc_s_ok)
                {
                    error = RPC_C_SOCKET_EIO;
                    rpc_string_free (&port_name, &status);
                    break;
                }
                m_freem(nam);
                error = sockargs_nck(&nam, (struct sockaddr *)&temp_addr->sa, 
                                     temp_addr->len, MT_SONAME);
                if (error)
                    goto bad;

                if (sobind(so, nam) == 0)
                {
                    found = true;
                    error = RPC_C_SOCKET_OK;
                }
                else
                    error = RPC_C_SOCKET_EIO;

                rpc_string_free (&port_name, &status);
            }                           /* for i */

            if (!found)
            {
                error = RPC_C_SOCKET_EADDRINUSE;
            }
        }                               /* no well-known endpoint */
    }
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
    struct iovec aiov[MSG_MAXIOVLEN];
    struct uio auio;
    long len;
    int i;
    int s;
    struct file *fp;
    struct socket *so;
    label_t jmpbuf;
    int rc;

    if (error = x_getf(sock, &fp))
    {
        RPC_DBG_GPRINTF(("(rpc__socket_sendmsg,x_getf) error = %d\n", error));
        return (error);
    }
    so = (struct socket *)(fp->f_data);


    if ((u_int)iov_len >= MSG_MAXIOVLEN) {
        error = EMSGSIZE;
        goto bad;
    }

    bcopy((caddr_t)iov, (caddr_t)aiov, iov_len * sizeof (struct iovec));
    auio.uio_iov = aiov;
    auio.uio_iovcnt = iov_len;
    auio.uio_segflg = UIO_SYSSPACE;
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
label1:
    setuerror(0);
    s = splnet();
    if (( rc = setjmpx(&jmpbuf)) == 0)
    {
	error = sosend(so, to, &auio, 0 /* flags */, 0 /* rights */);
	clrjmpx(&jmpbuf);
    }
    else if (rc == EINTR)
    {
       RPC_DBG_GPRINTF(("sosend was interrupted\n"));
       sbunlock(&so->so_snd);
       splx(s);
       goto label1;
    }
    splx(s);

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
    int s;
    struct uio auio;

    label_t jmpbuf;
    int rc;
    struct file *fp;
    struct socket *so;

    if (error = x_getf(sock, &fp))
    {
        RPC_DBG_GPRINTF(("(rpc__socket_recvfrom,x_getf) error = %d\n", error));
        return (error);
    }
    so = (struct socket *)(fp->f_data);

    iov.base = buf;
    iov.len = len;
    auio.uio_iov = (struct iovec *)&iov;
    auio.uio_iovcnt = 1;
    auio.uio_segflg = UIO_SYSSPACE;
    auio.uio_offset = 0;        /* XXX */
    auio.uio_resid = len;

    /*
     * Typically, qsave is setup at this point to deal with EINTR vs RESTARTSYS.
     * For now I'm assuming that this isn't an issue since all our sockets
     * *should* be set to non-blocking mode, hence we shouldn't sleep.
     */
label1:
    setuerror(0);
    s = splnet();
    if (( rc = setjmpx(&jmpbuf)) == 0)
    {
       error = soreceive(so, &from, &auio,
            NULL /* flagsp */, NULL /* rightsp */, NULL);

       clrjmpx(&jmpbuf);
    }
    else if (rc == EINTR)
    {
       RPC_DBG_GPRINTF(("soreceive was interrupted\n"));
       sbunlock(&so->so_rcv);
       splx(s);
       goto label1;
    }
    splx(s);

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
        (void) bcopy(mtod(from, caddr_t),
            (caddr_t)&addr->sa, (unsigned)addr->len);
    }

bad:
    if (from)
        m_freem(from);
    if (error)
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
    long len;
    int i;
    int s;
    struct iovec aiov[MSG_MAXIOVLEN];
    struct uio auio;

    label_t jmpbuf;
    int rc;
    struct file *fp;
    struct socket *so;

    if ((u_int)iov_len >= MSG_MAXIOVLEN) {
        error = EMSGSIZE;
        goto bad;
    }

    if (error = x_getf(sock, &fp))
    {
        RPC_DBG_GPRINTF(("(rpc__socket_recvfrom,x_getf) error = %d\n", error));
        return (error);
    }
    so = (struct socket *)(fp->f_data);

    bcopy((caddr_t)iov, (caddr_t)aiov, iov_len * sizeof (struct iovec));
    auio.uio_iov = aiov;
    auio.uio_iovcnt = iov_len;
    auio.uio_segflg = UIO_SYSSPACE;
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
label1:
    setuerror(0);
    s = splnet();
    if (( rc = setjmpx(&jmpbuf)) == 0)
    {
       error = soreceive(so, &from, &auio,
            NULL /* flagsp */, NULL /* rightsp */, NULL);

       clrjmpx(&jmpbuf);
    }
    else if (rc == EINTR)
    {
       RPC_DBG_GPRINTF(("soreceive was interrupted\n"));
       sbunlock(&so->so_rcv);
       splx(s);
       goto label1;
    }
    splx(s);

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
        (void) bcopy(mtod(from, caddr_t),
            (caddr_t)&addr->sa, (unsigned)addr->len);
    }

bad:
    if (from)
        m_freem(from);
    if (error)
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
    struct file *fp;
    struct socket *so;
    int s;

    if (error = x_getf(sock, &fp))
    {
        RPC_DBG_GPRINTF(("(rpc__socket_inq_addr,x_getf) error = %d\n", error));
        return (error);
    }
    so = (struct socket *)(fp->f_data);


    m = m_getclr(M_WAIT, MT_SONAME);
    if (m == NULL) {
        error = ENOBUFS;
        goto bad1;
    }
    s = splnet();
    error = (*so->so_proto->pr_usrreq)(so, PRU_SOCKADDR,
                        (struct mbuf *)0, m, (struct mbuf *)0);
    splx(s);

    if (error) {
        goto bad;
    }
    if (addr->len > m->m_len)
        addr->len = m->m_len;
#ifdef COMPAT_43
        mtod(m, struct osockaddr *)->sa_family =
            mtod(m, struct sockaddr *)->sa_family;
#endif
    bcopy(mtod(m, caddr_t), (caddr_t)&addr->sa, (u_int)addr->len);
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
    rpc_socket_error_t error = RPC_C_SOCKET_OK;
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
    unsigned32 sizelen;
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
    error = getsockopt_nck(sock, SOL_SOCKET, SO_SNDBUF, (char *)ntxsize, (int *)&sizelen);
    if (error) {
        RPC_DBG_GPRINTF(("(rpc__socket_set_bufs) WARNING: get sndbuf failed - error = %d\n", 
            error));
        *ntxsize = RPC_C_SOCK_GUESSED_SNDBUF;
    }

    sizelen = sizeof *nrxsize;
    error = getsockopt_nck(sock, SOL_SOCKET, SO_RCVBUF, (char *)nrxsize, (int *)&sizelen);
    if (error) {
        RPC_DBG_GPRINTF(("(rpc__socket_set_bufs) WARNING: get rcvbuf failed - error = %d\n", 
            error));
        *nrxsize = RPC_C_SOCK_GUESSED_RCVBUF;
    }
#else
    *ntxsize = RPC_C_SOCK_GUESSED_SNDBUF;
    *nrxsize = RPC_C_SOCK_GUESSED_RCVBUF;
#endif
    return (RPC_C_SOCKET_OK);
}


/*
 * R P C _ _ S O C K E T _ S E T _ N B I O
 *
 * Set a socket to non-blocking mode.
 * 
 * Return RPC_C_SOCKET_OK on success, otherwise an error value.
 */

PRIVATE rpc_socket_error_t rpc__socket_set_nbio(sock)
rpc_socket_t sock;
{
    rpc_socket_error_t error = RPC_C_SOCKET_OK;
    struct file *fp;
    struct socket *so;
    int s;

    if (error = x_getf(sock, &fp))
    {
        RPC_DBG_GPRINTF(("(rpc__socket_set_nbio,x_getf) error = %d\n", error));
        return (error);
    }
    so = (struct socket *)(fp->f_data);


    s = splnet();
    so->so_state |= SS_NBIO;
    splx(s);
    return (error);
}


/*
 * R P C _ _ S O C K E T _ S E T _ C L O S E _ O N _ E X E C
 *
 *
 * Set a socket to a mode whereby it is not inherited by a spawned process
 * executing some new image. This is possibly a no-op on some systems.
 *
 * Return RPC_C_SOCKET_OK on success, otherwise an error value.
 *
 * This is a no-op because rpc_socket_t's aren't part of a process'
 * state (i.e. they are not in the file table)
 */

PRIVATE rpc_socket_error_t rpc__socket_set_close_on_exec(sock)
rpc_socket_t sock;
{
    return (RPC_C_SOCKET_OK);
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
                        (int *)&optlen) == -1  ? -1 : RPC_C_SOCKET_OK);
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

PRIVATE rpc_socket_error_t rpc__socket_select(nfds, rsoc, nfsoc, fsoc, tmo)
int nfds;
rpc_socket_sel_t *rsoc;
int *nfsoc;
rpc_socket_sel_t *fsoc;
struct timeval *tmo;
{
   long timeout;
   struct pollfd *pfd;
   int i, j, rv;
   int size;

   if (tmo == NULL)
      timeout = -1;
   else
      timeout = tmo->tv_usec/1000 + tmo->tv_sec*1000;

   *nfsoc = 0;
   size = (nfds * sizeof(struct pollfd));
   pfd = (struct pollfd *)xmalloc(size, 3, kernel_heap);
   if (pfd == NULL)
   {
      printf("pfd NULL\n");
      _exit(-1);
   }
   for (i=0; i<nfds; i++)
   {
      pfd[i].fd = (long)x_ofile[(int)rsoc[i].so];
      pfd[i].reqevents = rsoc[i].events;
      fsoc[i].so = -1;
   }
   rv = fp_poll(pfd, nfds, timeout, 0);	/* carl says it is fine (splnet) */
   if (rv)
   {
      for (i=0; i<nfds; i++)
      {
         if (pfd[i].rtnevents)
         {
            (*nfsoc)++;
            fsoc[*nfsoc - 1].so = rsoc[i].so;
            fsoc[*nfsoc - 1].events = rsoc[i].events;
         }
      }
   }

   if (pfd != NULL)
      xmfree(pfd, kernel_heap);
   return (0);
}

/*
 * R P C _ _ S O C K E T _ N O W R I T E B L O C K _ W A I T
 *
 * Wait until the a write on the socket should succeed without
 * blocking.  If tmo is NULL, the wait is unbounded, otherwise
 * tmo specifies the max time to wait. RPC_C_SOCKET_ETIMEDOUT
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
        return RPC_C_SOCKET_ETIMEDOUT;

    return RPC_C_SOCKET_OK;
}
