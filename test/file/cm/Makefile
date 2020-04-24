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
# Revision 1.1.609.1  1996/10/17  18:18:14  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:49:59  damon]
#
# Revision 1.1.604.1  1993/12/28  18:01:37  delgado
# 	use quotes for MAKETOP include instead of brackets
# 	[1993/12/28  17:58:17  delgado]
# 
# Revision 1.1.4.4  1993/04/09  17:15:16  gmd
# 	Added README as to DATAFILES for ILIST so it is also
# 	included in test install tree.
# 	[1993/04/06  20:40:29  gmd]
# 
# Revision 1.1.4.3  1993/01/11  18:25:37  htf
# 	Embedded copyright notice
# 	[1993/01/11  16:57:46  htf]
# 
# Revision 1.1.4.2  1992/11/02  15:35:52  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:15:40  root]
# 
# Revision 1.1.2.2  1992/05/21  12:36:04  jaffe
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
# 	[1992/05/20  17:56:06  jaffe]
# 
# Revision 1.1  1992/01/19  03:18:41  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#

DATAFILES = runtests test0 test1 test2 test3 test4 test5 test6 README \
	runall

ILIST	= ${DATAFILES}


.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
