#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_delete_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:22:25  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:27  marty]
#
# Revision 1.1.4.1  1995/12/11  15:12:41  root
# 	Submit
# 	[1995/12/11  14:30:42  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:12  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:40  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:23  melman]
# 
# Revision 1.1.2.3  1994/09/28  17:54:50  kevins
# 	OT 12376 modified registry unavailable messages
# 	[1994/09/28  17:51:55  kevins]
# 
# Revision 1.1.2.2  1994/09/20  15:49:10  salamone
# 	Fix authentication messages
# 	[1994/09/20  15:48:01  salamone]
# 
# Revision 1.1.2.1  1994/06/29  21:38:58  salamone
# 	Initial version.
# 	[1994/06/29  20:06:13  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_delete_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_delete_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_delete_001_N {} {
    # Identify the test
    tet_infoline registry_delete_001_N: Check for an insufficient number of arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {registry delete} msg] $msg
    } compare_strings {1 {Registry replica not specified.}}

    # Cleanup steps
}

proc registry_delete_002_N {} {
    # Identify the test
    tet_infoline registry_delete_002_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry delete {foo foo2}} msg] $msg
    } compare_strings {1 {Only one registry replica is allowed.}}

    # Cleanup steps
}

proc registry_delete_003_N {} {
    # Identify the test
    tet_infoline registry_delete_003_N: Check for a bogus replica.

    # Setup steps

    # Run the test
    test {
        list [catch {registry delete foo} msg] $msg
    } compare_strings {1 {Registry server 'foo' is unavailable.}}

    # Cleanup steps
}

proc registry_delete_004_N {} {
    # Identify the test
    tet_infoline registry_delete_004_N: Check for a replica specified as local cell.

    # Setup steps

    # Run the test
    test {
        list [catch {registry delete /.:} msg] $msg
    } compare_strings {1 {Registry server '/.:' is unavailable.}}

    # Cleanup steps
}

proc registry_delete_005_N {} {
    # Identify the test
    tet_infoline registry_delete_005_N: Check for a replica specified as cell name.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry delete $DCP_CELLNAME_ONE} msg] $msg
    } compare_strings "1 {Registry server '$DCP_CELLNAME_ONE' is unavailable.}"

    # Cleanup steps
}

proc registry_delete_006_N {} {
    # Identify the test
    tet_infoline registry_delete_006_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry delete foo foo2} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo2'.}}

    # Cleanup steps
}

proc registry_delete_007_N {} {
    # Identify the test
    tet_infoline registry_delete_007_N: Check for bogus option specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry delete foo -} msg] $msg
    } compare_strings {1 {Registry server 'foo' is unavailable.}}

    # Cleanup steps
}

proc registry_delete_008_N {} {
    # Identify the test
    tet_infoline registry_delete_008_N: Check for bogus option specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry delete foo -foo2} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo2'.}}

    # Cleanup steps
}

proc registry_delete_009_N {} {
    global DCP_NOTROOT DCP_NOTROOT_PW DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline registry_delete_009_N: Check for authentication to perform this operation.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {group add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {account create $DCP_NOTROOT -group none -organization none -password $DCP_NOTROOT_PW -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_NOTROOT -p $DCP_NOTROOT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry delete $DCP_RGY_SA_REP_NAME_SLAVE1} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {account delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_NOTROOT}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 "
set ic1 "registry_delete_001_N"
set ic2 "registry_delete_002_N"
set ic3 "registry_delete_003_N"
set ic4 "registry_delete_004_N"
set ic5 "registry_delete_005_N"
set ic6 "registry_delete_006_N"
set ic7 "registry_delete_007_N"
set ic8 "registry_delete_008_N"
set ic9 "registry_delete_009_N"
set tet_startup registry_delete_startup
set tet_cleanup registry_delete_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
