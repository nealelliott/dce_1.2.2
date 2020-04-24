/* Copyright (C) 1996 Transarc Corporation - All rights reserved. */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xvfs_vfs2os.c,v $
 * Revision 1.1.118.1  1996/10/02  19:03:34  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:58  damon]
 *
 * $EndLog$
 */

/*
 * This module is a glue layer used by the Protocol Exporter to export the
 * HP/UX native ("ufs") file system.  It consists of functions that meet the
 * specs of the X-ops and that call N-ops; the N-ops are the HP/UX ufs
 * vnode ops.
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/volume.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/dacl.h>
#include <dcedfs/osi_user.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/debug.h>
#include <dcedfs/direntry.h>
#include <ufs/inode.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/xvnode/HPUX/xvfs_vfs2os.c,v 1.1.118.1 1996/10/02 19:03:34 damon Exp $")

/* Forward declarations */

int xufs_open(), xufs_close(), xufs_rdwr(), xufs_ioctl(), xufs_select();
int xufs_getattr(), xufs_setattr(), xufs_access(), xufs_lookup();
int xufs_create(), xufs_remove(), xufs_link(), xufs_rename(), xufs_mkdir();
int xufs_rmdir(), xufs_readdir(), xufs_symlink(), xufs_readlink();
int xufs_fsync(), xufs_inactive(), xufs_lockctl(), xufs_fid();
int xufs_setacl(), xufs_getacl(), xufs_afsfid(), xufs_getvolume();
int xufs_getlength();

int xufs_panic();
void xufs_void_panic();

/* Vector of X-ops */

struct xvfs_xops xufs_ops = {
    xufs_open,
    xufs_close,
    xufs_rdwr,
    xufs_ioctl,
    xufs_select,
    xufs_getattr,
    xufs_setattr,
    xufs_access,
    xufs_lookup,
    xufs_create,
    xufs_remove,
    xufs_link,
    xufs_rename,
    xufs_mkdir,
    xufs_rmdir,
    xufs_readdir,
    xufs_symlink,
    xufs_readlink,
    xufs_fsync,
    xufs_inactive,
    xufs_panic,		/* bmap */
    xufs_panic,		/* strategy */
    xufs_panic,		/* ustrategy */
    xufs_panic,		/* bread */
    xufs_panic,		/* brelse */
    xufs_lockctl,
    xufs_fid,
    xufs_panic,		/* hold */
    xufs_panic,		/* rele */
    xufs_setacl,
    xufs_getacl,
    xufs_afsfid,
    xufs_getvolume,
    xufs_getlength,
    xufs_panic,		/* map (SunOS, AIX) */
    xufs_panic,		/* unmap (AIX) */
    xufs_panic,		/* reclaim (OSF/1) */
    xufs_panic,		/* various SunOS ops ... */
    xufs_panic,
    xufs_panic,
    xufs_void_panic,
    xufs_void_panic,
    xufs_panic,
    xufs_panic,
    xufs_panic,
    xufs_panic,
    xufs_panic,
    xufs_panic,
    xufs_panic,		/* ... last of SunOS ops */
    xufs_panic,		/* pagein (HP/UX) */
    xufs_panic,		/* pageout (HP/UX) */
    xufs_panic,		/* SunOS: setfl */
    xufs_void_panic,	/* SunOS: dispose */
    xufs_panic,		/* SunOS: setsecattr */
    xufs_panic		/* SunOS: getsecattr */
};

/*
 * swapCred, restoreCred - The UX ufs routines take but ignore the "cred"
 * argument, so we mash/restore the cred into the current uarea here.
 */
static void swapCred(credp, savedcredp)
	struct ucred *credp;
     	struct ucred **savedcredp;
{
	crhold(credp);
	*savedcredp = u.u_cred;	  
	u.u_cred = credp;
}

static void restoreCred(credp, savedcredp)
	struct ucred *credp;
	struct ucred *savedcredp;
{
        crfree(credp);
	u.u_cred = savedcredp;
}

/* special routine called from HPUX/dirlook to override/bypass the glue vnodeops */
int xafs_access(vp, mode, cred)
	struct vnode *vp;
	int mode;
	struct ucred *cred;
{
	int (*vn_access)(struct vnode *vp, int mode, struct ucred *cred);
	
