/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * Copyright (C) 1994, 1989 Transarc Corporation - All rights reserved 
 */

#include <dcedfs/param.h>		/* Should be always first */
#include <dcedfs/sysincludes.h>		/* Standard vendor system headers */
#include <dcedfs/xvfs_export.h>		/* VFS+ basic defs */
#include <dcedfs/aclint.h>
#include <dcedfs/tkc.h>
#include <cm.h>				/* Cm-based standard headers */

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/cm/RCS/cm_expressops.c,v 4.27 1994/12/09 16:47:28 cfe Exp $")

/* 
 * These are VOPX-style operations that obtain the appropriate tokens on the
 * local ("backing") file system and then perform the operation locally.
 *
 * Why do we obtain the tokens in this code?  Because we, including the cache
 * manager, are trying to synchronize access to the underlying file system's
 * data, not the cache manager's AFS vnodes, and the tokens used for that
 * synchronization are the ones based on the fileIDs of the local file system,
 * not the CM.
 *
 * Note, however, that these are not VOPO-style operations because the
 * functions are generic, being called in a one-to-one fashion from the
 * corresponding cm VOPX funcs.
 */

cmglue_open(avp, aflags, acredp)
    struct vnode **avp;
    long aflags;
    osi_cred_t *acredp; 
{
    struct tkc_vcache *vcp;
    long code;

    if (vcp = tkc_Get(*avp, (aflags == FREAD? TKN_OPEN_READ : TKN_OPEN_WRITE), 0)) {
	code = VOPX_OPEN(avp, aflags, acredp);
	tkc_Put(vcp);
    } else 
	code = EINVAL;
    return code;
}

cmglue_close(avp, aflags, acredp)
    struct vnode *avp;
    long aflags;
    osi_cred_t *acredp; 
{
    register struct tkc_vcache *tvcp;
    register long code;

    tvcp = tkc_FindVcache(avp);
    /* XXXX need to do something here to tell the system the file is no
     * longer open, but that code doesn't exist yet.
     */
    tkc_Put(tvcp);
    code = VOPX_CLOSE(avp, aflags, acredp);
    return code;
}


cmglue_rdwr(avp, uiop, arw, aflags, acredp)
    register struct vnode *avp;
    struct uio *uiop;	/* uio structure */
    enum uio_rw arw;	/* r v.s. write */
    int aflags;		/* I/O sync, etc */
    osi_cred_t *acredp;
{
    register long code, flags;
    register struct tkc_vcache *tvcp;

    if (arw == UIO_READ) 
	flags = TKN_READ;
    else 
	flags = TKN_UPDATE;
    if (tvcp = tkc_Get(avp, flags, 0)) {
	code = VOPX_RDWR(avp, uiop, arw, aflags, acredp);
	tkc_Put(tvcp);
    } else 
	code = EINVAL;
    return code;
}


/* 
 * Called after the data has been pinned, and thus doesn't have to obtain
 * tokens itself.
 */
cmglue_nlrw(avp, uiop, arw, aflags, acredp)
    register struct vnode *avp;
    struct uio *uiop;	/* uio structure */
    enum uio_rw arw;	/* r v.s. write */
    int aflags;		/* I/O sync, etc */
    osi_cred_t *acredp;
{
    register long code;

    gluevn_SetThreadFlags(1);		/* tell get/put page to skip tokens */
    code = VOPX_RDWR(avp, uiop, arw, aflags, acredp);
    gluevn_Done();
    return code;
}

cmglue_fsync(vp, credp)
    struct vnode *vp;
    osi_cred_t *credp; 
{
    return VOPX_FSYNC(vp, credp);
}

cmglue_ioctl(avp, acmd, aparms)
    struct vnode *avp;
    int acmd, aparms; 
{
    long code;

    code = VOPX_IOCTL(avp, acmd, aparms);
    return code;
}


cmglue_select(avp, aw, acredp)
    struct vnode *avp;
    long aw;
    osi_cred_t *acredp; 
{
    return VOPX_SELECT(avp, aw, acredp);
}


cmglue_getattr(avp, attrsp, aflag, acredp)
    struct vnode *avp;
    struct vattr *attrsp;
    int aflag;
    osi_cred_t *acredp;
{
    register struct tkc_vcache *tvcp;
    register long code;

    if (tvcp = tkc_Get(avp, TKN_STATUS_READ, 0)) {
	code = VOPX_GETATTR(avp, attrsp, aflag, acredp);
	tkc_Put(tvcp);
    } else 
	code = EINVAL;
    return code;
}


