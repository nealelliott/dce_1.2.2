#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_delete_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:21:37  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:09  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:23  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:55:14  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:16  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:06  melman]
# 
# Revision 1.1.2.5  1994/09/01  17:49:35  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:53  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:52  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:14  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:54  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:28:42  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:23  melman
# 	updating to new API path
# 	[1994/05/06  17:15:01  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:30  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:51  kevins]
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
proc principal_delete_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_delete_4_P: Check to see that we can delete a principal.

    # Setup steps
        # Must be running as cell administrator. 
        # Principal needs to exist in the registry.
    if {[dcp_wrap {login $DCP_CLIENT -passw $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create foo}] != 0} {return;}

    # Run the test
     test { principal delete foo 
	    dcp_missing [principal catalog -simplename] foo 
          } compare_strings {not found}

        # Expected results are:
        # Successful completion.

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc principal_delete_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_delete_5_P: Check to see that we can delete a list of principals.

    # Setup steps
        # Must be running as cell administrator. 
        # Principals needs to exist in the registry.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create {foo foo1}}] != 0} {return;}

    # Run the test
    test { principal delete {foo foo1} 
	dcp_missing [principal catalog -simplename] {foo foo1}
    } compare_strings {not found}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "principal_delete_4_P"
set ic2 "principal_delete_5_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
