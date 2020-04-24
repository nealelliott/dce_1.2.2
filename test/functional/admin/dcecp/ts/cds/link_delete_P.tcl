#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: link_delete_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:14:11  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:03  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:35  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:43  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:06  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:13  melman]
# 
# Revision 1.1.2.5  1994/09/02  18:42:10  kevins
# 	OT 10617 fixed error messages
# 	[1994/09/02  18:29:17  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:56  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:41  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:12  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:19:04  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:26  melman
# 	updating to new API path
# 	[1994/05/06  15:20:17  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:11  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:49  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc link_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc link_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc link_delete_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline link_delete_4_P: Check to see that we can successfully delete a softlink.

    # Setup steps
        # Must be running as cell administrator.
        # The link /.:/linkde must exist in the cds namespace
    if {[dcp_wrap {link create /.:/linkde -to /.:/hosts}] != 0} {return;}

    # Run the test
    test { link delete /.:/linkde
         dcp_missing [directory list /.: -link -simple] linkde
          } compare_strings {not found}
 

    # Cleanup steps
}

proc link_delete_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline link_delete_5_P: Check to see that we can successfully delete a list of softlinks.

    # Setup steps
        # Must be running as cell administrator.
        # The links /.:/linkde and /.:/linkde2 must exist in the cds 
        # namespace
    if {[dcp_wrap {link create /.:/linkde -to /.:/hosts }] != 0} {return;}
    if {[dcp_wrap {link create /.:/linkde2 -to /.:/hosts }] != 0} {return;}

    # Run the test
    test { link delete {/.:/linkde /.:/linkde2}
          dcp_missing [directory list /.: -link -simple] {linkde linkde2}
         } compare_strings {not found}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "link_delete_4_P"
set ic2 "link_delete_5_P"

set tet_startup link_startup
set tet_cleanup link_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
