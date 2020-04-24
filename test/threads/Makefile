#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.27.2  1996/03/11  02:50:16  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:52  marty]
#
# Revision 1.2.27.1  1995/12/11  22:57:04  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/jrr_1.2_mothra/3  1995/12/06  18:57 UTC  dat
# 	Add ERRB to list of programs
# 
# 	HP revision /main/jrr_1.2_mothra/2  1995/12/06  18:19 UTC  psn
# 	Now build cuvb_nbi_011.
# 
# 	HP revision /main/jrr_1.2_mothra/1  1995/12/01  20:01 UTC  dat
# 	Add IDIR and ILIST so threads tests will be installed
# 	[1995/12/11  22:34:36  root]
# 
# Revision 1.2.22.3  1993/06/14  20:13:44  sommerfeld
# 	Fix "runtest" to invoke programs as "./file" rather than just "file"
# 	to deal with persistant paranoids like me who don't put `.' in $PATH.
# 	[1993/06/14  20:03:35  sommerfeld]
# 
# 	RP Changes
# 	[1993/04/23  22:32:02  hopkins]
# 
# Revision 1.2.22.2  1993/05/24  20:48:57  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:16:22  cjd]
# 
# Revision 1.2.20.2  1993/05/12  18:12:39  jd
# 	Initial 486 port.
# 	[1993/05/12  18:11:51  jd]
# 
# Revision 1.2.7.5  1993/02/01  21:33:53  hinman
# 	[hinman] - SVR4 changes
# 	[1993/02/01  21:05:29  hinman]
# 
# 	[hinman@sni] - Final merge before bsubmit
# 	[1993/01/31  16:30:53  hinman]
# 
# Revision 1.2.7.4  1992/12/30  22:30:55  jd
# 	Removed the -D_XOPEN_SOURCE from CFLAGS for euvb_wrp_001. This allows
# 	_AES_SOURCE to get defined in standards.h which in turn allows
# 	timers.h to include the right stuff.
# 	[1992/12/30  22:21:56  jd]
# 	Revision 1.2.14.2  1993/01/11  16:16:12  hinman
# 	[hinman] - Check in merged SNI version
# 
# Revision 9.5.1.5  92/11/18  10:58:39  blurie
# 	runtest must depend on ${PROGRAMS} or they will not get built.
# 	[92/11/18  10:58:03  blurie]
# 
# Revision 9.5.1.4  92/10/30  16:55:47  steveg
# 	Fix so libctest.a is build first.  The PROGRAMS dependancy on runtime script was wrong.
# 
# Revision 9.5.3.2  92/10/30  16:51:15  steveg
# 	Fix so libctest.a is built during first pass.  The PROGRAMS dependancy was incorrect.
# 
# Revision 9.5.1.3  92/09/16  17:21:23  raj
# 	[raj] folding in 1.0.2 changes
# 
# Revision 9.5.2.2  92/09/10  15:12:07  raj
# 	[raj] merging SVR4 changes into 1.0.2
# 
# Revision 1.2.5.3  1992/08/30  19:09:01  bolinger
# 	On RIOS, continue to link with -lbsd.  Though no longer needed
# 	for ftime(3) interface, it does seem to be required for
# 	O_NONBLOCK fcntl(2) command to work.
# 
# Revision 1.2.5.2  1992/08/11  21:34:45  bolinger
# 	Remove crvb_exc_002, prvb_exc_002 (which check for per-thread
# 	termination of unhandled exceptions) from list of active tests,
# 	because unhandled exceptions now terminate the application
# 	process.  Add TARGET_MACHINE CFLAGS ref to four per-target
# 	CFLAGS lines.  Remove -lbsd from RIOS, PMAX library lists --
# 	now that tests no longer use ftime(3), they don't need
# 	libbsd.a.
# 
# Revision 1.2.2.7  1992/06/30  14:07:56  bolinger
# 	Trivial change to comments.
# 	[1992/06/30  14:07:36  bolinger]
# 
# 	Fix OT defects 4490, 4366: Add new test prrb_thd_001 (for
# 	defect 4149); enable existing tests that had not been built
# 	previously.
# 	[1992/06/30  14:01:06  bolinger]
# 
# Revision 1.2.2.6  1992/06/02  16:44:09  bolinger
# 	Complete fix for OT defect 3953:  add new prrb_sig_005 test
# 	to list of tests made.
# 	[1992/06/02  16:40:39  bolinger]
# 
# Revision 1.2.2.5  1992/06/01  21:14:21  bolinger
# 	Complete fix for OT defect 3644:  add cuvb_nbi_010, puvb_nbi_007 to set
# 	of tests made.
# 	[1992/06/01  21:13:26  bolinger]
# 
# Revision 1.2.2.4  1992/05/15  16:50:26  bolinger
# 	Remove stoopid duplicate lists of test cases.  Add new tests to
# 	appropriate lists: prrb_nbi_001.c, cuvb_nbi_005.c, prrb_sig_004.c,
# 	prrb_stk_001.c, prvb_thd_007.c.
# 	[1992/05/15  13:39:31  bolinger]
# 
# Revision 1.2.4.2  1992/05/14  12:57:19  smith
# 	Build RIOS functional tests against libcma.a instead of libdce.a
# 
# Revision 1.2.2.2  1992/02/28  20:19:38  bolinger
# 	On RIOS, link tests against libcma.a, rather than against
# 	libdce.a.
# 	[1992/02/28  20:19:07  bolinger]
# 
# Revision 1.2  1992/01/19  22:14:42  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#
#  OSF DCE Version 1.0 
#
# $EndLog$
# Special targets:
#
# Special targets:
#	OSF/1 with kernel thread support:
#		setenv TARGET_MACHINE MACH
#		setenv target_machine mach
#	OSF/1 without kernel thread support
#		setenv TARGET_MACHINE PMAX
#		setenv target_machine pmax
#
#	OSF/1 with kernel thread support:
#		setenv TARGET_MACHINE MACH
#		setenv target_machine mach
#	OSF/1 without kernel thread support
#		setenv TARGET_MACHINE PMAX
#		setenv target_machine pmax
#

