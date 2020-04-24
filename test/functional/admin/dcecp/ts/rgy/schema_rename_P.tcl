#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_rename_P.tcl,v $
# Revision 1.1.8.3  1996/03/11  02:24:14  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:04  marty]
#
# Revision 1.1.8.2  1995/12/11  15:13:34  root
# 	Submit
# 	[1995/12/11  14:31:17  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:55  root]
# 
# Revision 1.1.4.1  1994/10/14  19:33:37  cuti
# 	Update to reflect schema_update's work done in security.
# 	[1994/10/14  19:21:23  cuti]
# 
# 	Update to reflect schema_update's work done in security.
# 
# Revision 1.1.2.1  1994/09/28  21:44:34  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:01  melman]
# 
# 	Initial creation.
# 	[1994/07/27  15:53:39  nav]
# 
# Revision 1.1.2.7  1994/09/26  16:28:11  truitt
# 	OT12108: Because the rename command is currently unimplemented,
# 	the original schema object name is left behind in the schema.
# 	Add a command to delete it.
# 	[1994/09/26  15:58:12  truitt]
# 
# Revision 1.1.2.6  1994/09/21  15:26:13  truitt
# 	Update tests to reflect new aclmgr for generic schema.
# 	[1994/09/21  14:58:36  truitt]
# 
# Revision 1.1.2.5  1994/09/07  12:56:18  truitt
# 	Update schema tests to reflect new messages
# 	[1994/09/06  18:58:38  truitt]
# 
# Revision 1.1.2.4  1994/08/26  17:21:09  nav
# 	real test suite in anticipation of support from security api for
# 	schema modify.
# 	[1994/08/26  17:19:50  nav]
# 
# Revision 1.1.2.3  1994/08/17  18:45:03  truitt
# 	Fix comment leader.
# 	[1994/08/17  18:32:50  truitt]
# 
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:46  annie]
# 
# Revision 1.1.2.2  1994/08/10  19:22:28  annie
# 	expand copyright with OSF copyright text
# 
# Revision 1.1.2.1  1994/07/27  18:52:08  nav
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


proc schema_rename_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_rename_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc schema_rename_1_P {} {
    # Identify the test
    tet_infoline schema_rename_1_P: Rename a schema entry in the registry schema

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_rename_1_P_1 -aclmgr {group r r r r} -encoding integer }] != 0} {return;}

    # Run the test
    test {xattrschema rename /.:/sec/xattrschema/schema_rename_1_P_1 -to schema_rename_1_P_2
        dcp_attrlist getval [xattrschema show /.:/sec/xattrschema/schema_rename_1_P_2] encoding
    } compare_strings integer

    # Cleanup steps
    # NOTE: the delete of '1_P_1' is here only because rename is unimplemented
    # and this test case fails.  When rename is implemented correctly, the
    # delete of '1_P_1' needs to be removed.
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_rename_1_P_2}] != 0} {return;}
}

set iclist "ic1 "
set ic1 "schema_rename_1_P"
set tet_startup schema_rename_startup
set tet_cleanup schema_rename_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
