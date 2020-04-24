#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: object_modify_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:14:39  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:25  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:43  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:51  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:22  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:25  melman]
# 
# Revision 1.1.2.5  1994/09/02  17:49:18  kevins
# 	OT 10617 modified fvt test messages
# 	[1994/09/02  15:13:45  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:12:04  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:50  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:21  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:19:37  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:34  melman
# 	updating to new API path
# 	[1994/05/06  15:20:29  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:23  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:59  kevins]
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
proc object_modify_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object_modify_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {object show /.:/test_1 foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc object_modify_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object_modify_2_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {object modify /.:/test_1 -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc object_modify_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object_modify_3_N: Check that missing arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {object modify } msg] $msg
    } compare_strings {1 {No object name.}}

    # Cleanup steps
}

proc object_modify_4_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object_modify_4_N: Check that missing arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {object modify /.:/test_1 } msg] $msg
    } compare_strings {1 {Option not specified.}}

    # Cleanup steps
}

proc object_modify_5_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline object_modify_5_N: Check that unauthenticated users are handled correctly

    # Setup steps
   if {[dcp_wrap {set obj_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {object create /.:/$obj_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        list [catch {object modify /.:/$obj_name -add {CDS_ClassVersion 1.0}} msg] $msg
    } compare_strings {1 {Insufficient rights to perform requested operation}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {object delete /.:/$obj_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

}

proc object_modify_6_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object_modify_6_N: Check that missing arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {object modify /.:/test_1 -add} msg] $msg
    } compare_strings {1 {The '-add' option requires a character string value.}}

    # Cleanup steps
}

proc object_modify_7_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object__modify_7_N: Check that missing arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {object modify /.:/test_1 -remove} msg] $msg
    } compare_strings {1 {The '-remove' option requires a character string value.}}

    # Cleanup steps
}

proc object_modify_8_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object__modify_8_N: Check that missing arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {object modify /.:/test_1 -change} msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}

proc object_modify_9_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline object__modify_9_N: Check that incorrectly formated attribute list is trapped as error

    # Setup steps

    # Run the test
    test {
        list [catch {object modify /.:/test_1 -add one} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

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


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9"
set ic1 "object_modify_1_N"
set ic2 "object_modify_2_N"
set ic3 "object_modify_3_N"
set ic4 "object_modify_4_N"
set ic5 "object_modify_5_N"
set ic6 "object_modify_6_N"
set ic7 "object_modify_7_N"
set ic8 "object_modify_8_N"
set ic9 "object_modify_9_N"


# Source the TET main script into our script. The TET main script is what 
# will do the actual function calling.

source $TET_ROOT/lib/tcl/tcm.tcl
