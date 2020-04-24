#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: shell_N.tcl,v $
# Revision 1.1.2.3  1996/03/11  02:18:08  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:44  marty]
#
# Revision 1.1.2.2  1995/12/13  15:27:00  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/03  14:42 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/03  14:41 UTC  truitt
# 	New negative tests.
# 	[1995/12/11  15:53:59  root]
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
proc shell_1_N {} {
    # Identify the test
    tet_infoline shell_1_N: See that it cannot run a bogus command.

    # Setup steps

    # Run the test
    test {
        list [catch {shell gumby} msg] $msg
    } compare_strings {1 {couldn't find "gumby" to execute}}

    # Cleanup steps
}


set iclist "ic1 "
set ic1 "shell_1_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
