#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: endpoint_show_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:24:41  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:13  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:57  root
# 	Submit
# 	[1995/12/11  14:31:25  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:07  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:51  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:13  melman]
# 
# Revision 1.1.2.5  1994/09/09  20:50:06  coren
# 	Added test for incorrect use of -uuid (CR 12022).
# 	[1994/09/09  20:07:35  coren]
# 
# Revision 1.1.2.4  1994/09/01  21:13:43  rousseau
# 	Mass test cleanup
# 	[1994/09/01  21:13:10  rousseau]
# 
# Revision 1.1.2.3  1994/08/18  21:16:20  coren
# 	Fixed to remove incorrect test and modify expected output resulting
# 	from added ability to take an argument (CR 11573).
# 	[1994/08/18  20:01:01  coren]
# 
# Revision 1.1.2.2  1994/08/10  19:22:35  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:54  annie]
# 
# Revision 1.1.2.1  1994/06/20  19:52:12  coren
# 	Initial version.
# 	[1994/06/16  17:09:17  coren]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc endpoint_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc endpoint_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc endpoint_show_1_N {} {
    # Identify the test
    tet_infoline endpoint_show_1_N: Check that uuid with any other option is trapped as an error.

    # Setup steps
    if {[dcp_wrap {global INTERFACE1}] != 0} {return;}

    # Run the test
    test {
        list [catch {endpoint show -uuid -interface $INTERFACE1} msg] $msg
    } compare_strings {1 {The '-uuid' option cannot be combined with other options.}}

    # Cleanup steps
}

proc endpoint_show_2_N {} {
    # Identify the test
    tet_infoline endpoint_show_2_N: Check that any unrecognized option is trapped as an error.

    # Setup steps
    if {[dcp_wrap {global INTERFACE1}] != 0} {return;}

    # Run the test
    test {
        list [catch {endpoint show -foo -interface $INTERFACE1} msg] $msg
    } compare_strings {1 {The value '-foo' is not a valid endpoint map binding.}}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "endpoint_show_1_N"
set ic2 "endpoint_show_2_N"
set tet_startup endpoint_show_startup
set tet_cleanup endpoint_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
