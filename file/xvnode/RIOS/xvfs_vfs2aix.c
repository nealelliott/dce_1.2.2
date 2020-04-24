/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xvfs_vfs2aix.c,v $
 * Revision 1.1.834.1  1996/10/02  19:04:06  damon
 * 	New DFS from Transarc
 * 	[1996/10/01  18:59:38  damon]
 *
 * $EndLog$
 */
/* 
 * Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved 
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/basicincludes.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/volume.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/dacl.h>
#include <dcedfs/osi_user.h>
#include <dcedfs/osi_uio.h>
#include <sys/chownx.h>
#include <sys/access.h>
#include <sys/lkup.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/xvnode/RIOS/xvfs_vfs2aix.c,v 1.1.834.1 1996/10/02 19:04:06 damon Exp $")

/*
 * This module is a glue layer used by the Protocol Exporter to export the
 * AIX 3 jfs file system.  It consists of functions that meet the specs of
 * the X-ops and that call N-ops; the N-ops are the AIX 3 jfs vnode ops.
 */

int xufs_rdwr(vp, uiop, rw, ioflag, credp)
    struct vnode *vp;
    struct uio *uiop;
    enum uio_rw rw;
    int ioflag;
    osi_cred_t *credp;
{
    osi_cred_t *savedcredp;
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_rdwr: vp=%x, rw=%d\n", vp, rw));
    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    ioflag |= FNDELAY;
    code = VOPN_RDWR(vp, rw, ioflag, uiop, NULL, NULL);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_rdwr: code = %d\n", code));

    restoreCred(savedcredp);		 /* restore the original credentials */
    return code;
}


int xufs_getattr(vp, xvattrp, flag, credp)
    struct vnode *vp;
    struct xvfs_attr *xvattrp;
    int flag;				/* indicates extended attributes */
    osi_cred_t *credp;
{
    struct afsFid Fid;
    osi_cred_t *savedcredp;
    long afsAccess, code = 0;

    if (flag)
	/* XXX REMOVE THIS!! XXX */
	bzero(xvattrp, sizeof(struct xvfs_attr));
    else
	bzero(xvattrp, sizeof(struct vattr));

    AFSLOG(XVFS_DEBUG, 1, ("xufs_getattr: vp=%x\n", vp));

    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    code = VOPN_GETATTR(vp, (struct vattr *)xvattrp);

    if (!flag) {
        restoreCred(savedcredp); /* restore the original credentials */
	return code;    
    } else {
	xvattrp->xvattr.serverModTime.sec = xvattrp->vattr.va_ctime.tv_sec;
	xvattrp->xvattr.serverModTime.usec = xvattrp->vattr.va_ctime.tv_usec;
    }
    /*
     * Fill the ep_attrs fields: In most cases they're not used by 
     * the UFS implementation
     */
    if (!VOPX_AFSFID(vp, &Fid, 0)) {			/* Ignore errors */
	AFS_hset64(xvattrp->xvattr.fileID, Fid.Vnode, Fid.Unique);
    }
    /*
     * Setup the callerAccess's bit in the form of "RWXCID"
     */
    afsAccess = 0;
    code = VOPN_ACCESS(vp, R_ACC, ACC_SELF);
    if (code == 0) 
        afsAccess |= dacl_perm_read;
    code = VOPN_ACCESS(vp, W_ACC, ACC_SELF);
    if (code == 0) {
       if (OSI_ISDIR(vp)) 
	   afsAccess |= (dacl_perm_insert|dacl_perm_delete|dacl_perm_write);
       else 
	   afsAccess |= dacl_perm_write;
    }
    code = VOPN_ACCESS(vp, X_ACC, ACC_SELF);
    if (code == 0) 
        afsAccess |= dacl_perm_execute;
    code = 0;

    restoreCred(savedcredp);	 /* restore the original credentials */

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
    if (((xvattrp->vattr.va_mode & 07) & R_ACC)        &&  /* AND "other" */
	(((xvattrp->vattr.va_mode >> 3) & 07) & R_ACC) &&  /* AND "group" */
	(((xvattrp->vattr.va_mode >> 6) & 07) & R_ACC))    /* AND "owner" */
        afsAccess |= dacl_perm_read;

    if (((xvattrp->vattr.va_mode & 07) & W_ACC)        &&  /* AND "other" */
	(((xvattrp->vattr.va_mode >> 3) & 07) & W_ACC) &&  /* AND "group" */
	(((xvattrp->vattr.va_mode >> 6) & 07) & W_ACC))    /* AND "owner" */
       if (OSI_ISDIR(vp))
	   afsAccess |= (dacl_perm_insert | dacl_perm_delete);
       else
	   afsAccess |= dacl_perm_write;

    if (((xvattrp->vattr.va_mode & 07) & X_ACC)        &&  /* AND "other" */
	(((xvattrp->vattr.va_mode >> 3) & 07) & X_ACC) &&  /* AND "group" */
	(((xvattrp->vattr.va_mode >> 6) & 07) & X_ACC))    /* AND "owner" */
        afsAccess |= dacl_perm_execute;

    xvattrp->xvattr.anonAccess = afsAccess;
    xvattrp->xvattr.parentVnode = -1;
    xvattrp->xvattr.parentUnique = -1;
    xvattrp->xvattr.fstype = AG_TYPE_UFS;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_getattr: code = %d\n", code));
out:
    return code;
}


