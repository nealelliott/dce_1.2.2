#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: cdsalias_set_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:12:34  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:41  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:03  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/06  18:37 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts33/1  1995/06/06  18:36 UTC  truitt
# 	CHFts15424: Change tests to reflect new dced acl behavior.
# 	[1995/12/11  15:52:13  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:31  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:10  melman]
# 
# Revision 1.1.2.1  1994/09/28  14:23:52  kevins
# 	OT 12378 Initial submission
# 	[1994/09/28  14:22:55  kevins]
# 
# 	OT 12378 cdsalias functional tests
# 	[1994/09/28  11:06:10  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc cdsalias_set_1_N {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_NOTROOT DCP_NOTROOT_PW

    # Identify the test
    tet_infoline cdsalias_set_1_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {group add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {account create $DCP_NOTROOT -group none -organization none -password $DCP_NOTROOT_PW -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_NOTROOT -password $DCP_NOTROOT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {cdsalias set /.:/foo} msg] $msg
    } compare_strings {1 {Insufficient rights to perform requested operation}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {account delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc cdsalias_set_2_N {} {
    # Identify the test
    tet_infoline cdsalias_set_2_N: Check to see that incomplete names are trapped as errors 

    # Setup steps

    # Run the test
    test {
        list [catch {cdsalias set foo} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc cdsalias_set_3_N {} {
    # Identify the test
    tet_infoline cdsalias_set_3_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {cdsalias set /.:/foo /.:/bar} msg] $msg
    } compare_strings {1 {Unrecognized argument '/.:/bar'.}}

    # Cleanup steps
}

proc cdsalias_set_4_N {} {
    # Identify the test
    tet_infoline cdsalias_set_4_N: Check that extra options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {cdsalias set foo -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "cdsalias_set_1_N"
set ic2 "cdsalias_set_2_N"
set ic3 "cdsalias_set_3_N"
set ic4 "cdsalias_set_4_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
