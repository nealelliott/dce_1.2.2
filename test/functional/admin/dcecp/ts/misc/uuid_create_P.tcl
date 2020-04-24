#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: uuid_create_P.tcl,v $
# Revision 1.1.2.3  1996/03/11  02:18:20  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:49  marty]
#
# Revision 1.1.2.2  1995/12/13  15:27:05  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/03  14:41 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/03  14:39 UTC  truitt
# 	New uuid tests.
# 	[1995/12/11  15:54:04  root]
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
proc uuid_create_1_P {} {
    # Identify the test
    tet_infoline uuid_create_1_P: Make sure the uuid create command works correctly.

    # Setup steps
    if {[dcp_wrap {set good_uuid [uuid create]}] != 0} {return;}

    # Run the test
    if {[catch {uuid compare $good_uuid $good_uuid} msg] != 0} {
        tet_infoline Unable to create a valid uuid
        tet_infoline $msg
        tet_result FAIL
    } else {
        tet_infoline Able to create a valid uuid
        tet_result PASS
    }

    # Cleanup steps
}

set iclist "ic1 "
set ic1 "uuid_create_1_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
