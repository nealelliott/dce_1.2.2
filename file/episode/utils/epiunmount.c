/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: epiunmount.c,v $
 * Revision 1.1.21.1  1996/10/02  17:42:33  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:08  damon]
 *
 * Revision 1.1.16.2  1994/06/09  14:04:33  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:39:16  annie]
 * 
 * Revision 1.1.16.1  1994/02/04  20:16:02  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:45  devsrc]
 * 
 * Revision 1.1.14.1  1993/12/07  17:21:55  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:03:50  jaffe]
 * 
 * Revision 1.1.6.7  1993/01/21  18:25:17  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  16:46:29  zeliff]
 * 
 * Revision 1.1.6.6  1992/11/24  16:58:17  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  17:52:32  bolinger]
 * 
 * Revision 1.1.6.5  1992/11/18  19:42:24  jaffe
 * 	Transarc delta: jdp-ot5415-mount-aggregate-on-attach-epi 1.1
 * 	  Selected comments:
 * 	    This delta contains the Episode part of the changes necessary to have
 * 	    aggregates mounted at attach time.  There are corresponding deltas in both
 * 	    the DFS and test configurations.
 * 	    Use FTU_ATTACH_FLAGS_NOEXPORT in call to ftu_DetachAggr().
 * 	[1992/11/17  21:23:28  jaffe]
 * 
 * Revision 1.1.6.4  1992/10/27  20:53:39  jaffe
 * 	fixed merge conflicts
 * 	[1992/10/27  13:49:43  jaffe]
 * 
 * 	Transarc delta: jdp-ot5758-fix-epiunmount-on-osf1 1.1
 * 	  Selected comments:
 * 	    Corrected screwup that made epiunmount useless on OSF/1.
 * 	    Added additional pre-processor nonsense to do an unmount on OSF/1 r1.0.4
 * 	    and left the OSF/1 r1.1 case unimplemented.
 * 	[1992/10/27  13:11:13  jaffe]
 * 
 * Revision 1.1.6.3  1992/10/13  18:11:21  marty
 * 	OSC1.1.1 upgrade.  Changes are osc1.0.4 compatible
 * 	[1992/10/13  17:11:00  marty]
 * 
 * Revision 1.1.7.2  1992/09/30  21:11:19  garyf
 * 	fix arguments to OSF/1 mount
 * 	[1992/09/29  20:07:01  garyf]
 * 
 * Revision 1.1.6.2  1992/08/31  20:49:09  jaffe
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
 * 	    This is a checkpoint of the first working HP-UX Episode system.
 * 	    HP changes.
 * 	    Needed to add the vnops routines specific to HP-UX.
 * 	    Fix compilation problems.
 * 	[1992/08/30  03:03:57  jaffe]
 * 
 * Revision 1.1.3.9  1992/06/23  13:56:22  jdp
 * 	Transarc delta: bwl-ot4074-bad-attach-for-epimount 1.2
 * 	  Selected comments:
 * 	    Changes to support recovery in epimount.
 * 	    ftu_Detach now takes a new parameter.
 * 	    Call ftu_Detach for every fs, determining the device by calling statfs.
 * 	    No longer need -detach flag.
 * 	    Corrected compilation problems on OSF/1.
 * 	    Only include <sys/statfs.h> on AIX and add include of <dcedfs/sysincludes.h>.
 * 	[1992/06/22  22:14:10  jdp]
 * 
 * Revision 1.1.3.8  1992/06/01  19:18:11  jdp
 * 	Transarc delta: jdp-ot3338-add-recovery-to-epimount 1.5
 * 	  Files modified:
 * 	    utils: epimount.c, epiunmount.c
 * 	  Selected comments:
 * 	    Activate attach/detach code in epimount/epiunmount on OSF/1.
 * 	    Remove "#ifndef AFS_OSF_ENV" from around attach code.
 * 	[1992/06/01  16:40:21  jdp]
 * 
 * Revision 1.1.3.7  1992/05/22  20:07:07  garyf
 * 	remove OSF/1 conditional
 * 	[1992/05/22  20:02:41  garyf]
 * 
 * Revision 1.1.3.6  1992/05/19  18:51:04  jdp
 * 	Transarc delta: jdp-ot3338-add-recovery-to-epimount 1.4
 * 	  Files modified:
 * 	    utils: Makefile, epimount.c, epiunmount.c
 * 	  Selected comments:
 * 	    This delta will add aggregate recovery to the epimount command.  It will
 * 	    also cause aggregates to be attached on the first mount of a fileset within
 * 	    the aggregate.  This revision, however, is merely a checkpoint.
 * 	    More recovery in epimount changes.
 * 	    Add call to ftu_DetachAggr() on each unmount.
 * 	    More recovery changes.
 * 	    Add code to handle "-detach" flag.
 * 	    Corrected minor problem in epimount.
 * 	[1992/05/19  15:51:33  jdp]
 * 
 * Revision 1.1.3.5  1992/05/13  22:00:44  jdp
 * 	Transarc delta: cburnett-ot2492-clean_up_episode_test_dir 1.2
 * 	  Files modified:
 * 	    test: Makefile, epimount.c, epiunmount.c
 * 	    utils: Makefile, epimount.c, epiunmount.c
 * 	  Selected comments:
 * 	    [  epimount and epiumount should not be in the test dir]
 * 	    [  updated with changes OFS had been applying to test/epiunmount.c]
 * 	    [ problems encounterd with first close regarding posting]
 * 	[1992/05/13  21:31:59  jdp]
 * 
 * Revision 1.1.3.4  1992/05/05  16:39:13  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  23:31:07  jdp]
 * 
 * Revision 1.1.3.3  1992/01/24  03:01:29  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:11:11  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:45:23  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:30:56  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:14:48  devsrc
 * 	Fixed logs
 * 
 * Revision 1.1  1992/01/19  02:40:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
