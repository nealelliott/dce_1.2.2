/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dcebase_config.c,v $
 * Revision 1.1.81.1  1996/10/02  17:53:41  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:41:44  damon]
 *
 * Revision 1.1.76.2  1994/06/09  14:12:35  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:25:56  annie]
 * 
 * Revision 1.1.76.1  1994/02/04  20:22:03  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:38  devsrc]
 * 
 * Revision 1.1.74.1  1993/12/07  17:27:36  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:44:11  jaffe]
 * 
 * Revision 1.1.6.2  1993/07/19  19:32:01  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:24:23  zeliff]
 * 
 * Revision 1.1.4.3  1993/07/16  19:50:54  kissel
 * 	*** empty log message ***
 * 	[1993/06/21  14:31:01  kissel]
 * 
 * 	Tweak for kernel extensions.
 * 	[1993/03/15  16:09:45  toml]
 * 
 * 	afs -> dcedfs.
 * 	[1993/01/14  19:42:26  toml]
 * 
 * 	10/23/92 	tmm	Add dfs_translate_creds pointer init.
 * 	[1992/11/23  17:08:25  tmm]
 * 
 * 	Initial coding for HPUX. Removed references to afs_xioctl for DFS1.0.2.
 * 	[1992/10/14  16:44:51  toml]
 * 
 * 	Initial GAMERA branch
 * 	[1993/04/02  11:51:04  mgm]
 * 
 * Revision 1.1.2.2  1993/06/04  14:52:56  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  21:40:01  kissel]
 * 
 * $EndLog$
*/
/*
 * Configure the 'dcebase' kernel extension.
 *
 * Right now, this involves init'ing any function
 * pointers stored in the kernel to point out into
 * this extension.  This includes the pointer to
 * AFS Syscall in the sysent table, and a miscellaneous
 * EPV used by UFS and VFS to call into xvnode.
 * 
 */
#include <dcedfs/param.h>
#include <sys/systm.h>
#include <sys/syscall.h>
#include <dcedfs/osi_sysconfig.h>	/* Before syscall.h for kload.h */
#include <dcedfs/syscall.h>
#include <sys/errno.h>

/* shared between CM and EPISODE */
#if	0
long cold_shutdown = 0;			/* Shutdown related */
#endif

/*
 * Function ptr in the kernel (ufs_dir.c) which points to
 * a replacement for VOP_ACCESS (xvfs_vfs2os.c), which
 * bypasses the 'glue' vnode ops.
 */
extern int (*xafs_access_op)();
/*
 * In hpux one may specify an init routine routine which is called once during initialization.
 * [This is the only DFS routine that is directly referenced from the kernel.]
 */
dfs_link()
{
    extern int afs_xnfs_svc(), afs_syscall(), afs_xsetgroups();
    extern int xafs_access();

    sysent_assign(AFS_SYSCALL, 6, afs_syscall); /* install DFS syscall into syscall table */
    xafs_access_op = &xafs_access;		/* link 'glue bypass' routine into dirlook() */
#ifdef	notdef
    sysent_assign(80, 2, afs_xsetgroups);	/* intercept setgroups() */
    sysent_assign(SYS_NFSSVC,1,afs_xnfs_svc);	/* install nfs exporter into syscall table */
#endif
}

#ifdef	AFS_DYNAMIC
/*
 * Entry called by KLOAD.
 */
dfs_core_config(arg)
int arg;
{
	if (arg==KLOAD_INIT_CONFIG) {
		dfs_link();
		all_configure();	/* add AFS syscalls to afs_sysent table */
		return(0);
	} else if (arg==KLOAD_INIT_UNCONFIG) {

		/*
		 * XXX 
		 *
		 * extern int kload_syscall();
		 * if (not config'ed)
		 *	/ * restore syscall to original value * /
		 * 	sysent_assign(AFS_SYSCALL, 6, kload_syscall);
		 */

		return(EBUSY);
	}		
}
#endif	/* AFS_DYNAMIC */
