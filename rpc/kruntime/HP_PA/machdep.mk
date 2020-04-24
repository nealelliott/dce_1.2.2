# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.6.2  1996/03/09  23:26:07  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:45  marty]
#
# Revision 1.1.6.1  1995/12/08  00:14:13  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:55:48  root]
# 
# Revision 1.1.4.3  1993/01/03  22:35:06  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:50:35  bbelch]
# 
# Revision 1.1.4.2  1992/12/23  19:36:49  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  14:47:36  zeliff]
# 
# Revision 1.1.2.2  1992/05/01  13:37:16  rsalz
# 	Added as part of rpc6 drop.
# 	[1992/04/30  22:48:42  rsalz]
# 
# $EndLog$
#
#
#
#  OSF DCE Version 1.0 
#

SHARED_LIBRARIES	= libdcerpc.so
OTHERS			= ${SHARED_LIBRARIES}

libdcerpc.so_OFILES	= ${libknck.a_OFILES}
libdcerpc.so_IDIR       = /sbin/subsys/
libdcerpc.so_EXPDIR     = /sbin/subsys/
libdcerpc.so_OFILES     = ${libknck.a_OFILES}
libdcerpc.so_LDFLAGS    = -nostdlib -e rpc__init -export dcerpc: \
			  -L${EXPORTBASE}/sbin/subsys \
			  -u rpc_ss_ee_ctx_from_wire -u rpc_ss_ee_ctx_to_wire \
			  -u rpc_ss_er_ctx_from_wire -u rpc_ss_er_ctx_to_wire \
			  -u rpc_ss_initialize_callee_pipe 
libdcerpc.so_EXPORTS    = -export dcerpc:
libdcerpc.so_LIBS       = -lkdes -lkidl -lvmunix 


