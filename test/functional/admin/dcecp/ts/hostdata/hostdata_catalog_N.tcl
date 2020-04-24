#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: hostdata_catalog_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:07  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:42  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:13  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/27  13:29 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts36/1  1995/06/27  13:27 UTC  truitt
# 	CHFts15407: Add tests for new hostdata behavior.
# 	[1995/12/11  15:53:18  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:56  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:13  melman]
# 
# Revision 1.1.2.3  1994/09/01  15:43:59  baum
# 	Fixed output messages in negative tests (OT 11093)
# 	[1994/09/01  15:42:31  baum]
# 
# Revision 1.1.2.2  1994/06/17  18:43:13  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:25:24  devsrc]
# 
# Revision 1.1.2.1  1994/05/19  22:58:17  baum
# 	Initial checkin
# 	[1994/05/19  22:53:26  baum]
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
proc hostdata_catalog_1_N {} {
    # Identify the test
    tet_infoline hostdata_catalog_1_N: Check that insufficient/invalid arguments trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata catalog foo} msg] $msg
    } compare_strings {1 {Bad or wrong arguments}}

    # Cleanup steps
}

proc hostdata_catalog_2_N {} {
    # Identify the test
    tet_infoline hostdata_catalog_2_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata catalog -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc hostdata_catalog_3_N {} {
    # Identify the test
    tet_infoline hostdata_catalog_3_N: Check that a bad hostname is an error.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata catalog hosts/gumby} msg] $msg
    } compare_strings {1 {Entry not found}}

    # Cleanup steps
}

proc hostdata_catalog_4_N {} {
    # Identify the test
    tet_infoline hostdata_catalog_4_N: Check that a bad string binding is an error.

    # Setup steps

    # Run the test
    test {
        list [catch {hostdata catalog ncacn_ip_udp:15.22.46.777} msg] $msg
    } compare_strings {1 {Bad or wrong arguments}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "hostdata_catalog_1_N"
set ic2 "hostdata_catalog_2_N"
set ic3 "hostdata_catalog_3_N"
set ic4 "hostdata_catalog_4_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
