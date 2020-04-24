# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# Copyright (c) 1994, Transarc Corporation
# $EndLog$
#

VPATH			= ../tkm

LIBRARIES		= libktkm.a

EXPDIR			= /usr/include/dcedfs/
libktkm.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=

CFLAGS			= ${KERNELCFLAGS} ${tkm_KERNELCFLAGS} \
			  -DTKM_DEBUG -D_NO_PROTO ${DFS_CORE}

OFILES			= tkm_recycle.o \
			  tkm_misc.o tkm_asyncgrant.o tkm_tokens.o\
			  tkm_filetoken.o tkm_voltoken.o \
			  tkm_file.o tkm_volume.o \
			  tkm_revokes.o tkm_conflicts.o tkm_ctable.o\
			  tkm_tokenlist.o tkm_range.o tkm_race.o tkm_linkage.o


INCFLAGS		= -I../tkm

GARBAGE			= tkm_errs.c

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
