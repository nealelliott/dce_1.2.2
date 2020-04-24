# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.129.1  1996/10/02  18:15:01  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:46:14  damon]
#
# Revision 1.1.124.3  1994/08/17  14:07:06  ruby
# 	Implement partial S12Y (Internationalization) for DFS commands and daemons
# 	[1994/08/17  14:06:42  ruby]
# 
# 	Implement partial S12Y (Internationalization) for DFS commands and daemons
# 	[1994/08/16  20:00:25  ruby]
# 
# Revision 1.1.124.2  1994/06/09  14:18:21  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:30:23  annie]
# 
# Revision 1.1.124.1  1994/02/04  20:28:07  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:17:11  devsrc]
# 
# Revision 1.1.122.1  1993/12/07  17:32:23  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  16:41:37  jaffe]
# 
# Revision 1.1.7.4  1993/01/21  15:16:09  cjd
# 	embedded copyright notice
# 	[1993/01/20  15:52:03  cjd]
# 
# Revision 1.1.7.3  1993/01/13  19:14:36  shl
# 	Transarc delta: kazar-ot6523-uuid-encodes-primary-secondary-pool-selection 1.3
# 	  Selected comments:
# 	    some changes to prevent deadlock between primary/secondary thread pools
# 	    Changes basic connection protocol.
# 	    scout now depends on libdfsncs.a
# 	    fix confusion about different types of sockaddr_ins
# 	[1993/01/12  21:54:44  shl]
# 
# Revision 1.1.7.2  1992/08/31  21:00:25  jaffe
# 	Transarc delta: jaffe-ot4923-remove-krb5-from-scout 1.1
# 	  Selected comments:
# 	    remove libkrb5 from the RIOS platform.
# 	    remove use of RIOS_LIB so that we don't get libkrb5 anymore.
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
# 	[1992/08/30  04:16:51  jaffe]
# 
# Revision 1.1.3.9  1992/06/08  22:46:08  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Replaced -lbsd with ${BSD_LIBS} variable
# 	[1992/06/08  21:48:43  mason]
# 
# Revision 1.1.3.8  1992/05/05  19:00:54  rec
# 	Removed dependence on libkrb5.a by allowing libdce.a
# 	to export required symbol (krb5_cksumarray).
# 	[1992/05/05  18:56:12  rec]
# 
# Revision 1.1.3.7  1992/04/07  20:27:29  rec
# 	Have -lkrb5 on link line depend on RIOS platform since
# 	it is not needed with a shared libdce on OSF/1.
# 	[1992/04/07  20:05:22  rec]
# 
# Revision 1.1.3.6  1992/01/30  14:55:32  carl
# 	add lib krb5 to the libs list
# 	[1992/01/30  14:55:02  carl]
# 
# Revision 1.1.3.5  1992/01/26  00:26:29  zeliff
# 	Merging OSF porting changes:  moved afssys to the end
# 	of LIBS
# 	[1992/01/26  00:16:32  zeliff]
# 
# Revision 1.1.3.4  1992/01/25  20:49:33  zeliff
# 	dfs6.3 merge, part2
# 	[1992/01/25  20:08:27  zeliff]
# 
# $EndLog$

PROGRAMS		= scout

CFLAGS			= -Dunix ${USERCFLAGS}
 
scout_OFILES 		=  scout.o dfsscmsvc.o dfsscmmsg.o
LIBS			= -lfsprobe -lgtx -lvolc -lpipe -lftutil \
			  -lrep -lftserver -lfldb \
			  -lnubik -licl -lcmd -lafs4int -lncompat -ltpq \
			  -ldacl -ldauth -ldacllfs -ldacl -ldauth \
			  -ldacllfs -ldfsncs -lbomb \
			  -lafsutil -lcom_err -lcommondata \
			  -lafssys -losi -ldce -lcurses ${OS_LIBS}

TECHNOLOGY=dfs

SAMSFILES = scm.sams
SAMS_TARGETS = dfsscmmsg.h dfsscmmsg.c dfsscmmac.h dfsscmsvc.c dfsscmsvc.h \
    dfsscm.cat

IDIR 			= /bin/
dfsscm.cat_IDIR          = /nls/msg/en_US.ASCII/
ILIST 			= ${PROGRAMS}  dfsscm.cat

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
