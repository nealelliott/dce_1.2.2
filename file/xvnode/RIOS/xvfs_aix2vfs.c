/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

/* 
 * This module is the glue between the aix gnode layer and afs. 
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/osi_cred.h>
#include <dcedfs/stds.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/debug.h>
#include <sys/user.h>
#include <sys/chownx.h>
#include <sys/access.h>
#include <sys/flock.h>
#include <sys/inode.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/xvnode/RIOS/RCS/xvfs_aix2vfs.c,v 4.43 1996/02/22 21:09:45 cfe Exp $")

/*
 * This module is a glue layer used in locally mounted Episode under AIX 3.
 * It consists of functions that meet the specs of the AIX 3 vnode interface,
 * and that call the X-ops.  (The X-op interface is based on an old version
 * of the SunOS vnode interface.)
 */


EXPORT int naix_link(vp, dp, namep)
  struct vnode *vp;
  struct vnode *dp;
  char *namep;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_link (namep=%s) = ", namep));
    error = VOPX_LINK(vp, dp, namep, u.u_cred);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_mkdir(dp, namep, mode)
    struct vnode *dp;
    char *namep;
    int mode;
{
    struct vattr va;
    struct vnode *vp;
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_mkdir (namep=%s mode=%x) = ", namep, mode));
    osi_vattr_init(&va, OSI_VA_TYPE | OSI_VA_MODE);
    va.va_type = VDIR;
    va.va_mode = (mode & 07777);
    error = VOPX_MKDIR(dp, namep, &va, &vp, u.u_cred);
    if (!error)
	VN_RELE(vp);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_mknod(dp, namep, mode, dev)
    struct vnode *dp;
    char *namep;
    int mode;
    dev_t dev;
{
    struct vattr va;
    struct vnode *vp;
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_mknod (namep=%s, mode=%x) = ",  namep, mode));
    osi_vattr_init(&va, OSI_VA_TYPE | OSI_VA_MODE);
    va.va_type = IFTOVT(mode);
    va.va_mode = (mode & 07777) & ~u.u_cmask;

    /**** 
     * I'm not sure if suser() should stay here since it makes no sense in AFS;
     * however the documentation says that one "should be super-user unless
     * making a FIFO file. Others systems such as SUN do this checking in the
     * early stages of mknod (before the abstraction), so it's equivalently
     * the same! 
     *****/
    if (va.va_type != VFIFO && va.va_type != VSOCK && !suser(&error))
	return(EPERM);
    switch (va.va_type) {
	case VDIR:
	    error = EISDIR;
	    break;
	case VNON:
	    error = EINVAL;
	    break;
	case VBAD:
	case VCHR:
	case VBLK:
	    va.va_rdev = dev;
	default:
	    error = VOPX_CREATE(dp, namep, &va, NONEXCL, mode, &vp, u.u_cred);
    }
    if (! error)
	VN_RELE(vp);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return(error);
}


EXPORT int naix_remove(vp, dp, namep)
    struct vnode *vp, *dp;
    char *namep;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_remove (dp=0x%x, namep=%s) = ", dp, namep));
    error = VOPX_REMOVE(dp, namep, u.u_cred);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_rename(vp, dp, namep, tp, tdp, tnamep)
    struct vnode *dp;
    char *namep;
    struct vnode *vp;	    /* Ignored in AFS */
    struct vnode *tp;	    /* Ignored in AFS */
    struct vnode *tdp;
    char *tnamep;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_rename (fnamep=%s, tnamep=%s) = ", namep, tnamep));
    error = VOPX_RENAME(dp, namep, tdp, tnamep, u.u_cred);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_rmdir(vp, dp, namep)
    struct vnode *vp;	    /* Ignored in AFS */
    struct vnode *dp;
    char *namep;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_rmdir (namep=%s) = ", namep));
    error = VOPX_RMDIR(dp, namep, (struct vnode *) 0, u.u_cred);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


/* 
 * The caller (in lookuppn.c) also passes a fourth parameter, flags, which
 * isn't present here. I Guess, a typo? It's not required for afs but it
 * should still be in the parameter list. 
 */
EXPORT int naix_lookup(dp, vpp, namep, flags)
    struct vnode *dp;
    struct vnode **vpp;
    char *namep;
    int flags;			/* Ignored by AFS */
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_lookup (namep=%s) = ", namep));

    /*
     * The next statement is for the benefit of the AIX 3.1 NFS server, which
     * expects *vpp to be defined (and null) even if the error code is non-zero.
     */
    *vpp = 0;

    error = VOPX_LOOKUP(dp, namep, vpp, u.u_cred);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT naix_fid(vp, fidp)
    struct vnode *vp;
    struct fid *fidp;
{
    VOPX_FID(vp, fidp);
    return(0);
}


EXPORT int naix_open(vp, flags, ext, vinfopp)
    struct vnode *vp;
    int flags;
    int ext;			/* Ignored in AFS */
    osi_cred_t **vinfopp;	/* ptr for fp->f_vinfo, used as fp->f_cred */
{
    int error;
    struct vattr va;
    int mode = 0;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_open (flags=0x%x) = ", flags));

    if (flags & FREAD)
	mode |= R_ACC;
    if (flags & (FWRITE|FTRUNC)) {
	if (OSI_ISDIR (vp))
	    return EISDIR;
	mode |= W_ACC;
    }
    if (flags & FEXEC) {
	if (flags & FTRUNC)
	    return EINVAL;
	if (!OSI_ISREG (vp))
	    return EACCES;
	mode = X_ACC;
    }

    error = VOPX_ACCESS (vp, mode << 6, u.u_cred);
    if (!error)
	error = VOPX_OPEN (&vp, flags, u.u_cred);

    if (!error) {
	if (flags & FTRUNC) {
	    osi_vattr_init(&va, OSI_VA_SIZE);
	    va.va_size = 0;
	    error = VOPX_SETATTR(vp, (struct xvfs_attr *)&va, 0, u.u_cred);
	}
    }
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_create(dp, vpp, flags, namep, mode, vinfopp)
    struct vnode *dp;
    struct vnode **vpp;
    int flags;
    char *namep;
    int mode;
    osi_cred_t **vinfopp; /* ptr for fp->f_vinfo, used as fp->f_cred */
{
    struct vattr va;
    enum vcexcl exclusive;
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_create (namep=%s, flags=0x%x,m ode=0x%x) = ", 
	   namep, flags, mode));
    if ((flags & (O_EXCL|O_CREAT)) == (O_EXCL|O_CREAT))
	exclusive = EXCL;
    else	
	exclusive = NONEXCL;
    osi_vattr_init(&va, OSI_VA_TYPE | OSI_VA_MODE);
    va.va_type = VREG;
    va.va_mode = (mode & 07777);
    error = VOPX_CREATE(dp, namep, &va, exclusive, mode, vpp, u.u_cred);
    if (error) {
	AFSLOG(XVFS_DEBUG, 1, ("naix_create failed=%d\n", error));
	return error;
    }
    /* 
     * 'cr_luid' is a flag (when it comes thru the NFS server it's set to
     * RMTUSER_REQ) that determines if we should call cm_open(). We shouldn't
     * call it when this NFS traffic since the close will never happen thus
     * we'd never flush the files out to the server! Gross but the simplest
     * solution we came out with 
     */
    if (u.u_cred->cr_luid != RMTUSER_REQ) {
	/* 
	 * Since in the standard copen() for bsd vnode kernels they do an
	 * vop_open after the vop_create, we must do the open here since there
	 * are stuff in cm_open that we need. For example advance the
	 * execsOrWriters flag (else we'll be treated as the sun's "core" case).
	 */
	*vinfopp = u.u_cred;		/* save user creds in fp->f_vinfo */
	if (IS_CONVERTED (*vpp))
	    error = VOPX_OPEN(vpp, flags, u.u_cred);
	else
	    /*
	     * Mustn't call VOPX_OPEN because the vnode doesn't have X-ops.
	     * This can arise if (1) there is a non-exclusive create, and
	     * (2) there was an existing file, and (3) the file was a FIFO
	     * or other special vnode that doesn't have X-ops.
	     */
	    error = VOPO_OPEN(*vpp, flags, 0, vinfopp);
    }
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_hold(vp)
    struct vnode *vp;
{
    AFSLOG(XVFS_DEBUG, 1, ("In naix_hold\n"));
    return VOPX_HOLD(vp);
}


EXPORT int naix_rele(vp)
    struct vnode *vp;
{
    AFSLOG(XVFS_DEBUG, 1, ("In naix_rele(vp=%x, cnt=%d)\n", vp, vp->v_count));
    return VOPX_RELE(vp);
}


EXPORT int naix_close(vp, flags, channel, ext, vinfo)
    struct vnode *vp;
    int flags;
    int channel;		/* Ignored in AFS */
    caddr_t ext;		/* Ignored in AFS */
    caddr_t vinfo;		/* Ignored in AFS */
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_close (flags=0x%x) = ", flags));
    error = VOPX_CLOSE(vp, flags, u.u_cred);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_map(vp, addr, length, offset, flag)
    u_int length, offset, flag;
    struct vnode *vp;
    caddr_t addr;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_map\n"));
    error = VOPX_MAP(vp, addr, length, offset, flag);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_unmap(vp, vinfo)
    struct vnode *vp;
    int vinfo;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_unmap\n"));
    error = VOPX_UNMAP(vp, vinfo);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_access(vp, mode, who)
    struct vnode *vp;
    int mode;
    int who; 
{
    int	error;
    struct vattr va;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_access (mode=0x%x) = ", mode));
    switch (who) {
	case ACC_SELF:
	    error = VOPX_ACCESS (vp, mode << 6, u.u_cred);
	    break;
	case ACC_OTHERS:
	case ACC_ALL:
	    error = ENOSYS;
	    break;
	case ACC_ANY:
	    /*
	     * Test if the file grants the permission to anbody (owner, group,
	     * or others), ignoring privileges.
	     */
	    /* Permission must be a single bit */
	    if (mode != R_ACC && mode != W_ACC && mode != X_ACC) {
		error = EINVAL;
		break;
	    }
	    error = VOPX_GETATTR (vp, (struct xvfs_attr *)&va, 0, u.u_cred);
	    if (error)
		break;
	    /* Mask all three classes at once *
	     * Perhaps (mode * 0111) would be faster on RIOS? */
	    if ((mode + (mode << 3) + (mode << 6)) & va.va_mode)
		error = 0;
	    else
		error = EACCES;
	    break;
	default:
	    error = EINVAL;
    }
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_getattr(vp, vattrp)
    struct vnode *vp;
    struct vattr *vattrp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_getattr (vattrp=0x%x) = ", vattrp));
    error = VOPX_GETATTR(vp, (struct xvfs_attr *)vattrp, 0, u.u_cred);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_setattr(vp, op, arg1, arg2, arg3)
    struct vnode *vp;
    int op;
    int arg1;
    int arg2;
    int arg3;
{
    struct vattr va;
    int error = 0;
    int flag = 0;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_setattr (op = %d, args = %o, %d, %d)\n", 
	   op, arg1, arg2, arg3));
    osi_vattr_init(&va, 0);
    switch(op) {
	/* 
	 * change mode 
	 */
	case V_MODE:
	    osi_vattr_add(&va, OSI_VA_MODE);
	    va.va_mode = (arg1 & 07777);
	    break;
	case V_OWN:
	    if ((arg1 & T_OWNER_AS_IS) == 0) {
		osi_vattr_add(&va, OSI_VA_UID);
		va.va_uid = arg2;
	    }  
	    if ((arg1 & T_GROUP_AS_IS) == 0) {
		osi_vattr_add(&va, OSI_VA_GID);
		va.va_gid = arg3;
	    }
	    break;
	case V_UTIME:
	    osi_vattr_add(&va, OSI_VA_MTIME | OSI_VA_ATIME);
	    if (arg1 & T_SETTIME) {
		flag |= XVN_SET_TIMES_NOW;
	    } else {
		va.va_atime.tv_sec = arg2;
		va.va_atime.tv_usec = 0;
		va.va_mtime.tv_sec = arg3;
		va.va_mtime.tv_usec = 0;
	    }
	    break;
	case V_STIME:
	    return 0;
	default:
	    return (EINVAL);
    }

    error = VOPX_SETATTR(vp, (struct xvfs_attr *)&va, flag, u.u_cred);
    AFSLOG(XVFS_DEBUG, 1, ("Leaving naix_setattr mode = %o uid/gid = %d/%d err = %d\n",
	   va.va_mode, va.va_uid, va.va_gid, error));
    return (error);
}


