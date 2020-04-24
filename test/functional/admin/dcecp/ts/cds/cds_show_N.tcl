#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: cds_show_N.tcl,v $
# Revision 1.1.2.3  1996/03/11  02:11:54  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:26  marty]
#
# Revision 1.1.2.2  1995/12/13  15:24:54  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/10/10  14:25 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts49/1  1995/10/10  14:24 UTC  truitt
# 	new test
# 	[1995/12/11  15:52:03  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc cds_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc cds_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc cds_show_1_N {} {
    # Identify the test
    tet_infoline cds_show_1_N: Pass nothing to the command.

    # Setup steps

    # Run the test
    test {
        list [catch {cds show} msg] $msg
    } compare_strings {1 {The name of the CDS server to operate on must be specified.}}

    # Cleanup steps
}

proc cds_show_2_N {} {
    # Identify the test
    tet_infoline cds_show_2_N: Pass in an illegal argument.

    # Setup steps

    # Run the test
    test {
        list [catch {cds show gumby pokey} msg] $msg
    } compare_strings {1 {Unrecognized argument 'pokey'.}}

    # Cleanup steps
}

proc cds_show_3_N {} {
    # Identify the test
    tet_infoline cds_show_3_N: Pass in an illegal option.

    # Setup steps

    # Run the test
    test {
        list [catch {cds show gumby -pokey} msg] $msg
    } compare_strings {1 {Unrecognized option '-pokey'.}}

    # Cleanup steps
}

proc cds_show_4_N {} {
    # Identify the test
    tet_infoline cds_show_4_N: Pass in a truly bad name to the command.

    # Setup steps

    # Run the test
    test {
        list [catch {cds show gumby} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc cds_show_5_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline cds_show_5_N: Pass in an unknown name to the command.

    # Setup steps

    # Run the test
    test {
        list [catch {cds show /.:/hosts/$DCP_HOSTNAME_ONE/cds-gumby} msg] $msg
    } compare_strings {1 {Entry not found}}

    # Cleanup steps
}

proc cds_show_6_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline cds_show_6_N: Pass the wrong name to the command.

    # Setup steps

    # Run the test
    test {
        list [catch {cds show /.:/hosts/$DCP_HOSTNAME_ONE/cds-clerk} msg] $msg
    } compare_strings {1 {Unknown interface}}

    # Cleanup steps
}

proc cds_show_7_N {} {
    # Identify the test
    tet_infoline cds_show_7_N: Pass two names to the command.

    # Setup steps

    # Run the test
    test {
        list [catch {cds show {gumby pokey}} msg] $msg
    } compare_strings {1 {Only one CDS server may be specified for this command.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 "
set ic1 "cds_show_1_N"
set ic2 "cds_show_2_N"
set ic3 "cds_show_3_N"
set ic4 "cds_show_4_N"
set ic5 "cds_show_5_N"
set ic6 "cds_show_6_N"
set ic7 "cds_show_7_N"
set tet_startup cds_show_startup
set tet_cleanup cds_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
