/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_os2dfserr.c,v $
 * Revision 1.1.913.1  1996/10/02  17:57:45  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:34  damon]
 *
 * $EndLog$
 */

/*
 *      Copyright (C) 1996, 1992 Transarc Corporation
 *      All rights reserved.
 */


#include <osi_dfserrors.h>
#include <sys/errno.h>

unsigned char err_localToDFS[MAXKERNELERRORS+1];

/*
 * The routine is used to initialize the Error Mapping Table used by the DFS 
 * server to map a native (HP-UX) KERNEL error to a DFS kernel-independent
 * error. This routine is called during the DFS server initialization process. 
 *
 * Note that those kernel errors that are either very HP-UX specific or
 * should never occur to (or be visible to ) remote callers will be treated
 * as DFS_ENOTDEFINED during the conversion and will be ignored by the CM.
 */

void
osi_initEncodeTable()
{
    int i;

    for (i= 0; i <= MAXKERNELERRORS; i++)
	err_localToDFS[i] = DFS_ENOTDEFINED; /* not defined */

    err_localToDFS[0] 			= 0;
    err_localToDFS[EPERM]		= DFS_EPERM;
    err_localToDFS[ENOENT]		= DFS_ENOENT;
    err_localToDFS[ESRCH]		= DFS_ESRCH;
    err_localToDFS[EINTR]		= DFS_EINTR;
    err_localToDFS[EIO]			= DFS_EIO;
    err_localToDFS[ENXIO]		= DFS_ENXIO;
    err_localToDFS[E2BIG]		= DFS_E2BIG;
    err_localToDFS[ENOEXEC]		= DFS_ENOEXEC;
    err_localToDFS[EBADF]		= DFS_EBADF;
    err_localToDFS[ECHILD]		= DFS_ECHILD;
    err_localToDFS[EAGAIN]		= DFS_EAGAIN;
    err_localToDFS[ENOMEM]		= DFS_ENOMEM;
    err_localToDFS[EACCES]		= DFS_EACCES;
    err_localToDFS[EFAULT]		= DFS_EFAULT;
    err_localToDFS[EBUSY]		= DFS_EBUSY;
    err_localToDFS[EEXIST]		= DFS_EEXIST;
    err_localToDFS[EXDEV]		= DFS_EXDEV;
    err_localToDFS[ENODEV]		= DFS_ENODEV;
    err_localToDFS[ENOTDIR]		= DFS_ENOTDIR;
    err_localToDFS[EISDIR]		= DFS_EISDIR;
    err_localToDFS[EINVAL]		= DFS_EINVAL;
    err_localToDFS[ENFILE]		= DFS_ENFILE;
    err_localToDFS[EMFILE]		= DFS_EMFILE;
    err_localToDFS[ENOTTY]		= DFS_ENOTTY;
    err_localToDFS[EFBIG]		= DFS_EFBIG;
    err_localToDFS[ENOSPC]		= DFS_ENOSPC;
    err_localToDFS[ESPIPE]		= DFS_ESPIPE;
    err_localToDFS[EROFS]		= DFS_EROFS;
    err_localToDFS[EMLINK]		= DFS_EMLINK;
    err_localToDFS[EPIPE]		= DFS_EPIPE;
    err_localToDFS[EDOM]		= DFS_EDOM;
    err_localToDFS[ERANGE]		= DFS_ERANGE;
    err_localToDFS[EDEADLK]		= DFS_EDEADLK;
    err_localToDFS[ENOLCK]		= DFS_ENOLCK;
    err_localToDFS[ENOTEMPTY]		= DFS_ENOTEMPTY;
    err_localToDFS[ENAMETOOLONG]	= DFS_ENAMETOOLONG;
    err_localToDFS[ENOSYS]		= DFS_ENOSYS;
    err_localToDFS[ENOTBLK]		= DFS_ENOTBLK;
    err_localToDFS[ETXTBSY]		= DFS_ETXTBSY;
    err_localToDFS[ENOMSG]		= DFS_ENOMSG;
    err_localToDFS[EIDRM]		= DFS_EIDRM;
    err_localToDFS[EUSERS]		= DFS_EUSERS;
    err_localToDFS[EDQUOT]		= DFS_EDQUOT;
    err_localToDFS[ESTALE]		= DFS_ESTALE;
    err_localToDFS[EREMOTE]		= DFS_EREMOTE;
    err_localToDFS[ENOTSOCK]		= DFS_ENOTSOCK;
    err_localToDFS[EDESTADDRREQ]	= DFS_EDESTADDRREQ;
    err_localToDFS[EMSGSIZE]		= DFS_EMSGSIZE;
    err_localToDFS[EPROTOTYPE]		= DFS_EPROTOTYPE;
    err_localToDFS[ENOPROTOOPT]		= DFS_ENOPROTOOPT;
    err_localToDFS[EPROTONOSUPPORT]	= DFS_EPROTONOSUPPORT;
    err_localToDFS[ESOCKTNOSUPPORT]	= DFS_ESOCKTNOSUPPORT;
    err_localToDFS[EOPNOTSUPP]		= DFS_EOPNOTSUPP;
    err_localToDFS[EPFNOSUPPORT]	= DFS_EPFNOSUPPORT;
    err_localToDFS[EAFNOSUPPORT]	= DFS_EAFNOSUPPORT;
    err_localToDFS[EADDRINUSE]		= DFS_EADDRINUSE;
    err_localToDFS[EADDRNOTAVAIL]	= DFS_EADDRNOTAVAIL;
    err_localToDFS[ENETDOWN]		= DFS_ENETDOWN;
    err_localToDFS[ENETUNREACH]		= DFS_ENETUNREACH;
    err_localToDFS[ENETRESET]		= DFS_ENETRESET;
    err_localToDFS[ECONNABORTED]	= DFS_ECONNABORTED;
    err_localToDFS[ECONNRESET]		= DFS_ECONNRESET;
    err_localToDFS[ENOBUFS]		= DFS_ENOBUFS;
    err_localToDFS[EISCONN]		= DFS_EISCONN;
    err_localToDFS[ENOTCONN]		= DFS_ENOTCONN;
    err_localToDFS[ESHUTDOWN]		= DFS_ESHUTDOWN;
    err_localToDFS[ETIMEDOUT]		= DFS_ETIMEDOUT;
    err_localToDFS[ECONNREFUSED]	= DFS_ECONNREFUSED;
    err_localToDFS[EHOSTDOWN]		= DFS_EHOSTDOWN;
    err_localToDFS[EHOSTUNREACH]	= DFS_EHOSTUNREACH;
    err_localToDFS[EALREADY]		= DFS_EALREADY;
    err_localToDFS[EINPROGRESS]		= DFS_EINPROGRESS;
    err_localToDFS[EWOULDBLOCK]		= DFS_EWOULDBLOCK;
    err_localToDFS[ELOOP]		= DFS_ELOOP;
/*
 * 64/32 bit interoperability
 */
    err_localToDFS[EOVERFLOW]		= DFS_EOVERFLOW;

#if 0
/* uniq to hpux - not translated */
    err_localToDFS[ECHRNG]		= DFS_ECHRNG;
    err_localToDFS[EL2NSYNC]		= DFS_EL2NSYNC;
    err_localToDFS[EL3HLT]		= DFS_EL3HLT;
    err_localToDFS[EL3RST]		= DFS_EL3RST;
    err_localToDFS[ELNRNG]		= DFS_ELNRNG;
    err_localToDFS[EUNATCH]		= DFS_EUNATCH;
    err_localToDFS[ENOCSI]		= DFS_ENOCSI;
    err_localToDFS[EL2HLT]		= DFS_EL2HLT;
    err_localToDFS[ENOSTR]		= DFS_ENOSTR;
    err_localToDFS[ENODATA]		= DFS_ENODATA;
    err_localToDFS[ETIME]		= DFS_ETIME;
    err_localToDFS[ENOSR]		= DFS_ENOSR;
    err_localToDFS[ENONET]		= DFS_ENONET;
    err_localToDFS[ENOPKG]		= DFS_ENOPKG;
    err_localToDFS[ENOLINK]		= DFS_ENOLINK;
    err_localToDFS[EADV]		= DFS_EADV;
    err_localToDFS[ESRMNT]		= DFS_ESRMNT;
    err_localToDFS[ECOMM]		= DFS_ECOMM;
    err_localToDFS[EPROTO]		= DFS_EPROTO;
    err_localToDFS[EMULTIHOP]		= DFS_EMULTIHOP;
    err_localToDFS[EDOTDOT]		= DFS_EDOTDOT;
    err_localToDFS[EBADMSG]		= DFS_EBADMSG;
    err_localToDFS[ENOSYM]		= DFS_ENOSYM;
#endif /* 0 */

}


