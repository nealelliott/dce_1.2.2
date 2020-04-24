#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_show_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:24:24  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:07  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:42  root
# 	Submit
# 	[1995/12/11  14:31:20  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:00  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:40  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:04  melman]
# 
# Revision 1.1.2.7  1994/09/23  17:01:51  truitt
# 	Updated to reflect new aclmgr names.
# 	[1994/09/23  16:59:22  truitt]
# 
# Revision 1.1.2.6  1994/09/21  15:26:16  truitt
# 	Update tests to reflect new aclmgr for generic schema.
# 	[1994/09/21  14:58:49  truitt]
# 
# Revision 1.1.2.5  1994/09/07  12:56:20  truitt
# 	Update schema tests to reflect new messages
# 	[1994/09/06  18:47:23  truitt]
# 
# Revision 1.1.2.4  1994/08/26  17:45:48  truitt
# 	OT11184: update tests to reflect new functionality.
# 	[1994/08/26  17:44:44  truitt]
# 
# Revision 1.1.2.3  1994/08/17  18:45:09  truitt
# 	OT11693 & others: update show tests to reflect new functionality.
# 	[1994/08/17  18:35:44  truitt]
# 
# Revision 1.1.2.2  1994/08/10  19:22:30  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:48  annie]
# 
# Revision 1.1.2.1  1994/07/26  18:24:55  truitt
# 	Initial schema test drop
# 	[1994/07/26  17:34:16  truitt]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc schema_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc schema_show_1_P {} {
    # Identify the test
    tet_infoline schema_show_1_P: Show an existing schema name in the registry schema

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_show_1_P -aclmgr {group r r r r} -encoding integer}] != 0} {return;}

    # Run the test
    test {catch {xattrschema show /.:/sec/xattrschema/schema_show_1_P} attr_list_1
        dcp_found $attr_list_1 {uuid encoding aclmgr unique multivalued reserved applydefs intercell trigtype trigbind scope annotation}
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_show_1_P}] != 0} {return;}
}

proc schema_show_2_P {} {
    # Identify the test
    tet_infoline schema_show_2_P: Show multiple existing schema names in the registry schema

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_show_2_P_1 -aclmgr {group r r r r} -encoding integer}] != 0} {return;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_show_2_P_2 -aclmgr {organization r r r r} -encoding integer}] != 0} {return;}

    # Run the test
    test {catch {xattrschema show {/.:/sec/xattrschema/schema_show_2_P_1 /.:/sec/xattrschema/schema_show_2_P_2}} attr_list_2
        dcp_found $attr_list_2 {uuid encoding aclmgr unique multivalued reserved applydefs intercell trigtype trigbind scope annotation}
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_show_2_P_1}] != 0} {return;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_show_2_P_2}] != 0} {return;}
}

proc schema_show_3_P {} {
    global DCP_HOSTNAME_ONE DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline schema_show_3_P: Show an existing schema name in the hosts schema

    # Setup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {xattrschema create "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_show_3_P" -aclmgr {srvrconf r r r r} -encoding integer}] != 0} {return;}

    # Run the test
    test {catch {xattrschema show "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_show_3_P"} attr_list_3
        dcp_found $attr_list_3 {uuid encoding aclmgr unique multivalued reserved applydefs intercell trigtype trigbind scope annotation}
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_show_3_P"}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_show_4_P {} {
    global DCP_HOSTNAME_ONE DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline schema_show_4_P: Show multiple existing schema names in two different schemas

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_show_4_P_1 -aclmgr {group r r r r} -encoding integer}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {xattrschema create "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_show_4_P_2" -aclmgr {srvrconf r r r r} -encoding integer}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {catch {xattrschema show {"/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_show_4_P_2" /.:/sec/xattrschema/schema_show_4_P_1}} attr_list_4
        dcp_found $attr_list_4 {uuid encoding aclmgr unique multivalued reserved applydefs intercell trigtype trigbind scope annotation}
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_show_4_P_1}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {xattrschema delete "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_show_4_P_2"}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

set iclist "ic1 ic2 ic3 ic4 "
set ic1 "schema_show_1_P"
set ic2 "schema_show_2_P"
set ic3 "schema_show_3_P"
set ic4 "schema_show_4_P"
set tet_startup schema_show_startup
set tet_cleanup schema_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
