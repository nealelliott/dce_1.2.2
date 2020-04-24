#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: ktb_setup.tcl,v $
# Revision 1.1.6.4  1996/03/11  02:17:11  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:18  marty]
#
# Revision 1.1.6.3  1995/12/13  15:26:37  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/08/04  15:23 UTC  jrr
# 	# Fix test to handle config/ketyab ACL correctly.
# 	# [1995/08/04  15:21 UTC  jrr  /main/HPDCE02/jrr_mothra_pure/1]
# 
# 	HP revision /main/HPDCE02/2  1995/06/08  13:42 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts33/1  1995/06/08  13:41 UTC  truitt
# 	CHFts15424: Change some tests to support new dced acl behavior.
# 
# 	HP revision /main/HPDCE02/1  1994/11/10  14:17 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/10  14:13 UTC  truitt
# 	Fix the keytab setup.
# 	[1995/12/11  15:53:40  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:39  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:54  melman]
# 
# Revision 1.1.2.5  1994/06/22  20:37:19  baum
# 	OT #11022 - Use tetexec.cfg for cell/host.  Use tet_setup
# 	[1994/06/22  20:36:13  baum]
# 
# Revision 1.1.2.4  1994/06/10  20:13:27  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:43  devsrc]
# 
# Revision 1.1.2.3  1994/05/25  22:43:05  baum
# 	Keytab tests now dce_login/out on their own.  Also, corrected some of the output errors
# 	[1994/05/25  22:41:44  baum]
# 
# Revision 1.1.2.2  1994/05/11  19:37:52  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:22:23  rousseau]
# 
# Revision 1.1.2.1  1994/05/10  22:19:59  baum
# 	INitial Checkin
# 	[1994/05/10  22:19:15  baum]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_misc.tcl

proc ktb_fvt_setup {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_ONE

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    if {[dcp_wrap {acl modify /.:/hosts/$DCP_HOSTNAME_ONE/config/keytab -io -change any_other:acdepr}] != 0} {
        tet_infoline Keytab FVT Setup Problem:
        tet_infoline Could not -change any_other on /.:/hosts/$DCP_HOSTNAME_ONE/config/keytab, trying -add.
        if {[dcp_wrap {acl modify /.:/hosts/$DCP_HOSTNAME_ONE/config/keytab -io -add any_other:acdepr}] != 0} {
            tet_infoline Keytab FVT Setup Problem:
            tet_infoline Could not acl modify /.:/hosts/$DCP_HOSTNAME_ONE/config/keytab -io -add any_other:acdepr
            tet_infoline If this ACL is not set, or equivalent priviledges are not provided through
            tet_infoline other means, some dcecp KEYTAB FVT's might fail
        }
    }

    set ret [catch "login $DCP_CLIENT -password $DCP_CLIENT_PW" msg]
    if { $ret != 0 } {
        tet_infoline Could not login as $DCP_CLIENT
        tet_infoline $msg
    }

    # Add two new accounts for the test

    set ret [catch "organization create osf" msg]
    if { $ret != 0 } {
        tet_infoline Could not add organization osf
        tet_infoline $msg
    }

    set ret [catch "group create osf" msg]
    if { $ret != 0 } {
        tet_infoline Could not add group osf
        tet_infoline $msg
    }

    set ret [catch "principal create $TEST_PRINC1" msg]
    if { $ret != 0 } {
        tet_infoline Could not create principal TEST_PRINC1
        tet_infoline $msg
    }
    set ret [catch "principal create $TEST_PRINC2" msg]
    if { $ret != 0 } {
        tet_infoline Could not create principal TEST_PRINC2
        tet_infoline $msg
    }

    set ret [catch "group add osf -member $TEST_PRINC1" msg]
    set ret [catch "group add osf -member $TEST_PRINC2" msg]
    set ret [catch "organization add osf -member $TEST_PRINC1" msg]
    set ret [catch "organization add osf -member $TEST_PRINC2" msg]

    set ret [catch "account create $TEST_PRINC1 -attr {{organization osf} {password $TEST_PRINC1} {group osf} {mypwd $DCP_CLIENT_PW}}" msg]
    if { $ret != 0 } {
        tet_infoline Could not create account $TEST_PRINC1
        tet_infoline $msg
      }

    set ret [catch "account create $TEST_PRINC2 -attr {{organization osf} {password $TEST_PRINC2} {group osf} {mypwd $DCP_CLIENT_PW}}" msg]
    if { $ret != 0 } {
        tet_infoline Could not create account $TEST_PRINC2
        tet_infoline $msg
    }

    set ret [catch "logout" msg]
    if { $ret != 0 } {
        tet_infoline Could not logout from $DCP_CLIENT
        tet_infoline $msg
    }

    tet_result PASS
}

set iclist "ic1 "
set ic1 "ktb_fvt_setup"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
