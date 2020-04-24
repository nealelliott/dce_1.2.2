#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: uuid_create_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:18:18  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:48  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:04  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:03  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:16  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:30  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:34  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:27:12  devsrc]
# 
# Revision 1.1.2.1  1994/06/02  18:50:40  kevins
# 	Initial Misc commands test drop
# 	[1994/06/02  18:44:33  kevins]
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
proc uuid_create_1_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline uuid_create_1_N: To test that extra arguments are handled properly

    # Setup steps

    # Run the test
    test {
        list [catch {uuid create a} msg] $msg
    } compare_strings {1 {Unrecognized argument 'a'.}}

    # Cleanup steps
}

proc uuid_create_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline uuid_create_2_N: To test that extra arguments are handled properly 

    # Setup steps

    # Run the test
    test {
        list [catch {uuid create -a} msg] $msg
    } compare_strings {1 {Unrecognized option '-a'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "uuid_create_1_N"
set ic2 "uuid_create_2_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
