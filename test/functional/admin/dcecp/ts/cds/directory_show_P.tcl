#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:52  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:50  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:30  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:39  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:56  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:01  melman]
# 
# Revision 1.1.2.5  1994/09/02  13:56:52  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:49  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:27  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:31  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:00  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:18:42  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:19  melman
# 	updating to new API path
# 	[1994/05/06  15:20:09  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:04  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:45  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc directory_show_3_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_show_3_P: Check to see that we can show an directory.

    # Setup steps

    # Run the test
    test { dcp_found  [directory show /.:/hosts] CDS_UTS
         } compare_strings found

    # Cleanup steps
}

proc directory_show_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_show_4_P: Check to see that we can show a list of directories.

    # Setup steps

    # Run the test
    test {  dcp_found [directory show {/.:/hosts /.:/subsys}] CDS_UTS
         } compare_strings found

    # Cleanup steps

}

set iclist "ic1 ic2 "
set ic1 "directory_show_3_P"
set ic2 "directory_show_4_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
