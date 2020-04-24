#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: clearinghouse_disable_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:12:56  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:06  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:11  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/08/22  20:28 UTC  truitt
# 	Merge test work.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts45/1  1995/08/22  20:27 UTC  truitt
# 	CHFts16097: Add additional clearinghouse tests.
# 
# 	HP revision /main/HPDCE02/1  1995/06/09  13:10 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts34/1  1995/06/09  13:08 UTC  truitt
# 	CHFts15424: Cleanup clearinghouse tests.
# 	[1995/12/11  15:52:24  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:36  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:24  melman]
# 
# Revision 1.1.2.5  1994/09/06  13:14:37  kevins
# 	OT 10617 modified messages in negative tests
# 	[1994/09/06  12:30:50  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:03:31  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:09  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:41  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:17:39  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:04  melman
# 	updating to new API path
# 	[1994/05/06  15:19:46  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:20:27  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:27  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc clearinghouse_disable_1_N {} {
    # Identify the test
    tet_infoline clearinghouse_disable_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {clearinghouse disable /.:/foo_ch extra_ch} msg] $msg
    } compare_strings {1 {Unrecognized argument 'extra_ch'.}}

    # Cleanup steps
}

proc clearinghouse_disable_2_N {} {
    # Identify the test
    tet_infoline clearinghouse_disable_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {clearinghouse disable} msg] $msg
    } compare_strings {1 {No clearinghouse name.}}

    # Cleanup steps
}

proc clearinghouse_disable_3_N {} {
    # Identify the test
    tet_infoline clearinghouse_disable_3_N: Check that non-existant clearinghouses are handled.

    # Setup steps

    # Run the test
    test {
        list [catch {clearinghouse disable /.:/foo_ch} msg] $msg
    } compare_strings {1 {Specified clearinghouse does not exist}}

    # Cleanup steps
}

proc clearinghouse_disable_4_N {} {
    # Identify the test
    tet_infoline clearinghouse_disable_4_N: Check that bogus names are handled correctly.

    # Setup steps

    # Run the test
    test {
        list [catch {clearinghouse disable foo} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "clearinghouse_disable_1_N"
set ic2 "clearinghouse_disable_2_N"
set ic3 "clearinghouse_disable_3_N"
set ic4 "clearinghouse_disable_4_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
