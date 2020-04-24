/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: epi_config.c,v $
 * Revision 1.1.52.1  1996/10/02  17:24:15  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:31:54  damon]
 *
 * Revision 1.1.47.2  1994/06/09  14:00:27  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:35:42  annie]
 * 
 * Revision 1.1.47.1  1994/02/04  20:13:16  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:20  devsrc]
 * 
 * Revision 1.1.45.2  1994/01/20  18:42:59  annie
 * 	added copyright header
 * 	[1994/01/20  18:39:14  annie]
 * 
 * Revision 1.1.45.1  1993/12/07  17:18:55  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:57:27  jaffe]
 * 
 * Revision 1.1.3.2  1993/08/13  16:49:01  kinney
 * 	<<<Empty log message>>>
 * 	[1993/08/11  20:16:09  kinney]
 * 
 * 	Correct name of variable with our VFS table index.
 * 	[1993/05/06  22:15:20  sommerfeld]
 * 
 * 	Save our VFS table index for future reference.
 * 	[1993/05/04  19:22:34  sommerfeld]
 * 
 * 	Initial revision.
 * 	[1993/04/12  18:31:26  toml]
 * 
 * $EndLog$
 */
/*
 * Routines for dynamically loading and unloading Episode.
 */

#include <dcedfs/osi.h>
#include <dcedfs/osi_sysconfig.h>
#include <sys/mount.h>
#include <sys/systm.h>
#include <sys/syscall.h>
#include <dcedfs/syscall.h>
#include <dcedfs/aggr.h>

extern struct vfsops efs_vfsops;
extern struct aggrops ag_efsops;
extern struct aggrops *agOpvec[];
extern int afscall_episode(), afs_nosys(), afscall_aggr();

int efs_vfs_mtype = -1;

int
dfs_episode_config(op)
int op;
{
	int	ret;

	switch (op) {
		case KLOAD_INIT_CONFIG:
#ifndef	AFS_HPUX80_ENV
			if ((ret = register_vfs_type("hp-efs")) == -1)
			        return(ret);
#endif	/* AFS_HPUX80_ENV */
			printf("LFS assigned VFS slot %d\n", ret);
			vfssw_assign(ret, efs_vfsops);
			efs_vfs_mtype = ret;
			setup(op);
			break;
		case SYSCONFIG_UNCONFIGURE:
			if ((ret = efs_unconfig()) != 0)
				return(ret);
			break;
		default:
			return EINVAL;
			break;
	}

	return 0;
}

setup(op)
{
#define afson (op == SYSCONFIG_CONFIGURE)

	if (op == SYSCONFIG_CONFIGURE) {

		agOpvec[AG_TYPE_EPI] = &ag_efsops;

	}
	AfsCall (AFSCALL_EPISODE, afson ? afscall_episode: 	afs_nosys);
	AfsCall (AFSCALL_AGGR,    afson ? afscall_aggr: 	afs_nosys);

	return 0;

}

efs_unconfig()
{
	return(EBUSY);
}