	/*
	 * if we have a converted vnode, we want to bypass the glue
	 * ops (in VOP/VOPO) and call through to the native ops (in VOPN).
	 * if not converted, call through the VOP/VOPO ops, like the
	 * caller was trying to do before we jumped in!
	 */
	AFSLOG(XVFS_DEBUG, 1, ("xafs_access: vp %x  fstype %x  converted %x\n", vp, vp->v_fstype, IS_CONVERTED(vp)));
	vn_access = (IS_CONVERTED(vp) ?
		/* VOPN_ACCESS */     ((struct xvfs_vnodeops *)vp->v_op)->nops.vn_access : 
		/* VOP_ACCESS  */     vp->v_op->vn_access);
	
	return((*vn_access)(vp, mode, cred));

}

int xufs_rdwr(vp, uiop, rw, ioflag, credp)
    struct vnode *vp;
    struct uio *uiop;
    enum uio_rw rw;
    int ioflag;
    struct ucred *credp;
{
    long code;
    struct ucred *savedcredp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_rdwr: vp=%x, rw=%d\n", vp, rw));
    u.u_error = 0; /* clean up as if syscall() has just started */
    swapCred(credp, &savedcredp);	
    code = VOPN_RDWR(vp, uiop, rw, ioflag, credp);
    restoreCred(credp, savedcredp);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_rdwr: code = %d\n", code));
    return code;
}
 
int xufs_getattr(vp, xvattrp, flag, credp)
    struct vnode *vp;
    struct xvfs_attr *xvattrp;
    int flag;				/* indicates extended attributes */
    struct ucred *credp;
{
    long code;
    enum vsync vsync = VASYNC;
    struct afsFid Fid;
    long afsAccess;
    struct ucred *savedcredp;	
	  
    u.u_error = 0; /* clean up as if syscall() has just started */
    if (flag)
	/* XXX REMOVE THIS!! XXX */
	bzero(xvattrp, sizeof(struct xvfs_attr));
    else
	bzero(xvattrp, sizeof(struct vattr));

    AFSLOG(XVFS_DEBUG, 1, ("xufs_getattr: vp=%x\n", vp));
    swapCred(credp, &savedcredp);	

    code = VOPN_GETATTR(vp, xvattrp, credp, vsync);

    if (!flag) {
        restoreCred(credp, savedcredp);
	return(code);    
    }

    /*
     * Fill the ep_attrs fields: In most cases they're not used by 
     * the UFS implementation
     */
    if (!VOPX_AFSFID(vp, &Fid, 0)) {			/* Ignore errors */
	AFS_hset64(xvattrp->xvattr.fileID, Fid.Vnode, Fid.Unique);
    }
    /*
     * Setup xvattr.callerAccess
     */
    afsAccess = 0;
    code = VOPN_ACCESS(vp, VREAD, credp);
    if (code == 0) 
        afsAccess |= dacl_perm_read;
    code = VOPN_ACCESS(vp, VWRITE, credp);
    if (code == 0) {
       if (OSI_ISDIR(vp)) 
	   afsAccess |= (dacl_perm_insert|dacl_perm_delete|dacl_perm_write);
       else 
	   afsAccess |= dacl_perm_write;
    }
    code = VOPN_ACCESS(vp, VEXEC, credp);
    if (code == 0) 
        afsAccess |= dacl_perm_execute;
    code = 0;

    if (xvattrp->vattr.va_uid == credp->cr_uid || xvfs_IsAdminGroup(credp) ||
	credp->cr_uid == 0 )
        afsAccess |= dacl_perm_control;

    AFS_hset64(xvattrp->xvattr.dataVersion, xvattrp->vattr.va_mtime.tv_sec,
	       xvattrp->vattr.va_mtime.tv_usec);
    xvattrp->xvattr.author = xvattrp->vattr.va_uid;
    xvattrp->xvattr.callerAccess = afsAccess;
    /*
     * Set up the minimum access rights for any caller, ie., anonAccess.
     * Note that anonAccess is an optimization used by CM for a "quick" check.
     */
    afsAccess = 0;
    if (((xvattrp->vattr.va_mode & 07) & R_OK)        &&  /* AND "other" */
	(((xvattrp->vattr.va_mode >> 3) & 07) & R_OK) &&  /* AND "group" */
	(((xvattrp->vattr.va_mode >> 6) & 07) & R_OK))    /* AND "owner" */
        afsAccess |= dacl_perm_read;

