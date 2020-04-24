/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1989-1994 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>	/* Should be always first */

#include <dcedfs/sysincludes.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/osi_cred.h>
#include <dcedfs/lock.h>

#include <sys/vfs.h>
#include <sys/vattr.h>
#include <sys/user.h>
#include <sys/buf.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/osi/RIOS/RCS/osi_fio.c,v 1.29 1995/03/09 22:26:47 andi Exp $")

osi_cred_t osi_cred;		/* Credential struct used when calling UFS layer */
osi_cred_t *osi_credp;
struct vattr tvattr;		/* Holds file's attributes */
struct lock_data osi_xosi;	/* lock is for tvattr */
static long osi_fioinit = 0;

/*
 * Generic local file system Open interface
 */
struct osi_file *osi_Open(vfsp, fhandle)
    register struct vfs *vfsp;
    struct fid *fhandle;
{
    register struct osi_file *afile;
    int rc;

    if (!osi_fioinit) {
	lock_Init(&osi_xosi);
	osi_fioinit = 1;
    }
    afile = (struct osi_file *) osi_Alloc(sizeof(struct osi_file));
    if (rc = vfstovfsop(vfsp, vfs_vget)(vfsp, &afile->vnode, fhandle)) {
	osi_Free((char *)afile, sizeof(struct osi_file));
	osi_setuerror(rc);
	return (struct osi_file *) 0;
    }
    afile->offset = 0;
    afile->proc = (int (*)()) 0;
    return afile;
}

/*
 * Generic local file system Stat interface
 */
osi_Stat(afile, astat)
    register struct osi_file *afile;
    register struct osi_stat *astat;
{
    register long code;

    lock_ObtainWrite(&osi_xosi);
    code = vtognop(afile->vnode,vn_getattr)(afile->vnode, &tvattr);
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
osi_Close(afile)
    register struct osi_file *afile;
{
    VN_RELE(afile->vnode);
    osi_Free((char *)afile, sizeof(struct osi_file));
    return 0;
}


/*
 * Generic local file system truncate interface
 */
osi_Truncate(afile, asize)
    register struct osi_file *afile;
    long asize;
{
    osi_cred_t *oldCred;
    register long code;

    lock_ObtainWrite(&osi_xosi);
    code = vtognop(afile->vnode,vn_ftrunc)(afile->vnode, FWRITE|FSYNC,
	asize, (caddr_t)0);
    lock_ReleaseWrite(&osi_xosi);
    return code;
}

/*
 * Generic local file system read interface
 */
osi_Read(afile, aptr, asize)
    register struct osi_file *afile;
    char *aptr;
    long asize;
{
    int resid;
    int code;

    /*
     * Note the difference in the way the afile->offset is passed
     * (see comments in  osi_rdwr() in osi_port_aix.c)
     */
    code = osi_rdwr(UIO_READ, afile->vnode, (caddr_t)aptr, asize,
	&afile->offset, OSI_UIOSYS, 0, &resid);
    if (code == 0) {
	code = asize - resid;
	afile->offset += code;
    } else {
	osi_setuerror(code);
	code = -1;
    }
    return code;
}


/*
 * Generic local file system write interface
 */
osi_Write(afile, aptr, asize)
    register struct osi_file *afile;
    char *aptr;
    long asize;
{
    int resid;
    long code;

    /*
     * Note the difference in the way the afile->offset is passed
     * (see comments in  osi_rdwr() in osi_port_aix.c)
     */
    code = osi_rdwr(UIO_WRITE, afile->vnode, (caddr_t)aptr, asize,
	&afile->offset, OSI_UIOSYS, 0, &resid);
    if (code == 0) {
	code = asize - resid;
	afile->offset += code;
    } else {
	osi_setuerror(code);
	code = -1;
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
    long flag = (arw == UIO_WRITE ? FWRITE : FREAD);

    return vtognop(afile->vnode,vn_rdwr)(afile->vnode, arw, flag, auio, 0, 0);
}

#include <sys/adspace.h>

/*
 * osi_MapStrategy -- call strategy function, but with buffer's data
 *                    temporarily made accessible.
 */
long osi_MapStrategy(aproc, bp)
	int (*aproc)();
	register struct buf *bp;
{
	long returnCode;
	caddr_t oldaddr;
	oldaddr = bp->b_un.b_addr;
	bp->b_un.b_addr = vm_att (bp->b_xmemd.subspace_id, oldaddr);

	returnCode = (*aproc) (bp);

	vm_det (bp->b_un.b_addr);
	bp->b_un.b_addr = oldaddr;

	return returnCode;
}
