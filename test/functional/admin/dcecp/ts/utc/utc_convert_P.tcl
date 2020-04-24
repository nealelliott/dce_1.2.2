#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: utc_convert_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:29:03  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:05  marty]
#
# Revision 1.1.4.1  1995/12/11  15:17:29  root
# 	Submit
# 	[1995/12/11  14:32:49  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:58  root]
# 
# Revision 1.1.2.1  1994/09/28  21:47:18  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:15:03  melman]
# 
# Revision 1.1.2.3  1994/09/01  21:39:03  rousseau
# 	Mass test fixup.
# 	[1994/09/01  21:38:41  rousseau]
# 
# Revision 1.1.2.2  1994/08/10  19:22:40  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:17:00  annie]
# 
# Revision 1.1.2.1  1994/07/06  15:22:32  rousseau
# 	Initial utc tests.
# 	[1994/07/06  15:21:57  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc utc_convert_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc utc_convert_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc utc_convert_5_P {} {
    # Identify the test
    tet_infoline utc_convert_5_P: Verify that we can convert a timestamp into local time.

    # Setup steps

    # Run the test
    test {
	utc convert 1994-07-01-17:51:56.101-09:00I90.232
    } compare_strings {1994-07-01-22:51:56.101-04:00I90.232}

    # Cleanup steps
}

proc utc_convert_6_P {} {
    # Identify the test
    tet_infoline utc_convert_6_P: Verify that we can convert a timestamp into GMT.

    # Setup steps

    # Run the test
    test {
	utc convert 1994-07-01-17:51:56.101+09:00I90.232 -gmt
    } compare_strings {1994-07-01-08:51:56.101I90.232}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "utc_convert_5_P"
set ic2 "utc_convert_6_P"
set tet_startup utc_convert_startup
set tet_cleanup utc_convert_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
