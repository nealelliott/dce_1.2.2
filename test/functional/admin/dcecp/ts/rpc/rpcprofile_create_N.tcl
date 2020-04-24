#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcprofile_create_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:57  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:42  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:54  root
# 	Submit
# 	[1995/12/11  14:31:49  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:38  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:35  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:44  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:56  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:28:59  devsrc]
# 
# Revision 1.1.2.1  1994/05/26  18:42:10  rousseau
# 	Initial submission.
# 	[1994/05/26  18:39:24  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc rpcprofile_create_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcprofile_create_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc rpcprofile_create_1_N {} {
    # Identify the test
    tet_infoline rpcprofile_create_1_N: Check that no supplied arguments is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile create} msg] $msg
    } compare_strings {1 {No RPC entry name given.}}

    # Cleanup steps
}


set iclist "ic1 "
set ic1 "rpcprofile_create_1_N"
set tet_startup rpcprofile_create_startup
set tet_cleanup rpcprofile_create_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
