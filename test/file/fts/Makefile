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
# Revision 1.1.810.1  1996/10/17  18:22:15  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:51:38  damon]
#
# Revision 1.1.805.1  1993/12/28  18:01:41  delgado
# 	use quotes for MAKETOP include instead of brackets
# 	[1993/12/28  17:58:22  delgado]
# 
# Revision 1.1.6.8  1993/04/09  17:15:26  gmd
# 	Added README as to DATAFILES for ILIST so it is also
# 	included in test install tree.
# 	[1993/04/06  20:41:17  gmd]
# 
# Revision 1.1.6.7  1993/01/13  20:25:57  shl
# 	Transarc delta: jdp-ot4575-directory-dump 1.7
# 	  Selected comments:
# 	    This delta contains the DFS changes necessary to accomplish a filesystem
# 	    independent directory dump format.  This includes the generic dump additions
# 	    in the ftserver and xvolume modules.  I've also stubbed in the new
# 	    UFS volops for someone else to fill in later.
# 	    This revision is merely a checkpoint to allow for the impending cml
# 	    cataclysm.
# 	    This revision is just another checkpoint.  It contains most of the Episode
# 	    changes.
# 	    This is another checkpoint to allow other deltas to be opened/imported.
# 	    Another checkpoint.  Everything seems to be working for Episode->Episode
# 	    dumps on the RIOS.
# 	    Fix compiler warning on the pmax.
# 	    This delta contains a new test for the directory dump changes and also
# 	    contains some of the intertwined volume sync changes.
# 	    Add "bigdir" and "test17".
# 	    Edited this delta to bring it into the internationalization fold, where
# 	    filenames carry codeset tags.
# 	Transarc delta: vijay-ot5614-fts-add-new-tests-to-suite 1.10
# 	  Selected comments:
# 
# 	    This delta captures all the new tests added to the fts test suite. The new
# 	    tests are added from the checklist.
# 	    Adding a new test for fts move, and some changes to test19
# 	    Fixed some problems in test17, mostly minor. Now that the test and src
# 	    deltas are in one place, I have to switch a lot more.
# 	    Some more changes to get test20 to pass. It still doesn't because of some
# 	    move problems, but have to close this delta to move on. Some more iterations
# 	    necessary to get test18 to work.
# 	    Included test17, 18, 19 and 20.
# 	    Get test18 and test20 to work. Test20 works fine, but there are a few more
# 	    problems with test18.
# 	    Yet another iteration in getting these tests to work. This version passes on
# 	    rios in one and two machine cases.
# 	    There are some problems with the management of the in-core fileset inconsistent
# 	    bit that affect test19. Jeff has since fixed these problems. This test now
# 	    calls lsheader after each restore to refresh the in-core inconsistent bit.
# 	    This will not be required later on when Jeff's delta is included.
# 	    Jeff Prem has test17 exported already. This revision moves my four tests from
# 	    18 through 21. The Makefile is changed appropriately. Take care when merging
# 	    this delta with Jeff's.
# 	    Add test21.
# 	    Forgot to modify the runtests driver to run the new tests.
# 	    Remove test17 from Makefile since it is not in the directory now.
# 	[1993/01/13  13:18:18  shl]
# 
# Revision 1.1.6.6  1993/01/11  18:28:06  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:03:53  htf]
# 
# Revision 1.1.6.5  1992/11/09  21:40:05  jaffe
# 	Transarc delta: jaffe-ot5942-make-test-tree-work 1.1
# 	  Selected comments:
# 	    Now that the test tree installs in dcetest/dce1.0, this delta
# 	    cleans up the Transarc files so that there are no extraneous additions
# 	    to the export tree, and so that all appropriate files are installed.
# 	    Add ILIST
# 	[1992/11/09  19:04:49  jaffe]
# 
# Revision 1.1.6.4  1992/11/04  19:22:12  jaffe
# 	Transarc delta: mcinerny-ot5920-Fix-fts-Makefile 1.1
# 	  Selected comments:
# 	    Need to install test1-16, and the util files.
# 	    Install test14-16, and not the etests.
# 	[1992/11/04  17:39:06  jaffe]
# 
# Revision 1.1.6.3  1992/11/02  15:20:23  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:16:00  root]
# 
# Revision 1.1.6.2  1992/10/29  20:17:22  jaffe
# 	Correct build errors by removing etest exported files.
# 	[92/10/29            jaffe]
# 
# Revision 1.1.2.2  1992/05/21  12:41:50  jaffe
# 	Transarc delta: fred-1028-fix-ufs 1.1
# 	  Selected comments:
# 	    Fixes for defect 1028.  Test 4-7 should now report correct outcomes on
# 	    both an LFS or UFS file system.  A shell function, dfs_ftsCheckResult
# 	    was added, and is used to verify the outcome of fts commands.  This function
# 	    hides any underlying file system dependencies.
# 	    Now sources the ftsStatusCheck file and uses dfs_ftsCheckResult to verify
# 	    fts command statuses.
# 	Transarc delta: fred-merge-tests 1.1
# 	  Selected comments:
# 	    Created this delta to merge Phil Hirsch's final DFS test code changes with
# 	    the test code that the OSF had.  Hopefully, this means we and the OSF
# 	    should be looking at the same test code.
# 	    Changes to facilitate merge of local DFS test code with the OSF's.
# 	[1992/05/20  19:34:49  jaffe]
# 
# Revision 1.1  1992/01/19  03:19:16  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#

DATAFILES = 	runall runtests ftsStatusCheck bigdir \
		test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 \
		test11 test12 test13 test14 test15 test16 test18 test19\
		test20 test21 README

ILIST   = ${DATAFILES}

SUBDIRS =	util

DATA_SUBDIRS = 	${SUBDIRS}



.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
