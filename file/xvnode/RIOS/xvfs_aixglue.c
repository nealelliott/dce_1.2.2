/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xvfs_aixglue.c,v $
 * Revision 1.1.733.1  1996/10/02  19:04:00  damon
 * 	New DFS from Transarc
 * 	[1996/10/01  18:59:36  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>	
#include <dcedfs/sysincludes.h>
#include <dcedfs/volume.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/tkc.h>
#include <dcedfs/osi_mount.h>

#include <sys/user.h>
#include <sys/uio.h>
#include <sys/flock.h>
#include <sys/inode.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/xvnode/RIOS/xvfs_aixglue.c,v 1.1.733.1 1996/10/02 19:04:00 damon Exp $")

/*
 * Generic token glue layer that guarantees data consistency among multiple
 * physical file systems.
 */

int xglue_link(vp, dp, namep)
    struct vnode *vp, *dp;
    char *namep;
  {
    register long code;
    struct volume *volp1, *volp2;
    struct tkc_sets sets[2];
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_link (namep=%s) = ", namep));
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
redo:
    if (!xvfs_NeedTokens(volp1)) {
	if (!(code = vol_StartVnodeOp(volp1, VNOP_LINK, 0, &dmptr))) {
	    code = VOPN_LINK(vp, dp, namep);
	    vol_EndVnodeOp(volp1, &dmptr);
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
    if (!(code = vol_StartVnodeOp(volp1, VNOP_LINK, 0, &dmptr))) {
	code = VOPN_LINK(vp, dp, namep);
	vol_EndVnodeOp(volp1, &dmptr);
    }
    tkc_PutTokens(sets, 2);

linkerr:
    if (dmptr && volp1) {
	if (VOL_DMWAIT(volp1, &dmptr)) goto redo;
	VOL_DMFREE(volp1, &dmptr);
    }
    xvfs_PutVolume(volp1); xvfs_PutVolume(volp2);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_mkdir(dp, namep, mode)
    struct vnode *dp;
    char *namep;
    int mode;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_mkdir (namep=%s mode=%x) = ", namep, mode));
    code = xvfs_GetVolume(dp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_mkdir: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_MKDIR, 0, &dmptr))) {
	    code = VOPN_MKDIR(dp, namep, mode);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto mkdirerr;
    }    
    if (vcp = tkc_Get(dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_MKDIR, 0, &dmptr))) {
	    code = VOPN_MKDIR(dp, namep, mode);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
mkdirerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_mknod(dp, namep, mode, dev)
    struct vnode *dp;
    char *namep;
    int mode;
    dev_t dev;
{
    register struct tkc_vcache *vcp;
    long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_mknod (namep=%s, mode=%x) = ",  namep, mode));
    code = xvfs_GetVolume(dp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_mknod: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_MKNOD, 0, &dmptr))) {
	    code = VOPN_MKNOD(dp, namep, mode, dev);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto mknoderr;
    }    
    if (vcp = tkc_Get(dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_MKNOD, 0, &dmptr))) {
	    code = VOPN_MKNOD(dp, namep, mode, dev);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
mknoderr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_remove(vp, dp, namep)
    struct vnode *vp, *dp;
    char *namep;
{
    struct tkc_sets sets[2];
    register long code;
    struct volume *volp1, *volp2;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_remove (dp=0x%x, namep=%s) = ", dp, namep));
    code = xvfs_GetVolume(vp, &volp1);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_remove: %d\n", code));
      return(code);
    }
redo:
    code = xvfs_GetVolume(dp, &volp2);
    if (code) {
      xvfs_PutVolume(volp1);
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_remove: %d\n", code));
      return(code);
    }
    if (volp1 != volp2) {
	code = EXDEV;
	goto remerr;
    }
    if (!xvfs_NeedTokens(volp1)) {
	if (!(code = vol_StartVnodeOp(volp1, VNOP_REMOVE, 0, &dmptr))) {
	    code = VOPN_REMOVE(vp, dp, namep);
	    vol_EndVnodeOp(volp1, &dmptr);
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
    if (!(code = vol_StartVnodeOp(volp1, VNOP_REMOVE, 0, &dmptr))) {
	code = VOPN_REMOVE(vp, dp, namep);
	vol_EndVnodeOp(volp1, &dmptr);
    }
    tkc_PutTokens(sets, 2);
    tkc_HandleDelete(vp, volp1);
remerr:
    if (dmptr && volp1) {
	if (VOL_DMWAIT(volp1, &dmptr)) goto redo;
	VOL_DMFREE(volp1, &dmptr);
    }
    xvfs_PutVolume(volp1); xvfs_PutVolume(volp2);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_rename(vp, dp, namep, tp, tdp, tnamep)
    struct vnode *vp, *dp;
    char *namep;
    struct vnode *tp, *tdp;
    char *tnamep;
{
    struct tkc_sets sets[4];
    register long code, size;
    struct volume *volp1, *volp2;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rename (namep=%s, tnamep=%s) = ", namep, tnamep));
    if (VTOIP(dp)->i_dev != VTOIP(tdp)->i_dev)
	return EXDEV;

    code = xvfs_GetVolume(dp, &volp1);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rename: %d\n", code));
      return(code);
    }
    code = xvfs_GetVolume(tdp, &volp2);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rename: %d\n", code));
      xvfs_PutVolume(volp1);
      return(code);
    }
    /* need to check both since otherwise could dephantomize 2nd dir
     * if it is offline at the time.
     */
    if (volp1 != volp2) {
	code = EXDEV;
	goto renerr;
    }
redo:
    if (!xvfs_NeedTokens(volp1)) {
	if (!(code = vol_StartVnodeOp(volp1, VNOP_RENAME, 0, &dmptr))) {
	    code = VOPN_RENAME(vp, dp, namep, tp, tdp, tnamep);
	    vol_EndVnodeOp(volp1, &dmptr);
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

    /* note that tkc_Set is a macro! */
    tkc_Set(0, dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
    size = 1;
    if (tdp != dp) {
	tkc_Set(size, tdp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
	size++;
    }
    tkc_Set(size, vp, TKN_STATUS_WRITE|TKN_DATA_WRITE, 0);
    size++;
    if (tp) {
	tkc_Set(size, tp, TKN_STATUS_WRITE, 0);
	size++;
    }
    tkc_GetTokens(sets, size);
    if (tp) OSI_VN_HOLD(tp);
    if (!(code = vol_StartVnodeOp(volp1, VNOP_RENAME, 0, &dmptr))) {
	code = VOPN_RENAME(vp, dp, namep, tp, tdp, tnamep);
	vol_EndVnodeOp(volp1, &dmptr);
    }
    tkc_PutTokens(sets, size);
    if (tp) {
	tkc_HandleDelete(tp, volp1);
	OSI_VN_RELE(tp);
    }

renerr:
    if (dmptr && volp1) {
	if (VOL_DMWAIT(volp1, &dmptr)) goto redo;
	VOL_DMFREE(volp1, &dmptr);
    }
    xvfs_PutVolume(volp1);
    xvfs_PutVolume(volp2);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}

int xglue_rmdir(vp, dp, namep)
    struct vnode *vp, *dp;
    char *namep;
{
    struct tkc_sets sets[2];
    register long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rmdir (namep=%s) = ", namep));
    code = xvfs_GetVolume(dp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rmdir: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_RMDIR, 0, &dmptr))) {
	    code = VOPN_RMDIR(vp, dp, namep);
	    vol_EndVnodeOp(volp, &dmptr);
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
    if (!(code = vol_StartVnodeOp(volp, VNOP_RMDIR, 0, &dmptr))) {
	code = VOPN_RMDIR(vp, dp, namep);
	vol_EndVnodeOp(volp, &dmptr);
    }
    tkc_PutTokens(sets, 2);
    tkc_HandleDelete(vp, volp);
    OSI_VN_RELE(vp);

rmdirerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_lookup(dp, vpp, namep, flags)
    struct vnode *dp;
    struct vnode **vpp;
    char *namep;
    int flags;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_lookup (namep=%s) = ", namep));
    *vpp = NULL;	/* eliminate any stack garbage in caller */
    code = xvfs_GetVolume(dp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_lookup: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_LOOKUP, 0, &dmptr))) {
	    code = VOPN_LOOKUP(dp, vpp, namep, flags);
	    if (volp && !code) /* registered volume but not exported? */
		XVFS_CONVERT(*vpp);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	 goto lookerr;
    }
    if (vcp = tkc_Get(dp, TKN_DATA_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_LOOKUP, 0, &dmptr))) {
	    code = VOPN_LOOKUP(dp, vpp, namep, flags);
	    if (!code)
		XVFS_CONVERT(*vpp);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
lookerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_open(vp, flags, ext, vinfopp)
    struct vnode *vp;
    long flags;
    int ext;
    osi_cred_t **vinfopp;
{
    struct tkc_vcache *vcp;
    register long code;
    long hoFlags;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_open (flags=0x%x) = ", flags));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_open: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_OPEN, 0, &dmptr))) {
	    code = VOPN_OPEN(vp, flags, ext, vinfopp);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto openerr;
    }

    /* Set flags for TKC HandleOpen call */
    if (flags & FEXEC)
	hoFlags = 2;
    else if (flags & FWRITE)
	hoFlags = 1;
    else
	hoFlags = 0;
    /* On AIX, VOPN_OPEN can do truncation */
    if (flags & FTRUNC)
	hoFlags |= 4;

    if (!(code = tkc_HandleOpen(vp, hoFlags, &vcp))) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_OPEN, 0, &dmptr))) {
	    code = VOPN_OPEN(vp, flags, ext, vinfopp);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Release(vcp);
	/* ensure that HandleOpen's matches HandleClose calls */
	if (code) {
	    if (vcp = tkc_HandleClose(vp, hoFlags)) {
		tkc_Release(vcp);
	    }
	}
    }

openerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_create(dp, vpp, flags, namep, mode, vinfopp)
    struct vnode *dp, **vpp;
    int flags, mode;
    char *namep;
    osi_cred_t **vinfopp;
{
    struct tkc_vcache *vcp;
    register long code;
    int hoFlags;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG,1,("In xglue_create (namep=%s, flags=0x%x,m ode=0x%x)=",
	   namep, flags, mode));
    *vpp = NULL;	/* eliminate any stack garbage in caller */
    code = xvfs_GetVolume(dp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_create: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_CREATE, 0, &dmptr))) {
	    code = VOPN_CREATE(dp, vpp, flags, namep, mode, vinfopp);
	    if (volp && !code) /* registered volume but not exported? */
		XVFS_CONVERT(*vpp);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto createrr;
    }
    if (vcp = tkc_Get(dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_CREATE, 0, &dmptr))) {
	    code = VOPN_CREATE(dp, vpp, flags, namep, mode, vinfopp);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

    /* now open the file, since AIX assumes that the open vnode processing is
     * invoked here.
     */
    if (code == 0) {
        XVFS_CONVERT(*vpp);
	if (flags & FEXEC) hoFlags = 2;
	else if (flags & FWRITE) hoFlags = 1;
	else hoFlags = 0;
	if (!(code = tkc_HandleOpen(*vpp, hoFlags, &vcp))) {
	    /* VOPN_CREATE will have called VOPX_OPEN (xvfs_aix2vfs.c)
	     * for us if it succeeded; thus we don't have to call it here,
	     * but we do have to obtain an open token.
	     */
	    tkc_Release(vcp);
	}
	else {
	    /* we've opened the file, but couln't get the open token. The basic
	     * create vnode operation is going to fail, but the VOPN_CREATE
	     * that we called succeeded.  We send it a close so that it can
	     * match opens and closes.  Don't damage return code.
	     */
	    VOPN_CLOSE(*vpp, flags, *vinfopp);
	}
    }

createrr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_hold(vp)
    struct vnode *vp;
{
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_hold=%x (ref=%d)\n", vp, vp->v_count));
    return (VOPN_HOLD(vp));
}


/* 
 * Inactive on ZLC files need to be blocked if volume operations
 * are in progress. Also if the the inactive request is from a volop
 * we need to let it pass through.
 */
int xglue_rele(vp)
    struct vnode *vp;
{
     int code1, code2;
     struct volume *volp = NULL;
     opaque dmptr = NULL;

     /*
      * XXX We may need to get tokens since VOPN_RELE calls VOPX_INACTIVE on 
      * last reference.
      */
     AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rele=%x (ref=%d)\n", vp, vp->v_count));

     code1 = xvfs_GetVolume(vp, &volp);
 redo:
     if (!code1 && (volp != NULL)) {
	 vol_StartInactiveVnodeOp(volp, vp, &dmptr);
     }
     code2 = VOPN_RELE(vp);
     if (!code1 && (volp != NULL)) {
       vol_EndVnodeOp(volp, &dmptr);
       if (dmptr && volp) {
	   if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	   VOL_DMFREE(volp, &dmptr);
       }
       xvfs_PutVolume(volp);
     }
     return code2;
}


int xglue_close(vp, flags, vinfo)
    struct vnode *vp;
    int flags;
    caddr_t vinfo;
{
    register struct tkc_vcache *vcp;
    register long code;
    int hoFlags;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_close (flags=0x%x) = ", flags));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_close: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_CLOSE, 0, &dmptr))) {
	    code = VOPN_CLOSE(vp, flags, vinfo);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto closerr;
    }
    /*
     * XXX Do we really need to get the OPEN tokens again; they've gotten
     * during the open system call.  Answer (kazar): No.
     */
    if (!(code = vol_StartVnodeOp(volp, VNOP_CLOSE, 0, &dmptr))) {
	code = VOPN_CLOSE(vp, flags, vinfo);
	vol_EndVnodeOp(volp, &dmptr);
    }

    if (flags & FEXEC) hoFlags = 2;
    else if (flags & FWRITE) hoFlags = 1;
    else hoFlags = 0;
    if (vcp = tkc_HandleClose(vp, hoFlags)) {
	/* HandleClose actually does all of our hard work */
	tkc_Release(vcp);
    }

closerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_map(vp, addr, length, offset, flag)
    u_int length, offset, flag;
    struct vnode *vp;
    caddr_t addr;
{
    register struct tkc_vcache *vcp;
    long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_map\n"));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_map: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_MAP, 0, &dmptr))) {
	    code = VOPN_MAP(vp, addr, length, offset, flag);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto out;
    }
    if (vcp = tkc_Get(vp, TKN_UPDATE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_MAP, 0, &dmptr))) {
	    code = VOPN_MAP(vp, addr, length, offset, flag);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
out:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_unmap(vp, vinfo)
    struct vnode *vp;
    caddr_t vinfo;
{
    register struct tkc_vcache *vcp;
    long code, dataToken;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_unmap\n"));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_unmap: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_UNMAP, 0, &dmptr))) {
	    code = VOPN_UNMAP(vp, vinfo);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto out;
    }
    /* Since it cannot be determined whether file is read or write
     * mapped at this point assume it is write mapped.  This way
     * if the file was written to locally, the read and write tokens
     * will be revoked from the clients, causing them to get the new
     * version of the file.
     */
    if (vcp = tkc_Get(vp, TKN_UPDATE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_UNMAP, 0, &dmptr))) {
	    code = VOPN_UNMAP(vp, vinfo);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
out:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_access(vp, mode, edp)
    struct vnode *vp;
    long mode;
    struct exec_data *edp;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_access (mode=0x%x) = ", mode));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_access: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_ACCESS, 0, &dmptr))) {
	    code = VOPN_ACCESS(vp, mode, edp);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto accesserr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_ACCESS, 0, &dmptr))) {
	    code = VOPN_ACCESS(vp, mode, edp);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

accesserr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_getattr(vp, vattrp)
    struct vnode *vp;
    struct vattr *vattrp;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_getattr (vattrp=0x%x) = ", vattrp));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_getattr: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_GETATTR, 0, &dmptr))) {
	    code = VOPN_GETATTR(vp, vattrp);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto getattrerr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_GETATTR, 0, &dmptr))) {
	    code = VOPN_GETATTR(vp, vattrp);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

getattrerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_setattr(vp, op, arg1, arg2, arg3)
    struct vnode *vp;
    int op, arg1, arg2, arg3;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_setattr (op = %d, args = %o, %d, %d)\n", 
	   op, arg1, arg2, arg3));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_setattr: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SETATTR, 0, &dmptr))) {
	    code = VOPN_SETATTR(vp, op, arg1, arg2, arg3);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto setattrerr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SETATTR, 0, &dmptr))) {
	    code = VOPN_SETATTR(vp, op, arg1, arg2, arg3);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

setattrerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_setattr\n"));
    return (code);
}


