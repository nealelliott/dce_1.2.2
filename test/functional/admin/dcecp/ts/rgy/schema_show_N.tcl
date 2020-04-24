#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_show_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:24:22  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:06  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:40  root
# 	Submit
# 	[1995/12/11  14:31:19  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:59  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:38  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:03  melman]
# 
# Revision 1.1.2.5  1994/09/21  17:53:21  truitt
# 	Change some messages to reflect new schema name parsing.
# 	[1994/09/21  17:39:30  truitt]
# 
# Revision 1.1.2.4  1994/08/26  17:45:47  truitt
# 	OT11184: update tests to reflect new functionality.
# 	[1994/08/26  17:44:30  truitt]
# 
# Revision 1.1.2.3  1994/08/17  18:45:06  truitt
# 	OT11693 & others: Update show tests to reflect new functionality.
# 	[1994/08/17  18:34:03  truitt]
# 
# Revision 1.1.2.2  1994/08/10  19:22:29  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:47  annie]
# 
# Revision 1.1.2.1  1994/07/26  18:24:35  truitt
# 	Initial schema test drop
# 	[1994/07/26  17:33:57  truitt]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc schema_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc schema_show_1_N {} {
    # Identify the test
    tet_infoline schema_show_1_N: The show command must have an attribute

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema show} msg] $msg
    } compare_strings {1 {No schema name.}}

    # Cleanup steps
}

proc schema_show_2_N {} {
    # Identify the test
    tet_infoline schema_show_2_N: Too many arguments passed to show

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema show gumby pokey} msg] $msg
    } compare_strings {1 {Unrecognized argument 'pokey'.}}

    # Cleanup steps
}

proc schema_show_3_N {} {
    # Identify the test
    tet_infoline schema_show_3_N: The schema name is not a fully qualified schema name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema show gumby} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc schema_show_4_N {} {
    # Identify the test
    tet_infoline schema_show_4_N: The schema name is not a fully qualified schema name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema show /.:/gumby} msg] $msg
    } compare_strings {1 {Not a fully qualified schema name.}}

    # Cleanup steps
}

proc schema_show_5_N {} {
    # Identify the test
    tet_infoline schema_show_5_N: The schema name does not exist in the registry schema

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema show /.:/sec/xattrschema/schema_show_5_N} msg] $msg
    } compare_strings {1 {Schema entry '/.:/sec/xattrschema/schema_show_5_N' not found.}}

    # Cleanup steps
}

proc schema_show_6_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline schema_show_6_N: The schema name does not exist in the hosts schema

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema show "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_show_6_N"} msg] $msg
    } compare_strings "1 {Schema entry '/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_show_6_N' not found.}"

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "schema_show_1_N"
set ic2 "schema_show_2_N"
set ic3 "schema_show_3_N"
set ic4 "schema_show_4_N"
set ic5 "schema_show_5_N"
set ic6 "schema_show_6_N"
set tet_startup schema_show_startup
set tet_cleanup schema_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
