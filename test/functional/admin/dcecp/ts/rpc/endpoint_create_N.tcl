#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: endpoint_create_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:24:30  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:09  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:48  root
# 	Submit
# 	[1995/12/11  14:31:22  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:02  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:44  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:06  melman]
# 
# Revision 1.1.2.3  1994/08/18  21:16:16  coren
# 	Fixed error messages to match improvements in dcp.sams.
# 	[1994/08/18  20:00:22  coren]
# 
# Revision 1.1.2.2  1994/08/10  19:22:31  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:49  annie]
# 
# Revision 1.1.2.1  1994/06/20  19:52:05  coren
# 	Initial version.
# 	[1994/06/16  17:09:03  coren]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc endpoint_create_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc endpoint_create_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc endpoint_create_1_N {} {
    # Identify the test
    tet_infoline endpoint_create_1_N: Check that any non-option argument is trapped as an error.

    # Setup steps
    if {[dcp_wrap {global INTERFACE1 BINDING1}] != 0} {return;}

    # Run the test
    test {
        list [catch {endpoint create foo -interface $INTERFACE1 -binding $BINDING1} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc endpoint_create_2_N {} {
    # Identify the test
    tet_infoline endpoint_create_2_N: Check that any unrecognized option is trapped as an error.

    # Setup steps
    if {[dcp_wrap {global INTERFACE1 BINDING1}] != 0} {return;}

    # Run the test
    test {
        list [catch {endpoint create -foo -interface $INTERFACE1 -binding $BINDING1} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc endpoint_create_3_N {} {
    # Identify the test
    tet_infoline endpoint_create_3_N: Check that -interface option is required.

    # Setup steps
    if {[dcp_wrap {global INTERFACE1 BINDING1}] != 0} {return;}

    # Run the test
    test {
        list [catch {endpoint create -binding $BINDING1} msg] $msg
    } compare_strings {1 {Interface id not entered.}}

    # Cleanup steps
}

proc endpoint_create_4_N {} {
    # Identify the test
    tet_infoline endpoint_create_4_N: Check that -binding option is required.

    # Setup steps
    if {[dcp_wrap {global INTERFACE1 BINDING1}] != 0} {return;}

    # Run the test
    test {
        list [catch "endpoint create -interface [list $INTERFACE1]" msg] $msg
    } compare_strings {1 {No string binding entered.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "endpoint_create_1_N"
set ic2 "endpoint_create_2_N"
set ic3 "endpoint_create_3_N"
set ic4 "endpoint_create_4_N"
set tet_startup endpoint_create_startup
set tet_cleanup endpoint_create_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
