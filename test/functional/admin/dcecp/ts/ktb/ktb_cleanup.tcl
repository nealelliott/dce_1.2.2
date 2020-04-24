#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: ktb_cleanup.tcl,v $
# Revision 1.1.4.4  1996/03/11  02:17:07  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:16  marty]
#
# Revision 1.1.4.3  1995/12/13  15:26:36  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/08/04  15:23 UTC  jrr
# 	# Fix test to handle config/ketyab ACL correctly.
# 	# [1995/08/04  15:21 UTC  jrr  /main/HPDCE02/jrr_mothra_pure/1]
# 
# 	HP revision /main/HPDCE02/2  1995/06/08  13:48 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts33/1  1995/06/08  13:45 UTC  truitt
# 	CHFts15424: Change some tests to support new dced acl behavior.
# 
# 	HP revision /main/HPDCE02/1  1994/11/10  14:19 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/10  14:18 UTC  truitt
# 	New file for keytab cleanup.
# 	[1995/12/11  15:53:38  root]
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

source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_misc.tcl

proc ktb_fvt_cleanup {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_ONE

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    set ret [catch "login $DCP_CLIENT -password $DCP_CLIENT_PW" msg]
    if { $ret != 0 } {
        tet_infoline Could not login as $DCP_CLIENT
        tet_infoline $msg
    }

    # Remove the accounts that were added
    set ret [catch "account delete $TEST_PRINC1" msg]
    if { $ret != 0 } {
        tet_infoline Could not delete account $TEST_PRINC1
        tet_infoline $msg
    }
    set ret [catch "account delete $TEST_PRINC2" msg]
    if { $ret != 0 } {
        tet_infoline Could not delete account $TEST_PRINC2
        tet_infoline $msg
    }

    set ret [catch "principal delete $TEST_PRINC1" msg]
    if { $ret != 0 } {
        tet_infoline Could not delete principal $TEST_PRINC1
        tet_infoline $msg
    }
    set ret [catch "principal delete $TEST_PRINC2" msg]
    if { $ret != 0 } {
        tet_infoline Could not delete principal $TEST_PRINC2
        tet_infoline $msg
    }

    set ret [catch "group delete osf" msg]
    if { $ret != 0 } {
        tet_infoline Could not delete group osf
        tet_infoline $msg
    }
    set ret [catch "organization delete osf" msg]
    if { $ret != 0 } {
        tet_infoline Could not delete organization osf
        tet_infoline $msg
    }

    set ret [catch "logout" msg]
    if { $ret != 0 } {
        tet_infoline Could not logout from $DCP_CLIENT
        tet_infoline $msg
    }

    if {[dcp_wrap {acl modify /.:/hosts/$DCP_HOSTNAME_ONE/config/keytab -io -change any_other:r}] != 0} {
        tet_infoline Keytab FVT Cleanup Problem:
        tet_infoline Could not acl modify /.:/hosts/$DCP_HOSTNAME_ONE/config/keytab -io -change any_other:
        tet_infoline If this ACL is not removed, some future dcecp KEYTAB FVT's might fail
    }
    tet_result PASS
}

set iclist "ic1 "
set ic1 "ktb_fvt_cleanup"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
