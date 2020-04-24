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
 * $Log: xvfs_osfglue.c,v $
 * Revision 1.1.120.1  1996/10/02  19:03:46  damon
 * 	New DFS from Transarc
 * 	[1996/10/01  18:55:45  damon]
 *
 * $EndLog$
*/

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/stds.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/osi.h>
#include <dcedfs/queue.h>
#include <dcedfs/volume.h>
#include <dcedfs/hs.h>
#include <dcedfs/tkm_tokens.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/tkc.h>

#include <sys/namei.h>
#include <mach/memory_object.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/xvnode/OSF1/xvfs_osfglue.c,v 1.1.120.1 1996/10/02 19:03:46 damon Exp $")

extern int
    xglue_lookup(),
    xglue_create(),
    xglue_mknod(),
    xglue_open(),
    xglue_close(),
    xglue_access(),
    xglue_getattr(),
    xglue_setattr(),
    xglue_read(),
    xglue_write(),
    xglue_ioctl(),
    xglue_select(),
    xglue_mmap(),
    xglue_fsync(),
    xglue_seek(),
    xglue_remove(),
    xglue_link(),
    xglue_rename(),
    xglue_mkdir(),
    xglue_rmdir(),
    xglue_symlink(),
    xglue_readdir(),
    xglue_readlink(),
    xglue_abortop(),
    xglue_inactive(),
    xglue_reclaim(),
    xglue_bmap(),
    xglue_strategy(),
    xglue_print(),
    xglue_page_read(),
#ifndef AFS_OSF11_ENV
    xglue_page_write();
#else
    xglue_page_write,
    xglue_filelock();
#endif /* AFS_OSF11_ENV */

struct osi_vnodeops xglue_ops = {
    xglue_lookup,
    xglue_create,
    xglue_mknod,
    xglue_open,
    xglue_close,
    xglue_access,
    xglue_getattr,
    xglue_setattr,
    xglue_read,
    xglue_write,
    xglue_ioctl,
    xglue_select,
    xglue_mmap,
    xglue_fsync,
    xglue_seek,
    xglue_remove,
    xglue_link,
    xglue_rename,
    xglue_mkdir,
    xglue_rmdir,
    xglue_symlink,
    xglue_readdir,
    xglue_readlink,
    xglue_abortop,
    xglue_inactive,
    xglue_reclaim,
    xglue_bmap,
    xglue_strategy,
    xglue_print,
    xglue_page_read,
#ifndef AFS_OSF11_ENV
    xglue_page_write
#else
    xglue_page_write,
    xglue_filelock
#endif /* AFS_OSF11_ENV */
};


int xglue_link(vp, ndp)
struct vnode *vp;
struct nameidata *ndp;
{
    register long code;
    struct volume *volp1, *volp2;
    struct tkc_sets sets[2];
    struct vnode *dp = ndp->ni_dvp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_link: (vp=0x%x)\n", vp));
    code = xvfs_GetVolume(vp, &volp1);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_link: %d\n", code));
      return(code);
    }
    code = xvfs_GetVolume(dp, &volp2);
    if (code) {
      xvfs_PutVolume(volp1);
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_link: %d\n", code));
      return(code);
    }
    if (volp1 != volp2) {
	code = EXDEV;
	goto linkerr;
    }
    if (!xvfs_NeedTokens(volp1)) {
	if (!(code = vol_StartVnodeOp(volp1, VNOP_LINK, 0))) {
	    code = VOPN_LINK(vp, ndp);
	    vol_EndVnodeOp(volp1);
	}
	goto linkerr;
    }    
    if (vp == dp) {
	code = EINVAL;
	goto linkerr;
    }
    tkc_Set(0, vp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
    tkc_Set(1, dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
    tkc_GetTokens(sets, 2);
    if (!(code = vol_StartVnodeOp(volp1, VNOP_LINK, 0))) {
	code = VOPN_LINK(vp, ndp);
	vol_EndVnodeOp(volp1);
    }
    tkc_PutTokens(sets, 2);

linkerr:
    xvfs_PutVolume(volp1);
    xvfs_PutVolume(volp2);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_link: %d\n", code));
    return (code);
}

int xglue_mkdir(ndp, vap)
struct nameidata *ndp;
struct vattr *vap;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    struct vnode *dp = ndp->ni_dvp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_mkdir: (dp=0x%x)\n", dp));
    code = xvfs_GetVolume(dp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_mkdir: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_MKDIR, 0))) {
	    code = VOPN_MKDIR(ndp, vap);
	    if (volp && !code) /* registered volume but not exported? */
		XVFS_CONVERT(ndp->ni_vp);
	    vol_EndVnodeOp(volp);
	}
	goto mkdirerr;
    }    
    if (vcp = tkc_Get(dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_MKDIR, 0))) {
	    code = VOPN_MKDIR(ndp, vap);
	    if (!code)
		XVFS_CONVERT(ndp->ni_vp);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
mkdirerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_mkdir: %d\n", code));
    return (code);
}

