#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_rename_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:24:11  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:03  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:32  root
# 	Submit
# 	[1995/12/11  14:31:16  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:54  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:32  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:00  melman]
# 
# 	Initial submission.
# 	[1994/07/26  19:23:10  nav]
# 
# Revision 1.1.2.6  1994/09/21  17:53:20  truitt
# 	Change some messages to reflect new schema name parsing.
# 	[1994/09/21  17:39:24  truitt]
# 
# Revision 1.1.2.5  1994/09/21  15:26:12  truitt
# 	Update tests to reflect new aclmgr for generic schema.
# 	[1994/09/21  14:58:26  truitt]
# 
# Revision 1.1.2.4  1994/09/07  12:56:17  truitt
# 	Update schema tests to reflect new messages
# 	[1994/09/06  18:54:13  truitt]
# 
# Revision 1.1.2.3  1994/08/17  18:45:00  truitt
# 	Fix comment leader.
# 	[1994/08/17  18:32:20  truitt]
# 
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:46  annie]
# 
# Revision 1.1.2.2  1994/08/10  19:22:28  annie
# 	expand copyright with OSF copyright text
# 
# Revision 1.1.2.1  1994/07/27  18:52:12  nav
# 	Initial submission.
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc schema_rename_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_rename_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc schema_rename_1_N {} {
    # Identify the test
    tet_infoline schema_rename_1_N: The rename command must have an attribute

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema rename} msg] $msg
    } compare_strings {1 {No schema name.}}

    # Cleanup steps
}

proc schema_rename_2_N {} {
    # Identify the test
    tet_infoline schema_rename_2_N: Too many arguments passed to rename

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema rename gumby pokey} msg] $msg
    } compare_strings {1 {Unrecognized argument 'pokey'.}}

    # Cleanup steps
}

proc schema_rename_3_N {} {
    # Identify the test
    tet_infoline schema_rename_3_N: The schema name is not a fully qualified schema name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema rename gumby -to junk} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc schema_rename_4_N {} {
    # Identify the test
    tet_infoline schema_rename_4_N: The schema name is not a fully qualified schema name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema rename /.:/gumby -to junk} msg] $msg
    } compare_strings {1 {Not a fully qualified schema name.}}

    # Cleanup steps
}

proc schema_rename_5_N {} {
    # Identify the test
    tet_infoline schema_rename_5_N: The schema name does not exist in the registry schema

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema rename /.:/sec/xattrschema/schema_rename_5_N -to junk} msg] $msg
    } compare_strings {1 {Schema entry '/.:/sec/xattrschema/schema_rename_5_N' not found.}}

    # Cleanup steps
}

proc schema_rename_6_N {} {
    # Identify the test
    tet_infoline schema_rename_6_N: No new name specified

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema rename /.:/sec/xattrschema/schema_rename_6_N} msg] $msg
    } compare_strings {1 {No new name specified.}}

    # Cleanup steps
}

proc schema_rename_7_N {} {
    # Identify the test
    tet_infoline schema_rename_7_N: No new name specified

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema rename /.:/sec/xattrschema/schema_rename_7_N -to} msg] $msg
    } compare_strings {1 {The '-to' option requires a character string value.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 "
set ic1 "schema_rename_1_N"
set ic2 "schema_rename_2_N"
set ic3 "schema_rename_3_N"
set ic4 "schema_rename_4_N"
set ic5 "schema_rename_5_N"
set ic6 "schema_rename_6_N"
set ic7 "schema_rename_7_N"
set tet_startup schema_rename_startup
set tet_cleanup schema_rename_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
