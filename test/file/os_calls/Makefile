# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.109.1  1996/10/17  18:30:58  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:55:30  damon]
#
# Revision 1.1.104.1  1993/12/28  18:01:57  delgado
# 	use quotes for MAKETOP include instead of brackets
# 	[1993/12/28  17:58:37  delgado]
# 
# Revision 1.1.9.2  1993/07/19  19:45:20  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:33:56  zeliff]
# 
# Revision 1.1.7.3  1993/07/16  22:51:06  kissel
# 	Put HP RP work on the 1.0.3i branch.
# 	[1993/06/21  15:13:56  kissel]
# 
# Revision 1.1.5.2  1993/06/04  20:53:01  kissel
# 	Initial HPUX RP version.
# 	[1993/06/03  22:48:01  kissel]
# 
# Revision 1.1.3.3  1993/04/30  14:13:47  maunsell_c
# 	use ${BSD_LIBS} so file is platform independent
# 	[1993/04/30  14:10:48  maunsell_c]
# 
# Revision 1.1.3.2  1993/04/28  18:49:45  maunsell_c
# 	use DEBUG_FLAG_ON to get -g compile
# 	[1993/04/28  18:10:01  maunsell_c]
# 
# Revision 1.1.2.7  1993/01/13  21:36:38  shl
# 	Transarc delta: fred-ot6239-fix-install-dirs 1.1
# 	  Selected comments:
# 	    Fix where test programs are installed.
# 	    Removed the IDIR of /bin/.
# 	    Added IMODE of 0755.
# 	[1993/01/13  14:56:26  shl]
# 
# Revision 1.1.2.6  1993/01/11  18:36:33  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:20:00  htf]
# 
# Revision 1.1.2.5  1992/11/19  13:35:24  jaffe
# 	Transarc delta: itl-sanzi-initial-spoke-library 1.1
# 	  Selected comments:
# 	    Addition of Spoke Library to the suite of ITL libraries.
# 	    Port of file_test and its libs to the Sun.
# 	    Add -litlHubSpoke to the list of libs that file_test
# 	    links with.
# 	Transarc delta: jess-itl-fix-makefile 1.5
# 	  Selected comments:
# 	    This delta is not precisely named. Do't be confused that the delta
# 	    is opened for porting Sanzi Rich's sun based distributed ITL to the
# 	    rios and pmax platforms and also for some bug fixes.
# 	    See above reason.
# 	    Fixed a merge problems (in fact, -lcom_err and -lbsd are needed).
# 	    Fix a parameter passing error.
# 	    There was an unavailable error when calling server_register_if.
# 	    This reopen fix the problem some way.
# 	    Chaned the definition of stat_compare function because dfs does not
# 	    care all of stat fields.
# 	[1992/11/18  16:37:30  jaffe]
# 
# Revision 1.1.2.4  1992/11/02  15:20:34  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:16:41  root]
# 
# Revision 1.1.2.3  1992/10/28  18:15:36  jaffe
# 	Transarc delta: fred-itl-add-fcntl-flock 1.3
# 	  Selected comments:
# 	    To add fcntl and flock to the ITL file stuff.
# 	    Also will fix some minor bugs, and convert to use Jeff Prem's utilities.
# 	    This is a checkpoint.  I need to import a delta from Jeff.
# 	    Complete the integration of Jeff's utility stuff and finish up work on
# 	    fcntl and flock.
# 	    Added the com_err library to get error_message.
# 	    Forgot to add the files that implement the file locking stuff. What
# 	    a dufus.
# 	Transarc delta: fred-itl-dir-stat-mem-cmds 1.2
# 	  Selected comments:
# 	    Adding support for directory functions, stat function, and memory mapped
# 	    files.
# 	    Not done yet, just upgrading.
# 	    This complets this delta.
# 	    No changes.
# 	[1992/10/28  15:02:18  jaffe]
# 
# Revision 1.1.2.2  1992/09/25  17:31:51  jaffe
# 	Transarc delta: fred-itl-file-tester 1.1
# 	  Selected comments:
# 	    This code just builds an executable program out of the file commands
# 	    (OS calls; open, close, etc.) that are in the libitlFile.a library in
# 	    the itl directory.
# 	    Initial checkin.
# 	[1992/09/24  12:59:02  jaffe]
# 
# $EndLog$
#
#  Makefile --
#	For the test/file/os_calls directory.
#

PROGRAMS = file_test

CCTYPE = ansi

#CARGS = -g -DITL_FS_TEST
CARGS = -DITL_FS_TEST

file_test_OFILES = file_test.o
file_test_LIBS   = -litlFile -litlcm -litlpioctl -litldfsSpoke -litlHubSpoke \
		   -litlUtils -litlcommon -litl -licl -lncompat \
		   -losi -lafssys -lafsutil -ldce -lcom_err \
		   ${BSD_LIBS} ${OS_LIBS} ${${TARGET_MACHINE}_LIBS}

IMODE = 0755

ILIST = ${PROGRAMS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>
