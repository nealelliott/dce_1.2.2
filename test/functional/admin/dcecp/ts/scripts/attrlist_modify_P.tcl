#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: attrlist_modify_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:26:50  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:03  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:31  root
# 	Submit
# 	[1995/12/11  14:32:05  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:02  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:56  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:06  melman]
# 
# Revision 1.1.2.1  1994/06/27  22:18:12  coren
# 	Initial version.
# 	[1994/06/27  20:02:36  coren]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc attrlist_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc attrlist_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc attrlist_modify_7_P {} {
    # Identify the test
    tet_infoline attrlist_modify_7_P: Check that a value can be added with -add.

    # Setup steps

    # Run the test
    test { attrlist modify {{a 1 2} {b 2}} -add {{a 3}}
           } compare_strings {{a 1 2 3} {b 2}}

    # Cleanup steps
}

proc attrlist_modify_8_P {} {
    # Identify the test
    tet_infoline attrlist_modify_8_P: Check that a value can be added to multiple instances of
    tet_infoline attrlist_modify_8_P: an attribute with -add.

    # Setup steps

    # Run the test
    test { attrlist modify {{a 1 2} {b 2} {a 4}} -add {{a 3}}
         } compare_strings {{a 1 2 3} {b 2} {a 4 3}}

    # Cleanup steps
}

proc attrlist_modify_9_P {} {
    # Identify the test
    tet_infoline attrlist_modify_9_P: Check that multiple values can be added with -add.

    # Setup steps

    # Run the test
    test {attrlist modify {{a 1 2} {b 2}} -add {{a 3 4}}
         } compare_strings {{a 1 2 3 4} {b 2}}

    # Cleanup steps
}

proc attrlist_modify_10_P {} {
    # Identify the test
    tet_infoline attrlist_modify_10_P: Check that an entire attribute can be added with -add.

    # Setup steps

    # Run the test
    test { attrlist modify {{a 1 2} {b 2}} -add {{c 3}}
         } compare_strings {{a 1 2} {b 2} {c 3}}

    # Cleanup steps
}

proc attrlist_modify_11_P {} {
    # Identify the test
    tet_infoline attrlist_modify_11_P: Check that an entire attribute is replaced with -change.

    # Setup steps

    # Run the test
    test { attrlist modify {{a 1 2} {b 2}} -change {{a 3}}
         } compare_strings {{a 3} {b 2}}

    # Cleanup steps
}

proc attrlist_modify_12_P {} {
    # Identify the test
    tet_infoline attrlist_modify_12_P: Check that all instances of an attribute are replaced with -change.

    # Setup steps

    # Run the test
    test { attrlist modify {{a 1 2} {b 2} {a 4 5}} -change {{a 3}}
         } compare_strings {{a 3} {b 2} {a 3}}

    # Cleanup steps
}

proc attrlist_modify_13_P {} {
    # Identify the test
    tet_infoline attrlist_modify_13_P: Check that multiple attribute can be replaced with -change.

    # Setup steps

    # Run the test
    test { attrlist modify {{a 1 2} {b 2} {c 4 5}} -change {{a 3} {c 6}}
         } compare_strings {{a 3} {b 2} {c 6}}

    # Cleanup steps
}

proc attrlist_modify_14_P {} {
    # Identify the test
    tet_infoline attrlist_modify_14_P: Check that a single value can be removed with -remove.

    # Setup steps

    # Run the test
    test { attrlist modify {{a 1 2} {b 2} {c 4 5}} -remove {{a 2}}
         } compare_strings {{a 1} {b 2} {c 4 5}}

    # Cleanup steps
}

proc attrlist_modify_15_P {} {
    # Identify the test
    tet_infoline attrlist_modify_15_P: Check that a single value can be removed from multiple
    tet_infoline attrlist_modify_15_P: instances of an attribute with -remove.

    # Setup steps

    # Run the test
    test { attrlist modify {{a 1 2} {b 2} {a 4 2 5}} -remove {{a 2}}
         } compare_strings {{a 1} {b 2} {a 4 5}}

    # Cleanup steps
}

proc attrlist_modify_16_P {} {
    # Identify the test
    tet_infoline attrlist_modify_16_P: Check that an entire attribute can be removed with -remove.

    # Setup steps

    # Run the test
    test { attrlist modify {{a 1 2} {b 2} {c 4 5}} -remove {a}
         } compare_strings {{b 2} {c 4 5}}

    # Cleanup steps
}

proc attrlist_modify_17_P {} {
    # Identify the test
    tet_infoline attrlist_modify_17_P: Check that removing all an attribute's values removes the
    tet_infoline attrlist_modify_17_P: entire attribute.

    # Setup steps

    # Run the test
    test { attrlist modify {{a 1 2} {b 2} {c 4 5} {b 7 3 2}} -remove {{b 2}}
         } compare_strings {{a 1 2} {c 4 5} {b 7 3}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 "
set ic1 "attrlist_modify_7_P"
set ic2 "attrlist_modify_8_P"
set ic3 "attrlist_modify_9_P"
set ic4 "attrlist_modify_10_P"
set ic5 "attrlist_modify_11_P"
set ic6 "attrlist_modify_12_P"
set ic7 "attrlist_modify_13_P"
set ic8 "attrlist_modify_14_P"
set ic9 "attrlist_modify_15_P"
set ic10 "attrlist_modify_16_P"
set ic11 "attrlist_modify_17_P"
set tet_startup attrlist_modify_startup
set tet_cleanup attrlist_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
