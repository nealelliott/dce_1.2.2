# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# $EndLog$

libftserver.a_OFILES	= ftserver_proc_cstub.o ftserver_proc_sstub.o

.if !defined(LIBDCEDFS_LIBRARY_MAKEFILE)
#
# The rest of the information in the Makefile is for when we are not
# building libdcedfs
#

INCLUDES		= ftserver.h ftserver_data.idl \
			  ftserver_trans.h ftserver_proc.h ftserver_data.h \
			  ftserver_trace.h

LIBRARIES		= libftserver.a
PROGRAMS		= ftserver

EXPDIR			= /usr/include/dcedfs/
libftserver.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfsfts.cat dfsftt.cat

TECHNOLOGY=dfs

SAMSFILES = tsv.sams
SAMS_TARGETS = dfstsvmsg.h dfstsvmsg.c dfstsvmac.h dfstsvsvc.c dfstsvsvc.h \
    dfstsv.cat

ILIST			= ${OTHERS} ftserver ftserver.h ftserver_data.h \
			  ftserver_trans.h ftserver_proc.h  dfstsv.cat

IDIR			= /share/include/dcedfs/
dfstsv.cat_IDIR          = /nls/msg/en_US.ASCII/
dfsfts.cat_IDIR		= /nls/msg/en_US.ASCII/
dfsftt.cat_IDIR		= /nls/msg/en_US.ASCII/
ftserver_IDIR		= /bin/


CFLAGS			= -Dunix ${USERCFLAGS} -DOSF_NO_SOCKET_DUMP
ftserver_OFILES		= ftserver_main.o ftserver_trans.o ftserver_vprocs.o dfstsvsvc.o dfstsvmsg.o

ftserver_LIBS		= -lftserver -lftutil -lbomb -lcmd -lasyncq \
			  -lpipe -lafssys -lafsutil -licl -lncompat \
                          -ldacl -ldauth -ldacllfs -ldacl -ldauth -ldacllfs \
                          -lcommondata -losi -lcom_err -ldce ${OS_LIBS}

IDLFILES		= ftserver_data.idl ftserver_proc.idl

IDLFLAGS		= -keep c_source -no_mepv -DOSF_NO_SOCKET_DUMP

IDLINCFLAGS		= -I.


.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

ftserver.h ftserver.msf: ftserver.p.h ftserver.et
	-${CP} ${ftserver.p.h:P} .
	${COMPILE_ET} ${ftserver.et:P} -h ftserver
	${RM} -f ftserver.p.h

dfsfts.cat: ftserver.msf
	 ${GENCAT} $@ $>

ftserver_trace.h ftserver_trace.msf: ftserver_trace.et
	${COMPILE_ET} ${ftserver_trace.et:P} -h ftserver_trace

dfsftt.cat: ftserver_trace.msf
	 ${GENCAT} $@ $>
.endif
