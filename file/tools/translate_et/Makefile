# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.549.1  1996/10/02  18:48:43  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:49:17  damon]
#
# Revision 1.1.544.2  1994/06/09  14:23:01  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:34:35  annie]
# 
# Revision 1.1.544.1  1994/02/04  20:33:05  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:19:12  devsrc]
# 
# Revision 1.1.542.1  1993/12/07  17:35:56  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  17:19:02  jaffe]
# 
# Revision 1.1.4.2  1993/01/21  15:55:46  cjd
# 	embedded copyright notice
# 	[1993/01/20  17:39:08  cjd]
# 
# Revision 1.1.2.3  1992/06/08  22:46:35  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Replaced -lbsd with ${BSD_LIBS} variable
# 	[1992/06/08  21:55:34  mason]
# 
# 	Merging OSF porting changes:  added afssys to the end
# 	of the LIBS line.
# 	[1992/01/26  00:22:49  zeliff]
# 
# 	dfs6.3 merge, part2
# 	[1992/01/25  20:10:24  zeliff]
# 
# Revision 1.1.2.2  1992/04/21  16:12:02  mason
# 	Restore the old tranlate_et file's Makefile.
# 	[1992/04/20  20:40:04  mason]
# 
# $EndLog$

PROGRAMS 		= translate_et

.if ${TARGET_OS}=="HPUX"
.if !defined(LPATH) || ${LPATH}==""
   LPATH = :
.endif
.endif

CFLAGS			= -Dunix ${USERCFLAGS}
OFILES			= translate_et.o

LIBS			= -lcmd -ldce ${${TARGET_OS}_LIBS}

UNSUPPORTED		= /unsupported
IDIR 			= ${UNSUPPORTED}/bin/
ILIST 			= ${PROGRAMS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
