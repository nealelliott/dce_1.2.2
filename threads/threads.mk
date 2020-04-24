# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
# Burlington, MA, USA
# All Rights Reserved
# (c) Copyright 1990, 1991, 1992
#
#
# HISTORY
# $Log: threads.mk,v $
# Revision 1.2.22.1  1996/05/10  13:46:29  arvind
# 	Drop 1 of DCE 1.2.2 to OSF
#
# 	HP revision /main/DCE_1.2/5  1996/03/18  21:40 UTC  psn
# 	Fix DCE 1.2.2 build errors on HPUX.
#
# 	HP revision /main/DCE_1.2/4  1996/03/14  20:33 UTC  psn
# 	Fix CMA build error after single threaded merge take 2.
#
# 	HP revision /main/DCE_1.2/3  1996/03/14  20:28 UTC  psn
# 	Fix CMA build error after single threaded merge.
#
# 	HP revision /main/DCE_1.2/2  1996/03/08  21:48 UTC  bissen
# 	merge back ux_release and p_is_running_PSTUB changes
# 	[1995/07/14  20:02 UTC  rmm  /main/HPDCE02/7]
#
# 	merge out Joe's changes so I can merge back to the mainline
# 	[1995/07/14  20:00 UTC  rmm  /main/HPDCE02/rmm_wc_work/3]
#
# 	Merge pthread.h changes for WC BE.
# 	[1995/07/11  17:25 UTC  jss  /main/HPDCE02/6]
#
# 	Added pthread_int.h to the HP800_INCLUDES.
# 	[1995/07/11  17:19 UTC  jss  /main/HPDCE02/jss_mothra_cose/2]
#
# 	Merge Spec 1170 changes for Walnut Creek.
# 	[1995/07/10  20:07 UTC  jss  /main/HPDCE02/5]
#
# 	merge Joes add of cma_cose.c
# 	[1995/07/05  21:20 UTC  rmm  /main/HPDCE02/rmm_wc_work/2]
#
# 	CHFts15474. Added file cma_cose.c.
# 	[1995/06/09  18:14 UTC  jss  /main/HPDCE02/jss_mothra_cose/1]
#
# 	change name of p_is_running_P to p_is_running_PSTUB.o for WC
# 	[1995/06/29  15:35 UTC  rmm  /main/HPDCE02/rmm_wc_work/1]
#
# 	Merge in changes to CHFts14939 and CHFts14807 from branch jss_mothra_fix3.
# 	[1995/04/26  20:04 UTC  jss  /main/HPDCE02/4]
#
# 	Clean up.  Only need one _start.o module for both 9.0 and 10.0.
# 	[1995/04/05  22:04 UTC  jss  /main/HPDCE02/jss_mothra_fix3/1]
#
# 	move purify stub to threads build
# 	[1995/03/28  23:22 UTC  rps  /main/HPDCE02/3]
#
# 	move purify stub to threads build
# 	[1995/03/28  21:41 UTC  rps  /main/HPDCE02/rps_mothra/1]
#
# 	Merge from branch jss_fork_mothra to HPDCE02.
# 	[1994/12/09  16:25 UTC  jss  /main/HPDCE02/2]
#
# 	Add define for _HP_CMA_ which enables exception wrapper performance changes.
# 	[1994/12/07  22:23 UTC  jss  /main/HPDCE02/jss_fork_mothra/1]
#
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 	[1994/07/06  16:46 UTC  jss  /main/HPDCE02/1]
#
# 	[1994/06/06  20:44 UTC  jss  /main/HPDCE01/2]
#
# 	Modified make to select the correct _start.o for 9.0 and 10.0 builds.
# 	[1994/06/06  20:42 UTC  jss  /main/HPDCE01/jss_rodan_q/1]
#
# 	merge kk and hpdce01
# 	[1994/01/25  18:15  truitt  /main/HPDCE01/1]
#
# 	Changed use of ux_release to expect
# 	more digits worth of precision.
# 	Add hpdce_platform.h to HP800_INCLUDES.
# 	[1993/11/03  19:43:57  hopkins]
# 	#
#
# 	Change ux_version to ux_release.
# 	[1993/09/22  20:42:11  hopkins]
#
# 	Remove code that adds -D__hpux100 to CFLAGS;
# 	this is now done (a little differently) by
# 	Makeconf.
# 	Change conditional expression for adding
# 	cma_streams.o to OFILES to use ux_version
# 	instead of HPUX_100.
# 	[1993/09/22  19:53:29  hopkins]
#
# 	merge kk and dce1_0_3
# 	[1993/09/21  15:20:49  truitt]
#
# 	Loading drop DCE1_0_3b03
# 	[1993/09/15  17:45:17  root  1.1.9.3]
#
# 	clean up genpath/INCDIRS macros slightly.
# 	[1993/09/02  17:56:23  sommerfeld  1.1.8.4]
#
# 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
# 	[1993/06/17  14:48:53  hinman]
#
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:18:32  cjd]
#
# 	Initial 486 port.
# 	[1993/05/11  21:45:12  jd]
#
# 	[1993/08/31  14:07:04  sommerfeld]
#
# 	clean up genpath stuff.
# 	[1993/07/29  20:29:51  sommerfe]
#
# 	Change HPUX_100 define to __hpux100
# 	[1993/08/11  17:47:14  hopkins]
#
# 	Add support for STREAMS in particular
# 	and HPUX_100 differences in general.
# 	[1993/08/06  19:40:02  hopkins]
#
# 	Initial King Kong branch
# 	[1993/07/08  19:29:19  root]
#
# 	Gamera merge
# 	[1993/03/26  23:57:47  hopkins]
#
# 	Loading drop DCE1_0_2B18
# 	[1993/03/11  22:29:57  osfrcs  1.1.1.8]
#
# 	Add cma_exec.o to HP800_OFILES
# 	[1992/12/21  23:28:41  hopkins]
#
# 	HP revision /main/DCE_1.2/1  1996/01/03  18:49 UTC  psn
# 	Changes to allow valid AIX core dumps.
# 	[1995/04/26  16:56 UTC  jrr  /main/DCE1_1_WP/1]
#
# Revision 1.2.12.2  1994/06/09  13:39:24  devsrc
# 	 CR10892 - fix copyright in file
# 	[1994/06/09  13:29:11  devsrc]
# 
# Revision 1.2.12.1  1994/04/01  20:18:16  jd
# 	Fist drop of code cleanup
# 	[1994/03/28  19:20:03  jd]
# 
# Revision 1.2.10.3  1993/06/24  21:23:13  hinman
# 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
# 	[1993/06/17  14:48:53  hinman]
# 
# Revision 1.2.10.2  1993/05/24  20:50:57  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:18:32  cjd]
# 
# Revision 1.2.8.2  1993/05/11  22:02:49  jd
# 	Initial 486 port.
# 	[1993/05/11  21:45:12  jd]
# 
# Revision 1.2.4.5  1993/02/01  22:33:02  hinman
# 	[hinman@sni] - Final merge before bsubmit
# 	[1993/01/31  17:40:15  hinman]
# 
# 	[hinman] - Check in merged SNI version
# 	[1993/01/11  16:24:38  hinman]
# 
# Revision 9.5.1.13  92/12/18  16:21:32  blurie
# 	Reentrant library changes.
# 
# 	Add additional SVR4 include files to SVR4_INCLUDES.
# 	[92/12/18  16:18:44  blurie]
# 
# Revision 9.5.1.12  92/12/17  16:05:56  raj
# 	[raj] New reentrant library changes
# 
# Revision 9.5.1.11  92/12/17  14:16:43  raj
# 	[raj] New naming + parallelized reentrant library changes.
# 
# Revision 9.5.1.10  92/09/30  12:43:47  root
# 	Acceptance of OSF rev 1.2.4.3
# 
# Revision 9.12.1.2  92/09/30  10:39:32  hinman
# 	Auto checkin of OSF rev 1.2.4.3
# 
# Revision 1.2.4.3  1992/09/29  20:16:41  devsrc
# 	[OT 5373]    SNI/SVR4 merge.
# 	[1992/09/17  21:01:14  sekhar]
# 
# Revision 1.2.4.2  1992/09/03  14:43:32  bolinger
# 	Replace current source file with equivalent from DEC BL10+
# 	source base, as originally integrated in the
# 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
# 	[1992/09/02  01:56:50  bolinger]
# 
# Revision 1.1.2.3  1992/05/10  17:05:48  bolinger
# 	Merge DEC BL10 threads.mk from nosupport/threads_bl10 with current
# 	threads.mk (revision 1.2.2.2) from threads directory.
# 	[1992/05/10  17:05:32  bolinger]
# 
# Revision 1.2.2.2  1992/04/14  13:20:17  mishkin
# 	      Add HP[38]00_INCLUDES, APOLLO68K_INCLUDES.
# 	[1992/04/10  19:44:52  mishkin]
# 
# Revision 1.2  1992/01/19  22:14:50  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
# Copyright (c) 1990, 1991  
# Open Software Foundation, Inc. 
#  
# Permission is hereby granted to use, copy, modify and freely distribute 
# the software in this file and its documentation for any purpose without 
# fee, provided that the above copyright notice appears in all copies and 
# that both the copyright notice and this permission notice appear in 
# supporting documentation.  Further, provided that the name of Open 
# Software Foundation, Inc. ("OSF") not be used in advertising or 
# publicity pertaining to distribution of the software without prior 
# written permission from OSF.  OSF makes no representations about the 
# suitability of this software for any purpose.  It is provided "as is" 
# without express or implied warranty. 
#
#  OSF DCE Version 1.0 
#
#
# Special targets:
#
#	OSF/1 with kernel thread support:
#		setenv TARGET_MACHINE MACH
#		setenv target_machine mach
#	OSF/1 without kernel thread support
#		setenv TARGET_MACHINE PMAX
#		setenv target_machine pmax
#	OSF/1 without kernel thread support or shared library
#		setenv TARGET_MACHINE PMAXN
#		setenv target_machine pmaxn
#

