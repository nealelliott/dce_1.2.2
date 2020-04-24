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
# Revision 1.1.8.2  1996/03/11  02:29:49  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:01:03  marty]
#
# Revision 1.1.8.1  1995/12/11  20:15:53  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/05/17  15:39 UTC  mgm
# 	merge
# 
# 	HP revision /main/mgm_test/3  1995/05/12  19:39 UTC  mgm
# 	add audit
# 
# 	HP revision /main/mgm_test/2  1995/05/11  14:47 UTC  mgm
# 	add svr_example
# 
# 	HP revision /main/mgm_test/1  1995/04/14  19:15 UTC  mgm
# 	build functional tests only
# 	[1995/12/11  19:48:56  root]
# 
# Revision 1.1.6.4  1994/08/04  16:16:06  mdf
# 	removed tet-tests becuase it was moved to the functional area
# 	[1994/08/03  12:23:04  mdf]
# 
# Revision 1.1.6.3  1994/08/02  20:12:13  bhaim
# 	move acl_server to test area
# 	[1994/08/02  20:10:30  bhaim]
# 
# Revision 1.1.6.2  1994/04/18  19:40:22  zeliff
# 	OT10386: turn on audit FVT in build
# 	[1994/04/18  19:30:39  zeliff]
# 
# Revision 1.1.6.1  1994/03/02  17:56:17  hanfei
# 	Initial ERA drop
# 	[1994/02/18  19:09:03  hanfei]
# 
# 	TET tests for ERA
# 
# Revision 1.1.4.2  1992/12/31  17:25:09  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:50:08  htf]
# 
# Revision 1.1.2.2  1992/07/17  16:41:16  sekhar
# 	OT CR 2778 : Deleted auth_rpc from DATA_SUBDIRS and SUBDIRS since auth_rpc
# 	             is now defuncted and should not be visited.
# 	[1992/07/16  13:41:20  sekhar]
# 
# Revision 1.1  1992/01/19  04:09:10  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# DCE Security Services 
# test/security Makefile
#

# Subdirectories that have DATAFILES
DATA_SUBDIRS = api commands

# Subdirectories that build programs
.ifdef BUILD_FUNC_TESTS
SUBDIRS = api audit svr_example
.else
SUBDIRS = audit api svr_example commands 
.endif

.include <${RULES_MK}>


