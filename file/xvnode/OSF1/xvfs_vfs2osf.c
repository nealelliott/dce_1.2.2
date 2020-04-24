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
 * $Log: xvfs_vfs2osf.c,v $
 * Revision 1.1.131.1  1996/10/02  19:03:52  damon
 * 	New DFS from Transarc
 * 	[1996/10/01  18:59:33  damon]
 *
 * $EndLog$
*/

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/basicincludes.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/volume.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/dacl.h>
#include <ufs/inode.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/xvnode/OSF1/xvfs_vfs2osf.c,v 1.1.131.1 1996/10/02 19:03:52 damon Exp $")

/*
 * This module is a glue layer used by the Protocol Exporter to export
 * the OSF/1 ufs file system.  It consists of functions that meet the
 * specs of the X-ops and that call N-ops; the N-ops are the OSF/1 ufs
 * vnode ops.
 */

/*
 * Forward declarations
 */
int xufs_rdwr(), xufs_getattr(), xufs_setattr(), xufs_access(), xufs_lookup();
int xufs_create(), xufs_remove(), xufs_link(), xufs_rename(), xufs_mkdir();
int xufs_rmdir(), xufs_readdir(), xufs_symlink(), xufs_readlink(), xufs_fid();
int xufs_fsync(), xufs_afsfid(), xufs_getvolume(), xufs_getacl(), xufs_setacl();
int xufs_nosys();
int xufs_vfsgetvolume();

int xufs_open(), xufs_close(), xufs_ioctl(), xufs_select();
int xufs_inactive(), xufs_bmap(), xufs_strategy(), xufs_lockctl();

int xufs_panic();

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
    xufs_symlink,
    xufs_readlink,
    xufs_fsync,
    xufs_inactive,
    xufs_bmap,
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
    xufs_map,		/* map */
    xufs_unmap,		/* unmap */
    xufs_reclaim,
    xufs_panic,		/* read */
    xufs_panic,		/* write */
    xufs_panic,		/* realvp */
    xufs_panic,		/* rwlock */
    xufs_panic,		/* rwunlock */
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
    xufs_panic,		/* dispose */
    xufs_panic,		/* setsecattr */
    xufs_panic		/* getsecattr */
};


/* special routine called from namei if the 
   flag SPECLOOKUP is turned on in vnode flags */
int xafs_lookup(dp, ndp)
    struct vnode *dp;
    struct nameidata *ndp;
{
    return(VOPN_LOOKUP(dp,ndp));
}

/* special routine called from vn_rdwr */
int afs_readop(vp, uio, ioflag, cred)
        struct vnode *vp;
        register struct uio *uio;
        int ioflag;
        osi_cred_t *cred;
{
    return(VOPN_READ(vp, uio, ioflag, cred));
}

/* special routine called from vn_rdwr */
int afs_writeop(vp, uio, ioflag, cred)
        struct vnode *vp;
        register struct uio *uio;
        int ioflag;
        osi_cred_t *cred;
{
    return(VOPN_WRITE(vp, uio, ioflag, cred));
}

/*
 * xufs_namei - like namei(), but for one element only
 *	fill in ndp and call VOPN_LOOKUP()
 * (it's important that the glue operation NOT be called)
 */
