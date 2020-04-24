# /* Copyright (C) 1995 Transarc Corporation - All rights reserved. */
# $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/service/RCS/Makefile,v 1.2 1995/04/20 20:41:31 ota Exp $

PROGRAMS 		= strip_dump
SCRIPTS			= display_fset

ILIST			= $PROGRAMS $SCRIPTS
IDIR			= /unsupported/etc/

CFLAGS			= ${USERCFLAGS}


.if exists(../${TARGET_MACHINE}/machdep.mk)
.include "../${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(../${TARGET_OS}/machdep.mk)
.include "../${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

NASF=THIS IS NOT A SOURCE FILE - DO NOT EDIT

.SUFFIXES: .pl

.pl:
	${SED} -e '1s;^#!;&;' -e t\
	       -e 's;^#\(.*\)\@SOURCEWARNING\@;\1${NASF};' -e t\
	       ${${.TARGET}_SED_OPTIONS:U${SED_OPTIONS}}\
	 ${.IMPSRC} > ${.TARGET}.X
	${CHMOD} +x ${.TARGET}.X
	${MV} -f ${.TARGET}.X ${.TARGET}
