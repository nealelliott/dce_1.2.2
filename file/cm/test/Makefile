#
# Copyright (C) 1995, 1994 Transarc Corporation
# All rights reserved.
#

VPATH			= ../

PROGRAMS		= cm_preftest

SCRIPTS			= test_preflock generate_prefs

CFLAGS			= ${USERCFLAGS} -DCM_USERTEST -I${MAKETOP}../../src/file/cm -I${SOURCEDIR}/file/cm

cm_preftest_OFILES	= cm_preftest.o cm_serverpref.o cm_server.o \
			  cm_cell.o cm_rrequest.o cm_volume.o cm_site.o
cm_preftest_LIBS	= -ltpq -lafssys -licl -lncompat -lafsutil \
			  -ldfsncs -losi -ldce ${OS_LIBS}

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

.SUFFIXES: .pl

.pl:
	${SED} -e '1s;^#!;&;' -e t\
	       -e 's;^#\(.*\)\@SOURCEWARNING\@;\1${NASF};' -e t\
	       ${${.TARGET}_SED_OPTIONS:U${SED_OPTIONS}}\
        ${.IMPSRC} > ${.TARGET}.X
	${CHMOD} +x ${.TARGET}.X
	${MV} -f ${.TARGET}.X ${.TARGET}
