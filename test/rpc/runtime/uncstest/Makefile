#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.2  1996/03/11  02:29:24  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:52  marty]
#
# Revision 1.1.6.1  1995/12/11  20:15:23  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:48:18  root]
# 
# Revision 1.1.4.3  1993/01/12  00:03:10  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:25:56  bbelch]
# 
# Revision 1.1.4.2  1992/11/02  15:37:49  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:40:01  root]
# 
# Revision 1.1.2.2  1992/05/01  15:42:03  rsalz
# 	Part of rpc6 code drop.
# 	[1992/05/01  01:24:21  rsalz]
# 
# Revision 1.1  1992/01/19  22:14:23  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#  OSF DCE Version 1.0 

VPATH		= ../ncstest

LIBS_DCE	= 
PROGRAMS 	= kernel_server kernel_client
INCFLAGS	= -I. -I../ncstest
CFLAGS		= -DDEBUG -DKERNEL_TEST_DRIVER ${${TARGET_MACHINE}_CFLAGS}

ILIST		= ${PROGRAMS}

kernel_server_OFILES	= server.o
kernel_client_OFILES	= client.o

.if exists (${TARGET_MACHINE}/machdep.mk)
.include <${TARGET_MACHINE}/machdep.mk>
.endif
.include <${RULES_MK}>
