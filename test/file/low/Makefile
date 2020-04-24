#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.178.1  1996/10/17  18:29:00  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:54:35  damon]
#
# Revision 1.1.173.3  1994/02/07  16:28:22  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:35  mckeen]
# 
# Revision 1.1.173.2  1994/02/04  20:46:39  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:25:08  devsrc]
# 
# Revision 1.1.171.1  1993/12/07  17:45:43  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  19:34:22  jaffe]
# 
# Revision 1.1.16.2  1993/07/19  19:43:09  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:32:33  zeliff]
# 
# Revision 1.1.14.3  1993/07/16  22:03:10  kissel
# 	Put HP RP work on the 1.0.3i branch.
# 	[1993/06/21  15:07:33  kissel]
# 
# Revision 1.1.12.2  1993/06/04  18:35:05  kissel
# 	Initial HPUX RP version.
# 	[1993/06/03  22:45:46  kissel]
# 
# Revision 1.1.6.4  1993/04/30  14:13:45  maunsell_c
# 	use ${BSD_LIBS} so file is platform independent
# 	[1993/04/30  14:10:19  maunsell_c]
# 
# Revision 1.1.6.3  1993/04/28  18:49:39  maunsell_c
# 	use DEBUG_ON flag to turn on -g compile!!
# 	[1993/04/28  18:09:42  maunsell_c]
# 
# Revision 1.1.8.8  1993/04/09  17:15:33  gmd
# 	Added READ_ME to ILIST via DATAFILES.
# 	[1993/04/08  23:34:20  gmd]
# 
# Revision 1.1.8.7  1993/01/11  18:32:35  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:13:51  htf]
# 
# Revision 1.1.8.6  1992/12/09  21:33:12  jaffe
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
# 	[1992/12/07  13:37:06  jaffe]
# 
# Revision 1.1.8.5  1992/11/09  21:40:10  jaffe
# 	Transarc delta: jaffe-ot5942-make-test-tree-work 1.1
# 	  Selected comments:
# 	    Now that the test tree installs in dcetest/dce1.0, this delta
# 	    cleans up the Transarc files so that there are no extraneous additions
# 	    to the export tree, and so that all appropriate files are installed.
# 	    Don't export the files, let them be installed.
# 	[1992/11/09  19:06:08  jaffe]
# 
# Revision 1.1.8.4  1992/11/02  15:20:30  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:16:22  root]
# 
# Revision 1.1.8.3  1992/10/28  18:00:15  jaffe
# 	Transarc delta: jess-add-dumpkernel-in-low-tests 1.5
# 	  Selected comments:
# 	    Add function call (icl_DumpKernel) in the test suite. In case of running
# 	    error of C programmes in the test suite the "icl_DumpKernel" function
# 	    will be called in errex() to do the kernel trace. The dump file is located
# 	    in "/tmp/prog#.kernel.dump.pid". You have to have the debug flag set to
# 	    have the dump file.
# 	    See above.
# 	    Change the Makefile to have icl_DumpKernel() called in the programmes.
# 	    Process another C file and add "low" as dump file header.
# 	    Turn on the prog3 debug flags.
# 	    It should not create the zero length dumpfile when there is nothing wrong.
# 	[1992/10/28  14:27:00  jaffe]
# 
# Revision 1.1.8.2  1992/08/28  18:15:41  jaffe
# 	Transarc delta: bk-ot4724-makefile-low 1.1
# 	  Selected comments:
# 	     For ot 4724
# 	    IDIR was set to test/low but it needed a / at the end.
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
# 	Transarc delta: mcinerny-ot4865-install-tests-util-low-fs 1.1
# 	  Selected comments:
# 	    install test suite in export tree at export time
# 	    ditto
# 	Transarc delta: rajesh-clean-low-tests 1.5
# 	  Selected comments:
# 	    To remove make and clean options from the low test suite. To change
# 	    the scripts to use common shell functions. To create a file
# 	    fs_functionsScript with the common shell functions in the util
# 	    directory.
# 	    Provide for builds of individual test programs, specially test3.
# 	    Specify install parameters like IDIR and IMODE. Provide for export of
# 	    header file common.h.
# 	    Modify scripts to follow coding standards, simplify the code, increase
# 	    readability etc.
# 	    Remove 16_bytes from DATAFILES
# 	    More of above.
# 	    Modify the tests to give them a simpler logical structure, modularity,
# 	    readability ( one command per line) and to change then to use the
# 	    common function created earlier.
# 	    Modification of rtest1 and rtest2 to use the utility functions
# 	    correctly.
# 	[1992/08/28  12:28:48  jaffe]
# 
# Revision 1.1.3.7  1992/06/19  17:19:12  jaffe
# 	Transarc delta: rajesh-ot4304-low-rtest2 1.1
# 	  Selected comments:
# 	    low rprog2.c assumes that gid of files and dirs is the gid of the
# 	    creating process. On OSF1 the group id of created files and dirs are
# 	    that of the parent dir and not the process's group id. Fix the tests
# 	    to operate correctly for OSF1 platforms.
# 	    Correct minor errors.
# 	[1992/06/19  17:16:50  jaffe]
# 
# Revision 1.1.3.6  1992/06/18  18:54:28  jaffe
# 	Transarc delta: fred-3925-remove-make 1.1
# 	  Selected comments:
# 	    Removed 16_bytes and half_meg data files.
# 	[1992/06/18  18:45:00  jaffe]
# 
# 	Transarc delta: fred-3925-remove-make 1.1
# 	  Selected comments:
# 	    Removed 16_bytes and half_meg data files.
# 	[1992/06/18  18:18:10  jaffe]
# 
# Revision 1.1.3.5  1992/05/21  12:45:54  jaffe
# 	Transarc delta: fred-merge-tests 1.1
# 	  Selected comments:
# 	    Created this delta to merge Phil Hirsch's final DFS test code changes with
# 	    the test code that the OSF had.  Hopefully, this means we and the OSF
# 	    should be looking at the same test code.
# 	    Changes to facilitate merge of local DFS test code with the OSF's.
# 	Transarc delta: rajesh-fix-lowtests-makefile 1.1
# 	  Selected comments:
# 	    Fix typos in low tests Makefile
# 	Transarc delta: rajesh-low-makefile-rprog2-fix 1.3
# 	  Selected comments:
# 	    To modify the Makefile to issue appropriate flags to gcc and
# 	    to link the libdcesec.a for rtest2
# 	    Change CCFLAGS in Makefile to CFLAGS
# 	    Change CCFLAGS to CFLAGS
# 	    Not neceessary .
# 	Transarc delta: rajesh-low-rtest2-passuidgid 1.1
# 	  Selected comments:
# 	    To modify the rtest2 program so that rprog2.c does not need to be linked
# 	    with the libdce.a so that the same cc command can be used both under DFS and
# 	    AFS. Done by determining the uid and gid in the rtest2 shellscript and
# 	    passing it to rprog2.c, instead of letting rprog2.c using DCE security API
# 	    calls in DCE/DFS and system calls in AFS to do it.
# 	    Remove changes made when rprog2.c was changed to use DCE security API calls.
# 	Transarc delta: rajesh-test-error-mapping 1.1
# 	  Selected comments:
# 	    Tests to check DFS error mapping.
# 	[1992/05/20  20:31:47  jaffe]
# 
# Revision 1.1.3.4  1992/03/06  21:20:04  mason
# 	Removed spurious CFLAGS argument in the makefile
# 	[1992/03/06  14:54:58  mason]
# 
# Revision 1.1.3.3  1992/01/24  04:15:09  devsrc
# 	Merging dfs6.3
# 	[1992/01/24  00:23:19  devsrc]
# 
# Revision 1.1.3.2  1992/01/23  20:30:36  zeliff
# 	Moving files onto the branch for dfs6.3 merge
# 	[1992/01/23  18:47:57  zeliff]
# 	Revision 1.1.1.3  1992/01/23  22:25:57  devsrc
# 	Fixed logs
# 
# $EndLog$

IMODE = 0755

DATAFILES = test1  test3  test5 test6 test7 test8 test9 test10 \
            test11 test12 test13 test14 rtest1 rtest2 testErrMap  \
            rtestErrMapQuota runtests READ_ME

prog3_PROGRAMS = prog3a prog3b prog3c

PROGRAMS = prog1 ${prog3_PROGRAMS} prog5 prog6 prog7 prog8 prog9 \
	prog10 prog11 prog12 prog13 prog14 rprog1 rprog2 progErrMap \
	rprogErrMapQuota 

#prog11_LIBS	= -lbsd -licl -lafssys -ldce 
LIBS	=  -licl -lafsutil -lncompat -losi -lafssys -ldce ${OS_LIBS}

# use export DEBUG_ON=1 to turn this on
#CFLAGS 	= -g 

ILIST	= ${PROGRAMS} ${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>






















