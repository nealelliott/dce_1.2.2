/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: agfs_aixmount.c,v $
 * Revision 1.1.39.1  1996/10/02  16:57:34  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:35  damon]
 *
 * Revision 1.1.33.3  1994/07/13  22:27:01  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:16:41  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:52:53  mbs]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  17:26:12  mbs]
 * 
 * Revision 1.1.33.2  1994/06/09  13:49:53  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:22  annie]
 * 
 * Revision 1.1.33.1  1994/02/04  20:04:08  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:05:40  devsrc]
 * 
 * Revision 1.1.31.1  1993/12/07  17:11:13  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:41:06  jaffe]
 * 
 * Revision 1.1.2.4  1993/01/18  19:49:41  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:23:08  cjd]
 * 
 * Revision 1.1.2.3  1992/11/24  15:36:33  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:04:01  bolinger]
 * 
 * Revision 1.1.2.2  1992/11/18  17:37:14  jaffe
 * 	Transarc delta: jdp-ot5415-mount-aggregate-on-attach 1.4
 * 	  Selected comments:
 * 	    This delta contains the bulk of the changes necessary to have aggregates
 * 	    mounted at attach time.  The purpose of this feature is to provide a
 * 	    means by which exported filesets (with no local mount points) will be
 * 	    sync'd periodically.  To accomplish this, we have created a new VFS type
 * 	    (agfs).  Whenever an aggregate is exported, a local mount of the aggregate
 * 	    is also done; then, when the sync daemon does its thing, the aggregate's sync
 * 	    vfsop will be called.  The agfs sync then calls the new sync aggrop, which
 * 	    in the case of Episode, calls efs_sync().  The UFS sync aggrop doen't do
 * 	    anything, since we know the UFS filesystem is mounted, and so, its sync vfsop
 * 	    will get called anyway.
 * 	    This delta must be bundled with the corresponding deltas in the Episode and
 * 	    test configurations.
 * 	    This file contains the RIOS-specific implementations of agfs_Mount() and
 * 	    agfs_Unmount().
 * 	    Remove leftover junk from $OFILES.
 * 	    Rearrange things slightly to make use of a vendor-supplied definition
 * 	    of MOUNT_AGFS.
 * 	    Replace AFS_MOUNT_AGGREGATE with MOUNT_AGFS.
 * 	    Fix bogus linker warnings about shared libraries when compiling .o files.
 * 	[1992/11/17  19:30:19  jaffe]
 * 
 * $EndLog$
 */
/*
 * Copyright (C) 1992 Transarc Corporation.
 * All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi_device.h>
#include <dcedfs/xvfs_vnode.h>

#include <agfs_mount.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/agfs/RIOS/agfs_aixmount.c,v 1.1.39.1 1996/10/02 16:57:34 damon Exp $")


int
agfs_Mount(struct osi_vfs *vfsP)
{	
    agfs_mount_t*	mtEntryP;
    agfs_mount_data_t	mountData;
    void*		mountDataP;
    struct vnode*	bDevVP;
    struct vnode*	stubVP = vfsP->vfs_mntdover;
    struct vnode*	rootVP;
    dev_t		dev;
    char		devName[ASTABSIZE_SPEC+1];
    long		code;

    if (!agfs_inited)
	return EIO;

    mountDataP = vmt2dataptr(vfsP->vfs_mdata, VMT_INFO);
    bcopy(mountDataP, &mountData, sizeof mountData);
    (void)strncpy(devName, mountData.as_spec, ASTABSIZE_SPEC);
    devName[ASTABSIZE_SPEC] = '\0';

    if (code = lookupvp(devName, L_SEARCHLOC, &bDevVP))
        return code;

    if (osi_vType(bDevVP) != VBLK) {
        code = ENOTBLK;
        goto rele_dev;
    }

    code = (*bDevVP->v_gnode->gn_ops->vn_open)(bDevVP, FREAD, NULL, NULL);
    if (code)
        goto rele_dev;

    dev = brdev(bDevVP->v_rdev);
    code = agfs_GetMountTableEntry(&mountData, dev, 0,
		bDevVP, vfsP, &mtEntryP);
    if (code)
	goto close_dev;

    rootVP = &mtEntryP->am_rootvnode;

    vfsP->vfs_mntd = rootVP;
    vfsP->vfs_vnodes = rootVP;
    vfsP->vfs_data = (caddr_t)mtEntryP;
    vfsP->vfs_bsize = -1;
    vfsP->vfs_fsid.fsid_dev = dev;
    vfsP->vfs_fsid.fsid_type = MOUNT_AGFS;
    vfsP->vfs_flag |= (VFS_DEVMOUNT | VFS_READONLY);

    stubVP->v_mvfsp = vfsP;		/* Why isn't AIX doing this for me? */

    return 0;

close_dev:
    (*bDevVP->v_gnode->gn_ops->vn_close)(bDevVP, FREAD, NULL);

rele_dev:
    VN_RELE(bDevVP);
    return code;
}	/* agfs_Mount() */


/* ARGSUSED */
int
agfs_Unmount(struct osi_vfs *vfsP, int flags)
{
    int			code = 0;
    struct vnode*	devVP;
    agfs_mount_t*	mtEntryP;
    struct vnode*	rootVP = vfsP->vfs_mntd;
    struct aggr*	aggrp;

    mtEntryP = VFSTOAGFS(vfsP);
    devVP = mtEntryP->am_aggr.devvp;

    aggrp = ag_GetAggr(mtEntryP->am_aggr.a_aggrId);
    if (aggrp) {
	lock_ObtainRead(&aggrp->a_lock);
	if (aggrp->a_localMounts != 0) {
	    code = EBUSY;
	}
	lock_ReleaseRead(&aggrp->a_lock);
	ag_PutAggr(aggrp);
    } else {
	code = EINVAL;
    }
    if (code) {
	return (code);
    }

    if (code = agfs_PutMountTableEntry(mtEntryP))
	return code;
    
    vfsP->vfs_mntd = NULL;
    vfsP->vfs_vnodes = NULL;

    code = (*devVP->v_gnode->gn_ops->vn_close)(devVP, FREAD, NULL);
    if (code)
	return code;

    return 0;
}	/* agfs_UnmountSubr() */