.if !defined(_THREADS_MK_) || defined (LIBDCE_LIBRARY_MAKEFILE)
_THREADS_MK_=

RIOS_CFLAGS_ALL = -Dunix -D_ALL_SOURCE
RIOS_CFLAGS	= ${RIOS_CFLAGS_ALL} -DAIX_CORE_FILE -DHP_MOD_FOR_CORE
HP800_CFLAGS	= -w1 -D_CMA_SYNC_KILL_
AT386_CFLAGS    = -D_CMA_SYNC_KILL_ -D_USER_THREADS_ -ansi -pedantic -Wchar-subscripts -Wcomment -Wformat -Wid-clash-32 -Wimplicit -Wmissing-prototypes  -Wreturn-type -Wstrict-prototypes -Wswitch -Wtraditional
AT386_AFLAGS	= -D__i386__ -Di386 \
 -D__host_i386__ -D__unix__ -D__osf__ -D__LANGUAGE_ASSEMBLY__\
 -DLANGUAGE_C -Dultrix -D_CMA_UNIPROCESSOR_
SVR4_CFLAGS   ?=      -D _libcma_code_
SVR4_AFLAGS   =       -D LANGUAGE_ASSEMBLY -I.

RIOS_INCLUDES	?= cma_host.h cma_ux.h cma_stdio.h
AT386_INCLUDES	?= cma_host.h cma_ux.h cma_stdio.h
HP800_INCLUDES	+= cma_host.h cma_ux.h cma_stdio.h

