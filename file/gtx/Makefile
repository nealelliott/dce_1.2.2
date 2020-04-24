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
# Revision 1.1.74.1  1996/10/02  17:50:38  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:39:42  damon]
#
# Revision 1.1.69.3  1994/08/03  16:08:01  ruby
# 	Implement partial S12Y (Internationalization) for DFS commands and daemons
# 	[1994/08/01  16:53:15  ruby]
# 
# Revision 1.1.69.2  1994/06/09  14:09:41  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:24:19  annie]
# 
# Revision 1.1.69.1  1994/02/04  20:20:16  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:13:51  devsrc]
# 
# Revision 1.1.67.1  1993/12/07  17:26:19  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:30:57  jaffe]
# 
# Revision 1.1.5.4  1993/01/21  19:38:29  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  19:52:33  zeliff]
# 
# Revision 1.1.5.3  1992/11/24  17:50:56  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  18:04:31  bolinger]
# 
# Revision 1.1.5.2  1992/08/31  19:57:55  jaffe
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
# 	    Remove -lcom_err in the var LIBS
# 	[1992/08/30  02:48:30  jaffe]
# 
# Revision 1.1.3.3  1992/06/08  22:45:34  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Replaced -lbsd with ${BSD_LIBS} variable
# 	[1992/06/08  21:38:28  mason]
# 
# Revision 1.1.3.2  1992/05/20  19:54:05  mason
# 	Transarc delta: mason-ot3289-fix-gtx-Makefile 1.1
# 	  Files modified:
# 	    gtx: Makefile
# 	  Selected comments:
# 	    gtxtest would not link.
# 	    Added a missing library (-llwpdummy) to the line.
# 	Transarc delta: vijay-ot3289-gtx-fix-lock-calls 1.2
# 	  Files modified:
# 	    gtx: Makefile, X11windows.c, dumbwindows.c, frame.c
# 	    gtx: gtxtextcb.h, input.c, lightobject.c, objects.c
# 	    gtx: textcb.c, textobject.c
# 	  Selected comments:
# 	    change the lock_ calls to afslk_ calls.
# 	    Use the ncompat library instead of osi and lwpdummy
# 	    remove a spurious cml diff line. This line was not deleted upon a merge
# 	    and gave some compilations problems.
# 	[1992/05/20  11:33:59  mason]
# 
# Revision 1.1  1992/01/19  02:41:28  devrcs
# 	Initial revision
# 
# $EndLog$
#
INCLUDES		= gtxobjects.h gtxwindows.h gtxcurseswin.h gtxinput.h \
			  gtxdumbwin.h gtxX11win.h gtxkeymap.h gtxlightobj.h \
			  gtxtextobj.h gtxframe.h gtxobjdict.h gtxtextcb.h

LIBRARIES		= libgtx.a
PROGRAMS		= gtxtest screen_test curses_test cb_test # object_test requires gtxscreenobj.h

EXPDIR			= /usr/include/dcedfs/
libgtx.a_EXPDIR		= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

TECHNOLOGY=dfs

SAMSFILES = gtx.sams
SAMS_TARGETS = dfsgtxmsg.h dfsgtxmsg.c dfsgtxmac.h dfsgtxsvc.c dfsgtxsvc.h \
    dfsgtx.cat

ILIST			= dfsgtx.cat
dfsgtx.cat_IDIR		= /nls/msg/en_US.ASCII/

CFLAGS			= -Dunix ${USERCFLAGS}

libgtx.a_OFILES		= keymap.o input.o frame.o \
			  curseswindows.o dumbwindows.o X11windows.o windows.o \
			  objects.o lightobject.o textobject.o \
			  objdict.o textcb.o  dfsgtxsvc.o dfsgtxmsg.o

gtxtest_OFILES	 	= gtxtest.o
object_test_OFILES	= object_test.o
screen_test_OFILES	= screen_test.o
curses_test_OFILES	= curses_test.o
cb_test_OFILES		= cb_test.o

LIBS			= -lgtx -lafssys -lcmd -lafsutil \
			  -losi -lcurses -ldce ${OS_LIBS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
