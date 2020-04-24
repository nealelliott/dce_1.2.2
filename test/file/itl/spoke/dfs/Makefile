# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/10/17  18:28:20  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:54:13  damon]
#
# $EndLog$

#
#  Makefile --
#	For the test/file/itl/spoke/dfs directory.
#

CCTYPE = ansi

CFLAGS = ${USERCFLAGS} -U_BSD -DITL_FS_TEST

LIBRARIES		= libitldfsSpoke.a
EXPDIR			= /usr/include/utils/itl/
EXPLIB_TARGETS		= ${LIBRARIES:S/^/export_/g}

libitldfsSpoke.a_OFILES	= dfsSpoke_mgr.o dfsSpoke_client.o \
			  dfsSpoke_cstub.o dfsSpoke_sstub.o \
			  dfsSpoke.o dfsSpoke_defs.o

INCLUDES                = dfsSpoke_defs.h dfsSpoke.h dfsSpoke_client.h \
			  dfsSpoke_mgr.h itl_spokeDFS.h

libitldfsSpoke.a_EXPDIR = /usr/lib/

IDLFILES = dfsSpoke.idl
ACFFILES = dfsSpoke.acf
.if ${TARGET_MACHINE} == "SPARC_SOLARIS2"
IDLFLAGS = -keep c_source -no_mepv -DSUNOS5
.else
IDLFLAGS = -keep c_source -no_mepv 
.endif
IDLDATAFILES = dfsSpoke.h
STUBS = dfsSpoke_cstub.o dfsSpoke_sstub.o

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>

dfsSpoke_mgr.o: dfsSpoke_defs.h
dfsSpoke_client.o: dfsSpoke_defs.h dfsSpoke.h
dfsSpoke_defs.o: dfsSpoke_defs.h
dfsSpoke.o: dfsSpoke_defs.h dfsSpoke_mgr.h dfsSpoke_client.h
