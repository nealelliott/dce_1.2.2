#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
# the full copyright text.
#
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
# All Rights Reserved
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.50.4  1996/10/03  17:48:25  arvind
# 	Better NO_GDS support.
# 	[1996/09/17  06:23 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
#
# 	Add expect_dce to the BUILD_ADMIN list.
# 	[1996/07/17  20:01 UTC  psn  /main/DCE_1.2.2/psn_krb5/1]
#
# 	Add expect_dce to the build.
# 	[1996/05/30  21:58 UTC  sommerfeld  /main/DCE_1.2.2/2]
#
# 	fix compile error (dce subtree not visited during AUTOGEN pass..)
# 	[1996/05/30  21:13 UTC  mullan_s  /main/DCE_1.2.2/1]
#
# 	Merge krb5 work to DCE1.2.2 mainline
# 	[1996/05/29  20:28 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
#
# Revision 1.2.50.3  1996/08/09  11:44:10  arvind
# 	Add expect_dce to the BUILD_ADMIN list.
# 	[1996/07/17  20:01 UTC  psn  /main/DCE_1.2.2/psn_krb5/1]
# 
# 	Add expect_dce to the build.
# 	[1996/05/30  21:58 UTC  sommerfeld  /main/DCE_1.2.2/2]
# 
# 	fix compile error (dce subtree not visited during AUTOGEN pass..)
# 	[1996/05/30  21:13 UTC  mullan_s  /main/DCE_1.2.2/1]
# 
# 	Merge krb5 work to DCE1.2.2 mainline
# 	[1996/05/29  20:28 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
# 
# Revision 1.2.50.2  1996/06/04  21:40:38  arvind
# 	fix compile error (dce subtree not visited during AUTOGEN pass..)
# 	[1996/05/30  21:13 UTC  mullan_s  /main/DCE_1.2.2/1]
# 
# 	Merge krb5 work to DCE1.2.2 mainline
# 	[1996/05/29  20:28 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
# 
# 	Update OSF copyright year
# 	[1996/03/09  22:24:56  marty]
# 
# 	#
# 
# 	ENDGAME Submission
# 	[1996/02/22  16:10:46  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  20:57:55  root]
# 
# 	Add mit_krb5 directory to build rules.
# 	[1996/05/03  19:54 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/1]
# 
# Revision 1.2.50.1  1996/05/10  13:06:13  arvind
# 	Drop 1 of DCE 1.2.2 to OSF
# 
# 	HP revision /main/DCE_1.2/1  1996/03/14  18:45 UTC  psn
# 	Merge Endgame fixes.
# 
# 	HP revision /main/psn_1.2_endgame/1  1996/02/28  20:11 UTC  psn
# 	Fix OT13347 as done during OSF 1.2.1 Endgame.
# 	[1996/05/09  20:41:50  arvind]
# 
# Revision 1.2.32.27  1994/09/07  17:36:54  annie
# 	fixed for cr10833
# 	[1994/09/07  16:18:10  annie]
# 
# Revision 1.2.32.26  1994/08/02  20:27:24  sasala
# 	Add dfs to SAMIDL_SUBDIRS
# 	[1994/08/02  20:25:35  sasala]
# 
# Revision 1.2.32.25  1994/07/11  21:05:29  marty
# 	CR # 9676 - Shorten history section
# 	[1994/07/11  19:50:47  marty]
# 
# Revision 1.2.32.24  1994/06/09  17:02:46  annie
# 	cr10911 - order of IDIR and ILIST is reversed.  mama-mia
# 	[1994/06/09  17:00:46  annie]
# 
# Revision 1.2.32.23  1994/06/09  16:16:55  annie
# 	CR10892 - fix copyright
# 	[1994/06/09  16:15:47  annie]
# 
# Revision 1.2.32.22  1994/06/08  14:17:53  annie
# 	fix bug 10892
# 	[1994/06/08  14:17:38  annie]
# 
# Revision 1.2.32.21  1994/05/25  15:36:33  tom
# 	Remove diskless.
# 	[1994/05/25  15:36:10  tom]
# 
# Revision 1.2.32.20  1994/05/23  23:13:23  sommerfeld
# 	Add rpc to SAMIDL_SUBDIRS; move outside of .if
# 	[1994/05/23  19:27:54  sommerfeld]
# 
# Revision 1.2.32.19  1994/05/05  18:07:06  zeliff
# 	OT10003: added BUILD_LIBNDS to build GDS libNDS after libdce.
# 	[1994/05/05  18:02:56  zeliff]
# 
# Revision 1.2.32.18  1994/04/19  20:31:32  rsalz
# 	Add dced to SAMIDL pass (OT CR 10364)
# 	[1994/04/19  17:12:47  rsalz]
# 
# Revision 1.2.32.17  1994/04/18  00:14:47  pwang
# 	Loaded libdced into libdce (BIULD_DCE) OT#10158
# 	[1994/04/17  21:52:28  pwang]
# 
# Revision 1.2.32.16  1994/04/07  17:52:36  mario
# 	Removed HPisms (${BUILD_HP_VALUE_ADDED} and ${NO_LATENT_DFS_SUPPORT} to make
# 	the Makefile file more vendor neutral.
# 	[1994/04/07  17:51:22  mario]
# 
# Revision 1.2.32.15  1994/03/25  19:44:09  sekhar
# 	[ OT 10164 ] Security SVC code drop ( sec_svc_bl1 )
# 	             removed BUILD_AUDIT ; added security SAMIDL_SUBDIRS
# 	[1994/03/25  19:43:14  sekhar]
# 
# Revision 1.2.32.14  1994/01/27  19:28:10  rhw
# 	Add time to SAMIDL_SUBDIRS.
# 	[1994/01/27  19:00:02  rhw]
# 
# Revision 1.2.32.13  1994/01/24  22:01:29  bowe
# 	Visit dce/utils/svc and dce/utils/dcecf (BUILD_DCE variable)
# 	early on instead of just dce.
# 	[1994/01/24  21:48:59  bowe]
# 
# Revision 1.2.32.12  1994/01/13  16:07:31  peckham
# 	Add SAMIDL_SUBDIRS for directory tree.
# 	[1994/01/13  16:06:53  peckham]
# 
# Revision 1.2.32.11  1994/01/12  20:51:02  zeliff
# 	OT9726: turn Audit on in the build
# 	[1994/01/12  19:36:52  zeliff]
# 
# Revision 1.2.32.10  1994/01/05  18:32:38  rsalz
# 	Yet more build-order fixes (OT CR 9663)
# 	[1994/01/05  18:32:20  rsalz]
# 
# Revision 1.2.32.9  1994/01/05  16:35:50  rsalz
# 	Trying again to fix build order (OT CR 9663).
# 	[1994/01/05  16:35:39  rsalz]
# 
# Revision 1.2.32.8  1994/01/04  19:35:26  ohara
# 	fix the include of machdep.mk for new make
# 	[1994/01/04  17:34:02  ohara]
# 
# 	fix include of machdep.mk for new make
# 
# Revision 1.2.32.7  1994/01/03  19:35:00  rsalz
# 	Get subdirs right (OT CR 9663).
# 	[1994/01/03  19:30:50  rsalz]
# 
# Revision 1.2.32.6  1993/12/30  20:20:45  rsalz
# 	More fixes to build order (part of OT CR 9663).
# 	[1993/12/30  20:20:36  rsalz]
# 
# Revision 1.2.32.5  1993/12/30  16:44:52  rsalz
# 	Fix build order (part of OT CR 9663).
# 	[1993/12/30  16:44:32  rsalz]
# 
# Revision 1.2.32.4  1993/12/29  19:21:51  rsalz
# 	Move dce_cf_XXX from config to dce/utils/dcecf (OT CR 9663).
# 	[1993/12/29  16:39:46  rsalz]
# 
# Revision 1.2.32.3  1993/12/14  23:12:59  mario
# 	Added admin (dcecp and tcl) to be built from top
# 	[1993/12/14  23:12:42  mario]
# 
# Revision 1.2.32.2  1993/12/14  16:57:49  mario
# 	Changes required to get svc built in libdce - added sams to setup pass
# 	[1993/12/14  16:57:16  mario]
# 
# Revision 1.2.32.1  1993/12/08  00:30:37  mario
# 	Removed DCE_MAKE conditional and the compiling of dce make in the setup pass
# 	[1993/12/08  00:30:14  mario]
# 
# Revision 1.2.30.1  1993/10/06  01:56:52  zeliff
# 	Removing debugging stuff
# 	[1993/10/06  01:43:06  zeliff]
# 
# 	Adding noship to BUILD_DFS on HPUX
# 	[1993/10/06  01:42:24  zeliff]
# 
# $EndLog$
#
#
# 
#  OSF DCE Version 1.1 
#

