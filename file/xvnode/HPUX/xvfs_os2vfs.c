/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xvfs_os2vfs.c,v $
 * Revision 1.1.114.1  1996/10/02  19:03:25  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:53  damon]
 *
 * Revision 1.1.109.2  1994/06/09  14:25:54  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:37:49  annie]
 * 
 * Revision 1.1.109.1  1994/02/04  20:36:40  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:20:49  devsrc]
 * 
 * Revision 1.1.107.1  1993/12/07  17:38:38  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:18:01  jaffe]
 * 
 * Revision 1.1.6.2  1993/07/19  19:37:31  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:27:59  zeliff]
 * 
 * 	Add nux_pagein() to dispatch appropriately to page fault handler code.
 * 	[1993/05/06  22:08:39  sommerfeld]
 * 
 * 	Initial GAMERA branch
 * 	[1993/04/02  11:59:41  mgm]
 * 
 * Revision 1.1.4.3  1993/07/16  21:00:27  kissel
 * 	*** empty log message ***
 * 	[1993/06/21  14:44:09  kissel]
 * 
 * 	afs -> dcedfs.
 * 	[1993/01/14  19:43:11  toml]
 * 
 * 	Adjust MAXFIDSZ for HPUX 9.0.
 * 	[1992/10/16  22:02:22  toml]
 * 
 * 	Initial revision for HPUX.
 * 	[1992/10/15  18:41:18  toml]
 * 
 * Revision 1.1.2.2  1993/06/04  15:45:31  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  21:55:56  kissel]
 * 
 * $EndLog$
 */

/*
 * This module is a glue layer used in locally mounted Episode under HP/UX.
 * It consists of functions that meet the specs of the HP/UX vnode interface,
 * and that call the X-ops.  (The X-op interface is based on a much older
 * version of the SunOS vnode interface.  That is why many of the functions
 * are trivial or nearly so.)
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/stds.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/debug.h>
#include <dcedfs/osi_user.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/osi_buf.h>
#include <ufs/inode.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/xvnode/HPUX/xvfs_os2vfs.c,v 1.1.114.1 1996/10/02 19:03:25 damon Exp $")

/* Forward declarations */

int nux_open(), nux_close(), nux_rdwr(), nux_ioctl(), nux_select();
int nux_getattr(), nux_setattr(), nux_access(), nux_lookup();
int nux_create(), nux_remove(), nux_link(), nux_rename();
int nux_mkdir(), nux_rmdir(), nux_readdir(), nux_symlink();
int nux_readlink(), nux_fsync(), nux_inactive(), nux_bmap();
int nux_strategy(), nux_bread(), nux_brelse(), nux_pathsend();
int nux_setacl(), nux_getacl(), nux_pathconf(), nux_fpathconf();
int nux_lockctl(), nux_lockf(), nux_fid(), nux_fsctl();
int nux_pagein(), nux_pageout();

extern int vfs_prefill();

/* vector of N-ops */

/* XXX Those 0's should be filled in.  Perhaps nux_panic? XXX */

struct osi_vnodeops nux_ops = {
    nux_open,
    nux_close,
    nux_rdwr,
    nux_ioctl,
    nux_select,
    nux_getattr,
    nux_setattr,
    nux_access,
    nux_lookup,
    nux_create,
    nux_remove,
    nux_link,
    nux_rename,
    nux_mkdir,
    nux_rmdir,
    nux_readdir,
    nux_symlink,
    nux_readlink,
    nux_fsync,
    nux_inactive,
    nux_bmap,
    nux_strategy,
    nux_bread,
    nux_brelse,
    nux_pathsend,
    nux_setacl,
    nux_getacl,
    nux_pathconf,
    nux_fpathconf,
    nux_lockctl,
    nux_lockf,
    nux_fid,
    nux_fsctl,
    vfs_prefill,
    nux_pagein,
    nux_pageout,
    0, /* nux_dbddup */
    0  /* nux_dbddealloc */
};

EXPORT int nux_open(vpp, flags, cred)
  struct vnode **vpp;
  int flags;
  struct ucred *cred;
{
    struct vattr va;
    struct vnode *vp = *vpp;
    int error = 0;
    int mode = 0;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_open (flags = 0x%x)\n", flags));

