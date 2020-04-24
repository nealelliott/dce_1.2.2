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
# Revision 1.1.113.5  1994/06/09  14:20:04  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:31:45  annie]
#
# Revision 1.1.113.4  1994/05/23  23:13:25  sommerfeld
# 	Fix submission of last Friday (EXPSHLIB)
# 	[1994/05/23  23:07:51  sommerfeld]
# 
# Revision 1.1.113.3  1994/05/20  22:03:26  sommerfeld
# 	comment out EXPSHLIB_TARGETS to avoid confusing export.
# 	[1994/05/17  20:23:53  sommerfeld]
# 
# Revision 1.1.113.2  1994/02/04  20:29:58  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:18:01  devsrc]
# 
# Revision 1.1.111.1  1993/12/07  17:33:39  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  16:57:09  jaffe]
# 
# Revision 1.1.4.4  1993/01/21  15:20:17  cjd
# 	embedded copyright notice
# 	[1993/01/20  15:59:12  cjd]
# 
# Revision 1.1.4.3  1992/11/20  20:14:02  rsarbo
# 	Need to link libdce on build of lclcalls.c to prevent
# 	text relocation warnings.
# 	[1992/11/20  19:35:19  rsarbo]
# 
# Revision 1.1.4.2  1992/11/12  18:12:10  rsarbo
# 	incorporate lclcalls.o to libdce on PMAX
# 	for DFS Episode ACL support
# 	[1992/11/12  18:02:34  rsarbo]
# 
# Revision 1.1  1992/01/19  02:56:07  devrcs
# 	Initial revision
# 
# $EndLog$
#


#
# the rest of the makefile is "private"
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
LIBRARIES		= libafssys.a

LIBS=-ldce

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}
EXPSHLIB_TARGETS        = ${OFILES}
CFLAGS			= ${USERCFLAGS}
OFILES			= lclcalls.o

.if ${TARGET_OS} == "HPUX"
IN_SHLIB                = 1
.endif

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

.else

.if ${TARGET_MACHINE} == "RIOS"

.if exists (${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif

.if exists (${TARGET_OS}/Makefile)
.include "${TARGET_OS}/Makefile"
.endif


.else

.if exists (${MAKETOP}/file/sys/${TARGET_MACHINE}/Makefile)
.include "${MAKETOP}/file/sys/${TARGET_MACHINE}/Makefile"
.endif

.if exists (${MAKETOP}/file/sys/${TARGET_OS}/Makefile)
.include "${MAKETOP}/file/sys/${TARGET_OS}/Makefile"
.endif

.endif

libafssys_OFILES = lclcalls.o ${OFILES}
libafssys.a_OFILES = ${libafssys_OFILES}
.endif
