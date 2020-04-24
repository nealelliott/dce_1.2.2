#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: utc_convert_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:29:01  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:02  marty]
#
# Revision 1.1.4.1  1995/12/11  15:17:28  root
# 	Submit
# 	[1995/12/11  14:32:49  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:57  root]
# 
# Revision 1.1.2.1  1994/09/28  21:47:16  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:15:03  melman]
# 
# Revision 1.1.2.2  1994/08/10  19:22:40  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:59  annie]
# 
# Revision 1.1.2.1  1994/07/06  15:22:30  rousseau
# 	Initial utc tests.
# 	[1994/07/06  15:21:56  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc utc_convert_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc utc_convert_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc utc_convert_1_N {} {
    # Identify the test
    tet_infoline utc_convert_1_N: Check handling of no arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {utc convert} msg] $msg
    } compare_strings {1 {The command requires at least one argument.}}

    # Cleanup steps
}

proc utc_convert_2_N {} {
    # Identify the test
    tet_infoline utc_convert_2_N: Check handling of an invalid argument.

    # Setup steps

    # Run the test
    test {
        list [catch {utc convert foo} msg] $msg
    } compare_strings {1 {The value 'foo' is not a valid absolute or relative timestamp.}}

    # Cleanup steps
}

proc utc_convert_3_N {} {
    # Identify the test
    tet_infoline utc_convert_3_N: Check handling of an extra argument.

    # Setup steps

    # Run the test
    test {
        list [catch {utc convert 1994-07-01-17:51:56.101+00:00I90.232 foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc utc_convert_4_N {} {
    # Identify the test
    tet_infoline utc_convert_4_N: Check handling of an extra option.

    # Setup steps

    # Run the test
    test {
        list [catch {utc convert 1994-07-01-17:51:56.101+00:00I90.232 -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "utc_convert_1_N"
set ic2 "utc_convert_2_N"
set ic3 "utc_convert_3_N"
set ic4 "utc_convert_4_N"
set tet_startup utc_convert_startup
set tet_cleanup utc_convert_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
