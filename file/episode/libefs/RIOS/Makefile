# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.82.1  1996/10/02  17:24:24  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:32:02  damon]
#
# Revision 1.1.77.3  1994/06/09  14:00:32  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:35:50  annie]
# 
# Revision 1.1.77.2  1994/02/04  20:13:21  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:10:23  devsrc]
# 
# Revision 1.1.75.1  1993/12/07  17:19:00  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  13:58:18  jaffe]
# 
# Revision 1.1.4.6  1993/01/19  15:26:49  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:47:09  cjd]
# 
# Revision 1.1.4.5  1993/01/18  16:16:58  jaffe
# 	remove usage of fp library
# 	[1993/01/18  16:14:45  jaffe]
# 
# Revision 1.1.4.4  1992/11/04  18:30:52  jaffe
# 	Transarc delta: jaffe-ot5852-install-cfglfs-in-ext-not-bin 1.1
# 	  Selected comments:
# 	    Install cfglfs in the ext directory like the other cfgXXX programs.
# 	[1992/11/04  17:29:36  jaffe]
# 
# Revision 1.1.4.3  1992/10/27  20:39:32  jaffe
# 	Transarc delta: comer-ot3013-dfs-zlc-file-handling-stubs 1.3
# 	  Selected comments:
# 	    Created a new package (library) to handle zero-linkcount files, and added
# 	    it to the dfs core extension under AIX.  There are calls to this package
# 	    from the xvolume package so these calls must be stubbed out for the
# 	    stand-alone episode extension.  The AIX libefs Makefile was, therefore,
# 	    changed to build and include ../../zlc/zlc_sepistubs.c in the stand-alone
# 	    extension.
# 	    For the PMAX, the "stand-alone" episode vmunix contains the dfs core and the
# 	    episode libraries.  For the PMAX, therefore, the normal zlc library is
# 	    included.
# 	    Build and link in ../../zlc/zlc_sepistubs.c
# 	    Fix compilation (link) problems with 1.52, specifically handle kzlc issues.
# 	    Due to merge problems, repeat last change.
# 	    (Probably should have opened a merge-* delta instead of reopening this one.
# 	    Sorry.)
# 	[1992/10/27  12:52:09  jaffe]
# 
# Revision 1.1.4.2  1992/08/31  19:37:15  jaffe
# 	Transarc delta: mason-ot4419-install-all-necessary-files 1.1
# 	  Selected comments:
# 	    We had a number of binaries which are necessary and/or useful for working with
# 	    DCE/LFS that were not being installed.
# 	    Begin installing cfglfs (for RIOS only)
# 	[1992/08/30  02:24:09  jaffe]
# 
# Revision 1.1.2.9  1992/07/02  18:00:58  jdp
# 	Transarc delta: jaffe-ot2785-dont-use-link-in-VPATH 1.1
# 	  Selected comments:
# 	    remove reference through link in the VPATH line to allow for multiple
# 	    backing trees.
# 	[1992/07/02  15:29:49  jdp]
# 
# Revision 1.1.2.8  1992/06/23  13:53:50  jdp
# 	Transarc delta: jdp-ot4137-lfs-turn-on-AIX-VM-integration 1.1
# 	  Selected comments:
# 	    Turn on VM integration on AIX.
# 	    Added -DAFS_AIX31_VM to CFLAGS.
# 	[1992/06/22  22:00:54  jdp]
# 
# Revision 1.1.2.7  1992/05/13  21:58:55  jdp
# 	Transarc delta: jdp-ot3264-fix-libefs-RIOS-makefile 1.1
# 	  Files modified:
# 	    libefs/RIOS: Makefile
# 	  Selected comments:
# 	    Fix up libefs/RIOS/Makefile.
# 	    Correct $VPATH, add includes for machdep.mk and $RULES_MK.
# 	[1992/05/13  21:07:57  jdp]
# 
# Revision 1.1.2.6  1992/05/12  18:37:05  jdp
# 	Corrected $VPATH.
# 	[1992/05/12  18:36:12  jdp]
# 
# Revision 1.1.2.2  1992/01/22  19:56:15  zeliff
# 	dce.77c file overlay
# 	[1992/01/22  18:25:18  zeliff]
# 
# $EndLog$

# Makefile to build DCE LFS extension which runs on dfscore.ext (dcelfs.ext)
# Also build a standalone version of DCE LFS sdcelfs.ext
# Build DCE LFS extension load command (cfglfs).

VPATH = ${TARGET_MACHINE}:../../export/${TARGET_MACHINE}:../../xvnode/${TARGET_MACHINE}:../../zlc:../../../../export/${target_machine}/usr/lib:../../libafs/${TARGET_MACHINE}

