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
# Revision 1.1.23.1  1996/10/02  17:53:47  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:41:51  damon]
#
# Revision 1.1.18.2  1994/06/09  14:12:41  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:26:01  annie]
# 
# Revision 1.1.18.1  1994/02/04  20:22:11  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:14:41  devsrc]
# 
# Revision 1.1.16.1  1993/12/07  17:27:41  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:44:59  jaffe]
# 
# Revision 1.1.5.9  1993/01/19  16:06:47  cjd
# 	embedded copyright notice
# 	[1993/01/19  14:16:45  cjd]
# 
# Revision 1.1.5.8  1992/12/10  22:27:23  marty
# 	Upgrade DFS to OSF/1 1.1.1.  [OT defect 5236]
# 	[1992/12/10  21:58:21  marty]
# 
# Revision 1.1.5.7  1992/12/09  20:20:40  jaffe
# 	Transarc delta: kazar-ot6050-cleanup-xcred-locking-and-sayonara-fp 1.2
# 	  Selected comments:
# 	    get rid of free pool package, and fix a bunch of locking problems
# 	    in the xcred package
# 	    get rid of libkfp.a
# 	    forgot to remove from makefile link lines, and left volreg reference
# 	    remove from link line
# 	Transarc delta: kazar-ot6086-tkc-needs-token-revoke-race-fixes 1.4
# 	  Selected comments:
# 	    tkc also needs to handle token revoke races.  Same code as in
# 	    tkset, so move common code to tkm, and get everyone to use it.
# 	    Reorder the libraries so that the tkm_race code is found.
# 	[1992/12/04  16:46:50  jaffe]
# 	Revision 1.1.7.4  1992/12/09  23:30:27  marty
# 	change to MACHO format for OSF 1.1.1
# 
# Revision 1.1.5.6  1992/11/18  20:35:11  jaffe
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
# 	    Add libagfs to the appropriate lists.
# 	    Remove leftover junk from $OFILES.
# 	    Remove spurious "agfs_config.o" from $OFILES.
# 	    Rearrange things slightly to make use of a vendor-supplied definition
# 	    of MOUNT_AGFS.
# 	    Fix bogus linker warnings about shared libraries when compiling .o files.
# 	[1992/11/17  21:54:55  jaffe]
# 
# Revision 1.1.5.5  1992/10/27  21:14:44  jaffe
# 	Transarc delta: comer-3013-zlc-file-handling 1.13
# 	  Selected comments:
# 	    In order to handle the ability to continue to read or write and open
# 	    file after that file has been unlinked, is addressed by open-for-delete
# 	    (OFD) tokens.  When a file is unlinked, the file exporter or glue code
# 	    tries to get this OFD token; but, the token is not granted until all
# 	    other open tokens have been returned or have expired.
# 	    This scheme has to be modified to work with fileset moves or a moved
# 	    zero-linkcount (deleted) file's storage will be recovered prematurely.
# 	    This delta contains the implementation for a zero-linkcount (ZLC) file
# 	    manager that sits in the core extension and manages ZLC files with its
# 	    own thread and as its own host object.
# 	    Fixed a problem with the tkc code where the cache entry reference
# 	    count was not being decremented (tkc_Release wasn't being called),
# 	    so vnodes were never being released (and storage not reclaimed).
# 	    1) Replaced a call to volreg_Lookup with one to VOL_VGET to fix a
# 	    deadlock problem.  The volreg_Lookup was unnecessary anyway.
# 	    2) Changed code to no longer try and reset SOURCE and TARGET flags
# 	    in volume structure.
# 	    3) Keep track of the number of items on the ZLC list.
# 	    4) Call the ZLC clean function when destroying a volume.
# 	    Was using the wrong fid when doing a VGET.
# 	    Was calling volreg_LookupExtended with an xcode = 1.  How foolish of
# 	    me!  xcode should be set to 2.
# 	    Fixed another fid problem and fixed a problem with returning bogus
# 	    tokens.
# 	    The fid passed from VOLOP_SETATTR did not contain a volume or
# 	    cell.  This is wrong....
# 	    In the event an asyncronous grant was granted for a file no longer in the
# 	    zlc list, an incorrect value was being returned to the token manager.
# 	    Since 1) the volume package makes calls to the zlc package, 2) stand-alone
# 	    episode includes the volume package, 3) stand-alone episode doesn't
# 	    require the functionality from this package, and 4) stand-alone episode
# 	    cannot link sdcelfs.ext because of references to zlc symbols, a stubs file
# 	    is created here.  The file is actually compiled from the libefs/<system>
# 	    Makefiles.
# 	    Added kzlc to this file since file/OSF1/Makefile is no longer used.
# 	    Need to hook zlc into build process.
# 	    link problems
# 	[1992/10/27  14:32:55  jaffe]
# 
# Revision 1.1.5.4  1992/09/29  13:48:07  delgado
# 	Add back some things which got clobbered during recent submissions; i.e.
# 	dependencies for afsvmunix and efsvmunix
# 	[1992/09/29  13:47:20  delgado]
# 
# Revision 1.1.5.3  1992/09/15  13:14:51  jaffe
# 	Transarc delta: kazar-ot4921-add-in-core-logging-package 1.5
# 	  Selected comments:
# 	    Start coding in-core logging package.  Not ready for export yet,
# 	    as mostly untested code.
# 	    link in new ICL package for OS/1
# 	    added missing file and cleaned up Makefile
# 	[1992/09/14  20:09:32  jaffe]
# 
# Revision 1.1.5.2  1992/08/31  20:06:13  jaffe
# 	Transarc delta: mason-put-tkm_control.c-defines-in 1.2
# 	  Selected comments:
# 	    Resolve a merge problem in tkm.
# 	    Even more minor changes.
# 	    Must link against libktpq.a
# 	[1992/08/30  02:53:48  jaffe]
# 
# Revision 1.1.2.7  1992/07/10  12:33:32  rsalz
# 	Fixed names of RPC test libraries.
# 	[1992/07/01  17:52:13  rsalz]
# 
# Revision 1.1.2.6  1992/06/15  14:58:12  garyf
# 	add dependency on libraries
# 	[1992/06/15  14:57:27  garyf]
# 
# Revision 1.1.2.5  1992/06/03  21:39:13  garyf
# 	afsvmunix->dfsvmunix
# 	[1992/06/03  21:38:56  garyf]
# 
# Revision 1.1.2.4  1992/05/28  20:38:48  toml
# 	Add px_config.o to libafs, rearrange libraries in afs_libs
# 
# Revision 1.1.2.3  1992/05/22  20:46:25  garyf
# 	move kernel links here to paralllel AIX version
# 	[1992/05/22  03:10:46  garyf]
# 
# Revision 1.1.2.2  1992/04/07  15:47:18  garyf
# 	initial version
# 	[1992/04/07  15:43:13  garyf]
# 
# $EndLog$