*/
/*
 * Episode unmount program
 *
 * This program unmounts all the Episode aggregates which are mounted at
 * the mount points specified on the command line. 
 */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_mount.h>
#include <dcedfs/cmdAux.h>

#define ARG_MOUNTPOINT_INDEX 0

void	PrintUsage 	_TAKES((int));
int 	unmountDriver 	_TAKES((struct cmd_syndesc *, char *));


static char*	programName;
int exitStat = 0;

void PrintUsage(int exstat)
{
    fprintf(stderr, "Usage:\n%s <mount_point>...\n", programName);
    exit(exstat);
}

int main(int argc, char* argv[])
{
    struct cmd_syndesc *ts;

    osi_setlocale(LC_ALL, "");
    programName = argv[0];
    exitStat = 0;

    ts = cmd_CreateSyntax((char *)0, unmountDriver, (char *)0, "epiunmount" );

    /* '-mountpoint' at offset 0 */
    cmd_AddParm(ts, "-mountpoint", CMD_LIST, CMD_REQUIRED, "mount_point");

    exit (cmd_Dispatch(argc, argv));
}

int unmountDriver(struct cmd_syndesc *as, char *arock)
{
    long 		code = 0;
    int			errors = 0;
    char		*mp;
    struct cmd_item 	*thisItemP;

    if (! cmd_IsParamPresent(as, ARG_MOUNTPOINT_INDEX)) {
	PrintUsage(1);
	/* NOTREACHED */
    }

    for (thisItemP = cmd_FirstItem(as, ARG_MOUNTPOINT_INDEX);
	 (thisItemP != (struct cmd_item *)NULL);
	 thisItemP = thisItemP->next) {

	mp = thisItemP->data;

	if (code = osi_umount(mp, 0)) {
	    (void)fprintf(stderr, "epiunmount: %s: %s\n", mp, strerror(code));
	    errors++;
	}
    }
    return (errors == 0 ? 0 : code);
}
