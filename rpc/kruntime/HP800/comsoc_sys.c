/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: comsoc_sys.c,v $
 * Revision 1.1.65.2  1996/02/17  23:59:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:23  marty]
 *
 * Revision 1.1.65.1  1995/12/08  00:13:52  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/7  1995/09/14  21:16 UTC  maunsell_c
 * 	merge Walnut Creek port to HPDCE02 mainline
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/6  1995/09/13  20:38 UTC  maunsell_c
 * 	fix typo discovered from audit u.u_save to u.u_qsave
 * 	remove __hpux1010p ifdef from around setjump call to
 * 	where the untimeout call occurs also part of audit changes
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/5  1995/09/13  15:19 UTC  maunsell_c
 * 	merge out erroneous mainline version (how did this get here anyway?)
 * 
 * 	HP revision /main/HPDCE02/6  1995/09/12  13:01 UTC  maunsell_c
 * 	use kthreadp not procp for selwakeup, untimeout, timeout calls
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/4  1995/08/22  17:48 UTC  maunsell_c
 * 	use krpc_ vs osi_ calls
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/3  1995/08/14  17:54 UTC  maunsell_c
 * 	use osi_xxxx macros vs. 1010p ifdefs
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/2  1995/07/18  16:46 UTC  maunsell_c
 * 	walnut creek port
 * 
 * 	HP revision /main/HPDCE02/5  1995/07/06  15:54 UTC  tatsu_s
 * 	Added missing KFREE().
 * 
 * 	HP revision /main/HPDCE02/4  1995/06/30  19:52 UTC  lmm
 * 	merge reduce stack usage fix
 * 
 * 	HP revision /main/HPDCE02/lmm_reduce_rpc_stack/3  1995/06/28  16:15 UTC  lmm
 * 	malloc aiov in rpc__socket_recvmsg to save stack space
 * 
 * 	HP revision /main/HPDCE02/lmm_reduce_rpc_stack/2  1995/06/27  21:29 UTC  lmm
 * 	use KALLOC instead of malloc for aiov
 * 
 * 	HP revision /main/HPDCE02/lmm_reduce_rpc_stack/1  1995/06/21  16:16 UTC  lmm
 * 	malloc aiov in rpc__socket_sendmsg to save stack space
 * 
 * 	HP revision /main/HPDCE02/3  1994/10/28  15:19 UTC  kissel
 * 	Fix MP locking for 9.x.  Of course it is different from 10.0.
 * 
 * 	HP revision /main/HPDCE02/2  1994/10/26  17:38 UTC  kissel
 * 	Merge in MP-safe changes from kissel_mpsafe_bug2.
 * 
 * 	HP revision /main/HPDCE01/kissel_mpsafe_bug2/1  1994/10/20  20:55 UTC  kissel
 * 	Add some locking that now seems to be required for MP-safeness.
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:33 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 
 * 	HP revision /main/HPDCE01/4  1994/06/09  13:00 UTC  kissel
 * 	Merge in MP-safe bugfix changes.
 * 
 * 	HP revision /main/HPDCE01/MOTHRA/kissel_mpsafe_bugs/1  1994/06/08  18:03 UTC  kissel
 * 	The MP_SPINLOCK_USAV versions don't exist for 700 at 9.x, and either one will work
 * 	for 800 at 9.x, so define macros to take care of it.
 * 
 * 	HP revision /main/HPDCE01/3  1994/05/26  13:26 UTC  kissel
 * 	Merge from the kissel_mpsafe branch to add locking to make this code MP-safe.
 * 
 * 	HP revision /main/mort_10.0_port/brezak_rodan/2  1994/05/02  20:51 UTC  brezak
 * 	Remove extraneous code in rpc__soo_select().
 * 
 * 	HP revision /main/mort_10.0_port/brezak_rodan/1  1994/04/29  23:00 UTC  brezak
 * 	Where's NORM when I need him?
 * 
 * 	HP revision /main/HPDCE01/1  1994/03/31  23:55 UTC  mort
 * 	Changes needed to get dce_krpc.ext to load into IC1
 * 	In 10.0, the macros NETMP_GO_EXCLUSIVE/NETMP_GO_UNEXCLUSIVE no longer
 * 	exist.  They are replaced by NET_SPLNET/SOCKET_LOCK and
 * 	SOCKET_UNLOCK/NET_SPLX, respectively.  SOCKET_{,UN}LOCK is defined in
 * 	<sys/socketvar.h> which includes <net/netmp.h>, which defines
 * 	NET_SPL{NET,X}.  The original code taken from sys/sys_socket.c for the
 * 	soo_select() function immediately follows rpc__soo_select under the
 * 	null ifdef UXKERN_NOTDEF.  The __hpux1000p has been used to merge this
 * 	sample code just like in the rpc__soo_select code.
 * 
 * 	HP revision /main/mort_10.0-port/2  1994/03/31  20:35 UTC  mort
 * 	left out return (*revents);'
 * 
 * 	HP revision /main/mort_10.0-port/1  1994/03/30  21:51 UTC  mort
 * 	merge hpux sys/sys_socket.c changes
 * 	[1995/12/07  23:55:37  root]
 * 
 * Revision 1.1.63.4  1994/06/10  20:54:02  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:59:48  devsrc]
 * 
 * Revision 1.1.63.3  1994/05/27  15:35:35  tatsu_s
 * 	Fixed the corrupted iov in rpc__socket_recvmsg().
 * 	[1994/05/05  18:00:12  tatsu_s]
 * 
 * 	DG multi-buffer fragments.
 * 	Added rpc__socket_recvmsg().
 * 	[1994/04/29  18:53:49  tatsu_s]
 * 
 * Revision 1.1.63.2  1994/02/02  21:48:50  cbrooks
 * 	OT9855 code cleanup breaks KRPC
 * 	[1994/02/02  20:59:53  cbrooks]
 * 
 * Revision 1.1.63.1  1994/01/21  22:31:16  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:32:56  cbrooks]
 * 
 * Revision 1.1.61.1  1993/10/05  13:41:39  root
 * 	Add port restriction support for krpc - this is porting
 * 	the code in runtime/comsoc_bsd.c  (rpc__socket_bind)
 * 	to comsoc_sys.c
 * 	[1993/10/05  13:29:52  root]
 * 
 * Revision 1.1.4.2  1993/06/10  19:21:37  sommerfeld
 * 	Initial HPUX RP version.
 * 	[1993/06/03  21:58:44  kissel]
 * 
 * 	Port to HPUX 9.0.
 * 	[1992/10/16  22:11:31  toml]
 * 
 * 	06/16/92 tmm Created from COSL drop
 * 	[1992/06/18  18:30:26  tmm]
 * 
 * $EndLog$
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
#include <dce/ker/krpc_osi_mach.h>
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
#if ! __hpux1000p
#include <net/netmp.h>
#endif /* __hpux1000p */