SVR4_INCLUDES ?=      cma_host.h cma_dummy_defs_2.h cma_dummy_defs_3.h \
		      cma_io_redefs_2.h cma_io_redefs_3.h \
		      cma_io_redefs_4.h cma_libc_redefs.h cma_stdio_redefs.h \
		      cma_ux_redefs.h cma_ux.h cma_stdio.h


AT386_libcma_OFILES	?= cma_vmprot.o cma_thread_io.o cma_thdio_2.o \
                           cma_thdio_3.o cma_thdio_4.o  cma_ux.o \
		  	   cma_stdio.o cma_signal.o cma_host.o
RIOS_libcma_OFILES	?= getasyncinfo.o cma_thread_io.o cma_thdio_2.o \
                           cma_thdio_3.o cma_thdio_4.o  cma_ux.o \
                           cma_stdio.o cma_signal.o cma_host.o cma_core.o

HP800_libcma_OFILES     ?= cma_host.o cma_thread_io.o cma_thdio_2.o \
                           cma_thdio_3.o  cma_thdio_4.o  cma_ux.o \
                           cma_stdio.o cma_signal.o cma_hpux.o cma_exec.o

.if ${ux_release} >= 1000
HP800_libcma_OFILES	+= cma_streams.o
.endif
.if ${ux_release} >= 1010
HP800_libcma_OFILES	+= cma_cose.o
.endif
SVR4_libcma_OFILES    ?= cma_vmprot_sysv.o cma_int.o cma_thread_io.o \
			 cma_thdio_2.o \
                         cma_thdio_3.o cma_thdio_4.o  cma_stdio.o cma_ux.o \
                         cma_signal.o cma_host.o


CFLAGS ?= ${${TARGET_MACHINE}_CFLAGS}
.endif