    if (((xvattrp->vattr.va_mode & 07) & W_OK)        &&  /* AND "other" */
	(((xvattrp->vattr.va_mode >> 3) & 07) & W_OK) &&  /* AND "group" */
	(((xvattrp->vattr.va_mode >> 6) & 07) & W_OK))    /* AND "owner" */
       if (OSI_ISDIR(vp))
	   afsAccess |= (dacl_perm_insert | dacl_perm_delete);
       else
	   afsAccess |= dacl_perm_write;

    if (((xvattrp->vattr.va_mode & 07) & X_OK)        &&  /* AND "other" */
	(((xvattrp->vattr.va_mode >> 3) & 07) & X_OK) &&  /* AND "group" */
	(((xvattrp->vattr.va_mode >> 6) & 07) & X_OK))    /* AND "owner" */
        afsAccess |= dacl_perm_execute;

    xvattrp->xvattr.anonAccess = afsAccess;
    xvattrp->xvattr.parentVnode = -1;
    xvattrp->xvattr.parentUnique = -1;
    xvattrp->xvattr.fstype = AG_TYPE_UFS;

out:
    AFSLOG(XVFS_DEBUG, 1, ("xufs_getattr: code = %d\n", code));
    restoreCred(credp, savedcredp);
    return code;
}

int xufs_setattr(vp, xvattrp, xflag, credp)
    struct vnode *vp;
    struct xvfs_attr *xvattrp;
    int xflag;				/* indicates extended attributes */
    struct ucred *credp;
{
    long code;
    struct ucred *tcredp;
    int null_time = 0;
    struct ucred *savedcredp; 
    u_short saved_mode, saved_uid, saved_gid;
    u_long  saved_size;
    int do_it;
    struct vattr tmp_vattr;
	
    AFSLOG(XVFS_DEBUG, 1, ("xufs_setattr: vp=%x\n", vp));

    u.u_error = 0; /* clean up as if syscall() has just started */

    if (xflag & XVN_SET_TIMES_NOW)
	null_time = 1;

    /* UX ufs_setattr() has the annoying habit of refusing to set the
     * size unless "we" have write access to the file, are root (i.e.,
     * dfs administrator).  The problem: the /bin/cp command creates the
     * file with mode rights of 0, writes to it, closes it and then chmod()'s
     * it.  The data write from the CM is accompanied by a size setting.
     *
     * If we're trying to change the size, do it NOW as root ... we know
     * we're root because only the protocol exporter calls us.  Doing
     * it this way seems no slower than checking first to see first if
     * we "will" be allowed to do the change.
     */
    saved_size = xvattrp->vattr.va_size;
    if (xvattrp->vattr.va_size != -1) {
	vattr_null(&tmp_vattr);
	tmp_vattr.va_size = saved_size;
	/* Don't set it again below */
	xvattrp->vattr.va_size = -1;
	code = VOPN_SETATTR(vp, &tmp_vattr, osi_getucred(), null_time);
	if (code) goto done;
	/* If we thought it likely that we might be setting ONLY the
	 * size, we could check for that and goto done if so. */
    }

    /* 
     * If the caller is the member of the system admin group. Let the caller 
     * assume root's privileges for the duration of setattr.
     *
     * Don't bash cr_uid it in place, since *credp is a shared structure.
     */
    crhold(credp);
    tcredp = crcopy(credp);	/* crfree's credp as side effect */
    if (xvfs_IsAdminGroup(credp)) 
       tcredp->cr_uid = 0;

