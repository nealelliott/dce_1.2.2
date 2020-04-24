#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_checkpoint_P.tcl,v $
# Revision 1.1.2.2  1996/03/11  02:22:21  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:26  marty]
#
# Revision 1.1.2.1  1995/12/13  15:28:49  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/09/11  16:02 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts46/1  1995/09/11  16:01 UTC  truitt
# 	CHFts16100: Additional tests to support new functionality.
# 
# 	HP revision /main/HPDCE02/2  1995/03/09  13:08 UTC  truitt
# 	New checkpoint functionality.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts18/1  1995/03/09  13:04 UTC  truitt
# 	CHFts14531: Add test cases for new checkpoint functionality.
# 
# 	HP revision /main/HPDCE02/1  1995/03/02  19:50 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/02  19:48 UTC  truitt
# 	New registry checkpoint tests.
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

proc registry_checkpoint_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_checkpoint_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_checkpoint_1_P {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_1_P: Issue the command with no options.

    # Setup steps

    # Run the test
    if {[catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER} msg] != 0} {
        tet_infoline Cannot checkpoint with no options.
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Successfully executed a checkpoint with no options.
        tet_result PASS
    }
}

proc registry_checkpoint_2_P {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_2_P: Checkpoint a specific replica.

    # Setup steps

    # Run the test
    if {[catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER} msg] != 0} {
        tet_infoline Cannot checkpoint the specified replica.
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Successfully executed a checkpoint on a specific replica.
        tet_result PASS
    }
}

proc registry_checkpoint_3_P {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_3_P: Issue the command with no the -now option.

    # Setup steps

    # Run the test
    if {[catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER -now} msg] != 0} {
        tet_infoline Cannot checkpoint with the -now option.
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Successfully executed a checkpoint with the -now option.
        tet_result PASS
    }
}

proc registry_checkpoint_4_P {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_4_P: Issue the command with a simple -cpi option.

    # Setup steps

    # Run the test
    if {[catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER -cpi 101} msg] != 0} {
        tet_infoline Cannot checkpoint with a simple -cpi option.
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Successfully executed a checkpoint with a simple -cpi option.
        tet_result PASS
    }
}

proc registry_checkpoint_5_P {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_5_P: Issue the command with a simple -cpi option.

    # Setup steps

    # Run the test
    if {[catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER -cpi 39m} msg] != 0} {
        tet_infoline Cannot checkpoint with a simple -cpi option.
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Successfully executed a checkpoint with a simple -cpi option.
        tet_result PASS
    }
}

proc registry_checkpoint_6_P {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_6_P: Issue the command with a simple -cpi option.

    # Setup steps

    # Run the test
    if {[catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER -cpi 3h} msg] != 0} {
        tet_infoline Cannot checkpoint with a simple -cpi option.
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Successfully executed a checkpoint with a simple -cpi option.
        tet_result PASS
    }
}

proc registry_checkpoint_7_P {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_7_P: Issue the command with the -at option.

    # Setup steps

    # Run the test
    if {[catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER -at 10:01} msg] != 0} {
        tet_infoline Cannot checkpoint with the -at option.
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Successfully executed a checkpoint with the -at option.
        tet_result PASS
    }
}

proc registry_checkpoint_8_P {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_8_P: Issue the command with the -at option.

    # Setup steps

    # Run the test
    if {[catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER -at *:*} msg] != 0} {
        tet_infoline Cannot checkpoint with the -at (wildcard) option.
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Successfully executed a checkpoint with the -at (wildcard) option.
        tet_result PASS
    }
}

proc registry_checkpoint_9_P {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_9_P: Issue the command with combined options.

    # Setup steps

    # Run the test
    if {[catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER -now -at 09:31} msg] != 0} {
        tet_infoline Cannot checkpoint with the -now and -at options.
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Successfully executed a checkpoint with the -now and -at options.
        tet_result PASS
    }
}

proc registry_checkpoint_10_P {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_checkpoint_10_P: Issue the command with combined options.

    # Setup steps

    # Run the test
    if {[catch {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER -now -cpi 2h} msg] != 0} {
        tet_infoline Cannot checkpoint with the -now and -cpi options.
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Successfully executed a checkpoint with the -now and -cpi options.
        tet_result PASS
    }
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 "
set ic1 "registry_checkpoint_1_P"
set ic2 "registry_checkpoint_2_P"
set ic3 "registry_checkpoint_3_P"
set ic4 "registry_checkpoint_4_P"
set ic5 "registry_checkpoint_5_P"
set ic6 "registry_checkpoint_6_P"
set ic7 "registry_checkpoint_7_P"
set ic8 "registry_checkpoint_8_P"
set ic9 "registry_checkpoint_9_P"
set ic10 "registry_checkpoint_10_P"
set tet_startup registry_checkpoint_startup
set tet_cleanup registry_checkpoint_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
