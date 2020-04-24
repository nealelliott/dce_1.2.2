/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efsconfig.c,v $
 * Revision 1.1.64.1  1996/10/02  17:24:30  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:06  damon]
 *
 * Revision 1.1.59.2  1994/06/09  14:00:36  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:35:54  annie]
 * 
 * Revision 1.1.59.1  1994/02/04  20:13:24  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:25  devsrc]
 * 
 * Revision 1.1.57.1  1993/12/07  17:19:04  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:58:53  jaffe]
 * 
 * Revision 1.1.4.4  1993/01/19  15:27:02  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:47:28  cjd]
 * 
 * Revision 1.1.4.3  1992/11/24  16:25:02  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:23:06  bolinger]
 * 
 * Revision 1.1.4.2  1992/08/31  19:37:34  jaffe
 * 	Transarc delta: jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	  Selected comments:
 * 	    This revision of this delta contains changes made to episode/async by the OSF
 * 	    between dce1.0 and dce1.0.1b9.  Subsequent revisions will contain merges
 * 	    for other episode subdirectories.
 * 	    This revision of this delta contains changes made to episode/dir by the OSF
 * 	    This revision of this delta contains changes made to episode/lobguf by the OSF
 * 	    This revision of this delta contains changes made to episode/salvage by the OSF
 * 	    This revision of this delta contains changes made to episode/sautils by the OSF
 * 	    This revision of this delta contains changes made to episode/tools by the OSF
 * 	    This revision of this delta contains changes made to episode/utils by the OSF
 * 	    This revision of this delta contains changes made to episode/vnops by the OSF
 * 	    between dce1.0 and dce1.0.1b9.
 * 	    This revision fixes some problems in the logbuf layer that occurred after
 * 	    the merge.
 * 	    This delta adds anode/epi_errs.msf.
 * 	    This delta fixes problems in the logbuf layer.
 * 	    This revision fixes some problems in the vnode layer.
 * 	    Fix more vnode layer problems.
 * 	    Add files that were introduced after DCE 1.0 to the .klib Makefiles.
 * 	    This delta fixes various problems that caused undefined references
 * 	    while building the AIX kernel extension.
 * 	    Add code to grab `devsw' and `csa' from the kernel, since these
 * 	    symbols are not officially exported by the AIX kernel.
 * 	    This revision fixes random problems encountered under OSF/1.
 * 	    This revision fixes some problems encountered when compiling test_vnodeops
 * 	    on OSF/1.
 * 	    This revision includes the OSF's changes to epimount.
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
 * 	[1992/08/30  02:24:40  jaffe]
 * 
 * Revision 1.1.2.3  1992/05/13  21:58:59  jdp
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
 * 	[1992/05/13  21:08:48  jdp]
 * 
 * Revision 1.1.2.2  1992/05/05  19:07:56  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	delta jdp-epimerge-add-libefs 1.2
 * 	[1992/05/04  22:07:58  jdp]
 * 
 * Revision 1.1  1992/01/19  02:39:27  devrcs
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
#include <dcedfs/osi_mount.h>
#include <dcedfs/aggr.h>
#include <dcedfs/syscall.h>
#include <dcedfs/export.h>
#include <dcedfs/dir_buffer.h>
#include <dcedfs/episode/efs_opcode.h>

#include <sys/vfs.h>
#include <sys/gfs.h>
#include <sys/lockl.h>

extern struct vfsops efs_vfsops;
extern struct osi_vnodeops naix_ops;
extern struct aggrops ag_efsops;
extern int afscall_episode();

/*
 * Now a bit of magic.  AIX will reject an unmount request without even
 * calling the VFS op, if there is an outstanding reference to the root
 * vnode of the filesystem.  Alas, our tkc tends to hold these references
 * until its lru algorithm randomly flushes them.  To allow unmounts to
 * succeed we must disable this AIX cleverness, which we do by setting
 * GFS_FUMNT in the gfs flags.
 */
#define	EFS_FLAGS	GFS_FUMNT
#define	EPI_INIT_SINGLE	0

mid_t dcelfs_kmid;

long efs_parms[10];


/* Startup code for the VMM biod and asyncd processes ORB:5128 */

