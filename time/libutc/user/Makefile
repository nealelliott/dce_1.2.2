#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
#(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
# All Rights Reserved
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.20.2  1996/03/09  23:31:41  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:50  marty]
#
# Revision 1.1.20.1  1995/12/08  18:12:58  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/4  1995/07/14  20:06 UTC  rmm
# 	merge back ux_release changes
# 
# 	HP revision /main/HPDCE02/rmm_wc_work/1  1995/06/29  22:46 UTC  rmm
# 	Change 1000 cond to work for Walnut Creek
# 
# 	HP revision /main/HPDCE02/3  1994/10/10  14:44 UTC  pare
# 	Merge bl21 changes.
# 	[1995/12/08  17:25:16  root]
# 
# Revision 1.1.18.2  1994/09/28  20:43:23  bowe
# 	Add dcedtsmsg.[co] - now in libdce. [CR 12366]
# 	[1994/09/28  20:42:44  bowe]
# 
# 	HP revision /main/HPDCE02/2  1994/07/15  10:57 UTC  mothra
# 	change <> to double quotes
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  12:55 UTC  pare
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/6  1994/03/28  20:43 UTC  pare
# 
# 	HP revision /main/HPDCE01/pare_rodan/4  1994/03/28  20:42 UTC  pare
# 	conditionalize so we don't build HP800 directory at 10.0
# 
# 	HP revision /main/HPDCE01/pare_rodan/3  1994/03/16  18:35 UTC  pare
# 	remove hpux1000 conditional
# 
# 	HP revision /main/HPDCE01/pare_rodan/2  1994/03/08  23:03 UTC  pare
# 	remove hpux1000 conditional
# 
# 	HP revision /main/HPDCE01/3  1994/02/18  18:02  pare
# 	Do not build HP800 subdir for hpux1000 OS
# 
# 	HP revision /main/HPDCE01/pare_rodan/1  1994/02/18  17:57  pare
# 	Do not build HP800 subdir for hpux1000 OS
# 
# 	HP revision /main/HPDCE01/2  1994/02/01  15:44  pare
# 	merge kk and hpdce01
# 
# 	HP revision /main/HPDCE01/1  1994/01/31  20:06  pare
# 	merge kk and hpdce01
# 
# Revision 1.1.11.3  1993/09/24  15:21:52  pare
# 	DCE1_0_3b03 Merge
# 	[1993/09/24  15:21:18  pare]
# 
# Revision 1.1.12.3  1993/09/15  17:48:01  root
# 	    Loading drop DCE1_0_3b03
# 	Revision 1.1.11.2  1993/07/08  19:57:29  root
# 	Initial King Kong branch
# 	[1993/07/08  19:57:00  root]
# 
# Revision 1.1.18.1  1994/05/12  15:17:45  rhw
# 	continued code cleanup & merging
# 	[1994/05/09  18:46:46  rhw]
# 
# Revision 1.1.16.2  1993/06/10  19:32:08  sommerfeld
# 	Clean up machdep.mk include so it happens before ${RULES_MK} is included.
# 	[1993/06/08  20:08:27  sommerfeld]
# 
# Revision 1.1.9.5  1993/06/11  16:15:30  truitt
# 	CHFts07810: Removed the LIBRARIES definition when no shared
# 	libraries is used.  The definition now defaults to libutc-user
# 	instead.
# 	[1993/06/11  16:15:05  truitt]
# 
# Revision 1.1.9.4  1993/03/26  13:26:30  truitt
# 	Put rules.mk include after machdep.mk include to avoid
# 	rule expansion ordering problems during nightly build.
# 	[1993/03/26  13:26:07  truitt]
# 
# Revision 1.1.9.3  1993/03/24  19:20:03  truitt
# 	Cleared up some confusion from the merge process surrounding the
# 	definition for LIBRARIES.
# 	[1993/03/24  19:19:40  truitt]
# 
# Revision 1.1.9.2  1993/03/18  14:55:24  truitt
# 	Merge Hp_dce1_0_1 with DCE1_0_2
# 	[1993/03/18  14:54:23  truitt]
# 
# Revision 1.1.1.8  1993/03/11  22:33:59  osfrcs
# 	    Loading drop DCE1_0_2B18
# 
# Revision 1.1.5.9  1992/07/10  18:14:28  smythe
# 	fixed typo which deleted } in PROGRAMS line
# 	[1992/07/10  18:13:58  smythe]
# 
# Revision 1.1.9.6  1993/02/01  19:52:10  tom
# 	Build user mode library and call it libutc-user.
# 	This is so the test will know which mode they are getting.
# 	[1993/02/01  18:31:18  tom]
# 
# Revision 1.1.9.3  1992/12/30  20:42:15  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:22:01  htf]
# 
# Revision 1.1.9.2  1992/09/29  21:28:12  devsrc
# 	[OT 5373]  SNI/SVR4 Merge.
# 	[1992/09/14  20:05:23  sekhar]
# 
# Revision 1.1.5.8  1992/07/09  17:15:06  smythe
# 	added -I${TARGET_MACHINE} to include list
# 	added support for shared library builds
# 	[1992/07/09  17:14:47  smythe]
# 
# Revision 1.1.5.7  1992/07/01  14:32:32  smythe
# 	back to .include <>
# 	[1992/07/01  14:32:13  smythe]
# 
# Revision 1.1.5.6  1992/07/01  14:11:45  smythe
# 	back to .include ""
# 	[1992/07/01  14:11:14  smythe]
# 
# Revision 1.1.5.5  1992/07/01  13:33:47  smythe
# 	changed .include "machdep.mk" to .include <machdep.mk>
# 	[1992/07/01  13:33:28  smythe]
# 
# Revision 1.1.2.5  1992/06/29  22:00:14  grober
# 	Add ability to build machine dependent .o files and binaries.
# 	[1992/06/29  17:50:03  grober]
# 
# Revision 1.1.5.4  1992/06/29  14:45:35  smythe
# 	put rules.mk include after machdep.mk include
# 	to avoid rule expansion ordering problems
# 	[1992/06/29  14:45:03  smythe]
# 
# Revision 1.1.5.3  1992/06/26  18:48:49  smythe
# 	added adjtime support for libdce build
# 	[1992/06/26  18:48:23  smythe]
# 
# Revision 1.1.5.2  1992/06/23  12:34:45  smythe
# 	added TARGET_MACHINE extensions to accomodate hpux adjtime
# 	[1992/06/23  12:33:16  smythe]
# 
# Revision 1.1.1.4  1992/06/16  07:06:07  root
# 	    Loading drop Dce1_0_1b19
# 
# Revision 1.1.2.4  1992/05/12  19:07:21  comuzzi
# 	Fixed typo, so libutc.a get object list
# 	[1992/05/12  19:07:01  comuzzi]
# 
# Revision 1.1.2.3  1992/05/06  21:56:34  comuzzi
# 	Fix defects 2295, 3143 and 2790.
# 	Added architecture dependent dirs in common to
# 	INCFLAGS. Export symbol for libdce Makefile.
# 	[1992/05/06  21:31:47  comuzzi]
# 
# Revision 1.1.2.2  1992/02/05  16:28:59  mhickey
# 	Removed the conditonal protecting the bulk of the Makefile from the
# 	libdce Makefile.  This was not working anyway, since
# 	LIBDCE_LIBRARY_MAKEFILE was spelled
# 	LIBDCE_LIBARAY_MAKEFILE in the '.if' statement
# 	          ^^^^
# 	[1992/02/05  16:28:25  mhickey]
# 
# Revision 1.1  1992/01/19  15:35:09  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# This is exported for libdce.
# probably nothing else should be in this section.
#
libutc_OFILES =  utcapi.o utc_cvt.o utc_gettime_user.o \
                 utc_adjtime_user.o utc_shared_create.o utc_shared_map.o \
                 dcedtsmsg.o ${${TARGET_MACHINE}_libutc_OFILES}

