# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.20.1  1996/10/02  17:14:48  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:06:36  damon]
#
# Revision 1.1.14.3  1994/08/01  17:56:18  sasala
# 	Implement partial S12Y (Internationalization) for DFS commands and daemons
# 	[1994/08/01  15:07:48  sasala]
# 
# 	Implement partial S12Y (internationalization) for DFS commands and daemons
# 	[1994/07/21  14:51:18  sasala]
# 
# Revision 1.1.14.2  1994/06/09  13:55:41  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:30:11  annie]
# 
# Revision 1.1.14.1  1994/02/04  20:09:39  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:08:20  devsrc]
# 
# Revision 1.1.12.1  1993/12/07  17:15:38  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  13:31:18  jaffe]
# 
# Revision 1.1.10.5  1993/01/18  20:53:49  cjd
# 	embedded copyright notice
# 	[1993/01/18  17:40:25  cjd]
# 
# Revision 1.1.10.4  1993/01/13  16:24:19  shl
# 	Transarc delta: cfe-ot6492-fix-catalog-install-for-dfsbind 1.1
# 	  Selected comments:
# 	    The install directory for dfszbd.cat was named as /nsl/msg/... rather than
# 	    /nls/msg/...
# 	Transarc delta: mcinerny-ot6390-do-not-install-bind_trace.h-into-bin 1.2
# 	  Selected comments:
# 	    Fix the dfsbind Makefile to not install bind_trace.h.
# 	    Clsing temporarily to import comer-ot6129-user-level-icl-dumping.
# 	    Remove the INCLUDES line and instead make main_helper.o dfsbind_test.o
# 	    bind_helper.o depend upon bind_trace.h
# 	[1993/01/12  19:52:08  shl]
# 
# Revision 1.1.10.3  1992/12/09  19:35:02  jaffe
# 	Transarc delta: comer-ot6129-user-level-icl-dumping 1.5
# 	  Selected comments:
# 	    Added code to ICL trace logs at user level.  The guinea pig for this
# 	    change was dfsbind, also modified as part of this delta.  Most of the
# 	    actual tracing and some of the ICL work was done at IBM by Jean
# 	    Pehkonen.
# 	    Force the program to dump its log: kill -30 <pid>
# 	    Added -licl to make lines.
# 	    Added dfsbind to EXPINC_SUBDIRS
# 	    In files where both osi.h and compat.h are included, the reference
# 	    to osi.h must be first.  In each of these files, osi.h was implicitly
# 	    included, so I added an explicit reference before compat.h.
# 	    Installed message catalog.
# 	[1992/12/03  21:11:31  jaffe]
# 
# Revision 1.1.10.2  1992/08/31  19:31:25  jaffe
# 	Transarc delta: jess-ot2869-com_err-usage-cleanup 1.5
# 	  Selected comments:
# 	    The use of com_err in dfs is inconsistent and in some cases
# 	    incorrect.
# 	    1) there are many sources which include <com_err.h> but don't
# 	    use any definitions from it.
# 	    2) there are sources which use com_err, but don't include the
# 	    <com_err.h>
# 	    3) there are subdirectories which do not need libcom_err.a for
# 	    any reason that link with it.
# 	    This delta is created to clean up the usage of com_err in dfs.
# 	    Remove -lcom_err in the var LIBS
# 	    Add -lcom_err to LIBS.
# 	[1992/08/30  02:13:46  jaffe]
# 
# Revision 1.1.3.10  1992/06/08  22:45:04  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Replaced -lbsd with ${BSD_LIBS} variable
# 	[1992/06/08  21:31:51  mason]
# 
# Revision 1.1.3.9  1992/05/05  19:00:31  rec
# 	Removed dependence on libkrb5.a by allowing libdce.a
# 	to export required symbol (krb5_cksumarray).
# 	[1992/05/05  18:54:20  rec]
# 
# Revision 1.1.3.8  1992/04/14  04:07:54  mason
# 	Transarc delta: mason-ot2542-dfsbind-link 1.1
# 	  Files modified:
# 	    dfsbind: Makefile
# 	  Selected comments:
# 	    Switch the link line so libafssys gets linked before libdce because OSF is in
# 	    the process of moving setpag from libafssys to libdce, but until they finish
# 	    we cannot use their version of it.  After they finish, this new ordering will
# 	    still be OK because they will remove setpag from libdce.
# 	    Moved -lafssys in front of -ldce
# 	[1992/04/13  15:59:44  mason]
# 
# Revision 1.1.3.7  1992/04/07  20:26:59  rec
# 	Have -lkrb5 on link line depend on RIOS platform since
# 	it is not needed with a shared libdce on OSF/1.
# 	[1992/04/07  19:55:18  rec]
# 
# Revision 1.1.3.6  1992/01/28  20:34:57  cfe
# 	No changes--already merged
# 	[1992/01/28  20:33:55  cfe]
# 
# Revision 1.1.3.5  1992/01/26  00:26:17  zeliff
# 	Merging OSF porting changes:  changed order of LIBS.
# 	[1992/01/25  22:32:56  zeliff]
# 
# Revision 1.1.3.4  1992/01/25  20:45:32  zeliff
# 	dfs6.3 merge, part2
# 	[1992/01/25  20:02:16  zeliff]
# 
# $EndLog$

PROGRAMS 		= dfsbind dfsbind_test
INCLUDES		= bind_trace.h dfsdsbmsg.h dfsdsbsvc.h
OTHERS			= dfszbd.cat

dfsbind_OFILES		= main_helper.o bind_helper.o \
			  dfsdsbsvc.o dfsdsbmsg.o dfsbind_svc.o
dfsbind_test_OFILES	= dfsbind_test.o bind_helper.o \
			  dfsdsbsvc.o dfsdsbmsg.o dfsbind_svc.o

EXPDIR			= /usr/include/dcedfs/

CFLAGS			= ${USERCFLAGS} -DDNS_V3API -DDNS_CDS 

LIBS 			= -lfldb -lnubik -ltpq -lcmd  \
			  -lncompat -ldacl -ldauth -ldacllfs -ldacl  \
			  -ldauth -ldacllfs -licl -losi -lauthelper \
			  -ldfstab -lftutil -lvolc -lpipe -ldfsncs \
			  -lrep -lftserver -lafs4int -lcommondata -losi \
			  -lbomb -lafsutil -lafssys -lcom_err ${LIC_LIBS} \
			  -ldce   ${OS_LIBS} 

TECHNOLOGY=dfs

SAMSFILES = dsb.sams
SAMS_TARGETS = dfsdsbmsg.h dfsdsbmsg.c dfsdsbmac.h dfsdsbsvc.c dfsdsbsvc.h \
    dfsdsb.cat

dfsdsb.cat_IDIR          = /nls/msg/en_US.ASCII/
dfszbd.cat_IDIR          = /nls/msg/en_US.ASCII/

ILIST			= ${OTHERS} dfsbind dfsdsb.cat
IDIR			= /bin/

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.if exists(${TARGET_OS}/machdep.mk)
.include "${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>

bind_trace.h bind_trace.msf: bind_trace.et
	${COMPILE_ET} ${bind_trace.et:P} -h bind_trace

dfszbd.cat: bind_trace.msf
	${GENCAT} $@ $>

main_helper.o dfsbind_test.o bind_helper.o: bind_trace.h dfsdsbmsg.h  dfsdsbsvc.h 