int xglue_fclear(vp, flags, offp, length, vinfo)
    struct vnode *vp;
    int flags;
    long *offp;
    u_long length;
    caddr_t vinfo;
{
    register long code;
    register struct tkc_vcache *vcp;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_fclear\n"));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_fclear: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_RDWR, 0, &dmptr))) {
	    code = VOPN_FCLEAR(vp, flags, offp, length, vinfo);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto fclearerr;
    }
    /*
     * Note: must call settattr due to a bug in fclear which does not
     * update mtime and ctime properly
     */
    if (vcp = tkc_Get(vp, TKN_STATUS_WRITE|TKN_DATA_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_RDWR, 0, &dmptr))) {
	    if (!(code = VOPN_FCLEAR(vp, flags, offp, length, vinfo)))
		code = VOPN_SETATTR(vp, V_STIME, 0, osi_Time(), osi_Time());
	    vol_EndVnodeOp(volp, &dmptr);
	}
        tkc_Put(vcp);
    } else
        code = EINVAL;
fclearerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("Exit xglue_fclear\n"));
    return code;
}

int xglue_fsync(vp, flags, vinfo)
    struct vnode *vp;
    int flags;
    caddr_t vinfo;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_fsync\n"));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_fsync: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_FSYNC, 0, &dmptr))) {
	    code = VOPN_FSYNC(vp, flags, vinfo);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto fsyncerr;
    }
    if (vcp = tkc_Get(vp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_FSYNC, 0, &dmptr))) {
	    code = VOPN_FSYNC(vp, flags, vinfo);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
fsyncerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_ftrunc(vp, flags, length, vinfo)
    struct vnode *vp;
    int flags;
    u_long length;
    caddr_t vinfo;
{
    register struct tkc_vcache *vcp;
    long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_ftrunc (length=%d) = ", length));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_ftrunc: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_FTRUNC, 0, &dmptr))) {
	    code = VOPN_FTRUNC(vp, flags, length, vinfo);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto ftruncerr;
    }
    if (vcp = tkc_Get(vp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_FTRUNC, 0, &dmptr))) {
	    code = VOPN_FTRUNC(vp, flags, length, vinfo);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

ftruncerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_rdwr(vp, op, flags, uiop, ext, vinfo)
    struct vnode *vp;
    enum uio_rw op;
    int flags;
    struct uio *uiop;
    caddr_t vinfo;
{
    register struct tkc_vcache *vcp;
    long code, dataToken;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rdwr (op=%d, flags=0x%x) = ", op, flags));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rdwr: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_RDWR, 0, &dmptr))) {
	    code = VOPN_RDWR(vp, op, flags, uiop, ext, vinfo);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto rdwrerr;
    }
    dataToken = (op == UIO_READ ? TKN_DATA_READ : TKN_DATA_WRITE);
    if (vcp = tkc_Get(vp, TKN_STATUS_WRITE|dataToken, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_RDWR, 0, &dmptr))) {
	    code = VOPN_RDWR(vp, op, flags, uiop, ext, vinfo);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
rdwrerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


/* The lockctl vnode operations themselves shouldn't be done under a
 * vol_StartVnodeOp.  This is because if the VOP*_LOCKCTL call blocks,
 * we don't want to be left blocking out new volume operations while waiting
 * for the lock.  Note that this means all file systems have to implement
 * VOP*_LOCKCTL such that it can run correctly independent of conflicting
 * volume operations (although the token work have been done properly).
 *
 * Note also that this doesn't apply to the VOP_GETATTR done here, since
 * it doesn't block waiting for other vnode operations to complete.
 */
int xglue_lockctl(vp, offset, lckdat, cmd, ignored_fcn, ignored_id)
    void (*ignored_fcn)(), *ignored_id;
    struct vnode *vp;
    off_t offset;
    struct flock *lckdat;
    int cmd;
{
    long code = 0, tokenType, startPos, endPos;
    register struct tkc_vcache *vcp;
    tkc_byteRange_t byteRange;
    struct volume *volp;
    afs_recordLock_t lockBlocker;
    int oldwhence = lckdat->l_whence;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_lockctl (cmd=0x%x) = ", cmd));

    if (u.u_vfs == MNT_NFS)
	return ENOLCK;
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_lockctl: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	code = VOPN_LOCKCTL(vp, offset, lckdat, cmd, ignored_fcn, ignored_id);
	goto lckerr;
    }

    switch (lckdat->l_whence) {
	case 0:
	    startPos = lckdat->l_start;
	    break;
	case 1:
	    startPos = lckdat->l_start + offset;
	    break;
	case 2:	
	{
	    register struct tkc_vcache *tvcp;
	    struct vattr va;
	    if (tvcp = tkc_Get(vp, TKN_STATUS_READ, 0)) {
		if (!(code = vol_StartVnodeOp(volp, VNOP_GETATTR, 0, &dmptr))) {
		    code = VOPN_GETATTR(vp, &va);
		    vol_EndVnodeOp(volp, &dmptr);
		}
		tkc_Put(tvcp);
	    } else {
		code = EINVAL;
		goto lckerr;
	    }

	    startPos = lckdat->l_start + va.va_size; 
	    break;
	}
	default:
	    code = EINVAL;
	    goto lckerr;
     }
    endPos = (lckdat->l_len == 0 ?  0x7fffffff : startPos + lckdat->l_len - 1);

    AFS_hset64(byteRange.beginRange, 0, startPos);
    AFS_hset64(byteRange.endRange, 0, endPos);

    if (lckdat->l_type == F_UNLCK) {
	code = VOPN_LOCKCTL(vp, offset, lckdat, cmd, ignored_fcn, ignored_id);
	if (code = tkc_PutLocks(vp, &byteRange)) {
	    code = EINVAL;	       
	    goto lckerr;
	}
    }  else {
	tokenType = (lckdat->l_type == F_RDLCK ? TKN_LOCK_READ:TKN_LOCK_WRITE);
	code = tkc_GetLocks(vp, tokenType, &byteRange, ((cmd&SLPFLCK)? 1 : 0),
			    &lockBlocker);
	if (code) {
	    /*
	     * If only F_GETLK, return the blocker's info
	     */
	    if (cmd == 0 /* ie., AIX way of saying F_GETLK */) {
	        code = 0;
		lckdat->l_whence = oldwhence;
		lckdat->l_type = lockBlocker.l_type;
		lckdat->l_start = AFS_hgetlo(lockBlocker.l_start_pos);
		lckdat->l_pid = lockBlocker.l_pid;
		lckdat->l_sysid = lockBlocker.l_sysid;
		lckdat->l_vfs	= OSI_MOUNT_TYPE_DFS;
		if (AFS_hgetlo(lockBlocker.l_end_pos) == 0x7fffffff)
		   lckdat->l_len = 0;
		else
		   lckdat->l_len = AFS_hgetlo(lockBlocker.l_end_pos) -
		       AFS_hgetlo(lockBlocker.l_start_pos) + 1;
	    }
	    goto lckerr;
	}
	code = VOPN_LOCKCTL(vp, offset, lckdat, cmd, ignored_fcn, ignored_id);
	tkc_FinishGetLocks(vp);
    }

lckerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}

