#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: name_compare_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:56  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:39  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:55  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:55  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:04  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:18  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:28  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:26:44  devsrc]
# 
# Revision 1.1.2.1  1994/06/02  18:50:14  kevins
# 	Initial Misc commands test drop
# 	[1994/06/02  18:43:52  kevins]
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
proc name_compare_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline name_compare_4_P: Check that names that are not equal are noted as such.

    # Setup steps

    # Run the test
    test { name compare /.:/hosts /.:/subsys } compare_strings {0}

    # Cleanup steps
}

proc name_compare_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline name_compare_5_P: Check that names that are equal are noted as such.

    # Setup steps

    # Run the test
    test { name compare /.:/hosts /.:/hosts } compare_strings {1}

        # Expected results are:
        # 1

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "name_compare_4_P"
set ic2 "name_compare_5_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
