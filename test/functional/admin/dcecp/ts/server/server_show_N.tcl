#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_show_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:28:28  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:47  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:55  root
# 	Submit
# 	[1995/12/11  14:32:36  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:40  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:52  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:45  melman]
# 
# Revision 1.1.2.3  1994/09/20  18:23:08  pwang
# 	Updated to latest changes [OT#10262]
# 	[1994/09/20  18:16:01  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:28  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:31:05  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:40:13  pwang
# 	Initial implementation
# 	[1994/06/10  21:33:06  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc server_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc server_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc server_show_1_N {} {
    # Identify the test
    tet_infoline server_show_1_N: Check that insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server show} msg] $msg
    } compare_strings {1 {'dced' object not specified.}}

    # Cleanup steps
}

proc server_show_2_N {} {
    # Identify the test
    tet_infoline server_show_2_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server show tserver -executing foo} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc server_show_3_N {} {
    # Identify the test
    tet_infoline server_show_3_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server show tserver -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc server_show_4_N {} {
    # Identify the test
    tet_infoline server_show_4_N: Check that wrong object name are returned as not found. 

    # Setup steps

    # Run the test
    test {
        list [catch {server show foo} msg] $msg
    } compare_strings {1 {Cannot find a matching object.}}

    # Cleanup steps
}

proc server_show_5_N {} {
    # Identify the test
    tet_infoline server_show_5_N: Check that a non-running server name with -executing switch is returned as not found. 

    # Setup steps
    catch {server stop tserver -method hard} donotcare

    # Run the test
    test {
        list [catch {server show tserver -executing} msg] $msg
    } compare_strings {1 {Cannot find a matching object.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "server_show_1_N"
set ic2 "server_show_2_N"
set ic3 "server_show_3_N"
set ic4 "server_show_4_N"
set ic5 "server_show_5_N"
set tet_startup server_show_startup
set tet_cleanup server_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