    error = VOPX_OPEN (vpp, flags, cred);

#ifdef notdef
    /* 
     * This truncation code is done in the ux kernel now 
     * (sys/vfs_vnode.c, vns_copen) and I claim it is doing this
     * a lot better than we are, so nuke this later.
     */
    if (!error) {
	if (flags & FTRUNC) {
	    osi_vattr_null(&va);
	    va.va_size = 0;
	    error = VOPX_SETATTR(vp, &va, 0, cred);
	}
    }
#endif /* notdef */

    AFSLOG(XVFS_DEBUG, 1, ("END nux_open %d\n", error));
    return (error);
      
}


EXPORT int nux_close(vp, flags, credp)
  struct vnode *vp;
  int flags;
  struct ucred *credp;
{
    int error;
	

    AFSLOG(XVFS_DEBUG, 1, ("In nux_close (flags = 0x%x)\n", flags));
    error = VOPX_CLOSE(vp, flags, credp);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_close %d\n", error));
    return (error);
      
}


EXPORT int nux_rdwr(vp, uiop, op, flags, credp)
  struct vnode *vp;
  struct uio *uiop;
  enum uio_rw op;
  int flags;
  struct ucred *credp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_rdwr (flags = 0x%x)\n", flags));
    error = VOPX_RDWR(vp, uiop, op, flags, credp);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_rdwr %d\n", error));
    return (error);
      
}


EXPORT int nux_ioctl(vp, com, data, flag, credp)
  struct vnode *vp;
  int com;
  caddr_t data;
  int flag;
  struct ucred *credp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_ioctl (flag = 0x%x)\n", flag));
    error = VOPX_IOCTL(vp, com, data);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_ioctl %d\n", error));
    return (error);
      
}


EXPORT int nux_select(vp, wait, credp)
  struct vnode *vp;
  int wait;
  struct ucred *credp;
{
    /* 
     * NO SUPPORT for this in afs YET! 
     */
    AFSLOG(XVFS_DEBUG, 1, ("In nux_select\n"));
    return (EOPNOTSUPP);
      
}


EXPORT int nux_getattr(vp, vattrp, credp, vsync)
  struct vnode *vp;
  struct vattr *vattrp;
  struct ucred *credp;
  enum vsync vsync;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_getattr\n"));
    /*
     * 'vsync' is used for DUX to synchronize inode.
     * we don't need this for dfs (nfs ignores it also).
     */
    vattrp->va_fstype = vp->v_vfsp->vfs_mtype;
    error = VOPX_GETATTR(vp, vattrp, 0, credp);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_getattr %d\n", error));
    return (error);
      
}


EXPORT int nux_setattr(vp, vattrp, credp, null_time)
  struct vnode *vp;
  struct vattr *vattrp;
  struct ucred *credp;
  int null_time;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_setattr\n"));
    error = VOPX_SETATTR(vp, vattrp, null_time ? XVN_SET_TIMES_NOW : 0, credp);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_setattr %d\n", error));
    return (error);
      
}

	
EXPORT int nux_access(vp, mode, credp)
  struct vnode *vp;
  long mode;
  struct ucred *credp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_access\n"));
    error = VOPX_ACCESS(vp, mode, credp);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_access %d\n", error));
    return (error);
      
}

EXPORT int nux_lookup(dp, namep, vpp, credp, mvp)
  struct vnode *dp, *mvp;
  char *namep;
  struct vnode **vpp;
  struct ucred *credp;
{
    int error;

    /* 
     * 'mvp' is a hint to DUX that claims if the path is ..
     * and across a mount point, mvp will be different from dp.
     * Otherwise they are the same.  We don't care.
     */
    AFSLOG(XVFS_DEBUG, 1, ("In nux_lookup\n"));
    error = VOPX_LOOKUP(dp, namep, vpp, credp);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_lookup %d\n", error));
    return (error);
      
}


EXPORT int nux_create(dp, namep, vattrp, excl, mode, vpp, credp)
    struct vnode *dp, **vpp;
    struct vattr *vattrp;
    enum vcexcl excl;
    int mode;
    char *namep;
    struct ucred *credp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_create\n"));
    error = VOPX_CREATE(dp, namep, vattrp, excl, mode, vpp, credp);
