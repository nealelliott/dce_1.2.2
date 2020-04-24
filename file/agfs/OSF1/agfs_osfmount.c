/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: agfs_osfmount.c,v $
 * Revision 1.1.48.1  1996/10/02  16:57:31  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:31  damon]
 *
 * Revision 1.1.42.3  1994/07/13  22:27:00  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:16:40  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:52:51  mbs]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  17:26:10  mbs]
 * 
 * Revision 1.1.42.2  1994/06/09  13:49:51  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:19  annie]
 * 
 * Revision 1.1.42.1  1994/02/04  20:04:06  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:05:39  devsrc]
 * 
 * Revision 1.1.40.1  1993/12/07  17:11:11  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:40:52  jaffe]
 * 
 * Revision 1.1.2.6  1993/01/18  19:49:34  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:22:54  cjd]
 * 
 * Revision 1.1.2.5  1993/01/13  15:22:41  shl
 * 	Transarc delta: jaffe-cleanup-2.3-compiler-warnings 1.8
 * 	  Selected comments:
 * 	    Fix compiler warnings found when moving to OSF/1 1.1.1.
 * 	    define IMPORT decl of agfs_Statfs correctly.
 * 	    Remove the function dacl_SetOwnerControl.  It is not used.
 * 	[1993/01/12  17:21:35  shl]
 * 
 * 	Add missing declarations to mount/unmount code specific to OSF/1 1.1.1.
 * 	Also use osi_Alloc() and osi_Free() in this code.
 * 	[1992/12/03  16:53:24  rsarbo]
 * 
 * Revision 1.1.2.4  1992/12/10  21:26:20  bolinger
 * 	As part of DFS upgrade to OSF/1 1.1.1, enable 1.1.1-dependent
 * 	code and make it use osi_Alloc()/osi_Free() interface.  [OT
 * 	defect 5236.]
 * 	[1992/12/10  21:19:59  bolinger]
 * 
 * Revision 1.1.2.3  1992/11/24  15:36:30  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:03:56  bolinger]
 * 
 * Revision 1.1.2.2  1992/11/18  17:37:00  jaffe
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
 * 	    This file contains the OSF1-specific implementations of agfs_Mount() and
 * 	    agfs_Unmount().
 * 	    Remove leftover junk from $OFILES.
 * 	    Rearrange things slightly to make use of a vendor-supplied definition
 * 	    of MOUNT_AGFS.
 * 	    Replace AFS_MOUNT_AGGREGATE with MOUNT_AGFS.
 * 	    Fix bogus linker warnings about shared libraries when compiling .o files.
 * 	[1992/11/17  19:29:06  jaffe]
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
#include <dcedfs/osi_uio.h>

#include <agfs_mount.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/agfs/OSF1/agfs_osfmount.c,v 1.1.48.1 1996/10/02 16:57:31 damon Exp $")

extern int agfs_Statfs(struct osi_vfs *vfsP);


int
agfs_Mount(vfsP, pathname, privateData, ndp)
    struct osi_vfs*	vfsP;
    char*		pathname;
    caddr_t		privateData;
    struct nameidata*	ndp;
{
    agfs_mount_t*	mtEntryP;
    agfs_mount_data_t	mountData;
    struct vnode*	bDevVP;
    dev_t		dev;
    char		devName[ASTABSIZE_SPEC+1];
    long		code;
    struct vnode*	cVP = ndp->ni_vp;
    int			ignore_error;
    struct statfs*	statP = &vfsP->m_stat;
    unsigned		size;
#ifdef AFS_OSF11_ENV
    char*		tempstring;
#endif /* AFS_OSF11_ENV */

#if SEC_BASE
    /*Must have the mount privilege. */
    if (!privileged(SEC_MOUNT, EPERM))
	return EPERM;
#else /* SEC_BASE */
    /* Must be super user */
    if (!osi_suser(osi_getucred()))
	return EPERM;
#endif /* SEC_BASE */

    BM(MOUNT_LOCK(vfsP));

    if (vfsP->m_flag & M_UPDATE) {
	struct ufs_args	mountdArgs;

	/*
	 * The mountd must be doing a mount for update.
	 */
	code = copyin(privateData, &mountdArgs, sizeof mountdArgs);
	if (code == 0 && 
	    ((mountdArgs.exflags & M_EXPORTED) ||
	     (vfsP->m_flag & M_EXPORTED))) {
	    if (mountdArgs.exflags & M_EXPORTED)
		vfsP->m_flag |= M_EXPORTED;
	    else
		vfsP->m_flag &= ~M_EXPORTED;

	    if (mountdArgs.exflags & M_EXRDONLY)
		vfsP->m_flag |= M_EXRDONLY;
	    else
		vfsP->m_flag &= ~M_EXRDONLY;

	    vfsP->m_exroot = mountdArgs.exroot;
	}

	BM(MOUNT_UNLOCK(vfsP));
	return code;
    }