int xglue_mknod(ndp, vap, cred)
struct nameidata *ndp;
struct vattr *vap;
osi_cred_t *cred;
{
    register struct tkc_vcache *vcp;
    long code;
    struct volume *volp;
    struct vnode *dp = ndp->ni_dvp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_mknod: (dp=0x%x)\n",  dp));
    code = xvfs_GetVolume(dp, &volp, VNOP_MKNOD);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_mknod: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_MKNOD, 0))) {
	    code = VOPN_MKNOD(ndp, vap, cred);
	    vol_EndVnodeOp(volp);
	}
	goto mknoderr;
    }    
    if (vcp = tkc_Get(dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_MKNOD, 0))) {
	    code = VOPN_MKNOD(ndp, vap, cred);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
mknoderr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_mknod: %d\n", code));
    return (code);
}

int xglue_remove(ndp)
struct nameidata *ndp;
{
    struct tkc_sets sets[2];
    register long code;
    struct volume *volp1;
    struct volume *volp2;
    struct vnode *vp = ndp->ni_vp;
    struct vnode *dp = ndp->ni_dvp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_remove: (dp=0x%x, vp=0x%x)\n", dp, vp));
    code = xvfs_GetVolume(vp, &volp1);
    if (code) {
	AFSLOG(XGLUE_DEBUG, 1, ("In xglue_remove: %d\n", code));
    }
    code = xvfs_GetVolume(dp, &volp2);
    if (code) {
	AFSLOG(XGLUE_DEBUG, 1, ("In xglue_remove: %d\n", code));
	xvfs_PutVolume(volp1);
	return(code);
    }
    if (volp1 != volp2) {
	code = EXDEV;
	goto remerr;
    }
    if (!xvfs_NeedTokens(volp1)){
	if (!(code = vol_StartVnodeOp(volp1, VNOP_REMOVE, 0))) {
	    code = VOPN_REMOVE(ndp);
	    vol_EndVnodeOp(volp1);
	}
	goto remerr;
    }    
    if (dp == vp) {
	code = EINVAL;
	goto remerr;
    }
    tkc_Set(0, dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
    tkc_Set(1, vp, TKN_STATUS_WRITE, 0);
    tkc_GetTokens(sets, 2);
    OSI_VN_HOLD(vp);
    if (!(code = vol_StartVnodeOp(volp1, VNOP_REMOVE, 0))) {
	code = VOPN_REMOVE(ndp);
	vol_EndVnodeOp(volp1);
    }
    tkc_PutTokens(sets, 2);
    tkc_HandleDelete(vp, volp1);
    OSI_VN_RELE(vp);

remerr:
    xvfs_PutVolume(volp1);
    xvfs_PutVolume(volp2);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_remove: %d\n", code));
    return (code);
}

int xglue_rename(fndp, tndp)
struct nameidata *fndp;
struct nameidata *tndp;
{
    struct tkc_sets sets[4];
    register long code, size;
    struct volume *volp1, *volp2;
    struct vnode *dp = fndp->ni_dvp, *tdp = tndp->ni_dvp;
    struct vnode *vp = fndp->ni_vp, *tp = tndp->ni_vp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rename: (dp=0x%x, vp=0x%x)\n", dp, vp));
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rename: (tdp=0x%x, tp=0x%x)\n", tdp, tp));
    code = xvfs_GetVolume(dp, &volp1);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rename: %d\n", code));
      return(code);
    }
    code = xvfs_GetVolume(tdp, &volp2);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rename: %d\n", code));
      return(code);
    }
    if (volp1 != volp2) {
	code = EXDEV;
	goto renerr;
    }
    if (!xvfs_NeedTokens(volp1)) {
	if (!(code = vol_StartVnodeOp(volp1, VNOP_RENAME, 0))) {
	    code = VOPN_RENAME(fndp, tndp);
	    vol_EndVnodeOp(volp1);
	}
	goto renerr;
    }    

    /* watch for invalid parms: duplicate vnode going into tkc_GetTokens
     * will cause an infinite loop.  dp == tdp is only legal dup vnode.
     * tp is only potentially null pointer.
     */
    if (vp == dp || vp == tdp || dp == tp || tp == tdp) {
	code = EINVAL;
	goto renerr;
    }

    /* 
     * Check for identical source and target objects to prevent 
     * token deadlocks
     */
    if (vp == tp) {
	code = 0;
	goto renerr;
    }

    tkc_Set(0, dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
    size = 1;
    if (tdp != dp) {
	tkc_Set(1, tdp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
	size++;
    }
    tkc_Set(size, vp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
    size++;
    if (tp) {
	tkc_Set(size, tp, TKN_STATUS_WRITE, 0);
	size++;
    }
    tkc_GetTokens(sets, size);
    if (tp) OSI_VN_HOLD(tp);
    if (!(code = vol_StartVnodeOp(volp1, VNOP_RENAME, 0))) {
	code = VOPN_RENAME(fndp, tndp);
	vol_EndVnodeOp(volp1);
    }
    tkc_PutTokens(sets, size);
    if (tp) {
	tkc_HandleDelete(tp, volp1);
	OSI_VN_RELE(tp);
    }

renerr:
    xvfs_PutVolume(volp1);
    xvfs_PutVolume(volp2);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rename: %d\n", code));
    return (code);

}

int xglue_rmdir(ndp)
struct nameidata *ndp;
{
    struct tkc_sets sets[2];
    register long code;
    struct volume *volp;
    struct vnode *dp = ndp->ni_dvp;
    struct vnode *vp = ndp->ni_vp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rmdir: (dp=0x%x, vp=0x%x)\n", dp, vp));
    code = xvfs_GetVolume(dp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rmdir: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_RMDIR, 0))) {
	    code = VOPN_RMDIR(ndp);
	    vol_EndVnodeOp(volp);
	}
	goto rmdirerr;
    }    
    if (dp == vp) {
	code = EINVAL;
	goto rmdirerr;
    }
    tkc_Set(0, dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
    tkc_Set(1, vp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
    tkc_GetTokens(sets, 2);
    OSI_VN_HOLD(vp);
    if (!(code = vol_StartVnodeOp(volp, VNOP_RMDIR, 0))) {
	code = VOPN_RMDIR(ndp);
	vol_EndVnodeOp(volp);
    }
    tkc_PutTokens(sets, 2);
    tkc_HandleDelete(vp, volp);
    OSI_VN_RELE(vp);

rmdirerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rmdir: %d\n", code));
    return (code);
}

int xglue_lookup(dp, ndp)
struct vnode *dp;
struct nameidata *ndp;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_lookup: (dp=0x%x)\n", dp));
    ndp->ni_dvp = dp;	/* eliminate possible garbage in caller's stg */
    ndp->ni_vp = NULL;	/* eliminate possible garbage in caller's stg */
    code = xvfs_GetVolume(dp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_lookup: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_LOOKUP, 0))) {
	    code = VOPN_LOOKUP(dp, ndp);
	    if (volp && !code) /* registered volume but not exported? */
		XVFS_CONVERT(ndp->ni_vp);
	    vol_EndVnodeOp(volp);
	}
	goto lookerr;
    }
    if (vcp = tkc_Get(dp, TKN_DATA_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_LOOKUP, 0))) {
	    code = VOPN_LOOKUP(dp, ndp);
	    if (!code)
		XVFS_CONVERT(ndp->ni_vp);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
lookerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_lookup: %d\n", code));
    return (code);
}

