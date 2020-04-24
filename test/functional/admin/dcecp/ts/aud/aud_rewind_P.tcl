#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: aud_rewind_P.tcl,v $
# Revision 1.1.2.3  1996/03/11  02:10:12  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:07  marty]
#
# Revision 1.1.2.2  1995/12/13  15:24:31  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/02  19:19 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/02  19:16 UTC  truitt
# 	New tests for audit rewind
# 	[1995/12/11  15:51:31  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc aud_rewind_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc aud_rewind_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}


proc aud_rewind_1_P {} {
    # Identify the test
    tet_infoline aud_show_1_P: Rewind the default file. 

    # Setup steps

    # Run the test
    if {[catch {aud rewind} msg] != 0} {
        tet_infoline Unable to rewind default file
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to rewind default file
        tet_result PASS
    }

    # Cleanup steps
}

proc aud_rewind_2_P {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline aud_show_2_P: Rewind a specified file.

    # Setup steps

    # Run the test
    if {[catch {aud rewind /.:/hosts/$DCP_HOSTNAME_ONE/audit-server} msg] != 0} {
        tet_infoline Unable to rewind specified file
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to rewind specified file
        tet_result PASS
    }

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "aud_rewind_1_P"
set ic2 "aud_rewind_2_P"
set tet_startup aud_rewind_startup
set tet_cleanup aud_rewind_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
