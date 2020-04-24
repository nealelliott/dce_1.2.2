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
# Revision 1.1.186.1  1996/10/02  18:13:39  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:45:42  damon]
#
# Revision 1.1.181.3  1994/08/01  17:57:38  sasala
# 	Implement partial S12Y (Internationalization) for DFS commands and daemons
# 	[1994/08/01  15:07:59  sasala]
# 
# 	Implement partial S12Y (internationalization) for DFS commands and daemons
# 	[1994/07/21  14:51:35  sasala]
# 
# Revision 1.1.181.2  1994/06/09  14:17:57  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:30:00  annie]
# 
# Revision 1.1.181.1  1994/02/04  20:27:31  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:16:53  devsrc]
# 
# Revision 1.1.179.1  1993/12/07  17:31:56  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  16:19:11  jaffe]
# 
# Revision 1.1.6.4  1993/01/21  14:54:37  cjd
# 	embedded copyright notice
# 	[1993/01/20  14:58:45  cjd]
# 
# Revision 1.1.6.3  1992/11/20  20:17:40  rsarbo
# 	gsys needs to link libdce rather than libafssys
# 	on PMAX
# 	[1992/11/20  19:32:20  rsarbo]
# 
# Revision 1.1.6.2  1992/08/31  20:51:32  jaffe
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
# 	[1992/08/30  03:56:08  jaffe]
# 
# Revision 1.1.2.4  1992/06/08  22:46:02  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Replaced -lbsd with ${BSD_LIBS} variable
# 	[1992/06/08  21:45:46  mason]
# 
# Revision 1.1.2.3  1992/05/05  19:00:49  rec
# 	Removed dependence on libkrb5.a by allowing libdce.a
# 	to export required symbol (krb5_cksumarray).
# 	[1992/05/05  18:55:45  rec]
# 
# Revision 1.1.2.2  1992/04/07  20:27:23  rec
# 	Have -lkrb5 on link line depend on RIOS platform since
# 	it is not needed with a shared libdce on OSF/1.
# 	[1992/04/07  20:05:00  rec]
# 
# Revision 1.1  1992/01/19  02:43:28  devrcs
# 	Initial revision
# 
# $EndLog$
#

PROGRAMS = fxd gsys

INCLUDES                = dfsfxdmsg.h dfsfxdsvc.h
EXPDIR                  = /usr/include/dcedfs/


CFLAGS 		= ${USERCFLAGS}
fxd_OFILES	= pxd.o  dfsfxdsvc.o dfsfxdmsg.o fxd_svc.o
gsys_OFILES	= gsys.o  dfsfxdsvc.o dfsfxdmsg.o fxd_svc.o

fxd_LIBS 	= -lafsrpcd -lafssys -lnubik -licl -lcmd -ltpq\
		  -lafsutil -lncompat -ldacl -ldauth -ldacllfs -ldacl \
		  -ldauth -ldacllfs -losi -lcom_err -ldce ${OS_LIBS}

.if ${TARGET_MACHINE} == "PMAX" 
gsys_LIBS 	= -ldce
.else
gsys_LIBS 	= -lafssys -ldce ${OS_LIBS}
.endif

TECHNOLOGY=dfs

SAMSFILES = fxd.sams
SAMS_TARGETS = dfsfxdmsg.h dfsfxdmsg.c dfsfxdmac.h dfsfxdsvc.c dfsfxdsvc.h \
    dfsfxd.cat

ILIST = fxd  dfsfxd.cat
IDIR = /bin/
dfsfxd.cat_IDIR          = /nls/msg/en_US.ASCII/

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
