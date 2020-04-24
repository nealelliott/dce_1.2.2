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
# Revision 1.1.89.1  1996/10/02  17:53:53  damon
# 	New DFS from Transarc
# 	[1996/10/01  18:58:30  damon]
#
# Revision 1.1.84.3  1994/08/10  14:23:54  rsarbo
# 	add -lksec to link line for dfscmfx.ext  (delegation support)
# 	[1994/08/10  14:22:24  rsarbo]
# 
# Revision 1.1.84.2  1994/06/09  14:12:46  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:26:05  annie]
# 
# Revision 1.1.84.1  1994/02/04  20:22:16  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:14:43  devsrc]
# 
# Revision 1.1.82.1  1993/12/07  17:27:45  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:45:53  jaffe]
# 
# Revision 1.1.4.9  1993/01/19  16:07:00  cjd
# 	embedded copyright notice
# 	[1993/01/19  14:16:59  cjd]
# 
# Revision 1.1.4.8  1993/01/14  21:25:07  marcyw
# 	Added RM before CP
# 	[1993/01/14  21:21:46  marcyw]
# 
# Revision 1.1.4.7  1992/12/09  20:20:46  jaffe
# 	Transarc delta: kazar-ot6050-cleanup-xcred-locking-and-sayonara-fp 1.2
# 	  Selected comments:
# 	    get rid of free pool package, and fix a bunch of locking problems
# 	    in the xcred package
# 	    get rid of libkfp.a
# 	    forgot to remove from makefile link lines, and left volreg reference
# 	    remove libkfp.a from link line
# 	[1992/12/04  16:47:23  jaffe]
# 
# Revision 1.1.4.6  1992/12/03  21:49:06  delgado
# 	Changes for new dfsbind and krpc helper
# 	[1992/12/03  21:48:35  delgado]
# 
# Revision 1.1.4.5  1992/11/18  20:35:33  jaffe
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
# 	    Add libagfs to the approprate lists.
# 	    Remove leftover junk from $OFILES.
# 	    Rearrange things slightly to make use of a vendor-supplied definition
# 	    of MOUNT_AGFS.
# 	    Fix bogus linker warnings about shared libraries when compiling .o files.
# 	[1992/11/17  21:55:59  jaffe]
# 
# Revision 1.1.4.4  1992/10/27  21:14:47  jaffe
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
# 	    Added libkzcl.a to core extension.
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
# 	    Need to hook zlc into build process.
# 	    link problems
# 	[1992/10/27  14:33:19  jaffe]
# 
# Revision 1.1.4.3  1992/09/15  13:14:54  jaffe
# 	Transarc delta: bab-ot2604-tkm-gc-impl 1.27
# 	Transarc delta: bab-ot2978-tkm-multi-thread-tkm_test 1.16
# 	Transarc delta: comer-ot3947-multithread-cm-daemon 1.25
# 	Transarc delta: kazar-ot4921-add-in-core-logging-package 1.5
# 	[1992/09/14  20:09:52  jaffe]
# 
# Revision 1.1.4.2  1992/08/31  20:06:19  jaffe
# 	Transarc delta: bab-ot2604-tkm-gc-impl 1.27
# 	Transarc delta: bab-ot2978-tkm-multi-thread-tkm_test 1.16
# 	Transarc delta: comer-ot3947-multithread-cm-daemon 1.25
# 	[1992/08/30  02:56:20  jaffe]
# 
# Revision 1.1.2.4  1992/05/20  19:56:33  mason
# 	Transarc delta: bab-ot2978-tkm-multi-thread-tkm_test 1.14
# 	  Files modified:
# 	    OSF1: Makefile; libafs/RIOS: Makefile; tkc: tkc.c, tkc.h
# 	    tkc.ulib: Makefile
# 	    tkm: Makefile, tkm_blockingOps.c, tkm_fidHash.c
# 	    tkm: tkm_getToken.c, tkm_hostData.h, tkm_hostList.h
# 	    tkm: tkm_hostStubs.c, tkm_hostStubs.h, tkm_test.h
# 	    tkm: tkm_test.l, tkm_test.y, tkm_testops.c, tkm_tkcHost.c
# 	    tkm: tkm_tkcsetHost.c, tkm_tkmHost.c, tkm_tksetHost.c
# 	    tkm: tkm_tokens.c, tkm_tokens_private.h
# 	    tkset: Makefile, tkset.h; tkset.ulib: Makefile
# 	    token_test: Makefile, tktest_hostStubs.c, tktest_hostStubs.h
# 	    token_test: tktest_test.h, tktest_test.l, tktest_test.y
# 	    token_test: tktest_testops.c, tktest_tkcsetHost.c
# 	    token_test: tktest_tkmHost.c
# 	  Selected comments:
# 	    This delta implements the use of the thread-per-host model in tkm_test.
# 	    This first version is a check-point that just implements the use of
# 	    host operation structures (to be used as work queue elements).
# 	    This version of this delta has the real per-host threads and work queues
# 	    in it.  This is another check-point before restoring the use of
# 	    the hostops vectors in the testing code.
# 	    Hostops vectors now used in user-space test.  Added more support
# 	    for tkset/tkc testing.
# 	    Checkpoint before importing the real tkc/tkset code.
# 	    Pick up changes to tkm_test.y that hadn't been saved.
# 	    Buildable check-point that includes tkm-host host auditing.
# 	    Changed name of kernel-space tkset library.
# 	    last check-point before pulling tkm_test out of tkm directory.
# 	    This one seems to be passing all the basic tkm/tkc/tkset tests.
# 	    Moved testing program into token_test directory.
# 	    Fix bug in tkc host handling.
# 	    Add support for host yield operation.
# 	    Support for printing strings during execution and attempt to get
# 	    thread numbers.
# 	    Compensate for error in build 11 version of pthread.h
# 	    tkc change to get desired interleaving in user-space tests.
# 	    Locate the fix for pthread_getunique_np in a place where more
# 	    of the test program can use it, and protected with ifdefs for RIOS.
# 	[1992/05/20  11:39:32  mason]
# 
# Revision 1.1.2.3  1992/04/21  13:40:29  mason
# 	Transarc delta: cburnett-ot2478-fix_libafs_RIOS_Makefile 1.1
# 	  Files modified:
# 	    libafs/RIOS: Makefile
# 	  Selected comments:
# 	    [ chg libcommondata to libkcommondata]
# 	    [ libcommondata to libkcommondata]
# 	[1992/04/20  22:38:31  mason]
# 	Revision 1.1.2.2  1992/04/02  13:09:28  mason
# 	Changed VPATH line to allow multiple backing sandbox model to work correctly
# 	as per scheme worked out with Mark Hickey.
# 	[1992/04/02  13:06:21  mason]
# 
# Revision 1.1  1992/01/19  02:54:55  devrcs
# 	Initial revision
# 
# $EndLog$
#
#VPATH2!=echo ${LIBDIRS:@CURR@${CURR:S/-L//}@} | sed -e 's/ /:/g'
VPATH = ${TARGET_MACHINE}:../export/${TARGET_MACHINE}:../../../export/${target_machine}/usr/lib

