#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_checkpoint_N.tcl,v $
# Revision 1.1.2.2  1996/03/11  02:22:18  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:25  marty]
#
# Revision 1.1.2.1  1995/12/13  15:28:47  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/09/11  16:03 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts46/1  1995/09/11  16:02 UTC  truitt
# 	CHFts16100: Additional tests to support new functionality.
# 
# 	HP revision /main/HPDCE02/2  1995/03/09  13:11 UTC  truitt
# 	New checkpoint functionality.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts18/1  1995/03/09  13:09 UTC  truitt
# 	CHFts14531: Add test cases for new checkpoint functionality.
# 
# 	HP revision /main/HPDCE02/1  1995/03/02  19:51 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/02  19:50 UTC  truitt
# 	New registry checkpoint tests.
# 	[1995/12/11  15:55:29  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_checkpoint_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_checkpoint_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_checkpoint_1_N {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_1_N: An invalid argument passed to checkpoint

    # Setup steps

    # Run the test
    test {
        list [catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER gumby} msg] $msg
    } compare_strings {1 {Unrecognized argument 'gumby'.}}

    # Cleanup steps
}

proc registry_checkpoint_2_N {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_2_N: An invalid option passed to checkpoint

    # Setup steps

    # Run the test
    test {
        list [catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER -gumby} msg] $msg
    } compare_strings {1 {Unrecognized option '-gumby'.}}

    # Cleanup steps
}

proc registry_checkpoint_3_N {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_3_N: The -at option requires an argument

    # Setup steps

    # Run the test
    test {
        list [catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER -at} msg] $msg
    } compare_strings {1 {The '-at' option requires a character string value.}}

    # Cleanup steps
}

proc registry_checkpoint_4_N {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_4_N: The -cpi option requires an argument

    # Setup steps

    # Run the test
    test {
        list [catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER -cpi} msg] $msg
    } compare_strings {1 {The '-cpi' option requires a character string value.}}

    # Cleanup steps
}

proc registry_checkpoint_5_N {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_5_N: The -at and -cpi options cannot be used together.

    # Setup steps

    # Run the test
    test {
        list [catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER -at 01:02 -cpi 1} msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}

proc registry_checkpoint_6_N {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_6_N: Pass a bad value to the -cpi option.

    # Setup steps

    # Run the test
    test {
        list [catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER -cpi 1:1:1:1} msg] $msg
    } compare_strings {1 {An invalid registry checkpoint time was specified.}}

    # Cleanup steps
}

proc registry_checkpoint_7_N {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_7_N: Pass a bad value to the -at option.

    # Setup steps

    # Run the test
    test {
        list [catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER -at 10101} msg] $msg
    } compare_strings {1 {The time string syntax for -at is incorrect.}}

    # Cleanup steps
}

proc registry_checkpoint_8_N {} {
    # Identify the test
    tet_infoline registry_checkpoint_8_N: You can only checkpoint one replica at a time.

    # Setup steps

    # Run the test
    test {
        list [catch {registry checkpoint {gumby pokey}} msg] $msg
    } compare_strings {1 {Only one registry replica is allowed.}}

    # Cleanup steps
}

proc registry_checkpoint_9_N {} {
    # Identify the test
    tet_infoline registry_checkpoint_9_N: Checkpoint a bogus replica.

    # Setup steps

    # Run the test
    test {
        list [catch {registry checkpoint gumby} msg] $msg
    } compare_strings {1 {Registry server 'gumby' is unavailable.}}

    # Cleanup steps
}

proc registry_checkpoint_10_N {} {
    # Identify the test
    tet_infoline registry_checkpoint_10_N: A replica name is required.

    # Setup steps

    # Run the test
    test {
        list [catch {registry checkpoint} msg] $msg
    } compare_strings {1 {Registry replica not specified.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 "
set ic1 "registry_checkpoint_1_N"
set ic2 "registry_checkpoint_2_N"
set ic3 "registry_checkpoint_3_N"
set ic4 "registry_checkpoint_4_N"
set ic5 "registry_checkpoint_5_N"
set ic6 "registry_checkpoint_6_N"
set ic7 "registry_checkpoint_7_N"
set ic8 "registry_checkpoint_8_N"
set ic9 "registry_checkpoint_9_N"
set ic10 "registry_checkpoint_10_N"
set tet_startup registry_checkpoint_startup
set tet_cleanup registry_checkpoint_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
