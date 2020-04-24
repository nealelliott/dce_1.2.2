/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * This module is a glue layer used in locally mounted Episode under OSF/1.
 * It consists of functions that meet the specs of the OSF/1 vnode interface,
 * and that call the X-ops.  (The X-op interface is based on a much older
 * version of the SunOS vnode interface.)
 */

/*
 * HISTORY
 * $Log: xvfs_osf2vfs.c,v $
 * Revision 1.1.919.1  1996/10/02  19:03:42  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:52:01  damon]
 *
 * Revision 1.1.914.2  1994/06/09  14:26:05  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:38:00  annie]
 * 
 * Revision 1.1.914.1  1994/02/04  20:36:57  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:20:56  devsrc]
 * 
 * Revision 1.1.912.1  1993/12/07  17:38:49  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:19:40  jaffe]
 * 
 * Revision 1.1.5.12  1993/03/31  21:08:53  rsarbo
 * 	nosf_page_read() gets called by the VM system to fetch
 * 	a page of an executable that's being run out of DFS.
 * 	VOPX_BREAD was being called in a do/while loop to fetch
 * 	the page.  A struct buf was being osi_Alloc'ed in
 * 	VOPX_BREAD, but the b_resid field was not initialized
 * 	which caused the loop.
 * 
 * 	I replaced the call to VOPX_BREAD with a call to VOPX_RDWR.
 * 	This has the benefit of removing the while loop and all
 * 	references to bufs completely and simplifying the code
 * 	substantially.  The resultant routine mirrors the ufs_page_read()
 * 	code very closely.
 * 	[1993/03/31  21:08:05  rsarbo]
 * 
 * Revision 1.1.5.11  1993/02/16  16:14:46  jaffe
 * 	Transarc delta: kazar-ot7077-need-uncache-in-write 1.1
 * 	  Selected comments:
 * 	    overwriting an executable doesn't purge the VM system on OSF/1
 * 	    purge VM system on write calls, ala UFS.
 * 	[1993/02/15  18:59:46  jaffe]
 * 
 * Revision 1.1.5.10  1993/01/29  22:34:41  bolinger
 * 	Fix OT defect 6030:  complete implementation of nosf_filelock()
 * 	function for OSF/1 glue layer.  [Original code from tu@transarc.com.]
 * 	[1993/01/29  21:30:59  bolinger]
 * 
 * Revision 1.1.5.9  1993/01/28  18:39:34  shl
 * 	Transarc delta: bwl-ot6533-osf1-glue-mknod 1.3
 * 	  Selected comments:
 * 	    Bug fix.
 * 	    nosf_mknod should not leave a (held) vnode in ndp->ni_vp.
 * 	    Related bug.
 * 	[1993/01/27  22:01:52  shl]
 * 
 * Revision 1.1.5.8  1993/01/21  16:34:38  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:28:25  cjd]
 * 
 * Revision 1.1.5.7  1993/01/11  16:49:21  delgado
 * 	Fix crash in nosf_page_read; basically VOP_BREAD, upon error, can
 * 	return a NULL buffer pointer.  In the event of error we should
 * 	return before the MIN(n, bsize -bp->b_resid) calculation is
 * 	performed, instead of after it.
 * 	[1993/01/11  16:44:12  delgado]
 * 
 * Revision 1.1.5.6  1992/11/24  20:44:48  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:40:48  bolinger]
 * 
 * Revision 1.1.5.5  1992/10/27  21:53:15  jaffe
 * 	Transarc delta: jdp-ot5579-fix-select-on-osf1 1.1
 * 	  Selected comments:
 * 	    Fix the OSF/1 select N-op.
 * 	    After a successful vn_select, OSF/1 expects to have the parameter `revents'
 * 	    set to match the available events.  We need to do this in N-op instead of
 * 	    in Episode or the cache manager since their vn_selects take different
 * 	    parameters.
 * 	[1992/10/27  15:40:41  jaffe]
 * 
 * Revision 1.1.5.4  1992/10/13  18:12:37  marty
 * 	OSC1.1.1 upgrade.  Changes are osc1.0.4 compatible
 * 	[1992/10/13  17:13:18  marty]
 * 
 * Revision 1.1.6.2  1992/10/01  20:39:08  garyf
 * 	fix for OSF/1 R1.1
 * 	[1992/09/29  20:09:30  garyf]
 * 
 * Revision 1.1.5.3  1992/09/22  20:14:16  delgado
 * 	 get rid of references to u.u_cred and used the cred which
 * 	 is passed into us instead.
 * 	[1992/09/22  20:13:43  delgado]
 * 
 * Revision 1.1.5.2  1992/08/31  22:07:51  jaffe
 * 	minor changes
 * 	[1992/08/30  13:58:20  jaffe]
 * 
 * Revision 1.1.2.6  1992/05/22  19:46:13  garyf
 * 	change to uiomove calling sequence
 * 	remove OSF/1 conditional
 * 	[1992/05/22  03:14:46  garyf]
 * 
 * Revision 1.1.2.5  1992/05/20  20:21:58  mason
 * 	Transarc delta: cfe-delgado-fixes 1.1
 * 	  Files modified:
 * 	    xvnode/OSF1: xvfs_osf2vfs.c, xvfs_osfglue.c, xvfs_vfs2osf.c
 * 	  Selected comments:
 * 	    Incorporate some Diane Delgado fixes.
 * 	[1992/05/20  17:18:18  mason]
 * 
 * Revision 1.1.2.4  1992/05/06  21:08:20  delgado
 * 	change nosf_reclaim to call VOPX_RECLAIM
 * 	[1992/05/06  21:06:20  delgado]
 * 
 * Revision 1.1.2.2  1992/01/24  04:14:33  devsrc
 * 	Fixed logs
 * 	[1992/01/23  22:25:13  devsrc]
 * 
 * $EndLog$
*/

