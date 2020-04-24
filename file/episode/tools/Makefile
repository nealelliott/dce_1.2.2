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
# Revision 1.1.745.1  1996/10/02  17:28:53  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:34:42  damon]
#
# Revision 1.1.740.3  1994/06/09  14:03:49  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:38:42  annie]
# 
# Revision 1.1.740.2  1994/02/04  20:15:31  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:11:27  devsrc]
# 
# Revision 1.1.73.1  1993/12/07  17:21:04  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:00:19  jaffe]
# 
# Revision 1.1.4.6  1993/01/21  18:24:14  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  14:14:26  zeliff]
# 
# Revision 1.1.4.5  1992/11/24  16:56:54  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  16:26:38  bolinger]
# 
# Revision 1.1.4.4  1992/09/16  16:54:29  jaffe
# 	add instrument.h to INCLUDE line
# 	[1992/09/16  16:53:23  jaffe]
# 
# Revision 1.1.4.3  1992/09/15  17:40:05  jaffe
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
# 	    Remove various obsolete files.
# 	    any program which uses lwp now must link with -ldce
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
# 	    Add "trace.o" to libtools.a_OFILES
# 	    Add files that were introduced after DCE 1.0 to the .klib Makefiles.
# 	    This delta fixes various problems that caused undefined references
# 	    while building the AIX kernel extension.
# 	    This revision fixes random problems encountered under OSF/1.
# 	    This revision fixes some problems encountered when compiling test_vnodeops
# 	    on OSF/1.
# 	    This revision includes the OSF's changes to epimount.
# 	Transarc delta: mason-add-hp800-osi-routines 1.15
# 	  Selected comments:
# 	    Need to checkpoint to fix a different bug.
# 	    Checkpoint the first system where test_anode works.  Only one change is not
# 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
# 	    is -lBSD.  This should be fixed more generally.
# 	    Added test program for lwp.
# 	    More HP-UX changes.
# 	    More HP-UX related changes.
# 	    Fix compile problem on the RIOS.
# 	    Fix more compilation problems.
# 	    Checkpoint.
# 	    Checpoint.
# 	    In this snap we turned all malloc() calls into osi_Alloc calls.
# 	    Also turned free calls into osi_Free.
# 	    HP-UX EOF detection fix.
# 	    Another checkpoint.
# 	    This is a checkpoint of the first working HP-UX Episode system.
# 	    Needed to add the vnops routines specific to HP-UX.
# 	    Fix compilation problems.
# 	Transarc delta: mason-ot2548-buffer-reservation 1.8
# 	  Selected comments:
# 	    This delta is used to implement the new buffer reservation scheme for users
# 	    of multiple buffers.  This delta is done.
# 	    Must snapshot so I can work on a different bug.  This code DOES NOT COMPILE.
# 	    This is a checkpoint.  Not functional yet.
# 	    Another checkpoint.
# 	    Added new files to the makefile.
# 	    Another checkpoint.  Basic code done, need to finish debugging.
# 	    Add test code for reservation package.
# 	    This is it.  Delta now works correctly, including multiple-aggregate
# 	    bashing of buffer reservation.
# 	[1992/09/14  21:09:08  jaffe]
# 
# Revision 1.1.4.2  1992/08/31  20:21:44  jaffe
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
# 	    Add "trace.o" to libtools.a_OFILES
# 	    Add files that were introduced after DCE 1.0 to the .klib Makefiles.
# 	    This delta fixes various problems that caused undefined references
# 	    while building the AIX kernel extension.
# 	    This revision fixes random problems encountered under OSF/1.
# 	    This revision fixes some problems encountered when compiling test_vnodeops
# 	    on OSF/1.
# 	    This revision includes the OSF's changes to epimount.
# 	Transarc delta: mason-add-hp800-osi-routines 1.15
# 	  Selected comments:
# 	    This delta is misnamed - these are really porting fixes.
# 	    Need to checkpoint to fix a different bug.
# 	    Checkpoint the first system where test_anode works.  Only one change is not
# 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
# 	    is -lBSD.  This should be fixed more generally.
# 	    Added test program for lwp.
# 	    More HP-UX changes.
# 	    More HP-UX related changes.
# 	    Fix compile problem on the RIOS.
# 	    Fix more compilation problems.
# 	    Checkpoint.
# 	    Checpoint.
# 	    In this snap we turned all malloc() calls into osi_Alloc calls.
# 	    Also turned free calls into osi_Free.
# 	    HP-UX EOF detection fix.
# 	    Another checkpoint.
# 	    This is a checkpoint of the first working HP-UX Episode system.
# 	    Needed to add the vnops routines specific to HP-UX.
# 	    Fix compilation problems.
# 	[1992/08/30  02:54:01  jaffe]
# 
# Revision 1.1.2.3  1992/05/13  22:00:29  jdp
# 	Transarc delta: jdp-ot3317-osi-move-lock.o-to-lwpdummy.a 1.1
# 	  Files modified:
# 	    tools: Makefile
# 	  Selected comments:
# 	    This delta goes with the DFS delta of the same name.
# 	    Add "lock.o" to list of objects included in "liblwp.a".
# 	[1992/05/13  21:28:19  jdp]
# 
# Revision 1.1.2.2  1992/05/05  17:44:41  jdp
# 	delta bwl-build-lwp 1.4
# 	delta bwl-build-test-vnodeops 1.2
# 	delta jdp-dynamic-tracing 1.1
# 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
# 	delta jdp-epimerge-use-ode-makefiles 1.5
# 	[1992/05/04  23:19:53  jdp]
# 
# Revision 1.1  1992/01/19  02:39:17  devrcs
# 	Initial revision
# 
# $EndLog$
#

INCLUDES		= epi_thread.h fifo.h \
			  reserve.h instrument.h critical.h

LIBRARIES		= libtools.a liblwp.a

PROGRAMS= test_res

EXPDIR			= /usr/include/dcedfs/episode/
libtools.a_EXPDIR	= /usr/lib/
liblwp.a_EXPDIR		= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

CFLAGS                  = ${USERCFLAGS} -D_TRANSARC_PTHREAD_PROTO

libtools.a_OFILES	= trace.o epis_reserve.o
liblwp.a_OFILES		= epi_thread.o

GARBAGE			= 

test_res.o_CFLAGS = ${USERCFLAGS}
test_res_LIBS	= libtools.a -lcmd -lcom_err -losi -llwp -licl -ldce ${OS_LIBS}
test_res_OFILES = test_res.o
test_res: epis_reserve.o

INCLUDES		+= ethr_trace.h
OTHERS			+= dfszet.cat
ILIST			+= dfszet.cat
dfszet.cat_IDIR		= /nls/msg/en_US.ASCII/

epi_thread.o: ethr_trace.h

ethr_trace.h ethr_trace.msf: ethr_trace.et
	${COMPILE_ET} ${ethr_trace.et:P} -h ethr_trace

dfszet.cat: ethr_trace.msf
	${GENCAT} $@ $>

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.if exists (${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif

.if exists (${TARGET_OS}/Makefile)
.include "${TARGET_OS}/Makefile"
.endif

.include <${RULES_MK}>