int xglue_ioctl(vp, cmd, arg, flags, ext)
    struct vnode *vp;
    int cmd, arg, flags, ext;
{
    register long code;

    /*
     * XXX ADD SOME TOKEN SUPPORT FOR THIS!! XXXX
     */
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_ioctl (cmd=0x%x, flags=%x, arg=%x) = ", 
	   cmd, flags, arg));
    code = VOPN_IOCTL(vp, cmd, arg, flags, ext);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_readlink(vp, uiop)
    struct vnode *vp;
    struct uio *uiop;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_readlink = "));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_readlink: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_READLINK, 0, &dmptr))) {
	    code = VOPN_READLINK(vp, uiop);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto readlinkerr;
    }
    if (vcp = tkc_Get(vp, TKN_DATA_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_READLINK, 0, &dmptr))) {
	    code = VOPN_READLINK(vp, uiop);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

readlinkerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_select(vp, corl, reqevs, rtnevs, notify, vinfo)
    int corl, reqevs, rtnevs, notify, vinfo;
    struct vnode *vp;
{
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_select\n"));
    return (VOPN_SELECT(vp, corl, reqevs, rtnevs, notify, vinfo));
}


int xglue_symlink(vp, namep, targetp)
    struct vnode *vp;
    char *namep, *targetp;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_symlink (namep=%s, target=%s) = ", 
	   namep, targetp));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_symlink: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SYMLINK, 0, &dmptr))) {
	    code = VOPN_SYMLINK(vp, namep, targetp);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto symlinkerr;
    }    
    if (vcp = tkc_Get(vp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SYMLINK, 0, &dmptr))) {
	    code = VOPN_SYMLINK(vp, namep, targetp);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

symlinkerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}


