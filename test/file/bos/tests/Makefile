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
# Revision 1.1.12.1  1996/10/17  18:16:36  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:49:17  damon]
#
# Revision 1.1.4.8  1993/01/11  17:37:35  htf
# 	Embedded copyright notice
# 	[1993/01/11  16:54:00  htf]
# 
# Revision 1.1.4.7  1992/11/09  21:39:42  jaffe
# 	Transarc delta: jaffe-ot5942-make-test-tree-work 1.1
# 	  Selected comments:
# 	    Now that the test tree installs in dcetest/dce1.0, this delta
# 	    cleans up the Transarc files so that there are no extraneous additions
# 	    to the export tree, and so that all appropriate files are installed.
# 	    Don't export the files, let them be installed.
# 	[1992/11/09  19:00:44  jaffe]
# 
# Revision 1.1.4.6  1992/11/04  19:40:31  jaffe
# 	Transarc delta: comer-ot5803-fix-erronenous-bosserver-return-code-test 1.1
# 	  Selected comments:
# 	    Added test to the bos test suite for bos uninstall.
# 	    Install test13
# 	[1992/11/04  17:32:47  jaffe]
# 
# Revision 1.1.4.4  1992/11/02  15:20:05  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:15:21  root]
# 
# Revision 1.1.4.3  1992/10/28  15:46:27  jaffe
# 	Transarc delta: comer-ot4849-misc-bos-command-test-cleanup 1.3
# 	  Selected comments:
# 	    Miscellaneous clean-up of the bos command test suite.
# 	    More cleanup of the bos test suite
# 	    Change Makefiles to install files into the export tree.  This should
# 	    later be changed to install into the install tree as soon as we decide
# 	    on the correct tree layout.  In the meantime, put the files under
# 	    export/<machine>/test/bos.
# 	    See above.
# 	Transarc delta: mcinerny-merge-remove-bos-tests-test7-from-Makefile 1.1
# 	  Selected comments:
# 	    Build/Make warnings about a defuncted file.
# 	    Remove test7 from list of datafiles.
# 	[1992/10/27  16:44:19  jaffe]
# 
# Revision 1.1.4.2  1992/09/25  16:54:52  jaffe
# 	Transarc delta: rajesh-ot5015-bos-test-fixes 1.7
# 	  Selected comments:
# 	    Various changes to fix problems in Ot 5015. This version of the delta
# 	    is only a checkpoint and is not yet ready.
# 	    Remove test7 which tested bos exec.
# 	    Add in a utility functions that is used to check if a server has
# 	    a dfs-server DCE principal.
# 	    Accidentally deleted all functions except DetermineLocalAuth() in
# 	    util/gen_FunctionsScript. So modified to include all functions previously
# 	    existing in the util/gen_FunctionsScript.
# 	    Some more fixes.
# 	    Fix problem in DetermineLocalAuth function in gen_FunctionsScript.
# 	    Just another checkpoint.
# 	    Changes to fix problems found during execution of test11. The delta is now
# 	    ready for export.
# 	[1992/09/23  20:37:28  jaffe]
# 
# Revision 1.1.2.2  1992/05/21  12:33:56  jaffe
# 	Transarc delta: fred-merge-tests 1.1
# 	  Files modified:
# 	    Makefile
# 	    bos: Makefile, READ_ME, init_dce, kills, runc, runtests
# 	    bos: setup, shows
# 	    bos/tests: Makefile, test0, test1, test10, test11, test12
# 	    bos/tests: test2, test3, test4, test5, test6, test7, test8
# 	    bos/tests: test9; bos/tests/procs: Makefile
# 	    cm: Makefile, runtests, test0, test1, test2, test3, test4
# 	    cm: test5, test6; config: chk_errnr, echeck, environment
# 	    fs: Makefile, READ_ME, err1, err2, longalpha, longfox
# 	    fs: longmix, prog1.c, rtest1, shortalpha, shortfox, shortmix
# 	    fs: test1, test10, test2, test2b, test2s, test3, test4
# 	    fs: test5, test6, test7, test8, test9
# 	    fts: Makefile, check, etest.sed, etest1, etest2, etest3
# 	    fts: etest4, etest5, etest6, rtest1, rtest2, runrtests
# 	    fts: runtests, test1, test10, test11, test12, test13, test2
# 	    fts: test3, test4, test5, test6, test7, test8, test9
# 	    low: Makefile, ctrl.asc, prog11.c, prog12.c, prog14.c
# 	    low: prog3a.c, prog8.c, prog9.c, rtest1, rtest2, styl.asc
# 	    low: test1, test10, test11, test12, test13, test14, test2
# 	    low: test3, test4, test5, test6, test7, test8, test9
# 	    naming: Makefile, bostest, fstest, ftstest
# 	    scout: Makefile, test1, test2; setup: Makefile, environment
# 	    ubik: Makefile, READ_ME, init, kills, runc, setup, shows
# 	    ubik: udbg; util: Makefile, sys.c
# 	  Selected comments:
# 	    Created this delta to merge Phil Hirsch's final DFS test code changes with
# 	    the test code that the OSF had.  Hopefully, this means we and the OSF
# 	    should be looking at the same test code.
# 	    Changes to facilitate merge of local DFS test code with the OSF's.
# 	[1992/05/20  17:09:49  jaffe]
# 
# Revision 1.1  1992/01/19  03:18:18  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#

SUBDIRS =	procs

DATAFILES =	test0 test1 test2 test3 test4 test5 test6 test8 test9 \
		test10 test11 test12 test13

ILIST	= ${DATAFILES}
 
.include <${RULES_MK}>
