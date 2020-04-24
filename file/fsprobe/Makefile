# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.20.1  1996/10/02  17:47:13  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:37:39  damon]
#
# Revision 1.1.15.3  1994/08/01  18:49:36  ruby
# 	Implement partial S12Y (Internationaliztion) for DFS commands and daemons
# 	[1994/08/01  18:41:29  ruby]
# 
# Revision 1.1.15.2  1994/06/09  14:08:06  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:23:00  annie]
# 
# Revision 1.1.15.1  1994/02/04  20:18:54  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:13:08  devsrc]
# 
# Revision 1.1.13.1  1993/12/07  17:24:27  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:24:08  jaffe]
# 
# Revision 1.1.7.5  1993/01/21  19:36:22  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  19:50:09  zeliff]
# 
# Revision 1.1.7.4  1993/01/13  17:49:54  shl
# 	Transarc delta: kazar-ot6523-uuid-encodes-primary-secondary-pool-selection 1.3
# 	  Selected comments:
# 	    some changes to prevent deadlock between primary/secondary thread pools
# 	    Changes basic connection protocol.
# 	    fsprobe now depends on libdfsncs.a
# 	    fix confusion about different types of sockaddr_ins
# 	[1993/01/12  20:59:18  shl]
# 
# Revision 1.1.7.3  1992/11/24  17:48:32  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  17:59:41  bolinger]
# 
# Revision 1.1.7.2  1992/08/31  19:46:23  jaffe
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
# 	    Remove -lcom_err in the file
# 	    Add -lcom_err to LIBS.
# 	[1992/08/30  02:28:19  jaffe]
# 
# Revision 1.1.3.9  1992/06/08  22:45:19  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Replaced -lbsd with ${BSD_LIBS} variable
# 	[1992/06/08  21:35:52  mason]
# 
# Revision 1.1.3.8  1992/05/05  19:00:40  rec
# 	Removed dependence on libkrb5.a by allowing libdce.a
# 	to export required symbol (krb5_cksumarray).
# 	[1992/05/05  18:54:58  rec]
# 
# Revision 1.1.3.7  1992/04/07  20:27:13  rec
# 	Have -lkrb5 on link line depend on RIOS platform since
# 	it is not needed with a shared libdce on OSF/1.
# 	[1992/04/07  19:58:51  rec]
# 
# Revision 1.1.3.6  1992/01/30  14:49:11  carl
# 	added lib krb5 for linking phase
# 	[1992/01/30  14:48:43  carl]
# 
# Revision 1.1.3.5  1992/01/26  00:26:22  zeliff
# 	Merging OSF porting changes:  moved -lafssys to end of
# 	LIBS line to resolve reference to setpag.
# 	[1992/01/25  22:50:42  zeliff]
# 
# Revision 1.1.3.4  1992/01/25  20:47:42  zeliff
# 	dfs6.3 merge, part2
# 	[1992/01/25  20:05:39  zeliff]
# 
# $EndLog$

INCLUDES		= fsprobe.h

LIBRARIES		= libfsprobe.a

EXPDIR			= /usr/include/dcedfs/
libfsprobe.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

TECHNOLOGY=dfs

SAMSFILES = fsp.sams
SAMS_TARGETS = dfsfspmsg.h dfsfspmsg.c dfsfspmac.h dfsfspsvc.c dfsfspsvc.h \
    dfsfsp.cat

ILIST			= dfsfsp.cat
dfsfsp.cat_IDIR		= /nls/msg/en_US.ASCII/

CFLAGS			= -Dunix -DDEBUG ${USERCFLAGS}

libfsprobe.a_OFILES	= fsprobe.o  dfsfspsvc.o dfsfspmsg.o

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
