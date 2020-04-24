/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sefsconfig.c,v $
 * Revision 1.1.65.1  1996/10/02  17:24:32  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:07  damon]
 *
 * Revision 1.1.60.2  1994/06/09  14:00:37  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:35:54  annie]
 * 
 * Revision 1.1.60.1  1994/02/04  20:13:26  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:26  devsrc]
 * 
 * Revision 1.1.58.1  1993/12/07  17:19:06  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:59:09  jaffe]
 * 
 * Revision 1.1.4.4  1993/01/19  15:27:05  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:47:35  cjd]
 * 
 * Revision 1.1.4.3  1992/11/24  16:25:06  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:23:11  bolinger]
 * 
 * Revision 1.1.4.2  1992/08/31  19:37:56  jaffe
 * 	Transarc delta: jdp-ot4179-lfs-encapsulate-initialization 1.3
 * 	  Selected comments:
 * 	    This delta simplifies Episode user-space initialization.  There are two parts
 * 	    to this change.  The first simplifies the process of calling the *_Init
 * 	    routines.  Previously, you typically had to call dirb_Init(), N anode init
 * 	    routines, and logbuf_Init().  With this change, you need only specify the
 * 	    initialization parameters to the necessary layers in one structure and call
 * 	    the top-most init routine.  Since each initialization routine is now
 * 	    responsible for calling the lower layers' initialization routines, only
 * 	    one call is necessary.  The second change made in this delta was the
 * 	    encapsulation of the user-space device initialization code in one routine,
 * 	    us_InitDevice().  This routine is declared in tools/us.h and defined in
 * 	    tools/us_io.c.
 * 	    Convert to use the new initialization model.
 * 	    Fix a couple of remaining compilation problems.
 * 	    Fix compilation problems on OSF/1.
 * 	[1992/08/30  02:25:09  jaffe]
 * 
 * Revision 1.1.2.3  1992/05/13  21:59:01  jdp
 * 	Transarc delta: cburnett-ot2477-fix_bld_bugs_w_aix31_vm 1.1
 * 	  Files modified:
 * 	    anode: strategy.c; anode.klib: Makefile
 * 	    libefs/RIOS: efsconfig.c, sefsconfig.c
 * 	    vnops: efs_evnode.h, efs_init.c, efs_vnodeops.c
 * 	    vnops.klib: Makefile; vnops.klib/RIOS: Makefile
 * 	    vnops/RIOS: efs_aixvmm.c
 * 	  Selected comments:
 * 	    [ fix compile bugs when AFS_AIX31_VM is on.  This also takes care
 * 	    of ot2481 which is to move the Episode AIX VMM integration into
 * 	    the episode code base. It made sense to do this because of the
 * 	    interdependencies of the changes]
 * 	    [ include fixes and strategy assignment]
 * 	Transarc delta: cburnett-ot2640-aix32_changes_for_sefsconfig.c 1.1
 * 	  Files modified:
 * 	    libefs/RIOS: sefsconfig.c
 * 	  Selected comments:
 * 	    changes for AIX 3.2
 * 	    added include of xvfs_vnode.h to pick up struct vfsops def.
 * 	[1992/05/13  21:09:53  jdp]
 * 
 * Revision 1.1.2.2  1992/05/05  19:08:12  jdp
 * 	delta jdp-epimerge-add-libefs 1.2
 * 	delta jdp-epimerge-fix-libefs-RIOS-Makefile 1.1
 * 	[1992/05/04  22:09:11  jdp]
 * 
 * Revision 1.1  1992/01/19  02:39:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * epi_config -	entry point for the episode kernel extension
 */
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_device.h>
#include <dcedfs/osi_vmm.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/osi_cred.h>
#include <dcedfs/osi_mount.h>
#include <dcedfs/export.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/dir_buffer.h>
#include <dcedfs/episode/efs_opcode.h>

#include <sys/gfs.h>
#include <sys/lockl.h>


extern struct vfsops efs_vfsops;
extern struct osi_vnodeops naix_ops;

long efs_parms[10];

int efs_init(gfsp)
    struct gfs *gfsp;
{
    long *iparms = (long *) gfsp->gfs_data;
    struct efs_params efs_params;
#if	defined(AFS_AIX31_VM)
    extern int efs_vmstrategy();
    (void) vm_mount(D_REMOTE, efs_vmstrategy, 300/*XXX*/);
#endif

