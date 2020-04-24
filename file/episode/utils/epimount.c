/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: epimount.c,v $
 * Revision 1.1.102.1  1996/10/02  17:42:31  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:06  damon]
 *
 * Revision 1.1.97.2  1994/06/09  14:04:31  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:39:14  annie]
 * 
 * Revision 1.1.97.1  1994/02/04  20:15:59  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:44  devsrc]
 * 
 * Revision 1.1.95.1  1993/12/07  17:21:54  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:03:27  jaffe]
 * 
 * Revision 1.1.5.8  1993/01/21  18:25:14  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  16:46:17  zeliff]
 * 
 * Revision 1.1.5.7  1993/01/13  16:57:36  shl
 * 	Transarc delta: jaffe-cleanup-2.3-compiler-warnings 1.8
 * 	  Selected comments:
 * 	    Fix compiler warnings found when moving to OSF/1 1.1.1.
 * 	    cast data to char * in mount call.
 * 	    Remove the function dacl_SetOwnerControl.  It is not used.
 * 	Transarc delta: jdp-ot6481-detach-on-failed-mount 1.1
 * 	  Selected comments:
 * 	    This delta fixes the problem where epimount was not detaching the aggregate
 * 	    after a failed mount.
 * 	    If the mount fails, detach the aggregate before exiting.
 * 	[1993/01/12  20:25:29  shl]
 * 
 * Revision 1.1.5.6  1992/11/24  16:58:13  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  17:52:25  bolinger]
 * 
 * Revision 1.1.5.5  1992/11/18  19:42:19  jaffe
 * 	Transarc delta: jdp-ot5415-mount-aggregate-on-attach-epi 1.1
 * 	  Selected comments:
 * 	    This delta contains the Episode part of the changes necessary to have
 * 	    aggregates mounted at attach time.  There are corresponding deltas in both
 * 	    the DFS and test configurations.
 * 	    Use FTU_ATTACH_FLAGS_NOEXPORT in call to ftu_AttachAggr().
 * 	[1992/11/17  21:22:52  jaffe]
 * 
 * Revision 1.1.5.4  1992/10/28  21:43:36  jaffe
 * 	Fixed RCSID
 * 	[1992/10/28  21:14:19  jaffe]
 * 
 * Revision 1.1.5.3  1992/10/27  20:53:33  jaffe
 * 	Transarc delta: rajesh-ot5575-epimount-remove-bogus-default-values 1.1
 * 	  Selected comments:
 * 
 * 	    epimount assumes wierd default values for volume id, name, device name
 * 	    and mount points. In fact the default value of 37 for volume id
 * 	    prevents epimount from working when only the volume name is specified.
 * 	    Remove default value of 37 for volume_id. Remove DEFINED_VBLS and
 * 	    various platform specific ifdefs [AFS_IBM_ENV,AFS_AIX31_ENV,AFS_VFS40]
 * 	    in which default values for device name, volume name and mount points
 * 	    were being set.
 * 	[1992/10/27  13:07:54  jaffe]
 * 
 * Revision 1.1.5.2  1992/08/31  20:48:52  jaffe
 * 	Fix Header, sync with Transarc
 * 	[1992/08/30  14:37:35  jaffe]
 * 
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
 * 	    Merged in changes made by the OSF between dce1.0 and dce1.0.1b9.
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
 * 	    This revision fixes random problems encountered under OSF/1.
 * 	    This revision fixes some problems encountered when compiling test_vnodeops
 * 	    on OSF/1.
 * 	    This revision includes the OSF's changes to epimount.
 * 	    Merge OSF changes of "test/epimount.c" into "utils/epimount.c".
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
 * 	    Added include of <dcedfs/dir_buffer.h> since efs_opcode.h requires it.
 * 	    Fix a couple of remaining compilation problems.
 * 	    Fix compilation problems on OSF/1.
 * 	Transarc delta: mason-add-hp800-osi-routines 1.15
 * 	  Selected comments:
 * 	    This delta is misnamed - these are really porting fixes.
 * 	    Need to checkpoint to fix a different bug.
 * 	    Checkpoint the first system where test_anode works.  Only one change is not
 * 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
 * 	    is -lBSD.  This should be fixed more generally.
 * 	    More HP-UX changes.
 * 	    More HP-UX related changes.
 * 	    Fix compile problem on the RIOS.
 * 	    Fix more compilation problems.
 * 	    Checkpoint.
 * 	    Checpoint.
 * 	    In this snap we turned all malloc() calls into osi_Alloc calls.
 * 	    Also turned free calls into osi_Free.
 * 	    HP-UX EOF detection fix.
 * 	    Another checkpoint.
 * 	    Port epimount to hp-ux.
 * 	    This is a checkpoint of the first working HP-UX Episode system.
 * 	    HP changes.
 * 	    Needed to add the vnops routines specific to HP-UX.
 * 	    Fix compilation problems.
 * 	[1992/08/30  03:03:33  jaffe]
 * 
 * Revision 1.1.2.9  1992/07/02  18:02:50  jdp
 * 	Transarc delta: mason-ot4011-restore-rcsids 1.1
 * 	  Selected comments:
 * 	    The RCSIDs are useful for debugging.  Putting a dollar sign in place of the
 * 	    source system's identifier is really not a good idea.  This delta restores
 * 	    them.
 * 	    Replace dollar with dollar-header-dollar (for RCS)
 * 	[1992/07/02  15:36:35  jdp]
 * 
 * Revision 1.1.2.8  1992/06/23  13:56:14  jdp
 * 	Transarc delta: bwl-ot4074-bad-attach-for-epimount 1.2
 * 	  Selected comments:
 * 	    Changes to support recovery in epimount.
 * 	    ftu_Attach now takes a new parameter.
 * 	    Corrected compilation problems on OSF/1.
 * 	Transarc delta: jdp-ot3839-xaggr-add-recovery-to-dfsexport 1.1
 * 	  Selected comments:
 * 	    Make calls from Episode to the ftutil layer match the new signature of
 * 	    ftu_AttachAggr().
 * 	    Pass two more arguments to ftu_AttachAggr().
 * 	[1992/06/22  22:13:39  jdp]
 * 
 * Revision 1.1.2.7  1992/06/02  19:19:30  garyf
 * 	remove OSF/1 conditional
 * 	add default case
 * 	[1992/06/02  19:16:23  garyf]
 * 
 * Revision 1.1.2.6  1992/06/01  19:18:00  jdp
 * 	Transarc delta: jdp-ot3338-add-recovery-to-epimount 1.5
 * 	  Files modified:
 * 	    utils: epimount.c, epiunmount.c
 * 	  Selected comments:
 * 	    Activate attach/detach code in epimount/epiunmount on OSF/1.
 * 	    Remove "#ifndef AFS_OSF_ENV" from around attach code.
 * 	[1992/06/01  16:39:36  jdp]
 * 
 * Revision 1.1.2.5  1992/05/22  20:06:58  garyf
 * 	remove OSF1 conditional
 * 	[1992/05/22  19:45:41  garyf]
 * 
 * Revision 1.1.2.4  1992/05/19  18:50:58  jdp
 * 	Transarc delta: jdp-ot3338-add-recovery-to-epimount 1.4
 * 	  Files modified:
 * 	    utils: Makefile, epimount.c, epiunmount.c
 * 	  Selected comments:
 * 	    This delta will add aggregate recovery to the epimount command.  It will
 * 	    also cause aggregates to be attached on the first mount of a fileset within
 * 	    the aggregate.  This revision, however, is merely a checkpoint.
 * 	    Added basic attach/recovery code.
 * 	    More recovery in epimount changes.
 * 	    Add call to ftu_AttachAggr() on each mount.
 * 	    More recovery changes.
 * 	    Remove code to attach an aggregate once for each mount, since the xaggr layer
 * 	    doesn't currently support this.
 * 	    Corrected minor problem in epimount.
 * 	    Added missing stream argument (stderr) to fprintf().
 * 	[1992/05/19  15:49:56  jdp]
 * 
 * Revision 1.1.2.3  1992/05/13  22:00:41  jdp
 * 	Transarc delta: cburnett-ot2492-clean_up_episode_test_dir 1.2
 * 	  Files modified:
 * 	    test: Makefile, epimount.c, epiunmount.c
 * 	    utils: Makefile, epimount.c, epiunmount.c
 * 	  Selected comments:
 * 	    [  epimount and epiumount should not be in the test dir]
 * 	    [  updated with changes OFS had been applying to test/epimount.c]
 * 	    [ problems encounterd with first close regarding posting]
 * 	Transarc delta: jdp-epimerge-2.17+-into-epi-weekly-1.2 1.4
 * 	  Files modified:
 * 	    anode: strategy.c; logbuf: Makefile; utils: epimount.c
 * 	    vnops: efs_vnodeops.c; vnops.klib/RIOS: Makefile
 * 	  Selected comments:
 * 	    This delta fixes problems with the merge of post episode-2.17 changes into
 * 	    episode-weekly-1.2.
 * 	    Fix more merge problems.
 * 	    Remove include of "efs_opcodes.h".
 * 	[1992/05/13  21:31:35  jdp]
 * 
 * Revision 1.1.2.2  1992/05/05  16:39:01  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  23:29:57  jdp]
 * 
 * Revision 1.1.2.2  1992/01/24  03:01:11  devsrc
 * 	Fixed logs
 * 	[1992/01/23  22:14:29  devsrc]
 * 
 * $EndLog$
