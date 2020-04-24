#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: secval_setup.tcl,v $
# Revision 1.1.6.3  1996/03/11  02:27:22  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:15  marty]
#
# Revision 1.1.6.2  1995/12/11  15:15:58  root
# 	Submit
# 	[1995/12/11  14:32:16  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:14  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:12  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:20  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:44:05  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:29:44  devsrc]
# 
# Revision 1.1.2.1  1994/05/25  21:30:03  pwang
# 	Initial Implementation
# 	[1994/05/25  21:19:32  pwang]
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

proc secval_startup {} {
    global DCP_HOSTNAME_THREE

    # Change the acls for the secval object.
    set rcode [catch {acl modify /.:/$DCP_HOSTNAME_THREE/config/secval -remove unauthenticated} result]
    if {$rcode != 0} {
        tet_infoline Command failed: acl modify $DCP_HOSTNAME_THREE -remove
        tet_infoline Command returned: $result
        tet_infoline Test Setup cancelled
        tet_result UNRESOLVED
        return
    }
    set rcode [catch {acl modify /.:/$DCP_HOSTNAME_THREE/config/secval -add {user:cell_admin:csux}} result]
    if {$rcode != 0} {
        tet_infoline Command failed: acl modify $DCP_HOSTNAME_THREE -add
        tet_infoline Command returned: $result
        tet_infoline Test Setup cancelled
        tet_result UNRESOLVED
        return
    }
    tet_infoline Test Setup passed.
    tet_result PASS
}

set iclist "ic1 "
set ic1 "secval_startup"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