ILIST           = ${PROGRAMS} ${OTHERS}

IDIR            = /test/threads/
IMODE           = 755
#IGROUP          = bin
#IOWNER          = bin

VPATH		= ../../threads/${TARGET_MACHINE}:../../threads/

CRPB		= crpb_cvx_001	crpb_mut_001	crpb_mut_002	crpb_thd_001 \
		  crpb_thd_002	crpb_thd_003	
CRQB		= crqb_thd_001	crqb_thd_002 	crqb_thd_003	
CRRB		= crrb_att_001	crrb_cvx_001	crrb_exc_001 	crrb_exc_002 \
		  crrb_ptc_001	crrb_ptc_002	crrb_ptc_003	
CRUB		= crub_err_001	crub_err_002	crub_err_003	crub_stk_001
CRVB		= crvb_alt_001	crvb_alt_002 	crvb_alt_003	crvb_alt_004 \
		  crvb_alt_cvx_001 crvb_alt_cvx_002 crvb_alt_exc_001 \
		  crvb_att_001	crvb_att_002	crvb_att_003	crvb_cvx_001 \
		  crvb_cvx_002	crvb_cvx_tim_001 crvb_err_001	crvb_exc_001 \
		  crvb_han_001	crvb_ini_001	crvb_ini_002 \
		  crvb_mut_001	crvb_mut_002	crvb_mut_003	crvb_mut_004 \
		  crvb_ptc_001	crvb_ptc_002	crvb_sam_001	crvb_sig_001 \
		  crvb_sig_002	crvb_sig_003	crvb_sig_003_001 \
		  crvb_stk_001	crvb_stk_002 \
		  crvb_stk_003	crvb_thd_001	crvb_thd_002	crvb_thd_003 \
		  crvb_thd_004	crvb_thd_005	crvb_thd_006	crvb_thd_007 \
		  crvb_thd_008	crvb_thd_009	crvb_thd_010	crvb_thd_011 \
		  crvb_tim_001  crvb_tim_002	crvb_stk_004
CRVI		= crvi_sig_003
CUPB		= cupb_nbi_001	
CUVB		= cuvb_nbi_001	cuvb_nbi_002	cuvb_nbi_003	cuvb_nbi_004 \
		  cuvb_nbi_005	cuvb_nbi_006	cuvb_nbi_007	cuvb_nbi_008 \
		  cuvb_nbi_009  cuvb_nbi_010	cuvb_nbi_011	cuvb_wrp_001 \
		  cuvb_wrp_002	cuvb_wrp_003
CURB		= curb_exc_001
CUVI		= cuvi_nbi_004
ERRB		= errb_obj_001
ERVB		= ervb_can_001	ervb_cvx_001	
EUVB		= euvb_can_sig_001	euvb_wrp_001	euvb_exc_nbi_001 \
		  euvb_exc_nbi_002
EURB		= eurb_nbi_001
EVRB		= evrb_exc_001
PRPB		= prpb_cvx_001	prpb_mut_001	prpb_thd_001	prpb_thd_002 \
		  prpb_thd_003
