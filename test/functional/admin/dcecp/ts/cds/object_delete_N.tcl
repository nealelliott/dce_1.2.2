#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: object_delete_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:14:32  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:21  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:41  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:49  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:20  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:22  melman]
# 
# Revision 1.1.2.5  1994/09/02  17:49:16  kevins
# 	OT 10617 modified fvt test messages
# 	[1994/09/02  15:13:43  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:12:02  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:48  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:19  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:19:30  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:31  melman
# 	updating to new API path
# 	[1994/05/06  15:20:26  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:20  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:57  kevins]
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
proc object_delete_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object_delete_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {object delete /.:/test_1 foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc object_delete_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline object_delete_2_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    login $DCP_CLIENT -password $DCP_CLIENT_PW

    object create /.:/test_1    

    logout

    # Run the test
    test {
        list [catch {object delete /.:/test_1} msg] $msg
    } compare_strings {1 {Insufficient rights to perform requested operation}}

    # Cleanup steps

    login $DCP_CLIENT -password $DCP_CLIENT_PW

    object delete /.:/test_1    

    logout
}

proc object_delete_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object_delete_3_N: Check that missing arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {object delete} msg] $msg
    } compare_strings {1 {No object name.}}

    # Cleanup steps
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
set ic1 "object_delete_1_N"
set ic2 "object_delete_2_N"
set ic3 "object_delete_3_N"

# Source the TET main script into our script. The TET main script is what 
# will do the actual function calling.

source $TET_ROOT/lib/tcl/tcm.tcl
