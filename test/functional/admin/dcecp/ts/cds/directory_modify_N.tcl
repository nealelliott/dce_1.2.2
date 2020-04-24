#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_modify_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:39  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:39  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:25  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/10/10  14:33 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts49/1  1995/10/10  14:32 UTC  truitt
# 	CHFts16537: Update the tests for new functionality.
# 	[1995/12/11  15:52:35  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:51  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:52  melman]
# 
# Revision 1.1.2.5  1994/09/02  13:56:48  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:42  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:18  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:26  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:55  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:18:24  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:15  melman
# 	updating to new API path
# 	[1994/05/06  15:20:03  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:20:53  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:39  kevins]
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
proc directory_modify_1_N {} {
    # Identify the test
    tet_infoline directory_modify_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory show /.:/test_1 foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc directory_modify_2_N {} {
    # Identify the test
    tet_infoline directory_modify_2_N: Check that extra options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc directory_modify_3_N {} {
    # Identify the test
    tet_infoline directory_modify_3_N: Check that missing arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify} msg] $msg
    } compare_strings {1 {No directory name.}}

    # Cleanup steps
}

proc directory_modify_4_N {} {
    # Identify the test
    tet_infoline directory_modify_4_N: Check that missing options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1} msg] $msg
    } compare_strings {1 {Option not specified.}}

    # Cleanup steps
}

proc directory_modify_5_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_modify_5_N: Check that unauthenticated users are handled correctly

    # Setup steps
    if {[dcp_wrap {set dir_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/$dir_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        list [catch {directory modify /.:/$dir_name -add {CDS_UpgradeTo 4.0}} msg] $msg
    } compare_strings {1 {Insufficient rights to perform requested operation}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/$dir_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc directory_modify_6_N {} {
    # Identify the test
    tet_infoline directory_modify_6_N: Check that missing values are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -add} msg] $msg
    } compare_strings {1 {The '-add' option requires a character string value.}}

    # Cleanup steps
}

proc directory_modify_7_N {} {
    # Identify the test
    tet_infoline directory_modify_7_N: Check that missing values are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -remove} msg] $msg
    } compare_strings {1 {The '-remove' option requires a character string value.}}

    # Cleanup steps
}

proc directory_modify_8_N {} {
    # Identify the test
    tet_infoline directory_modify_8_N: Check that missing values are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -change} msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}

proc directory_modify_9_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline directory_modify_9_N: Check that a bad attribute list is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -add one} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc directory_modify_10_N {} {
    # Identify the test
    tet_infoline directory_modify_10_N: Check that missing values are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -master} msg] $msg
    } compare_strings {1 {The '-master' option requires a character string value.}}

    # Cleanup steps
}

proc directory_modify_11_N {} {
    # Identify the test
    tet_infoline directory_modify_10_N: Check that combining epoch and attr modification is bad.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -add gumby -master pokey} msg] $msg
    } compare_strings {1 {The change epoch options cannot be used when modifying directory attributes.}}

    # Cleanup steps
}

proc directory_modify_12_N {} {
    # Identify the test
    tet_infoline directory_modify_12_N: Check that combining epoch and attr modification is bad.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -add gumby -readonly pokey} msg] $msg
    } compare_strings {1 {The change epoch options cannot be used when modifying directory attributes.}}

    # Cleanup steps
}

proc directory_modify_13_N {} {
    # Identify the test
    tet_infoline directory_modify_13_N: Check that combining epoch and attr modification is bad.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -add gumby -exclude pokey} msg] $msg
    } compare_strings {1 {The change epoch options cannot be used when modifying directory attributes.}}

    # Cleanup steps
}

proc directory_modify_14_N {} {
    # Identify the test
    tet_infoline directory_modify_14_N: Check that combining epoch and attr modification is bad.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -remove gumby -master pokey} msg] $msg
    } compare_strings {1 {The change epoch options cannot be used when modifying directory attributes.}}

    # Cleanup steps
}

