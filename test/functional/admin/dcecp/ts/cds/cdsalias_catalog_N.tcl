#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: cdsalias_catalog_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:12:03  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:28  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:56  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/11/07  17:04 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/07  17:00 UTC  truitt
# 	Correct execution of test if gdad is not running.
# 	[1995/12/11  15:52:05  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:23  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:03  melman]
# 
# Revision 1.1.2.1  1994/09/28  14:23:45  kevins
# 	OT 12378 Initial submission
# 	[1994/09/28  14:22:47  kevins]
# 
# 	OT 12378 cdsalias functional tests
# 	[1994/09/28  11:06:03  kevins]
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


proc cdsalias_catalog_1_N {} {
    # Identify the test
    tet_infoline cdsalias_catalog_1_N: Check to see that incomplete names are trapped as errors 

    # Setup steps

    # Run the test
    test {
        list [catch {cdsalias catalog foo} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc cdsalias_catalog_2_N {} {
    # Identify the test
    tet_infoline cdsalias_catalog_2_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {cdsalias catalog /.:/foo /.:/bar} msg] $msg
    } compare_strings {1 {Unrecognized argument '/.:/bar'.}}

    # Cleanup steps
}

proc cdsalias_catalog_3_N {} {
    # Identify the test
    tet_infoline cdsalias_catalog_3_N: Check that extra options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {cdsalias catalog foo -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}

proc cdsalias_catalog_4_N {} {
    # Identify the test
    tet_infoline cdsalias_catalog_4_N: Check that a bogus cell name cannot be cataloged

    # Setup steps
    if {[catch {exec ps -ef | grep gdad | grep -v grep} msg] == 0} {
        tet_infoline setup: gdad is running.
        tet_infoline setup: The first part of the test will be used.
    } else {
        tet_infoline setup: gdad is not running.
        tet_infoline setup: The second part of the test will be used.
    }

    # Run the test
    if {[catch {exec ps -ef | grep gdad | grep -v grep} msg] == 0} {
        test {
            list [catch {cdsalias catalog /.../bogus_cell_name} msg] $msg
        } compare_strings {1 {Requested entry does not exist}}
    } else {
        test {
            list [catch {cdsalias catalog /.../bogus_cell_name} msg] $msg
        } compare_strings {1 {Requested operation would result in lost connectivity to root directory}}
    }

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "cdsalias_catalog_1_N"
set ic2 "cdsalias_catalog_2_N"
set ic3 "cdsalias_catalog_3_N"
set ic4 "cdsalias_catalog_4_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