#define VNUTIL_VNODENULL(vp) ((vp) == (struct vnode *)0)

int xglue_open(avp, flags, acred)
struct vnode **avp;
long flags;
osi_cred_t *acred;
{
    struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    long hoFlags;
    struct vnode *vp = *avp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_open: (vp=0x%x, flags=0x%x)\n", vp, flags));
    if (VNUTIL_VNODENULL(vp)){
	code = VOPN_OPEN(avp, flags, acred);
	return(code);
    }
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_open: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_OPEN, 0))) {
	    code = VOPN_OPEN(avp, flags, acred);
	    vol_EndVnodeOp(volp);
	}
	goto openerr;
    }

    /* Set flags for TKC HandleOpen call */
    hoFlags = ((flags & FWRITE) ? 1 : 0);
    /* In OSF/1, VOPN_OPEN may do truncation */
    if (flags & FTRUNC)
	hoFlags |= 4;

    if (!(code = tkc_HandleOpen(vp, hoFlags, &vcp))) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_OPEN, 0))) {
	    code = VOPN_OPEN(avp, flags, acred);
	    vol_EndVnodeOp(volp);
	}
	tkc_Release(vcp);
	if (code) {
	    if (vcp = tkc_HandleClose(vp, hoFlags)) {
		tkc_Release(vcp);
	    }
	}
    }

openerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_open: %d\n", code));
    return (code);
}

int xglue_create(ndp, vap)
struct nameidata *ndp;
struct vattr *vap;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    struct vnode *dp = ndp->ni_dvp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_create: (dp=0x%x)\n", dp));
    ndp->ni_vp = NULL;	/* eliminate possible garbage in caller's data */
    code = xvfs_GetVolume(dp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_create: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_CREATE, 0))) {
	    code = VOPN_CREATE(ndp, vap);
	    if (volp && !code) /* registered volume but not exported? */
		XVFS_CONVERT(ndp->ni_vp);
	    vol_EndVnodeOp(volp);
	}
	goto createrr;
    }
    if (vcp = tkc_Get(dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_CREATE, 0))) {
	    code = VOPN_CREATE(ndp, vap);
	    if (!code)
		XVFS_CONVERT(ndp->ni_vp);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
createrr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_create: %d\n", code));
    return (code);
}

int xglue_close(vp, flags, acred)
struct vnode *vp;
int flags;
osi_cred_t *acred;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_close: (vp=0x%x, flags=0x%x)\n", vp, flags));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_close: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_CLOSE, 0))) {
	    code = VOPN_CLOSE(vp, flags, acred);
	    vol_EndVnodeOp(volp);
	}
	goto closerr;
    }
    /*
     * XXX Do we really need to get the OPEN tokens again; they've gotten
     * during the open system call.  Answer (kazar): No.
     */
    if (!(code = vol_StartVnodeOp(volp, VNOP_CLOSE, 0))) {
	code = VOPN_CLOSE(vp, flags, acred);
	vol_EndVnodeOp(volp);
    }
    if (vcp = tkc_HandleClose(vp, (flags & FWRITE) ? 1 : 0)) {
	/* HandleClose actually does all of our hard work */
	tkc_Release(vcp);
    }

closerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_close: %d\n", code));
    return (code);
}

int xglue_access(vp, mode, acred)
struct vnode *vp;
long mode;
osi_cred_t *acred;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_access: (vp=0x%x, mode=0x%x)\n", vp, mode));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_access: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_ACCESS, 0))) {
	    code = VOPN_ACCESS(vp, mode, acred);
	    vol_EndVnodeOp(volp);
	}
	goto accesserr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_ACCESS, 0))) {
	    code = VOPN_ACCESS(vp, mode, acred);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

accesserr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_access: %d\n", code));
    return (code);
}

int xglue_getattr(vp, vattrp, acred)
struct vnode *vp;
struct vattr *vattrp;
osi_cred_t *acred;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_getattr: (vp=0x%x, vattrp=0x%x)\n", vp, vattrp));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_getattr: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_GETATTR, 0))) {
	    code = VOPN_GETATTR(vp, vattrp, acred);
	    vol_EndVnodeOp(volp);
	}
	goto getattrerr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_GETATTR, 0))) {
	    code = VOPN_GETATTR(vp, vattrp, acred);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

getattrerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_getattr: %d\n", code));
    return (code);
}

int xglue_setattr(vp, vattrp, acred)
struct vnode *vp;
struct vattr *vattrp;
osi_cred_t *acred;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_setattr: (vp = 0x%x, vattrp = 0x%x)\n", 
	   vp, vattrp));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_setattr: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SETATTR, 0))) {
	    code = VOPN_SETATTR(vp, vattrp, acred);
	    vol_EndVnodeOp(volp);
	}
	goto setattrerr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SETATTR, 0))) {
	    code = VOPN_SETATTR(vp, vattrp, acred);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

setattrerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_setattr: \n"));
    return (code);
}

