#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_cleanup.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:27:47  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:27  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:20  root
# 	Submit
# 	[1995/12/11  14:32:23  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:23  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:27  melman
# 	#	moved admin tests to test/functional
# 	[1994/09/28  21:14:28  melman]
# 
# Revision 1.1.2.3  1994/09/21  16:37:33  pwang
# 	Spell out path for tet_setup.
# 	[1994/09/21  16:35:53  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:08  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:30:03  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:39:38  pwang
# 	Initial implementation
# 	[1994/06/10  21:26:12  pwang]
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

proc server_cleanup {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW DCP_SR_ENTRYNAME DCP_CELLNAME_THREE \
           DCP_HOSTNAME_THREE DCP_HOSTNAME_ONE

    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {
        tet_infoline Server FVT Cleanup problem
        tet_infoline Unable to login as cell administrator
        tet_result UNRESOLVED
        return;
    }

    # Delete object
    catch {object delete $DCP_SR_ENTRYNAME} dontcare

    # Remove server principal from the registry and keytab entry
    if {[dcp_wrap "account delete tserver"] != 0} {return;}
    if {[dcp_wrap {organization remove none -member tserver}] != 0} {return;}
    if {[dcp_wrap {group remove none -member tserver}] != 0} {return;}
    if {[dcp_wrap {principal delete tserver}] != 0} {return;}

    if {[dcp_wrap {logout}] != 0} {
        tet_infoline Server FVT Cleanup problem
        tet_infoline Unable to logout from cell administrator
        tet_result UNRESOLVED
        return;
    }

    # Delete acl entry user:cell_admin
    set rcode [catch {exec server_cleanup.sh $DCP_HOSTNAME_THREE} result]
    if {$rcode != 0} {
        tet_infoline Command failed: exec server_cleanup.sh $DCP_HOSTNAME_THREE
        tet_infoline Command returned: $result
        tet_infoline Test aborted.
        tet_result UNRESOLVED
        return;
    }
    if {[dcp_wrap "acl modify -io $DCP_CELLNAME_THREE/$DCP_HOSTNAME_THREE/config/srvrconf \
            -remove user:cell_admin:cdfrwx"] != 0} {return;}
    if {[dcp_wrap "keytab remove /.:/hosts/$DCP_HOSTNAME_ONE/config/keytab/self \
            -member tserver"] != 0} {return;}

    tet_infoline Test Cleanup passed.
    tet_result PASS
}

set iclist "ic1 "
set ic1 "server_cleanup"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
