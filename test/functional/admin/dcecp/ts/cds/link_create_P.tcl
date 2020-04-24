#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: link_create_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:14:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:59  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:33  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:42  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:02  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:10  melman]
# 
# Revision 1.1.2.6  1994/09/02  18:42:09  kevins
# 	OT 10617 fixed error messages
# 	[1994/09/02  18:29:15  kevins]
# 
# Revision 1.1.2.5  1994/08/26  14:24:56  kevins
# 	OT 11869 update for switch from -linkto to -to
# 	[1994/08/26  14:06:20  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:55  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:34  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:07  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:18:57  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:22  melman
# 	updating to new API path
# 	[1994/05/06  15:20:14  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:08  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:47  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW CELL_NAME
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set CELL_NAME [name expand /.:]}] != 0} {return;}
}

proc registry_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}



## Test purposes
proc link_create_8_P {} {
    global TET_ROOT CELL_NAME linkcr

    # Identify the test
    tet_infoline link_create_8_P: Check to see that we can successfully create a link.

    # Setup steps
        # Must be running as cell administrator.
    if {[dcp_wrap {set linkcr $CELL_NAME/linkcr}] != 0} {return;}
   
    # Run the test
    test { link create $linkcr -to /.:/hosts 
	  directory list /.: -link 
         } dcp_find_pgo $linkcr

    # Cleanup steps
        # The link must be deleted from the cds namespace.
    if {[dcp_wrap {link delete $linkcr}] != 0} {return;}

}

proc link_create_9_P {} {
    global TET_ROOT CELL_NAME linkcr

    # Identify the test
    tet_infoline link_create_9_P: Check to see that we can successfully create a link with a 
    tet_infoline link_create_9_P: specific timeout. 

    # Setup steps
        # Must be running as cell administrator.

    if {[dcp_wrap {set linkcr $CELL_NAME/linkcr}] != 0} {return;}
    # Run the test
    test { link create $linkcr -to /.:/hosts 
         directory list /.: -link
         } dcp_find_pgo $linkcr

        # Expected results are:
        # Successful completion

    # Cleanup steps
        # The link must be deleted from the cds namespace.
    if {[dcp_wrap {link delete $linkcr}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "link_create_8_P"
set ic2 "link_create_9_P"

set tet_startup registry_modify_startup
set tet_cleanup registry_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
