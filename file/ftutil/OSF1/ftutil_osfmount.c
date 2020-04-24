/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftutil_osfmount.c,v $
 * Revision 1.1.43.1  1996/10/02  17:48:12  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:15  damon]
 *
 * Revision 1.1.38.3  1994/07/13  22:28:04  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:35:17  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:06  mbs]
 * 
 * Revision 1.1.38.2  1994/06/09  14:08:56  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:23:37  annie]
 * 
 * Revision 1.1.38.1  1994/02/04  20:19:22  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:26  devsrc]
 * 
 * Revision 1.1.36.1  1993/12/07  17:25:18  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:27:59  jaffe]
 * 
 * Revision 1.1.2.5  1993/01/21  19:37:47  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  19:51:49  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/10  21:28:04  bolinger
 * 	As part of DFS upgrade to OSF/1 1.1.1, enable 1.1.1-dependent code
 * 	and give umount() syscall right number of arguments.  [OT defect
 * 	5236]
 * 	[1992/12/10  21:21:22  bolinger]
 * 
 * Revision 1.1.2.3  1992/11/24  17:50:10  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:03:39  bolinger]
 * 
 * Revision 1.1.2.2  1992/11/18  20:34:22  jaffe
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
 * 	    This file contains the OSF1-specific implementations of _ftu_Mount() and
 * 	    _ftu_Unmount().
 * 	    Remove leftover junk from $OFILES.
 * 	    Rearrange things slightly to make use of a vendor-supplied definition
 * 	    of MOUNT_AGFS.
 * 	    Replace AFS_MOUNT_AGGREGATE with MOUNT_AGFS.
 * 	    Fix bogus linker warnings about shared libraries when compiling .o files.
 * 	[1992/11/17  21:49:46  jaffe]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1990, 1991, 1992 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <dcedfs/stds.h>
#include <dcedfs/agfs_mount.h>

#include <ftutil.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ftutil/OSF1/ftutil_osfmount.c,v 1.1.43.1 1996/10/02 17:48:12 damon Exp $")

SHARED long
_ftu_Mount(mountDataP, mountPoint)
    IN ftu_mountData_t*	mountDataP;
    IN char*		mountPoint;
{
    caddr_t	privateData;

    switch (mountDataP->md_type) {
      case MOUNT_AGFS:
	privateData = (caddr_t)mountDataP->md_astab;
	break;

	/* Allow room for other mount types, like MOUNT_EFS */

      default:
	afsl_PAssertTruth(0, ("Bad mount type"));
	break;
    }

    if (mount(MOUNT_AGFS, mountPoint, 0, privateData) == -1)
	return errno;

    return 0;
}	/* _ftu_Mount() */


SHARED long
_ftu_Unmount(mountPoint)
    char*	mountPoint;
{
    if (umount(mountPoint, 0) == -1)
	return errno;

    return 0;
}	/* _ftu_Unmount() */
