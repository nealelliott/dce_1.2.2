#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: clock_synchronize_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:08  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:51  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:52  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/09/29  13:31 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts47/1  1995/09/29  13:31 UTC  truitt
# 	CHFts14292: Improvement in tests.
# 
# 	HP revision /main/HPDCE02/1  1994/12/01  15:43 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/12/01  15:41 UTC  truitt
# 	Add tests for new clock functionality.
# 	[1995/12/11  15:52:58  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:32  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:41  melman]
# 
# Revision 1.1.2.5  1994/06/10  20:12:32  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:58  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:23:55  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:23:01  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc clock_synchronize_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc clock_synchronize_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc clock_synchronize_1_N {} {
    # Identify the test
    tet_infoline clock_synchronize_1_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize /.:/hosts/[exec hostname]/dts-entity foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc clock_synchronize_2_N {} {
    # Identify the test
    tet_infoline clock_synchronize_2_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize /.:/hosts/[exec hostname]/dts-entity -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc clock_synchronize_3_N {} {
    # Identify the test
    tet_infoline clock_synchronize_3_N: Check that the combination of switches is legal.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize -dced -abruptly} msg] $msg
    } compare_strings {1 {When using '-dced' or '-inetd', the '-abruptly' option is not allowed.}}

    # Cleanup steps
}

proc clock_synchronize_4_N {} {
    # Identify the test
    tet_infoline clock_synchronize_4_N: Check that the combination of switches is legal.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize -inetd -abruptly} msg] $msg
    } compare_strings {1 {When using '-dced' or '-inetd', the '-abruptly' option is not allowed.}}

    # Cleanup steps
}

proc clock_synchronize_5_N {} {
    # Identify the test
    tet_infoline clock_synchronize_5_N: Check that the combination of switches is legal.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize -dced -inetd -abruptly} msg] $msg
    } compare_strings {1 {When using '-dced' or '-inetd', the '-abruptly' option is not allowed.}}

    # Cleanup steps
}

proc clock_synchronize_6_N {} {
    # Identify the test
    tet_infoline clock_synchronize_6_N: Check that the hostname is there if using inetd or dced.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize -dced} msg] $msg
    } compare_strings {1 {Missing hostname.}}

    # Cleanup steps
}

proc clock_synchronize_7_N {} {
    # Identify the test
    tet_infoline clock_synchronize_7_N: Check that the hostname is there if using inetd or dced.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize -inetd} msg] $msg
    } compare_strings {1 {Missing hostname.}}

    # Cleanup steps
}

proc clock_synchronize_8_N {} {
    # Identify the test
    tet_infoline clock_synchronize_8_N: Check that the hostname is there if using inetd or dced.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize -dced -inetd} msg] $msg
    } compare_strings {1 {Missing hostname.}}

    # Cleanup steps
}

proc clock_synchronize_9_N {} {
    # Identify the test
    tet_infoline clock_synchronize_9_N: Check that only one of inetd and dced is specified.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize -dced -inetd [exec hostname]} msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}

proc clock_synchronize_10_N {} {
    # Identify the test
    tet_infoline clock_synchronize_10_N: Check that a valid hostname is entered when using dtsd.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize gumby_and_pokey} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc clock_synchronize_11_N {} {
    # Identify the test
    tet_infoline clock_synchronize_11_N: Check that a valid hostname is entered when using dtsd.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize /.:/hosts/gumby_and_pokey/dts-entity} msg] $msg
    } compare_strings {1 {Entry not found}}

    # Cleanup steps
}

proc clock_synchronize_12_N {} {
    # Identify the test
    tet_infoline clock_synchronize_12_N: Check that a valid hostname is entered when using dced.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize -dced gumby_and_pokey} msg] $msg
    } compare_strings {1 {Invalid network address}}

    # Cleanup steps
}

proc clock_synchronize_13_N {} {
    global DCP_HOSTNAME_WITHOUT_DCED
    # Identify the test
    tet_infoline clock_synchronize_13_N: Check that a valid hostname is entered when using dced.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize -dced $DCP_HOSTNAME_WITHOUT_DCED} msg] $msg
    } compare_strings {1 {Could not get current time using dced services.}}

    # Cleanup steps
}

proc clock_synchronize_14_N {} {
    # Identify the test
    tet_infoline clock_synchronize_14_N: Check that a valid hostname is entered when using inetd.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize -inetd gumby_and_pokey} msg] $msg
    } compare_strings {1 {Could not get current time using inetd socket connection.}}

    # Cleanup steps
}

proc clock_synchronize_15_N {} {
    global DCP_HOSTNAME_WITHOUT_INETD
    # Identify the test
    tet_infoline clock_synchronize_15_N: Check that a valid hostname is entered when using inetd.

    # Setup steps

    # Run the test
    test {
        list [catch {clock synchronize -inetd $DCP_HOSTNAME_WITHOUT_INETD} msg] $msg
    } compare_strings {1 {Could not get current time using inetd socket connection.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 "
set ic1 "clock_synchronize_1_N"
set ic2 "clock_synchronize_2_N"
set ic3 "clock_synchronize_3_N"
set ic4 "clock_synchronize_4_N"
set ic5 "clock_synchronize_5_N"
set ic6 "clock_synchronize_6_N"
set ic7 "clock_synchronize_7_N"
set ic8 "clock_synchronize_8_N"
set ic9 "clock_synchronize_9_N"
set ic10 "clock_synchronize_10_N"
set ic11 "clock_synchronize_11_N"
set ic12 "clock_synchronize_12_N"
set ic13 "clock_synchronize_13_N"
set ic14 "clock_synchronize_14_N"
set ic15 "clock_synchronize_15_N"
set tet_startup clock_synchronize_startup
set tet_cleanup clock_synchronize_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