# Everything else is private

.if !defined(LIBDCE_LIBRARY_MAKEFILE)

# defines for shared libraries
OBJECTS = 
LIBS = 

# Build user-mode library

VPATH = ${TARGET_MACHINE}:../../common:../../common/POSIX

SAMSFILES	= dts.sams
SAMS_TARGETS	= dcedtsmsg.c

LIBRARIES = libutc-user.a
libutc-user.a_OFILES = ${libutc_OFILES}

.if ${USE_SHARED_LIBRARIES} == 0
  EXPLIB_TARGETS = ${LIBRARIES:S/^/export_/g}
.endif

# This is needed for building the ofiles for libdce.
EXPSHLIB_TARGETS=${libutc_OFILES}

# export directories
EXPDIR	= /usr/lib/

# compiler flags
CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} ${DCEPATHS}

# include directories
INCFLAGS = -I../../common -I../../common/${TARGET_MACHINE}/${TARGET_OS_PLATFORM} -I../../common/POSIX

# install directories and files for machine dependant things
.if ${ux_release}=="900"
PROGRAMS = ${${TARGET_MACHINE}_PROGRAMS}
.endif
IDIR = /usr/lib/
ILIST = ${${TARGET_MACHINE}_ILIST}

.endif

.if ${ux_release}=="900"
.if exists(${MAKETOP}/time/libutc/user/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/time/libutc/user/${TARGET_MACHINE}/machdep.mk"
.endif
.endif

.if !defined(LIBDCE_LIBRARY_MAKEFILE)
.include <${RULES_MK}>
.endif


