# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.8.1  1996/10/02  17:49:19  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:38:55  damon]
#
# Revision /main/HPDCE02/1  1995/06/22  20:13 UTC  maunsell_c
# 	mothra C&S fixes
# 
# Revision /main/brezak_mothra10/1  1995/05/18  15:05 UTC  brezak
# 	This shouldn't be machdep
# 
# Revision 1.1.2.2  1994/08/08  18:54:27  mckeen
# 	Made compile of gateway machine dependent.
# 	[1994/08/08  17:46:31  mckeen]
# 
# Revision 1.1.2.1  1994/07/26  19:11:08  mckeen
# 	Expanded copyrights
# 	[1994/07/26  18:57:27  mckeen]
# 
# 	Integration into 1.1 baseline
# 	[1994/07/26  16:00:55  mckeen]
# 
# $EndLog$
#
# SUBDIR variables defines in machdep.mk file
.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${TARGET_OS}/machdep.mk)
.include "${TARGET_OS}/machdep.mk"
.endif

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

