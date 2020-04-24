#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: clearinghouse_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:10  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:12  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:14  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/06/09  13:02 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts34/1  1995/06/09  13:00 UTC  truitt
# 	CHFts15424: Cleanup clearinghouse tests.
# 
# 	HP revision /main/HPDCE02/2  1994/11/15  15:23 UTC  truitt
# 	Fix the second test case to use two new clearinghouses.
# 
# 	HP revision /main/HPDCE02/1  1994/11/15  14:21 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/15  14:20 UTC  truitt
# 	Do not create clearinghouses when not necessary.
# 	[1995/12/11  15:52:27  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:40  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:31  melman]
# 
# Revision 1.1.2.5  1994/09/06  13:14:40  kevins
# 	OT 10617 modified messages in negative tests
# 	[1994/09/06  12:30:59  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:03:33  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:17  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:47  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:17:51  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:08  melman
# 	updating to new API path
# 	[1994/05/06  15:19:51  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:20:33  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:30  kevins]
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
proc clearinghouse_show_4_P {} {
    global DCP_ROOT_CH

    # Identify the test
    tet_infoline clearinghouse_show_4_P: Check that we can show the contents of a clearinghouse

    # Setup steps

    # Run the test
    test { 
        dcp_found [clearinghouse show /.:/$DCP_ROOT_CH] CDS_NSCellname
    } compare_strings found

    # Cleanup steps
}

proc clearinghouse_show_5_P {} {
    # Identify the test
    tet_infoline clearinghouse_show_5_P: Check that we can show the contents of a list of clearinghouses.

    # Setup steps
    if {[dcp_wrap {clearinghouse create /.:/clear_show5a_ch}] != 0} {return;}
    if {[dcp_wrap {clearinghouse create /.:/clear_show5b_ch}] != 0} {return;}

    # Run the test
    test {
        dcp_found [clearinghouse show {/.:/clear_show5a_ch /.:/clear_show5b_ch}] CDS_NSCellname
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {clearinghouse delete /.:/clear_show5a_ch}] != 0} {return;}
    if {[dcp_wrap {clearinghouse delete /.:/clear_show5b_ch}] != 0} {return;}
}

proc clearinghouse_show_6_P {} {
    global DCP_ROOT_CH

    # Identify the test
    tet_infoline clearinghouse_show_6_P: Check that we can show the counters of a clearinghouse

    # Setup steps

    # Run the test
    test {
        dcp_found [clearinghouse show /.:/$DCP_ROOT_CH -counters] reads
    } compare_strings found

    # Cleanup steps
}

proc clearinghouse_show_7_P {} {
    global DCP_ROOT_CH

    # Identify the test
    tet_infoline clearinghouse_show_7_P: Check that we can show the counters and attributes
    tet_infoline clearinghouse_show_7_P: of a clearinghouse

    # Setup steps

    # Run the test
    test { 
        dcp_found [clearinghouse show /.:/$DCP_ROOT_CH -all] {CDS_NSCellname reads}
    } compare_strings found

    # Cleanup steps
}

set iclist "ic1 ic2 ic3 ic4 "
set ic1 "clearinghouse_show_4_P"
set ic2 "clearinghouse_show_5_P"
set ic3 "clearinghouse_show_6_P"
set ic4 "clearinghouse_show_7_P"
set tet_startup clearinghouse_startup
set tet_cleanup clearinghouse_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