int xglue_readdir(vp, uiop)
    struct vnode *vp;
    struct uio *uiop;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_readdir = "));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_readdir: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_READDIR, 0, &dmptr))) {
	    code = VOPN_READDIR(vp, uiop);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto readdirerr;
    }
    if (vcp = tkc_Get(vp, TKN_DATA_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_READDIR, 0, &dmptr))) {
	    code = VOPN_READDIR(vp, uiop);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

readdirerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}

int xglue_strategy(vp, bp)
struct vnode *vp;
struct buf *bp;
{
    return (VOPN_STRATEGY(vp, bp));    
}


int xglue_revoke(vp, cmd, flags, vattrp)
    int cmd, flags;
    struct vattr *vattrp;
    struct vnode *vp;
{
    /* 
     * XXX Any TOKENS here?? XXX
     */
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_revoke\n"));
    return (VOPN_REVOKE(vp, cmd, flags, vattrp));    
}


int xglue_fid(vp, fidp)
    struct vnode *vp;
    struct fid *fidp;
{
    int code; 

    /* 
     * XXX Any TOKENS here?? XXX
     */
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_fid - WHO CALLS THIS??? = "));
    code = VOPN_FID(vp, fidp);    
    AFSLOG(XGLUE_DEBUG, 1, ("%d\n", code));
    return (code);
}

