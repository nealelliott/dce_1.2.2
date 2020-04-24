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
# Revision 1.1.17.1  1996/10/17  18:16:28  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:49:14  damon]
#
# Revision 1.1.12.1  1993/12/28  18:01:33  delgado
# 	use quotes for MAKETOP include instead of brackets
# 	[1993/12/28  17:58:14  delgado]
# 
# Revision 1.1.10.2  1993/07/19  19:38:42  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:29:54  zeliff]
# 
# Revision 1.1.8.3  1993/07/16  21:16:13  kissel
# 	Add DATA_SUBDIRS so that bos/tests can build automatically.
# 	[1993/07/13  20:27:13  buhner]
# 
# Revision 1.1.5.5  1993/01/11  17:37:29  htf
# 	Embedded copyright notice
# 	[1993/01/11  16:53:48  htf]
# 
# Revision 1.1.5.4  1992/11/09  21:39:38  jaffe
# 	Transarc delta: jaffe-ot5942-make-test-tree-work 1.1
# 	  Selected comments:
# 	    Now that the test tree installs in dcetest/dce1.0, this delta
# 	    cleans up the Transarc files so that there are no extraneous additions
# 	    to the export tree, and so that all appropriate files are installed.
# 	    Don't export the files, let them be installed.
# 	[1992/11/09  18:59:29  jaffe]
# 
# Revision 1.1.5.3  1992/11/02  15:20:01  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:15:11  root]
# 
# Revision 1.1.5.2  1992/10/28  15:46:12  jaffe
# 	Transarc delta: comer-ot4849-misc-bos-command-test-cleanup 1.3
# 	  Selected comments:
# 	    Miscellaneous clean-up of the bos command test suite.
# 	    More cleanup of the bos test suite
# 	    Install REMOTE_TESTING file
# 	    Change Makefiles to install files into the export tree.  This should
# 	    later be changed to install into the install tree as soon as we decide
# 	    on the correct tree layout.  In the meantime, put the files under
# 	    export/<machine>/test/bos.
# 	    See Above.
# 	Transarc delta: comer-ot5747-fix-bos-test-readme 1.2
# 	  Selected comments:
# 	    Fixed up the READ_ME file so that it actually gives the user some
# 	    idea of how to run the tests.  I also incorporated the info from
# 	    REMOTE_TESTING into this file.
# 	    Fix build errors.
# 	    Remove reference to removed file.
# 	[1992/10/27  16:41:36  jaffe]
# 
# Revision 1.1.2.2  1992/05/21  12:32:58  jaffe
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
# 	[1992/05/20  16:20:41  jaffe]
# 
# Revision 1.1  1992/01/19  03:18:32  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#

SUBDIRS =	tests

DATA_SUBDIRS =	tests

DATAFILES =	READ_ME runtests

ILIST	= ${DATAFILES}



.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