xufs_namei(ndp, namep)
     register struct nameidata *ndp;
     char *namep;
{
	register int i;
	register char *cp;
	struct vnode *dp;
	int error = 0;

	if ((ndp->ni_nameiop & HASBUF) == 0) {

		/*
		 * Copy the name from the pathname to the d_name field of ndp
		 * and set the various ndp fields appropriately.
		 */
		cp = namep;
		ndp->ni_hash = 0;
		for (i = 0; *cp != 0 && *cp != '/'; cp++) {
			if (i >= NAME_MAX) {
				error = ENAMETOOLONG;
				return(error);
			}
			ndp->ni_dent.d_name[i++] = *cp;
			ndp->ni_hash += (unsigned char)*cp * i;
		}
	} 
#ifdef AFS_DEBUG
	 else
                printf("illegal argument to xufs_namei\n");
#endif
	ndp->ni_namelen = i;
	ndp->ni_dent.d_namlen = i;
	ndp->ni_dent.d_name[i] = '\0';
	ndp->ni_pathlen -= i;
	ndp->ni_pnbuf = ndp->ni_dirp = ndp->ni_ptr = &ndp->ni_dent.d_name[0];
	ndp->ni_next = cp;
	ndp->ni_nameiop |= (NOCROSSMOUNT | REMOTE | HASBUF | SPECLOOKUP);
	ndp->ni_lookup = xafs_lookup;

        dp = ndp->ni_dvp;
	if (dp->v_type != VDIR) {
		return (ENOTDIR);
	}
	/*
	 * Must set current directory here to avoid confusion in namei()
	 * called from rename()
	 */
	ndp->ni_cdir = dp;
	ndp->ni_rdir = NULLVP;

	/*
	 * And call namei() to do the real work
	 */
	error = namei(ndp);
	return (error);
}

int xufs_rdwr(vp, uiop, rw, ioflag, credp)
    struct vnode *vp;
    struct uio *uiop;
    enum uio_rw rw;
    int ioflag;
    osi_cred_t *credp;
{
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_rdwr: vp=0x%x, rw=%d\n", vp, rw));

    if (rw == UIO_READ) {
        uiop->uio_rw = UIO_READ;
    	code = VOPN_READ(vp, uiop, ioflag, credp);
    }
    else {
        uiop->uio_rw = UIO_WRITE;
	code = VOPN_WRITE(vp, uiop, ioflag, credp);
    }

    AFSLOG(XVFS_DEBUG, 1, ("xvfs_rdwr: code = %d\n", code));
    return(code);
}


int xufs_getattr(vp, xvattrp, flag, credp)
    struct vnode *vp;
    struct xvfs_attr *xvattrp;
    int flag;				/* indicates extended attributes */
    osi_cred_t *credp;
{
    struct afsFid Fid;
    long afsAccess, code=0;

    if (flag) {
	/* XXX REMOVE THIS!! -- i.e., the bzero, I assume, but I
	 * ain't touching it.
	 * XXX */
	bzero(xvattrp, sizeof(struct xvfs_attr));
	xvattrp->xvattr.serverModTime.sec = VTOI(vp)->i_ctime;
	xvattrp->xvattr.serverModTime.usec = VTOI(vp)->i_ctime_usec;
    }
    else
	bzero(xvattrp, sizeof(struct vattr));

    AFSLOG(XVFS_DEBUG, 1, ("xufs_getattr: vp=0x%x\n", vp));

    code = VOPN_GETATTR(vp, xvattrp, credp);

    AFSLOG(XVFS_DEBUG, 1, ("xufs_getattr: code = %d\n", code));

    if (!flag)
	return(code);    
    /*
     * Fill the ep_attrs fields: In most cases they're not used by 
     * the UFS implementation
     */
    if (!VOPX_AFSFID(vp, &Fid, 0)) {			/* Ignore errors */
        AFS_hset64(xvattrp->xvattr.fileID, Fid.Vnode, Fid.Unique);
    }
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
    return(code);
}


int xufs_setattr(vp, vattrp, xflag, credp)
    struct vnode *vp;
    struct vattr *vattrp;
    int xflag;				/* indicates extended attributes */
    osi_cred_t *credp;
{
    long code;
    register osi_cred_t *tcredp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_setattr: vp=0x%x\n", vp));
    /* 
     * If the caller is the member of the system admin group. Let the caller 
     * assume root's privileges for the duration of setattr.
     * We must save the uid away in case we are going to change it; we do
     * it in all cases 'cuz it probably won't cost any more anyway.
     *
     * Don't bash cr_uid it in place, since *credp is a shared structure.
     */

    crhold(credp);
    tcredp = crcopy(credp);	/* crfree's credp as side effect */
    if (xvfs_IsAdminGroup(tcredp)) 
       tcredp->cr_uid = 0;

    code = VOPN_SETATTR(vp, vattrp, tcredp);

    crfree(tcredp);	/* free credential */

    AFSLOG(XVFS_DEBUG, 1, ("xufs_setattr: code = %d\n", code));
    return(code);
}


