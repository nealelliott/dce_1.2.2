#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.122.1  1996/10/02  17:42:19  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:34:57  damon]
#
# Revision 1.1.117.3  1994/06/09  14:04:26  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:39:09  annie]
# 
# Revision 1.1.117.2  1994/02/04  20:15:50  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:11:36  devsrc]
# 
# Revision 1.1.11.1  1993/12/07  17:21:46  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:02:15  jaffe]
# 
# Revision 1.1.4.6  1993/01/21  18:25:02  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  16:45:47  zeliff]
# 
# Revision 1.1.4.5  1992/12/17  23:07:13  rsarbo
# 	Don't build epidaemon on PMAX
# 	[1992/12/17  23:05:35  rsarbo]
# 
# Revision 1.1.4.4  1992/11/18  19:42:14  jaffe
# 	Transarc delta: jdp-ot5415-mount-aggregate-on-attach 1.4
# 	  Selected comments:
# 	    This delta contains the bulk of the changes necessary to have aggregates
# 	    mounted at attach time.  The purpose of this feature is to provide a
# 	    means by which exported filesets (with no local mount points) will be
# 	    sync'd periodically.  To accomplish this, we have created a new VFS type
# 	    (agfs).  Whenever an aggregate is exported, a local mount of the aggregate
# 	    is also done; then, when the sync daemon does its thing, the aggregate's sync
# 	    vfsop will be called.  The agfs sync then calls the new sync aggrop, which
# 	    in the case of Episode, calls efs_sync().  The UFS sync aggrop doen't do
# 	    anything, since we know the UFS filesystem is mounted, and so, its sync vfsop
# 	    will get called anyway.
# 	    This delta must be bundled with the corresponding deltas in the Episode and
# 	    test configurations.
# 	    Remove leftover junk from $OFILES.
# 	    Rearrange things slightly to make use of a vendor-supplied definition
# 	    of MOUNT_AGFS.
# 	    Add "-losi" to lists of libs for epimount and epiunmount.
# 	    Fix bogus linker warnings about shared libraries when compiling .o files.
# 	[1992/11/17  21:22:17  jaffe]
# 
# Revision 1.1.4.3  1992/09/15  17:41:59  jaffe
# 	Transarc delta: bwl-ot4959-user-tests-on-osf1 1.4
# 	  Selected comments:
# 	    Get test_vnodeops to run under OSF/1.
# 	    This runs, but using dummy lwp.  So it doesn't support multi threading.
# 	    This is not so important for test_vnodeops (all we miss is testing of
# 	    asynchronous I/O) but critical for test_anode.  So we are not yet ready
# 	    for prime time.
# 	    Bug fix (failing to return a value from a dummy function).
# 	    Convert to Bob Sidebotham's new LWP, which is based on pthreads and hence
# 	    can use CMA.  This allows us to deleted platform-specific process.s files,
# 	    and makes us completely portable.  Also, deleted various extraneous files
# 	    left over from earlier versions of LWP.
# 	    any program which uses lwp now must link with -ldce
# 	    make default libs link with libdce.a
# 	Transarc delta: jdp-ftutils 1.11
# 	  Selected comments:
# 	    Converted newft.c to use ftu_AttachAggr() for testing before adding
# 	    ftu_AttachAggr() to epimount.
# 	    Fixed bug in newft.c.
# 	    Checkpoint to upgrade to a new release.
# 	    Checkpoint to work on another delta.
# 	    Checkpoint to fix vol_efsSwapIDs() (see OT #4671).
# 	    Another checkpoint.
# 	    Rearrange link lines in utils/Makefile.
# 	    Add -lcom_err and -ldce to the link lines for epimount and epiunmount.
# 	    Fix compilation problems on OSF/1.
# 	    Move "-lftutil" before "-lafssys" in epi{,un}mount_LIBS.
# 	    Add lsquota command to efts.
# 	Transarc delta: mason-add-hp800-osi-routines 1.15
# 	  Selected comments:
# 	    Need to checkpoint to fix a different bug.
# 	    Checkpoint the first system where test_anode works.  Only one change is not
# 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
# 	    is -lBSD.  This should be fixed more generally.
# 	    More HP-UX changes.
# 	    More HP-UX related changes.
# 	    Fix compile problem on the RIOS.
# 	    Fix more compilation problems.
# 	    Checkpoint.
# 	    Checpoint.
# 	    Don't link against unnecessary libraries
# 	    In this snap we turned all malloc() calls into osi_Alloc calls.
# 	    Also turned free calls into osi_Free.
# 	    HP-UX EOF detection fix.
# 	    Another checkpoint.
# 	    This is a checkpoint of the first working HP-UX Episode system.
# 	    HP changes
# 	    Needed to add the vnops routines specific to HP-UX.
# 	    Fix compilation problems.
# 	[1992/09/15  11:43:16  jaffe]
# 
# Revision 1.1.4.2  1992/08/31  20:46:57  jaffe
# 	Transarc delta: jdp-ftutils 1.11
# 	  Selected comments:
# 	    Checkpoint to upgrade to a new release.
# 	    Checkpoint to work on another delta.
# 	    Checkpoint to fix vol_efsSwapIDs() (see OT #4671).
# 	    Another checkpoint.
# 	    Rearrange link lines in utils/Makefile.
# 	    Add -lcom_err and -ldce to the link lines for epimount and epiunmount.
# 	    Fix compilation problems on OSF/1.
# 	    Move "-lftutil" before "-lafssys" in epi{,un}mount_LIBS.
# 	    Add lsquota command to efts.
# 	Transarc delta: mason-add-hp800-osi-routines 1.15
# 	  Selected comments:
# 	    This delta is misnamed - these are really porting fixes.
# 	    Need to checkpoint to fix a different bug.
# 	    Checkpoint the first system where test_anode works.  Only one change is not
# 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
# 	    is -lBSD.  This should be fixed more generally.
# 	    More HP-UX changes.
# 	    More HP-UX related changes.
# 	    Fix compile problem on the RIOS.
# 	    Fix more compilation problems.
# 	    Checkpoint.
# 	    Checpoint.
# 	    Don't link against unnecessary libraries
# 	    In this snap we turned all malloc() calls into osi_Alloc calls.
# 	    Also turned free calls into osi_Free.
# 	    HP-UX EOF detection fix.
# 	    Another checkpoint.
# 	    This is a checkpoint of the first working HP-UX Episode system.
# 	    HP changes
# 	    Needed to add the vnops routines specific to HP-UX.
# 	    Fix compilation problems.
# 	Transarc delta: mason-ot4419-install-all-necessary-files 1.1
# 	  Selected comments:
# 	    We had a number of binaries which are necessary and/or useful for working with
# 	    DCE/LFS that were not being installed.
# 	    Install epiinit and epiunmount.
# 	[1992/08/30  02:59:09  jaffe]
# 
# Revision 1.1.2.7  1992/07/15  18:50:00  rsarbo
# 	install epiinit and epiunmount
# 	[1992/07/14  20:23:01  rsarbo]
# 
# Revision 1.1.2.6  1992/07/02  18:02:43  jdp
# 	Transarc delta: jdp-ot4353-remove-recovery 1.1
# 	  Selected comments:
# 	    This delta matches the DFS delta of the same name.  It undoes some of
# 	    the changes that were made to add userspace recovery to epimount and
# 	    efts.  In particular, it is no longer necessary to link those two commands
# 	    with as many libraries.
# 	    Reduce number of libraries linked into epi(un)mount.
# 	[1992/07/02  15:35:55  jdp]
# 
# Revision 1.1.2.5  1992/06/23  13:56:09  jdp
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    User space binaries were being linked against -lbsd.  This library is
# 	    not present on all machine types.  Fixed this by adding a BSD_LIBS in
# 	    the machdep.mk file and using that instead.  Now, each architecture
# 	    can define its own set of BSD-compatibility libraries. (This is a
# 	    portability fix).
# 	    Use BSD_LIBS instead of -lbsd
# 	[1992/06/22  22:13:13  jdp]
# 
# Revision 1.1.2.4  1992/05/19  18:50:54  jdp
# 	Transarc delta: jdp-ot3338-add-recovery-to-epimount 1.4
# 	  Files modified:
# 	    utils: Makefile, epimount.c, epiunmount.c
# 	  Selected comments:
# 	    This delta will add aggregate recovery to the epimount command.  It will
# 	    also cause aggregates to be attached on the first mount of a fileset within
# 	    the aggregate.  This revision, however, is merely a checkpoint.
# 	    More recovery in epimount changes.
# 	    Link epimount and epiunmount with the proper libraries.
# 	    More recovery changes.
# 	    Corrected minor problem in epimount.
# 	[1992/05/19  15:48:14  jdp]
# 
# Revision 1.1.2.3  1992/05/13  22:00:38  jdp
# 	Transarc delta: cburnett-ot2492-clean_up_episode_test_dir 1.2
# 	  Files modified:
# 	    test: Makefile, epimount.c, epiunmount.c
# 	    utils: Makefile, epimount.c, epiunmount.c
# 	  Selected comments:
# 	    [  epimount and epiumount should not be in the test dir]
# 	    [  updated to install epidaemon]
# 	    [ problems encounterd with first close regarding posting]
# 	[1992/05/13  21:30:04  jdp]
# 
# Revision 1.1.2.2  1992/05/05  16:38:02  jdp
# 	delta jdp-epimerge-use-ode-makefiles 1.5
# 	delta mason-new-recoverlog 1.2
# 	[1992/05/04  23:27:33  jdp]
# 
# Revision 1.1  1992/01/19  02:40:32  devrcs
# 	Initial revision
# 
# $EndLog$
#

