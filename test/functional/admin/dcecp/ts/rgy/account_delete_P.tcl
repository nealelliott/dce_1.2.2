#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: account_delete_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:18:39  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:58  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:12  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:11  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:27  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:41  melman]
# 
# Revision 1.1.2.5  1994/09/01  17:49:22  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:35  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:04  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:02  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:38:50  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:23:26  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:36  melman
# 	updating to new API path
# 	[1994/05/06  17:14:00  melman]
# 
# Revision 1.1.2.1  1994/04/15  19:24:22  kevins
# 	CR 9938 add account functional tests
# 	[1994/04/15  15:31:20  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

## Test purposes
proc account_delete_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_delete_4_P: Check to see that we can delete a account.

    # Setup steps
        # Must be running as cell administrator. 
        # Account needs to exist in the registry.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {set LIST [list " group $GROUP" " organization $ORG" "password $PRINCIPAL"  "mypwd $DCP_CLIENT_PW" ]}] != 0} {return;}
    if {[dcp_wrap { account create $PRINCIPAL -attribute $LIST }] != 0} {return;}

    # Run the test
     test { account delete $PRINCIPAL
	    dcp_missing [account catalog -simplename] $PRINCIPAL 
	} compare_strings "not found"

    # Cleanup steps
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc account_delete_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_delete_5_P: Check to see that we can delete a list of accounts.

    # Setup steps
        # Must be running as cell administrator. 
        # Accounts needs to exist in the registry.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set PRINCIPAL1 [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append PRINCIPAL1 "foo"}] != 0} {return;}
    if {[dcp_wrap {set GROUP [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set ORG [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create "$PRINCIPAL $PRINCIPAL1"}] != 0} {return;}
    if {[dcp_wrap {group create $GROUP}] != 0} {return;}
    if {[dcp_wrap {group add $GROUP -member "$PRINCIPAL $PRINCIPAL1"}] != 0} {return;}
    if {[dcp_wrap {organization create $ORG}] != 0} {return;}
    if {[dcp_wrap {organization add $ORG -member "$PRINCIPAL $PRINCIPAL1"}] != 0} {return;}
    if {[dcp_wrap {account create "$PRINCIPAL $PRINCIPAL1" -group $GROUP -organization $ORG  -password $PRINCIPAL  -mypwd $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { account delete "$PRINCIPAL $PRINCIPAL1" 
           dcp_missing [account catalog -simplename] "$PRINCIPAL $PRINCIPAL1"
         } compare_strings "not found"

    # Cleanup steps
    if {[dcp_wrap {group delete $GROUP}] != 0} {return;}
    if {[dcp_wrap {organization delete $ORG}] != 0} {return;}
    if {[dcp_wrap {principal delete "$PRINCIPAL $PRINCIPAL1"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "account_delete_4_P"
set ic2 "account_delete_5_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