int xufs_setattr(vp, xvattrp, xflag, credp)
    struct vnode *vp;
    struct xvfs_attr *xvattrp;
    int xflag;				/* indicates extended attributes */
    osi_cred_t *credp;
{
    long code = 0, flag = 0;
    osi_cred_t *savedcredp;
    struct vattr tvattr;
    struct timeval atime, mtime;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_setattr: vp=%x\n", vp));
    if ((xvattrp->vattr.va_nlink != -1) || (xvattrp->vattr.va_blocksize != -1) || 
	(xvattrp->vattr.va_rdev != -1) || (xvattrp->vattr.va_blocks != -1) || 
	(xvattrp->vattr.va_fsid != -1) || (xvattrp->vattr.va_nodeid != -1) ||
	((int)xvattrp->vattr.va_type != -1))
	return EINVAL;

    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    /*
     * Truncate file	
     */
    if (xvattrp->vattr.va_size != (long)-1) {
	if (OSI_ISDIR(vp)) {
	    code = EISDIR;
	    goto out;
	}
	/*
	 * We do the truncate here and we're all done (courtesy of aix's
	 * ftrunc call).
	 * NOTE, the caller should do the access check!
	 * Also check if we're growing the file, then we must write
	 * a byte at the end, until AIX 3.2.
	 */
#ifndef AFS_AIX32_ENV
	code = VOPN_GETATTR(vp, &tvattr);
	if (code) goto out;
	if (tvattr.va_size < xvattrp->vattr.va_size) {
	    /* properly growing file */
	    code = VOPN_FCLEAR(vp, 0, xvattrp->vattr.va_size-1, 1, (char *) 0);
	}
	else {
	    /* shrinking the file */
	    code = VOPN_FTRUNC(vp, 0, (u_long)xvattrp->vattr.va_size, (char *)0);
	}
#else	/* AIX 3.2 */
	code = VOPN_FTRUNC(vp, 0, (u_long)xvattrp->vattr.va_size, (char *)0);
#endif	/* AIX 3.2 */
	if (code) 
	   goto out;
    }
    /* 
     * If the caller is the member of the system admin group. Let the caller 
     * assume root's privileges for the duration of setattr.
     */
    if (xvfs_IsAdminGroup(credp)) 
       u.u_cred->cr_uid = 0;

    /* 
     * Change ownership 
     */
    if ((xvattrp->vattr.va_uid != -1) || (xvattrp->vattr.va_gid != -1)) {
	flag = T_OWNER_AS_IS | T_GROUP_AS_IS;
	if (xvattrp->vattr.va_uid != -1)
	   flag &= ~T_OWNER_AS_IS;
	if (xvattrp->vattr.va_gid != -1)
	   flag &= ~T_GROUP_AS_IS;
	code =  VOPN_SETATTR(vp, V_OWN, flag, xvattrp->vattr.va_uid,
			     xvattrp->vattr.va_gid);
	if (code) goto out;
    }

    /*
     * Change access modes
     */
    if (xvattrp->vattr.va_mode != (short)-1) {
	code = VOPN_SETATTR(vp, V_MODE, (int)xvattrp->vattr.va_mode, 0, 0);
	if (code) 
	    goto out;
    }

    /*
     * Change access/modify time
     */
    if ((xvattrp->vattr.va_atime.tv_sec != -1) || (xvattrp->vattr.va_mtime.tv_sec != -1)
	|| (xflag & XVN_SET_TIMES_NOW)) {
	struct vattr vattr;
	int flag = 0;

	if (!(xflag & XVN_SET_TIMES_NOW) &&
	    ((xvattrp->vattr.va_atime.tv_sec == -1) || (xvattrp->vattr.va_mtime.tv_sec == -1))) {
	    /*
	     * If only setting one of them must do a getattr so that we can
	     * get the proper value for the other time.
	     */
	    if (code = VOPN_GETATTR(vp, &vattr))
		goto out;
	}
	if (xflag & XVN_SET_TIMES_NOW) {
	    flag |= T_SETTIME;
	    atime.tv_sec = mtime.tv_sec = 0;
	} else {
	    atime = (xvattrp->vattr.va_atime.tv_sec == -1
		     ? vattr.va_atime : xvattrp->vattr.va_atime);
	    mtime = (xvattrp->vattr.va_mtime.tv_sec == -1
		     ? vattr.va_mtime : xvattrp->vattr.va_mtime);
	}
	code = VOPN_SETATTR(vp, V_UTIME, flag, atime.tv_sec, mtime.tv_sec);
    }
out:
    AFSLOG(XVFS_DEBUG, 1, ("xufs_setattr: code = %d\n", code));
    restoreCred(savedcredp);	 /* restore the original credentials */
    return (code);
}


