#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: utc_subtract_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:29:15  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:11  marty]
#
# Revision 1.1.4.1  1995/12/11  15:17:38  root
# 	Submit
# 	[1995/12/11  14:32:52  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:18:03  root]
# 
# Revision 1.1.2.1  1994/09/28  21:47:22  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:15:07  melman]
# 
# Revision 1.1.2.2  1994/08/10  19:22:44  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:17:03  annie]
# 
# Revision 1.1.2.1  1994/07/06  15:22:39  rousseau
# 	Initial utc tests.
# 	[1994/07/06  15:22:03  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc utc_subtract_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc utc_subtract_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc utc_subtract_7_P {} {
    # Identify the test
    tet_infoline utc_subtract_7_P: Verify that we can subtract two absolute timestamps.

    # Setup steps

    # Run the test
    test {
	utc subtract 1994-07-01-17:51:56.101+00:00I90.232 1994-07-01-17:51:56.101+00:00I90.232
    } compare_strings {+0-00:00:00.000I180.464}

    # Cleanup steps
}

proc utc_subtract_8_P {} {
    # Identify the test
    tet_infoline utc_subtract_8_P: Verify that we can subtract two realtive timestamps.

    # Setup steps

    # Run the test
    test {
	utc subtract +0-00:00:00.375I----- +0-00:02:00.000I-----
    } compare_strings {-0-00:01:59.625I-----}

    # Cleanup steps
}

proc utc_subtract_9_P {} {
    # Identify the test
    tet_infoline utc_subtract_9_P: Verify that we can subtract a realtive and an absolute timestamp.

    # Setup steps

    # Run the test
    test {
	utc subtract 1994-07-01-17:51:56.101+00:00I90.232 +0-00:00:00.375I-----
    } compare_strings {1994-07-01-17:51:55.726+00:00I-----}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "utc_subtract_7_P"
set ic2 "utc_subtract_8_P"
set ic3 "utc_subtract_9_P"
set tet_startup utc_subtract_startup
set tet_cleanup utc_subtract_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