#ifdef notdef
    VN_RELE(dp);
#endif /* notdef */
    AFSLOG(XVFS_DEBUG, 1, ("END nux_create %d\n", error));
    return (error);
      
}


EXPORT int nux_remove(dp, namep, credp)
  struct vnode *dp;
  char *namep;
  struct ucred *credp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_remove\n"));
    error = VOPX_REMOVE(dp, namep, credp);
#ifdef	notdef
    VN_RELE(dp);
#endif /* notdef */
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);
      
}


EXPORT int nux_link(vp, dp, namep, credp)
  struct vnode *vp, *dp;
  char *namep;
  struct ucred *credp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_link\n"));
    error = VOPX_LINK(vp, dp, namep, credp);
#ifdef notdef
    VN_RELE(dp);
#endif /* notdef */
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
    return (error);     
}


EXPORT int nux_rename(dp, namep, tdp, tnamep, credp)
  struct vnode *dp, *tdp;
  char *namep,*tnamep;
  struct ucred *credp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_rename\n"));
    error = VOPX_RENAME(dp, namep, tdp, tnamep, credp);
#ifdef notdef
more VN_RELE's
#endif
    AFSLOG(XVFS_DEBUG, 1, ("END nux_rename %d\n", error));
    return (error);      
}


EXPORT int nux_mkdir(dp, namep, vattrp, vpp, credp)
  struct vnode *dp, **vpp;
  char *namep;
  struct vattr *vattrp;
  struct ucred *credp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_mkdir\n"));
    error = VOPX_MKDIR(dp, namep, vattrp, vpp, credp);
#ifdef notdef
    if (!error)
	VN_RELE(*vpp);
#endif /* notdef */
    AFSLOG(XVFS_DEBUG, 1, ("END nux_mkdir %d\n", error));
    return (error);
      
}


EXPORT int nux_rmdir(dp, namep, credp)
  struct vnode *dp;
  char *namep;
  struct ucred *credp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_rmdir\n"));
    error = VOPX_RMDIR(dp, namep, (struct vnode *) 0, credp);
#ifdef	notdef
more VN_RELE's
#endif /* notdef */
    AFSLOG(XVFS_DEBUG, 1, ("END nux_rmdir %d\n", error));
    return (error);      

}


EXPORT int nux_readdir(vp, uiop, credp)
  struct vnode *vp;
  struct uio *uiop;
  struct ucred *credp;
{
    int error;
    int eofflag;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_readdir\n"));
    error = VOPX_READDIR(vp, uiop, credp, &eofflag, 0);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_readdir %d\n", error));
    return (error);      

}


EXPORT int nux_symlink(vp, namep, vattrp, targetp, credp)
  struct vnode *vp;
  struct vattr *vattrp;
  char *namep, *targetp;
  struct ucred *credp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_symlink\n"));
    error = VOPX_SYMLINK(vp, namep, vattrp, targetp, credp);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_symlink %d\n", error));
    return (error);
      
}


EXPORT int nux_readlink(vp, uiop, credp)
  struct vnode *vp;
  struct uio *uiop;
  struct ucred *credp;

{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_readlink\n"));
    error = VOPX_READLINK(vp, uiop, credp);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_readlink %d\n", error));
    return (error);
      
}


EXPORT int nux_fsync(vp, credp, ino_chg)
  struct vnode *vp;
  struct ucred *credp;
  int ino_chg;
{
    int error;

    /*
     * ino_chg is hackery associated with null_time.
     */
    AFSLOG(XVFS_DEBUG, 1, ("In nux_fsync\n"));
    error = VOPX_FSYNC(vp, credp);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_fsync %d\n", error));
    return (error);      

}


EXPORT int nux_inactive(vp, credp)
  struct vnode *vp;
  struct ucred *credp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_inactive\n"));
    error = VOPX_INACTIVE(vp, credp);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_inactive %d\n", error));
    return (error);
      
}


