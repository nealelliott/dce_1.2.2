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
 * $Log: xvfs_osglue.c,v $
 * Revision 1.1.115.1  1996/10/02  19:03:28  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:55  damon]
 *
 * $EndLog$
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/volume.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/tkc.h>
#include <dcedfs/osi_user.h>
#include <dcedfs/osi_uio.h>
#include <ufs/inode.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/xvnode/HPUX/xvfs_osglue.c,v 1.1.115.1 1996/10/02 19:03:28 damon Exp $")

/*
 * Generic token glue layer that guarantees data consistency among multiple
 * physical file systems.
 */

/* Forward declarations */

int xglue_open(), xglue_close(), xglue_rdwr(), xglue_ioctl(), xglue_select();
int xglue_getattr(), xglue_setattr(), xglue_access(), xglue_lookup();
int xglue_create(), xglue_remove(), xglue_link(), xglue_rename();
int xglue_mkdir(), xglue_rmdir(), xglue_readdir(), xglue_symlink();
int xglue_readlink(), xglue_fsync(), xglue_inactive();
int xglue_bmap(), xglue_strategy(), xglue_bread(), xglue_brelse();
/* int xglue_pathsend(); */
int xglue_setacl(), xglue_getacl(), xglue_pathconf(), xglue_fpathconf();
int xglue_lockctl(), xglue_fid(), xglue_fsctl();
int xglue_pagein(), xglue_pageout();

extern int vfs_prefill();

/* vector of N-ops */

/* XXX Serious problems with bmap, strategy, bread, brelse.  Must figure out
 * XXX when and how to get and release tokens, and must figure out when and
 * XXX how to call StartBusyOp and EndBusyOp.
 */

/* XXX Need to fill all these in.  The ones that are currently 0 should
 * do something more graceful (even if only a panic) XXX */

struct osi_vnodeops xglue_ops = {
    xglue_open,
    xglue_close,
    xglue_rdwr,
    xglue_ioctl,
    xglue_select,
    xglue_getattr,
    xglue_setattr,
    xglue_access,
    xglue_lookup,
    xglue_create,
    xglue_remove,
    xglue_link,
    xglue_rename,
    xglue_mkdir,
    xglue_rmdir,
    xglue_readdir,
    xglue_symlink,
    xglue_readlink,
    xglue_fsync,
    xglue_inactive,
    xglue_bmap,
    xglue_strategy,
    xglue_bread,
    xglue_brelse,
    0,			/* pathsend */
    xglue_setacl,
    xglue_getacl,
    xglue_pathconf,
    xglue_fpathconf,
    xglue_lockctl,
    0,			/* lockf */
    xglue_fid,
    xglue_fsctl,
    vfs_prefill,
    xglue_pagein,
    xglue_pageout,
    0,			/* dbddup */
    0			/* dbddealloc */
};

int xglue_open(vpp, flags, credp)
    struct vnode **vpp;
    long flags;
    struct ucred *credp;
{
    struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    long hoFlags;
    struct vnode *vp = *vpp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_open (flags=0x%x) = ", flags));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_open: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_OPEN, 0))) {
	    code = VOPN_OPEN(vpp, flags, credp);
	    vol_EndVnodeOp(volp);
	}
	goto openerr;
    }
    if (flags & FWRITE) hoFlags = 1;
    else hoFlags = 0;
    if (!(code = tkc_HandleOpen(vp, hoFlags, &vcp))) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_OPEN, 0))) {
	    code = VOPN_OPEN(vpp, flags, credp);
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
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_open %d\n", code));
    return (code);
}


int xglue_close(vp, flags, credp)
  struct vnode *vp;
  int flags;
  struct ucred *credp;
  {
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;
    int hoFlags;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_close (flags=0x%x) = ", flags));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_close: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_CLOSE, 0))) {
	    code = VOPN_CLOSE(vp, flags, credp);
	    vol_EndVnodeOp(volp);
	}
	goto closerr;
    }
    /*
     * XXX Do we really need to get the OPEN tokens again; they've gotten
     * during the open system call.  Answer (kazar): No.
     */
    if (!(code = vol_StartVnodeOp(volp, VNOP_CLOSE, 0))) {
	code = VOPN_CLOSE(vp, flags, credp);
	vol_EndVnodeOp(volp);
    }

    if (flags & FWRITE) hoFlags = 1;
    else hoFlags = 0;
    if (vcp = tkc_HandleClose(vp, hoFlags)) {
	/* HandleClose actually does all of our hard work */
	tkc_Release(vcp);
    }

closerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_close %d\n", code));
    return (code);
}


int xglue_rdwr(vp, uiop, op, flags, credp)
  struct vnode *vp;
  struct uio *uiop;
  enum uio_rw op;
  int flags;
  struct ucred *credp;
{
    register struct tkc_vcache *vcp;
    long code, dataToken;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rdwr (op=%d, flags=0x%x) = ", op, flags));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rdwr: code = %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_RDWR, 0))) {
	    code = VOPN_RDWR(vp, uiop, op, flags, credp);
	    vol_EndVnodeOp(volp);
	}
	goto rdwrerr;
    }
    dataToken = (op == UIO_READ ? TKN_DATA_READ : TKN_DATA_WRITE);
    if (vcp = tkc_Get(vp, TKN_STATUS_WRITE|dataToken, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_RDWR, 0))) {
	    code = VOPN_RDWR(vp, uiop, op, flags, credp);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
rdwrerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_rdwr %d\n", code));
    return (code);
}


int xglue_ioctl(vp, cmd, arg, flags, credp)
  struct vnode *vp;
  int cmd, arg, flags;
  struct ucred *credp;
{
    register long code;

    /*
     * XXX ADD SOME TOKEN SUPPORT FOR THIS!! XXXX
     */
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_ioctl (cmd=0x%x, flags=%x, arg=%x) = ", 
	   cmd, flags, arg));
    code = VOPN_IOCTL(vp, cmd, arg, flags, credp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_ioctl %d\n", code));
    return (code);
}


int xglue_select(vp, wait, credp)
  struct vnode *vp;
  int wait;
  struct ucred *credp;
{
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_select\n"));
    return (VOPN_SELECT(vp, wait, credp));
}


int xglue_getattr(vp, vattrp, credp, sync)
    struct vnode *vp;
    struct vattr *vattrp;
    struct ucred *credp;
    opaque sync; /* Can ignore this and pass thru */
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_getattr (vattrp=0x%x) = ", vattrp));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_getattr: code = %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_GETATTR, 0))) {
	    code = VOPN_GETATTR(vp, vattrp, credp, sync);
	    vol_EndVnodeOp(volp);
	}
	goto getattrerr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_GETATTR, 0))) {
	    code = VOPN_GETATTR(vp, vattrp, credp, sync);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

getattrerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_getattr %d\n", code));
    return (code);
}


int xglue_setattr(vp, vattrp, credp, nonce)
  struct vnode *vp;
  struct vattr *vattrp;
  struct ucred *credp;
  opaque nonce; /* can ignore this */
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_setattr "));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_setattr: code = %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SETATTR, 0))) {
	    code = VOPN_SETATTR(vp, vattrp, credp, nonce);
	    vol_EndVnodeOp(volp);
	}
	goto setattrerr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SETATTR, 0))) {
	    code = VOPN_SETATTR(vp, vattrp, credp, nonce);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

setattrerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_setattr %d\n", code));
    return (code);
}


int xglue_access(vp, mode, credp)
  struct vnode *vp;
  long mode;
  struct ucred *credp;
  {
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_access (mode=0x%x) = ", mode));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_access code = %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_ACCESS, 0))) {
	    code = VOPN_ACCESS(vp, mode, credp);
	    vol_EndVnodeOp(volp);
	}
	goto accesserr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_ACCESS, 0))) {
	    code = VOPN_ACCESS(vp, mode, credp);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

accesserr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_access %d\n", code));
    return (code);
}


int xglue_lookup(dp, namep, vpp, credp, mvp)
  struct vnode *dp, *mvp;
  char *namep;
  struct vnode **vpp;
  struct ucred *credp;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_lookup (namep=%s) = ", namep));
    code = xvfs_GetVolume(dp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_lookup: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_LOOKUP, 0))) {
	    code = VOPN_LOOKUP(dp, namep, vpp, credp, mvp);
	    if (volp && !code) /* registered volume but not exported? */
		XVFS_CONVERT(*vpp);
	    vol_EndVnodeOp(volp);
	}
	goto lookerr;
    }
    if (vcp = tkc_Get(dp, TKN_DATA_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_LOOKUP, 0))) {
	    code = VOPN_LOOKUP(dp, namep, vpp, credp, mvp);
	    /* the v_fstype hack looks dearly bought, wish I remember how */
	    if ((!code) && ((*vpp)->v_fstype != VNFS_SPEC))
		XVFS_CONVERT(*vpp);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
lookerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_lookup %d\n", code));
    return (code);
}