cmglue_setattr(avp, attrsp, aflag, acredp)
    struct vnode *avp;
    struct vattr *attrsp;
    int aflag;
    osi_cred_t *acredp;
{
    register struct tkc_vcache *tvcp;
    register long code;

    if (tvcp = tkc_Get(avp, TKN_STATUS_READ | TKN_STATUS_WRITE, 0)) {
	gluevn_SetThreadFlags(1);
	code = VOPX_SETATTR(avp, attrsp, aflag, acredp);
	gluevn_Done();
#ifdef AFS_SUNOS5_ENV
	VOPN_PUTPAGE(avp, 0, 0, B_INVAL, acredp);
#endif /* AFS_SUNOS5_ENV */
	tkc_Put(tvcp);
    } else code = EINVAL;
    return code;
}


cmglue_access(avp, amode, acredp)
    struct vnode *avp;
    long amode;
    osi_cred_t *acredp; 
{
    register struct tkc_vcache *tvcp;
    register long code;

    if (tvcp = tkc_Get(avp, TKN_STATUS_READ, 0)) {
	code = VOPX_ACCESS(avp, amode, acredp);
	tkc_Put(tvcp);
    } else 
	code = EINVAL;
    return code;
}


cmglue_lookup(avp, aname, avpp, acredp)
    struct vnode *avp;
    char *aname;
    struct vnode **avpp;
    osi_cred_t *acredp;
{
    register struct tkc_vcache *tvcp;
    register long code;

    if (tvcp = tkc_Get(avp, TKN_READ, 0)) {
	code = VOPX_LOOKUP(avp, aname, avpp, acredp);
	tkc_Put(tvcp);
	if (code == 0) 
	    xvfs_convert(*avpp);	/* in case px doesn't do it */
    } else 
	code = EINVAL;
    return code;
}


cmglue_create(dscp, namep, attrsp, aexcl, mode, scpp, credp)
    register struct vnode *dscp;
    char *namep;
    struct vattr *attrsp;
    enum vcexcl aexcl;
    int mode;
    struct vnode **scpp;
    osi_cred_t *credp; 
{
    register struct tkc_vcache *tvcp;
    register long code;

    if (tvcp = tkc_Get(dscp, TKN_UPDATE, 0)) {
	code = VOPX_CREATE(dscp, namep, attrsp, aexcl, mode, scpp, credp);
	tkc_Put(tvcp);
	if (code == 0) 
	    xvfs_convert(*scpp);
    } else 
	code = EINVAL;
    return code;
}


cmglue_remove(dscp, namep, credp)
    register struct vnode *dscp;
    char *namep;
    osi_cred_t *credp; 
{
    register struct tkc_vcache *tvcp;
    register long code;

    if (tvcp = tkc_Get(dscp, TKN_UPDATE, 0)) {
	code = VOPX_REMOVE(dscp, namep, credp);
	tkc_Put(tvcp);
    } else 
	code = EINVAL;
    return code;
}


cmglue_link(scp, dscp, namep, credp)
    register struct vnode *scp;
    register struct vnode *dscp;
    char *namep;
    osi_cred_t *credp; 
{
    register long code;
    struct tkc_sets sets[2];

    tkc_Set(0, dscp, TKN_UPDATE, 0);
    tkc_Set(1, scp, TKN_UPDATE, 0);
    while (tkc_GetTokens(sets, 2));
    code = VOPX_LINK(scp, dscp, namep, credp);
    tkc_PutTokens(sets, 2);
    return code;
}


cmglue_rename(odscp, OldNamep, ndscp, NewNamep, credp)
    register struct vnode *odscp, *ndscp;
    char *OldNamep, *NewNamep;
    osi_cred_t *credp; 
{
    struct tkc_sets sets[2];
    register struct tkc_vcache *tvcp;
    register long code;
    struct vnode *vp1, *vp2;

    /* let's try it file server-style (dirs first) */
    tkc_Set(0, odscp, TKN_UPDATE, 0);
    tkc_Set(1, ndscp, TKN_UPDATE, 0);
    while (tkc_GetTokens(sets, 2));
    if (code = VOPX_LOOKUP(odscp, OldNamep, &vp1, credp)) {
	tkc_PutTokens(sets, 2);
	return code;
    }
    if (code = VOPX_LOOKUP(ndscp, NewNamep, &vp2, credp))
	vp2 = (struct vnode *) 0;
    /* ensure that we really have an extended vnode */
    xvfs_ConvertDev(&vp1);
    if (vp2) 
	xvfs_ConvertDev(&vp2);
    code = VOPX_RENAME(odscp, OldNamep, ndscp, NewNamep, credp);
    /* 
     * we can release the dir read tokens now, since we've done the rename,
     * and thus the file ID's can't change (and thus we know which files
     * the rename actually affected).
     */
    tkc_PutTokens(sets, 2);
    if (tvcp = tkc_Get(vp1, TKN_UPDATE, 0))
	tkc_Put(tvcp);
    if (vp2) {
	if (tvcp = tkc_Get(vp2, TKN_STATUS_READ | TKN_STATUS_WRITE, 0))
	    tkc_Put(tvcp);
    }
    return code;
}


