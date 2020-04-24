#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: object_create_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:14:26  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:17  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:39  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:47  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:15  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:19  melman]
# 
# Revision 1.1.2.5  1994/09/02  17:49:14  kevins
# 	OT 10617 modified fvt test messages
# 	[1994/09/02  15:13:41  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:12:01  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:46  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:17  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:19:22  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:30  melman
# 	updating to new API path
# 	[1994/05/06  15:20:24  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:17  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:55  kevins]
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

proc object_create_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object_create_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {object create /.:/test_1 foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc object_create_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object_create_2_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {object create /.:/test_1 -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc object_create_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object_create_3_N: Check that missing arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {object create } msg] $msg
    } compare_strings {1 {No object name.}}

    # Cleanup steps
}

proc object_create_4_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object_create_4_N: Check that unauthenticated users are handled correctly.

    # Setup steps

    # Run the test
    test {
        list [catch {object create /.:/test_1} msg] $msg
    } compare_strings {1 {Insufficient rights to perform requested operation}}

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

set iclist "ic1 ic2 ic3 ic4"

set ic1 "object_create_1_N"
set ic2 "object_create_2_N"
set ic3 "object_create_3_N"
set ic4 "object_create_4_N"

# Source the TET main script into our script. The TET main script is what 
# will do the actual function calling.

source $TET_ROOT/lib/tcl/tcm.tcl
