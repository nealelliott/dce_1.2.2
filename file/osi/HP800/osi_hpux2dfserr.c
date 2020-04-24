/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_hpux2dfserr.c,v $
 * Revision 1.1.41.1  1996/10/02  17:57:03  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:18  damon]
 *
 * Revision 1.1.36.2  1994/07/13  22:18:52  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  17:39:26  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:00:20  mbs]
 * 
 * Revision 1.1.36.1  1994/06/09  14:14:47  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:27:28  annie]
 * 
 * Revision 1.1.2.4  1993/01/21  14:50:05  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:51:55  cjd]
 * 
 * Revision 1.1.2.3  1992/09/25  18:32:07  jaffe
 * 	Remove duplicate HEADER and LOG entries
 * 	[1992/09/25  12:27:36  jaffe]
 * 
 * Revision 1.1.2.2  1992/08/31  20:23:29  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    make osi_initEncodeTable return void.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 	[1992/08/30  03:12:08  jaffe]
 * 
 * Revision 1.1.1.2  1992/08/30  03:12:08  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    make osi_initEncodeTable return void.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 
 * 	$TALog: osi_hpux2dfserr.c,v $
 * 	Revision 1.4  1994/11/01  21:53:53  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[added by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.2  1992/07/29  21:35:45  jaffe
 * 	Fixed many compiler warnings in the osi directory.
 * 	Reworked ALL of the header files.  All files in the osi directory now
 * 	have NO machine specific ifdefs.  All machine specific code is in the
 * 	machine specific subdirectories.  To make this work, additional flags
 * 	were added to the afs/param.h file so that we can tell if a particular
 * 	platform has any additional changes for a given osi header file.
 * 
 * 	make osi_initEncodeTable return void.
 * 	[from revision 1.1 by delta jaffe-ot4719-cleanup-gcc-Wall-in-osi, revision 1.1]
 * 
 * Revision 1.1  1992/06/06  23:38:45  mason
 * 	Really a bit misnamed - this delta adds code necessary for HP-UX; it isn't
 * 	sufficient for a complete port.
 * 
 * 	HP-UX error conversion routines.
 * 	[added by delta mason-add-hp800-osi-routines, revision 1.1]
 * 
 * $EndLog$
 */

/*
 *      Copyright (C) 1992 Transarc Corporation
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

void osi_initEncodeTable()
{
    int i;

    for (i= 0; i <= MAXKERNELERRORS; i++)
	err_localToDFS[i] = DFS_ENOTDEFINED; /* not defined */

    err_localToDFS[0] 			= 0;
    err_localToDFS[EPERM]		= DFS_EPERM;
    err_localToDFS[ENOENT]		= DFS_ENOENT;
    err_localToDFS[ESRCH]		= DFS_ESRCH;
    err_localToDFS[EINTR]		= DFS_EINTR;
    err_localToDFS[EIO]		= DFS_EIO;
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
    err_localToDFS[EDEADLK]		= DFS_EDEADLK;
    err_localToDFS[ENOLCK]		= DFS_ENOLCK;
    err_localToDFS[ENOTEMPTY]		= DFS_ENOTEMPTY;
    err_localToDFS[ENAMETOOLONG]		= DFS_ENAMETOOLONG;
    err_localToDFS[ENOSYS]		= DFS_ENOSYS;
    err_localToDFS[ENOTBLK]		= DFS_ENOTBLK;
    err_localToDFS[ETXTBSY]		= DFS_ETXTBSY;
    err_localToDFS[ENOMSG]		= DFS_ENOMSG;
    err_localToDFS[EIDRM]		= DFS_EIDRM;
    err_localToDFS[EALREADY]		= DFS_EALREADY;
    err_localToDFS[EINPROGRESS]		= DFS_EINPROGRESS;
    err_localToDFS[EWOULDBLOCK]		= DFS_EWOULDBLOCK;
    err_localToDFS[ELOOP]		= DFS_ELOOP;

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


