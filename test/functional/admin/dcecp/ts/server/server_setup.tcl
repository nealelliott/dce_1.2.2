#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_setup.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:28:25  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:46  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:53  root
# 	Submit
# 	[1995/12/11  14:32:36  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:39  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:50  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:44  melman]
# 
# Revision 1.1.2.3  1994/09/21  16:37:34  pwang
# 	Spell out the path for tet_setup.
# 	[1994/09/21  16:36:57  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:27  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:31:01  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:40:09  pwang
# 	Initial implementation
# 	[1994/06/10  21:32:46  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc server_startup {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_THREE \
           DCP_HOSTNAME_THREE DCP_HOSTNAME_ONE

    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {
        tet_infoline Server FVT Setup problem
        tet_infoline Unable to login as cell administrator
        tet_result UNRESOLVED
        return;
    }

    # Add server principal to the registry and keytab entry
    if {[dcp_wrap {principal create tserver}] != 0} {return;}
    if {[dcp_wrap {group add none -member tserver}] != 0} {return;}
    if {[dcp_wrap {organization add none -member tserver}] != 0} {return;}
    if {[dcp_wrap "account create tserver -group none -organization none \
            -mypwd $DCP_CLIENT_PW -password foobar"] != 0} {return;}

    if {[dcp_wrap {logout}] != 0} {
        tet_infoline Server FVT Setup problem
        tet_infoline Unable to logout from cell administrator
        tet_result UNRESOLVED
        return;
    }

    # Add acl entry user:cell_admin
    set rcode [catch {exec server_setup.sh $DCP_HOSTNAME_THREE} result]
    if {$rcode != 0} {
        tet_infoline Command failed: exec server_setup.sh $DCP_HOSTNAME_THREE
        tet_infoline Command returned: $result
        tet_infoline Test aborted.
        tet_result UNRESOLVED
        return;
    }
    if {[dcp_wrap "acl modify -io $DCP_CELLNAME_THREE/$DCP_HOSTNAME_THREE/config/srvrconf \
            -add user:cell_admin:cdfrwx"] != 0} {return;}
    if {[dcp_wrap "keytab add /.:/hosts/$DCP_HOSTNAME_ONE/config/keytab/self \
            -member tserver -version 1 -key foobar"] != 0} {return;}

    tet_infoline Test Setup passed.
    tet_result PASS
}

set iclist "ic1 "
set ic1 "server_startup"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
