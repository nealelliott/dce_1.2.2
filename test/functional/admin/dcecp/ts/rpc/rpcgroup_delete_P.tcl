#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcgroup_delete_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:34  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:35  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:34  root
# 	Submit
# 	[1995/12/11  14:31:42  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:29  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:22  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:34  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:48  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:28:26  devsrc]
# 
# Revision 1.1.2.1  1994/05/31  21:40:18  rousseau
# 	Initial submission.
# 	[1994/05/31  21:39:29  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc rpcgroup_delete_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcgroup_delete_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc rpcgroup_delete_4_P {} {
    # Identify the test
    tet_infoline rpcgroup_delete_4_P: Verify that we can delete an rpc group.

    # Setup steps
    if {[dcp_wrap {rpcgroup add /.:/rpcg_del_4_P -member /.:/foo}] != 0} {return;}

    # Run the test
    test {
        rpcgroup delete /.:/rpcg_del_4_P
	rpcgroup list /.:/rpcg_del_4_P
    } compare_lists {}

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcg_del_4_P}] != 0} {return;}
}

proc rpcgroup_delete_5_P {} {
    # Identify the test
    tet_infoline rpcgroup_delete_5_P: Verify that we can delete a list of rpc groups.

    # Setup steps
    if {[dcp_wrap {rpcgroup add {/.:/rpcg_del_5_P_1 /.:/rpcg_del_5_P_2} -member /.:/foo}] != 0} {return;}

    # Run the test
    test {
        rpcgroup delete {/.:/rpcg_del_5_P_1 /.:/rpcg_del_5_P_2}
	rpcgroup list {/.:/rpcg_del_5_P_1 /.:/rpcg_del_5_P_2}
    } compare_lists {}

    # Cleanup steps
    if {[dcp_wrap {object delete {/.:/rpcg_del_5_P_1 /.:/rpcg_del_5_P_2}}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "rpcgroup_delete_4_P"
set ic2 "rpcgroup_delete_5_P"
set tet_startup rpcgroup_delete_startup
set tet_cleanup rpcgroup_delete_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
