# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.24.1  1994/10/26  15:45:47  sasala
# 	UHYP: CR 12618 Implement S12Y in DFS flserver
# 	[1994/10/26  15:43:33  sasala]
#
# Revision 1.1.22.4  1994/09/07  15:48:24  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:20  marty]
# 
# Revision 1.1.22.3  1994/07/13  22:19:18  devsrc
# 	merged with bl-10
# 	[1994/06/28  18:03:05  devsrc]
# 
# 	Added  afsvl_s2c.h afsvl_data.h to ILIST
# 	[1994/03/08  21:20:05  mckeen]
# 
# Revision 1.1.22.2  1994/06/09  14:06:46  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:22:02  annie]
# 
# Revision 1.1.22.1  1994/02/04  20:17:44  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:12:34  devsrc]
# 
# Revision 1.1.20.1  1993/12/07  17:23:24  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:14:44  jaffe]
# 
# Revision 1.1.9.6  1993/01/21  19:33:24  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  19:46:57  zeliff]
# 
# Revision 1.1.9.5  1992/11/24  17:01:19  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  17:56:09  bolinger]
# 
# Revision 1.1.9.4  1992/09/15  13:12:05  jaffe
# 	Transarc delta: jaffe-sync-with-aug-31-osf-sources 1.4
# 	  Selected comments:
# 	    Pick up changes from last submission to the OSF.
# 	    remove commondefs.idl.  This was done incompletely in another delta
# 	[1992/09/14  19:34:33  jaffe]
# 
# Revision 1.1.9.3  1992/09/01  15:36:19  jaffe
# 	Remove reference to commondefs
# 	[1992/09/01  13:56:47  jaffe]
# 
# Revision 1.1.9.2  1992/08/31  19:32:30  jaffe
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
# 	    Remove -lcom_err in the var flserver_LIBS
# 	    Add -lcom_err to LIBS.
# 	Transarc delta: vijay-ot2172-cmdproc-into-flserver 1.2
# 	  Selected comments:
# 	    1. delete some obsolete options
# 	    2. Use the standard cmd command parser
# 	    no change here, -lcmd already in
# 	    accidentally erase admin list file name
# 	[1992/08/30  02:17:53  jaffe]
# 
# Revision 1.1.3.17  1992/06/30  21:43:20  mason
# 	Add the STUB_OPT_LEVEL variable to control optimization of stub files.
# 	[1992/06/30  21:28:49  mason]
# 
# Revision 1.1.3.16  1992/06/08  22:45:08  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Replaced -lbsd with ${BSD_LIBS} variable
# 	[1992/06/08  21:34:46  mason]
# 
# Revision 1.1.3.15  1992/05/20  19:50:52  mason
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
# 	[1992/05/20  11:12:55  mason]
# 
# Revision 1.1.3.14  1992/05/05  19:00:37  rec
# 	Removed dependence on libkrb5.a by allowing libdce.a
# 	to export required symbol (krb5_cksumarray).
# 	[1992/05/05  18:54:45  rec]
# 
# Revision 1.1.3.13  1992/04/16  19:59:24  mckeen
# 	Removed definition of IDL
# 	[1992/04/15  19:23:28  mckeen]
# 
# Revision 1.1.3.12  1992/04/16  17:18:52  garyf
# 	fix .p.h rules
# 	[1992/04/16  17:12:57  garyf]
# 
# Revision 1.1.3.11  1992/04/14  04:08:02  mason
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
# 	[1992/04/13  16:00:17  mason]
# 
# Revision 1.1.3.10  1992/04/07  20:27:11  rec
# 	Have -lkrb5 on link line depend on RIOS platform since
# 	it is not needed with a shared libdce on OSF/1.
# 	[1992/04/07  19:58:38  rec]
# 
# Revision 1.1.3.9  1992/01/31  16:44:28  delgado
# 	Add modifications to build libdcedfs.a
# 	[1992/01/31  16:44:09  delgado]
# 
# Revision 1.1.3.8  1992/01/28  20:43:20  cfe
# 	No changes necessary
# 	[1992/01/28  20:42:44  cfe]
# 
# Revision 1.1.3.7  1992/01/26  00:26:20  zeliff
# 	Merging OSF porting changes:  added -lafssys to end of
# 	LIBS lines.
# 	[1992/01/25  22:46:13  zeliff]
# 
# Revision 1.1.3.6  1992/01/25  20:47:30  zeliff
# 	dfs6.3 merge, part2
# 	[1992/01/25  20:05:14  zeliff]
# 
# $EndLog$