PROGRAMS		= cfgdfs
OTHERS                  = extras.exp export.exp dfscore.ext \
			  afs.exp dfscore.exp dfscmfx.ext dfscmfx.exp

ILIST                   = cfgdfs dfscore.ext dfscmfx.ext
IDIR                    = /ext/
dfscore.ext_NOSTRIP	= -nostrip
dfscmfx.ext_NOSTRIP	= -nostrip

dfscore_ext_LIBS	= ${libvolreg.a:P} ${libvolume.a:P} ${libkxcred.a:P} \
			  ${libkdfskutil.a:P} ${libkosi.a:P} \
			  ${libkcommondata.a:P} \
			  ${libaixexport.a:P} ${libxvnode.a:P} ${libagfs.a:P} \
			  ${libktkc.a:P} ${libktkm.a:P} ${libaggr.a:P} \
			  ${libkolddacl.a:P} ${libufsops.a:P} \
			  ${libkdacl.a:P} ${libkdacllfs.a:P} \
			  ${libktpq.a:P} ${libkzlc.a:P} ${libkicl.a:P} \
			  ${libkbomb.a:P} ${libkafsutil.a:P}

dfscmfx_ext_LIBS	= ${libcm.a:P} ${libpx.a:P} \
			  ${libkfldb.a:P} \
			  ${libhost.a:P} ${libktkset.a:P} \
			  ${libknck.a:P} ${libkafs4srv.a:P} \
			  ${libkafs4clt.a:P} ${libkrepcli.a:P} ${libkidl.a:P} \
			  ${libkdes.a:P} ${libkdfsncs.a:P} ${libksec.a:P}