cmglue_mkdir(dscp, namep, attrsp, scpp, credp)
    register struct vnode *dscp;
    char *namep;
    struct vattr *attrsp;
    register struct vnode **scpp;
    osi_cred_t *credp; 
{
    register struct tkc_vcache *tvcp;
    register long code;

    if (tvcp = tkc_Get(dscp, TKN_UPDATE, 0)) {
	code = VOPX_MKDIR(dscp, namep, attrsp, scpp, credp);
	if (code == 0) 
	    xvfs_convert(*scpp);
	tkc_Put(tvcp);
    } else 
	code = EINVAL;
    return code;
}


cmglue_rmdir(dscp, namep, cdirp, credp)
  register struct vnode *dscp;
  char *namep;
  struct vnode *cdirp;
  osi_cred_t *credp; 
{
    register struct tkc_vcache *tvcp;
    register long code;
    struct vnode *vp1;

    if (tvcp = tkc_Get(dscp, TKN_UPDATE, 0)) {
	if (code = VOPX_LOOKUP(dscp, namep, &vp1, credp)) {
	    tkc_Put(tvcp);
	    return code;
	}
	xvfs_ConvertDev(&vp1);
	code = VOPX_RMDIR(dscp, namep, cdirp, credp);
	tkc_Put(tvcp);
	/* temporarily grab update token since we changed the link count */
	if (tvcp = tkc_Get(vp1, TKN_STATUS_READ | TKN_STATUS_WRITE, 0))
	    tkc_Put(tvcp);
    } else 
	code = EINVAL;
    return code;
}


cmglue_readdir(scp, uiop, credp, endp)
  register struct vnode *scp;
  struct uio *uiop;
  osi_cred_t *credp;
  int *endp;
{
    register struct tkc_vcache *tvcp;
    register long code;

    if (tvcp = tkc_Get(scp, TKN_READ, 0)) {
	code = VOPX_READDIR(scp, uiop, credp, endp, 0);
	tkc_Put(tvcp);
    } else 
	code = EINVAL;
    return code;
}


cmglue_symlink(dscp, namep, attrsp, targetNamep, credp)
    register struct vnode *dscp;
    char *namep;
    struct vattr *attrsp;
    register char *targetNamep;
    osi_cred_t *credp; 
{
    register struct tkc_vcache *tvcp;
    register long code;

    if (tvcp = tkc_Get(dscp, TKN_UPDATE, 0)) {
	code = VOPX_SYMLINK(dscp, namep, attrsp, targetNamep, credp);
	tkc_Put(tvcp);
    } else 
	code = EINVAL;
    return code;
}


cmglue_readlink(scp, uiop, credp)
    register struct vnode *scp;
    struct uio *uiop;
    osi_cred_t *credp; 
{
    register struct tkc_vcache *tvcp;
    register long code;

    if (tvcp = tkc_Get(scp, TKN_READ, 0)) {
	code = VOPX_READLINK(scp, uiop, credp);
	tkc_Put(tvcp);
    } else 
	code = EINVAL;
    return code;
}


#ifdef notdef
/* This code is not referenced anywhere */
cmglue_lockctl(scp, ldp, cmd, c, id)
    struct vnode *scp;
    struct flock *ldp;
    int cmd;
    osi_cred_t *c;
    int id;
{
    register struct tkc_vcache *tvcp;
    register long code;
    long flags;

    /* XXXX this is all wrong, waiting for basic glue to get it right */
    return VOPX_LOCKCTL(scp, ldp, cmd, c, id);
}
#endif /* notdef */

/* 
 * The next two calls assume that token management was already done by
 * a parent vnode operation (usually VOPN_GETPAGE, via VOPX_PIN).
 */
cmglue_getlength(vp, lengthp, credp)
    struct vnode *vp;
    long *lengthp;
    osi_cred_t *credp;
{
    register long code;

    code = VOPX_GETLENGTH(vp, lengthp, credp);
    return code;
}


cmglue_getacl(vp, aclp, w, credp)
    struct vnode *vp;
    struct afsACL *aclp;
    long w;
    osi_cred_t *credp;
{
    register struct tkc_vcache *tvcp;
    register long code;

    if (tvcp = tkc_Get(vp, TKN_STATUS_READ, 0)) {
	code = VOPX_GETACL(vp, aclp, w, credp);
	tkc_Put(tvcp);
    } else 
	code = EINVAL;
    return code;
}


cmglue_setacl(vp, aclp, svp, dw, sw, credp)
    struct vnode *vp;
    struct afsACL *aclp;
    struct vnode *svp;
    long dw;
    long sw;
    osi_cred_t *credp;
{
    register struct tkc_vcache *tvcp;
    register long code;

    if (tvcp = tkc_Get(vp, TKN_STATUS_READ | TKN_STATUS_WRITE, 0)) {
	code = VOPX_SETACL(vp, aclp, svp, dw, sw, credp);
	tkc_Put(tvcp);
    } else 
	code = EINVAL;
    return code;
}
