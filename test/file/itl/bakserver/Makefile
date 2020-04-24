#
# @OSF_COPYRIGHT@
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/10/17  18:24:24  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:52:23  damon]
#
# $EndLog$
#
#  Makefile for DFS ITL backup server library.
#

LIBRARIES = libitlbaksrv.a

EXPDIR = /usr/include/utils/itl/

CCTYPE = ansi

CFLAGS = ${USERCFLAGS} -U_BSD -DITL_FS_TEST

INCLUDES = itl_bakPrincipal.h itl_bakTapeSet.h itl_bakDumpEntry.h        \
	   itl_bakCommon.h itl_bakTapeEntry.h itl_bakVolumeEntry.h       \
	   itl_bakDBHeader.h itl_bakListType.h itl_bakCharList.h         \
	   itl_bakCommands.h


libitlbaksrv.a_OFILES = itl_bakPrincipal.o itl_bakTapeSet.o              \
			itl_bakDumpEntry.o itl_bakCommon.o               \
			itl_bakTapeEntry.o itl_bakVolumeEntry.o          \
			itl_bakDBHeader.o itl_bakListType.o              \
			itl_bakCharList.o itl_bakCommands.o

libitlbaksrv.a_EXPDIR = /usr/lib/

EXPLIB_TARGETS = ${LIBRARIES:S/^/export_/g}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
