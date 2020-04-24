#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: attrlist_getvalues_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:26:40  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:56  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:24  root
# 	Submit
# 	[1995/12/11  14:32:02  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:57  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:52  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:01  melman]
# 
# Revision 1.1.2.2  1994/08/02  18:19:51  coren
# 	Corrected to not expect separate lists for multiple instances (CR 11297).
# 	[1994/08/02  17:59:15  coren]
# 
# Revision 1.1.2.1  1994/06/27  22:18:04  coren
# 	Initial version.
# 	[1994/06/27  20:02:26  coren]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc attrlist_getvalues_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc attrlist_getvalues_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc attrlist_getval_6_P {} {
    # Identify the test
    tet_infoline attrlist_getval_6_P: Check that a single value is returned correctly.

    # Setup steps

    # Run the test
      test {attrlist getvalues {{a 1} {b 2}} -type a
           } compare_strings {1}

    # Cleanup steps
}

proc attrlist_getval_7_P {} {
    # Identify the test
    tet_infoline attrlist_getval_7_P: Check that a multiple values are returned as a list.

    # Setup steps

    # Run the test
      test { attrlist getvalues {{a 1 3} {b 2}} -type a
           } compare_strings {1 3}

    # Cleanup steps
}

proc attrlist_getval_8_P {} {
    # Identify the test
    tet_infoline attrlist_getval_8_P: Check that values for multiple instances of the same
    tet_infoline attrlist_getval_8_P: attribute are returned as a single list.

    # Setup steps

    # Run the test
      test { attrlist getvalues {{a 1 3} {b 2} {a 4}} -type a
           } compare_lists {1 3 4}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "attrlist_getval_6_P"
set ic2 "attrlist_getval_7_P"
set ic3 "attrlist_getval_8_P"
set tet_startup attrlist_getvalues_startup
set tet_cleanup attrlist_getvalues_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