    swapCred(tcredp, &savedcredp);	
    /*
     * UX ufs_setattr will set mode/uid/gid before setting other state,
     * which will cause failures when setting multiple attributes
     * at the same time.  So set the attributes in 2 phases: everything
     * other than mode/uid/gid ... and then mode/uid/gid.
     */
    do_it = 0;
    if (xvattrp->vattr.va_mode != -1) {
	do_it = 1;
	saved_mode = xvattrp->vattr.va_mode;
    }
    if (xvattrp->vattr.va_uid != -1) {
	do_it = 1;
	saved_uid = xvattrp->vattr.va_uid;
    }
    if (xvattrp->vattr.va_gid != -1) {
	do_it = 1;
	saved_gid = xvattrp->vattr.va_gid;
    }
    if (do_it) {
	/* we are setting mode/uid/gid ... do everything else first */
	vattr_null(&tmp_vattr);
	xvattrp->vattr.va_mode = -1;
	xvattrp->vattr.va_uid  = -1;
	xvattrp->vattr.va_gid  = -1;
	code = 0;

	/* set everything other than mode/uid/gid  -- if there's anything */
	if (bcmp(&tmp_vattr, &xvattrp->vattr, sizeof(struct vattr)))
	    code = VOPN_SETATTR(vp, &xvattrp->vattr, tcredp, null_time);

	/* make sure to put these back ... on the off chance that our
	 * caller is relying on the structure he passed in */
	xvattrp->vattr.va_mode = saved_mode;
	xvattrp->vattr.va_uid  = saved_uid;
	xvattrp->vattr.va_gid  = saved_gid;

	/* now set mode/uid/gid */
	if (! code) {
	    tmp_vattr.va_mode = saved_mode;
	    tmp_vattr.va_uid  = saved_uid;
	    tmp_vattr.va_gid  = saved_gid;
	    code = VOPN_SETATTR(vp, &tmp_vattr, tcredp, null_time);
	}
    } else
	/* not setting the mode, uid or gid */
	code = VOPN_SETATTR(vp, &xvattrp->vattr, tcredp, null_time);

    restoreCred(tcredp, savedcredp);

    crfree(tcredp);		/* free credential */
done:
    /* put this back in case we changed it above */
    xvattrp->vattr.va_size  = saved_size;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_setattr: code = %d\n", code));
    return code;
}

int xufs_access(vp, mode, credp)
    struct vnode *vp;
    int mode;
    struct ucred *credp;
{
    long code;
    struct ucred *savedcredp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_access: vp=0x%x, mode=%d\n", vp, mode));
    u.u_error = 0; /* clean up as if syscall() has just started */
    swapCred(credp, &savedcredp);	
    code = VOPN_ACCESS(vp, mode, credp);
    restoreCred(credp, savedcredp);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_access: code = %d\n", code));
    return(code);
}



int xufs_lookup(dvp, namep, vpp, credp)
    struct vnode *dvp;
    char *namep;
    struct vnode **vpp;
    struct ucred *credp;
{
    long code;
    struct vnode *mvp = dvp; /* xxx 'mounted' vnode ptr */
    struct ucred *savedcredp;	

    AFSLOG(XVFS_DEBUG, 1, ("xufs_lookup: dvp=%x, name=%s\n", dvp, namep));

    u.u_error = 0; /* clean up as if syscall() has just started */
    swapCred(credp, &savedcredp);	
    code = VOPN_LOOKUP(dvp, namep, vpp, credp, mvp);
    restoreCred(credp, savedcredp);
    if (!code) {
	/* 
	 * We convert its v_op pointer from the standard ufs_vnodeops 
	 * to the extended xvnodeops.
	 */
	xvfs_convert(*vpp);
    }

    AFSLOG(XVFS_DEBUG, 1, ("xufs_lookup: code = %d\n", code));
    return code;
}

int xufs_create(dvp, namep, vattrp, excl, mode, vpp, credp)
    struct vnode *dvp;
    char *namep;
    struct vattr *vattrp;
    enum vcexcl excl;
    int mode;
    struct vnode **vpp;
    struct ucred *credp;
{
    char *vinfop = 0;
    long code, flags = 0;
    struct ucred *savedcredp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_create: dvp=%x, name=%s, mode=%d\n", 
			   dvp, namep, mode));

    u.u_error = 0; /* clean up as if syscall() has just started */
    /*
     * can't create directories. use mkdir.
     */
    if (vattrp->va_type == VDIR)
        return (EISDIR);

    swapCred(credp, &savedcredp);	
    code = VOPN_CREATE(dvp, namep, vattrp, excl, mode, vpp, credp);
    restoreCred(credp, savedcredp);

    if (!code && *vpp) {
	/* 
	 * We convert its v_op pointer from the standard ufs_vnodeops 
	 * to the extended xvnodeops.
	 */
	xvfs_convert(*vpp);
    }
    AFSLOG(XVFS_DEBUG, 1, ("xufs_create: code = %d\n", code));
    return code;
}