int xglue_create(dp, namep, vattrp, flags, mode, vpp, credp)
    struct vnode *dp, **vpp;
    int flags, mode;
    char *namep;
    struct ucred *credp;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG,1,("In xglue_create (namep=%s, flags=0x%x,m ode=0x%x)=",
	   namep, flags, mode));
    code = xvfs_GetVolume(dp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_create: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_CREATE, 0))) {
	    code = VOPN_CREATE(dp, namep, vattrp, flags, mode, vpp, credp);
	    if (volp && !code) /* registered volume but not exported? */
		XVFS_CONVERT(*vpp);
	    vol_EndVnodeOp(volp);
	}
	goto createrr;
    }
    if (vcp = tkc_Get(dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_CREATE, 0))) {
	    code = VOPN_CREATE(dp, namep, vattrp, flags, mode, vpp, credp);
	    if ((!code) && ((*vpp)->v_fstype != VNFS_SPEC)) /* zzz */
		XVFS_CONVERT(*vpp);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

createrr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_create %d\n", code));
    return (code);
}


int xglue_remove(dp, namep, credp)
  struct vnode *dp;
  char *namep;
  struct ucred *credp;
{
    struct vnode *vp = 0;
    struct tkc_sets sets[2];
    register long code;
    struct volume *volp1 = 0, *volp2 = 0;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_remove "));
    if (code = VOPN_LOOKUP(dp, namep, &vp, credp, dp)) 
        return (code);
    code = xvfs_GetVolume(dp, &volp1);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_remove: %d\n", code));
      volp1 = (struct volume *)0;
      goto remerr;
    }
    code = xvfs_GetVolume(vp, &volp2);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_remove: %d\n", code));
      volp2 = (struct volume *)0;      
      goto remerr;
    }
    if (volp1 != volp2) {
	code = EXDEV;
	goto remerr;
    }

    if (!xvfs_NeedTokens(volp1)) {
	if (!(code = vol_StartVnodeOp(volp1, VNOP_REMOVE, 0))) {
	    code = VOPN_REMOVE(dp, namep, credp);
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
	code = VOPN_REMOVE(dp, namep, credp);
	vol_EndVnodeOp(volp1);
    }
    tkc_PutTokens(sets, 2);
    tkc_HandleDelete(vp, volp1);
    OSI_VN_RELE(vp);

remerr:
    if (vp)    OSI_VN_RELE(vp);
    if (volp1) xvfs_PutVolume(volp1);
    if (volp2) xvfs_PutVolume(volp2);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_remove %d\n", code));
    return (code);
}


