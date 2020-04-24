#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: link_create_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:14:02  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:57  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:33  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:41  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:00  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:08  melman]
# 
# Revision 1.1.2.6  1994/09/02  18:42:08  kevins
# 	OT 10617 fixed error messages
# 	[1994/09/02  18:29:14  kevins]
# 
# Revision 1.1.2.5  1994/08/26  14:24:56  kevins
# 	OT 11869 update for switch from -linkto to -to
# 	[1994/08/26  14:06:18  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:14  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:22  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:06  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:18:53  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:22  melman
# 	updating to new API path
# 	[1994/05/06  15:20:13  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:06  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:46  kevins]
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
proc link_create_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_create_1_N: Check that extra arguments are trapped as errors.

    # Run the test
    test {
        list [catch {link create /.:/link -to /.:/hosts extra } msg] $msg
    } compare_strings {1 {Unrecognized argument 'extra'.}}

    # Cleanup steps
}

proc link_create_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_create_2_N: Check that missing required argument is trapped as error.

    # Run the test
    test {
        list [catch {link create /.:/link } msg] $msg
    } compare_strings {1 {The directory to link to not specified.}}

    # Cleanup steps
}

proc link_create_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_create_3_N: Check that missing link name argument is trapped as error.

    # Run the test
    test {
        list [catch {link create -to /.:/hosts} msg] $msg
    } compare_strings {1 {No link name.}}

    # Cleanup steps
}

proc link_create_4_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_create_4_N: Check that missing link name argument is trapped as error.

    # Run the test
    test {
        list [catch {link create /.:/link -to } msg] $msg
    } compare_strings {1 {The '-to' option requires a character string value.}}

    # Cleanup steps
}

proc link_create_5_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_create_5_N: Check that missing link name argument is trapped as error.

    # Run the test
    test {
        list [catch {link create /.:/link -timeout } msg] $msg
    } compare_strings {1 {The '-timeout' option requires a character string value.}}

    # Cleanup steps
}

proc link_create_6_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_create_6_N: Check that unauthenticated users are handled correctly.

    # Run the test
    test {
        list [catch {link create /.:/link -to /.:/hosts} msg] $msg
    } compare_strings {1 {Insufficient rights to perform requested operation}}

    # Cleanup steps
}

proc link_create_7_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_create_7_N: Check that invalid time format is trapped as an error

    # Run the test
    test {
        list [catch {link create /.:/link -to /.:/hosts -timeout a/b/c} msg] $msg
    } compare_strings {1 {Invalid DTS style timestamp.}}

}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 "
set ic1 "link_create_1_N"
set ic2 "link_create_2_N"
set ic3 "link_create_3_N"
set ic4 "link_create_4_N"
set ic5 "link_create_5_N"
set ic6 "link_create_6_N"
set ic7 "link_create_7_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
