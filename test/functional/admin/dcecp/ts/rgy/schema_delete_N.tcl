#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_delete_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:23:46  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:53  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:20  root
# 	Submit
# 	[1995/12/11  14:31:09  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:46  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:21  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:53  melman]
# 
# Revision 1.1.2.5  1994/09/21  17:53:18  truitt
# 	Change some messages to reflect new schema name parsing.
# 	[1994/09/21  17:38:50  truitt]
# 
# Revision 1.1.2.4  1994/08/26  17:45:46  truitt
# 	OT11184: update tests to reflect new functionality.
# 	[1994/08/26  17:44:11  truitt]
# 
# Revision 1.1.2.3  1994/08/17  18:44:50  truitt
# 	OT11693 & others: update tests to reflect new functionality.
# 	[1994/08/17  18:36:54  truitt]
# 
# Revision 1.1.2.2  1994/08/10  19:22:24  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:43  annie]
# 
# Revision 1.1.2.1  1994/07/26  18:23:39  truitt
# 	Initial schema test drop
# 	[1994/07/26  17:33:02  truitt]
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
proc schema_delete_1_N {} {
    # Identify the test
    tet_infoline schema_delete_1_N: The delete command must have an attribute

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema delete} msg] $msg
    } compare_strings {1 {No schema name.}}

    # Cleanup steps
}

proc schema_delete_2_N {} {
    # Identify the test
    tet_infoline schema_delete_2_N: Too many arguments passed to delete

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema delete gumby pokey} msg] $msg
    } compare_strings {1 {Unrecognized argument 'pokey'.}}

    # Cleanup steps
}

proc schema_delete_3_N {} {
    # Identify the test
    tet_infoline schema_delete_3_N: The schema name is not a fully qualified schema name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema delete gumby} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc schema_delete_4_N {} {
    # Identify the test
    tet_infoline schema_delete_4_N: The schema name is not a fully qualified schema name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema delete /.:/gumby} msg] $msg
    } compare_strings {1 {Not a fully qualified schema name.}}

    # Cleanup steps
}

proc schema_delete_5_N {} {
    # Identify the test
    tet_infoline schema_delete_5_N: The schema name does not exist in the registry schema

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema delete /.:/sec/xattrschema/schema_delete_5_N} msg] $msg
    } compare_strings {1 {Schema entry '/.:/sec/xattrschema/schema_delete_5_N' not found.}}

    # Cleanup steps
}

proc schema_delete_6_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline schema_delete_6_N: The schema name does not exist in the hosts schema

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema delete "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_delete_6_N"} msg] $msg
    } compare_strings "1 {Schema entry '/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_delete_6_N' not found.}"

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "schema_delete_1_N"
set ic2 "schema_delete_2_N"
set ic3 "schema_delete_3_N"
set ic4 "schema_delete_4_N"
set ic5 "schema_delete_5_N"
set ic6 "schema_delete_6_N"
set tet_startup schema_delete_startup
set tet_cleanup schema_delete_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