#include <dcedfs/param.h>

#include <dcedfs/sysincludes.h>
#include <dcedfs/basicincludes.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/osi_cred.h>
#include <kern/mfs.h>
#include <sys/poll.h>

/*
 * Layer that converts from a vanilla OSF vnode layer to the more generic (SunOS based) VFS
 * layer. Thus the VOPX_foo() type calls below will typically map to generic VFS packages
 * such as the CM and EPISODE
 */

/*
 * declare all the functions so they can be used to init the table
 */
int nosf_lookup(), nosf_create(), nosf_mknod(), nosf_open();
int nosf_close(), nosf_access(), nosf_getattr(), nosf_setattr();
int nosf_read(), nosf_write(), nosf_ioctl(), nosf_select();
int nosf_mmap(), nosf_fsync(), nosf_seek(), nosf_remove();
int nosf_link(), nosf_rename(), nosf_mkdir(), nosf_rmdir();
int nosf_symlink(), nosf_readdir(), nosf_readlink(), nosf_abortop();
int nosf_inactive(), nosf_reclaim(), nosf_bmap(), nosf_strategy();
int nosf_print(), nosf_page_read(), nosf_page_write();
#ifdef AFS_OSF11_ENV
int nosf_filelock();
#endif /* AFS_OSF11_ENV */
/*
 * declare a struct vnodeops and initialize it with ptrs to all functions
 */
struct osi_vnodeops nosf_ops = {
    nosf_lookup,
    nosf_create,
    nosf_mknod,
    nosf_open,
    nosf_close,
    nosf_access,
    nosf_getattr,
    nosf_setattr,
    nosf_read,
    nosf_write,
    nosf_ioctl,
    nosf_select,
    nosf_mmap,
    nosf_fsync,
    nosf_seek,
    nosf_remove,
    nosf_link,
    nosf_rename,
    nosf_mkdir,
    nosf_rmdir,
    nosf_symlink,
    nosf_readdir,
    nosf_readlink,
    nosf_abortop,
    nosf_inactive,
    nosf_reclaim,
    nosf_bmap,
    nosf_strategy,
    nosf_print,
    nosf_page_read,
#ifndef AFS_OSF11_ENV
    nosf_page_write
#else
    nosf_page_write,
    nosf_filelock
#endif /* AFS_OSF11_ENV */
};