int xglue_link(vp, dp, namep, credp)
  struct vnode *vp, *dp;
  char *namep;
  struct ucred *credp;
  {
    register long code;
    struct volume *volp1, *volp2;
    struct tkc_sets sets[2];

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
    if (!xvfs_NeedTokens(volp1)) {
	if (!(code = vol_StartVnodeOp(volp1, VNOP_LINK, 0))) {
	    code = VOPN_LINK(vp, dp, namep, credp);
	    vol_EndVnodeOp(volp1);
	}
	goto linkerr;
    }    
    tkc_Set(0, vp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
    tkc_Set(1, dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
    tkc_GetTokens(sets, 2);
    if (!(code = vol_StartVnodeOp(volp1, VNOP_LINK, 0))) {
        code = VOPN_LINK(vp, dp, namep, credp);
	vol_EndVnodeOp(volp1);
    }
    tkc_PutTokens(sets, 2);

linkerr:
    xvfs_PutVolume(volp1); xvfs_PutVolume(volp2);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_link%d\n", code));
    return (code);
}


int xglue_rename(dp, namep, tdp, tnamep, credp)
  struct vnode *dp, /* 'from' directory' */
              *tdp; /* 'to'   directory  */
  char *namep,*tnamep;
  struct ucred *credp;
{
    struct tkc_sets sets[4];
    struct vnode *vp = 0, /* 'from' vnode */
                 *tp = 0; /* 'to'   vnode */
    register long code, size;
    struct volume *volp1, *volp2;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rename (namep=%s, tnamep=%s) = ",
			    namep, tnamep));

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
    if (!xvfs_NeedTokens(volp1)) {
	if (!(code = vol_StartVnodeOp(volp1, VNOP_RENAME, 0))) {
	    code = VOPN_RENAME(dp, namep, tdp, tnamep, credp);
	    vol_EndVnodeOp(volp1);
	}
	goto renerr;
    }

    /* note that tkc_Set is a macro which references "sets"! */
    tkc_Set(0, dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
    size = 1;
    if (tdp != dp) {
	tkc_Set(size, tdp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
	size++;
    }

    /* look up 'from' vnode to do token sync. */
    if (code = VOPN_LOOKUP(dp, namep, &vp, credp, dp)) {
	vp = (struct vnode *)0;
	goto renerr;
    } 
    tkc_Set(size, vp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
    size++;

    /* look up 'to' vnode to do token sync. */
    code = VOPN_LOOKUP(tdp, tnamep, &tp, credp, tdp);
    if (code) {
        tp = (struct vnode *)0;
    } else {
	tkc_Set(size, tp, TKN_STATUS_WRITE, 0);
	size++;
    }

    /*
     * watch for invalid parms: duplicate vnode going into tkc_GetTokens
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

    tkc_GetTokens(sets, size);
    if (tp) OSI_VN_HOLD(tp);
    if (!(code = vol_StartVnodeOp(volp1, VNOP_RENAME, 0))) {
	code = VOPN_RENAME(dp, namep, tdp, tnamep, credp);
	vol_EndVnodeOp(volp1);
    }
    tkc_PutTokens(sets, size);
    /* if the rename over-wrote an existing object, delete that object */
    if (tp) {
	tkc_HandleDelete(tp, volp1);
	OSI_VN_RELE(tp);
    }

renerr:
    if (tp) OSI_VN_RELE(tp);
    if (vp) OSI_VN_RELE(vp);	 
    xvfs_PutVolume(volp1);
    xvfs_PutVolume(volp2);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_rename %d\n", code));
    return (code);
}


int xglue_mkdir(dp, namep, vattrp, vpp, credp)
  struct vnode *dp, **vpp;
  char *namep;
  struct vattr *vattrp;
  struct ucred *credp;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_mkdir (namep=%s) = ", namep));
    code = xvfs_GetVolume(dp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_mkdir: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_MKDIR, 0))) {
	    code = VOPN_MKDIR(dp, namep, vattrp, vpp, credp);
	    vol_EndVnodeOp(volp);
	}
	goto mkdirerr;
    }    
    if (vcp = tkc_Get(dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_MKDIR, 0))) {
	    code = VOPN_MKDIR(dp, namep, vattrp, vpp, credp);
	    vol_EndVnodeOp(volp);
	}
	if (!code)
	    XVFS_CONVERT (*vpp);
	tkc_Put(vcp);
    } else
	code = EINVAL;
mkdirerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_mkdir %d\n", code));
    return (code);
}


int xglue_rmdir(dp, namep, credp)
  struct vnode *dp;
  char *namep;
  struct ucred *credp;
{
    struct vnode *vp = 0;
    struct tkc_sets sets[2];
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rmdir (namep=%s) = ", namep));
    code = xvfs_GetVolume(dp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_rmdir: %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_RMDIR, 0))) {
	    code = VOPN_RMDIR(dp, namep, credp);
	    vol_EndVnodeOp(volp);
	}
	goto rmdirerr;
    }    

    if (code = vol_StartVnodeOp(volp, VNOP_RMDIR, 0)) {
	vp = (struct vnode *)0;
	goto rmdirerr;
    }
    code = VOPN_LOOKUP(dp, namep, &vp, credp, dp);
    /*
     * We can't call tkc_Set() below until we do this...
     */
    vol_EndVnodeOp(volp);
    if (code) {
        vp = (struct vnode *)0;
	goto rmdirerr;
    }
    XVFS_CONVERT(dp);
    if (dp == vp) {
	code = EINVAL;
	goto rmdirerr;
    }

    tkc_Set(0, dp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0);
    tkc_Set(1, vp, TKN_STATUS_WRITE, 0);
    tkc_GetTokens(sets, 2);
    OSI_VN_HOLD(vp);
    if (!(code = vol_StartVnodeOp(volp, VNOP_RMDIR, 0))) {
	code = VOPN_RMDIR(dp, namep, credp);
	vol_EndVnodeOp(volp);
    }
    tkc_PutTokens(sets, 2);
    tkc_HandleDelete(vp, volp);
    OSI_VN_RELE(vp);

