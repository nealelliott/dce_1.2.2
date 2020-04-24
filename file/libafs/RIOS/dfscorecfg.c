/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/osi_device.h>
#include <dcedfs/osi_mount.h>
#include <dcedfs/export.h>
#include <dcedfs/aggr.h>
#include <dcedfs/syscall.h>
#include <dcedfs/agfs_mount.h>
#include <sys/lockl.h>
#include <sys/vfs.h>	/* for struct vfsops */

extern struct aggrops ag_ufsops;
extern struct vfsops agfs_vfsops;
extern struct vnodeops agfs_vnops;
extern int afscall_volser();
extern int afscall_aggr();
extern int afscall_plumber();
extern int afscall_tkm_control();
extern int afscall_vnode_ops();
extern int afscall_icl();
extern int afscall_bomb();
extern int afscall_setpag();
extern int afscall_getpag();
extern int afscall_resetpag();
extern int zlc_Init();
extern int agfs_Init();

static struct gfs agfs_gfs = {
	&agfs_vfsops,			/* VFS ops */
	&agfs_vnops,			/* Vnode ops */
	OSI_MOUNT_TYPE_AGFS,		/* VFS type */
	"agfs",				/* VFS name */
	agfs_Init,			/* Initialization function */
	0,				/* Flags */
	NULL,				/* Configration data */
	NULL				/* (*gfs_rinit)() ? */
};

/*
 * dfs_core_config -	configure DFS core services
 *
 * Input:
 *	cmd	-	add/delete command
 *	uiop	-	uio vector describing any config params
 */

mid_t	dfscore_kmid;
int     dfscore_configured = FALSE;

dfs_core_config(cmd, uiop)
    struct uio *uiop;
{
	int	err, nest;

	err  = 0;
	nest = lockl(&kernel_lock, LOCK_SHORT);

	if (cmd == CFG_INIT) {

		if (dfscore_configured)
		    goto out;

		/*
		 * init symbol exporter
		 */
		if (err = kluge_init())
			goto out;

		/*
		 * make sure the timer blocks are initialized before any code
		 * that might call osi_Wait.  Need one of these structures
		 * for each process that might call osi_Wait, i.e. each
		 * process that might be executing DFS code.
		 */
		timeoutcf(300);

		/*
		 * Init modules in extension
		 */
		osi_init();
		vol_Init();
		volreg_Init();
		xcred_Init();
		tkc_Init();
		tkm_Init();
		vnl_init(256);
		zlc_Init();
		OSI_TRACE_INIT();

		/*
		 * Plug in UFS aggregate & fileset ops
		 */
		ag_setops (AG_TYPE_UFS, &ag_ufsops);

		/*
		 * Setup system calls supported by the core extension
		 */
		afs_set_syscall(AFSCALL_VOL, afscall_volser);
		afs_set_syscall(AFSCALL_AGGR, afscall_aggr);
		afs_set_syscall(AFSCALL_PLUMBER, afscall_plumber);
		afs_set_syscall(AFSCALL_TKM, afscall_tkm_control);
		afs_set_syscall(AFSCALL_VNODE_OPS, afscall_vnode_ops);
		afs_set_syscall(AFSCALL_ICL, afscall_icl);
		afs_set_syscall(AFSCALL_BOMB, afscall_bomb);
		afs_set_syscall(AFSCALL_SETPAG, afscall_setpag);
		afs_set_syscall(AFSCALL_GETPAG, afscall_getpag);
		afs_set_syscall(AFSCALL_RESETPAG, afscall_resetpag);

		/*
		 * make sure that we pin everything
		 */
		if (err = pincode(dfs_core_config))
			goto out;

		err = gfsadd(OSI_MOUNT_TYPE_AGFS, &agfs_gfs);
		err = (err == EBUSY ? 0 : err); /* OK, if already installed */
		if (err)
		    goto out;

		dfscore_configured = TRUE;

	} else if (cmd == CFG_TERM) {
		unpincode(dfs_core_config);
		timeoutcf(-32);
		err = gfsdel(OSI_MOUNT_TYPE_AGFS);
		err = (err == ENOENT ? 0 : err); /* OK, if already deleted */
		if (err)
		    goto out;
	} else					/* unknown command */
		err = EINVAL;

    out:
	if (nest != LOCK_NEST)
		unlockl(&kernel_lock);
	return err;
}

/*
 * The following stuff is to account for the fact that stuff we need exported
 * from the kernel isn't, so we must be devious.
 */

void *(*kluge_iptovp)();
void *(*kluge_ilock)();
void *(*kluge_irele)();
void *(*kluge_iget)();
void *(*kluge_iput)();
void *(*kluge_commit)();
void *(*kluge_crcopy)();
void *(*kluge_dev_ialloc)();
void *(*kluge_iuncache)();
void *(*kluge_dc_purge)();

/*
 * kernel function import list
 */
struct k_func kfuncs[] = {
	{ &kluge_iptovp,	".iptovp"	},
	{ &kluge_ilock,		".ilock"	},
	{ &kluge_irele,		".irele"	},
	{ &kluge_iget,		".iget"		},
	{ &kluge_iput,		".iput"		},
	{ &kluge_commit,	".commit"	},
	{ &kluge_crcopy,	".crcopy"	},
	{ &kluge_dev_ialloc,	".dev_ialloc"	},
	{ &kluge_iuncache,	".iuncache"	},
	{ &kluge_dc_purge,	".dc_purge"	},
	{ 0,			0		},
};

/*void *vnodefops;	/* dummy vnodeops	*/

/*
 * kernel variable import list
 */
struct k_var kvars[] = {
/*	{ (void *) &vnodefops,	"vnodefops"	}, */
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
	for (kv = kvars; !err && kv->name; ++kv)
		err = import_kvar(kv, toc);
	return err;
}

iptovp(vfsp, ip, vpp)
    struct vfs	*vfsp;
    struct inode *ip, **vpp;
{
    return (*kluge_iptovp)(vfsp, ip, vpp);
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


struct inode *
iget(dev, ino, ipp)
    dev_t dev;
    ino_t ino;
    struct inode **ipp;
{
    return (*kluge_iget)(dev, ino, ipp);
}


iput(ip)
    struct inode *ip;
{
    return (*kluge_iput)(ip);
}


commit(n, i0, i1, i2)
    struct inode *i0, *i1, *i2;
{
    return (*kluge_commit)(n, i0, i1, i2);
}


osi_cred_t *
crcopy(cr)
    osi_cred_t *cr;
{
    return (osi_cred_t *) (*kluge_crcopy)(cr);
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

dc_purge(dev)
    dev_t dev;
{
    return (*kluge_dc_purge)(dev);
}
