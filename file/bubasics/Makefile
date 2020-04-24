# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.90.1  1996/10/02  17:05:19  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:05:14  damon]
#
# Revision 1.1.84.3  1994/09/07  15:48:21  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:17  marty]
# 
# Revision 1.1.84.2  1994/06/09  13:52:42  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:26:34  annie]
# 
# Revision 1.1.84.1  1994/02/04  20:06:43  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:06:51  devsrc]
# 
# Revision 1.1.82.1  1993/12/07  17:13:20  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  12:59:35  jaffe]
# 
# Revision 1.1.7.3  1993/01/18  19:57:31  cjd
# 	Embedded copyright notice
# 	[1993/01/18  19:34:01  cjd]
# 
# Revision 1.1.7.2  1992/11/24  15:41:59  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  16:10:05  bolinger]
# 
# Revision 1.1.3.9  1992/06/30  21:43:07  mason
# 	Add the STUB_OPT_LEVEL variable to control optimization of stub files.
# 	[1992/06/30  21:28:08  mason]
# 
# Revision 1.1.3.8  1992/05/27  16:44:17  mason
# 	OT 3833 - fix mismatches between Transarc and OSF source.
# 	[1992/05/27  16:07:34  mason]
# 
# Revision 1.1.3.7  1992/05/20  19:44:23  mason
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
# 	[1992/05/19  22:21:00  mason]
# 
# Revision 1.1.3.6  1992/04/16  19:59:15  mckeen
# 	Removed definition of IDL
# 	[1992/04/15  19:08:07  mckeen]
# 
# Revision 1.1.3.5  1992/04/16  17:18:48  garyf
# 	fix .p.h rules
# 	[1992/04/16  17:12:47  garyf]
# 
# Revision 1.1.3.4  1992/01/25  20:44:23  zeliff
# 	dfs6.3 merge, part2
# 	[1992/01/25  20:00:31  zeliff]
# 
# $EndLog$

INCLUDES		= bubasics.h butm.h butc.h tcdata.h

LIBRARIES		= libbubasics.a

EXPDIR			= /usr/include/dcedfs/
libbubasics.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfsbtm.cat dfsbtc.cat

ILIST			= ${OTHERS} butm.h butc.h tcdata.h bubasics.h
IDIR			= /share/include/dcedfs/
dfsbtm.cat_IDIR		= /nls/msg/en_US.ASCII/
dfsbtc.cat_IDIR		= /nls/msg/en_US.ASCII/

CFLAGS			= -D_VOPS -D_BSD_INCLUDES -Daiws -D__unix ${USERCFLAGS}
OFILES			= butc_sstub.o butc_cstub.o 

IDLFILES		= butc.idl

IDLFLAGS		= -keep c_source -no_mepv
IDLINCFLAGS		= -I.

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
CC_OPT_LEVEL=


butm.h: butm_errs.et butm.p.h
	-${CP} ${butm.p.h:P} .
	${COMPILE_ET} ${butm_errs.et:P} -h butm
	${RM} -f butm.p.h

dfsbtm.cat: butm_errs.msf
	 ${GENCAT} $@ $>

tcdata.h: butc_errs.et tcdata.p.h butm.h
	-${CP} ${tcdata.p.h:P} .
	${COMPILE_ET} ${butc_errs.et:P} -h tcdata
	${RM} -f tcdata.p.h

dfsbtc.cat: butc_errs.msf
	 ${GENCAT} $@ $>
