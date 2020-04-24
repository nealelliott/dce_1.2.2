# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# Copyright (C) 1994 Transarc Corporation -- All Rights Reserved
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.129.1  1996/10/02  21:06:02  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:50:09  damon]
#
# $EndLog$

PROGRAMS 		= bos

INCLUDES		=  dfsbosmsg.h dfsbossvc.h
EXPDIR			=  /usr/include/dcedfs/

# Include -DDFS_BOS_DEBUG_BOSSERVER to define the ``bos setdebug'' command
CFLAGS			= -Dunix ${USERCFLAGS} -Duse_cds_compat
OFILES			= bos_main.o bos_pipeFile.o bos_userint.o bos_hostutils.o \
			  dfsbossvc.o dfsbosmsg.o bos_svc.o

LIBS			= -lbos -lcom_err -lcmd -lafsutil -lncompat -licl \
                          -ldacl -ldauth -ldacllfs -ldacl -ldauth -ldacllfs \
                          -licl -losi -ldce ${OS_LIBS} -lafssys

TECHNOLOGY=dfs

SAMSFILES = bos.sams
SAMS_TARGETS = dfsbosmsg.h dfsbosmsg.c dfsbosmac.h dfsbossvc.c dfsbossvc.h \
    dfsbos.cat

IDIR 			= /bin/
dfsbos.cat_IDIR          = /nls/msg/en_US.ASCII/
ILIST 			= ${PROGRAMS}  dfsbos.cat

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