char zero_buffer[PAGESIZE];
EXPORT int naix_fclear(vp, flags, offset, length, vinfo) 
    struct vnode *vp;
    int flags;
    long offset;
    u_long length;
    caddr_t vinfo;
{
    int i, len, error = 0;
    struct iovec iov;
    struct uio uio;
    static int fclear_init =0; 

    AFSLOG(XVFS_DEBUG, 1, ("In naix_fclear = "));

   if (VTOGP(vp)->gn_type != VREG)
	return EINVAL;

    if (!fclear_init) {
	bzero(zero_buffer, PAGESIZE);
	fclear_init = 1;
    }
    uio.osi_uio_offset = offset;
    for (i = offset; i < offset + length; i = uio.osi_uio_offset) {
	len = offset + length - i;
	iov.iov_len = (len > PAGESIZE) ? PAGESIZE : len;	
	iov.iov_base = zero_buffer;
	uio.osi_uio_iov = &iov;
	uio.osi_uio_iovcnt = 1;
	uio.osi_uio_seg = OSI_UIOSYS;
	uio.osi_uio_resid = iov.iov_len;
	if (error = VOPX_RDWR(vp, &uio, UIO_WRITE, 0, u.u_cred))
	    break;
    }
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_fsync(vp, flags, vinfo)
    struct vnode *vp;
    int flags;		    /* Not used by AFS */
    caddr_t vinfo;	    /* Not used by AFS */
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_fsync = "));
    error = VOPX_FSYNC(vp, u.u_cred);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_ftrunc(vp, flags, length, vinfo)
    struct vnode *vp;
    int flags;		    /* Ignored in AFS */
    u_long length;
    caddr_t vinfo;	    /* Ignored in AFS */
{
    struct vattr va;
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_ftrunc (length=%d) = ", length));
    osi_vattr_init(&va, OSI_VA_SIZE);
    va.va_size = length;
    error = VOPX_SETATTR(vp, (struct xvfs_attr *)&va, 0, u.u_cred);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_rdwr(vp, op, flags, ubuf, ext, vinfo)
    struct vnode *vp;
    enum uio_rw op;
    int flags;
    struct uio *ubuf;
    int ext;			/* Ignored in AFS */
    caddr_t vinfo;		/* Ignored in AFS */
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_rdwr (op=%d, flags=0x%x) = ", op, flags));
    error = VOPX_RDWR(vp, ubuf, op, flags, u.u_cred);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_lockctl(vp, offset, lckdat, cmd, ignored_fcn, ignored_id)
    void (*ignored_fcn)();
    void *ignored_id;
    struct vnode *vp;
    off_t offset;
    struct flock *lckdat;
    int cmd;
{
    int newcmd, error, oldwhence, len;

     AFSLOG(XVFS_DEBUG, 1, ("In naix_lockctl (cmd=0x%x) = ", cmd));
     /*
      * Handle relative offsets
      */
     oldwhence = lckdat->l_whence;
     switch (oldwhence) {
	case 0:	break;
	case 1: lckdat->l_start += offset; break;
	case 2:
	{
	    struct vattr va;
	    if (error = VOPX_GETATTR (vp, (struct xvfs_attr *)&va, 0, u.u_cred)) {
		return (error);
	    }
	    len = va.va_size;
	    lckdat->l_start += len;
	    break;
	}
	default: return EINVAL;
     }
     lckdat->l_whence = 0;
     /* 
      * Convert from AIX's cmd to standard lockctl lock types... 
      */
     if (cmd == 0)
	 newcmd = F_GETLK;
     else if (cmd & SETFLCK) {
	 newcmd = F_SETLK;
	 if (cmd & SLPFLCK)
	     newcmd = F_SETLKW;
     }
     error = VOPX_LOCKCTL(vp, lckdat, newcmd, u.u_cred, 0, offset);

     /* For POSIX avoid restoring whence if a successfull F_GETLK request */
     if ((cmd & SETFLCK) || error) {
	switch (oldwhence) {
	    case 1:	lckdat->l_start -= offset; break;
	    case 2: lckdat->l_start -= len;
	}
	lckdat->l_whence = oldwhence;
     } 
     AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
     return (error);
}


/* 
 * NOTE: In the nfs glue routine (nfs_gn2sun.c) the order was wrong (vp, flags,
 * cmd, arg, ext); was that another typo? 
 */
EXPORT int naix_ioctl(vp, cmd, arg, flags, ext)
    struct vnode *vp;
    int cmd;
    int arg;
    int flags;		/* Ignored in AFS */
    int ext;		/* Ignored in AFS */
{
    int	error;

    /* 
     * This seems to be a perfect fit for our ioctl redirection (cm_xioctl
     * hack); thus the ioctl(2) entry in sysent.c is unaffected in the aix/afs
     * port. 
     */ 
    AFSLOG(XVFS_DEBUG, 1, ("In naix_ioctl (cmd=0x%x, flags=%x, arg=%x) = ", 
	   cmd, flags, arg));
    error = VOPX_IOCTL(vp, cmd, arg);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_readlink(vp, uiop) 
    struct vnode *vp;
    struct uio *uiop;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_readlink = "));
    error = VOPX_READLINK(vp, uiop, u.u_cred);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}

EXPORT int naix_select(vp, corl, reqevs, rtnevs, notify, vinfo)
    int corl, reqevs, rtnevs, notify, vinfo;
    struct vnode *vp;
{

    /* 
     * NO SUPPORT for this in afs YET! 
     */
    AFSLOG(XVFS_DEBUG, 1, ("In naix_select\n"));
    return (EOPNOTSUPP);
}


EXPORT int naix_symlink(vp, link, target)
    struct vnode *vp;
    char *target;
    char *link;
{
    struct vattr va;
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_symlink (link=%s, target=%s) = ", 
	   link, target));
    osi_vattr_init(&va, OSI_VA_MODE);
    va.va_mode = 0777;
    error = VOPX_SYMLINK(vp, link, &va, target, u.u_cred);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_readdir(vp, uiop)
    struct vnode *vp;
    struct uio *uiop;
{
    int error;
    int eofflag;

    AFSLOG(XVFS_DEBUG, 1, ("In naix_readdir = "));
    error = VOPX_READDIR(vp, uiop, u.u_cred, &eofflag, 0);
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
}


EXPORT int naix_strategy(vp, bp)
    struct vnode *vp;
    struct buf *bp;
{
    panic ("naix_strategy\n");
}


EXPORT int naix_revoke(vp, cmd, flags, vattrp)
    int cmd, flags;
    struct vattr *vattrp;
    struct vnode *vp;
{
    /* 
     * NO SUPPORT for this in afs YET! 
     */
    AFSLOG(XVFS_DEBUG, 1, ("In naix_revoke\n"));
    return (EOPNOTSUPP);
}

EXPORT int naix_getacl(vp, uiop)
    struct vnode *vp;
    struct uio   *uiop;
{
    return ENOSYS;
}

EXPORT int naix_setacl(vp, uiop)
    struct vnode *vp;
    struct uio   *uiop;
{
    return ENOSYS;
}

#ifdef	EFS_SINGLE
xvfs_InitFromXOps() {};		/* XXX Dummy XXX */
xvfs_InitFromVFSOps() {};		/* XXX Dummy XXX */
tkc_Init() {};				/* XXX Dummy XXX */
#endif	/* EFS_SINGLE */

/* 
 * declare a struct vnodeops and initialize it with ptrs to all functions 
 */
/*static*/ struct osi_vnodeops naix_ops = {
    /* 
     * creation/naming/deletion 
     */
    naix_link,
    naix_mkdir,
    naix_mknod,
    naix_remove,
    naix_rename,
    naix_rmdir,
    /* 
     * lookup, file handle stuff 
     */
    naix_lookup,
    naix_fid,
    /* 
     * access to files 
     */
    naix_open,
    naix_create,
    naix_hold,
    naix_rele,
    naix_close,
    naix_map,
    naix_unmap,
    /* 
     * manipulate attributes of files 
     */
    naix_access,
    naix_getattr,
    naix_setattr,
    /* 
     * data update operations 
     */
    naix_fclear,
    naix_fsync,
    naix_ftrunc,
    naix_rdwr,
    naix_lockctl,
    /* 
     * extensions 
     */
    naix_ioctl,
    naix_readlink,
    naix_select,
    naix_symlink,
    naix_readdir,
    /* 
     * buffer ops 
     */
    naix_strategy,
    /* 
     * security things
     */
    naix_revoke,
    naix_getacl,
    naix_setacl
};
