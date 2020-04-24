#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_rename_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:21:57  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:18  marty]
#
# Revision 1.1.4.1  1995/12/13  15:28:35  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:55:22  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:29  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:16  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:04:35  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:54  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:55  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:18  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:59  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:28:57  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:27  melman
# 	updating to new API path
# 	[1994/05/06  17:15:06  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:35  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:56  kevins]
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
proc principal_rename_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_rename_4_P: Check see that we can rename the principal.

    # Setup steps
        # Must be running as cell administrator.
    set principal_init [lindex [info level [info level]] 0]
    set principal_renamed [format "%s_new" $principal_init]
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $principal_init}] != 0} {return;}

    # Run the test
    test { principal rename $principal_init -to $principal_renamed
        if {[dcp_find_pgo [principal catalog -simplename] \
            $principal_renamed] == 0 } { set a "found" 
            } else {set b "not_found" } } compare_strings found

    # Cleanup steps
        # The principal foo1 needs to be deleted
    if {[dcp_wrap {principal delete $principal_renamed}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}


set iclist "ic1 "
set ic1 "principal_rename_4_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
