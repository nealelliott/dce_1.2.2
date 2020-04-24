#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: utc_compare_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:28:59  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:00  marty]
#
# Revision 1.1.4.1  1995/12/11  15:17:26  root
# 	Submit
# 	[1995/12/11  14:32:48  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:55  root]
# 
# Revision 1.1.2.1  1994/09/28  21:47:15  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:15:01  melman]
# 
# Revision 1.1.2.3  1994/09/01  21:39:00  rousseau
# 	Mass test fixup.
# 	[1994/09/01  21:38:39  rousseau]
# 
# Revision 1.1.2.2  1994/08/10  19:22:39  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:58  annie]
# 
# Revision 1.1.2.1  1994/07/06  15:22:29  rousseau
# 	Initial utc tests.
# 	[1994/07/06  15:21:55  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc utc_compare_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc utc_compare_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc utc_compare_8_P {} {
    # Identify the test
    tet_infoline utc_compare_8_P: Verify that we can compare two absolute timestamps.

    # Setup steps

    # Run the test
    test {
	utc compare 1994-07-01-17:51:56.101+00:00I90.232 1994-10-30-06:00:00.000+00:00I0.000
    } compare_strings {-1}

    # Cleanup steps
}

proc utc_compare_9_P {} {
    # Identify the test
    tet_infoline utc_compare_9_P: Verify that we can compare two realtive timestamps.

    # Setup steps

    # Run the test
    test {
	utc compare +0-00:00:40.375I00.100 +0-00:00:00.100I00.100
    } compare_strings {1}

    # Cleanup steps
}

proc utc_compare_10_P {} {
    # Identify the test
    tet_infoline utc_compare_10_P: Verify that we can compare two absolute timestamps with 
    tet_infoline utc_compare_10_P: -noinaccuracy.

    # Setup steps

    # Run the test
    test {
	utc compare -noinaccuracy 1994-07-01-17:51:56.101+00:00I90.232 1994-10-30-06:00:00.000+00:00I0.000
    } compare_strings {-1}

    # Cleanup steps
}

proc utc_compare_11_P {} {
    # Identify the test
    tet_infoline utc_compare_11_P: Verify that we can compare two realtive timestamps with 
    tet_infoline utc_compare_11_P: -noinaccuracy.

    # Setup steps

    # Run the test
    test {
	utc compare -noinaccuracy +0-00:00:00.375I90.232 +0-00:00:00.375I90.100
    } compare_strings {0}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "utc_compare_8_P"
set ic2 "utc_compare_9_P"
set ic3 "utc_compare_10_P"
set ic4 "utc_compare_11_P"
set tet_startup utc_compare_startup
set tet_cleanup utc_compare_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
