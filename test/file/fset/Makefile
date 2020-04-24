#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.116.1  1996/10/17  18:20:35  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:50:53  damon]
#
# Revision 1.1.111.3  1994/02/07  16:28:09  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:22  mckeen]
# 
# Revision 1.1.111.2  1994/02/04  20:40:48  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:22:35  devsrc]
# 
# Revision 1.1.109.1  1993/12/07  17:41:56  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  19:02:22  jaffe]
# 
# Revision 1.1.2.6  1993/04/09  17:15:23  gmd
# 	Added README as to DATAFILES for ILIST so it is also
# 	included in test install tree.
# 	[1993/04/06  20:40:51  gmd]
# 
# Revision 1.1.2.5  1993/01/11  18:27:15  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:01:30  htf]
# 
# Revision 1.1.2.4  1992/11/18  21:16:19  jaffe
# 	Transarc delta: jdp-ot5415-mount-aggregate-on-attach-test 1.1
# 	  Selected comments:
# 	    This delta brings the ftutil tets up to date with the modifications made
# 	    to the ftutil layer in the DFS delta of the same name.  There is also a
# 	    matching Episode delta.
# 	    Add -losi to fset_test's list of libraries.
# 	[1992/11/18  14:34:38  jaffe]
# 
# Revision 1.1.2.3  1992/11/09  21:39:51  jaffe
# 	Transarc delta: jaffe-ot5942-make-test-tree-work 1.1
# 	  Selected comments:
# 	    Now that the test tree installs in dcetest/dce1.0, this delta
# 	    cleans up the Transarc files so that there are no extraneous additions
# 	    to the export tree, and so that all appropriate files are installed.
# 	    Add SUBDIRS, and ILIST.
# 	[1992/11/09  19:02:47  jaffe]
# 
# Revision 1.1.2.2  1992/10/28  16:22:58  jaffe
# 	Transarc delta: jdp-add-itl-stuff 1.8
# 	  Selected comments:
# 	    Add code for three new itl libraries: libitlUtils.a, libitlAggr.a, and
# 	    libitlFset.a
# 	    Update error message in itlu_CheckReturnCode() so that the offending command
# 	    is printed.
# 	    Added documentation for libitlUtils.a, fixed some bugs, and reorganized.
# 	    Changed interface and implementation of result checking routines.
# 	    Provide routine for converting a value back to a name, assuming that
# 	    the value--name mapping was constructed using itlu_ProcessValueMap().
# 	    Checkpointing in order to import another delta.
# 	    Fix misc. compilation problems and close inorder to open another delta.
# 	    Add -lbsd and remove -lcma from link line.
# 	    Make this stuff compile on the pmax.
# 	Transarc delta: jdp-itl-add-fset-driver 1.2
# 	  Selected comments:
# 	    Add the code for fset_test, the fileset (ftutil) test driver.
# 	    Builds fset_test.
# 	    This delta adds "fset" to the list of build directories in the test tree
# 	    and adds all of the test scripts.
# 	[1992/10/27  18:33:00  jaffe]
# 
# $EndLog$

SUBDIRS			= scripts
DATA_SUBDIRS		= scripts

PROGRAMS		= fset_test
DATAFILES		= README runall

EXPDIR			= /usr/include/utils/itl/
EXPLIB_TARGETS		= ${LIBRARIES:S/^/export_/g}

ILIST   = ${PROGRAMS} ${DATAFILES}

CFLAGS			= ${USERCFLAGS} -DITL_FS_TEST
LIBS			= -litlAggr -litlFset -litlFile -litlUtils -litlBomb \
			  -litl -lcom_err -lftutil -lpipe -lafssys -lbomb \
			  -lncompat -lafsutil -licl -losi -ldacl -ldce \
			  -lasyncq ${OS_LIBS}

.if ${TARGET_OS} == "SUNOS5"
LIBS += -lsocket
.endif

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>

