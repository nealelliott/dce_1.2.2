#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: utc_multiply_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:29:10  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:08  marty]
#
# Revision 1.1.4.1  1995/12/11  15:17:33  root
# 	Submit
# 	[1995/12/11  14:32:51  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:18:01  root]
# 
# Revision 1.1.2.1  1994/09/28  21:47:20  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:15:05  melman]
# 
# Revision 1.1.2.2  1994/08/10  19:22:42  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:17:02  annie]
# 
# Revision 1.1.2.1  1994/07/06  15:22:34  rousseau
# 	Initial utc tests.
# 	[1994/07/06  15:22:02  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc utc_multiply_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc utc_multiply_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc utc_multiply_8_P {} {
    # Identify the test
    tet_infoline utc_multiply_8_P: Verify that we can multiply a realtive timestamp.

    # Setup steps

    # Run the test
    test {
	utc multiply +0-00:00:00.375I----- 2
    } compare_strings {+0-00:00:00.750I-----}

    # Cleanup steps
}


set iclist "ic1 "
set ic1 "utc_multiply_8_P"
set tet_startup utc_multiply_startup
set tet_cleanup utc_multiply_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
