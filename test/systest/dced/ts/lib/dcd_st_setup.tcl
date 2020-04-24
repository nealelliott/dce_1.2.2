#!/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: dcd_st_setup.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:37:58  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:10  marty]
#
# Revision 1.1.4.1  1995/12/11  21:55:01  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:55:11  root]
# 
# Revision 1.1.2.2  1994/08/10  19:23:18  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:17:34  annie]
# 
# Revision 1.1.2.1  1994/06/27  18:26:40  baum
# 	Fixed install path and moved setup and misc.tcl files to lib directory
# 	[1994/06/27  18:26:15  baum]
# 
# $EndLog$

#
# systest_setup.tcl - Prepare the environment for the dced system tests.
#		      Must be run as 'self'.
#

source ./dcd_st_utils.tcl

set TEST_NAME "dcd_st_setup"

proc systest_cleanup { } {
}
  
proc systest_setup {} {

  ### 1. acl modify $KTB_CONTAINER -io  -change any_other:acdepr 
  ###    acl modify $HD_CONTAINER -io -change any_other:cdprw
  ###    acl modify $SVR_CONTAINER -io -change any_other:cdfrwx
  ###
  ### 2. acl modify $KTB_CONTAINER -add $WHO:cr-
  ###    acl modify $HD_CONTAINER -add $WHO:cr-
  ###    acl modify $SVR_CONTAINER -add $WHO:cr-
  ###    acl modify $KTB_CONTAINER -io -add $WHO:-c--pr
  ###    acl modify $HD_CONTAINER -io -add $WHO:c-pr-
  ###    acl modify $SVR_CONTAINER -io -add $WHO:c-fr-x
  ###
  ### 3. Create $TGT_CELL_HOST/systest for the server_conf portion of the
  ###    system test (dcdrel03_svrconf.tcl)
  ### 
  ### 4. acl modify $TGT_CELL_HOST/systest -change any_other:rwdtcia (as cell_Admin)
  ###

  global T_CELL_ADMIN T_CELL_ADMIN_PWD
  global TGT_CELL_HOST WHO

  set KTB_CONTAINER $TGT_CELL_HOST/config/keytab
  set HD_CONTAINER $TGT_CELL_HOST/config/hostdata
  set SVR_CONTAINER $TGT_CELL_HOST/config/srvrconf

  ###
  ### 1. Setup the containers for all objects to allow manipulation
  ###
  set cmd "acl modify $KTB_CONTAINER -io  -change any_other:acdepr"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "Command: $cmd"
    tet_infoline "Returns: $msg"
  }

  set cmd "acl modify $HD_CONTAINER -io -change any_other:cdprw"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "Command: $cmd"
    tet_infoline "Returns: $msg"
  }

  set cmd "acl modify $SVR_CONTAINER -io -change any_other:cdfrwx"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "Command: $cmd"
    tet_infoline "Returns: $msg"
  }

  ### 
  ### 2. Allow out test principal to manipulate the ACL's on the container
  ###

  set cmd "acl modify $KTB_CONTAINER -add $WHO:cr-"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "Command: $cmd"
    tet_infoline "Returns: $msg"
  }

  set cmd "acl modify $HD_CONTAINER -add $WHO:cr-"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "Command: $cmd"
    tet_infoline "Returns: $msg"
  }

  set cmd "acl modify $SVR_CONTAINER -add $WHO:cr-"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "Command: $cmd"
    tet_infoline "Returns: $msg"
  }

  set cmd "acl modify $KTB_CONTAINER -io -add $WHO:-c--pr"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "Command: $cmd"
    tet_infoline "Returns: $msg"
  }

  set cmd "acl modify $HD_CONTAINER -io -add $WHO:c-pr-"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "Command: $cmd"
    tet_infoline "Returns: $msg"
  }

  set cmd "acl modify $SVR_CONTAINER -io -add $WHO:c-fr-x"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "Command: $cmd"
    tet_infoline "Returns: $msg"
  }


  ###
  ### 3. Add CDS entry for srvrconf test
  ###

  set ret [catch "login $T_CELL_ADMIN -password $T_CELL_ADMIN_PWD" msg]
  if { $ret != 0 } {
    tet_infoline "Could not dce_login $TEST_PRINC $TEST_PRINC_PWD ($msg)"
  }  

  set cmd "directory create $TGT_CELL_HOST/systest"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "Command: $cmd"
    tet_infoline "Returns: $msg"
  }

  ###
  ### 4. ACL it so anyone can get to it.
  ###


  set cmd "acl modify $TGT_CELL_HOST/systest -change any_other:rwdtcia"
  set ret [catch "$cmd" msg]
  if { $ret != 0 } {
    tet_infoline "Command: $cmd"
    tet_infoline "Returns: $msg"
  }

  tet_result DONE
  return 0 
  
}

parse_args 

tet_infoline "Starting Setup"

set ret [catch systest_setup msg]
if { $ret != 0 } {
  tet_infoline "Error Occurred ($msg)"
  exit -1
}

exit 0
