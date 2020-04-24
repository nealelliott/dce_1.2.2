#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: object_show_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:14:46  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:30  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:45  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:52  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:25  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:29  melman]
# 
# Revision 1.1.2.5  1994/09/02  17:49:20  kevins
# 	OT 10617 modified fvt test messages
# 	[1994/09/02  15:13:47  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:12:06  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:52  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:24  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:19:46  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:35  melman
# 	updating to new API path
# 	[1994/05/06  15:20:32  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:25  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:02  kevins]
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
proc object_show_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object_show_1_N: Check that missing arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {object show} msg] $msg
    } compare_strings {1 {No object name.}}

    # Cleanup steps
}



set iclist "ic1"
set ic1 "object_show_1_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
