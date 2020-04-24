#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_remove_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:46  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:45  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:28  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:37  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:54  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:56  melman]
# 
# Revision 1.1.2.5  1994/09/02  13:56:50  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:46  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:25  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:29  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:58  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:18:34  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:18  melman
# 	updating to new API path
# 	[1994/05/06  15:20:06  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:01  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:43  kevins]
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
proc directory_remove_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_remove_5_P: Check to see that we can remove the child pointer.

    # Setup steps
        # Must be running as cell administrator
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/dirrem}] != 0} {return;}

    # Run the test
    test { directory remove /.: -member dirrem 
        dcp_missing [directory list /.:] /.:/dirrem
         } compare_strings {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/dirrem}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}



set iclist "ic1 "
set ic1 "directory_remove_5_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
