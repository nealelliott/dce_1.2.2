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
# $Log: svr4.rm600.mk,v $
# Revision 1.1.6.2  1996/03/09  23:25:34  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:09  marty]
#
# Revision 1.1.6.1  1995/12/07  22:11:24  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:09:09  root]
# 
# Revision 1.1.4.1  1994/10/06  20:27:29  agd
# 	expand copyright
# 	[1994/10/06  14:27:13  agd]
# 
# Revision 1.1.2.2  1993/02/01  19:26:43  hinman
# 	[hinman] - Check in SNI revision 1.1.2.6
# 	[1993/01/08  12:48:43  hinman]
# 
# Revision 1.1.2.6  92/12/18  17:25:37  tikku
# 	Removed the define for SNI_PRIVATE.
# 	[92/12/18  17:24:29  tikku]
# 
# 	[bein] - Re-enable RPC_UUID_KERNEL for dcosx/rm600 platforms
# 	[92/12/16  11:08:44  bein]
# 
# 	PTC - do not build rpc/kernel/SVR4/kern_uniqaddr.o.
# 	PTC - it is already in the kernel by other means.
# 	[92/11/02  09:30:48  bein]
# 
# Revision 1.1.2.5  92/12/16  11:15:56  bein
# 		[bein] - Re-enable RPC_UUID_KERNEL for dcosx/rm600 platforms
# 
# Revision 1.1.2.4  92/12/16  10:11:07  tikku
# 	Reset HOST_CC, TRADITIONAL_CC and ANSI_CC back to the original
# 	settings. For some reason threads is not building with them. Also, turned
# 	on SNI_PRIVATE.
# 	[92/12/16  10:10:04  tikku]
# 
# 	hacks to build kernel subdir
# 	[92/08/18  14:55:15  greg]
# 
# Revision 1.1.2.3  92/12/11  15:24:19  tikku
# 	Set HOST_CC, TRADITIONAL_CC and ANSI_CC to newcc.
# 
# Revision 1.1.4.2  92/12/11  15:23:11  tikku
# 	Set HOST_CC, TRADITIONAL_CC and ANSI_CC to newcc.
# 
# Revision 1.1.2.2  92/11/18  12:03:18  hinman
# 	[hinman] - Initial version
# 	[92/11/18  12:02:16  hinman]
# 
# 	New version based on svr4.sinix.mk.
# 	[92/10/26  23:54:20  bein]
# 
# 	Get rid of EXPORTBASE in interpreter
# 	[92/08/06  14:10:51  steveg]
# 
# 	Add TARGET_LIBS
# 	[92/08/06  11:40:05  steveg]
# 
# 	First version
# 	[92/08/05  13:50:56  steveg]
# 
# Revision 1.1.2.3  92/11/06  07:14:13  bein
# 	PTC - Do not build kern_uniqueaddr.o. It is already in the kernel.
# 
# Revision 1.1.2.2  92/10/27  00:18:14  bein
# 		New file for DCOSX.
# 
# Revision 1.1.2.8  92/10/01  14:11:13  steveg
# 	Remove SNI_PROPRIETARY to be put in svr4.mk
# 	[92/10/01  14:09:56  steveg]
# 
# 	Added -D SNI_PRIVATE to list of TARGET_FLAGS.
# 	[92/09/29  16:38:08  tikku]
# 
# Revision 1.1.2.7  92/09/29  16:39:55  tikku
# 	Added -D SNI_PRIVATE to the list of TARGET flags.
# 
# Revision 1.1.2.6  92/08/18  15:16:53  greg
# 	cleaned up headers in svr4-specific files and hacked various make files to
# 	build them.
# 
# Revision 1.1.2.5  92/08/14  10:29:48  blurie
# 	Add SNI_PROPRIETARY to TARGET_FLAGS.
# 	[92/08/14  10:29:16  blurie]
# 
# Revision 1.1.2.4  92/08/06  14:11:57  steveg
# 	Get rid of EXPORTBASE from interpreter
# 
# Revision 9.5.2.2  92/08/05  13:20:54  steveg
# 	New version for OSF Merge
# 
# $EndLog$
# 

.if !defined(_RM600_DCE_MK_)
_RM600_DCE_MK_=

#
# To add things to svr4.sinix.mk:
#	Put HARDWIRED variable assignments BEFORE <svr4.mk>
#	Put variable assignments that depend on other variables AFTER <svr4.mk>
#	Put new rules AFTER including svr4.mk 
#

TARGET_OS_PLATFORM	?= PYRMIPS
# NOTE: kernel uuid capability is already in the kernel.
RPC_UUID_KERNEL         ?= kernel 

TARGET_FLAGS		+= -D SNI_DCOSX -D SNI_DCOSX_MIPS -D mips_is_ansi -DSNI_RM600_OLD_VARARGS

.include <svr4.mk>
.endif
