#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: clearinghouse_disable_P.tcl,v $
# Revision 1.1.6.4  1996/03/11  02:13:04  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:08  marty]
#
# Revision 1.1.6.3  1995/12/13  15:25:12  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/06/09  13:08 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts34/1  1995/06/09  13:07 UTC  truitt
# 	CHFts15424: Cleanup clearinghouse tests.
# 
# 	HP revision /main/HPDCE02/1  1994/11/15  15:18 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/15  15:17 UTC  truitt
# 	Fix tests.
# 	[1995/12/11  15:52:25  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:37  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:26  melman]
# 
# Revision 1.1.2.5  1994/09/06  13:14:38  kevins
# 	OT 10617 modified messages in negative tests
# 	[1994/09/06  12:30:52  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:03:32  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:15  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:43  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:17:44  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:05  melman
# 	updating to new API path
# 	[1994/05/06  15:19:48  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:20:29  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:28  kevins]
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
proc clearinghouse_disable_4_P {} {
    # Identify the test
    tet_infoline clearinghouse_disable_4_P: Check that we can disable a cds clearinghouse

    # Setup steps
    if {[dcp_wrap {clearinghouse create /.:/clear_disable4_ch }] != 0} {return;}

    # Run the test
    clearinghouse disable /.:/clear_disable4_ch
    test {
        list [catch {clearinghouse delete /.:/clear_disable4_ch} msg] $msg
    } compare_strings {1 {Specified clearinghouse does not exist}}

    # Cleanup steps
    exec sleep 5
    if {[dcp_wrap {clearinghouse create /.:/clear_disable4_ch}] != 0} {return;}
    exec sleep 5
    if {[dcp_wrap {clearinghouse delete /.:/clear_disable4_ch}] != 0} {return;}
}

proc clearinghouse_disable_5_P {} {
    # Identify the test
    tet_infoline clearinghouse_disable_5_P: Check that we can disable a list of clearinghouses

    # Setup steps
    if {[dcp_wrap {clearinghouse create /.:/clear_disable5a_ch}] != 0} {return;}
    if {[dcp_wrap {clearinghouse create /.:/clear_disable5b_ch}] != 0} {return;}

    # Run the test
    clearinghouse disable {/.:/clear_disable5a_ch /.:/clear_disable5b_ch}
    test {
        list [catch {clearinghouse delete {/.:/clear_disable5a_ch /.:/clear_disable5b_ch}} msg] $msg
    } compare_strings {1 {Specified clearinghouse does not exist}}

    # Cleanup steps
    exec sleep 5
    if {[dcp_wrap {clearinghouse create /.:/clear_disable5a_ch}] != 0} {return;}
    if {[dcp_wrap {clearinghouse create /.:/clear_disable5b_ch}] != 0} {return;}
    exec sleep 5
    if {[dcp_wrap {clearinghouse delete /.:/clear_disable5a_ch}] != 0} {return;}
    if {[dcp_wrap {clearinghouse delete /.:/clear_disable5b_ch}] != 0} {return;}
}

set iclist "ic1 ic2 "
set ic1 "clearinghouse_disable_4_P"
set ic2 "clearinghouse_disable_5_P"
set tet_startup clearinghouse_startup
set tet_cleanup clearinghouse_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