int xglue_fsync(vp, flags, acred, waitfor)
struct vnode *vp;
int flags;
osi_cred_t *acred;
int waitfor;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_fsync: (vp=0x%x)\n",vp));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_fsync: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_FSYNC, 0))) {
	    code = VOPN_FSYNC(vp, flags, acred, waitfor);
	    vol_EndVnodeOp(volp);
	}
	goto fsyncerr;
    }
    if (vcp = tkc_Get(vp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_FSYNC, 0))) {
	    code = VOPN_FSYNC(vp, flags, acred, waitfor);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
fsyncerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_fsync: %d\n", code));
    return (code);
}

int xglue_read(vp, uiop, flags, acred)
struct vnode *vp;
struct uio *uiop;
int flags;
osi_cred_t *acred;
{
    register struct tkc_vcache *vcp;
    long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_read: (vp=0x%x, flags=0x%x)\n", vp, flags));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_read: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_RDWR, 0))) {
	    code = VOPN_READ(vp, uiop, flags, acred);
	    vol_EndVnodeOp(volp);
	}
	goto rderr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_WRITE|TKN_DATA_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_RDWR, 0))) {
	    code = VOPN_READ(vp, uiop, flags, acred);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

rderr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_read: %d\n", code));
    return (code);
}

int xglue_write(vp, uiop, flags,acred)
struct vnode *vp;
struct uio *uiop;
int flags;
osi_cred_t *acred;
{
    register struct tkc_vcache *vcp;
    long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_write: (vp=0x%x, flags=0x%x)\n", vp, flags));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_write: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_RDWR, 0))) {
	    code = VOPN_WRITE(vp, uiop, flags, acred);
	    vol_EndVnodeOp(volp);
	}
	goto wrerr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_WRITE|TKN_DATA_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_RDWR, 0))) {
	    code = VOPN_WRITE(vp, uiop, flags, acred);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
wrerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_write: %d\n", code));
    return (code);
}

#ifndef AFS_OSF11_ENV
int xglue_ioctl(vp, cmd, adata, flags, acred)
struct vnode *vp;
long cmd;
char *adata;
long flags;
osi_cred_t *acred;
#else
int xglue_ioctl(vp, cmd, adata, flags, acred, retval)
struct vnode *vp;
long cmd;
char *adata;
long flags;
osi_cred_t *acred;
int *retval;
#endif /* AFS_OSF11_ENV */
{
    long code;
    
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_ioctl: (vp=0x%x, cmd=0x%x, flags=%x)\n", 
	   vp, cmd, flags));
#ifndef AFS_OSF11_ENV
    code = VOPN_IOCTL(vp, cmd, adata, flags, acred);
#else
    code = VOPN_IOCTL(vp, cmd, adata, flags, acred, retval);
#endif /* AFS_OSF11_ENV */

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_ioctl: %d\n", code));
    return(code);
}

int xglue_readlink(vp, uiop, acred)
struct vnode *vp;
struct uio *uiop;
osi_cred_t *acred;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_readlink: (vp=0x%x)\n", vp));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_readlink: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_READLINK, 0))) {
	    code = VOPN_READLINK(vp, uiop, acred);
	    vol_EndVnodeOp(volp);
	}
	goto readlinkerr;
    }
    if (vcp = tkc_Get(vp, TKN_DATA_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_READLINK, 0))) {
	    code = VOPN_READLINK(vp, uiop, acred);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

readlinkerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_readlink: %d\n", code));
    return (code);
}

int xglue_select(vp, events, revents, scanning, acred)
struct vnode *vp;
short *events, *revents;
int scanning;
osi_cred_t *acred;
{
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_select: (vp=0x%x)\n", vp));
    return(VOPN_SELECT(vp, events, revents, scanning, acred));
}

int xglue_symlink(ndp, vap, target)
struct nameidata *ndp;
struct vattr *vap;
char *target;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    struct vnode *vp = ndp->ni_dvp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_symlink: (vp=0x%x)\n", vp));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_symlink: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SYMLINK, 0))) {
	    code = VOPN_SYMLINK(ndp, vap, target);
	    vol_EndVnodeOp(volp);
	}
	goto symlinkerr;
    }
    if (vcp = tkc_Get(vp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SYMLINK, 0))) {
	    code = VOPN_SYMLINK(ndp, vap, target);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
symlinkerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_symlink: %d\n", code));
    return (code);
}

