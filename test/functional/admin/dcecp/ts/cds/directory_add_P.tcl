#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_add_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:17  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:17  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:16  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:28  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:42  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:34  melman]
# 
# Revision 1.1.2.5  1994/09/02  13:56:40  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:26  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:03:35  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:18  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:49  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:17:58  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:10  melman
# 	updating to new API path
# 	[1994/05/06  15:19:54  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:20:36  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:32  kevins]
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
proc directory_add_6_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_add_6_P: Check to see that we can create the child pointer.

    # Setup steps
        # Must be running as cell administrator
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set clr_name [find_clearinghouse_name]}] != 0} {return;}
    if {[dcp_wrap {set dir_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/$dir_name }] != 0} {return;}
    if {[dcp_wrap {directory remove /.: -member $dir_name}] != 0} {return;}

    # Run the diradd
    test { directory add /.: -member $dir_name -clearinghouse $clr_name
       dcp_found [directory list /.: -directories -simplename] $dir_name
         } compare_strings found

    # Cleanup steps
        # The child pointer /.:/$dir_name must be deleted from the cds 
        # namespace
    if {[dcp_wrap {directory delete /.:/$dir_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc find_clearinghouse_name {} {

  set cell_home_contents [directory show /.:]
  set replica_list [attrlist getvalues $cell_home_contents -type CDS_Replicas]

  foreach replica $replica_list {
     set replica_type [attrlist getvalues $replica -type Replica_Type]
     if {[string compare $replica_type Master] == 0 } {
         return  [attrlist getvalues $replica  -type CH_Name]
     }
  }
}      
  



set iclist "ic1 "
set ic1 "directory_add_6_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
