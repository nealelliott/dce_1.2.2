#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: link_modify_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:14:16  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:11  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:37  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:45  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:09  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:15  melman]
# 
# Revision 1.1.2.5  1994/09/02  18:42:12  kevins
# 	OT 10617 fixed error messages
# 	[1994/09/02  18:29:20  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:58  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:43  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:14  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:19:10  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:27  melman
# 	updating to new API path
# 	[1994/05/06  15:20:20  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:13  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:51  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc link_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc link_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc link_modify_10_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline link_modify_10_P: Check that we are able to modify a softlink in the cds namespace.

    # Setup steps
        # Must be running as cell administrator.
    if {[dcp_wrap {link create /.:/linkmd -to /.:/hosts}] != 0} {return;}

    # Run the test
    test { link modify /.:/linkmd -change {CDS_LinkTarget /.:/subsys}
           dcp_attrlist getval [link show /.:/linkmd] CDS_LinkTarget
         } compare_strings [name expand /.:/subsys]

    # Cleanup steps
    if {[dcp_wrap {link delete /.:/linkmd}] != 0} {return;}

}

proc link_modify_11_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline link_modify_11_P: Check that we are able to modify a list of attributes on a 
    tet_infoline link_modify_11_P: softlink within the cds namespace

    # Setup steps
        # Must be running as cell administrator.
    if {[dcp_wrap {link create /.:/linkmd -to /.:/hosts}] != 0} {return;}

    # Run the test
    test { 
      link modify /.:/linkmd -change {{CDS_LinkTimeout \
             {expiration 1993-11-11-11:10:30.000} {extension 10-10:10:10}}\
             {CDS_LinkTarget /.:/subsys}}
      attrlist getvalues [link show /.:/linkmd] -type CDS_LinkTimeout 
      } compare_strings "{expiration 1993-11-11-06:10:30.000}\n {extension +10-10:10:10.000I-----}"

    # Cleanup steps
    if {[dcp_wrap {link delete /.:/linkmd}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "link_modify_10_P"
set ic2 "link_modify_11_P"

set tet_startup link_startup
set tet_cleanup link_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
