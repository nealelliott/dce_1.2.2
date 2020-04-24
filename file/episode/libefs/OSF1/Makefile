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
# Revision 1.1.21.1  1996/10/02  17:24:17  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:31:56  damon]
#
# Revision 1.1.16.2  1994/06/09  14:00:29  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:35:44  annie]
# 
# Revision 1.1.16.1  1994/02/04  20:13:19  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:10:22  devsrc]
# 
# Revision 1.1.14.1  1993/12/07  17:18:58  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  13:57:46  jaffe]
# 
# Revision 1.1.5.9  1993/01/19  15:26:40  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:46:50  cjd]
# 
# Revision 1.1.5.8  1993/01/18  16:16:54  jaffe
# 	remove usage of fp library.
# 	[1993/01/18  16:14:28  jaffe]
# 
# Revision 1.1.5.7  1992/12/10  22:26:53  marty
# 	Upgrade DFS to OSF/1 1.1.1.  [OT defect 5236]
# 	[1992/12/10  21:56:29  marty]
# 
# Revision 1.1.5.6  1992/12/09  19:49:30  jaffe
# 	Transarc delta: kazar-ot6086-tkc-needs-token-revoke-race-fixes 1.4
# 	  Selected comments:
# 	    tkc also needs to handle token revoke races.  Same code as in
# 	    tkset, so move common code to tkm, and get everyone to use it.
# 	    Reorder the libraries so that the tkm_race code is found.
# 	[1992/12/03  21:43:09  jaffe]
# 	Revision 1.1.7.4  1992/12/09  23:40:54  marty
# 	change to MACHO format
# 
# Revision 1.1.5.5  1992/10/27  20:39:30  jaffe
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
# 	    Include libkzlc.a.
# 	    Fix compilation (link) problems with 1.52, specifically handle kzlc issues.
# 	    Use -lkzlc (not -lzlc), and make sure it's in the right spot.
# 	    Due to merge problems, repeat last change.
# 	    (Probably should have opened a merge-* delta instead of reopening this one.
# 	    Sorry.)
# 	    Use -lkzlc (not -lzlc).
# 	[1992/10/27  12:51:45  jaffe]
# 
# Revision 1.1.5.4  1992/10/05  19:57:46  toml
# 	Add libkidl and libkdes to libraries for sefsvmunix.
# 	[1992/10/05  19:57:24  toml]
# 
# Revision 1.1.5.3  1992/09/15  17:36:16  jaffe
# 	Transarc delta: kazar-ot4921-add-in-core-logging-package 1.1
# 	  Selected comments:
# 	    got to add kicl to the standalone episode kernel on OSF/1.
# 	    add kicl to the libs list.
# 	[1992/09/14  20:26:56  jaffe]
# 
# Revision 1.1.5.2  1992/08/31  19:36:58  jaffe
# 	Transarc delta: jaffe-ot4632-add-libktpq-to-sefsvmunix 1.1
# 	  Selected comments:
# 	    since sefsvmunix links against libktkm and libktkm requires libktpq, we
# 	    now link with libktpq also.
# 	[1992/08/30  02:23:46  jaffe]
# 
# Revision 1.1.2.4  1992/06/15  14:50:09  garyf
# 	build stand alone episode kernel
# 	add dependencies for kernel
# 	[1992/06/15  14:47:28  garyf]
# 
# Revision 1.1.2.3  1992/05/05  19:07:13  jdp
# 	delta jdp-epimerge-add-dynamic-configuration 1.1
# 	[1992/05/04  21:40:36  jdp]
# 
# Revision 1.1.2.2  1992/04/02  17:58:06  garyf
# 	Initial revision
# 	[1992/04/02  17:48:46  garyf]
# 
# $EndLog$

VPATH			= ${TARGET_OS}:${MAKETOP}/../export/${target_machine}/usr/lib

LIBRARIES		= libefs.a

KERNEL_CC		=

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}
CFLAGS			= ${KERNELCFLAGS}
OFILES			= epi_config.o efs_stubs.o

OTHERS		= sefsvmunix

IDIR		= /

ILIST		= sefsvmunix

CONFIG		= ${KERNEL_${TARGET_MACHINE}_CONFIG:U${KERNEL_CONFIG:UDCE}}

afs_libs	= -lkdfskutil 
		  
afs_libs_deps	= libkdfskutil.a libktkm.a libktpq.a libvolreg.a \
		  libxvnode.a libktkc.a libvolume.a libkdacl.a libkdacllfs.a \
		  libaggr.a libufsops.a libknck.a libkidl.a libkdes.a \
		  libkxcred.a libkicl.a libkosi.a libkzlc.a

efs_libs 	= -lefs -lkefsops -lkdir -lkeacl -lkanode \
		  -lklogbuf -lkasync -lktools -lkolddacl

efs_libs_deps	= libefs.a libkefsops.a libkdir.a libkeacl.a libkanode.a \
		  libklogbuf.a libkasync.a libktools.a libkolddacl.a

afs_libs2	= -lxvnode -lktkc -lktkm -lktpq -lvolreg -lkzlc -lvolume \
		  -lkdacl -lkdacllfs -lkdfskutil \
		  -laggr -lufsops -lkdacl -lknck \
		  -lkidl -lkdes -lkxcred -lkicl -lkosi

sefsvmunix: ${MAKETOP}/${OBJECTDIR}/kernel/${CONFIG}/vmunix.sys  ${KERNEL_LDDEP} ${afs_libs_deps} ${efs_libs_deps}
	${MACHO_GCC_EXEC_PREFIX}ld ${KERNEL_LDOPTS}  -e start -o sefsvmunix ${LIBDIRS} \
	${${MAKETOP}/${OBJECTDIR}/kernel/${CONFIG}/vmunix.sys:P} ${KERNEL_LDDEP} ${afs_libs} ${efs_libs} ${afs_libs2}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists (${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif

.include <${RULES_MK}>
