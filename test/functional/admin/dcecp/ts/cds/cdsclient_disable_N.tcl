#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: cdsclient_disable_N.tcl,v $
# Revision 1.1.2.3  1996/03/11  02:12:38  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:46  marty]
#
# Revision 1.1.2.2  1995/12/13  15:25:05  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/10/10  14:21 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts49/1  1995/10/10  14:19 UTC  truitt
# 	new test.
# 	[1995/12/11  15:52:16  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc cdsclient_disable_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc cdsclient_disable_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc cdsclient_disable_1_N {} {
    # Identify the test
    tet_infoline cdsclient_disable_1_N: Pass nothing to the command

    # Setup steps

    # Run the test
    test {
        list [catch {cdsclient disable} msg] $msg
    } compare_strings {1 {The name of the CDS client to operate on must be specified.}}

    # Cleanup steps
}

proc cdsclient_disable_2_N {} {
    # Identify the test
    tet_infoline cdsclient_disable_2_N: Pass an illegal argument to the command

    # Setup steps

    # Run the test
    test {
        list [catch {cdsclient disable gumby pokey} msg] $msg
    } compare_strings {1 {Unrecognized argument 'pokey'.}}

    # Cleanup steps
}

proc cdsclient_disable_3_N {} {
    # Identify the test
    tet_infoline cdsclient_disable_3_N: Pass an illegal option to the command

    # Setup steps

    # Run the test
    test {
        list [catch {cdsclient disable gumby -pokey} msg] $msg
    } compare_strings {1 {Unrecognized option '-pokey'.}}

    # Cleanup steps
}

proc cdsclient_disable_4_N {} {
    # Identify the test
    tet_infoline cdsclient_disable_4_N: Pass a truly bad name to the command.

    # Setup steps

    # Run the test
    test {
        list [catch {cdsclient disable gumby} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc cdsclient_disable_5_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline cdsclient_disable_5_N: Pass an unknown name to the command.

    # Setup steps

    # Run the test
    test {
        list [catch {cdsclient disable /.:/hosts/$DCP_HOSTNAME_ONE/cds-gumby} msg] $msg
    } compare_strings {1 {Entry not found}}

    # Cleanup steps
}

proc cdsclient_disable_6_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline cdsclient_disable_6_N: Pass the wrong name to the command.

    # Setup steps

    # Run the test
    test {
        list [catch {cdsclient disable /.:/hosts/$DCP_HOSTNAME_ONE/cds-server} msg] $msg
    } compare_strings {1 {Unknown interface}}

    # Cleanup steps
}

proc cdsclient_disable_7_N {} {
    # Identify the test
    tet_infoline cdsclient_disable_7_N: Pass two names to the command.

    # Setup steps

    # Run the test
    test {
        list [catch {cdsclient disable {gumby pokey}} msg] $msg
    } compare_strings {1 {Only one CDS client may be specified for this command.}}

    # Cleanup steps
}

proc cdsclient_disable_8_N {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_TWO

    # Identify the test
    tet_infoline cdsclient_disable_8_N: Check on a user without the rights to disable.

    # Setup steps
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    if {[catch {cdsclient disable /.:/hosts/$DCP_HOSTNAME_TWO/cds-clerk} msg] != 0} {
        tet_infoline Unable to disable remote client as expected
        tet_infoline $msg
        tet_result PASS
    } else {
        tet_infoline Able to disable remote client even without priviledges
        tet_result FAIL
    }

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 "
set ic1 "cdsclient_disable_1_N"
set ic2 "cdsclient_disable_2_N"
set ic3 "cdsclient_disable_3_N"
set ic4 "cdsclient_disable_4_N"
set ic5 "cdsclient_disable_5_N"
set ic6 "cdsclient_disable_6_N"
set ic7 "cdsclient_disable_7_N"
set ic8 "cdsclient_disable_8_N"
set tet_startup cdsclient_disable_startup
set tet_cleanup cdsclient_disable_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
