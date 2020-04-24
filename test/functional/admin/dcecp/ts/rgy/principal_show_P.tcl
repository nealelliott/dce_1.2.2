#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_show_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:22:01  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:19  marty]
#
# Revision 1.1.4.1  1995/12/13  15:28:38  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:55:24  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:32  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:18  melman]
# 
# Revision 1.1.2.6  1994/09/21  17:11:14  kevins
# 	OT 9938 Modified pgo functional tests
# 	[1994/09/21  17:10:54  kevins]
# 
# Revision 1.1.2.5  1994/09/01  17:49:38  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:56  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:57  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:20  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:40:03  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:29:05  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:29  melman
# 	updating to new API path
# 	[1994/05/06  17:15:09  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:38  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:58  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc principal_show_startup {} {
     global DCP_CLIENT DCP_CLIENT_PW
     if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc principal_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}


## Test purposes
proc principal_show_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_show_4_P: Check to see that we can show the principal attributes.

    # Setup steps
        # Must be running as cell administrator. 
        # The principal foo must exist.
    if {[dcp_wrap {set name [lindex [info level [info level]] 0]
            principal create $name}] != 0} {return;}

    # Run the test
    test { catch {principal show $name} foo_attr_list
           dcp_found $foo_attr_list {uid uuid alias quota}
    	} compare_strings found

    # Cleanup steps
        # The principal foo needs to be removed from the registry.
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc principal_show_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_show_5_P: Check to see that we can show a list of principal's attributes.

    # Setup steps
        # Must be running as cell administrator. 
        # The principal foo and foo_bar must exist.
    if {[dcp_wrap {set name [lindex [info level [info level]] 0]
                   principal create "$name bar_$name"}] != 0} {return;}

    # Run the test
        test { catch {principal show "$name bar_$name"} attr_list
           dcp_found $attr_list {uid uuid alias quota}
    	} compare_strings found


        # Expected results are:
        # Successful completion.

    # Cleanup steps
        # The principal foo needs to be removed from the registry.
    if {[dcp_wrap {principal delete "$name bar_$name"}] != 0} {return;}

}


set iclist "ic1 ic2 "
set ic1 "principal_show_4_P"
set ic2 "principal_show_5_P"

set tet_startup principal_show_startup
set tet_cleanup principal_show_cleanup


# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
