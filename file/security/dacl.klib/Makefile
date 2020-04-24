# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.833.1  1996/10/02  18:15:08  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:46:22  damon]
#
# Revision 1.1.825.5  1994/08/23  18:58:50  rsarbo
# 	delegation
# 	[1994/08/23  16:16:27  rsarbo]
# 
# Revision 1.1.825.4  1994/07/13  22:26:53  devsrc
# 	merged with bl-10
# 	[1994/06/28  21:16:31  devsrc]
# 
# 	delegation ACL support
# 	[1994/06/07  14:05:24  delgado]
# 
# Revision 1.1.825.3  1994/06/09  14:18:26  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:30:28  annie]
# 
# Revision 1.1.825.2  1994/02/04  20:28:11  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:17:14  devsrc]
# 
# Revision 1.1.824.1  1993/12/07  17:32:26  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  16:42:31  jaffe]
# 
# Revision 1.1.5.2  1993/01/21  15:16:29  cjd
# 	embedded copyright notice
# 	[1993/01/20  15:52:25  cjd]
# 
# Revision 1.1.3.4  1992/04/14  03:33:49  mason
# 	Transarc delta: bab-ot2288-dacl-source-id-size 1.4
# 	  Files modified:
# 	    security/dacl: epi_id.h; security/dacl.klib: Makefile
# 	    security/dacl2acl: Makefile; security/dacl_lfs: Makefile
# 	    security/dacl_lfs.klib: Makefile
# 	  Selected comments:
# 	    Specification of source principal id size needs to be separated from
# 	    size of stored principal ids, since they aren't always the same.
# 	    Also, should only compare as many bits in a principal id comparison
# 	    as are passed from the external source.
# 	    Make sure that the affected Makefiles set the appropriate flag to indicate
# 	    the size of the source ids.
# 	    Missed one of the Makefiles that need this flag set on the last pass.
# 	    Add -DEPI_SOURCE_IDS_ARE_32BITS to list of CFLAGS.
# 	    NB: if you import this delta, you MUST also import bab-ot2370-dacl-use-all-bits
# 	    if you want the dacl package to work in non-Episode user-space (i.e., in any
# 	    of our servers)!
# 	[1992/04/13  17:12:48  mason]
# 
# Revision 1.1.3.3  1992/01/24  03:48:02  devsrc
# 	Merging dfs6.3
# 	[1992/01/24  00:18:33  devsrc]
# 
# Revision 1.1.3.2  1992/01/23  20:22:21  zeliff
# 	Moving files onto the branch for dfs6.3 merge
# 	[1992/01/23  18:38:30  zeliff]
# 	Revision 1.1.1.3  1992/01/23  22:21:01  devsrc
# 	Fixed logs
# 
# $EndLog$
#
VPATH			= ../dacl

LIBRARIES		= libkdacl.a

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=

CFLAGS 			= ${KERNELCFLAGS} -DEPI_USE_FULL_ID \
			  -DEPI_SOURCE_IDS_ARE_32BITS ${DFS_CORE}
INCFLAGS		= -I../dacl  

libkdacl.a_OFILES	= dacl_debug.o dacl_access.o dacl_pac.o \
			  dacl_flatten.o dacl_manager.o \
		 	  dacl_mgruuids.o dacl_parse.o dacl_permbits.o \
			  dacl_strings.o dacl_default.o dacl_getmode.o \
			  dacl_uuidxlat.o dacl_validate.o dacl_init.o \
			  dacl_linkage.o

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