int xufs_remove(dvp, namep, credp)
    struct vnode *dvp;
    char *namep;
    struct ucred *credp;
{
    struct ucred *savedcredp;
    struct vnode *vp;
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_remove: dvp=%x, name=%s\n", dvp, namep));
    u.u_error = 0; /* clean up as if syscall() has just started */
    swapCred(credp, &savedcredp);	
    code = VOPN_REMOVE(dvp, namep, credp);
    restoreCred(credp, savedcredp);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_remove: code = %d\n", code));
    return code;
}


int xufs_link(fvp, tdvp, namep, credp)
    struct vnode *fvp, *tdvp;
    char *namep;
    struct ucred *credp;
{
    long code;
    struct ucred *savedcredp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_link: tdvp=%x, fvp=%x, name=%s\n", tdvp, fvp, namep));
    u.u_error = 0; /* clean up as if syscall() has just started */
    swapCred(credp, &savedcredp);	
    code = VOPN_LINK(fvp, tdvp, namep, credp);
    restoreCred(credp, savedcredp);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_link: code = %d\n", code));
    return (code);
}


int xufs_rename(odvp, onamep, ndvp, nnamep, credp)
    struct vnode *odvp, *ndvp;
    char *onamep, *nnamep;
    struct ucred *credp;
{
    long code;
    struct ucred *savedcredp;

    AFSLOG(XVFS_DEBUG,1,("xufs_rename: odvp=%x, oname=%s, ndvp=%x, nname=%s\n",
	   odvp, onamep, ndvp, nnamep));
    u.u_error = 0; /* clean up as if syscall() has just started */
    swapCred(credp, &savedcredp);	
    code = VOPN_RENAME(odvp, onamep, ndvp, nnamep, credp);
    restoreCred(credp, savedcredp);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_rename: code = %d\n", code));
    return code;
}


int xufs_mkdir(dvp, namep, vattrp, vpp, credp)
    struct vnode **vpp, *dvp;
    char *namep;
    struct vattr *vattrp;
    struct ucred *credp;
{
    long code;
    struct ucred *savedcredp;	

    AFSLOG(XVFS_DEBUG, 1, ("xufs_mkdir: dvp=%x, name=%s\n", dvp, namep));

    u.u_error = 0; /* clean up as if syscall() has just started */
    swapCred(credp, &savedcredp);	
    code = VOPN_MKDIR(dvp, namep, vattrp, vpp, credp);
    restoreCred(credp, savedcredp);
    if (!code && *vpp)
        xvfs_convert(*vpp);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_mkdir: code = %d\n", code));
    return code;
}


int xufs_rmdir(dvp, namep, cdp, credp)
    struct vnode *dvp;
    char *namep;
    struct vnode *cdp;
    struct ucred *credp;
{
    struct vnode *vp;
    long code;
    struct ucred *savedcredp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_rmdir: dvp=%x, name=%s\n", dvp, namep));
    u.u_error = 0; /* clean up as if syscall() has just started */
    swapCred(credp, &savedcredp);	
    code = VOPN_RMDIR(dvp, namep, credp);
    restoreCred(credp, savedcredp);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_rmdir: code = %d\n", code));
    return code;
}


#define ROUNDUP4(x) (((x) + 3) & (-4))

