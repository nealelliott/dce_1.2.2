#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: clock_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:05  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:49  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:51  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/02/27  17:27 UTC  truitt
# 	Merge clock test work.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts16/1  1995/02/27  17:25 UTC  truitt
# 	CHFts14470: The hostname will be looked up if not provided
# 	when doing a show with the -inetd or -dced options.
# 
# 	HP revision /main/HPDCE02/1  1994/12/01  15:40 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/12/01  15:39 UTC  truitt
# 	Add tests for new clock functionality.
# 	[1995/12/11  15:52:57  root]
# 
# Revision 1.1.2.6  1994/09/28  21:39:31  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:39  melman]
# 
# Revision 1.1.2.5  1994/06/10  20:12:31  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:57  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:23:54  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:23:00  rousseau]
# 
# Revision 1.1.2.3  1994/05/11  19:36:59  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:20:05  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:35:48  melman
# 	updating to new API path
# 	[1994/05/06  15:35:15  melman]
# 
# Revision 1.1.2.1  1994/04/21  13:57:39  rousseau
# 	Initial drop of dts and clock tests (CR 9938).
# 	[1994/04/21  13:55:50  rousseau]
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
proc clock_show_1_P {} {
    # Identify the test
    tet_infoline clock_show_1_P: Check that we can show the value of the system clock using dtsd.

    # Setup steps

    # Run the test
    if {[catch {clock show} msg] != 0} {
	tet_infoline Unable to show the time using dtsd.
	tet_result FAIL
    } else {
	tet_infoline Successfully retrieved the time using dtsd.
	tet_result PASS
    }
}

proc clock_show_2_P {} {
    # Identify the test
    tet_infoline clock_show_2_P: Check that we can show the value of the system clock using dtsd.

    # Setup steps

    # Run the test
    if {[catch {clock show /.:/hosts/[exec hostname]/dts-entity} msg] != 0} {
	tet_infoline Unable to show the time using a hostname and dtsd.
	tet_result FAIL
    } else {
	tet_infoline Successfully retrieved the time using a hostname and dtsd.
	tet_result PASS
    }
}

proc clock_show_3_P {} {
    # Identify the test
    tet_infoline clock_show_3_P: Show the value of the system clock using inetd.

    # Setup steps

    # Run the test
    if {[catch {clock show -inetd} msg] != 0} {
	tet_infoline Unable to show the time using inetd.
	tet_result FAIL
    } else {
	tet_infoline Successfully retrieved the time using inetd.
	tet_result PASS
    }
}

proc clock_show_4_P {} {
    global DCP_HOSTNAME_WITH_RIGHT_INETD
    # Identify the test
    tet_infoline clock_show_4_P: Show the value of the system clock using inetd.

    # Setup steps

    # Run the test
    if {[catch {clock show -inetd $DCP_HOSTNAME_WITH_RIGHT_INETD} msg] != 0} {
	tet_infoline Unable to show the time using inetd.
	tet_result FAIL
    } else {
	tet_infoline Successfully retrieved the time using inetd.
	tet_result PASS
    }
}

proc clock_show_5_P {} {
    # Identify the test
    tet_infoline clock_show_5_P: Show the value of the system clock using dced.

    # Setup steps

    # Run the test
    if {[catch {clock show -dced} msg] != 0} {
	tet_infoline Unable to show the time using dced.
	tet_result FAIL
    } else {
	tet_infoline Successfully retrieved the time using dced.
	tet_result PASS
    }
}

proc clock_show_6_P {} {
    global DCP_HOSTNAME_WITH_RIGHT_DCED
    # Identify the test
    tet_infoline clock_show_6_P: Show the value of the system clock using dced.

    # Setup steps

    # Run the test
    if {[catch {clock show -dced $DCP_HOSTNAME_WITH_RIGHT_DCED} msg] != 0} {
	tet_infoline Unable to show the time using dced.
	tet_result FAIL
    } else {
	tet_infoline Successfully retrieved the time using dced.
	tet_result PASS
    }
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "clock_show_1_P"
set ic2 "clock_show_2_P"
set ic3 "clock_show_3_P"
set ic4 "clock_show_4_P"
set ic5 "clock_show_5_P"
set ic6 "clock_show_6_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