VPATH			= ${TARGET_OS}:${MAKETOP}../export/${target_machine}/usr/lib

LIBRARIES		= libafs.a

KERNEL_CC		=

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}
CFLAGS			= ${KERNELCFLAGS}
OFILES			= cm_config.o px_config.o

OTHERS		= dfsvmunix efsvmunix

IDIR		= /

ILIST		= dfsvmunix efsvmunix

CONFIG		= ${KERNEL_${TARGET_MACHINE}_CONFIG:U${KERNEL_CONFIG:UDCE}}

afs_libs	= -lcm -lkdfskutil -lafs -laggr -lcm -lkdfsncs -lkdfskutil \
		-lpx -lhost -lkzlc -lktkset -lkxcred -lkrepcli \
		-lkosi -lkcommondata -lkicl -lkfldb -lkafs4srv \
		-lkafs4clt -lufsops -lxvnode -lagfs \
		-lktkc -lktkm -lktpq -lvolreg -lvolume -lkdacl -lkdacllfs \
		-lkolddacl 

afs_libs_dep	= libcm.a libkdfskutil.a libafs.a libaggr.a libkdfsncs.a \
		libpx.a libhost.a libktkm.a libktpq.a libktkset.a \
		libkxcred.a libkrepcli.a libkcommondata.a \
		libkosi.a libvolreg.a libkfldb.a libkafs4srv.a \
		libkafs4clt.a libufsops.a libxvnode.a \
		libagfs.a libktkc.a libvolume.a libkdacl.a libkdacllfs.a \
		libkolddacl.a

rpc_test_libs   =
# Uncomment this line to link KRPC test driver into kernel.
# (this requires a different version of conf.c )
# rpc_test_libs	= -lkncsdev   -lkncstest  -lkv2test

rpc_libs 	= ${rpc_test_libs} -lknck -lkidl -lkdes

rpc_libs_dep	= libknck.a libkidl.a libkdes.a


efs_libs 	= -lkefsops -lkdir -lkeacl -lkanode \
		-lklogbuf -lkasync -lkdacl -lkdacllfs -lktools -lkolddacl \
		-lefs

efs_libs_dep	= libkefsops.a libkdir.a libkeacl.a libkanode.a \
		libklogbuf.a libkasync.a libkdacllfs.a libktools.a \
		libkolddacl.a libefs.a


afs_repeats	= -lkosi -lkdacl


dfsvmunix: ${MAKETOP}/${OBJECTDIR}/kernel/${CONFIG}/vmunix.sys  ${KERNEL_LDDEP} ${afs_libs_dep} ${rpc_libs_dep}
	${MACHO_GCC_EXEC_PREFIX}ld ${KERNEL_LDOPTS}  -e start -o dfsvmunix ${LIBDIRS} \
	${${MAKETOP}/${OBJECTDIR}/kernel/${CONFIG}/vmunix.sys:P}  ${KERNEL_LDDEP} ${afs_libs} -lepidum ${afs_repeats} ${rpc_libs}

efsvmunix: ${MAKETOP}/${OBJECTDIR}/kernel/${CONFIG}/vmunix.sys  ${KERNEL_LDDEP} ${afs_libs_dep} ${rpc_libs_dep} ${efs_libs_dep}
	${MACHO_GCC_EXEC_PREFIX}ld ${KERNEL_LDOPTS}  -e start -o efsvmunix ${LIBDIRS} \
	${${MAKETOP}/${OBJECTDIR}/kernel/${CONFIG}/vmunix.sys:P} ${KERNEL_LDDEP} ${afs_libs} ${efs_libs} ${afs_repeats} ${rpc_libs} 

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists (${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif

.include <${RULES_MK}>
