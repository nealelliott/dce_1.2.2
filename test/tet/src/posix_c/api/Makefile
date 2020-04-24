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
# Revision 1.1.4.2  1996/03/11  02:50:04  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:44  marty]
#
# Revision 1.1.4.1  1995/12/11  22:55:23  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:34:09  root]
# 
# Revision 1.1.2.2  1993/07/20  20:44:59  cmckeen
# 	Initial DCE version
# 	[1993/07/15  16:30:17  cmckeen]
# 
# $EndLog$
#
CFLAGS		= -DLANGUAGE_C
LIBRARIES	= libtetapi.a libtettcm.a libtettcmchild.a

EXPDIR		= /usr/lib/

libtetapi.a_OFILES	= cancel.o config.o resfile.o tet_exec.o tet_fork.o
libtettcm.a_OFILES	= tcm.o
libtettcmchild.a_OFILES = tcmchild.o

tcm.o_CCXPG	= 
tet_fork.o_CCXPG =

INCFLAGS	= -I../inc

.include <${RULES_MK}>