rmdirerr:
    if (vp) OSI_VN_RELE(vp);	/* release the reference acquired by VOPN_LOOKUP */
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_rmdir %d\n", code));
    return (code);

}


int xglue_readdir(vp, uiop, credp)
  struct vnode *vp;
  struct uio *uiop;
  struct ucred *credp;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_readdir = "));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_readdir: code = %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_READDIR, 0))) {
	    code = VOPN_READDIR(vp, uiop, credp);
	    vol_EndVnodeOp(volp);
	}
	goto readdirerr;
    }
    if (vcp = tkc_Get(vp, TKN_DATA_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_READDIR, 0))) {
	    code = VOPN_READDIR(vp, uiop, credp);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

readdirerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_readdir %d\n", code));
    return (code);
}


int xglue_symlink(vp, namep, vattrp, targetp, credp)
  struct vnode *vp;
  struct vattr *vattrp;
  char *namep, *targetp;
  struct ucred *credp;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_symlink (namep=%s, target=%s) = ", 
	   namep, targetp));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_symlink: code = %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SYMLINK, 0))) {
	    code = VOPN_SYMLINK(vp, namep, vattrp, targetp, credp);
	    vol_EndVnodeOp(volp);
	}
	goto symlinkerr;
    }    
    if (vcp = tkc_Get(vp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SYMLINK, 0))) {
	    code = VOPN_SYMLINK(vp, namep, vattrp, targetp, credp);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

symlinkerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_symlink %d\n", code));
    return (code);
}


int xglue_readlink(vp, uiop, credp)
  struct vnode *vp;
  struct uio *uiop;
  struct ucred *credp;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_readlink = "));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_readlink: code = %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_READLINK, 0))) {
	    code = VOPN_READLINK(vp, uiop, credp);
	    vol_EndVnodeOp(volp);
	}
	goto readlinkerr;
    }
    if (vcp = tkc_Get(vp, TKN_DATA_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_READLINK, 0))) {
	    code = VOPN_READLINK(vp, uiop, credp);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;

readlinkerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_readlink%d\n", code));
    return (code);
}


int xglue_fsync(vp, credp, nonce)
  struct vnode *vp;
  struct ucred *credp;
  opaque nonce; /* more null_time nonsense */
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_fsync\n"));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_fsync: code = %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_FSYNC, 0))) {
	    code = VOPN_FSYNC(vp, credp, nonce);
	    vol_EndVnodeOp(volp);
	}
	goto fsyncerr;
    }
    if (vcp = tkc_Get(vp, TKN_DATA_WRITE|TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_FSYNC, 0))) {
	    code = VOPN_FSYNC(vp, credp, nonce);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
	code = EINVAL;
fsyncerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_fsync %d\n", code));
    return (code);
}


int xglue_inactive(vp,credp)
  struct vnode *vp;
  struct ucred *credp;
{
    long code;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_inactive=%x\n", vp));
    code = VOPN_INACTIVE(vp,credp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_inactive: %d\n", code));
    return(code);
}


int xglue_bmap(vp,bn,vpp,bnp)
  struct vnode *vp;
  long bn,*bnp;
  struct vnode **vpp;
{
    long code;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_bmap: (vp=0x%x)\n", vp));
    code = VOPN_BMAP(vp, bn, vpp, bnp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_bmap: %d\n", code));
    return (code);
}


int xglue_strategy(bp)
  struct buf *bp;
{
	long code;