int xufs_access(vp, mode, credp)
    struct vnode *vp;
    int mode;
    osi_cred_t *credp;
{
    long code = 0;
    osi_cred_t *savedcredp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_access: vp=%x, mode=%d\n", vp, mode));
    u.u_error = 0;  

    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    code = VOPN_ACCESS(vp, mode, ACC_SELF);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_access: code = %d\n", code));
    restoreCred(savedcredp);	 /* restore the original credentials */
    return code;
}


int xufs_lookup(dvp, namep, vpp, credp)
    struct vnode *dvp;
    char *namep;
    struct vnode **vpp;
    osi_cred_t *credp;
{
    long code = 0;
    osi_cred_t *savedcredp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_lookup: dvp=%x, name=%s\n", dvp, namep));
    /* XXX Should make sure we deal with a directory component! XXX */

    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    code = VOPN_LOOKUP(dvp, vpp, namep, 0);
    if (!code) {
	/* 
	 * We convert its v_op pointer from the standard ufs_vnodeops 
	 * to the extended xvnodeops.
	 */
	xvfs_convert(*vpp);
    }
    AFSLOG(XVFS_DEBUG, 1, ("xufs_lookup: code = %d\n", code));
    restoreCred(savedcredp);		 /* restore the original credentials */
    return code;
}


static int vttoif_table[] = { 0, S_IFREG, S_IFDIR, S_IFBLK, S_IFCHR,
			      S_IFLNK, S_IFSOCK, S_IFMT, S_IFIFO };
