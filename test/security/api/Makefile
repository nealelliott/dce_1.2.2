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
# Revision 1.1.14.1  1996/11/14  16:17:38  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  14:31:41  arvind]
#
# Revision /main/DCE_1.2.2/3  1996/11/04  22:39 UTC  arvind
# 	Don't build CAPI for HP/UX
# 	[1996/10/18  15:36 UTC  arvind  /main/DCE_1.2.2/arvind_work/1]
# 
# Revision /main/DCE_1.2.2/arvind_work/4  1996/10/31  19:50 UTC  arvind
# 	Don't build CAPI tests for intl build
# 
# Revision /main/DCE_1.2.2/arvind_work/1  1996/10/18  15:36 UTC  arvind
# 	Don't build CAPI for HP/UX
# 
# Revision /main/DCE_1.2.2/1  1996/10/18  00:45 UTC  arvind
# 	CAPI drop from DEC
# 	[1996/10/16  20:47 UTC  arvind  /main/arvind_capi/1]
# 
# Revision 1.1.12.2  1996/03/11  02:29:52  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:01:04  marty]
# 
# Revision 1.1.12.1  1995/12/11  20:15:54  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:48:58  root]
# 
# Revision 1.1.10.1  1994/06/28  20:01:05  wray
# 	GSSAPI BL3 (OT 10824)
# 	[1994/06/28  19:25:27  wray]
# 
# Revision 1.1.8.2  1993/05/24  20:48:43  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:16:11  cjd]
# 
# Revision 1.1.6.2  1993/05/12  18:20:02  jd
# 	486 port: Add control to the subdirs line so install will visit.
# 	[1993/05/12  18:15:17  jd]
# 
# Revision 1.1.2.2  1992/12/31  17:25:15  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:50:12  htf]
# 
# Revision 1.1  1992/01/19  04:08:46  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# DCE Security Services 
# test/security/api Makefile
#

EXPINC_SUBDIRS = control
DATA_SUBDIRS   = control moretests
.if ${TARGET_MACHINE}=="HP800" || !defined(USE_DES)
SUBDIRS = testsh moretests control gssapi 
.else
SUBDIRS = testsh moretests control gssapi capi
.endif

.include <${RULES_MK}>