    if (cVP->v_usecount != 1 ) {
	code = EBUSY;
	goto unlock_vfs;
    }

    if (cVP->v_type != VDIR || vfsP->m_vnodecovered->v_type != VDIR) {
	code = ENOTDIR;
	goto unlock_vfs;
    }

    if (!agfs_inited) {
	code = EIO;
	goto unlock_vfs;
    }

    if (code = copyin(privateData, &mountData, sizeof mountData))
	goto unlock_vfs;

    (void)strncpy(devName, mountData.as_spec, ASTABSIZE_SPEC);
    devName[ASTABSIZE_SPEC] = '\0';

    if (code = osi_getmdev(devName, &dev))
	goto unlock_vfs;

    code = osi_lookupname(devName, OSI_UIOSYS, FOLLOW_LINK,
			  (struct vnode*)0, &bDevVP);
    if (code)
        goto unlock_vfs;

    if (osi_vType(bDevVP) != VBLK) {
        code = ENOTBLK;
        goto rele_dev;
    }

    VOP_OPEN(&bDevVP, FREAD, ndp->ni_cred, code);
    if (code)
	goto rele_dev;

    code = agfs_GetMountTableEntry(&mountData, dev, 0, 
		bDevVP, vfsP, &mtEntryP);
    if (code)
	goto close_dev;

    vfsP->m_data = (qaddr_t)mtEntryP;
    vfsP->m_flag |= M_SWAP_NEVER;
    statP->f_fsid.val[0] = dev;
    statP->f_fsid.val[1] = MOUNT_AGFS;

#ifndef AFS_OSF11_ENV
    size = sizeof statP->f_mntfromname - 1;
    (void)strncpy(statP->f_mntfromname, devName, size);
    statP->f_mntfromname[size] = '\0';

    (void)copyinstr(pathname, statP->f_mntonname,
		    sizeof statP->f_mntonname - 1, &size);
    statP->f_mntonname[size] = '\0';
#else
    size = strlen(devName) + 1;
    statP->f_mntfromname = (char *)osi_Alloc(size);
    if (!statP->f_mntfromname) {
	code = ENOMEM;
	goto close_dev;
    }
    (void)strcpy(statP->f_mntfromname, devName);

    tempstring = (char *)osi_Alloc(MAXPATHLEN+1);
    if (!tempstring) {
	osi_Free(statP->f_mntfromname, size);
	code = ENOMEM;
	goto close_dev;
    }
    (void)copyinstr(pathname, tempstring, MAXPATHLEN, &size);
    tempstring[size] = '\0';

    statP->f_mntonname = (char *)osi_Alloc(size + 1);
    if (!statP->f_mntonname) {
	osi_Free(statP->f_mntfromname, size);
	osi_Free(tempstring, MAXPATHLEN+1);
	code = ENOMEM;
	goto close_dev;
    }
    (void)strcpy(statP->f_mntonname, tempstring);
    osi_Free(tempstring, MAXPATHLEN+1);
#endif /* AFS_OSF11_ENV */

    if (code = agfs_Statfs(vfsP))
	goto close_dev;

    return 0;

close_dev:
    VOP_CLOSE(bDevVP, FREAD, ndp->ni_cred, ignore_error);

rele_dev:
    VN_RELE(bDevVP);

unlock_vfs:
    BM(MOUNT_UNLOCK(vfsP));
    return code;
}	/* agfs_Mount() */


int
agfs_Unmount(struct osi_vfs *vfsP)
{
    int			code;
    struct vnode*	devVP;
    agfs_mount_t*	mtEntryP;
    int			ignore_error;
#ifdef AFS_OSF11_ENV
    struct statfs*	statP = &vfsP->m_stat;
    int			fromnlen = strlen(statP->f_mntfromname) + 1;
    int			tonlen = strlen(statP->f_mntonname) + 1;
#endif

#if SEC_BASE
    /*Must have the mount privilege. */
    if (!privileged(SEC_MOUNT, EPERM))
	return (EPERM);
#else /* SEC_BASE */
    /* Must be super user */
    if (!osi_suser(osi_getucred()))
	return (EPERM);
#endif /* SEC_BASE */

    mtEntryP = VFSTOAGFS(vfsP);
    devVP = mtEntryP->am_aggr.devvp;

    if (code = agfs_PutMountTableEntry(mtEntryP))
	return code;
    
#ifdef AFS_OSF11_ENV
    osi_Free(statP->f_mntfromname, fromnlen);
    osi_Free(statP->f_mntonname, tonlen);
#endif	/* AFS_OSF11_ENV */

    VOP_CLOSE (devVP, FREAD, u.u_cred, ignore_error);
    VN_RELE(devVP);

    return 0;
}	/* agfs_UnmountSubr() */


