#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: clock_set_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:00  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:45  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:49  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/12/01  15:32 UTC  truitt
# 	Initial HPDCE02 branch
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/12/01  15:30 UTC  truitt
# 	Add tests for new clock functionality.
# 	[1995/12/11  15:52:55  root]
# 
# Revision 1.1.2.6  1994/09/28  21:39:29  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:35  melman]
# 
# Revision 1.1.2.5  1994/06/10  20:12:30  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:55  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:23:52  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:22:56  rousseau]
# 
# Revision 1.1.2.3  1994/05/11  19:36:56  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:19:58  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:35:46  melman
# 	updating to new API path
# 	[1994/05/06  15:35:12  melman]
# 
# Revision 1.1.2.1  1994/04/21  13:57:37  rousseau
# 	Initial drop of dts and clock tests (CR 9938).
# 	[1994/04/21  13:55:48  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc clock_set_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc clock_set_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc clock_set_1_P {} {
    # Identify the test
    tet_infoline clock_set_1_P: Check to see that we can set the clock using a hostname and dtsd.

    # Setup steps

    # Run the test
    if {[catch {clock set /.:/hosts/[exec hostname]/dts-entity -to [clock show -inetd [exec hostname]]} msg] != 0} {
        tet_infoline Unable to set clock to specified time using a hostname and dtsd
        tet_result FAIL
    } else {
        tet_infoline Successfully set the system clock using a hostname and dtsd
        tet_result PASS
    }

    # Cleanup steps
    while {[dcp_get_element [dts show] status] != "enabled"} {
        exec sleep 2
    }
}

proc clock_set_2_P {} {
    # Identify the test
    tet_infoline clock_set_2_P: Check to see that we can set the clock using dtsd.

    # Setup steps

    # Run the test
    if {[catch {clock set -to [clock show -inetd [exec hostname]]} msg] != 0} {
        tet_infoline Unable to set clock to specified time using dtsd
        tet_result FAIL
    } else {
        tet_infoline Successfully set the system clock using dtsd
        tet_result PASS
    }

    # Cleanup steps
    while {[dcp_get_element [dts show] status] != "enabled"} {
        exec sleep 2
    }
}

proc clock_set_3_P {} {
    # Identify the test
    tet_infoline clock_set_3_P: Check handling of -abruptly option using a hostname and dtsd.

    # Setup steps

    # Run the test
    if {[catch {clock set -epoch 101 -abruptly /.:/hosts/[exec hostname]/dts-entity -to [clock show -inetd [exec hostname]]} msg] != 0} {
        tet_infoline Unable to abruptly set clock to specified time using a hostname and dtsd
        tet_result FAIL
    } else {
        tet_infoline Successfully set the system clock abruptly using a hostname and dtsd
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {clock set -epoch 0}] != 0} {return;}
    while {[dcp_get_element [dts show] status] != "enabled"} {
        exec sleep 2
    }
}

proc clock_set_4_P {} {
    # Identify the test
    tet_infoline clock_set_4_P: Check handling of -abruptly option using dtsd.

    # Setup steps

    # Run the test
    if {[catch {clock set -epoch 101 -abruptly -to [clock show -inetd [exec hostname]]} msg] != 0} {
        tet_infoline Unable to abruptly set clock to specified time using dtsd
        tet_result FAIL
    } else {
        tet_infoline Successfully set the system clock abruptly using dtsd
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {clock set -epoch 0}] != 0} {return;}
    while {[dcp_get_element [dts show] status] != "enabled"} {
        exec sleep 2
    }
}

proc clock_set_5_P {} {
    # Identify the test
    tet_infoline clock_set_5_P: Check that we can change the epoch without changing the time.

    # Setup steps

    # Run the test
    test {clock set -epoch 101
        dcp_attrlist getval [dts show] epoch
    } compare_strings 101

    # Cleanup steps
    if {[dcp_wrap {clock set -epoch 0}] != 0} {return;}
    while {[dcp_get_element [dts show] status] != "enabled"} {
        exec sleep 2
    }
}

proc clock_set_6_P {} {
    # Identify the test
    tet_infoline clock_set_6_P: Check to see that we can set the clock without using dtsd.

    # Setup steps

    # Run the test
    if {[catch {clock set -bypass -to [clock show -inetd [exec hostname]]} msg] != 0} {
        tet_infoline Unable to set clock to specified time without using dtsd
        tet_result FAIL
    } else {
        tet_infoline Successfully set the system clock without using dtsd
        tet_result PASS
    }

    # Cleanup steps
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "clock_set_1_P"
set ic2 "clock_set_2_P"
set ic3 "clock_set_3_P"
set ic4 "clock_set_4_P"
set ic5 "clock_set_5_P"
set ic6 "clock_set_6_P"
set tet_startup clock_set_startup
set tet_cleanup clock_set_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
