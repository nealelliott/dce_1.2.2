#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcgroup_add_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:23  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:31  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:27  root
# 	Submit
# 	[1995/12/11  14:31:39  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:24  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:16  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:31  melman]
# 
# Revision 1.1.2.4  1994/09/19  19:55:29  rousseau
# 	Fix test variable usage.
# 	[1994/09/19  19:54:55  rousseau]
# 
# Revision 1.1.2.3  1994/09/19  17:57:24  rousseau
# 	Fix hardcoded cell name (CR 12242).
# 	[1994/09/19  17:55:28  rousseau]
# 
# Revision 1.1.2.2  1994/06/17  18:43:46  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:28:14  devsrc]
# 
# Revision 1.1.2.1  1994/05/31  21:40:06  rousseau
# 	Initial submission.
# 	[1994/05/31  21:39:26  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc rpcgroup_add_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcgroup_add_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc rpcgroup_add_6_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcgroup_add_6_P: Verify that we can add a member to a group.

    # Setup steps

    # Run the test
    test {
        rpcgroup add /.:/rpcg_add_6_P -member /.:/foo
	rpcgroup list /.:/rpcg_add_6_P
    } compare_lists "$DCP_CELLNAME_ONE/foo"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcg_add_6_P}] != 0} {return;}
}

proc rpcgroup_add_7_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcgroup_add_7_P: Verify that we can add a list of members to a list of groups.

    # Setup steps

    # Run the test
    test {
        rpcgroup add {/.:/rpcg_add_7_P_1 /.:/rpcg_add_7_P_2} -member {/.:/foo /.:/bar}
	rpcgroup list {/.:/rpcg_add_7_P_1 /.:/rpcg_add_7_P_2}
    } compare_lists "$DCP_CELLNAME_ONE/foo $DCP_CELLNAME_ONE/bar $DCP_CELLNAME_ONE/foo $DCP_CELLNAME_ONE/bar"

    # Cleanup steps
    if {[dcp_wrap {object delete {/.:/rpcg_add_7_P_1 /.:/rpcg_add_7_P_2}}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "rpcgroup_add_6_P"
set ic2 "rpcgroup_add_7_P"
set tet_startup rpcgroup_add_startup
set tet_cleanup rpcgroup_add_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