PRRB		= prrb_att_001	prrb_nbi_001	prrb_sig_001	prrb_sig_002 \
		  prrb_sig_003	prrb_sig_004	prrb_sig_005	prrb_stk_001 \
		  prrb_thd_001
PRVB		= prvb_alt_001	prvb_alt_002	prvb_alt_cvx_001 \
		  prvb_alt_exc_001 \
		  prvb_att_001	prvb_att_002	prvb_can_001	prvb_can_002 \
		  prvb_can_003	prvb_can_cvx_001 prvb_cvx_001 \
		  prvb_cvx_tim_001 prvb_err_001	prvb_ini_001 \
		  prvb_mut_001	prvb_mut_002	prvb_ptc_001	prvb_ptc_002 \
		  prvb_sam_001	prvb_thd_001	prvb_thd_002	prvb_thd_003 \
		  prvb_thd_004	prvb_thd_005	prvb_thd_006	prvb_thd_007 \
		  prvb_tim_001
PUVB		= puvb_can_sig_001 puvb_nbi_001 puvb_nbi_002	puvb_nbi_003 \
		  puvb_nbi_005	puvb_nbi_006	puvb_nbi_007
PUVI		= puvi_nbi_004
#
# AQO tests are for "DECthreads Library Services", not part of DCE; so don't
# include these tests in the PROGRAMS variable.
#
AQO		= crvb_aqo_001	crvb_aqo_002	crvb_aqo_003	crvb_aqo_004 \
		  crvb_aqo_005	crvb_aqo_006	crvb_aqo_007

PTHREAD		= pthread_hello

PROGRAMS	= ${CRPB} ${CRQB} ${CRRB} ${CRUB} ${CRVB} ${CRVI} ${CUPB} \
		  ${CUVB} ${CURB} ${CUVI} \
		  ${ERVB} ${EURB} ${EUVB} \
		  ${PRRB} ${PRPB} ${PRVB} ${PUVB} ${PUVI} ${PTHREAD} ${ERRB} \
		  pthread_exc_test 

OTHERS		= runtest

LIBRARIES	= libctest.a

DATAFILES	= cts_test.h

INCFLAGS	= -I../../../../src/threads -I../../threads -I../../threads/${TARGET_MACHINE}
LIBFLAGS        = -L../../../obj/${target_machine}/test/threads

INCLUDES	= ${DATAFILES}

EXPLIB_TARGETS	= export_libctest.a

EXPDIR			= /usr/lib/
cts_test.h_EXPDIR	= /usr/include/dce/

PMAX_CFLAGS	= -D_CMA_UNIPROCESSOR_

CFLAGS		= -Dunix ${${TARGET_MACHINE}_CFLAGS}
cuvb_wrp_001.o_CFLAGS	= -Dunix ${${TARGET_MACHINE}_CFLAGS} -D_BSD
cuvb_wrp_002.o_CFLAGS	= -Dunix ${${TARGET_MACHINE}_CFLAGS} -D_BSD
cuvb_wrp_003.o_CFLAGS	= -Dunix ${${TARGET_MACHINE}_CFLAGS} -D_BSD
euvb_wrp_001.o_CFLAGS	= -Dunix ${${TARGET_MACHINE}_CFLAGS}

MACH_LDFLAGS	= -non_shared
#PMAX_LDFLAGS	= -non_shared /* unknown switch for our version  */
LDFLAGS		= ${${TARGET_MACHINE}_LDFLAGS}
RIOS_LIBS       = -lcma -lbsd
MIPS_LIBS       = -ldce -li
MACH_LIBS       = -lcma -lc_r -lcma -lmach -lbsd
OSF1_LIBS       = -ldce
VAX_LIBS        = -ldce -li
HP800_LIBS	= -ldce
LIBS            = -lctest ${${TARGET_MACHINE}_LIBS} ${${TARGET_OS}_LIBS}

libctest.a_OFILES	= cts_test.o
pthread_exc_test_OFILES = pthread_exc_test.o pthread_exc.o

# Targets to allow tests to be built by suite
crpb: ${CRPB}
crqb: ${CRQB}
crrb: ${CRRB}
crub: ${CRUB}
crvb: ${CRVB}
crvi: ${CRVI}
cupb: ${CUPB}
cuvb: ${CUVB}
cuvi: ${CUVI}
errb: ${ERRB}
ervb: ${ERVB}
euvb: ${EUVB}
prpb: ${PRPB}
prrb: ${PRRB}
prvb: ${PRVB}
puvb: ${PUVB}
puvi: ${PUVI}
aqo:  ${AQO}
crpb: ${CRPB}
crqb: ${CRQB}
crrb: ${CRRB}
crub: ${CRUB}
crvb: ${CRVB}
crvi: ${CRVI}
cupb: ${CUPB}
cuvb: ${CUVB}
curb: ${CURB}
cuvi: ${CUVI}
errb: ${ERRB}
ervb: ${ERVB}
euvb: ${EUVB}
evrb: ${EVRB}
prpb: ${PRPB}
prrb: ${PRRB}
prvb: ${PRVB}
puvb: ${PUVB}
puvi: ${PUVI}
aqo:  ${AQO}

