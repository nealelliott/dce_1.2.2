#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_dump_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:22:44  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:34  marty]
#
# Revision 1.1.4.1  1995/12/11  15:12:52  root
# 	Submit
# 	[1995/12/11  14:30:48  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:21  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:44  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:28  melman]
# 
# Revision 1.1.2.1  1994/06/29  21:39:08  salamone
# 	Initial version.
# 	[1994/06/29  20:06:58  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_dump_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_dump_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_dump_001_N {} {
    # Identify the test
    tet_infoline registry_dump_001_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry dump foo foo2} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo2'.}}

    # Cleanup steps
}

proc registry_dump_002_N {} {
    # Identify the test
    tet_infoline registry_dump_002_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {registry dump foo -foo2} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo2'.}}

    # Cleanup steps
}

proc registry_dump_003_N {} {
    # Identify the test
    tet_infoline registry_dump_003_N: Check that invalid replica is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {registry dump foo} msg] $msg
    } compare_strings {0 {Error: Registry server 'foo' is unavailable.}}

    # Cleanup steps
}

proc registry_dump_004_N {} {
    # Identify the test
    tet_infoline registry_dump_004_N: Too many replicas specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry dump {foo bar}} msg] $msg
    } compare_strings {1 {Only one registry replica is allowed.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "registry_dump_001_N"
set ic2 "registry_dump_002_N"
set ic3 "registry_dump_003_N"
set ic4 "registry_dump_004_N"
set tet_startup registry_dump_startup
set tet_cleanup registry_dump_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
