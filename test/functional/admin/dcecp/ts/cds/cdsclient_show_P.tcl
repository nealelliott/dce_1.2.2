#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: cdsclient_show_P.tcl,v $
# Revision 1.1.2.3  1996/03/11  02:12:45  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:53  marty]
#
# Revision 1.1.2.2  1995/12/13  15:25:07  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/10/10  14:14 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts49/1  1995/10/10  14:12 UTC  truitt
# 	new test
# 	[1995/12/11  15:52:20  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc cdsclient_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc cdsclient_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc cdsclient_show_1_P {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline cdsclient_show_1_P: Show the attributes of the local client.

    # Setup steps

    # Run the test
    if {[catch {cdsclient show /.:/hosts/$DCP_HOSTNAME_ONE/cds-clerk} msg] != 0} {
        tet_infoline Unable to show local clerk
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to show local clerk
        tet_result PASS
    }

    # Cleanup steps
}

proc cdsclient_show_2_P {} {
    global DCP_HOSTNAME_TWO

    # Identify the test
    tet_infoline cdsclient_show_2_P: Show the attributes of the remote client.

    # Setup steps

    # Run the test
    if {[catch {cdsclient show /.:/hosts/$DCP_HOSTNAME_TWO/cds-clerk} msg] != 0} {
        tet_infoline Unable to show remote clerk
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to show remote clerk
        tet_result PASS
    }

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "cdsclient_show_1_P"
set ic2 "cdsclient_show_2_P"
set tet_startup cdsclient_show_startup
set tet_cleanup cdsclient_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
