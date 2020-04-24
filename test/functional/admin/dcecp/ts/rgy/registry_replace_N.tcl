#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: registry_replace_N.tcl,v $
# Revision 1.1.2.2  1996/03/09  20:47:27  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:20  marty]
#
# Revision 1.1.2.1  1995/12/11  15:13:00  root
# 	Submit
# 	[1995/12/11  14:30:54  root]
# 
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/09/11  16:15 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts46/1  1995/09/11  16:13 UTC  truitt
# 	Initial test file.
# 	[1995/12/08  23:15:29  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_replace_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_replace_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_replace_001_N {} {
    # Identify the test
    tet_infoline registry_replace_001_N: Check for an insufficient number of arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {registry replace} msg] $msg
    } compare_strings {1 {Registry replica not specified.}}

    # Cleanup steps
}

proc registry_replace_002_N {} {
    # Identify the test
    tet_infoline registry_replace_002_N: Check that only one replica can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry replace {foo foo2}} msg] $msg
    } compare_strings {1 {Only one registry replica is allowed.}}

    # Cleanup steps
}

proc registry_replace_003_N {} {
    # Identify the test
    tet_infoline registry_replace_003_N: Check for a bogus replica.

    # Setup steps

    # Run the test
    test {
        list [catch {registry replace foo} msg] $msg
    } compare_strings {1 {Registry server 'foo' is unavailable.}}

    # Cleanup steps
}

proc registry_replace_004_N {} {
    # Identify the test
    tet_infoline registry_replace_004_N: Check for a replica specified as local cell.

    # Setup steps

    # Run the test
    test {
        list [catch {registry replace /.:} msg] $msg
    } compare_strings {1 {Registry server '/.:' is unavailable.}}

    # Cleanup steps
}

proc registry_replace_005_N {} {
    # Identify the test
    tet_infoline registry_replace_005_N: Check for a replica specified as cell name.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry replace $DCP_CELLNAME_ONE} msg] $msg
    } compare_strings "1 {Registry server '$DCP_CELLNAME_ONE' is unavailable.}"

    # Cleanup steps
}

proc registry_replace_006_N {} {
    # Identify the test
    tet_infoline registry_replace_006_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry replace foo foo2} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo2'.}}

    # Cleanup steps
}

proc registry_replace_007_N {} {
    # Identify the test
    tet_infoline registry_replace_007_N: Check for bogus option specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry replace foo -foo2} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo2'.}}

    # Cleanup steps
}

proc registry_replace_008_N {} {
    # Identify the test
    tet_infoline registry_replace_008_N: Check that the address option has been specified.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry replace $DCP_RGY_SA_REP_NAME_SLAVE1} msg] $msg
    } compare_strings {1 {Required options not entered.}}

    # Cleanup steps
}

proc registry_replace_009_N {} {
    # Identify the test
    tet_infoline registry_replace_009_N: Check that the address option is correct.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry replace $DCP_RGY_SA_REP_NAME_SLAVE1 -address {gumby pokey}} msg] $msg
    } compare_strings {1 {Invalid RPC protocol sequence}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 "
set ic1 "registry_replace_001_N"
set ic2 "registry_replace_002_N"
set ic3 "registry_replace_003_N"
set ic4 "registry_replace_004_N"
set ic5 "registry_replace_005_N"
set ic6 "registry_replace_006_N"
set ic7 "registry_replace_007_N"
set ic8 "registry_replace_008_N"
set ic9 "registry_replace_009_N"
set tet_startup registry_replace_startup
set tet_cleanup registry_replace_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