int xglue_getacl(vp, uiop)
    struct vnode *vp;
    struct uio   *uiop;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In BEGIN xglue_getacl \n"));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_getacl: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_GETACL, 0, &dmptr))) {
	    code = VOPN_GETACL(vp, uiop);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto getaclerr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_GETACL, 0, &dmptr))) {
	    code = VOPN_GETACL(vp, uiop);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
        code = EINVAL;

getaclerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In END xglue_getacl (code %d)\n", code));
    return (code);

}

int xglue_setacl(vp, uiop)
    struct vnode *vp;
    struct uio   *uiop;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    opaque dmptr = NULL;

    AFSLOG(XGLUE_DEBUG, 1, ("In BEGIN xglue_setacl \n"));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_setacl: %d\n", code));
      return(code);
    }
redo:
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SETACL, 0, &dmptr))) {
	    code = VOPN_SETACL(vp, uiop);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto setaclerr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SETACL, 0, &dmptr))) {
	    code = VOPN_SETACL(vp, uiop);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	tkc_Put(vcp);
    } else
        code = EINVAL;

setaclerr:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("In END xglue_setacl (code %d)\n", code));
    return (code);
}

int xglue_unmount (vfsp, flag)
    struct vfs *vfsp;			/* virtual file system */
    int flag;
{
    long code;				/* error return code */
    struct volume *volp;		/* volume registry entry */
    struct vfs *tvfsp;			/* loop control */
    opaque dmptr = NULL;

    /*
     * Check for mount points embedded in current systems.  This is supposed
     * to be done above the VFS layer, but we have set GFS_FUMNT, so we must
     * do it ourselves.
     */
    tvfsp = vfsp->vfs_next;
    while (tvfsp) {
	if (tvfsp->vfs_mntdover
	    && tvfsp->vfs_mntdover->v_vfsp == vfsp
	    && !(tvfsp->vfs_flag & VFS_UNMOUNTING))
	    return EBUSY;
	tvfsp = tvfsp->vfs_next;
    }
    code = xvfs_VfsGetVolume (vfsp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("xglue_unmount: %d\n", code));
      return(code);
    }