EXPORT int nux_bmap(vp,bn,vpp,bnp)
  struct vnode *vp;
  daddr_t bn;
  struct vnode **vpp;
  daddr_t *bnp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_bmap\n"));
    error = VOPX_BMAP(vp, bn, vpp, bnp);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_bmap %d\n", error));
    return (error);
      
}


EXPORT int nux_strategy(bp)
  struct buf *bp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("nux_strategy: (bp=0x%x)\n", bp));
    error = VOPX_STRATEGY(bp->b_vp, bp);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_strategy: error=%d\n", error));
    return (error);
      
}


EXPORT int nux_bread(vp,bn,bpp)
  struct vnode *vp;
  daddr_t bn;
  struct buf **bpp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_bread\n"));
    error = VOPX_BREAD(vp, bn, bpp);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_bread %d\n", error));
    return (error);
      
}


EXPORT int nux_brelse(vp,bp)
  struct vnode *vp;
  struct buf *bp;
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_brelse\n"));
    error = VOPX_BRELSE(vp,bp);
    AFSLOG(XVFS_DEBUG, 1, ("END nux_brelse %d\n", error));
    return (error);
      
}


EXPORT int nux_pathsend(vp,pnp,follow,nlinkp,dirvpp,compvpp,opcode,dep)
  struct vnode *vp, **dirvpp, *compvpp;
  long follow, opcode, dep;
  caddr_t pnp, nlinkp; 
{
    int error;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_pathsend\n"));
    error = EINVAL;
    AFSLOG(XVFS_DEBUG, 1, ("END nux_pathsend %d\n", error));
    return (error);
      
}


EXPORT int nux_setacl(vp, nt, bp)
  struct vnode *vp;
  opaque nt; /* XXX: what is this? */
  struct buf *bp;
  {
      int error;

      AFSLOG(XVFS_DEBUG, 1, ("In nux_setacl "));
      error = EINVAL;
      AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
      return (error);
      
  }

EXPORT int nux_getacl(vp, nt, bp)
  struct vnode *vp;
  opaque nt;   /* XXX: what is this, need to check kernel code */
  struct buf  *bp;
  {
      int error;

      AFSLOG(XVFS_DEBUG, 1, ("In nux_getacl "));
      error = EINVAL;
      AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
      return (error);
      
  }

EXPORT int nux_pathconf(vp, nt, bp, cr)
  struct vnode *vp;
  struct buf *bp;
  opaque nt, cr; /* XXX: these types are wrong */
  {
      int error;

      AFSLOG(XVFS_DEBUG, 1, ("In nux_pathconf "));
      error = EINVAL;
      AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
      return (error);
      
  }

EXPORT int nux_fpathconf(vp, nt, bp, cr)
  struct vnode *vp;
  struct buf *bp;
  opaque nt, cr; /* XXX: these types are wrong */
  {
      int error;

      AFSLOG(XVFS_DEBUG, 1, ("In nux_fpathconf "));
      error = EINVAL;
      AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
      return (error);
      
  }

EXPORT int nux_lockctl(vp, lckdat, cmd, cred, fp, LB, UB)
  struct vnode *vp;
  struct flock *lckdat;
  int cmd;
  struct ucred *cred;
  struct file *fp;
  off_t LB, UB;
  {

     int error, oldwhence, len;

     AFSLOG(XVFS_DEBUG, 1, ("In nux_lockctl (cmd=0x%x) = ", cmd));

     /*
      * Handle relative offsets
      */
     oldwhence = lckdat->l_whence;
     switch (oldwhence) {
	case SEEK_SET:	break;
	case SEEK_CUR:  lckdat->l_start += fp->f_offset; break;
	case SEEK_END:	VOPX_GETLENGTH (vp, &len, u.u_cred);
		        lckdat->l_start += len; break;
	default:	return EINVAL;
     }
     lckdat->l_whence = 0;

     error = VOPX_LOCKCTL(vp, lckdat, cmd, u.u_cred, 0, fp->f_offset);

     switch (oldwhence) {
	case SEEK_CUR:	lckdat->l_start -= fp->f_offset; break;
	case SEEK_END:  lckdat->l_start -= len;
     }
     if (cmd != F_GETLK || lckdat->l_type == F_UNLCK)
	 lckdat->l_whence = oldwhence;

     AFSLOG(XVFS_DEBUG, 1, ("END nux_lockctl (code %d)\n", error));
     return (error);
}