PROGRAMS		= cfglfs
OBJECTS			= xvfs_aix2vfs.o efsconfig.o sefsconfig.o zlc_sepistubs.o
cfglfs_OFILES		= cfgefs.o
OTHERS                  = efs.exp extras.exp export.exp dfscore.exp \
			  dcelfs.ext sdcelfs.ext

ILIST                   = dcelfs.ext cfglfs
IDIR                    = /ext/
dcelfs.ext_NOSTRIP	= -nostrip

dcelfs_ext_LIBS		= ${libkefsops.a:P} ${libkdir.a:P} ${libkanode.a:P}\
			  ${libklogbuf.a:P} ${libkeacl.a:P} ${libkasync.a:P}\
			  ${libktools.a:P}

sdcelfs_ext_LIBS	= ${libvolreg.a:P} ${libvolume.a:P} ${libkxcred.a:P}\
			  ${libkdfskutil.a:P} ${libkosi.a:P} ${libaggr.a:P}\
			  ${libkefsops.a:P} ${libkdir.a:P} ${libkanode.a:P}\
			  ${libklogbuf.a:P} ${libkeacl.a:P} ${libkasync.a:P}\
			  ${libaixexport.a:P} ${libkdacllfs.a:P} ${libktpq.a:P}\
			  ${libkdacl.a:P} ${libktools.a:P} ${libkicl.a:P} \
			  ${libktkm.a:P} ${libkbomb.a:P} ${libkafsutil.a:P}

dcelfs_LIBS             = -lkefsops -lkdir -lkanode -lklogbuf -lkeacl \
                          -lkasync -lktools  -laixexport

sdcelfs_LIBS            = -lvolreg -lvolume -lkxcred -lkdfskutil -lkosi -laggr \
			  -laixexport -lkdacl -lkefsops -lkdir -lkanode \
			  -lklogbuf -lkeacl -lkasync -lkdacllfs -lktools \
			  -lkicl -lktkm -lkbomb -lkafsutil -lktpq

cfglfs_LIBS		= -lafssys -ldce ${OS_LIBS}

dcelfs.ext_LIBFLAGS     = -L../../${TARGET_MACHINE}
sdcelfs.ext_LIBFLAGS    = -L../../${TARGET_MACHINE}

dcelfs.ext_LIBS         = efsconfig.o ${dcelfs_LIBS} -lcsys -lsys
sdcelfs.ext_LIBS        = sefsconfig.o ${sdcelfs_LIBS} -lcsys -lsys

dcelfs_ext_exps		= ${efs.exp:P} ${extras.exp:P} ${export.exp:P} \
			  ${dfscore.exp:P}

sdcelfs_ext_exps	= ${efs.exp:P} ${extras.exp:P} ${export.exp:P}

dcelfs.ext_LDFLAGS      = -H8 -eepi_config -bexport:${efs.exp:P} \
			  -bI:/lib/kernex.exp -bI:${extras.exp:P} \
			  -bI:/lib/syscalls.exp -bI:${export.exp:P} \
			  -bI:${dfscore.exp:P} -bR:dcelfs.ext.rmap \
			  -bloadmap:dcelfs.ext.ldmap

sdcelfs.ext_LDFLAGS     = -H8 -eepi_config -bexport:${efs.exp:P} \
			  -bI:/lib/kernex.exp -bI:${extras.exp:P} \
			  -bI:/lib/syscalls.exp -bI:${export.exp:P} \
			  -bR:sdcelfs.ext.rmap -bloadmap:sdcelfs.ext.ldmap

xvfs_aix2vfs.o_CFLAGS   = ${KERNELCFLAGS} -DEFS_SINGLE -DAFS_AIX31_VM
CFLAGS                  = ${KERNELCFLAGS} -DAFS_AIX31_VM
cfgefs.o_CFLAGS		= ${USERCFLAGS}

GARBAGE                 = efs.exp extras.exp export.exp \
			  sdcelfs.ext sdcelfs.ext.ldmap sdcelfs.ext.rmap \
			  dcelfs.ext dcelfs.ext.ldmap dcelfs.ext.rmap

dcelfs.ext: ${dcelfs_ext_LIBS} ${dcelfs_ext_exps} ${OBJECTS}
	${_LD_} -o $@ ${$@_LDFLAGS} `genpath ${$@_LIBFLAGS} ` \
        ${LIBDIRS} ${$@_LIBS}

sdcelfs.ext: ${sdcelfs_ext_LIBS} ${sdcelfs_ext_exps} ${OBJECTS}
	${_LD_} -o $@ ${$@_LDFLAGS} `genpath ${$@_LIBFLAGS} ` \
        ${LIBDIRS} ${$@_LIBS} xvfs_aix2vfs.o zlc_sepistubs.o

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(../${TARGET_MACHINE}/machdep.mk)
.include "../${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(../${TARGET_OS}/machdep.mk)
.include "../${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
