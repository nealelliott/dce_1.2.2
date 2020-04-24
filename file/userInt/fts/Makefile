# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# $EndLog$


libvolc.a_OFILES	= volc_vprocs.o volc_queue.o volc_volsint.o \
			  volc_misc.o volc_vldbsubr.o volc_vldbint.o \
        		  volc_io.o volc_tokens.o volc_wait.o \
			  dfsftisvc.o dfsftimsg.o fts_svc.o

.if !defined(LIBDCEDFS_LIBRARY_MAKEFILE)
#
# Information for building components when we are not building libdcedfs.a
#
INCLUDES		= volc.h  dfsftimsg.h dfsftisvc.h

LIBRARIES		= libvolc.a
PROGRAMS		= fts

EXPDIR			= /usr/include/dcedfs/
dfsftimsg.h_EXPDIR	= /usr/include/dcedfs/
dfsftisvc.h_EXPDIR	= /usr/include/dcedfs/
libvolc.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}


CFLAGS			= -Dunix ${USERCFLAGS} -DOSF_NO_SOCKET_DUMP
 
fts_OFILES		= volc_main.o fts_userint.o volc_pioctl.o

fts_LIBS		= -lvolc -lfldb -lftserver -lafs4int -lnubik -ltpq \
			  -lftutil -licl -lcom_err -lcmd -lbomb -lcommondata \
                          -lafsutil -lcmdAux -lrep -lncompat -ldfsncs -lpipe \
                          -ldacl -ldauth -ldacllfs -ldacl -ldauth -ldacllfs \
                          -losi -ldce ${OS_LIBS} -lafssys

TECHNOLOGY=dfs

SAMSFILES = fti.sams
SAMS_TARGETS = dfsftimsg.h dfsftimsg.c dfsftimac.h dfsftisvc.c dfsftisvc.h \
    dfsfti.cat

ILIST 			= ${PROGRAMS} volc.h  dfsfti.cat
IDIR 			= /bin/
dfsfti.cat_IDIR          = /nls/msg/en_US.ASCII/
volc.h_IDIR		= /share/include/dcedfs/

# Note that fts must be rebuilt if libvolc.a is.
fts:	libvolc.a

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
.endif