redo:
    if (!xvfs_NeedTokens (volp)) {
	if (!(code = vol_StartVnodeOp(volp, VFSOP_UNMOUNT, 0, &dmptr))) {
	    code = VFSX_UNMOUNT (vfsp, flag);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto done;
    }

    if (tkc_flushvfsp (vfsp)) {
	 code = EBUSY;
	 goto done;
     }

    if (!(code = vol_StartVnodeOp(volp, VFSOP_UNMOUNT, 0, &dmptr))) {
	code = VFSX_UNMOUNT (vfsp, flag);
	vol_EndVnodeOp(volp, &dmptr);
    }

  done:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume (volp);
    return (code);
}

int xglue_root (vfsp, vpp)
    struct vfs *vfsp;			/* virtual file system */
    struct vnode **vpp;			/* where to put vnode pointer */
{
    long code;				/* error return code */
    struct volume *volp;		/* volume registry entry */
    opaque dmptr = NULL;

    *vpp = NULL;	/* eliminate any stack garbage in caller */
    code = xvfs_VfsGetVolume (vfsp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("xglue_root: %d\n", code));
      return(code);
    }
    
redo:
    if (!xvfs_NeedTokens (volp)) {
	if (!(code = vol_StartVnodeOp(volp, VFSOP_ROOT, 0, &dmptr))) {
	    code = VFSX_ROOT (vfsp, vpp);
	    if (volp && !code) /* registered volume but not exported? */
		XVFS_CONVERT (*vpp);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto done;
    }

    if (!(code = vol_StartVnodeOp(volp, VFSOP_ROOT, 0, &dmptr))) {
	code = VFSX_ROOT (vfsp, vpp);
	vol_EndVnodeOp(volp, &dmptr);
    }
    if (!code)
	XVFS_CONVERT (*vpp);

  done:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume (volp);
    return (code);
}

