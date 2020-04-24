#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: dcdrel003_ktb.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:38:41  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:28  marty]
#
# Revision 1.1.4.1  1995/12/11  21:55:56  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:55:50  root]
# 
# Revision 1.1.2.4  1994/10/05  15:26:14  fadden
# 	Modify to printout commands as they are executed
# 	[1994/10/05  15:20:45  fadden]
# 
# Revision 1.1.2.3  1994/07/26  21:41:52  fadden
# 	Modify error reporting to use the standard system test setup
# 	Obtain the location of the utility file to be sourced from
# 	  an environment variable
# 	[1994/07/26  19:51:12  fadden]
# 
# Revision 1.1.2.2  1994/06/27  18:26:49  baum
# 	Fixed install path and moved setup and misc.tcl files to lib directory
# 	[1994/06/27  18:26:22  baum]
# 
# Revision 1.1.2.1  1994/06/27  17:12:41  baum
# 	Changed names from 03 -> 003
# 	[1994/06/27  17:09:39  baum]
# 
# $EndLog$

#
# dcdrel003_ktb.tcl - System Test Script to exercise dced keytab operations
#		     via dcecp commands
#

# 
# Define the directory where the test case routines reside
#
set TCDIR $env(TCDIR)

source $TCDIR/dcd_st_utils.tcl

set TEST_NAME "dcdrel003_ktb"

proc systest_cleanup { } {

  global KTB_NAME

  ###
  ### Cleanup: Remove ktb file
  ###
  set cmd "keytab delete $KTB_NAME"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "FATAL: cleanup: Could not remove $KTB_NAME"
  }
 
  return
}   
  
proc ktb_systest {} {

  # 1. Create keytab with PID in name for uniqueness
  # 2. Do a keytab catalog to make sure it is there
  # 3. Add 2 keys princ: cell_admin pwd: -dce- vrs: 1 & 2
  # 4. Show key tab to make sure keys are there
  # 5. Remove version 2 key
  # 6. Show keytab to make sure version 2 key is gone
  # 7. Add another key.  Princ: cell_admin pwd: -dce-, vrs: 3
  # 8. Show keytab to make sure the vrs 3 key is there.
  # 9. Remove all keys
  # 10. show keytab to make sure it is empty
  # 11. CLEANUP: Remove keytab

  global KTB_NAME T_CELL_ADMIN T_CELL_ADMIN_PWD
  global TGT_CELL_HOST

  ###
  ### 1. Create keytab with PID in name for uniqueness and version 1 key
  ###
  set VERSION1 53
  set VERSION2 54
  set VERSION3 55
  set MY_PID [pid]
  set KTB_SHORT_NAME systest_ktb.$MY_PID
  set KTB_NAME $TGT_CELL_HOST/config/keytab/$KTB_SHORT_NAME
  set STORAGE /tmp/$KTB_SHORT_NAME

  tet_infoline "Name of keytab - $KTB_NAME"

  set ret [doit_noexit "keytab create $KTB_NAME  -storage $STORAGE -data {$T_CELL_ADMIN plain $VERSION1 $T_CELL_ADMIN_PWD}"]
  if { $ret != 0 } {
    tet_infoline "FATAL: Failed to create keytab $KTB_NAME"
    return -1
  }

  ###
  ### 2. Do a keytab catalog to make sure it is there
  ###
  set msg [doit_ret_msg "keytab catalog -simple"]

  set ret [string match *$KTB_SHORT_NAME* $msg]
  if { $ret != 1 } {
    tet_infoline "FATAL: keytab create $KTB_SHORT_NAME succeeds, but $KTB_SHORT_NAME not found"
    tet_infoline "FATAL: in keytab catalog show"
    return -1
  }

  ###
  ### 3. Add 2nd key princ: cell_admin pwd: -dce- vrs: 2
  ###
  doit "keytab add $KTB_NAME -member $T_CELL_ADMIN -key $T_CELL_ADMIN_PWD -version $VERSION2"

  ###
  ### 4. Show keytab to make sure 2 keys are there
  ###
  set msg [doit_ret_msg "keytab show -members $KTB_NAME"]

  set ret [string match *$VERSION1* $msg]
  if { $ret != 1 } {
    tet_infoline "FATAL: Key with version #$VERSION1 not found in $KTB_NAME"
    return -1
  }

  set ret [string match *$VERSION2* $msg]
  if { $ret != 1 } {
    tet_infoline "FATAL: Key with version #$VERSION2 not found in $KTB_NAME"
    return -1
  }
  
  ###
  ### 5. Remove VERSION2 key
  ###
  doit "keytab remove $KTB_NAME -member $T_CELL_ADMIN -version $VERSION2"
  
  ###
  ### 6.  Show keytab to make sure VERSION2 key is gone
  ###
  set msg [doit_ret_msg "keytab show -members $KTB_NAME"]

  set ret [string match *$VERSION2* $msg]
  if { $ret != 0 } {
    tet_infoline "FATAL: Key with version #$VERSION2 is still in keytab $KTB_NAME after remove"
    return -1
  }
  
  ###
  ### 7. Add another key with VERSION3
  ###
  doit "keytab add $KTB_NAME -member $T_CELL_ADMIN -key $T_CELL_ADMIN_PWD -version $VERSION3"
  
  ###
  ### 8. Show keytab to make sure the version 3 key is there.
  ###
  set msg [doit_ret_msg "keytab show -members $KTB_NAME"]

  set ret [string match *$VERSION3* $msg]
  if { $ret != 1 } {
    tet_infoline "FATAL: Key with version #$VERSION3 not found in $KTB_NAME"
    return -1
  }

  ###
  ### 9. Remove all keys
  ###
  doit "keytab remove $KTB_NAME -member $T_CELL_ADMIN"

  ###
  ### 10. Show keytab to make sure it is empty
  ###
  set msg [doit_ret_msg "keytab show -members $KTB_NAME"]

  set ret [string match "" $msg]
  if { $ret != 1 } {
    tet_infoline "FATAL: Keytab $KTB_NAME not empty when it should be"
    return -1
  }
  
  return 0 
}

parse_args

tet_infoline "Starting Test"

set ret [catch "login $TEST_PRINC -password $TEST_PRINC_PWD" msg]
if { $ret != 0 } {
  tet_infoline "FATAL: Could not dce_login $TEST_PRINC $TEST_PRINC_PWD ($msg)"
  exit -1
}
  
set ret [catch ktb_systest msg]
if { $ret != 0 } {
  tet_infoline "FATAL: Error Occurred ($msg)"
}

systest_cleanup

if { $ret != 0 } {
  exit -1
} else {
exit 0
}
