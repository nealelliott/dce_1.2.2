# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.22.1  1996/10/02  17:03:05  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:04:33  damon]
#
# Revision 1.1.16.2  1994/06/09  13:51:33  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:25:01  annie]
# 
# Revision 1.1.16.1  1994/02/04  20:05:40  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:06:24  devsrc]
# 
# Revision 1.1.14.1  1993/12/07  17:12:31  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  12:52:07  jaffe]
# 
# Revision 1.1.6.3  1993/01/18  19:54:19  cjd
# 	Embedded copyright notice
# 	[1993/01/18  19:29:56  cjd]
# 
# Revision 1.1.6.2  1992/08/31  18:23:03  jaffe
# 	Transarc delta: jess-ot2869-com_err-usage-cleanup 1.5
# 	  Selected comments:
# 	    The use of com_err in dfs is inconsistent and in some cases
# 	    incorrect.
# 	    1) there are many sources which include <com_err.h> but don't
# 	    use any definitions from it.
# 	    2) there are sources which use com_err, but don't include the
# 	    <com_err.h>
# 	    3) there are subdirectories which do not need libcom_err.a for
# 	    any reason that link with it.
# 	    This delta is created to clean up the usage of com_err in dfs.
# 	    Remove -lcom_err in the var budb_test_LIBS
# 	    Add -lcom_err to LIBS.
# 	[1992/08/28  20:45:03  jaffe]
# 
# Revision 1.1.3.10  1992/06/08  22:42:15  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Replaced -lbsd with ${BSD_LIBS} variable
# 	[1992/06/08  21:12:16  mason]
# 
# Revision 1.1.3.9  1992/05/05  19:00:22  rec
# 	Removed dependence on libkrb5.a by allowing libdce.a
# 	to export required symbol (krb5_cksumarray).
# 	[1992/05/05  18:53:41  rec]
# 
# Revision 1.1.3.8  1992/04/21  12:18:45  mason
# 	Transarc delta: cburnett-ot2665-bakutil_changes_for_AIX32 1.1
# 	  Files modified:
# 	    bakutil: Makefile, budb_test.c
# 	  Selected comments:
# 	    changes so bakutil will compile on AIX 32
# 	    Remove BSD_INCLUDES for fms.o
# 	[1992/04/20  21:28:20  mason]
# 
# Revision 1.1.3.7  1992/04/07  20:26:50  rec
# 	Have -lkrb5 on link line depend on RIOS platform since
# 	it is not needed with a shared libdce on OSF/1.
# 	[1992/04/07  19:54:43  rec]
# 
# Revision 1.1.3.6  1992/03/30  17:05:41  garyf
# 	fix library order for OSF/1
# 	[1992/03/30  17:04:34  garyf]
# 
# Revision 1.1.3.5  1992/01/26  00:26:11  zeliff
# 	Merging in OSF porting changes: removed -lafssys from
# 	budb_test_LIBS.
# 	[1992/01/25  22:19:46  zeliff]
# 
# Revision 1.1.3.4  1992/01/25  20:43:56  zeliff
# 	dfs6.3 merge, part2
# 	[1992/01/25  19:59:36  zeliff]
# 
# $EndLog$
#

PROGRAMS        	= fms

IDIR 			= /bin/
ILIST 			= fms

fms.o_CFLAGS		= -D_VOPS -Daiws -D__unix ${USERCFLAGS}
CFLAGS			= -D_VOPS -D_BSD_INCLUDES -Daiws -D__unix ${USERCFLAGS}
fms_OFILES		= fms.o

fms_LIBS            	= -lcmd -lcurses -ltermcap -ldce ${OS_LIBS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
