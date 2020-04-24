#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: cds_disable_P.tcl,v $
# Revision 1.1.2.3  1996/03/11  02:11:49  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:26  marty]
#
# Revision 1.1.2.2  1995/12/13  15:24:53  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/10/10  14:27 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts49/1  1995/10/10  14:25 UTC  truitt
# 	New test.
# 	[1995/12/11  15:52:02  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc cds_disable_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc cds_disable_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc cds_disable_1_P {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline cds_disable_1_P: Disable the local server

    # Setup steps

    # Run the test
    if {[catch {cds disable /.:/hosts/$DCP_HOSTNAME_ONE/cds-server} msg] != 0} {
        tet_infoline Unable to disable local server
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to disable local server
        tet_result PASS
    }

    # Cleanup steps
}

proc cds_disable_2_P {} {
    global DCP_HOSTNAME_TWO

    # Identify the test
    tet_infoline cds_disable_2_N: Disable a remote server

    # Setup steps

    # Run the test
    if {[catch {cds disable /.:/hosts/$DCP_HOSTNAME_TWO/cds-server} msg] != 0} {
        tet_infoline Unable to disable remote server
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to disable remote server
        tet_result PASS
    }

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "cds_disable_1_P"
set ic2 "cds_disable_2_P"
set tet_startup cds_disable_startup
set tet_cleanup cds_disable_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
