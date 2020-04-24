#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:50:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:45  marty]
#
# Revision 1.1.4.1  1995/12/11  22:56:04  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:34:19  root]
# 
# Revision 1.1.2.3  1993/08/09  19:54:59  cmckeen
# 	Changed the install directory to /test/tet/ instead of /tet/.
# 	[1993/08/09  19:35:51  cmckeen]
# 
# Revision 1.1.2.2  1993/07/20  20:46:23  cmckeen
# 	Initial DCE version
# 	[1993/07/15  16:35:10  cmckeen]
# 
# $EndLog$
#
TET_ROOT	= /opt/dme1.0/test

PROGRAMS	= tcc

tcc_OFILES	= again.o config.o exec.o journal.o scenario.o \
                  startit.o tcc.o tool.o

tcc.o_CCXPG	=

SIG_IGNORE ?=
SIG_LEAVE  ?=
INCFLAGS	= -I../inc
CFLAGS		= -DLANGUAGE_C -DTET_ROOT='"$(TET_ROOT)"' \
                  -DTET_SIG_IGNORE="$(SIG_IGNORE)" -DTET_SIG_LEAVE="$(SIG_LEAVE)"

IDIR		= /test/tet/bin/
ILIST		= ${PROGRAMS}

.include <${RULES_MK}>