nosf_lookup(vp, ndp)
	struct vnode *vp;
	register struct nameidata *ndp;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_lookup: vp=0x%x namep=%s\n", vp, ndp->ni_ptr));

        ndp->ni_dvp = vp;
	ndp->ni_vp = NULLVP;
	error = VOPX_LOOKUP(vp, ndp->ni_dent.d_name, &ndp->ni_vp, ndp->ni_cred);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_lookup: error=%d\n", error));
	return (error);
}

nosf_create(ndp, vap)
	struct nameidata *ndp;
	struct vattr *vap;
{
	int error;
        int mode;
        enum vcexcl exclusive;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_create: (namep=%s)\n", ndp->ni_ptr));

	mode = (int) vap->va_mode;
	/* we assume that when we are called, the client does NOT want the
	   exclusive flag set (as in NFS)
	 */
	exclusive = NONEXCL;
        ndp->ni_vp = NULLVP;
	error = VOPX_CREATE(ndp->ni_dvp, ndp->ni_dent.d_name, vap, exclusive, mode, &ndp->ni_vp, ndp->ni_cred);

	VN_RELE(ndp->ni_dvp);
	AFSLOG(XVFS_DEBUG, 1, ("nosf_create: error=%d\n", error));
	return(error);
}

nosf_mknod(ndp, vap, credp)
	struct nameidata *ndp;
	osi_cred_t *credp;
	register struct vattr *vap;
{
	int error, mode;
	struct vnode *vp;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_mknod: (vp=0x%x)\n", ndp->ni_dvp));

	/* check for super-user (only he can do mknod()),
		except for a FIFO file
	 */
	if (vap->va_type != VFIFO && !osi_suser(credp)) {
	        VN_RELE(ndp->ni_dvp);
		return(EPERM);
	}
	switch (vap->va_type) {
	case VDIR :
		error = VOPX_MKDIR(ndp->ni_dvp, ndp->ni_dent.d_name, vap,
				   &vp, credp);
		if (!error) VN_RELE(vp);
		break;
	case VNON :
		break;
	default :
		mode = (int) vap->va_mode;
		error = VOPX_CREATE(ndp->ni_dvp, ndp->ni_dent.d_name, vap,
				    NONEXCL, mode, &vp, credp);
		if (!error) VN_RELE(vp);
	}

	VN_RELE(ndp->ni_dvp);
	AFSLOG(XVFS_DEBUG, 1, ("nosf_mknod: error=%d\n", error));
	return (error);
}

nosf_open(vpp, fflag, credp)
	struct vnode **vpp;
	int fflag;
	osi_cred_t *credp;
{
	int error;
	struct vattr va;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_open: (vp=0x%x, fflag=%d)\n", *vpp, fflag));

	error = VOPX_OPEN(vpp, fflag, credp);
	/* not sure if this code is needed */
	if (fflag & FTRUNC) {
		osi_vattr_init(&va, OSI_VA_SIZE);
		va.va_size = 0;
       	        error = VOPX_SETATTR(*vpp, &va, 0, credp);
	}

	AFSLOG(XVFS_DEBUG, 1, ("nosf_open: error =%d\n", error));
	return (error);

}

nosf_close(vp, fflag, credp)
	struct vnode *vp;
	int fflag;
	osi_cred_t *credp;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_close: (fflags=0x%x)\n", fflag));

	error = VOPX_CLOSE(vp, fflag, credp);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_close: error = %d\n", error));
	return (error);
}

nosf_access(vp, mode, credp)
	struct vnode *vp;
	int mode;
	osi_cred_t *credp;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_access: (mode=0x%x)\n", mode));

	error = VOPX_ACCESS(vp, mode, credp);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_access: error=%d\n", error));
	return (error);

}