*/
/*
 * Episode mount program
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/common_data.h>
#include <dcedfs/osi_mount.h>
#include <dcedfs/syscall.h>
#include <dcedfs/cmdAux.h>
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */
#include <dcedfs/episode/efs_opcode.h>
#include <dcedfs/episode/volume.h>
#include <dcedfs/episode/efsmount.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/utils/epimount.c,v 1.1.102.1 1996/10/02 17:42:31 damon Exp $")

#define ARG_FILESET_INDEX    0
#define ARG_MOUNTPOINT_INDEX 1
#define ARG_READONLY_INDEX   2
#define ARG_NFS_INDEX        3


void	PrintUsage 	_TAKES((int));
int 	mountDriver 	_TAKES((struct cmd_syndesc *, char *));

static char*	programName;
int exitStat = 0;

void PrintUsage(int exstat)
{
    fprintf(stderr, "Usage:\n%s <fileset> <mount_point> [-readonly] [-nfs] [-help]\n",
	    programName);
    exit(exstat);
}

int main(int argc, char* argv[])
{
    struct cmd_syndesc *ts;


    osi_setlocale(LC_ALL, "");
    programName = argv[0];
    exitStat = 0;

    ts = cmd_CreateSyntax((char *)0, mountDriver, (char *)0, "epimount" );

    /* '-fileset' at offset 0 */
    cmd_AddParm(ts, "-fileset", CMD_SINGLE, CMD_REQUIRED, "fileset_name");

    /* '-mountpoint' at offset 1 */
    cmd_AddParm(ts, "-mountpoint", CMD_SINGLE, CMD_REQUIRED, "mount_point");

    /* '-readonly' at offset 2 */
    cmd_AddParm(ts, "-readonly", CMD_FLAG, CMD_OPTIONAL, "mount_read_only");

    /* '-nfs' at offset 3 */
    cmd_AddParm(ts, "-nfs", CMD_FLAG, CMD_OPTIONAL, "mount_nfs_synchronization");

    exit (cmd_Dispatch(argc, argv));
}