${PROGRAMS}:	libctest.a

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif


.include <${RULES_MK}>

runtest: ${PROGRAMS}
	ls ./???b_???*_??? >runtest ; chmod u+x,g+x runtest

#  DEC/CMS REPLACEMENT HISTORY, Element MAKEFILE.
#  *38    6-NOV-1991 11:52:50 BUTENHOF "Add test"
#  *37    6-NOV-1991 09:10:38 BUTENHOF "Add a new test"
#  *36   31-OCT-1991 12:35:18 BUTENHOF "Add new tests"
#  *35   18-OCT-1991 11:52:01 BUTENHOF "Add a test"
#  *34   16-OCT-1991 08:54:54 BUTENHOF "Add AQO tests in (but non-default)"
#  *33   15-OCT-1991 09:37:21 BUTENHOF "Don't build pthread_exit from main test"
#  *32   15-OCT-1991 06:59:16 BUTENHOF "Change ""x1"" test to 001"
#  *31   14-OCT-1991 13:48:59 BUTENHOF "Add new test"
#  *30   24-SEP-1991 16:38:51 BUTENHOF "Add new tests"
#  *29    4-SEP-1991 14:27:45 BUTENHOF "Add new test"
#  *28    2-JUL-1991 16:49:33 BUTENHOF "Integrate Tin stuff"
#  *27   21-JUN-1991 14:29:43 CURTIN "Added cuvb_nbi_008"
#  *26   18-JUN-1991 16:24:13 CURTIN "making changes for platform specific libi"
#  *25   17-JUN-1991 12:20:03 BUTENHOF "Update for ODE 2.0.2"
#  *24   14-JUN-1991 13:11:29 CURTIN "added crrb_exc_001.c"
#  *23   13-JUN-1991 17:08:08 CURTIN "Added a library for link time, for messages"
#  *22    6-JUN-1991 17:45:04 CURTIN "Added cuvb_wrp_003 to test atfork"
#  *21    6-JUN-1991 09:40:34 CURTIN "Added new test cuvb_wrp_002"
#  *20   29-MAY-1991 13:20:24 CURTIN "Added a new test"
#  *19   29-MAY-1991 13:03:28 BUTENHOF "Fix VPATH"
#  *18    3-APR-1991 15:58:48 BUTENHOF "Repair last addition"
#  *17    3-APR-1991 13:50:01 BUTENHOF "Add new test"
#  *16    1-APR-1991 16:58:18 CURTIN "put most of AQO tests back"
#  *15   27-MAR-1991 18:33:26 BUTENHOF "Fix INCFLAGS"
#  *14   15-MAR-1991 13:17:24 BUTENHOF "Fix exporting"
#  *13   15-MAR-1991 11:12:00 BUTENHOF "Fix some stuff"
#  *12    7-MAR-1991 08:56:17 BUTENHOF "Fix runtest creation"
#  *11   27-FEB-1991 19:28:12 BUTENHOF "Include libbsd.a on RIOS"
#  *10   21-FEB-1991 01:18:36 BUTENHOF "Try again"
#  *9    21-FEB-1991 00:57:24 BUTENHOF "Modify runtest target"
#  *8    21-FEB-1991 00:05:49 BUTENHOF "Remove AQO tests (not really portable yet)"
#  *7    20-FEB-1991 23:47:34 BUTENHOF "Build ""runtest"" procedure"
#  *6    20-FEB-1991 23:41:07 BUTENHOF "Add test"
#  *5    12-FEB-1991 14:20:21 BUTENHOF "(oops) Fix pthread cancel test names"
#  *4    12-FEB-1991 01:31:29 BUTENHOF "Changes to alert control"
#  *3    28-JAN-1991 23:58:21 BUTENHOF "Fix filename"
#  *2     4-JAN-1991 01:50:27 BUTENHOF "Use subdirectory (to link to RTL source disk)"
#  *1     2-JAN-1991 22:54:05 BUTENHOF "Test system makefile (OSF build environment)"
#  DEC/CMS REPLACEMENT HISTORY, Element MAKEFILE.