int xglue_readdir(vp, uiop, acred, eofflagp)
struct vnode *vp;
struct uio *uiop;
osi_cred_t *acred;
int *eofflagp;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_readdir: (vp=0x%x)\n", vp));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_readdir: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_READDIR, 0))) {
	    code = VOPN_READDIR(vp, uiop, acred, eofflagp);
	    vol_EndVnodeOp(volp);
	}
	goto readdirerr;
    }
    if (vcp = tkc_Get(vp, TKN_DATA_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_READDIR, 0))) {
	    code = VOPN_READDIR(vp, uiop, acred, eofflagp);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
readdirerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_readdir: %d\n", code));
    return (code);
}

int xglue_strategy(bp)
    struct buf *bp;
{
    long code;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_strategy: (bp=0x%x)\n", bp));
    code = VOPN_STRATEGY(bp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_strategy: %d\n"));
    return(code);
}

int xglue_pgrd(vp, uiop, cred)
struct vnode *vp;
struct uio *uiop;
osi_cred_t *cred;
{
    register struct tkc_vcache *vcp;
    long code;
    struct volume *volp;
    long flags = 0; /* dummy argument */

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_pgrd: (vp=0x%x)\n", vp));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_pgrd: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_PGRD, 0))) {
	    code = VOPN_PGRD(vp, uiop, cred);
	    vol_EndVnodeOp(volp);
	}
	goto pgrderr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_WRITE|TKN_DATA_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_PGRD, 0))) {
	    code = VOPN_PGRD(vp, uiop, cred);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

pgrderr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_pgrd: %d\n", code));
    return (code);
}

#ifndef AFS_OSF11_ENV
int xglue_pgwr(vp, uiop, cred, pager, offset)
struct vnode	*vp;
struct uio 	*uiop;
osi_cred_t	*cred;
memory_object_t pager;
vm_offset_t	offset;
#else
int xglue_pgwr(vp, uiop, cred, async_args, async_func)
struct vnode	*vp;
struct uio 	*uiop;
osi_cred_t	*cred;
void   *async_args;
void   (*async_func)();
#endif /* AFS_OSF11_ENV */
{
    register struct tkc_vcache *vcp;
    long code;
    struct volume *volp;
    long flags = 0; /* dummy argument */

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_pgwr: (vp=0x%x)\n", vp));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_pgwr: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_PGWR, 0))) {
#ifndef AFS_OSF11_ENV
	    code = VOPN_PGWR(vp, uiop, cred, pager, offset);
#else
	    code = VOPN_PGWR(vp, uiop, cred, async_args, async_func);
#endif /* AFS_OSF11_ENV */
	    vol_EndVnodeOp(volp);
	}
	goto pgwrerr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_WRITE|TKN_DATA_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_PGWR, 0))) {
#ifndef AFS_OSF11_ENV
	    code = VOPN_PGWR(vp, uiop, cred, pager, offset);
#else
	    code = VOPN_PGWR(vp, uiop, cred, async_args, async_func);
#endif /* AFS_OSF11_ENV */
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

pgwrerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_pgwr: %d\n", code));
    return (code);
}

/* The following represent glue for vnode operations added by OSF/1
   For now, most of these will do minimal work */

