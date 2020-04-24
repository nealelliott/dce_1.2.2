#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: clock_show_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:02  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:47  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:49  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/4  1995/09/29  13:33 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts47/1  1995/09/29  13:32 UTC  truitt
# 	CHFts14292: Improvement in tests.
# 
# 	HP revision /main/HPDCE02/3  1995/05/04  19:35 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts28/1  1995/05/04  19:34 UTC  truitt
# 	CHFts15009: Improve the 'no more bindings' error message.
# 
# 	HP revision /main/HPDCE02/2  1995/02/27  18:27 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts16/1  1995/02/27  18:26 UTC  truitt
# 	The -inetd and -dced options no longer require
# 	a hostname, although one is allowed.
# 
# 	HP revision /main/HPDCE02/1  1994/12/01  15:39 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/12/01  15:38 UTC  truitt
# 	Add tests for new clock functionality.
# 	[1995/12/11  15:52:56  root]
# 
# Revision 1.1.2.7  1994/09/28  21:39:30  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:36  melman]
# 
# Revision 1.1.2.6  1994/06/10  20:12:30  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:57  devsrc]
# 
# Revision 1.1.2.5  1994/05/26  18:45:18  rousseau
# 	Updated tests.
# 	[1994/05/26  18:45:04  rousseau]
# 
# Revision 1.1.2.4  1994/05/23  18:23:53  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:22:57  rousseau]
# 
# Revision 1.1.2.3  1994/05/11  19:36:58  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:20:01  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:35:47  melman
# 	updating to new API path
# 	[1994/05/06  15:35:14  melman]
# 
# Revision 1.1.2.1  1994/04/21  13:57:38  rousseau
# 	Initial drop of dts and clock tests (CR 9938).
# 	[1994/04/21  13:55:49  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc clock_show_1_N {} {
    # Identify the test
    tet_infoline clock_show_1_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {clock show /.:/hosts/[exec hostname]/dts-entity foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc clock_show_2_N {} {
    # Identify the test
    tet_infoline clock_show_2_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {clock show /.:/hosts/[exec hostname]/dts-entity -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc clock_show_3_N {} {
    # Identify the test
    tet_infoline clock_show_3_N: Check that the dts name is required.

    # Setup steps

    # Run the test
    test {
        list [catch {clock show [exec hostname]} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc clock_show_4_N {} {
    global DCP_CELLNAME_ONE DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline clock_show_4_N: Check that dts name is required.

    # Setup steps

    # Run the test
    test {
        list [catch {clock show $DCP_CELLNAME_ONE/hosts/$DCP_HOSTNAME_ONE} msg] $msg
    } compare_strings "1 {Unable to find appropriate DTS interface at '$DCP_CELLNAME_ONE/hosts/$DCP_HOSTNAME_ONE'.}"

    # Cleanup steps
}

proc clock_show_5_N {} {
    # Identify the test
    tet_infoline clock_show_5_N: Check that only one option is specified.

    # Setup steps

    # Run the test
    test {
        list [catch {clock show -dced -inetd} msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}

proc clock_show_6_N {} {
    # Identify the test
    tet_infoline clock_show_6_N: Check that only one option is specified.

    # Setup steps

    # Run the test
    test {
        list [catch {clock show -dced -inetd [exec hostname]} msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}

proc clock_show_7_N {} {
    # Identify the test
    tet_infoline clock_show_7_N: Check that the hostname is valid when using dtsd.

    # Setup steps

    # Run the test
    test {
        list [catch {clock show hosts/gumby} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc clock_show_8_N {} {
    # Identify the test
    tet_infoline clock_show_8_N: Check that the hostname is valid when using dtsd.

    # Setup steps

    # Run the test
    test {
        list [catch {clock show /.:/hosts/gumby/dts-entity} msg] $msg
    } compare_strings {1 {Entry not found}}

    # Cleanup steps
}

proc clock_show_9_N {} {
    # Identify the test
    tet_infoline clock_show_9_N: Check that the host is valid when using inetd.

    # Setup steps

    # Run the test
    test {
        list [catch {clock show -inetd gumby_and_pokey} msg] $msg
    } compare_strings {1 {Could not get current time using inetd socket connection.}}

    # Cleanup steps
}

proc clock_show_10_N {} {
    global DCP_HOSTNAME_WITHOUT_INETD
    # Identify the test
    tet_infoline clock_show_10_N: Check that the host is valid when using inetd.

    # Setup steps

    # Run the test
    test {
        list [catch {clock show -inetd $DCP_HOSTNAME_WITHOUT_INETD} msg] $msg
    } compare_strings {1 {Could not get current time using inetd socket connection.}}

    # Cleanup steps
}

proc clock_show_11_N {} {
    # Identify the test
    tet_infoline clock_show_11_N: Check that the host is valid when using dced.

    # Setup steps

    # Run the test
    test {
        list [catch {clock show -dced gumby_and_pokey} msg] $msg
    } compare_strings {1 {Invalid network address}}

    # Cleanup steps
}

proc clock_show_12_N {} {
    global DCP_HOSTNAME_WITHOUT_DCED
    # Identify the test
    tet_infoline clock_show_12_N: Check that the host is valid when using dced.

    # Setup steps

    # Run the test
    test {
        list [catch {clock show -dced $DCP_HOSTNAME_WITHOUT_DCED} msg] $msg
    } compare_strings {1 {Could not get current time using dced services.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 "
set ic1 "clock_show_1_N"
set ic2 "clock_show_2_N"
set ic3 "clock_show_3_N"
set ic4 "clock_show_4_N"
set ic5 "clock_show_5_N"
set ic6 "clock_show_6_N"
set ic7 "clock_show_7_N"
set ic8 "clock_show_8_N"
set ic9 "clock_show_9_N"
set ic10 "clock_show_10_N"
set ic11 "clock_show_11_N"
set ic12 "clock_show_12_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