extern struct timeval time;

/* ======================================================================== */

/*
 * What we think a socket's buffering is in case rpc__socket_set_bufs()
 * fails miserably.  This is system dependent if you want it to be correct.
 */

#define RPC_C_SOCK_GUESSED_RCVBUF    (4 * 1024)
#define RPC_C_SOCK_GUESSED_SNDBUF    (4 * 1024)

/*
 * The following macros are used to centralize the locking code definitions.  It
 * turns out that the _USAV versions of the SPINLOCK macros aren't available on
 * the 700 at 9.x.  They are available on the 800 at 9.x and on both machine
 * types at 10.x.  Hence, this macro ugliness to handle all the cases.  For the
 * MP_LOCK case, we will just pay the penalty of an extra stack variable
 * (savestate) in the 10.0 case in order not to get too crazy.
*/
#if __hpux1000p
#define COMSOC_SPINLOCK_DECL	u_int context
#define COMSOC_SPINLOCK		MP_SPINLOCK_USAV(sched_lock, context)
#define COMSOC_SPINUNLOCK	MP_SPINUNLOCK_USAV(sched_lock, context)
#define COMSOC_MP_LOCK		NET_SPLNET(oldlevel);	/* This only applies to uniprocessors. */ \
				SOCKET_LOCK(sock);	/* This only applies to multiprocessors. */
#define COMSOC_MP_UNLOCK	SOCKET_UNLOCK(sock); \
				NET_SPLX(oldlevel);
#else
#define COMSOC_SPINLOCK_DECL
#define COMSOC_SPINLOCK		MP_SPINLOCK(sched_lock)
#define COMSOC_SPINUNLOCK	MP_SPINUNLOCK(sched_lock)
#define COMSOC_MP_LOCK		NETMP_GO_EXCLUSIVE(oldlevel, savestate);
#define COMSOC_MP_UNLOCK	NETMP_GO_UNEXCLUSIVE(oldlevel, savestate);
#endif /* __hpux1000p */

/* ======================================================================== */

INTERNAL rpc_socket_error_t getsockopt_nck _DCE_PROTOTYPE_((
        rpc_socket_t  /*sock*/,
        int  /*level*/, 
        int  /*name*/,
        char * /*val*/,
        int * /*valsize*/
    ));

INTERNAL rpc_socket_error_t setsockopt_nck _DCE_PROTOTYPE_((
        rpc_socket_t  /*sock*/,
        int  /*level*/, 
        int  /*name*/,
        char * /*val*/,
        int  /*valsize*/
    ));

INTERNAL rpc_socket_error_t sockargs_nck _DCE_PROTOTYPE_((
        struct mbuf ** /*aname*/,
        struct sockaddr * /*name*/,
        int  /*namelen*/,
        int  /*type*/
    ));

/* ========================================================================= */


INTERNAL rpc_socket_error_t getsockopt_nck
#ifdef _DCE_PROTO_
(
    rpc_socket_t sock,
    int level,
    int name,
    char *val,
    int *avalsize
)
#else
(sock, level, name, val, avalsize)
rpc_socket_t sock;
int level;
int name;
char *val;
int *avalsize;
#endif
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
        bcopy(mtod(m, caddr_t), val, (u_int)valsize);

        *avalsize = valsize;
    }
bad:
    if (m != NULL)
        (void) m_free(m);
    return (error);
}

