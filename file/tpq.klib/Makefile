# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.550.1  1996/10/02  18:48:46  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:49:18  damon]
#
# Revision 1.1.545.3  1994/06/09  14:23:03  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:34:38  annie]
# 
# Revision 1.1.545.2  1994/02/04  20:33:10  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:19:14  devsrc]
# 
# Revision 1.1.544.1  1993/12/07  17:35:58  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  17:19:35  jaffe]
# 
# Revision 1.1.2.4  1993/01/21  15:55:52  cjd
# 	embedded copyright notice
# 	[1993/01/20  17:39:20  cjd]
# 
# Revision 1.1.2.3  1992/09/25  19:43:20  jaffe
# 	Cleanup Minor header differences
# 	[1992/09/24  15:46:32  jaffe]
# 
# Revision 1.1.2.2  1992/08/31  21:33:33  jaffe
# 	Transarc delta: comer-ot3947-multithread-cm-daemon 1.25
# 	[1992/08/30  12:57:31  jaffe]
# 
# Revision 1.1.1.2  1992/08/30  12:57:31  jaffe
# 	Transarc delta: comer-ot3947-multithread-cm-daemon 1.25
# 
# $EndLog$
#

VPATH			= ../tpq

LIBRARIES		= libktpq.a

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=
CFLAGS			= ${KERNELCFLAGS} ${DFS_CORE}

INCFLAGS		= -I../tpq

libktpq.a_OFILES	= tpq_die.o tpq_dispatch.o tpq_helper.o \
			  tpq_init.o tpq_queue.o tpq_access.o tpq_stat.o \
			  tpq_pardo.o tpq_parset.o tpq_linkage.o

.if exists(../${TARGET_MACHINE}/machdep.mk)
.include "../${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(../${TARGET_OS}/machdep.mk)
.include "../${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

