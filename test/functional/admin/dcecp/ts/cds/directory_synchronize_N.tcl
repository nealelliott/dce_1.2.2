#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_synchronize_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:56  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:52  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:31  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:40  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:58  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:03  melman]
# 
# Revision 1.1.2.5  1994/09/02  13:56:53  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:50  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:28  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:32  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:02  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:18:46  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:20  melman
# 	updating to new API path
# 	[1994/05/06  15:20:10  melman]
# 
# Revision 1.1.2.1  1994/04/15  19:24:11  kevins
# 	CR 9938 added directory synchronize
# 	[1994/04/15  15:32:46  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc directory_synchronize_1_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_synchronize_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory synchronize /.: /.:/foo} msg] $msg
    } compare_strings {1 {Unrecognized argument '/.:/foo'.}}

    # Cleanup steps
}

proc directory_synchronize_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_synchronize_2_N: Check that unauthenticated users are handled correctly. 

    # Setup steps

    # Run the test
    test {
        list [catch {directory synchronize /.:} msg] $msg
    } compare_strings {1 {Insufficient rights to perform requested operation}}

    # Cleanup steps
}

proc directory_synchronize_3_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_synchronize_3_N: Check that missing arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory synchronize} msg] $msg
    } compare_strings {1 {No directory name.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "directory_synchronize_1_N"
set ic2 "directory_synchronize_2_N"
set ic3 "directory_synchronize_3_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