libfldb.a_OFILES	= fldb_proc_cstub.o  \
			  dfslsvsvc.o dfslsvmsg.o lsv_svc.o

.if !defined(LIBDCEDFS_LIBRARY_MAKEFILE)
#
# Information to build components when we are not building libdcedfs.a
#

INCLUDES		= afsvl_data.idl \
			  fldb_data.idl fldb_proc.h fldb_data.h\
		          flclient.h flserver.h afsvl_data.h \
			  afsvl_proc.h \
			  dfslsvmsg.h dfslsvmac.h dfslsvsvc.h

LIBRARIES		= libfldb.a
PROGRAMS		= flserver flclient

EXPDIR			= /usr/include/dcedfs/
dfslsvmsg.h_EXPDIR	= /usr/include/dcedfs/
dfslsvsvc.h_EXPDIR	= /usr/include/dcedfs/
libfldb.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfsvls.cat dfszfl.cat
TECHNOLOGY=dfs

SAMSFILES = lsv.sams
SAMS_TARGETS = dfslsvmsg.h dfslsvmsg.c dfslsvmac.h dfslsvsvc.c dfslsvsvc.h \
    dfslsv.cat

ILIST			= ${OTHERS} flclient.h fldb_data.h fldb_proc.h \
			  flserver.h flserver afsvl_data.h afsvl_proc.h \
			  dfslsv.cat
IDIR			= /share/include/dcedfs/

dfslsv.cat_IDIR          = /nls/msg/en_US.ASCII/
dfsvls.cat_IDIR		= /nls/msg/en_US.ASCII/
dfszfl.cat_IDIR		= /nls/msg/en_US.ASCII/
flserver_IDIR		= /bin/

CFLAGS			= -Dunix ${USERCFLAGS}
flserver_OFILES		= fldb_proc_sstub.o flserver.o flutils.o flprocs.o \
			  dfslsvsvc.o dfslsvmsg.o lsv_svc.o
flclient_OFILES		= flclient.o  dfslsvsvc.o dfslsvmsg.o lsv_svc.o

flserver_LIBS		= -lnubik -licl -lcmd -lafsutil -lncompat -ltpq \
			  -lfldb -lcommondata \
			  -ldacl -ldauth -ldacllfs -ldacl -ldauth \
			  -ldacllfs -losi -lcom_err -ldce ${OS_LIBS} -lafssys

flclient_LIBS		= -lfldb -lnubik -licl -lcmd -lafsutil -lncompat \
			  -lcommondata -ltpq \
			  -ldacl -ldauth -ldacllfs -ldacl -ldauth -ldacllfs \
                          -lafssys -losi -lcom_err -ldce ${OS_LIBS}

IDLFILES		= afsvl_proc.idl afsvl_data.idl \
			  fldb_data.idl fldb_proc.idl 

IDLFLAGS		= -keep c_source  -no_mepv -DOSF_NO_SOCKET_DUMP

IDLINCFLAGS		= -I.

# On some platforms the optimizer is very expensive relative to the
# actual gain.  We disable it on a per-platform basis.
OPT_LEVEL		+= ${STUB_OPT_LEVEL}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

flserver.h fl_errors.msf: flserver.p.h fl_errors.et fl_trace.h
	-${CP} ${flserver.p.h:P} .
	${COMPILE_ET} ${fl_errors.et:P} -h flserver
	${RM} -f flserver.p.h

fl_trace.h fl_trace.msf: fl_trace.et
	${COMPILE_ET} ${fl_trace.et:P} -h fl_trace

dfsvls.cat: fl_errors.msf
	${GENCAT} $@ $>

dfszfl.cat: fl_trace.msf
	${GENCAT} $@ $>

.endif
