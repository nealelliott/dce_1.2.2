#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.143.1  1996/10/02  17:27:41  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:34:07  damon]
#
# $EndLog$
#

EXPLIB_TARGETS		= ${LIBRARIES}
PROGRAMS		= asdb epistat efts esync allocinfo

efts_OFILES		= newft.o efts_userint.o

efts_LIBS		= -lftutil -lpipe -lasyncq -ldacl -lafssys -lcmd -lbomb \
			  -lcom_err -licl -losi -lafsutil -lncompat -ldce \
			  ${OS_LIBS}

LIBS                    = -lafssys -ldce ${OS_LIBS}

CFLAGS			= ${USERCFLAGS}

IDIR			= /bin/
efts_IDIR		= /unsupported/bin/
epistat_IDIR		= /unsupported/bin/
epistle_IDIR		= /unsupported/bin/
ILIST			= asdb efts epistat epistle allocinfo

NASF=THIS IS NOT A SOURCE FILE - DO NOT EDIT

SCRIPTS		= epistle

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
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
