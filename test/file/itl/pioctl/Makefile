#
# @OSF_COPYRIGHT@
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/10/17  18:28:10  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:54:09  damon]
#
# $EndLog$
#
#  Makefile for DFS ITL pioctl library.
#

LIBRARIES = libitlpioctl.a

EXPDIR = /usr/include/utils/itl/

CCTYPE = ansi

CFLAGS = ${USERCFLAGS} -U_BSD -DITL_FS_TEST

INCLUDES = itl_pioctl.h 

libitlpioctl.a_OFILES = itl_pioctl.o 

libitlpioctl.a_EXPDIR = /usr/lib/

EXPLIB_TARGETS = ${LIBRARIES:S/^/export_/g}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
