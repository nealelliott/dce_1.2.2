#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: utc_add_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:28:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:57  marty]
#
# Revision 1.1.4.1  1995/12/11  15:17:22  root
# 	Submit
# 	[1995/12/11  14:32:46  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:53  root]
# 
# Revision 1.1.2.1  1994/09/28  21:47:11  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:58  melman]
# 
# Revision 1.1.2.2  1994/08/10  19:22:37  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:56  annie]
# 
# Revision 1.1.2.1  1994/07/08  16:57:04  rousseau
# 	More utc tests.
# 	[1994/07/08  16:56:41  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc utc_add_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc utc_add_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc utc_add_7_P {} {
    # Identify the test
    tet_infoline utc_add_7_P: Verify that we can add two realtive timestamps.

    # Setup steps

    # Run the test
    test {
        utc add +0-00:00:00.375I----- +0-00:02:00.000I-----
    } compare_strings {+0-00:02:00.375I-----}

    # Cleanup steps
}

proc utc_add_8_P {} {
    # Identify the test
    tet_infoline utc_add_8_P: Verify that we can add a realtive and an absolute timestamp.

    # Setup steps

    # Run the test
    test {
        utc add +0-00:00:00.375I----- 1994-07-01-17:51:56.101+00:00I90.232
    } compare_strings {1994-07-01-17:51:56.476+00:00I-----}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "utc_add_7_P"
set ic2 "utc_add_8_P"
set tet_startup utc_add_startup
set tet_cleanup utc_add_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
