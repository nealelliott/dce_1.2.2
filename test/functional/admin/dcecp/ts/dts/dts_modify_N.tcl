#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: dts_modify_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:28  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:09  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:59  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:05  root]
# 
# Revision 1.1.2.2  1994/10/01  01:33:12  rousseau
# 	Work around dts (serverentry) problem.
# 	[1994/10/01  01:24:04  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:39:40  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:54  melman]
# 
# Revision 1.1.2.6  1994/09/02  22:21:05  rousseau
# 	Mass test updates.
# 	[1994/09/02  22:20:39  rousseau]
# 
# Revision 1.1.2.5  1994/06/10  20:12:40  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:13  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:24:02  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:23:09  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc dts_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc dts_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc dts_modify_1_N {} {
    # Identify the test
    tet_infoline dts_modify_1_N: Check that no supplied arguments is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify} msg] $msg
    } compare_strings {1 {The command requires at least one option.}}

    # Cleanup steps
}

proc dts_modify_2_N {} {
    # Identify the test
    tet_infoline dts_modify_2_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify /.:/hosts/[exec hostname]/dts-entity foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc dts_modify_3_N {} {
    # Identify the test
    tet_infoline dts_modify_3_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify /.:/hosts/[exec hostname]/dts-entity -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc dts_modify_4_N {} {
    # Identify the test
    tet_infoline dts_modify_4_N: Check the handling of the -globaltimeout option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -globaltimeout foo /.:/hosts/[exec hostname]/dts-entity} msg] $msg
    } compare_strings {1 {Invalid DTS style timestamp.}}

    # Cleanup steps
}

proc dts_modify_5_N {} {
    # Identify the test
    tet_infoline dts_modify_5_N: Check the handling of the -globaltimeout option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -globaltimeout} msg] $msg
    } compare_strings {1 {The '-globaltimeout' option requires a character string value.}}

    # Cleanup steps
}

proc dts_modify_7_N {} {
    # Identify the test
    tet_infoline dts_modify_7_N: Check the handling of the -localtimeout option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -localtimeout foo /.:/hosts/[exec hostname]/dts-entity} msg] $msg
    } compare_strings {1 {Invalid DTS style timestamp.}}

    # Cleanup steps
}

proc dts_modify_8_N {} {
    # Identify the test
    tet_infoline dts_modify_8_N: Check the handling of the -localtimeout option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -localtimeout} msg] $msg
    } compare_strings {1 {The '-localtimeout' option requires a character string value.}}

    # Cleanup steps
}

proc dts_modify_10_N {} {
    # Identify the test
    tet_infoline dts_modify_10_N: Check the handling of the -maxinaccuracy option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -maxinaccuracy foo} msg] $msg
    } compare_strings {1 {Invalid DTS style timestamp.}}

    # Cleanup steps
}

proc dts_modify_11_N {} {
    # Identify the test
    tet_infoline dts_modify_11_N: Check the handling of the -maxinaccuracy option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -maxinaccuracy} msg] $msg
    } compare_strings {1 {The '-maxinaccuracy' option requires a character string value.}}

    # Cleanup steps
}

proc dts_modify_13_N {} {
    # Identify the test
    tet_infoline dts_modify_13_N: Check the handling of the -minservers option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -minservers foo} msg] $msg
    } compare_strings {1 {Expected integer argument for '-minservers' but received 'foo'.}}

    # Cleanup steps
}

proc dts_modify_14_N {} {
    # Identify the test
    tet_infoline dts_modify_14_N: Check the handling of the -minservers option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -minservers} msg] $msg
    } compare_strings {1 {The '-minservers' option requires an integer value.}}

    # Cleanup steps
}

proc dts_modify_16_N {} {
    # Identify the test
    tet_infoline dts_modify_16_N: Check the handling of the -queryattempts option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify /.:/hosts/[exec hostname]/dts-entity -queryattempts foo} msg] $msg
    } compare_strings {1 {Expected integer argument for '-queryattempts' but received 'foo'.}}

    # Cleanup steps
}

proc dts_modify_17_N {} {
    # Identify the test
    tet_infoline dts_modify_17_N: Check the handling of the -queryattempts option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -queryattempts} msg] $msg
    } compare_strings {1 {The '-queryattempts' option requires an integer value.}}

    # Cleanup steps
}

proc dts_modify_19_N {} {
    # Identify the test
    tet_infoline dts_modify_19_N: Check the handling of the -syncinterval option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -syncinterval foo /.:/hosts/[exec hostname]/dts-entity} msg] $msg
    } compare_strings {1 {Invalid DTS style timestamp.}}

    # Cleanup steps
}

proc dts_modify_20_N {} {
    # Identify the test
    tet_infoline dts_modify_20_N: Check the handling of the -syncinterval option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -syncinterval} msg] $msg
    } compare_strings {1 {The '-syncinterval' option requires a character string value.}}

    # Cleanup steps
}

proc dts_modify_22_N {} {
    # Identify the test
    tet_infoline dts_modify_22_N: Check the handling of the -tolerance option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -tolerance foo} msg] $msg
    } compare_strings {1 {Invalid DTS style timestamp.}}

    # Cleanup steps
}

proc dts_modify_23_N {} {
    # Identify the test
    tet_infoline dts_modify_23_N: Check the handling of the -tolerance option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -tolerance} msg] $msg
    } compare_strings {1 {The '-tolerance' option requires a character string value.}}

    # Cleanup steps
}