GARBAGE                 = dfscore.ext dfscore.ext.ldmap dfscmfx.ext \
			  dfscmfx.ext.ldmap config.o dfscorecfg.o

dfscore_LIBS            = -lvolreg -lvolume -lkxcred -lkdfskutil -lkosi \
			  -lkcommondata \
			  -lkicl -laixexport -lxvnode -lagfs -lktkc -lktkm \
			  -laggr -lkolddacl -lufsops -lkdacl -lkdacllfs -lktpq\
			  -lkzlc -lkbomb -lkafsutil

dfscmfx_LIBS            = -lpx -lcm -lkfldb -lhost -lktkset \
			  -lkafs4srv -lkafs4clt \
			  -lkrepcli -lkdfsncs -lknck -lkidl -lkdes \
			  -lksec

cfgafs.o_CFLAGS		= ${USERCFLAGS}
cfgdfs_OFILES		= cfgafs.o
cfgdfs_LIBS		= -lafssys -lcfg -lodm

#
# Build the DFS kernel extensions
# dfscore.ext is core functions used by both DFS and DCE-LFS
# dfscmfx.ext is DFS client and server plus supporting function. dfscmfx.ext
# runs on top of dfscore.ext which must be loaded first.
#
config.o_CFLAGS		 = ${KERNELCFLAGS}
dfscorecfg.o_CFLAGS	 = ${KERNELCFLAGS}
dfscore.ext_LIBFLAGS     = -L../${TARGET_MACHINE}
dfscmfx.ext_LIBFLAGS     = -L../${TARGET_MACHINE}

dfscore.ext_LIBS         = dfscorecfg.o ${dfscore_LIBS} -lcsys -lsys
dfscmfx.ext_LIBS         = config.o ${dfscmfx_LIBS} -lcsys -lsys

dfscore.ext_LDFLAGS 	= -H8 -edfs_core_config \
			  -bexport:dfscore.exp -bexport:afs.exp \
			  -bI:/lib/kernex.exp \
                          -bI:extras.exp -bI:/lib/syscalls.exp \
                          -bI:export.exp -bloadmap:dfscore.ext.ldmap

dfscmfx.ext_LDFLAGS 	= -H8 -eafs_config -bI:/lib/kernex.exp \
			  -bexport:dfscmfx.exp \
                          -bI:extras.exp -bI:/lib/syscalls.exp \
                          -bI:export.exp -bI:dfscore.exp -bI:afs.exp\
			  -bloadmap:dfscmfx.ext.ldmap

dfscore.ext: ${dfscore_ext_LIBS} dfscore.exp extras.exp afs.exp \
    export.exp dfscorecfg.o
	-${RM} -f ./dfscore.exp
	-${CP} ${dfscore.exp:P} .
	-${RM} -f ./extras.exp
	-${CP} ${extras.exp:P} .
	-${RM} -f ./export.exp
	-${CP} ${export.exp:P} .
	-${RM} -f ./afs.exp
	-${CP} ${afs.exp:P} .
	${_LD_} -o $@ ${$@_LDFLAGS} `genpath ${$@_LIBFLAGS} ` \
                ${LIBDIRS} ${$@_LIBS}
	${RM} -f  dfscore.exp
	${RM} -f extras.exp
	${RM} -f export.exp
	${RM} -f afs.exp

dfscmfx.ext: ${dfscmfx_ext_LIBS} dfscore.exp extras.exp afs.exp dfscmfx.exp \
    export.exp config.o
	-${RM} -f ./dfscmfx.exp
	-${CP} ${dfscmfx.exp:P} .
	-${RM} -f ./dfscore.exp
	-${CP} ${dfscore.exp:P} .
	-${RM} -f ./extras.exp
	-${CP} ${extras.exp:P} .
	-${RM} -f ./export.exp
	-${CP} ${export.exp:P} .
	-${RM} -f ./afs.exp
	-${CP} ${afs.exp:P} .
	${_LD_} -o $@ ${$@_LDFLAGS} `genpath ${$@_LIBFLAGS} ` \
                ${LIBDIRS} ${$@_LIBS}
	${RM} -f  dfscmfx.exp
	${RM} -f  dfscore.exp
	${RM} -f extras.exp
	${RM} -f export.exp
	${RM} -f afs.exp
.include <osf.debugvar.mk>

