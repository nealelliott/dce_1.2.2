#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_rename_generic_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:24:19  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:06  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:37  root
# 	Submit
# 	[1995/12/11  14:31:19  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:59  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:36  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:02  melman]
# 
# Revision 1.1.2.3  1994/09/21  15:26:15  truitt
# 	Update tests to reflect new aclmgr for generic schema.
# 	[1994/09/21  14:58:42  truitt]
# 
# Revision 1.1.2.2  1994/09/07  12:56:19  truitt
# 	Update schema tests to reflect new messages
# 	[1994/09/06  18:59:07  truitt]
# 
# Revision 1.1.2.1  1994/08/25  16:16:43  nav
# 	Initial creation.
# 
# 	evision 1.1.1.2  1994/08/25  16:15:55  nav
# 	Initial creation.
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
proc schema_rename_generic_1_P {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline schema_rename_generic_1_P: Rename a schema entry in the registry schema

    # Setup steps
    if {[dcp_wrap {xattrschema create "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_rename_generic_1_P_1" -aclmgr {srvrconf r r r r} -encoding integer}] != 0} {return;}

    # Run the test
    test {xattrschema rename "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_rename_generic_1_P_1" -to schema_rename_generic_1_P_2
        dcp_attrlist getval [xattrschema show "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_rename_generic_1_P_2"] encoding
    } compare_strings integer

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_rename_generic_1_P_2"}] != 0} {return;}
}


set iclist "ic1 "
set ic1 "schema_rename_generic_1_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