nosf_getattr(vp, vap, credp)
	struct vnode *vp;
	register struct vattr *vap;
	osi_cred_t *credp;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_getattr: (vp=0x%x,vap=0x%x)\n", vp, vap));

        error = VOPX_GETATTR(vp, vap, 0, credp);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_getattr: error=%d\n", error));

	return (error);
}

nosf_setattr(vp, vap, credp)
	register struct vnode *vp;
	register struct vattr *vap;
	register osi_cred_t *credp;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_setattr: (vp=0x%x,vap=0x%x)\n", vp, vap));

        error = VOPX_SETATTR(vp, vap, 0, credp);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_setattr: error = %d\n", error));
	return (error);
}


nosf_read(vp, uiop, ioflag, credp)
	struct vnode *vp;
	register struct uio *uiop;
	int ioflag;
	osi_cred_t *credp;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_read: (vp=0x%x)\n", vp));

	error = VOPX_RDWR(vp, uiop, UIO_READ, ioflag, credp);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_read: error=%d\n", error));
	return (error);
}

nosf_write(vp, uiop, ioflag, credp)
	register struct vnode *vp;
	struct uio *uiop;
	int ioflag;
	osi_cred_t *credp;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_write: (vp=0x%x)\n", vp));

	BM(VN_LOCK(vp));
	if ((vp->v_vm_info != VM_INFO_NULL) &&
	    (vp->v_vm_info->pager != MEMORY_OBJECT_NULL)) {
	    BM(VN_UNLOCK(vp));
	    vnode_uncache(vp);
	} else
	    BM(VN_UNLOCK(vp));

	error = VOPX_RDWR(vp, uiop, UIO_WRITE, ioflag, credp);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_write: error=%d\n", error));
	return (error);
}

#ifndef AFS_OSF11_ENV
nosf_ioctl(vp, com, data, fflag, credp)
	struct vnode *vp;
	int com;
	caddr_t data;
	int fflag;
	osi_cred_t *credp;
#else
nosf_ioctl(vp, com, data, fflag, credp, retval)
	struct vnode *vp;
	int com;
	caddr_t data;
	int fflag;
	osi_cred_t *credp;
	int *retval;     
#endif /* AFS_OSF11_ENV */
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_ioctl: (vp=0x%x)\n", vp));

	error = VOPX_IOCTL(vp, com, data);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_ioctl: error=%d\n", error));
	return (error);
}

nosf_select(vp, events, revents, scanning, cred)
	struct vnode *vp;
	short *events, *revents;
        int scanning;
	osi_cred_t *cred;
{
	int error;
	int which = 0;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_select: (vp=0x%x)\n", vp));

	if (*events & POLLNORM)
	    which |= FREAD;
	if (*events & POLLOUT)
	    which |= FWRITE;

	error = VOPX_SELECT(vp, which, cred);

	if (!error && scanning)
	    *revents = *events;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_select: error=%d\n", error));
	return (error);
}

nosf_mmap(vp, fflags, cred)
	struct vnode *vp;
	int fflags;
	osi_cred_t *cred;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_mmap: (vp=0x%x)\n", vp));

	error = EOPNOTSUPP;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_mmap: error=%d\n", error));
	return (error);
}

nosf_fsync(vp, fflags, credp, waitfor)
	struct vnode *vp;
	int fflags;
	osi_cred_t *credp;
	int waitfor;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_fsync: (vp=0x%x)\n", vp));

	error = VOPX_FSYNC(vp, credp);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_fsync: error=%d\n", error));
	return (error);
}

nosf_seek(vp, oldoff, newoff, cred)
	struct vnode *vp;
	off_t oldoff, newoff;
	osi_cred_t *cred;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_seek: (vp=0x%x)\n", vp));

	if ((int) newoff < 0)
		return(EINVAL);
	else
		return(0);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_seek: error=%d\n", error));
	return (error);
}