int xufs_readdir(vp, uiop, credp, eofp, isPX)
    struct vnode *vp;
    struct uio *uiop;
    struct ucred *credp;
    int *eofp;
    int isPX;
{
    long code;
    struct ucred *savedcredp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_readdir: vp=%x\n", vp));
    u.u_error = 0; /* clean up as if syscall() has just started */
    swapCred(credp, &savedcredp);

    if (isPX) {
	/*
	 * If called from the PX, we are expected to return the format
	 * defined by dir_minxentry, rather than the native format.
	 *
	 * Could we be called with isPX == 0?  In principle, yes, if we
	 * were called from the CM express ops.  These haven't worked for
	 * some time, but we still allow for the possibility.
	 *
	 * We request a whole directory page (= DEV_BSIZE bytes) from
	 * UFS, so that it will not play any tricks like compressing out
	 * the first (empty) entry.  Then we select the part that we were
	 * asked for.
	 */
	int count, offset, origoffset, aoffset;
	int entryLen, bytes;
	caddr_t dirbuff;
	struct uio tuio;
	struct iovec tiovec;
	struct dirent *ux_entryP;	/* dir entry from HP/UX UFS */
	struct dir_minxentry *xentryP;	/* dir entry for our PX */

	count = uiop->uio_resid;
	origoffset = uiop->uio_offset;
	offset = aoffset = origoffset & ~(DEV_BSIZE-1);
	dirbuff = osi_Alloc (DEV_BSIZE);

	tuio.uio_seg = UIOSEG_KERNEL;
	tuio.uio_fpflags = 0;
	tuio.uio_iov = &tiovec;
	tuio.uio_iovcnt = 1;

	xentryP = (struct dir_minxentry *) uiop->uio_iov->iov_base;

	while (1) {
	    tuio.uio_offset = offset;
	    tuio.uio_resid = tiovec.iov_len = DEV_BSIZE;
	    tiovec.iov_base = dirbuff;

	    if (code = VOPN_READDIR(vp, &tuio, credp))
		break;

	    bytes = DEV_BSIZE - tuio.uio_resid;
	    if (bytes == 0)
		break;

	    ux_entryP = (struct dirent *) dirbuff;
	    while (bytes > 0) {
		/* Check if entry is valid and is past our starting point */
		if (ux_entryP->d_fileno && aoffset >= origoffset) {
		    /* Compute output entry size */
		    entryLen = sizeof (struct dir_minxentry) +
				    ROUNDUP4 (ux_entryP->d_namlen + 1);
		    /* If entry won't fit, exit the loop. */
		    if (entryLen > count)
			goto done;
		    count -= entryLen;

		    /* Fill in PX-format entry */
		    xentryP->offset = aoffset + ux_entryP->d_reclen;
		    xentryP->inode = ux_entryP->d_fileno;
		    xentryP->namelen = ux_entryP->d_namlen;
		    xentryP->recordlen = entryLen;
		    strcpy((char *)xentryP + sizeof(struct dir_minxentry),
			   ux_entryP->d_name);

		    /* Next PX-format entry */
		    xentryP = (struct dir_minxentry *) ((char *)xentryP + entryLen);
		}
		aoffset += ux_entryP->d_reclen;
		bytes -= ux_entryP->d_reclen;

		/* Next HP/UX-format entry */
		ux_entryP = (struct dirent *) ((char *)ux_entryP + ux_entryP->d_reclen);
	    }
	    /* Next DEV_BSIZE block */
	    offset += DEV_BSIZE;
	    aoffset = offset;
	}
done:
	osi_Free (dirbuff, DEV_BSIZE);
	uiop->uio_resid = count;
	uiop->uio_offset = aoffset;
    } else
	code = VOPN_READDIR(vp, uiop, credp);
    restoreCred(credp, savedcredp);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_readdir: code = %d\n", code));
    return (code);
}


int xufs_symlink(dvp, namep, vattrp, linkcontentsp, credp)
    struct vnode *dvp;
    char *namep, *linkcontentsp;
    struct vattr *vattrp;
    struct ucred *credp;
{
    long code;
    struct vnode *vp;
    struct vnode *mvp = dvp; /* xxx */
    struct ucred *savedcredp;	
    
