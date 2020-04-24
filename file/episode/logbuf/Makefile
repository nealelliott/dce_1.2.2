# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.908.1  1996/10/02  17:24:36  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:32:08  damon]
#
# Revision 1.1.903.3  1994/06/09  14:00:39  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:35:57  annie]
# 
# Revision 1.1.903.2  1994/02/04  20:13:28  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:10:27  devsrc]
# 
# Revision 1.1.90.1  1993/12/07  17:19:10  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  13:59:37  jaffe]
# 
# Revision 1.1.5.6  1993/01/19  15:27:16  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:47:50  cjd]
# 
# Revision 1.1.5.5  1992/12/09  19:49:34  jaffe
# 	Transarc delta: mason-ot5976-defer-io-during-recovery 1.2
# 	  Selected comments:
# 	    The NBS changes meant that there could be many pages which didn't really need
# 	    I/O.  The existing simplistic algorithm was merrily reading and writing those
# 	    pages even though no data on the pages was modified.  To fix this, we merely
# 	    defer the I/O until we find a transaction really needing to write on the data
# 	    page; once found we keep it around.  At the end we check to see if we ever
# 	    read it.  If so, we write it back to disk. If not, we just go to the next
# 	    modified disk block.
# 	    This first delta is a checkpoint.
# 	    Add dependency for testbuf
# 	    Complete work here.  This version completes the delta.  Add several
# 	    regression tests and enhance the testing language to support better
# 	    concurrency guarantees.
# 	Transarc delta: rajesh-ot6194-install-readlog 1.1
# 	  Selected comments:
# 	    Install readlog in the sandbox install tree by default.
# 	    See above.
# 	[1992/12/03  21:43:36  jaffe]
# 
# Revision 1.1.5.4  1992/11/24  16:25:10  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  16:23:18  bolinger]
# 
# Revision 1.1.5.3  1992/09/15  17:36:32  jaffe
# 	Transarc delta: jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
# 	Transarc delta: mason-add-hp800-osi-routines 1.15
# 	Transarc delta: mason-ot2930-logbuf-parts-of-nbs 1.14
# 	Transarc delta: mason-ot3721-start-tran-not-required 1.6
# 	Transarc delta: mason-ot4815-make-nested-tran-work-again 1.1
# 	Transarc delta: mason-tran-0033 1.11
# 	[1992/09/14  20:29:46  jaffe]
# 
# Revision 1.1.5.2  1992/08/31  19:38:12  jaffe
# 	sync with Transarc
# 	[1992/08/30  13:56:00  jaffe]
# 
# 	Transarc delta: jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
# 	  Selected comments:
# 	    add anode/epi_errs.msf.
# 	    fix problems in the logbuf layer.
# 	    Add "tran.o" to the list of objects.
# 	    fix some problems in the vnode layer.
# 	    Fix more vnode layer problems.
# 	    Add files that were introduced after DCE 1.0 to the .klib Makefiles.
# 	    fix various problems that caused undefined references
# 	    while building the AIX kernel extension.
# 	    fix random problems encountered under OSF/1.
# 	    fix some problems encountered when compiling test_vnodeops on OSF/1.
# 	    include OSF's changes to epimount.
# 	Transarc delta: jdp-ot4179-lfs-encapsulate-initialization 1.3
# 	  Selected comments:
# 	    This delta simplifies Episode user-space initialization.
# 	    Added line to remove logbuf.p.h before building a new one.
# 	    Fix a couple of remaining compilation problems.
# 	    Fix compilation problems on OSF/1.
# 	Transarc delta: mason-add-hp800-osi-routines 1.15
# 	  Selected comments:
# 	    Need to checkpoint to fix a different bug.
# 	    Checkpoint the first system where test_anode works.  Only one change is not
# 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
# 	    is -lBSD.  This should be fixed more generally.
# 	    Added readlog.
# 	    More HP-UX changes.
# 	    More HP-UX related changes.
# 	    Fix compile problem on the RIOS.
# 	    Fix more compilation problems.
# 	    Change "-lBSD" to ${BSD_LIBS}.
# 	    Checkpoint.
# 	    Checpoint.
# 	    In this snap we turned all malloc() calls into osi_Alloc calls.
# 	    Also turned free calls into osi_Free.
# 	    HP-UX EOF detection fix.
# 	    Another checkpoint.
# 	    This is a checkpoint of the first working HP-UX Episode system.
# 	    Needed to add the vnops routines specific to HP-UX.
# 	    Fix compilation problems.
# 	Transarc delta: mason-ot4436-fix-inverted-search-code-in-recovery 1.5
# 	  Selected comments:
# 	    The inverted search code in recovery can be collapsed to a single
# 	    search using an arithmetic operation rather than a comparision
# 	    operation.
# 	    Really make the inverted search work correctly.
# 	    Added readlog program to makefile.  Need to test one final case.
# 	    Test this change by setting the initial pass number to force the
# 	    inverted-comparison case to really happen during the recovery phase.
# 	    Found that the log pkg was not computing active size correctly in that
# 	    instance.  These changes now work.
# 	    Cleanup.
# 	    The restructuring of the code allowed an unusual, but important case slip
# 	    through the cracks - that of a mostly-newly-initialized log (where the pass #
# 	    is the dreaded "0").  I added a loop which checks for this condition and, if
# 	    necessary, seeks backwards in the log to find the highest REAL page in the
# 	    log.
# 	Transarc delta: mason-tran-0033 1.11
# 	  Selected comments:
# 	    Checkpoint for the year-end.
# 	    Added new files.
# 	    Intermediate snapshot - single threaded code now working correctly.
# 	    SHOULD NOT BE USED IN PRODUCTION.
# 	    Added test programs for transaction system.
# 	    Forced close to fix a different problem. This delta still does not work; do
# 	    not use in production!
# 	    Snapshot - passes all_tests.
# 	    More work - now fixing bugs.
# 	    Final round of testing; appears to be production quality now.
# 	    Update internal development records.
# 	    Found single-threaded case in TranForce which would block.
# 	    Macro removed in delta 1.9 was used in multiple files.
# 	    Add dependency of buffer.o on tran.o
# 	    Found race condition where tran was referenced before initialization.
# 	[1992/08/30  02:27:50  jaffe]
# 
# Revision 1.1.3.9  1992/07/02  18:01:08  jdp
# 	Transarc delta: mason-ot4415-change-log-format 1.2
# 	  Selected comments:
# 	    Some log format changes were necessary to allow us to implement a
# 	    checksum scheme.  This delta makes the on-disk format changes
# 	    necessary but does not yet implement the checksum scheme.
# 	    Changed one of the spare fields to an io_window field to record the
# 	    current number of outstanding I/Os when the log page gets written.
# 	    Added dependencies so things get rebuilt properly
# 	[1992/07/02  15:30:22  jdp]
# 
# Revision 1.1.3.8  1992/06/23  13:53:54  jdp
# 	Transarc delta: ota-ot3609-no-msf-rules 1.1
# 	  Selected comments:
# 	    Fix problem with Episode makefiles wherein they didn't have a rule to
# 	    make the *.msf files.  Since this is a side effect of running
# 	    compile_et, just add this to the left-hand-side of that rule.
# 	    Also remove fifo_errors.et since this isn't used.
# 	    Change makefile.
# 	[1992/06/22  22:01:09  jdp]
# 
# Revision 1.1.3.7  1992/05/13  21:59:05  jdp
# 	Transarc delta: bab-ot2394-change-catalog-names 1.2
# 	  Files modified:
# 	    anode: Makefile; async: Makefile; logbuf: Makefile
# 	    salvage: Makefile
# 	  Selected comments:
# 	    The dfs error catalogs were generated with names that made them
# 	    dce_error_inq_text unable to locate them. (dce_error_inq_text pulls
# 	    a facility code out of the error code and uses this to construct a
# 	    filename in which to look for the error message.  Our (dfs) facility
# 	    code is such that our error catalog filenames must start with "dfs",
# 	    rather then "dce".)
# 	    Change occurrences of "dce...\.cat" to "dfs...\.cat"
# 	    ot 2394
# 	Transarc delta: jdp-epimerge-2.17+-into-epi-weekly-1.2 1.4
# 	  Files modified:
# 	    anode: strategy.c; logbuf: Makefile; utils: epimount.c
# 	    vnops: efs_vnodeops.c; vnops.klib/RIOS: Makefile
# 	  Selected comments:
# 	    This delta fixes problems with the merge of post episode-2.17 changes into
# 	    episode-weekly-1.2.
# 	    Remove "../tools" from list of include directories.  This caused several
# 	    redefinition/redeclaration problems in "lock.h" on OSF/1.
# 	    Fix more merge problems.
# 	[1992/05/13  21:10:30  jdp]
# 
# Revision 1.1.3.6  1992/05/05  19:08:20  jdp
# 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
# 	delta jdp-epimerge-use-ode-makefiles 1.5
# 	delta mason-tran-0033 1.11
# 	[1992/05/04  22:10:54  jdp]
# 
# Revision 1.1.3.5  1992/04/16  17:19:33  garyf
# 	fix .p.h rules
# 	[1992/04/16  17:14:25  garyf]
# 
# Revision 1.1.3.4  1992/01/25  20:46:24  zeliff
# 	dfs6.3 merge, part2
# 	[1992/01/25  20:03:39  zeliff]
# 
# $EndLog$

