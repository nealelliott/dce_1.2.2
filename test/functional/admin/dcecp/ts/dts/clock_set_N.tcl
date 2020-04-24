#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: clock_set_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:14:56  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:35  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:47  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/12/01  15:29 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/12/01  15:25 UTC  truitt
# 	Add tests for new clock functionality.
# 	[1995/12/11  15:52:55  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:28  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:33  melman]
# 
# Revision 1.1.2.6  1994/06/10  20:12:29  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:54  devsrc]
# 
# Revision 1.1.2.5  1994/05/26  18:45:17  rousseau
# 	Updated tests.
# 	[1994/05/26  18:45:03  rousseau]
# 
# Revision 1.1.2.4  1994/05/23  18:23:51  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:22:55  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc clock_set_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc clock_set_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc clock_set_1_N {} {
    # Identify the test
    tet_infoline clock_set_1_N: Check handling of invalid arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -to 1994-01-13-smurf+00:00} msg] $msg
    } compare_strings {1 {Invalid absolute DTS style time string.}}

    # Cleanup steps
}

proc clock_set_2_N {} {
    # Identify the test
    tet_infoline clock_set_2_N: Check handling of invalid options.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set /.:/hosts/[exec hostname]/dts-entity -to 1994-05-23-15:34:48.271+00:00I----- -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc clock_set_3_N {} {
    # Identify the test
    tet_infoline clock_set_3_N: Check handling of extra arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -to 1993-12-30-20:31:59.795+00:00 /.:/hosts/[exec hostname]/dts-entity foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc clock_set_4_N {} {
    # Identify the test
    tet_infoline clock_set_4_N: Check handling of -abruptly with extra options.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -to 1993-12-30-20:31:59.795+00:00 -abruptly -epoch 3 /.:/hosts/[exec hostname]/dts-entity -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc clock_set_5_N {} {
    # Identify the test
    tet_infoline clock_set_5_N: Check handling of -abruptly with extra arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -to 1993-12-30-20:31:59.795+00:00 -abruptly -epoch 1 /.:/hosts/[exec hostname]/dts-entity foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc clock_set_6_N {} {
    # Identify the test
    tet_infoline clock_set_6_N: Check handling of extra options.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -to 1993-12-30-20:31:59.795+00:00 /.:/hosts/[exec hostname]/dts-entity -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc clock_set_7_N {} {
    # Identify the test
    tet_infoline clock_set_7_N: Check that the time is required.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set} msg] $msg
    } compare_strings {1 {'-to' option not specified.}}

    # Cleanup steps
}

proc clock_set_8_N {} {
    # Identify the test
    tet_infoline clock_set_8_N: Check that when changing the epoch, abrupt is not allowed.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -epoch 101 -abruptly} msg] $msg
    } compare_strings {1 {When just setting the epoch, the '-abruptly' option is not allowed.}}

    # Cleanup steps
}

proc clock_set_9_N {} {
    # Identify the test
    tet_infoline clock_set_9_N: Check that when using abrupt, epoch is required.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -abruptly -to [clock show -inetd [exec hostname]]} msg] $msg
    } compare_strings {1 {'-epoch' option not specified.}}

    # Cleanup steps
}

proc clock_set_10_N {} {
    # Identify the test
    tet_infoline clock_set_10_N: Check that when using epoch, abrupt is required.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -epoch 101 -to [clock show -inetd [exec hostname]]} msg] $msg
    } compare_strings {1 {'-abruptly' option not specified.}}

    # Cleanup steps
}

proc clock_set_11_N {} {
    # Identify the test
    tet_infoline clock_set_11_N: Check that a valid epoch is entered.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -epoch blech} msg] $msg
    } compare_strings {1 {Expected integer argument for '-epoch' but received 'blech'.}}

    # Cleanup steps
}

