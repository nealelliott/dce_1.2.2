# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.83.1  1996/10/02  17:46:54  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:37:24  damon]
#
# Revision 1.1.78.4  1994/09/07  15:48:25  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:21  marty]
# 
# Revision 1.1.78.3  1994/08/08  17:47:18  rsarbo
# 	add ../fsint to IDLINCFLAGS line to prevent missing .acf file if
# 	.idl file is checked out in a sandbox.
# 	[1994/08/08  17:33:56  rsarbo]
# 
# Revision 1.1.78.2  1994/06/09  14:07:46  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:22:46  annie]
# 
# Revision 1.1.78.1  1994/02/04  20:18:40  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:13:02  devsrc]
# 
# Revision 1.1.76.1  1993/12/07  17:24:11  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:22:01  jaffe]
# 
# Revision 1.1.6.3  1993/01/21  19:35:39  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  19:49:17  zeliff]
# 
# Revision 1.1.6.2  1992/11/24  17:03:25  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  17:58:57  bolinger]
# 
# Revision 1.1.3.5  1992/06/30  21:43:27  mason
# 	Add the STUB_OPT_LEVEL variable to control optimization of stub files.
# 	[1992/06/30  21:29:09  mason]
# 
# Revision 1.1.3.4  1992/04/16  19:59:28  mckeen
# 	Removed definition of IDL
# 	[1992/04/15  19:08:34  mckeen]
# 
# Revision 1.1.3.3  1992/01/24  03:14:05  devsrc
# 	Merging dfs6.3
# 	[1992/01/24  00:12:50  devsrc]
# 
# Revision 1.1.3.2  1992/01/23  19:48:25  zeliff
# 	Moving files onto the branch for dfs6.3 merge
# 	[1992/01/23  18:33:12  zeliff]
# 	Revision 1.1.1.3  1992/01/23  22:16:30  devsrc
# 	Fixed logs
# 
# $EndLog$

VPATH			= ../fsint
LIBRARIES		= libkafs4srv.a libkafs4clt.a libkrepcli.a

EXPDIR			= /usr/include/dcedfs/
libkafs4srv.a_EXPDIR	= /usr/lib/
libkafs4clt.a_EXPDIR	= /usr/lib/
libkrepcli.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}


KERNEL_CC		=

CFLAGS			= -Dunix ${KERNELCFLAGS}
INCFLAGS		= -I../fsint 
libkafs4srv.a_OFILES	= afs4int_sstub.o tkn4int_cstub.o 
libkafs4clt.a_OFILES	= afs4int_cstub.o tkn4int_sstub.o
libkrepcli.a_OFILES	= rep_proc_cstub.o


IDLFILES                = rep_proc.idl rep_data.idl afs4int.idl tkn4int.idl 
IDLFLAGS                = -keep c_source -no_cpp -no_mepv
IDLINCFLAGS		= -I. ${INCDIRS} -I../config -I../fsint

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