PROGRAMS		= readlog patchlog
OTHERS			= readlog patchlog
IDIR			= /bin/

INCLUDES		= logbuf.h

LIBRARIES		= liblogbuf.a

EXPDIR			= /usr/lib/
logbuf.h_EXPDIR		= /usr/include/dcedfs/episode/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			+= dfslgb.cat dfszel.cat
ILIST			= ${OTHERS} readlog
dfslgb.cat_IDIR		= /nls/msg/en_US.ASCII/
dfszel.cat_IDIR		= /nls/msg/en_US.ASCII/
readlog_IDIR		= /unsupported/bin/

CFLAGS			= ${USERCFLAGS} -D_TRANSARC_PTHREAD_PROTO
YFLAGS			= -vd
OFILES			= buffer.o log.o recover.o tran.o crc.o

readlog_LIBS		= liblogbuf.a -lasync -ltools -lcmd -lcom_err \
			  -llwp -licl -lafsutil -losi -ldce ${OS_LIBS}
readlog_OFILES		= readlog.o

patchlog_LIBS	       	= liblogbuf.a -ltools -llwp -licl -lafsutil -losi \
			  -ldce ${OS_LIBS}
patchlog_OFILES		= patchlog.o

testbuf_gram.o_OPT_LEVEL = -g
testbuf_lex.o_OPT_LEVEL	= -g
testbuf_OFILES  = testbuf_gram.o testbuf_lex.o
testbuf_LIBS	= liblogbuf.a -lasync -ltools -lafsutil -lcom_err -losi \
		  -ldce ${OS_LIBS}

