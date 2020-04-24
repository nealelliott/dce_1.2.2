#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: dcdrel002.tcl,v $
# Revision 1.1.5.2  1996/03/11  02:38:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:23  marty]
#
# Revision 1.1.5.1  1995/12/11  21:55:41  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/jrr_1.2_mothra/1  1995/11/30  20:21 UTC  bell_d
# 	Many changes by others
# 	[1995/12/11  20:55:39  root]
# 
# Revision 1.1.2.6  1994/10/04  19:44:37  fadden
# 	Modify method for checking state of "auto-start" servers.
# 	[1994/10/04  17:34:26  fadden]
# 
# Revision 1.1.2.5  1994/10/02  21:06:34  fadden
# 	Change references to PWD to use TCDIR instead
# 	[1994/10/02  18:58:14  fadden]
# 
# Revision 1.1.2.4  1994/09/30  21:57:10  fadden
# 	Rewrite test using current dcecp syntax and additional procedures
# 	for displaying commands as they're executed.
# 
# 	Massive rewrite to handle actual operation of dcecp.
# 	Revision 1.1.2.3  1994/07/27  19:26:29  fadden
# 	Pick up test case directory from env var
# 	Modify error reporting to work in standard DCE system test setup
# 	[1994/07/26  23:54:29  fadden]
# 
# Revision 1.1.2.2  1994/06/27  18:26:45  baum
# 	Fixed install path and moved setup and misc.tcl files to lib directory
# 	[1994/06/27  18:26:18  baum]
# 
# Revision 1.1.2.1  1994/06/27  17:15:04  baum
# 	Initial checkin
# 	[1994/06/27  17:14:29  baum]
# 
# $EndLog$

#
# dcdrel002.tcl - System Test Script to exercise dced server operations
#		 via dcecp commands
#

set TCDIR $env(TCDIR)
set EXEC_DIR $env(EXEC_DIR)

source $TCDIR/dcd_st_utils.tcl

set TEST_NAME "dcdrel002"

proc systest_cleanup { } {

  global SVR_OBJ_NAME_REQ SVR_OBJ_NAME_RPC SVR_OBJ_NAME_REQ_RESTART
  global SVR_OBJ_NAME_RPC_RESTART SVR_CDS_PATH_REQ  SVR_CDS_PATH_RPC
  global SVR_CDS_PATH_REQ_RESTART SVR_CDS_PATH_RPC_RESTART TCDIR
  global TGT_CELL_HOST EXEC_DIR

  ###
  ### Cleanup: Stop server and remove config entry
  ###

  tet_infoline "Executing systest cleanup procedure"

  set ret [doit_noexit_nofatal "server stop $SVR_OBJ_NAME_REQ -method hard"]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not stop $SVR_OBJ_NAME_REQ (may not be running)"
  }

  set ret [doit_noexit_nofatal  "server delete $SVR_OBJ_NAME_REQ"]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not remove $SVR_OBJ_NAME_REQ"
  }

  set ret [doit_noexit_nofatal  "object delete $SVR_CDS_PATH_REQ"]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not delete ns entry $SVR_CDS_PATH_REQ"
  }

  set ret [doit_noexit_nofatal "server stop $SVR_OBJ_NAME_RPC -method hard"]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not stop $SVR_OBJ_NAME_RPC (may not be running)"
  }

  set ret [doit_noexit_nofatal "server delete $SVR_OBJ_NAME_RPC"]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not remove $SVR_OBJ_NAME_RPC"
  }

  set ret [doit_noexit_nofatal  "object delete $SVR_CDS_PATH_RPC"]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not delete ns entry $SVR_CDS_PATH_RPC"
  }

  set ret [doit_noexit_nofatal "server stop $SVR_OBJ_NAME_REQ_RESTART -method hard"]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not stop $SVR_OBJ_NAME_REQ_RESTART (may not be running)"
  }

  set ret [doit_noexit_nofatal "server delete $SVR_OBJ_NAME_REQ_RESTART"]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not remove $SVR_OBJ_NAME_REQ_RESTART"
  }

  set ret [doit_noexit_nofatal  "object delete $SVR_CDS_PATH_REQ_RESTART"]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not delete ns entry $SVR_CDS_PATH_REQ_RESTART"
  }

  set ret [doit_noexit_nofatal "server stop $SVR_OBJ_NAME_RPC_RESTART -method hard"]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not stop $SVR_OBJ_NAME_RPC_RESTART (may not be running)"
  }

  set ret [doit_noexit_nofatal "server delete $SVR_OBJ_NAME_RPC_RESTART"]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not remove $SVR_OBJ_NAME_RPC_RESTART"
  }

  set ret [doit_noexit_nofatal  "object delete $SVR_CDS_PATH_RPC_RESTART"]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not delete ns entry $SVR_CDS_PATH_RPC_RESTART"
  }

  set ret [doit_noexit_nofatal "directory delete $TGT_CELL_HOST/systest"]
  if { $ret != 0 } {
    tet_infoline "cleanup: Could not delete $TGT_CELL_HOST/systest"
  }
 
  return
}   
  
