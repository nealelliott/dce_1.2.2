#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: uuid_compare_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:18:12  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:46  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:02  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:01  root]
# 
# Revision 1.1.2.2  1994/09/30  15:29:31  kevins
# 	OT 12435 modified tests and test messages.
# 	[1994/09/30  15:27:08  kevins]
# 
# Revision 1.1.2.1  1994/09/28  21:41:14  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:27  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:32  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:27:04  devsrc]
# 
# Revision 1.1.2.1  1994/06/02  18:50:31  kevins
# 	Initial Misc commands test drop
# 	[1994/06/02  18:44:20  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc uuid_compare_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc uuid_compare_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc uuid_compare_1_N {} {
    # Identify the test
    tet_infoline uuid_compare_1_N: To check that extra arguments are handled correctly. 

    # Setup steps

    # Run the test
    test {
        list [catch {uuid compare a b c} msg] $msg
    } compare_strings {1 {Unrecognized argument 'c'.}}

    # Cleanup steps
}

proc uuid_compare_2_N {} {
    # Identify the test
    tet_infoline uuid_compare_2_N: To check that extra arguments are handled correctly. 

    # Setup steps

    # Run the test
    test {
        list [catch {uuid compare a b -c} msg] $msg
    } compare_strings {1 {Unrecognized option '-c'.}}

    # Cleanup steps
}

proc uuid_compare_3_N {} {
    # Identify the test
    tet_infoline uuid_compare_3_N: To check that missing arguments are handled correctly

    # Setup steps

    # Run the test
    test {
        list [catch {uuid compare a} msg] $msg
    } compare_strings {1 {Wrong number of arguments, exactly 2 arguments are required.}}

    # Cleanup steps
}

proc uuid_compare_4_N {} {
    # Identify the test
    tet_infoline uuid_compare_4_N: To check that a bad uuid is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {uuid compare a b} msg] $msg
    } compare_strings {1 {Invalid string UUID}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "uuid_compare_1_N"
set ic2 "uuid_compare_2_N"
set ic3 "uuid_compare_3_N"
set ic4 "uuid_compare_4_N"
set tet_startup uuid_compare_startup
set tet_cleanup uuid_compare_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
