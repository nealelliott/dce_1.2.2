#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: clock_synchronize_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:11  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:53  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:53  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/12/01  15:45 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/12/01  15:44 UTC  truitt
# 	Add tests for new clock functionality.
# 	[1995/12/11  15:52:58  root]
# 
# Revision 1.1.2.7  1994/09/28  21:39:33  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:42  melman]
# 
# Revision 1.1.2.6  1994/09/21  19:35:28  rousseau
# 	Test cleanup.
# 	[1994/09/21  19:35:19  rousseau]
# 
# Revision 1.1.2.5  1994/06/10  20:12:33  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:06  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:23:55  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:23:02  rousseau]
# 
# Revision 1.1.2.3  1994/05/11  19:37:02  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:20:14  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:35:49  melman
# 	updating to new API path
# 	[1994/05/06  15:35:17  melman]
# 
# Revision 1.1.2.1  1994/04/21  13:57:41  rousseau
# 	Initial drop of dts and clock tests (CR 9938).
# 	[1994/04/21  13:55:52  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc clock_synchronize_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc clock_synchronize_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc clock_synchronize_1_P {} {
    # Identify the test
    tet_infoline clock_synchronize_1_P: Check to see that we can synchronize a DTS entity 
    tet_infoline clock_synchronize_1_P: with other DTS servers.

    # Setup steps

    # Run the test
    test {
        while {[dcp_get_element [dts show] status] != "enabled"} {
            exec sleep 2
        }
        clock synchronize
    } compare_strings {}

    # Cleanup steps
}

proc clock_synchronize_2_P {} {
    # Identify the test
    tet_infoline clock_synchronize_2_P: Check to see that we can synchronize a DTS entity 
    tet_infoline clock_synchronize_2_P: with a specific DTS server.

    # Setup steps

    # Run the test
    test {
        while {[dcp_get_element [dts show] status] != "enabled"} {
            exec sleep 2
        }
        clock synchronize /.:/hosts/[exec hostname]/dts-entity
    } compare_strings {}

    # Cleanup steps
}

proc clock_synchronize_3_P {} {
    global DCP_HOSTNAME_WITH_RIGHT_INETD
    # Identify the test
    tet_infoline clock_synchronize_3_P: Synchronize the clock on the local machine using inetd.

    # Setup steps
    while {[dcp_get_element [dts show] status] != "enabled"} {
        exec sleep 2
    }

    # Run the test
    if {[catch {clock synchronize -inetd $DCP_HOSTNAME_WITH_RIGHT_INETD} msg] != 0} {
        tet_infoline Unable to synchronize the clock using inetd
        tet_result FAIL
    } else {
        tet_infoline Successfully able to synchronize the clock using inetd
        tet_result PASS
    }
}

proc clock_synchronize_4_P {} {
    global DCP_HOSTNAME_WITH_RIGHT_DCED
    # Identify the test
    tet_infoline clock_synchronize_4_P: Synchronize the clock on the local machine using dced.

    # Setup steps

    # Run the test
    if {[catch {clock synchronize -dced $DCP_HOSTNAME_WITH_RIGHT_DCED} msg] != 0} {
        tet_infoline Unable to synchronize the clock using dced
        tet_result FAIL
    } else {
        tet_infoline Successfully able to synchronize the clock using dced
        tet_result PASS
    }
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "clock_synchronize_1_P"
set ic2 "clock_synchronize_2_P"
set ic3 "clock_synchronize_3_P"
set ic4 "clock_synchronize_4_P"
set tet_startup clock_synchronize_startup
set tet_cleanup clock_synchronize_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
