#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: utc_subtract_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:29:13  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:09  marty]
#
# Revision 1.1.4.1  1995/12/11  15:17:36  root
# 	Submit
# 	[1995/12/11  14:32:51  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:18:02  root]
# 
# Revision 1.1.2.1  1994/09/28  21:47:21  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:15:06  melman]
# 
# Revision 1.1.2.2  1994/08/10  19:22:43  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:17:02  annie]
# 
# Revision 1.1.2.1  1994/07/06  15:22:37  rousseau
# 	Initial utc tests.
# 	[1994/07/06  15:22:03  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc utc_subtract_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc utc_subtract_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc utc_subtract_1_N {} {
    # Identify the test
    tet_infoline utc_subtract_1_N: Check handling of no arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {utc subtract} msg] $msg
    } compare_strings {1 {Wrong number of arguments, exactly 2 arguments are required.}}

    # Cleanup steps
}

proc utc_subtract_2_N {} {
    # Identify the test
    tet_infoline utc_subtract_2_N: Check handling of only one argument.

    # Setup steps

    # Run the test
    test {
        list [catch {utc subtract 1994-10-30-06:00:00.000+00:00I0.000} msg] $msg
    } compare_strings {1 {Wrong number of arguments, exactly 2 arguments are required.}}

    # Cleanup steps
}

proc utc_subtract_3_N {} {
    # Identify the test
    tet_infoline utc_subtract_3_N: Check handling of garbage arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {utc subtract foo bar} msg] $msg
    } compare_strings {1 {The value 'foo' is not a valid absolute or relative timestamp.}}

    # Cleanup steps
}

proc utc_subtract_4_N {} {
    # Identify the test
    tet_infoline utc_subtract_4_N: Check handling of extra arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {utc subtract 1994-07-01-17:51:56.101+00:00I90.232 +0-00:02:00.000I----- foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc utc_subtract_5_N {} {
    # Identify the test
    tet_infoline utc_subtract_5_N: Check handling of extra options.

    # Setup steps

    # Run the test
    test {
        list [catch {utc subtract 1994-07-01-17:51:56.101+00:00I90.232 +0-00:02:00.000I----- -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc utc_subtract_6_N {} {
    # Identify the test
    tet_infoline utc_subtract_6_N: Check handling of relative - absolute operation.

    # Setup steps

    # Run the test
    test {
        list [catch {utc subtract +0-00:02:00.000I----- 1994-07-01-17:51:56.101+00:00I90.232} msg] $msg
    } compare_strings {1 {Subtracting an absolute timestamp from a relative timestamp is not allowed.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "utc_subtract_1_N"
set ic2 "utc_subtract_2_N"
set ic3 "utc_subtract_3_N"
set ic4 "utc_subtract_4_N"
set ic5 "utc_subtract_5_N"
set ic6 "utc_subtract_6_N"
set tet_startup utc_subtract_startup
set tet_cleanup utc_subtract_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