int xglue_vget (vfsp, vpp, fidp)
    struct vfs *vfsp;			/* virtual file system */
    struct vnode **vpp;			/* where to put vnode pointer */
    struct fid *fidp;			/* file ID */
{
    long code;				/* error return code */
    struct volume *volp;		/* volume registry entry */
    opaque dmptr = NULL;

    *vpp = NULL;	/* eliminate any stack garbage in caller */
    code = xvfs_VfsGetVolume (vfsp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("xglue_vget: %d\n", code));
      return(code);
    }

redo:
    if (!xvfs_NeedTokens (volp)) {
	if (!(code = vol_StartVnodeOp(volp, VFSOP_VGET, 0, &dmptr))) {
	    code = VFSX_VGET (vfsp, vpp, fidp);
	    if (volp && !code) /* registered volume but not exported? */
		XVFS_CONVERT (*vpp);
	    vol_EndVnodeOp(volp, &dmptr);
	}
	goto done;
    }

    if (!(code = vol_StartVnodeOp(volp, VFSOP_VGET, 0, &dmptr))) {
	code = VFSX_VGET (vfsp, vpp, fidp);
	if (!code)
	    XVFS_CONVERT (*vpp);
	vol_EndVnodeOp(volp, &dmptr);
    }

  done:
    if (dmptr && volp) {
	if (VOL_DMWAIT(volp, &dmptr)) goto redo;
	VOL_DMFREE(volp, &dmptr);
    }
    xvfs_PutVolume (volp);
    return (code);
}

/* The ops vector itself. */
struct osi_vnodeops xglue_ops = {
    /* 
     * creation/naming/deletion 
     */
    xglue_link,
    xglue_mkdir,
    xglue_mknod,
    xglue_remove,
    xglue_rename,
    xglue_rmdir,
    /* 
     * lookup, file handle stuff 
     */
    xglue_lookup,
    xglue_fid,
    /* 
     * access to files 
     */
    xglue_open,
    xglue_create,
    xglue_hold,
    xglue_rele,
    xglue_close,
    xglue_map,
    xglue_unmap,
    /* 
     * manipulate attributes of files 
     */
    xglue_access,
    xglue_getattr,
    xglue_setattr,
    /* 
     * data update operations 
     */
    xglue_fclear,
    xglue_fsync,
    xglue_ftrunc,
    xglue_rdwr,
    xglue_lockctl,
    /* 
     * extensions 
     */
    xglue_ioctl,
    xglue_readlink,
    xglue_select,
    xglue_symlink,
    xglue_readdir,
    /* 
     * buffer ops 
     */
    xglue_strategy,
    /* 
     * security things
     */
    xglue_revoke,
    xglue_getacl,
    xglue_setacl
};
