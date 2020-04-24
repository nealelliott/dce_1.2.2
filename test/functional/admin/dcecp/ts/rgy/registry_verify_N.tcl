#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_verify_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:23:24  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:45  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:10  root
# 	Submit
# 	[1995/12/11  14:31:03  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:37  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:08  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:45  melman]
# 
# Revision 1.1.2.2  1994/08/10  19:22:17  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:36  annie]
# 
# Revision 1.1.2.1  1994/07/20  19:58:11  salamone
# 	Initial version.
# 	[1994/07/20  19:42:36  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_verify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_verify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_verify_001_N {} {
    # Identify the test
    tet_infoline registry_verify_001_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry verify foo} msg] $msg
    } compare_strings {0 {Error: Registry server 'foo' is unavailable.}}

    # Cleanup steps
}

proc registry_verify_002_N {} {
    # Identify the test
    tet_infoline registry_verify_002_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry verify -foo} msg] $msg
    } compare_strings {0 {Error: Registry server '-foo' is unavailable.}}

    # Cleanup steps
}

proc registry_verify_003_N {} {
    # Identify the test
    tet_infoline registry_verify_003_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry verify foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc registry_verify_004_N {} {
    # Identify the test
    tet_infoline registry_verify_004_N: Too many replicas specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry verify {foo bar}} msg] $msg
    } compare_strings {1 {Only one registry replica is allowed.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "registry_verify_001_N"
set ic2 "registry_verify_002_N"
set ic3 "registry_verify_003_N"
set ic4 "registry_verify_004_N"
set tet_startup registry_verify_startup
set tet_cleanup registry_verify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
