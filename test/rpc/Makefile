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
# Revision 1.1.12.1  1996/06/04  22:11:39  arvind
# 	Comment out XIDL from HP800.
# 	[1996/04/09  15:15 UTC  arvind  /main/DCE_1.2/2]
#
# 	Comment out XIDL for HP800.
# 	[1996/04/09  15:14 UTC  arvind  /main/DCE_1.2/arvind_checkout/1]
#
# Revision 1.1.10.2  1996/03/11  02:08:14  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:51:03  marty]
# 
# Revision 1.1.10.1  1995/12/11  19:54:46  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/30  15:45 UTC  jrr
# 	Add idlcxx to proper DIR lists.
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/30  15:44 UTC  psn
# 	Add idlcxx to appropriate DIR's.
# 
# 	HP revision /main/HPDCE02/1  1995/05/17  15:39 UTC  mgm
# 	merge
# 
# 	HP revision /main/mgm_test/1  1995/04/14  19:15 UTC  mgm
# 	build functional tests only
# 	[1995/12/11  19:27:43  root]
# 
# Revision 1.1.8.1  1993/11/01  15:57:57  ganni
# 	OT 9220 - idl_frontend tests are moved to nosupport.
# 	    remove the reference to these tests.
# 	[1993/11/01  15:57:33  ganni]
# 
# Revision 1.1.6.2  1993/01/07  19:00:34  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  13:24:21  bbelch]
# 
# Revision 1.1.2.2  1992/05/01  15:50:32  rsalz
# 	Edited as part of rpc6 drop.
# 	[1992/05/01  00:57:08  rsalz]
# 
# Revision 1.1  1992/01/19  04:04:44  devrcs
# 	Initial revision
# 
# $EndLog$
#

.ifdef BUILD_FUNC_TESTS
DATA_SUBDIRS = rtandidl
SUBDIRS = rtandidl
.else
RIOS_XIDL = idlcxx
HP800_XIDL =

EXPINC_SUBDIRS = runtime rtandidl
DATA_SUBDIRS = idl rtandidl ${${TARGET_PLATFORM}_XIDL}
EXPLIB_SUBDIRS = idl ${${TARGET_PLATFORM}_XIDL}
SUBDIRS = runtime rtandidl idl ${${TARGET_PLATFORM}_XIDL}
.endif

.include <${RULES_MK}>
