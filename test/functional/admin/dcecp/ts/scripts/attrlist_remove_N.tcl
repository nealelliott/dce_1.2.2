#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: attrlist_remove_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:26:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:05  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:32  root
# 	Submit
# 	[1995/12/11  14:32:06  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:03  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:58  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:07  melman]
# 
# Revision 1.1.2.1  1994/06/27  22:18:13  coren
# 	Initial version.
# 	[1994/06/27  20:02:40  coren]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc attrlist_remove_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc attrlist_remove_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc attrlist_remove_1_N {} {
    # Identify the test
    tet_infoline attrlist_remove_1_N: Check handling of no arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist remove} msg] $msg
    } compare_strings {1 {No input attribute list.}}

    # Cleanup steps
}

proc attrlist_remove_2_N {} {
    # Identify the test
    tet_infoline attrlist_remove_2_N: Check handling of no options.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist remove {{a 1} {b 2}}} msg] $msg
    } compare_strings {1 {No attributes to remove.}}

    # Cleanup steps
}

proc attrlist_remove_3_N {} {
    # Identify the test
    tet_infoline attrlist_remove_3_N: Check that attempt to remove a nonexistent attribute is treated
    tet_infoline attrlist_remove_3_N: as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist remove {{a 1 2} {b 2}} -member {c}} msg] $msg
    } compare_strings {1 {Attribute "c" not found in list.}}

    # Cleanup steps
}

proc attrlist_remove_4_N {} {
    # Identify the test
    tet_infoline attrlist_remove_4_N: Check handling of empty -member option.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist remove {{a 1} {b 2}} -member} msg] $msg
    } compare_strings {1 {Option "-member" requires a string argument.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "attrlist_remove_1_N"
set ic2 "attrlist_remove_2_N"
set ic3 "attrlist_remove_3_N"
set ic4 "attrlist_remove_4_N"
set tet_startup attrlist_remove_startup
set tet_cleanup attrlist_remove_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