int xufs_access(vp, mode, credp)
    struct vnode *vp;
    int mode;
    osi_cred_t *credp;
{
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_access: vp=0x%x, mode=%d\n", vp, mode));

    code = VOPN_ACCESS(vp, mode, credp);

    AFSLOG(XVFS_DEBUG, 1, ("xufs_access: code = %d\n", code));
    return(code);
}


int xufs_lookup(dvp, namep, vpp, credp)
    struct vnode *dvp;
    char *namep;
    struct vnode **vpp;
    osi_cred_t *credp;
{
    struct nameidata nami;
    register struct nameidata *ndp = &nami;
#ifdef AFS_OSF11_ENV
    struct filedesc fdsc;
#endif /* AFS_OSF11_ENV */
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_lookup: dvp=0x%x, name=%s\n", dvp, namep));

#ifndef AFS_OSF11_ENV
    ndinit(ndp, (struct utask_nd *)0);
#else
    ndinit(ndp, &fdsc);
#endif /* AFS_OSF11_ENV */
    ndp->ni_nameiop = LOOKUP;
    ndp->ni_cred = credp;
    ndp->ni_dvp = dvp;
    code = xufs_namei(ndp, namep);
    if (!code) {
	*vpp = ndp->ni_vp;
	/* 
	 * We convert its v_op pointer from the standard ufs_vnodeops 
	 * to the extended xvnodeops.
	 */
	xvfs_convert(*vpp);
    }

    AFSLOG(XVFS_DEBUG, 1, ("xufs_lookup: code = %d\n", code));
    return(code);
}


int xufs_create(dvp, namep, vattrp, excl, mode, vpp, credp)
    struct vnode *dvp;
    char *namep;
    struct vattr *vattrp;
    enum vcexcl excl;
    int mode;
    struct vnode **vpp;
    osi_cred_t *credp;
{
    struct nameidata nami;
    register struct nameidata *ndp = &nami;
#ifdef AFS_OSF11_ENV
    struct filedesc fdsc;
#endif /* AFS_OSF11_ENV */
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_create: dvp=0x%x, name=%s, mode=%d\n", dvp, namep, mode));

#ifndef AFS_OSF11_ENV
    ndinit(ndp, (struct utask_nd *)0);
#else
    ndinit(ndp, &fdsc);
#endif /* AFS_OSF11_ENV */
    ndp->ni_nameiop = CREATE | WANTPARENT;
    ndp->ni_cred = credp;
    ndp->ni_dvp = dvp;
    code = xufs_namei(ndp, namep);
    if (!code ) { 
	if (ndp->ni_vp == NULL) { /* entry does not exist */
	    if (vattrp->va_type == VFIFO)
		vattrp->va_rdev = VNOVAL;
       	    code = VOPN_CREATE(ndp, vattrp);
            if (!code) {
               *vpp = ndp->ni_vp;
		/* 
		 * We convert its v_op pointer from the standard ufs_vnodeops 
		 * to the extended xvnodeops.
		 */
         	xvfs_convert(*vpp);
	    }
        }
	else {
		vrele(ndp->ni_vp);
		vrele(ndp->ni_dvp);
		code = EEXIST;
	}
    }

    AFSLOG(XVFS_DEBUG, 1, ("xufs_create: code = %d\n", code));
    return(code);
}


int xufs_remove(dvp, namep, credp)
    struct vnode *dvp;
    char *namep;
    osi_cred_t *credp;
{
    struct nameidata nami;
    register struct nameidata *ndp = &nami;
#ifdef AFS_OSF11_ENV
    struct filedesc fdsc;
#endif /* AFS_OSF11_ENV */
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_remove: dvp=0x%x, name=%s\n", dvp, namep));

#ifndef AFS_OSF11_ENV
    ndinit(ndp, (struct utask_nd *)0);
#else
    ndinit(ndp, &fdsc);
#endif /* AFS_OSF11_ENV */
    ndp->ni_nameiop = DELETE | WANTPARENT;
    ndp->ni_cred = credp;
    ndp->ni_dvp = dvp;
    code = xufs_namei(ndp, namep);
    if (!code) {
	code = VOPN_REMOVE(ndp);
    }

    AFSLOG(XVFS_DEBUG, 1, ("xufs_remove: code = %d\n", code));
    return(code);
}


