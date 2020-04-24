#
# Copyright (C) 1996, 1993  Transarc Corporation - All rights reserved 
#
#  
#  Makefile --
#	For the test/file/micro directory.
#

SUBDIRS =
DATA_SUBDIRS =

DATAFILES = README runtests

PROGRAMS = micro1 micro2 micro3 micro3pre micro4 micro5 micro6 microio

ILIST = ${PROGRAMS} ${DATAFILES}

CCTYPE = ansi

CFLAGS = ${USERCFLAGS} -O

micro1_OFILES = micro1.o aux.o
micro1_LIBS = ${OS_LIBS}

micro2_OFILES = micro2.o aux.o
micro2_LIBS = ${OS_LIBS}

micro3_OFILES = micro3.o aux.o
micro3_LIBS = ${OS_LIBS}

micro3pre_OFILES = micro3pre.o aux.o
micro3_LIBS = ${OS_LIBS}

micro4_OFILES = micro4.o aux.o
micro4_LIBS = ${OS_LIBS}

micro5_OFILES = micro5.o aux.o
micro5_LIBS = ${OS_LIBS}

micro6_OFILES = micro6.o aux.o
micro6_LIBS = ${OS_LIBS}

microio_OFILES = microio.o aux.o
microio_LIBS = ${OS_LIBS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
