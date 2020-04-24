#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: audfilter_modify_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:11:14  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:19  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:45  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:51  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:16  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:55  melman]
# 
# Revision 1.1.2.6  1994/09/02  20:28:39  rousseau
# 	Mass test cleanup.
# 	[1994/09/02  20:27:56  rousseau]
# 
# Revision 1.1.2.5  1994/06/10  20:03:21  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:58  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:09:26  rousseau
# 	Various updates.
# 	[1994/05/23  18:09:00  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:27:07  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:24:56  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:55  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:33  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc audfilter_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc audfilter_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}


proc audfilter_modify_1_N {} {
    # Identify the test
    tet_infoline audfilter_modify_1_N: Check that no supplied arguments is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify} msg] $msg
    } compare_strings {1 {The command requires at least one argument.}}

    # Cleanup steps
}

proc audfilter_modify_2_N {} {
    # Identify the test
    tet_infoline audfilter_modify_2_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -add {dce_dts_synch success log} foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc audfilter_modify_3_N {} {
    # Identify the test
    tet_infoline audfilter_modify_3_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -add {dce_dts_synch success log} -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc audfilter_modify_4_N {} {
    # Identify the test
    tet_infoline audfilter_modify_4_N: Check the handling of no options.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world} msg] $msg
    } compare_strings {1 {At least one of the options '-add' or '-remove' is required.}}

    # Cleanup steps
}

proc audfilter_modify_5_N {} {
    # Identify the test
    tet_infoline audfilter_modify_5_N: Check the handling of -add with no arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify -add {dce_dts_synch success log}} msg] $msg
    } compare_strings {1 {The command requires at least one argument.}}

    # Cleanup steps
}

proc audfilter_modify_6_N {} {
    # Identify the test
    tet_infoline audfilter_modify_6_N: Check the handling of the -add option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -add} msg] $msg
    } compare_strings {1 {The '-add' option requires a character string value.}}

    # Cleanup steps
}

proc audfilter_modify_7_N {} {
    # Identify the test
    tet_infoline audfilter_modify_7_N: Check that too few elements in the list to -add is trapped as an
    tet_infoline audfilter_modify_7_N: error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -add {foo}} msg] $msg
    } compare_strings {1 {The list {foo} contains too few elements.}}

    # Cleanup steps
}

proc audfilter_modify_8_N {} {
    # Identify the test
    tet_infoline audfilter_modify_8_N: Check that too many elements in the list to -add is trapped as an
    tet_infoline audfilter_modify_8_N: error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -add {dce_dts_synch success log foo}} msg] $msg
    } compare_strings {1 {The list {dce_dts_synch success log foo} contains too many elements.}}

    # Cleanup steps
}

proc audfilter_modify_9_N {} {
    # Identify the test
    tet_infoline audfilter_modify_9_N: Check that an invalid event class passed to -add is trapped as an
    tet_infoline audfilter_modify_9_N: error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -add {foo success log}} msg] $msg
    } compare_strings {1 {The event class 'foo' is not valid.}}

    # Cleanup steps
}

proc audfilter_modify_10_N {} {
    # Identify the test
    tet_infoline audfilter_modify_10_N: Check that an invalid event class passed to -add is trapped as an
    tet_infoline audfilter_modify_10_N: error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -add {foo success log}} msg] $msg
    } compare_strings {1 {The event class 'foo' is not valid.}}

    # Cleanup steps
}

proc audfilter_modify_11_N {} {
    # Identify the test
    tet_infoline audfilter_modify_11_N: Check that an invalid condition passed to -add is trapped as an
    tet_infoline audfilter_modify_11_N: error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -add {dce_dts_synch foo log}} msg] $msg
    } compare_strings {1 {The audit condition 'foo' is not valid.}}

    # Cleanup steps
}

proc audfilter_modify_12_N {} {
    # Identify the test
    tet_infoline audfilter_modify_12_N: Check that an invalid action passed to -add is trapped as an
    tet_infoline audfilter_modify_12_N: error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -add {dce_dts_synch success foo}} msg] $msg
    } compare_strings {1 {The audit action 'foo' is not valid.}}

    # Cleanup steps
}

