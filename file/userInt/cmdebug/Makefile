#
# @OSF_COPYRIGHT@
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.5.1  1996/10/02  21:16:55  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:50:28  damon]
#
# $EndLog$
#

VPATH                   = ../../fsint
IDLFILES                = tkn4int.idl
IDLFLAGS                = -keep c_source -v -no_cpp -no_mepv
IDLINCFLAGS             = -I.

PROGRAMS 		= cm_debug

CFLAGS			= -Dunix ${USERCFLAGS}

cm_debug_OFILES		= cm_debug.o tkn4int_cstub.o

LIBS			= -lcommondata -lcmd -ldce -lcom_err ${OS_LIBS}

IDIR 			= /bin/
ILIST 			= ${PROGRAMS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>