INTERNAL rpc_socket_error_t setsockopt_nck
#ifdef _DCE_PROTO_
(
    rpc_socket_t sock,
    int level, 
    int name,
    char *val,
    int valsize
)
#else
(sock, level, name, val, valsize)
rpc_socket_t sock;
int level, name;
char *val;
int valsize;
#endif
{
    struct mbuf *m = NULL;
    rpc_socket_error_t error;

    if (valsize > MLEN)
        return(EINVAL);
    if (val) {
        m = m_get(M_WAIT, MT_SOOPTS);
        if (m == NULL)
            return(ENOBUFS);
        bcopy(val, mtod(m, caddr_t), (u_int)valsize);

        m->m_len = valsize;
    }
    error = sosetopt(sock, level, name, m);
    return (error);
}

INTERNAL rpc_socket_error_t sockargs_nck
#ifdef _DCE_PROTO_
(
    struct mbuf **aname,
    struct sockaddr *name,
    int namelen, 
    int type
)
#else
(aname, name, namelen, type)
struct mbuf **aname;
struct sockaddr *name;
int namelen, type;
#endif
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
#ifndef	HPUX
    /* HPUX does not need this bsd44 -> bsd43 sockaddr conversion.
     * this code isn't really correctly portable anyway. */
    if (type == MT_SONAME) {
        register struct sockaddr *sa = mtod(m, struct sockaddr *);
#if defined(COMPAT_43) && BYTE_ORDER != BIG_ENDIAN
        if (sa->sa_family == 0 && sa->sa_len < AF_MAX)
            sa->sa_family = sa->sa_len;
#endif
        sa->sa_len = namelen;
    }
#endif /* HPUX */
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

