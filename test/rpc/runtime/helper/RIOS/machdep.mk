#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.4.2  1996/03/11  02:28:18  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:11  marty]
#
# Revision 1.1.4.1  1995/12/11  20:13:47  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:46:42  root]
# 
# Revision 1.1.2.2  1993/01/13  19:52:04  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:19:34  bbelch]
# 
# Revision 1.1  1992/01/19  03:43:19  devrcs
# 	Initial revision
# 
# $EndLog$
#

RIOS_EXPORT_FILE = rpc.exp

RIOS_CFLAGS  = -D_BSD -DBSD

RIOS_LIBS = -lbsd

auth_helper_RIOS_LDFLAGS = -D0 -H512 -T512 -v -bR:auth_helper.map -bI:${${${TARGET_MACHINE}_EXPORT_FILE}:P}