proc directory_modify_15_N {} {
    # Identify the test
    tet_infoline directory_modify_15_N: Check that combining epoch and attr modification is bad.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -remove gumby -readonly pokey} msg] $msg
    } compare_strings {1 {The change epoch options cannot be used when modifying directory attributes.}}

    # Cleanup steps
}

proc directory_modify_16_N {} {
    # Identify the test
    tet_infoline directory_modify_16_N: Check that combining epoch and attr modification is bad.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -remove gumby -exclude pokey} msg] $msg
    } compare_strings {1 {The change epoch options cannot be used when modifying directory attributes.}}

    # Cleanup steps
}

proc directory_modify_17_N {} {
    # Identify the test
    tet_infoline directory_modify_17_N: Check that combining epoch and attr modification is bad.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -change gumby -master pokey} msg] $msg
    } compare_strings {1 {The change epoch options cannot be used when modifying directory attributes.}}

    # Cleanup steps
}

proc directory_modify_18_N {} {
    # Identify the test
    tet_infoline directory_modify_18_N: Check that combining epoch and attr modification is bad.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -change gumby -readonly pokey} msg] $msg
    } compare_strings {1 {The change epoch options cannot be used when modifying directory attributes.}}

    # Cleanup steps
}

proc directory_modify_19_N {} {
    # Identify the test
    tet_infoline directory_modify_19_N: Check that combining epoch and attr modification is bad.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -change gumby -exclude pokey} msg] $msg
    } compare_strings {1 {The change epoch options cannot be used when modifying directory attributes.}}

    # Cleanup steps
}

proc directory_modify_20_N {} {
    # Identify the test
    tet_infoline directory_modify_20_N: Check the interaction of attr change options.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -types -add gumby} msg] $msg
    } compare_strings {1 {The '-types' option must be used with the '-remove' option.}}

    # Cleanup steps
}

proc directory_modify_21_N {} {
    # Identify the test
    tet_infoline directory_modify_21_N: Check the interaction of attr change options.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/test_1 -single -remove gumby} msg] $msg
    } compare_strings {1 {The '-single' option must be used with the '-add' option.}}

    # Cleanup steps
}

proc directory_modify_22_N {} {
    # Identify the test
    tet_infoline directory_modify_22_N: Only one directory at a time for epoch change.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify {gumby pokey} -master /.:/foo_ch} msg] $msg
    } compare_strings {1 {Only one directory can be specified.}}

    # Cleanup steps
}

proc directory_modify_23_N {} {
    # Identify the test
    tet_infoline directory_modify_23_N: Only one master at a time for epoch change.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify gumby -master {gumby pokey}} msg] $msg
    } compare_strings {1 {Only one master clearinghouse can be specified.}}

    # Cleanup steps
}

proc directory_modify_24_N {} {
    # Identify the test
    tet_infoline directory_modify_24_N: Duplicate name in exclude and readonly.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify gumby -master pokey_ch -readonly goober_ch -exclude goober_ch} msg] $msg
    } compare_strings {1 {The same clearinghouse name was found more than once.}}

    # Cleanup steps
}

proc directory_modify_25_N {} {
    # Identify the test
    tet_infoline directory_modify_25_N: Duplicate name in exclude and master.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify gumby -master pokey_ch -readonly goober_ch -exclude pokey_ch} msg] $msg
    } compare_strings {1 {The same clearinghouse name was found more than once.}}

    # Cleanup steps
}

proc directory_modify_26_N {} {
    # Identify the test
    tet_infoline directory_modify_26_N: Duplicate name in master and readonly.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify gumby -master pokey_ch -readonly pokey_ch -exclude goober_ch} msg] $msg
    } compare_strings {1 {The same clearinghouse name was found more than once.}}

    # Cleanup steps
}

proc directory_modify_27_N {} {
    # Identify the test
    tet_infoline directory_modify_27_N: Pass an invalid name to the master.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/hosts -master pokey_ch} msg] $msg
    } compare_strings {1 {Invalid name}}

    # Cleanup steps
}