nosf_remove(ndp)
	struct nameidata *ndp;
{
        int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_remove: (dp=0x%x, namep=%s)\n", ndp->ni_dvp, ndp->ni_ptr));

	error = VOPX_REMOVE(ndp->ni_dvp, ndp->ni_dent.d_name, ndp->ni_cred);

	VN_RELE(ndp->ni_dvp);
	VN_RELE(ndp->ni_vp);
	AFSLOG(XVFS_DEBUG, 1, ("nosf_remove: error=%d\n", error));
	return (error);
}

nosf_link(vp, ndp)
	register struct vnode *vp;
	register struct nameidata *ndp;
{
        int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_link: (dp=0x%x, namep=%s)\n", ndp->ni_dvp, ndp->ni_ptr));

	error = VOPX_LINK(vp, ndp->ni_dvp, ndp->ni_dent.d_name, ndp->ni_cred);

	VN_RELE(ndp->ni_dvp);
	AFSLOG(XVFS_DEBUG, 1, ("nosf_link: error=%d\n", error));
	return (error);
}

nosf_rename(fndp, tndp)
	register struct nameidata *fndp, *tndp;
{
        int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_rename: (dp=0x%x, namep=%s)\n", fndp->ni_dvp, fndp->ni_dent.d_name));
	AFSLOG(XVFS_DEBUG, 1, ("nosf_rename: (dp=0x%x, namep=%s)\n", tndp->ni_dvp, tndp->ni_dent.d_name));

	error = VOPX_RENAME(fndp->ni_dvp, fndp->ni_dent.d_name, tndp->ni_dvp, tndp->ni_dent.d_name, fndp->ni_cred);

	VN_RELE(fndp->ni_dvp);
	VN_RELE(fndp->ni_vp);
	VN_RELE(tndp->ni_dvp);
	if (tndp->ni_vp)
	  VN_RELE(tndp->ni_vp);
	AFSLOG(XVFS_DEBUG, 1, ("nosf_rename: error=%d\n", error));
	return (error);
}

nosf_mkdir(ndp, vap)
	struct nameidata *ndp;
	struct vattr *vap;
{
        int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_mkdir: (dp=0x%x, namep=%s)\n", ndp->ni_dvp, ndp->ni_ptr));

	error = VOPX_MKDIR(ndp->ni_dvp, ndp->ni_dent.d_name, vap, &ndp->ni_vp, ndp->ni_cred);

	VN_RELE(ndp->ni_dvp);
	AFSLOG(XVFS_DEBUG, 1, ("nosf_mkdir: error=%d\n", error));
	return (error);
}

nosf_rmdir(ndp)
	register struct nameidata *ndp;
{
        int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_rmdir: (dp=0x%x)\n", ndp->ni_dvp));

	error = VOPX_RMDIR(ndp->ni_dvp, ndp->ni_dent.d_name,
			   (struct vnode *) 0, ndp->ni_cred);

	VN_RELE(ndp->ni_dvp);
	VN_RELE(ndp->ni_vp);
	AFSLOG(XVFS_DEBUG, 1, ("nosf_rmdir: error=%d\n", error));
	return (error);
}

nosf_symlink(ndp, vap, target)
	struct nameidata *ndp;
	struct vattr *vap;
	char *target;
{
        int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_symlink: (dp=0x%x, namep=%s)\n", ndp->ni_dvp, ndp->ni_ptr));

	error = VOPX_SYMLINK(ndp->ni_dvp, ndp->ni_dent.d_name, vap, target, ndp->ni_cred);

	VN_RELE(ndp->ni_dvp);
	AFSLOG(XVFS_DEBUG, 1, ("nosf_symlink: error=%d\n", error));
	return (error);
}

nosf_readdir(vp, uiop, credp, eofflagp)
	struct vnode *vp;
	register struct uio *uiop;
	osi_cred_t *credp;
	int *eofflagp;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_readdir: vp=0x%x\n", vp));

	error = VOPX_READDIR(vp, uiop, credp, eofflagp, 0);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_readdir: error=%d\n", error));
	return (error);
}