proc dts_modify_29_N {} {
    # Identify the test
    tet_infoline dts_modify_29_N: Check the handling of the -checkinterval option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -checkinterval foo} msg] $msg
    } compare_strings {1 {Invalid DTS style timestamp.}}

    # Cleanup steps
}

proc dts_modify_30_N {} {
    # Identify the test
    tet_infoline dts_modify_30_N: Check the handling of the -checkinterval option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -checkinterval} msg] $msg
    } compare_strings {1 {The '-checkinterval' option requires a character string value.}}

    # Cleanup steps
}

proc dts_modify_32_N {} {
    # Identify the test
    tet_infoline dts_modify_32_N: Check the handling of the -courierrole option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -courierrole foo} msg] $msg
    } compare_strings {1 {Invalid courier role.}}

    # Cleanup steps
}

proc dts_modify_33_N {} {
    # Identify the test
    tet_infoline dts_modify_33_N: Check the handling of the -courierrole option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -courierrole} msg] $msg
    } compare_strings {1 {The '-courierrole' option requires a character string value.}}

    # Cleanup steps
}

proc dts_modify_36_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline dts_modify_36_N: Check the handling of the -serverentry option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -serverentry foo} msg] $msg
    } compare_strings {1 {No permission for name service operation}}

    # Cleanup steps
    if {[dcp_wrap {dts modify -serverentry hosts/$DCP_HOSTNAME_ONE/dts-entity}] != 0} {return;}
}

proc dts_modify_37_N {} {
    # Identify the test
    tet_infoline dts_modify_37_N: Check the handling of the -serverentry option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -serverentry} msg] $msg
    } compare_strings {1 {The '-serverentry' option requires a character string value.}}

    # Cleanup steps
}

proc dts_modify_39_N {} {
    # Identify the test
    tet_infoline dts_modify_39_N: Check the handling of the -servergroup option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -servergroup} msg] $msg
    } compare_strings {1 {The '-servergroup' option requires a character string value.}}

    # Cleanup steps
}

proc dts_modify_41_N {} {
    # Identify the test
    tet_infoline dts_modify_41_N: Check the handling of mutually exclusive options.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -servergroup none -change {courierrole backup}} msg] $msg
    } compare_strings {1 {The '-change' option can not be used with the individual attribute options.}}

    # Cleanup steps
}

proc dts_modify_42_N {} {
    # Identify the test
    tet_infoline dts_modify_42_N: Check that invalid input to -change is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -change foo} msg] $msg
    } compare_strings {1 {The list contains too few elements.}}

    # Cleanup steps
}

proc dts_modify_43_N {} {
    # Identify the test
    tet_infoline dts_modify_43_N: Check that no input to -change is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -change} msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}

proc dts_modify_44_N {} {
    # Identify the test
    tet_infoline dts_modify_44_N: Check that bad (short) lists to -change is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -change {{minservers}}} msg] $msg
    } compare_strings {1 {The list contains too few elements.}}

    # Cleanup steps
}

proc dts_modify_45_N {} {
    # Identify the test
    tet_infoline dts_modify_45_N: Check that bad (long) lists to -change is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify -change {{minservers bar bang}}} msg] $msg
    } compare_strings {1 {The list contains too many elements.}}

    # Cleanup steps
}

proc dts_modify_46_N {} {
    # Identify the test
    tet_infoline dts_modify_46_N: Check that using -add is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify /.:/hosts/[exec hostname]/dts-entity -add {minservers 3}} msg] $msg
    } compare_strings {1 {Unrecognized option '-add'.}}

    # Cleanup steps
}

proc dts_modify_47_N {} {
    # Identify the test
    tet_infoline dts_modify_47_N: Check that using -remove is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {dts modify /.:/hosts/[exec hostname]/dts-entity -remove {minservers 3}} msg] $msg
    } compare_strings {1 {Unrecognized option '-remove'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31"
set ic1 "dts_modify_1_N"
set ic2 "dts_modify_2_N"
set ic3 "dts_modify_3_N"
set ic4 "dts_modify_4_N"
set ic5 "dts_modify_5_N"
set ic6 "dts_modify_7_N"
set ic7 "dts_modify_8_N"
set ic8 "dts_modify_10_N"
set ic9 "dts_modify_11_N"
set ic10 "dts_modify_13_N"
set ic11 "dts_modify_14_N"
set ic12 "dts_modify_16_N"
set ic13 "dts_modify_17_N"
set ic14 "dts_modify_19_N"
set ic15 "dts_modify_20_N"
set ic16 "dts_modify_22_N"
set ic17 "dts_modify_23_N"
set ic18 "dts_modify_29_N"
set ic19 "dts_modify_30_N"
set ic20 "dts_modify_32_N"
set ic21 "dts_modify_33_N"
set ic22 "dts_modify_36_N"
set ic23 "dts_modify_37_N"
set ic24 "dts_modify_39_N"
set ic25 "dts_modify_41_N"
set ic26 "dts_modify_42_N"
set ic27 "dts_modify_43_N"
set ic28 "dts_modify_44_N"
set ic29 "dts_modify_45_N"
set ic30 "dts_modify_46_N"
set ic31 "dts_modify_47_N"
set tet_startup dts_modify_startup
set tet_cleanup dts_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