    EFS_ELBB_PARAMS(efs_params).byteCount = iparms[0];
    EFS_EPIG_PARAMS(efs_params).maxHandles = iparms[1];
    EFS_DIRB_PARAMS(efs_params).abuffers = iparms[2];
    efs_params.vnodePoolSize = iparms[3];
    efs_params.flockPoolSize = iparms[4];
    efs_params.standalone = iparms[5];
    return (epii_Init (&efs_params));
}

struct gfs efs_gfs = {
	&efs_vfsops,
	&naix_ops,
	OSI_MOUNT_TYPE_LFS, 
	"efs",
	efs_init,
	(int)NULL,
	(caddr_t)efs_parms,
	(int)NULL
};

mid_t	dcelfs_kmid;

/*
 * epi_config	-	handle episode configuration requests
 * Input:
 *	cmd	-	add/delete command
 *	uiop	-	uio vector describing any config params
 */
epi_config(cmd, uiop)
    struct uio *uiop; 
{
	int	err, nest;

	err  = 0;

	if (uiop)
		uiomove((caddr_t)efs_parms, sizeof(efs_parms), UIO_WRITE, uiop);

	nest = lockl(&kernel_lock, LOCK_SHORT);

	if (cmd == CFG_INIT) {			/* add gfs */
		if (err = kluge_init())
			goto out;
		if (err = pincode(epi_config))
			goto out;

		err = gfsadd(OSI_MOUNT_TYPE_LFS, &efs_gfs);
		/*
		 * ok, if already installed
		 */
		if (err == EBUSY)
			err = 0;

		timeoutcf(32);			/* silly VRMIX		*/
	} else if (cmd == CFG_TERM) {		/* delete gfs	*/
		err = gfsdel(OSI_MOUNT_TYPE_LFS);
		err = 0;
		/*
		 * ok, if already deleted
		 */
		if (err == ENOENT)
			err = 0;
		else if (!err) {
			if (err = unpincode(epi_config))
				err = 0;

			timeoutcf(-32);
		}
	} else					/* unknown command */
		err = EINVAL;

    out:
	if (nest != LOCK_NEST)
		unlockl(&kernel_lock);
	return err;
}


/* Temporary stuff hopefully */

iptovp(vfsp, ip, vpp)
    struct vfs	*vfsp;
    struct inode *ip, **vpp; 
{
    panic("Panic: iptovp stub");
}


struct inode *
iget(dev, ino, ipp)
    dev_t dev;
    ino_t ino;
    struct inode **ipp; 
{
    panic("Panic: iget stub");
}


iput(ip)
    struct inode *ip; 
{
    panic("Panic: iput stub");
}


/*
 * The following stuff is to account for the fact that stuff we need exported
 * from the kernel isn't, so we must be devious.
 */

void *(*kluge_crcopy)();
void *(*kluge_dev_ialloc)();
void *(*kluge_iuncache)();
void *(*kluge_ilock)();
void *(*kluge_irele)();
void *(*kluge_commit)();

/*
 * kernel function import list
 */
struct k_func kfuncs[] = {
	{ &kluge_crcopy,	".crcopy"	},
	{ &kluge_ilock,		".ilock"	},
	{ &kluge_irele,		".irele"	},
	{ &kluge_commit,	".commit"	},
	{ &kluge_dev_ialloc,	".dev_ialloc"	},
	{ &kluge_iuncache,	".iuncache"	},
	{ 0,			0		},
};

/*
 * kluge_init -	initialise the kernel imports kluge
 */
kluge_init() {
	register struct k_func *kf;
	register struct k_var  *kv;
	register ulong  toc;
	register err = 0;

	toc = get_toc();

	for (kf = kfuncs; !err && kf->name; ++kf)
		err = import_kfunc(kf);
	return err;
}


osi_cred_t *
crcopy(cr)
    osi_cred_t *cr; 
{
    return (osi_cred_t *) (*kluge_crcopy)(cr);
}


ilock(ip)
    struct inode *ip; 
{
    return (*kluge_ilock)(ip);
}


irele(ip)
    struct inode *ip; 
{
    return (*kluge_irele)(ip);
}


commit(n, i0, i1, i2)
    struct inode *i0, *i1, *i2; 
{
    return (*kluge_commit)(n, i0, i1, i2);
}


dev_ialloc(dev, ino, mode, ipp)
    dev_t dev;
    ino_t ino;
    mode_t mode;
    struct inode **ipp;
{
    return (*kluge_dev_ialloc)(dev, ino, mode, ipp);
}

iuncache(ip)
    struct inode *ip;
{
    return (*kluge_iuncache)(ip);
}