int xufs_link(fvp, tdvp, namep, credp)
    struct vnode *fvp, *tdvp;
    char *namep;
    osi_cred_t *credp;
{
    struct nameidata nami;
    register struct nameidata *ndp = &nami;
#ifdef AFS_OSF11_ENV
    struct filedesc fdsc;
#endif /* AFS_OSF11_ENV */
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_link: tdvp=0x%x, fvp=0x%x, name=%s\n", tdvp, fvp, namep));

#ifndef AFS_OSF11_ENV
    ndinit(ndp, (struct utask_nd *)0);
#else
    ndinit(ndp, &fdsc);
#endif /* AFS_OSF11_ENV */
    ndp->ni_nameiop = CREATE | WANTPARENT;
    ndp->ni_cred = credp;
    ndp->ni_dvp = tdvp;
    code = xufs_namei(ndp, namep);
    if (!code) {
       if (ndp->ni_vp == NULL){
        code = VOPN_LINK(fvp, ndp);
       }else{
          vrele(ndp->ni_vp);
          vrele(ndp->ni_dvp);
          code = EEXIST;
       }
     }
    AFSLOG(XVFS_DEBUG, 1, ("xufs_link: code = %d\n", code));
    return (code);
}

int xufs_rename(odvp, onamep, ndvp, nnamep, credp)
    struct vnode *odvp, *ndvp;
    char *onamep, *nnamep;
    osi_cred_t *credp;
{
    struct nameidata nami,nami2;
    register struct nameidata *fndp = &nami,*tndp=&nami2;
#ifndef AFS_OSF11_ENV
    struct utask_nd utnd1, utnd2, *utp1=&utnd1, *utp2=&utnd2;
#else
    struct filedesc fdsc1, fdsc2;
#endif /* AFS_OSF11_ENV */
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_rename: odvp=0x%x, oname=%s, ndvp=0x%x, nname=%s\n", 
	   odvp, onamep, ndvp, nnamep));

#ifndef AFS_OSF11_ENV
    ndinit(fndp, utp1);
#else
    ndinit(fndp, &fdsc1);
#endif /* AFS_OSF11_ENV */
    fndp->ni_nameiop = DELETE | WANTPARENT;
    fndp->ni_cred = credp;
    fndp->ni_dvp = odvp;
    code = xufs_namei(fndp, onamep);
    if (code)
       return(code);

#ifndef AFS_OSF11_ENV
    ndinit(tndp, utp2);
#else
    ndinit(tndp, &fdsc2);
#endif /* AFS_OSF11_ENV */
    tndp->ni_nameiop = RENAME | WANTPARENT;
    tndp->ni_cred = credp;
    tndp->ni_dvp = ndvp;
    code = xufs_namei(tndp, nnamep);
    if (!code) {
	code = VOPN_RENAME(fndp, tndp);
    }

    AFSLOG(XVFS_DEBUG, 1, ("xufs_rename: code = %d\n", code));
    return(code);
}


int xufs_mkdir(dvp, namep, vattrp, vpp, credp)
    struct vnode **vpp, *dvp;
    char *namep;
    struct vattr *vattrp;
    osi_cred_t *credp;
{
    struct nameidata nami;
    register struct nameidata *ndp = &nami;
#ifdef AFS_OSF11_ENV
    struct filedesc fdsc;
#endif /* AFS_OSF11_ENV */
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_mkdir: dvp=0x%x, name=%s\n", dvp, namep));

#ifndef AFS_OSF11_ENV
    ndinit(ndp, (struct utask_nd *)0);
#else
    ndinit(ndp, &fdsc);
#endif /* AFS_OSF11_ENV */
    ndp->ni_nameiop = CREATE | WANTPARENT;
    ndp->ni_cred = credp;
    ndp->ni_dvp = dvp;
    code = xufs_namei(ndp, namep);
    if (!code) {
        if (ndp->ni_vp == NULL){
	    code = VOPN_MKDIR(ndp, vattrp);
	    if (!code && (*vpp = ndp->ni_vp)) {
		/* 
		 * We convert its v_op pointer from the standard ufs_vnodeops 
		 * to the extended xvnodeops.
		 */
		xvfs_convert(*vpp);
	    }
        } else {
	    vrele(ndp->ni_vp);
	    vrele(ndp->ni_dvp);
	    code = EEXIST;
        }
    }

    AFSLOG(XVFS_DEBUG, 1, ("xufs_mkdir: code = %d\n", code));
    return(code);
    }


