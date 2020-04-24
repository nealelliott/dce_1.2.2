#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_catalog_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:23:34  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:48  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:14  root
# 	Submit
# 	[1995/12/11  14:31:05  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:41  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:14  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:48  melman]
# 
# Revision 1.1.2.7  1994/09/23  17:01:41  truitt
# 	Updated to reflect new aclmgr names.
# 	[1994/09/23  16:58:01  truitt]
# 
# Revision 1.1.2.6  1994/09/21  15:26:02  truitt
# 	Update tests to reflect new aclmgr for generic schema.
# 	[1994/09/21  14:55:36  truitt]
# 
# Revision 1.1.2.5  1994/09/07  12:56:11  truitt
# 	Update schema tests to reflect new messages
# 	[1994/09/06  18:44:31  truitt]
# 
# Revision 1.1.2.4  1994/08/26  17:45:37  truitt
# 	OT11184: update tests to reflect new functionality.
# 	[1994/08/26  17:43:06  truitt]
# 
# Revision 1.1.2.3  1994/08/17  18:44:41  truitt
# 	OT11693 & others: update tests to reflect new functionality.
# 	[1994/08/17  18:41:54  truitt]
# 
# Revision 1.1.2.2  1994/08/10  19:22:19  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:39  annie]
# 
# Revision 1.1.2.1  1994/07/26  18:22:23  truitt
# 	Initial schema test drop
# 	[1994/07/26  17:31:19  truitt]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc schema_catalog_1_P {} {
    # Identify the test
    tet_infoline schema_catalog_1_P: Show the registry schema catalog with longnames

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}

    # Run the test
    test {if {[lsearch [xattrschema catalog /.:/sec/xattrschema] "$DCP_CELLNAME_ONE/sec/xattrschema/X500_DN"] != -1} {
        list found} else {list not_found}} compare_strings found

    # Cleanup steps
}

proc schema_catalog_2_P {} {
    # Identify the test
    tet_infoline schema_catalog_2_P: Show the registry schema catalog with shortnames

    # Setup steps

    # Run the test
    test {if {[lsearch [xattrschema catalog -simplename /.:/sec/xattrschema] "X500_DN"] != -1} {
        list found} else {list not_found}} compare_strings found

    # Cleanup steps
}

proc schema_catalog_3_P {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline schema_catalog_3_P: Show the hosts schema catalog with longnames

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}

    # Run the test
    test {if {[lsearch [xattrschema catalog "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema"] "$DCP_CELLNAME_ONE/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/hostdata/data"] != -1} {
        list found} else {list not_found}} compare_strings found

    # Cleanup steps
}

proc schema_catalog_4_P {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline schema_catalog_4_P: Show the hosts schema catalog with shortnames

    # Setup steps

    # Run the test
    test {if {[lsearch [xattrschema catalog -simplename "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema"] "hostdata/data"] != -1} {
        list found} else {list not_found}} compare_strings found

    # Cleanup steps
}

proc schema_catalog_5_P {} {
    global DCP_CELLNAME_ONE DCP_HOSTNAME_ONE DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline schema_catalog_5_P: Show both schemas

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_catalog_5_P -encoding integer -aclmgr {group r r r r}}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {xattrschema create "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_catalog_5_P" -encoding integer -aclmgr {srvrconf r r r r}}] != 0} {return;}

    # Run the test
    test {if {[lsearch [xattrschema catalog -simplename {"/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema" /.:/sec/xattrschema}] "schema_catalog_5_P"] != -1} {
        list found} else {list not_found}} compare_strings found

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_catalog_5_P}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {xattrschema delete "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_catalog_5_P"}] != 0} {return;}
}

proc schema_catalog_6_P {} {
    # Identify the test
    tet_infoline schema_catalog_6_P: Show that the schema entry can end in '/'

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}

    # Run the test
    test {if {[lsearch [xattrschema catalog /.:/sec/xattrschema/] "$DCP_CELLNAME_ONE/sec/xattrschema/X500_DN"] != -1} {
        list found} else {list not_found}} compare_strings found

    # Cleanup steps
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "schema_catalog_1_P"
set ic2 "schema_catalog_2_P"
set ic3 "schema_catalog_3_P"
set ic4 "schema_catalog_4_P"
set ic5 "schema_catalog_5_P"
set ic6 "schema_catalog_6_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
