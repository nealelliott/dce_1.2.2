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
# Revision 1.1.110.1  1996/10/17  18:34:41  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:57:15  damon]
#
# Revision 1.1.105.1  1993/12/28  18:01:58  delgado
# 	use quotes for MAKETOP include instead of brackets
# 	[1993/12/28  17:58:38  delgado]
# 
# Revision 1.1.9.2  1993/07/19  19:45:25  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:34:02  zeliff]
# 
# Revision 1.1.7.3  1993/07/16  22:51:12  kissel
# 	Put HP RP work on the 1.0.3i branch.
# 	[1993/06/21  15:14:25  kissel]
# 
# Revision 1.1.5.2  1993/06/04  20:53:47  kissel
# 	Initial HPUX RP version.
# 	[1993/06/03  22:48:37  kissel]
# 
# Revision 1.1.2.5  1993/04/09  17:15:37  gmd
# 	Added ILIST so tests would be installed.
# 	[1993/04/08  23:18:17  gmd]
# 
# Revision 1.1.3.2  1993/03/29  21:35:23  maunsell_c
# 	03/16/93	maunsell_c	add README file to DATAFILES list
# 	[1993/03/29  21:07:28  maunsell_c]
# 
# Revision 1.1.2.4  1993/01/11  18:36:41  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:20:31  htf]
# 
# Revision 1.1.2.3  1992/12/09  21:33:37  jaffe
# 	Transarc delta: cfe-ot5994-get-rep-tests-working 1.4
# 	  Selected comments:
# 	    Lots of changes to make these real.
# 	    Not ready for the real world yet.
# 	    Changed names of some tests.
# 	    fix errors
# 	    Continuing on an unexpected failure.
# 	    Found the idiot bug!
# 	[1992/12/07  13:38:42  jaffe]
# 
# Revision 1.1.2.2  1992/11/19  13:35:48  jaffe
# 	Transarc delta: cfe-ot5695-move-tests-to-rep 1.2
# 	  Selected comments:
# 	    Create a house for replication tests.
# 	    New house.
# 	    Forgot to add new subdir to the higher-level Makefile.
# 	[1992/11/18  16:44:39  jaffe]
# 
# Revision 1.1  1992/01/19  03:19:16  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#

DATAFILES = README rtest1 rtest2 rtest3 rtest5 rtest6 runtests statusCheck runall
PROGRAMS = rstest1_aux mkfile
ILIST	= ${DATAFILES} ${PROGRAMS}

CFLAGS = ${USERCFLAGS}
LIBS =

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
