#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: errtext_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:40  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:33  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:48  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:50  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:51  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:07  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:22  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:26:14  devsrc]
# 
# Revision 1.1.2.1  1994/06/02  18:49:51  kevins
# 	Initial Misc commands test drop
# 	[1994/06/02  18:43:13  kevins]
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
proc errtext_1_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline errtext_1_N: Check handling of no arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {errtext} msg] $msg
    } compare_strings {1 {The command requires at least one argument.}}

    # Cleanup steps
}

proc errtext_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline errtext_2_N: Check handling of extra/invalid arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {errtext 0x1712207a foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc errtext_3_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline errtext_3_N: Check handling of extra/invalid options.

    # Setup steps

    # Run the test
    test {
        list [catch {errtext 0x1712207a -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "errtext_1_N"
set ic2 "errtext_2_N"
set ic3 "errtext_3_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
