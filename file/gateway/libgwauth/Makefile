# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/10/02  17:50:24  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:39:37  damon]
#
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

INCLUDES		= auth_at.h  dfsgw_trace.h authtest.h

PROGRAMS		= authtest

EXPDIR			= /usr/include/dcedfs/

CFLAGS			= ${USERCFLAGS}

authtest_OFILES		= authtest.o auth_at.o
authtest_LIBS		= -lafsutil -lcom_err -losi -licl -ldce ${OS_LIBS}

OTHERS			= dfszgw.cat
dfszgw.cat_IDIR		= /nls/msg/en_US.ASCII/
ILIST			= ${OTHERS}

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

dfsgw_trace.h dfsgw_trace.msf: dfsgw_trace.et
	${COMPILE_ET} ${dfsgw_trace.et:P} -h dfsgw_trace

dfszgw.cat: dfsgw_trace.msf
	${GENCAT} $@ $>
