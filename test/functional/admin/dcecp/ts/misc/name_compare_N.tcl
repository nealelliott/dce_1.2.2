#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: name_compare_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:54  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:38  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:54  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:55  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:57  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:17  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:28  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:26:40  devsrc]
# 
# Revision 1.1.2.1  1994/06/02  18:50:11  kevins
# 	Initial Misc commands test drop
# 	[1994/06/02  18:43:47  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc name_compare_1_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline name_compare_1_N: Check handling of extra/invalid options.

    # Setup steps

    # Run the test
    test {
        list [catch {name compare foo foo foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc name_compare_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline name_compare_2_N: Check handling of extra/invalid options.

    # Setup steps

    # Run the test
    test {
        list [catch {name compare foo foo -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc name_compare_3_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline name_compare_3_N: Check handling of missing arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {name compare} msg] $msg
    } compare_strings {1 {Wrong number of arguments, exactly 2 arguments are required.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "name_compare_1_N"
set ic2 "name_compare_2_N"
set ic3 "name_compare_3_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
