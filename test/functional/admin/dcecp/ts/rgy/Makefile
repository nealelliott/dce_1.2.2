# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/10/03  18:36:07  arvind
# 	Merge changes.
# 	[1996/08/20  16:29 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk2/1]
#
# 	Changes for pk tests.
# 	[1996/08/19  16:59 UTC  truitt  /main/DCE_1.2.2/1]
#
# 	Initial branch.
# 	[1996/03/11  02:18:23  marty  1.1.4.3]
#
# Revision 1.1.4.3  1996/03/11  02:18:23  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:49  marty]
# 
# Revision 1.1.4.2  1995/12/13  15:27:06  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/09/11  16:21 UTC  truitt
# 	Merge test file additions.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts46/1  1995/09/11  16:20 UTC  truitt
# 	CHFts16100: Add new registry tests.
# 
# 	HP revision /main/HPDCE02/2  1995/03/30  15:33 UTC  truitt
# 	Added the registry destroy and designate tests.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts23/1  1995/03/30  15:32 UTC  truitt
# 	CHFts14675: Warranty patch work.  Changes to the registry
# 	delete and set command.
# 
# 	HP revision /main/HPDCE02/1  1995/03/02  20:07 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/02  20:06 UTC  truitt
# 	Add registry checkpoint tests.
# 	Add schema generic negative tests.
# 	[1995/12/11  15:54:05  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:17  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:32  melman]
# 
# Revision 1.1.2.16  1994/09/16  18:26:28  kevins
# 	OT 12199 added account generate tests
# 	[1994/09/16  18:25:59  kevins]
# 
# Revision 1.1.2.15  1994/08/31  23:04:17  salamone
# 	CR11916 - Add _b(sec) support.
# 	[1994/08/31  23:02:47  salamone]
# 
# Revision 1.1.2.14  1994/08/26  18:16:59  truitt
# 	OT11448: Add modify era tests.
# 	[1994/08/26  18:12:50  truitt]
# 
# Revision 1.1.2.13  1994/08/25  16:16:30  nav
# 	Added generic schema era tests.
# 	[1994/08/25  16:15:19  nav]
# 
# Revision 1.1.2.12  1994/08/24  19:05:20  truitt
# 	OT11448: Updated era tests
# 	[1994/08/24  19:03:35  truitt]
# 
# Revision 1.1.2.11  1994/08/05  19:15:42  nav
# 	Add pgo era create tests.
# 	[1994/08/05  18:53:14  nav]
# 
# Revision 1.1.2.10  1994/07/27  18:52:05  nav
# 	Added support for schema modify and schema rename.
# 	[1994/07/27  15:55:16  nav]
# 
# Revision 1.1.2.9  1994/07/26  18:20:09  truitt
# 	Add "schema" support.
# 	[1994/07/26  17:29:55  truitt]
# 
# Revision 1.1.2.8  1994/07/20  19:58:08  salamone
# 	Add "registry verify" support.
# 	[1994/07/20  19:41:59  salamone]
# 
# Revision 1.1.2.7  1994/06/29  21:38:54  salamone
# 	Add rest of "registry" FVT support.
# 	[1994/06/29  20:13:10  salamone]
# 
# Revision 1.1.2.6  1994/06/10  20:13:58  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:54  devsrc]
# 
# Revision 1.1.2.5  1994/05/11  14:46:54  rousseau
# 	Updated to new install path and removed bad use of SCRIPTS (CR 10601).
# 	[1994/05/11  14:46:20  rousseau]
# 
# Revision 1.1.2.4  1994/05/11  13:48:02  kevins
# 	Added registry modify/show commands.
# 	[1994/05/11  13:16:05  kevins]
# 
# Revision 1.1.2.3  1994/04/15  19:24:14  kevins
# 	CR 9938 add account functional tests
# 	[1994/04/15  15:31:13  kevins]
# 
# Revision 1.1.2.2  1994/03/10  17:35:15  kevins
# 	CR10118 Changed SCRIPTS to TCL_CODE
# 	[1994/03/10  17:29:41  kevins]
# 
# Revision 1.1.2.1  1994/03/09  19:21:28  kevins
# 	    Initial test drop.
# 	    [1994/03/09  19:18:04  kevins]
# 
# $EndLog$

