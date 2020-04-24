#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: hostdata_delete_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:17  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:53  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:16  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:21  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:02  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:19  melman]
# 
# Revision 1.1.2.4  1994/09/20  15:47:29  rousseau
# 	Update tests (CR 12254).
# 	[1994/09/20  15:32:58  rousseau]
# 
# Revision 1.1.2.3  1994/06/17  18:43:16  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:25:43  devsrc]
# 
# Revision 1.1.2.2  1994/05/25  16:51:07  baum
# 	Hostdata tests now dce_login on their own
# 	[1994/05/25  16:49:27  baum]
# 
# Revision 1.1.2.1  1994/05/19  22:58:20  baum
# 	Initial checkin
# 	[1994/05/19  22:53:42  baum]
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
proc hostdata_delete_1_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline hostdata_delete_1_N: Check that insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata delete} msg] $msg
    } compare_strings {1 {'dced' object not specified.}}

    # Cleanup steps
}

proc hostdata_delete_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline hostdata_delete_2_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata delete name -entry foo} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc hostdata_delete_3_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline hostdata_delete_3_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata delete name -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc hostdata_delete_4_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline hostdata_delete_4_N: Check that wrong object name are returned as not found. 

    # Setup steps
    dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}

    # Run the test
    test {
        list [catch {hostdata delete foo} msg] $msg
    } compare_strings {1 {Cannot find a matching object.}}

   
    # Cleanup steps
    dcp_wrap {logout}
}

proc hostdata_delete_5_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline hostdata_delete_5_N: Check that unauthenticated users are handled correctly.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata delete name} msg] $msg
    } compare_strings {1 {Cannot find a matching object.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "hostdata_delete_1_N"
set ic2 "hostdata_delete_2_N"
set ic3 "hostdata_delete_3_N"
set ic4 "hostdata_delete_4_N"
set ic5 "hostdata_delete_5_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
