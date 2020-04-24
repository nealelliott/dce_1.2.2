#
# ID: $Id: Makefile,v 1.3 1994/11/16 16:28:09 cfe Exp $
#
# COMPONENT_NAME: bak tests
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
# Revision 1.3  1994/11/16  16:28:09  cfe
# Fix compilation problems.
# Make all the .include stuff in Makefile files use quotes.
# [from r1.2 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.2]
#
# Revision 1.2  1993/12/01  19:46:39  vijay
# Two places in the TEST_PRE where pass was unset instead of run
# [from r1.1 by delta vijay-db4692-bak-add-restoreft-tests-to-suite, r1.3]
#
# Revision 1.1  1993/11/15  23:00:09  vijay
# Add the test suite for the bak restoreft command to the test harness.
# [added by delta vijay-db4692-bak-add-restoreft-tests-to-suite, r1.1]
#
#

DATAFILES = restoreft-1.1.1b        restoreft-1.2.2.4b      restoreft-2.2e \
	    restoreft-1.1.2b        restoreft-1.2.2.5b      restoreft-2.3e \
	    restoreft-1.1.3b        restoreft-10.1e         restoreft-2.4e \
	    restoreft-1.1.4b        restoreft-10.2e         restoreft-2.5e \
	    restoreft-1.1.5b        restoreft-10.3e         restoreft-2.6e \
	    restoreft-1.1.6b        restoreft-11.1e         restoreft-7e   \
	    restoreft-1.2.1.1.1b    restoreft-12.1.1e       restoreft-8.1e \
	    restoreft-1.2.1.1.2b    restoreft-12.1.2e       restoreft-8.2e \
	    restoreft-1.2.1.2.1b    restoreft-12.2e         restoreft-9e   \
	    restoreft-1.2.1.2.2b    restoreft-12.3e         restoreft-cmd  \
	    restoreft-1.2.2.1b      restoreft-1e	    		   \
	    restoreft-1.2.2.2b 	    restoreft-2.1e


ILIST     	= 	${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
