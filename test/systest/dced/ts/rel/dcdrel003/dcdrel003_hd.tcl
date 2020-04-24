#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: dcdrel003_hd.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:38:40  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:27  marty]
#
# Revision 1.1.4.1  1995/12/11  21:55:54  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:55:49  root]
# 
# Revision 1.1.2.4  1994/10/05  15:26:13  fadden
# 	Modify command execution to display commands as they are
# 	  executed.
# 	[1994/10/05  15:20:41  fadden]
# 
# Revision 1.1.2.3  1994/07/26  21:41:51  fadden
# 	Modify error reporting to work within the standard system test
# 	  setup
# 	Obtain location of utility file to be sourced from an
# 	  environment variable
# 	[1994/07/26  19:51:09  fadden]
# 
# Revision 1.1.2.2  1994/06/27  18:26:48  baum
# 	Fixed install path and moved setup and misc.tcl files to lib directory
# 	[1994/06/27  18:26:21  baum]
# 
# Revision 1.1.2.1  1994/06/27  17:12:40  baum
# 	Changed names from 03 -> 003
# 	[1994/06/27  17:09:38  baum]
# 
# $EndLog$

#
# dcdrel003_hd.tcl - System Test Script to exercise dced hostdata operations
#		    via dcecp commands

# 
# Define the directory where the test case routines reside
#
set TCDIR $env(TCDIR)

source $TCDIR/dcd_st_utils.tcl

set TEST_NAME "dcdrel003_hd"

proc systest_cleanup { } {

  global HD_NAME

  ###
  ### Cleanup: Remove hd file
  ###
  set ret [doit_noexit_nofatal "hostdata delete $HD_NAME"]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not remove $HD_NAME"
  }
 
  return
}   
  
proc hd_systest {} {

  # 1. create hostdata with certain value "Initial value"
  # 2. catalog to make sure it is there
  # 3. show to make sure value is in there
  # 4. modify string to something else: "new value"
  # 5. show to make sure it has changed.
  # CLEANUP: delete hostdata

  global HD_NAME T_CELL_ADMIN T_CELL_ADMIN_PWD
  global TGT_CELL_HOST

  ###
  ### 1. Create hostdata with PID in name for uniqueness and "initial value"
  ###
  set MY_PID [pid]
  set HD_SHORT_NAME systest_hd.$MY_PID
  set HD_NAME $TGT_CELL_HOST/config/hostdata/$HD_SHORT_NAME
  set STORAGE /tmp/$HD_SHORT_NAME
  set INITIAL_DATA "Initial_data"
  set MODIFIED_DATA "Modified_data"

  tet_infoline "Name of hostdata - $HD_NAME"

  doit "hostdata create $HD_NAME -storage $STORAGE -data $INITIAL_DATA"

  ###
  ### 2. Do a hostdata catalog to make sure it is there
  ###
  set msg [doit_ret_msg "hostdata catalog -simple"]

  set ret [string match *$HD_SHORT_NAME* $msg]
  if { $ret != 1 } {
    tet_infoline "FATAL: hostdata create $HD_SHORT_NAME succeeds, but $HD_SHORT_NAME not found"
    tet_infoline "FATAL: in hostdata catalog show"
    return -1
  }

  ###
  ### 3. Show hostdata to make sure INITIAL_DATA is there
  ###
  set msg [doit_ret_msg "hostdata show $HD_NAME"]

  set ret [string match *$INITIAL_DATA* $msg]
  if { $ret != 1 } {
    tet_infoline "FATAL: Hostdata $HD_SHORT_NAME does not contain $INITIAL_DATA"
    return -1
  }

  ###
  ### 4. Modify string to MODIFIED_DATA
  ###
  doit "hostdata modify $HD_NAME -data $MODIFIED_DATA"
  
  ###
  ### 5. Show hostdata to make sure the MODIFIED_DATA is applied
  ###
  set msg [doit_ret_msg "hostdata show $HD_NAME"]

  set ret [string match *$MODIFIED_DATA* $msg]
  if { $ret != 1 } {
    tet_infoline "FATAL: Hostdata $HD_SHORT_NAME does not contain modified value ($MODIFIED_DATA)"
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
  
set ret [catch hd_systest msg]
if { $ret != 0 } {
  tet_infoline "FATAL: Error Occurred ($msg)"
}

systest_cleanup

if { $ret != 0 } {
  exit -1
} else {
  exit 0
}