int xufs_create(dvp, namep, vattrp, excl, mode, vpp, credp)
    struct vnode *dvp;
    char *namep;
    struct vattr *vattrp;
    enum vcexcl excl;
    int mode;
    struct vnode **vpp;
    osi_cred_t *credp;
{
    char *vinfop = 0;
    long code, flags = 0;
    long amode = vttoif_table[(int)vattrp->va_type] | vattrp->va_mode;
    osi_cred_t *savedcredp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_create: dvp=%x, name=%s, mode=%d\n", 
			   dvp, namep, mode));

    swapCred(credp, &savedcredp);  	/* let u.u_cred assume new credp */
    CONV_MODE(mode, excl, flags);	/* conv from bsd to AIX mode bits */
    if (vattrp->va_type == VREG) {
	code = VOPN_CREATE(dvp, vpp, flags, namep, vattrp->va_mode, &vinfop);
	if (!code && !vattrp->va_size) 
	    code = vtognop (*vpp, vn_ftrunc)(*vpp, 0, 0, NULL);
    } else {
        code = VOPN_ACCESS(dvp, W_ACC, ACC_SELF);
	if (code == 0) {
	    if (xvfs_IsAdminGroup(credp))
	        u.u_cred->cr_uid = 0;
	    code = VOPN_MKNOD(dvp, namep, amode, vattrp->va_rdev);
	    if (!code)
	        code = VOPN_LOOKUP(dvp, vpp, namep, L_SEARCH);
	}
    }
    if (!code && *vpp) {
	/* 
	 * We convert its v_op pointer from the standard ufs_vnodeops 
	 * to the extended xvnodeops.
	 */
	xvfs_convert(*vpp);
    }
    AFSLOG(XVFS_DEBUG, 1, ("xufs_create: code = %d\n", code));
    restoreCred(savedcredp);		 /* restore the original credentials */
    return code;
}


int xufs_remove(dvp, namep, credp)
    struct vnode *dvp;
    char *namep;
    osi_cred_t *credp;
{
    struct vnode *vp;
    osi_cred_t *savedcredp;
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_remove: dvp=%x, name=%s\n", dvp, namep));
    /*
     * We need to get hold of the vnode of the file to remove; VNOP_LOOKUP to
     * the rescue.
     */
    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    code = VOPN_LOOKUP(dvp, &vp, namep, 0);
    if (!code) {
	code = VOPN_REMOVE(vp, dvp, namep);
	VOPN_RELE(vp);
    }
    AFSLOG(XVFS_DEBUG, 1, ("xufs_remove: code = %d\n", code));
    restoreCred(savedcredp);		 /* restore the original credentials */
    return code;
}


int xufs_link(fvp, tdvp, namep, credp)
    struct vnode *fvp, *tdvp;
    char *namep;
    osi_cred_t *credp;
{
    osi_cred_t *savedcredp;
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_link: tdvp=%x, fvp=%x, name=%s\n", 
			   tdvp, fvp, namep));
    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    code = VOPN_LINK(fvp, tdvp, namep);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_link: code = %d\n", code));
    restoreCred(savedcredp);		 /* restore the original credentials */
    return (code);
}


int xufs_rename(odvp, onamep, ndvp, nnamep, credp)
    struct vnode *odvp, *ndvp;
    char *onamep, *nnamep;
    osi_cred_t *credp;
{
    struct vnode *ovp, *nvp = 0;
    osi_cred_t *savedcredp;
    long code;

    AFSLOG(XVFS_DEBUG,1,("xufs_rename: odvp=%x, oname=%s, ndvp=%x, nname=%s\n",
	   odvp, onamep, ndvp, nnamep));
    /*
     * We need to get hold of the vnode of the file to be renamed
     */
    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    code = VOPN_LOOKUP(odvp, &ovp, onamep, 0);
    if (!code) {
	/*
	 * We also need to get hold of the vnode of newname on target, if it
	 * already exists; ignore any errors.
	 */
	(void) VOPN_LOOKUP(ndvp, &nvp, nnamep, 0);
	u.u_error = 0;			/* Since the lookup affected it */
	code = VOPN_RENAME(ovp, odvp, onamep, nvp, ndvp, nnamep);
	VOPN_RELE(ovp);
	if (nvp)
	    VOPN_RELE(nvp);
    }
    AFSLOG(XVFS_DEBUG, 1, ("xufs_rename: code = %d\n", code));
    restoreCred(savedcredp);		 /* restore the original credentials */
    return code;
}


