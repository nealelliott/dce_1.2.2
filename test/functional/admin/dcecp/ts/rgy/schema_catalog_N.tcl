#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_catalog_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:23:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:47  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:13  root
# 	Submit
# 	[1995/12/11  14:31:05  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:39  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:12  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:47  melman]
# 
# Revision 1.1.2.5  1994/09/07  12:56:09  truitt
# 	Update schema tests to reflect new messages
# 	[1994/09/06  18:44:20  truitt]
# 
# Revision 1.1.2.4  1994/08/26  17:45:35  truitt
# 	OT11184: update tests to reflect new functionality.
# 	[1994/08/26  17:42:53  truitt]
# 
# Revision 1.1.2.3  1994/08/17  18:44:36  truitt
# 	OT11693 & others: update tests to reflect new functionality.
# 	[1994/08/17  18:40:23  truitt]
# 
# Revision 1.1.2.2  1994/08/10  19:22:18  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:38  annie]
# 
# Revision 1.1.2.1  1994/07/26  18:21:56  truitt
# 	Initial schema test drop
# 	[1994/07/26  17:30:52  truitt]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc schema_catalog_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_catalog_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc schema_catalog_1_N {} {
    # Identify the test
    tet_infoline schema_catalog_1_N: The catalog command must have an attribute

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema catalog} msg] $msg
    } compare_strings {1 {No schema name.}}

    # Cleanup steps
}

proc schema_catalog_2_N {} {
    # Identify the test
    tet_infoline schema_catalog_2_N: Too few arguments passed to catalog

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema catalog -simplename} msg] $msg
    } compare_strings {1 {No schema name.}}

    # Cleanup steps
}

proc schema_catalog_3_N {} {
    # Identify the test
    tet_infoline schema_catalog_3_N: Too many arguments passed to catalog

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema catalog gumby pokey} msg] $msg
    } compare_strings {1 {Unrecognized argument 'pokey'.}}

    # Cleanup steps
}

proc schema_catalog_4_N {} {
    # Identify the test
    tet_infoline schema_catalog_4_N: The schema is illegal or unknown

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema catalog gumby} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc schema_catalog_5_N {} {
    # Identify the test
    tet_infoline schema_catalog_5_N: The schema is illegal or unknown

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema catalog -simplename gumby} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc schema_catalog_6_N {} {
    # Identify the test
    tet_infoline schema_catalog_6_N: Catch a slightly illegal schema name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema catalog /.:/sec/xattrschem} msg] $msg
    } compare_strings {1 {Bad or invalid dce attr schema object name(s)}}

    # Cleanup steps
}

proc schema_catalog_7_N {} {
    # Identify the test
    tet_infoline schema_catalog_7_N: Catch a slightly illegal schema name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema catalog /.:/sec/xattrschemagumby} msg] $msg
    } compare_strings {1 {Invalid attribute schema name specified}}

    # Cleanup steps
}

proc schema_catalog_8_N {} {
    # Identify the test
    tet_infoline schema_catalog_8_N: Catch a slightly illegal schema name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema catalog /.:/sec/xattrschema/gumby} msg] $msg
    } compare_strings {1 {Invalid attribute schema name specified}}

    # Cleanup steps
}

proc schema_catalog_9_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline schema_catalog_9_N: Catch a slightly illegal schema name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema catalog "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschem"} msg] $msg
    } compare_strings {1 {Bad or invalid dce attr schema object name(s)}}

    # Cleanup steps
}

proc schema_catalog_10_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline schema_catalog_10_N: Catch a slightly illegal schema name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema catalog "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschemagumby"} msg] $msg
    } compare_strings {1 {Bad or invalid dce attr schema object name(s)}}

    # Cleanup steps
}

proc schema_catalog_11_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline schema_catalog_11_N: Catch a slightly illegal schema name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema catalog "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/gumby"} msg] $msg
    } compare_strings {1 {Invalid attribute schema name specified}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 "
set ic1 "schema_catalog_1_N"
set ic2 "schema_catalog_2_N"
set ic3 "schema_catalog_3_N"
set ic4 "schema_catalog_4_N"
set ic5 "schema_catalog_5_N"
set ic6 "schema_catalog_6_N"
set ic7 "schema_catalog_7_N"
set ic8 "schema_catalog_8_N"
set ic9 "schema_catalog_9_N"
set ic10 "schema_catalog_10_N"
set ic11 "schema_catalog_11_N"
set tet_startup schema_catalog_startup
set tet_cleanup schema_catalog_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
