#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: utc_compare_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:28:56  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:59  marty]
#
# Revision 1.1.4.1  1995/12/11  15:17:24  root
# 	Submit
# 	[1995/12/11  14:32:47  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:54  root]
# 
# Revision 1.1.2.1  1994/09/28  21:47:13  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:59  melman]
# 
# Revision 1.1.2.2  1994/08/10  19:22:38  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:57  annie]
# 
# Revision 1.1.2.1  1994/07/06  15:22:27  rousseau
# 	Initial utc tests.
# 	[1994/07/06  15:21:54  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc utc_compare_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc utc_compare_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc utc_compare_1_N {} {
    # Identify the test
    tet_infoline utc_compare_1_N: Check handling of no arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {utc compare} msg] $msg
    } compare_strings {1 {Wrong number of arguments, exactly 2 arguments are required.}}

    # Cleanup steps
}

proc utc_compare_2_N {} {
    # Identify the test
    tet_infoline utc_compare_2_N: Check handling of only one argument.

    # Setup steps

    # Run the test
    test {
        list [catch {utc compare 1994-10-30-06:00:00.000+00:00I0.000} msg] $msg
    } compare_strings {1 {Wrong number of arguments, exactly 2 arguments are required.}}

    # Cleanup steps
}

proc utc_compare_3_N {} {
    # Identify the test
    tet_infoline utc_compare_3_N: Check handling of garbage arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {utc compare foo bar} msg] $msg
    } compare_strings {1 {The value 'foo' is not a valid absolute or relative timestamp.}}

    # Cleanup steps
}

proc utc_compare_4_N {} {
    # Identify the test
    tet_infoline utc_compare_4_N: Check handling of extra arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {utc compare 1994-07-01-17:51:56.101+00:00I90.232 +0-00:02:00.000I----- foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc utc_compare_5_N {} {
    # Identify the test
    tet_infoline utc_compare_5_N: Check handling of extra options.

    # Setup steps

    # Run the test
    test {
        list [catch {utc compare 1994-07-01-17:51:56.101+00:00I90.232 +0-00:02:00.000I----- -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc utc_compare_6_N {} {
    # Identify the test
    tet_infoline utc_compare_6_N: Check handling of comparing dissimilar timestamps (rel abs).

    # Setup steps

    # Run the test
    test {
        list [catch {utc compare 1994-07-01-17:51:56.101+00:00I90.232 +0-00:02:00.000I-----} msg] $msg
    } compare_strings {1 {Comparison of a relative and an absolute timestamp is not allowed.}}

    # Cleanup steps
}

proc utc_compare_7_N {} {
    # Identify the test
    tet_infoline utc_compare_7_N: Check handling of comparing dissimilar timestamps (abs rel).

    # Setup steps

    # Run the test
    test {
        list [catch {utc compare +0-00:02:00.000I----- 1994-07-01-17:51:56.101+00:00I90.232} msg] $msg
    } compare_strings {1 {Comparison of a relative and an absolute timestamp is not allowed.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 "
set ic1 "utc_compare_1_N"
set ic2 "utc_compare_2_N"
set ic3 "utc_compare_3_N"
set ic4 "utc_compare_4_N"
set ic5 "utc_compare_5_N"
set ic6 "utc_compare_6_N"
set ic7 "utc_compare_7_N"
set tet_startup utc_compare_startup
set tet_cleanup utc_compare_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