int xufs_mkdir(dvp, namep, vattrp, vpp, credp)
    struct vnode **vpp, *dvp;
    char *namep;
    struct vattr *vattrp;
    osi_cred_t *credp;
{
    osi_cred_t *savedcredp;
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_mkdir: dvp=%x, name=%s\n", dvp, namep));

    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    code = VOPN_MKDIR(dvp, namep, vattrp->va_mode & 0777);
    if (!code) {
	if (vpp) {
	    code = VOPN_LOOKUP(dvp, vpp, namep, 0);
	    if (!code)
		xvfs_convert(*vpp);
	}
    }
    AFSLOG(XVFS_DEBUG, 1, ("xufs_mkdir: code = %d\n", code));
    restoreCred(savedcredp);		 /* restore the original credentials */
    return code;
}


int xufs_rmdir(dvp, namep, cdp, credp)
    struct vnode *dvp;
    char *namep;
    struct vnode *cdp;			/* ignored */
    osi_cred_t *credp;
{
    struct vnode *vp;
    osi_cred_t *savedcredp;
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_rmdir: dvp=%x, name=%s\n", dvp, namep));
    /*
     * We need to get hold of the vnode of the file to be renamed
     */
    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    code = VOPN_LOOKUP(dvp, &vp, namep, 0);
    if (!code) {    
	code = VOPN_RMDIR(vp, dvp, namep);
	VOPN_RELE(vp);
    }
    AFSLOG(XVFS_DEBUG, 1, ("xufs_rmdir: code = %d\n", code));
    restoreCred(savedcredp);		 /* restore the original credentials */
    return code;
}


int xufs_readdir(vp, uiop, credp, eofp, isPX)
    struct vnode *vp;
    struct uio *uiop;
    osi_cred_t *credp;
    int *eofp;				/* ignored */
    int isPX;				/* ignored */
{
    osi_cred_t *savedcredp;
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_readdir: vp=%x\n", vp));
    swapCred(credp, &savedcredp);	/* let u.u_cred assume new credp */
    code = VOPN_READDIR(vp, uiop);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_readdir: code = %d\n", code));
    restoreCred(savedcredp);		/* restore the original credentials */
    return (code);
}


int xufs_symlink(dvp, namep, vattrp, linkcontentsp, credp)
    struct vnode *dvp;
    char *namep, *linkcontentsp;
    struct vattr *vattrp;
    osi_cred_t *credp;
{
    struct vnode *vp;
    osi_cred_t *savedcredp;
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_symlink: dvp=%x, name=%s\n", dvp, namep));
    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    code = VOPN_SYMLINK(dvp, namep, linkcontentsp);
    if (!code) {
	code = VOPN_LOOKUP(dvp, &vp, namep, 0);
	if (!code) {
	    xvfs_convert(vp);
	    vattrp->va_type = -1;
	    code = VOPX_SETATTR(vp, (struct xvfs_attr *)vattrp, 0, credp);
	    VOPN_RELE(vp);
	}
    }
    AFSLOG(XVFS_DEBUG, 1, ("xufs_symlink: code = %d\n", code));
    restoreCred(savedcredp);		 /* restore the original credentials */
    return (code);
}


int xufs_readlink(vp, uiop, credp)
    struct vnode *vp;
    struct uio *uiop;
    osi_cred_t *credp;
{
    osi_cred_t *savedcredp;
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_readlink: vp=%x\n", vp));
    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    code = VOPN_READLINK(vp, uiop);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_readlink: code = %d\n", code));
    restoreCred(savedcredp);		/* restore the original credentials */
    return (code);
}


