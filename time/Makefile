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
# Revision 1.1.9.2  1996/03/09  23:31:08  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:21  marty]
#
# Revision 1.1.9.1  1995/12/08  18:04:02  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/06/29  18:48 UTC  pare
# 	Merge hpdce02 (DCE 1 beta) and HPDCE01 into HPDCE02
# 	[1995/12/08  17:23:21  root]
# 
# Revision 1.1.6.2  1994/01/21  21:30:23  rsalz
# 	Merged with changes from 1.1.6.1
# 	[1994/01/21  21:30:09  rsalz]
# 
# 	OT CR 9747:  Add SVC and AUD to dtsd
# 	[1994/01/21  21:07:19  rsalz]
# 
# Revision 1.1.6.1  1994/01/21  21:07:58  rsalz
# 	OT CR 9747:  Add SVC and AUD to dtsd
# 	[1994/01/21  21:07:19  rsalz]
# 
# Revision 1.1.2.4  1992/12/30  20:37:08  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:05:25  htf]
# 
# Revision 1.1.2.3  1992/10/21  19:23:34  john
# 	Added SETUP_SUBDIRS to build zoneinfo/zic
# 	[1992/10/21  19:21:41  john]
# 
# Revision 1.1.2.2  1992/10/20  19:07:21  comuzzi
# 	Add public domain timezone code
# 	[1992/10/20  18:54:16  comuzzi]
# 
# Revision 1.1  1992/01/19  15:35:28  devrcs
# 	Initial revision
# 
# $EndLog$
#

DATA_SUBDIRS = ${KERNEL} libutc
EXPINC_SUBDIRS = ${KERNEL} common libutc service
EXPLIB_SUBDIRS = ${KERNEL} common libutc control
SUBDIRS = ${KERNEL} common libutc service control zoneinfo
SAMIDL_SUBDIRS = common
SETUP_SUBDIRS = zoneinfo
.include <${RULES_MK}>
