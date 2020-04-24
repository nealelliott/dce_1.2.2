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
# Revision 1.1.6.2  1996/03/11  02:28:21  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:13  marty]
#
# Revision 1.1.6.1  1995/12/11  20:14:05  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:46:54  root]
# 
# Revision 1.1.4.2  1993/01/13  19:52:58  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:20:25  bbelch]
# 
# Revision 1.1.2.2  1992/05/01  13:48:12  rsalz
# 	Added as part of rpc6 drop.
# 	[1992/04/30  23:22:13  rsalz]
# 
# $EndLog$
#
#  OSF DCE Version 1.0 
#

SHARED_LIBRARIES	= libkncsdev.so
OTHERS			= ${SHARED_LIBRARIES}
SHARED_LIB_DEFS		= -DNCSDEV_DYNAMIC

libkncsdev.so_OFILES	= ${libkncsdev.a_OFILES}
libkncsdev.so_IDIR	= /sbin/subsys/
libkncsdev.so_EXPDIR	= /sbin/subsys/
libkncsdev.so_OFILES	= ${libkncsdev.a_OFILES}
libkncsdev.so_LDFLAGS	= -e ncsdev_init -nostdlib -export ncsdev: \
			  -L../kncstest -L../kv2test -L${EXPORTBASE}/sbin/subsys
libkncsdev.so_EXPORTS	= -export ncsdev: 
libkncsdev.so_LIBS	= -lkncstest -lkv2test -ldcerpc -lvmunix


