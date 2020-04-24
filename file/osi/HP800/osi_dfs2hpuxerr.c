/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_dfs2hpuxerr.c,v $
 * Revision 1.1.38.1  1996/10/02  17:57:01  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:17  damon]
 *
 * Revision 1.1.33.1  1994/06/09  14:14:44  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:27:27  annie]
 * 
 * Revision 1.1.2.4  1993/01/21  14:50:03  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:51:50  cjd]
 * 
 * Revision 1.1.2.3  1992/09/25  18:32:05  jaffe
 * 	Cleanup Minor header differences
 * 	[1992/09/24  15:43:20  jaffe]
 * 
 * Revision 1.1.2.2  1992/08/31  20:23:20  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    make osi_initDecodeTable return void.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 	[1992/08/30  03:11:50  jaffe]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1992 Transarc Corporation
 * All Rights Reserved
 */

#include <osi_dfserrors.h>
#include <sys/errno.h>

unsigned char err_DFSToLocal[MAXKERNELERRORS+1];

/*
 * This routine is used to initialize the Error Mapping Table which is used by
 * the CM to perform the decoding work. This routine is invoked when when the
 * CM is initialized.
 */
void osi_initDecodeTable()
{
    int i;
    for (i= 0; i <= MAXKERNELERRORS; i++)
	err_DFSToLocal[i] = DFS_ENOTDEFINED; /* not defined */

    err_DFSToLocal[DFS_ESUCCESS] 	= 0;
    err_DFSToLocal[DFS_EPERM]		= EPERM;
    err_DFSToLocal[DFS_ENOENT]		= ENOENT;
    err_DFSToLocal[DFS_ESRCH]		= ESRCH;
    err_DFSToLocal[DFS_EINTR]		= EINTR;
    err_DFSToLocal[DFS_EIO]		= EIO;
    err_DFSToLocal[DFS_ENXIO]		= ENXIO;
    err_DFSToLocal[DFS_E2BIG]		= E2BIG;
    err_DFSToLocal[DFS_ENOEXEC]		= ENOEXEC;
    err_DFSToLocal[DFS_EBADF]		= EBADF;
    err_DFSToLocal[DFS_ECHILD]		= ECHILD;
    err_DFSToLocal[DFS_EDEADLK]		= EDEADLK;
    err_DFSToLocal[DFS_ENOMEM]		= ENOMEM;
    err_DFSToLocal[DFS_EACCES]		= EACCES;
    err_DFSToLocal[DFS_EFAULT]		= EFAULT;
    err_DFSToLocal[DFS_ENOTBLK]		= ENOTBLK;
    err_DFSToLocal[DFS_EBUSY]		= EBUSY;
    err_DFSToLocal[DFS_EEXIST]		= EEXIST;
    err_DFSToLocal[DFS_EXDEV]		= EXDEV;
    err_DFSToLocal[DFS_ENODEV]		= ENODEV;
    err_DFSToLocal[DFS_ENOTDIR]		= ENOTDIR;
    err_DFSToLocal[DFS_EISDIR]		= EISDIR;
    err_DFSToLocal[DFS_EINVAL]		= EINVAL;
    err_DFSToLocal[DFS_ENFILE]		= ENFILE;
    err_DFSToLocal[DFS_EMFILE]		= EMFILE;
    err_DFSToLocal[DFS_ENOTTY]		= ENOTTY;
    err_DFSToLocal[DFS_ETXTBSY]		= ETXTBSY;    
    err_DFSToLocal[DFS_EFBIG]		= EFBIG;
    err_DFSToLocal[DFS_ENOSPC]		= ENOSPC;	
    err_DFSToLocal[DFS_ESPIPE]		= ESPIPE;	
    err_DFSToLocal[DFS_EROFS]		= EROFS;	
    err_DFSToLocal[DFS_EMLINK]		= EMLINK;	
    err_DFSToLocal[DFS_EPIPE]		= EPIPE;
    err_DFSToLocal[DFS_EDOM]		= EDOM;	
    err_DFSToLocal[DFS_ERANGE]		= ERANGE;	
    err_DFSToLocal[DFS_EWOULDBLOCK]	= EWOULDBLOCK;
    err_DFSToLocal[DFS_EINPROGRESS]	= EINPROGRESS;
    err_DFSToLocal[DFS_EALREADY]	= EALREADY;
/* 
 * ipc/network software
 */
    err_DFSToLocal[DFS_ENOTSOCK]	= ENOTSOCK;
    err_DFSToLocal[DFS_EDESTADDRREQ]	= EDESTADDRREQ;
    err_DFSToLocal[DFS_EMSGSIZE]	= EMSGSIZE;
    err_DFSToLocal[DFS_EPROTOTYPE]	= EPROTOTYPE;
    err_DFSToLocal[DFS_ENOPROTOOPT]	= ENOPROTOOPT;
    err_DFSToLocal[DFS_EPROTONOSUPPORT]	= EPROTONOSUPPORT;
    err_DFSToLocal[DFS_ESOCKTNOSUPPORT]	= ESOCKTNOSUPPORT;
    err_DFSToLocal[DFS_EOPNOTSUPP]	= EOPNOTSUPP;
    err_DFSToLocal[DFS_EPFNOSUPPORT]	= EPFNOSUPPORT;	
    err_DFSToLocal[DFS_EAFNOSUPPORT]	= EAFNOSUPPORT;
    err_DFSToLocal[DFS_EADDRINUSE]	= EADDRINUSE;
    err_DFSToLocal[DFS_EADDRNOTAVAIL]	= EADDRNOTAVAIL;
    err_DFSToLocal[DFS_ENETDOWN]	= ENETDOWN;
    err_DFSToLocal[DFS_ENETUNREACH]	= ENETUNREACH;	
    err_DFSToLocal[DFS_ENETRESET]	= ENETRESET;
    err_DFSToLocal[DFS_ECONNABORTED]	= ECONNABORTED;
    err_DFSToLocal[DFS_ECONNRESET]	= ECONNRESET;
    err_DFSToLocal[DFS_ENOBUFS]		= ENOBUFS;
    err_DFSToLocal[DFS_EISCONN]		= EISCONN;
    err_DFSToLocal[DFS_ENOTCONN]	= ENOTCONN;
    err_DFSToLocal[DFS_ESHUTDOWN]	= ESHUTDOWN;
    err_DFSToLocal[DFS_ETIMEDOUT]	= ETIMEDOUT;
    err_DFSToLocal[DFS_ECONNREFUSED]	= ECONNREFUSED;
    err_DFSToLocal[DFS_ELOOP]		= ELOOP;
    err_DFSToLocal[DFS_ENAMETOOLONG]	= ENAMETOOLONG;
    err_DFSToLocal[DFS_EHOSTDOWN]	= EHOSTDOWN;
    err_DFSToLocal[DFS_EHOSTUNREACH]	= EHOSTUNREACH;
    err_DFSToLocal[DFS_ENOTEMPTY]	= ENOTEMPTY;
/* 
 * disk quotas and limits
 */
    err_DFSToLocal[DFS_EUSERS]		= EUSERS;
    err_DFSToLocal[DFS_EDQUOT]		= EDQUOT;
/*
 *  NFS errors.
 */
    err_DFSToLocal[DFS_ESTALE]		= ESTALE;
    err_DFSToLocal[DFS_EREMOTE]		= EREMOTE;
/*
 * misc ..
 */
    err_DFSToLocal[DFS_ENOLCK]		= ENOLCK;
    err_DFSToLocal[DFS_ENOSYS]		= ENOSYS;
    err_DFSToLocal[DFS_EAGAIN]		= EAGAIN;		
/* 
 * SYS V IPC errors 
 */
    err_DFSToLocal[DFS_ENOMSG]		= ENOMSG;
    err_DFSToLocal[DFS_EIDRM]		= EIDRM;
}

