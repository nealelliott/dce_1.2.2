#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_destroy_N.tcl,v $
# Revision 1.1.2.2  1996/03/11  02:22:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:30  marty]
#
# Revision 1.1.2.1  1995/12/11  15:12:47  root
# 	Submit
# 	[1995/12/11  14:30:45  root]
# 
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/09/11  15:53 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts46/1  1995/09/11  15:52 UTC  truitt
# 	CHFts16100: Additional tests to support new functionality.
# 
# 	HP revision /main/HPDCE02/2  1995/06/30  13:58 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts37/1  1995/06/30  13:57 UTC  truitt
# 	CHFts14292: Change tests to reflect new behavior of dced acls.
# 
# 	HP revision /main/HPDCE02/1  1995/03/30  15:27 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts23/1  1995/03/30  15:26 UTC  truitt
# 	New test file
# 	[1995/12/08  23:15:17  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_destroy_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_destroy_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_destroy_001_N {} {
    # Identify the test
    tet_infoline registry_destroy_001_N: Check for an insufficient number of arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {registry destroy} msg] $msg
    } compare_strings {1 {Registry replica not specified.}}

    # Cleanup steps
}

proc registry_destroy_002_N {} {
    # Identify the test
    tet_infoline registry_destroy_002_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry destroy {foo foo2}} msg] $msg
    } compare_strings {1 {Only one registry replica is allowed.}}

    # Cleanup steps
}

proc registry_destroy_003_N {} {
    # Identify the test
    tet_infoline registry_destroy_003_N: Check for a bogus replica.

    # Setup steps

    # Run the test
    test {
        list [catch {registry destroy foo} msg] $msg
    } compare_strings {1 {Registry server 'foo' is unavailable.}}

    # Cleanup steps
}

proc registry_destroy_004_N {} {
    # Identify the test
    tet_infoline registry_destroy_004_N: Check for a replica specified as local cell.

    # Setup steps

    # Run the test
    test {
        list [catch {registry destroy /.:} msg] $msg
    } compare_strings {1 {Registry server '/.:' is unavailable.}}

    # Cleanup steps
}

proc registry_destroy_005_N {} {
    # Identify the test
    tet_infoline registry_destroy_005_N: Check for a replica specified as cell name.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry destroy $DCP_CELLNAME_ONE} msg] $msg
    } compare_strings "1 {Registry server '$DCP_CELLNAME_ONE' is unavailable.}"

    # Cleanup steps
}

proc registry_destroy_006_N {} {
    # Identify the test
    tet_infoline registry_destroy_006_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry destroy foo foo2} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo2'.}}

    # Cleanup steps
}

proc registry_destroy_007_N {} {
    # Identify the test
    tet_infoline registry_destroy_007_N: Check for bogus option specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry destroy foo -} msg] $msg
    } compare_strings {1 {Unrecognized option '-'.}}

    # Cleanup steps
}

proc registry_destroy_008_N {} {
    # Identify the test
    tet_infoline registry_destroy_008_N: Check for bogus option specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry destroy foo -foo2} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo2'.}}

    # Cleanup steps
}

proc registry_destroy_009_N {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_NOTROOT DCP_NOTROOT_PW

    # Identify the test
    tet_infoline registry_destroy_009_N: Check for authentication to perform this operation.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {group add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {account create $DCP_NOTROOT -group none -organization none -password $DCP_NOTROOT_PW -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_NOTROOT -p $DCP_NOTROOT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry destroy $DCP_RGY_SA_REP_NAME_SLAVE1} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {account delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_NOTROOT}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 "
set ic1 "registry_destroy_001_N"
set ic2 "registry_destroy_002_N"
set ic3 "registry_destroy_003_N"
set ic4 "registry_destroy_004_N"
set ic5 "registry_destroy_005_N"
set ic6 "registry_destroy_006_N"
set ic7 "registry_destroy_007_N"
set ic8 "registry_destroy_008_N"
set ic9 "registry_destroy_009_N"
set tet_startup registry_destroy_startup
set tet_cleanup registry_destroy_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