BUILD_FROM_TOP=true
#
# Set some list variables based on project switch values
#
.ifdef	DFS_ONLY
NO_DISKLESS=true
NO_THREADS=true
.endif

.if !defined(NO_DFS) || defined(DFS_ONLY)
BUILD_DFS = file
.if ${TARGET_OS} == "HPUX"
BUILD_DFS += noship
.endif
#.if ${TARGET_OS} == "AIX"
#BUILD_LIBDCEDFS = libdcedfs
#.endif
.endif

.if ${USE_SHARED_LIBRARIES} == 0
LIBDCE_LAST ?= libdce
.endif

.ifndef NO_THREADS
BUILD_THREADS ?= threads
.endif

BUILD_ADMIN ?= admin/tcl_dce admin/dcecp admin/expect_dce

.if defined (USE_OSC1_0_4_LEX)
# Defining USE_OSC1_0_4_LEX assumes that the lex sources are available
# in the "noship" directory.
NOSHIP = noship
.endif

SETUP_SUBDIRS=tools dce time security ${NOSHIP} rpc directory dce mit_krb5

.ifdef	DFS_ONLY
BUILD_DCE = rpc dce config
BUILD_LIBDCE =
.else
BUILD_DCE = rpc dce/utils/svc dce/utils/dcecf config time security \
	    dce directory admin/dced mit_krb5
