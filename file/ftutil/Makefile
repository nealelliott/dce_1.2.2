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
# $EndLog$
#

INCLUDES		= ftutil.h ftu_syscalls.h ftu_trace.h \
			  pipe.h ncs_pipe.h file_pipe.h sock_pipe.h \
			  async_queue.h \
			  dfsftlmsg.h dfsftlmac.h dfsftlsvc.h

# Episode already exports ``libasync.a'' 
LIBRARIES		= libftutil.a libpipe.a libasyncq.a

EXPDIR			= /usr/include/dcedfs/
libftutil.a_EXPDIR	= /usr/lib/
libpipe.a_EXPDIR	= /usr/lib/
libasyncq.a_EXPDIR	= /usr/lib/

EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfsftu.cat dfszfu.cat
TECHNOLOGY=dfs

SAMSFILES = ftl.sams
SAMS_TARGETS = dfsftlmsg.h dfsftlmsg.c dfsftlmac.h dfsftlsvc.c dfsftlsvc.h \
    dfsftl.cat

dfsftl.cat_IDIR		= /nls/msg/en_US.ASCII/
dfsftu.cat_IDIR		= /nls/msg/en_US.ASCII/
dfszfu.cat_IDIR		= /nls/msg/en_US.ASCII/

ILIST			= ncs_pipe.h pipe.h async_queue.h ${OTHERS} dfsftl.cat
IDIR			= /share/include/dcedfs/
INCFLAGS		= -I../xaggr

CFLAGS			= ${USERCFLAGS} -DOSF_NO_SOCKET_DUMP

libftutil.a_OFILES	= ftu_log.o ftu_util.o ftu_lock.o ftu_aggrreg.o \
			  ftu_aggr.o ftu_aggrfset.o ftu_fset.o ftu_syscalls.o \
			  cloneops.o dumpops.o restoreops.o imageprep.o \
			  aggr_ops.o dfsftlsvc.o dfsftlmsg.o ftutil_svc.o
libpipe.a_OFILES	= local_pipe.o ncs_pipe.o file_pipe.o sock_pipe.o \
			  dfsftlsvc.o dfsftlmsg.o ftutil_svc.o
libasyncq.a_OFILES	= async_queue.o

GARBAGE			= ftutil.p.h

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

ftutil.h ftu_errs.msf: ftutil.p.h ftu_errs.et
	-${CP} ${ftutil.p.h:P} .
	${COMPILE_ET} ${ftu_errs.et:P} -h ftutil
	${RM} -f ftutil.p.h

dfsftu.cat: ftu_errs.msf
	${GENCAT} $@ $>

ftu_trace.h ftu_trace.msf: ftu_trace.et
	${COMPILE_ET} ${ftu_trace.et:P} -h ftu_trace

dfszfu.cat: ftu_trace.msf
	${GENCAT} $@ $>