void
efs_vmm_proc(proc_type)
    int proc_type;
{
    sigset_t block_set;
    lock_t lock;

    setpinit();
    setsid();

    /* Block out all signals except SIGKILL which cannot be blocked */
    SIGFILLSET(block_set);
    sigprocmask (SIG_BLOCK, &block_set, 0);

    lock = lockl(&kernel_lock, LOCK_SHORT);

    switch(proc_type) {
	case 1:
	    epia_StartAsyncDaemon();
	    break;
	case 2:
	    efs_PageInDaemon();
	    break;
	case 3:
	    efs_PageOutDaemon();
	    break;
	case 4:
	    efs_PageUnprotectDaemon();
	    break;
    }
    if (lock != LOCK_NEST)
	unlockl(&kernel_lock);
}

void efs_asyncd()
{
    efs_vmm_proc(1 /* start asyncd */);
}

void efs_pageind()
{
    efs_vmm_proc(2 /* start pagein */);
}

void efs_pageoutd()
{
    efs_vmm_proc(3 /* start pageout */);
}

void efs_protectd()
{
    efs_vmm_proc(4 /* start page protect path */);
}

int efs_init(gfsp)
    struct gfs *gfsp;
{
    int id, rc;
    long *iparams = (long *) gfsp->gfs_data;
    struct efs_params efs_params;
#if	defined(AFS_AIX31_VM)
    extern int efs_vmstrategy();
    extern void efs_pageioInit();

    (void) vm_mount(D_REMOTE, efs_vmstrategy, 300/*XXX*/);
    efs_pageioInit();
#endif

    EFS_ELBB_PARAMS(efs_params).byteCount = iparams[0];
    EFS_EPIG_PARAMS(efs_params).maxHandles = iparams[1];
    EFS_DIRB_PARAMS(efs_params).abuffers = iparams[2];
    efs_params.vnodePoolSize = iparams[3];
    efs_params.flockPoolSize = iparams[4];
    efs_params.standalone = EPI_INIT_SINGLE;
    if (rc = epii_Init(&efs_params))
	goto out;
    
    if ((id = creatp()) == -1)
	panic("DCELFS pageind cfg creatp()\n");
    if (initp(id, efs_pageind, 0, 0, "EPGI"))
	panic ("DCELFS pageind cfg, initp()\n");

    if ((id = creatp()) == -1)
	panic("DCELFS pageoutd cfg creatp()\n");
    if (initp(id, efs_pageoutd, 0, 0, "EPGO"))
	panic ("DCELFS pageoutd cfg, initp()\n");

    if ((id = creatp()) == -1)
	panic("DCELFS protectd cfg creatp()\n");
    if (initp(id, efs_protectd, 0, 0, "EPRT"))
	panic ("DCELFS protectd cfg, initp()\n");

    if ((id = creatp()) == -1)
	panic("DCELFS asyncd cfg creatp()\n");
    if (initp(id, efs_asyncd, 0, 0, "EASY"))
	panic ("DCELFS asyncd cfg, initp()\n");
out:
    return rc;
}

struct gfs efs_gfs = {
	&efs_vfsops,
	&naix_ops,
	OSI_MOUNT_TYPE_LFS, 
	"efs",
	efs_init,
	EFS_FLAGS,
	(caddr_t)efs_parms,
	NULL
};

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

	nest = lockl(&kernel_lock, LOCK_SHORT);

	if (uiop)
		uiomove((caddr_t)efs_parms, sizeof(efs_parms), UIO_WRITE, uiop);
	else {

		/* 
		 * Set init parms for DCE LFS (episode)
		 */
	        efs_parms[0] = (1 << 18) + (1<< 17); /* buffer cache */
	        efs_parms[1] = 1 << 10;              /* anode pool   */
  	        efs_parms[2] = 10;                   /* dir buffers  */
  	        efs_parms[3] = 200;                  /* vnodes  */
  	        efs_parms[4] = 50;                   /* record locks */
 	        efs_parms[5] = 0;                    /* remote access*/
	}

	if (cmd == CFG_INIT) {			/* add gfs */
		if (err = pincode(epi_config))
			goto out;
		/*
		 * Put in dce lfs system call
		 */
		afs_set_syscall(AFSCALL_EPISODE, afscall_episode);

                /*
                 * Plug in aggregate ops
                 */
		ag_setops (AG_TYPE_EPI, &ag_efsops);

		err = gfsadd(OSI_MOUNT_TYPE_LFS, &efs_gfs);
		/*
		 * ok, if already installed
		 */
		if (err == EBUSY)
			err = 0;

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
		}
	} else					/* unknown command */
		err = EINVAL;

    out:
	if (nest != LOCK_NEST)
		unlockl(&kernel_lock);
	return err;
}