nosf_readlink(vp, uiop, cred)
	struct vnode *vp;
	struct uio *uiop;
	osi_cred_t *cred;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_readlink: (vp=0x%x)\n", vp));

	error = VOPX_READLINK(vp, uiop, cred);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_readlink: error=%d\n", error));
	return (error);
}

nosf_abortop(ndp)
	register struct nameidata *ndp;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_abortop: (ndp=0x%x)\n", ndp));

	error = 0;  /* always return success */

	AFSLOG(XVFS_DEBUG, 1, ("nosf_abortop: error=%d\n", error));
	return (error);
}

nosf_inactive(vp)
	register struct vnode *vp;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_inactive: (vp=0x%x)\n", vp));

	error = VOPX_INACTIVE(vp, u.u_cred);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_inactive: error=%d\n", error));
	return (error);
}

nosf_reclaim(vp)
	register struct vnode *vp;
{
	int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_reclaim: (vp=0x%x)\n", vp));

	error = VOPX_RECLAIM(vp);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_reclaim: error=%d\n", error));
	return (error);
}

nosf_bmap(vp, bn, vpp, bnp)
	struct vnode *vp;
	daddr_t bn;
	struct vnode **vpp;
	daddr_t *bnp;
{
        int error;
	AFSLOG(XVFS_DEBUG, 1, ("nosf_bmap: (vp=0x%x)\n", vp));

	error = VOPX_BMAP(vp, bn, vpp, bnp);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_bmap: error=%d\n", error));
	return(error);
}

nosf_strategy(bp)
	register struct buf *bp;
{
        int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_strategy: (bp=0x%x)\n", bp));

	error = VOPX_STRATEGY(bp->b_vp, bp);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_strategy: error=%d\n", error));
	return (error);
}

nosf_print(vp)
	struct vnode *vp;
{
        int error;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_print: (vp=0x%x)", vp));

	error  = 0; /* always succeeds */

	AFSLOG(XVFS_DEBUG, 1, ("nosf_print: error=%d\n", error));
	return (error);
}

/*
 *
 * Read some number of pages for the VM system.
 * The only difference between this read and a normal read is that
 * reads beyond EOF return EINVAL, and the space between EOF and
 * the end of an overlapping page is zero-filled on successful reads.
 */
nosf_page_read(vp, uio, credp)
	struct vnode *vp;
	struct uio *uio;
	osi_cred_t *credp;
{
	int limit;		/* can't read more bytes than this */
	int error;		/* our error return */
	int size;
	caddr_t orig_base;
	u_long isize;
	int diff;
	int n;		/* the number of bytes read this time */

	AFSLOG(XVFS_DEBUG, 1, ("nosf_page_read: (vp=0x%x)\n", vp));

	size = uio->osi_uio_resid;
	orig_base = uio->osi_uio_iov->iov_base;

        error = VOPX_GETLENGTH(vp, &isize, credp);
        if (error)
           return(error);

	limit = isize - uio->osi_uio_offset;
	if ((limit <= 0) || (size <= 0))
		error = EIO;
	else
		error = VOPX_RDWR(vp, uio, UIO_READ, 0, osi_getucred());

	/*
	 *	If we are returning real data in the buffer,
	 *	and we ran up against EOF, we must zero the
	 *	remainder of the page that crosses EOF, and not 
	 *	beyond, since any pages beyond the page that crosses 
	 *	EOF are in error.
	 */
	if ((error == 0) && (limit < size)) {
		ASSERT(uio->uio_resid == (size-limit));
		bzero(orig_base + limit, (size-limit)&page_mask);
	}

	AFSLOG(XVFS_DEBUG, 1, ("nosf_page_read: error=%d\n", error));
	return(error);
}

