#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: hostdata_show_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:26  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:02  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:20  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/27  13:32 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts36/1  1995/06/27  13:31 UTC  truitt
# 	CHFts15407: Add tests for new hostdata behavior.
# 	[1995/12/11  15:53:24  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:12  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:26  melman]
# 
# Revision 1.1.2.4  1994/09/20  15:47:30  rousseau
# 	Update tests (CR 12254).
# 	[1994/09/20  15:33:00  rousseau]
# 
# Revision 1.1.2.3  1994/06/17  18:43:19  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:26:02  devsrc]
# 
# Revision 1.1.2.2  1994/05/25  16:51:10  baum
# 	Hostdata tests now dce_login on their own
# 	[1994/05/25  16:49:28  baum]
# 
# Revision 1.1.2.1  1994/05/19  22:58:21  baum
# 	Initial checkin
# 	[1994/05/19  22:53:35  baum]
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
proc hostdata_show_1_N {} {
    # Identify the test
    tet_infoline hostdata_show_1_N: Check that insufficient arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata show} msg] $msg
    } compare_strings {1 {'dced' object not specified.}}

    # Cleanup steps
}

proc hostdata_show_2_N {} {
    # Identify the test
    tet_infoline hostdata_show_2_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata show gumby -entry foo} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc hostdata_show_3_N {} {
    # Identify the test
    tet_infoline hostdata_show_3_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata show gumby -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc hostdata_show_4_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline hostdata_show_4_N: Check that wrong object name are returned as not found. 

    # Setup steps
    dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}

    # Run the test
    test {
        list [catch {hostdata show gumby} msg] $msg
    } compare_strings {1 {The object 'gumby' does not exist.}}

    # Cleanup steps
    dcp_wrap {logout}
}

proc hostdata_show_5_N {} {
    # Identify the test
    tet_infoline hostdata_show_5_N: Check that not specified interface name is an error.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata show cell_name -ifname} msg] $msg
    } compare_strings {1 {The '-ifname' option requires a character string value.}}

    # Cleanup steps
}

proc hostdata_show_6_N {} {
    global DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER

    # Identify the test
    tet_infoline hostdata_show_6_N: Check that string binding without interface is an error.

    # Setup steps

    # Run the test
    test {
        list [catch "hostdata show $DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER" msg] $msg
    } compare_strings "1 {The object '$DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER' does not exist.}"

    # Cleanup steps
}

proc hostdata_show_7_N {} {
    global DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER

    # Identify the test
    tet_infoline hostdata_show_7_N: Check that string binding with bad interface is an error.

    # Setup steps

    # Run the test
    test {
        list [catch "hostdata show $DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER -ifname gumby" msg] $msg
    } compare_strings {1 {The object 'gumby' does not exist.}}

    # Cleanup steps
}

proc hostdata_show_8_N {} {
    global DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER

    # Identify the test
    tet_infoline hostdata_show_8_N: Check that too many ifnames is an error.

    # Setup steps

    # Run the test
    test {
        list [catch "hostdata show $DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER -ifname {gumby pokey}" msg] $msg
    } compare_strings {1 {More than one interface name has been specified.}}

    # Cleanup steps
}

proc hostdata_show_9_N {} {
    global DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER

    # Identify the test
    tet_infoline hostdata_show_9_N: Check that too many string bindings is an error.

    # Setup steps

    # Run the test
    test {
        list [catch "hostdata show {$DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER $DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER} -ifname cell_name" msg] $msg
    } compare_strings {1 {Invalid network address}}

    # Cleanup steps
}

proc hostdata_show_10_N {} {
    global DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER

    # Identify the test
    tet_infoline hostdata_show_10_N: Check that combining string binding and binary is an error.

    # Setup steps

    # Run the test
    test {
        list [catch "hostdata show $DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER -ifname cell_name -binary" msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}

proc hostdata_show_11_N {} {
    # Identify the test
    tet_infoline hostdata_show_11_N: Check that a name cannot be used with ifname.

    # Setup steps

    # Run the test
    test {
        list [catch "hostdata show cell_name -ifname cell_name" msg] $msg
    } compare_strings {1 {Invalid RPC protocol sequence}}

    # Cleanup steps
}

proc hostdata_show_12_N {} {
    # Identify the test
    tet_infoline hostdata_show_12_N: Check that entry and binary cannot be used together.

    # Setup steps

    # Run the test
    test {
        list [catch "hostdata show cell_name -entry -binary" msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 "
set ic1 "hostdata_show_1_N"
set ic2 "hostdata_show_2_N"
set ic3 "hostdata_show_3_N"
set ic4 "hostdata_show_4_N"
set ic5 "hostdata_show_5_N"
set ic6 "hostdata_show_6_N"
set ic7 "hostdata_show_7_N"
set ic8 "hostdata_show_8_N"
set ic9 "hostdata_show_9_N"
set ic10 "hostdata_show_10_N"
set ic11 "hostdata_show_11_N"
set ic12 "hostdata_show_12_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
