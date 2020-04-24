#
# @OSF_FREE_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1993, 1992, 1991, 1990, 1996 Open Software Foundation, Inc.
# Permission is hereby granted to use, copy, modify and freely distribute
# the software in this file and its documentation for any purpose without
# fee, provided that the above copyright notice appears in all copies and
# that both the copyright notice and this permission notice appear in
# supporting documentation.  Further, provided that the name of Open
# Software Foundation, Inc. ("OSF") not be used in advertising or
# publicity pertaining to distribution of the software without prior
# written permission from OSF.  OSF makes no representations about the
# suitability of this software for any purpose.  It is provided "as is"
# without express or implied warranty.
#
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
# All Rights Reserved
#
#	SNI sinix (MX300i) SVR4 makefile
#	@(#)svr4.sinix.mk	1.10
# 
# HISTORY
# $Log: svr4.sinix.mk,v $
# Revision 1.1.8.2  1996/03/09  23:25:35  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:17  marty]
#
# Revision 1.1.8.1  1995/12/07  22:11:30  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:09:11  root]
# 
# Revision 1.1.6.2  1993/06/24  20:08:35  hinman
# 	[hinman] - Save merged-in SNI version (these files have been freed of nasty code)
# 	[1993/06/17  14:22:56  hinman]
# 
# Revision 1.1.3.5  1993/02/01  20:27:15  hinman
# 	[hinman@sni] - Final merge before bsubmit
# 	[1993/01/31  15:59:07  hinman]
# 
# Revision 1.1.3.4  1992/12/31  16:48:15  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:52:33  zeliff]
# 	Revision 1.1.4.2  1993/01/11  16:10:19  hinman
# 	[hinman] - Check in merged SNI version
# 
# Revision 1.1.3.3  1992/12/23  17:35:17  john
# 	Fixed the copyright marker to be OSF_FREE_COPYRIGHT.
# 	[1992/12/23  17:15:16  john]
# 
# Revision 1.1.3.2  1992/09/29  20:02:51  devsrc
# 	New file for SVR4 OSF merge
# 
# 	Submitted by Steve Gilbane
# 	[1992/09/11  15:33:05  rec]
# 
# $EndLog$
# 

.if !defined(_SINIX_DCE_MK_)
_SINIX_DCE_MK_=

#
# To add things to svr4.sinix.mk:
#	Put HARDWIRED variable assignments BEFORE <svr4.mk>
#	Put variable assignments that depend on other variables AFTER <svr4.mk>
#	Put new rules AFTER including svr4.mk 
#

BLD_USR_OR_KERNEL	?= kernel
TARGET_OS_PLATFORM	?= SINIX
RPC_UUID_KERNEL         ?= kernel 

TARGET_FLAGS		+= -D SNI_SINIX  -D SNI_MX300I -D M_I86 -D SNI_PRIVATE
.include <svr4.mk>
.endif
