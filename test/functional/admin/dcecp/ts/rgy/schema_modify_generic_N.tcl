#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_modify_generic_N.tcl,v $
# Revision 1.1.2.2  1996/03/11  02:24:03  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:00  marty]
#
# Revision 1.1.2.1  1995/12/11  15:13:27  root
# 	Submit
# 	[1995/12/11  14:31:14  root]
# 
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/04/05  19:39 UTC  truitt
# 	Merge test work.
# 
# 	HP revision /main/HPDCE02/truitt_mothra_chfts24/1  1995/04/05  19:38 UTC  truitt
# 	CHFts14652: Improved error message for bad schema name.
# 
# 	HP revision /main/HPDCE02/1  1995/03/02  19:46 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/02  19:45 UTC  truitt
# 	New schema tests.
# 	[1995/12/08  23:15:51  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc schema_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc schema_modify_1_N {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_1_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_1_N  -aclmgr {group rc id tD z}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_modify_2_N {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_2_N: badly formatted permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_2_N -aclmgr {group r r r r r}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_modify_3_N {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_3_N: Incorrect permission set name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_3_N -aclmgr {blech r r r r}} msg] $msg
    } compare_strings {1 {Invalid ACL manager name.}}

    # Cleanup steps
}

proc schema_modify_4_N {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline schema_modify_4_N: Incorrect schema object name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_modify_4_N -aclmgr {group r r r r}} msg] $msg
    } compare_strings {1 {Schema entry 'schema_modify_4_N' not found.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "schema_modify_1_N"
set ic2 "schema_modify_2_N"
set ic3 "schema_modify_3_N"
set ic4 "schema_modify_4_N"
set tet_startup schema_modify_startup
set tet_cleanup schema_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
