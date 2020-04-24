#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: audtrail_show_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:11:34  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:22  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:49  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:57  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:20  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:00  melman]
# 
# Revision 1.1.2.5  1994/09/02  20:28:42  rousseau
# 	Mass test cleanup.
# 	[1994/09/02  20:28:00  rousseau]
# 
# Revision 1.1.2.4  1994/06/10  20:03:25  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:02  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:11  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:17:16  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:27:12  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:04:08  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:29:03  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:39  rousseau]
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
proc audtrail_show_startup {} {
}

proc audtrail_show_cleanup {} {
}    


proc audtrail_show_1_N {} {
    global DCP_AUD_TRAIL_FILE_1

    # Identify the test
    tet_infoline audtrail_show_1_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {audtrail show {$DCP_AUD_TRAIL_FILE_1} foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc audtrail_show_2_N {} {
    global DCP_AUD_TRAIL_FILE_1

    # Identify the test
    tet_infoline audtrail_show_2_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {audtrail show {$DCP_AUD_TRAIL_FILE_1} -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "audtrail_show_1_N"
set ic2 "audtrail_show_2_N"
set tet_startup audtrail_show_startup
set tet_cleanup audtrail_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

