# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.2  1996/03/09  23:29:54  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:15  marty]
#
# Revision 1.1.6.1  1995/12/08  17:46:16  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/11/23  18:02 UTC  sommerfeld
# 	checkpoint (kernel & misc makefile cleanup)
# 	[1994/11/07  21:29 UTC  sommerfeld  /main/sommerfeld_mothmk_1/1]
# 
# Revision 1.1.4.1  1994/08/09  17:32:41  burati
# 	DFS/EPAC/KRPC/dfsbind changes
# 	[1994/08/09  17:00:44  burati]
# 
# Revision 1.1.2.2  94/07/22  15:49:25  rsarbo
# 	new makefile to build libksec.a which contains sec_cred interfaces
# 	for kernel use
# 
# $EndLog$
#
#
#
#  OSF DCE Version 1.1
#
#  NAME:
#
#      src/security/kutil/Makefile for SEC kernel runtime library
#
#  FACILITY:
#
#      Security 
#
#  ABSTRACT:
#
#  Kernel security library containing sec_cred() interfaces for doing
#  delegation in the DFS server.
#

VPATH		= ../utils:../utils/${TARGET_MACHINE}:${TARGET_MACHINE}:../idl

IDLFILES	= id_encode.idl sec_login_encode.idl authz_encode.idl
IDLFLAGS	= -keep c_source

HP800_KERNEL_CFLAGS += +Z

CFLAGS		= -DKERNEL -D_KERNEL ${${TARGET_MACHINE}_CFLAGS} \
		  ${KERNELCFLAGS} ${${TARGET_MACHINE}_KERNEL_CFLAGS}

EXPDIR		= /usr/lib/
INCFLAGS	= -I../utils -I../h

LIBRARIES	= libksec.a

OBJS	= sec_cred.o sec_cred_internal.o sec_encode.o id_encode_cstub.o \
	  sec_login_encode_cstub.o authz_encode_cstub.o sec_kmalloc.o \
	  restrictions.o

libksec.a_OFILES= $(OBJS)

.if exists (${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists (${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>

