#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: object_create_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:14:29  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:19  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:40  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:48  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:17  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:21  melman]
# 
# Revision 1.1.2.5  1994/09/02  17:49:16  kevins
# 	OT 10617 modified fvt test messages
# 	[1994/09/02  15:13:42  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:12:01  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:47  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:18  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:19:26  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:31  melman
# 	updating to new API path
# 	[1994/05/06  15:20:25  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:19  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:56  kevins]
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
## Test purposes

proc object_create_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline object_create_5_P: Check to see that we can create the object.
    # Setup steps

    # Run the test
    test { object create /.:/objcr1 
	dcp_found [directory list /.: -obj -simple] objcr1
    } compare_strings found

    # Cleanup steps

    if {[dcp_wrap {object delete /.:/objcr1}] !=0 } {return;}



}
proc object_create_6_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline object_create_6_P: Check handling of -attribute option.
    # Setup steps

    # Run the test
    test { object create /.:/objcr1 -attribute {CDS_ClassVersion 1.0}
      attrlist getvalues [object show /.:/objcr1] -type CDS_ClassVersion
    } compare_strings 1.0

    # Cleanup steps

    if {[dcp_wrap {object delete /.:/objcr1}] !=0 } {return;}

}
proc object_create_7_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline object_create_7_P: Check to see that we can create a list of objects.
    # Setup steps

    # Run the test
    test { 
	object create {/.:/objcr1 /.:/objcr2}
        dcp_found [directory list /.: -object -simple] {objcr1 objcr2}
    } compare_strings found

    # Cleanup steps


    if {[dcp_wrap {object delete {/.:/objcr2 /.:/objcr1}}] !=0 } {return;}


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

set iclist "ic1 ic2 ic3"

set ic1 "object_create_5_P"
set ic2 "object_create_6_P"
set ic3 "object_create_7_P"

set tet_startup obj_startup
set tet_cleanup obj_cleanup

# Source the TET main script into our script. The TET main script is what 
# will do the actual function calling.

source $TET_ROOT/lib/tcl/tcm.tcl
