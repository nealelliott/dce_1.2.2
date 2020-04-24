#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: attrlist_getvalues_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:26:36  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:55  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:23  root
# 	Submit
# 	[1995/12/11  14:32:01  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:55  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:51  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:00  melman]
# 
# Revision 1.1.2.1  1994/06/27  22:18:02  coren
# 	Initial version.
# 	[1994/06/27  20:02:23  coren]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc attrlist_getvalues_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc attrlist_getvalues_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc attrlist_getval_1_N {} {
    # Identify the test
    tet_infoline attrlist_getval_1_N: Check handling of no arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist getvalues} msg] $msg
    } compare_strings {1 {No input attribute list.}}

    # Cleanup steps
}

proc attrlist_getval_2_N {} {
    # Identify the test
    tet_infoline attrlist_getval_2_N: Check handling of no options.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist getvalues {{a 1} {b 2}}} msg] $msg
    } compare_strings {1 {No type specified.}}

    # Cleanup steps
}

proc attrlist_getval_3_N {} {
    # Identify the test
    tet_infoline attrlist_getval_3_N: Check handling of invalid option.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist getvalues {{a 1} {b 2}} -wrongoption} msg] $msg
    } compare_strings {1 {Unknown option "-wrongoption".}}

    # Cleanup steps
}

proc attrlist_getval_4_N {} {
    # Identify the test
    tet_infoline attrlist_getval_4_N: Check handling of empty -type option.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist getvalues {{a 1} {b 2}} -type} msg] $msg
    } compare_strings {1 {Option "-type" requires a string argument.}}

    # Cleanup steps
}

proc attrlist_getval_5_N {} {
    # Identify the test
    tet_infoline attrlist_getval_5_N: Check that specification of a list of types
    tet_infoline attrlist_getval_5_N: is treated as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {attrlist getvalues {{a 1} {b 2}} -type {a b}} msg] $msg
    } compare_strings {1 {-type cannot be a list.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "attrlist_getval_1_N"
set ic2 "attrlist_getval_2_N"
set ic3 "attrlist_getval_3_N"
set ic4 "attrlist_getval_4_N"
set ic5 "attrlist_getval_5_N"
set tet_startup attrlist_getvalues_startup
set tet_cleanup attrlist_getvalues_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
