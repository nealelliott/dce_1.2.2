#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: registry_connect_N.tcl,v $
# Revision 1.1.2.2  1996/03/09  20:47:23  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:16  marty]
#
# Revision 1.1.2.1  1995/12/13  15:28:51  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/09/11  16:20 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts46/1  1995/09/11  16:18 UTC  truitt
# 	Initial test file.
# 	[1995/12/11  15:55:30  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_connect_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_connect_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_connect_001_N {} {
    # Identify the test
    tet_infoline registry_connect_001_N: Check that the cell name is required.

    # Setup steps

    # Run the test
    test {
        list [catch {registry connect} msg] $msg
    } compare_strings {1 {No cell name.}}

    # Cleanup steps
}

proc registry_connect_002_N {} {
    global DCP_CELLNAME_TWO

    # Identify the test
    tet_infoline registry_connect_002_N: Check that the users password is required.

    # Setup steps

    # Run the test
    test {
        list [catch {registry connect $DCP_CELLNAME_TWO} msg] $msg
    } compare_strings {1 {The users password must be supplied.}}

    # Cleanup steps
}

proc registry_connect_003_N {} {
    global DCP_CELLNAME_TWO DCP_CLIENT_PW

    # Identify the test
    tet_infoline registry_connect_003_N: Check that the foreign password is required.

    # Setup steps

    # Run the test
    test {
        list [catch {registry connect $DCP_CELLNAME_TWO -mypwd $DCP_CLIENT_PW} msg] $msg
    } compare_strings {1 {The foreign users password must be supplied.}}

    # Cleanup steps
}

proc registry_connect_004_N {} {
    global DCP_CELLNAME_TWO DCP_CLIENT_PW

    # Identify the test
    tet_infoline registry_connect_004_N: Check that the local organization is required.

    # Setup steps

    # Run the test
    test {
        list [catch {registry connect $DCP_CELLNAME_TWO -mypwd $DCP_CLIENT_PW \
                     -facctpwd $DCP_CLIENT_PW} msg] $msg
    } compare_strings {1 {Local organization not specified.}}

    # Cleanup steps
}

proc registry_connect_005_N {} {
    global DCP_CELLNAME_TWO DCP_CLIENT_PW

    # Identify the test
    tet_infoline registry_connect_005_N: Check that the local group is required.

    # Setup steps

    # Run the test
    test {
        list [catch {registry connect $DCP_CELLNAME_TWO -mypwd $DCP_CLIENT_PW \
                     -facctpwd $DCP_CLIENT_PW -org none} msg] $msg
    } compare_strings {1 {Local group not specified.}}

    # Cleanup steps
}

proc registry_connect_006_N {} {
    global DCP_CELLNAME_TWO DCP_CLIENT_PW

    # Identify the test
    tet_infoline registry_connect_006_N: Check that the foreign organization is required.

    # Setup steps

    # Run the test
    test {
        list [catch {registry connect $DCP_CELLNAME_TWO -mypwd $DCP_CLIENT_PW \
                     -facctpwd $DCP_CLIENT_PW -org none -group none} msg] $msg
    } compare_strings {1 {Foreign organization name not specified.}}

    # Cleanup steps
}

proc registry_connect_007_N {} {
    global DCP_CELLNAME_TWO DCP_CLIENT_PW

    # Identify the test
    tet_infoline registry_connect_007_N: Check that the foreign account is required.

    # Setup steps

    # Run the test
    test {
        list [catch {registry connect $DCP_CELLNAME_TWO -mypwd $DCP_CLIENT_PW \
                     -facctpwd $DCP_CLIENT_PW -org none -group none -forg none} msg] $msg
    } compare_strings {1 {Foreign account not specified.}}

    # Cleanup steps
}

proc registry_connect_008_N {} {
    global DCP_CELLNAME_TWO DCP_CLIENT_PW DCP_CLIENT

    # Identify the test
    tet_infoline registry_connect_008_N: Check that the foreign group is required.

    # Setup steps

    # Run the test
    test {
        list [catch {registry connect $DCP_CELLNAME_TWO -mypwd $DCP_CLIENT_PW \
                     -facctpwd $DCP_CLIENT_PW -org none -group none -forg none \
                     -facct $DCP_CLIENT} msg] $msg
    } compare_strings {1 {Foreign group name not specified.}}

    # Cleanup steps
}

proc registry_connect_009_N {} {
    global DCP_CELLNAME_TWO DCP_CLIENT_PW DCP_CLIENT

    # Identify the test
    tet_infoline registry_connect_009_N: Check that the foreign cell is available.

    # Setup steps

    # Run the test
    test {
        list [catch {registry connect $DCP_CELLNAME_TWO -mypwd $DCP_CLIENT_PW \
                     -facctpwd $DCP_CLIENT_PW -org none -group none -forg none \
                     -facct $DCP_CLIENT -fgroup none} msg] $msg
    } compare_strings "1 {Registry server '$DCP_CELLNAME_TWO' is unavailable.}"

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 "
set ic1 "registry_connect_001_N"
set ic2 "registry_connect_002_N"
set ic3 "registry_connect_003_N"
set ic4 "registry_connect_004_N"
set ic5 "registry_connect_005_N"
set ic6 "registry_connect_006_N"
set ic7 "registry_connect_007_N"
set ic8 "registry_connect_008_N"
set ic9 "registry_connect_009_N"
set tet_startup registry_connect_startup
set tet_cleanup registry_connect_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