EXPORT int nux_lockf(vp, ld, cmd, credp, fp, lb, ub)
  struct vnode *vp;
  struct flock *ld;
  int cmd;
  struct ucred *credp;
  void (*fp)(); /* XXX type OK here? */
  off_t lb, ub;
  {
      int error;

      AFSLOG(XVFS_DEBUG, 1, ("In nux_lockf "));

      /* again, I refuse to code on grounds that it should be
       * a generic routine in VFS that calls VOP_LOCKCTL() */
      error = EINVAL;
      AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
      return (error);
      
  }

EXPORT int nux_fid(vp, fidpp)
    struct vnode *vp;
    struct fid **fidpp;
{
    int error;
    struct fid *fidp;

    AFSLOG(XVFS_DEBUG, 1, ("In nux_fid "));

    /* allocate a full size FID for DFS */
    fidp = (struct fid *) kmem_alloc(sizeof(struct fid));
    bzero((caddr_t)fidp, sizeof(struct fid));
#ifndef	CM_MAXFIDSZ
#define	CM_MAXFIDSZ 10
#endif
    fidp->fid_len = CM_MAXFIDSZ;
    error = VOPX_FID(vp, fidp);
    if (!error)
	*fidpp = fidp;
    AFSLOG(XVFS_DEBUG, 1, ("END nux_fid %d\n", error));
    return (error);
      
}

EXPORT int nux_fsctl(vp, cmd, uiop, credp)
  struct vnode *vp;
  int cmd;
  struct uio *uiop;
  struct ucred *credp;
  {
      int error;

      AFSLOG(XVFS_DEBUG, 1, ("In nux_fsctl "));
      error = EINVAL;
      AFSLOG(XVFS_DEBUG, 1, ("%d\n", error));
      return (error);
      
  }

#include <sys/region.h>
#include <sys/pregion.h>

/*
 *  VFS variant of the DBD format.  Simply the DBD type, and a block number
 *  for reading the data off the devvp for the vnode.
 */
typedef struct vfsdbd {
	uint dbd_type : 4;
	uint dbd_blkno : 28;
} vfsdbd_t;

int nux_pagein(prp, wrt, space, vaddr, ret_startindex)
    preg_t *prp;
    int wrt;
    space_t space;
    caddr_t vaddr;
    int *ret_startindex;
{
    reg_t *rp = prp->p_reg;
    register vfsdbd_t *dbd;
    register vfd_t *vfd;
    dbd_t *dbd2;
    vfd_t *vfd2;
    int pgindx;
    struct vnode *filevp;
    /*
     * HP-UX, in its infinite wisdom, does not pass the vnode to the
     * underlying pagein call, so we have to do the following grunge
     * to extract the information.
     * 
     * Of course, it will probably all wind up in cache so it will be
     * much faster the second time around :-)
     */
    vm_sema_state;		/* semaphore save state */

    vmemp_lockx();		/* lock down VM empire */
    /*	
     * Find vfd and dbd as mapped by region
     */
    pgindx = regindx(prp, vaddr);

    FINDENTRY(rp, pgindx, &vfd2, &dbd2);
    vfd = vfd2;
    dbd = (vfsdbd_t *)dbd2;

    /*
     * Which vnode did I fault for?
     *
     * Note:  Code in hdl_pfault (S800) depends on the algorithm
     * used here for hashing pages in the page cache.  This
     * should probably be a future VOP function so that the
     * details aren't exported to that routine.
     */
    if (dbd->dbd_type == DBD_FSTORE)
	filevp = rp->r_fstore;
    else
	filevp = rp->r_bstore;
    vmemp_unlockx();		/* free up VM empire for a microsec */

    return VOPX_PAGEIN(filevp, prp, wrt, space, vaddr, ret_startindex);
}

int nux_pageout(prp, start, end, flags)
    preg_t *prp;
    size_t start, end;
    int flags;
{
    reg_t *rp = prp->p_reg;
    struct vnode *filevp = rp->r_bstore;

    return VOPX_PAGEOUT(filevp, prp, start, end, flags);
}
