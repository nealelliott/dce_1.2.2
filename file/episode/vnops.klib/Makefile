#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
VPATH			= ${TARGET_MACHINE}:${TARGET_OS}:../vnops:../vnops/${TARGET_MACHINE}:../vnops/${TARGET_OS}

LIBRARIES		= libkefsops.a

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

efs_access.o_CFLAGS     = ${KERNELCFLAGS} -DEPI_USE_FULL_ID \
			  -DEPI_SOURCE_IDS_ARE_32BITS

KERNEL_CC		=
CFLAGS			= ${KERNELCFLAGS} ${vnops_KERNELCFLAGS} \
			  -DEPI_USE_FULL_ID

libkefsops.a_OFILES	= efs_dhops.o efs_access.o efs_vattr.o efs_dir.o \
			  efs_misc.o efs_vnode.o \
			  efs_vnodeops.o efs_vfsops.o efs_init.o \
			  efs_volops.o efs_volvnops.o efs_agops.o \
			  efs_debug.o efs_vnops_debug.o \
			  efs_promote.o efs_newalloc.o efs_volio.o

INCFLAGS		= -I../vnops

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

efs_misc.o: efs_newalloc.h efs_tmpcopy.h efs_promote.h efs_volvnops.h
efs_newalloc.o: efs_newalloc.h efs_tmpcopy.h efs_evnode.h efs_misc.h
efs_promote.o: efs_newalloc.h efs_tmpcopy.h efs_misc.h efs_evnode.h
efs_tmpcopy.o: efs_tmpcopy.h efs_evnode.h efs_misc.h efs_newalloc.h
efs_volops.o: efs_volio.h efs_evnode.h efs_misc.h efs_volvnops.h