PRIVATE rpc_socket_error_t rpc__socket_open
#ifdef _DCE_PROTO_
(
    rpc_protseq_id_t protseq,
    rpc_socket_t *sock
)
#else
(protseq, sock)
rpc_protseq_id_t protseq;
rpc_socket_t *sock;
#endif
{
    rpc_socket_error_t error;

    error = socreate(
                (int) RPC_PROTSEQ_INQ_NAF_ID(protseq),
                sock,
                (int) RPC_PROTSEQ_INQ_NET_IF_ID(protseq),
                (int) RPC_PROTSEQ_INQ_NET_PROT_ID(protseq)
#ifndef	HPUX80
	     ,0 /* flags */      
#endif
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

PRIVATE rpc_socket_error_t rpc__socket_open_basic
#ifdef _DCE_PROTO_
(
    rpc_naf_id_t naf,
    rpc_network_if_id_t net_if,
    rpc_network_protocol_id_t net_prot,
    rpc_socket_t *sock
)
#else
(naf, net_if, net_prot, sock)
rpc_naf_id_t naf;
rpc_network_if_id_t net_if;
rpc_network_protocol_id_t net_prot;
rpc_socket_t *sock;
#endif
{
    rpc_socket_error_t error;

#ifdef	HPUX80
    error = socreate((int) naf, sock, (int) net_if, (int) net_prot);
#else
    error = socreate((int) naf, sock, (int) net_if, (int) net_prot, 0 /* flags */);
#endif	/* HPUX80 */
    return (error);
}


/*
 * R P C _ _ S O C K E T _ C L O S E
 *
 * Close (destroy) a socket.
 *
 * (see BSD UNIX close(2)).
 */

PRIVATE rpc_socket_error_t rpc__socket_close
#ifdef _DCE_PROTO_
(
    rpc_socket_t sock
)
#else
(sock)
rpc_socket_t sock;
#endif
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

PRIVATE rpc_socket_error_t rpc__socket_bind
#ifdef _DCE_PROTO_
(
    rpc_socket_t sock,
    rpc_addr_p_t addr
)
#else
(sock, addr)
rpc_socket_t sock;
rpc_addr_p_t addr;
#endif
{
    struct mbuf *nam;
    rpc_socket_error_t error;
    unsigned32 status;
    rpc_addr_p_t temp_addr=NULL;

    error = sockargs_nck(&nam, (struct sockaddr *)&addr->sa, addr->len, MT_SONAME);
    if (error)
        goto bad;


    if (! RPC_PROTSEQ_TEST_PORT_RESTRICTION (addr -> rpc_protseq_id))
    {
        error = sobind(sock, nam);	/* does its own splnet */
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
            error = sobind(sock, nam);
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

                if (sobind(sock, nam) == 0)
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

PRIVATE rpc_socket_error_t rpc__socket_sendmsg
#ifdef _DCE_PROTO_
(
    rpc_socket_t sock,
    rpc_socket_iovec_p_t iov,   /* array of bufs of data to send */
    int iov_len,                /* number of bufs */
    rpc_addr_p_t addr,          /* addr of receiver */
    int *cc                    /* returned number of bytes actually sent */
)
#else
(sock, iov, iov_len, addr, cc)
rpc_socket_t sock;
rpc_socket_iovec_p_t iov;   /* array of bufs of data to send */
int iov_len;                /* number of bufs */
rpc_addr_p_t addr;          /* addr of receiver */
int *cc;                    /* returned number of bytes actually sent */
#endif
{
    rpc_socket_error_t error;
    struct mbuf *to = NULL;
    struct uio auio;
#ifndef HPDCE_SAVE_STACK
    struct iovec *aiov = NULL;
#else
    struct iovec aiov[MSG_MAXIOVLEN];	/* XXX use malloc */
#endif 
    long len;
    int i;

#ifndef HPDCE_SAVE_STACK
    aiov = (struct iovec *)KALLOC(sizeof(struct iovec) * MSG_MAXIOVLEN);
    if (aiov == NULL){
	error = ENOMEM;
	goto bad;
    }
#endif
    if ((u_int)iov_len >= MSG_MAXIOVLEN) {
        error = EMSGSIZE;
        goto bad;
    }


    /* don't corrupt the passed iov */
    bcopy((caddr_t)iov,(caddr_t)aiov, iov_len * sizeof (struct iovec));

    auio.uio_iov = aiov;
    auio.uio_iovcnt = iov_len;
    auio.uio_segflg = UIO_SYSSPACE;
    auio.uio_rw = UIO_WRITE;
    auio.uio_offset = 0;        /* XXX */
    auio.uio_resid = 0;
    for (i = 0; i < iov_len; i++, iov++) {
        if (iov->len < 0)
#ifndef HPDCE_SAVE_STACK
	{
	    KFREE((caddr_t)aiov, sizeof(struct iovec) * MSG_MAXIOVLEN);
            return(EINVAL);
	}
#else
            return(EINVAL);
#endif
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
#ifdef HPUX
    /* XXX HPUX has no 'control' parameter */
#ifdef	HPUX80
    error = sosend(sock, to, &auio, 0 /* flags */, (caddr_t)NULL /* rights */);
#else
    error = sosend(sock, to, &auio, 0 /* flags */, (caddr_t)NULL /* rights */, NULL /* kdatap */);
#endif	/* HPUX80 */
#else
    error = sosend(sock, to, &auio, 0 /* flags */, 0 /* rights */, 0 /* control */);
#endif	/* HPUX */
    *cc = len - auio.uio_resid;
bad:
    if (to)
        m_freem(to);
    if (error)
        RPC_DBG_GPRINTF(("(rpc__socket_sendmsg) error = %d\n", error));
#ifndef HPDCE_SAVE_STACK
    if (aiov != NULL)
    {
	KFREE((caddr_t)aiov, sizeof(struct iovec) * MSG_MAXIOVLEN);
    }
#endif
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

PRIVATE rpc_socket_error_t rpc__socket_recvfrom
#ifdef _DCE_PROTO_
(
    rpc_socket_t sock,
    byte_p_t buf,           /* buf for rcvd data */
    int len,                /* len of above buf */
    rpc_addr_p_t addr,      /* addr of sender, really in/out */
    int *cc                /* returned number of bytes actually rcvd */
)
#else
(sock, buf, len, addr, cc)
rpc_socket_t sock;
byte_p_t buf;           /* buf for rcvd data */
int len;                /* len of above buf */
rpc_addr_p_t addr;      /* addr of sender; really in/out */
int *cc;                /* returned number of bytes actually rcvd */
#endif
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
#ifdef HPUX
    {
    /* HPUX must pass a non-null pointer to 'flag' and it must be initialized */
    int flag = 0;  
#ifdef	HPUX80
    error = soreceive(sock, &from, &auio, &flag, (caddr_t) NULL /* rightsp */);
#else
    error = soreceive(sock, &from, &auio, &flag, (caddr_t) NULL /* rightsp */, NULL /* kdatap */);
#endif	/* HPUX80 */
    }
#else
    error = soreceive(sock, &from, &auio, 
            NULL /* flagsp */, NULL /* rightsp */, NULL /* controlp */);
#endif	/* HPUX */
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
        (void) bcopy(mtod(from, caddr_t), (caddr_t)&addr->sa, (unsigned)addr->len);

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
 * (see BSD UNIX recvmsg(2)).
 */

PRIVATE rpc_socket_error_t rpc__socket_recvmsg
#ifdef _DCE_PROTO_
(
    rpc_socket_t sock,
    rpc_socket_iovec_p_t iov,   /* array of bufs for rcvd data */
    int iov_len,                /* number of bufs */
    rpc_addr_p_t addr,          /* addr of sender; really in/out */
    int *cc                     /* returned number of bytes actually rcvd */
)
#else
(sock, iov, iov_len, addr, cc)
rpc_socket_t sock;
rpc_socket_iovec_p_t iov;   /* array of bufs for rcvd data */
int iov_len;                /* number of bufs */
rpc_addr_p_t addr;          /* addr of sender; really in/out */
int *cc;                    /* returned number of bytes actually rcvd */
#endif
{
    rpc_socket_error_t error;
    struct mbuf *from = NULL;
    struct uio auio;
#ifndef HPDCE_SAVE_STACK
    struct iovec *aiov = NULL;
#else
    struct iovec aiov[MSG_MAXIOVLEN];	/* XXX use malloc */
#endif 
    long len;
    int i;

#ifndef HPDCE_SAVE_STACK
    aiov = (struct iovec *)KALLOC(sizeof(struct iovec) * MSG_MAXIOVLEN);
    if (aiov == NULL){
	error = ENOMEM;
	goto bad;
    }
#endif
    if ((u_int)iov_len >= MSG_MAXIOVLEN) {
        error = EMSGSIZE;
        goto bad;
    }

    /* don't corrupt the passed iov */
    bcopy((caddr_t)iov, (caddr_t)aiov, iov_len * sizeof (struct iovec));

    auio.uio_iov = aiov;
    auio.uio_iovcnt = iov_len;
    auio.uio_segflg = UIO_SYSSPACE;
    auio.uio_rw = UIO_READ;
    auio.uio_offset = 0;        /* XXX */
    auio.uio_resid = 0;
    for (i = 0; i < iov_len; i++, iov++) {
        if (iov->len < 0)
#ifndef HPDCE_SAVE_STACK
	{
	    KFREE((caddr_t)aiov, sizeof(struct iovec) * MSG_MAXIOVLEN);
            return(EINVAL);
	}
#else
            return(EINVAL);
#endif
        auio.uio_resid += iov->len;
    }

    len = auio.uio_resid;

    /*
     * Typically, qsave is setup at this point to deal with EINTR vs RESTARTSYS.
     * For now I'm assuming that this isn't an issue since all our sockets
     * *should* be set to non-blocking mode, hence we shouldn't sleep.
     */
    {
    /* HPUX must pass a non-null pointer to 'flag' and it must be initialized */
    int flag = 0;  
#ifdef	HPUX80
    error = soreceive(sock, &from, &auio, &flag, (caddr_t) NULL /* rightsp */);
#else
    error = soreceive(sock, &from, &auio, &flag, (caddr_t) NULL /* rightsp */, NULL /* kdatap */);
#endif	/* HPUX80 */
    }
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
    if (error && (error != EWOULDBLOCK))
        RPC_DBG_GPRINTF(("(rpc__socket_recvmsg) error = %d\n", error));
#ifndef HPDCE_SAVE_STACK
    if (aiov != NULL)
    {
	KFREE((caddr_t)aiov, sizeof(struct iovec) * MSG_MAXIOVLEN);
    }
#endif
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

PRIVATE rpc_socket_error_t rpc__socket_inq_endpoint
#ifdef _DCE_PROTO_
(
    rpc_socket_t sock,
    rpc_addr_p_t addr  /* really in/out */
)
#else
(sock, addr)
rpc_socket_t sock;
rpc_addr_p_t addr;  /* really in/out */
#endif
{
    struct mbuf *m = NULL;
    rpc_socket_error_t error;
    int oldlevel;
    sv_sema_t savestate;

    m = m_getclr(M_WAIT, MT_SONAME);
    if (m == NULL) {
        error = ENOBUFS;
        goto bad1;
    }
    /*
    ** In keeping with getsockname() in kern_bld/sys/uipc_syscall.c, this
    ** routine should do MP locking like that does.  The following call actually
    ** ends up in udp_usrreq() in kern_bld/netinet/udp_usrreq.c, which has a
    ** LOCK_ASSERT macro that checks to make sure the socket is locked.  Thus,
    ** we had better do it here.  The other socket calls seem to do their own
    ** locking.  Only this one expects its caller to do the locking.  And, of
    ** course, the locking is different between 9.x and 10.0.  This is handled
    ** by the macro definitions at the beginning of this file.
    */
    COMSOC_MP_LOCK;
    error = (*sock->so_proto->pr_usrreq)(sock, PRU_SOCKADDR,
                        (struct mbuf *)0, m, (struct mbuf *)0);
    COMSOC_MP_UNLOCK;

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
 * Used only by Datagram based Protocol Services. */

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

PRIVATE rpc_socket_error_t rpc__socket_set_bufs
#ifdef _DCE_PROTO_
(
    rpc_socket_t sock,
    unsigned32 txsize,
    unsigned32 rxsize,
    unsigned32 *ntxsize,
    unsigned32 *nrxsize
)
#else
(sock, txsize, rxsize, ntxsize, nrxsize)
rpc_socket_t sock;
unsigned32 txsize;
unsigned32 rxsize;
unsigned32 *ntxsize;
unsigned32 *nrxsize;
#endif
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
        *ntxsize = RPC_C_SOCK_GUESSED_SNDBUF;
    }

    sizelen = sizeof *nrxsize;
    error = getsockopt_nck(sock, SOL_SOCKET, SO_RCVBUF, (char *)nrxsize, &sizelen);
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
 * Return rpc_c_socket_ok on success, otherwise an error value.
 */

PRIVATE rpc_socket_error_t rpc__socket_set_nbio
#ifdef _DCE_PROTO_
(
    rpc_socket_t sock
)
#else
(sock)
rpc_socket_t sock;
#endif
{
    rpc_socket_error_t error = RPC_C_SOCKET_OK;

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

PRIVATE rpc_socket_error_t rpc__socket_set_close_on_exec
#ifdef _DCE_PROTO_
(
    rpc_socket_t sock
)
#else
(sock)
rpc_socket_t sock;
#endif
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

PRIVATE rpc_socket_error_t rpc__socket_getpeername 
#ifdef _DCE_PROTO_
(
    rpc_socket_t sock,
    rpc_addr_p_t addr
)
#else
(sock, addr)
rpc_socket_t sock;
rpc_addr_p_t addr;
#endif
{
    DIE("(rpc__socket_getpeername) Not Supported");
}

/*
 * R P C _ _ S O C K E T _ G E T _ I F _ I D
 *
 * Get socket network interface id (socket type).
 *
 */

PRIVATE rpc_socket_error_t rpc__socket_get_if_id 
#ifdef _DCE_PROTO_
(
    rpc_socket_t        sock,
    rpc_network_if_id_t *network_if_id
)
#else
(sock, network_if_id)
rpc_socket_t        sock;
rpc_network_if_id_t *network_if_id;
#endif
{
    int optlen;
    
    optlen = sizeof(rpc_network_if_id_t);
    
    return (getsockopt_nck (sock,
                        SOL_SOCKET,
                        SO_TYPE,
                        (char *)network_if_id,
                        &optlen) == -1  ? -1 : RPC_C_SOCKET_OK);
}

/*
 * R P C _ _ S O C K E T _ S E L E C T
 * 
 *
 * Perform a 'select' operation on the list of sockets defined by the
 * soc array.  Return the active sockets in the "found" fsoc[] array
 * and the found count as a return value.
 *
 * This select stuff should look very similar to standard BSD processing (see
 * kern_bld/sys/sys_gen.c in selwakeup() for MP safe code).
 */

PRIVATE rpc_socket_error_t rpc__socket_select
#ifdef _DCE_PROTO_
(
    int nsoc,
    rpc_socket_sel_t *soc,
    int *nfsoc,
    rpc_socket_sel_t *fsoc,
    struct timeval *tmo
)
#else
(nsoc, soc, nfsoc, fsoc, tmo)
int nsoc;
rpc_socket_sel_t *soc;
int *nfsoc;
rpc_socket_sel_t *fsoc;
struct timeval *tmo;
#endif
{ 
    int err = RPC_C_SOCKET_OK;
    int timeout_hz;
    int s;
    struct timeval atv;
    label_t lqsave;
    int ncoll;
    extern int nselcoll;     /* used by selwakeup */
    extern int unselect();
    COMSOC_SPINLOCK_DECL;

    if (tmo) 
    {
        bcopy(tmo, &atv, sizeof (struct timeval));

        if (itimerfix(&atv)) {
	    err = EINVAL;
	    goto done;
	}

	/* Convert select timeout to hz. If too large, use */
	/* maximum hz value.                               */

	if (atv.tv_sec <= 0x7fffffff / HZ - HZ)
	    timeout_hz = atv.tv_sec * HZ + ((atv.tv_usec * HZ + 500000) / 1000000);
	else
	    timeout_hz = 0x7fffffff;
    } 

retry:
    ncoll = nselcoll;
    COMSOC_SPINLOCK;
    krpc_osi_set_sel();
    COMSOC_SPINUNLOCK;

    *nfsoc = rpc__socscan(nsoc, soc, fsoc);
    if (*nfsoc)
        goto done;

    if (tmo && timeout_hz <= 0)
	goto done;

    s = spl6();
    if (krpc_osi_get_sel() == 0 || nselcoll != ncoll) {
	COMSOC_SPINLOCK;
        krpc_osi_clear_sel();
	COMSOC_SPINUNLOCK;
	splx(s);
	goto retry;
    }
    COMSOC_SPINLOCK;
    krpc_osi_clear_sel();
    COMSOC_SPINUNLOCK;
    if (tmo) {
    
	lqsave = u.u_qsave;
        if (setjmp(&u.u_qsave)) {
#if __hpux1010p
            untimeout(unselect, (caddr_t)u.u_kthreadp);
#else
	    untimeout(unselect, (caddr_t)u.u_procp);
#endif
	    splx(s);
	    return(EINTR);
	}
#if __hpux1010p
        timeout(unselect, (caddr_t)u.u_kthreadp, timeout_hz);
#else
	timeout(unselect, (caddr_t)u.u_procp, timeout_hz);
#endif
    }

    sleep((caddr_t)&selwait, PZERO+1);

    if (tmo) {
	u.u_qsave = lqsave;

	/* untimeout returns the number of hz left until */
	/* the timeout would have gone off, or -1 if it  */
	/* has already gone off.                         */
#if __hpux1010p
        timeout_hz = untimeout(unselect, (caddr_t)u.u_kthreadp);
#else
	timeout_hz = untimeout(unselect, (caddr_t)u.u_procp);
#endif
    }

    splx(s);
    goto retry;

done:
    return(err);      
}

#ifdef	UXKERN_NOTDEF
select()
{
	register struct uap  {
		int	nd;
		fd_set	*in, *ou, *ex;
		struct	timeval *tv;
	} *uap = (struct uap *)u.u_ap;
	fd_set ibits[3], obits[3];
	struct timeval atv;
	int s, ncoll, ni;
	label_t lqsave;
	int timeout_hz;
	COMSOC_SPINLOCK_DECL;

	/*b_z_e_r_o((caddr_t)ibits, sizeof(ibits));*/
	memset((caddr_t)ibits, 0, sizeof(ibits));
	
	/*b_z_e_r_o((caddr_t)obits, sizeof(obits));*/
	memset((caddr_t)obits, 0, sizeof(obits));

	if (uap->nd > u.u_maxof)
		uap->nd = u.u_maxof;	/* forgiving, if slightly wrong */
	ni = howmany(uap->nd, NFDBITS);

	if (uap->nd < 0) {
		u.u_error = EINVAL;
		return;
	}

#define	getbits(name, x) \
	if (uap->name) { \
		u.u_error = copyin((caddr_t)uap->name, (caddr_t)&ibits[x], \
		    (unsigned)(ni*sizeof (fd_mask))); \
		if (u.u_error) \
			goto done; \
	}
	getbits(in, 0);
	getbits(ou, 1);
	getbits(ex, 2);
#undef	getbits
	if (uap->tv) {
		u.u_error = copyin((caddr_t)uap->tv, (caddr_t)&atv,
			sizeof (atv));
		if (u.u_error)
			goto done;
		if (itimerfix(&atv)) {
			u.u_error = EINVAL;
			goto done;
		}

		/* Convert select timeout to hz. If too large, use */
		/* maximum hz value.                               */

		if (atv.tv_sec <= 0x7fffffff / HZ - HZ)
		    timeout_hz = atv.tv_sec * HZ + ((atv.tv_usec * HZ + 500000) / 1000000);
		else
		    timeout_hz = 0x7fffffff;
	/* at this point, atv is delta time.
	 * timeout_hz is delta hz.
	 */
	}
retry:
	ncoll = nselcoll;
	COMSOC_SPINLOCK;
        krpc_osi_set_sel();
	COMSOC_SPINUNLOCK;
	u.u_r.r_val1 = selscan(ibits, obits, uap->nd);
	if (u.u_error || u.u_r.r_val1)
		goto done;

	if (uap->tv && timeout_hz <= 0)
	    goto done;

	s = spl6();
        if (krpc_osi_get_sel() == 0 || nselcoll != ncoll) {
		COMSOC_SPINLOCK;
                krpc_osi_clear_sel();
		COMSOC_SPINUNLOCK;
		splx(s);
		goto retry;
	}
	COMSOC_SPINLOCK;
        krpc_osi_clear_sel();
	COMSOC_SPINUNLOCK;
	if (uap->tv) {

		/*
		 * Note: if we get a signal but no timer
		 * was set then the masks will remain as
		 * they were on input.  The code will take
		 * a longjmp into syscall and syscall will
		 * return with EINTR.
		 */

		lqsave = u.u_qsave;
                if (setjmp(&u.u_qsave)) {
#if __hpux1010p
                        untimeout(unselect, (caddr_t)u.u_kthreadp);
#else
			untimeout(unselect, (caddr_t)u.u_procp);
#endif
			u.u_error = EINTR;
			splx(s);
			return;
		}
#if __hpux1010p
		timeout(unselect, (caddr_t)u.u_kthreadp, timeout_hz);
#else
                timeout(unselect, (caddr_t)u.u_procp, timeout_hz);
#endif
	}
	sleep((caddr_t)&selwait, PZERO+1);
	if (uap->tv) {
		u.u_qsave = lqsave;

		/* untimeout returns the number of hz left until */
		/* the timeout would have gone off, or -1 if it  */
		/* has already gone off.                         */
#if __hpux1010p
		timeout_hz = untimeout(unselect, (caddr_t)u.u_kthreadp);
#else
                timeout_hz = untimeout(unselect, (caddr_t)u.u_procp);
#endif
	}
	splx(s);
	goto retry;
done:
#define	putbits(name, x) \
	if (uap->name) { \
		int error = copyout((caddr_t)&obits[x], (caddr_t)uap->name, \
		    (unsigned)(ni*sizeof (fd_mask))); \
		if (error) \
			u.u_error = error; \
	}
	if (u.u_error ==0) {
	  putbits(in, 0);
	  putbits(ou, 1);
	  putbits(ex, 2);
	}
#undef putbits
}
#endif	/* UXKERN_NOTDEF */

int
rpc__socscan(nsoc, soc, fsoc)
int nsoc;
rpc_socket_sel_t *soc;
rpc_socket_sel_t *fsoc;
{
    int i;
    int revents;
    int nfnd;
    int s = spl6();

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

    splx(s);

    return(nfnd);
}

rpc__soo_select(ssel, fssel)
rpc_socket_sel_t *ssel;
rpc_socket_sel_t *fssel;
{
        rpc_socket_t    so = ssel->so;
        int           events = ssel->events;
        int           *revents = &fssel->events;
	int oldlevel;

#if __hpux1000p
	NET_SPLNET(oldlevel);
	SOCKET_LOCK(so);
#else /* not __hpux1000p */
	sv_sema_t savestate;		/* MPNET: MP save state */
	/*MPNET: turn MP protection on */
	NETMP_GO_EXCLUSIVE(oldlevel, savestate);
#endif /* __hpux1000p */

        fssel->so = so;
        *revents = 0;

        if (events & POLLNORM) {
#if __hpux1000p
		SOCKBUF_LOCK(&so->so_rcv);
#endif /* __hpux1000p */
                if (soreadable(so)) 
                    *revents |= POLLNORM;
                else
                    sbselqueue(&so->so_rcv);
#if __hpux1000p
		SOCKBUF_UNLOCK(&so->so_rcv);
#endif /* __hpux1000p */
        }

        if (events & POLLOUT) {
#if __hpux1000p
		SOCKBUF_LOCK(&so->so_snd);
#endif /* __hpux1000p */
                if (sowriteable(so))
                        *revents |= POLLOUT;
                else
                        sbselqueue(&so->so_snd);
#if __hpux1000p
		SOCKBUF_UNLOCK(&so->so_snd);
#endif /* __hpux1000p */
        }

        if (events & POLLPRI) {
                if (so->so_oobmark ||
                    (so->so_state & SS_RCVATMARK))
                        *revents |= POLLPRI;
                else {
#if __hpux1000p
			SOCKBUF_LOCK(&so->so_rcv);
                        sbselqueue(&so->so_rcv);
			SOCKBUF_UNLOCK(&so->so_rcv);
#else /* not __hpux1000p */
                        sbselqueue(&so->so_rcv);
#endif /* __hpux1000p */
                }
        }

#if __hpux1000p
	SOCKET_UNLOCK(so);
	NET_SPLX(oldlevel);	
#else /* not __hpux1000p */
	NETMP_GO_UNEXCLUSIVE(oldlevel, savestate);
	/*MPNET: MP protection is now off. */
#endif /* __hpux1000p */
	return (*revents);
}

#ifdef	UXKERN_NOTDEF
soo_select(fp, which)
	struct file *fp;
	int which;
{
	register struct socket *so = (struct socket *)fp->f_data;
	int oldlevel;

#if __hpux1000p
	NET_SPLNET(oldlevel);
	SOCKET_LOCK(so);
#else /* not __hpux1000p */
	sv_sema_t savestate;		/* MPNET: MP save state */
	/*MPNET: turn MP protection on */
	NETMP_GO_EXCLUSIVE(oldlevel, savestate);
#endif /* __hpux1000p */

	switch (which) {

	case FREAD:
#if __hpux1000p
		SOCKBUF_LOCK(&so->so_rcv);
		if (soreadable(so)) {
			SOCKBUF_UNLOCK(&so->so_rcv);
			SOCKET_UNLOCK(so);
			NET_SPLX(oldlevel);
			return (1);
		}
		sbselqueue(&so->so_rcv);
		SOCKBUF_UNLOCK(&so->so_rcv);
#else /* not __hpux1000p */
		if (soreadable(so)) {
			NETMP_GO_UNEXCLUSIVE(oldlevel, savestate);
			/*MPNET: MP protection is now off. */
			return (1);
		}
		sbselqueue(&so->so_rcv);
#endif /* __hpux1000p */
		break;

	case FWRITE:
#if __hpux1000p
		SOCKBUF_LOCK(&so->so_snd);
		if (sowriteable(so)) {
			SOCKBUF_UNLOCK(&so->so_snd);
			SOCKET_UNLOCK(so);
			NET_SPLX(oldlevel);
			return (1);
		}
		sbselqueue(&so->so_snd);
		SOCKBUF_UNLOCK(&so->so_snd);
#else /* not __hpux1000p */
		if (sowriteable(so)) {
			NETMP_GO_UNEXCLUSIVE(oldlevel, savestate);
			/*MPNET: MP protection is now off. */
			return (1);
		}
		sbselqueue(&so->so_snd);
#endif /* __hpux1000p */
		break;

	case 0:
		if (so->so_oobmark ||
		    (so->so_state & SS_RCVATMARK)) {
#if __hpux1000p
			SOCKET_UNLOCK(so);
			NET_SPLX(oldlevel);
#else /* not __hpux1000p */
			NETMP_GO_UNEXCLUSIVE(oldlevel, savestate);
			/*MPNET: MP protection is now off. */
#endif /* __hpux1000p */
			return (1);
		}
#if __hpux1000p
		SOCKBUF_LOCK(&so->so_rcv);
		sbselqueue(&so->so_rcv);
		SOCKBUF_UNLOCK(&so->so_rcv);
#else /* not __hpux1000p */
		sbselqueue(&so->so_rcv);
#endif /* __hpux1000p */
		break;
	}
#if __hpux1000p
	SOCKET_UNLOCK(so);
	NET_SPLX(oldlevel);	
#else /* not __hpux1000p */
	NETMP_GO_UNEXCLUSIVE(oldlevel, savestate);
	/*MPNET: MP protection is now off. */
#endif /* __hpux1000p */
	return (0);
}
#endif	/* UXKERN_NOTDEF */

/*
 * R P C _ _ S O C K E T _ N O W R I T E B L O C K _ W A I T
 *
 * Wait until the a write on the socket should succeed without
 * blocking.  If tmo is NULL, the wait is unbounded, otherwise
 * tmo specifies the max time to wait. rpc_c_socket_etimedout
 * if a timeout occurs.  This operation in not cancellable.
 */

PRIVATE rpc_socket_error_t rpc__socket_nowriteblock_wait
#ifdef _DCE_PROTO_
(
    rpc_socket_t so,
    struct timeval *tmo
)
#else
(so, tmo)
rpc_socket_t so;
struct timeval *tmo;
#endif
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
