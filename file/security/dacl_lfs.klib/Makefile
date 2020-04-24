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
# Revision 1.1.770.1  1996/10/02  20:58:27  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:47:03  damon]
#
# Revision 1.1.764.5  1994/08/23  18:59:13  rsarbo
# 	delegation
# 	[1994/08/23  16:18:20  rsarbo]
# 
# Revision 1.1.764.4  1994/07/13  22:26:51  devsrc
# 	merged with bl-10
# 	[1994/06/28  21:16:29  devsrc]
# 
# 	delegation ACL support
# 	[1994/06/07  14:05:20  delgado]
# 
# Revision 1.1.764.3  1994/06/09  14:19:08  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:31:08  annie]
# 
# Revision 1.1.764.2  1994/02/04  20:29:17  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:17:42  devsrc]
# 
# Revision 1.1.763.1  1993/12/07  17:33:05  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  16:51:18  jaffe]
# 
# Revision 1.1.4.2  1993/01/21  15:18:22  cjd
# 	embedded copyright notice
# 	[1993/01/20  15:55:45  cjd]
# 
# Revision 1.1.2.2  1992/04/14  03:35:20  mason
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
# 	    Add -DEPI_SOURCE_IDS_ARE_32BITS to list of CFLAGS.
# 	    Missed one of the Makefiles that need this flag set on the last pass.
# 	    NB: if you import this delta, you MUST also import bab-ot2370-dacl-use-all-bits
# 	    if you want the dacl package to work in non-Episode user-space (i.e., in any
# 	    of our servers)!
# 	[1992/04/13  17:14:26  mason]
# 
# Revision 1.1  1992/01/19  02:53:31  devrcs
# 	Initial revision
# 
# $EndLog$
#
VPATH			= ../dacl

LIBRARIES		= libkdacllfs.a

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=

CFLAGS 			= ${KERNELCFLAGS} -DEPI_USE_FULL_ID \
			  -DDACL_EPISODE -DEPI_SOURCE_IDS_ARE_32BITS \
			  ${DFS_CORE} 
INCFLAGS		= -I../dacl
OFILES			= dacl_access.o dacl_pac.o epi_id.o 

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