	code = VOPN_STRATEGY(bp);
        return (code);
}

int xglue_bread(vp,bn,bpp)
  struct vnode *vp;
  long bn;
  struct buf **bpp;
{
    return (VOPN_BREAD(vp,bn,bpp));
}

int xglue_brelse(vp,bp)
  struct vnode *vp;
  struct buf *bp;
{
    return (VOPN_BRELSE(vp,bp));
}

#ifdef	NOTDEF
int xglue_pathsend(vp,pnp,follow,nlinkp,dirvpp,compvpp,opcode,dep)
  struct vnode *vp, **dirvpp, *compvpp;
  long follow, opcode, dep;
  opaque pnp, nlinkp; 
{
    return (VOPN_PATHSEND(&vp,pnp,follow,nlinkp,dirvpp,compvpp,opcode,dep));
}
#endif	/* NOTDEF */

int xglue_setacl(vp, nt, bp)
  struct vnode *vp;
  opaque nt; /* number of "tuples" XXX */
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_setacl \n"));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_setacl: code = %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SETACL, 0))) {
	    code = VOPN_SETACL(vp, nt, bp);
	    vol_EndVnodeOp(volp);
	}
	goto setaclerr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_WRITE, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_SETACL, 0))) {
	    code = VOPN_SETACL(vp, nt, bp);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
        code = EINVAL;

setaclerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_setacl (code %d)\n", code));
    return (code);
}


int xglue_getacl(vp, nt, bp)
  struct vnode *vp;
  opaque nt;   /* see above comment XXX */
  struct buf  *bp;
{
    register struct tkc_vcache *vcp;
    register long code;
    struct volume *volp;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_getacl \n"));
    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_getacl: code = %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_GETACL, 0))) {
	    code = VOPN_GETACL(vp, nt, bp);
	    vol_EndVnodeOp(volp);
	}
	goto getaclerr;
    }
    if (vcp = tkc_Get(vp, TKN_STATUS_READ, 0)) {
	if (!(code = vol_StartVnodeOp(volp, VNOP_GETACL, 0))) {
	    code = VOPN_GETACL(vp, nt, bp);
	    vol_EndVnodeOp(volp);
	}
	tkc_Put(vcp);
    } else
        code = EINVAL;

getaclerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_getacl (code %d)\n", code));
    return (code);
}


int xglue_pathconf(vp,nt, bp, cr)
  struct vnode *vp;
  struct buf *bp;
  opaque nt, cr; /* XXX */
{
      return (VOPN_PATHCONF(vp,nt,bp,cr));
}

