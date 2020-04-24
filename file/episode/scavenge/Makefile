#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.807.1  1996/10/02  17:27:59  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:34:18  damon]
#
# Revision 1.1.800.3  1994/06/09  14:03:20  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:38:22  annie]
# 
# Revision 1.1.800.2  1994/02/04  20:15:21  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:11:22  devsrc]
# 
# Revision 1.1.79.1  1993/12/07  17:20:55  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  14:59:08  jaffe]
# 
# Revision 1.1.4.8  1993/02/01  21:20:33  zeliff
# 	Bringing Elliot's fix to the head of the queue.
# 	[1993/02/01  21:19:56  zeliff]
# 
# Revision 1.1.4.6  1993/02/01  19:14:05  jaffe
# 	Correct 7058
# 	[1993/02/01  16:50:06  jaffe]
# 
# Revision 1.1.4.5  1993/01/28  17:15:21  shl
# 	Transarc delta: blake-ot5950-new-salvager-tests 1.13
# 	  Selected comments:
# 	    The beginnings of a fairly comprehensive test suite for the salvager;
# 	    checkpointed here because of upcoming CML cataclysm.
# 	    Another checkpoint.
# 	    Another checkpoint, since a large number of changes have accumulated.
# 	    Another checkpoint; various new tests and bug fixes.
# 	    Another checkpoint to save recent changes
# 	    just another checkpoint...
# 	    another checkpoint...
# 	    Another checkpoint; more tests, reorganized into module.
# 	    salvager test suite, now ostensibly complete
# 	    Add rules for installing salvager tests
# 	    Fix a couple of typos.
# 	    Fix a couple of bugs; take out perl -w option.
# 	    More silly bugs.
# 	[1993/01/27  18:03:23  shl]
# 
# Revision 1.1.4.4  1993/01/21  18:23:32  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  14:13:23  zeliff]
# 
# Revision 1.1.4.3  1993/01/13  16:57:16  shl
# 	Transarc delta: jdp-ot5924-add-chftids.pl-to-scavenge-dir 1.1
# 	  Selected comments:
# 	    This delta adds a scavenge tool that changes the fileset ID for each fileset
# 	    in an aggregate.  Both the fileset anode and the fileset header are modified.
# 	    This tool will be used with RCX to allow fileset IDs to be changed before
# 	    mounting a copy of a fileset while the original is still mounted.
# 	    Add "chftids" to $(SCRIPTS)
# 	[1993/01/12  20:16:59  shl]
# 
# Revision 1.1.4.2  1992/09/25  16:27:43  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:50:36  jaffe]
# 
# 	Transarc delta: jdp-ot5076-lfs-add-basic_test.pl 1.1
# 	  Selected comments:
# 	    Add basic_test.pl--a place for putting general scavenger tests.
# 	    Add basic_test to list of installed scripts.
# 	[1992/09/23  17:49:29  jaffe]
# 
# Revision 1.1.2.2  1992/05/22  21:15:33  jdp
# 	Transarc delta: jdp-ot3675-lfs-add-salvager-tests 1.1
# 	  Files modified:
# 	    Makefile
# 	    ravage: Makefile, and.c, bash.c, copy.c, copyall.sh
# 	    ravage: metamucil.c, nebconv.pl, pfaults.pl, ravage.sh
# 	    ravage: reravage.sh, rvg_setup.sh, scp.c, setall.sh, swap.c
# 	    ravage: swapall.sh, which.sh, zeroall.sh
# 	    scavenge: Makefile, aggrinfo.pl, efs.pl, efs_query.pl
# 	    scavenge: epidump.pl, fsdump.pl, simple_test.pl, upquota.pl
# 	    scavenge: util.pl
# 	  Selected comments:
# 	    This delta adds the ravager and scavenger tests to the Episode source tree.
# 	    These packages are for testing the Episode salvager.
# 	[1992/05/22  20:25:38  jdp]
# 
# $EndLog$
#

# Copyright (C) 1992, 1991 Transarc Corporation - All rights reserved

#
#  Definitions for rules using sed
#
NASF=THIS IS NOT A SOURCE FILE - DO NOT EDIT

SCRIPTS		= aggrinfo fsdump epidump basic_test simple_test upquota \
		  efs_query efs util chftids \
		  salt ablk aggr anode direct file volume mkaggr

ILIST		= epidump efs efs_query util
IDIR		= /unsupported/bin/

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
