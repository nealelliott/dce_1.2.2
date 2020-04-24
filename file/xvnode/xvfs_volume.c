/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xvfs_volume.c,v $
 * Revision 1.1.740.1  1996/10/02  19:04:17  damon
 * 	New DFS from Transarc
 * 	[1996/10/01  18:59:49  damon]
 *
 * $EndLog$
*/
/* Copyright (C) 1990, 1994 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/basicincludes.h>
#include <dcedfs/osi.h>
#include <dcedfs/lock.h>
#include <dcedfs/common_data.h>
#include <dcedfs/volume.h>
#include <dcedfs/xvfs_vnode.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/xvnode/xvfs_volume.c,v 1.1.740.1 1996/10/02 19:04:17 damon Exp $")

#ifdef notdef /* following code probably should be removed */

int lv_hold(), lv_rele(), lv_lock(), lv_unlock(), lv_inval();

static struct volumeops localVolOps = {
    lv_hold,
    lv_rele,
    lv_inval,	/* lock */
    lv_inval,	/* unlock */
    lv_inval,	/* open */
    lv_inval,	/* seek */
    lv_inval,	/* tell */
    lv_inval,	/* scan */
    lv_inval,	/* close */
    lv_inval,	/* destroy */
    lv_inval,	/* attach */
    lv_inval,	/* detach */
    lv_inval,	/* getstatus */
    lv_inval,	/* setstatus */

    lv_inval,	/* create */
    lv_inval,	/* read */
    lv_inval,	/* write */
    lv_inval,	/* truncate */
    lv_inval,	/* delete */
    lv_inval,	/* getattr */
    lv_inval,	/* setattr */
    lv_inval,	/* getacl */
    lv_inval,	/* setacl */
    lv_inval,	/* clone */
    lv_inval,	/* reclone */
    lv_inval,	/* unclone */

    lv_inval,	/* vget */
    lv_inval,	/* root */
    lv_inval,	/* isroot */
    lv_inval,	/* getvv */
    lv_inval,	/* setdystat */
    lv_inval,	/* freedystat */
};

static lv_hold(volp)
    register struct volume *volp; 
{
    volp->v_refCount++;
    return 0;
}


static lv_rele(volp)
    register struct volume *volp; 
{
    volp->v_refCount--;
    return 0;
}
#endif

long xvfs_GetVolume(struct vnode *vp, struct volume **volpp)
{
    long code;

    /* if it isn't converted, this is probably being called as
     * part of a cross-device operation (we got into the glue
     * by virtue of another vnode's ops, and this is another
     * parm).  The check to see if both volumes are the same
     * done by the glue will bounce the request.  If we call
     * VOPX_<anything>, however, we'll panic, so bail out early.
     */
    if (!IS_CONVERTED(vp)) {
	*volpp = NULL;
	return 0;
    }

     /* 
      * cm_getvolume always returns 0 and a null volp.
      * xufs_getvolume (vol_FindVfs) will return 0. If it finds volp, it will 
      *        return it else it will return a null volp.
      * efs_getvolume return either 0 and a volp, or ESTALE and a null volp
      */
    code = VOPX_GETVOLUME(vp, volpp);
    if (code == ENODEV) {
	code = 0;
	*volpp = NULL;
    }
    return code;
}


long xvfs_VfsGetVolume(struct osi_vfs *vfsp, struct volume **volpp)
{
    long code;

    code = VFSX_VFSGETVOLUME (vfsp, volpp);
    if (code == ENODEV) {
	code = 0;
	*volpp = NULL;
    }
    return code;
}


/*
 * We only need tokens if the volume is exported and not read-only.
 * If volp is null, then a previous call to xvfs_GetVolume() failed, indicating
 * that the volume is not exported.
 */
/* #define	xvfs_NeedTokens(volp)	((volp) ? VOL_READWRITE(volp) : 0) */
int xvfs_NeedTokens(struct volume *volp)
{
    int result;

    if (volp == 0) return 0;
    lock_ObtainRead(&volp->v_lock);
    result = ((volp->v_states & (VOL_RW | VOL_NOEXPORT)) == VOL_RW);
    lock_ReleaseRead(&volp->v_lock);
    return result;
}
