#
# ID: $Id: Makefile,v 1.3 1994/11/16 16:28:05 cfe Exp $
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
# Revision 1.3  1994/11/16  16:28:05  cfe
# Fix compilation problems.
# Make all the .include stuff in Makefile files use quotes.
# [from r1.2 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.2]
#
# Revision 1.2  1993/11/15  23:05:29  vijay
# Don't have to install the Makefile
#
# remove Makefile from the list of installed items
# [from r1.1 by delta vijay-db4691-bak-add-dump-tests-to-suite, r1.2]
#
# Revision 1.1  1993/11/15  22:21:44  vijay
# Add the expect script tests for bak dump command. All these scripts reside
# in the dump directory. The descriptions for each of these tests could be
# found in the bak checklist.
# [added by delta vijay-db4691-bak-add-dump-tests-to-suite, r1.1]
#
#

DATAFILES = dump-1.1.5b       dump-1.3.1.2b    dump-3.1.5e \
	    dump-1.1.1b       dump-1.1.6b       dump-1.3.1.3b    dump-3.1.7e \
	    dump-1.1.2.1b     dump-1.2.1b       dump-1.3.1.4b    dump-3.2.1.1e\
	    dump-1.1.2.2.1b   dump-1.2.2b       dump-1e          dump-3.2.1.2e\
	    dump-1.1.2.2.2b   dump-1.2.3b       dump-2.1e        dump-3.2.2e \
	    dump-1.1.3.1b     dump-1.2.4b       dump-2.2e        dump-3.2.3e \
	    dump-1.1.3.2b     dump-1.2.5b       dump-3.1.1e      dump-cmd \
	    dump-1.1.4b       dump-1.3.1.1b     dump-3.1.2e

ILIST     	= 	${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
