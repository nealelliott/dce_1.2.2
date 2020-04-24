# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
BINRevision 1.1.2.1  1996/10/02  17:50:23  damon
BIN	Newest DFS from Transarc
BIN
# Revision /main/HPDCE02/3  1995/07/28  12:39 UTC  maunsell_c
# 	walnut creek port
# 
# Revision /main/HPDCE02/2  1995/06/22  20:14 UTC  maunsell_c
# 	mothra C&S fixes
# 
# Revision /main/HPDCE02/brezak_dfsgw/3  1995/06/12  14:08 UTC  brezak
# 	Add user space test program.
# 
# Revision /main/HPDCE02/brezak_dfsgw/2  1995/05/26  17:26 UTC  brezak
# 	create msgcat for dfsgw_trace
# 
# Revision /main/HPDCE02/brezak_mothra10/1  1995/05/18  16:23 UTC  brezak
# 	Add check for osdep and machdep mkfiles.
# 
# Revision /main/HPDCE02/1  1995/02/14  22:42 UTC  maunsell_c
# 	@sys @host changes for nfs/dfs gateway
# 
# Revision /main/maunsell_mothra1.45/2  1995/01/31  21:05 UTC  maunsell_c
# 	use dcedfs cm.h instead of ../../cm.h
# 
# Revision /main/maunsell_mothra1.45/1  1995/01/31  19:21 UTC  maunsell_c
# 	add @sys @host support for dfs_login/dfsgwd/dfsgw
# 
# Revision 1.1.2.1  1994/07/26  19:13:32  mckeen
# 	Expanded copyrights
# 	[1994/07/26  18:58:04  mckeen]
#
# 	Initial Version
# 	[1994/07/26  16:07:21  mckeen]
#
# $EndLog$

VPATH		= ${TARGET_MACHINE}:${TARGET_OS}:../libgwauth:../libgwauth/${TARGET_MACHINE}:../libgwauth/${TARGET_OS}

INCFLAGS                = -I../libgwauth -I../../cm 

LIBRARIES		= libkgwauth.a

EXPDIR			= /usr/include/dcedfs/
libkgwauth.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS		= ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=

.if ${TARGET_MACHINE}=="HP800"
.if ${ux_release} >= 1000
CFLAGS			= ${KERNELCFLAGS} +e
.else
CFLAGS                  = ${KERNELCFLAGS}
.endif
.else 
CFLAGS			= ${KERNELCFLAGS}
.endif

libkgwauth.a_OFILES	= auth_at.o auth_at_sys.o auth_at_ki.o

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${TARGET_OS}/machdep.mk)
.include "${TARGET_OS}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>

