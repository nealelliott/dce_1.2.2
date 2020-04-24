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
# Revision 1.1.731.1  1996/10/02  17:28:46  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:34:38  damon]
#
# Revision 1.1.726.3  1994/06/09  14:03:42  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:38:38  annie]
# 
# Revision 1.1.726.2  1994/02/04  20:15:29  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:11:26  devsrc]
# 
# Revision 1.1.72.1  1993/12/07  17:21:02  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:00:04  jaffe]
# 
# Revision 1.1.4.3  1993/01/21  18:24:04  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  14:14:09  zeliff]
# 
# Revision 1.1.4.2  1992/09/15  17:40:01  jaffe
# 	Transarc delta: jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
# 	  Selected comments:
# 	    This revision of this delta contains changes made to episode/async by the OSF
# 	    between dce1.0 and dce1.0.1b9.  Subsequent revisions will contain merges
# 	    for other episode subdirectories.
# 	    This revision of this delta contains changes made to episode/dir by the OSF
# 	    This revision of this delta contains changes made to episode/lobguf by the OSF
# 	    This revision of this delta contains changes made to episode/salvage by the OSF
# 	    This revision of this delta contains changes made to episode/sautils by the OSF
# 	    This revision of this delta contains changes made to episode/tools by the OSF
# 	    This revision of this delta contains changes made to episode/utils by the OSF
# 	    This revision of this delta contains changes made to episode/vnops by the OSF
# 	    between dce1.0 and dce1.0.1b9.
# 	    This revision fixes some problems in the logbuf layer that occurred after
# 	    the merge.
# 	    This delta adds anode/epi_errs.msf.
# 	    This delta fixes problems in the logbuf layer.
# 	    This revision fixes some problems in the vnode layer.
# 	    Fix more vnode layer problems.
# 	    Add files that were introduced after DCE 1.0 to the .klib Makefiles.
# 	    Add "trace.o"
# 	    This delta fixes various problems that caused undefined references
# 	    while building the AIX kernel extension.
# 	    This revision fixes random problems encountered under OSF/1.
# 	    This revision fixes some problems encountered when compiling test_vnodeops
# 	    on OSF/1.
# 	    This revision includes the OSF's changes to epimount.
# 	Transarc delta: mason-ot2548-buffer-reservation 1.8
# 	  Selected comments:
# 	    This delta is used to implement the new buffer reservation scheme for users
# 	    of multiple buffers.  This delta is done.
# 	    Must snapshot so I can work on a different bug.  This code DOES NOT COMPILE.
# 	    This is a checkpoint.  Not functional yet.
# 	    Another checkpoint.
# 	    Another checkpoint.  Basic code done, need to finish debugging.
# 	    This is it.  Delta now works correctly, including multiple-aggregate
# 	    bashing of buffer reservation.
# 	    must put epis_reserve.o into the klib library in order to get this to work
# 	[1992/09/14  21:08:34  jaffe]
# 
# Revision 1.1.2.2  1992/05/05  17:44:33  jdp
# 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
# 	delta jdp-epimerge-add-klib-dirs 1.1
# 	[1992/05/04  23:18:28  jdp]
# 
# Revision 1.1  1992/01/19  02:40:40  devrcs
# 	Initial revision
# 
# $EndLog$
#

VPATH			= ../tools

LIBRARIES		= libktools.a

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=
CFLAGS                  = ${KERNELCFLAGS}
OFILES			= trace.o epis_reserve.o epi_thread.o

INCFLAGS		= -I../tools

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
