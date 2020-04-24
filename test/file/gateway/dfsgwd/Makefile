#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/10/17  18:23:55  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:52:10  damon]
#
# Revision 1.1.2.2  1994/08/22  16:16:31  maunsell_c
# 	add scripts to config/stop/start/unconfig gateway server
# 	[1994/08/10  17:07:21  maunsell_c]
# 
# Revision 1.1.2.1  1994/07/28  17:38:36  mckeen
# 	Initial Revision
# 	[1994/07/28  14:10:35  mckeen]
# 
# $EndLog$

DATAFILES = README runtests test1 test2 add_dfsgwd.sh rem_dfsgwd.sh \
            start_dfsgwd stop_dfsgwd

ILIST = ${DATAFILES}

.if exists($MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>

