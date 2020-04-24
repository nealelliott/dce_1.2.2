# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.121.1  1996/10/02  18:48:47  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:49:19  damon]
#
# Revision 1.1.116.3  1994/06/09  14:23:04  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:34:40  annie]
# 
# Revision 1.1.116.2  1994/02/04  20:33:12  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:19:15  devsrc]
# 
# Revision 1.1.115.1  1993/12/07  17:35:59  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  17:19:52  jaffe]
# 
# Revision 1.1.2.5  1993/01/21  15:55:55  cjd
# 	embedded copyright notice
# 	[1993/01/20  17:39:27  cjd]
# 
# Revision 1.1.2.4  1992/11/24  19:48:06  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  19:32:21  bolinger]
# 
# Revision 1.1.2.3  1992/09/25  19:43:23  jaffe
# 	Sync with Transarc
# 	[1992/09/23  20:52:11  jaffe]
# 
# 	Transarc delta: comer-remove-lwpdummy 1.1
# 	  Selected comments:
# 	    Tests in the tpq directory were linking with the lwpdummy
# 	    library but were not using any symbols from it.  -llwpdummy
# 	    was removed from the Makefile.
# 	    Remove unneeded references to lwpdummp by test programs.
# 	    TESTING
# 	    This change was tested by compiling and running the test
# 	    programs on both a RIOS and a PMAX under dfs-102-1.49.
# 	[1992/09/23  19:56:29  jaffe]
# 
# Revision 1.1.2.2  1992/08/31  21:33:37  jaffe
# 	Transarc delta: comer-ot3947-multithread-cm-daemon 1.25
# 	[1992/08/30  13:08:08  jaffe]
# 
# $EndLog$

INCLUDES		= tpq.h tpq_trace.h

PROGRAMS		= tpq_test tpq_pardo_test tpq_parset_test

LIBRARIES		= libtpq.a

OTHERS			= dfsztq.cat
ILIST			= ${OTHERS}
IDIR			= /nls/msg/en_US.ASCII/

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

tpq.h_EXPDIR		= /usr/include/dcedfs/
tpq_trace.h_EXPDIR		= /usr/include/dcedfs/

CFLAGS			= ${USERCFLAGS}

libtpq.a_OFILES		= tpq_die.o tpq_dispatch.o tpq_helper.o \
			  tpq_init.o tpq_queue.o tpq_stat.o tpq_access.o \
			  tpq_wait.o tpq_pardo.o tpq_parset.o

tpq_test_OFILES		= tpq_test.o
tpq_test_LIBS		= -ltpq -licl -losi -lncompat -ldce ${OS_LIBS}

tpq_pardo_test_OFILES	= tpq_pardo_test.o
tpq_pardo_test_LIBS	= -ltpq -licl -losi -lncompat -ldce ${OS_LIBS}

tpq_parset_test_OFILES	= tpq_parset_test.o
tpq_parset_test_LIBS	= -ltpq -licl -losi -lncompat -ldce ${OS_LIBS}

tpq_trace.h tpq_trace.msf: tpq_trace.et
	${COMPILE_ET} ${tpq_trace.et:P} -h tpq_trace

dfsztq.cat: tpq_trace.msf
	${GENCAT} $@ $>

.if exists(../${TARGET_MACHINE}/machdep.mk)
.include "../${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(../${TARGET_OS}/machdep.mk)
.include "../${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
