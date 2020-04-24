#
# ID: $Id: Makefile,v 1.11 1994/11/16 16:28:03 cfe Exp $
#
# COMPONENT_NAME: bak
#
# The following functions list may not be complete.
# Functions defined by/via macros may not be included.
#
# FUNCTIONS:
#    An automated test suite for bak commands
#
# ORIGINS: Transarc Corp.
#
# (C) COPYRIGHT Transarc Corp. 1993
# All Rights Reserved
# Licensed Materials - Property of Transarc
#
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
#
# HISTORY
# $TALog: Makefile,v $
# Revision 1.11  1994/11/16  16:28:03  cfe
# Fix compilation problems.
# Make all the .include stuff in Makefile files use quotes.
# [from r1.10 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.2]
#
# Revision 1.10  1994/02/25  15:09:35  andi
# [merge of changes from 1.6 to 1.9 into 1.4]
#
# Revision 1.9  1994/02/21  16:30:41  davecarr
# Upgraded tests to work with expect-5.1.3.  Also moved some configuration
# dependencies from restoredisk-cmd to generic.exp.  In the process, some
# variables had "dkr_" prepended to them to indicate that they are used only
# for the restoredisk tests.
#
# Added restoredisk to SUBDIRS.
# [from r1.8 by delta davecarr-db3672-bak-restoredisk-tests-I, r1.3]
#
# Revision 1.8  1994/01/10  18:27:36  davecarr
# Upgraded tests to work with expect-5.1.3.  Also moved some configuration
# dependencies from savedb-cmd to generic.exp.  In the process, some
# variables had "sa_" prepended to them to indicate that they are used only
# for the savedb tests.
#
# Added savedb to SUBDIRS.
# [from r1.7 by delta davecarr-db4737-bak-savedb-tests, r1.2]
#
# Revision 1.7  1994/01/07  18:02:20  davecarr
# Upgraded tests to work with expect-5.1.3.  Also moved some configuration
# dependencies from restoredb-cmd to generic.exp.  In the process, some
# variables had "dbr_" prepended to them to indicate that they are used only
# for the restoredb tests.
#
# Added restoredb to SUBDIRS.
# [from r1.6 by delta davecarr-db4736-bak-restoredb-tests, r1.2]
#
# Revision 1.4  1993/11/15  23:00:08  vijay
# Add the test suite for the bak restoreft command to the test harness.
#
# add the restoreft command
# [from r1.3 by delta vijay-db4692-bak-add-restoreft-tests-to-suite, r1.1]
#
# Revision 1.3  1993/11/15  22:21:41  vijay
# Add the expect script tests for bak dump command. All these scripts reside
# in the dump directory. The descriptions for each of these tests could be
# found in the bak checklist.
#
# add the dump sub-directory
# [from r1.2 by delta vijay-db4691-bak-add-dump-tests-to-suite, r1.1]
#
# Revision 1.2  1993/11/15  21:20:00  vijay
# This revision checks in a working set of scripts to test the DFS backup
# system. The files checked in for this delta are high level scripts to
# configure and run the test suite. Separate deltas would export the scripts
# to test individual bak commands.
#
# The README file provides a description on how to configure and run these tests.
#
# Add the generic.exp file
# [from r1.1 by delta vijay-db3516-add-bak-test-driver, r1.2]
#

DATAFILES	= 	backup_driver.exp util.exp results.exp README \
			generic.exp

ILIST     	= 	${DATAFILES}
SUBDIRS  	=	dump restoreft restoredb savedb restoredisk
DATA_SUBDIRS 	= 	${SUBDIRS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
