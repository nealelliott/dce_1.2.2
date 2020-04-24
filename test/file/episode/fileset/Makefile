#
# @OSF_COPYRIGHT@
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.5.1  1996/10/17  18:19:13  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:50:24  damon]
#
# $EndLog$
#
# Copyright (C) 1993 Transarc Corporation - All rights reserved
#

SCRIPTS 	= fsettestbasic fsettestfunc fsettest00 fsettest01 \
		  fsettest02 fsettest03

DATAFILES 	= README

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>

.SUFFIXES: .sh

.sh:
	${SED} -e '1s;^#!;&;' -e t\
	       -e 's;^#\(.*\)\@SOURCEWARNING\@;\1${NASF};' -e t\
	       ${${.TARGET}_SED_OPTIONS:U${SED_OPTIONS}}\
	 ${.IMPSRC} > ${.TARGET}.X
	${CHMOD} +x ${.TARGET}.X
	${MV} -f ${.TARGET}.X ${.TARGET}

.csh:
	${SED} -e '1s;^#!;&;' -e t\
	       -e 's;^#\(.*\)\@SOURCEWARNING\@;\1${NASF};' -e t\
	       ${${.TARGET}_SED_OPTIONS:U${SED_OPTIONS}}\
	 ${.IMPSRC} > ${.TARGET}.X
	${CHMOD} +x ${.TARGET}.X
	${MV} -f ${.TARGET}.X ${.TARGET}
