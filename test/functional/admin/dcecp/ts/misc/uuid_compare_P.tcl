#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: uuid_compare_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:18:16  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:47  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:03  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:02  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:15  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:29  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:33  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:27:08  devsrc]
# 
# Revision 1.1.2.1  1994/06/02  18:50:35  kevins
# 	Initial Misc commands test drop
# 	[1994/06/02  18:44:25  kevins]
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
proc uuid_compare_5_P {} {
    # Identify the test
    tet_infoline uuid_compare_5_P: To check that two uuids that are not equal are checked properly

    # Setup steps

    # Run the test
    test {uuid compare [uuid create] [uuid create] } compare_strings {0}

    # Cleanup steps
}

proc uuid_compare_6_P {} {
    # Identify the test
    tet_infoline uuid_compare_6_P: To check that two uuids that are equal are checked properly

    # Setup steps
    if {[dcp_wrap {set uuid_1 [ uuid create ]}] != 0} {return;}

    # Run the test
    test { uuid compare $uuid_1 $uuid_1 } compare_strings {1}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "uuid_compare_5_P"
set ic2 "uuid_compare_6_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
