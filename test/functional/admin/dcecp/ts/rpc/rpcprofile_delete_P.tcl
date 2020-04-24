#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcprofile_delete_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:26:05  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:44  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:00  root
# 	Submit
# 	[1995/12/11  14:31:51  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:41  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:38  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:46  melman]
# 
# Revision 1.1.2.3  1994/09/01  21:13:50  rousseau
# 	Mass test cleanup
# 	[1994/09/01  21:13:17  rousseau]
# 
# Revision 1.1.2.2  1994/06/17  18:43:57  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:29:07  devsrc]
# 
# Revision 1.1.2.1  1994/05/26  18:42:12  rousseau
# 	Initial submission.
# 	[1994/05/26  18:39:26  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc rpcprofile_delete_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcprofile_delete_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc rpcprofile_delete_4_P {} {
    # Identify the test
    tet_infoline rpcprofile_delete_4_P: Verify that we can delete an rpc profile.

    # Setup steps
    if {[dcp_wrap {rpcprofile add /.:/rpcp_del_4_P -member /.:/foo -default}] != 0} {return;}

    # Run the test
    test {
	rpcprofile delete /.:/rpcp_del_4_P
	rpcprofile show /.:/rpcp_del_4_P
    } compare_strings {}

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcp_del_4_P}] != 0} {return;}
}

proc rpcprofile_delete_5_P {} {
    # Identify the test
    tet_infoline rpcprofile_delete_5_P: Verify that we can delete a list of rpc groups.

    # Setup steps
    if {[dcp_wrap {rpcprofile add /.:/rpcp_del_5_P_1 -member /.:/foo -interface [list [uuid create] 1.0]}] != 0} {return;}
    if {[dcp_wrap {rpcprofile add /.:/rpcp_del_5_P_2 -member /.:/bar -interface [list [uuid create] 1.0]}] != 0} {return;}

    # Run the test
    test {
	rpcprofile delete {/.:/rpcp_del_5_P_1 /.:/rpcp_del_5_P_2}
	rpcprofile show {/.:/rpcp_del_5_P_1 /.:/rpcp_del_5_P_2}
    } compare_lists {}

    # Cleanup steps
    if {[dcp_wrap {object delete {/.:/rpcp_del_5_P_1 /.:/rpcp_del_5_P_2}}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "rpcprofile_delete_4_P"
set ic2 "rpcprofile_delete_5_P"
set tet_startup rpcprofile_delete_startup
set tet_cleanup rpcprofile_delete_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

