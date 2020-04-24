/*
 ** @OSF_COPYRIGHT@
 ** COPYRIGHT NOTICE
 ** Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 ** ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 ** the full copyright text.
*/
/*
 * HISTORY
 * $Log: px_config.c,v $
 * Revision 1.1.11.1  1996/10/02  17:53:51  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:41:53  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:12:44  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:26:04  annie]
 * 
 * Revision 1.1.4.4  1993/01/19  16:06:56  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:16:54  cjd]
 * 
 * Revision 1.1.4.3  1992/11/24  17:54:14  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:09:02  bolinger]
 * 
 * Revision 1.1.4.2  1992/11/18  20:35:26  jaffe
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
 * 	    Add a call to vfssw_add() to install the aggregate filesystem.
 * 	    Remove leftover junk from $OFILES.
 * 	    Rearrange things slightly to make use of a vendor-supplied definition
 * 	    of MOUNT_AGFS.
 * 	    Replace AFS_MOUNT_AGGREGATE with MOUNT_AGFS.
 * 	    Fix bogus linker warnings about shared libraries when compiling .o files.
 * 	[1992/11/17  21:55:30  jaffe]
 * 
 * 	Merging dfs5.11 to dfs6.3
 * 
 * Revision 1.1.2.2  1992/05/28  20:38:57  toml
 * 	Add px_configure() for dynamic init of AFS syscall table.
 * 	[1992/05/28  20:28:37  toml]
 * 
 * Revision 1.1.2.2  1992/01/24  01:47:31  devsrc
 * 	Bring in dfs6.3 sources
 * 	[1992/01/22  19:28:42  devsrc]
 * 
 * Revision 1.1.2.2  91/11/20  17:38:33  garyf
 * 	initial coding
 * 	[91/11/20  17:36:58  garyf]
 * 
 * $EndLog$
*/

#include <dce_afs.h>
#include <dcedfs/param.h>		/* Should be always first */
#include <dcedfs/syscall.h>
#include <dcedfs/agfs_mount.h>
#include <sys/sysconfig.h>

extern int afscall_exporter(), afscall_volser(), afscall_aggr(), afs_nosys();
extern struct vfsops agfs_vfsops;

px_configure(op, indata, indatalen, outdata, outdatalen)
  sysconfig_op_t    op;
  void              *indata;
  size_t            indatalen;
  void              *outdata;  
  size_t            outdatalen;
{
	int afson = (op == SYSCONFIG_CONFIGURE);
	int code;
	
	AfsCall(AFSCALL_PX,   afson ? afscall_exporter : afs_nosys);
	AfsCall(AFSCALL_VOL,  afson ? afscall_volser   : afs_nosys);
	AfsCall(AFSCALL_AGGR, afson ? afscall_aggr     : afs_nosys);
	  
	if (afson && (code = vfssw_add(MOUNT_AGFS, &agfs_vfsops)))
	    return code;

	return 0;
}