proc directory_modify_28_N {} {
    global DCP_ROOT_CH

    # Identify the test
    tet_infoline directory_modify_28_N: Pass an invalid name to the exclude option.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/hosts -master /.:/$DCP_ROOT_CH -exclude gumby_ch} msg] $msg
    } compare_strings {1 {Invalid name}}

    # Cleanup steps
}

proc directory_modify_29_N {} {
    global DCP_ROOT_CH DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline directory_modify_29_N: Pass an invalid name to the exclude option.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/hosts -master /.:/$DCP_ROOT_CH -exclude /.:/gumby_ch} msg] $msg
    } compare_strings "1 {No replica exists at clearinghouse '$DCP_CELLNAME_ONE/gumby_ch'.}"

    # Cleanup steps
}

proc directory_modify_30_N {} {
    global DCP_ROOT_CH

    # Identify the test
    tet_infoline directory_modify_30_N: Pass an invalid name to the readonly option.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/hosts -master /.:/$DCP_ROOT_CH -readonly gumby_ch} msg] $msg
    } compare_strings {1 {Invalid name}}

    # Cleanup steps
}

proc directory_modify_31_N {} {
    global DCP_ROOT_CH DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline directory_modify_31_N: Pass an invalid name to the readonly option.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/hosts -master /.:/$DCP_ROOT_CH -readonly /.:/gumby_ch} msg] $msg
    } compare_strings "1 {No replica exists at clearinghouse '$DCP_CELLNAME_ONE/gumby_ch'.}"

    # Cleanup steps
}

proc directory_modify_32_N {} {
    global DCP_ROOT_CH

    # Identify the test
    tet_infoline directory_modify_32_N: Pass an invalid directory name.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify gumby -master /.:/$DCP_ROOT_CH} msg] $msg
    } compare_strings {1 {Invalid name}}

    # Cleanup steps
}

proc directory_modify_33_N {} {
    global DCP_ROOT_CH

    # Identify the test
    tet_infoline directory_modify_33_N: Pass an invalid directory name.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/gumby -master /.:/$DCP_ROOT_CH} msg] $msg
    } compare_strings {1 {Requested entry does not exist}}

    # Cleanup steps
}

proc directory_modify_34_N {} {
    global DCP_ROOT_CH DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline directory_modify_34_N: Forget to pass in one of the clearinghouse names.

    # Setup steps

    # Run the test
    test {
        list [catch {directory modify /.:/hosts -master /.:/gumby_ch} msg] $msg
    } compare_strings "1 {The replica at clearinghouse '$DCP_CELLNAME_ONE/$DCP_ROOT_CH' was not specified.}"

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 "
set ic1 "directory_modify_1_N"
set ic2 "directory_modify_2_N"
set ic3 "directory_modify_3_N"
set ic4 "directory_modify_4_N"
set ic5 "directory_modify_5_N"
set ic6 "directory_modify_6_N"
set ic7 "directory_modify_7_N"
set ic8 "directory_modify_8_N"
set ic9 "directory_modify_9_N"
set ic10 "directory_modify_10_N"
set ic11 "directory_modify_11_N"
set ic12 "directory_modify_12_N"
set ic13 "directory_modify_13_N"
set ic14 "directory_modify_14_N"
set ic15 "directory_modify_15_N"
set ic16 "directory_modify_16_N"
set ic17 "directory_modify_17_N"
set ic18 "directory_modify_18_N"
set ic19 "directory_modify_19_N"
set ic20 "directory_modify_20_N"
set ic21 "directory_modify_21_N"
set ic22 "directory_modify_22_N"
set ic23 "directory_modify_23_N"
set ic24 "directory_modify_24_N"
set ic25 "directory_modify_25_N"
set ic26 "directory_modify_26_N"
set ic27 "directory_modify_27_N"
set ic28 "directory_modify_28_N"
set ic29 "directory_modify_29_N"
set ic30 "directory_modify_30_N"
set ic31 "directory_modify_31_N"
set ic32 "directory_modify_32_N"
set ic33 "directory_modify_33_N"
set ic34 "directory_modify_34_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
