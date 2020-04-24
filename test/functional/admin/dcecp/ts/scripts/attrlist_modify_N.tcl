#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: attrlist_modify_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:26:48  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:02  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:29  root
# 	Submit
# 	[1995/12/11  14:32:05  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:01  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:55  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:05  melman]
# 
# Revision 1.1.2.1  1994/06/27  22:18:10  coren
# 	Corrected error messages in tests 3 and 4.
# 	[1994/06/27  20:28:07  coren]
# 
# 	Initial version.
# 	[1994/06/27  20:02:34  coren]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc attrlist_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc attrlist_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc attrlist_modify_1_N {} {
    # Identify the test
    tet_infoline attrlist_modify_1_N: Check handling of no arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist modify} msg] $msg
    } compare_strings {1 {No input attribute list.}}

    # Cleanup steps
}

proc attrlist_modify_2_N {} {
    # Identify the test
    tet_infoline attrlist_modify_2_N: Check handling of no options.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist modify {{a 1} {b 2}}} msg] $msg
    } compare_strings {1 {No options specified.}}

    # Cleanup steps
}

proc attrlist_modify_3_N {} {
    # Identify the test
    tet_infoline attrlist_modify_3_N: Check that attempt to add an existing value is treated as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist modify {{a 1 2} {b 2}} -add {{a 1}}} msg] $msg
    } compare_strings {1 {Value "1" already present for attribute "a".}}

    # Cleanup steps
}

proc attrlist_modify_4_N {} {
    # Identify the test
    tet_infoline attrlist_modify_4_N: Check that attempt to remove a nonexistent value is treated
    tet_infoline attrlist_modify_4_N: as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist modify {{a 1 2} {b 2}} -remove {{a 3}}} msg] $msg
    } compare_strings {1 {Value "3" not found for attribute "a".}}

    # Cleanup steps
}

proc attrlist_modify_5_N {} {
    # Identify the test
    tet_infoline attrlist_modify_5_N: Check that attempt to remove a nonexistent attribute is treated
    tet_infoline attrlist_modify_5_N: as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist modify {{a 1 2} {b 2}} -remove {c}} msg] $msg
    } compare_strings {1 {Attribute "c" not found in list.}}

    # Cleanup steps
}

proc attrlist_modify_6_N {} {
    # Identify the test
    tet_infoline attrlist_modify_6_N: Check that attempt to change a nonexistent attribute is treated
    tet_infoline attrlist_modify_6_N: as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist modify {{a 1 2} {b 2}} -change {{a 3} {c 1}}} msg] $msg
    } compare_strings {1 {Attribute "c" not present in list.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "attrlist_modify_1_N"
set ic2 "attrlist_modify_2_N"
set ic3 "attrlist_modify_3_N"
set ic4 "attrlist_modify_4_N"
set ic5 "attrlist_modify_5_N"
set ic6 "attrlist_modify_6_N"
set tet_startup attrlist_modify_startup
set tet_cleanup attrlist_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