int xglue_fpathconf(vp,nt, bp, cr)
  struct vnode *vp;
  struct buf *bp;
  opaque nt, cr; /* XXX */
{
      return (VOPN_PATHCONF(vp,nt,bp,cr));
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
int xglue_lockctl(vp, lckdat, cmd, cred, fp, LB, UB)
  struct vnode *vp;
  struct flock *lckdat;
  int cmd;
  struct ucred *cred;
  struct file *fp;
  off_t LB, UB;
{
    long code = 0, tokenType, startPos, endPos;
    register struct tkc_vcache *vcp;
    tkc_byteRange_t byteRange;
    struct volume *volp;
    afs_recordLock_t lockBlocker;
    int oldwhence = lckdat->l_whence;
    enum vsync vsync = VASYNC;

    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_lockctl "));

    code = xvfs_GetVolume(vp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_lockctl: code = %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens(volp)) {
	code = VOPN_LOCKCTL(vp, lckdat, cmd, cred, fp, LB, UB);
	goto lckerr;
    }

    switch (lckdat->l_whence) {
	case SEEK_SET: startPos = lckdat->l_start; break;
	case SEEK_CUR: startPos = lckdat->l_start + fp->f_offset; break;
	case SEEK_END:	
	{
	    register struct tkc_vcache *tvcp;
	    struct vattr va;
	    if (tvcp = tkc_Get(vp, TKN_STATUS_READ, 0)) {
		if (!(code = vol_StartVnodeOp(volp, VNOP_GETATTR, 0))) {
		    code = VOPN_GETATTR(vp, &va, cred, vsync);
		    vol_EndVnodeOp(volp);
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
	code = VOPN_LOCKCTL(vp, lckdat, cmd, cred, fp, LB, UB);
	if (code = tkc_PutLocks(vp, &byteRange)) {
	    code = EINVAL;	       
	    goto lckerr;
	}
    } else {
	tokenType = (lckdat->l_type == F_RDLCK ? TKN_LOCK_READ:TKN_LOCK_WRITE);
	code = tkc_GetLocks(vp, tokenType, &byteRange, (cmd==F_SETLKW ? 1 : 0),
			    &lockBlocker);
	if (code) {
	    /*
	     * If only F_GETLK, return the blocker's info
	     */
	    if (cmd == F_GETLK) {
	        code = 0;
		lckdat->l_whence = oldwhence;
		lckdat->l_type = lockBlocker.l_type;
		lckdat->l_start = AFS_hgetlo(lockBlocker.l_start_pos);
		lckdat->l_pid = lockBlocker.l_pid;
#ifdef	AIX
		lckdat->l_sysid = lockBlocker.l_sysid;
		lckdat->l_vfs	= AFS_MOUNT_AFS;
#endif	/* AIX */
		if (AFS_hgetlo(lockBlocker.l_end_pos) == 0x7fffffff)
		   lckdat->l_len = 0;
		else
		   lckdat->l_len = AFS_hgetlo(lockBlocker.l_end_pos) -
		       AFS_hgetlo(lockBlocker.l_start_pos) + 1;
	    }
	    goto lckerr;
	}
	code = VOPN_LOCKCTL(vp, lckdat, cmd, cred, fp, LB, UB);
	tkc_FinishGetLocks(vp);
    }

lckerr:
    xvfs_PutVolume(volp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_lockctl (code %d)\n", code));
    return (code);
}


xglue_lockf(vp, flag, len, cred, fp, LB, UB)
    struct vnode *vp;
    int flag;
    off_t len;
    struct ucred *cred;
    struct file *fp;
    off_t LB, UB;
{
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_lockf\n"));
    /* I refuse to implement lockf on the grounds it should be a 
     * generic VFS routine that calls VOP_LOCKCTL() -toml */
    return (EINVAL);
}

int xglue_fid(vp, fidpp)
    struct vnode *vp;
    struct fid **fidpp;
{
    int code; 

    /* 
     * XXX Any TOKENS here?? XXX
     */
    AFSLOG(XGLUE_DEBUG, 1, ("In xglue_fid, vp = %x", vp));
    code = VOPN_FID(vp, fidpp);
    AFSLOG(XGLUE_DEBUG, 1, ("END xglue_fid (code %d)\n", code));
    return (code);
}


int xglue_fsctl(vp,cmd,uiop,credp)
  struct vnode *vp;
  int cmd;
  struct uio *uiop;
  struct ucred *credp;
{
	panic("xglue_fsctl");
}

/* VFS Operation */
int xglue_unmount (vfsp, flag)
    struct vfs *vfsp;			/* virtual file system */
    int flag;
{
    long code;				/* error return code */
    struct volume *volp;		/* volume registry entry */
    struct vfs *tvfsp;			/* loop control */

    code = xvfs_VfsGetVolume(vfsp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("xglue_unmount: code = %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens (volp)) {
	if (!(code = vol_StartVnodeOp(volp, VFSOP_UNMOUNT, 0))) {
	    code = VFSX_UNMOUNT (vfsp);
	    vol_EndVnodeOp(volp);
	}
	goto done;
    }

    if (tkc_flushvfsp (vfsp)) {
	 code = EBUSY;
	 goto done;
    }

    if (!(code = vol_StartVnodeOp(volp, VFSOP_UNMOUNT, 0))) {
	code = VFSX_UNMOUNT (vfsp);
	vol_EndVnodeOp(volp);
    }

done:
    xvfs_PutVolume (volp);
    return (code);
}


/* VFS Operation */
int xglue_root (vfsp, vpp, nm)
    struct vfs *vfsp;			/* virtual file system */
    struct vnode **vpp;			/* where to put vnode pointer */
    char *nm;				/* component name for dux */
{
    long code;				/* error return code */
    struct volume *volp;		/* volume registry entry */

    *vpp = 0; 			/* clear any garbage of caller's */
    code = xvfs_VfsGetVolume(vfsp, &volp);
    if (code) {
      AFSLOG(XGLUE_DEBUG, 1, ("In xglue_root: code = %d\n", code));
      return(code);
    }
    if (!xvfs_NeedTokens (volp)) {
	if (!(code = vol_StartVnodeOp(volp, VFSOP_ROOT, 0))) {
	    code = VFSX_ROOT (vfsp, vpp, nm);
	    if (volp && !code) /* registered volume but not exported? */
		XVFS_CONVERT (*vpp);
	    vol_EndVnodeOp(volp);
	}
	goto done;
    }

    if (!(code = vol_StartVnodeOp(volp, VFSOP_ROOT, 0))) {
	code = VFSX_ROOT (vfsp, vpp, nm);
	vol_EndVnodeOp(volp);
    }
    if (!code)
	XVFS_CONVERT (*vpp);

done:
    xvfs_PutVolume (volp);
    return (code);
}


/* VFS Operation */
int xglue_vget (vfsp, vpp, fidp)
    struct vfs *vfsp;			/* virtual file system */
    struct vnode **vpp;			/* where to put vnode pointer */
    struct fid *fidp;			/* file ID */
{
    long code;				/* error return code */
    struct volume *volp;		/* volume registry entry */

    *vpp = NULL;	/* eliminate any stack garbage in caller */
    code = xvfs_VfsGetVolume (vfsp, &volp);
    if (!xvfs_NeedTokens(volp)) {
	if (!(code = vol_StartVnodeOp(volp, VFSOP_VGET, 0))) {
	    code = VFSX_VGET (vfsp, vpp, fidp);
	    if (volp && !code && *vpp) /* registered volume but not exported? */
		XVFS_CONVERT (*vpp);
	    vol_EndVnodeOp(volp);
	}
	goto done;
    }

    if (!(code = vol_StartVnodeOp(volp, VFSOP_VGET, 0))) {
	code = VFSX_VGET (vfsp, vpp, fidp);
	if (!code && *vpp)
	    XVFS_CONVERT (*vpp);
	vol_EndVnodeOp(volp);
    }

done:
    xvfs_PutVolume (volp);
    return (code);
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

int xglue_pagein(prp, wrt, space, vaddr, ret_startindex)
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

    return VOPN_PAGEIN(filevp, prp, wrt, space, vaddr, ret_startindex);
}

int xglue_pageout(prp, start, end, flags)
    preg_t *prp;
    size_t start, end;
    int flags;
{
    reg_t *rp = prp->p_reg;
    struct vnode *filevp = rp->r_bstore;

    return VOPN_PAGEOUT(filevp, prp, start, end, flags);
}

int (*xglue_vcache_delete_op)() = 0;
int (*xglue_cm_dcache_delete_op)() = 0;

/*
 * xglue_vn_rele - Remove a vnode from DFS caches.
 *
 * DFS contains caches that hold vnodes for performance
 * reasons.  One side effect of these caches is that a 
 * reference count is left held on the vnode.  Entries are 
 * typically recycled out of the caches via a lazy LRU mechanism, 
 * so even after being "unused", the vnodes may stay in the
 * cache.
 *
 * This function is called when the value of the vnode ref count
 * is sensitive to some OS operation.  It's role is to direct
 * the cleansing of caches for a particular vnode.
 *
 * The two caches that we are interested in now are the
 * CM dname cache and the tkc vcache.
 */
int xglue_vn_rele(vp)
register struct vnode *vp;
{
	int code1 = 0, code2 = 0;
	
	if (xglue_vcache_delete_op) {
		code1 = (*xglue_vcache_delete_op)(vp);
	}
	if (xglue_cm_dcache_delete_op) {
		code2 = (*xglue_cm_dcache_delete_op)(vp);
	}
	return(code1 ? code1 : code2);
}

/*
 * xglue_vcache_delete - Remove a vnode from the tkc vcache.
 *
 * The DFS tkc vcache's role is to increase performance by caching
 * tokens that have been allocated to vnodes.  We clean the tkc
 * cache by looking at the vnode, and if it is not held by anyone, 
 * it is removed from the vcache (and the subsequent vnode 
 * reference is decremented).
 */
int xglue_vcache_delete(vp)
register struct vnode *vp;
{
	register struct tkc_vcache *vcp;
	int code = 0;

	/*
	 * tkc_FindVcache will return a refernce on the vcache
	 * entry iff one exists.
	 */
	if (vcp = tkc_FindVcache(vp)) {
		/*
		 * Actually found in the cache.  The call
		 * to tkc_RecycleVcache will fail if more 
		 * than one reference count is held.
		 */
		if (tkc_RecycleVcache(vcp, 1)) {
			code = EBUSY;
		}
	}
	return(code);
}