BUILD_LIBDCE = libdce
.if !defined(NO_GDS)
BUILD_LIBNDS = directory/gds/nds
.endif
.endif

SAMIDL_SUBDIRS=rpc directory time security dce admin file 
.if ${TARGET_OS} == "HPUX"
EXPINC_SUBDIRS=${BUILD_THREADS} ${BUILD_DCE} ${BUILD_DFS:Ufile} \
               ${BUILD_ADMIN}
.else
EXPINC_SUBDIRS=${BUILD_THREADS} ${BUILD_DCE} ${BUILD_DFS} \
               ${BUILD_ADMIN}
.endif
#DATA_SUBDIRS=  ${BUILD_THREADS} time security directory
.if ${TARGET_OS} == "HPUX"
DATA_SUBDIRS=	${BUILD_DFS}
.endif
EXPSHLIB_SUBDIRS = ${BUILD_LIBDCE}
EXPLIB_SUBDIRS=${BUILD_THREADS} ${BUILD_DCE} \
               ${BUILD_DFS} ${LIBDCE_LAST} \
	       ${BUILD_LIBNDS} ${BUILD_LIBDCEDFS} ${BUILD_ADMIN}
SUBDIRS=       ${BUILD_THREADS} ${BUILD_DCE} ${BUILD_LIBDCE} \
               ${BUILD_DFS} ${BUILD_LIBDCEDFS} \
	       ${BUILD_ADMIN}

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

ILIST= COPYRIGHT.DCE
IDIR= /

.include <${RULES_MK}>
