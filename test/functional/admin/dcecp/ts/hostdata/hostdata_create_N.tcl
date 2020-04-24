#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: hostdata_create_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:12  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:46  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:15  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:19  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:59  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:16  melman]
# 
# Revision 1.1.2.5  1994/09/20  15:47:27  rousseau
# 	Update tests (CR 12254).
# 	[1994/09/20  15:32:57  rousseau]
# 
# Revision 1.1.2.4  1994/09/01  15:44:01  baum
# 	Fixed output messages in negative tests (OT 11093)
# 	[1994/09/01  15:42:33  baum]
# 
# Revision 1.1.2.3  1994/06/17  18:43:14  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:25:34  devsrc]
# 
# Revision 1.1.2.2  1994/05/25  16:51:06  baum
# 	Hostdata tests now dce_login on their own
# 	[1994/05/25  16:49:26  baum]
# 
# Revision 1.1.2.1  1994/05/19  22:58:49  baum
# 	Initial checkin
# 	[1994/05/19  22:53:11  baum]
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
proc hostdata_create_1_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline hostdata_create_1_N: Check that insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata create} msg] $msg
    } compare_strings {1 {'dced' object not specified.}}

    # Cleanup steps
}

proc hostdata_create_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline hostdata_create_2_N: Check that insufficient/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata create name} msg] $msg
    } compare_strings {1 {Required option 'attribute or storage' not entered.}}

    # Cleanup steps
}

proc hostdata_create_3_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline hostdata_create_3_N: Check that insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata create name -storage} msg] $msg
    } compare_strings {1 {The '-storage' option requires a character string value.}}

    # Cleanup steps
}

proc hostdata_create_4_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline hostdata_create_4_N: Check that insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata create name -storage filename -annotation} msg] $msg
    } compare_strings {1 {The '-annotation' option requires a character string value.}}

    # Cleanup steps
}

proc hostdata_create_5_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline hostdata_create_5_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {[hostdata create name -storage filename \
-annotation annotation  -entry foo]} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc hostdata_create_6_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline hostdata_create_6_N: Check that insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {[hostdata create name -storage filename \
-annotation annotation -data]} msg] $msg
    } compare_strings {1 {The '-data' option requires a character string value.}}

    # Cleanup steps
}

proc hostdata_create_7_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline hostdata_create_7_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {[hostdata create name -storage filename \
-annotation annotation  -foo]} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc hostdata_create_8_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline hostdata_create_8_N: Check that unauthenticated users are handled correctly.

    # Setup steps

    # Run the test
    test {
        list [catch {[hostdata create name -storage filename \
-annotation annotation -entry]} msg] $msg
    } compare_strings {1 {Failed to find file on dced import attempt}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 "
set ic1 "hostdata_create_1_N"
set ic2 "hostdata_create_2_N"
set ic3 "hostdata_create_3_N"
set ic4 "hostdata_create_4_N"
set ic5 "hostdata_create_5_N"
set ic6 "hostdata_create_6_N"
set ic7 "hostdata_create_7_N"
set ic8 "hostdata_create_8_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
