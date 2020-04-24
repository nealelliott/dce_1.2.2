# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.984.1  1996/10/02  21:02:28  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:49:50  damon]
#
# Revision 1.1.979.4  1994/09/07  15:48:30  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:27  marty]
# 
# Revision 1.1.979.3  1994/08/01  21:06:02  sasala
# 	Implement partial S12Y (Internationalization) for DFS commands and daemons
# 	[1994/08/01  15:08:05  sasala]
# 
# 	Implement partial S12Y (internationalization) for DFS commands and daemons
# 	[1994/07/21  14:51:48  sasala]
# 
# Revision 1.1.979.2  1994/06/09  14:23:40  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:35:33  annie]
# 
# Revision 1.1.979.1  1994/02/04  20:34:09  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:19:41  devsrc]
# 
# Revision 1.1.977.1  1993/12/07  17:36:40  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  17:44:41  jaffe]
# 
# Revision 1.1.8.4  1993/01/21  15:57:13  cjd
# 	embedded copyright notice
# 	[1993/01/20  17:41:40  cjd]
# 
# Revision 1.1.8.3  1992/11/24  19:49:28  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  19:34:22  bolinger]
# 
# Revision 1.1.8.2  1992/08/31  21:40:54  jaffe
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
# 	[1992/08/30  13:25:08  jaffe]
# 
# Revision 1.1.3.14  1992/06/30  21:43:45  mason
# 	Add the STUB_OPT_LEVEL variable to control optimization of stub files.
# 	[1992/06/30  21:30:12  mason]
# 
# Revision 1.1.3.13  1992/06/08  22:46:42  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Replaced -lbsd with ${BSD_LIBS} variable
# 	[1992/06/08  22:09:23  mason]
# 
# Revision 1.1.3.12  1992/05/20  20:18:23  mason
# 	Transarc delta: cfe-ot3085-fix-msf-dependencies 1.1
# 	  Files modified:
# 	    bak: Makefile; bakserver: Makefile; bosserver: Makefile
# 	    bubasics: Makefile; flserver: Makefile
# 	    flserver.klib: Makefile; fshost: Makefile; fsint: Makefile
# 	    ftserver: Makefile; host: Makefile; ncscompat: Makefile
# 	    ncsubik: Makefile; security/dacl: Makefile
# 	    security/dfsauth: Makefile; sysacl: Makefile; tkm: Makefile
# 	    tools/cmd: Makefile; update: Makefile; xcred: Makefile
# 	    xvolume: Makefile
# 	  Selected comments:
# 	    Make explicit the dependency that builds an .msf file from an .et file.
# 	    Add dependency to build .msf file.
# 	[1992/05/20  16:44:59  mason]
# 
# Revision 1.1.3.11  1992/05/05  19:00:59  rec
# 	Removed dependence on libkrb5.a by allowing libdce.a
# 	to export required symbol (krb5_cksumarray).
# 	[1992/05/05  18:57:13  rec]
# 
# Revision 1.1.3.10  1992/04/16  19:59:42  mckeen
# 	Removed definition of IDL
# 	[1992/04/15  19:08:58  mckeen]
# 
# Revision 1.1.3.9  1992/04/16  17:19:12  garyf
# 	fix .p.h rules
# 	[1992/04/16  17:13:16  garyf]
# 
# Revision 1.1.3.8  1992/04/15  19:44:12  garyf
# 	let AFS_DEBUG be set by normal means
# 	[1992/04/15  19:43:17  garyf]
# 
# Revision 1.1.3.7  1992/04/14  04:11:11  mason
# 	Transarc delta: bab-ot2394-change-catalog-names 1.3
# 	  Files modified:
# 	    bak: Makefile; bakserver: Makefile; bosserver: Makefile
# 	    bubasics: Makefile; flserver: Makefile; fshost: Makefile
# 	    fsint: Makefile; ftserver: Makefile; host: Makefile
# 	    ncscompat: Makefile; ncsubik: Makefile
# 	    security/dacl: Makefile; security/dfsauth: Makefile
# 	    tkm: Makefile; tools/cmd: Makefile; update: Makefile
# 	    xcred: Makefile; xvolume: Makefile
# 	  Selected comments:
# 	    The dfs error catalogs were generated with names that made them
# 	    dce_error_inq_text unable to locate them. (dce_error_inq_text pulls
# 	    a facility code out of the error code and uses this to construct a
# 	    filename in which to look for the error message.  Our (dfs) facility
# 	    code is such that our error catalog filenames must start with "dfs",
# 	    rather then "dce".)
# 	    Change occurrences of "dce...\.cat" to "dfs...\.cat"
# 	    ot 2394
# 	    The sed script missed one occurrence of "dfs...\.cat"
# 	    the first time through because it was hidden by another
# 	    occurrence earlier on the line.
# 	[1992/04/13  18:05:18  mason]
# 
# Revision 1.1.3.6  1992/04/07  20:27:31  rec
# 	Have -lkrb5 on link line depend on RIOS platform since
# 	it is not needed with a shared libdce on OSF/1.
# 	[1992/04/07  20:05:35  rec]
# 
# Revision 1.1.3.5  1992/01/26  00:26:31  zeliff
# 	Merging OSF porting changes: moved afssys to the end of
# 	the LIBS line.
# 	[1992/01/26  00:20:26  zeliff]
# 
# Revision 1.1.3.4  1992/01/25  20:50:26  zeliff
# 	dfs6.3 merge, part2
# 	[1992/01/25  20:10:04  zeliff]
# 
# $EndLog$


