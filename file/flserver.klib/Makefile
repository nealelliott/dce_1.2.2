# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.77.1  1996/10/02  17:45:42  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:36:48  damon]
#
# Revision 1.1.72.3  1994/09/07  15:48:23  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:19  marty]
# 
# Revision 1.1.72.2  1994/06/09  14:06:45  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:22:01  annie]
# 
# Revision 1.1.72.1  1994/02/04  20:17:41  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:12:33  devsrc]
# 
# Revision 1.1.70.1  1993/12/07  17:23:23  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:14:27  jaffe]
# 
# Revision 1.1.5.2  1993/01/21  19:33:21  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  19:46:53  zeliff]
# 
# Revision 1.1.3.8  1992/07/03  01:53:44  mason
# 	Transarc delta: cburnett-ot4362-flserver-klib-makefile 1.1
# 	  Selected comments:
# 	    the flserver.klib/Makefile does not need to export includes as
# 	    this is done by the flserver/Makefile.
# 	    Removed export of includes
# 	[1992/07/02  19:00:05  mason]
# 
# Revision 1.1.3.7  1992/07/02  21:51:52  mason
# 	Transarc delta: cburnett-ot4362-flserver-klib-makefile 1.1
# 	  Selected comments:
# 	    the flserver.klib/Makefile does not need to export includes as
# 	    this is done by the flserver/Makefile.
# 	    Removed export of includes
# 	[1992/07/02  19:00:05  mason]
# 
# Revision 1.1.3.6  1992/06/30  21:43:16  mason
# 	Add the STUB_OPT_LEVEL variable to control optimization of stub files.
# 	[1992/06/30  21:28:37  mason]
# 
# Revision 1.1.3.5  1992/05/20  19:50:48  mason
# 	Transarc delta: cfe-ot3085-fix-msf-dependencies 1.1
# 	  Files modified:
# 	    bak: Makefile; bakserver: Makefile; bosserver: Makefile
# 	    bubasics: Makefile; flserver: Makefile
# 	    flserver.klib: Makefile; fshost: Makefile; fsint: Makefile
# 	    ftserver: Makefile; host: Makefile; ncscompat: Makefile
# 	    ncsubik: Makefile; security/dacl: Makefile
# 	    security/dfsauth: Makefile; sysacl: Makefile; tkm: Makefile
# 	    tools/cmd: Makefile; update: Makefile; xcred: Makefile
# 	    xvolume: Makefile
# 	  Selected comments:
# 	    Make explicit the dependency that builds an .msf file from an .et file.
# 	    Add dependency to build .msf file.
# 	[1992/05/20  11:12:33  mason]
# 
# Revision 1.1.3.4  1992/04/16  19:59:21  mckeen
# 	Removed definition of IDL
# 	[1992/04/15  19:08:17  mckeen]
# 
# Revision 1.1.3.3  1992/01/24  04:23:46  devsrc
# 	Merging dfs6.3
# 	[1992/01/24  00:12:03  devsrc]
# 
# Revision 1.1.3.2  1992/01/23  19:47:02  zeliff
# 	Moving files onto the branch for dfs6.3 merge
# 	[1992/01/23  18:32:04  zeliff]
# 	Revision 1.1.1.3  1992/01/23  22:15:42  devsrc
# 	Fixed logs
# 
# Revision 1.1  1992/01/19  02:59:22  devrcs
# 	Initial revision
# 
# $EndLog$
#

VPATH 			= ../flserver

LIBRARIES		= libkfldb.a

libkfldb.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=
CFLAGS			= -Dunix ${KERNELCFLAGS}

OFILES	 		= afsvl_proc_cstub.o

INCFLAGS		= -I../flserver

IDLFILES		= afsvl_proc.idl  

IDLFLAGS		= -keep c_source -no_cpp -no_mepv 
IDLINCFLAGS		= ${INCDIRS} -I../config -I. -I../flserver

# On some platforms the optimizer is very expensive relative to the
# actual gain.  We disable it on a per-platform basis.
OPT_LEVEL		+= ${STUB_OPT_LEVEL}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

flserver.h fl_errors.c: flserver.p.h fl_errors.et
	-${CP} ${flserver.p.h:P} .
	${COMPILE_ET} ${fl_errors.et:P} -h flserver