int xufs_rmdir(dvp, namep, cdp, credp)
    struct vnode *dvp;
    char *namep;
    struct vnode *cdp;			/* ignored */
    osi_cred_t *credp;
{
    struct nameidata nami;
    register struct nameidata *ndp = &nami;
#ifdef AFS_OSF11_ENV
    struct filedesc fdsc;
#endif /* AFS_OSF11_ENV */
    long code;
    struct vnode *vp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_rmdir: dvp=0x%x, name=%s\n", dvp, namep));

#ifndef AFS_OSF11_ENV
    ndinit(ndp, (struct utask_nd *)0);
#else
    ndinit(ndp, &fdsc);
#endif /* AFS_OSF11_ENV */
    ndp->ni_nameiop = DELETE | WANTPARENT;
    ndp->ni_cred = credp;
    ndp->ni_dvp = dvp;
    code = xufs_namei(ndp, namep);
    if (!code) {
       code = VOPN_RMDIR(ndp);
    }

    AFSLOG(XVFS_DEBUG, 1, ("xufs_rmdir: code = %d\n", code));
    return(code);
}


int xufs_readdir(vp, uiop, credp, eofp, isPX)
    struct vnode *vp;
    struct uio *uiop;
    osi_cred_t *credp;
    int *eofp;
    int isPX;
{
    long code;
    AFSLOG(XVFS_DEBUG, 1, ("xufs_readdir: vp=0x%x\n", vp));

    uiop->uio_rw = UIO_READ;
    /*
     * Ignoring isPX for now.
     */
    code = VOPN_READDIR(vp, uiop, credp, eofp);

    AFSLOG(XVFS_DEBUG, 1, ("xufs_readdir: code = %d\n", code));
    return(code);
}


int xufs_symlink(dvp, namep, vattrp, linkcontentsp, credp)
    struct vnode *dvp;
    char *namep, *linkcontentsp;
    struct vattr *vattrp;
    osi_cred_t *credp;
{
    struct nameidata nami;
    register struct nameidata *ndp = &nami;
#ifdef AFS_OSF11_ENV
    struct filedesc fdsc;
#endif /* AFS_OSF11_ENV */
    long code;
    struct vnode *vp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_symlink: dvp=0x%x, name=%s\n", dvp, namep));

#ifndef AFS_OSF11_ENV
    ndinit(ndp, (struct utask_nd *)0);
#else
    ndinit(ndp, &fdsc);
#endif /* AFS_OSF11_ENV */
    ndp->ni_nameiop = CREATE | WANTPARENT;
    ndp->ni_cred = credp;
    ndp->ni_dvp = dvp;
    code = xufs_namei(ndp, namep);
    if (!code) {
        if (ndp->ni_vp == NULL){
	    code = VOPN_SYMLINK(ndp, vattrp, linkcontentsp);
        }else{
               vrele(ndp->ni_vp);
	       vrele(ndp->ni_dvp);
	       code = EEXIST;
        }
    }

    AFSLOG(XVFS_DEBUG, 1, ("xufs_symlink: code = %d\n", code));
    return(code);
}


