# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/10/02  17:49:47  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:39:10  damon]
#
# Revision /main/HPDCE02/4  1995/02/14  22:42 UTC  maunsell_c
# 	@sys @host changes for nfs/dfs gateway
# 
# Revision /main/HPDCE02/3  1995/02/02  18:57 UTC  sommerfeld
# 	Fix international build
# 	[1995/02/02  18:43 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_4/1]
# 
# Revision /main/HPDCE02/2  1994/09/21  16:33 UTC  kissel
# 	Merge to OSF bl18.
# 
# Revision 1.1.2.3  1994/09/01  13:45:10  mckeen
# 	Removed improper export of dfsgwymsg.h
# 	[1994/09/01  12:52:45  mckeen]
# 
# Revision /main/HPDCE02/1  1994/09/01  14:23 UTC  maunsell_c
# 	remove export of message catalog include file
# 
# Revision /main/HPDCE02/maunsell_mothra1.4/1  1994/09/01  14:21 UTC  maunsell_c
# 	remove export of message catalog include file.
# 
# Revision 1.1.2.2  1994/08/08  18:54:33  mckeen
# 	Added s12y support
# 	[1994/08/08  17:46:48  mckeen]
# 
# Revision 1.1.2.1  1994/07/26  19:12:59  mckeen
# 	Expanded copyrights
# 	[1994/07/26  18:57:41  mckeen]
# 
# 	Initial Version
# 	[1994/07/26  16:01:48  mckeen]
# 
# $EndLog$


VPATH	= ../dfsgwd

PROGRAMS		= dfsgw

.if ${TARGET_MACHINE} == "SPARC_SOLARIS2"
CFLAGS			= ${USERCFLAGS}
.endif

dfsgw_OFILES		= dfsgw.o dfsgw_userint.o dcecred.o inetid.o \
			  dfsgwymsg.o dfsgwysvc.o

dfsgw_LIBS		= -lcmd -ldce ${OS_LIBS}

.if defined(DES_HIDDEN)
dfsgw_LIBS		:= ../../../libdce/bound_dce_nostrip.o ${dfsgw_LIBS}
.endif

TECHNOLOGY		= dfs
SAMSFILES		= gwy.sams
SAMS_TARGETS 		= dfsgwymsg.h dfsgwymsg.c dfsgwymac.h dfsgwysvc.c \
			  dfsgwysvc.h dfsgwy.cat

ILIST			= ${PROGRAMS}
IDIR			= /bin/

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>

dfsgw.o dcecred.o: dfsgwymsg.h dfsgwymac.h dfsgwysvc.h
