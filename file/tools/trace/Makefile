# @OSF_COPYRIGHT@
# Copyright (C) 1993 Transarc Corporation - All rights reserved
# $Log$
# $EndLog$

# The trace preprocessor relies on GNU's lexical analyser generator
# flex. To build the trace preprocessor run "build LEX=flex"
#	

PROGRAMS	= tracepp

SCRIPTS		= tracepostp dotrace

DATAFILES	= README RIOSRoutines.pl ICLRoutines.pl common.pl

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>

.SUFFIXES:	.pl

.pl:
	${SED} -e '1s;^#!;&;' -e t\
	       -e 's;^#\(.*\)\@SOURCEWARNING\@;\1${NASF};' -e t\
	       ${${.TARGET}_SED_OPTIONS:U${SED_OPTIONS}}\
	 ${.IMPSRC} > ${.TARGET}.X
	${CHMOD} +x ${.TARGET}.X
	${MV} -f ${.TARGET}.X ${.TARGET}

