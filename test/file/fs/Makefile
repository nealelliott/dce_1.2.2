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
# Revision 1.1.622.1  1996/10/17  18:19:35  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:50:28  damon]
#
# Revision 1.1.617.3  1994/02/07  16:28:08  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:21  mckeen]
# 
# Revision 1.1.617.2  1994/02/04  20:40:26  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:22:26  devsrc]
# 
# Revision 1.1.615.1  1993/12/07  17:41:49  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  18:54:07  jaffe]
# 
# 	1.0.3a update from Transarc
# 
# Revision 1.1.4.8  1993/04/09  17:15:19  gmd
# 	Added READ_ME to ILIST via DATAFILES.
# 	[1993/04/08  23:36:36  gmd]
# 
# Revision 1.1.4.7  1993/01/11  18:26:24  htf
# 	Embedded copyright notice
# 	[1993/01/11  16:59:10  htf]
# 
# Revision 1.1.4.6  1992/12/09  21:31:48  jaffe
# 	Transarc delta: comer-ot6129-user-level-icl-dumping 1.5
# 	  Selected comments:
# 	    Added code to ICL trace logs at user level.  The guinea pig for this
# 	    change was dfsbind, also modified as part of this delta.  Most of the
# 	    actual tracing and some of the ICL work was done at IBM by Jean
# 	    Pehkonen.
# 	    Force the program to dump its log: kill -30 <pid>
# 	    Added dfsbind to EXPINC_SUBDIRS
# 	    In files where both osi.h and compat.h are included, the reference
# 	    to osi.h must be first.  In each of these files, osi.h was implicitly
# 	    included, so I added an explicit reference before compat.h.
# 	    Add ncompat and osi for the lock package.
# 	    Installed message catalog.
# 	[1992/12/04  20:26:37  jaffe]
# 
# Revision 1.1.4.5  1992/11/09  21:39:48  jaffe
# 	Transarc delta: jaffe-ot5942-make-test-tree-work 1.1
# 	  Selected comments:
# 	    Now that the test tree installs in dcetest/dce1.0, this delta
# 	    cleans up the Transarc files so that there are no extraneous additions
# 	    to the export tree, and so that all appropriate files are installed.
# 	    Don't export the files, let them be installed.
# 	[1992/11/09  19:01:59  jaffe]
# 
# Revision 1.1.4.4  1992/11/02  15:20:20  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:15:50  root]
# 
# Revision 1.1.4.3  1992/10/28  16:12:16  jaffe
# 	Transarc delta: jess-add-dumpkernel-in-fs-tests 1.3
# 	  Selected comments:
# 	    The icldump command and debug option for the tests have been added in the
# 	    fs test scripts. You can run fs/tests with -debug to dump kernel in case
# 	    of error. The delta also adds the icl_DumpKernel function in the C programm.
# 	    Cahneged to compile the new involvement.
# 	    Put the debug option in test1.
# 	    When the debug is set the dumpfile is always created eventhough
# 	    there is no error.
# 	[1992/10/27  18:08:35  jaffe]
# 
# Revision 1.1.4.2  1992/08/28  18:10:30  jaffe
# 	Transarc delta: fred-ot3850-add-runtests 1.4
# 	  Selected comments:
# 	    Not done yet.
# 	    This delta adds simple runtests scripts to the fs and low directories.
# 	    These scripts will be enhanced in the future to permit greater control of
# 	    the underlying tests.
# 	    Install the runtests scripts from fs and low
# 	    Install test script with mode 0755, so that runtests can execute them directly.
# 	Transarc delta: jaffe-fix-test-102-1.6 1.2
# 	  Selected comments:
# 	    fixed extra spaces in the Makefile
# 	    don't export non-existant file.
# 	Transarc delta: mcinerny-fix-merge-makefile-refs-wrong-files 1.5
# 	  Selected comments:
# 	    (see below)
# 	    My merge caused this makefile to reference non-existent files.  Removed
# 	    those references.
# 	    Couple of typos
# 	    Couple of minor changes to get tests running more smoothly.
# 	Transarc delta: mcinerny-ot4865-install-tests-util-low-fs 1.1
# 	  Selected comments:
# 	    install test suite in export tree at export time
# 	    ditto
# 	Transarc delta: rajesh-clean-fs-tests 1.6
# 	  Selected comments:
# 	    To remove make, clean options from fs tests
# 	    Put reminders to add clean option
# 	    Make the scripts modular, structured.
# 	    The variable names are changed to follow coding standards.
# 	    Apply modification mentioned in revision 1.2 to 2 missed test file
# 	    err1 and err2.
# 	    Some bug fixes, and revision of tests err1 and err2. Introduction of
# 	    a common functions ProcessResult to analyse the result.
# 	    Further changes to some utility functions.
# 	    Added error test err2 to the Makefile
# 	    See above.
# 	[1992/08/28  12:06:56  jaffe]
# 
# Revision 1.1.2.3  1992/05/29  22:57:31  mason
# 	Transarc delta: fred-3754-change-script-name 1.1
# 	  Selected comments:
# 	    To clean up a number of defects with the tests.  In addition to defect
# 	    3754, the changes also fix defects 3750 and 3748.
# 	    Removed files that are no longer used.
# 	Transarc delta: fred-remove-fs-make-clean 1.1
# 	  Selected comments:
# 	    I originally created it to remove the make and clean options from the
# 	    test1 and test5 scripts.  However I fixed a bug in test1 and closed it.  This
# 	    keeps the make/clean fixes separate from the actual bug.  We may not want
# 	    the make/clean fixes to go out until after 1.0.1.
# 	    No change.
# 	[1992/05/28  22:01:56  mason]
# 
# Revision 1.1.2.2  1992/05/21  12:37:31  jaffe
# 	Transarc delta: fred-2980-and-2981-fix-sys 1.1
# 	  Files modified:
# 	    fs: Makefile, err1, err2, half_meg, test1, test10, test2
# 	    fs: test2b, test2s, test3, test4, test5, test6, test7, test8
# 	    fs: test9
# 	  Selected comments:
# 	    To fix reported defects indicating files could not be found.  Added two
# 	    shell scripts, one to issue the correct command to get the system type
# 	    (either AFS or DFS), and a second to generate a file from a string.
# 	    Remove unnecessary files (half_meg) and add the two shell script files.
# 	Transarc delta: fred-add-files 1.1
# 	  Files modified:
# 	    fs: Makefile, fsGenerateFile, fsSetSysName
# 	  Selected comments:
# 	    I forgot to add the two new shell scripts, fsSetSysName and fsGenerateFile,
# 	    to the previous delta.
# 	    Added fsGenerateFile as a datafile.
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
# 	Transarc delta: fred-remove-sys 1.1
# 	  Files modified:
# 	    Makefile; cm: runtests, test0, test3, test4, test6
# 	    fs: Makefile, err1, err2, rtest1, test10, test2, test2b
# 	    fs: test2s, test3, test4, test5, test6, test7, test8, test9
# 	    low: progErrMap.c, rprogErrMapQuota.c, rtest1, rtest2
# 	    low: rtestErrMapQuota, test1, test10, test11, test12, test13
# 	    low: test14, test2, test3, test4, test5, test6, test7, test8
# 	    low: test9, testErrMap
# 	    util: Makefile, fs_GenerateFile, fs_SetSysName
# 	  Selected comments:
# 	    To remove dependency on the AFS sys command in the test scripts.
# 	    To remove dependency on AFS sys command.
# 	[1992/05/20  18:18:04  jaffe]
# 
# Revision 1.1  1992/01/19  03:17:21  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#

#
# test1 clean - rm tf.proto  

IMODE = 0755

DATAFILES = err1 err2 test1 test2 test2b test2s test3 test4 test5 test6 test7 \
	    test8 test9 test10 rtest1 prog1.c runtests \
            longalpha longfox longmix shortalpha shortfox shortmix \
	    READ_ME

PROGRAMS = prog1

prog1_LIBS = -licl -lafsutil -lncompat -losi -lafssys -ldce ${OS_LIBS}

ILIST	= ${PROGRAMS} ${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