#ifndef AFS_OSF11_ENV
nosf_page_write(vp, uiop, credp, pager, offset)
	struct vnode	*vp;
	struct uio 	*uiop;
	osi_cred_t	*credp;
        memory_object_t pager;
        vm_offset_t     offset;
{
        int error, ioflag=0;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_page_write: (vp=0x%x)", vp));

	error = VOPX_RDWR(vp, uiop, UIO_WRITE, ioflag, credp); 

	AFSLOG(XVFS_DEBUG, 1, ("nosf_page_write: error=%d\n", error));
	return (error);
}
#else
nosf_page_write(vp, uio, credp, async_args, async_func)
	struct vnode	*vp;
	struct uio 	*uio;
	osi_cred_t	*credp;
	void 		*async_args;
	void		(*async_func)();
{
	int mapped, error = 0, ioflag=0;
	int amount_truncated;

	AFSLOG(XVFS_DEBUG, 1, ("nosf_page_write: (vp=0x%x)", vp));

	/*
	 * We must be sure to avoid extending the file via paging, so
	 * we cut off any length beyond EOF in the page out request, but
	 * only for files not in use as paging files.  If the request
	 * begins past EOF, then we return EIO, causing any future
	 * pageins from this file to fail.  This decision could be 
	 * changed if the semantic is not that desired.  It's possible
	 * that we should seg fault the user.
	 *
	 * XXX -- the check for paging or not is pretty gross, but it's
	 * all we've got unless we pass it as a parameter (an option...).
	 */
	BM(VN_LOCK(vp));
	mapped = (int) vp->v_vm_info;
	BM(VN_UNLOCK(vp));
	if (mapped) {
		u_long isize;

		amount_truncated = 0;
		error = VOPX_GETLENGTH(vp, &isize, credp);
		if (error)
			return(error);
		if (uio->uio_offset > isize)
			error = EIO;
		else {
			int eorq = uio->uio_offset + uio->uio_resid;
			if (eorq > isize) {
				amount_truncated = (eorq - isize);
				uio->uio_resid -= amount_truncated;
				uio->uio_iov->iov_len = uio->uio_resid;
			}
		}
	}
	if (!error)
		error = VOPX_RDWR(vp, uio, UIO_WRITE, ioflag, credp); 
	if (error && !mapped)
		printf("error %d on pageout (nosf_page_write)\n", error);
	if (mapped && amount_truncated)
		uio->uio_resid += amount_truncated;
	if (async_args)
		(*async_func)(async_args, error);

	AFSLOG(XVFS_DEBUG, 1, ("nosf_page_write: error=%d\n", error));
	return (error);
}
#endif /* AFS_OSF11_ENV */

#ifdef AFS_OSF11_ENV
int nosf_filelock(vp, bf, flag, offset)
struct vnode *vp;
struct flock *bf;
int flag;
off_t offset;
{
	long code;
	int command, oldwhence;
	struct vattr va;

	osi_assert(flag & (SETFLCK|GETFLCK));
	/*
	 * Compute the desired relative offset 
	 */
	oldwhence = bf->l_whence;
	switch(oldwhence) {
	case SEEK_SET:
		break;
	case SEEK_CUR:
		bf->l_start += offset;
		break;
	case SEEK_END:  {
		if (code = VOPX_GETATTR(vp, &va, 0, u.u_cred))
			return code;
		bf->l_start += va.va_size;
		break;
	}
	default:
		return EINVAL;
	}

	bf->l_whence = 0;
	if (flag & GETFLCK)
		command = F_GETLK;
	else if (flag & SETFLCK) {
		command = F_SETLK;
		if (flag & SLPFLCK)
			command = F_SETLKW;
	}
	code = VOPX_LOCKCTL(vp, bf, command, u.u_cred, 0);

	/* For POSIX avoid restoring whence for a successful F_GETLK request */
	if ((flag & SETFLCK) || code) {
		switch (oldwhence) {
		case SEEK_CUR:	 
			bf->l_start -= offset; 
			break;
		case SEEK_END: 
			bf->l_start -= va.va_size;
		}
		bf->l_whence = oldwhence;
	}
	return code;
}
#endif /* AFS_OSF11_ENV */