testtran_gram.o_OPT_LEVEL = -g
testtran_lex.o_OPT_LEVEL = -g
testtran_OFILES = testtran_gram.o testtran_lex.o
testtran_LIBS	= liblogbuf.a -lasync -ltools -lafsutil -lcom_err -losi \
		  -ldce ${OS_LIBS}


GARBAGE			= logbuf.p.h testtran_gram.c testbuf_gram.c \
			  testtran_lex.c testbuf_lex.c

readlog.o_OPT_LEVEL	= -g
readlog_OPT_LEVEL	= -g
testbuf_OPT_LEVEL	= -g
patchlog.o_OPT_LEVEL	= -g

#
# The test programs really only work properly if built with bison and flex.
# if they do not exist here, don't try to build them.  If you want them to
# build, run "build YACC=bison LEX=flex" and hope for the best!
#
#YACC 			= bison
#LEX 			= flex
.if defined(YACC)
.if defined(LEX)
.if (${YACC}=="bison")
.if ($(LEX)=="flex")
PROGRAMS		+= testbuf # testtran - when it works again
.endif
.endif
.endif
.endif

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>



.if (${YACC} == "bison")
.y.o:
	-${CP} ${.IMPSRC} ${.TARGET:S/.o/.y/}
	${YACC} ${_YFLAGS_} ${.TARGET:S/.o/.y/}
.if (${TARGET_MACHINE}=="RIOS")
	${_CC_} -c -ma ${_CCFLAGS_} ${.TARGET:S/.o/.tab.c/}
.else
	${_CC_} -c ${BISON_ALLOC_FLAGS} ${_CCFLAGS_} ${.TARGET:S/.o/.tab.c/}
.endif
	${RM} ${_RMFLAGS_} ${.TARGET:S/.o/.tab.c/}
	${MV} -f ${.TARGET:S/.o/.tab.o/} ${.TARGET}
	${RM} ${_RMFLAGS_} ${.TARGET:S/.o/.y/}
.y.c:
	-${CP} ${.IMPSRC} ${.TARGET:S/.c/.y/}
	${YACC} ${_YFLAGS_} ${.TARGET:S/.c/.y/}
	${RM} ${_RMFLAGS} ${.TARGET:S/.c/.tab.h/}
	${RM} ${_RMFLAGS_} ${.TARGET:S/.c/.y/}
.y.h:
	-${CP} ${.IMPSRC} ${.TARGET:S/.h/.y/}
	${YACC} ${_YFLAGS_} ${.TARGET:S/.h/.y/}
	${RM} ${_RMFLAGS_} ${.TARGET:S/.h/.tab.c/}
	${MV} -f ${.TARGET:S/.h/.tab.h/} ${.TARGET}
	${RM} ${_RMFLAGS_} ${.TARGET:S/.h/.y/}
.endif

testbuf_lex.o : testbuf_gram.h
#testbuf: ${OFILES} testbuf_gram.o
testbuf_gram.o: testbuf_gram.y

testtran_lex.o: testtran_gram.h
testtran: ${OFILES} testtran_gram.o
testtran_gram.o: testtran_gram.y

logbuf.h logbuf_errors.msf: logbuf.p.h logbuf_errors.et
	${RM} -f ./logbuf.p.h
	${CP} ${logbuf.p.h:P} .
	${COMPILE_ET} ${logbuf_errors.et:P} -h logbuf
	${RM} -f logbuf.p.h

elb_trace.h elb_trace.msf: elb_trace.et
	${COMPILE_ET} ${elb_trace.et:P} -h elb_trace

dfszel.cat: elb_trace.msf
	${GENCAT} $@ $>

readlog.o : readlog.c
readlog: ${OFILES} readlog.o
patchlog.o : patchlog.c
patchlog: ${OFILES} patchlog.o
testbuf: ${OFILES}
dfslgb.cat: logbuf_errors.msf
	 ${GENCAT} $@ $>

#
# If we add the following, everything builds all the time - twice.
# I don't know why...
buffer.o: elb.h elb_trace.h logrec.h tran.h logbuf.h
log.o: elb.h elb_trace.h logrec.h tran.h recover.h logbuf.h
recover.o: elb.h elb_trace.h recover.h logrec.h logbuf.h
tran.o: elb.h elb_trace.h logrec.h tran.h logbuf.h
