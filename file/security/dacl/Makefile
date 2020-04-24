# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.931.1  1996/10/02  18:15:10  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:46:23  damon]
#
# Revision 1.1.926.5  1994/08/23  18:58:51  rsarbo
# 	delegation
# 	[1994/08/23  16:31:31  rsarbo]
# 
# Revision 1.1.926.4  1994/07/13  22:26:22  devsrc
# 	merged with bl-10
# 	[1994/06/28  21:11:10  devsrc]
# 
# 	delegation ACL support
# 	[1994/06/07  14:04:50  delgado]
# 
# Revision 1.1.926.3  1994/06/09  14:18:27  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:30:29  annie]
# 
# Revision 1.1.926.2  1994/02/04  20:28:13  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:17:15  devsrc]
# 
# Revision 1.1.925.1  1993/12/07  17:32:28  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  16:42:53  jaffe]
# 
# Revision 1.1.6.5  1993/01/21  15:16:32  cjd
# 	embedded copyright notice
# 	[1993/01/20  15:52:31  cjd]
# 
# Revision 1.1.6.4  1992/11/24  18:27:10  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  19:19:41  bolinger]
# 
# Revision 1.1.6.3  1992/09/15  13:18:10  jaffe
# 	Transarc delta: jaffe-ot5166-dont-install-testacl.o 1.1
# 	  Selected comments:
# 	    don't install testacl.o.
# 	    put dfsdcl.cat explicitly into the ILIST instead of using OTHERS.
# 	[1992/09/14  20:31:59  jaffe]
# 
# Revision 1.1.6.2  1992/08/31  21:01:01  jaffe
# 	no change
# 	[1992/08/30  04:17:58  jaffe]
# 
# Revision 1.1.3.14  1992/05/27  16:44:22  mason
# 	OT 3833 - fix mismatches between Transarc and OSF source.
# 	[1992/05/27  16:08:37  mason]
# 
# Revision 1.1.3.13  1992/05/20  20:03:48  mason
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
# 	[1992/05/20  11:56:59  mason]
# 
# Revision 1.1.3.12  1992/05/06  23:48:47  jdp
# 	Added "dacl_default.o" to libdacl.a_OFILES.
# 	[1992/05/06  23:48:05  jdp]
# 
# Revision 1.1.3.11  1992/04/21  14:27:29  mason
# 	Transarc delta: bab-ot2379-dacl-build-testacl 1.2
# 	  Files modified:
# 	    security/dacl: Makefile, testacl.c
# 	    security/dacl_lfs: Makefile
# 	  Selected comments:
# 	    Get the user-space testing programs built for the dacl package.
# 	    Get testacl.o built in this directory (w/o the Episode flags), for testacl.
# 	    Error table initialization needed to be removed from testacl.c
# 	[1992/04/20  22:57:23  mason]
# 
# Revision 1.1.3.10  1992/04/17  19:03:17  mason
# 	*** empty log message ***
# 
# Revision 1.1.3.9  1992/04/16  17:19:38  garyf
# 	fix .p.h rule
# 	[1992/04/16  17:16:16  garyf]
# 
# Revision 1.1.3.8  1992/04/15  19:34:54  garyf
# 	auth5b definitions no longer needed
# 	let AFS_DEBUG be controlled by normal mechanisms
# 	[1992/04/15  19:34:23  garyf]
# 
# Revision 1.1.3.7  1992/04/15  16:10:22  mason
# 	Added testacl.o to the OTHERS line so it gets built.
# 	[1992/04/15  16:09:08  mason]
# 
# Revision 1.1.3.6  1992/03/27  21:53:47  toml
# 	Add -Dauth5b -D_AUTH5b_ to $CFLAGS
# 	[1992/03/27  21:53:27  toml]
# 
# Revision 1.1.3.5  1992/01/31  14:52:35  delgado
# 	Add modifications to build libdcedfs.a
# 	[1992/01/31  14:52:15  delgado]
# 
# Revision 1.1.3.4  1992/01/27  22:37:53  treff
# 	1/26/92 DCT @ OSF
# 	RE-add AUTH compile_et changes
# 	[1992/01/27  22:37:26  treff]
# 
# Revision 1.1.3.3  1992/01/24  03:48:03  devsrc
# 	Merging dfs6.3
# 	[1992/01/24  00:18:36  devsrc]
# 
# Revision 1.1.3.2  1992/01/23  20:22:28  zeliff
# 	Moving files onto the branch for dfs6.3 merge
# 	[1992/01/23  18:38:35  zeliff]
# 	Revision 1.1.1.3  1992/01/23  22:21:04  devsrc
# 	Fixed logs
# 
# $EndLog$


libdacl.a_OFILES	= dacl_debug.o dacl_access.o dacl_pac.o \
			  dacl_flatten.o dacl_manager.o \
		 	  dacl_mgruuids.o dacl_parse.o dacl_permbits.o \
			  dacl_strings.o dacl_modify.o dacl_print.o \
			  dacl_read.o dacl_sec_rgy.o dacl_sec_acl.o \
			  dacl_getmode.o dacl_validate.o exception_print.o \
			  dacl_init.o dacl_default.o 


.if !defined(LIBDCEDFS_LIBRARY_MAKEFILE)
#
#  Information to build components when we are not build libdecdfs.a
#

VPATH			= ..

INCLUDES 	 	= dacl.h epi_id.h dacl_mgruuids.h dacl_trace.h

LIBRARIES		= libdacl.a


INCFLAGS		= -I..

EXPDIR			= /usr/lib/
dacl.h_EXPDIR		= /usr/include/dcedfs/
epi_id.h_EXPDIR		= /usr/include/dcedfs/
dacl_mgruuids.h_EXPDIR	= /usr/include/dcedfs/
dacl_trace.h_EXPDIR	= /usr/include/dcedfs/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfsdcl.cat dfszdl.cat testacl.o

ILIST			= dfsdcl.cat dfszdl.cat dacl.h dacl_mgruuids.h epi_id.h
IDIR			= /share/include/dcedfs/
dfsdcl.cat_IDIR		= /nls/msg/en_US.ASCII/
dfszdl.cat_IDIR		= /nls/msg/en_US.ASCII/

CFLAGS 			= -DEPI_USE_FULL_ID ${USERCFLAGS}

GARBAGE 		= dacl.h 

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

dacl.h dacl_errs.msf: dacl.p.h dacl_errs.et
		-${CP} ${dacl.p.h:P} .
		${COMPILE_ET} ${dacl_errs.et:P} -h dacl
		${RM} -f dacl.p.h

dacl_trace.h dacl_trace.msf: dacl_trace.et
	${COMPILE_ET} ${dacl_trace.et:P} -h dacl_trace

dfsdcl.cat: dacl_errs.msf
	${GENCAT} $@ $>

dfszdl.cat: dacl_trace.msf
	${GENCAT} $@ $>

.endif
