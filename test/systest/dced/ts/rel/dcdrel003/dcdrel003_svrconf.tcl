#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: dcdrel003_svrconf.tcl,v $
# Revision 1.1.5.2  1996/03/11  02:38:45  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:31  marty]
#
# Revision 1.1.5.1  1995/12/11  21:56:04  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:55:55  root]
# 
# Revision 1.1.2.3  1994/07/26  21:41:53  fadden
# 	Obtain the location of the utility file to be sourced from an
# 	  environment variable
# 	[1994/07/26  19:51:15  fadden]
# 
# Revision 1.1.2.2  1994/06/27  18:26:51  baum
# 	Fixed install path and moved setup and misc.tcl files to lib directory
# 	[1994/06/27  18:26:23  baum]
# 
# Revision 1.1.2.1  1994/06/27  17:12:48  baum
# 	Changed names from 03 -> 003
# 	[1994/06/27  17:09:45  baum]
# 
# $EndLog$


#
# dcdrel003_svrconf.tcl - System Test Script to exercise dced server operations
#		         via dcecp commands
#

# 
# Define the directory where the test case routines reside
#
set TCDIR $env(TCDIR)

source $TCDIR/dcd_st_utils.tcl

set TEST_NAME "dcdrel003_svrconf"
set PWD [pwd]
set SVR_CDS_PATH ""
set SVR_OBJ_NAME ""

proc systest_cleanup { } {

  global SVR_OBJ_NAME PWD SVR_CDS_PATH

  ###
  ### Cleanup: Stop server and remove hd file
  ###
  set cmd "exec $PWD/dcdrel003_client $SVR_CDS_PATH 0"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not stop test server (may not be running)"
  }

  set cmd "server delete $SVR_OBJ_NAME"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not remove $SVR_OBJ_NAME"
  }
 
  return
}   
  
proc svr_systest {} {

  # 1. Create a server conf object 
  # 2. Catalog to make sure it is there
  # 3. Start the server with server start
  # 4. Wait 20 seconds
  # 5. server ping the server to see that it is running
  # 6. stop the server with server stop
  # 7. Wait 20 seconds
  # 8. server ping the server to see that it is stopped
  # 9. CLEANUP: remove the server conf object

  global T_CELL_ADMIN T_CELL_ADMIN_PWD SVR_OBJ_NAME PWD SVR_CDS_PATH
  global TGT_CELL_HOST TEST_PRINC TEST_PRINC_PWD

  ###
  ### 1. Create a server conf object 
  ###
  set MY_PID [pid]
  set SVR_EXE dcdrel003_server
  set SVR_PATH $PWD/$SVR_EXE

  set SVR_SHORT_NAME $SVR_EXE.$MY_PID
  set SVR_OBJ_NAME $TGT_CELL_HOST/config/srvrconf/$SVR_SHORT_NAME
  set SVR_CDS_PATH $TGT_CELL_HOST/systest/$SVR_SHORT_NAME

  set SLEEP_TIME 20
  set INTERFACE_UUID 008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0
  set PING_SUCCESS "1"
  set PING_FAIL "0"

  tet_infoline "Name of Server Object - $SVR_OBJ_NAME"

  set cmd "server create $SVR_OBJ_NAME -attribute { {program $SVR_PATH}  {arguments $SVR_CDS_PATH} { services { {entryname $SVR_CDS_PATH} {interface $INTERFACE_UUID}} } }"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "Failed to create server $SVR_OBJ_NAME ($msg)"
    tet_result FAIL
    return -1
  }

  ###
  ### 2. Do a server catalog to make sure it is there
  ###
  set cmd "server catalog"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } { unwanted_fail_proc; return -1 }

  set ret [string match *$SVR_SHORT_NAME* $msg]
  if { $ret != 1 } {
    tet_infoline "server create $SVR_SHORT_NAME succeeds, but $SVR_SHORT_NAME not found"
    tet_infoline "in server catalog show"
    tet_result FAIL
    systest_cleanup
    return -1
  }

  ###
  ### 3. Start server with server start
  ###
  set cmd "server start $SVR_OBJ_NAME"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } { unwanted_fail_proc; return -1 }

  ###
  ### 4. Sleep for 20 seconds to wait for server to come up.
  ###
  set ret [catch "exec sleep $SLEEP_TIME" msg]
  
  ###
  ### 5. Ping server to see if it is running
  ###
  set cmd "server ping $SVR_CDS_PATH"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } { unwanted_fail_proc; return -1 }

  set ret [string match *$PING_SUCCESS* $msg]
  if { $ret != 1 } {
    tet_infoline "server ping $SVR_CDS_PATH returns $msg when $PING_SUCCESS was expected""
    tet_result FAIL
    systest_cleanup
    return -1
  }
  
  ###
  ### 6. Stop the server
  ###
#
#  UNTIL THIS IS IMPLEMENTED, USE TCLIENT TO STOP SERVER....
#  set cmd "server stop $SVR_OBJ_NAME"
  set cmd "exec $PWD/dcdrel003_client $SVR_CDS_PATH 0"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } { unwanted_fail_proc; return -1 }

  ###
  ### 7. Sleep for 20 seconds to wait for server to stop.
  ###
  set ret [catch "exec sleep $SLEEP_TIME" msg]

  ###
  ### 8. Ping server to see that it is stopped.
  ###
  set cmd "server ping $SVR_CDS_PATH"
  set ret [catch "$cmd" msg]
  if { $ret == 0 } { 
    tet_infoline "Command: $cmd"
    tet_infoline "Succeeds when  server should have been stopped"
    tet_result FAIL
    systest_cleanup
    return -1
  }
    
  ###
  ### 9. Remove server objct
  ###
  set cmd "server delete $SVR_OBJ_NAME"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } { unwanted_fail_proc; return -1 }
  
  tet_result PASS
  return 0 
  
}

parse_args

tet_infoline "Starting Test"

set ret [catch "login $TEST_PRINC -password $TEST_PRINC_PWD" msg]
if { $ret != 0 } {
  tet_infoline "Could not dce_login $TEST_PRINC $TEST_PRINC_PWD ($msg)"
  exit -1
}
  
set ret [catch svr_systest msg]
if { $ret != 0 } {
  tet_infoline "Error Occurred ($msg)"
  systest_cleanup 
  exit -1
}

exit 0
