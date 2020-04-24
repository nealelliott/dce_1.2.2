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
# Revision 1.1.834.1  1996/10/02  21:06:22  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:50:22  damon]
#
# Revision 1.1.829.4  1994/08/08  17:05:13  sasala
# 	Implement partial S12Y (Internationalization) for cm command, including general Makefile changes
# 	[1994/08/08  15:30:01  sasala]
# 
# Revision 1.1.829.3  1994/06/09  14:24:04  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:35:59  annie]
# 
# Revision 1.1.829.2  1994/02/04  20:34:36  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:19:52  devsrc]
# 
# Revision 1.1.828.1  1993/12/07  17:37:03  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  17:48:34  jaffe]
# 
# Revision 1.1.5.3  1993/01/21  16:28:57  cjd
# 	embedded copyright notice
# 	[1993/01/20  18:22:12  cjd]
# 
# Revision 1.1.5.2  1992/08/31  21:53:23  jaffe
# 	monir changes
# 	[1992/08/30  13:35:54  jaffe]
# 
# Revision 1.1.2.4  1992/06/08  22:46:50  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Replaced -lbsd with ${BSD_LIBS} variable
# 	[1992/06/08  22:15:52  mason]
# 
# Revision 1.1.2.3  1992/05/05  19:01:05  rec
# 	Removed dependence on libkrb5.a by allowing libdce.a
# 	to export required symbol (krb5_cksumarray).
# 	[1992/05/05  18:57:42  rec]
# 
# Revision 1.1.2.2  1992/04/07  20:27:39  rec
# 	Have -lkrb5 on link line depend on RIOS platform since
# 	it is not needed with a shared libdce on OSF/1.
# 	[1992/04/07  20:06:08  rec]
# 
# Revision 1.1  1992/01/19  02:50:17  devrcs
# 	Initial revision
# 
# $EndLog$
#


PROGRAMS 		= cm

INCLUDES		=  dfscmumsg.h dfscmusvc.h
EXPDIR			=  /usr/include/dcedfs/

CFLAGS			= -Dunix ${USERCFLAGS}

cm_OFILES		= cm.o cm_userint.o  dfscmusvc.o dfscmumsg.o cmu_svc.o

LIBS			= -lcom_err -lcmd -lcmdAux -lafsutil -lafssys -licl \
			  -lncompat -ldacl -ldauth -ldacllfs -ldacl \
			  -ldauth -ldacllfs -losi -ldce \
			  ${OS_LIBS}

TECHNOLOGY=dfs

SAMSFILES = cmu.sams
SAMS_TARGETS = dfscmumsg.h dfscmumsg.c dfscmumac.h dfscmusvc.c dfscmusvc.h \
    dfscmu.cat

IDIR 			= /bin/
dfscmu.cat_IDIR		= /nls/msg/en_US.ASCII/
ILIST 			= ${PROGRAMS}  dfscmu.cat

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
