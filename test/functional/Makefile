# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.3  1996/03/11  02:08:23  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:24  marty]
#
# Revision 1.1.4.2  1995/12/13  15:23:39  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/05/17  15:38 UTC  mgm
# 	merge
# 
# 	HP revision /main/mgm_test/1  1995/04/14  19:15 UTC  mgm
# 	build functional tests only
# 	[1995/12/11  15:50:27  root]
# 
# Revision 1.1.2.7  1994/09/28  22:27:43  melman
# 	added admin
# 	[1994/09/28  22:27:20  melman]
# 
# Revision 1.1.2.6  1994/08/04  16:15:04  mdf
# 	added security to EXPLIB_SUBDIRS
# 	[1994/08/03  12:25:13  mdf]
# 
# Revision 1.1.2.5  1994/06/10  20:19:52  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:58:49  devsrc]
# 
# Revision 1.1.2.4  1994/05/10  19:28:14  zeliff
# 	ot 10519: add TETized GDS functional tests
# 	[1994/05/10  19:27:59  zeliff]
# 
# Revision 1.1.2.3  1994/05/06  13:50:33  hasbrouc
# 	add security/grp_override tests for group override support
# 	[1994/05/06  13:39:30  hasbrouc]
# 
# Revision 1.1.2.2  1994/05/04  21:53:01  mori_m
# 	CR 9701:  RPC runtime I18N extension functional test drop.
# 	[1994/05/04  21:07:16  mori_m]
# 
# Revision 1.1.2.1  1994/04/27  16:31:54  bhaim
# 	Makefile for src/test/functional
# 	[1994/04/27  16:30:35  bhaim]
# 
# $EndLog$

.ifdef BUILD_FUNC_TESTS
DATA_SUBDIRS = security admin 
EXPLIB_SUBDIRS = security
SUBDIRS = security admin
.else
SUBDIRS = rpc security directory admin
EXPLIB_SUBDIRS = security
DATA_SUBDIRS = rpc security directory admin
.endif

.include <${RULES_MK}>
