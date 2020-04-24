#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: audfilter_show_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:11:25  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:21  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:47  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:54  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:18  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:57  melman]
# 
# Revision 1.1.2.5  1994/09/02  20:28:41  rousseau
# 	Mass test cleanup.
# 	[1994/09/02  20:27:58  rousseau]
# 
# Revision 1.1.2.4  1994/06/10  20:03:23  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:00  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:09  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:17:08  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:27:10  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:04:07  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:58  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:36  rousseau]
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
proc audfilter_show_startup {} {
}

proc audfilter_show_cleanup {} {
}    


proc audfilter_show_1_N {} {
    # Identify the test
    tet_infoline audfilter_show_1_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter show {principal audfilter_show_1_N} foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc audfilter_show_2_N {} {
    # Identify the test
    tet_infoline audfilter_show_2_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter show {principal audfilter_show_2_N} -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "audfilter_show_1_N"
set ic2 "audfilter_show_2_N"
set tet_startup audfilter_show_startup
set tet_cleanup audfilter_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

