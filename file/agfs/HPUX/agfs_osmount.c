/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: agfs_osmount.c,v $
 * Revision 1.1.97.1  1996/10/02  16:57:06  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:28  damon]
 *
 * Revision 1.1.91.3  1994/07/13  22:26:59  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:16:39  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:52:50  mbs]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  17:26:07  mbs]
 * 
 * Revision 1.1.91.2  1994/06/09  13:49:47  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:16  annie]
 * 
 * Revision 1.1.91.1  1994/02/04  20:04:04  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:05:38  devsrc]
 * 
 * Revision 1.1.89.2  1994/01/20  17:38:03  annie
 * 	added copyright header
 * 	[1994/01/20  17:34:12  annie]
 * 
 * Revision 1.1.89.1  1993/12/07  17:11:09  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:40:35  jaffe]
 * 
 * Revision 1.1.5.2  1993/07/19  19:28:19  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:21:29  zeliff]
 * 
 * Revision 1.1.2.3  1993/07/16  19:15:33  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/21  21:09:31  kissel]
 * 
 * 	Initial revision by tmm.
 * 	[1993/01/15  21:41:13  toml]
 * 
 * Revision 1.1.3.2  1993/04/02  12:07:47  mgm
 * 	Initial GAMERA branch
 * 
 * $EndLog$
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi_device.h>
#include <dcedfs/osi_uio.h>

#include <agfs_mount.h>

#ifdef GETMOUNT 
/* In ftutil.c, AGGR_MOUNT_DIR is defined in terms of
 * DCELOCAL_PATH ... but that's not defined for this
 * kernel component.  For now, just stick the text here
 * -- although something has to be done about this later.
 */
#define AGGR_MOUNT_DIR	"/opt/dcelocal/var/dfs/aggrs"

/*
 * Set .vfs_name to something like /dev/dsk/5s0:DFS
 */
#define GETMOUNT_SUFFIX		":DFS"

int agfs_GetMount (vfsp, fsmntdir, mdp)
    struct vfs *vfsp;
    caddr_t fsmntdir;
    struct mount_data *mdp;
{
    agfs_mount_t *mp;
    long code;
    int len;
    char laggname[MAX_AGGRNAME + 128];		/* should be big enough */
      
    mp = VFSTOAGFS(vfsp);

    /* only fields specific to AGFS are set */
    mdp->md_fstype = vfsp->vfs_mtype;
    /* Code used to do this
     * mdp->md_fstype = dfs_agfs_mcode;
     */

    /* is the following stuff really needed? */
    mdp->md_fsopts = 0;
    mdp->md_msite = 0; /* episode not site specific */
    mdp->md_rdev = mdp->md_dev = -1;
#if 0    
	makedev(mp->deviceP->majdev, mp->deviceP->mindev);
#endif

    /* fsmntdir == name of where we're mounted :
     *    /opt/dcelocal/var/dfs/aggr-namne */

    sprintf(laggname, sizeof(laggname), "%s%s%s", AGGR_MOUNT_DIR,
	    mp->am_aggr.a_aggrName[0] != '/' ? "/" : "",
	    mp->am_aggr.a_aggrName);

    return(copyoutstr(laggname, fsmntdir, sizeof(laggname), &len));
    
    return (0);
}
#endif

int
agfs_Mount(vfsP, pathname, data)
struct osi_vfs	*vfsP;
char 		*pathname;
char            *data;
{
	agfs_mount_t		*mtEntryP;
	agfs_mount_data_t	mountData;
	char 			devName[ASTABSIZE_SPEC+1];
	long 			code;
	dev_t			dev;
	struct vnode 		*bDevVP;
	int			devNameLen, suffixLen;

	/*
	 * Check initial conditions.  Copyin arguments.
	 */
	if (!agfs_inited) {
		return(EIO);
	}
	if (code = copyin(data, &mountData, sizeof(agfs_mount_data_t))) {
		return(code);
	}
	/*
	 * Now get and open the vnode referring to the device.
	 */
	(void)strncpy(devName, mountData.as_spec, ASTABSIZE_SPEC);
	devName[ASTABSIZE_SPEC] = '\0';
	if (code = osi_getmdev(devName, &dev)) {
		return(code);
	}
	if (code = osi_lookupname(devName, OSI_UIOSYS, FOLLOW_LINK,
				  (struct vnode *)0, &bDevVP)) {
		return(code);
	}
	/*
	 * If it's not a block special device, quit.
	 */
	if (osi_vType(bDevVP) != VBLK) {
		code = ENOTBLK;
		goto rele_dev;
	}
	if (code = agfs_GetMountTableEntry(&mountData, dev, 0, bDevVP, vfsP, &mtEntryP)) {
		goto close_dev;
	}
	vfsP->vfs_fsid[0] = dev;
	vfsP->vfs_fsid[1] = vfsP->vfs_mtype;
	vfsP->vfs_bsize = -1; 
	vfsP->vfs_data = (caddr_t)mtEntryP;
#ifdef GETMOUNT
	/* Set .vfs_name */
	devNameLen = strlen (devName);
	suffixLen = sizeof (GETMOUNT_SUFFIX);
	bcopy (devName, vfsP->vfs_name, devNameLen+1);
	if (devNameLen + suffixLen < MAXPATHLEN)
	    bcopy (GETMOUNT_SUFFIX, &(vfsP->vfs_name[devNameLen]), suffixLen+1);
	/* Save aggregate name */
	mountData.as_aggrName[ASTABSIZE_DIR-1] = '\0';	/* paranoid */
	bcopy (mountData.as_aggrName, mtEntryP->am_aggr.a_aggrName,
		strlen (mountData.as_aggrName));
#endif /* GETMOUNT */
 	return(0);

close_dev:
rele_dev:
	VN_RELE(bDevVP);
	return(code);
} /* agfs_Mount */

SHARED int
agfs_UnmountSubr (vfsP, force)
    struct osi_vfs *vfsP;
{
    int			code;
    struct vnode*	devVP;
    agfs_mount_t*	mtEntryP;

    mtEntryP = VFSTOAGFS(vfsP);
    devVP = mtEntryP->am_aggr.devvp;

    if (code = agfs_PutMountTableEntry(mtEntryP))
	return code;
    
    VN_RELE(devVP);

    return 0;
}	/* agfs_UnmountSubr() */


SHARED int
agfs_Unmount(vfsp)
    struct osi_vfs *vfsp;
{
    return agfs_UnmountSubr(vfsp, 0);
}	/* agfs_Unmount() */

