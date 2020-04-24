#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_designate_N.tcl,v $
# Revision 1.1.2.2  1996/03/09  20:47:25  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:18  marty]
#
# Revision 1.1.2.1  1995/12/11  15:12:44  root
# 	Submit
# 	[1995/12/11  14:30:43  root]
# 
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/09/11  15:56 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts46/1  1995/09/11  15:55 UTC  truitt
# 	CHFts16100: Additional tests to support new functionality.
# 
# 	HP revision /main/HPDCE02/2  1995/06/30  13:59 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts37/1  1995/06/30  13:59 UTC  truitt
# 	CHFts14292: Change tests to reflect new behavior of dced acls.
# 
# 	HP revision /main/HPDCE02/1  1995/03/30  15:31 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts23/1  1995/03/30  15:30 UTC  truitt
# 	New test file.
# 	[1995/12/08  23:15:15  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_designate_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_designate_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_designate_001_N {} {
    # Identify the test
    tet_infoline registry_designate_001_N: Check for an insufficient number of arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {registry designate} msg] $msg
    } compare_strings {1 {Registry replica not specified.}}

    # Cleanup steps
}

proc registry_designate_002_N {} {
    # Identify the test
    tet_infoline registry_designate_002_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry designate {foo foo2}} msg] $msg
    } compare_strings {1 {Only one registry replica is allowed.}}

    # Cleanup steps
}

proc registry_designate_003_N {} {
    # Identify the test
    tet_infoline registry_designate_003_N: Check for a bogus replica.

    # Setup steps

    # Run the test
    test {
        list [catch {registry designate foo} msg] $msg
    } compare_strings {1 {Registry server 'foo' is unavailable.}}

    # Cleanup steps
}

proc registry_designate_004_N {} {
    # Identify the test
    tet_infoline registry_designate_004_N: Check for a replica specified as local cell.

    # Setup steps

    # Run the test
    test {
        list [catch {registry designate /.:} msg] $msg
    } compare_strings {1 {Registry server '/.:' is unavailable.}}

    # Cleanup steps
}

proc registry_designate_005_N {} {
    # Identify the test
    tet_infoline registry_designate_005_N: Check for a replica specified as cell name.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry designate $DCP_CELLNAME_ONE} msg] $msg
    } compare_strings "1 {Registry server '$DCP_CELLNAME_ONE' is unavailable.}"

    # Cleanup steps
}

proc registry_designate_006_N {} {
    # Identify the test
    tet_infoline registry_designate_006_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry designate foo foo2} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo2'.}}

    # Cleanup steps
}

proc registry_designate_007_N {} {
    # Identify the test
    tet_infoline registry_designate_007_N: Check for bogus option specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry designate foo -} msg] $msg
    } compare_strings {1 {Ambiguous argument '-': -force -master -slave}}

    # Cleanup steps
}

proc registry_designate_008_N {} {
    # Identify the test
    tet_infoline registry_designate_008_N: Check for bogus option specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry designate foo -foo2} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo2'.}}

    # Cleanup steps
}

proc registry_designate_009_N {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_NOTROOT DCP_NOTROOT_PW

    # Identify the test
    tet_infoline registry_designate_009_N: Check for authentication to perform this operation.

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
        list [catch {registry designate $DCP_RGY_SA_REP_NAME_SLAVE1} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {account delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_NOTROOT}] != 0} {return;}
}

proc registry_designate_010_N {} {
    # Identify the test
    tet_infoline registry_designate_010_N: Check that all options are mutually exclusive
    tet_infoline registry_designate_010_N: for this command.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry designate $DCP_RGY_SA_REP_NAME_SLAVE1 -master -slave} msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}

proc registry_designate_011_N {} {
    # Identify the test
    tet_infoline registry_designate_011_N: Check that all options are mutually exclusive
    tet_infoline registry_designate_011_N: for this command.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry designate $DCP_RGY_SA_REP_NAME_SLAVE1 -force} msg] $msg
    } compare_strings {1 {The -force option is valid only with the -master option.}}

    # Cleanup steps
}

proc registry_designate_012_N {} {
    # Identify the test
    tet_infoline registry_designate_012_N: Check that the specified replica is already a 
    tet_infoline registry_designate_012_N: master replica.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry designate $DCP_RGY_SA_REP_NAME_MASTER} msg] $msg
    } compare_strings {1 {Registry replica specified is the master.}}

    # Cleanup steps
}

proc registry_designate_013_N {} {
    # Identify the test
    tet_infoline registry_designate_013_N: Check that the specified replica is already a 
    tet_infoline registry_designate_013_N: slave replica.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry designate $DCP_RGY_SA_REP_NAME_SLAVE1 -slave} msg] $msg
    } compare_strings {1 {Replica is not the master}}

    # Cleanup steps
}

proc registry_designate_014_N {} {
    # Identify the test
    tet_infoline registry_designate_014_N: Check that the specified replica is already a 
    tet_infoline registry_designate_014_N: master replica using the -master option.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry designate $DCP_RGY_SA_REP_NAME_MASTER -master} msg] $msg
    } compare_strings {1 {Replica is the master}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 "
set ic1 "registry_designate_001_N"
set ic2 "registry_designate_002_N"
set ic3 "registry_designate_003_N"
set ic4 "registry_designate_004_N"
set ic5 "registry_designate_005_N"
set ic6 "registry_designate_006_N"
set ic7 "registry_designate_007_N"
set ic8 "registry_designate_008_N"
set ic9 "registry_designate_009_N"
set ic10 "registry_designate_010_N"
set ic11 "registry_designate_011_N"
set ic12 "registry_designate_012_N"
set ic13 "registry_designate_013_N"
set ic14 "registry_designate_014_N"
set tet_startup registry_designate_startup
set tet_cleanup registry_designate_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
