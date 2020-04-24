# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.22.1  1996/10/02  17:54:52  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:42:24  damon]
#
# $EndLog$
# Copyright (C) 1996 Transarc Corporation - All rights reserved

libnubik.a_OFILES	= disk.o remote.o beacon.o recovery.o ubik.o vote.o \
                          lock.o phys.o ubikclient.o \
			  ubikdisk_proc_cstub.o ubikdisk_proc_sstub.o \
			  ubikvote_proc_cstub.o ubikvote_proc_sstub.o \
			  upipe.o

.if !defined(LIBDCEDFS_LIBRARY_MAKEFILE)
#
# This information is for when we are not building libdcedfs.a


INCLUDES		= nubik.h ubik_data.idl \
			  ubikdisk_proc.h ubikvote_proc.h \
			  ubik_data.h ubik.h

LIBRARIES		= libnubik.a
PROGRAMS		= udebug

EXPDIR			= /usr/include/dcedfs/
libnubik.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfsubk.cat dfszuk.cat
ILIST			= ${OTHERS} nubik.h ubik_data.h ${PROGRAMS}
IDIR			= /share/include/dcedfs/
udebug_IDIR		= /bin/
dfsubk.cat_IDIR		= /nls/msg/en_US.ASCII/
dfszuk.cat_IDIR		= /nls/msg/en_US.ASCII/

CFLAGS			= -Dunix ${USERCFLAGS}

udebug_OFILES		= udebug.o
LIBS			= -lnubik -licl -lcmd -lafsutil -lncompat \
			  -ldacl -ldauth -ldacllfs -ldacl -ldauth -ldacllfs \
			  -licl -lafssys -losi -lcom_err -ldce ${OS_LIBS} 

IDLFILES		= ubik_data.idl ubikdisk_proc.idl ubikvote_proc.idl

IDLFLAGS		= -keep c_source -no_cpp -no_mepv 
IDLINCFLAGS		= -I. ${INCDIRS} -I../config

GARBAGE			= ubikdisk_proc_cstub.c ubikdisk_proc_sstub.c \
			  ubikvote_proc_cstub.c ubikvote_proc_sstub.c

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>


ubik.h uerrors.msf: ubik.p.h uerrors.et utrace.h
	-${CP} ${ubik.p.h:P} .
	${COMPILE_ET} ${uerrors.et:P} -h ubik
	${RM} -f ubik.p.h

utrace.h utrace.msf: utrace.et
	${COMPILE_ET} ${utrace.et:P} -h utrace

dfsubk.cat: uerrors.msf
	 ${GENCAT} $@ $>

dfszuk.cat: utrace.msf
	${GENCAT} $@ $>

nubik.h: ubik.h
	-${CP} ubik.h nubik.h
.endif
