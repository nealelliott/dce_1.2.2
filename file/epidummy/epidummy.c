/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: epidummy.c,v $
 * Revision 1.1.12.1  1996/10/02  17:15:26  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:06:52  damon]
 *
 * Revision 1.1.6.1  1994/06/09  13:55:55  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:30:28  annie]
 * 
 * Revision 1.1.4.7  1993/03/31  17:38:55  jaffe
 * 	    Provide dummy definitions for frozenAggrCount and xvfs_ops1.
 * 	[1993/03/31  16:18:38  jaffe]
 * 
 * Revision 1.1.4.6  1993/03/09  18:29:02  jaffe
 * 	Transarc delta: rajesh-ot7341-dfsvmunix-and-acls 1.1
 * 	  Selected comments:
 * 
 * 	    Acl_edit on a client only pmax running dfsvmunix was not calling into
 * 	    xvn_getacl. The sysacl layer was not compiled into the dfsvmunix
 * 	    kernel (though it was in efsvmunix). This resulted in the getacl
 * 	    syscall calling into an incorrect definition of afscall_vnode_ops in
 * 	    the epidummy layer, which just returned EINVAL.
 * 	    Remove bogus definition of afscall_vnode_ops
 * 	[1993/03/09  16:54:21  jaffe]
 * 
 * Revision 1.1.4.5  1993/01/18  20:54:12  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  17:40:56  cjd]
 * 
 * Revision 1.1.4.4  1992/11/24  21:19:05  rsarbo
 * 	Remove annoying printf from afscall_vnode_ops.
 * 	[1992/11/24  21:18:36  rsarbo]
 * 
 * Revision 1.1.4.3  1992/11/24  16:17:51  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:16:10  bolinger]
 * 
 * Revision 1.1.4.2  1992/08/31  19:32:03  jaffe
 * 	Transarc delta: bab-ot3539-syscall-error-propagation 1.6
 * 	  Selected comments:
 * 	    This delta unifies the error propagation model used by afs_syscall
 * 	    and the routines that it calls through the syscall switch.
 * 	    ot 3539
 * 	    This first version is simply a check-point so I can reopen another delta.
 * 	    Another check-point.
 * 	    Another checkpoint.
 * 	    Change the afssyscall calling convention for RS/6000 to correspond
 * 	    more closely to that of OSF/1.
 * 	    Debugging
 * 	    A little bit of cleanup.
 * 	    Corrected number of arguments to afscall_vnode_ops.  Surprisingly, it
 * 	    always worked when compiled with -g?!
 * 	[1992/08/30  02:14:50  jaffe]
 * 
 * Revision 1.1.2.4  1992/05/28  20:39:13  toml
 * 	Add stub for epi_configure().
 * 	[1992/05/28  20:29:40  toml]
 * 
 * Revision 1.1.2.3  1992/04/02  17:58:16  garyf
 * 	eliminate vfs initialization
 * 	[1992/04/02  17:13:01  garyf]
 * 
 * Revision 1.1.2.2  1992/01/24  01:51:10  devsrc
 * 	Fixed logs
 * 	[1992/01/23  22:10:28  devsrc]
 * 
 * $EndLog$
*/
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/lock.h>
#include <dcedfs/aggr.h>

#if defined(AFS_OSF_ENV) && defined (FREEZE_AGGR_OSF_2)
#include <dcedfs/xvfs_vnode.h>		/* struct xvfs_vnodeops */
#endif /* AFS_OSF_ENV */

#if defined(AFS_OSF_ENV) && defined (FREEZE_AGGR_OSF_2)
unsigned int frozenAggrCount = 0;
struct xvfs_vnodeops *xvfs_ops1 = 0;		/* EPI vfs ops */
#endif /* AFS_OSF_ENV */

static efs_inval () {
    return EINVAL;
}

struct aggrops ag_efsops = {
    efs_inval,
    efs_inval,
    efs_inval,
    efs_inval,
    efs_inval,
    efs_inval,
    efs_inval,
    efs_inval,
    efs_inval,
};

afscall_episode() {
    printf("afscall_episode no episode\n");
    return EINVAL;
}

epi_configure() {
    return EINVAL;
}

