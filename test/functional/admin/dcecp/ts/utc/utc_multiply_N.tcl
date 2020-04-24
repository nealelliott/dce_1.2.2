#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: utc_multiply_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:29:08  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:06  marty]
#
# Revision 1.1.4.1  1995/12/11  15:17:31  root
# 	Submit
# 	[1995/12/11  14:32:50  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:18:00  root]
# 
# Revision 1.1.2.1  1994/09/28  21:47:19  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:15:04  melman]
# 
# Revision 1.1.2.3  1994/09/01  21:39:05  rousseau
# 	Mass test fixup.
# 	[1994/09/01  21:38:45  rousseau]
# 
# Revision 1.1.2.2  1994/08/10  19:22:41  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:17:01  annie]
# 
# Revision 1.1.2.1  1994/07/06  15:22:33  rousseau
# 	Initial utc tests.
# 	[1994/07/06  15:22:01  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc utc_multiply_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc utc_multiply_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc utc_multiply_1_N {} {
    # Identify the test
    tet_infoline utc_multiply_1_N: Check handling of no arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {utc multiply} msg] $msg
    } compare_strings {1 {Wrong number of arguments, exactly 2 arguments are required.}}

    # Cleanup steps
}

proc utc_multiply_2_N {} {
    # Identify the test
    tet_infoline utc_multiply_2_N: Check handling of only one argument.

    # Setup steps

    # Run the test
    test {
        list [catch {utc multiply +0-00:10:00.000I-----} msg] $msg
    } compare_strings {1 {Wrong number of arguments, exactly 2 arguments are required.}}

    # Cleanup steps
}

proc utc_multiply_3_N {} {
    # Identify the test
    tet_infoline utc_multiply_3_N: Check handling of garbage timestamps.

    # Setup steps

    # Run the test
    test {
        list [catch {utc multiply foo bar} msg] $msg
    } compare_strings {1 {The value 'foo' is not a valid relative timestamp.}}

    # Cleanup steps
}

proc utc_multiply_4_N {} {
    # Identify the test
    tet_infoline utc_multiply_4_N: Check handling of garbage multipliers.

    # Setup steps

    # Run the test
    test {
        list [catch {utc multiply +0-00:10:00.000I----- foo} msg] $msg
    } compare_strings {1 {The value 'foo' is not a valid integer or floating point number.}}

    # Cleanup steps
}

proc utc_multiply_5_N {} {
    # Identify the test
    tet_infoline utc_multiply_5_N: Check handling of extra arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {utc multiply +0-00:10:00.000I----- 2 foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc utc_multiply_6_N {} {
    # Identify the test
    tet_infoline utc_multiply_6_N: Check handling of extra options.

    # Setup steps

    # Run the test
    test {
        list [catch {utc multiply +0-00:10:00.000I----- 2 -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc utc_multiply_7_N {} {
    # Identify the test
    tet_infoline utc_multiply_7_N: Check handling of absolute timestamps.

    # Setup steps

    # Run the test
    test {
        list [catch {utc multiply 1994-07-01-17:51:56.101+00:00I90.232 2} msg] $msg
    } compare_strings {1 {The value '1994-07-01-17:51:56.101+00:00I90.232' is not a valid relative timestamp.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 "
set ic1 "utc_multiply_1_N"
set ic2 "utc_multiply_2_N"
set ic3 "utc_multiply_3_N"
set ic4 "utc_multiply_4_N"
set ic5 "utc_multiply_5_N"
set ic6 "utc_multiply_6_N"
set ic7 "utc_multiply_7_N"
set tet_startup utc_multiply_startup
set tet_cleanup utc_multiply_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
