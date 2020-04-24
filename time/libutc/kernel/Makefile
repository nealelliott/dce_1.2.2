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
# Revision 1.2.17.2  1996/03/09  23:31:33  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:43  marty]
#
# Revision 1.2.17.1  1995/12/08  18:11:54  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/jrr_xidl/1  1995/09/19  17:47 UTC  jrr
# 	Fix typo
# 	[1995/12/08  17:24:58  root]
# 
# Revision 1.2.14.4  1994/09/28  20:43:22  bowe
# 	Add dcedtsmsg.[co] - now in libdce. [CR 12366]
# 	[1994/09/28  20:42:41  bowe]
# 
# Revision 1.2.14.3  1994/05/12  15:17:43  rhw
# 	continued code cleanup & merging
# 	[1994/05/09  18:46:45  rhw]
# 
# Revision 1.2.14.2  1993/12/07  17:54:39  mario
# 	Changed for ode2.3 - < and > becomes this except for RULES_MK
# 	[1993/12/07  17:25:27  mario]
# 
# Revision 1.2.7.7  1993/02/01  19:52:08  tom
# 	Build the kernel mode library and call it libutc-kernel.
# 	This is so the test will know which mode they are getting.
# 	[1993/02/01  18:31:11  tom]
# 
# Revision 1.2.7.4  1992/12/30  20:41:32  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:19:47  htf]
# 
# Revision 1.2.7.3  1992/10/13  18:13:09  marty
# 	OSC1.1.1 upgrade.  Changes are osc1.0.4 compatible
# 	[1992/10/13  17:15:06  marty]
# 
# Revision 1.2.8.2  1992/09/30  17:54:15  garyf
# 	Merge OSC1.1.1 changes to dce1.0.2 (post SVR4)
# 	[1992/09/30  17:47:35  garyf]
# 
# Revision 1.2.7.2  1992/09/29  21:27:39  devsrc
# 	[OT 5373]  SNI/SVR4 Merge.
# 	[1992/09/14  20:04:50  sekhar]
# 	Revision 1.2.4.2  1992/09/29  20:28:48  garyf
# 	fix include of machdep.mk
# 
# Revision 1.2.3.4  1992/05/11  17:51:38  mhickey
# 	Removed obsolete define of NEED_PROJECT_MAKE_DEFS
# 	and include of osf.dce.mk.  Fixes defect 2031.
# 	[1992/04/22  15:23:37  mhickey]
# 
# Revision 1.2.3.3  1992/05/07  21:13:52  rec
# 	Added $ to TARGET_MACHINE}_libutc_OFILES to force expansion to
# 	get the machine dependent object.  Also added ../user to INCFLAGS
# 	line to search for header files there.
# 	[1992/05/07  21:08:45  rec]
# 
# Revision 1.2.3.2  1992/05/06  21:55:00  comuzzi
# 	Fix defects 2295, 3143 and 2790. Added architecture specific dirs to INCFLAGS.
# 	Include architecture dependent machdep's for libdce Makefile.
# 	[1992/05/06  21:40:07  comuzzi]
# 
# Revision 1.2  1992/01/19  22:14:52  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
# 
# 
#  OSF DCE Version 1.0 
# 

###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#

libutc_OFILES = utcapi.o utc_cvt.o dcedtsmsg.o \
	${${TARGET_MACHINE}_libutc_OFILES}

#
# The rest of the makefile is private
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)

# build kernel-mode library 
VPATH = ${TARGET_MACHINE}:../../kernel/${TARGET_MACHINE}:../../common:../user:../../common/POSIX

SAMSFILES	= dts.sams
SAMS_TARGETS	= dcedtsmsg.c

LIBRARIES = libutc-kernel.a
libutc-kernel.a_OFILES = ${libutc_OFILES}

# This is needed for building the ofiles for libdce.
EXPSHLIB_TARGETS=${libutc_OFILES}

# compiler flags
CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} 

# include directories
INCFLAGS = -I../../common -I../../common/${TARGET_MACHINE}/${TARGET_OS_PLATFORM} -I../user -I../../common/POSIX

# export library
EXPDIR = /usr/lib/

LINTFILES = utcapi.c utc_cvt.c

# For AIX, the symbols in utc_gettime.o and utc_adjtime.o are defined in
#   the kernel extension kern_utctime.  Resolution is deferred at link
#   time via dtskernext.exp.  These symbols are bound to the entry points
#   in the kernel extension at load time.

.include <${RULES_MK}>

.endif

.if exists(${MAKETOP}/time/libutc/kernel/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/time/libutc/kernel/${TARGET_MACHINE}/machdep.mk"
.endif