int xglue_inactive(vp)
    struct vnode *vp;
{
    long code;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_inactive: (vp=0x%x)\n", vp));
    code = VOPN_INACTIVE(vp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_inactive: %d\n", code));
    return(code);
}

int xglue_bmap(vp, bn, vpp, bnp)
    struct vnode *vp;
    daddr_t bn;
    struct vnode **vpp;
    daddr_t *bnp;
{
    long code;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_bmap: (vp=0x%x)\n", vp));
    code = VOPN_BMAP(vp, bn, vpp, bnp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_bmap: %d\n", code));
    return(code);
}

int xglue_abortop(ndp)
    struct nameidata *ndp;
{
    long code;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_abortop: (dp=0x%x,vp=0x%x)\n", ndp->ni_dvp, ndp->ni_vp));
    code = VOPN_ABORTOP(ndp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_abortop: %d\n", code));
    return(code);
}

int xglue_mmap(vp, fflags, cred)
    struct vnode *vp;
    int fflags;
    osi_cred_t *cred;
{
    long code;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_mmap: (vp=0x%x)\n", vp));
    code = VOPN_MMAP(vp, fflags, cred);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_mmap: %d\n", code));
    return(code);
}

int xglue_print(vp)
    struct vnode *vp;
{
    long code;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_print: (vp=0x%x)\n", vp));
    code = VOPN_PRINT(vp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_print: %d\n", code));
    return(code);
}

int xglue_reclaim(vp)
    struct vnode *vp;
{
    long code;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_reclaim: (vp=0x%x)\n", vp));
    code = VOPN_RECLAIM(vp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_reclaim: %d\n", code));
    return(code);
}

int xglue_seek(vp, oldoff, newoff, cred)
    struct vnode *vp;
    off_t oldoff, newoff;
    osi_cred_t *cred;
{
    long code;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_seek: (vp=0x%x)\n", vp));
    code = VOPN_SEEK(vp, oldoff, newoff, cred);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_seek: %d\n", code));
    return(code);
}

int xglue_unmount (vfsp, flag)
    struct osi_vfs *vfsp;		/* virtual file system */
    int flag;
{
    long code;				/* error return code */
    struct volume *volp;		/* volume registry entry */

    code = xvfs_VfsGetVolume (vfsp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("xglue_unmount: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens (volp)) {
	if (!(code = vol_StartVnodeOp(volp, VFSOP_UNMOUNT, 0))) {
	    code = VFSX_UNMOUNT (vfsp);
	    vol_EndVnodeOp(volp);
	}
	goto done;
    }

    if (tkc_flushvfsp (vfsp))
	return EBUSY;

    if (!(code = vol_StartVnodeOp(volp, VFSOP_UNMOUNT, 0))) {
	code = VFSX_UNMOUNT (vfsp);
	vol_EndVnodeOp(volp);
    }

done:
    xvfs_PutVolume (volp);
    return (code);
}

int xglue_root (vfsp, vpp)
    struct mount *vfsp;			/* virtual file system */
    struct vnode **vpp;			/* where to put vnode pointer */
{
    long code;				/* error return code */
    struct volume *volp;		/* volume registry entry */

    *vpp = NULL;	/* clear any garbage of caller's */
    code = xvfs_VfsGetVolume (vfsp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("xglue_root: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens (volp)) {
	if (!(code = vol_StartVnodeOp(volp, VFSOP_ROOT, 0))) {
	    code = VFSX_ROOT (vfsp, vpp);
	    if (volp && !code) /* registered volume but not exported? */
		XVFS_CONVERT (*vpp);
	    vol_EndVnodeOp(volp);
	}
	goto done;
    }

    if (!(code = vol_StartVnodeOp(volp, VFSOP_ROOT, 0))) {
	code = VFSX_ROOT (vfsp, vpp);
	if (!code)
	    XVFS_CONVERT (*vpp);
	vol_EndVnodeOp(volp);
    }

done:
    xvfs_PutVolume (volp);
    return (code);
}

int xglue_fhtovp (vfsp, fidp, vpp)
    struct mount *vfsp;                   /* virtual file system */
    struct fid *fidp;                   /* file ID */
    struct vnode **vpp;                 /* where to put vnode pointer */
{
    long code;                          /* error return code */
    struct volume *volp;                /* volume registry entry */

    code = xvfs_VfsGetVolume (vfsp, &volp, VFSOP_FHTOVP);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("xglue_fhtovp: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens (volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_TYPE_READWRITE, 0))) {
	    code = VFSX_FHTOVP (vfsp,  fidp, vpp);
	    if (volp && !code) /* registered volume but not exported? */
		XVFS_CONVERT (*vpp);
	    vol_EndVnodeOp(volp);
	}
        goto done;
    }

    if (!(code = vol_StartVnodeOp(volp, VNOP_TYPE_READWRITE, 0))) {
	code = VFSX_FHTOVP (vfsp, fidp, vpp);
	if (!code)
	    XVFS_CONVERT (*vpp);
	vol_EndVnodeOp(volp);
    }

  done:
    xvfs_PutVolume (volp);
    return (code);
}

#ifdef AFS_OSF11_ENV
int xglue_filelock(vp, bf, flag, offset)
struct vnode *vp;
struct flock *bf;
int flag;
off_t offset;
{
	long code = 0, tokenType, startPos, endPos;
	register struct tkc_vcache *vcp;
	tkc_byteRange_t byteRange;
	struct volume *volp;
	afs_recordLock_t lockBlocker;
	short oldwhence;
	struct flock clndata;

	/*
	 * The ufs flock manager can be called with numerous
	 * flag settings that don't concern the cache manager.
	 */
	if (!(flag & (CLNFLCK|GETFLCK|SETFLCK))) {
		code = VOPN_FILELOCK(vp, bf, flag, offset);
		return code;	/* not "goto done" -- don't have volp */
	}
	/*
	 * Convert OSF/1-specific CLNFLCK call into an F_UNLCK
	 * over entire file.
 	 */
	if (flag & CLNFLCK) {
		flag = SETFLCK;
		bf = &clndata;
		bf->l_pid = u.u_procp->p_pid;
		bf->l_whence = 0;
		bf->l_start = 0;
		bf->l_len = 0;
		bf->l_type = F_UNLCK;
	}
	osi_assert(bf);
	oldwhence = bf->l_whence;
	if (code = xvfs_GetVolume(vp, &volp, VNOP_LOCKCTL))
		return code;
	if (!xvfs_NeedTokens(volp)) {
		code = VOPN_FILELOCK(vp, bf, flag, offset);
		goto done;
	}
	switch (bf->l_whence) {
	case SEEK_SET: 
		startPos = bf->l_start; 
		break;
	case SEEK_CUR: 
		startPos = bf->l_start + offset; 
		break;
	case SEEK_END: {
		register struct tkc_vcache *tvcp;
		struct vattr va;

		if (tvcp = tkc_Get(vp, TKN_STATUS_READ, 0)) {
		    if (!(code = vol_StartVnodeOp(volp, VNOP_GETATTR, 0))) {
			code = VOPN_GETATTR(vp, &va, u.u_cred);
			vol_EndVnodeOp(volp);
		    }
		    tkc_Put(tvcp);
		} else {
			code = EINVAL;
			goto done;
		}
		startPos = bf->l_start + va.va_size;
		break;
	}
	default: 
		code = EINVAL;
		goto done;
	}
	endPos = (bf->l_len == 0 ? 0x7fffffff : startPos + bf->l_len - 1);

	AFS_hset64(byteRange.beginRange, 0, startPos);
	AFS_hset64(byteRange.endRange, 0, endPos);

	if (bf->l_type == F_UNLCK) {
		code = VOPN_FILELOCK(vp, bf, flag, offset);
		if (code = tkc_PutLocks(vp, &byteRange)) {
			code = EINVAL;
			goto done;
		}
	} else {
		tokenType = (bf->l_type == F_RDLCK ? TKN_LOCK_READ : TKN_LOCK_WRITE);
		code = tkc_GetLocks(vp, tokenType, &byteRange,((flag&SLPFLCK)? 1 : 0),
			&lockBlocker);
		if (code) {
			/*
			 * If only F_GETLK, return the blocker's info
			 */
			if (flag & GETFLCK) {
				code = 0;
				bf->l_whence = oldwhence;
				bf->l_type = lockBlocker.l_type;
				bf->l_start = AFS_hgetlo(lockBlocker.l_start_pos);
				bf->l_pid = lockBlocker.l_pid;
				if (AFS_hgetlo(lockBlocker.l_end_pos) == 0x7fffffff)
					bf->l_len = 0;
				else
					bf->l_len = AFS_hgetlo(lockBlocker.l_end_pos) -
					    AFS_hgetlo(lockBlocker.l_start_pos) + 1;
			}
			goto done;
		}
		code = VOPN_FILELOCK(vp, bf, flag, offset);
		tkc_FinishGetLocks(vp);
	}
done:
	if (!code && (flag & SETFLCK) && bf->l_type != F_UNLCK) {
		/*
		 * Once a lock has been set successfully, OSF/1 expects
		 * a flag bit to be set in the vnode.  If this flag isn't
		 * set, then the filelock vnode op won't be called at
		 * process termination to clean up the lock.
		 */
		VN_LOCK(vp);
		vp->v_flag |= VLOCKS;
		VN_UNLOCK(vp);
	}
	xvfs_PutVolume(volp);
	return code;
}
#endif /* AFS_OSF11_ENV */
