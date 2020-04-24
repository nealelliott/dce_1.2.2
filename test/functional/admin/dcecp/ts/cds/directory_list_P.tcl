#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_list_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:31  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:22  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:33  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:48  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:45  melman]
# 
# Revision 1.1.2.5  1994/09/02  13:56:45  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:36  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:17  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:25  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:55  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:18:20  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:15  melman
# 	updating to new API path
# 	[1994/05/06  15:20:01  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:20:51  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:38  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc directory_list_3_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_list_3_P: Check to see that we can list the contents of a directory.

    # Setup steps

    # Run the test
    test { dcp_found [directory list /.: -simplename] subsys
         } compare_strings found

}

proc directory_list_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_list_4_P: Check to see that we can list the directory objects of a directory

    # Setup steps
        # Must be running as cell administrator.

    # Run the test
    test { dcp_missing [directory list /.: -directories -simple ] sec
         } compare_strings {not found}

        # Expected results are:
        # Successful completion

    # Cleanup steps
}

proc directory_list_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_list_5_P: Check to see that we can list the objects of a directory.

    # Setup steps


    # Run the test
    test { dcp_missing [directory list /.: -object] /.:/hosts
	} compare_strings {not found}

    # Cleanup steps
}

proc directory_list_6_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_list_6_P: Check to see that we can list the link objects of a directory.

    # Setup steps
        # Must be running as cell administrator.

    # Run the test
    test {  directory list /.: -link } compare_strings {}

    # Cleanup steps
}

proc directory_list_7_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_list_7_P: Check to see that we can list the simplenames of entries in a 
    tet_infoline directory_list_7_P: directory.

    # Setup steps
        # Must be running as cell administrator.

    # Run the test
    test { dcp_found [directory list /.: -simplename] hosts
         } compare_strings found


    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "directory_list_3_P"
set ic2 "directory_list_4_P"
set ic3 "directory_list_5_P"
set ic4 "directory_list_6_P"
set ic5 "directory_list_7_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