proc clock_set_12_N {} {
    # Identify the test
    tet_infoline clock_set_12_N: Check that a valid epoch is entered.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -epoch 1001} msg] $msg
    } compare_strings {1 {Epoch out of range or not different from current epoch}}

    # Cleanup steps
}

proc clock_set_13_N {} {
    # Identify the test
    tet_infoline clock_set_13_N: Check that a valid epoch is entered.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -epoch 1001 -abruptly -to [clock show -inetd [exec hostname]]} msg] $msg
    } compare_strings {1 {Invalid epoch. The value must be in the range of [0..255].}}

    # Cleanup steps
}

proc clock_set_14_N {} {
    # Identify the test
    tet_infoline clock_set_14_N: Check that a valid epoch is entered.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -epoch -1001 -abruptly -to [clock show -inetd [exec hostname]]} msg] $msg
    } compare_strings {1 {Invalid epoch. The value must be in the range of [0..255].}}

    # Cleanup steps
}

proc clock_set_15_N {} {
    # Identify the test
    tet_infoline clock_set_15_N: Check that other options are not allowed when using bypass.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -bypass -abruptly} msg] $msg
    } compare_strings {1 {When using '-bypass', other options are not allowed.}}

    # Cleanup steps
}

proc clock_set_16_N {} {
    # Identify the test
    tet_infoline clock_set_16_N: Check that other options are not allowed when using bypass.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -bypass -epoch 101} msg] $msg
    } compare_strings {1 {When using '-bypass', other options are not allowed.}}

    # Cleanup steps
}

proc clock_set_17_N {} {
    # Identify the test
    tet_infoline clock_set_17_N: Check that other options are not allowed when using bypass.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -bypass -abruptly -epoch 101} msg] $msg
    } compare_strings {1 {When using '-bypass', other options are not allowed.}}

    # Cleanup steps
}

proc clock_set_18_N {} {
    # Identify the test
    tet_infoline clock_set_18_N: Check that a hostname is not allowed when using bypass.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -bypass [exec hostname] -to [clock show -inetd [exec hostname]]} msg] $msg
    } compare_strings {1 {When using '-bypass', specifying a hostname is not allowed.}}

    # Cleanup steps
}

proc clock_set_19_N {} {
    # Identify the test
    tet_infoline clock_set_19_N: Check that bypass will not allow an invalid timestamp.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set -bypass -to gumby} msg] $msg
    } compare_strings {1 {Invalid absolute DTS style time string.}}

    # Cleanup steps
}

proc clock_set_20_N {} {
    # Identify the test
    tet_infoline clock_set_20_N: Check that a bogus hostname is not allowed.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set gumby -to [clock show -inetd [exec hostname]]} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc clock_set_21_N {} {
    # Identify the test
    tet_infoline clock_set_21_N: Check that a bogus hostname is not allowed.

    # Setup steps

    # Run the test
    test {
        list [catch {clock set /.:/hosts/[exec hostname]/gumby -to [clock show -inetd [exec hostname]]} msg] $msg
    } compare_strings {1 {Entry not found}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 "
set ic1 "clock_set_1_N"
set ic2 "clock_set_2_N"
set ic3 "clock_set_3_N"
set ic4 "clock_set_4_N"
set ic5 "clock_set_5_N"
set ic6 "clock_set_6_N"
set ic7 "clock_set_7_N"
set ic8 "clock_set_8_N"
set ic9 "clock_set_9_N"
set ic10 "clock_set_10_N"
set ic11 "clock_set_11_N"
set ic12 "clock_set_12_N"
set ic13 "clock_set_13_N"
set ic14 "clock_set_14_N"
set ic15 "clock_set_15_N"
set ic16 "clock_set_16_N"
set ic17 "clock_set_17_N"
set ic18 "clock_set_18_N"
set ic19 "clock_set_19_N"
set ic20 "clock_set_20_N"
set ic21 "clock_set_21_N"
set tet_startup clock_set_startup
set tet_cleanup clock_set_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
