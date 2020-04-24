# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.3  1996/03/11  02:11:42  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:24  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:51  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/10/10  14:45 UTC  truitt
# 	Merge new test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts49/1  1995/10/10  14:43 UTC  truitt
# 	CHFts16537: Update the tests.
# 
# 	HP revision /main/HPDCE02/2  1995/06/09  13:19 UTC  truitt
# 	Add new test.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts34/1  1995/06/09  13:18 UTC  truitt
# 	CHFts14292: Add missing test.
# 
# 	HP revision /main/HPDCE02/1  1995/03/03  13:04 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/03  13:03 UTC  truitt
# 	Add new cds misc tests.
# 	[1995/12/11  15:51:59  root]
# 
# Revision 1.1.2.3  1994/10/05  22:01:29  rousseau
# 	Renamed hcell tests
# 	[1994/10/05  21:59:45  rousseau]
# 
# Revision 1.1.2.2  1994/10/05  18:55:17  zee
# 	Add cdsalias_func tests.
# 	[1994/10/05  18:28:48  zee]
# 
# Revision 1.1.2.1  1994/09/28  21:38:22  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:02  melman]
# 
# Revision 1.1.2.9  1994/09/28  14:23:44  kevins
# 	OT 12378 added cdsalias tests
# 	[1994/09/28  14:17:04  kevins]
# 
# 	OT 12378 cdsalias functional tests
# 	[1994/09/28  11:07:03  kevins]
# 
# Revision 1.1.2.8  1994/09/28  11:51:01  kevins
# 	Cover submission to restore file to previous state
# 
# Revision 1.1.2.6  1994/06/10  20:03:26  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:03  devsrc]
# 
# Revision 1.1.2.5  1994/05/19  14:10:50  coren
# 	Added directory merge.
# 	[1994/05/11  21:10:13  coren]
# 
# Revision 1.1.2.4  1994/05/11  14:46:51  rousseau
# 	Updated to new install path and removed bad use of SCRIPTS (CR 10601).
# 	[1994/05/11  14:46:17  rousseau]
# 
# Revision 1.1.2.3  1994/04/15  19:24:09  kevins
# 	CR 9938 added directory synchronize
# 	[1994/04/15  15:32:45  kevins]
# 
# Revision 1.1.2.2  1994/03/10  17:35:11  kevins
# 	CR10118 Changed SCRIPTS to TCL_CODE
# 	[1994/03/10  17:32:49  kevins]
# 
# Revision 1.1.2.1  1994/03/09  19:20:14  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:20  kevins]
# 
# $EndLog$

CDS_FILES       = object_create_N.tcl object_create_P.tcl \
                  object_delete_N.tcl object_delete_P.tcl \
                  object_modify_N.tcl object_modify_P.tcl \
                  object_show_N.tcl object_show_P.tcl \
                  directory_add_N.tcl directory_add_P.tcl \
                  directory_create_N.tcl directory_create_P.tcl \
                  directory_delete_N.tcl directory_delete_P.tcl \
                  directory_list_N.tcl directory_list_P.tcl \
                  directory_merge_N.tcl directory_merge_P.tcl \
                  directory_modify_N.tcl directory_modify_P.tcl \
                  directory_show_N.tcl directory_show_P.tcl \
                  directory_remove_N.tcl directory_remove_P.tcl \
                  directory_synchronize_N.tcl directory_synchronize_P.tcl \
                  link_create_N.tcl link_create_P.tcl \
                  link_delete_N.tcl link_delete_P.tcl \
                  link_show_N.tcl link_show_P.tcl \
                  link_modify_N.tcl link_modify_P.tcl \
                  clearinghouse_create_N.tcl clearinghouse_create_P.tcl \
                  clearinghouse_delete_N.tcl clearinghouse_delete_P.tcl \
                  clearinghouse_disable_N.tcl clearinghouse_disable_P.tcl\
                  clearinghouse_show_N.tcl clearinghouse_show_P.tcl \
                  cdsalias_catalog_N.tcl cdsalias_catalog_P.tcl \
                  cdsalias_create_N.tcl cdsalias_create_P.tcl \
                  cdsalias_delete_N.tcl cdsalias_delete_P.tcl \
                  cdsalias_set_N.tcl cdsalias_set_P.tcl \
                  cdsalias_hcell_N.tcl cdsalias_hcell_P.tcl \
                  cds_disable_N.tcl cds_disable_P.tcl \
                  cds_show_N.tcl cds_show_P.tcl \
                  cdsclient_disable_N.tcl cdsclient_disable_P.tcl \
                  cdsclient_show_N.tcl cdsclient_show_P.tcl
IDIR            = /test/tet/functional/admin/dcecp/ts/cds/
ILIST           = ${CDS_FILES}
DATAFILES       = ${CDS_FILES}
IMODE           = 755

.include <${RULES_MK}>
