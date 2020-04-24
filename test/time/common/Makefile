#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
# All Rights Reserved
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.12.2  1996/03/11  02:50:23  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:57  marty]
#
# Revision 1.1.12.1  1995/12/11  23:05:20  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/4  1995/06/15  14:23 UTC  pare
# 	Merge files
# 
# 	HP revision /main/HPDCE02/pare_mothra/1  1995/06/15  12:31 UTC  pare
# 	Add new test to makefile
# 
# 	HP revision /main/HPDCE02/3  1994/07/25  17:39 UTC  pare
# 	Add -lm to test-kernel link line
# 
# 	HP revision /main/HPDCE02/2  1994/07/25  15:12 UTC  pare
# 	change libapi to libtetapi
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  18:09 UTC  pare
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/2  1994/03/30  21:25 UTC  pare
# 
# 	HP revision /main/HPDCE01/pare_rodan/1  1994/03/30  21:24 UTC  pare
# 	remove tcm.o, replace with -ltettcm
# 
# 	HP revision /main/HPDCE01/1  1994/01/31  19:57  pare
# 	merge kk and hpdce01
# 	[1995/12/11  22:37:02  root]
# 
# Revision 1.1.8.5  1993/10/21  19:50:27  pare
# 	Added rancover2_api test
# 	[1993/10/21  19:49:13  pare]
# 
# Revision 1.1.8.4  1993/09/13  19:17:54  pare
# 	Added -I../../../time/libutc/user to INCFLAGS
# 	[1993/09/13  19:16:55  pare]
# 
# Revision 1.1.8.3  1993/07/14  17:10:17  truitt
# 	Added support for new tests blech and rancover_api.  Both
# 	are tests written under tet for coverage purposes.
# 	[1993/07/14  17:09:58  truitt]
# 
# Revision 1.1.8.2  1993/07/09  17:12:33  root
# 	Initial King Kong branch
# 	[1993/07/09  17:12:06  root]
# 
# Revision 1.1.6.5  1993/04/16  18:49:02  truitt
# 	Changes to support TET builds of functional tests.
# 	[1993/04/16  18:48:45  truitt]
# 
# Revision 1.1.6.4  1993/04/15  13:15:19  truitt
# 	Ensure zonecvt.dat file makes it into the obj/hp800 tree during build.
# 	[1993/04/15  13:14:56  truitt]
# 
# Revision 1.1.6.3  1993/04/12  14:42:55  truitt
# 	Removed build for test_kernel-kernel, since we don't use it.
# 	[1993/04/12  14:42:31  truitt]
# 
# Revision 1.1.6.2  1993/03/17  19:57:54  truitt
# 	Merge Hp_dce1_0_1 with DCE1_0_2
# 	[1993/03/17  19:57:01  truitt]
# 
# Revision 1.1.1.7  1993/03/11  22:23:19  osfrcs
# 	    Loading drop DCE1_0_2B18
# 
# Revision 1.1.4.2  1992/11/24  15:55:30  smythe
# 	added HP800_LIBS = -lm line needed by some of the tests
# 	[1992/11/24  15:49:55  smythe]
# 
# Revision 1.1.4.8  1993/02/01  19:52:03  tom
# 	Bug 6591 - revisit since both utc libraries weren't built.
# 	Now both are always built/exported and called libutc-{kernel,user}.
# 	[1993/02/01  18:30:55  tom]
# 
# Revision 1.1.4.5  1993/01/21  22:12:19  tom
# 	Bug 6591 - Add -lutc back to link line for test_kernel-{user,kernel}.
# 	This way they will actually test the code in user/kernel libutc.a.
# 	[1993/01/21  20:49:34  tom]
# 
# Revision 1.1.4.4  1992/12/31  20:35:04  htf
# 	Embedded copyright notice
# 	[1992/12/31  20:28:23  htf]
# 
# Revision 1.1.4.3  1992/11/02  15:39:03  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:42:12  root]
# 
# Revision 1.1.4.2  1992/09/29  21:25:16  devsrc
# 	[OT 5373]    SNI/SVR4 merge.
# 	[1992/09/15  13:47:14  sekhar]
# 
# Revision 1.1.1.6  1992/09/21  18:48:10  osfrcs
# 	    Loading drop Dce1_0_1fnl
# 
# Revision 1.1  1992/01/19  14:38:17  devrcs
# 	Initial revision
# 
# $EndLog$
#

# Build for rantest_api and test_kernel test program 
PROGRAMS = blech rantest_api rancover_api rancover2_api test_zonecvt test_kernel-user \
	   test_kernel utc_gettime_noshm

DATAFILES = zonecvt.dat

HP800_LIBS = -lm

ILIST	  = ${PROGRAMS} ${DATAFILES}

# compiler flags
CFLAGS = ${${TARGET_OS}_CFLAGS} -DSYSTEM_FIVE

# include directories 
INCFLAGS = -I../../../time/common/${TARGET_MACHINE}/${TARGET_OS_PLATFORM} \
	   -I../../../time/common  -I../../../time/libutc/user

# build blech libutc cover program
blech_OFILES =	blech.o 
blech_LIBS = -ldce -ltettcm -ltetapi ${${TARGET_MACHINE}_LIBS}

# build utc_gettime_noshm cover program
utc_gettime_noshm_OFILES =	utc_gettime_noshm.o 
utc_gettime_noshm_LIBS = -ldce -ltettcm -ltetapi ${${TARGET_MACHINE}_LIBS}

# build api test program
rantest_api_OFILES =	rantest_api.o 
rantest_api_LIBS = -ldce -ltettcm -ltetapi ${${TARGET_MACHINE}_LIBS}

# build api cover program
rancover_api_OFILES =	rancover_api.o 
rancover_api_LIBS = -ldce -ltettcm -ltetapi ${${TARGET_MACHINE}_LIBS}

rancover2_api_OFILES =	rancover2_api.o
rancover2_api_LIBS = -ldce -ltettcm -ltetapi ${${TARGET_MACHINE}_LIBS}

# build zone convert test program
test_zonecvt_OFILES = 	test_zonecvt.o 
test_zonecvt_LIBS = -ldce -ltettcm -ltetapi ${${TARGET_MACHINE}_LIBS}

# build user mode kernel test program
test_kernel-user_OFILES = test_kernel.o 
test_kernel-user_LIBS = -lutc-user -ldce -lm -ltettcm -ltetapi ${${TARGET_MACHINE}_LIBS}

# build shared lib kernel test program
test_kernel_OFILES = test_kernel.o 
test_kernel_LIBS = -ldce -lm -ltettcm -ltetapi ${${TARGET_MACHINE}_LIBS}

.include <${RULES_MK}>

.if exists(${TARGET_OS}/osdep.mk)
.include "${TARGET_OS}/osdep.mk"
.endif