    AFSLOG(XVFS_DEBUG, 1, ("xufs_symlink: dvp=%x, name=%s\n", dvp, namep));
    u.u_error = 0; /* clean up as if syscall() has just started */
    swapCred(credp, &savedcredp);	
    code = VOPN_SYMLINK(dvp, namep, vattrp, linkcontentsp, credp);
    if (!code) {
        /* obtain a vnode ptr to the just created symlink */
	code = VOPN_LOOKUP(dvp, namep, &vp, credp, mvp);
	if (!code) {
	    xvfs_convert(vp);
	    vattrp->va_type = -1;
	    vattrp->va_rdev = -1; /* HPUX ufs_symlink zeroes this for some reason */
	    code = VOPX_SETATTR(vp, vattrp, 0, credp);
	    VN_RELE(vp);
	}
    }
    restoreCred(credp, savedcredp);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_symlink: code = %d\n", code));
    return (code);
}


int xufs_readlink(vp, uiop, credp)
    struct vnode *vp;
    struct uio *uiop;
    struct ucred *credp;
{
    long code;
    struct ucred *savedcredp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_readlink: vp=%x\n", vp));
    u.u_error = 0; /* clean up as if syscall() has just started */
    swapCred(credp, &savedcredp);	
    code = VOPN_READLINK(vp, uiop, credp);
    restoreCred(credp, savedcredp);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_readlink: code = %d\n", code));
    return (code);
}


int xufs_afsfid(vp, afsFidp, wantv) 
    struct vnode *vp;
    struct afsFid *afsFidp;
    int wantv;				/* want volume field filled in, too */
{
    struct volume *volp;
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_afsfid: vp=%x(%x) = ", vp, vp->v_op));
    u.u_error = 0; /* clean up as if syscall() has just started */
    code = xufs_fid(vp, &afsFidp);
    if (!code && wantv) {
	afsFidp->Cell = tkm_localCellID;
	code = xvfs_GetVolume(vp, &volp);
        if (!code) {
	    afsFidp->Volume = volp->v_volId;
            xvfs_PutVolume(volp);
        } else
	    AFS_hzero(afsFidp->Volume);
    }
    AFSLOG(XVFS_DEBUG, 1, ("xufs_fid: code=%d [vnode=0x%x, unique=0x%x]\n", 
	   code, afsFidp->Vnode, afsFidp->Unique));
    return code;
}


/*
 * This routine is in the VOPX vector,
 * but is really only called by xufs_afsfid().
 */
int xufs_fid(vp, afsFidpp)
    struct vnode *vp;
    struct afsFid **afsFidpp;
{
	struct fid *fidp;
	int code;

    u.u_error = 0; /* clean up as if syscall() has just started */
	code = VOPN_FID(vp, &fidp);
	if (!code) {
		(*afsFidpp)->Vnode = ((struct ufid *)fidp)->ufid_ino;
		(*afsFidpp)->Unique = ((struct ufid *)fidp)->ufid_gen;
		freefid(fidp);
	}
	return (code);
}


/* 
 * The call to convert a vnode pointer into a held volume pointer.
 *
 * XXXXXX UFS SPECIFIC - FIX THIS!!!! XXXXX
 */
int xufs_getvolume(vp, volpp)
    struct vnode *vp;
    struct volume **volpp; 
{
    if (vol_FindVfs(vp->v_vfsp, volpp))
      return EIO;
    return 0;
}



int xufs_getacl(vp, AccessListp, w, credp) 
    struct vnode *vp;
    struct afsACL *AccessListp;
    long w;
    struct ucred *credp;
{
    AFSLOG(XVFS_DEBUG, 1, ("xufs_getacl: vp=0x%x\n", vp));

    /*
     * Currently UFS doesn't handle ACL. So we simply return the following
     */
    return ENOTTY;
}

int xufs_setacl(vp, AccessListp, svp, w, credp) 
    struct vnode *vp;
    struct afsACL *AccessListp;
    struct vnode *svp;
    long w;
    struct ucred *credp;
{
    AFSLOG(XVFS_DEBUG, 1, ("xufs_setacl: vp=0x%x\n", vp));

    /*
     * Currently UFS doesn't handle ACL. So we simply return the following
     */
    return ENOTTY;
}



