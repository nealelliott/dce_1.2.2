#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: clearinghouse_create_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:12:50  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:58  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:09  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/06/09  13:14 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts34/1  1995/06/09  13:13 UTC  truitt
# 	CHFts15424: Cleanup clearinghouse create tests.
# 
# 	HP revision /main/HPDCE02/1  1994/11/15  14:23 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/15  14:21 UTC  truitt
# 	Do not create clearinghouses when not necessary.
# 	[1995/12/11  15:52:22  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:33  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:17  melman]
# 
# Revision 1.1.2.5  1994/09/06  13:14:35  kevins
# 	OT 10617 modified messages in negative tests
# 	[1994/09/06  12:30:39  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:03:28  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:06  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:37  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:17:29  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:20:59  melman
# 	updating to new API path
# 	[1994/05/06  15:19:42  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:20:18  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:23  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc clearinghouse_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc clearinghouse_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc clearinghouse_create_4_P {} {
    # Identify the test
    tet_infoline clearinghouse_create_4_P: Check that we can successfully create a clearinghouse 

    # Setup steps

    # Run the test
    test {clearinghouse create /.:/clear_create4_ch
        exec sleep 10
        directory list /.: -object -simplename
    } dcp_find_pgo clear_create4_ch

    # Cleanup steps
    if {[dcp_wrap {clearinghouse delete /.:/clear_create4_ch}] != 0} {return;}
}

proc clearinghouse_create_5_P {} {
    # Identify the test
    tet_infoline clearinghouse_create_5_P: Check that we can successfully create
    tet_infoline clearinghouse_create_5_P: a list of clearinghouses

    # Setup steps

    # Run the test
    test {clearinghouse create {/.:/clear_create5a_ch /.:/clear_create5b_ch} 
        exec sleep 10
        dcp_found [directory list /.: -object -simple] {clear_create5a_ch clear_create5b_ch} 
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {clearinghouse delete {/.:/clear_create5a_ch /.:/clear_create5b_ch}}] != 0} {return;}
}

set iclist "ic1 ic2 "
set ic1 "clearinghouse_create_4_P"
set ic2 "clearinghouse_create_5_P"
set tet_startup clearinghouse_startup
set tet_cleanup clearinghouse_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