RGY_FILES	= principal_catalog_N.tcl principal_catalog_P.tcl \
		  principal_create_N.tcl principal_create_P.tcl \
		  principal_delete_N.tcl principal_delete_P.tcl \
		  principal_show_N.tcl principal_show_P.tcl \
		  principal_modify_N.tcl principal_modify_P.tcl \
		  principal_rename_N.tcl principal_rename_P.tcl \
		  principal_create_era_N.tcl principal_create_era_P.tcl \
		  principal_modify_era_N.tcl principal_modify_era_P.tcl \
		  principal_show_era_N.tcl principal_show_era_P.tcl \
		  group_add_N.tcl group_add_P.tcl \
		  group_catalog_N.tcl group_catalog_P.tcl \
		  group_create_N.tcl group_create_P.tcl \
		  group_delete_N.tcl group_delete_P.tcl \
		  group_list_N.tcl group_list_P.tcl \
		  group_modify_N.tcl group_modify_P.tcl \
		  group_show_N.tcl group_show_P.tcl \
		  group_remove_N.tcl group_remove_P.tcl \
		  group_rename_N.tcl group_rename_P.tcl \
		  group_create_era_N.tcl group_create_era_P.tcl \
		  group_modify_era_N.tcl group_modify_era_P.tcl \
		  group_show_era_N.tcl group_show_era_P.tcl \
		  organization_add_N.tcl organization_add_P.tcl \
		  organization_catalog_N.tcl organization_catalog_P.tcl \
		  organization_create_N.tcl organization_create_P.tcl \
		  organization_list_N.tcl organization_list_P.tcl \
		  organization_modify_N.tcl organization_modify_P.tcl \
		  organization_rename_N.tcl organization_rename_P.tcl \
		  organization_remove_N.tcl organization_remove_P.tcl \
		  organization_delete_N.tcl organization_delete_P.tcl \
		  organization_show_N.tcl organization_show_P.tcl \
		  org_create_era_N.tcl org_create_era_P.tcl \
		  org_modify_era_N.tcl org_modify_era_P.tcl \
		  org_show_era_N.tcl org_show_era_P.tcl \
		  account_catalog_N.tcl account_catalog_P.tcl \
		  account_create_N.tcl account_create_P.tcl \
		  account_delete_N.tcl account_delete_P.tcl \
		  account_show_N.tcl account_show_P.tcl \
		  account_modify_N.tcl account_modify_P.tcl \
                  account_generate_N.tcl account_generate_P.tcl \
		  registry_catalog_N.tcl registry_catalog_P.tcl \
		  registry_checkpoint_N.tcl registry_checkpoint_P.tcl \
		  registry_connect_N.tcl registry_connect_P.tcl \
		  registry_delete_N.tcl registry_delete_P.tcl \
		  registry_designate_N.tcl registry_designate_P.tcl \
		  registry_destroy_N.tcl registry_destroy_P.tcl \
		  registry_disable_N.tcl registry_disable_P.tcl \
		  registry_dump_N.tcl registry_dump_P.tcl \
		  registry_enable_N.tcl registry_enable_P.tcl \
		  registry_modify_N.tcl registry_modify_P.tcl \
		  registry_replace_N.tcl registry_replace_P.tcl \
		  registry_show_N.tcl registry_show_P.tcl \
		  registry_stop_N.tcl registry_stop_P.tcl \
		  registry_synchronize_N.tcl registry_synchronize_P.tcl \
		  registry_verify_N.tcl registry_verify_P.tcl \
		  schema_catalog_N.tcl schema_catalog_P.tcl \
		  schema_create_N.tcl schema_create_P.tcl \
		  schema_delete_N.tcl schema_delete_P.tcl \
		  schema_show_N.tcl schema_show_P.tcl \
		  schema_modify_N.tcl schema_modify_P.tcl \
		  schema_rename_N.tcl schema_rename_P.tcl \
		  schema_modify_generic_N.tcl schema_modify_generic_P.tcl \
		  schema_rename_generic_N.tcl schema_rename_generic_P.tcl
PK_FILES	= pk_data_1 pk_data_2 pk_data_3
IDIR            = /test/tet/functional/admin/dcecp/ts/rgy/
ILIST           = ${RGY_FILES} ${PK_FILES}
DATAFILES       = ${RGY_FILES} ${PK_FILES}
IMODE           = 755

.include <${RULES_MK}>
