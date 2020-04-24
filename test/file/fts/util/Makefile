#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.411.1  1996/10/17  18:23:18  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:51:49  damon]
#
# Revision 1.1.406.1  1993/12/28  18:01:43  delgado
# 	use quotes for MAKETOP include instead of brackets
# 	[1993/12/28  17:58:23  delgado]
# 
# Revision 1.1.2.4  1993/01/11  18:29:16  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:06:28  htf]
# 
# Revision 1.1.2.3  1992/11/09  21:40:07  jaffe
# 	Transarc delta: jaffe-ot5942-make-test-tree-work 1.1
# 	  Selected comments:
# 	    Now that the test tree installs in dcetest/dce1.0, this delta
# 	    cleans up the Transarc files so that there are no extraneous additions
# 	    to the export tree, and so that all appropriate files are installed.
# 	    Add ILIST
# 	[1992/11/09  19:05:25  jaffe]
# 
# Revision 1.1.2.2  1992/11/04  19:22:17  jaffe
# 	Transarc delta: mcinerny-ot5920-Fix-fts-Makefile 1.1
# 	  Selected comments:
# 	    Need to install test1-16, and the util files.
# 	    Install the util files.
# 	[1992/11/04  17:39:25  jaffe]
# 
# Revision 1.1  1992/01/19  03:19:16  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#

DATAFILES = 	aggr.clean fldb.clean sentry.clean

ILIST   = ${DATAFILES}



.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
