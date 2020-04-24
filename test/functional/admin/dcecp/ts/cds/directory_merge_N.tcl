#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_merge_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:34  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:33  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:23  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:33  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:49  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:47  melman]
# 
# Revision 1.1.2.3  1994/09/02  13:56:46  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:40  kevins]
# 
# Revision 1.1.2.2  1994/06/17  18:43:08  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:24:53  devsrc]
# 
# Revision 1.1.2.1  1994/05/19  14:10:52  coren
# 	Initial test drop.
# 	[1994/05/17  20:59:41  coren]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc directory_merge_1_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_merge_1_N: Check that missing arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory merge} msg] $msg
    } compare_strings {1 {No directory name.}}

    # Cleanup steps
}

proc directory_merge_3_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW expect

    # Identify the test
    tet_infoline directory_merge_3_N: Check that already-existing object is detected as error.

    # Setup steps
        # Must be running as cell administrator and the 
        # directories must exist in cds namespace.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {dcp_dir_merge_setup}] != 0} {return;}
    if {[dcp_wrap {object create /.:/testmertarg/o1}] != 0} {return;}

    # Run the test
    test {
        list [catch {directory merge /.:/testmersrc -into /.:/testmertarg} msg] $msg
    } compare_strings [format "1 {{%s}}" [list [name expand /.:/testmertarg/o1] {object already exists}]]

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/testmertarg -tree}] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/testmersrc -tree}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc directory_merge_8_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_merge_8_N: Check that list of directories is detected as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {directory merge {/.:/testmersrc /.:/testmersrc2} -into /.:/testmertarg} msg] $msg
    } compare_strings {1 {Command does not accept a list of names as argument.}}

    # Cleanup steps
}

proc directory_merge_9_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_merge_9_N: Check that omitting target directory is detected as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {directory merge /.:/testmersrc} msg] $msg
    } compare_strings {1 {No target directory specified.}}

    # Cleanup steps
}

proc directory_merge_10_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_merge_10_N: Check that existing object matching source directory is an error.

    # Setup steps
        # Must be running as cell administrator and the 
        # directories must exist in cds namespace.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {dcp_dir_merge_setup}] != 0} {return;}
    if {[dcp_wrap {object create /.:/testmertarg/d1}] != 0} {return;}

    # Run the test
    test {
        list [catch {directory merge /.:/testmersrc -into /.:/testmertarg} msg] $msg
    } compare_strings [format "1 {{%s}}" [list [name expand /.:/testmertarg/d1] {object already exists}]]

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/testmertarg -tree}] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/testmersrc -tree}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc directory_merge_11_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_merge_11_N: Check that existing directory matching source object is an error.

    # Setup steps
        # Must be running as cell administrator and the 
        # directories must exist in cds namespace.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {dcp_dir_merge_setup}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/testmertarg/o1}] != 0} {return;}

    # Run the test
    test {
        list [catch {directory merge /.:/testmersrc -into /.:/testmertarg} msg] $msg
    } compare_strings [format "1 {{%s}}" [list [name expand /.:/testmertarg/o1] {object already exists}]]

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/testmertarg -tree}] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/testmersrc -tree}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc directory_merge_12_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_merge_12_N: Check that lack of permission to create objects is an error.

    # Setup steps
        # Must be running as cell administrator and the 
        # directories must exist in cds namespace.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {dcp_dir_merge_setup}] != 0} {return;}
    if {[dcp_wrap {dcp_acl_change_entry /.:/testmertarg user $DCP_CLIENT rwdtc-a}] != 0} {return;}

    # Run the test
    test {
        list [catch {directory merge /.:/testmersrc -into /.:/testmertarg} msg] $msg
    } compare_strings [format "1 {{%s}}" [list [name expand /.:/testmertarg] {No permission to add objects}]]

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/testmertarg -tree}] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/testmersrc -tree}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc directory_merge_13_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_merge_13_N: Check that multiple errors are all reported

    # Setup steps
        # Must be running as cell administrator and the 
        # directories must exist in cds namespace.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {dcp_dir_merge_setup}] != 0} {return;}
    if {[dcp_wrap {object create /.:/testmertarg/o1}] != 0} {return;}
    if {[dcp_wrap {dcp_acl_change_entry /.:/testmertarg user $DCP_CLIENT rwdtc-a}] != 0} {return;}

    # Run the test
    test {
        list [catch {directory merge /.:/testmersrc -into /.:/testmertarg} msg] $msg
    } compare_strings [format "1 {{%s}\n{%s}}" [list [name expand /.:/testmertarg/o1] {object already exists}] [list [name expand /.:/testmertarg] {No permission to add objects}]]

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/testmertarg -tree}] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/testmersrc -tree}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 "
set ic1 "directory_merge_1_N"
set ic2 "directory_merge_3_N"
set ic3 "directory_merge_8_N"
set ic4 "directory_merge_9_N"
set ic5 "directory_merge_10_N"
set ic6 "directory_merge_11_N"
set ic7 "directory_merge_12_N"
set ic8 "directory_merge_13_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
