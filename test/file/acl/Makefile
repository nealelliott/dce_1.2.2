#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# Copyright (C) 1994 Transarc Corporation.  All rights reserved.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.10.1  1996/10/17  18:10:56  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:47:09  damon]
#
# $EndLog$
#

#
#  Definitions for rules using sed
#
NASF=THIS IS NOT A SOURCE FILE - DO NOT EDIT

SUBDIRS   = utils scripts
DATA_SUBDIRS = utils scripts

# XXX Due to SUFFIX rule ordering problems in ODE, driver.pl needed
# to be renamed to testacl. Similar renaming was necessary for 
# all .pl files that had same names as old .sh files.

SCRIPTS = testacl
DATAFILES = README runall
ILIST	  = ${DATAFILES} ${SCRIPTS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

.SUFFIXES: .pl

.pl:
	${SED} -e '1s;^#!;&;' -e t\
	       -e 's;^#\(.*\)\@SOURCEWARNING\@;\1${NASF};' -e t\
	       ${${.TARGET}_SED_OPTIONS:U${SED_OPTIONS}}\
	${.IMPSRC} > ${.TARGET}.X
	${CHMOD} +x ${.TARGET}.X
	${MV} -f ${.TARGET}.X ${.TARGET}

