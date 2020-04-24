# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.996.3  1994/06/09  14:19:11  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:31:10  annie]
#
# Revision 1.1.996.2  1994/02/04  20:29:19  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:17:43  devsrc]
# 
# Revision 1.1.995.1  1993/12/07  17:33:08  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  16:52:07  jaffe]
# 
# Revision 1.1.6.3  1993/01/21  15:18:27  cjd
# 	embedded copyright notice
# 	[1993/01/20  15:55:56  cjd]
# 
# Revision 1.1.6.2  1992/11/24  18:30:56  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  19:22:45  bolinger]
# 
# Revision 1.1.3.10  1992/06/08  22:46:18  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Replaced -lbsd with ${BSD_LIBS} variable
# 	[1992/06/08  21:51:29  mason]
# 
# Revision 1.1.3.9  1992/05/20  20:04:03  mason
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
# 	[1992/05/20  11:57:51  mason]
# 
# Revision 1.1.3.8  1992/05/05  04:34:09  mason
# 	delta mason-fix-security-dfsauth-Makefile 1.1
# 	[1992/04/27  12:15:56  mason]
# 
# Revision 1.1.3.7  1992/04/14  18:06:24  mason
# 	auth5b/AUTH5B defines no longer needed.
# 	[1992/04/14  18:02:51  mason]
# 
# Revision 1.1.3.6  1992/04/14  03:35:26  mason
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
# 	[1992/04/13  17:23:11  mason]
# 
# Revision 1.1.3.5  1992/01/31  16:07:29  delgado
# 	Add modifications to build libdcedfs.a
# 	[1992/01/31  16:06:48  delgado]
# 
# Revision 1.1.3.4  1992/01/29  19:08:56  cfe
# 	Get the security flags right--in particular, define auth5b stuff
# 	and don't define FORCE_NOAUTH.
# 	[1992/01/29  19:08:32  cfe]
# 
# Revision 1.1.3.3  1992/01/24  03:48:14  devsrc
# 	Merging dfs6.3
# 	[1992/01/24  00:18:54  devsrc]
# 
# Revision 1.1.3.2  1992/01/23  20:23:01  zeliff
# 	Moving files onto the branch for dfs6.3 merge
# 	[1992/01/23  18:39:02  zeliff]
# 	Revision 1.1.1.3  1992/01/23  22:21:22  devsrc
# 	Fixed logs
# 
# $EndLog$


libdauth.a_OFILES	= dfsauth_client.o dfsauth_server.o \
			  dfsauth_localprincipal.o  dfsauth_localkey.o \
	                  dfsauth_keytab.o dfsauth_debug.o \
			  dfsauth_pathnames.o \
			  dfssec_errorStrings.o dfsauth_rpc_mgmt_authz.o \
			  dfsauth_sec_key_mgmt.o


.if !defined(LIBDCEDFS_LIBRARY_MAKEFILE)
#
#
#

VPATH			= ..

INCLUDES 	 	= dfsauth.h dfsauth_trace.h

LIBRARIES		= libdauth.a

EXPDIR			= /usr/lib/
dfsauth.h_EXPDIR	= /usr/include/dcedfs/
dfsauth_trace.h_EXPDIR	= /usr/include/dcedfs/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS                  = dfsdau.cat dfszdt.cat
ILIST                   = ${OTHERS} dfsauth.h
IDIR                    = /share/include/dcedfs/
dfsdau.cat_IDIR         = /nls/msg/en_US.ASCII/
dfszdt.cat_IDIR         = /nls/msg/en_US.ASCII/

CFLAGS 			= ${USERCFLAGS} -D_BSD_INCLUDES -D_aiws -D__unix -D_BSD

.if ${TARGET_MACHINE} == "RIOS"
CFLAGS += -DOSF_DCE
.endif

INCFLAGS		= -I..



GARBAGE 		= dfsauth.h dfsauth.c

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

dfsauth.h daut_errs.msf: dfsauth.p.h daut_errs.et
		-${CP} ${dfsauth.p.h:P} .
		${COMPILE_ET} ${daut_errs.et:P} -h dfsauth
	        ${RM} -f dfsauth.p.h

dfsauth_trace.h dfsauth_trace.msf: dfsauth_trace.et
	${COMPILE_ET} ${dfsauth_trace.et:P} -h dfsauth_trace

dfsdau.cat: daut_errs.msf
	${GENCAT} $@ $>

dfszdt.cat: dfsauth_trace.msf
	${GENCAT} $@ $>

.endif
