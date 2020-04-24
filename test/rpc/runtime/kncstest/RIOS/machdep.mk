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
# Revision 1.1.4.2  1996/03/11  02:28:28  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:18  marty]
#
# Revision 1.1.4.1  1995/12/11  20:14:20  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:47:07  root]
# 
# Revision 1.1.2.2  1993/01/13  19:53:47  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:21:25  bbelch]
# 
# Revision 1.1  1992/01/19  03:42:40  devrcs
# 	Initial revision
# 
# $EndLog$
#


RIOS_CFLAGS = -D_NONSTD_TYPES -DBSD -D_BSD -Dunix -U__STR__ -DDEBUG

RIOS_IMPORT_FILE = krpc.imp

RIOS_EXPORT_FILE = rpc.exp


ncsrdwr.o_CFLAGS = -DKERNEL -D_KERNEL ${${TARGET_MACHINE}_CFLAGS}

ncsio.o_CFLAGS   = -DKERNEL -D_KERNEL ${${TARGET_MACHINE}_CFLAGS}

RIOS_LIBS  = -lbsd

user_server_RIOS_LDFLAGS = -D0 -H512 -T512 -v -bR:user_server.map  \
                           -bI:${${${TARGET_MACHINE}_EXPORT_FILE}:P}

kserver_RIOS_LDFLAGS = -D0 -H512 -T512 -v  -bR:kserver.map   \
                       -bI:/lib/syscalls.exp -bI:/lib/kernex.exp \
                       -bI:${${${TARGET_MACHINE}_IMPORT_FILE}:P} -bE:${${${TARGET_MACHINE}_EXPORT_FILE}:P}

user_client_RIOS_LDFLAGS = -D0 -H512 -T512 -v -bR:user_client.map \
                           -bI:${${${TARGET_MACHINE}_EXPORT_FILE}:P}

kclient_RIOS_LDFLAGS = -D0 -H512 -T512 -v -bR:kclient.map \
                       -bI:/lib/syscalls.exp -bI:/lib/kernex.exp  \
                       -bI:${${${TARGET_MACHINE}_IMPORT_FILE}:P} -bE:${${${TARGET_MACHINE}_EXPORT_FILE}:P}


ncsio.o: ncsio.c ${${TARGET_MACHINE}_EXPORT_FILE}