.if ${TARGET_OS}=="SUNOS5"
SYS_SPECIFIC=mount_agfs umount_agfs fsck_agfs mount_lfs
mount_agfs_LIBS		=
mount_agfs_OFILES	= mount_agfs.o mntutil.o

umount_agfs_LIBS	=
umount_agfs_OFILES	= umount_agfs.o mntutil.o

fsck_agfs_LIBS		= -lasync -lanode -ledsk -llogbuf -lbomb -ltools \
			  -llwp -ldfstab -lftutil -lafsutil -lncompat \
			  -licl -lafssys -losi -ldce ${OS_LIBS}

fsck_agfs_OFILES	= fsck_agfs.o mntutil.o

mount_lfs_LIBS		=
mount_lfs_OFILES	= mount_lfs.o mntutil.o
.endif

.if ${TARGET_MACHINE}=="RIOS"
SYS_SPECIFIC=epidaemon
.endif

.if ${TARGET_MACHINE}=="OSF1"
SYS_SPECIFIC=epiinit
.endif

.if ${TARGET_MACHINE}=="HP800"
SYS_SPECIFIC=epiinit
.endif

EXPLIB_TARGETS		= ${LIBRARIES}
PROGRAMS		= epimount epiunmount ${SYS_SPECIFIC}

LIBFLAGS		= -L./
LIBS			= -lafssys -ldce ${OS_LIBS}
epimount_LIBS		= -lftutil -lcmd -lafssys -licl -lncompat -losi \
			  -lcom_err -ldce ${OS_LIBS}
epiunmount_LIBS		= -lftutil -lcmd -lafssys -licl -losi -lcom_err \
			  -ldce ${OS_LIBS}

CFLAGS			= ${USERCFLAGS}

ILIST			= epimount epiunmount ${SYS_SPECIFIC}
IDIR			= /bin/

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.if exists(../${TARGET_MACHINE}/machdep.mk)
.include "../${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(../${TARGET_OS}/machdep.mk)
.include "../${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