PROGRAMS        	= upclient upserver 
INCLUDES 	 	= update.h upcommon.h dfsudtmsg.h dfsudtsvc.h

EXPDIR                  = /usr/include/dcedfs/

OTHERS			= dfsupd.cat

TECHNOLOGY=dfs

SAMSFILES = udt.sams
SAMS_TARGETS = dfsudtmsg.h dfsudtmsg.c dfsudtmac.h dfsudtsvc.c dfsudtsvc.h \
		   upsmsg.c upsmsg.h upcmsg.c upcmsg.h dfsudt.cat

IDIR 			= /bin/
dfsudt.cat_IDIR         = /nls/msg/en_US.ASCII/
dfsupd.cat_IDIR		= /nls/msg/en_US.ASCII/
ILIST 			= ${PROGRAMS} ${OTHERS}  dfsudt.cat
CFLAGS                  = -Dunix -D_BSD ${USERCFLAGS}

IDLFILES		= update.idl

IDLFLAGS                = -keep c_source -no_cpp -no_mepv
IDLINCFLAGS		= -I.

LIBS            	= -lcmd -lcmdAux -lafsutil -licl \
			  -lncompat -ldacl -ldauth -ldacllfs -ldacl \
			  -ldauth -ldacllfs -licl -lcommondata \
			  -lafssys -losi -lcom_err -ldce ${OS_LIBS}

upclient_OFILES 	= util.o client.o update_cstub.o  dfsudtsvc.o \
			  dfsudtmsg.o udt_svc.o
upserver_OFILES 	= util.o server.o update_sstub.o manager.o  dfsudtsvc.o \
			  dfsudtmsg.o udt_svc.o

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

upcommon.h uperrors.msf:	uperrors.et upcommon.p.h
	-${CP} ${upcommon.p.h:P} .
	${COMPILE_ET} ${uperrors.et:P} -h upcommon
	${RM} -f upcommon.p.h

dfsupd.cat: uperrors.msf
	 ${GENCAT} $@ $>

upsmsg.c: udt.sams
	${SAMS} -ot -t ups -n ups ${.ALLSRC}
upsmsg.h: udt.sams
	${SAMS} -oh -t ups -n ups ${.ALLSRC}
upcmsg.c: udt.sams
	${SAMS} -ot -t upc -n upc ${.ALLSRC}
upcmsg.h: udt.sams
	${SAMS} -oh -t upc -n upc ${.ALLSRC}
