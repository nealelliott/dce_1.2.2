#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_synchronize_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:23:18  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:44  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:07  root
# 	Submit
# 	[1995/12/11  14:31:01  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:35  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:03  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:42  melman]
# 
# Revision 1.1.2.3  1994/09/28  17:54:55  kevins
# 	OT 12376 modified registry unavailable messages
# 	[1994/09/28  17:52:01  kevins]
# 
# Revision 1.1.2.2  1994/09/20  15:49:16  salamone
# 	Fix authentication messages
# 	[1994/09/20  15:48:08  salamone]
# 
# Revision 1.1.2.1  1994/06/29  21:39:21  salamone
# 	Initial version.
# 	[1994/06/29  20:09:50  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_synchronize_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_synchronize_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_synchronize_001_N {} {
    # Identify the test
    tet_infoline registry_synchronize_001_N: Check for an insufficient number of arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {registry synchronize} msg] $msg
    } compare_strings {1 {Registry replica not specified.}}

    # Cleanup steps
}

proc registry_synchronize_002_N {} {
    # Identify the test
    tet_infoline registry_synchronize_002_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry synchronize {foo foo2}} msg] $msg
    } compare_strings {1 {Only one registry replica is allowed.}}

    # Cleanup steps
}

proc registry_synchronize_003_N {} {
    # Identify the test
    tet_infoline registry_synchronize_003_N: Check for a bogus replica.

    # Setup steps

    # Run the test
    test {
        list [catch {registry synchronize foo} msg] $msg
    } compare_strings {1 {Registry server 'foo' is unavailable.}}

    # Cleanup steps
}

proc registry_synchronize_004_N {} {
    # Identify the test
    tet_infoline registry_synchronize_004_N: Check for a replica specified as local cell.

    # Setup steps

    # Run the test
    test {
        list [catch {registry synchronize /.:} msg] $msg
    } compare_strings {1 {Registry server '/.:' is unavailable.}}

    # Cleanup steps
}

proc registry_synchronize_005_N {} {
    # Identify the test
    tet_infoline registry_synchronize_005_N: Check for a replica specified as cell name.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry synchronize $DCP_CELLNAME_ONE} msg] $msg
    } compare_strings "1 {Registry server '$DCP_CELLNAME_ONE' is unavailable.}"

    # Cleanup steps
}

proc registry_synchronize_006_N {} {
    # Identify the test
    tet_infoline registry_synchronize_006_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry synchronize foo foo2} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo2'.}}

    # Cleanup steps
}

proc registry_synchronize_007_N {} {
    # Identify the test
    tet_infoline registry_synchronize_007_N: Check for bogus option specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry synchronize foo -} msg] $msg
    } compare_strings {1 {Unrecognized option '-'.}}

    # Cleanup steps
}

proc registry_synchronize_008_N {} {
    # Identify the test
    tet_infoline registry_synchronize_008_N: Check for bogus option specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry synchronize foo -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc registry_synchronize_009_N {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_NOTROOT DCP_NOTROOT_PW

    # Identify the test
    tet_infoline registry_synchronize_009_N: Check for authentication to perform this operation.

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
        list [catch {registry synchronize $DCP_RGY_SA_REP_NAME_SLAVE1} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {account delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_NOTROOT}] != 0} {return;}
}

proc registry_synchronize_010_N {} {
    # Identify the test
    tet_infoline registry_synchronize_010_N: Check that the master security server can not
    tet_infoline registry_synchronize_010_N: be synchronized.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry synchronize $DCP_RGY_SA_REP_NAME_MASTER} msg] $msg
    } compare_strings {1 {Replica is the master}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 "
set ic1 "registry_synchronize_001_N"
set ic2 "registry_synchronize_002_N"
set ic3 "registry_synchronize_003_N"
set ic4 "registry_synchronize_004_N"
set ic5 "registry_synchronize_005_N"
set ic6 "registry_synchronize_006_N"
set ic7 "registry_synchronize_007_N"
set ic8 "registry_synchronize_008_N"
set ic9 "registry_synchronize_009_N"
set ic10 "registry_synchronize_010_N"
set tet_startup registry_synchronize_startup
set tet_cleanup registry_synchronize_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