proc svr_systest {} {

  global T_CELL_ADMIN T_CELL_ADMIN_PWD SVR_OBJ_NAME TCDIR SVR_CDS_PATH EXEC_DIR
  global TGT_CELL_HOST TEST_PRINC TEST_PRINC_PWD
  global SVR_OBJ_NAME_REQ SVR_OBJ_NAME_RPC SVR_OBJ_NAME_REQ_RESTART
  global SVR_OBJ_NAME_RPC_RESTART SVR_CDS_PATH_REQ  SVR_CDS_PATH_RPC
  global SVR_CDS_PATH_REQ_RESTART SVR_CDS_PATH_RPC_RESTART 
  global PING_SUCCESS PING_FAIL

  ###
  ### 1. Create server objects with different start attributes
  ###    request - rpc - request/failure - rpc/failure

  set MY_PID [pid]
  set SVR_EXE dcdrel002_server
  set SVR_PATH $EXEC_DIR/$SVR_EXE

  set SVR_SHORT_NAME $SVR_EXE.$MY_PID

  set SLEEP_TIME 60
  set INTERFACE_UUID 00032b04-f3f0-1e0e-9ea7-0000c0dc0d4b,1.0
  set PING_SUCCESS "1"
  set PING_FAIL "0"

  set SVR_OBJ_NAME_REQ $TGT_CELL_HOST/config/server/$SVR_SHORT_NAME-req
  set SVR_OBJ_NAME_RPC $TGT_CELL_HOST/config/server/$SVR_SHORT_NAME-rpc
  set SVR_OBJ_NAME_REQ_RESTART $TGT_CELL_HOST/config/server/$SVR_SHORT_NAME-req-restart
  set SVR_OBJ_NAME_RPC_RESTART $TGT_CELL_HOST/config/server/$SVR_SHORT_NAME-rpc-restart

  set SVR_CDS_PATH_REQ $TGT_CELL_HOST/systest/$SVR_SHORT_NAME-req
  set SVR_CDS_PATH_RPC $TGT_CELL_HOST/systest/$SVR_SHORT_NAME-rpc
  set SVR_CDS_PATH_REQ_RESTART $TGT_CELL_HOST/systest/$SVR_SHORT_NAME-req-restart
  set SVR_CDS_PATH_RPC_RESTART $TGT_CELL_HOST/systest/$SVR_SHORT_NAME-rpc-restart

  doit_noexit_nofatal "directory create $TGT_CELL_HOST/systest" 
  doit "acl modify $TGT_CELL_HOST/systest -change {{any_other rwdtcia}}"

  doit "server create $SVR_OBJ_NAME_REQ -attribute { 
	{program $SVR_PATH}  
	{arguments $SVR_CDS_PATH_REQ} 
	{starton explicit} 
	{entryname $SVR_CDS_PATH_REQ}
	{ services 
	    {interface $INTERFACE_UUID}}
	}"

  doit "server create $SVR_OBJ_NAME_RPC -attribute { 
	{program $SVR_PATH}  
	{arguments $SVR_CDS_PATH_RPC} 
	{starton auto explicit} 
	{entryname $SVR_CDS_PATH_RPC} 
	{ services 
	    {interface $INTERFACE_UUID}}
	}"

  doit "server create $SVR_OBJ_NAME_REQ_RESTART -attribute { 
	{program $SVR_PATH}  
	{arguments $SVR_CDS_PATH_REQ_RESTART} 
	{starton {explicit failure} } 
	{entryname $SVR_CDS_PATH_REQ_RESTART} 
	{ services 
	    {interface $INTERFACE_UUID}}
	}"

  doit "server create $SVR_OBJ_NAME_RPC_RESTART -attribute { 
	{program $SVR_PATH}  
	{arguments $SVR_CDS_PATH_RPC_RESTART} 
	{starton {auto failure explicit} } 
	{entryname $SVR_CDS_PATH_RPC_RESTART} 
	{ services 
	     {interface $INTERFACE_UUID}} 
	}"

  
  ###
  ### 2. Start servers created in step #1
  ###

  doit "server start $SVR_OBJ_NAME_REQ"

  doit "server start $SVR_OBJ_NAME_RPC"

  doit "server start $SVR_OBJ_NAME_REQ_RESTART"

  doit "server start $SVR_OBJ_NAME_RPC_RESTART"


  ###
  ### 3. Check that each server is running
  ###

  set ret [catch "exec sleep $SLEEP_TIME" msg]

  #
  # First use server ping.
  #
  doit_ping_success "server ping $SVR_CDS_PATH_REQ"

  doit_ping_success "server ping $SVR_CDS_PATH_RPC"

  doit_ping_success "server ping $SVR_CDS_PATH_REQ_RESTART"

  doit_ping_success "server ping $SVR_CDS_PATH_RPC_RESTART"

  ###
  ### 4. Then perform an RPC
  ###

  doit "exec $EXEC_DIR/dcdrel002_client ping_op $SVR_CDS_PATH_REQ"

  doit "exec $EXEC_DIR/dcdrel002_client ping_op $SVR_CDS_PATH_RPC"

  doit "exec $EXEC_DIR/dcdrel002_client ping_op $SVR_CDS_PATH_REQ_RESTART"

  doit "exec $EXEC_DIR/dcdrel002_client ping_op $SVR_CDS_PATH_RPC_RESTART"

  ###
  ### 5. Stop the auto-start servers and check whether they're
  ###    shown as running via "server show -exec"
  ### 

  doit_noexit "server stop $SVR_OBJ_NAME_RPC"

  doit_noexit "server stop $SVR_OBJ_NAME_RPC_RESTART"

  set ret [catch "exec sleep $SLEEP_TIME" msg]

  set cmd "server show $SVR_OBJ_NAME_RPC -exec" 
  tet_infoline "Executing $cmd"
  set ret [catch "$cmd" msg]
  if {$ret == 0} {
      tet_infoline "FATAL: Server not stopped according to \"server show\""
  }

  set cmd "server show $SVR_OBJ_NAME_RPC_RESTART -exec" 
  tet_infoline "Executing $cmd"
  set ret [catch "$cmd" msg]
  if {$ret == 0} {
      tet_infoline "FATAL: Server not stopped according to \"server show\""
  }

  ###
  ### 6. Restart via rpc call, then check state via 
  ###    "server show -exec"
  ###

  doit "exec $EXEC_DIR/dcdrel002_client ping_op $SVR_CDS_PATH_RPC"

  doit "exec $EXEC_DIR/dcdrel002_client ping_op $SVR_CDS_PATH_RPC_RESTART"

  set ret [catch "exec sleep $SLEEP_TIME" msg]

  set ret [doit_noexit "server show $SVR_OBJ_NAME_RPC -exec"]
  if {$ret != 0} {
      tet_infoline "FATAL: Server $SVR_OBJ_NAME_RPC did not auto-start"
  }

  set ret [doit_noexit "server show $SVR_OBJ_NAME_RPC_RESTART -exec"]
  if {$ret != 0} {
      tet_infoline "FATAL: Server $SVR_OBJ_NAME_RPC_RESTART did not auto-start"
  }

  ###
  ### 7. Test the 'restart' servers ability to restart after
  ###    exiting with bad exit value.
  ###

  doit "exec $EXEC_DIR/dcdrel002_client stop_bad_op $SVR_CDS_PATH_REQ_RESTART"

  doit "exec $EXEC_DIR/dcdrel002_client stop_bad_op $SVR_CDS_PATH_RPC_RESTART"

  set ret [catch "exec sleep $SLEEP_TIME" msg]

  doit_noexit "exec $EXEC_DIR/dcdrel002_client ping_op $SVR_CDS_PATH_REQ_RESTART"

  doit_noexit "exec $EXEC_DIR/dcdrel002_client ping_op $SVR_CDS_PATH_RPC_RESTART"

  doit_ping_success "server ping $SVR_CDS_PATH_REQ_RESTART"

  doit_ping_success "server ping $SVR_CDS_PATH_RPC_RESTART"

  ###
  ### 8. Shutdown all servers.
  ###

  doit_noexit "server stop $SVR_OBJ_NAME_REQ"

  doit_noexit "server stop $SVR_OBJ_NAME_RPC"

  doit_noexit "server stop $SVR_OBJ_NAME_REQ_RESTART"

  doit_noexit "server stop $SVR_OBJ_NAME_RPC_RESTART"

  ###
  ### 9. Verify that all servers have been shutdown
  ###

  set ret [catch "exec sleep $SLEEP_TIME" msg]

  doit_ping_failure "server ping $SVR_CDS_PATH_REQ"

  doit_ping_failure "server ping $SVR_CDS_PATH_RPC"

  doit_ping_failure "server ping $SVR_CDS_PATH_REQ_RESTART"

  doit_ping_failure "server ping $SVR_CDS_PATH_RPC_RESTART"

  ###
  ### 10. Delete all server objects
  ###

  doit_noexit "server delete $SVR_OBJ_NAME_REQ"

  doit_noexit "server delete $SVR_OBJ_NAME_RPC"

  doit_noexit "server delete $SVR_OBJ_NAME_REQ_RESTART"

  doit_noexit "server delete $SVR_OBJ_NAME_RPC_RESTART"
  
  doit "directory delete $TGT_CELL_HOST/systest"

  return 0 
  
}

parse_args

tet_infoline "Starting Test"

set ret [catch "login $TEST_PRINC -password $TEST_PRINC_PWD" msg]
if { $ret != 0 } {
  tet_infoline "FATAL: Could not dce_login $TEST_PRINC $TEST_PRINC_PWD ($msg)"
  exit -1
}
  
set ret [catch svr_systest msg]
if { $ret != 0 } {
  systest_cleanup 
  exit -1
} else {
  tet_infoline "Successful exit"
  exit 0
}