proc audfilter_modify_14_N {} {
    # Identify the test
    tet_infoline audfilter_modify_14_N: Check the handling of -remove with no arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify -remove {dce_dts_synch success log}} msg] $msg
    } compare_strings {1 {The command requires at least one argument.}}

    # Cleanup steps
}

proc audfilter_modify_15_N {} {
    # Identify the test
    tet_infoline audfilter_modify_15_N: Check the handling of the -remove option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -remove} msg] $msg
    } compare_strings {1 {The '-remove' option requires a character string value.}}

    # Cleanup steps
}

proc audfilter_modify_16_N {} {
    # Identify the test
    tet_infoline audfilter_modify_16_N: Check that too few elements in the list to -remove is trapped as
    tet_infoline audfilter_modify_16_N: an error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -remove {foo}} msg] $msg
    } compare_strings {1 {The list {foo} contains too few elements.}}

    # Cleanup steps
}

proc audfilter_modify_17_N {} {
    # Identify the test
    tet_infoline audfilter_modify_17_N: Check that too many elements in the list to -remove is trapped as
    tet_infoline audfilter_modify_17_N: an error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -remove {dce_dts_synch success log foo}} msg] $msg
    } compare_strings {1 {The list {dce_dts_synch success log foo} contains too many elements.}}

    # Cleanup steps
}

proc audfilter_modify_18_N {} {
    # Identify the test
    tet_infoline audfilter_modify_18_N: Check that an invalid event class passed to -remove is trapped as
    tet_infoline audfilter_modify_18_N: an error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -remove {foo success log}} msg] $msg
    } compare_strings {1 {The event class 'foo' is not valid.}}

    # Cleanup steps
}

proc audfilter_modify_19_N {} {
    # Identify the test
    tet_infoline audfilter_modify_19_N: Check that an invalid event class passed to -remove is trapped as
    tet_infoline audfilter_modify_19_N: an error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -remove {foo success log}} msg] $msg
    } compare_strings {1 {The event class 'foo' is not valid.}}

    # Cleanup steps
}

proc audfilter_modify_20_N {} {
    # Identify the test
    tet_infoline audfilter_modify_20_N: Check that an invalid condition passed to -remove is trapped as an
    tet_infoline audfilter_modify_20_N: error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -remove {dce_dts_synch foo log}} msg] $msg
    } compare_strings {1 {The audit condition 'foo' is not valid.}}

    # Cleanup steps
}

proc audfilter_modify_21_N {} {
    # Identify the test
    tet_infoline audfilter_modify_21_N: Check that an invalid action passed to -remove is trapped as an
    tet_infoline audfilter_modify_21_N: error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter modify world -remove {dce_dts_synch success foo}} msg] $msg
    } compare_strings {1 {The audit action 'foo' is not valid.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 "
set ic1 "audfilter_modify_1_N"
set ic2 "audfilter_modify_2_N"
set ic3 "audfilter_modify_3_N"
set ic4 "audfilter_modify_4_N"
set ic5 "audfilter_modify_5_N"
set ic6 "audfilter_modify_6_N"
set ic7 "audfilter_modify_7_N"
set ic8 "audfilter_modify_8_N"
set ic9 "audfilter_modify_9_N"
set ic10 "audfilter_modify_10_N"
set ic11 "audfilter_modify_11_N"
set ic12 "audfilter_modify_12_N"
set ic13 "audfilter_modify_14_N"
set ic14 "audfilter_modify_15_N"
set ic15 "audfilter_modify_16_N"
set ic16 "audfilter_modify_17_N"
set ic17 "audfilter_modify_18_N"
set ic18 "audfilter_modify_19_N"
set ic19 "audfilter_modify_20_N"
set ic20 "audfilter_modify_21_N"
set tet_startup audfilter_modify_startup
set tet_cleanup audfilter_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

