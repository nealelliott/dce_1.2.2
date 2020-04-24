#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_show_era_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:22:04  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:20  marty]
#
# Revision 1.1.4.1  1995/12/13  15:28:40  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/02  19:53 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/02  19:52 UTC  truitt
# 	Add test of mutually exclusive options for show.
# 	[1995/12/11  15:55:25  root]
# 
# Revision 1.1.2.2  1994/09/30  16:21:58  truitt
# 	OT12414: Update tests to reflect new show functionality.
# 	[1994/09/30  15:43:05  truitt]
# 
# Revision 1.1.2.1  1994/09/28  21:43:33  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:19  melman]
# 
# Revision 1.1.2.2  1994/09/16  18:25:34  truitt
# 	Update tests to reflect new functionality.
# 	[1994/09/16  18:23:26  truitt]
# 
# Revision 1.1.2.1  1994/08/24  19:08:08  truitt
# 	OT11448: Updated era tests
# 	[1994/08/24  19:02:02  truitt]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc principal_show_era_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc principal_show_era_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc principal_show_era_1_N {} {
    # Identify the test
    tet_infoline principal_show_era_1_N: The show command must have an attribute

    # Setup steps

    # Run the test
    test {
        list [catch {principal show} msg] $msg
    } compare_strings {1 {No principal name.}}

    # Cleanup steps
}

proc principal_show_era_2_N {} {
    # Identify the test
    tet_infoline principal_show_era_2_N: Too many arguments passed to show

    # Setup steps

    # Run the test
    test {
        list [catch {principal show gumby pokey} msg] $msg
    } compare_strings {1 {Unrecognized argument 'pokey'.}}

    # Cleanup steps
}

proc principal_show_era_3_N {} {
    # Identify the test
    tet_infoline principal_show_era_3_N: The principal name does not exist

    # Setup steps

    # Run the test
    test {
        list [catch {principal show gumby} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
}

proc principal_show_era_4_N {} {
    # Identify the test
    tet_infoline principal_show_era_4_N: The principal name does not exist

    # Setup steps

    # Run the test
    test {
        list [catch {principal show gumby -xattrs} msg] $msg
    } compare_strings {1 {registry object not found}}

    # Cleanup steps
}

proc principal_show_era_5_N {} {
    # Identify the test
    tet_infoline principal_show_era_5_N: The -all and -xattrs options cannot be used together.

    # Setup steps

    # Run the test
    test {
        list [catch {principal show gumby -all -xattrs} msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}

set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "principal_show_era_1_N"
set ic2 "principal_show_era_2_N"
set ic3 "principal_show_era_3_N"
set ic4 "principal_show_era_4_N"
set ic5 "principal_show_era_5_N"
set tet_startup principal_show_era_startup
set tet_cleanup principal_show_era_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
