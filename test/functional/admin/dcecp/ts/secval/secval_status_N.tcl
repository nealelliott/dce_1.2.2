#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: secval_status_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:27:24  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:16  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:00  root
# 	Submit
# 	[1995/12/11  14:32:17  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:15  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:14  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:21  melman]
# 
# Revision 1.1.2.1  1994/09/01  20:24:40  pwang
# 	Initial implementation
# 	[1994/09/01  20:19:28  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup and common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc secval_status_1_N {} {

    # Identify the test
    tet_infoline secval_status_1_N: check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {secval status -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc secval_status_2_N {} {

    # Identify the test
    tet_infoline secval_status_2_N: check that a wrong host are returned as errors. 

    # Setup steps

    # Run the test
    test {
        list [catch {secval status foo} msg] $msg
    } compare_strings {1 {Bad or wrong arguments}}

    # Cleanup steps
}

set iclist "ic1 ic2 "
set ic1 "secval_status_1_N"
set ic2 "secval_status_2_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
