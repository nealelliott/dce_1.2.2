#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: audfilter_catalog_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:10:48  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:14  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:39  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:44  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:09  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:48  melman]
# 
# Revision 1.1.2.4  1994/06/10  20:02:42  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:52  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:34:57  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:16:36  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:26:58  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:04:03  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:47  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:27  rousseau]
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
proc audfilter_catalog_1_N {} {
    # Identify the test
    tet_infoline audfilter_catalog_1_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter catalog foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc audfilter_catalog_2_N {} {
    # Identify the test
    tet_infoline audfilter_catalog_2_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter catalog -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "audfilter_catalog_1_N"
set ic2 "audfilter_catalog_2_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

