/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1989, 1994 Transarc Corporation - All rights reserved */

/*
 * HISTORY
 * $Log: osi_fio.c,v $
 * Revision 1.1.977.1  1996/10/02  17:58:20  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:54  damon]
 *
 * Revision 1.1.972.2  1994/06/09  14:15:35  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:05  annie]
 * 
 * Revision 1.1.972.1  1994/02/04  20:24:46  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:44  devsrc]
 * 
 * Revision 1.1.970.1  1993/12/07  17:29:56  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:01:50  jaffe]
 * 
 * Revision 1.1.5.7  1993/01/21  14:50:29  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:52:33  cjd]
 * 
 * Revision 1.1.5.6  1992/11/24  18:22:19  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:13:12  bolinger]
 * 
 * Revision 1.1.5.5  1992/11/11  19:42:02  bolinger
 * 	Fix OT defect 5767:  move osi_credp initialization from here to
 * 	file/libafs/OSF1/cm_config.c\cm_configure(), to ensure that
 * 	it precedes all possible uses of the osi_credp.
 * 	[1992/11/06  16:55:20  bolinger]
 * 
 * Revision 1.1.5.4  1992/10/13  18:11:52  marty
 * 	OSC1.1.1 upgrade.  Changes are osc1.0.4 compatible
 * 	[1992/10/13  17:11:58  marty]
 * 
 * Revision 1.1.6.2  1992/10/02  19:02:45  garyf
 * 	OSC1.1 updates
 * 	[1992/09/29  20:07:47  garyf]
 * 
 * Revision 1.1.5.3  1992/09/25  18:32:24  jaffe
 * 	Cleanup Minor header differences
 * 	[1992/09/24  15:43:26  jaffe]
 * 
 * Revision 1.1.5.2  1992/08/31  20:24:44  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    osi_Stat, osi_Close, osi_Truncate, osi_Read, and osi_Write all return
 * 	    an int.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 	[1992/08/30  03:16:01  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/05  04:49:54  mason
 * 	OT 2497
 * 	[1992/05/05  04:49:11  mason]
 * 
 * $EndLog$
 */
#include <dcedfs/param.h>	/* Should be always first */

#include <dcedfs/sysincludes.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/lock.h>

osi_cred_t *osi_credp;

struct vattr tvattr;		/* Holds file's attributes */
struct lock_data osi_xosi;	/* lock is for tvattr */
static long osi_fioinit = 0;

/* 
 * Generic local file system Open interface 
 */
struct osi_file *osi_Open(mp, handle)
    struct mount *mp;
    struct fid *handle;
{
    register struct osi_file *afile;
    int error;
    struct vnode *vp = (struct vnode *)0;

    if (!osi_fioinit) {
	lock_Init(&osi_xosi);
	osi_fioinit = 1;
    }
    afile = (struct osi_file *) osi_Alloc(sizeof(struct osi_file));
    error = 0;
    VFS_FHTOVP(mp, handle, &vp, error);
    if (error)
	    return (struct osi_file *) 0;

    afile->vnode = vp;
    afile->offset = 0;
    afile->proc = (int (*)()) 0;
    return afile;
}


/* 
 * Generic local file system Stat interface 
 */
int osi_Stat(afile, astat)
    register struct osi_file *afile;
    register struct osi_stat *astat; 
{
    long code;

    lock_ObtainWrite(&osi_xosi);
    VOP_GETATTR(afile->vnode, &tvattr, osi_credp, code);
    if (code == 0) {
	astat->size = tvattr.va_size;
	astat->blocksize = tvattr.va_blocksize;
	astat->mtime = tvattr.va_mtime.tv_sec;
	astat->atime = tvattr.va_atime.tv_sec;
    }
    lock_ReleaseWrite(&osi_xosi);
    return code;
}


/* 
 * Generic local file system Close interface 
 */
int osi_Close(afile)
    register struct osi_file *afile; 
{
    VN_RELE(afile->vnode);
    osi_Free((char *)afile, sizeof(struct osi_file));
    return 0;
}


/* 
 * Generic local file system truncate interface 
 */
int osi_Truncate(afile, asize)
    register struct osi_file *afile;
    long asize; 
{
    osi_cred_t *oldCred;
    long code;

    lock_ObtainWrite(&osi_xosi);
    osi_vattr_null(&tvattr);
    tvattr.va_size = asize;
    oldCred = u.u_cred;			/* remember old credentials pointer  */
    u.u_cred = osi_credp;		/* temporarily use superuser credentials */
    VOP_SETATTR(afile->vnode, &tvattr, osi_credp, code);
    u.u_cred = oldCred;			/* restore */
    lock_ReleaseWrite(&osi_xosi);
    return code;
}

/* 
 * Generic local file system read interface 
 */
int osi_Read(afile, aptr, asize)
    register struct osi_file *afile;
    char *aptr;
    long asize; 
{
    unsigned int resid;
    register long code;

    code = osi_rdwr(UIO_READ, afile->vnode, (caddr_t)aptr, asize, afile->offset,
		  OSI_UIOSYS, IO_UNIT, u.u_cred, &resid);
    if (code == 0) {
	code = asize - resid;
	afile->offset += code;
    }
    return code;
}


/* 
 * Generic local file system write interface 
 */
int osi_Write(afile, aptr, asize)
    register struct osi_file *afile;
    char *aptr;
    long asize; 
{
    unsigned int resid;
    register long code;

    code = osi_rdwr(UIO_WRITE, afile->vnode, (caddr_t)aptr, asize, afile->offset,
		  OSI_UIOSYS, IO_UNIT, u.u_cred, &resid);
    if (code == 0) {
	code = asize - resid;
	afile->offset += code;
    }
    if (afile->proc) {
	(*afile->proc)(afile, code);
    }
    return code;
}


/*
 * Generic local file system RDWR interface
 */
osi_RDWR(afile, arw, auio)
    struct osi_file *afile;
    enum uio_rw arw;
    struct uio *auio;
{
    int code;
    auio->uio_rw = arw;
    if (arw == UIO_WRITE) {
	VOP_WRITE(afile->vnode, auio, 0, osi_credp, code);
    } else /* UIO_READ */ {
	VOP_READ(afile->vnode, auio, 0, osi_credp, code);
    }
    return code;
}


/* 
 * osi_MapStrategy -- call strategy function, but with buffer's data
 *                    temporarily made accessible.
 */
long osi_MapStrategy(aproc, bp)
	int (*aproc)();
	register struct buf *bp;
{
	long returnCode;

	returnCode = (*aproc) (bp);
	return returnCode;
}

