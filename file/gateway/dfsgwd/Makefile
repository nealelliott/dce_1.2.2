#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/10/02  17:50:03  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:39:24  damon]
#
# Revision /main/HPDCE02/7  1995/08/17  13:50 UTC  kissel
# 	Fix to not build with archive libraries if not DES_HIDDEN (it was right at KK+
# 	but got screwed up along the way).
# 
# Revision /main/HPDCE02/6  1995/07/28  15:21 UTC  maunsell_c
# 	walnut creek port
# 
# Revision /main/HPDCE02/5  1995/06/22  20:13 UTC  maunsell_c
# 	mothra C&S fixes
# 
# Revision /main/HPDCE02/brezak_dfsgw/1  1995/05/26  17:26 UTC  brezak
# 	Need to link in -losi
# 
# Revision /main/HPDCE02/4  1995/02/14  22:42 UTC  maunsell_c
# 	@sys @host changes for nfs/dfs gateway
# 
# Revision /main/HPDCE02/maunsell_mothra1.45/2  1995/01/31  22:20 UTC  maunsell_c
# 	use dcedfs/cm.h not ../../cm.h
# 
# Revision /main/HPDCE02/maunsell_mothra1.45/1  1995/01/31  19:21 UTC  maunsell_c
# 	add @sys @host support for dfs_login/dfsgwd/dfsgw
# 
# Revision /main/HPDCE02/3  1994/09/21  16:33 UTC  kissel
# 	Merge to OSF bl18.
# 
# Revision 1.1.2.4  1994/09/01  13:45:12  mckeen
# 	Removed improper export of dfsgwymsg.h
# 	[1994/09/01  12:52:46  mckeen]
# 
# Revision 1.1.2.3  1994/08/22  16:16:17  maunsell_c
# 	remove sendrcv.o from build, here erroneously
# 	[1994/08/10  16:34:00  maunsell_c]
# 
# Revision /main/HPDCE02/2  1994/09/01  14:25 UTC  maunsell_c
# 	fixes to Makefile post osf1.1 merge1
# 
# Revision /main/HPDCE02/maunsell_mothra1.4/2  1994/09/01  14:07 UTC  maunsell_c
# 	add hp product cflags, 10.0 build and DES
# 	and remove export of msg catalog include file.
# 
# Revision /main/HPDCE02/1  1994/08/30  13:39 UTC  maunsell_c
# 	fix -lm LIBS build, remove sendrcv from build
# 
# Revision /main/HPDCE02/maunsell_mothra1.4/1  1994/08/30  13:18 UTC  maunsell_c
# 	use -lm in LIBS, remove sendrcv from build.
# 
# Revision 1.1.2.2  1994/08/08  18:54:36  mckeen
# 	Added serviceability
# 	[1994/08/08  17:46:58  mckeen]
# 
# Revision 1.1.2.1  1994/07/26  19:13:15  mckeen
# 	Expanded copyrights
# 	[1994/07/26  18:57:52  mckeen]
# 
# 	Initial Version
# 	[1994/07/26  16:05:35  mckeen]
# 
# $EndLog$
#

PROGRAMS		= dfsgwd

INCFLAGS		= -I. 
.if ${TARGET_MACHINE}=="HP800"
CFLAGS			= -D_POSIX_REENTRANT_FUNCTIONS -D_REENTRANT ${USERCFLAGS}
.else
CFLAGS			= ${USERCFLAGS}
.endif

.if ${TARGET_MACHINE}=="HP800"
.if ${ux_release} >= 1000
LTOP                    = ../../../../hp700_ux100
.else
LTOP                    = ../../../../hp800
.endif

.ifndef DES_HIDDEN
DCE_LIBS                = -ldce
.else
LDIRS                   = -L${LTOP}/security/krb5/lib/krb5
DCE_LIBS                = ${LTOP}/libdce/bound_dce_nostrip.o -ldce -lkrb5
.if ${ux_release} >= 1000
LDFLAGS                 = -Wl,-a,archive -Wl,+n -l:libdld.sl
.else
LDFLAGS                 = -Wl,-a,archive
.endif
.endif

.if ${ux_release} >= 1000
LIBS                    = ${LDIRS} -lcmd -lncompat -losi ${DCE_LIBS}
.else
LIBS                    = ${LDIRS} -lcmd -lncompat -losi ${DCE_LIBS} -lm
.endif

.else

LIBS			= -lcmd -lncompat -losi -ldce ${OS_LIBS}

.endif

dfsgwd_OFILES 		= as.o dfsgwd.o tgt_unpack.o \
			  dfsgwymsg.o dfsgwysvc.o
TECHNOLOGY		= dfs
SAMSFILES		= gwy.sams
SAMS_TARGETS		= dfsgwymsg.h dfsgwymsg.c dfsgwymac.h dfsgwysvc.c \
			  dfsgwysvc.h dfsgwy.cat

IDIR			= /bin/
dfsgwy.cat_IDIR		= /nls/msg/en_US.ASCII/
ILIST			= ${PROGRAMS} dfsgwy.cat

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

