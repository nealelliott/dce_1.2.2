# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.104.1  1996/10/02  17:46:56  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:37:25  damon]
#
# Revision 1.1.99.3  1994/09/07  15:48:26  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:22  marty]
# 
# Revision 1.1.99.2  1994/06/09  14:07:48  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:22:46  annie]
# 
# Revision 1.1.99.1  1994/02/04  20:18:41  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:13:02  devsrc]
# 
# Revision 1.1.97.1  1993/12/07  17:24:13  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:22:16  jaffe]
# 
# Revision 1.1.6.4  1993/01/21  19:35:42  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  19:49:21  zeliff]
# 
# Revision 1.1.6.3  1992/11/24  17:03:29  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  17:59:02  bolinger]
# 
# Revision 1.1.6.2  1992/08/31  19:45:18  jaffe
# 	Transarc delta: jaffe-3645-remove-hardcoded-idl-path 1.1
# 	  Selected comments:
# 	    Makefile was redefining IDL and thereby requiring the idl program to be
# 	    on the path.
# 	    Removed IDL definition.
# 	[1992/08/30  02:25:50  jaffe]
# 
# Revision 1.1.3.10  1992/06/30  21:43:31  mason
# 	Add the STUB_OPT_LEVEL variable to control optimization of stub files.
# 	[1992/06/30  21:29:32  mason]
# 
# Revision 1.1.3.9  1992/05/20  19:52:25  mason
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
# 	[1992/05/20  11:30:47  mason]
# 
# Revision 1.1.3.8  1992/04/21  13:12:58  mason
# 	Transarc delta: cburnett-ot2690-fsint_changes_for_aix32 1.1
# 	  Files modified:
# 	    fsint: Makefile
# 	  Selected comments:
# 	    Changes for AIX 3.2 compile
# 	    Turned on xlc compiler
# 	[1992/04/20  22:32:56  mason]
# 
# Revision 1.1.3.7  1992/04/16  19:59:31  mckeen
# 	Removed CCTYPE=traditional statement
# 	[1992/04/16  18:38:08  mckeen]
# 
# 	Removed definition of IDL
# 	[1992/04/15  18:41:19  mckeen]
# 
# Revision 1.1.3.6  1992/04/14  04:08:42  mason
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
# 	[1992/04/13  16:04:04  mason]
# 
# Revision 1.1.3.5  1992/01/31  20:02:26  delgado
# 	Add modifications to build libdcedfs.a
# 	[1992/01/31  20:01:36  delgado]
# 
# Revision 1.1.3.4  1992/01/25  20:47:39  zeliff
# 	dfs6.3 merge, part2
# 	[1992/01/25  20:05:35  zeliff]
# 
# $EndLog$


libafs4int.a_OFILES	= afs4int_cstub.o tkn4int_sstub.o afs4int_sstub.o \
			  tkn4int_cstub.o

librep.a_OFILES		= rep_proc_cstub.o rep_proc_sstub.o

.if !defined(LIBDCEDFS_LIBRARY_MAKEFILE)
#
# Information to build components if lidcedfs.a is not being built
#
INCLUDES		= rep_data.idl rep_errs.h rep_data.h \
			  afs4int.h tkn4int.h rep_proc.h scx_errs.h

LIBRARIES		= libafs4int.a librep.a libafsrpcd.a 

EXPDIR			= /usr/include/dcedfs/
libafs4int.a_EXPDIR	= /usr/lib/
librep.a_EXPDIR	        = /usr/lib/
libafsrpcd.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfsrep.cat dfsscx.cat
ILIST			= ${OTHERS} afs4int.h tkn4int.h rep_data.h rep_proc.h \
			  rep_errs.h scx_errs.h

dfsrep.cat_IDIR		= /nls/msg/en_US.ASCII/
dfsscx.cat_IDIR		= /nls/msg/en_US.ASCII/
IDIR			= /share/include/dcedfs/

CFLAGS			= -Dunix ${USERCFLAGS} -Daiws -D__unix

libafsrpcd.a_OFILES	= afsrpcd_sstub.o

IDLFILES		= rep_proc.idl rep_data.idl afs4int.idl tkn4int.idl \
			  afs4_if.idl afs4_data.idl

IDLFLAGS		= -keep c_source -no_cpp -no_mepv 
IDLINCFLAGS 		= -I.

# On some platforms the optimizer is very expensive relative to the
# actual gain.  We disable it on a per-platform basis.
OPT_LEVEL		+= ${STUB_OPT_LEVEL}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

rep_errs.h rep_errs.msf: rep_errs.et
	${COMPILE_ET} ${rep_errs.et:P} -h rep_errs

dfsrep.cat: rep_errs.msf
	 ${GENCAT} $@ $>

scx_errs.h scx_errs.msf: scx_errs.et
	${COMPILE_ET} ${scx_errs.et:P} -h scx_errs

dfsscx.cat: scx_errs.msf
	 ${GENCAT} $@ $>

#
# The following rules generate condensed versions of the server stubs
# which are used by the fxd and dfsd deamons for registering the 
# AFS and TKN interface endpoints with the rpcd on behalf of the kernel
#

afsrpcd_sstub.c: afs4int_sstub.c
	-$(ECHO) "#include <dce/idlbase.h>" > $@
	-$(ECHO) "#include <dce/rpc.h>" >> $@
	-$(ECHO) "#include <dce/stubbase.h>" >> $@
	-$(SED) -n -e "/rpc_v2_server_stub_proc_t IDL_epva/,/\};/p" \
		-e "/rpc_v2_server_stub_proc_t NIDL_epva/,/\};/p" \
		afs4int_sstub.c > $@.x
	-$(SED) "s/op.*_ssr/NULL/" $@.x >> $@
	-$(SED) -n -e "/rpc_syntax_id_t IDL/,/global/p" \
		-e "/rpc_syntax_id_t NIDL/,/global/p" \
		afs4int_sstub.c >> $@
	-$(RM) $@.x

.endif
