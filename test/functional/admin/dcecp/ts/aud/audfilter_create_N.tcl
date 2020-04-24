#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: audfilter_create_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:10:58  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:16  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:41  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:46  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:11  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:51  melman]
# 
# Revision 1.1.2.5  1994/09/02  20:28:37  rousseau
# 	Mass test cleanup.
# 	[1994/09/02  20:27:54  rousseau]
# 
# Revision 1.1.2.4  1994/06/10  20:02:45  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:54  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:04  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:16:44  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:27:00  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:24:39  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:50  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:30  rousseau]
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
proc audfilter_create_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc audfilter_create_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}


proc audfilter_create_1_N {} {
    # Identify the test
    tet_infoline audfilter_create_1_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps
    if {[dcp_wrap {principal create audfilter_create_1_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {audfilter create {principal audfiltre_create_1_N} foo -a {dce_audit_filter_query success log}} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete audfilter_create_1_N}] != 0} {return;}
}

proc audfilter_create_2_N {} {
    # Identify the test
    tet_infoline audfilter_create_2_N: Check that extra/invalid options are trapped as errors.

    # Setup steps
    if {[dcp_wrap {principal create audfilter_create_2_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {audfilter create {principal audfilter_create_2_N} -foo -a {dce_audit_filter_query success log}} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete audfilter_create_2_N}] != 0} {return;}
}

proc audfilter_create_3_N {} {
    # Identify the test
    tet_infoline audfilter_create_3_N: Check that not specifying -attribute is handled as an error.

    # Setup steps
    if {[dcp_wrap {principal create audfilter_create_3_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {audfilter create {principal audfilter_create_3_N}} msg] $msg
    } compare_strings {1 {The '-attribute' option is required.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete audfilter_create_3_N}] != 0} {return;}
}

proc audfilter_create_4_N {} {
    # Identify the test
    tet_infoline audfilter_create_4_N: Check that specifying -attribute without any data is handled 
    tet_infoline audfilter_create_4_N: as an error.

    # Setup steps
    if {[dcp_wrap {principal create audfiltre_create_4_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {audfilter create {principal audfiltre_create_4_N} -attribute} msg] $msg
    } compare_strings {1 {The '-attribute' option requires a character string value.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete audfiltre_create_4_N}] != 0} {return;}
}

proc audfilter_create_5_N {} {
    # Identify the test
    tet_infoline audfilter_create_5_N: Check that specifying an filter with too many elements is handled 
    tet_infoline audfilter_create_5_N: as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter create {principal foo bar} -attribute {dce_audit_filter_query success log}} msg] $msg
    } compare_strings {1 {The list {principal foo bar} contains too many elements.}}

    # Cleanup steps
}

proc audfilter_create_6_N {} {
    # Identify the test
    tet_infoline audfilter_create_6_N: Check that specifying a filter type that requires a key, without
    tet_infoline audfilter_create_6_N: a key is handled as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter create {principal} -attribute {dce_audit_filter_query success log}} msg] $msg
    } compare_strings {1 {The specified filter type 'principal' requires a key.}}

    # Cleanup steps
}

proc audfilter_create_7_N {} {
    # Identify the test
    tet_infoline audfilter_create_7_N: Check that specifying a filter type that does not take a key, with
    tet_infoline audfilter_create_7_N: a key is handled as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter create {world mars} -attribute {dce_audit_filter_query success log}} msg] $msg
    } compare_strings {1 {The supplied filter type 'world' does not take a key.}}

    # Cleanup steps
}

proc audfilter_create_8_N {} {
    # Identify the test
    tet_infoline audfilter_create_8_N: Check that specifying a guide with too few elements is 
    tet_infoline audfilter_create_8_N: handled as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter create world -attribute {success log}} msg] $msg
    } compare_strings {1 {The list {success log} contains too few elements.}}

    # Cleanup steps
}

proc audfilter_create_9_N {} {
    # Identify the test
    tet_infoline audfilter_create_9_N: Check that specifying a guide with too many elements is 
    tet_infoline audfilter_create_9_N: handled as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter create world -attribute {dce_audit_filter_query success log foo}} msg] $msg
    } compare_strings {1 {The list {dce_audit_filter_query success log foo} contains too many elements.}}

    # Cleanup steps
}

proc audfilter_create_10_N {} {
    # Identify the test
    tet_infoline audfilter_create_10_N: Check that specifying a guide with an invalid event class is 
    tet_infoline audfilter_create_10_N: handled as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter create world -attribute {foo success log}} msg] $msg
    } compare_strings {1 {The event class 'foo' is not valid.}}

    # Cleanup steps
}

proc audfilter_create_11_N {} {
    # Identify the test
    tet_infoline audfilter_create_11_N: Check that specifying a guide with an invalid condition is 
    tet_infoline audfilter_create_11_N: handled as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter create world -attribute {dce_audit_filter_query foo log}} msg] $msg
    } compare_strings {1 {The audit condition 'foo' is not valid.}}

    # Cleanup steps
}

proc audfilter_create_12_N {} {
    # Identify the test
    tet_infoline audfilter_create_12_N: Check that specifying a guide with an invalid action is 
    tet_infoline audfilter_create_12_N: handled as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {audfilter create world -attribute {dce_audit_filter_query success foo}} msg] $msg
    } compare_strings {1 {The audit action 'foo' is not valid.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 "
set ic1 "audfilter_create_1_N"
set ic2 "audfilter_create_2_N"
set ic3 "audfilter_create_3_N"
set ic4 "audfilter_create_4_N"
set ic5 "audfilter_create_5_N"
set ic6 "audfilter_create_6_N"
set ic7 "audfilter_create_7_N"
set ic8 "audfilter_create_8_N"
set ic9 "audfilter_create_9_N"
set ic10 "audfilter_create_10_N"
set ic11 "audfilter_create_11_N"
set ic12 "audfilter_create_12_N"
set tet_startup audfilter_create_startup
set tet_cleanup audfilter_create_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

