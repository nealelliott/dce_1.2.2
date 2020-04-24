#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: hd_cleanup.tcl,v $
# Revision 1.1.2.3  1996/03/11  02:15:54  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:32  marty]
#
# Revision 1.1.2.2  1995/12/13  15:26:08  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/06/06  18:20 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts33/1  1995/06/06  18:20 UTC  truitt
# 	CHFts13959: Change some tests to support new dced acl behavior.
# 
# 	HP revision /main/HPDCE02/1  1995/03/03  15:10 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/03  15:09 UTC  truitt
# 	New cleanup.
# 	[1995/12/11  15:53:13  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

# Test specific routines
source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_misc.tcl

proc hd_fvt_cleanup {} {
    global TET_ROOT

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    set rcode [catch {exec hd_cleanup.sh $CUR_CELL_NAME $CUR_HOST_NAME} result]
    if {$rcode != 0} {
        tet_infoline Hostdata FVT Cleanup problem
        tet_infoline Command failed: exec hd_cleanup.sh $CUR_CELL_NAME $CUR_HOST_NAME
        tet_infoline Command returned: $result
        tet_result UNRESOLVED
    } else {
        tet_infoline Hostdata Cleanup passed.
        tet_result PASS
    }
}

set iclist "ic1 "
set ic1 "hd_fvt_cleanup"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
