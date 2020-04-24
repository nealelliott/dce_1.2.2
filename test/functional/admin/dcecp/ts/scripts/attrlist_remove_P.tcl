#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: attrlist_remove_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:26:56  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:06  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:34  root
# 	Submit
# 	[1995/12/11  14:32:08  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:04  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:59  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:08  melman]
# 
# Revision 1.1.2.1  1994/06/27  22:18:15  coren
# 	Initial version.
# 	[1994/06/27  20:02:43  coren]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc attrlist_remove_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc attrlist_remove_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc attrlist_remove_5_P {} {
    # Identify the test
    tet_infoline attrlist_remove_5_P: Check removal of a single attribute.

    # Setup steps

    # Run the test
    test { attrlist remove {{a 1} {b 2}} -member b
         } compare_strings {{a 1}}

    # Cleanup steps
}

proc attrlist_remove_6_P {} {
    # Identify the test
    tet_infoline attrlist_remove_6_P: Check removal of a multiple attributes.

    # Setup steps

    # Run the test
    test { attrlist remove {{a 1} {b 2} {c 3}} -member {a b}
         } compare_strings {{c 3}}

    # Cleanup steps
}

proc attrlist_remove_7_P {} {
    # Identify the test
    tet_infoline attrlist_remove_7_P: Check removal of a multiple instances of an attribute.

    # Setup steps

    # Run the test
    test { attrlist remove {{a 1} {b 2} {a 3}} -member {a}
         } compare_strings {{b 2}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "attrlist_remove_5_P"
set ic2 "attrlist_remove_6_P"
set ic3 "attrlist_remove_7_P"
set tet_startup attrlist_remove_startup
set tet_cleanup attrlist_remove_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
