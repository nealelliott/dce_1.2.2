#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_delete_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:23:49  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:54  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:21  root
# 	Submit
# 	[1995/12/11  14:31:10  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:47  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:23  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:54  melman]
# 
# Revision 1.1.2.6  1994/09/23  17:01:45  truitt
# 	Updated to reflect new aclmgr names.
# 	[1994/09/23  16:58:28  truitt]
# 
# Revision 1.1.2.5  1994/09/21  15:26:08  truitt
# 	Update tests to reflect new aclmgr for generic schema.
# 	[1994/09/21  14:57:03  truitt]
# 
# Revision 1.1.2.4  1994/08/26  17:45:46  truitt
# 	OT11184: update tests to reflect new functionality.
# 	[1994/08/26  17:44:23  truitt]
# 
# Revision 1.1.2.3  1994/08/17  18:44:53  truitt
# 	OT11693 & others: update tests to reflect new functionality.
# 	[1994/08/17  18:37:36  truitt]
# 
# Revision 1.1.2.2  1994/08/10  19:22:25  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:44  annie]
# 
# Revision 1.1.2.1  1994/07/26  18:24:06  truitt
# 	Initial schema test drop
# 	[1994/07/26  17:33:23  truitt]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc schema_delete_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_delete_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc schema_delete_1_P {} {
    # Identify the test
    tet_infoline schema_delete_1_P: Delete an existing schema name in the registry schema

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_delete_1_P -aclmgr {group r r r r} -encoding integer}] != 0} {return;}

    # Run the test
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_delete_1_P}] != 0} {return;}
    test {
        list [catch {xattrschema show /.:/sec/xattrschema/schema_delete_1_P} msg] $msg
    } compare_strings {1 {Schema entry '/.:/sec/xattrschema/schema_delete_1_P' not found.}}

    # Cleanup steps
}

proc schema_delete_2_P {} {
    # Identify the test
    tet_infoline schema_delete_2_P: Delete multiple existing schema names in the registry schema

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_delete_2_P_1 -aclmgr {group r r r r} -encoding integer}] != 0} {return;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_delete_2_P_2 -aclmgr {organization r r r r} -encoding integer}] != 0} {return;}

    # Run the test
    if {[dcp_wrap {xattrschema delete {/.:/sec/xattrschema/schema_delete_2_P_1 /.:/sec/xattrschema/schema_delete_2_P_2}}] != 0} {return;}
    test {
        list [catch {xattrschema show {/.:/sec/xattrschema/schema_delete_2_P_1 /.:/sec/xattrschema/schema_delete_2_P_2}} msg] $msg
    } compare_strings {1 {Schema entry '/.:/sec/xattrschema/schema_delete_2_P_2' not found.}}

    # Cleanup steps
}

proc schema_delete_3_P {} {
    global DCP_HOSTNAME_ONE DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline schema_delete_3_P: Delete an existing schema name in the hosts schema

    # Setup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {xattrschema create "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_delete_3_P" -aclmgr {srvrconf r r r r} -encoding integer}] != 0} {return;}

    # Run the test
    if {[dcp_wrap {xattrschema delete "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_delete_3_P"}] != 0} {return;}
    test {
        list [catch {xattrschema show "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_delete_3_P"} msg] $msg
    } compare_strings "1 {Schema entry '/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_delete_3_P' not found.}"

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_delete_4_P {} {
    global DCP_HOSTNAME_ONE DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline schema_delete_4_P: Delete multiple existing schema names in two different schemas

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_delete_4_P_1 -aclmgr {group r r r r} -encoding integer}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {xattrschema create "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_delete_4_P_2" -aclmgr {srvrconf r r r r} -encoding integer}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_delete_4_P_1}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {xattrschema delete "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_delete_4_P_2"}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    test {
        list [catch {xattrschema show {"/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_delete_4_P_2" /.:/sec/xattrschema/schema_delete_4_P_1}} msg] $msg
    } compare_strings {1 {Schema entry '/.:/sec/xattrschema/schema_delete_4_P_1' not found.}}

    # Cleanup steps
}

set iclist "ic1 ic2 ic3 ic4 "
set ic1 "schema_delete_1_P"
set ic2 "schema_delete_2_P"
set ic3 "schema_delete_3_P"
set ic4 "schema_delete_4_P"
set tet_startup schema_delete_startup
set tet_cleanup schema_delete_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
