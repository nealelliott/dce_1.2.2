#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: registry_catalog_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:22:11  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:22  marty]
#
# Revision 1.1.4.1  1995/12/13  15:28:44  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/09/11  16:07 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/truitt_dcecp_chfts46/1  1995/09/11  16:05 UTC  truitt
# 	CHFts16100: Additional tests to support new functionality.
# 	[1995/12/11  15:55:27  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:36  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:21  melman]
# 
# Revision 1.1.2.4  1994/09/28  17:54:49  kevins
# 	OT 12376 modified registry unavailable messages
# 	[1994/09/28  17:51:54  kevins]
# 
# Revision 1.1.2.3  1994/08/31  23:04:19  salamone
# 	CR11916 - Add _b(sec) support.
# 	[1994/08/31  23:02:49  salamone]
# 
# Revision 1.1.2.2  1994/08/15  17:28:41  salamone
# 	CR11636: Allow registry catalog to take an argument.
# 	[1994/08/15  17:22:51  salamone]
# 
# Revision 1.1.2.1  1994/06/29  21:38:55  salamone
# 	Initial version.
# 	[1994/06/29  20:05:29  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_catalog_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_catalog_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_catalog_001_N {} {
    # Identify the test
    tet_infoline registry_catalog_001_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry catalog {foo foo2}} msg] $msg
    } compare_strings {1 {Only one registry replica is allowed.}}

    # Cleanup steps
}

proc registry_catalog_002_N {} {
    # Identify the test
    tet_infoline registry_catalog_002_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry catalog foo foo2} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo2'.}}

    # Cleanup steps
}

proc registry_catalog_003_N {} {
    # Identify the test
    tet_infoline registry_catalog_003_N: Check for bogus option specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry catalog foo -foo2} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo2'.}}

    # Cleanup steps
}

proc registry_catalog_004_N {} {
    # Identify the test
    tet_infoline registry_catalog_004_N: Check for bogus replica specification.

    # Setup steps

    # Run the test
    test {
        list [catch {registry catalog foo} msg] $msg
    } compare_strings {1 {Registry server 'foo' is unavailable.}}

    # Cleanup steps
}

proc registry_catalog_005_N {} {
    # Identify the test
    tet_infoline registry_catalog_005_N: Check for bogus replica specification.

    # Setup steps

    # Run the test
    test {
        list [catch {registry catalog /.:/foo} msg] $msg
    } compare_strings {1 {Registry server '/.:/foo' is unavailable.}}

    # Cleanup steps
}

proc registry_catalog_006_N {} {
    # Identify the test
    tet_infoline registry_catalog_006_N: Check for bogus replica specification.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry catalog $DCP_CELLNAME_ONE/foo} msg] $msg
    } compare_strings "1 {Registry server '$DCP_CELLNAME_ONE/foo' is unavailable.}"

    # Cleanup steps
}

proc registry_catalog_007_N {} {
    # Identify the test
    tet_infoline registry_catalog_007_N: Check for bogus replica specification.

    # Setup steps

    # Run the test
    test {
        list [catch {registry catalog ncacn_ip_tcp:111.11.11.11} msg] $msg
    } compare_strings "1 {Registry server 'ncacn_ip_tcp:111.11.11.11' is unavailable.}"

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 "
set ic1 "registry_catalog_001_N"
set ic2 "registry_catalog_002_N"
set ic3 "registry_catalog_003_N"
set ic4 "registry_catalog_004_N"
set ic5 "registry_catalog_005_N"
set ic6 "registry_catalog_006_N"
set ic7 "registry_catalog_007_N"
set tet_startup registry_catalog_startup
set tet_cleanup registry_catalog_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
