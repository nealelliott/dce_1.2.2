# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.110.1  1996/10/17  18:28:14  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:54:10  damon]
#
# Revision 1.1.105.3  1994/02/07  16:28:20  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:33  mckeen]
# 
# Revision 1.1.105.2  1994/02/04  20:46:07  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:24:53  devsrc]
# 
# Revision 1.1.103.1  1993/12/07  17:45:11  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  19:22:16  jaffe]
# 
# Revision 1.1.2.4  1993/01/11  18:31:59  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:12:19  htf]
# 
# Revision 1.1.2.3  1992/11/20  15:06:13  jaffe
# 	Add dependencies for spoke.h
# 	[1992/11/20  14:58:38  jaffe]
# 
# Revision 1.1.2.2  1992/11/18  22:01:19  jaffe
# 	New File
# 	[1992/11/18  15:47:20  jaffe]
# 
# $EndLog$

#
#  Makefile --
#	For the test/file/itl/spoke directory.
#

CCTYPE = ansi

CFLAGS = ${USERCFLAGS} -U_BSD -DITL_FS_TEST

SUBDIRS = dfs
EXPINC_SUBDIRS = ${SUBDIRS}
EXPLIB_SUBDIRS = ${SUBDIRS}

EXPINC_SUBDIRS = ${SUBDIRS}
EXPLIB_SUBDIRS = ${SUBDIRS}

LIBRARIES		= libitlHubSpoke.a
EXPDIR			= /usr/include/utils/itl/
EXPLIB_TARGETS		= ${LIBRARIES:S/^/export_/g}

libitlHubSpoke.a_OFILES	= ns.o \
		spoke_mgr.o spoke_client.o \
		spoke_cstub.o spoke_sstub.o \
		itl_spoke.o auto_spoke.o

INCLUDES                = defs.h spoke.h itl_spoke.h auto_spoke.h

libitlHubSpoke.a_EXPDIR = /usr/lib/

IDLFILES = spoke.idl
ACFFILES = spoke.acf
IDLFLAGS = -keep c_source -no_mepv
IDLDATAFILES = spoke.h
STUBS = spoke_cstub.o spoke_sstub.o

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>

spoke_mgr.o: spoke.h
spoke_client.o: spoke.h
ns.o: spoke.h
itl_spoke.o: spoke.h
