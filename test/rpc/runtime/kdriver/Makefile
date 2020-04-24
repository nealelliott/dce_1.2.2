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
# Revision 1.2.60.2  1996/03/11  02:28:23  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:14  marty]
#
# Revision 1.2.60.1  1995/12/11  20:14:06  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:46:56  root]
# 
# Revision 1.2.58.1  1993/12/28  22:11:38  pellis
# 	Updated for ODE 2.3 make
# 	[1993/12/28  21:55:03  pellis]
# 
# Revision 1.2.4.2  1993/01/13  19:53:02  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:20:34  bbelch]
# 
# Revision 1.2.2.2  1992/05/01  15:51:12  rsalz
# 	Edited as part of rpc6 drop.
# 	[1992/05/01  00:57:44  rsalz]
# 
# Revision 1.2  1992/01/19  22:14:03  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
# Makefile for kernel test jig driver
#
#  OSF DCE Version 1.0 

VPATH		= ${TARGET_OS}_TEMPLATE

LIBRARIES	= libkncsdev.a
EXPDIR		= /usr/lib/
CFLAGS		= -DDEBUG -DKERNEL -D_KERNEL -D_NO_PROTO ${${TARGET_MACHINE}_CFLAGS} \
		   ${${TARGET_MACHINE}_KERNEL_CFLAGS} ${TEST_DEFS} ${SHARED_LIB_DEFS}
TEST_DEFS	= -DNCSDEV_NCSTEST -DNCSDEV_V2TEST 

libkncsdev.a_OFILES	= ncsdev_sys.o

.if exists (${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>
