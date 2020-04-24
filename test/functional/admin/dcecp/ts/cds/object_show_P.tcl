#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: object_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:14:50  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:32  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:46  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:53  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:26  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:30  melman]
# 
# Revision 1.1.2.5  1994/09/02  17:49:21  kevins
# 	OT 10617 modified fvt test messages
# 	[1994/09/02  15:13:49  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:12:27  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:53  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:26  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:19:51  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:37  melman
# 	updating to new API path
# 	[1994/05/06  15:20:34  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:26  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:03  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc obj_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc obj_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}


proc object_show_3_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW VERBOSE


    # Identify the test
    tet_infoline object_show_3_P: Check to see that we can show an object.

    # Setup steps
   if {[dcp_wrap {object create /.:/objsh1}] !=0 } {return;}

    # Run the test
    test { object show /.:/objsh1 } dcp_find_attr CDS_UTS

    # Cleanup steps
   if {[dcp_wrap {object delete /.:/objsh1}] !=0 } {return;}
	

}

proc object_show_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline object_show_4_P: Check to see that we can show a list of objects.

    # Setup steps
    if {[dcp_wrap {object create {/.:/objsh1 /.:/objsh2}}] !=0 } {return;}

    # Run the test
    test { object show {/.:/objsh1 /.:/objsh2} } dcp_find_attr CDS_CTS

    # Cleanup steps
   if {[dcp_wrap {object delete {/.:/objsh1 /.:/objsh2}}] !=0 } {return;}
	
}

# The following code will setup the list of functions which TET will execute.
# The variable iclist should contain a list of all the invocable components
# in the testset; these are named ic1, ic2 ... etc.  For each invocable
# component thus specified, the user should define a variable should contain
# the name(s) of the test purposes associated with each invocable component;
# for example:
# iclist="ic1 ic2"
# ic1="test1-1 test1-2 test1-3"
# ic2="test2-1 test2-2"
#
# To execute a specific invocable component (or components) give the NUMBER
# of the component as an argument to this script.


set iclist "ic1 ic2"
set ic1 "object_show_3_P"
set ic2 "object_show_4_P"

set tet_startup obj_startup
set tet_cleanup obj_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
