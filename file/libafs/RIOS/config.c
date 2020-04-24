/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/libafs/RIOS/RCS/config.c,v 4.16 1995/08/03 18:37:42 blake Exp $
 *
 * afs_config -	entry point for the AFS kernel extension
 *
 * Copyright (c) 1995, 1990, Transarc Corporation.
 */
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/osi_mount.h>
#include <dcedfs/syscall.h>
#include <dcedfs/xvfs_vnode.h>

#include <sys/gfs.h>
#include <sys/lockl.h>
#include <sys/device.h>

extern struct vfsops afs_vfsops;
extern struct osi_vnodeops naix_ops;
extern int afscall_cm();
extern int afscall_cm_pioctl();
extern int afscall_cm_newtgt();
extern int afscall_exporter();


int afs_init(gfsp)
    char *gfsp;
{
    return 0;
}

struct gfs afs_gfs = {
	&afs_vfsops,
	&naix_ops,
	OSI_MOUNT_TYPE_DFS,
	"dfs",
	afs_init,
	GFS_REMOTE,
	NULL
};

mid_t	dfscmfx_kmid;

/*
 * afs_config	-	handle AFS configuration requests
 *
 * Input:
 *	cmd	-	add/delete command
 *	uiop	-	uio vector describing any config params
 */
afs_config(cmd, uiop)
    struct uio *uiop;
{
	int	err, nest;
	long krpchdd_major = -1;

	err  = 0;
	nest = lockl(&kernel_lock, LOCK_SHORT);

	if (cmd == CFG_INIT) {			/* add AFS gfs		*/
		/*
		 * make sure that we pin everything
		 */
		if (err = pincode(afs_config))
			goto out;

		/*
		 * Put in system calls here
		 */
		afs_set_syscall(AFSCALL_CM, afscall_cm);
		afs_set_syscall(AFSCALL_PIOCTL, afscall_cm_pioctl);
		afs_set_syscall(AFSCALL_PX, afscall_exporter);
		afs_set_syscall(AFSCALL_NEWTGT, afscall_cm_newtgt);

		/*
		 * Setup up krpc helper and the krpchdd psuedo device driver.
		 */
		krpc_helper_init();
		if (uiop)
		    uiomove((caddr_t)&krpchdd_major, sizeof(krpchdd_major),
		    UIO_WRITE, uiop);

		if (krpchdd_major > 0) {
		    err = krpchdd_config(makedev(krpchdd_major, 0), CFG_INIT);
		    if (err)
			goto out;
		} else {
		    err = EINVAL;
		    goto out;
		}

		err = gfsadd(OSI_MOUNT_TYPE_DFS, &afs_gfs);
		/*
		 * ok, if already installed
		 */
		if (err == EBUSY)
			err = 0;

	} else if (cmd == CFG_TERM) {		/* delete AFS gfs	*/
		err = gfsdel(OSI_MOUNT_TYPE_DFS);
		/*
		 * ok, if already deleted
		 */
		if (err == ENOENT)
			err = 0;
		else if (!err) {
			if (err = unpincode(afs_config))
				err = 0;
		}
	} else					/* unknown command */
		err = EINVAL;

    out:
	if (nest != LOCK_NEST)
		unlockl(&kernel_lock);
	return err;
}
