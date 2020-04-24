#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: ktb_misc.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:09  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:17  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:36  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/08  13:44 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts33/1  1995/06/08  13:42 UTC  truitt
# 	CHFts15424: Change some tests to support new dced acl behavior.
# 	[1995/12/11  15:53:39  root]
# 
# Revision 1.1.2.2  1994/09/29  22:01:39  baum
# 	Fixed negative messages (OT 12370)
# 	[1994/09/29  22:01:12  baum]
# 
# Revision 1.1.2.1  1994/09/28  21:40:38  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:53  melman]
# 
# Revision 1.1.2.9  1994/06/22  20:37:18  baum
# 	OT #11022 - Use tetexec.cfg for cell/host.  Use tet_setup
# 	[1994/06/22  20:36:12  baum]
# 
# Revision 1.1.2.8  1994/06/10  20:13:26  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:42  devsrc]
# 
# Revision 1.1.2.7  1994/06/07  23:13:54  baum
# 	Mods to handle new dcecp output (CR #10761)
# 	[1994/06/07  23:13:25  baum]
# 
# Revision 1.1.2.6  1994/05/26  02:44:10  baum
# 	Removed VERBOSE
# 	[1994/05/26  02:43:47  baum]
# 
# Revision 1.1.2.5  1994/05/26  00:00:56  baum
# 	Cell Name
# 	[1994/05/25  23:56:57  baum]
# 
# Revision 1.1.2.4  1994/05/25  22:43:04  baum
# 	Keytab tests now dce_login/out on their own.  Also, corrected some of the output errors
# 	[1994/05/25  22:41:43  baum]
# 
# Revision 1.1.2.3  1994/05/19  23:06:28  baum
# 	Resync with changes made to commands and command output
# 	[1994/05/19  23:05:55  baum]
# 
# Revision 1.1.2.2  1994/05/11  19:37:51  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:22:18  rousseau]
# 
# Revision 1.1.2.1  1994/05/10  22:19:58  baum
# 	INitial Checkin
# 	[1994/05/10  22:19:14  baum]
# 
# $EndLog$

### Utility routines and general purpose global variables for KEYTAB
### FVTS

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
  set expected "<Unknown>"

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
 
proc create_ktb1 {} {

  global TET_ROOT

  source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

  # First delete old one, if its just a storage, latch onto it.
  set cmd "keytab create $KTB1_LONG_NAME -attr {{storage $KTB1_STORAGE} {uuid $KTB1_UUID} } -entry"
  set ret [catch "$cmd" msg]
  
  set cmd "keytab delete $KTB1_LONG_NAME"
  set ret [catch "$cmd" msg]

  # Create new one
  set cmd "keytab create $KTB1_LONG_NAME  -storage $KTB1_STORAGE -data {$TEST_PRINC1 plain 1 $TEST_PRINC1} -uuid $KTB1_UUID -annotation $KTB1_ANNOTATION"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline Could not create ktb1:
    tet_infoline $msg
  }

  set cmd "keytab add $KTB1_LONG_NAME -member $TEST_PRINC2 -key $TEST_PRINC2 -version 1"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline Could not add key2 to ktb1:
    tet_infoline $msg
  }
 
  return ret
}

proc create_ktb2 {} {

  global TET_ROOT
	
  source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

  # First delete old one, if its just a storage, latch onto it.
  set cmd "keytab create $KTB2_LONG_NAME -attr {{storage $KTB2_STORAGE} {uuid $KTB2_UUID} } -entry"
  set ret [catch "$cmd" msg]

  set cmd "keytab delete $KTB2_LONG_NAME"
  set ret [catch "$cmd" msg]

  # Create new one
  set cmd "keytab create $KTB2_LONG_NAME  -storage $KTB2_STORAGE -data {$TEST_PRINC1 plain 1 $TEST_PRINC1} -uuid $KTB2_UUID -annotation $KTB2_ANNOTATION"

  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline Could not create ktb2:
    tet_infoline $msg
  }

  set cmd "keytab add $KTB2_LONG_NAME -member $TEST_PRINC2 -key $TEST_PRINC2 -version 1"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline Could not add key2 to ktb2:
    tet_infoline $msg
  }
 
  return ret
}

proc ktb_std_setup {} {

  global DCP_CLIENT DCP_CLIENT_PW DO_DCE_LOGIN

  if { $DO_DCE_LOGIN == 1 } {
    dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}
  }
  set ret [create_ktb1]
  set ret [create_ktb2]
}   

proc ktb_std_cleanup {} { 

  global TET_ROOT DCP_CLIENT DCP_CLIENT_PW DO_DCE_LOGIN

  source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

  # Delete ktb1, first attempt to latch onto stranded storage file
  set cmd "keytab create $KTB1_LONG_NAME -attr {{storage $KTB1_STORAGE} {uuid $KTB1_UUID} } -entry"
  set ret [catch "$cmd" msg]
  
  set cmd "keytab delete $KTB1_LONG_NAME"
  set ret [catch "$cmd" msg]

  # Delete ktb2, first attempt to latch onto stranded storage file
  set cmd "keytab create $KTB2_LONG_NAME -attr {{storage $KTB2_STORAGE} {uuid $KTB2_UUID} } -entry"
  set ret [catch "$cmd" msg]

  set cmd "keytab delete $KTB2_LONG_NAME"
  set ret [catch "$cmd" msg]

  if { $DO_DCE_LOGIN == 1 } {
    dcp_wrap {logout}
  }
}   