int xufs_readlink(vp, uiop, credp)
    struct vnode *vp;
    struct uio *uiop;
    osi_cred_t *credp;
{
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_readlink: vp=0x%x\n", vp));

    uiop->uio_rw = UIO_READ;
    code = VOPN_READLINK(vp, uiop, credp);

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

    AFSLOG(XVFS_DEBUG, 1, ("xufs_afsfid: vp=0x%x(0x%x)\n", vp, vp->v_op));

    code = xufs_fid(vp, &afsFidp);
    if (!code) {
	if (wantv) {
	    /* local cell is always special-cased */
	    afsFidp->Cell = tkm_localCellID;
	    code = xvfs_GetVolume(vp, &volp, VNOP_TYPE_NOOP);
	    if (code) {
		AFSLOG(XVFS_DEBUG, 1, ("xufs_afsfid: code=%d\n", code));
		return ((int)code);
	    }
	    if (volp) {
		afsFidp->Volume = volp->v_volId;
	    }
	    xvfs_PutVolume(volp);
	}
    }

    AFSLOG(XVFS_DEBUG, 1, ("xufs_afsfid: code = %d\n", code));
    return(code);
}

int xufs_fid(vp, afsFidpp)
    struct vnode *vp;
    struct afsFid **afsFidpp;
{
    long code;
    struct fid myfid;
    struct ufid *fidp;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_fid: vp=0x%x\n", vp));

    VFS_VPTOFH(vp, &myfid, code);
    if (!code) {
        fidp = (struct ufid *)&myfid;
        (*afsFidpp)->Vnode = fidp->ufid_ino;
        (*afsFidpp)->Unique = fidp->ufid_gen;
    }
    AFSLOG(XVFS_DEBUG, 1, ("xufs_fid: code=%d [vnode=0x%x, unique=0x%x]\n", 
	   code, (*afsFidpp)->Vnode, (*afsFidpp)->Unique));
    return(code);
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
    if(vol_FindVfs(vp->v_mount, volpp))
      return EIO; /* this is a persistent error */
    return 0;
}

int xufs_getacl(vp, AccessListp, w, credp) 
    struct vnode *vp;
    struct afsACL *AccessListp;
    long w;
    osi_cred_t *credp;
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
    osi_cred_t *credp;
{
    AFSLOG(XVFS_DEBUG, 1, ("xufs_setacl: vp=0x%x\n", vp));

    /*
     * Currently UFS doesn't handle ACL. So we simply return the following
     */
    return ENOTTY;
}


int xufs_getlength(vp, lenp, blksizep, cred) 
    struct vnode *vp;
    long *lenp;
    long *blksizep;
    osi_cred_t *cred;
{
    struct vattr vattr;
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_getlength: vp=0x%x ", vp));

    *lenp = 0;
    code = VOPN_GETATTR(vp, &vattr, cred);
    if (!code) {
	*lenp = (long) vattr.va_size;
    }

    AFSLOG(XVFS_DEBUG, 1, ("len=%d (code = %d)\n", *lenp, code));
    return(code);
}

int xufs_fsync(vp, credp)
    struct vnode *vp;
    osi_cred_t *credp;
{
    long code;

    AFSLOG(XVFS_DEBUG, 1, ("xufs_fsync: vp=0x%x\n", vp));

    code = VOPN_FSYNC(vp, 0, credp, 0);
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

#ifdef AFS_OSF11_ENV

    int osfcmd;
    int code;

    switch (cmd) {
        case F_GETLK:  osfcmd = 0; break;
        case F_SETLK:  osfcmd = SETFLCK; break;
        case F_SETLKW: osfcmd = SETFLCK | SLPFLCK; break;
    }

    code = VOPN_FILELOCK(vp, flockp, osfcmd, off);

    return code;

#endif /* AFS_OSF11_ENV */

}


int xufs_nosys()
{
    AFSLOG(XVFS_DEBUG, 0, ("AN EXTENDED UNSUPPORTED ROUTINE IS CALLED... EXITING\n"));

    return EINVAL;
}

int xufs_vfsgetvolume (vfsp, volpp)
    struct vfs *vfsp;			/* virtual file system */
    struct volume **volpp;		/* where to put volume ptr */
{
    if(vol_FindVfs (vfsp, volpp))
      return EIO;	/* this is a persistent error */
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
int xufs_bmap() { panic("xufs_bmap"); }
int xufs_strategy() { panic("xufs_strategy"); }