int xufs_quota(vp, quotaStr, quotaLen, credp)
     struct vnode *vp;
     char *quotaStr;
     long quotaLen;
     struct ucred *credp; 
{
    long code = EINVAL;
    AFSLOG(XVFS_DEBUG, 1, ("xufs_quota: vp=%x, quotaStr='%s'\n", vp,
			   quotaStr));
    AFSLOG(XVFS_DEBUG, 1, ("xufs_quota: code=%d\n", code));
    return code;
}


int xufs_getlength(vp, lenp, cred) 
    struct vnode *vp;
    long *lenp;
    struct ucred *cred;
{
    struct vattr vattr;
    long code;
    enum vsync vsync = VASYNC;
    struct ucred *savedcredp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_getlength: vp=0x%x ", vp));

    *lenp = 0;
    u.u_error = 0; /* clean up as if syscall() has just started */
    swapCred(cred, &savedcredp);	
    code = VOPN_GETATTR(vp, &vattr, cred, vsync);
    if (!code) {
	*lenp = (long) vattr.va_size;
    }
    restoreCred(cred, savedcredp);
    AFSLOG(XVFS_DEBUG, 1, ("len=%d (code = %d)\n", *lenp, code));
    return(code);
}



int xufs_fsync(vp, credp)
    struct vnode *vp;
    struct ucred *credp;
{
    long code;
    int ino_chg = 0;
    struct ucred *savedcredp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_fsync: vp=%x\n", vp));
    u.u_error = 0; /* clean up as if syscall() has just started */
    swapCred(credp, &savedcredp);	
    code = VOPN_FSYNC(vp, credp, ino_chg);
    restoreCred(credp, savedcredp);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_fsync: code=%d\n", code));
    return code;
}


/*
 * File/record locks
 *
 * We do not actually set file/record locks in the Protocol Exporter; that is
 * all done by the client, after the exchange of appropriate lock tokens.
 * However, the TKC (glue for local file systems) will sometimes call this
 * file/record lock op, passing the F_GETLK flag, to determine if it is
 * OK to revoke a lock token.
 */
int xufs_lockctl(vp, flockp, cmd, credp, id, off)
    struct vnode *vp;
    struct flock *flockp;
    int cmd;
    osi_cred_t *credp;
    int id;
    off_t off;
{
    struct file f;
    off_t LB, UB;
    struct ucred *savedcredp;
    long code;

    /* Choose f_flag to allow locks */
    f.f_flag = F_READ | F_WRITE;
    f.f_offset = off;
    /* Assume whence = 0; choose LB and UB such that start is OK, end is OK. */
    LB = flockp->l_start;
    if (LB > MAX_LOCK_SIZE) LB = MAX_LOCK_SIZE;
    if (flockp->l_len == 0)
  	UB = MAX_LOCK_SIZE;
    else
	UB = LB + flockp->l_len;

    swapCred(credp, &savedcredp);	
    code = VOPN_LOCKCTL(vp, flockp, cmd, credp, &f, LB, UB);
    restoreCred(credp, savedcredp);
    return code;
}


int xufs_nosys()
{
    AFSLOG(XVFS_DEBUG, 0, 
	   ("xufs_nosys: UNSUPPORTED... EXITING\n"));
    return EINVAL;
}


int xufs_vfsgetvolume (vfsp, volpp)
    struct vfs *vfsp;			/* virtual file system */
    struct volume **volpp;		/* where to put volume ptr */
{
    if (vol_FindVfs (vfsp, volpp))
      return EIO;
    return 0;
}

/*
 * The following functions are not called from the Protocol Exporter, but
 * may eventually be called from the CM express ops, if those are ever
 * gotten to work again.  In that event, these functions will have to be
 * implemented. For now, they are just no-ops.
 */
int xufs_open()  {;}
int xufs_close() {;}
int xufs_ioctl() {;}
int xufs_select()  {;}
int xufs_inactive()  {;}
int xufs_bmap() {;}
int xufs_strategy() {;}
int xufs_ustrategy() {;}
int xufs_bread() {;}
int xufs_brelse() {;}
int xufs_cmp() {;}
int xufs_hold() {;}
int xufs_rele() {;}
int xufs_lock() {;}
int xufs_unlock() {;}
int xufs_setlength() {;}

int xufs_panic() {panic("Invalid X-op");}
void xufs_void_panic() {panic("Invalid X-op");}
