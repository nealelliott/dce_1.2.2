#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: link_show_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:14:20  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:13  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:38  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:46  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:11  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:17  melman]
# 
# Revision 1.1.2.5  1994/09/02  18:42:13  kevins
# 	OT 10617 fixed error messages
# 	[1994/09/02  18:29:22  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:59  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:44  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:15  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:19:14  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:28  melman
# 	updating to new API path
# 	[1994/05/06  15:20:21  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:14  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:53  kevins]
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
proc link_show_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_show_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {link show /.:/link_1  extra} msg] $msg
    } compare_strings {1 {Unrecognized argument 'extra'.}}

    # Cleanup steps
}

proc link_show_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_show_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {link show } msg] $msg
    } compare_strings {1 {No link name.}}

    # Cleanup steps
}


set iclist "ic1 ic2"
set ic1 "link_show_1_N"
set ic2 "link_show_2_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
