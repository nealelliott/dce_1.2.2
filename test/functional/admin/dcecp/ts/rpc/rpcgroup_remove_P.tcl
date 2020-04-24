#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcgroup_remove_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:50  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:39  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:47  root
# 	Submit
# 	[1995/12/11  14:31:46  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:34  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:31  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:41  melman]
# 
# Revision 1.1.2.4  1994/09/19  19:55:36  rousseau
# 	Fix test variable usage.
# 	[1994/09/19  19:55:00  rousseau]
# 
# Revision 1.1.2.3  1994/09/19  17:57:26  rousseau
# 	Fix hardcoded cell name (CR 12242).
# 	[1994/09/19  17:55:32  rousseau]
# 
# Revision 1.1.2.2  1994/06/17  18:43:52  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:28:47  devsrc]
# 
# Revision 1.1.2.1  1994/05/31  21:40:38  rousseau
# 	Initial submission.
# 	[1994/05/31  21:39:36  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc rpcgroup_remove_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcgroup_remove_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc rpcgroup_remove_6_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcgroup_remove_6_P: Verify that we can remove a member from a group.

    # Setup steps
    if {[dcp_wrap {rpcgroup add /.:/rpcg_rem_6_P -member {/.:/foo /.:/bar}}] != 0} {return;}

    # Run the test
    test {
        rpcgroup remove /.:/rpcg_rem_6_P -member /.:/bar
	rpcgroup list /.:/rpcg_rem_6_P
    } compare_lists "$DCP_CELLNAME_ONE/foo"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcg_rem_6_P}] != 0} {return;}
}

proc rpcgroup_remove_7_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcgroup_remove_7_P: Verify that we can remove a list of members from a list of groups.

    # Setup steps
    if {[dcp_wrap {rpcgroup add {/.:/rpcg_rem_7_P_1 /.:/rpcg_rem_7_P_2} -member {/.:/foo /.:/bar /.:/biz /.:/bang}}] != 0} {return;}

    # Run the test
    test {
        rpcgroup remove {/.:/rpcg_rem_7_P_1 /.:/rpcg_rem_7_P_2} -member {/.:/bar /.:/biz}
	rpcgroup list {/.:/rpcg_rem_7_P_1 /.:/rpcg_rem_7_P_1}
    } compare_lists "$DCP_CELLNAME_ONE/foo $DCP_CELLNAME_ONE/bang $DCP_CELLNAME_ONE/foo $DCP_CELLNAME_ONE/bang"

    # Cleanup steps
    if {[dcp_wrap {object delete {/.:/rpcg_rem_7_P_1 /.:/rpcg_rem_7_P_2}}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "rpcgroup_remove_6_P"
set ic2 "rpcgroup_remove_7_P"
set tet_startup rpcgroup_remove_startup
set tet_cleanup rpcgroup_remove_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
