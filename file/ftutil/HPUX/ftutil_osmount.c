/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * @HP_COPYRIGHT@
 */
/* 
 * HISTORY 
 * $Log: ftutil_osmount.c,v $
 * Revision 1.1.16.1  1996/10/02  17:48:08  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:13  damon]
 *
 * Revision 1.1.11.3  1994/07/13  22:28:03  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:35:16  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:05  mbs]
 * 
 * Revision 1.1.11.2  1994/06/09  14:08:52  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:23:34  annie]
 * 
 * Revision 1.1.11.1  1994/02/04  20:19:20  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:25  devsrc]
 * 
 * Revision 1.1.9.1  1994/01/28  20:43:42  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:17  annie]
 * 
 * 	expand OSF copyright
 * 
 * Revision 1.1.7.3  1993/08/13  13:30:55  kinney
 * 	More HP porting changes
 * 	[1993/08/12  21:10:53  kinney]
 * 
 * Revision 1.1.7.2  1993/07/19  19:30:58  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:23:29  zeliff]
 * 
 * Revision 1.1.4.3  1993/07/16  19:49:31  kissel
 * 	*** empty log message ***
 * 	[1993/06/21  14:28:41  kissel]
 * 
 * 	Initial GAMERA branch
 * 	[1993/04/02  11:48:48  mgm]
 * 
 * Revision 1.1.2.2  1993/06/04  14:36:14  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  21:37:54  kissel]
 * 
 * Revision 1.1.2.2  1993/01/19  13:31:40  toml
 * 	HPUX-dependent version of _ftu_Mount and _ftu_Unmount
 * 	[1993/01/19  13:23:28  toml]
 * 
 * $EndLog$
 */ 

#include <dcedfs/stds.h>
#include <dcedfs/agfs_mount.h>

#include <ftutil.h>

SHARED long
_ftu_Mount(vfsName, mountDataP, mountPoint)
	IN ftu_mountData_t *mountDataP;
	IN char		   *mountPoint;
	IN char		   *vfsName;	
{
    char vfsSlot[MAXPATHLEN];

    /*
     * In this routine we hook into HPUX dynamic vfs switch. 
     * Note the special hackery (-1 as fs type) that returns the
     * slot number (in ASCII) in the input parameter that
     * we supplied as an identifier.
     */	
    if (strcmp(vfsName, MOUNT_AGFS_NAME) == 0) {
	strcpy(vfsSlot, vfsName);
	if (vfsmount(-1, 0, 0, vfsSlot) >= 0) {
	    int vfsSlotNum = atoi(vfsSlot);
	    if (vfsmount(vfsSlotNum, mountPoint, 0, (caddr_t)mountDataP->md_astab) >= 0) 
		return 0;
	}
	return errno;
    } else {
	return FTU_E_BAD_MOUNT_TYPE;
    }
} /* _ftu_Mount() */

SHARED long
_ftu_Unmount(mountPoint)
    char*	mountPoint;
{
    if (umount(mountPoint) == -1)
	return errno;

    return 0;
}	/* _ftu_Unmount() */
