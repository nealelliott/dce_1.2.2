#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
#  @SNI_COPYRIGHT@
#
#	SNI sinix (MX300i) SVR4 makefile
#	@(#)svr4.mk	1.10
# 
# HISTORY
# $Log: svr4.v3.mk,v $
# Revision 1.1.8.2  1996/03/09  23:25:36  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:18  marty]
#
# Revision 1.1.8.1  1995/12/07  22:11:37  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:09:15  root]
# 
# Revision 1.1.6.1  1994/10/06  20:27:45  agd
# 	expand copyright
# 	[1994/10/06  14:27:17  agd]
# 
# Revision 1.1.4.2  1993/06/24  20:08:44  hinman
# 	[hinman] - Save merged-in SNI version (these files have been freed of nasty code)
# 	[1993/06/17  14:23:14  hinman]
# 
# Revision 1.1.2.2  1993/02/01  19:26:52  hinman
# 	[hinman] - Check in SNI revision 1.1.2.3
# 	[1993/01/08  13:35:20  hinman]
# 
# 	Initial version.
# 
# Revision 1.1.2.3  92/10/28  17:43:14  tikku
# 	The rule to setup TARGET_LIBS to nothing in the setup phase.
# 
# Revision 1.1.2.2  92/10/03  10:47:02  tikku
# 	Initial version.
# 
# $EndLog$
# 

.if !defined(_SVR4_V3_MK_)
_SVR4_V3_MK_=

#
# To add things to svr4.v3.mk:
#	Put HARDWIRED variable assignments BEFORE <svr4.mk>
#	Put variable assignments that depend on other variables AFTER <svr4.mk>
#	Put new rules AFTER including svr4.mk 
#

BLD_USR_OR_KERNEL	?= kernel
TARGET_OS_PLATFORM	?= SVR4V3
RPC_UUID_KERNEL         ?= kernel 

TARGET_FLAGS		+= -D SNI_SVR4V3 -D SNI_SINIX  -D SNI_MX300I -D M_I86 

# SNI_PRIVATE -- SNI_NO_PRIVACY_FLAG stuff -- rcc
# by default, turn privacy on for svr4v3
SNI_NO_PRIVACY_FLAG	?=

NO_CDSBROWSER		= 1

.include <svr4.mk>
.endif
