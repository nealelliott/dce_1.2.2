#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcgroup_list_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:45  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:38  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:41  root
# 	Submit
# 	[1995/12/11  14:31:45  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:32  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:28  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:38  melman]
# 
# Revision 1.1.2.4  1994/09/19  19:55:34  rousseau
# 	Fix test variable usage.
# 	[1994/09/19  19:54:57  rousseau]
# 
# Revision 1.1.2.3  1994/09/19  17:57:25  rousseau
# 	Fix hardcoded cell name (CR 12242).
# 	[1994/09/19  17:55:31  rousseau]
# 
# Revision 1.1.2.2  1994/06/17  18:43:51  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:28:38  devsrc]
# 
# Revision 1.1.2.1  1994/05/31  21:40:30  rousseau
# 	Initial submission.
# 	[1994/05/31  21:39:34  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc rpcgroup_list_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcgroup_list_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc rpcgroup_list_4_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcgroup_list_4_P: Verify that we can list the members in an rpc group.

    # Setup steps
    if {[dcp_wrap {rpcgroup add /.:/rpcg_list_4_P -member /.:/foo}] != 0} {return;}

    # Run the test
    test {
        rpcgroup list /.:/rpcg_list_4_P
    } compare_lists "$DCP_CELLNAME_ONE/foo"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcg_list_4_P}] != 0} {return;}
}

proc rpcgroup_list_5_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcgroup_list_5_P: Verify that we can list the members in a list of rpc groups.

    # Setup steps
    if {[dcp_wrap {rpcgroup add {/.:/rpcg_list_5_P_1 /.:/rpcg_list_5_P_2} -member {/.:/foo /.:/bar}}] != 0} {return;}

    # Run the test
    test {
        rpcgroup list {/.:/rpcg_list_5_P_1 /.:/rpcg_list_5_P_2}
    } compare_lists "$DCP_CELLNAME_ONE/foo $DCP_CELLNAME_ONE/bar $DCP_CELLNAME_ONE/foo $DCP_CELLNAME_ONE/bar"

    # Cleanup steps
    if {[dcp_wrap {object delete {/.:/rpcg_list_5_P_1 /.:/rpcg_list_5_P_2}}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "rpcgroup_list_4_P"
set ic2 "rpcgroup_list_5_P"
set tet_startup rpcgroup_list_startup
set tet_cleanup rpcgroup_list_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
