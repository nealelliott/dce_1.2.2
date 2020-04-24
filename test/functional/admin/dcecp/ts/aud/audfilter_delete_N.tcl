#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: audfilter_delete_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:11:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:17  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:43  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:49  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:13  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:53  melman]
# 
# Revision 1.1.2.5  1994/09/02  20:28:38  rousseau
# 	Mass test cleanup.
# 	[1994/09/02  20:27:55  rousseau]
# 
# Revision 1.1.2.4  1994/06/10  20:03:18  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:56  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:06  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:16:52  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:27:04  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:24:47  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:53  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:31  rousseau]
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
proc audfilter_delete_startup {} {
}

proc audfilter_delete_cleanup {} {
}    


proc audfilter_delete_1_N {} {
    # Identify the test
    tet_infoline audfilter_delete_1_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter delete {principal foo} foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc audfilter_delete_2_N {} {
    # Identify the test
    tet_infoline audfilter_delete_2_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter delete {principal foo} -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "audfilter_delete_1_N"
set ic2 "audfilter_delete_2_N"
set tet_startup audfilter_delete_startup
set tet_cleanup audfilter_delete_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