int mountDriver(struct cmd_syndesc *as, char *arock)
{
    struct efs_mount_data	mountData;
    char			*fileset, *mountPoint;
    int				code, flags = 0;


    
    /* '-fileset'    at offset 0 */
    /* '-mountpoint' at offset 1 */
    /* '-readonly'   at offset 2 */
    /* '-nfs'        at offset 3 */
    if ((cmd_IsParamPresent(as, ARG_FILESET_INDEX) == 0) ||
	(cmd_IsParamPresent(as, ARG_MOUNTPOINT_INDEX) == 0)) {
	PrintUsage(1);
	/* NOTREACHED */
    }

    fileset = cmd_FirstData(as, ARG_FILESET_INDEX);
    mountPoint = cmd_FirstData(as, ARG_MOUNTPOINT_INDEX);

    if (cmd_IsParamPresent(as, ARG_READONLY_INDEX)) {
#if	!defined(READONLY_SUPPORT)
	fprintf(stderr, "%s: Read-only local mount option unsupported\n",
		programName);
	return(1);
#else
	flags |= OSI_MOUNT_FLAGS_READONLY;
#endif
    }

    bzero(&mountData, sizeof mountData);
    if (cmd_IsParamPresent(as, ARG_NFS_INDEX)) {
	mountData.flags |= EFS_MOUNT_DATA_FLAG_NFS;
    }

    if (isdigit(fileset[0])) {
	if (code = ftu_ParseFsetId(fileset, &mountData.volId)) {
	    fprintf(stderr, "%s: Error parsing %s as fileset ID: %s\n",
		    programName, fileset, dfs_dceErrTxt(code));
	    return(2);
	}
	mountData.flags |= EFS_MOUNT_DATA_FLAG_USE_ID;
    }
	
    code = osi_mount(fileset, mountPoint, OSI_MOUNT_TYPE_LFS, flags, NULL,
		     &mountData, sizeof mountData);
    if (code) {
	fprintf(stderr, "%s: Error mounting %s on %s: %s\n",
		programName, fileset, mountPoint, strerror(code));
	return(1);
    }
    return(0);
}


