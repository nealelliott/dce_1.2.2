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
# Revision 1.1.10.1  1996/10/17  18:35:07  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:57:25  damon]
#
# Revision 1.1.2.4  1993/01/11  18:37:08  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:21:37  htf]
# 
# Revision 1.1.2.3  1992/11/09  21:40:14  jaffe
# 	Transarc delta: jaffe-ot5942-make-test-tree-work 1.1
# 	  Selected comments:
# 	    Now that the test tree installs in dcetest/dce1.0, this delta
# 	    cleans up the Transarc files so that there are no extraneous additions
# 	    to the export tree, and so that all appropriate files are installed.
# 	    Don't export the files, let them be installed.
# 	[1992/11/09  19:07:07  jaffe]
# 
# Revision 1.1.2.2  1992/10/28  18:15:57  jaffe
# 	Transarc delta: rajesh-ot5367-salvage-recovery-authorization-check-tests 1.8
# 	  Selected comments:
# 	    To add authorization check tests for salvage/recovery. This is
# 	    the initial checkpoint.
# 	    Create the Makefile and add AuthCheckTest to it.
# 	    Just a checkpoint.
# 	    Another checkpoint.
# 	    Checkpoint.
# 	    This delta version has been tested.
# 	    Changed permissions.
# 	    Update makefiles.
# 	    See above.
# 	    Fixed logical errors in the script pertaining to requiring read/write
# 	    access to aggregate to perform salvage and/or recovery, handling
# 	    of error counts generated in the AuthCheckTest.noroot script.
# 	    Fix a build-time error.
# 	    Put a trailing slash (/) on EXPDIR.
# 	[1992/10/28  15:04:32  jaffe]
# 
# Revision 1.1  1992/01/19  03:17:21  devrcs
# 	Initial revision
# 
# $EndLog$
#

DATAFILES = AuthCheckTest AuthCheckTest.noroot \
	    regression_test all_salvage misc_salvage cmp_salvage

PROGRAMS = 

IMODE = 0755

ILIST   = ${PROGRAMS} ${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
