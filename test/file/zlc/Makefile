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
# Revision 1.1.420.1  1996/10/17  18:37:54  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:58:29  damon]
#
# Revision 1.1.415.3  1994/02/07  16:28:26  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:38  mckeen]
# 
# Revision 1.1.415.2  1994/02/04  20:48:06  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:25:53  devsrc]
# 
# Revision 1.1.413.1  1993/12/07  17:46:55  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  19:44:50  jaffe]
# 
# Revision 1.1.2.4  1993/04/09  17:15:50  gmd
# 	Added CRASH_README to ILIST via DATAFILES.
# 	[1993/04/09  00:08:06  gmd]
# 
# Revision 1.1.2.3  1993/01/11  18:38:20  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:24:03  htf]
# 
# Revision 1.1.2.2  1992/11/19  13:37:25  jaffe
# 	Transarc delta: comer-ot5597-add-zlc-tests 1.5
# 	  Selected comments:
# 	    Add a suite of tests for zero linkcount files.
# 	    Install the tests.
# 	    Modified tests to check whether or not the blocks used by
# 	    a file are returned on removal.
# 	    Cleaned up the code that keeps the file open.  Now just uses the
# 	    shell file descriptor mechanism (exec 3< testfile) rather than
# 	    using a sleeping process (duh!).
# 	    Misc cleanup.
# 	    Fixed it to install the tests in the proper directory.
# 	    Clean-up TET usage.
# 	[1992/11/18  17:04:52  jaffe]
# 
# Revision 1.1  1992/01/19  03:18:32  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#

IMODE = 		0755
README_IMODE	=	0644
CRASH_README_IMODE =	0644

DATAFILES =	README CRASH_README

SCRIPTS = 	zlctest utils

ILIST = ${SCRIPTS} ${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
