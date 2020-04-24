# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.15.2  1996/03/09  23:25:51  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:32  marty]
#
# Revision 1.2.15.1  1995/12/07  22:23:30  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/07/21  22:52 UTC  lmm
# 	New branch for stack saving mods
# 
# 	HP revision /main/lmm_reduce_rpc_stack/1  1995/06/28  17:03 UTC  lmm
# 	add -DHPDCE_SAVE_STACK to enable libidl stack saving mods
# 	[1995/12/07  21:13:33  root]
# 
# Revision 1.2.13.2  1994/08/19  15:55:14  tom
# 	Bug 11770 - Add CFLAGS and INCFLAGS for ctxeecli.o.
# 	[1994/08/19  15:55:01  tom]
# 
# Revision 1.2.13.1  1994/08/09  17:32:27  burati
# 	DFS/EPAC/KRPC/dfsbind changes
# 	[1994/08/08  21:53:34  burati]
# 
# Revision 1.2.9.2  1993/07/12  23:06:37  pwang
# 	Updated this makefile to match with lib/Makefile
# 	[1993/07/12  23:05:46  pwang]
# 
# Revision 1.2.4.3  1993/01/03  22:11:53  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  14:38:36  bbelch]
# 
# Revision 1.2.4.2  1992/12/23  19:08:38  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  01:05:57  zeliff]
# 
# Revision 1.2.2.2  1992/05/01  17:35:49  rsalz
# 	  27-apr-92 nm    Remove KERNEL_CC
# 	[1992/05/01  17:33:33  rsalz]
# 
# Revision 1.2  1992/01/19  22:13:27  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#
#  OSF DCE Version 1.0 
#
# src/idl/klib/Makefile for kernel IDL runtime library, libkidl.a
#

VPATH		= ../lib
INCFLAGS	= -I../lib

ctxeecli.o_INCFLAGS	= ${INCFLAGS} -I../../runtime
ctxeecli.o_CFLAGS	= ${CFLAGS} -DDCE_RPC_SVC

# Library to build and export, and its location
LIBRARIES	= libkidl.a
EXPDIR		= /usr/lib/

CFLAGS	= ${${TARGET_MACHINE}_CFLAGS} ${${TARGET_MACHINE}_KERNEL_CFLAGS} -D_KERNEL -DKERNEL -DIDL_LIB_FLAG -DMIA

.if ${SITE} == "HP" && ${TARGET_OS} == "HPUX"
CFLAGS  += -DHPDCE_SAVE_STACK
.endif	# SITE == "HP" && TARGET_OS == "HPUX"

# Files needed by both KERNEL and USER space
KERNEL_OFILES	= alfrsupp.o allocate.o ctxeecli.o ctxeectx.o ctxerrtl.o \
		  eenodtbl.o ernodtbl.o marbfman.o ndrchars.o nbaseool.o \
		  ndrcharp.o nidlalfr.o pipesupp.o sscmaset.o sscmasrv.o \
		  ndrui.o ndrui2.o ndrui3.o ndrmi.o ndrmi2.o ndrmi3.o \
		  interpsh.o pickling.o

# --- The USER_OFILES are not currently needed for kernel RPC applications ---
USER_OFILES	= eebool.o eebyte.o eechar.o eedouble.o eeenum.o eefloat.o \
		  erbool.o erbyte.o erchar.o erdouble.o erenum.o erfloat.o \
		  eehyper.o eelong.o eeshort.o eesmall.o eeuhyper.o \
		  erhyper.o erlong.o ershort.o ersmall.o eruhyper.o \
		  erulong.o erushort.o erusmall.o eeulong.o eeushort.o \
		  eeusmall.o ndrfloat.o autohndl.o cvt_glob.o ndrmi5.o \
		  ndrui5.o pickling.o

OFILES		= ${KERNEL_OFILES}

.if exists (${TARGET_MACHINE}/machdep.mk)
.include <${TARGET_MACHINE}/machdep.mk>
.endif

#
#standard set of rules
#
.include <${RULES_MK}>
