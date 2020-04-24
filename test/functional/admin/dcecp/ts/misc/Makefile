# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.3  1996/03/11  02:17:38  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:32  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:47  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/03  14:44 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/03  14:43 UTC  truitt
# 	Add a couple missing tests.
# 	[1995/12/11  15:53:49  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:50  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:06  melman]
# 
# Revision 1.1.2.3  1994/06/30  19:21:52  kevins
# 	OT 9938 added resolve_P.tcl
# 	[1994/06/30  19:20:22  kevins]
# 
# Revision 1.1.2.2  1994/06/17  18:43:21  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:26:10  devsrc]
# 
# Revision 1.1.2.1  1994/06/02  18:50:43  kevins
# 	Initial Misc commands test drop
# 	[1994/06/02  18:44:38  kevins]
# 
# $EndLog$

MISC_FILES 	= errtext_N.tcl errtext_P.tcl \
		  login_N.tcl  login_P.tcl \
		  logout_N.tcl logout_P.tcl \
		  name_compare_N.tcl name_compare_P.tcl \
		  name_expand_N.tcl name_expand_P.tcl \
		  resolve_N.tcl resolve_P.tcl \
		  shell_N.tcl shell_P.tcl \
		  uuid_compare_N.tcl uuid_compare_P.tcl \
		  uuid_create_N.tcl uuid_create_P.tcl

IDIR            = /test/tet/functional/admin/dcecp/ts/misc/
ILIST           = ${MISC_FILES}
DATAFILES       = ${MISC_FILES}
IMODE           = 755

.include <${RULES_MK}>
