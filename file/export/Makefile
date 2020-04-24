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
# Revision 1.1.41.1  1996/10/02  17:45:26  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:36:38  damon]
#
# Revision 1.1.35.3  1994/06/09  14:06:26  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:21:52  annie]
# 
# Revision 1.1.35.2  1994/02/04  20:17:34  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:12:31  devsrc]
# 
# Revision 1.1.33.1  1993/12/07  17:23:18  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:13:57  jaffe]
# 
# Revision 1.1.2.2  1993/01/21  19:32:31  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  19:46:17  zeliff]
# 
# Revision 1.1  1992/01/19  02:41:11  devrcs
# 	Initial revision
# 
# $EndLog$
#

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
