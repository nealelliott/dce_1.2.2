# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: hd_misc.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:57  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:34  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:09  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/06/27  13:26 UTC  truitt
# 	Merge cleanup fixes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts36/1  1995/06/27  13:25 UTC  truitt
# 	Fix cleanup of hostdata objects.
# 
# 	HP revision /main/HPDCE02/1  1995/06/06  18:23 UTC  truitt
# 	Initial HPDCD02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts33/1  1995/06/06  18:21 UTC  truitt
# 	CHFts15424: Change some tests to support new dced acl behavior.
# 	[1995/12/11  15:53:14  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:50  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:06  melman]
# 
# Revision 1.1.2.6  1994/06/22  20:42:39  baum
# 	Merged with changes from 1.1.2.5
# 	[1994/06/22  20:42:14  baum]
# 
# 	OT #11022 - Use tet_setup, use tetexec.cfg for cell/host
# 	[1994/06/22  20:40:11  baum]
# 
# Revision 1.1.2.5  1994/06/17  18:43:10  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:25:08  devsrc]
# 
# Revision 1.1.2.4  1994/06/07  23:13:42  baum
# 	Mods to handle new dcecp output (CR #10761)
# 	[1994/06/07  23:13:13  baum]
# 
# Revision 1.1.2.3  1994/05/26  00:01:25  baum
# 	Cell Name
# 	[1994/05/25  23:56:31  baum]
# 
# Revision 1.1.2.2  1994/05/25  16:51:05  baum
# 	Hostdata tests now dce_login on their own
# 	[1994/05/25  16:49:25  baum]
# 
# Revision 1.1.2.1  1994/05/19  22:58:15  baum
# 	INitial checkin
# 	[1994/05/19  22:56:29  baum]
# 
# $EndLog$

### Utility routines and general purpose global variables for HOSTDATA FVTS

set DO_DCE_LOGIN 0

proc compare_and_report { command ret string1 string2 } {
  global VERBOSE

  if { $ret == 0 } {
    set ret [string compare $string1 $string2]
  }

  tet_infoline Test Command is:
  tet_infoline $command

  if { $ret == 0 } {
    if {$VERBOSE} {
      tet_infoline $string1
      tet_infoline $string2       
    }

    tet_infoline Test Passed.
    tet_result PASS

    } else {
      tet_infoline "Actual Result:"
      tet_infoline $string1
      tet_infoline "Expected Result:"
      tet_infoline $string2       

      tet_infoline Test Failed.
      tet_result FAIL
  }
}

proc compare_list_and_report { command ret actual_output expected_items } {
  global VERBOSE

  set mismatch 0

  if { $ret == 0 } {
    foreach expected $expected_items {
      set ret [string match *$expected* $actual_output]
      if { $ret == 0 } {
        set mismatch 1
      }
    }
  } else {
    set mismatch 1
  }

  tet_infoline Test Command is:
  tet_infoline $command

  if { $mismatch == 0 } {
    if {$VERBOSE} {
      tet_infoline $actual_output
      tet_infoline $expected_items
    }

    tet_infoline Test Passed.
    tet_result PASS

    } else {
      tet_infoline "Actual Result:"
      tet_infoline $actual_output
      tet_infoline "Expected Token Not Found:"
      tet_infoline $expected

      tet_infoline Test Failed.
      tet_result FAIL
  }
}


proc create_hd1 {} {
  global TET_ROOT

  source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

  # First delete old one, if its just a storage, latch onto it.
  set cmd "hostdata create $HD1_LONG_NAME -storage $HD1_STORAGE -uuid $HD1_UUID -annotation $HD1_ANNOTATION -entry"
  set ret [catch "$cmd" msg]

  set cmd "hostdata delete $HD1_SHORT_NAME"
  set ret [catch "$cmd" msg]

  # Create new one
  set cmd "hostdata create $HD1_LONG_NAME -storage $HD1_STORAGE -uuid $HD1_UUID -annotation $HD1_ANNOTATION -data {$LINE1 $LINE2}"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline Could not create hd1:
    tet_infoline $msg
  }

  return ret
}

proc create_hd2 {} {
  global TET_ROOT

  source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

  # First delete old one, if its just a storage, latch onto it.
  set cmd "hostdata create $HD2_LONG_NAME -storage $HD2_STORAGE -uuid $HD2_UUID -annotation $HD2_ANNOTATION -entry"
  set ret [catch "$cmd" msg]

  set cmd "hostdata delete $HD2_SHORT_NAME"
  set ret [catch "$cmd" msg]

  # Create new one
  set cmd "hostdata create $HD2_LONG_NAME -storage $HD2_STORAGE -uuid $HD2_UUID -annotation $HD2_ANNOTATION -data {$LINE1 $LINE2}"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline Could not create hd2:
    tet_infoline $msg
  }

  return ret
}

proc hd_std_setup {} {
  global DCP_CLIENT DCP_CLIENT_PW DO_DCE_LOGIN

  if {$DO_DCE_LOGIN == 1} {
    dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}
  }
  set ret [create_hd1]
  set ret [create_hd2]
}

proc hd_std_cleanup {} { 
  global DCP_CLIENT DCP_CLIENT_PW DO_DCE_LOGIN TET_ROOT

  source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

  # Delete HD1, if its just a storage, latch onto it.
  set cmd "hostdata create $HD1_LONG_NAME -storage $HD1_STORAGE -uuid $HD1_UUID -annotation $HD1_ANNOTATION -entry"
  set ret [catch "$cmd" msg]

  set cmd "hostdata delete $HD1_SHORT_NAME"
  set ret [catch "$cmd" msg]

  # Delete HD2, if its just storagem latch onto it.
  set cmd "hostdata create $HD2_LONG_NAME -storage $HD2_STORAGE -uuid $HD2_UUID -annotation $HD2_ANNOTATION -entry"
  set ret [catch "$cmd" msg]

  set cmd "hostdata delete $HD2_SHORT_NAME"
  set ret [catch "$cmd" msg]

  if {$DO_DCE_LOGIN == 1} {
    dcp_wrap {logout}
  }
}