int xufs_afsfid(vp, afsFidp, wantv) 
    struct vnode *vp;
    struct afsFid *afsFidp;
    int wantv;				/* want volume field filled in, too */
{
    struct volume *volp;
    struct fileid fid;
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_afsfid: vp=%x(%x) = ", vp, vp->v_op));
    code = xufs_fid(vp, (struct fid *)&fid);
    if (!code) {
	afsFidp->Vnode = fid.fid_ino;
	afsFidp->Unique = fid.fid_gen;

	if (wantv) {
	    afsFidp->Cell = tkm_localCellID;
	    if (((code = xvfs_GetVolume(vp, &volp)) == 0) && volp) {
		afsFidp->Volume = volp->v_volId;
		xvfs_PutVolume(volp);
	    }
	    else
		AFS_hzero(afsFidp->Volume);
	}
    }
    AFSLOG(XVFS_DEBUG, 1, ("%d\n", code));
    return code;
}


int xufs_fid(vp, fidp)
    struct vnode *vp;
    struct fid *fidp;
{
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_fid: vp=%x\n", vp));
    code = VOPN_FID(vp, fidp);
    AFSLOG(XVFS_DEBUG, 1, ("xufs_fid: code=%d [vnode=%d, unique=%d]\n", 
	   code, ((struct fileid *)fidp)->fid_ino, ((struct fileid *)fidp)->fid_gen));
    return code;
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
    struct dfs_acl *AccessListp;
    int w;
    osi_cred_t *credp;
{
    osi_cred_t *savedcredp;
    AFSLOG(XVFS_DEBUG, 1, ("xufs_getacl: vp=%x\n", vp));
    /*
     * Currently UFS doesn't handle ACL. So we simply return the following
     */
    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    restoreCred(savedcredp);		 /* restore the original credentials */
    return ENOSYS;
}


int xufs_setacl(vp, AccessListp, svp, dw, sw, credp) 
    struct vnode *vp;
    struct dfs_acl *AccessListp;
    struct vnode *svp;
    int dw;
    int sw;
    osi_cred_t *credp;
{
    osi_cred_t *savedcredp;
    AFSLOG(XVFS_DEBUG, 1, ("xufs_setacl: vp=%x\n", vp));
    /*
     * Currently UFS doesn't handle ACL. So we simply return the following
     */
    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    restoreCred(savedcredp);		 /* restore the original credentials */
    return ENOSYS;
}


int xufs_getlength(vp, lenp, credp) 
    struct vnode *vp;
    long *lenp;
    osi_cred_t *credp;
{
    struct vattr vattr;
    osi_cred_t *savedcredp;
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_getlength: vp=%x ", vp));

    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    *lenp = 0;
    code = VOPN_GETATTR(vp, &vattr);
    if (!code) {
	*lenp = (long) vattr.va_size;
    }
    AFSLOG(XVFS_DEBUG, 1, ("len=%d (code = %d)\n", *lenp, code));
    restoreCred(savedcredp);		 /* restore the original credentials */
    return code;
}


int xufs_fsync(vp, credp)
    struct vnode *vp;
    osi_cred_t *credp;
{
    osi_cred_t *savedcredp;
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_fsync: vp=%x\n", vp));

    swapCred(credp, &savedcredp);	  /* let u.u_cred assume new credp */
    code = VOPN_FSYNC(vp, 0, (char *)0);
    restoreCred(savedcredp);		 /* restore the original credentials */
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
    long off;
{
    osi_cred_t *savedcredp;
    long code;
    int aixcmd;

    switch (cmd) {
	case F_GETLK:  aixcmd = 0; break;
	case F_SETLK:  aixcmd = SETFLCK; break;
	case F_SETLKW: aixcmd = SETFLCK | SLPFLCK; break;
    }
    swapCred(credp, &savedcredp);
    code = VOPN_LOCKCTL(vp, off, flockp, aixcmd, 0, 0);
    restoreCred(savedcredp);

    /*
     * AIX VOP_LOCKCTL doesn't return error code when GETLK indicates
     * a blocker, but our spec says VOPX_LOCKCTL does, so we "convert" here
     */
    if (!code && (cmd == F_GETLK) && (flockp->l_type != F_UNLCK))
	code = EACCES;

    return code;
}
int xufs_nosys()
{
    AFSLOG(XVFS_DEBUG, 0, 
	   ("AN EXTENDED UNSUPPORTED ROUTINE IS CALLED... EXITING\n"));
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
 * This internal routine is to let the current kernel process "temporarily"
 * assume the caller's credentials, credp. So, it could do the work on 
 * behalf of this caller. (Note, This is also because AIX's VOPs do not take
 * caller's cred !)
 */
static swapCred(credp, savedcredp)
     osi_cred_t *credp;
     osi_cred_t **savedcredp;
{
    osi_cred_t *newcredp;

    *savedcredp = u.u_cred;
    newcredp = crget();
    newcredp->cr_uid = credp->cr_uid;
    newcredp->cr_gid = credp->cr_gid;
    newcredp->cr_ngrps = credp->cr_ngrps;
    bcopy ((caddr_t)credp->cr_groups, (caddr_t)newcredp->cr_groups, 
	   credp->cr_ngrps * sizeof (credp->cr_gid));	   
    u.u_cred = newcredp;
    return 0;
}
/*
 * To restore the original credentials 
 */
static restoreCred(savedcredp)
     osi_cred_t *savedcredp;
{
    osi_cred_t *newcredp;

    newcredp = u.u_cred;
    u.u_cred = savedcredp;
    crfree(newcredp);
    return 0;
}
     
/*
 * The following functions are not called from the Protocol Exporter, but
 * may eventually be called from the CM express ops, if those are ever
 * gotten to work again.  In that event, these functions will have to be
 * implemented.  For now, they are just panics.
 */
int xufs_open() { panic("xufs_open"); }
int xufs_close() { panic("xufs_close"); }
int xufs_ioctl() { panic("xufs_ioctl"); }
int xufs_select() { panic("xufs_select"); }
int xufs_inactive() { panic("xufs_inactive"); }
int xufs_strategy() { panic("xufs_strategy"); }
int xufs_map() { panic("xufs_map"); }
int xufs_unmap() { panic("xufs_unmap"); }

int xufs_panic() { panic("invalid X-op"); }
void xufs_void_panic() { panic("invalid X-op"); }

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
    xufs_panic,         /* bmap */
    xufs_strategy,
    xufs_panic,         /* ustrategy */
    xufs_panic,         /* bread */
    xufs_panic,         /* brelse */
    xufs_lockctl,
    xufs_fid,
    xufs_panic,         /* hold */
    xufs_panic,         /* rele */
    xufs_setacl,
    xufs_getacl,
    xufs_afsfid,
    xufs_getvolume,
    xufs_getlength,
    xufs_map,
    xufs_unmap,
    xufs_panic,         /* reclaim */
    xufs_panic,		/* read */
    xufs_panic,		/* write */
    xufs_panic,		/* realvp */
    xufs_void_panic,	/* rwlock */
    xufs_void_panic,	/* rwunlock */
    xufs_panic,		/* seek */
    xufs_panic,		/* space */
    xufs_panic,		/* getpage */
    xufs_panic,		/* putpage */
    xufs_panic,		/* addmap */
    xufs_panic,		/* delmap */
    xufs_panic,		/* pageio */
    xufs_panic,		/* pagein */
    xufs_panic,		/* pageout */
    xufs_panic,		/* setfl */
    xufs_void_panic,	/* dispose */
    xufs_panic,		/* setsecattr */
    xufs_panic		/* getsecattr */
}
;
