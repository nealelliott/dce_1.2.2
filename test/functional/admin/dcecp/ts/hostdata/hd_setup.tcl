#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: hd_setup.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:02  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:37  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:11  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/06/06  18:26 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts33/1  1995/06/06  18:24 UTC  truitt
# 	CHFts13959: Change some tests to support new dced acl behavior.
# 
# 	HP revision /main/HPDCE02/2  1994/11/08  16:07 UTC  truitt
# 	Change the way the test setup script is executed.
# 
# 	HP revision /main/HPDCE02/1  1994/11/07  20:37 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/07  20:33 UTC  truitt
# 	Fix the setup routine.
# 	[1995/12/11  15:53:15  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:52  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:08  melman]
# 
# Revision 1.1.2.3  1994/06/22  20:42:39  baum
# 	Merged with changes from 1.1.2.2
# 	[1994/06/22  20:42:16  baum]
# 
# 	OT #11022 - Use tet_setup, use tetexec.cfg for cell/host
# 	[1994/06/22  20:40:12  baum]
# 
# Revision 1.1.2.2  1994/06/17  18:43:11  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:25:13  devsrc]
# 
# Revision 1.1.2.1  1994/05/25  16:51:03  baum
# 	ACL's on hostdata entry now setup automatically for tests
# 	[1994/05/25  16:48:25  baum]
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

# Test specific routines
source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_misc.tcl

proc hd_fvt_setup {} {
    global TET_ROOT

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    set rcode [catch {exec hd_setup.sh $CUR_CELL_NAME $CUR_HOST_NAME} result]
    if {$rcode != 0} {
        tet_infoline Hostdata FVT Setup problem
        tet_infoline Command failed: exec hd_setup.sh $CUR_CELL_NAME $CUR_HOST_NAME
        tet_infoline Command returned: $result
        tet_infoline If this ACL is not set, or equivalent priviledges are not provided
        tet_infoline through other means, some of the dcecp HOSTDATA FVT's may fail
        tet_result UNRESOLVED
    } else {
        tet_infoline Hostdata Setup passed.
        tet_result PASS
    }
}

set iclist "ic1 "
set ic1 "hd_fvt_setup"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
