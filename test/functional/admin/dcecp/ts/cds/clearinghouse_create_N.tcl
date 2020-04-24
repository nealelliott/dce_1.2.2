#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: clearinghouse_create_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:12:48  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:56  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:08  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/08/22  20:32 UTC  truitt
# 	Merge test work.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts45/1  1995/08/22  20:31 UTC  truitt
# 	CHFts16073: Add additional clearinghouse tests.
# 
# 	HP revision /main/HPDCE02/1  1995/06/09  13:16 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts34/1  1995/06/09  13:15 UTC  truitt
# 	CHFts15424: Cleanup clearinghouse create tests.
# 	[1995/12/11  15:52:21  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:32  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:14  melman]
# 
# Revision 1.1.2.5  1994/09/06  13:14:33  kevins
# 	OT 10617 modified messages in negative tests
# 	[1994/09/06  12:30:36  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:03:27  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:04  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:35  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:17:25  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:20:57  melman
# 	updating to new API path
# 	[1994/05/06  15:19:41  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:20:17  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:22  kevins]
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
proc clearinghouse_create_1_N {} {
    # Identify the test
    tet_infoline clearinghouse_create_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {clearinghouse create /.:/foo_ch extra_ch} msg] $msg
    } compare_strings {1 {Unrecognized argument 'extra_ch'.}}

    # Cleanup steps
}

proc clearinghouse_create_2_N {} {
    # Identify the test
    tet_infoline clearinghouse_create_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {clearinghouse create} msg] $msg
    } compare_strings {1 {No clearinghouse name.}}

    # Cleanup steps
}

proc clearinghouse_create_3_N {} {
    # Identify the test
    tet_infoline clearinghouse_create_3_N: Check that unauthenticated users are handled correctly.

    # Setup steps

    # Run the test
    test {
        list [catch {clearinghouse create /.:/foo_ch} msg] $msg
    } compare_strings {1 {Insufficient rights to perform requested operation}}

    # Cleanup steps
}

proc clearinghouse_create_4_N {} {
    # Identify the test
    tet_infoline clearinghouse_create_4_N: Check that bogus names are handled correctly.

    # Setup steps

    # Run the test
    test {
        list [catch {clearinghouse create foo} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "clearinghouse_create_1_N"
set ic2 "clearinghouse_create_2_N"
set ic3 "clearinghouse_create_3_N"
set ic4 "clearinghouse_create_4_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
