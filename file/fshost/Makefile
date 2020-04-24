# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.81.1  1996/10/02  17:46:29  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:37:20  damon]
#
# Revision 1.1.76.3  1994/08/23  18:58:33  rsarbo
# 	delegation
# 	[1994/08/23  16:45:01  rsarbo]
# 
# Revision 1.1.76.2  1994/06/09  14:07:27  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:22:30  annie]
# 
# Revision 1.1.76.1  1994/02/04  20:18:15  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:12:51  devsrc]
# 
# Revision 1.1.74.1  1993/12/07  17:23:48  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:18:56  jaffe]
# 
# Revision 1.1.6.4  1993/01/21  19:34:51  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  19:48:29  zeliff]
# 
# Revision 1.1.6.3  1993/01/13  17:30:47  shl
# 	Transarc delta: jaffe-cleanup-2.3-compiler-warnings 1.8
# 	  Selected comments:
# 	    Fix compiler warnings found when moving to OSF/1 1.1.1.
# 	    Remove the function dacl_SetOwnerControl.  It is not used.
# 	    install dfszhs.cat into the NLS directory, not the include dir.
# 	    fix typo here.
# 	Transarc delta: tu-ot5849a-ot5849b-ot6048-super-delta 1.2
# 	  Selected comments:
# 	    This super-delta was created from configuration dfs-102, revision 2.2,
# 	    by importing the following deltas:
# 	    tu-ot5849-add-icl-fshost 1.2
# 	    tu-ot5849-resolve-merge-conflict 1.1
# 	    tu-ot6048-deadlock-primary-sec-service 1.3
# 	    fix some merge problems
# 	[1993/01/12  20:49:52  shl]
# 
# Revision 1.1.6.2  1992/11/24  17:02:49  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  17:58:08  bolinger]
# 
# Revision 1.1.3.6  1992/05/20  19:51:39  mason
# 	Transarc delta: cfe-ot3085-fix-msf-dependencies 1.1
# 	  Files modified:
# 	    bak: Makefile; bakserver: Makefile; bosserver: Makefile
# 	    bubasics: Makefile; flserver: Makefile
# 	    flserver.klib: Makefile; fshost: Makefile; fsint: Makefile
# 	    ftserver: Makefile; host: Makefile; ncscompat: Makefile
# 	    ncsubik: Makefile; security/dacl: Makefile
# 	    security/dfsauth: Makefile; sysacl: Makefile; tkm: Makefile
# 	    tools/cmd: Makefile; update: Makefile; xcred: Makefile
# 	    xvolume: Makefile
# 	  Selected comments:
# 	    Make explicit the dependency that builds an .msf file from an .et file.
# 	    Add dependency to build .msf file.
# 	[1992/05/20  11:14:35  mason]
# 
# Revision 1.1.3.5  1992/04/14  04:08:38  mason
# 	Transarc delta: bab-ot2394-change-catalog-names 1.3
# 	  Files modified:
# 	    bak: Makefile; bakserver: Makefile; bosserver: Makefile
# 	    bubasics: Makefile; flserver: Makefile; fshost: Makefile
# 	    fsint: Makefile; ftserver: Makefile; host: Makefile
# 	    ncscompat: Makefile; ncsubik: Makefile
# 	    security/dacl: Makefile; security/dfsauth: Makefile
# 	    tkm: Makefile; tools/cmd: Makefile; update: Makefile
# 	    xcred: Makefile; xvolume: Makefile
# 	  Selected comments:
# 	    The dfs error catalogs were generated with names that made them
# 	    dce_error_inq_text unable to locate them. (dce_error_inq_text pulls
# 	    a facility code out of the error code and uses this to construct a
# 	    filename in which to look for the error message.  Our (dfs) facility
# 	    code is such that our error catalog filenames must start with "dfs",
# 	    rather then "dce".)
# 	    Change occurrences of "dce...\.cat" to "dfs...\.cat"
# 	    ot 2394
# 	    The sed script missed one occurrence of "dfs...\.cat"
# 	    the first time through because it was hidden by another
# 	    occurrence earlier on the line.
# 	[1992/04/13  16:03:27  mason]
# 
# Revision 1.1.3.4  1992/01/25  20:47:37  zeliff
# 	dfs6.3 merge, part2
# 	[1992/01/25  20:05:24  zeliff]
# 
# $EndLog$

INCLUDES		= fshs.h fshs_host.h fshs_princ.h \
			  fshs_errs.h fshs_trace.h fshs_deleg.h

LIBRARIES		= libhost.a

EXPDIR			= /usr/include/dcedfs/
libhost.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfsfsh.cat dfszhs.cat 
ILIST			= ${OTHERS} fshs_errs.h  fshs_trace.h
IDIR			= /share/include/dcedfs/
dfsfsh.cat_IDIR		= /nls/msg/en_US.ASCII/
dfszhs.cat_IDIR		= /nls/msg/en_US.ASCII/

KERNEL_CC		=
CFLAGS			= ${KERNELCFLAGS}
libhost.a_OFILES	= fshs_host.o fshs_hostops.o fshs_princ.o \
			  fshs_prutils.o fshs_subr.o fshs_deleg.o

GARBAGE			= fshs_errs.h   fshs_trace.h

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

fshs_errs.h fshs_errs.msf: fshs_errs.et
	${COMPILE_ET} ${fshs_errs.et:P} -h fshs_errs

dfsfsh.cat: fshs_errs.msf
	 ${GENCAT} $@ $>

fshs_trace.h fshs_trace.msf: fshs_trace.et
	${COMPILE_ET} ${fshs_trace.et:P} -h fshs_trace
dfszhs.cat: fshs_trace.msf
	${GENCAT} $@ $>


