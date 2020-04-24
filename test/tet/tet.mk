#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: tet.mk,v $
# Revision 1.1.22.2  1996/03/11  02:50:15  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:51  marty]
#
# Revision 1.1.22.1  1995/12/11  22:57:00  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:34:35  root]
# 
# Revision 1.1.20.1  1993/10/15  21:16:13  ohara
# 	create file
# 	[1993/10/15  16:50:40  ohara]
# 
# $EndLog$

.if !defined(_TET_MK_)
_TET_MK_=

.if ${TARGET_MACHINE}=="SVR4"
CC_LD = /usr/ccs/bin/cc
DCELIB  =
TARGET_LIBS = -lc -lsocket -lnsl
TARGET_LDFLAGS =
.endif

.endif
